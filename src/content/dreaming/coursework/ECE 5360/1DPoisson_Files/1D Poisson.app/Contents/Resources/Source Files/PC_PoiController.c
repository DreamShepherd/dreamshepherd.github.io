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
#include "1Dpoi_type.h"
#include "1Dpoi_def.h"
struct flag_type *pFlags;
struct global_type *pGlobals;
struct mtrl_data_type *pMtrl_data;

int CVFileWrite(struct global_type *pGlobals);
int OutputFileWrite(struct global_type *pGlobals, char *extension);
int StatusFileWrite(struct global_type *pGlobals, char *extension);
int startBoundstates(struct global_type *pGlobals, struct flag_type *pFlags);

//int solve(double loc_converg, struct global_type *pGlobals, struct flag_type *pFlags);



/*************************************/
void startPC() {
/*************************************/
	int error_loc;

	pGlobals = (struct global_type *) malloc(sizeof(struct global_type));
    pFlags = (struct flag_type *) malloc(sizeof(struct flag_type));

    pFlags->noinputfile = true;
    pFlags->schMemAlloc = false;
    pFlags->schElecMemAlloc = false; // Start with the Schrodinger memory flag unset
    pFlags->schHhMemAlloc = false; // Start with the Schrodinger memory flag unset
    pFlags->schLhMemAlloc = false; // Start with the Schrodinger memory flag unset.
    pFlags->schSohMemAlloc = false; // Start with the Schrodinger memory flag unset. If another simulation has already been run, free the memory allocated for the layer and mesh arrays */
    pGlobals->pMtrl_data = pMtrl_data;
    pGlobals->pFirstContact = NULL;
    pGlobals->pFirstVoltage = NULL;
    pGlobals->filename = NULL;
    pGlobals->inputPath = NULL;
    pGlobals->OutputFile = NULL;
    pGlobals->StatusFile = NULL;
    pGlobals->CVFile = NULL;
    pGlobals->ShellFile = NULL;

	error_loc = startPoisson(pGlobals, pFlags);
	return;
}


/****************************************/
int startPoisson(struct global_type *pGlobals, struct flag_type *pFlags) {
/****************************************/
//int main(int argc, char *argv[]) {
//#if PLATFORM != MAC

	int choice, choice2,ioerr_loc,infoLength,searchstart,i,filepointer,length;
	char access[3],extension[MAX_EXTENSION],field2[MAX_EXPRESS],
		expression[MAX_EXPRESS],message[MAX_MESSAGE],*info=NULL,*pPath,*pProgramPath;
	struct layer_type *pLayer;
	struct mesh_type *pMesh;

    //ioerr_loc = Init_1DPoisson(pGlobals,pFlags);
	pGlobals->inputPath = (char*) malloc(MAX_FILENAME*sizeof(char));
	pGlobals->filename = (char*) malloc(MAX_FILENAME*sizeof(char));
	pProgramPath = (char*) malloc(MAX_FILENAME*sizeof(char));
	pPath = _getcwd (pGlobals->inputPath, MAX_FILENAME); // get the directory path for the program
	length = strlen(pPath);
    for (i=0; i<length; i++) {
        pProgramPath[i] = pPath[i];
    }
    pProgramPath[length] = '\0'; // Copy program path into a permanent string (otherwise gets auto dealloc)
	strcpy(pGlobals->inputPath, pProgramPath);
	pGlobals->pMtrl_data = (struct mtrl_data_type *) malloc(sizeof(struct mtrl_data_type));
    pGlobals->pMtrl_data->pTopMtrl = NULL;
    length = strlen(pPath);
    pGlobals->pMtrl_data->matrlfilePath = (char *) malloc((length+13)*sizeof(char));

	strcpy(pGlobals->pMtrl_data->matrlfilePath, pGlobals->inputPath);
	strcat(pGlobals->pMtrl_data->matrlfilePath,"\\materials.txt");		// Default materials file is the file 'materials'
	//	 open materials file
	strcpy(access,"r");
	strcpy(extension,"");
    pGlobals->pMtrl_data->MatrlFile = fileread(pGlobals->pMtrl_data->matrlfilePath, access, extension);

	ioerr_loc = checkMaterials(pGlobals->pMtrl_data);  // check materials file version
	if (ioerr_loc == 0)ioerr_loc = loadMaterials(pGlobals->pMtrl_data);  // Load materials file

	if (ioerr_loc) { // If materials file not found, ask for one
        //while (pFlags->badversion) {
            strcpy(message, "Please Choose Materials File");
            alertbox (message);
            return 1;
            system("pause");

           // [MenuHandler ChooseMtrlFile:nil];		// select a materials file
        //}
    }


//#if program_type == NORMAL
	printf("\n");
	printf(" 1 D   P O I S S O N / S C H R O D I N G E R  S O L V E R\n");
	printf("\n");
	printf("Copyright 2014 Build B9b6_010\n");
	printf("\n");
	printf("Please send any questions or comments regarding this program to:\n");
	printf("\n");
	printf("Greg Snider\n");
	printf("Department of Electrical Engineering\n");
	printf("275 Fitzpatrick Hall\n");
	printf("University of Notre Dame\n");
	printf("Notre Dame, IN 46556\n");
	printf("Phone: (574) 631-4148   email: snider.7@nd.edu\n");
	printf("\n");
	printf("Please send email to be added to the distribution list\n");
	printf("\n");


	choice=0;
	choice2=0;
	while (choice != 7) {
		printf("\n");
		printf("Select one of the following:\n");
		printf("(1) Select and Run Input File...\n");
		printf("(2) Find Quantized States\n");
		printf("(3) Materials Menu\n");
		printf("(4) QUIT\n");
		printf("\n");
		choice=0;

	    scanf("%d",&choice);
	    switch(choice) {
	    case 1:
	    	selrun(pGlobals, pFlags);
			break;
	   	case 2:
	    	startBoundstates(pGlobals, pFlags);
	    	break;
	    case 3:
	  		while (choice2 != 3) {
				printf("\n");
				printf("Select one of the following:\n");
				//printf("(1) Select Materials File...\n");
				printf("(1) List Materials\n");
				printf("(2) List Physical Parameters...\n");
				//printf("(4) Add a Material\n");
				//printf("(5) Delete a Material\n");
				printf("(3) Exit\n");
				printf("\n");
				choice2=0;

				scanf("%i", &choice2);
    			switch(choice2) {
        		case 1:
        			listmaterials(pGlobals, pFlags);
        			break;
        		case 2:
        			materialparams(pGlobals, pFlags);
        			break;
				}
			}
			break;
		case 4:
			exit(0);
			return 0;
		default:
    		break;
		}
	strcpy(pGlobals->inputPath, pProgramPath);
	}
	return 0;
//#endif
/*#else
	// This is the command line code.  The filename is passed as an argument
	strcpy(pGlobals->filename,pGlobals->inputPath);
	strcat(pGlobals->filename,argv[1]);
	strcpy(access,"r");
	strcpy(extension,"");
	ioerr_loc = fileopen(4, pGlobals->filename, access, extension, pGlobals);
	// If the input file open failed, stop.  Otherwise proceed.
	if (ioerr_loc) return 0;

	ioerr_loc = input(pGlobals,pFlags);

	// If the input read failed, stop.  Otherwise initialize the arrays.
	if (ioerr_loc)
		return 0;
	else {
		ioerr_loc = initialize(pGlobals,pFlags);
		onedpoisson(pGlobals,pFlags);
		return 0;
	}
#endif
	 */
}


