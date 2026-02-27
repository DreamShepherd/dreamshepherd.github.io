#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "1Dpoi_type.h"
#include "1Dpoi_def.h"

/*************************************************/
void choose(struct global_type *pGlobals, struct flag_type *pFlags) {
/*************************************************/

/* Chose an input file without running the simulation */

	char extension[MAX_EXTENSION],access[3],message[MAX_MESSAGE];
	int error,ioerr_loc;
	struct layer_type *pLayer;
	struct mesh_type *pMesh;

/* If another simulation has already been run, free the memory allocated for the
   layer and mesh arrays */

	/*if (!pFlags->noinputfile) {
		pLayer = pGlobals->pTopLayer;
		while (pLayer->pDownLayer != NULL) {
			pLayer = pLayer->pDownLayer;
			free(pLayer->pUpLayer);
		}
		free(pLayer);
		pMesh = pGlobals->pTopMesh;
		if (pMesh->pUpMesh !=NULL) free(pMesh->pUpMesh);
		while (pMesh->pDownMesh != NULL) {
			pMesh = pMesh->pDownMesh;
			free(pMesh->pUpMesh);
		}
		free(pMesh);
		pFlags->noinputfile = true;
	}

	strcpy(pGlobals->filename,"*");
	strcpy(access,"r");
	strcpy(extension,"");

	ioerr_loc = fileopen(4, pGlobals->filename, access, extension, pGlobals);

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
	}

	fileclose(4, pGlobals);

	error = input(pGlobals,pFlags);

	// If the input read failed, stop.  Otherwise initialize the arrays.
	if (error)
		return;
	else {
		error = initialize(pGlobals,pFlags);
		return;
	}*/
}

/*****************************************/
void script(struct global_type *pGlobals, struct flag_type *pFlags) {
/*****************************************/

/* Run a script file, a file containing the names of input files to be run. */

	char *info=NULL,shellfilename[MAX_EXTENSION],extension[MAX_EXTENSION],access[3];
	char name[MAX_FILENAME],name2[MAX_FILENAME];
	int infoLength,fieldlength,startsearch,error,ioerr_loc,sign_possible,filepointer;


	/*strcpy(shellfilename,pGlobals->filename);
	strcpy(access,"r");
	strcpy(extension,"");
	sign_possible = true;*/

	//ioerr_loc = fileopen(8, pGlobals->filename, access, extension, pGlobals);

	pGlobals->ShellFile = pGlobals->InputFile;  //Move file contents to shell file
	filepointer =0;
	infoLength=getInputNew(pGlobals->ShellFile, &filepointer, &info);  //skip #script line
	//infoLength=getInput(8,info,pGlobals);	//skip #script line

	while (true) {
		infoLength=getInputNew(pGlobals->ShellFile, &filepointer, &info);  //get next line, contains input file name
		//infoLength=getInput(8,info,pGlobals);
			/* Check for end of file */
		if (infoLength == EOF)
			break;

		fieldlength = getop(name,info,&startsearch,&sign_possible);
		if (pGlobals->filename[0] == '#')
			continue;
		printf("Now calculating=,%s,\n",pGlobals->filename);
		strcpy(name2,pGlobals->inputPath);
		strcat(name2,name);
		strcat(name2,".txt");
		strcpy(access,"r");
		strcpy(extension,"");
		pGlobals->InputFile = fileread(name2, access, extension); // read in the input file

		error = input(pGlobals,pFlags);
		if (pFlags->stopthemusic) {
			printf("Error in input file\n");
			continue;
		}

		error = initialize(pGlobals,pFlags);
		if (error) {
			printf("Error in initializing simulation\n");
			continue;
		}
        //onedpoisson(pGlobals,pFlags);	/*do the calculations */
	}
    if (info != NULL) free(info);
	//fileclose(8, pGlobals);

}
