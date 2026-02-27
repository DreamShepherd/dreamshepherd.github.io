#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "1Dpoi_type.h"
#include "1Dpoi_def.h"
#include <ctype.h>

#define MAXLOOP 100

double MatEval(struct mtrl_line_type * pCurrentInterval, double xval, double yval, double zval, 
				double wval, int *loc_badversion, struct global_type *pGlobals, struct flag_type *pFlags);

struct layer_stack_type {
	struct layer_type *pStartLayer;
	int loopcount;
};
struct layer_stack_type layerStack[MAXLOOP];
void pushLayer(struct layer_stack_type layerStack);
struct layer_stack_type popLayer();
int is2DMaterial(char seminame[], double * thickness, struct global_type *pGlobals, struct flag_type *pFlags);
int layerStackPointer=0;
struct layer_type * layerloop(struct layer_stack_type layerStack_loc, struct layer_type *pLayer);


/*************************************************/

int readlayers(char **info, int *filepointer, struct global_type *pGlobals, struct flag_type *pFlags) {
 
/*************************************************/

	
	int nlayers;		// layer number
	int layertdefined;			// set if the layer thickness has been defined
    int mtrlType;  // used to check for 2D Materials
	int sheetfound,loopfound,type;
	int startsearch;
	char keyword[MAX_EXPRESS],seminame[MAX_SEMINAME],message[MAX_MESSAGE],expression[MAX_EXPRESS];
	int infoLength,fieldlength;
    double dummyDouble;
	struct layer_type *pLayer;
	struct layer_stack_type layerStack_loc;
    struct contact_type *pCurrentContact;

  
//  Layers start at the surface, and the layer info is stored into the layer list  


//  the first thing to do is to get surface boundary condition   
//  information, which was passed in the info string.  
//  There are three possible boundary conditions, Schottky, ohmic, and slope=0  
//  the type of the  boundary condition is stored for later use.  

    pCurrentContact = pGlobals->pFirstContact;
    if (pCurrentContact == NULL) { //check to see if any other contact have already been defined.
        pGlobals->pFirstContact = (struct contact_type *) malloc(sizeof(struct contact_type));
        pCurrentContact = pGlobals->pFirstContact;
    }
    else {
        while (pCurrentContact->pNextContact != NULL) {
            pCurrentContact = pCurrentContact->pNextContact;
        }
        pCurrentContact->pNextContact = (struct contact_type *) malloc(sizeof(struct contact_type));
        pCurrentContact = pCurrentContact->pNextContact;
    }
    pCurrentContact->pNextContact = NULL;
    pCurrentContact->pVoltageSource = NULL;
    pCurrentContact->voltageSourceNumber = NoVoltageSource;
    pCurrentContact->contactType = SLOPE; //Default surface BC is slope=0
    pCurrentContact->contactParameter = 0.0; //Set slope
    pCurrentContact->contactNumber = SurfContact; //The surface contact number is -1.
    
	startsearch = 0;
	fieldlength = fieldtrim(*info,&startsearch,keyword,expression,caseInSens);
	if (!strcmp(keyword,"surface")) {
		while(fieldlength != 0) {
			fieldlength = fieldtrim(*info,&startsearch,keyword,expression,caseInSens); // seperate out the BC keyword
			if (!strcmp(keyword,"schottky")) {
				if (fieldlength == -1) {
					strcpy(message,"You must declare a Schottky barrier height.");
					if (!pFlags->silent) alertbox(message);
					pFlags->stopthemusic = true;
					return 1;
				}
				pCurrentContact->contactType = SCHOTTKY;
				if (strlen(expression) != 0) {
					pCurrentContact->contactParameter = numconv(expression);	// define a barrier height
				}
				fieldlength = fieldtrim(*info,&startsearch,keyword,expression,caseInSens);
				if (keyword[0] == 'v') {	  
					// surface voltage
					keyword[0] = ' ';
	        		pCurrentContact->voltageSourceNumber = numconv(keyword);
	        	}
				else {
					strcpy(message,"You must declare a voltage source number for a Schottky.");
					if (!pFlags->silent) alertbox(message);
					pFlags->stopthemusic = true;
					return 1;
				}
			}
			
			else if (!strcmp(keyword,"ohmic")) { 
				pCurrentContact->contactType = OHMIC;
				fieldlength = fieldtrim(*info,&startsearch,keyword,expression,caseInSens);
				if (keyword[0] == 'v') {	  
					// surface voltage
					keyword[0] = ' ';
	        		pCurrentContact->voltageSourceNumber = numconv(keyword);
	        	}
				else {
					strcpy(message,"You must declare a voltage source number for n Ohmic contact");
					if (!pFlags->silent) alertbox(message);
					pFlags->stopthemusic = true;
					return 1;
				}
			}
			else if (!strcmp(keyword,"slope") || !strcmp(keyword,"field")){  // Electric field (slope) boundary condition
				pCurrentContact->contactType = SLOPE;
                if (strlen(expression) != 0) {
                    if (expression[0] == '-' || expression[0] == '+' || expression[0] == '.' || isdigit(expression[0])) {
                        pCurrentContact->contactParameter = numconv(expression);	// define a surface slope boundary condition
                    }
                }
				else {
					strcpy(message,"You must declare an electric field for a slope boundary condition.");
					alertbox(message);
					pFlags->stopthemusic = true;
                    return 1;
				}
			}
			else if (!strcmp(keyword,"charge")) {  // charge boundary condition (equiv to field)
				pCurrentContact->contactType = CHARGE;
                if (strlen(expression) != 0) {
                    if (expression[0] == '-' || expression[0] == '+' || expression[0] == '.' || isdigit(expression[0])) {
                        pCurrentContact->contactParameter = numconv(expression);	// define a surface slope boundary condition
                    }
                }
				else {
					strcpy(message,"You must give a sheet charge concentration for a charge boundary condition.");
					alertbox(message);
					pFlags->stopthemusic = true;
				}
			}
		}
	}
	else {
		strcpy(message,"The layers must be ordered from surface to substrate in the input file.");
		if (!pFlags->silent) alertbox(message);
		pFlags->stopthemusic = true;
		return 1;
	}
	

//  Now we are ready to begin to process the layers

	nlayers = 0;		// initialize layer counter
	pGlobals->pTopLayer =(struct layer_type *) malloc(sizeof(struct layer_type)); //allocate memory for first layer 
	pGlobals->pTopLayer->pDownLayer = NULL;
	pGlobals->pTopLayer->pUpLayer = NULL;
	pLayer = pGlobals->pTopLayer;	// initialize current layer pointer 
	loopfound = false;
	
	while (true) {
		layertdefined = false;		// reset flag for layer thickness
		sheetfound = false;
		
		// Get next line from the input file 
		infoLength=getInputNew(pGlobals->InputFile, filepointer, info);  //Get new line from input file
		//infoLength=getInput(4,info,pGlobals);
		if (infoLength == EOF) {
			strcpy(message,"Error in input file.  Expecting substrate definition.");
			if (!pFlags->silent) alertbox(message);
			pFlags->stopthemusic = true;
			return 1;
		}
		
		startsearch = 0;
		fieldlength = fieldtrim(*info,&startsearch,keyword,expression,caseInSens);
		if ((!strncmp(keyword,"#",1)) || fieldlength == 0) continue;	// look for comment or blank line, skip around if it is
		
        if (!strncmp(keyword,"/",1) && *(*info+startsearch) == '/')
            continue; // look for comment as "//", skip around if it is
        
		if (keyword[0] == '}') {
			layerStack_loc = popLayer();
			pLayer = layerloop(layerStack_loc, pLayer);
			continue; // loop to get next line
		}

		if (!strncmp(keyword,"sheet",5)){ 
			sheetfound = true;
			layertdefined = true; // fool loop into ignoring error for this line 
			pLayer->sheet = numconv(expression);
		}
		else {
			// if a superlattice loop is requested the line must start with a number followed by a {
			startsearch = 0;
			if ((type=getSingField(keyword,*info,&startsearch)) == NUMBER) {
				if (keyword[strlen(keyword)-1] == '{') {
                    keyword[strlen(keyword)-1] = '\0';
                    layerStack_loc.loopcount = atoi(keyword);
					loopfound = true;
					continue; // loop to get next line
                }
                layerStack_loc.loopcount = atoi(keyword);
				type=getSingField(keyword,*info,&startsearch);
				if (keyword[0] == '{') {
					loopfound = true;
					continue; // loop to get next line
                }
			}
		}
		
		if (!sheetfound) {
			
// Allocate memory for the new layer.  If requesting a sheet charge or superlattice loop, no memory is allocated 		
			if (!pFlags->firstlayer) {
				pLayer->pDownLayer = (struct layer_type *) malloc(sizeof(struct layer_type));
				if (pLayer->pDownLayer == NULL) {
					strcpy(message,"Can't allocate memory for layer definitions.  Increase Memory allocation.");
					if (!pFlags->silent) alertbox(message);
					pFlags->stopthemusic = true;
					return 1;
				}
				pLayer->pDownLayer->pDownLayer = NULL;
				pLayer->pDownLayer->pUpLayer = pLayer;
				pLayer = pLayer->pDownLayer;
			}
			else 
				pFlags->firstlayer = false;
			
			if (loopfound) {
				loopfound = false;
				layerStack_loc.pStartLayer = pLayer;
				pushLayer(layerStack_loc);
			}
			
			pLayer->dyLayer = pGlobals->defaultdy;
			pLayer->noholesLayer = pFlags->defaultNoHoles;
			pLayer->noelecLayer = pFlags->defaultNoElec;
			pLayer->fullyionLayer = pFlags->defFullyIonized;
			strcpy(pLayer->NasLayer,"0.0");
			strcpy(pLayer->NdsLayer,"0.0");
			pLayer->sheet = 0.0;
			pLayer->EfLayer[0] = '\0';
			pLayer->NaLayer[0] = '\0';
			pLayer->EaLayer[0] = '\0';
			pLayer->NdLayer[0] = '\0';
			pLayer->EdLayer[0] = '\0';
			pLayer->NdaLayer[0] = '\0';
			pLayer->EdaLayer[0] = '\0';
			pLayer->NddLayer[0] = '\0';
			pLayer->EddLayer[0] = '\0';
			pLayer->EdsLayer[0] = '\0';
			pLayer->EasLayer[0] = '\0';
			pLayer->xvalLayer[0] = '\0';
			pLayer->yvalLayer[0] = '\0';
			pLayer->zvalLayer[0] = '\0';
			pLayer->wvalLayer[0] = '\0';
			pLayer->EgLayer[0] = '\0';
			pLayer->offsetLayer[0] = '\0';
			pLayer->erLayer[0] = '\0';
			pLayer->eemLayer[0] = '\0';
			pLayer->mhhLayer[0] = '\0';
			pLayer->mlhLayer[0] = '\0';
			pLayer->msohLayer[0] = '\0';
			pLayer->EsoLayer[0] = '\0';
			pLayer->emobLayer[0] = '\0';
			pLayer->hmobLayer[0] = '\0';
			pLayer->taunLayer[0] = '\0';
			pLayer->taupLayer[0] = '\0';
			pLayer->polarizLayer[0] = '\0';
			pLayer->alphaLayer[0] = '\0';
			pLayer->valleyLayer[0] = '\0';
		
//  Look for the information for this layer by searching the line from the input file   

				
		    strcpy(pLayer->EfLayer,"0.0");	// default is for the Fermi level to be 0 
  
//  extract first field which is required to be the semiconductor name for the layer,  
//  or the substrate delimiter to signal the end of the layers  

			startsearch = 0;
			fieldlength = fieldtrim(*info,&startsearch,seminame,expression,caseInSens);


			if (!strcmp(seminame,"substrate")) {
                pCurrentContact->pNextContact = (struct contact_type *) malloc(sizeof(struct contact_type));
                pCurrentContact = pCurrentContact->pNextContact;
                pCurrentContact->pNextContact = NULL;
                pCurrentContact->pVoltageSource = NULL;
                pCurrentContact->voltageSourceNumber = NoVoltageSource;
                pCurrentContact->contactType = SLOPE; //Default substrate BC is slope=0
                pCurrentContact->contactParameter = 0.0; //Set slope
                pCurrentContact->contactNumber = SubContact; //The surface contact number is -2.
                
				
                while(fieldlength != 0) {
                    fieldlength = fieldtrim(*info,&startsearch,keyword,expression,caseInSens); // seperate out the BC keyword
                    if (!strcmp(keyword,"schottky")) {
                        if (fieldlength == -1) {
                            strcpy(message,"You must declare a Schottky barrier height.");
                            if (!pFlags->silent) alertbox(message);
                            pFlags->stopthemusic = true;
                            return 1;
                        }
                        pCurrentContact->contactType = SCHOTTKY;
                        if (strlen(expression) != 0) {
                            pCurrentContact->contactParameter = numconv(expression);	// define a barrier height
                        }
                        fieldlength = fieldtrim(*info,&startsearch,keyword,expression,caseInSens);
                        if (keyword[0] == 'v') {
                            // surface voltage
                            keyword[0] = ' ';
                            pCurrentContact->voltageSourceNumber = numconv(keyword);
                        }
                        else {
                            strcpy(message,"You must declare a voltage source number for a Schottky.");
                            if (!pFlags->silent) alertbox(message);
                            pFlags->stopthemusic = true;
                            return 1;
                        }
                    }
                    
                    else if (!strcmp(keyword,"ohmic")) {
                        pCurrentContact->contactType = OHMIC;
                        fieldlength = fieldtrim(*info,&startsearch,keyword,expression,caseInSens);
                        if (keyword[0] == 'v') {
                            // surface voltage
                            keyword[0] = ' ';
                            pCurrentContact->voltageSourceNumber = numconv(keyword);
                        }
                        else {
                            strcpy(message,"You must declare a voltage source number for n Ohmic contact");
                            if (!pFlags->silent) alertbox(message);
                            pFlags->stopthemusic = true;
                            return 1;
                        }
                    }
                    else if (!strcmp(keyword,"slope") || !strcmp(keyword,"field")){  // Electric field (slope) boundary condition
                        pCurrentContact->contactType = SLOPE;
                        if (strlen(expression) != 0) {
                            if (expression[0] == '-' || expression[0] == '+' || expression[0] == '.' || isdigit(expression[0])) {
                                pCurrentContact->contactParameter = numconv(expression);	// define a surface slope boundary condition
                            }
                        }
                        else {
                            strcpy(message,"You must declare an electric field for a slope boundary condition.");
                            alertbox(message);
                            pFlags->stopthemusic = true;
                            return 1;
                        }
                    }
                    else if (!strcmp(keyword,"charge")) {  // charge boundary condition (equiv to field)
                        pCurrentContact->contactType = CHARGE;
                        if (strlen(expression) != 0) {
                            if (expression[0] == '-' || expression[0] == '+' || expression[0] == '.' || isdigit(expression[0])) {
                                pCurrentContact->contactParameter = numconv(expression);	// define a surface slope boundary condition
                            }
                        }
                        else {
                            strcpy(message,"You must give a sheet charge concentration for a charge boundary condition.");
                            alertbox(message);
                            pFlags->stopthemusic = true;
                        }
                    }
                }

				pLayer = pLayer->pUpLayer;
				free(pLayer->pDownLayer);
                pLayer->pDownLayer = NULL;
                pGlobals->numLayers = nlayers; //store the final number of layers
				return 0;
			}

//  At this point the seminame must contain a semiconductor name, so:

			startsearch = 0;
			fieldlength = fieldtrim(*info,&startsearch,seminame,expression,caseSens);
			strcpy(pLayer->nameLayer,seminame);// save the semiconductor name for confirmation, case sensitive
/*			startsearch = 0;
			fieldlength = fieldtrim(info,&startsearch,seminame,caseInSens); // get case insensitive name for information retrevial 
//			ioerr_loc = semisearch(seminame,pLayer->xvalLayer,pLayer,simfamily,pGlobals,pFlags);*/	// get information from materials file*/
//		    if (pFlags->badversion) return;*/

  
//  the rest of each line is assumed to contain parameter info for each layer  //

			while (fieldlength != 0) {

				fieldlength = fieldtrim(*info,&startsearch,keyword,expression,caseInSens);	// look for addition info fields
                
                if (!strncmp(keyword,"no",2)) {  // Carrier restrictions requested
					fieldlength = fieldtrim(*info,&startsearch,keyword,expression,caseInSens);	// get next field
					if (!strncmp(keyword,"el",2)) pLayer->noelecLayer = true;		// no electrons allowed
					else if (!strncmp(keyword,"ho",2)) pLayer->noholesLayer = true;		// no holes allowed
					else {
                        strcpy(message,"Please correct input file. You must specify electrons or hole when using the 'no' flag.");
                        if (!pFlags->silent) alertbox (message);
                        pFlags->stopthemusic = true;
						return 1; // kick out on on error
					}

				}

				else if (!strncmp(keyword,"elec",4)) 	// electrons allowed 
					pLayer->noelecLayer = false;

				else if (!strncmp(keyword,"holes",5)) // holes allowed
					pLayer->noholesLayer = false;	

				else if (!strncmp(keyword,"fullyionized",12)) // Assume dopants are all ionized 
					pLayer->fullyionLayer = true;

				else if (!strncmp(keyword,"ionize",6)) // Calculate dopant ionization 
					pLayer->fullyionLayer = false;

				else if (!strncmp(keyword,"t",1)) {  // if layer thickness is defined, then a valid layer, but not 2D
					layertdefined = true;
					pLayer->numberLayer = ++nlayers;
					pLayer->tLayer = numconv(expression);
					pLayer->mtrlType = is2DMaterial(seminame, &dummyDouble, pGlobals, pFlags); // set material type
                    if (pLayer->mtrlType) { //
                        layertdefined = true;
                        strcpy(message,"Please correct input file. A 2D material cannot have a thickness defined in the input file.");
                        if (!pFlags->silent) alertbox (message);
                        pFlags->stopthemusic = true;
						return 1;
                }
				}
				
				else if (!strncmp(keyword,"x",1)) {
					strcpy(pLayer->xvalLayer,expression);
					infix_postfix(pLayer->xvalLayer);
				}

				else if (!strncmp(keyword,"y",1)){
					strcpy(pLayer->yvalLayer,expression);
					infix_postfix(pLayer->yvalLayer);
				}

				else if (!strncmp(keyword,"z",1)){
					strcpy(pLayer->zvalLayer,expression);
					infix_postfix(pLayer->zvalLayer);
				}

				else if (!strncmp(keyword,"w",1)){
					strcpy(pLayer->wvalLayer,expression);
					infix_postfix(pLayer->wvalLayer);
				}

				else if (!strncmp(keyword,"dy",2)) 
					pLayer->dyLayer = numconv(expression);

//  A 'Ef' keyword signals that the fermi level is non-zero in this layer.  
//  identifier is stored in the array voltage

				else if (!strncmp(keyword,"ef",2)) {
					strcpy(pLayer->EfLayer,expression);
					infix_postfix(pLayer->EfLayer);
				}
		
//  The rest of the possible keywords signal an override of the values taken from the semiconductors file

//		Energy gap
				else if (!strncmp(keyword,"eg",2)) {
					strcpy(pLayer->EgLayer,expression);
					infix_postfix(pLayer->EgLayer);
				}
//		Band offset
		   	 	else if (!strncmp(keyword,"deltaec",7)) {
					strcpy(pLayer->offsetLayer,expression);
					infix_postfix(pLayer->offsetLayer);
				}

//		Relative dielectric constant
		    	else if (!strncmp(keyword,"er",2)) {
					strcpy(pLayer->erLayer,expression);
					infix_postfix(pLayer->erLayer);
				}

//		Electron effective mass
				else if (!strncmp(keyword,"me",2)) {
					strcpy(pLayer->eemLayer,expression);
					infix_postfix(pLayer->eemLayer);
				}

//		Heavy Hole effective mass
				else if (!strncmp(keyword,"mhh",3)) {
					strcpy(pLayer->mhhLayer,expression);
					infix_postfix(pLayer->mhhLayer);
				}
                
//		Light Hole effective mass
				else if (!strncmp(keyword,"mlh",3)) {
					strcpy(pLayer->mlhLayer,expression);
					infix_postfix(pLayer->mlhLayer);
				}
                
//		Split-off Hole effective mass
				else if (!strncmp(keyword,"msoh",3)) {
					strcpy(pLayer->mlhLayer,expression);
					infix_postfix(pLayer->mlhLayer);
				}
                
//		Split-off band energy
				else if (!strncmp(keyword,"Eso",3)) {
					strcpy(pLayer->mlhLayer,expression);
					infix_postfix(pLayer->mlhLayer);
				}

//		Donor level
		    	else if (!strncmp(keyword,"ed",2)) {
					strcpy(pLayer->EdLayer,expression);
					infix_postfix(pLayer->EdLayer);
				}

//		Acceptor level
				else if (!strncmp(keyword,"ea",2)) {
					strcpy(pLayer->EaLayer,expression);
					infix_postfix(pLayer->EaLayer);
				}

//		Deep donor level
				else if (!strncmp(keyword,"edd",3)) {
					strcpy(pLayer->EddLayer,expression);
					infix_postfix(pLayer->EddLayer);
				}

//		Deep acceptor level
				else if (!strncmp(keyword,"eda",3)) {
					strcpy(pLayer->EdaLayer,expression);
					infix_postfix(pLayer->EdaLayer);
			}

//		Deep donor concentration
		    	else if (!strncmp(keyword,"ndd",3)) {
					strcpy(pLayer->NddLayer,expression);
					infix_postfix(pLayer->NddLayer);
				}

//		Deep acceptor concentration
		    	else if (!strncmp(keyword,"nda",3)) {
					strcpy(pLayer->NdaLayer,expression);
					infix_postfix(pLayer->NdaLayer);
				}

//		Donor concentration
				else if (!strncmp(keyword,"nd",2)) {
					strcpy(pLayer->NdLayer,expression);
					infix_postfix(pLayer->NdLayer);
				}

//		Acceptor concentration
				else if (!strncmp(keyword,"na",2)) {
					strcpy(pLayer->NaLayer,expression);
					infix_postfix(pLayer->NaLayer);
				}

//		electron mobility
		    	else if (!strncmp(keyword,"emob",4)) {
					strcpy(pLayer->emobLayer,expression);
					infix_postfix(pLayer->emobLayer);
				}

//		hole mobility
		    	else if (!strncmp(keyword,"hmob",4)) {
					strcpy(pLayer->hmobLayer,expression);
					infix_postfix(pLayer->hmobLayer);
				}

//		electron recombination time	  
				else if (!strncmp(keyword,"tn",2)) {
					strcpy(pLayer->taunLayer,expression);
					infix_postfix(pLayer->taunLayer);
				}

//		hole recombination time	  
				else if (!strncmp(keyword,"tp",2)) {
					strcpy(pLayer->taupLayer,expression);
					infix_postfix(pLayer->taupLayer);
				}

//		Semiconductor Polarization	  
		    	else if (!strncmp(keyword,"pol",3)) {
					strcpy(pLayer->polarizLayer,expression);
					infix_postfix(pLayer->polarizLayer);
				}

//		Absorption coefficient	  
				else if (!strncmp(keyword,"alpha",5)) {
					strcpy(pLayer->alphaLayer,expression);
					infix_postfix(pLayer->alphaLayer);
				}

//		Valley degeneracy	  
				else if (!strncmp(keyword,"valley",6)) {
					strcpy(pLayer->valleyLayer,expression);
					infix_postfix(pLayer->valleyLayer);
				}
  
//		Sheet donor concentration	  
		    	else if (!strncmp(keyword,"nds",3)) {
					strcpy(pLayer->NdsLayer,expression);
					infix_postfix(pLayer->NdsLayer);
				}

//		Sheet acceptor concentration	  
		    	else if (!strncmp(keyword,"nas",3)) {
					strcpy(pLayer->NasLayer,expression);
					infix_postfix(pLayer->NasLayer);
				}

				else
					if (fieldlength > 0) {
						strcpy(message,"Please correct input file.  Sorry, what does ");
						strcat(message,keyword);
						strcat(message," mean?");
						if (!pFlags->silent) alertbox (message);
						pFlags->stopthemusic = true;
						return 1;
					}
			}
		}

/*		pLayer->NdsLayer = pLayer->NdsLayer*1.0e8/pLayer->tLayer;
		pLayer->NasLayer = pLayer->NasLayer*1.0e8/pLayer->tLayer;	*/
        
        if (!layertdefined) {  // If the layer thickness has not been defined, check for 2D layer
			pLayer->mtrlType = is2DMaterial(seminame, &pLayer->dyLayer, pGlobals, pFlags);
            if (pLayer->mtrlType){
                layertdefined = true;
                pLayer->tLayer = pLayer->dyLayer;
            }
        }

		if (!layertdefined && *info[0] != '#') {
			strcpy(message,"Please correct input file. A material definition line must contain at least the material name and thickness.");
			if (!pFlags->silent) alertbox (message);
			pFlags->stopthemusic = true;
			return 1; // kick out on error
		}
	}
    
	return 0;  //Should never return here
}


