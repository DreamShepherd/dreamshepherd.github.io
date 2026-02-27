//
//  oneDPDocument.m
//  1D Poisson
//
//  Created by Greg Snider on 4/30/13.
//  Copyright (c) 2013 Greg Snider. All rights reserved.
//

#import "oneDPDocument.h"
#import "AppController.h"
#import "PoissonController.h"
#import "BndStDialogController.h"
#import "1Dpoi_type.h"
#import "1Dpoi_def.h"


//extern struct flag_type *pFlags;
//extern struct global_type *pGlobals;
extern struct mtrl_data_type *pMtrl_data;

// Private Methods
@interface oneDPDocument()

- (void)cleanup;

@end

@implementation oneDPDocument

@synthesize poiController;
@synthesize bndStController;

- (id)init
{
    int error_loc;
    self = [super init];
    if (!poiController) {
     poiController = [[PoissonController alloc] init];
     }
    if (self) {
        // Add your subclass-specific initialization here.
    }
    return self;
}

- (NSString *)windowNibName
{
    // Override returning the nib file name of the document
    // If you need to use a subclass of NSWindowController or if your document supports multiple NSWindowControllers, you should remove this method and override -makeWindowControllers instead.
    return @"oneDPDocument";
}

- (void)windowControllerDidLoadNib:(NSWindowController *)aController
{
    [super windowControllerDidLoadNib:aController];
    [self->textView setAllowsUndo:YES];
    if (fileString !=nil) {
        [self->textView setString:fileString];
        fileContents = nil;
    }
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
    pGlobals->pTopLayer = NULL;
    pGlobals->pTopMesh = NULL;
    pGlobals->pStartSim = NULL;
    pGlobals->filename = NULL;
    pGlobals->inputPath = NULL;
    pGlobals->OutputFile = NULL;
    pGlobals->StatusFile = NULL;
    pGlobals->CVFile = NULL;
    pGlobals->ShellFile = NULL;
    
    cleanedUp = NO;
    
    // Observe NSApplication close notification
   /* [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(cleanup)
                                                 name:NSWindowWillCloseNotification
                                               object:nil];*/
}

+ (BOOL)autosavesInPlace
{
    return YES;
}

- (NSData *)dataOfType:(NSString *)typeName error:(NSError **)outError
{
    NSData *data;
    NSString *filestring;
    filestring = [self setMString:[self->textView textStorage]]; // Synchronize data model with the text storage
    //NSMutableDictionary *dict = [NSDictionary
     //                            dictionaryWithObject:NSRTFTextDocumentType
     //                            forKey:NSDocumentTypeDocumentAttribute];
    [self->textView breakUndoCoalescing];
    //data = [self->mString dataFromRange:NSMakeRange(0, [self->mString length]) documentAttributes:dict error:outError];
    data = [self mString:filestring];
    if (!data && outError) {
        *outError = [NSError errorWithDomain:NSCocoaErrorDomain
                                        code:NSFileWriteUnknownError userInfo:nil];
    }
    return data;
}

- (BOOL)readFromData:(NSData *)data ofType:(NSString *)typeName error:(NSError **)outError
{
    BOOL readSuccess = NO;
    fileContents = [[NSAttributedString alloc]
                    initWithData:data options:NULL documentAttributes:NULL
                    error:outError];
    if (!fileContents && outError) {
        *outError = [NSError errorWithDomain:NSCocoaErrorDomain
                                        code:NSFileReadUnknownError userInfo:nil];
    }
    if (fileContents) {
        readSuccess = YES;
        fileString = [self setMString:fileContents];
        [textView setString:fileString];
    }
    return readSuccess;
}

-(NSString *)setMString: (NSAttributedString *) fileContentsStr
{
    NSString *String_loc;
    String_loc = [String_loc init];
    String_loc = [fileContentsStr string];
    return String_loc;
}

-(NSData *)mString: (NSString *) string
{
    NSData *fileData=[string dataUsingEncoding: [NSString defaultCStringEncoding] ];
    return fileData;
}

-(IBAction)RunSimulation:(id)sender {
    NSURL *pathURL;
    BOOL run_error;
    char *locInputPath;
    int error_loc, i, j, length, extenPosition;
    
    if (pGlobals->filename != NULL) free(pGlobals->filename);
    if (pGlobals->inputPath != NULL) free(pGlobals->inputPath);
    
    pGlobals->InputFile = [[textView  string] UTF8String];
    pathURL = [self fileURL];
    if (pathURL == nil){
        // create the save panel
        /*NSSavePanel *panel = [NSSavePanel savePanel];
        
        // set a new file name
        [panel setNameFieldStringValue:@"NewFile.txt"];
        
        // display the panel
        NSInteger result	= [panel runModal];
        //[panel beginWithCompletionHandler:^(NSInteger result) {
            
            if (result == NSFileHandlingPanelOKButton) {
                
                // create a file namaner and grab the save panel's returned URL
                //NSFileManager *manager = [NSFileManager defaultManager];
                pathURL = [panel URL];
                
                // then copy a previous file to the new location
                //[manager copyItemAtURL:self.myURL toURL:saveURL error:nil];
                NSString *myString = [textView string];
                NSData *fileData=[myString dataUsingEncoding: [NSString defaultCStringEncoding] ];
                Boolean result = [fileData writeToURL:pathURL atomically:YES];
            }*/
        //[self saveDocumentAs:NULL];
        [self saveDocumentWithDelegate:self didSaveSelector:NULL contextInfo:NULL];
        
        
    }
    locInputPath = [[pathURL absoluteString] UTF8String];
	//Extract the file name and path
    length = (int) strlen(locInputPath);
    i = length;
    while (locInputPath[i] != '.'){
        i--;
    }
    extenPosition = i;
    i--;
    
    while (locInputPath[i] != '/'){
        i--;
    }
    pGlobals->filename = (char *) malloc((extenPosition-i+50)*sizeof(char));
    j=0;
    i++;
    while (i != extenPosition) {
        pGlobals->filename[j] = locInputPath[i];
        j++;
        i++;
    }
    pGlobals->filename[j] = '\0';
    pGlobals->inputPath = (char *) malloc((extenPosition+50)*sizeof(char));
    for (i=0; i<extenPosition; i++) {
        pGlobals->inputPath[i] = locInputPath[i];
    }
    pGlobals->inputPath[extenPosition] = '\0'; // Copy input path and filename into a permanent string (otherwise gets auto dealloc)
    //pGlobals->inputPath[length] = '\0'; // Copy input path and filename into a permanent string (otherwise gets auto dealloc)
    run_error = [self.poiController RunNow: pMtrl_data globals:pGlobals flags:pFlags];
	
    
    return;
}

-(IBAction)RunQuantized:(id)sender {
    BOOL run_error;
    
    if (!bndStController) {
        bndStController = [[BndStDialogController alloc] initWithWindowNibName:@"BndStDialogController"];
    }
    self.bndStController.pGlobals = pGlobals;
    self.bndStController.pFlags = pFlags;
    [self.bndStController showWindow:nil];
    //[self.bndStController loadWindow];
    
    return;
}


- (void)cleanup{
    //int error_loc;
    cleanedUp = YES;
    //NSString *name = [NSNotification name];
    //error_loc = Init_1DPoisson(pGlobals,pFlags);
    //free (pFlags);
	//free (pGlobals);

}

@end
