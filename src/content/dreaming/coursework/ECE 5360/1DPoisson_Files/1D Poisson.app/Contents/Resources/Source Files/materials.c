#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "1Dpoi_type.h"
#include "1Dpoi_def.h"


struct mtrl_line_type * parameterInfo(char materialLine[]);
void val_append(char expression[]); // function to append val to x, y, z, or w
void unloadMaterials(struct mtrl_data_type *pMtrl_data);



/*************************************/
void materialparams(struct global_type *pGlobals, struct flag_type *pFlags) {
/*************************************/

/*  This subroutine shows the material parameters for a user selected material.
  To do this, the input from the user is treated as if it were a line from
  an input file, and the subroutine semisearch is called to extract the info.
  The problem is that the info extracted must be put someplace where it won't
  interfere with existing layers, so it is stored in layer maxlayers+1. */

	char info[MAX_SEMINAME],seminame[MAX_SEMINAME],keyword[MAX_KEYWORD],expression[MAX_EXPRESS],simfamily[MAX_SEMINAME],c;
	int fieldlength, ioerr_loc,i,startsearch;
	struct dummy_layer_type *pdummyLayer;

	pdummyLayer =(struct dummy_layer_type *) malloc(sizeof(struct dummy_layer_type)); //allocate memory for layer

	pGlobals->xval = 999.0e0;		// default x value
	pGlobals->yval = 999.0e0;		// default y value
	pGlobals->zval = 999.0e0;		// default z value
	pGlobals->wval = 999.0e0;		// default w value
	pGlobals->temperature = 300.0e0;		// default temperature
	pFlags->firstlayer = true;  // set first layer flag so that family is stored in simfamily
	pFlags->badversion = false;

	printf("Enter material name, mole fractions if an alloy, and temperature (Temp=): ");
	/*scanf("%s %s", info,field);*/
	c=getchar();
	for (i=0;i<MAX_SEMINAME-1 && (c=getchar()) != '\n' && c!= '\r';i++)
		info[i] = c;
		info[i] = '\0';

	startsearch = 0;
	fieldlength = fieldtrim(info,&startsearch,seminame,expression,caseInSens);
	pFlags->checkfamily = false;

//  Look for the x value of this layer by searching the rest of the line from the input file

	while (fieldlength != 0) {
		fieldlength = fieldtrim(info,&startsearch,keyword,expression,caseInSens);
		if (!strncmp(keyword,"x",1))pGlobals->xval = numconv(expression);
		else if (!strncmp(keyword,"y",1))pGlobals->yval = numconv(expression);
		else if (!strncmp(keyword,"z",1))pGlobals->zval = numconv(expression);
		else if (!strncmp(keyword,"w",1))pGlobals->wval = numconv(expression);
		else if (!strncmp(keyword,"t",1))pGlobals->temperature = numconv(expression);
	}

	ioerr_loc = semisearch(seminame,pGlobals->xval,pGlobals->yval,pGlobals->zval,pGlobals->wval,pdummyLayer,simfamily,pGlobals,pFlags);	// get information from materials file
	if (pFlags->badversion || ioerr_loc) return;

	printf("\n");
	printf("Energy Gap = %6.3f eV\n",pdummyLayer->EgLayer);
	printf("Conduction band offset relative to family basis %s = %5.3f eV\n",simfamily,pdummyLayer->offsetLayer);
	printf("Relative dielectric constant = %6.3f\n",pdummyLayer->erLayer);
	printf("Polarization concentration = %9.2e Ccm-2\n",pdummyLayer->polarizLayer);
	printf("Electron effective mass = %6.3f\n",pdummyLayer->eemLayer);
	printf("Cond. band degeneracy = %5.3f\n",pdummyLayer->valleyLayer);
	printf("Heavy hole effective mass = %5.3f\n",pdummyLayer->mhhLayer);
	printf("Light hole effective mass = %5.3f\n",pdummyLayer->mlhLayer);
	printf("Donor level = %5.3f eV\n",pdummyLayer->EdLayer);
	printf("Default donor concentration = %9.2e cm-3\n",pdummyLayer->NdLayer);
	printf("Acceptor level = %9.2e eV\n",pdummyLayer->EaLayer);
	printf("Default acceptor concentration = %9.2e cm-3\n",pdummyLayer->NaLayer);
	printf("Deep donor level = %9.2e eV\n",pdummyLayer->EddLayer);
	printf("Default deep donor concentration =  %9.2e cm-3\n",pdummyLayer->NddLayer);
	printf("Deep acceptor level = %9.2e eV\n",pdummyLayer->EdaLayer);
	printf("Default deep acceptor concentration =  %9.2e cm-3\n",pdummyLayer->NdaLayer);
	printf("Default electron mobility = %10.3e cm2/V-s\n",pdummyLayer->emobLayer);
	printf("Default hole mobility = %10.3e cm2/V-s\n",pdummyLayer->hmobLayer);
	printf("Default electron recombination time = %10.3e s\n",pdummyLayer->taunLayer);
	printf("Default hole recombination time = %10.3e s\n",pdummyLayer->taupLayer);
	printf("Default absorption coefficient = %10.3e\n",pdummyLayer->alphaLayer);

	free(pdummyLayer);

	return;
}



