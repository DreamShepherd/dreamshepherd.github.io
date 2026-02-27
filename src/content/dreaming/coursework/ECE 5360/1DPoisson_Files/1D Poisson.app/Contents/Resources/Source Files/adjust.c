#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "1Dpoi_type.h"
#include "1Dpoi_def.h" 
	
/***********************************************/

void adjust(struct global_type *pGlobals, struct flag_type *pFlags) {

/***********************************************/


/*  This subroutine adjusts the voltage vector according to Newton's
/*  method, see Hildebrand, Introduction to Numerical Methods p 583.  
/*  Delta v is found using Crouts method to solve the jacobian matrix (which
/*  fortunately is a tridiagonal matrix) equation.  The maximum change in
/*  the voltage vector is returned as the convergence parameter. */

/*	double wherey;
	char outdata[100],extension2[30],access;
 int ioerr_loc;  // variables for trouble shooting */

	double deltav,esemiloc,esemiloc1, field, fieldsq;
    double deDphi, deDphiUp;
	struct mesh_type *pMesh, *wherey;
    int ioerr_loc;
	char extension2[MAX_EXTENSION],access[3],message[MAX_MESSAGE];

/*  Find the current error and form the jacobian matrix (in Crout form).  The
  Jacobian is the partial derivative of the poisson equation with respect to
  each of the pMesh->v.
  Deriv is the partial derivative of the charge concentration with respect
  to pMesh->v (calculated in subroutine charge), and is included only on the
  main diagonal of the jacobian. */


	pMesh = pGlobals->pStartMesh;
    
    if (!strcmp(pMesh->pLayer->nameLayer,"SrTiO3")) {
        field=(pMesh->pDownMesh->v-pMesh->v)*1.e8/pMesh->pDownMesh->dy;
        fieldsq = field*field;
        deDphi = dicon*pGlobals->epsilonB*fieldsq/(pow((pGlobals->epsilonA+fieldsq),1.5));
    }
    else {
        deDphi = 0.0;
    }

//	 determine surface boundary condition 

	if (pGlobals->pTopMesh->pContact->contactType == SLOPE) {
		pMesh->pUpMesh->v = pMesh->pDownMesh->v - 2.0*pGlobals->pStartMesh->pContact->contactParameter*pMesh->dy*1e-8; //use the field stored in contact parameter
		pMesh->f = 2.0*invh*(pMesh->esemi-deDphi)/(pMesh->dy * pMesh->dy);   // superdiagonal
	}		
	else
		pMesh->f = invh*(pMesh->esemi-deDphi)/(pMesh->dy * pMesh->dy);   // superdiagonal
	
	pMesh->c = -1.0*(invh*2.0e0/(pMesh->dy + pMesh->pUpMesh->dy)
		*(pMesh->esemi*(pMesh->pDownMesh->v - pMesh->v)/pMesh->dy
		-pMesh->pUpMesh->esemi*(pMesh->v - pMesh->pUpMesh->v)/pMesh->pUpMesh->dy)
		-q*(pMesh->fcharge + pMesh->p - pMesh->n));				// poisson error term 
	pMesh->d = -invh*2.0e0/(pMesh->dy + pMesh->pUpMesh->dy)
		*((pMesh->esemi-deDphi)/pMesh->dy + (pMesh->pUpMesh->esemi-0.0)
		/pMesh->pUpMesh->dy)-q*pMesh->deriv;					// main diagonal 

	pMesh->e = 0.0e0;   // subdiagonal

	pGlobals->poierr = pMesh->c;						// find max poisson error
    wherey =pMesh; //troubleshoot

	pMesh = pMesh->pDownMesh;
    deDphiUp = 0.0;
    
    do {
        
        if (!strcmp(pMesh->pLayer->nameLayer,"SrTiO3")) {
            if(!strcmp(pMesh->pUpMesh->pLayer->nameLayer,"SrTiO3"))
                deDphiUp = deDphi;
            else
                deDphiUp = 0.0;
            field=(pMesh->pDownMesh->v-pMesh->v)*1.e8/pMesh->pDownMesh->dy;
            fieldsq = field*field;
            deDphi = dicon*pGlobals->epsilonB*fieldsq/(pow((pGlobals->epsilonA+fieldsq),1.5));
        }
        else {
            if(!strcmp(pMesh->pUpMesh->pLayer->nameLayer,"SrTiO3")){
                deDphiUp = deDphi;
                deDphi=0.0;
            }
            else {
                deDphiUp = 0.0;
                deDphi = 0.0;
            }
        }
        
		pMesh->c = -1.0*(invh*2.0e0/(pMesh->dy+pMesh->pUpMesh->dy)
            *(pMesh->esemi*(pMesh->pDownMesh->v - pMesh->v)/pMesh->dy
            -pMesh->pUpMesh->esemi*(pMesh->v - pMesh->pUpMesh->v)/pMesh->pUpMesh->dy)
            -q*(pMesh->fcharge + pMesh->p - pMesh->n));				// poisson error term
        
		/*pMesh->e = invh*2.0e0/(pMesh->dy + pMesh->pUpMesh->dy)
        *pMesh->pUpMesh->esemi/pMesh->pUpMesh->dy;				// subdiagonal*/
        
		pMesh->e = invh*2.0e0/(pMesh->dy + pMesh->pUpMesh->dy)
        *(pMesh->pUpMesh->esemi-deDphiUp)/pMesh->pUpMesh->dy;				// subdiagonal
        
		/*pMesh->d = -invh*2.0e0/(pMesh->dy+pMesh->pUpMesh->dy)
        *(pMesh->esemi/pMesh->dy+pMesh->pUpMesh->esemi
          /pMesh->pUpMesh->dy)-q*pMesh->deriv;					// main diagonal*/
        
		pMesh->d = -invh*2.0e0/(pMesh->dy+pMesh->pUpMesh->dy)
        *((pMesh->esemi-deDphi)/pMesh->dy+(pMesh->pUpMesh->esemi-deDphiUp)
          /pMesh->pUpMesh->dy)-q*pMesh->deriv;					// main diagonal
        
		/*pMesh->f = invh*2.0e0/(pMesh->dy+pMesh->pUpMesh->dy)
        *pMesh->esemi/pMesh->dy;						// superdiagonal*/
        
		pMesh->f = invh*2.0e0/(pMesh->dy+pMesh->pUpMesh->dy)
        *(pMesh->esemi-deDphi)/pMesh->dy;						// superdiagonal

		if (fabs(pMesh->c) > fabs(pGlobals->poierr)) {		// find max poisson error
			pGlobals->poierr = pMesh->c;
            wherey =pMesh; //troubleshoot
        }

	} while((pMesh = pMesh->pDownMesh) != pGlobals->pStopMesh);

//	 determine substrate boundary condition 

	pMesh = pGlobals->pStopMesh;
	if (pGlobals->pBottomMesh->pContact->contactType == SLOPE) {
		pMesh->pDownMesh->v = pMesh->pUpMesh->v + 2.0*pGlobals->pStopMesh->pContact->contactParameter*pMesh->dy*1e-8;
		pMesh->e = 2.0e0*invh*pMesh->pUpMesh->esemi
			*2.0e0/(pMesh->pUpMesh->dy*(pMesh->dy + pMesh->pUpMesh->dy));	// subdiagonal 
	}
	else
		pMesh->e = invh*pMesh->pUpMesh->esemi
			*2.0e0/(pMesh->pUpMesh->dy*(pMesh->dy+pMesh->pUpMesh->dy));	// subdiagonal 


	pMesh->c = -1.0*(invh*2.0e0/(pMesh->dy+pMesh->pUpMesh->dy)
		*(pMesh->esemi*(pMesh->pDownMesh->v-pMesh->v)/pMesh->dy
		-pMesh->pUpMesh->esemi*(pMesh->v-pMesh->pUpMesh->v)/pMesh->pUpMesh->dy)
		-q*(pMesh->fcharge + pMesh->p - pMesh->n));				// poisson error term 

	pMesh->d = -invh*2.0e0/(pMesh->dy+pMesh->pUpMesh->dy)
		*(pMesh->esemi/pMesh->dy+pMesh->pUpMesh->esemi
		/pMesh->pUpMesh->dy)-q*pMesh->deriv;						// main diagonal 

	pMesh->f = 0.0e0;

	if (fabs(pMesh->c) > fabs(pGlobals->poierr)) { 
		pGlobals->poierr = pMesh->c;
        wherey =pMesh; //troubleshoot
	}
	
//  Trouble shooting code 

	 /*strcpy(access,"w");
	 strcpy(extension2,"_helpadjust");
	 ioerr_loc = fileopen(4, pGlobals->filename, access, extension2, pGlobals);
	 pMesh = pGlobals->pStartMesh;
    do {
        field=(pMesh->pDownMesh->v-pMesh->v)*1.e8/pMesh->pDownMesh->dy;
        fieldsq = field*field;
		fprintf(pGlobals->pFile[4],"%11.4e\t%11.4e\t%11.4e\t%11.4e\t%11.4e\t%11.4e\t%11.4e\t%11.4e\t%11.4e\t%11.4e\t%11.4e\t%11.4e\t%11.4e\n",
			pMesh->meshcoord,pMesh->v+pMesh->offset, pMesh->e,
                pMesh->d,pMesh->f,pMesh->c,field,pMesh->fcharge,
                dicon*pGlobals->epsilonB*fieldsq/(pow((pGlobals->epsilonA+fieldsq),1.5)),pMesh->deriv,pMesh->p,pMesh->n,pMesh->esemi/dicon);
	 } while((pMesh = pMesh->pDownMesh) != pGlobals->pStopMesh);
	 fileclose(4, pGlobals);*/
//   End troubleshooting code

//  find the correction terms deltav using Crout's reduction 

//  Form the Crout auxiliary matrix. 

	pMesh = pGlobals->pStartMesh;
	pMesh->f = pMesh->f/pMesh->d;
	pMesh->c = pMesh->c/pMesh->d;

	pMesh=pMesh->pDownMesh;
	do {
		pMesh->d = pMesh->d - pMesh->e*pMesh->pUpMesh->f;
		pMesh->f = pMesh->f/pMesh->d;
		pMesh->c = (pMesh->c-pMesh->e*pMesh->pUpMesh->c)/pMesh->d;
	} while((pMesh=pMesh->pDownMesh) != pGlobals->pStopMesh->pDownMesh);

//  Trouble shooting code

/*	access='w';
	strcpy(extension2,"");
	ioerr_loc = fileopen(4, "help1", access, extension2, pGlobals);
	pMesh = pGlobals->pStartMesh;
	do {
	sprintf(outdata, "%11.4e\t%11.4e\t%11.4e\t%11.4e\t%11.4e\t%11.4e\n", 
			pMesh->meshcoord,pMesh->v+pMesh->offset, pMesh->e,
			pMesh->d,pMesh->f,pMesh->c);
	fprintf(pGlobals->pFile[4],outdata);
	} while((pMesh = pMesh->pDownMesh) != pGlobals->pStopMesh);
	fileclose(4, pGlobals);*/

// find new solution of the voltage v.

	pMesh = pGlobals->pStopMesh;
    wherey = pMesh;
	deltav = pMesh->c;
	pMesh->v = pMesh->v + deltav;
	pGlobals->ConvgError = fabs(deltav);

	pMesh = pMesh->pUpMesh;
	do {
		deltav = pMesh->c - pMesh->f*deltav;
		pMesh->v = pMesh->v + deltav;
		if (fabs(deltav) > pGlobals->ConvgError) {
            pGlobals->ConvgError = fabs(deltav);
            wherey = pMesh;
		}
        if (!strcmp(pMesh->pLayer->nameLayer,"SrTiO3")) {
            if (pGlobals->pStartMesh->pContact->contactType != SLOPE && pMesh == pGlobals->pStartMesh) {  //If not a slope BC find epsilon for top mesh
                field=(pMesh->v-pMesh->pUpMesh->v)*1.e8/pMesh->dy;
                pMesh->pUpMesh->esemi = dicon*pGlobals->epsilonB/sqrt((pGlobals->epsilonA+field*field));
            }
            field=(pMesh->pDownMesh->v-pMesh->v)*1.e8/pMesh->pDownMesh->dy;
            pMesh->esemi = dicon*pGlobals->epsilonB/sqrt((pGlobals->epsilonA+field*field));
        }
	} while ((pMesh = pMesh->pUpMesh) != pGlobals->pStartMesh->pUpMesh);

/*	write (*,*) meshcoord(wherey),deriv(wherey),fcharge(wherey),p(wherey)
/*	write (*,*) wherey
/*	write (*,*) "adjust",wherey,meshcoord(wherey),pGlobals->ConvgError
/*	write (*,*) ' '
 /*	pause */
    //sprintf(message,"%8s,%10.3e,%10.3e,%10.3e,%10.3e", "wherey=",wherey->meshcoord,pGlobals->ConvgError, wherey->deriv, wherey->fcharge);
    //alertbox(message);

	return;
}



