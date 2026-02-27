//
//  PC_PoiController.c
//
//  Created by Greg Snider on 3/14/14.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "OutPlot.h"
//#include "_DPoissonWX3Main.h"
#include "..\1Dpoi_type.h"
//#include "..\1Dpoi_def.h"
//struct global_type *pGlobals;
//struct mtrl_data_type *pMtrl_data;

int CVFileWrite(struct global_type *pGlobals);
int OutputFileWrite(struct global_type *pGlobals, char *extension);
int StatusFileWrite(struct global_type *pGlobals, char *extension);
int startBoundstates(struct global_type *pGlobals, struct flag_type *pFlags);
int PlotPrep(struct global_type *pGlobals, struct flag_type *pFlags, OutPlot* plot, struct plotSetUp_type* plotSetUp);
//extern class OutPlot *plot;

extern "C" int solve(double loc_converg, struct global_type *pGlobals, struct flag_type *pFlags);
extern "C" int Init_1DPoisson(struct global_type *pGlobals, struct flag_type *pFlags);
extern "C" void alertbox(char message[]);
extern "C" int getInputNew(char file[], int *filepointer, char **info);
extern "C" void script(struct global_type *pGlobals, struct flag_type *pFlags);
extern "C" int input(struct global_type *pGlobals, struct flag_type *pFlags);
extern "C" int initialize(struct global_type *pGlobals, struct flag_type *pFlags);
extern "C" int MultiRun(struct global_type *pGlobals, struct flag_type *pFlags);
extern "C" void bndStDefaults(struct global_type *pGlobals, struct flag_type *pFlags);
extern "C" int boundstates(struct global_type *pGlobals, struct flag_type *pFlags);
extern "C" double initBands(struct mesh_type *pMesh, double posCharge, double negCharge, double kT);
extern "C" void invert(struct global_type *pGlobals, struct flag_type *pFlags);
extern "C" int output(struct global_type *pGlobals, struct flag_type *pFlags);
extern "C" struct mesh_type  *ymesh(double position,struct global_type *pGlobals);
extern "C" int fieldtrim(char info[], int *startsearch,char field[],char subfield[],int keepCase);
extern "C" double numconv(char field[]);