/*************************************/
void listmaterials(struct global_type *pGlobals, struct flag_type *pFlags) {
/*************************************/

//	this subroutine lists the materials contained in the materials file

	char message[MAX_MESSAGE];
	struct mtrl_list *pmaterial;

	pmaterial = pGlobals->pMtrl_data->pTopMtrl;

//  First, see if the materials file exists
	if (pGlobals->pMtrl_data->pTopMtrl == NULL) {
		strcpy(message,"Materials File Not Found");
		if (!pFlags->silent) alertbox (message);
		return;
	}


	printf("\n");
	printf("The materials defined in the materials file are:\n");

	do {
		printf("%s\t%s\t%s\t%s\n",pmaterial->name,pmaterial->type,pmaterial->family,pmaterial->comment);
	} while ((pmaterial = pmaterial->pNext_mtrl) != NULL);

	return;
}

/***********************************************************/

double MatEval(struct mtrl_line_type * pCurrentInterval, double xval, double yval, double zval,
			   double wval, int *loc_badversion, struct global_type *pGlobals, struct flag_type *pFlags) {

/***********************************************************/

//This routine evaluates the materials parameter for the given mole fraction and returns the value
	int rightrange;
	double functionvalue;
	char message[MAX_MESSAGE];

	rightrange = false;

	//  Get the parameter info that is stored in the materials structure

	//  If the x value is in the specification range for this parameter, store the information
	while(pCurrentInterval != NULL) {
		if (xval <= pCurrentInterval->xmax  && xval >= pCurrentInterval->xmin &&
			yval <= pCurrentInterval->ymax  && yval >= pCurrentInterval->ymin &&
			zval <= pCurrentInterval->zmax  && zval >= pCurrentInterval->zmin &&
			wval <= pCurrentInterval->wmax  && wval >= pCurrentInterval->wmin ) {

			functionvalue = evaluate(pCurrentInterval->expression, pGlobals, pFlags);	//calculate the parameter
			rightrange = true;
			return functionvalue;
		}
		else
			pCurrentInterval = pCurrentInterval->pNextInterval;
	}

	/*	if (xval == 2.0e0) {
	 strcpy(message,"A ternary must have an x value declared");
	 if (!pFlags->silent) alertbox(message);
	 *loc_badversion = true;
	 return 0.0;
	 }
	 else {
	 strcpy(message,"Improper x value in a ternary definition");
	 if (!pFlags->silent) alertbox(message);
	 *loc_badversion = true;
	 return 0.0;
	 }*/

	strcpy(message,"Improper mole-fraction definition");
	alertbox(message);
	*loc_badversion = true;
	return 0.0;
}


/***************************************************************************/

int checkMaterials(struct mtrl_data_type *pMtrl_data) {

/***************************************************************************/
//This routine checks the materials file to ensure that it is the correct version

	char field[MAX_EXPRESS],extension[MAX_EXTENSION];
	char *materialLine=NULL;
	int semistart,infoLength,fieldlength,filepointer;


	if (pMtrl_data->MatrlFile == NULL) {
		return 1; //"Materials file not found!"
	}


	// read first line, which will then be analyzed
    filepointer = 0; //start at the beginning of the file
	infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine);

	semistart = 0;
	fieldlength=fieldtrim(materialLine,&semistart,field,extension,caseInSens);
    if (materialLine != NULL) {
        free(materialLine);
        materialLine = NULL;
    }
	if (strcmp(field,MaterialsVersion)) {  // Check the version of the materials file
		return 2;
	}
    return 0;
}