/***********************************************/

void invert(struct global_type *pGlobals, struct flag_type *pFlags) {
    
/***********************************************/
    
    
/*  This function computes the fixed charge necessary for a solution to the Poisson
     equation, given a band diagram and electron and hole concentrations*/
    
    /*	double wherey;
     char outdata[100],extension2[30],access;
     int ioerr_loc;  // variables for trouble shooting */
    
    struct mesh_type *pMesh;
    
    pMesh = pGlobals->pTopMesh;
    if (pMesh->pUpMesh == NULL) {
        pMesh->fcharge = (invh*2.0e0/(pMesh->pDownMesh->dy+pMesh->dy)
                          *(pMesh->pDownMesh->esemi*(pMesh->pDownMesh->pDownMesh->v - pMesh->pDownMesh->v)/pMesh->pDownMesh->dy
                            -pMesh->esemi*(pMesh->pDownMesh->v - pMesh->v)/pMesh->dy)
                          -q*(pMesh->p - pMesh->n))/q;				// poisson term
        if (pMesh->fcharge >= 0.0)
            pMesh->Nd = pMesh->fcharge;
        else
            pMesh->Na = pMesh->fcharge;
        pMesh = pMesh->pDownMesh;
    }
    
    do {
        pMesh->fcharge = (invh*2.0e0/(pMesh->dy+pMesh->pUpMesh->dy)
                         *(pMesh->esemi*(pMesh->pDownMesh->v - pMesh->v)/pMesh->dy
                         -pMesh->pUpMesh->esemi*(pMesh->v - pMesh->pUpMesh->v)/pMesh->pUpMesh->dy)
                         -q*(pMesh->p - pMesh->n))/q;				// poisson term
        
        if (pMesh->fcharge >= 0.0)
            pMesh->Nd = pMesh->fcharge;
        else
            pMesh->Na = pMesh->fcharge;
        
    } while((pMesh = pMesh->pDownMesh) != pGlobals->pBottomMesh);
    
    pMesh->fcharge = (invh*2.0e0/(pMesh->pUpMesh->dy+pMesh->pUpMesh->pUpMesh->dy)
                      *(pMesh->pUpMesh->esemi*(pMesh->v - pMesh->pUpMesh->v)/pMesh->pUpMesh->dy
                        -pMesh->pUpMesh->pUpMesh->esemi*(pMesh->pUpMesh->v - pMesh->pUpMesh->pUpMesh->v)/pMesh->pUpMesh->pUpMesh->dy)
                      -q*(pMesh->p - pMesh->n))/q;
    
    
}


