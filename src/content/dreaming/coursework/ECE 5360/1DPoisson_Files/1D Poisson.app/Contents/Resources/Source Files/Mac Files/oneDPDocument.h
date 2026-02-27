//
//  oneDPDocument.h
//  1D Poisson
//
//  Created by Greg Snider on 4/30/13.
//  Copyright (c) 2013 Greg Snider. All rights reserved.
//

#import <Cocoa/Cocoa.h>
//@class AppController;
@class PoissonController;
@class BndStDialogController;
NSAttributedString *fileContents;
NSString *fileString;

@interface oneDPDocument : NSDocument
{
    IBOutlet NSTextView *textView;
    BOOL cleanedUp;
    struct flag_type *pFlags;
    struct global_type *pGlobals;
}


-(NSString *)setMString: (NSAttributedString *) fileContentsStr;
-(NSData *)mString: (NSString *) string;
-(IBAction)RunSimulation:(id)sender;
-(IBAction)RunQuantized:(id)sender;
@property (strong) PoissonController *poiController;
@property (strong) BndStDialogController *bndStController;

@end
