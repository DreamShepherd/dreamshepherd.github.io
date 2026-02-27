//
//  AppController.m
//  DropPlot
//
//  Created by Greg Snider on 5/20/13.
//
//

#import "AppController.h"
#import "CPTController.h"
#import "1Dpoi_type.h"
#import "1Dpoi_def.h"

//extern struct flag_type *pFlags;
//extern struct global_type *pGlobals;
extern struct mtrl_data_type *pMtrl_data;

@implementation AppController
@synthesize poiController;

-(void)awakeFromNib
{
    if (!poiController) {
     //poiController = [[PoissonController alloc] init];
     }
}

- (IBAction)ChooseMtrlFile:(id)sender {
	int i; // Loop counter.
	int load_error;
    const char *tempString;
    char message[MAX_MESSAGE];
    NSError *loc_error;
	
	
	
	// Create the File Open Dialog class.
	NSOpenPanel* openDlg = [NSOpenPanel openPanel];
	
	// Enable the selection of files in the dialog.
	[openDlg setCanChooseFiles:YES];
	
	// Disable the selection of directories in the dialog.
	[openDlg setCanChooseDirectories:NO];
	
	// Display the dialog.  If the OK button was pressed,
	// process the files.
	//if ( [openDlg runModalForDirectory:nil file:nil] == NSOKButton )
    if ( [openDlg runModal] == NSOKButton)
    {
		// Get an array containing the full filenames of all
		// files and directories selected.
		NSArray* files = [openDlg URLs];
		
		// Loop through all the files and process them.
		for( i = 0; i < [files count]; i++ )
		{
			NSURL *fileName = [files objectAtIndex:i];
            NSString *stringFromFileAtURL = [[NSString alloc]
                                             initWithContentsOfURL:fileName
                                             encoding:NSUTF8StringEncoding
                                             error:&loc_error];
            if (stringFromFileAtURL) {  // If new file loaded, process it
                //free(pMtrl_data->MatrlFile);
                //tempString = [stringFromFileAtURL UTF8String];
                //pMtrl_data->MatrlFile = (char *) malloc(strlen(tempString)*sizeof(char));
                //strcpy(pMtrl_data->MatrlFile, tempString);
                pMtrl_data->MatrlFile=[stringFromFileAtURL UTF8String];
                load_error = checkMaterials(pMtrl_data);  // check materials file version
                if (load_error == 1){
                    strcpy(message,"Error opening Materials file.");
                    alertbox(message);
                }
                else if (load_error == 2){
                    strcpy(message,"Wrong version of Material file.");
                    alertbox(message);
                }
                else load_error = loadMaterials(pMtrl_data);  // Load materials file
            }
		}
	}
}

@end