//***********************************************************/
	
int semisearch(char seminame[MAX_SEMINAME], double xval, double yval, double zval, double wval, 
		struct dummy_layer_type *pLayer, char simfamily[MAX_SEMINAME], struct global_type *pGlobals, 
		struct flag_type *pFlags) {

//***********************************************************/ 
  
//  this routine searches the materials data structure in its quest to find the desired data  


	char message[MAX_MESSAGE],dummyname[MAX_SEMINAME],dummyname2[MAX_SEMINAME];
	struct mtrl_list *pMtrlList; 


// First, see if the needed material is in the materials data structure
	strcpy(dummyname,seminame);
	lowercase(dummyname);
	pMtrlList = pGlobals->pMtrl_data->pTopMtrl;
	strcpy(dummyname2,pMtrlList->name);
	lowercase(dummyname2);
	while(strcmp(dummyname2,dummyname)) {
		if (pMtrlList->pNext_mtrl == NULL) {
			pMtrlList = NULL;
			break;
		}
		else {
			pMtrlList=pMtrlList->pNext_mtrl;
			strcpy(dummyname2,pMtrlList->name);
			lowercase(dummyname2);
		}
	}

//  If the material is not in the materials list, issue an error	
	if (pMtrlList == NULL) {
		strcpy(message,"Material ");
		strcat(message,seminame);
		strcat(message," not found.  Please update materials file");
		if (!pFlags->silent) alertbox(message);
		pFlags->badversion = true;
		return 1;	// kick out if material is not in matrl file
	}

//  Check to make sure that all materials in the simulation are of the same family

	if (pFlags->firstlayer) {
		strcpy(simfamily,pMtrlList->family);
		pFlags->firstlayer = false;
	}
	if (pFlags->checkfamily && strcmp(simfamily,pMtrlList->family)) {
		strcpy(message,"Two different material families are used in your input file. All layers must be of one family.");
		if (!pFlags->silent) alertbox(message);
		pFlags->badversion = true;
		return 1;
	}


// Get the data from the materials structure 

	pLayer->mtrlType = is2DMaterial(seminame, &pLayer->dyLayer, pGlobals, pFlags);
	pLayer->NdLayer = pMtrlList->pMaterial->Nd;
	pLayer->NaLayer = pMtrlList->pMaterial->Na;
	pLayer->NddLayer = pMtrlList->pMaterial->Ndd;
	pLayer->NdaLayer = pMtrlList->pMaterial->Nda;
    
    pLayer->dyLayer = pMtrlList->pMaterial->thickness;

	pLayer->EgLayer = MatEval(pMtrlList->pMaterial->Egline,xval,yval,zval,wval,&pFlags->badversion,pGlobals, pFlags);
			if (pFlags->badversion) 
				return 1;

	pLayer->offsetLayer = MatEval(pMtrlList->pMaterial->offsetline,xval,yval,zval,wval,&pFlags->badversion,pGlobals, pFlags);
			if (pFlags->badversion) 
				return 1;

	pLayer->erLayer = MatEval(pMtrlList->pMaterial->erline,xval,yval,zval,wval,&pFlags->badversion,pGlobals, pFlags);
			if (pFlags->badversion) 
				return 1;

	pLayer->EdLayer = MatEval(pMtrlList->pMaterial->Edline,xval,yval,zval,wval,&pFlags->badversion,pGlobals, pFlags);
			if (pFlags->badversion) 
				return 1;

	pLayer->EaLayer = MatEval(pMtrlList->pMaterial->Ealine,xval,yval,zval,wval,&pFlags->badversion,pGlobals, pFlags);
			if (pFlags->badversion) 
				return 1;

	pLayer->EddLayer = MatEval(pMtrlList->pMaterial->Eddline,xval,yval,zval,wval,&pFlags->badversion,pGlobals, pFlags);
			if (pFlags->badversion) 
			return 1;

	pLayer->EdaLayer = MatEval(pMtrlList->pMaterial->Edaline,xval,yval,zval,wval,&pFlags->badversion,pGlobals, pFlags);
			if (pFlags->badversion) 
				return 1;

	pLayer->eemLayer = MatEval(pMtrlList->pMaterial->eemline,xval,yval,zval,wval,&pFlags->badversion,pGlobals, pFlags);
			if (pFlags->badversion) 
				return 1;

	pLayer->valleyLayer = MatEval(pMtrlList->pMaterial->valleyline,xval,yval,zval,wval,&pFlags->badversion,pGlobals, pFlags);
			if (pFlags->badversion) 
				return 1;

	pLayer->mhhLayer = MatEval(pMtrlList->pMaterial->mhhline,xval,yval,zval,wval,&pFlags->badversion,pGlobals, pFlags);
			if (pFlags->badversion) 
				return 1;
    
	pLayer->mlhLayer = MatEval(pMtrlList->pMaterial->mlhline,xval,yval,zval,wval,&pFlags->badversion,pGlobals, pFlags);
            if (pFlags->badversion)
                return 1;
    
	pLayer->msohLayer = MatEval(pMtrlList->pMaterial->msohline,xval,yval,zval,wval,&pFlags->badversion,pGlobals, pFlags);
            if (pFlags->badversion)
                return 1;
    
	pLayer->EsoLayer = MatEval(pMtrlList->pMaterial->Esoline,xval,yval,zval,wval,&pFlags->badversion,pGlobals, pFlags);
            if (pFlags->badversion)
                return 1;

	pLayer->emobLayer = MatEval(pMtrlList->pMaterial->emobline,xval,yval,zval,wval,&pFlags->badversion,pGlobals, pFlags);
			if (pFlags->badversion) 
				return 1;

	pLayer->hmobLayer = MatEval(pMtrlList->pMaterial->hmobline,xval,yval,zval,wval,&pFlags->badversion,pGlobals, pFlags);
			if (pFlags->badversion) 
				return 1;

	pLayer->taupLayer = MatEval(pMtrlList->pMaterial->taupline,xval,yval,zval,wval,&pFlags->badversion,pGlobals, pFlags);
			if (pFlags->badversion) 
				return 1;

	pLayer->taunLayer = MatEval(pMtrlList->pMaterial->taunline,xval,yval,zval,wval,&pFlags->badversion,pGlobals, pFlags);
			if (pFlags->badversion) 
				return 1;

	pLayer->polarizLayer = MatEval(pMtrlList->pMaterial->polarizline,xval,yval,zval,wval,&pFlags->badversion,pGlobals, pFlags);
			if (pFlags->badversion) 
				return 1;

	pLayer->alphaLayer = MatEval(pMtrlList->pMaterial->alphaline,xval,yval,zval,wval,&pFlags->badversion,pGlobals, pFlags);
			if (pFlags->badversion) 
				return 1;

	return 0;

}



