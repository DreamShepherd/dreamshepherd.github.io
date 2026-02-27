//
//  PoissonController.m
//
//  Created by Greg Snider on 5/20/13.
//
//

//#import "oneDPDocument.h"
#import "PoissonController.h"
#import "CPTController.h"
#import "BndStDialogController.h"
#import "1Dpoi_type.h"
#import "1Dpoi_def.h"

int solve(double loc_converg, struct global_type *pGlobals, struct flag_type *pFlags);

@implementation PoissonController

@synthesize graphController;
@synthesize bndStController;

- (BOOL)RunNow:(struct mtrl_data_type *) mtrl_data globals:(struct global_type *) pGlobals flags:(struct flag_type *) pFlags {
	BOOL error_run;
    NSString *inputPathNS;
    NSMutableString *inputPathTemp;
    inputPathTemp = [[NSMutableString alloc] init];
    NSMutableString *multiRunExtension;
    NSURL *outputFileURL;
    NSString *outputFile;
	int error_loc, filepointer,infoLength;
    unsigned long i,j,length,extenPosition;
	char *info=NULL, *inputFileTest;
	char message[MAX_MESSAGE];
    
	char extension[MAX_EXTENSION];
    int error;
    struct mesh_type *pMesh;
    
    
    inputPathNS = [NSString stringWithUTF8String:pGlobals->inputPath];
    
    // Setup simulation
    error_loc = Init_1DPoisson(pGlobals,pFlags);
	filepointer = 0;
	infoLength=getInputNew(pGlobals->InputFile, &filepointer, &info);  //Get new line from input file
	if (!strncmp(info,"#script",7)) {
        if (info != NULL) {
            free(info);
            info = NULL;
        }
		script(pGlobals,pFlags);
		return 1;
	}
	if (info != NULL) {
        free(info);
        info = NULL;
    }
    
	error_loc = input(pGlobals,pFlags); //Process input file
	if (error_loc) return YES; // If the input read failed, stop.  Otherwise initialize the arrays.
    
	error_loc = initialize(pGlobals,pFlags);  //Initialize the simulation
	if (error_loc) return YES;  // If the initialization failed, stop.  Otherwise run the simulation.
    
    
    
    
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
            //if (error_loc == 1) return YES;
            // Now start the plot
            //Is graphController nil?
            /*if (!graphController) {
                self.graphController = [[CPTController alloc] init];
                self.graphController.pGlobals = pGlobals;
                [self.graphController showWindow:nil];
            }
            else {
                self.graphController.pGlobals = pGlobals;
                [self.graphController updateGraph];
                [self.graphController.graph reloadData];
            }*/
			/*strcpy(message,"Calculation ");
			strcat(message,voltconv);
			strcat(message,"V Completed");
			if (!pFlags->silent) printf("%s",message);
			if (!pFlags->silent) printf("\n");*/
            
            // Write output files
            if (!pFlags->cvonly || pGlobals->multiRun.multiCount == 1) {
              
                [inputPathTemp setString:inputPathNS];
                multiRunExtension= [NSMutableString stringWithUTF8String:pGlobals->multiRun.extension];
                [multiRunExtension appendString:@"_out.txt"];
                [inputPathTemp appendString:multiRunExtension];
                outputFileURL = [NSURL URLWithString:inputPathTemp];
                outputFile= [NSString stringWithUTF8String:pGlobals->OutputFile];
                NSData *fileData=[outputFile dataUsingEncoding: [NSString defaultCStringEncoding] ];
                Boolean result = [fileData writeToURL:outputFileURL atomically:YES];
            
            
                [inputPathTemp setString:inputPathNS];;
                multiRunExtension= [NSMutableString stringWithUTF8String:pGlobals->multiRun.extension];
                [multiRunExtension  appendString:@"_status.txt"];
                [inputPathTemp appendString:multiRunExtension];
                outputFileURL = [NSURL URLWithString:inputPathTemp];
                outputFile= [NSString stringWithUTF8String:pGlobals->StatusFile];
                fileData=[outputFile dataUsingEncoding: [NSString defaultCStringEncoding] ];
                result = [fileData writeToURL:outputFileURL atomically:YES];
                //cleanup output file memory
                free (pGlobals->OutputFile);
                pGlobals->OutputFile = NULL;
                free(pGlobals->StatusFile);
                pGlobals->StatusFile = NULL;
            }
            if (error_loc == 1) return YES;

		} while (error_loc != 0);
        
        if (pFlags->cv) {
            // Write CV output file
            [inputPathTemp setString:inputPathNS];
            [inputPathTemp appendString:@"_CV.txt"];
            outputFileURL = [NSURL URLWithString:inputPathTemp];
            outputFile= [NSString stringWithUTF8String:pGlobals->CVFile];
            NSData *fileData=[outputFile dataUsingEncoding: [NSString defaultCStringEncoding] ];
            Boolean result = [fileData writeToURL:outputFileURL atomically:YES];
            free (pGlobals->CVFile);
            pGlobals->CVFile = NULL;
            //free (pCVdata);
        }
        strcpy(message,"Caclculation done");
        if (!pFlags->silent) alertbox(message);
	}
    
    else if (pFlags->invert) { //Find an inverse solution of fixed charge given a mobile charge distribution
        
        pMesh = pGlobals->pStartMesh;
        
        do  {
            // find the band energies for each mesh point
            pMesh->v = initBands(pMesh, pMesh->n, pMesh->p, pGlobals->kT);
        } while((pMesh = pMesh->pDownMesh) != NULL);
        invert(pGlobals, pFlags); // Find the fixed charge that corresponds to the band diagram and mobile charge
        
        // Now start the plot
        //Is graphController nil?
        if (!graphController) {
            self.graphController = [[CPTController alloc] init];
            self.graphController.pGlobals = pGlobals;
            [self.graphController showWindow:nil];
        }
        else {
            self.graphController.pGlobals = pGlobals;
            [self.graphController updateGraph];
        }
        
        // Write output files
        output(pGlobals,pFlags);		// output results
        [inputPathTemp setString:inputPathNS];
        [inputPathTemp appendString:@"_out.txt"];
        outputFileURL = [NSURL URLWithString:inputPathTemp];
        outputFile= [NSString stringWithUTF8String:pGlobals->OutputFile];
        NSData *fileData=[outputFile dataUsingEncoding: [NSString defaultCStringEncoding] ];
        Boolean result = [fileData writeToURL:outputFileURL atomically:YES];
        
        
        [inputPathTemp setString:inputPathNS];
        [inputPathTemp appendString:@"_status.txt"];
        outputFileURL = [NSURL URLWithString:inputPathTemp];
        outputFile= [NSString stringWithUTF8String:pGlobals->StatusFile];
        fileData=[outputFile dataUsingEncoding: [NSString defaultCStringEncoding] ];
        result = [fileData writeToURL:outputFileURL atomically:YES];
        //cleanup output file memory
        free (pGlobals->OutputFile);
        pGlobals->OutputFile = NULL;
        free(pGlobals->StatusFile);
        pGlobals->StatusFile = NULL;
    }
    
    else {
        error = solve(pGlobals->converg,pGlobals, pFlags);		// Normal run, nothing fancy
        
        if (error) {
			strcpy(extension,"");
			output(pGlobals,pFlags);		// output results, error during run
			return 1;
        }
        strcpy(extension,"");
        output(pGlobals,pFlags);		// output results, normal run
        // Check to see if find all boundstates is requested
        if (pFlags->findBst) {
            if (!(pFlags->useBstDefaults || pFlags->BndStRangeSet)) { //Don't use defaults, ask in dialog
                if (!bndStController) {  //Is bndStController nil?
                    self.bndStController = [[BndStDialogController alloc] initWithWindowNibName:@"BndStDialogController"];
                }
                self.bndStController.pGlobals = pGlobals;
                self.bndStController.pFlags = pFlags;
                [self.bndStController showWindow:nil];
            }
            else { //use defaults
                bndStDefaults(pGlobals, pFlags);
                error_loc =boundstates(pGlobals, pFlags); // find the boundstates
            }
        }
		//if (!pFlags->silent) printf("\n");
		//strcpy(message,"Calculation Completed");
		//if (!pFlags->silent) alertbox(message);
		//if (!pFlags->silent) printf("\n");
		if (pFlags->DangerWill && pFlags->ignoreDanger) {
			strcpy(message,"Final structure has a confinement problem in the Schrodinger mesh");
			if (!pFlags->silent) alertbox(message);
        }
        
        // Now start the plot
        //Is graphController nil?
        if (!graphController) {
            self.graphController = [[CPTController alloc] init];
            self.graphController.pGlobals = pGlobals;
            [self.graphController showWindow:nil];
        }
        else {
            self.graphController.pGlobals = pGlobals;
            [self.graphController updateGraph];
        }
        
        // Write output files
        [inputPathTemp setString:inputPathNS];
        [inputPathTemp appendString:@"_out.txt"];
        outputFileURL = [NSURL URLWithString:inputPathTemp];
        outputFile= [NSString stringWithUTF8String:pGlobals->OutputFile];
        NSData *fileData=[outputFile dataUsingEncoding: [NSString defaultCStringEncoding] ];
        Boolean result = [fileData writeToURL:outputFileURL atomically:YES];
        
        
        [inputPathTemp setString:inputPathNS];
        [inputPathTemp appendString:@"_status.txt"];
        outputFileURL = [NSURL URLWithString:inputPathTemp];
        outputFile= [NSString stringWithUTF8String:pGlobals->StatusFile];
        fileData=[outputFile dataUsingEncoding: [NSString defaultCStringEncoding] ];
        result = [fileData writeToURL:outputFileURL atomically:YES];
        
        //cleanup output file memory
        free (pGlobals->OutputFile);
        pGlobals->OutputFile = NULL;
        free(pGlobals->StatusFile);
        pGlobals->StatusFile = NULL;
         
	}
    
    
    //free (pFlags);
	//free (pGlobals);
    return NO;
    
}
@end
