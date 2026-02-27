#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "1Dpoi_type.h"
#include "1Dpoi_def.h" 

/*****************************************/
int input(struct global_type *pGlobals, struct flag_type *pFlags) {
/*****************************************/

/*  This routine reads the input file */

	int dosefound,Rpfound,drpfound,imptypefound,filepointer;
	int sourcenumber,fieldlength,infoLength,ioerr_loc,startsearch;
	char *info=NULL,keyword[MAX_KEYWORD],extension[MAX_EXTENSION],message[MAX_MESSAGE],expression[MAX_EXPRESS];
	char access[3],*charPt;
	double tempconv;
    struct voltageSource_type *pVoltageSource = NULL;
    struct contact_type *pCurrentContact = NULL;

	/* Set defaults */
	pGlobals->defaultdy = 10.0e0;	/* Set default dy to 10Ang */
	pGlobals->temperature = 300;	/* Set default pGlobals->temperature to 300K */
	pFlags->defFullyIonized = false;	/* Set default to calculate dopant ionization */
	pFlags->schrodinger = false;	/* Default is no pFlags->schrodinger solution */
	pFlags->schstartflag = false;	/* used to be sure that both schstart and schstop are set */
	pFlags->schstopflag = false;
	pGlobals->pSchStart = NULL;		/* To be on the safe side, set schroding start and stop pointers to Null */
	pGlobals->pSchStop = NULL;
	pFlags->badversion = false;	/* materials file version flag */
	pFlags->stopthemusic = false;	/* halt flag */
	pFlags->multiruns = false;	/* default to only one voltage run */
	//strcpy(pGlobals->surfcontact,"schottky"); /* default surface contact is a schottky barrier */
	//strcpy(pGlobals->subcontact,"slope");	/* default substrate boundary condition is slope=0 */
	//pGlobals->surfvoltage = 1;		/* surface schottky is connected to voltage source 1 */
	//pGlobals->subvoltage = 1;		/* default substrate connected to voltage source 1 */
    //pGlobals->pFirstContact = NULL; //Initialize firstContact pointer
	//pGlobals->pFirstVoltage->vbias = 0.0e0;	/* default voltage of first source is 0 */
	//pFlags->usesurfbar = false;	/* use default surface barrier unless specified */
	//pFlags->usesubbar = false;	/* use default substrate barrier unless specified */
	//pFlags->surfCharge = false;	// default is to not use a charge boundary condition at the surface
	//pFlags->subCharge = false;	// default is to not use a charge boundary condition at the substrate
	//pGlobals->surfSlope =0.0;	//default surface slope is 0 V/cm
	//pGlobals->subSlope =0.0;		//default substrate slope is 0 V/cm
	pFlags->cv = false;			/* default is no CV */
	pGlobals->cvvoltage = 1.0e-3;	/* default pFlags->cv voltage is 10mV */
	pFlags->cvonly = false;		/* default is not only pFlags->cv output */
	pFlags->implant = false;		/* default is no pFlags->implant */
	dosefound = false;	/* pGlobals->dose not found */
	Rpfound	= false;		/* range not found */
	drpfound = false;	/* pGlobals->dRp not found */
	imptypefound = false;	/* pFlags->implant type not found */
	pFlags->etched = false;		/* default is no etch simulation */
	pGlobals->etchdepth = 0.0e0;
	pGlobals->etchinc = 0.0e0;
	pFlags->etchonly = false;	/* default is output for all etch depths */
	pGlobals->impnumber = 0;		/* initialize pFlags->implant counter */
	pFlags->checkfamily = true;
	pFlags->firstlayer = true;
	pFlags->exchange = false;	/* pFlags->exchange potential flag */
	pGlobals->maxcount = 100;		/* default max interations = 100 */
	pGlobals->converg = 1.0e-5;		/* default convergence criteria */
	pGlobals->CVconverg = 1.0e-10;	/* default convergence criteria for CV sims */
	pGlobals->numeleceval = 0;		//initialize number of electron eigenvalues
	pGlobals->numhheval = 0;		//initialize number of heavy hole eigenvalues
	pGlobals->numlheval = 0;		//initialize number of light hole eigenvalues
	pGlobals->numsoheval = 0;		//initialize number of split-off hole eigenvalues
	pFlags->defaultNoHoles = false;	/* default is to calculate the holes */
	pFlags->defaultNoElec = false;	/* default is to calculate the electrons */
	pFlags->defFullyIonized = false;	/* default is calculate dopant ionization */
	pFlags->DangerWill = false;
	pFlags->ignoreDanger = false;	/* default is to act on "Danger Will Rob." error */
	pFlags->printDanger = true;	/* print the danger message on the first time */
	pFlags->stopDanger = false;	/* default is to not stop on "Danger Will Rob." error */
	pGlobals->crossover = 1.0e-3;	/* default pGlobals->crossover is 1 meV */
	pFlags->cleanFiles = true;	/* default is to delete old files */
	pFlags->noStatus = false;	// Default is to output status file
	pFlags->restart = false;		/* default is to start new simulations from scratch */
    pFlags->readParam = false;	/* default is to not read doping table file */
    pFlags->findBst = false;		/* default is to not find all bound states automatically */
    pFlags->BndStRangeSet = false;		// default is that the user has not set the limits of the bound states search
    pGlobals->BndStBounds.eBoundsSet = false;	// default is not set limits of the electron bound states search
    pGlobals->BndStBounds.hhBoundsSet = false;	// default is not set limits of the heavy hole bound states search
    pGlobals->BndStBounds.lhBoundsSet = false;	// default is not set limits of the light hole bound states search
    pGlobals->BndStBounds.sohBoundsSet = false;	// default is not set limits of the split-off hole bound states search
    pFlags->useBstDefaults = false; // default is to not use the default energy limits in finding bound states
    pFlags->outfileexists = false;  //declare that there is no output file space allocated
    pFlags->invert = false;  //declare that there is no output file space allocated
#if program_type == NORMAL
	pFlags->silent = false;		/* For normal program default is to display normal messages */
#else
	pFlags->silent = true;		/* For command line default is not to display normal messages */
#endif
    pGlobals->epsilonA = 1.64e11; //used for field dep dielectric constant
    pGlobals->epsilonB = 1.21e8; //used for field dep dielectric constant

/*	  We first need to find dy for the structure (needed for graded layers) */
	
	filepointer = 0;

	while(true) { 
		// read data line, which will then be analyzed 
		infoLength=getInputNew(pGlobals->InputFile, &filepointer, &info);  //Get new line from input file

		// Check for end of file 
		if (infoLength == EOF)
			break;
		
		// extract the first field 
		startsearch = 0;
		fieldlength=fieldtrim(info,&startsearch,keyword,expression,caseInSens);
        
        if (!strncmp(keyword,"#",1) || (!strncmp(keyword,"/",1) && info[startsearch] == '/'))
            continue; //skip comments
        
		else if (!strcmp(keyword,"no")) {  // Restrictions requested
			fieldlength=fieldtrim(info,&startsearch,keyword,expression,caseInSens);
			if (!strncmp(keyword,"el",2))
	    		pFlags->defaultNoElec = true;	// no electrons allowed
			else if (!strncmp(keyword,"ho",2)) 
	    		pFlags->defaultNoHoles = true;	// no holes allowed 
	    	else if (!strncmp(keyword,"fa",2))
	    		pFlags->checkfamily = false;	// Materials file family will not be checked
	    	else if (!strncmp(keyword,"st",2)) 
	    		pFlags->noStatus = true;		// no status file will be output
	    	else if (!strncmp(keyword,"cl",2)) 
	    		pFlags->cleanFiles = false;	// do not delete old output files
			else {
				strcpy(message,"Please correct input file. You must specify 'no' something.");
				if (!pFlags->silent) alertbox (message);
				pFlags->stopthemusic = true;
				return 1; // kick out on on error
			}
		}

		else if (!strcmp(keyword,"fullyionized"))
			pFlags->defFullyIonized = true;

		else if (!strncmp(keyword,"restart",7)) 
			pFlags->restart = true;

		else if (!strncmp(keyword,"silent",6)) 
			pFlags->silent = true;  // supress normal messages

		else if (!strncmp(keyword,"find",4)){
			fieldlength=fieldtrim(info,&startsearch,keyword,expression,caseInSens);
            if (!strncmp(keyword,"quant",5)) pFlags->findBst = true;	// find all bound states automatically
            
            while (fieldlength != 0){
                fieldlength=fieldtrim(info,&startsearch,keyword,expression,caseInSens);
                if (!strncmp(keyword,"use",3)){
                    pFlags->useBstDefaults = true;  //Use defaults in search range
                }
                else if (!strncmp(keyword,"eub",3)) {  //Set electron upper bound
                    pGlobals->BndStBounds.eUB = numconv(expression);
                    pGlobals->BndStBounds.eBoundsSet = true;
                    pFlags->BndStRangeSet = true;
                }
                else if (!strncmp(keyword,"elb",3)) {  //Set electron lower bound
                    pGlobals->BndStBounds.eLB = numconv(expression);
                    pGlobals->BndStBounds.eBoundsSet = true;
                    pFlags->BndStRangeSet = true;
                }
                else if (!strncmp(keyword,"hhub",4)) {  //Set heavy hole upper bound
                    pGlobals->BndStBounds.hhUB = -numconv(expression);
                    pGlobals->BndStBounds.hhBoundsSet = true;
                    pFlags->BndStRangeSet = true;
                }
                else if (!strncmp(keyword,"hhlb",4)) {  //Set heavy hole lower bound
                    pGlobals->BndStBounds.hhLB = -numconv(expression);
                    pGlobals->BndStBounds.hhBoundsSet = true;
                    pFlags->BndStRangeSet = true;
                }
                else if (!strncmp(keyword,"lhub",4)) {  //Set light hole upper bound
                    pGlobals->BndStBounds.lhUB = -numconv(expression);
                    pGlobals->BndStBounds.lhBoundsSet = true;
                    pFlags->BndStRangeSet = true;
                }
                else if (!strncmp(keyword,"lhlb",4)) {  //Set light hole lower bound
                    pGlobals->BndStBounds.lhLB = -numconv(expression);
                    pGlobals->BndStBounds.lhBoundsSet = true;
                    pFlags->BndStRangeSet = true;
                }
                else if (!strncmp(keyword,"sohub",5)) {  //Set split-off hole upper bound
                    pGlobals->BndStBounds.sohUB = -numconv(expression);
                    pGlobals->BndStBounds.sohBoundsSet = true;
                    pFlags->BndStRangeSet = true;
                }
                else if (!strncmp(keyword,"sohlb",5)) {  //Set split-off hole lower bound
                    pGlobals->BndStBounds.sohLB = -numconv(expression);
                    pGlobals->BndStBounds.sohBoundsSet = true;
                    pFlags->BndStRangeSet = true;
                }
            }
        }

		else if (!strncmp(keyword,"read",4)) {
			fieldlength=fieldtrim(info,&startsearch,keyword,expression,caseInSens);
			if (!strncmp(keyword,"param",5))
				pFlags->readParam = true;		// Read parameter file
		}
	
		else if (!strncmp(keyword,"dy",2)) {
			pGlobals->defaultdy = numconv(expression);
		}
        
        // Check to see if the first field contains 'surface', then skip over the layer specifications
        else if (!strcmp(keyword,"surface")) {
            while (strcmp(keyword, "substrate")) {
                infoLength=getInputNew(pGlobals->InputFile, &filepointer, &info);  //Get new line from input file
            
                // Check for end of file
                if (infoLength == EOF)
                    break;
            
                // extract the first field
                startsearch = 0;
                fieldlength=fieldtrim(info,&startsearch,keyword,expression,caseInSens);
            }
            if (infoLength == EOF) {
                if (info != NULL){
                    free(info);
                    info = NULL;
                }
                return 2; //If substrate flag not found, kick out with error 2
            }
            
        }
        
        else if (keyword[0] == 'v') {
            if ( pVoltageSource == NULL) {
                pGlobals->pFirstVoltage = (struct voltageSource_type *) malloc(sizeof(struct voltageSource_type));
                pVoltageSource = pGlobals->pFirstVoltage;
                pVoltageSource->pNextVoltageSource = NULL;
                pVoltageSource->multiRun = false;
            }
            else {
                pVoltageSource->pNextVoltageSource = (struct voltageSource_type *) malloc(sizeof(struct voltageSource_type));
                pVoltageSource = pVoltageSource->pNextVoltageSource;
                pVoltageSource->pNextVoltageSource = NULL;
                pVoltageSource->multiRun = false;
            }
            keyword[0] = ' ';
            pVoltageSource->sourceNumber=atoi(keyword);
            fieldlength = getSingField(expression,info,&startsearch);	// get the voltage value
            if (strlen(expression) != 0) {
                startsearch = 0;
                fieldlength = getSingField(keyword,info,&startsearch); //ignore keyword
                fieldlength = getSingField(keyword,info,&startsearch);
                pVoltageSource->vbias=strtod(keyword,&charPt); // get the source voltage
            }
            else {
                strcpy(message,"A voltage source must have a voltage declared");
                if (!pFlags->silent) alertbox(message);
                pFlags->stopthemusic = true;
                if (info != NULL){
                    free(info);
                    info = NULL;
                }
                return 1;
            }
            fieldlength = getSingField(keyword,info,&startsearch);	// get the stop voltage
            if (fieldlength != '\0') {
                pFlags->multiruns = true;
                pVoltageSource->vstop=strtod(keyword,&charPt);
                fieldlength = getSingField(keyword,info,&startsearch);	// get the voltage increment
                if (fieldlength != 0) {
                    pVoltageSource->vincrement=strtod(keyword,&charPt);
                    pVoltageSource->multiRun = true;
                }
                else {
                    strcpy(message,"You must give a voltage increment for multiple voltage runs");
                    if (!pFlags->silent) alertbox(message);
                    pFlags->stopthemusic = true;
                    if (info != NULL){
                        free(info);
                        info = NULL;
                    }
                    return 1;
                }
            }
        }
        
        else if (!strncmp(keyword,"contac",6)) {  	// read contact info
            if ( pCurrentContact == NULL) {
                pGlobals->pFirstContact = (struct contact_type *) malloc(sizeof(struct contact_type));
                pCurrentContact = pGlobals->pFirstContact;
                pCurrentContact->pNextContact = NULL;
                pCurrentContact->pVoltageSource = NULL;
            }
            else {
                pCurrentContact->pNextContact = (struct contact_type *) malloc(sizeof(struct contact_type));
                pCurrentContact = pCurrentContact->pNextContact;
                pCurrentContact->pNextContact = NULL;
                pCurrentContact->pVoltageSource = NULL;
            }
            while(fieldlength != 0) {
                fieldlength = fieldtrim(info,&startsearch,keyword,expression,caseInSens); // seperate out the contact keyword
                if (keyword[0] == 'y') {
                    pCurrentContact->realPosition = numconv(expression);
                }
                if (!strcmp(keyword,"schottky")) {
                    if (fieldlength == -1) {
                        strcpy(message,"You must declare a Schottky barrier height.");
                        if (!pFlags->silent) alertbox(message);
                        pFlags->stopthemusic = true;
                        if (info != NULL){
                            free(info);
                            info = NULL;
                        }
                        return 1;
                    }
                    pCurrentContact->contactType = SCHOTTKY;
                    if (strlen(expression) != 0) {
                        pCurrentContact->contactParameter = numconv(expression);	// define a barrier height
                    }
                    fieldlength = fieldtrim(info,&startsearch,keyword,expression,caseInSens);
                    if (keyword[0] == 'v') {
                        // surface voltage
                        keyword[0] = ' ';
                        pCurrentContact->voltageSourceNumber = numconv(keyword);
                    }
                    else {
                        strcpy(message,"You must declare a voltage source number for a Schottky.");
                        if (!pFlags->silent) alertbox(message);
                        pFlags->stopthemusic = true;
                        if (info != NULL){
                            free(info);
                            info = NULL;
                        }
                        return 1;
                    }
                }
                
                else if (!strcmp(keyword,"ohmic")) {
                    pCurrentContact->contactType = OHMIC;
                    fieldlength = fieldtrim(info,&startsearch,keyword,expression,caseInSens);
                    if (keyword[0] == 'v') {	  
                        // surface voltage
                        keyword[0] = ' ';
                        pCurrentContact->voltageSourceNumber = numconv(keyword);
                    }
                    else {
                        strcpy(message,"You must declare a voltage source number for n Ohmic contact");
                        if (!pFlags->silent) alertbox(message);
                        pFlags->stopthemusic = true;
                        if (info != NULL){
                            free(info);
                            info = NULL;
                        }
                        return 1;
                    }
                }
            }
        }
            
        else if (!strncmp(keyword,"cv",2)) {  	// read CV parameter info
            if (!strncmp(keyword,"cvconverg",9)) {  // Change convergence criteria for CV run
                fieldlength = fieldtrim(info,&startsearch,keyword,expression,caseInSens);
                pGlobals->CVconverg = strtod(expression,&charPt);
            }
            else {
                pFlags->cv = true;  // A CV run has been requested
                startsearch = 2;
                fieldlength = fieldtrim(info,&startsearch,keyword,expression,caseInSens);
                if (fieldlength != 0) {
                    if(!strncmp(keyword,"only",4))
                        pFlags->cvonly = true;  //Set the CV only flag, only the first band diagram is output
                    else{
                        if(!strncmp(keyword,"voltage",7)) {
                            pGlobals->cvvoltage=numconv(expression); // get CV voltage
                        }
                    }
                }
            }
        }
        
        else if (!strncmp(keyword,"implant",7)) {  	// read ion pFlags->implant parameters
            pGlobals->impnumber++;		/* increment the pFlags->implant counter */
            fieldlength = getSingField(expression,info,&startsearch);	// get the impant type
            pGlobals->imptype[pGlobals->impnumber-1] = expression[0]; // save the implant type
            if (pGlobals->imptype[pGlobals->impnumber-1] == 'a' || pGlobals->imptype[pGlobals->impnumber-1] == 'd')
                imptypefound = true;
            
            while (fieldlength != 0){
                fieldlength = fieldtrim(info,&startsearch,keyword,expression,caseInSens);	/* get the rest of the pFlags->implant info */
                if (!strncmp(keyword,"dose",4)) {
                    pGlobals->dose[pGlobals->impnumber-1] = numconv(expression);
                    dosefound = true;
                }
                else if (!strncmp(keyword,"rp",2)) {
                    pGlobals->Rp[pGlobals->impnumber-1] = numconv(expression);
                    Rpfound = true;
                }
                else if (!strncmp(keyword,"drp",3)) {
                    pGlobals->dRp[pGlobals->impnumber-1] = numconv(expression);
                    drpfound = true;
                }
            }
            
            if (dosefound && Rpfound && drpfound && imptypefound) {
                pFlags->implant = true;
                if (pGlobals->impnumber > 10) {
                    strcpy(message,"Only 10 implants may be specified");
                    if (!pFlags->silent) alertbox(message);
                    pFlags->stopthemusic = true;
                    if (info != NULL){
                        free(info);
                        info = NULL;
                    }
                    return 1;
                }
                pGlobals->dose[pGlobals->impnumber-1] = pGlobals->dose[pGlobals->impnumber-1]/(2.506628
                        *pGlobals->dRp[pGlobals->impnumber-1]*1.0e-8);	// pGlobals->dose scale factors: pGlobals->dose/(sqrt(2pi)*pGlobals->dRp) 
            }
            if (!imptypefound) {
                strcpy(message,"An implant type must be given for Ion Implant");
                if (!pFlags->silent) alertbox(message);
                pGlobals->impnumber = pGlobals->impnumber-1;
                pFlags->stopthemusic = true;
                if (info != NULL){
                    free(info);
                    info = NULL;
                }
                return 1;
            }
            else if (!dosefound) {
                strcpy(message,"A Dose must be given for Ion Implant");
                if (!pFlags->silent) alertbox(message);
                pGlobals->impnumber = pGlobals->impnumber-1;
                pFlags->stopthemusic = true;
                if (info != NULL){
                    free(info);
                    info = NULL;
                }
                return 1;
            }
            else if (!Rpfound) {
                strcpy(message,"An Ion Range must be given for Ion Implant");
                if (!pFlags->silent) alertbox(message);
                pGlobals->impnumber = pGlobals->impnumber-1;
                pFlags->stopthemusic = true;
                if (info != NULL){
                    free(info);
                    info = NULL;
                }
                return 1;
            }
            else if (!drpfound) {
                strcpy(message,"A Range Std. Dev. must be given for Ion Implant");
                if (!pFlags->silent) alertbox(message);
                pGlobals->impnumber = pGlobals->impnumber-1;
                pFlags->stopthemusic = true;
                if (info != NULL){
                    free(info);
                    info = NULL;
                }
                return 1;
            }
            
            /* now reset all pFlags->implant flags to be ready for another pFlags->implant spec. */
            dosefound = false;
            imptypefound = false;
            Rpfound = false;
            drpfound = false;
        }
        
        else if (!strncmp(keyword,"etch",4)) {  	/* An etch simulation is requested */
            pFlags->etched = true;
			
            fieldlength = getSingField(expression,info,&startsearch); /* get etch depth */
            pGlobals->etchdepth = numconv(expression);
            
            fieldlength = getSingField(expression,info,&startsearch); /* get etch increment */
            pGlobals->etchinc = numconv(expression);
        }
        
        else if (!strncmp(keyword,"etchonly",8))   /* Only sheet resistance is requested */
            pFlags->etchonly = true;
        
        
        else if (!strncmp(keyword,"temp",4)) {
            pGlobals->temperature = strtod(expression,&charPt);
            if (*charPt == 'k')
            /* pGlobals->temperature is given in kelven */
                tempconv = 0.0e0;	/* conversion factor */
            
            else if (*charPt == 'c')
            /* pGlobals->temperature is given in celsius */
                tempconv = 273.15e0;
            
            else
            /* pGlobals->temperature is assumed to be kelvin (default) */
                tempconv = 0.0e0;
            
            pGlobals->temperature = pGlobals->temperature + tempconv;	/* pGlobals->temperature in K */
        }
        
        else if (!strncmp(keyword,"inverse",7)) {
            // set the call for inverse solution
            pFlags->invert = true;
        }
        
        else if (!strncmp(keyword,"schrodingerstart",16)) {
            /* read in the starting point of the pFlags->schrodinger mesh */
            pFlags->schstartflag = true;
            pGlobals->realschstart = numconv(expression);
        }
        
        else if (!strncmp(keyword,"schrodingerstop",15)) {
            pFlags->schstopflag = true;
            pGlobals->realschstop = numconv(expression);
        }
        
        else if (!strncmp(keyword,"converg",7)) {  /* Change convergence criteria */
            pGlobals->converg = strtod(expression,&charPt);
        }
        
        else if (!strncmp(keyword,"maxiterat",9)) {  /* Change convergence criteria */
            pGlobals->maxcount = strtod(expression,&charPt);
        }
        
        else if (!strncmp(keyword,"crossover",9)) {  /* Change point where schrodinger solver kicks in */
            pGlobals->crossover = strtod(expression,&charPt);
        }
        
        else if (!strncmp(keyword,"exchange",8))   /* Use pFlags->exchange potential */
            pFlags->exchange = true;
        
        else if (!strncmp(keyword,"stop",4)) {  /* Stop  */
            fieldlength = getSingField(expression,info,&startsearch);
            fieldlength = getSingField(expression,info,&startsearch);
            if (!strncmp(expression,"will",4)) 
                pFlags->stopDanger = true;		/* Stop on Will Robinson error */
        }
        
        else if (!strncmp(keyword,"ignore",6))
            pFlags->ignoreDanger = true;		/* Ignore  Will Robinson error */
        
        
        else if (!strncmp(keyword,"epsilona",8))
            pGlobals->epsilonA = strtod(expression,&charPt);	//set new value of epsilon parameter
        
        
        else if (!strncmp(keyword,"epsilonb",8))
            pGlobals->epsilonB = strtod(expression,&charPt);	//set new value of epsilon parameter
        
        else {
            strcpy(message,"Please correct input file.  ");
            strcat(message,"Sorry, I dont know what \"");
            strcat(message,keyword);
            strcat(message,"\" means.");
            if (!pFlags->silent) alertbox (message);
            pFlags->stopthemusic = true;
            if (info != NULL){
                free(info);
                info = NULL;
            }
            return 3; //kick out with error 3
        }
	}


	// Now skip through the input file and read the layer definitions
	
	filepointer =0;
	// loop until all the data has been read 
	while(true) {
	 
		// read data line, which will then be analyzed 
		infoLength=getInputNew(pGlobals->InputFile, &filepointer, &info);  //Get new line from input file
		//infoLength=getInput(4,info,pGlobals);

		 // Look for end of file
		if (infoLength == EOF)
			break;
		
	   // extract the first field 
		startsearch = 0;
		fieldlength=fieldtrim(info,&startsearch,keyword,expression,caseInSens);


		if (fieldlength != 0) {

			// Look for surface flag, then process layers
			if (!strcmp(keyword,"surface")) {
	    		ioerr_loc = readlayers(&info, &filepointer, pGlobals, pFlags);		// read the layer specifications 
                if (ioerr_loc){
                    if (info != NULL){
                        free(info);
                        info = NULL;
                    }
	      			return 4; //kick out on a readlayers error
                }
			}

        }
	}

    if (info != NULL) {
        free(info);
        info = NULL;
    }
	return 0;  // Normal return
}