/***************************************************************************/

int loadMaterials(struct mtrl_data_type *pMtrl_data) {

/***************************************************************************/
//This routine loads the materials file into a linked list, the first element of which is pointed to by pGlobals->pTopMtrl

    char field[MAX_EXPRESS];
    char *materialLine=NULL;
    int semistart,infoLength,firstMaterial,fieldlength,filepointer;
    struct mtrl_list *pMtrlList;


/*  Now that we know the material file exists, and is the correct version, load the materials
	 by loading the materials info into the materials structure.*/

	firstMaterial = true;
    if (pMtrl_data->pTopMtrl != NULL) unloadMaterials(pMtrl_data);  //empty unwanted materials data structure
    pMtrl_data->pTopMtrl = (struct mtrl_list *) malloc(sizeof(struct mtrl_list));
    pMtrl_data->pTopMtrl->pNext_mtrl = NULL;
    pMtrlList = pMtrl_data->pTopMtrl;

    filepointer = 0; //start at the beginning of the file
	infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine);  //Skip the first line, require to be version number
	infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine);  //Get new line from materials file, skip comment

	while (infoLength != EOF) {

  		if(materialLine[0] == '#'){
			infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine);  //Get new line from materials file
  			continue;
		}

		//	   first field contains the semiconductor name
		semistart = 0;
		fieldlength = getSingField(field,materialLine,&semistart);
		//lowercase(field);

		if (fieldlength != '\0') {

			if (firstMaterial) {
				firstMaterial = false;
                pMtrlList->pMaterial = (struct mtrl_type *) malloc(sizeof(struct mtrl_type)); // pointer for material layer
			}
			else {
				pMtrlList->pNext_mtrl = (struct mtrl_list *) malloc(sizeof(struct mtrl_list));
				pMtrlList->pNext_mtrl->pNext_mtrl = NULL;
				pMtrlList = pMtrlList->pNext_mtrl;
                pMtrlList->pMaterial = (struct mtrl_type *) malloc(sizeof(struct mtrl_type)); // pointer for material layer
			}

			strcpy(pMtrlList->name,field); // Save semiconductor name
			fieldlength = getSingField(field,materialLine,&semistart);
			/*lowercase(field);*/
			strcpy(pMtrlList->type,field); // save semiconductor type
			fieldlength = getSingField(field,materialLine,&semistart);
			lowercase(field);
			strcpy(pMtrlList->family,field);	// save family name
			fieldlength = getSingField(field,materialLine,&semistart);
			if (fieldlength != '\0') {
				strcpy(pMtrlList->comment,field);	// save material comment
				while (fieldlength != '\0') {
					fieldlength = getSingField(field,materialLine,&semistart);
					strcat(pMtrlList->comment," ");	// Add a space
					strcat(pMtrlList->comment,field);	// save the rest of material comment
				}
			}
			else
				strcpy(pMtrlList->comment,"");

			infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (default dopings)
			lowercase(materialLine);
			semistart = 0;
			fieldlength = getSingField(field,materialLine,&semistart);
			pMtrlList->pMaterial->Nd=numconv(field);
			fieldlength = getSingField(field,materialLine,&semistart);
			pMtrlList->pMaterial->Na=numconv(field);
			fieldlength = getSingField(field,materialLine,&semistart);
			pMtrlList->pMaterial->Ndd=numconv(field);
			fieldlength = getSingField(field,materialLine,&semistart);
			pMtrlList->pMaterial->Nda=numconv(field);

            if (strncmp(pMtrlList->type,"2D",2)) {  //If not a 2D material read in the data
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (Eg info)
                lowercase(materialLine);
                pMtrlList->pMaterial->Egline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (Delta EC info)
                lowercase(materialLine);
                pMtrlList->pMaterial->offsetline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (Rel diel const info)
                lowercase(materialLine);
                pMtrlList->pMaterial->erline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (Ed info)
                lowercase(materialLine);
                pMtrlList->pMaterial->Edline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (Ea info)
                lowercase(materialLine);
                pMtrlList->pMaterial->Ealine = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (Edd info)
                lowercase(materialLine);
                pMtrlList->pMaterial->Eddline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (Eda info)
                lowercase(materialLine);
                pMtrlList->pMaterial->Edaline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (elec eff mass info)
                lowercase(materialLine);
                pMtrlList->pMaterial->eemline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (Cond band degen info)
                lowercase(materialLine);
                pMtrlList->pMaterial->valleyline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (heavy hole eff mass info)
                lowercase(materialLine);
                pMtrlList->pMaterial->mhhline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (Light hole eff mass info)
                lowercase(materialLine);
                pMtrlList->pMaterial->mlhline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (split-off hole eff mass info)
                lowercase(materialLine);
                pMtrlList->pMaterial->msohline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (split-off energy info)
                lowercase(materialLine);
                pMtrlList->pMaterial->Esoline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (elec mobil info)
                lowercase(materialLine);
                pMtrlList->pMaterial->emobline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (hole mobil info)
                lowercase(materialLine);
                pMtrlList->pMaterial->hmobline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (elec recomb time info)
                lowercase(materialLine);
                pMtrlList->pMaterial->taunline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (hole recomb time info)
                lowercase(materialLine);
                pMtrlList->pMaterial->taupline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (polariz info)
                lowercase(materialLine);
                pMtrlList->pMaterial->polarizline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (absorption info)
                lowercase(materialLine);
                pMtrlList->pMaterial->alphaline = parameterInfo(materialLine);
            }
            else if (!strcmp(pMtrlList->type,"2D-parabolic")) {  // 2D material with parabolic bands
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (Eg info)
                lowercase(materialLine);
                pMtrlList->pMaterial->Egline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (Delta EC info)
                lowercase(materialLine);
                pMtrlList->pMaterial->offsetline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (Rel diel const info)
                lowercase(materialLine);
                pMtrlList->pMaterial->erline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (Thickness info)
                semistart = 0;
                infoLength = getSingField(field,materialLine,&semistart); // ignore keyword
                infoLength = getSingField(field,materialLine,&semistart); // get thickness
                pMtrlList->pMaterial->thickness = numconv(field);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (Ed info)
                lowercase(materialLine);
                pMtrlList->pMaterial->Edline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (Ea info)
                lowercase(materialLine);
                pMtrlList->pMaterial->Ealine = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (Edd info)
                lowercase(materialLine);
                pMtrlList->pMaterial->Eddline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (Eda info)
                lowercase(materialLine);
                pMtrlList->pMaterial->Edaline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (elec eff mass info)
                lowercase(materialLine);
                pMtrlList->pMaterial->eemline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (Cond band degen info)
                lowercase(materialLine);
                pMtrlList->pMaterial->valleyline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (heavy hole eff mass info)
                lowercase(materialLine);
                pMtrlList->pMaterial->mhhline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (Light hole eff mass info)
                lowercase(materialLine);
                pMtrlList->pMaterial->mlhline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (split-off hole eff mass info)
                lowercase(materialLine);
                pMtrlList->pMaterial->msohline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (split-off energy info)
                lowercase(materialLine);
                pMtrlList->pMaterial->Esoline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (elec mobil info)
                lowercase(materialLine);
                pMtrlList->pMaterial->emobline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (hole mobil info)
                lowercase(materialLine);
                pMtrlList->pMaterial->hmobline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (elec recomb time info)
                lowercase(materialLine);
                pMtrlList->pMaterial->taunline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (hole recomb time info)
                lowercase(materialLine);
                pMtrlList->pMaterial->taupline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (polariz info)
                lowercase(materialLine);
                pMtrlList->pMaterial->polarizline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (absorption info)
                lowercase(materialLine);
                pMtrlList->pMaterial->alphaline = parameterInfo(materialLine);
            }
            else if (!strcmp(pMtrlList->type,"2D-Dirac")) {  // 2D material with parabolic bands
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (Delta EC info)
                lowercase(materialLine);
                pMtrlList->pMaterial->offsetline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (Rel diel const info)
                lowercase(materialLine);
                pMtrlList->pMaterial->erline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (Thickness info)
                semistart = 0;
                infoLength = getSingField(field,materialLine,&semistart); // ignore keyword
                infoLength = getSingField(field,materialLine,&semistart); // get thickness
                pMtrlList->pMaterial->thickness = numconv(field);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (Ed info)
                lowercase(materialLine);
                pMtrlList->pMaterial->Edline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (Ea info)
                lowercase(materialLine);
                pMtrlList->pMaterial->Ealine = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (Edd info)
                lowercase(materialLine);
                pMtrlList->pMaterial->Eddline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (Eda info)
                lowercase(materialLine);
                pMtrlList->pMaterial->Edaline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (elec mobil info)
                lowercase(materialLine);
                pMtrlList->pMaterial->emobline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (hole mobil info)
                lowercase(materialLine);
                pMtrlList->pMaterial->hmobline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (elec recomb time info)
                lowercase(materialLine);
                pMtrlList->pMaterial->taunline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (hole recomb time info)
                lowercase(materialLine);
                pMtrlList->pMaterial->taupline = parameterInfo(materialLine);
                infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine); // get next line (absorption info)
                lowercase(materialLine);
                pMtrlList->pMaterial->alphaline = parameterInfo(materialLine);
                pMtrlList->pMaterial->Egline = parameterInfo("eg=0.0");  // Now load the unused parameters
                pMtrlList->pMaterial->eemline = parameterInfo("me=0.0");
                pMtrlList->pMaterial->valleyline = parameterInfo("val=0.0");
                pMtrlList->pMaterial->mhhline = parameterInfo("mh=0.0");
                pMtrlList->pMaterial->mlhline = parameterInfo("mlh=0.0");
                pMtrlList->pMaterial->polarizline = parameterInfo("pol=0.0");
                pMtrlList->pMaterial->msohline = parameterInfo("mhso=0.0");
                pMtrlList->pMaterial->Esoline = parameterInfo("eso=0.0");
            }



			infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine);  // skip the "end" line
			if (infoLength == EOF) {
				//pGlobals->errorCode = 2;  //"Error in Materials File!"  This is a rather crude check for errors in the materials file.  File ended too soon.
				//pFlags->badversion = true;
				return 2;  //"Error in Materials File!"  This is a rather crude check for errors in the materials file.  File ended too soon.
			}
		}
		infoLength=getInputNew(pMtrl_data->MatrlFile, &filepointer, &materialLine);  // Read in the next line from the materials file
	}
	free(materialLine);  //free line storage
    materialLine = NULL;
    //free(pMtrl_data->MatrlFile);  //release materials file contents
	return 0;
}
/**************************************************/