/***********************************************/
double initBands(struct mesh_type *pMesh, double Nd, double Na, double kT) {
/***********************************************/
   
    double tem,f;
    
    if (Nd > 10.0*Na) {
        if (kT > 1.0e-10) {
            tem = Nd/pMesh->Nc;
            if (pMesh->pLayer->mtrlType == graphenemtrl){ //graphene
                f = tem*pMesh->dy*1e-8;
                if (f > 6.11) //unger degenerate
                    pMesh->v = pMesh->Efn - pMesh->offset-kT*sqrt(2.0*f-3.22);	// set initial v for graphene
                else
                    pMesh->v = pMesh->Efn - pMesh->offset-kT*log(-1.0+exp(-2.6247-0.1819*f+sqrt(pow((2.6247+0.1819*f),2.0)+5.2495*f)));	// set initial v for graphene
            }
            else if (pMesh->pLayer->mtrlType == twoDmtrl){ //2D Fermi materials
                    pMesh->v = pMesh->Efn - pMesh->offset - kT*log(exp((Nd*pMesh->dy*1e-8)/(pMesh->n*kT))-1.0);	// set initial v for 2D material
            }
            else { // 3D material
                if (tem > 22.6) //use sommerfeld
                    pMesh->v = pMesh->Efn - pMesh->offset - kT*pow(1.329*tem, 0.66666);
                else //unger approx
                    pMesh->v = pMesh->Efn - pMesh->offset - kT*log(exp((sqrt(1.0+0.6*tem)-1.0)/0.3)-1.0);
                }
        }
        else {  // T=0
            if (pMesh->pLayer->mtrlType == graphenemtrl) //graphene
                pMesh->v = pMesh->Efn - pMesh->offset - Nd/pMesh->Nc;	// set initial v for graphene
            
            else if (pMesh->pLayer->mtrlType == twoDmtrl) //2D Fermi materials
                pMesh->v = pMesh->Efn - pMesh->offset + Nd/pMesh->Nc;	// set initial v for 2D material
            
            else // 3D material
                pMesh->v = pMesh->Efn - pMesh->offset + 1.5*Nd/pMesh->Nc;
        }
    }
    else if (Na > 10.0*Nd) {
        if (kT > 1.0e-10) {
            tem = Na/pMesh->Nv;
            if (pMesh->pLayer->mtrlType == graphenemtrl){ //graphene
                f = tem*pMesh->dy*1e-8;
                if (f > 6.11) //unger degenerate
                    pMesh->v = pMesh->Efp + pMesh->Eg - pMesh->offset + kT*sqrt(2.0*f-3.22);	// set initial v for graphene
                else
                    pMesh->v = pMesh->Efp + pMesh->Eg - pMesh->offset + kT*log(-1.0+exp(-2.6247-0.1819*f+sqrt(pow((2.6247+0.1819*f),2.0)+5.2495*f)));	// set initial v for graphene
            }
            else if (pMesh->pLayer->mtrlType == twoDmtrl){ //2D Fermi materials
                pMesh->v = pMesh->Efp + pMesh->Eg - pMesh->offset + kT*log(exp((Nd*pMesh->dy*1e-8)/(pMesh->n*kT))-1.0);	// set initial v for 2D material
            }
            else { // 3D material
                if (tem > 22.6) //use sommerfeld
                    pMesh->v = pMesh->Efp + pMesh->Eg - pMesh->offset + kT*pow(1.329*tem, 0.66666);
                else //unger approx
                    pMesh->v = pMesh->Efp + pMesh->Eg - pMesh->offset + kT*log(exp((sqrt(1.0+0.6*tem)-1.0)/0.3)-1.0);
            }
        }
        else {  // T=0
            if (pMesh->pLayer->mtrlType == graphenemtrl) //graphene
                pMesh->v = pMesh->Efp + pMesh->Eg - pMesh->offset + Na/pMesh->Nv;	// set initial v for graphene
            
            else if (pMesh->pLayer->mtrlType == twoDmtrl) //2D Fermi materials
                pMesh->v = pMesh->Efp + pMesh->Eg - pMesh->offset - Na/pMesh->Nv;	// set initial v for 2D material
            
            else // 3D material
                pMesh->v = pMesh->Efp + pMesh->Eg - pMesh->offset - 1.5*Na/pMesh->Nv;
        }
    }

    else // Close to intrinsic
        pMesh->v = pMesh->Efp + pMesh->Eg/2.0 - pMesh->offset; // Put the fermi level at midgap
    
    return pMesh->v;
}