/***********************************************************/

int output(struct global_type *pGlobals, struct flag_type *pFlags) {
	  
/***********************************************************/
  
//  This routine creates the output file. 

	double y;		/* the real y coordinate */
	int ioerr_loc,j,i,fileLength;
	char access[3];
	char extension2[MAX_EXTENSION],*outdata,*buffer,outsubdata[20];
	struct mesh_type *pMesh;
	
    if (pGlobals->OutputFile !=NULL)
        free(pGlobals->OutputFile);
    
    pGlobals->numeval = pGlobals->numeleceval+pGlobals->numhheval+pGlobals->numlheval+pGlobals->numsoheval;
    
    //Size of output file: Header(256 characters for header of normal output plus 2*20 for each subband)
    //  + data(number of mesh points*20*8values + order of matrix * 2* nuber of subbands*20)
    
    //fileLength = 120+pGlobals->numMesh*128+pGlobals->numeval*(50+36*pGlobals->order);
    fileLength = 256+2*20*pGlobals->numeval
                +((pGlobals->numMesh+100)*8*20 + (2*20*pGlobals->numeval*pGlobals->order));
    
	outdata =  (char *) malloc(fileLength*sizeof(char));
	pFlags->outfileexists = true;
	//strcpy(access,"w");
	//strcpy(extension2,extension);
	//strcat(extension2,"_Out");
	//ioerr_loc = fileopen(4, pGlobals->filename, access, extension2, pGlobals);
	//if(ioerr_loc != 0) return 1;
  
//  File_out contains the conduction and valence bands, hole and electron concentrations, and the eigenvalues found
	
	y=0.0e0;
		
	if (pFlags->schrodinger && pGlobals->numeval > 0) {
		strcpy(outdata,"Y (ang)\tEc (eV)\tEv (eV)\tE (V/cm)\tEf (eV)\tn (cm-3)\tp (cm-3)\tNd - Na (cm-3)");
		if (pGlobals->numeleceval > 0) {
			for (j=0;j<pGlobals->numeleceval;j++) {
				strcat(outdata,"\tEL Eval ");
				sprintf(outsubdata, "%d", j+1);
				strcat(outdata,outsubdata);
				strcat(outdata, " (eV)");
			}
		}
		if (pGlobals->numhheval > 0) {
			for (j=0;j<pGlobals->numhheval;j++) {
				strcat(outdata,"\tHH Eval ");
				sprintf(outsubdata, "%d", j+1);
				strcat(outdata,outsubdata);
				strcat(outdata, " (eV)");
			}
		}
		if (pGlobals->numlheval > 0) {
			for (j=0;j<pGlobals->numlheval;j++) {
				strcat(outdata,"\tLH eval ");
				sprintf(outsubdata, "%d", j+1);
				strcat(outdata,outsubdata);
				strcat(outdata, " (eV)");
			}
		}
		if (pGlobals->numsoheval > 0) {
			for (j=0;j<pGlobals->numsoheval;j++) {
				strcat(outdata,"\tSOH eval ");
				sprintf(outsubdata, "%d", j+1);
				strcat(outdata,outsubdata);
				strcat(outdata, " (eV)");
			}
		}
		if (pGlobals->numeleceval > 0) {
			for (j=0;j<pGlobals->numeleceval;j++) {
				strcat(outdata,"\tEL Psi ");
				sprintf(outsubdata, "%d", j+1);
				strcat(outdata,outsubdata);
			}
		}
		if (pGlobals->numhheval > 0) {
			for (j=0;j<pGlobals->numhheval;j++) {
				strcat(outdata,"\tHH Psi ");
				sprintf(outsubdata, "%d", j+1);
				strcat(outdata,outsubdata);
			}
		}
		if (pGlobals->numlheval > 0) {
			for (j=0;j<pGlobals->numlheval;j++) {
				strcat(outdata,"\tLH Psi ");
				sprintf(outsubdata, "%d", j+1);
				strcat(outdata,outsubdata);
			}
		}
		if (pGlobals->numsoheval > 0) {
			for (j=0;j<pGlobals->numsoheval;j++) {
				strcat(outdata,"\tSOH Psi ");
				sprintf(outsubdata, "%d", j+1);
				strcat(outdata,outsubdata);
			}
		}
		strcat(outdata,"\n");
	}
	else 
		strcpy(outdata,"Y (ang)\tEc (eV)\tEv (eV)\tE (V/cm)\tEf (eV)\tn (cm-3)\tp (cm-3)\tNd - Na (cm-3)\n");
		
    
    //fileLength = pGlobals->numMesh*100+pGlobals->numeval*24;
    fileLength = pGlobals->numMesh*500+pGlobals->numeval*100;
	buffer =  (char *) malloc(fileLength*sizeof(char));
	pMesh = pGlobals->pTopMesh;
	i=0;
    do {
		
		sprintf(buffer, "%13.6e\t%15.8e\t%15.8e\t%15.8e\t%15.8e\t%11.4e\t%11.4e\t%11.4e",
                pMesh->meshcoord,pMesh->v+pMesh->offset, pMesh->v+pMesh->offset-pMesh->Eg,
                (pMesh->pDownMesh->v-pMesh->v)*1.e8/pMesh->pDownMesh->dy,pMesh->Efn,
                //(pMesh->pDownMesh->v-pMesh->v)*1.e8/pMesh->pDownMesh->dy,pMesh->esemi/dicon,  //For Amit
			pMesh->n, pMesh->p, pMesh->fcharge);
			
		if ( pFlags->schrodinger && ((pMesh->meshcoord >= pGlobals->pSchStart->meshcoord) && (pMesh->meshcoord < pGlobals->pSchStop->meshcoord))) {
			if (pGlobals->numeleceval > 0) {
				for (j=0;j<pGlobals->numeleceval;j++) {
					sprintf(outsubdata, "\t%11.4e", pGlobals->pEleceval[j]);
					strcat(buffer,outsubdata);
				}
			}
			if (pGlobals->numhheval > 0) {
				for (j=0;j<pGlobals->numhheval;j++) {
					sprintf(outsubdata, "\t%11.4e", pGlobals->pHheval[j]);
					strcat(buffer,outsubdata);
				}
			}
			if (pGlobals->numlheval > 0) {
				for (j=0;j<pGlobals->numlheval;j++) {
					sprintf(outsubdata, "\t%11.4e", pGlobals->pLheval[j]);
					strcat(buffer,outsubdata);
				}
			}
			if (pGlobals->numsoheval > 0) {
				for (j=0;j<pGlobals->numsoheval;j++) {
					sprintf(outsubdata, "\t%11.4e", pGlobals->pSoheval[j]);
					strcat(buffer,outsubdata);
				}
			}
            if (pGlobals->numeleceval > 0) {
                for (j=0;j<pGlobals->numeleceval;j++) {
                    sprintf(outsubdata, "\t%11.4e", pGlobals->pElecevec[i][j]);
                    strcat(buffer,outsubdata);
                }
            }
            if (pGlobals->numhheval > 0) {
                for (j=0;j<pGlobals->numhheval;j++) {
                    sprintf(outsubdata, "\t%11.4e", pGlobals->pHhevec[i][j]);
                    strcat(buffer,outsubdata);
                }
            }
            if (pGlobals->numlheval > 0) {
                for (j=0;j<pGlobals->numlheval;j++) {
                    sprintf(outsubdata, "\t%11.4e", pGlobals->pLhevec[i][j]);
                    strcat(buffer,outsubdata);
                }
            }
            if (pGlobals->numsoheval > 0) {
                for (j=0;j<pGlobals->numsoheval;j++) {
                    sprintf(outsubdata, "\t%11.4e", pGlobals->pSohevec[i][j]);
                    strcat(buffer,outsubdata);
                }
            }
            i++;

		}
		strcat(buffer,"\n");
        //printf("11i",strlen(outdata));
        //printf("\n");
        //outdata = (char *) realloc(outdata, (strlen(outdata)+lineLength+1)*sizeof(char));
        strcat(outdata, buffer);
        
    } while ((pMesh = pMesh->pDownMesh) != pGlobals->pBottomMesh);
    
    // write last coordinate
    sprintf(buffer, "%13.6e\t%15.8e\t%15.8e\t%15.8e\t%15.8e\t%11.4e\t%11.4e\t%11.4e",
            pMesh->meshcoord,pMesh->v+pMesh->offset, pMesh->v+pMesh->offset-pMesh->Eg,
            (pMesh->v-pMesh->pUpMesh->v)*1.e8/pMesh->dy,pMesh->Efn,
            //(pMesh->pDownMesh->v-pMesh->v)*1.e8/pMesh->pDownMesh->dy,pMesh->esemi/dicon,  //For Amit
            pMesh->n, pMesh->p, pMesh->fcharge);
    
    if ( pFlags->schrodinger && ((pMesh->meshcoord >= pGlobals->pSchStart->meshcoord) && (pMesh->meshcoord < pGlobals->pSchStop->meshcoord))) {
        if (pGlobals->numeleceval > 0) {
            for (j=0;j<pGlobals->numeleceval;j++) {
                sprintf(outsubdata, "\t%11.4e", pGlobals->pEleceval[j]);
                strcat(buffer,outsubdata);
            }
        }
        if (pGlobals->numhheval > 0) {
            for (j=0;j<pGlobals->numhheval;j++) {
                sprintf(outsubdata, "\t%11.4e", pGlobals->pHheval[j]);
                strcat(buffer,outsubdata);
            }
        }
        if (pGlobals->numlheval > 0) {
            for (j=0;j<pGlobals->numlheval;j++) {
                sprintf(outsubdata, "\t%11.4e", pGlobals->pLheval[j]);
                strcat(buffer,outsubdata);
            }
        }
        if (pGlobals->numsoheval > 0) {
            for (j=0;j<pGlobals->numsoheval;j++) {
                sprintf(outsubdata, "\t%11.4e", pGlobals->pSoheval[j]);
                strcat(buffer,outsubdata);
            }
        }
        if (pGlobals->numeleceval > 0) {
            for (j=0;j<pGlobals->numeleceval;j++) {
                sprintf(outsubdata, "\t%11.4e", pGlobals->pElecevec[i][j]);
                strcat(buffer,outsubdata);
            }
        }
        if (pGlobals->numhheval > 0) {
            for (j=0;j<pGlobals->numhheval;j++) {
                sprintf(outsubdata, "\t%11.4e", pGlobals->pHhevec[i][j]);
                strcat(buffer,outsubdata);
            }
        }
        if (pGlobals->numlheval > 0) {
            for (j=0;j<pGlobals->numlheval;j++) {
                sprintf(outsubdata, "\t%11.4e", pGlobals->pLhevec[i][j]);
                strcat(buffer,outsubdata);
            }
        }
        if (pGlobals->numsoheval > 0) {
            for (j=0;j<pGlobals->numsoheval;j++) {
                sprintf(outsubdata, "\t%11.4e", pGlobals->pSohevec[i][j]);
                strcat(buffer,outsubdata);
            }
        }
        i++;
        
    }
    strcat(buffer,"\n");
    //printf("11i",strlen(outdata));
    //printf("\n");
    //outdata = (char *) realloc(outdata, (strlen(outdata)+lineLength+1)*sizeof(char));
    strcat(outdata, buffer);
	
	pGlobals->OutputFile = outdata;
    //lineLength = strlen(outdata);

     
  
	free(buffer);
	  
		
	ioerr_loc = writestatus(pGlobals, pFlags);
	if(ioerr_loc != 0) return 1;
	
	return 0;
}

