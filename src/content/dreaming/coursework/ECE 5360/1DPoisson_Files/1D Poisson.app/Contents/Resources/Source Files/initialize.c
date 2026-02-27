#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "1Dpoi_type.h"
#include "1Dpoi_def.h"


/**************************************************/

int initialize(struct global_type *pGlobals, struct flag_type *pFlags) {

/**************************************************/

/*  This routine initializes the constants used in the program and finds an  
    initial solution of the poisson equation which is used to start the  
    Newton-Raphson interation */

	double yincrement,tem;
	int firstmesh,k,j,i,numMeshPts,ioerr_loc;
	char message[MAX_MESSAGE],simfamily[MAX_SEMINAME];
	struct layer_type *pLayer, *pOhmicLayer;
	struct dummy_layer_type *pDefaultLayer;
	struct mesh_type *pMesh;
    struct contact_type *pCurrentContact;


    /* initialize constants */
	pGlobals->kT = 8.61733e-5*pGlobals->temperature;
	pGlobals->y=0;  
    	
  
/*  The physical parameters of the structure at each point are held in storage  
    vectors, so that a calculation involving the point i will use the information  
    stored in location i of the storage vector.  For example, the conduction  
    band at the point i is given by v(i) (the solution of the poisson eq.)  
    plus offset(i) (the quasipotential at that point).  */ 

	k=-1;
	pGlobals->y=0.0;
	firstmesh = true;
	pGlobals->pTopMesh = (struct mesh_type *) malloc(sizeof(struct mesh_type));	/* allocate first mesh point */
	pDefaultLayer = (struct dummy_layer_type *) malloc(sizeof(struct dummy_layer_type)); /* allocate default layer information */
	pGlobals->pTopMesh->pUpMesh = NULL;
	pGlobals->pTopMesh->pDownMesh = NULL;
	pMesh = pGlobals->pTopMesh;	/* initialize mesh pointer */
	pLayer = pGlobals->pTopLayer;	/* initialize layer pointer */
	pFlags->firstlayer = true;
	pGlobals->numMesh = 1;  // Initialize meshpoint count
	do {
		
		pGlobals->y_loc = 0.0;
		yincrement = 1.0/((int) (pLayer->tLayer/pLayer->dyLayer+0.5e0));
			
		/* initalize the mesh points */	
		for (i=0;i<((int) (pLayer->tLayer/pLayer->dyLayer+0.5e0)); i++) {	
			if (!firstmesh) {	
				pMesh->pDownMesh = (struct mesh_type *) malloc(sizeof(struct mesh_type));
				if (pMesh->pDownMesh == NULL) {
					strcpy(message,"Can't allocate memory for mesh array.  Increase Memory allocation.");
					if (!pFlags->silent) alertbox(message);
					pFlags->stopthemusic = true;
					return 1;
				}
				pMesh->pDownMesh->pDownMesh = NULL;
				pMesh->pDownMesh->pUpMesh = pMesh;
				pMesh = pMesh->pDownMesh;
                pGlobals->numMesh++;
			}
			else
				firstmesh = false;
			
/* Now assign values to the parameters at this mesh point.  The basic plan is to first calculate the 
semiconductor composition, then retrieve data from the materials file.  If a parameter has been overridden
in the input file, this value is used and the data from the materials file is ignored, for that parameter only. */
			
			pGlobals->xval = 999.0; /* default value for unused xval */
			pGlobals->yval = 999.0; /* default value for unused yval */
			pGlobals->zval = 999.0; /* default value for unused zval */
			pGlobals->wval = 999.0; /* default value for unused wval */
			if (pLayer->xvalLayer[0] != '\0') pGlobals->xval = evaluate(pLayer->xvalLayer, pGlobals, pFlags);
			if (pLayer->yvalLayer[0] != '\0') pGlobals->yval = evaluate(pLayer->yvalLayer, pGlobals, pFlags);
			if (pLayer->zvalLayer[0] != '\0') pGlobals->zval = evaluate(pLayer->zvalLayer, pGlobals, pFlags);
			if (pLayer->wvalLayer[0] != '\0') pGlobals->wval = evaluate(pLayer->wvalLayer, pGlobals, pFlags);
			ioerr_loc = semisearch(pLayer->nameLayer,pGlobals->xval,pGlobals->yval,pGlobals->zval,pGlobals->wval,
									pDefaultLayer,simfamily,pGlobals,pFlags);	/* get information from materials file*/
		    if (pFlags->badversion) return 1;
		    
		    ioerr_loc = meshInit(pMesh,pLayer,pDefaultLayer,pGlobals, pFlags); /* load up most of the mesh point parameters */
		    
			pMesh->meshcoord = pGlobals->y;		/* init real space coordinate */
			pGlobals->y = pGlobals->y + pLayer->dyLayer;
	    	k= k+1;
	    	pGlobals->y_loc = pGlobals->y_loc + yincrement;
		}
		if (pLayer->sheet < 0.0) 
			pMesh->Na = pMesh->Na - pLayer->sheet/pMesh->dy*1e8;
		else
			pMesh->Nd = pMesh->Nd + pLayer->sheet/pMesh->dy*1e8;
				
	} while((pLayer=pLayer->pDownLayer) != NULL);
	
/* Add another mesh point to be the bottom mesh */
	pLayer = pMesh->pUpMesh->pLayer;	
	pMesh->pDownMesh = (struct mesh_type *) malloc(sizeof(struct mesh_type));
    pGlobals->numMesh++;
	if (pMesh->pDownMesh == NULL) {
		strcpy(message,"Can't allocate memory for mesh array.  Increase Memory allocation.");
		if (!pFlags->silent) alertbox(message);
		pFlags->stopthemusic = true;
		return 1;
	}
	pMesh->pDownMesh->pDownMesh = NULL;
	pMesh->pDownMesh->pUpMesh = pMesh;
	pMesh = pMesh->pDownMesh;
		    
	ioerr_loc = meshInit(pMesh,pLayer,pDefaultLayer,pGlobals, pFlags); /* load up most of the mesh point parameters */
				
	pMesh->meshcoord = pGlobals->y;		/* init real space coordinate */
	pGlobals->y = pGlobals->y + pLayer->dyLayer;
	k= k+1;
	
/* Mesh initializiation done */

	pGlobals->pBottomMesh = pMesh;
	numMeshPts = k; /* Save the number of mesh points in the structure */

	/* Add in implanted dopants.  Add these to dopants already in the structure. */
	if (pFlags->implant) {
		for (j=0; j<pGlobals->impnumber; j++) {
			if (pGlobals->imptype[j] == 'd') {
				pMesh = pGlobals->pTopMesh;
				do {
					pMesh->Nd = pMesh->Nd + pGlobals->dose[j]*exp(-0.5e0*(pow((pMesh->meshcoord - pGlobals->Rp[j]),2)/(pGlobals->dRp[j]*pGlobals->dRp[j])));
				} while((pMesh=pMesh->pDownMesh) != NULL);
			}
			else if (pGlobals->imptype[j] == 'a') {
				pMesh = pGlobals->pTopMesh;
				do {
					pMesh->Na = pMesh->Na + pGlobals->dose[j]*exp(-0.5e0*(pow((pMesh->meshcoord - pGlobals->Rp[j]),2)/(pGlobals->dRp[j]*pGlobals->dRp[j])));
				} while((pMesh=pMesh->pDownMesh) != NULL);
			}
		}
	}

	if (pFlags->schstartflag && pFlags->schstopflag) {

/*  if both the pFlags->schrodinger start and stop points have been defined,  
     convert start and stop to mesh pointers, and set the pFlags->schrodinger flag */

		pGlobals->pSchStart = ymesh(pGlobals->realschstart,pGlobals);
		pGlobals->pSchStop = ymesh(pGlobals->realschstop,pGlobals);

		/* Find the pGlobals->order of the Schrodinger matrices*/
		pGlobals->order =1;
		pMesh = pGlobals->pSchStart;
		while ((pMesh = pMesh->pDownMesh) != pGlobals->pSchStop) pGlobals->order = pGlobals->order +1;
		pGlobals->order = pGlobals->order + 1;

		if (pGlobals->order > 0) { 
			pFlags->schrodinger = true;
			pGlobals->pPotential = (double *) malloc(pGlobals->order*sizeof(double));
			if (pGlobals->pPotential == NULL) {
				strcpy(message,"Can't allocate memory for Schrodinger array.  Increase Memory allocation.");
				if (!pFlags->silent) alertbox(message);
				pFlags->stopthemusic = true;
				return 1;
			}
			pGlobals->pMass = (double *) malloc(pGlobals->order*sizeof(double));
			if (pGlobals->pMass == NULL) {
				strcpy(message,"Can't allocate memory for Schrodinger array.  Increase Memory allocation.");
				if (!pFlags->silent) alertbox(message);
				pFlags->stopthemusic = true;
				return 1;
			}
			pGlobals->pSdy = (double *) malloc(pGlobals->order*sizeof(double));
			if (pGlobals->pSdy == NULL) {
				strcpy(message,"Can't allocate memory for Schrodinger array.  Increase Memory allocation.");
				if (!pFlags->silent) alertbox(message);
				pFlags->stopthemusic = true;
				return 1;
			}
			pGlobals->pEval = (double *) malloc(pGlobals->order*sizeof(double));
			if (pGlobals->pEval == NULL) {
				strcpy(message,"Can't allocate memory for Schrodinger array.  Increase Memory allocation.");
				if (!pFlags->silent) alertbox(message);
				pFlags->stopthemusic = true;
				return 1;
			}
			pGlobals->pEvcharge = (double *) malloc(pGlobals->order*sizeof(double));
			if (pGlobals->pEvcharge == NULL) {
				strcpy(message,"Can't allocate memory for Schrodinger array.  Increase Memory allocation.");
				if (!pFlags->silent) alertbox(message);
				pFlags->stopthemusic = true;
				return 1;
			}
			pGlobals->pEvderiv = (double *) malloc(pGlobals->order*sizeof(double));
			if (pGlobals->pEvderiv == NULL) {
				strcpy(message,"Can't allocate memory for Schrodinger array.  Increase Memory allocation.");
				if (!pFlags->silent) alertbox(message);
				pFlags->stopthemusic = true;
				return 1;
			}
			pFlags->schMemAlloc = true; /* Schrodinger memory has been allocated */
		}
		else {
			strcpy(message,"Schrodinger interval must be at least one mesh point.");
			if (!pFlags->silent) alertbox(message);
			pFlags->stopthemusic = true;
			return 1;
		}
	
		/* Check for a flat Fermi Level in the Schrodinger region */
		tem = 1.e-5*((int)(1.e5*pGlobals->pSchStart->Efn+0.5e0));
		pMesh = pGlobals->pSchStart;
		while ((pMesh = pMesh->pDownMesh) != pGlobals->pSchStop) {
			if (1.e-5*((int) (1.e5*pMesh->Efn+0.5e0)) != tem) {
				//strcpy(message,"The Fermi level must be constant in the Schrodinger region");
				//if (!pFlags->silent) alertbox(message);
				//pFlags->stopthemusic = true;
				//return 1;
			}
		}
	}

//Process contacts
    ioerr_loc = processContacts(pGlobals,pFlags);
    
	if (pFlags->readParam) {
		ioerr_loc = readParamFile(pGlobals,pFlags);
		if (pFlags->stopthemusic) return 1;	/* exit from initialize if error in reading file */
	}
    
//Finish up the initialization
	pGlobals->pStartSim = pGlobals->pTopMesh; //Set the simulation start to the top of the structure
	pFlags->firstcv = true;	// set first iteration flag for CV runs
	pFlags->firstetch = true;	// set first iteration flag for etch runs
	//cvcount = 1;			// set default cvcount
    if (pFlags->multiruns) {
        pCurrentContact = pGlobals->pFirstContact;
        while (pCurrentContact != NULL) {
            if (pCurrentContact->pVoltageSource != NULL) {
                if (pCurrentContact->pVoltageSource->multiRun) {
                    pGlobals->multiRun.pContact = pCurrentContact;
                }
            }
            pCurrentContact = pCurrentContact->pNextContact;
        }
		pGlobals->multiRun.multiCount = 0;
		pGlobals->multiRun.multiCountMax  = (int) (fabs((pGlobals->multiRun.pContact->pVoltageSource->vstop-pGlobals->multiRun.pContact->pVoltageSource->vbias)/pGlobals->multiRun.pContact->pVoltageSource->vincrement) + 0.5e0) +1;
		pGlobals->CVFile = (char *) malloc((pGlobals->multiRun.multiCountMax*75+80)*sizeof(char));
        strcpy(pGlobals->CVFile,"V\tC (Fcm-2)\tInitcharge (cm-2)\tFinalcharge (cm-2)\tns Total (cm-2)\tps Total (cm-2)\n");
    }
	free(pDefaultLayer);
	pFlags->noinputfile = false;		// declare that there is a valid input file 
	return 0;
}