/*************************************/
int currentfile(struct global_type *pGlobals, struct flag_type *pFlags) {
/*************************************/

// current file function.  Displays current input file
	char message[MAX_MESSAGE];

	if (pFlags->noinputfile) {
		strcpy(message,"There is no Current Input File");
		if (!pFlags->silent) alertbox(message);
	}
	else {
		strcpy(message,"Current Input File is: ");
		strcat(message,pGlobals->filename);
		if (!pFlags->silent) alertbox(message);
	}

	return 0;
}

/************************************************/
void selrun(struct global_type *pGlobals, struct flag_type *pFlags) {
/************************************************/

    /* Select input file then run simulation */

	char *info=NULL,extension[MAX_EXTENSION],access[3],message[MAX_MESSAGE],name[MAX_FILENAME],name2[MAX_FILENAME];
	int infoLength,error,error_loc,i,filepointer,length;
	struct layer_type *pLayer;
	struct mesh_type *pMesh;

    /* If another simulation has alaready been run, free the memory allocated for the
     layer and mesh arrays */
    // Setup simulation
    error_loc = Init_1DPoisson(pGlobals,pFlags);
	pFlags->firstcv = true;	// set first iteration flag for CV runs
	pFlags->firstetch = true;	// set first iteration flag for etch runs

	/*  To set up the simulation, call the subroutines input and initialize.
     If an error is found, stop the program. */

	printf("Enter input filename \n");
	printf("(Input file must have .txt extension.  Do not type .txt extension!):\n");
	scanf("%s", name);
	//strcpy(name2,pGlobals->inputPath);
	strcat(pGlobals->inputPath,"\\");
	strcat(pGlobals->inputPath,name);
	//strcat(name2,".txt");
	strcpy(access,"r");
	strcpy(extension,".txt");

	pGlobals->InputFile = fileread(pGlobals->inputPath, access, extension); // read in the input file
	//Save input filename
    //length =strlen(name2);
    strcpy(pGlobals->filename, name);
	//pGlobals->filename[length-4] = '\0';

	/*
     if (ioerr_loc) {
     if (ioerr_loc == 26) {
     strcpy(message,"Sorry, That file is already open elsewhere.");
     if (!pFlags->silent) alertbox(message);
     }
     else if (ioerr_loc == 29)
     // user canceled open
     ;
     else{
     strcpy(message,"Some sort of un-natural file error has occured.");
     if (!pFlags->silent) alertbox(message);
     }
     return;
     }*/

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
		return;
	}

	error = input(pGlobals,pFlags);  // Process the input file

	// If the input read failed, stop.  Otherwise initialize the arrays.
	if (error)
		return;
	else {
		error = initialize(pGlobals,pFlags);
	}
	if (error) return;


    //first check to see that there is an input file
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
            if (error_loc == 1) return;

			if (!pFlags->cvonly) {
				if (pFlags->findBst) {
					bndStDefaults(pGlobals, pFlags);  //Find default bounds
					error = boundstates(pGlobals,pFlags);	// find all bound states if requested
				}
				if (error) return 1;
				error = OutputFileWrite(pGlobals, pGlobals->multiRun.extension); // output results
				error = StatusFileWrite(pGlobals, pGlobals->multiRun.extension); // output results
            }

			strcpy(message,"Calculation ");
			strcat(message,pGlobals->multiRun.extension);
			strcat(message," Completed");
			if (!pFlags->silent) printf("%s",message);
			if (!pFlags->silent) printf("\n");
		} while (error_loc != 0);

        // Write CV output file
        error = CVFileWrite(pGlobals);
        strcpy(message,"Caclculation done");
        if (!pFlags->silent) alertbox(message);
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
        error = OutputFileWrite(pGlobals, extension);
        error = StatusFileWrite(pGlobals, extension);

        strcpy(message,"Caclculation done");
        if (!pFlags->silent) alertbox(message);
    }
	else {
		error = solve(pGlobals->converg,pGlobals, pFlags);		// Normal run, nothing fancy
		if (error) {
			strcpy(extension,"");
			output(pGlobals,pFlags);		// output results, error during run
			return;
		}
		if (error) return;
		strcpy(extension,"");
		output(pGlobals,pFlags);		// output results, normal run
		if (pFlags->findBst) {	// find all bound states if requested
			bndStDefaults(pGlobals, pFlags);  //Find default bounds
			error = boundstates(pGlobals,pFlags);	// find all bound states if requested
		}
		if (!pFlags->silent) printf("\n");
		if (pFlags->DangerWill && pFlags->ignoreDanger) {
			strcpy(message,"Final structure has a confinement problem in the Schrodinger mesh");
			if (!pFlags->silent) alertbox(message);
		}


	    if (!pFlags->silent) {
			sprintf(message,"%-4i,%20s,%10.3e,%3s\n", pGlobals->iterationCount, "  convergence error=", pGlobals->ConvgError," eV");
			alertbox(message);
		}
		strcpy(extension,"");
        error = OutputFileWrite(pGlobals, extension);
        error = StatusFileWrite(pGlobals, extension);
	}
	return;

}