/**********************************************************/
int writestatus(struct global_type *pGlobals, struct flag_type *pFlags) {
/**********************************************************/
  
/*  This routine creates the _status file. */

	double sheetres;	/* Sheet resistance of the structure */
	double nsTotal,psTotal;
	int ioerr_loc,i, lineLength, filelength;
	char access[3],extension2[MAX_EXTENSION],*outdata,*buffer,*buffer2;
	struct layer_type *pLayer;
	
    //return 0;
	if (!pFlags->noStatus) {
        
        if (pGlobals->StatusFile !=NULL)
            free(pGlobals->StatusFile);
        lineLength = 1024;
        filelength = 2000 + 512*pGlobals->numLayers + 5000*pGlobals->numeval;
        outdata =  (char *) malloc(filelength*sizeof(char));
        buffer =  (char *) malloc(lineLength*sizeof(char));
		sheetcharge(&nsTotal,&psTotal,&sheetres,pGlobals); //Calculate the sheet charge in each layer
	  
	  
//  File _status  contains info on the execution and the final solution
	
		sprintf(outdata,"Number of iterations to converge = %i\n",pGlobals->iterationCount);

		sprintf(buffer,"Final correction to bands = %10.3e eV\n",pGlobals->ConvgError);
		strcat(outdata,buffer);

		sprintf(buffer,"Maximum error in poisson equation= %10.3e\n",pGlobals->poierr);
		strcat(outdata,buffer);
		
		if (pGlobals->poierr < 1.0e-3) 
			strcat(outdata,"Don't worry, be happy! The convergence is good!\n");
		else
			strcat(outdata,"Worry, be sad.  The convergence is not good.\n");
	
		if (pFlags->multiruns) {
			sprintf(buffer,"Multi-run  %10.3e, %10.3e, %10.3e\n",pGlobals->multiRun.pContact->pVoltageSource->vbias,pGlobals->multiRun.pContact->pVoltageSource->vstop,pGlobals->multiRun.pContact->pVoltageSource->vincrement);
            strcat(outdata,buffer);
		}
        
        //outdata = (char *) realloc(outdata, (strlen(outdata)+lineLength+1)*sizeof(char));
        
		strcat(outdata,"\n");
		sprintf(buffer,"Structure Sheet Resistance = %10.3e Ohms/square\n",sheetres);
        strcat(outdata,buffer);

		strcat(outdata,"\n");
		strcat(outdata,"Layer sheet concentrations\n");
		strcat(outdata,"\n");
		
		/*if (!strncmp(pGlobals->surfcontact,"slope",5)) {
			strcat(outdata,"surface            ");
			strcat(outdata,pGlobals->surfcontact);
			strcat(outdata,"=0\n");
		}
		else {*/
			strcat(outdata,"surface            ");
		//	strcat(outdata,pGlobals->surfcontact);
			strcat(outdata,"\n");
		//}
		
        //outdata = (char *) realloc(outdata, (strlen(outdata)+lineLength+1)*sizeof(char));
		pLayer = pGlobals->pTopLayer;
		do {
            
            //outdata = (char *) realloc(outdata, (strlen(outdata)+lineLength+1)*sizeof(char));
        strcat(outdata,"-----------------------------------------------------------------------------------------\n");
			sprintf(buffer, "%5.1f",pLayer->tLayer);
            strcat(outdata,buffer);
			strcat(outdata," Ang\t");
			strcat(outdata,pLayer->nameLayer);
			strcat(outdata,"\tns= ");
			sprintf(buffer, "%10.3e",pLayer->nsLayer);
			strcat(outdata,buffer);
			strcat(outdata," cm-2,\tps= ");
			sprintf(buffer, "%10.3e",pLayer->psLayer);
			strcat(outdata,buffer);
			strcat(outdata," cm-2\n");
            //outdata = (char *) realloc(outdata, (strlen(outdata)+lineLength+1)*sizeof(char));
		} while((pLayer=pLayer->pDownLayer) != NULL);
		strcat(outdata,"-----------------------------------------------------------------------------------------\n");
		
		/*if (!strncmp(pGlobals->subcontact,"slope",5)) {
			strcat(outdata,"substrate            ");
			strcat(outdata,pGlobals->subcontact);
			strcat(outdata,"=0\n");
		}
		else {*/
			strcat(outdata,"substrate            ");
		//	strcat(outdata,pGlobals->subcontact);
			strcat(outdata,"\n");
		//}
		
		strcat(outdata,"\n");
		strcat(outdata,"Temperature = ");
		sprintf(buffer, "%5.1f",pGlobals->temperature);
		strcat(outdata,buffer);
		strcat(outdata," K\n");
		strcat(outdata,"\n");
		
        //lineLength = 50 * (pGlobals->numeval+4);
        //outdata = (char *) realloc(outdata, (strlen(outdata)+lineLength+1000*pGlobals->numeval)*sizeof(char));
        
		if (pFlags->schrodinger) {
			strcat(outdata,"Schrodinger solution from ");
			sprintf(buffer, "%10.3e",pGlobals->pSchStart->meshcoord);
			strcat(outdata,buffer);
			strcat(outdata," Ang to ");
			sprintf(buffer, "%10.3e",pGlobals->pSchStop->meshcoord);
			strcat(outdata,buffer);
			strcat(outdata," Ang.\n");
			
			strcat(outdata,"\n");
			strcat(outdata,"The following subband energies were found (E-Ef):\n");
			strcat(outdata,"\n");
			for (i=0;i<pGlobals->numeleceval;i++) {
				sprintf(buffer, "Electron eigenvalue %i = %13.6e eV\n",i+1,pGlobals->pEleceval[i]);
                strcat(outdata,buffer);
            }
			for (i=0;i<pGlobals->numhheval;i++) {
				sprintf(buffer, "Heavy hole eigenvalue %i = %13.6e eV\n",i+1,pGlobals->pHheval[i]);
                strcat(outdata,buffer);
            }
            //return 0;
			for (i=0;i<pGlobals->numlheval;i++) {
				sprintf(buffer, "Light hole eigenvalue %i = %13.6e eV\n",i+1,pGlobals->pLheval[i]);
                strcat(outdata,buffer);
            }
            //return 0;
			for (i=0;i<pGlobals->numsoheval;i++) {
				sprintf(buffer, "Split-off hole eigenvalue %i = %13.6e eV\n",i+1,pGlobals->pSoheval[i]);
                strcat(outdata,buffer);
            }
        }
        
        //return 0;
        pGlobals->StatusFile = outdata;
        
        free(buffer);
	}
	
	return 0;
}