/************************************************************/
	
int meshInit (struct mesh_type *pMesh, struct layer_type *pLayer, struct dummy_layer_type *pDefaultLayer, 
				struct global_type *pGlobals, struct flag_type *pFlags) {

/************************************************************/	

	double doping,tem;
	
	pMesh->dy = pLayer->dyLayer;
	if (strcmp(pLayer->EgLayer,""))
		pMesh->Eg = evaluate(pLayer->EgLayer, pGlobals, pFlags);
	else
		pMesh->Eg = pDefaultLayer->EgLayer;
	if (strcmp(pLayer->offsetLayer,""))
		pMesh->offset = evaluate(pLayer->offsetLayer, pGlobals, pFlags);
	else
		pMesh->offset = pDefaultLayer->offsetLayer;
	if (strcmp(pLayer->eemLayer,""))
		pMesh->eem = evaluate(pLayer->eemLayer, pGlobals, pFlags);
	else
		pMesh->eem = pDefaultLayer->eemLayer;
	if (strcmp(pLayer->valleyLayer,""))
		pMesh->valley = evaluate(pLayer->valleyLayer, pGlobals, pFlags);
	else
		pMesh->valley = pDefaultLayer->valleyLayer;
	if (strcmp(pLayer->erLayer,""))
		pMesh->esemi = evaluate(pLayer->erLayer, pGlobals, pFlags)*dicon;		// initialize dielectric const. array 
	else
		pMesh->esemi = pDefaultLayer->erLayer*dicon;
	if (strcmp(pLayer->mhhLayer,""))
		pMesh->ehm = evaluate(pLayer->mhhLayer, pGlobals, pFlags);
	else
		pMesh->ehm = pDefaultLayer->mhhLayer;
	if (strcmp(pLayer->mlhLayer,""))
		pMesh->elhm = evaluate(pLayer->mlhLayer, pGlobals, pFlags);
	else
		pMesh->elhm = pDefaultLayer->mlhLayer;
	if (strcmp(pLayer->msohLayer,""))
		pMesh->esohm = evaluate(pLayer->msohLayer, pGlobals, pFlags);
	else
		pMesh->esohm = pDefaultLayer->msohLayer;
	if (strcmp(pLayer->EsoLayer,""))
		pMesh->Eso = evaluate(pLayer->EsoLayer, pGlobals, pFlags);
	else
		pMesh->Eso = pDefaultLayer->EsoLayer;
	if (strcmp(pLayer->NdLayer,""))
		pMesh->Nd = evaluate(pLayer->NdLayer, pGlobals, pFlags);
	else
		pMesh->Nd = pDefaultLayer->NdLayer;
	if (strcmp(pLayer->NaLayer,""))
		pMesh->Na = evaluate(pLayer->NaLayer, pGlobals, pFlags);
	else
		pMesh->Na = pDefaultLayer->NaLayer;
	if (strcmp(pLayer->NdsLayer,""))
		pMesh->Nds = evaluate(pLayer->NdsLayer, pGlobals, pFlags);
	else
		pMesh->Nds = pDefaultLayer->NdsLayer;
	if (strcmp(pLayer->NasLayer,""))
		pMesh->Nas = evaluate(pLayer->NasLayer, pGlobals, pFlags);
	else
		pMesh->Nas = pDefaultLayer->NasLayer;
	if (strcmp(pLayer->NddLayer,""))
		pMesh->Ndd = evaluate(pLayer->NddLayer, pGlobals, pFlags);
	else
		pMesh->Ndd = pDefaultLayer->NddLayer;
	if (strcmp(pLayer->NdaLayer,""))
		pMesh->Nda = evaluate(pLayer->NdaLayer, pGlobals, pFlags);
	else
		pMesh->Nda = pDefaultLayer->NdaLayer;
	if (strcmp(pLayer->polarizLayer,""))
		pMesh->polariz = evaluate(pLayer->polarizLayer, pGlobals, pFlags);
	else
		pMesh->polariz = pDefaultLayer->polarizLayer;
	if (pMesh->pUpMesh != NULL) 
		pMesh->polcharge = (pMesh->pUpMesh->polariz - pMesh->polariz)/(q*pMesh->dy*1.0e-8); /* Polarization charge density */
	else
		pMesh->polcharge = 0.0;
	if (strcmp(pLayer->EdLayer,""))
		pMesh->Ed = evaluate(pLayer->EdLayer, pGlobals, pFlags);
	else
		pMesh->Ed = pDefaultLayer->EdLayer;
	if (strcmp(pLayer->EaLayer,""))
		pMesh->Ea = evaluate(pLayer->EaLayer, pGlobals, pFlags);
	else
		pMesh->Ea = pDefaultLayer->EaLayer;
	if (strcmp(pLayer->EddLayer,""))
		pMesh->Edd = evaluate(pLayer->EddLayer, pGlobals, pFlags);
	else
		pMesh->Edd = pDefaultLayer->EddLayer;
	if (strcmp(pLayer->EdaLayer,""))
		pMesh->Eda = evaluate(pLayer->EdaLayer, pGlobals, pFlags);
	else
		pMesh->Eda = pDefaultLayer->EdaLayer;
	if (strcmp(pLayer->emobLayer,""))
		pMesh->emob = evaluate(pLayer->emobLayer, pGlobals, pFlags);
	else
		pMesh->emob = pDefaultLayer->emobLayer;
	if (strcmp(pLayer->hmobLayer,""))
		pMesh->hmob = evaluate(pLayer->hmobLayer, pGlobals, pFlags);
	else
		pMesh->hmob = pDefaultLayer->hmobLayer;
	if (strcmp(pLayer->taunLayer,""))
		pMesh->taun = evaluate(pLayer->taunLayer, pGlobals, pFlags);
	else
		pMesh->taun = pDefaultLayer->taunLayer;
	if (strcmp(pLayer->taupLayer,""))
		pMesh->taup = evaluate(pLayer->taupLayer, pGlobals, pFlags);
	else
		pMesh->taup = pDefaultLayer->taupLayer;
	if (strcmp(pLayer->EfLayer,""))
		pMesh->Efn = evaluate(pLayer->EfLayer, pGlobals, pFlags);
	else
		pMesh->Efn = pDefaultLayer->EfLayer;
	pMesh->Efp = pMesh->Efn;
	pMesh->pLayer = pLayer;			/* initialize layer pointer */
	pMesh->noholes = pLayer->noholesLayer;
	pMesh->noelec = pLayer->noelecLayer;
	pMesh->fullyionized = pLayer->fullyionLayer;
	pMesh->aBohrElec = 59758.87*pMesh->esemi/pMesh->eem;	/* electron Bohr radius (cm) */
	pMesh->aBohrHhole = 59758.87*pMesh->esemi/pMesh->ehm;	/* heavy hole Bohr radius (cm) */
	pMesh->aBohrLhole = 59758.87*pMesh->esemi/pMesh->elhm;	/* light hole Bohr radius (cm) */
	pMesh->aBohrHhole=0.5e0*(pMesh->aBohrHhole+pMesh->aBohrLhole);	/* for now average the hole Bohr radii */
	if (pGlobals->kT > 0.0e0) {
        if (!pLayer->mtrlType){  //If a 3D semiconductor
            pMesh->Nc = 6.038165e21*pMesh->valley*pow(pMesh->eem*pGlobals->kT,1.5);	/* conduction band eff. den. st. */
            pMesh->Nv = 6.038165e21*(pow(pMesh->ehm,1.5) + pow(pMesh->elhm,1.5))*pow(pGlobals->kT,1.5);	/* valence band eff. den. st. */
            pMesh->Nvso = 6.038165e21*pow(pMesh->ehm*pGlobals->kT,1.5);	// valence band eff. den. st.
        }
        else if (pLayer->mtrlType == twoDmtrl){ //2D parabolic semiconductor
            pMesh->Nc = 4.177756e14*pMesh->valley*pMesh->eem;	// conduction band eff. den. st. for 2D
            pMesh->Nv = 4.177756e14*pow(pow(pMesh->ehm,1.5) + pow(pMesh->elhm,1.5),0.666667);	// valence band eff. den. st. for 2D
            pMesh->Nvso = 4.177756e14*pMesh->esohm;	// Split-off band eff. den. st. for 2D
        }
        else if (pLayer->mtrlType == graphenemtrl){ //2D parabolic semiconductor
            pMesh->Nc = 1.469411e30*pGlobals->kT*pGlobals->kT/(FermiVel*FermiVel);	// conduction band eff. den. st. for graphene
            pMesh->Nv = 1.469411e30*pGlobals->kT*pGlobals->kT/(FermiVel*FermiVel);	// valence band eff. den. st. for graphene
        }
    }
    else { //allow T=0
        if (!pLayer->mtrlType){  //If a 3D semiconductor
            pMesh->Nc = 6.813341e21*pMesh->valley*pow(pMesh->eem,1.5);	/* conduction band eff. den. st. */
            pMesh->Nv = 6.813341e21*(pow(pMesh->ehm,1.5) + pow(pMesh->elhm,1.5));	/* valence band eff. den. st. */
            pMesh->Nvso = 6.813341e21*pow(pMesh->esohm,1.5);	// split-off band eff. den. st. 
        }
        else if (pLayer->mtrlType == twoDmtrl){ //2D parabolic semiconductor
            pMesh->Nc = 4.177756e14*pMesh->valley*pMesh->eem;	// conduction band eff. den. st. for 2D
            pMesh->Nv = 4.177756e14*pow(pow(pMesh->ehm,1.5) + pow(pMesh->elhm,1.5),0.666667);	// valence band eff. den. st. for 2D
            pMesh->Nvso = 4.177756e14*pMesh->esohm;	// splitk-off band eff. den. st. for 2D
        }
        else if (pLayer->mtrlType == graphenemtrl){ //graphene
            pMesh->Nc = 1.469411e30/(FermiVel*FermiVel);	// conduction band eff. den. st. for graphene
            pMesh->Nv = 1.469411e30/(FermiVel*FermiVel);	// valence band eff. den. st. for graphene
        }
    }
	// find the initial flat band condition for each mesh point
	pMesh->v = initBands(pMesh, pMesh->Nd+pMesh->Ndd, pMesh->Na+pMesh->Nda, pGlobals->kT);
    
	return 0;
}




