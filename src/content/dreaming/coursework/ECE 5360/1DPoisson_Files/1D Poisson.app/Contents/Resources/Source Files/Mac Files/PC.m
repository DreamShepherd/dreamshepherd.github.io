/* PC specific functions */
#import <Cocoa/Cocoa.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "1Dpoi_type.h"
#include "1Dpoi_def.h"


/****************************************/
void alertbox(char message[]) {
/****************************************/

	NSString *messg;
	// Show alert message
	
	//strcat(messag,"\n");
	//printf("%s",message);
	messg = [NSString stringWithUTF8String:message];
	NSRunAlertPanel(@"Alert",messg,@"OK", nil, nil);
}


//*******************************************************************************************************************
char *fileread(char name[], char access[], char extension[]) {
//*******************************************************************************************************************
    
    // Function to open and read a file
	char *pFileData, name2[MAX_FILENAME];
	NSURL *FileURL;
    
    strcpy(name2,name);
	strcat(name2,extension);		// Add the extension to the end of the filename
    FileURL = [NSURL URLWithString:[NSString stringWithUTF8String:name2]];
    NSData *pFile = [NSData dataWithContentsOfURL:FileURL];
    pFileData = [[[NSString alloc] initWithData:pFile encoding:NSUTF8StringEncoding] UTF8String];
    
	return pFileData;
}
