// PC Release B8e.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <direct.h>
#include <iostream>
//#include <1Dpoi_type.h>
//#include "1Dpoi_def.h"
//struct flag_type *pFlags;
//struct global_type *pGlobals;

extern "C" void startPC();

int _tmain(int argc, _TCHAR* argv[])
{
	int error_loc;
  //  char message[MAX_MESSAGE],access[3],extension[MAX_EXTENSION], *pPath;
	/*
	pGlobals = (struct global_type *) malloc(sizeof(struct global_type));
    pGlobals->pTopMtrl = NULL;
	pFlags = (struct flag_type *) malloc(sizeof(struct flag_type));
    
    error_loc = Init_1DPoisson(pGlobals,pFlags);	
	pPath = _getcwd (pGlobals->inputPath, MAX_FILENAME); // get the directory path for the program
	strcpy(pGlobals->inputPath, pPath);
	strcat(pGlobals->inputPath,"\\");
	strcat(pGlobals->matrlfilePath,"materials.txt");		// Default materials file is the file 'materials'
	//	 open materials file
	strcpy(access,"r");
	strcpy(extension,"");
    pGlobals->fileContents = fileread(pGlobals->matrlfilePath, access, extension);
    
	error_loc = checkMaterials(pGlobals,pFlags);  // check materials file version    
	if (!error_loc)error_loc = loadMaterials(pGlobals,pFlags);  // Load materials file
	
	if (pFlags->badversion) { // If materials file not found, ask for one
        while (pFlags->badversion) {
            strcpy(message, "Please Choose Materials File");
            alertbox (message);
			
           // [MenuHandler ChooseMtrlFile:nil];		// select a materials file
        }
    }*/
	startPC();
	return 0;
}