/*************************************/
int Init_1DPoisson(struct global_type *pGlobals, struct flag_type *pFlags) {
/*************************************/
// This routine cleans up memory (if needed) before starting a new simulation
    
    int i;
	struct layer_type *pLayer;
	struct mesh_type *pMesh;
    struct contact_type *pContact; //pointer to contact list
    struct contact_type *pNextContact; //pointer to contact list
    struct voltageSource_type *pVoltage; //pointer to voltage source list
    struct voltageSource_type *pNextVoltage; //pointer to voltage source list
    
	//strcpy(pGlobals->matrlfilePath,PATH);
    //pFlags->badversion = false;
	//pFlags->noinputfile = true;
	//pFlags->schMemAlloc = false;
	//pFlags->schElecMemAlloc = false; // Start with the Schrodinger memory flag unset
	//pFlags->schHhMemAlloc = false; // Start with the Schrodinger memory flag unset
	//pFlags->schLhMemAlloc = false; // Start with the Schrodinger memory flag unset/* If another simulation has already been run, free the memory allocated for the layer and mesh arrays */
    
	//if (pGlobals->inputPath != NULL) free(pGlobals->inputPath);
	//if (pGlobals->filename != NULL) free(pGlobals->filename);
    
	//if (!pFlags->noinputfile) {
    pLayer = pGlobals->pTopLayer;
    if (pLayer != NULL){
		while (pLayer->pDownLayer != NULL) {
            pLayer = pLayer->pDownLayer;
            free(pLayer->pUpLayer);
            pLayer->pUpLayer = NULL;
		}
		free(pLayer);
        pLayer = NULL;
        pGlobals->pTopLayer = NULL;
    }
    pMesh = pGlobals->pTopMesh;
    if (pMesh != NULL){
		if (pMesh->pUpMesh !=NULL) free(pMesh->pUpMesh);
		while (pMesh->pDownMesh != NULL) {
            pMesh = pMesh->pDownMesh;
            free(pMesh->pUpMesh);
            pMesh->pUpMesh = NULL;
		}
		free(pMesh);
        pMesh = NULL;
        pGlobals->pTopMesh = NULL;
        pGlobals->pBottomMesh = NULL;
        pGlobals->pStartSim = NULL;
        pGlobals->pStartMesh = NULL;
        pGlobals->pStopMesh = NULL;
        pGlobals->pSchStart = NULL;
        pGlobals->pSchStop = NULL;
    }
    if ( pGlobals->pFirstContact != NULL) {
        pContact = pGlobals->pFirstContact;
        while (pContact != NULL) {
            pNextContact = pContact->pNextContact;
            free(pContact);
            pContact = pNextContact;
        }
        pGlobals->pFirstContact = NULL;
    }
    if ( pGlobals->pFirstVoltage != NULL) {
        pVoltage = pGlobals->pFirstVoltage;
        while (pVoltage != NULL) {
            pNextVoltage = pVoltage->pNextVoltageSource;
            free(pVoltage);
            pVoltage = pNextVoltage;
        }
        pGlobals->pFirstVoltage = NULL;
    }
    if (pGlobals->ShellFile != NULL) {
        free(pGlobals->ShellFile);
        pGlobals->ShellFile =NULL;
    }
    if (pGlobals->OutputFile != NULL) {
        free(pGlobals->OutputFile);
        pGlobals->OutputFile =NULL;
    }
    if (pGlobals->CVFile != NULL) {
        free(pGlobals->CVFile);
        pGlobals->CVFile =NULL;
    }
    if (pGlobals->StatusFile != NULL) {
        free(pGlobals->StatusFile);
        pGlobals->StatusFile =NULL;
    }
    pFlags->noinputfile = true;
	//}
	
	if (pFlags->schMemAlloc) {
		free(pGlobals->pPotential);
        pGlobals->pPotential = NULL;
        free(pGlobals->pMass);
        pGlobals->pMass = NULL;
        free(pGlobals->pSdy);
        pGlobals->pSdy = NULL;
        free(pGlobals->pEval);
        pGlobals->pEval = NULL;
        free(pGlobals->pEvcharge);
        pGlobals->pEvcharge = NULL;
        free(pGlobals->pEvderiv);
        pGlobals->pEvderiv = NULL;
		pFlags->schMemAlloc = false;
	}
	
	if (pFlags->schElecMemAlloc) {
		for (i=0; i<pGlobals->order; i++)
			free(pGlobals->pElecevec[i]);
		free(pGlobals->pElecevec);
		free(pGlobals->pEleceval);
		pFlags->schElecMemAlloc = false;
	}
	if (pFlags->schHhMemAlloc) {
		for (i=0; i<pGlobals->order; i++)
			free(pGlobals->pHhevec[i]);
		free(pGlobals->pHhevec);
		free(pGlobals->pHheval);
		pFlags->schHhMemAlloc = false;
	}
	if (pFlags->schLhMemAlloc) {
		for (i=0; i<pGlobals->order; i++)
			free(pGlobals->pLhevec[i]);
		free(pGlobals->pLhevec);
		free(pGlobals->pLheval);
		pFlags->schLhMemAlloc = false;
	}
	if (pFlags->schSohMemAlloc) {
		for (i=0; i<pGlobals->order; i++)
			free(pGlobals->pSohevec[i]);
		free(pGlobals->pSohevec);
		free(pGlobals->pSoheval);
		pFlags->schSohMemAlloc = false;
	}
    
    
    //if (pGlobals->filename != NULL) free(pGlobals->filename);
    
    
    return 0;
}

