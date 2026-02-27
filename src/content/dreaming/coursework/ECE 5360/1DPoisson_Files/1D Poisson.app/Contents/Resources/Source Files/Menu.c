	/* Main routine for 1D Poisson */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "1Dpoi_type.h"
#include "1Dpoi_def.h"
struct flag_type *pFlags;
struct global_type *pGlobals;

int startPoisson(struct global_type *pGlobals, struct flag_type *pFlags) {
//int main(int argc, char *argv[]) {
#if Platform != MAC
	
	int choice, choice2,ioerr_loc,infoLength,searchstart,i,filepointer;
	char access[3],extension[MAX_EXTENSION],field2[MAX_EXPRESS],
		expression[MAX_EXPRESS],message[MAX_MESSAGE],*info=NULL,*pPath;
	struct layer_type *pLayer;
	struct mesh_type *pMesh;
    
    ioerr_loc = Init_1DPoisson(pGlobals,pFlags);	
	pPath = _getcwd (pGlobals->inputPath, MAX_FILENAME); // get the directory path for the program
	strcpy(pGlobals->inputPath, pPath);
	strcat(pGlobals->inputPath,"\\");
	strcpy(pGlobals->matrlfilePath, pGlobals->inputPath);
	strcat(pGlobals->matrlfilePath,"materials.txt");		// Default materials file is the file 'materials'
	//	 open materials file
	strcpy(access,"r");
	strcpy(extension,"");
    pGlobals->fileContents = fileread(pGlobals->matrlfilePath, access, extension);
    
	ioerr_loc = checkMaterials(pGlobals,pFlags);  // check materials file version    
	if (!ioerr_loc)ioerr_loc = loadMaterials(pGlobals,pFlags);  // Load materials file
	
	if (pFlags->badversion) { // If materials file not found, ask for one
        while (pFlags->badversion) {
            strcpy(message, "Please Choose Materials File");
            alertbox (message);
			
           // [MenuHandler ChooseMtrlFile:nil];		// select a materials file
        }
    }
	
	
//#if program_type == NORMAL
	printf("\n");
	printf(" 1 D   P O I S S O N / S C H R O D I N G E R  S O L V E R\n");
	printf("\n");
	printf("Copyright 2013 Build B9004\n");
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
		//printf("(2) Choose Input File...\n");
		printf("(3) What is Current Input File?\n");
		//printf("(4) Run Poisson\n");
		printf("(5) Find Quantized States\n");
		printf("(6) Materials Menu\n");
		printf("(7) QUIT\n");
		printf("\n");
		choice=0;
	
	    scanf("%d",&choice);
	    switch(choice) {
	    case 1:
	    	selrun(pGlobals, pFlags);
			break;
	    case 2:
	    	//choose(pGlobals, pFlags);
	    	break;
	    case 3:
    		currentfile(pGlobals, pFlags);
    	break;
	    case 4:
	    	onedpoisson(pGlobals, pFlags);
	    	break;
	   	case 5:
	    	boundstates(pGlobals, pFlags);
	    	break;
	    case 6:
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
		default:
    		break; 
		}
	}
	return 0;
#endif
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