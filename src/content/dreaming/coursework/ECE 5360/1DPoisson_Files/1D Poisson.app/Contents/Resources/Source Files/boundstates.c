#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "1Dpoi_type.h"
#include "1Dpoi_def.h" 


/**************************************************/
void bndStDefaults(struct global_type *pGlobals, struct flag_type *pFlags) {
/**************************************************/
    
	struct mesh_type *pMesh;
    int i;
    
	pMesh = pGlobals->pSchStart;
	pGlobals->eLB = pMesh->v+pMesh->offset;
	pGlobals->eUB = pMesh->v+pMesh->offset;
    pGlobals->hhLB = -(pMesh->v+pMesh->offset-pMesh->Eg);
    pGlobals->hhUB = -(pMesh->v+pMesh->offset-pMesh->Eg);
    pGlobals->sohLB = -(pMesh->v+pMesh->offset-pMesh->Eg-pMesh->Eso);
    pGlobals->sohUB = -(pMesh->v+pMesh->offset-pMesh->Eg-pMesh->Eso);
	
	for(i=1; i<pGlobals->order; i++) {
		if (pMesh->v+pMesh->offset < pGlobals->eLB) 	// look for lowest point in conduction band
			pGlobals->eLB = pMesh->v+pMesh->offset;
		if (-(pMesh->v+pMesh->offset-pMesh->Eg) < pGlobals->hhLB) 	// look for lowest point in valence band (flipped over)
			pGlobals->hhLB = -(pMesh->v+pMesh->offset-pMesh->Eg);
		if (-(pMesh->v+pMesh->offset-pMesh->Eg-pMesh->Eso) < pGlobals->sohLB) 	// look for lowest point in split-off band (flipped over)
			pGlobals->sohLB = -(pMesh->v+pMesh->offset-pMesh->Eg-pMesh->Eso);
		pMesh = pMesh->pDownMesh;
	}
    
	pMesh = pGlobals->pSchStop;
	if (pMesh->v+pMesh->offset < pGlobals->eUB) pGlobals->eUB = pMesh->v+pMesh->offset;	// find least upper bound (check each end)
	if (-(pMesh->v+pMesh->offset-pMesh->Eg) < pGlobals->hhUB) pGlobals->hhUB = -(pMesh->v+pMesh->offset-pMesh->Eg);	// find least upper bound (check each end)
    if (-(pMesh->v+pMesh->offset-pMesh->Eg-pMesh->Eso) < pGlobals->sohUB) pGlobals->sohUB = -(pMesh->v+pMesh->offset-pMesh->Eg-pMesh->Eso);	// find least upper bound (check each end)
    pGlobals->lhUB = pGlobals->hhUB;  //For default set LH bounds equal to HH bounds
    pGlobals->lhLB = pGlobals->hhLB;  //For default set LH bounds equal to HH bounds
    if (pGlobals->BndStBounds.eBoundsSet){  //Check for user defined bounds
        pGlobals->eUB = pGlobals->BndStBounds.eUB;
        pGlobals->eLB = pGlobals->BndStBounds.eLB;
    }
    if (pGlobals->BndStBounds.hhBoundsSet){  //Check for user defined bounds
        pGlobals->hhUB = pGlobals->BndStBounds.hhUB;
        pGlobals->hhLB = pGlobals->BndStBounds.hhLB;
    }
    if (pGlobals->BndStBounds.lhBoundsSet){  //Check for user defined bounds
        pGlobals->lhUB = pGlobals->BndStBounds.lhUB;
        pGlobals->lhLB = pGlobals->BndStBounds.lhLB;
    }
    if (pGlobals->BndStBounds.sohBoundsSet){  //Check for user defined bounds
        pGlobals->sohUB = pGlobals->BndStBounds.sohUB;
        pGlobals->sohLB = pGlobals->BndStBounds.sohLB;
    }
}
    
/**************************************************/
int boundstates(struct global_type *pGlobals, struct flag_type *pFlags) {
/**************************************************/
        
  
/* this subroutine finds the electron and hole bound states of a previously calculated structure */
	
	char ans,dummy,statesname[MAX_FILENAME],charDummy[MAX_EXTENSION],extension[MAX_EXTENSION],access[3],
		info[MAX_INPUTLINE],message[MAX_MESSAGE];
	int ioerr_loc,fieldlength,fixBounds,i,j,count,startsearch;
	double nthird,pthird,*pOldn,*pOldderiv,tem;
	struct mesh_type *pMesh;
	
	pFlags->schrodinger = true; // set the pFlags->schrodinger flag so that the proper output files are written
	// Find the pGlobals->order of the Schrodinger matrices
	pGlobals->order =1;
	pMesh = pGlobals->pSchStart;
	while ((pMesh = pMesh->pDownMesh) != pGlobals->pSchStop) 
		pGlobals->order = pGlobals->order +1;
	pGlobals->order = pGlobals->order + 1;

	if (pGlobals->order > 0 && !pFlags->schMemAlloc) { 
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
	}
	else if (pGlobals->order <= 0) {
		strcpy(message,"Schrodinger interval must be at least one mesh point.");
		if (!pFlags->silent) alertbox(message);
		pFlags->stopthemusic = true;
		return 1;
	}
	
	// Check for a flat Fermi Level in the Schrodinger region 
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
    
    schrodingersolve(pGlobals,pFlags,FixBounds);  // Find the mobile charge in the Schrodiger mesh

	
	if (!pFlags->schrodinger) {
		pFlags->schrodinger = true;
	}
	
	//count = 0;
	//strcpy(extension,"_St");
	//if (!pFlags->multiruns) strcpy(statesname,pGlobals->filename);
	ioerr_loc = output(pGlobals,pFlags);	/* output the bound states data */
	
	
	strcpy(message,"Calculation Complete");
	if (!pFlags->silent) alertbox(message);
	
	return 0;
}
	