/**************************************************/
int processContacts(struct global_type *pGlobals, struct flag_type *pFlags) {
/**************************************************/

    char message[MAX_MESSAGE];
    struct contact_type *pCurrentContact;
    struct voltageSource_type *pCurrentVoltageSource;
    struct mesh_type *pMesh;
    struct layer_type *pOhmicLayer;
    
    pCurrentContact = pGlobals->pFirstContact;
    while (pCurrentContact != NULL) {
        if (pCurrentContact->voltageSourceNumber != NoVoltageSource) { // If a voltage source is connected to this contact, attach the source pointer.
            pCurrentVoltageSource = pGlobals->pFirstVoltage;
            while (pCurrentVoltageSource != NULL) {
                if (pCurrentVoltageSource->sourceNumber == pCurrentContact->voltageSourceNumber)
                    pCurrentContact->pVoltageSource = pCurrentVoltageSource;
                pCurrentVoltageSource = pCurrentVoltageSource->pNextVoltageSource;
            }
        }
        if (pCurrentContact->contactNumber == SurfContact) {
            pCurrentContact->pMeshPosition = pGlobals->pTopMesh;
            pGlobals->pTopMesh->pContact = pCurrentContact;
            if (pCurrentContact->contactType == CHARGE) {
                pCurrentContact->contactParameter = pCurrentContact->contactParameter/(dicon*pGlobals->pTopMesh->esemi);//If charge BC was used, set the value in contact parameter to the equiv field
                pCurrentContact->contactType = SLOPE;
            }
            if (pCurrentContact->contactType == SLOPE) {
                // create the dummy mesh point used to set the slope
                pGlobals->pTopMesh->pUpMesh = (struct mesh_type *) malloc(sizeof(struct mesh_type));
                if (pGlobals->pTopMesh->pUpMesh == NULL) {
                    strcpy(message,"Can't allocate memory for mesh array.  Increase Memory allocation.");
                    if (!pFlags->silent) alertbox(message);
                    pFlags->stopthemusic = true;
                    return 1;
                }
                pGlobals->pTopMesh->pUpMesh->pUpMesh = NULL;
                pGlobals->pTopMesh->pUpMesh->pDownMesh = pGlobals->pTopMesh;
                pGlobals->pTopMesh->pUpMesh->dy = pGlobals->pTopMesh->dy;
                pGlobals->pTopMesh->pUpMesh->esemi = pGlobals->pTopMesh->esemi;
                pGlobals->pStartMesh = pGlobals->pTopMesh; // For slope BC, set the surf mesh
            }
            else {
                pGlobals->pStartMesh = pGlobals->pTopMesh->pDownMesh; // For schottky and ohmic BCs, don't calc the surface mesh
            }
        }
        else if (pCurrentContact->contactNumber == SubContact) {
            pCurrentContact->pMeshPosition = pGlobals->pBottomMesh;
            pGlobals->pBottomMesh->pContact = pCurrentContact;
            if (pCurrentContact->contactType == CHARGE) {
                pCurrentContact->contactParameter = pCurrentContact->contactParameter/(dicon*pGlobals->pBottomMesh->esemi);//If charge BC was used, set the value in contact parameter to the equiv field
                pCurrentContact->contactType = SLOPE;
            }
            if (pCurrentContact->contactType == SLOPE) {
                // create the dummy mesh point used to set the slope
                pGlobals->pBottomMesh->pDownMesh = (struct mesh_type *) malloc(sizeof(struct mesh_type));
                if (pGlobals->pBottomMesh->pDownMesh == NULL) {
                    strcpy(message,"Can't allocate memory for mesh array.  Increase Memory allocation.");
                    if (!pFlags->silent) alertbox(message);
                    pFlags->stopthemusic = true;
                    return 1;
                }
                pGlobals->pBottomMesh->pDownMesh->pDownMesh = NULL;
                pGlobals->pBottomMesh->pDownMesh->pUpMesh = pGlobals->pBottomMesh;
                pGlobals->pBottomMesh->pDownMesh->dy = pGlobals->pBottomMesh->dy;
                pGlobals->pBottomMesh->pDownMesh->esemi = pGlobals->pBottomMesh->esemi;
                pGlobals->pStopMesh = pGlobals->pBottomMesh; // For slope BC, set the substrate mesh
            }
            else {
                pGlobals->pStopMesh = pGlobals->pBottomMesh->pUpMesh; // For schottky and ohmic BCs, don't calc the substrate mesh
            }
        }
        else {
            pCurrentContact->pMeshPosition = ymesh(pCurrentContact->realPosition,pGlobals);
            pCurrentContact->pMeshPosition->pContact = pCurrentContact;
            
        }
        
        if (pCurrentContact->contactType == SCHOTTKY) {
            pCurrentContact->pMeshPosition->v = pCurrentContact->contactParameter - pCurrentContact->pVoltageSource->vbias - pCurrentContact->pMeshPosition->offset; //Schottky voltage
            pCurrentContact->pMeshPosition->Efn = -pCurrentContact->pVoltageSource->vbias;
            pCurrentContact->pMeshPosition->Efp = -pCurrentContact->pVoltageSource->vbias;
            mobilecharge(pCurrentContact->pMeshPosition,pGlobals->kT);	// Find the mobile and fixed charge for the point that's not calculated*/
            fixedcharge(pCurrentContact->pMeshPosition,pGlobals->kT);
        }
        else if (pCurrentContact->contactType == OHMIC) {
            pMesh = pCurrentContact->pMeshPosition;
            pCurrentContact->contactParameter =pMesh->v; //Store the v of the contact point, for use later
            pMesh->v = pMesh->v - pCurrentContact->pVoltageSource->vbias;			// ohmic contact
            pMesh->Efn = - pCurrentContact->pVoltageSource->vbias;
            pMesh->Efp = - pCurrentContact->pVoltageSource->vbias;
            mobilecharge(pMesh,pGlobals->kT);	// Find the mobile and fixed charge for the point that's not calculated*/
            fixedcharge(pMesh,pGlobals->kT);
            pOhmicLayer = pMesh->pLayer;
            
            while (pMesh->pLayer == pOhmicLayer  && pMesh->pDownMesh != NULL){  // Set the Fermi level of the adjacent layer
                pMesh->Efn = - pCurrentContact->pVoltageSource->vbias;
                pMesh->Efp = - pCurrentContact->pVoltageSource->vbias;
                pMesh = pMesh->pDownMesh;
            }
            while (pMesh->pLayer == pOhmicLayer  && pMesh->pUpMesh != NULL){  // Set the Fermi level of the adjacent layer
                pMesh->Efn = - pCurrentContact->pVoltageSource->vbias;
                pMesh->Efp = - pCurrentContact->pVoltageSource->vbias;
                pMesh = pMesh->pUpMesh;
            }
        }
        pCurrentContact = pCurrentContact->pNextContact;
        
    }
    return 0;
}