//*************************************************/

struct layer_type * layerloop(struct layer_stack_type layerStack_loc, struct layer_type *pLayer) {
	
//*************************************************/
	
	int i,layernumber;
	char message[MAX_MESSAGE];
	struct layer_type *pLayer_loc,*pLayer_new;
	
	pLayer_new = pLayer;
	layernumber = pLayer->numberLayer;
	for (i=1;i<=layerStack_loc.loopcount-1;i++) {
		pLayer_loc = layerStack_loc.pStartLayer;
		while (pLayer_loc->pUpLayer != pLayer) {
			pLayer_new->pDownLayer = (struct layer_type *) malloc(sizeof(struct layer_type));
			if (pLayer_new->pDownLayer == NULL) {
				strcpy(message,"Can't allocate memory for layer definitions.  Increase Memory allocation.");
				alertbox(message);
				return NULL;
			}
			pLayer_new->pDownLayer->pDownLayer = NULL;
			pLayer_new->pDownLayer->pUpLayer = pLayer_new;
			pLayer_new = pLayer_new->pDownLayer;
			
			strcpy(pLayer_new->nameLayer,pLayer_loc->nameLayer);
			pLayer_new->dyLayer = pLayer_loc->dyLayer;
			pLayer_new->tLayer = pLayer_loc->tLayer;
			pLayer_new->noholesLayer = pLayer_loc->noholesLayer;
			pLayer_new->noelecLayer = pLayer_loc->noelecLayer;
			pLayer_new->fullyionLayer = pLayer_loc->fullyionLayer;
			strcpy(pLayer_new->NasLayer,pLayer_loc->NasLayer);
			strcpy(pLayer_new->NdsLayer,pLayer_loc->NdsLayer);
			pLayer_new->sheet = pLayer_loc->sheet;
			strcpy(pLayer_new->xvalLayer,pLayer_loc->xvalLayer);
			strcpy(pLayer_new->EfLayer,pLayer_loc->EfLayer);
			strcpy(pLayer_new->NaLayer,pLayer_loc->NaLayer);
			strcpy(pLayer_new->EaLayer,pLayer_loc->EaLayer);
			strcpy(pLayer_new->NdLayer,pLayer_loc->NdLayer);
			strcpy(pLayer_new->EdLayer,pLayer_loc->EdLayer);
			strcpy(pLayer_new->NdaLayer,pLayer_loc->NdaLayer);
			strcpy(pLayer_new->EdaLayer,pLayer_loc->EdaLayer);
			strcpy(pLayer_new->NddLayer,pLayer_loc->NddLayer);
			strcpy(pLayer_new->EddLayer,pLayer_loc->EddLayer);
			strcpy(pLayer_new->NdsLayer,pLayer_loc->NdsLayer);
			strcpy(pLayer_new->EdsLayer,pLayer_loc->EdsLayer);
			strcpy(pLayer_new->NasLayer,pLayer_loc->NasLayer);
			strcpy(pLayer_new->EasLayer,pLayer_loc->EasLayer);
			strcpy(pLayer_new->xvalLayer,pLayer_loc->xvalLayer);
			strcpy(pLayer_new->yvalLayer,pLayer_loc->yvalLayer);
			strcpy(pLayer_new->zvalLayer,pLayer_loc->zvalLayer);
			strcpy(pLayer_new->wvalLayer,pLayer_loc->wvalLayer);
			strcpy(pLayer_new->EgLayer,pLayer_loc->EgLayer);
			strcpy(pLayer_new->offsetLayer,pLayer_loc->offsetLayer);
			strcpy(pLayer_new->erLayer,pLayer_loc->erLayer);
			strcpy(pLayer_new->eemLayer,pLayer_loc->eemLayer);
			strcpy(pLayer_new->mhhLayer,pLayer_loc->mhhLayer);
			strcpy(pLayer_new->mlhLayer,pLayer_loc->mlhLayer);
			strcpy(pLayer_new->msohLayer,pLayer_loc->msohLayer);
			strcpy(pLayer_new->EsoLayer,pLayer_loc->EsoLayer);
			strcpy(pLayer_new->emobLayer,pLayer_loc->emobLayer);
			strcpy(pLayer_new->hmobLayer,pLayer_loc->hmobLayer);
			strcpy(pLayer_new->taunLayer,pLayer_loc->taunLayer);
			strcpy(pLayer_new->taupLayer,pLayer_loc->taupLayer);
			strcpy(pLayer_new->polarizLayer,pLayer_loc->polarizLayer);
			strcpy(pLayer_new->alphaLayer,pLayer_loc->alphaLayer);
			strcpy(pLayer_new->valleyLayer,pLayer_loc->valleyLayer);
		    //strcpy(pLayer_new->contactLayer,pLayer_loc->contactLayer);
		    strcpy(pLayer_new->EfLayer,pLayer_loc->EfLayer);
            
		    pLayer_new->numberLayer = ++layernumber;
		    pLayer_loc = pLayer_loc->pDownLayer;	
		} 
	}
	return pLayer_new;
}