struct mtrl_line_type * parameterInfo(char materialLine[]) {

/**************************************************/

	int infostart,fieldlength,type,sign_possible;
	char field[MAX_EXPRESS];
	struct mtrl_line_type *pFirstInterval, *pCurrentInterval;

	pFirstInterval = (struct mtrl_line_type *)
	malloc(sizeof(struct mtrl_line_type)); // pointer for first interval of information
	pCurrentInterval = pFirstInterval;
	pCurrentInterval->xmin = 998.0;
	pCurrentInterval->xmax = 1000.0;
	pCurrentInterval->ymin = 998.0;
	pCurrentInterval->ymax = 1000.0;
	pCurrentInterval->zmin = 998.0;
	pCurrentInterval->zmax = 1000.0;
	pCurrentInterval->wmin = 998.0;
	pCurrentInterval->wmax = 1000.0;
	pCurrentInterval->pNextInterval = NULL;
	infostart = 0;
	fieldlength = getSingField(field,materialLine,&infostart); // ignore keyword
	fieldlength = getSingField(field,materialLine,&infostart); // get expression for this interval
	infix_postfix(field); // convert the expression into postfix format
	val_append(field);  // append val to x, y, z, or w
	strcpy(pCurrentInterval->expression,field);

	fieldlength = getSingField(field,materialLine,&infostart); // look for possible next field
	sign_possible = true;