/****************************************/
int startPoisson(struct global_type *pGlobals, struct flag_type *pFlags, OutPlot* plot, struct plotSetUp_type* plotSetUp) {
/****************************************/
//int main(int argc, char *argv[]) {
//#if PLATFORM != MAC

	int choice, choice2,ioerr_loc,infoLength,searchstart,i,filepointer,length,error_loc;
	char access[3],extension[MAX_EXTENSION],field2[MAX_EXPRESS],
		expression[MAX_EXPRESS],message[MAX_MESSAGE],*info=NULL;
	struct layer_type *pLayer;
	struct mesh_type *pMesh;

    //ioerr_loc = Init_1DPoisson(pGlobals,pFlags);
	//pGlobals->inputPath = (char*) malloc(MAX_FILENAME*sizeof(char));
	//pGlobals->filename = (char*) malloc(MAX_FILENAME*sizeof(char));
	//	 open materials file
	//strcpy(access,"r");
	//strcpy(extension,"");
    //pGlobals->pMtrl_data->MatrlFile = fileread(pGlobals->pMtrl_data->matrlfilePath, access, extension);


    // Setup simulation
    error_loc = Init_1DPoisson(pGlobals,pFlags);

	filepointer =0;
	infoLength=getInputNew(pGlobals->InputFile, &filepointer, &info);  //Get new line from input file
	//infoLength=getInput(4,info,pGlobals);
	//fileclose(4, pGlobals);
	if (!strncmp(info,"#script",7)) {  // Check to see if it is a script file
        if (info != NULL) {
            free(info);
            info = NULL;
        }
		script(pGlobals,pFlags); // Run the script routine
		return 0;
	}

	error_loc = input(pGlobals,pFlags);  // Process the input file

	// If the input read failed, stop.  Otherwise initialize the arrays.
	if (error_loc)
		return 1;
	else {
		error_loc = initialize(pGlobals,pFlags);
	}
	if (error_loc) return 1;


    //first check to see that there is a valid input file
	if (pFlags->noinputfile) {
		strcpy(message,"You Must First Choose an Input File");
		if (!pFlags->silent) alertbox(message);
		return 1;
	}

	strcpy(extension,"");
	//if (pFlags->cleanFiles) cleanup();  //Delete out of date output files
	//if (!pFlags->silent) printf("\n");

	if (pFlags->multiruns) {
        pGlobals->multiRun.multiCount = 0;

        do {
			error_loc = MultiRun(pGlobals,pFlags);
            if (error_loc == 1) return 1;

			if (!pFlags->cvonly) {
				if (pFlags->findBst) {
					bndStDefaults(pGlobals, pFlags);  //Find default bounds
					error_loc = boundstates(pGlobals,pFlags);	// find all bound states if requested
				}
				if (error_loc) return 1;
				error_loc = OutputFileWrite(pGlobals, pGlobals->multiRun.extension); // output results
				error_loc = StatusFileWrite(pGlobals, pGlobals->multiRun.extension); // output results
                //cleanup output file memory
                free (pGlobals->OutputFile);
                pGlobals->OutputFile = NULL;
                free(pGlobals->StatusFile);
                pGlobals->StatusFile = NULL;
            }

			//strcpy(message,"Calculation ");
			//strcat(message,pGlobals->multiRun.extension);
			//strcat(message," Completed");
			//if (!pFlags->silent) printf("%s",message);
			//if (!pFlags->silent) printf("\n");
		} while (error_loc != 0);

        // Write CV output file
        error_loc = CVFileWrite(pGlobals);
        free (pGlobals->CVFile);
        pGlobals->CVFile = NULL;
        //strcpy(message,"Caclculation done");
        //if (!pFlags->silent) alertbox(message);
	}



    else if (pFlags->invert) { //Find an inverse solution of fixed charge given a mobile charge distribution

        pMesh = pGlobals->pStartMesh;

        do  {
            // find the band energies for each mesh point
            pMesh->v = initBands(pMesh, pMesh->n, pMesh->p, pGlobals->kT);
        } while((pMesh = pMesh->pDownMesh) != pGlobals->pBottomMesh);
            pMesh->v = initBands(pGlobals->pBottomMesh, pMesh->n, pMesh->p, pGlobals->kT);
        invert(pGlobals, pFlags); // Find the fixed charge that corresponds to the band diagram and mobile charge

		strcpy(extension,"");
		output(pGlobals,pFlags);		// output results

		strcpy(extension,"");
        error_loc = OutputFileWrite(pGlobals, extension);
        error_loc = StatusFileWrite(pGlobals, extension);
        //cleanup output file memory
        free (pGlobals->OutputFile);
        pGlobals->OutputFile = NULL;
        free(pGlobals->StatusFile);
        pGlobals->StatusFile = NULL;

        strcpy(message,"Caclculation done");
        if (!pFlags->silent) alertbox(message);
    }
	else {
		error_loc = solve(pGlobals->converg,pGlobals, pFlags);		// Normal run, nothing fancy
		if (error_loc) {
			strcpy(extension,"");
			output(pGlobals,pFlags);		// output results, error during run
			return 1;
		}
		if (error_loc) return 1;
		strcpy(extension,"");
		output(pGlobals,pFlags);		// output results, normal run
		if (pFlags->findBst) {	// find all bound states if requested
			bndStDefaults(pGlobals, pFlags);  //Find default bounds
			error_loc = boundstates(pGlobals,pFlags);	// find all bound states if requested
		}
		//strcpy(message,"Calculation Completed");
		//if (!pFlags->silent) alertbox(message);
		//if (pFlags->DangerWill && pFlags->ignoreDanger) {
		//	strcpy(message,"Final structure has a confinement problem in the Schrodinger mesh");
		//	if (!pFlags->silent) alertbox(message);
		//}


	    //if (!pFlags->silent) {
		//	sprintf(message,"%-4i,%20s,%10.3e,%3s\n", pGlobals->iterationCount, "  convergence error=", pGlobals->ConvgError," eV");
		//	alertbox(message);
		//}

        //strcpy(message ,"Just before plot");
        //alertbox(message);
        //Plot results
        PlotPrep(pGlobals, pFlags, plot, plotSetUp);

		// write the output data
		strcpy(extension,"");
        error_loc = OutputFileWrite(pGlobals, extension);
        error_loc = StatusFileWrite(pGlobals, extension);
        //cleanup output file memory
        free (pGlobals->OutputFile);
        pGlobals->OutputFile = NULL;
        free(pGlobals->StatusFile);
        pGlobals->StatusFile = NULL;
    }
	return 0;
}