/************************************************/
startBoundstates(struct global_type *pGlobals, struct flag_type *pFlags){
/************************************************/

    int error, error_loc;
    double *eUB,*eLB,*hUB,*hLB;
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
		printf("Use the default energy search range (%f, to %f eV) for hole subbands? (y/n): ",-pGlobals->hLB,-pGlobals->hUB);

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
        pGlobals->hLB = numconv(keyword);
		//scanf("%lf", &pGlobals->LB);
		pGlobals->hLB = -pGlobals->hLB;
		printf("Enter upper search bound: ");
        inputline = fgets (info,MAX_INPUTLINE,stdin);
        if (inputline == NULL) {
            /* Some error occurred, or we reached end-of-file. No input was read. */
        } else {
            startsearch =0;
            fieldtrim(info, &startsearch, keyword, extension, 1);
        }
        pGlobals->hUB = numconv(keyword);
		//scanf("%lf", &pGlobals->UB);
		pGlobals->hUB = -pGlobals->hUB;
	}

	//	 See if the user wants to change the split-off energy range to search
	if (!pFlags->silent) {
		printf("Use the default energy search range (%f, to %f eV) for split-off subbands? (y/n): ",-pGlobals->soLB,-pGlobals->soUB);

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
        pGlobals->soLB = numconv(keyword);
		//scanf("%lf", &pGlobals->LB);
		pGlobals->soLB = -pGlobals->soLB;
		printf("Enter upper search bound: ");
        inputline = fgets (info,MAX_INPUTLINE,stdin);
        if (inputline == NULL) {
            /* Some error occurred, or we reached end-of-file. No input was read. */
        } else {
            startsearch =0;
            fieldtrim(info, &startsearch, keyword, extension, 1);
        }
        pGlobals->soUB = numconv(keyword);
		//scanf("%lf", &pGlobals->UB);
		pGlobals->soUB = -pGlobals->soUB;
	}
    error = boundstates(pGlobals,pFlags);
	strcpy(extension, "");
    if (!error)error = OutputFileWrite(pGlobals, extension); //If no error in run, write the output files.
    if (!error)error = StatusFileWrite(pGlobals, extension); //If no error in run, write the output files.
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
	strcpy(name2,pGlobals->filename);
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
	strcpy(name2,pGlobals->filename);
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
	strcpy(name2,pGlobals->filename);
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


/****************************************/
void alertbox(char message[]) {
/****************************************/

	strcat(message,"\n");
	printf("%s",message);
}
