#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "1Dpoi_type.h"
#include "1Dpoi_def.h"

int solve(double loc_converg, struct global_type *pGlobals, struct flag_type *pFlags);
int count;


/*****************************************************************/

int MultiRun(struct global_type *pGlobals, struct flag_type *pFlags) {

/*****************************************************************/
    int error, error_sim, i;
    double initcharge;	// charge of the initial run
    char voltconv[MAX_EXTENSION], outdata[75];
	double nsTotal,psTotal,sheetres;
    struct mesh_type *pMesh;
    struct contact_type *pCurrentContact;
	struct layer_type *pOhmicLayer;

    pCurrentContact = pGlobals->multiRun.pContact;
    pGlobals->vmulti = pCurrentContact->pVoltageSource->vbias + pCurrentContact->pVoltageSource->vincrement*(double) (pGlobals->multiRun.multiCount);
    pCurrentContact->pMeshPosition->Efn = -pGlobals->vmulti; //Set Fermi level
    pCurrentContact->pMeshPosition->Efp = -pGlobals->vmulti; //Set Fermi level

    if (pCurrentContact->contactType == SCHOTTKY) {
        pCurrentContact->pMeshPosition->v = pCurrentContact->contactParameter - pGlobals->vmulti - pCurrentContact->pMeshPosition->offset; //Schottky voltage
    }
    else if (pCurrentContact->contactType == OHMIC) {
        pMesh = pCurrentContact->pMeshPosition;
        pMesh->v = pCurrentContact->contactParameter - pGlobals->vmulti;			// ohmic contact
        pOhmicLayer = pMesh->pLayer;

        while (pMesh->pLayer == pOhmicLayer  && pMesh->pDownMesh != NULL){  // Set the Fermi level of the adjacent layer
            pMesh->Efn = -pGlobals->vmulti;
            pMesh->Efp = -pGlobals->vmulti;
            pMesh = pMesh->pDownMesh;
        }
        while (pMesh->pLayer == pOhmicLayer  && pMesh->pUpMesh != NULL){  // Set the Fermi level of the adjacent layer
            pMesh->Efn = -pGlobals->vmulti;
            pMesh->Efp = -pGlobals->vmulti;
            pMesh = pMesh->pUpMesh;
        }
    }

    sprintf(voltconv,"%-10.2f", pGlobals->vmulti);
    i=0;  // strip out any trailing spaces
    while (voltconv[i] != '\0') {
        if (voltconv[i] == ' ') {
            voltconv[i] = '\0';
            break;
        }
        i++;
    }

    if ( !pFlags->cv ) {
        error_sim = solve(pGlobals->converg,pGlobals,pFlags);		// solve Poissons and Schrodingers eq.
        if (error_sim) return 1;
        if (pFlags->findBst) {  // Check to see if find all boundstates is requested
            bndStDefaults(pGlobals, pFlags);
            error =boundstates(pGlobals, pFlags); // find the boundstates
        }
        if (error) return 1;
        strcpy(pGlobals->multiRun.extension,"_V");
        strcat(pGlobals->multiRun.extension,voltconv);
        error = writestatus(pGlobals,pFlags);	// write status file used by cleanup
        if (error) return 1;
        output(pGlobals,pFlags);		// output results
        if (error_sim) return 1;
    }
    else {
        // Capacitance vs voltage scan
        error = solve(pGlobals->CVconverg,pGlobals,pFlags);		// solve Poissons and Schrodingers eq.
        if (error) return 1;
        if (pFlags->findBst) {  // Check to see if find all boundstates is requested
            bndStDefaults(pGlobals, pFlags);
            error =boundstates(pGlobals, pFlags); // find the boundstates
        }
        if (error) return 1;
        strcpy(pGlobals->multiRun.extension,"");

        if (!pFlags->cvonly || pGlobals->multiRun.multiCount == 0) {
            strcpy(pGlobals->multiRun.extension,"_V");
            strcat(pGlobals->multiRun.extension,voltconv);
            output(pGlobals,pFlags);	// output results
        }
        initcharge = cvcharge(pGlobals);		// find the charge for this voltage
        //if (!pFlags->silent) printf("\n");
        if (pCurrentContact->contactType == SCHOTTKY) {
            pCurrentContact->pMeshPosition->v = pCurrentContact->pMeshPosition->v + pGlobals->cvvoltage; //Schottky voltage
        }
        else if (pCurrentContact->contactType == OHMIC) {
            pMesh = pCurrentContact->pMeshPosition;
            pMesh->v = pCurrentContact->contactParameter - pGlobals->vmulti;			// ohmic contact
            pOhmicLayer = pMesh->pLayer;

            while (pMesh->pLayer == pOhmicLayer  && pMesh->pDownMesh != NULL){  // Set the Fermi level of the adjacent layer
                pMesh->Efn = -pGlobals->vmulti - pGlobals->cvvoltage;
                pMesh->Efp = -pGlobals->vmulti - pGlobals->cvvoltage;
                pMesh = pMesh->pDownMesh;
            }
            while (pMesh->pLayer == pOhmicLayer  && pMesh->pUpMesh != NULL){  // Set the Fermi level of the adjacent layer
                pMesh->Efn = -pGlobals->vmulti - pGlobals->cvvoltage;
                pMesh->Efp = -pGlobals->vmulti - pGlobals->cvvoltage;
                pMesh = pMesh->pUpMesh;
            }
        }
        error = solve(pGlobals->CVconverg,pGlobals,pFlags);		// Solve Poi/Sch for pFlags->cv voltage
        if (error) return 1;

        pGlobals->capacitance = (cvcharge(pGlobals)-initcharge)/pGlobals->cvvoltage;
        sheetcharge(&nsTotal, &psTotal, &sheetres, pGlobals);
        //pCVdata[multicount] = (char *) malloc(75*sizeof(char));
        sprintf(outdata, "%10.3e\t%11.4e\t%11.4e\t%11.4e\t%11.4e\t%11.4e\n",
            pGlobals->vmulti,pGlobals->capacitance*q,initcharge,cvcharge(pGlobals),nsTotal,psTotal);
        strcat(pGlobals->CVFile,outdata);
    }
    pGlobals->multiRun.multiCount++;
    if(pGlobals->multiRun.multiCount ==pGlobals->multiRun.multiCountMax)
        return 0;
    else
        return 2;
}


