//
//  BndStDialogController.h
//  1D Poisson
//
//  Created by Greg Snider on 9/6/13.
//  Copyright (c) 2013 Greg Snider. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "1Dpoi_type.h"

@interface BndStDialogController : NSWindowController

@property   struct flag_type *pFlags;
@property   struct global_type *pGlobals;


@property (weak) IBOutlet NSTextField *SchStart;
@property (weak) IBOutlet NSTextField *SchStop;
@property (weak) IBOutlet NSTextField *eUB;
@property (weak) IBOutlet NSTextField *eLB;
@property (weak) IBOutlet NSTextField *hhUB;
@property (weak) IBOutlet NSTextField *hhLB;
@property (weak) IBOutlet NSTextField *sohUB;
@property (weak) IBOutlet NSTextField *sohLB;

- (IBAction)CancelBSt:(id)sender;
- (IBAction)RunBSt:(id)sender;
//- (BOOL)QuantizedNow:(struct global_type *) pGlobals flags:(struct flag_type *) pFlags;
@end
