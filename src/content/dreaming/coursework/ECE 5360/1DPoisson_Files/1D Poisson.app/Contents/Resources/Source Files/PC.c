/* PC specific functions */
//#include <Cocoa/Cocoa.h>
#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "1Dpoi_type.h"
#include "1Dpoi_def.h"


//****************************************
/*void alertbox(char message[]) {
//****************************************


	// Show alert message
    strcat(message,"\n");
    printf("%s",message);
}*/


//*******************************************************************************************************************
int fileopen(int unit, char name[], char access[], char extension[], struct global_type *pGlobals) {
//*******************************************************************************************************************

    // Function to open a file

	int ioerr_loc;
	char name2[MAX_FILENAME];

	if (name[0] != '*') {
		strcpy(name2,name);
		strcat(name2,extension);
		strcat(name2,".txt");
		pGlobals->pFile[unit] = fopen(name2, access);
		if (pGlobals->pFile[unit] == NULL) {
			ioerr_loc = errno;
			//printf("File error %i\n",ferror);
			perror("error");
		}
		else
			ioerr_loc = 0;
	}

	else {
		printf("Enter input filename \n");
		printf("(Input file must have .txt extension.  Do not type .txt extension!):\n");
		scanf("%s", name);
		strcpy(name2,pGlobals->inputPath);
		strcat(name2,name);
		strcpy(name,name2);
		strcat(name2,".txt");
		pGlobals->pFile[unit] = fopen(name2, access);
		if (pGlobals->pFile[unit] == NULL) {
			ioerr_loc = errno;
			//printf("File error %i\n",ferror);
			perror("error");
		}
		else
			ioerr_loc = 0;
	}
	return ioerr_loc;
}

//*******************************************************************************************************************
char *fileread(char name[], char access[], char extension[]) {
//*******************************************************************************************************************

// Function to open a file
#define bufferlength 256  // max buffer length
	int ioerr_loc, i, c;
	char name2[MAX_FILENAME];
	char buffer[bufferlength];
	char *pFileData;
	FILE *pFile;

	pFileData = (char*) malloc(bufferlength*sizeof(char));
    pFileData[0] = '\0';
	strcpy(name2,name);
	strcat(name2,extension);
	//strcat(name2,".txt");
	pFile = fopen(name2, access);
	if (pFile == NULL) {
		ioerr_loc = errno;
		//printf("File error %i\n",ferror);
		perror("error");
		return NULL;
	}
	else
		ioerr_loc = 0;

	for (i=0;i<bufferlength-1 && (c=getc(pFile)) != EOF; i++) // load the first chuck of data, up to 256 bytes
		pFileData[i] = c;
    //if (c !=EOF)
        pFileData[i] = '\0';  //close off the first string
    //else
     //   pFileData[i] = EOF;

	while (c !=EOF) {
		i=0;
		for (i=0;i<bufferlength-1 && (c=getc(pFile)) != EOF ;i++) // load the next chuck of data, up to 256 bytes
			buffer[i] = c;
        //if (c !=EOF)
            buffer[i] = '\0';  //close off the string
        //else
           // buffer[i] = EOF;
		pFileData = (char *) realloc(pFileData, (strlen(pFileData)+bufferlength+1)*sizeof(char));
		strcat(pFileData, buffer);
	}

	fclose(pFile);

	return pFileData;
}


/****************************************/
void fileclose(int unit, struct global_type *pGlobals) {
/****************************************/

/* Function to close a file */

	fclose(pGlobals->pFile[unit]);
}


/***********************************************/
int getInput(int unit, char info[], struct global_type *pGlobals) {
/***********************************************/

/* Fuction to get a line of data from a file*/

	int length,c,i,loc_error;

	for (i=0;i<MAX_INPUTLINE-1 && (c=getc(pGlobals->pFile[unit])) != EOF && c != '\n' && c!= '\r';i++)
		info[i] = c;
	info[i] = '\0';
	if (c == '\n' || c == '\r') {
		c=getc(pGlobals->pFile[unit]);
		if (c != '\n' && c != '\r')
			loc_error=ungetc(c,pGlobals->pFile[unit]);
	}
	else {
		while ((c=getc(pGlobals->pFile[unit])) != EOF
			&& c != '\n' && c!= '\r') {
			;
		}
		if (c == '\n' || c == '\r') {
			c=getc(pGlobals->pFile[unit]);
			if (c != '\n' && c != '\r')
				loc_error=ungetc(c,pGlobals->pFile[unit]);

		}
	}
	info[i] = '\0';

	if (c == EOF && i == 0)
		length = EOF;
	else
		length = i;

	return length;
}


/**********************************************************/
int MaterialsFileRead(struct global_type *pGlobals) {
/**********************************************************/

    char message[MAX_MESSAGE];

	int err = checkMaterials(pGlobals->pMtrl_data);  // check materials file version
        if (err == 0)
            err = loadMaterials(pGlobals->pMtrl_data);  // Load materials file
        else if (err == 2){
            strcpy(message,"Wrong version of Material file. Please open file of proper version.");
            alertbox(message);
        }
        else{
            strcpy(message,"Error opening Materials file.");
            alertbox(message);
        }
    return err;
}

//*******************************************************************************************************************
int filewrite(struct global_type *pGlobals) {
//*******************************************************************************************************************

    // Function to open a file
#define bufferlength 256  // max buffer length
    int ioerr_loc, i, c;
    char name2[MAX_FILENAME], extension[MAX_EXTENSION],access[3];
    char buffer[bufferlength];
    char *pFileData;
    FILE *pFile;

    //pFileData = (char*) malloc(bufferlength*sizeof(char));
    strcpy(access,"w");
    strcpy(extension,"_Out");
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
    fprintf(pFile, "%s",pGlobals->OutputFile);
    fclose(pFile);

    //Now write the status file
    strcpy(access,"w");
    strcpy(name2,pGlobals->filename);
    strcpy(extension,"_Status.txt");
    strcat(name2,extension);
    pFile = fopen(name2, access);
    if (pFile == NULL) {
        ioerr_loc = errno;
        //printf("File error %i\n",ferror);
        perror("error");
    }
    else
        ioerr_loc = 0;
    fprintf(pFile, "%s",pGlobals->StatusFile);
    fclose(pFile);
    return 0; // normal return
}

//*******************************************************************************************************************
int fileCVwrite(struct global_type *pGlobals) {
//*******************************************************************************************************************

    // Function to open a file
    #define bufferlength 256  // max buffer length
    int ioerr_loc, i, c;
    char name2[MAX_FILENAME], extension[MAX_EXTENSION],access[3];
    char buffer[bufferlength];
    char *pFileData;
    FILE *pFile;

    strcpy(access,"w");
    strcpy(extension,"_CV");
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

    return 0; // normal return
}