/**********************************************************/
int readParamFile(struct global_type *pGlobals, struct flag_type *pFlags){
/**********************************************************/
//Routine to read in the parameter file
    
	char message[MAX_MESSAGE], keyword[MAX_KEYWORD];
    char *info=NULL,extension[MAX_EXTENSION],access[3],expression[MAX_EXPRESS], *ParamFile=NULL;
    int filepointer, linepointer, infoLength, numparams=0, numpoints=0,keywordLength;
    int i,j,paramList[30];
    double proportion;
    struct mesh_type *pMesh;
    struct param_type *pParams;
    
    strcpy(access,"r");
    strcpy(extension,"_param.txt");
    ParamFile = fileread(pGlobals->inputPath, access, extension); // read in the parameter file
    if (ParamFile == NULL) {
        strcpy(message, "Parameter file not found");
        alertbox(message);
        pFlags->stopthemusic = true;
        return 1;
    }
    

    // Get first line from the parameter file
    filepointer = 0;
    infoLength=getInputNew(ParamFile, &filepointer, &info);  //Get new line from input file
    // extract the first field
    linepointer = 0;
    keywordLength=fieldtrim(info,&linepointer,keyword,expression,caseInSens);
    
    if (strncmp(keyword,"depth",5)) {
        strcpy(message, "Incorrect format for Parameter file");
        alertbox(message);
        pFlags->stopthemusic = true;
        return 1;
    }
    
    //Now read the rest of the first line to get the parameter list
    while(keywordLength != 0) {
        keywordLength=fieldtrim(info,&linepointer,keyword,expression,caseInSens);
        //Energy gap
        if (!strncmp(keyword,"eg",2)) {
            paramList[numparams] = 1;
            numparams++;
        }
        //Band offset
        else if (!strncmp(keyword,"deltaec",7)) {
            paramList[numparams] = 2;
            numparams++;
        }
        
        //Relative dielectric constant
        else if (!strncmp(keyword,"er",2)) {
            paramList[numparams] = 3;
            numparams++;
        }
        
        //Electron effective mass
        else if (!strncmp(keyword,"me",2)) {
            paramList[numparams] = 4;
            numparams++;
        }
        
        //Valley degeneracy
        else if (!strncmp(keyword,"valley",6)) {
            paramList[numparams] = 5;
            numparams++;
        }
        
        //Heavy Hole effective mass
        else if (!strcmp(keyword,"mhh")) {
            paramList[numparams] = 6;
            numparams++;
        }
        
        //Light Hole effective mass
        else if (!strcmp(keyword,"mlh")) {
            paramList[numparams] = 7;
            numparams++;
        }
        
        //Split-off Hole effective mass
        else if (!strcmp(keyword,"msoh")) {
            paramList[numparams] = 8;
            numparams++;
        }
        
        //Split-off band energy
        else if (!strncmp(keyword,"Eso",3)) {
            paramList[numparams] = 9;
            numparams++;
        }
        
        //	Donor concentration
        else if (!strcmp(keyword,"nd")) {
            paramList[numparams] = 10;
            numparams++;
        }
        
        //		Deep donor concentration
        else if (!strcmp(keyword,"ndd")) {
            paramList[numparams] = 11;
            numparams++;
        }
        
        //		Acceptor concentration
        else if (!strcmp(keyword,"na")) {
            paramList[numparams] = 12;
            numparams++;
        }
        
        //Deep acceptor concentration
        else if (!strcmp(keyword,"nda")) {
            paramList[numparams] = 13;
            numparams++;
        }
        
        //Donor level
        else if (!strcmp(keyword,"ed")) {
            paramList[numparams] = 14;
            numparams++;
        }
        
        //Deep donor level
        else if (!strcmp(keyword,"edd")) {
            paramList[numparams] = 15;
            numparams++;
        }
        
        //Acceptor level
        else if (!strcmp(keyword,"ea")) {
            paramList[numparams] = 16;
            numparams++;
        }
        
        //Deep acceptor level
        else if (!strcmp(keyword,"eda")) {
            paramList[numparams] = 17;
            numparams++;
        }
        
        //electron mobility
        else if (!strncmp(keyword,"emob",4)) {
            paramList[numparams] = 18;
            numparams++;
        }
        
        //hole mobility
        else if (!strncmp(keyword,"hmob",4)) {
            paramList[numparams] = 19;
            numparams++;
                }
        
        //electron recombination time
        else if (!strncmp(keyword,"tn",2)) {
            paramList[numparams] = 20;
            numparams++;
        }
        
        //hole recombination time
        else if (!strncmp(keyword,"tp",2)) {
            paramList[numparams] = 21;
            numparams++;
        }
        
        //Semiconductor Polarization
        else if (!strncmp(keyword,"pol",3)) {
            paramList[numparams] = 22;
            numparams++;
        }
        
        //Absorption coefficient
        else if (!strncmp(keyword,"alpha",5)) {
            paramList[numparams] = 23;
            numparams++;
        }
        
        //	Electron concentration
        else if (!strcmp(keyword,"n")) {
            paramList[numparams] = 24;
            numparams++;
        }
        
        //	hole concentration
        else if (!strcmp(keyword,"p")) {
            paramList[numparams] = 25;
            numparams++;
        }
    }
    
	while(true) {
		// read data line, which will then be analyzed
		infoLength=getInputNew(ParamFile, &filepointer, &info);  //Get new line from input file
        
		// Check for end of file
		if (infoLength == EOF)
			break;
        
        numpoints++;
    }
    
    pParams = (struct param_type *) malloc(numpoints*sizeof(struct param_type));
    
    filepointer = 0;
    infoLength=getInputNew(ParamFile, &filepointer, &info);  //skip first line
    i=0;
	while(true) {
		// read data line, which will then be analyzed
		infoLength=getInputNew(ParamFile, &filepointer, &info);  //Get new line from input file
        
		// Check for end of file
		if (infoLength == EOF)
			break;
        
        
        linepointer = 0;
        keywordLength=fieldtrim(info,&linepointer,keyword,expression,caseInSens);
        pParams[i].depth = numconv(keyword);
        
        //Now read the rest of the  line to get the parameter info
        j=0;
        keywordLength=fieldtrim(info,&linepointer,keyword,expression,caseInSens);
        while(keywordLength != 0) {
            if (paramList[j] == 1) pParams[i].Eg = numconv(keyword);
            else if (paramList[j] == 2) pParams[i].offset = numconv(keyword);
            else if (paramList[j] == 3) pParams[i].esemi = numconv(keyword);
            else if (paramList[j] == 4) pParams[i].eem = numconv(keyword);
            else if (paramList[j] == 5) pParams[i].valley = numconv(keyword);
            else if (paramList[j] == 6) pParams[i].ehm = numconv(keyword);
            else if (paramList[j] == 7) pParams[i].elhm = numconv(keyword);
            else if (paramList[j] == 8) pParams[i].esohm = numconv(keyword);
            else if (paramList[j] == 9) pParams[i].Eso = numconv(keyword);
            else if (paramList[j] == 10) pParams[i].Nd = numconv(keyword);
            else if (paramList[j] == 11) pParams[i].Ndd = numconv(keyword);
            else if (paramList[j] == 13) pParams[i].Na = numconv(keyword);
            else if (paramList[j] == 14) pParams[i].Nda = numconv(keyword);
            else if (paramList[j] == 15) pParams[i].Ed = numconv(keyword);
            else if (paramList[j] == 16) pParams[i].Edd = numconv(keyword);
            else if (paramList[j] == 17) pParams[i].Ea = numconv(keyword);
            else if (paramList[j] == 18) pParams[i].Eda = numconv(keyword);
            else if (paramList[j] == 19) pParams[i].emob = numconv(keyword);
            else if (paramList[j] == 20) pParams[i].hmob = numconv(keyword);
            else if (paramList[j] == 21) pParams[i].taun = numconv(keyword);
            else if (paramList[j] == 22) pParams[i].taup = numconv(keyword);
            else if (paramList[j] == 23) pParams[i].alpha = numconv(keyword);
            else if (paramList[j] == 24) pParams[i].n = numconv(keyword);
            else if (paramList[j] == 25) pParams[i].p = numconv(keyword);
            j++;
            keywordLength=fieldtrim(info,&linepointer,keyword,expression,caseInSens);
        }
        i++;
    }
    //free(ParamFile);

    // Now use the parameter data to overwrite the parameters in the structure
    pMesh = pGlobals->pTopMesh;
    i = 0;
    //while (pMesh != pGlobals->pBottomMesh  && i < numpoints) {
    while (pMesh != NULL  && i < numpoints) {
        if (pMesh->meshcoord >= pParams[i].depth && pMesh->meshcoord <= pParams[i+1].depth) {
            proportion = (pMesh->meshcoord - pParams[i].depth) / (pParams[i+1].depth - pParams[i].depth);
            for (j=0; j<numparams; j++) {
                if (paramList[j] == 1) pMesh->Eg = pParams[i].Eg + proportion*(pParams[i+1].Eg - pParams[i].Eg);
                else if (paramList[j] == 2) pMesh->offset = pParams[i].offset + proportion*(pParams[i+1].offset - pParams[i].offset);
                else if (paramList[j] == 3) pMesh->esemi = pParams[i].esemi + proportion*(pParams[i+1].esemi - pParams[i].esemi);
                else if (paramList[j] == 4) pMesh->eem = pParams[i].eem + proportion*(pParams[i+1].eem - pParams[i].eem);
                else if (paramList[j] == 5) pMesh->valley = pParams[i].valley + proportion*(pParams[i+1].valley - pParams[i].valley);
                else if (paramList[j] == 6) pMesh->ehm = pParams[i].ehm + proportion*(pParams[i+1].ehm - pParams[i].ehm);
                else if (paramList[j] == 7) pMesh->elhm = pParams[i].elhm + proportion*(pParams[i+1].elhm - pParams[i].elhm);
                else if (paramList[j] == 8) pMesh->esohm = pParams[i].esohm + proportion*(pParams[i+1].esohm - pParams[i].esohm);
                else if (paramList[j] == 9) pMesh->Eso = pParams[i].Eso + proportion*(pParams[i+1].Eso - pParams[i].Eso);
                else if (paramList[j] == 10) pMesh->Nd = pParams[i].Nd + proportion*(pParams[i+1].Nd - pParams[i].Nd);
                else if (paramList[j] == 11) pMesh->Ndd = pParams[i].Ndd + proportion*(pParams[i+1].Ndd - pParams[i].Ndd);
                else if (paramList[j] == 13) pMesh->Na = pParams[i].Na + proportion*(pParams[i+1].Na - pParams[i].Na);
                else if (paramList[j] == 14) pMesh->Nda = pParams[i].Nda + proportion*(pParams[i+1].Nda - pParams[i].Nda);
                else if (paramList[j] == 15) pMesh->Ed = pParams[i].Ed + proportion*(pParams[i+1].Ed - pParams[i].Ed);
                else if (paramList[j] == 16) pMesh->Edd = pParams[i].Edd + proportion*(pParams[i+1].Edd - pParams[i].Edd);
                else if (paramList[j] == 17) pMesh->Ea = pParams[i].Ea + proportion*(pParams[i+1].Ea - pParams[i].Ea);
                else if (paramList[j] == 18) pMesh->Eda = pParams[i].Eda + proportion*(pParams[i+1].Eda - pParams[i].Eda);
                else if (paramList[j] == 19) pMesh->emob = pParams[i].emob + proportion*(pParams[i+1].emob - pParams[i].emob);
                else if (paramList[j] == 20) pMesh->hmob = pParams[i].hmob + proportion*(pParams[i+1].hmob - pParams[i].hmob);
                else if (paramList[j] == 21) pMesh->taun = pParams[i].taun + proportion*(pParams[i+1].taun - pParams[i].taun);
                else if (paramList[j] == 22) pMesh->taup = pParams[i].taup + proportion*(pParams[i+1].taup - pParams[i].taup);
                else if (paramList[j] == 23) pMesh->alpha = pParams[i].alpha + proportion*(pParams[i+1].alpha - pParams[i].alpha);
                else if (paramList[j] == 24) pMesh->n = pParams[i].n + proportion*(pParams[i+1].n - pParams[i].n);
                else if (paramList[j] == 25) pMesh->p = pParams[i].p + proportion*(pParams[i+1].p - pParams[i].p);
            }
            
            //Now correct for possible changes in effective masses
            if (pGlobals->kT > 0.0e0) {
                if (!pMesh->pLayer->mtrlType){  //If a 3D semiconductor
                    pMesh->Nc = 6.038165e21*pMesh->valley*pow(pMesh->eem*pGlobals->kT,1.5);	// conduction band eff. den. st.
                    pMesh->Nv = 6.038165e21*(pow(pMesh->ehm,1.5) + pow(pMesh->elhm,1.5))*pow(pGlobals->kT,1.5);	// valence band eff. den. st.
                    pMesh->Nvso = 6.038165e21*pow(pMesh->ehm*pGlobals->kT,1.5);	// valence band eff. den. st.
                }
                else if (pMesh->pLayer->mtrlType == twoDmtrl){ //2D parabolic semiconductor
                    pMesh->Nc = 4.177756e14*pMesh->valley*pMesh->eem;	// conduction band eff. den. st. for 2D
                    pMesh->Nv = 4.177756e14*pow(pow(pMesh->ehm,1.5) + pow(pMesh->elhm,1.5),0.666667);	// valence band eff. den. st. for 2D
                    pMesh->Nvso = 4.177756e14*pMesh->esohm;	// Split-off band eff. den. st. for 2D
                }
                else if (pMesh->pLayer->mtrlType == graphenemtrl){ //2D parabolic semiconductor
                    pMesh->Nc = 1.469411e30*pGlobals->kT*pGlobals->kT/(FermiVel*FermiVel);	// conduction band eff. den. st. for graphene
                    pMesh->Nv = 1.469411e30*pGlobals->kT*pGlobals->kT/(FermiVel*FermiVel);	// valence band eff. den. st. for graphene
                }
            }
            else {
                if (!pMesh->pLayer->mtrlType){  //If a 3D semiconductor
                    pMesh->Nc = 6.813341e21*pMesh->valley*pow(pMesh->eem,1.5);	// conduction band eff. den. st.
                    pMesh->Nv = 6.813341e21*(pow(pMesh->ehm,1.5) + pow(pMesh->elhm,1.5));	// valence band eff. den. st.
                    pMesh->Nvso = 6.813341e21*pow(pMesh->esohm,1.5);	// split-off band eff. den. st.
                }
                else if (pMesh->pLayer->mtrlType == twoDmtrl){ //2D parabolic semiconductor
                    pMesh->Nc = 4.177756e14*pMesh->valley*pMesh->eem;	// conduction band eff. den. st. for 2D
                    pMesh->Nv = 4.177756e14*pow(pow(pMesh->ehm,1.5) + pow(pMesh->elhm,1.5),0.666667);	// valence band eff. den. st. for 2D
                    pMesh->Nvso = 4.177756e14*pMesh->esohm;	// splitk-off band eff. den. st. for 2D
                }
                else if (pMesh->pLayer->mtrlType == graphenemtrl){ //graphene
                    pMesh->Nc = 1.469411e30/(FermiVel*FermiVel);	// conduction band eff. den. st. for graphene
                    pMesh->Nv = 1.469411e30/(FermiVel*FermiVel);	// valence band eff. den. st. for graphene
                }
            }
        }
        pMesh = pMesh->pDownMesh;
        if (pMesh == NULL) break;
        if (pMesh->meshcoord >= pParams[i+1].depth){
            while (pParams[i+1].depth < pMesh->meshcoord) {
                i++;
                if (i >= numpoints) break;
            }
        }
    
    }
    free(pParams);
    
    return 0;
}