	while (fieldlength != '\0') {
		switch (field[0]) {
			case 'x':
				type = getop(field,materialLine,&infostart,&sign_possible); //ignore = sign
				type = getop(field,materialLine,&infostart,&sign_possible);
				pCurrentInterval->xmin=numconv(field);
				type = getop(field,materialLine,&infostart,&sign_possible); //ignore - sign
				type = getop(field,materialLine,&infostart,&sign_possible);
				pCurrentInterval->xmax=numconv(field);
				break;
			case 'y':
				type = getop(field,materialLine,&infostart,&sign_possible); //ignore = sign
				type = getop(field,materialLine,&infostart,&sign_possible);
				pCurrentInterval->ymin=numconv(field);
				type = getop(field,materialLine,&infostart,&sign_possible); //ignore - sign
				type = getop(field,materialLine,&infostart,&sign_possible);
				pCurrentInterval->ymax=numconv(field);
				break;
			case 'z':
				type = getop(field,materialLine,&infostart,&sign_possible); //ignore = sign
				type = getop(field,materialLine,&infostart,&sign_possible);
				pCurrentInterval->zmin=numconv(field);
				type = getop(field,materialLine,&infostart,&sign_possible); //ignore - sign
				type = getop(field,materialLine,&infostart,&sign_possible);
				pCurrentInterval->zmax=numconv(field);
				break;
			case 'w':
				type = getop(field,materialLine,&infostart,&sign_possible); //ignore = sign
				type = getop(field,materialLine,&infostart,&sign_possible);
				pCurrentInterval->wmin=numconv(field);
				type = getop(field,materialLine,&infostart,&sign_possible); //ignore - sign
				type = getop(field,materialLine,&infostart,&sign_possible);
				pCurrentInterval->wmax=numconv(field);
				break;
			default:
				pCurrentInterval->pNextInterval = (struct mtrl_line_type *)
				malloc(sizeof(struct mtrl_line_type)); // pointer for next interval information
				pCurrentInterval = pCurrentInterval->pNextInterval;
				pCurrentInterval->xmin = 998.0;
				pCurrentInterval->xmax = 1000.0;
				pCurrentInterval->ymin = 998.0;
				pCurrentInterval->ymax = 1000.0;
				pCurrentInterval->zmin = 998.0;
				pCurrentInterval->zmax = 1000.0;
				pCurrentInterval->wmin = 998.0;
				pCurrentInterval->wmax = 1000.0;
				pCurrentInterval->pNextInterval = NULL;
				fieldlength = getSingField(field,materialLine,&infostart); // get expression for this interval
				infix_postfix(field); // convert the expression into postfix format
				val_append(field);
				strcpy(pCurrentInterval->expression,field);
		}
		fieldlength = getSingField(field,materialLine,&infostart); // get next field
	}
	return pFirstInterval;
}