/************************************************/
int startBoundstates(struct global_type *pGlobals, struct flag_type *pFlags){
/************************************************/

    int error, error_loc;
    double *eUB,*eLB,*hhUB,*hhLB;
	char ans,dummy,statesname[MAX_FILENAME],charDummy[30],keyword[MAX_KEYWORD],extension[MAX_EXTENSION],access[3],info[MAX_INPUTLINE],message[128],*inputline;
    //=(char *)malloc(MAX_INPUTLINE*sizeof(char));
	int ioerr_loc,fieldlength,fixBounds,i,j,count,startsearch,c,len=MAX_INPUTLINE;
	double nthird,pthird,*pOldn,*pOldderiv,tem;
	struct mesh_type *pMesh;


	if (pFlags->schrodinger) {
        //	   if the input file defines a pFlags->schrodinger range, see if the user wants it changed
		printf("Use the default Schrodinger range (%f,%f Ang)? (y/n): ",
               pGlobals->pSchStart->meshcoord,pGlobals->pSchStop->meshcoord);
		dummy = getchar();
		ans = getchar();
		dummy = getchar();
	}

	if (ans != 'y' || (!pFlags->schrodinger)) {
		pFlags->schMemAlloc = false;
		printf("Enter starting point of Schrodinger mesh (Ang): ");
		scanf("%f", &pGlobals->realschstart);
		pGlobals->pSchStart = ymesh(pGlobals->realschstart,pGlobals);

		printf("Enter stopping point of Schrodinger mesh (Ang): ");
		scanf("%f", &pGlobals->realschstop);
		pGlobals->pSchStop = ymesh(pGlobals->realschstop,pGlobals);
	}

    bndStDefaults(pGlobals, pFlags);  //Find default bounds

    //	 See if the user wants to change the energy range to search
	if (!pFlags->silent) {
		sprintf(message,"Use the default energy search range (%f, to %f eV) for electron subbands? (y/n): ",pGlobals->eLB,pGlobals->eUB);
		alertbox(message);

        inputline = fgets (info,MAX_INPUTLINE,stdin);  /* Uh-oh. Don't panic. See below. */
        if (inputline == NULL) {
            /* Some error occurred, or we reached end-of-file. No input was read. */
        } else {
            ans = info[0];/* Parse the line. */
        }
	}
	else ans = 'y';

	if (ans != 'y') {
		printf("Enter lower search bound: ");
        inputline = fgets (info,MAX_INPUTLINE,stdin);
        if (inputline == NULL) {
            /* Some error occurred, or we reached end-of-file. No input was read. */
        } else {
            startsearch =0;
            fieldtrim(info, &startsearch, keyword, extension, 1);
        }
        pGlobals->eLB = numconv(keyword);
		//scanf("%lf", &pGlobals->LB);
		printf("Enter upper search bound: ");
        inputline = fgets (info,MAX_INPUTLINE,stdin);
        if (inputline == NULL) {
            /* Some error occurred, or we reached end-of-file. No input was read. */
        } else {
            startsearch =0;
            fieldtrim(info, &startsearch, keyword, extension, 1);
        }
        pGlobals->eUB = numconv(keyword);
		//scanf("%lf", &pGlobals->UB);
	}

	//	 See if the user wants to change the energy range to search
	if (!pFlags->silent) {
		printf("Use the default energy search range (%f, to %f eV) for hole subbands? (y/n): ",-pGlobals->hhLB,-pGlobals->hhUB);

        //dummy = getchar();
        inputline = fgets (info,MAX_INPUTLINE,stdin);  /* Uh-oh. Don't panic. See below. */
        if (inputline == NULL) {
            /* Some error occurred, or we reached end-of-file. No input was read. */
        } else {
            ans = info[0];/* Parse the line. */
        }
	}
	else ans = 'y';

	if (ans != 'y') {
		printf("Enter lower search bound: ");
        inputline = fgets (info,MAX_INPUTLINE,stdin);
        if (inputline == NULL) {
            /* Some error occurred, or we reached end-of-file. No input was read. */
        } else {
            startsearch =0;
            fieldtrim(info, &startsearch, keyword, extension, 1);
        }
        pGlobals->hhLB = numconv(keyword);
		//scanf("%lf", &pGlobals->LB);
		pGlobals->hhLB = -pGlobals->hhLB;
		printf("Enter upper search bound: ");
        inputline = fgets (info,MAX_INPUTLINE,stdin);
        if (inputline == NULL) {
            /* Some error occurred, or we reached end-of-file. No input was read. */
        } else {
            startsearch =0;
            fieldtrim(info, &startsearch, keyword, extension, 1);
        }
        pGlobals->hhUB = numconv(keyword);
		//scanf("%lf", &pGlobals->UB);
		pGlobals->hhUB = -pGlobals->hhUB;
	}

	//	 See if the user wants to change the split-off energy range to search
	if (!pFlags->silent) {
		printf("Use the default energy search range (%f, to %f eV) for split-off subbands? (y/n): ",-pGlobals->sohLB,-pGlobals->sohUB);

        //dummy = getchar();
        inputline = fgets (info,MAX_INPUTLINE,stdin);  /* Uh-oh. Don't panic. See below. */
        if (inputline == NULL) {
            /* Some error occurred, or we reached end-of-file. No input was read. */
        } else {
            ans = info[0];/* Parse the line. */
        }
	}
	else ans = 'y';

	if (ans != 'y') {
		printf("Enter lower search bound: ");
        inputline = fgets (info,MAX_INPUTLINE,stdin);
        if (inputline == NULL) {
            /* Some error occurred, or we reached end-of-file. No input was read. */
        } else {
            startsearch =0;
            fieldtrim(info, &startsearch, keyword, extension, 1);
        }
        pGlobals->sohLB = numconv(keyword);
		//scanf("%lf", &pGlobals->LB);
		pGlobals->sohLB = -pGlobals->sohLB;
		printf("Enter upper search bound: ");
        inputline = fgets (info,MAX_INPUTLINE,stdin);
        if (inputline == NULL) {
            /* Some error occurred, or we reached end-of-file. No input was read. */
        } else {
            startsearch =0;
            fieldtrim(info, &startsearch, keyword, extension, 1);
        }
        pGlobals->sohUB = numconv(keyword);
		//scanf("%lf", &pGlobals->UB);
		pGlobals->sohUB = -pGlobals->sohUB;
	}
    error = boundstates(pGlobals,pFlags);
	strcpy(extension, "");
    if (!error)error = OutputFileWrite(pGlobals, extension); //If no error in run, write the output files.
    if (!error)error = StatusFileWrite(pGlobals, extension); //If no error in run, write the output files.

    return 0;
}