// Push layer pointer and loop count onto the stack 
//*************************************************/
void pushLayer(struct layer_stack_type layerStack_loc)
//*************************************************/
{
	char message[MAX_MESSAGE];
	if (layerStackPointer < MAXLOOP)
		layerStack[layerStackPointer++] = layerStack_loc;
	else {
		strcpy(message,"Error: stack full, can't push new layer value\n");
		alertbox(message);
	}
}


// pop: pop and return top value from layer stack 
//*************************************************/
struct layer_stack_type popLayer()
//*************************************************/
{
	char message[MAX_MESSAGE];
	if (layerStackPointer > 0)
		return layerStack[--layerStackPointer];
	else {
		strcpy(message,"Error: Layer stack empty\n");
		alertbox(message);
		return layerStack[0];
	}
}


//*************************************************/
int is2DMaterial(char seminame[], double * thickness,struct global_type *pGlobals, struct flag_type *pFlags)
//*************************************************/
{
    
	char dummyname[MAX_SEMINAME],dummyname2[MAX_SEMINAME];
	struct mtrl_list *pMtrlList;
    
    // First, see if the needed material is in the materials data structure
	strcpy(dummyname,seminame);
	lowercase(dummyname);
	pMtrlList = pGlobals->pMtrl_data->pTopMtrl;
	strcpy(dummyname2,pMtrlList->name);
	lowercase(dummyname2);
	while(strcmp(dummyname2,dummyname)) {
		if (pMtrlList->pNext_mtrl == NULL) {
			pMtrlList = NULL;
			break;
		}
		else {
			pMtrlList=pMtrlList->pNext_mtrl;
			strcpy(dummyname2,pMtrlList->name);
			lowercase(dummyname2);
		}
	}
    
    //  If the material is not in the materials list, issue an error
	if (pMtrlList == NULL) {
		//pFlags->badversion = true;
		return 0;	// kick out if material is not in matrl file
	}
    
    if (!strcmp(pMtrlList->type, "2D-Dirac")) {
        *thickness = pMtrlList->pMaterial->thickness; //for graphene return the thickness and type2
        return graphenemtrl;
    }
    else if (!strcmp(pMtrlList->type, "2D-parabolic"))  {
        *thickness = pMtrlList->pMaterial->thickness;
        return twoDmtrl; // for a 2D semiconductor return the thickness and type 1
    }
    else return 0;
    
}

