//
//  main.m
//  1D Poisson
//
//  Created by Greg Snider on 4/30/13.
//  Copyright (c) 2013 Greg Snider. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "PoissonController.h"
#import "1Dpoi_type.h"
#import "1Dpoi_def.h"
//struct flag_type *pFlags;
//struct global_type *pGlobals;
struct mtrl_data_type *pMtrl_data;

int main(int argc, char *argv[])
{
    NSURL *matrlFileURL;
    int error_loc;
    unsigned long i,length;
    char message[MAX_MESSAGE],access[3];
    const char *pathtemp;
    
	pMtrl_data = (struct mtrl_data_type *) malloc(sizeof(struct mtrl_data_type));
    pMtrl_data->pTopMtrl = NULL;
    pMtrl_data->matrlfilePath = NULL;
    pMtrl_data->MatrlFile = NULL;
    
    pathtemp = [[[[NSBundle mainBundle] bundleURL] absoluteString] UTF8String];  //This will find path of application bundle!!!
	//Extract the file name
    length = strlen(pathtemp);
    pMtrl_data->matrlfilePath = (char *) malloc((length+MAX_FILENAME)*sizeof(char));
    strcpy(pMtrl_data->matrlfilePath,pathtemp);
    i = length-5;
    while (pMtrl_data->matrlfilePath[i] != '/'){
        i--;
    }
    while (pMtrl_data->matrlfilePath[i] != '/'){
        i--;
    }
    pMtrl_data->matrlfilePath[i+1] = '\0';
    //pMtrl_data->matrlfilePath = (char *) realloc(pMtrl_data->matrlfilePath, (strlen(pMtrl_data->matrlfilePath)+15)*sizeof(char));
	strcat(pMtrl_data->matrlfilePath,"materials.txt/");		// Default materials file is the file 'materials'
    matrlFileURL = [NSURL URLWithString:[NSString stringWithUTF8String:pMtrl_data->matrlfilePath]];
    NSData *mtrlFile = [NSData dataWithContentsOfURL:matrlFileURL];
    pMtrl_data->MatrlFile = [[[NSString alloc] initWithData:mtrlFile encoding:NSUTF8StringEncoding] UTF8String];

    error_loc = checkMaterials(pMtrl_data);  // check materials file version
	if (!error_loc)error_loc = loadMaterials(pMtrl_data);  // Load materials file
	
	if (error_loc == 1) { // If materials file not found, ask for one
        strcpy(message, "Please Choose Materials File");
        alertbox (message);
            
            //[MenuHandler ChooseMtrlFile:nil];		// select a materials file
    }
	else if (error_loc == 2) {	strcpy(message,"Wrong Version of the Materials File");
		alertbox (message);
    }
    
    
    return NSApplicationMain(argc, (const char **)argv);
}