//************************************************************************************************
int OutputFileWrite(struct global_type *pGlobals, char *extension) {
//************************************************************************************************

    // Function to open a file
#define bufferlength 256  // max buffer length
	int ioerr_loc, i, c;
	char name2[MAX_FILENAME], access[3];
	char buffer[bufferlength];
	char *pFileData;
	FILE *pFile;

	//pFileData = (char*) malloc(bufferlength*sizeof(char));
	strcpy(access,"w");
	//ioerr_loc = fileopen(4, pGlobals->filename, access, extension2, pGlobals);
	strcpy(name2,pGlobals->inputPath);
	strcat(name2,extension);
	strcat(name2,"_Out.txt");
	pFile = fopen(name2, access);
	if (pFile == NULL) {
		ioerr_loc = errno;
		//printf("File error %i\n",ferror);
		perror("error");
		return ioerr_loc;
	}
	else
		ioerr_loc = 0;
    fprintf(pFile, "%s",pGlobals->OutputFile);
    fclose(pFile);
	return 0;
}



//************************************************************************************************
int StatusFileWrite(struct global_type *pGlobals, char *extension) {
//************************************************************************************************

    // Function to open a file
	int ioerr_loc, i, c;
	char name2[MAX_FILENAME], access[3];
	char buffer[bufferlength];
	char *pFileData;
	FILE *pFile;
    //Now write the status file
    strcpy(access,"w");
	strcpy(name2,pGlobals->inputPath);
	strcat(name2,extension);
    strcat(name2,"_Status.txt");
	pFile = fopen(name2, access);
	if (pFile == NULL) {
		ioerr_loc = errno;
		//printf("File error %i\n",ferror);
		perror("error");
		return ioerr_loc;
	}
	else
		ioerr_loc = 0;
    fprintf(pFile, "%s",pGlobals->StatusFile);
    fclose(pFile);
    return 0; // normal return
}

/**********************************************************/
int CVFileWrite(struct global_type *pGlobals) {
/**********************************************************/

/*  This routine creates the _CV file. */
	int ioerr_loc, multicount, lineLength;
	char *outdata;
	char access[3],name2[MAX_FILENAME];
	char extension[MAX_EXTENSION];
	FILE *pFile;

	/*strcpy(access,"w");
	strcpy(extension,"_CV");
	lineLength = 80;
	outdata =  (char *) malloc(lineLength*sizeof(char));

	strcpy(outdata,"V\tC (Fcm-2)\tInitcharge (cm-2)\tFinalcharge (cm-2)\tns Total (cm-2)\tps Total (cm-2)\n");
	fprintf(pGlobals->pFile[4],outdata);

	for (multicount=0; multicount<multicountmax; multicount++) fprintf(pGlobals->pFile[4],pCVdata[multicount]);
	fileclose (4, pGlobals);*/

	strcpy(access,"w");
	strcpy(extension,"_CV");
	//ioerr_loc = fileopen(4, pGlobals->filename, access, extension2, pGlobals);
	strcpy(name2,pGlobals->inputPath);
	strcat(name2,extension);
	strcat(name2,".txt");
	pFile = fopen(name2, access);
	if (pFile == NULL) {
		ioerr_loc = errno;
		//printf("File error %i\n",ferror);
		perror("error");
	}
	else
		ioerr_loc = 0;
    fprintf(pFile, "%s",pGlobals->CVFile);
    fclose(pFile);

	return 0;
}