/**************************************************/

void val_append(char expression[]) {

/**************************************************/

	/* This routine replaces x, y, z, or w with xval, yval, zval, or wval in a string expression.
	 Used to eliminate confusion between mole-fraction and real space coordinates */

	int i,fieldlength;
	char dummy[MAX_EXPRESS],var[MAX_EXPRESS];

	strcpy(dummy,"");
	i=0;
	fieldlength = getSingField(var,expression,&i); // get first operator in this expression

	while(fieldlength != '\0'){
		switch(var[0]){
			case 'x':
				strcat(dummy,"xval");
				break;
			case 'y':
				strcat(dummy,"yval");
				break;
			case 'z':
				strcat(dummy,"zval");
				break;
			case 'w':
				strcat(dummy,"wval");
				break;
			default:
				strcat(dummy,var);
				break;
		}
		strcat(dummy," ");
		fieldlength = getSingField(var,expression,&i); // get next operator in this expression
	}
	strcpy(expression,dummy);
}


/*************************************/
void unloadMaterials(struct mtrl_data_type *pMtrl_data) {
/*************************************/

    struct mtrl_list *pCurrentMtrl,*pNextMtrl;

    pCurrentMtrl = pMtrl_data->pTopMtrl;
    while (pCurrentMtrl != NULL) {
        pNextMtrl = pCurrentMtrl->pNext_mtrl;
        free(pCurrentMtrl->pMaterial);
        pCurrentMtrl->pMaterial = NULL;
        free(pCurrentMtrl);
        pCurrentMtrl = pNextMtrl;
    }
    pMtrl_data->pTopMtrl = NULL;
    return;

}