/***********************************/

int solve(double loc_converg, struct global_type *pGlobals, struct flag_type *pFlags) {

/***********************************/

	int outputen,ioerr_loc;	/*output enable */
	char number[10],message[MAX_MESSAGE],access[3],extension2[MAX_EXTENSION];


	if (!pFlags->schrodinger)
		outputen = true;
	else
		outputen = false;

	pFlags->schex = false;		// reset pFlags->schrodinger execute flag

	if (pFlags->restart && pFlags->schrodinger) {
		pFlags->schex = true;
		outputen = true;	// On a pFlags->restart, jump right into a pFlags->schrodinger calculation
	}


//  Now we are ready to start the calculation

	for (count=1; count<pGlobals->maxcount; count++) {		// do up to specified max iterations

/*  When the convergence error falls below the pGlobals->crossover value, the pFlags->schrodinger solver kicks in, if requested */
		if (pGlobals->ConvgError < pGlobals->crossover && pFlags->schrodinger && !pFlags->schex && count > 1) {
			pFlags->schex = true;
			outputen = true;
		}

		charge(pGlobals,pFlags);		// find electron and hole concentration, and fixed charge
        adjust(pGlobals,pFlags);		// adjust band diagram

        //  Trouble shooting code
		/*output(pGlobals,pFlags);
         strcpy(access,"w");
         strcpy(extension2,"_itOut");
         ioerr_loc = fileopen(4, pGlobals->filename, access, extension2, pGlobals);
         fprintf(pGlobals->pFile[4], "%s",pGlobals->OutputFile);
         fileclose(4, pGlobals);*/

	   // printf("%-4i,%20s,%10.3e,%3s\n", count, "  convergence error=", pGlobals->ConvgError," eV");

/*  check for convergence, which occurs when the max correction to the conduction
    band falls below the requested convergence.  At least two iterations must be done. */

		if ((pGlobals->ConvgError < loc_converg) && outputen && (count > 2)) {
	    	if (!pFlags->silent) {
				//sprintf(message,"%-4i,%20s,%10.3e,%3s\n", count, "  convergence error=", pGlobals->ConvgError," eV");
				//alertbox(message);
            }
            pGlobals->iterationCount = count;
            return 0;
		}
	}

//  if not converged after max iterations, print error and stop.
	if (pFlags->silent) return 1; // Return now if not printing any messages
	strcpy(message,"I canna hold her much longer cap'n, she's breakin up!\n");
	if (!pFlags->silent) alertbox (message);
	sprintf(number,"%6i",pGlobals->maxcount);
	strcpy(message,"Execution stopped after ");
	strcat(message,number);
	strcat(message," iterations.\n");
	if (!pFlags->silent) alertbox (message);
	if (!pFlags->silent) {
		sprintf(message,"%-4i,%19s,%10.3e,%3s\n", count, "  convergence error=", pGlobals->ConvgError," eV\n");
		alertbox(message);
	}
	return 1;

}
