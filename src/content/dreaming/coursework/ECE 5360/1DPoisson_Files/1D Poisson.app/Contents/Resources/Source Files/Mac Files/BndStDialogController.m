//
//  BndStDialogController.m
//  1D Poisson
//
//  Created by Greg Snider on 9/6/13.
//  Copyright (c) 2013 Greg Snider. All rights reserved.
//

#import "BndStDialogController.h"
#import "1Dpoi_type.h"
#import "1Dpoi_def.h"

@interface BndStDialogController ()

@end

@implementation BndStDialogController

- (id)initWithWindow:(NSWindow *)window
{
    int i;
    self = [super initWithWindow:window];
    if (self) {
        // Initialization code here.
        i=0;
    }
    
    return self;
}

- (void)windowDidLoad
{
    int i;
    [super windowDidLoad];
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
    i=0;
    if (_pFlags->schrodinger){
        [_SchStart setStringValue:[NSString stringWithFormat:@"%f", _pGlobals->pSchStart->meshcoord]];
        [_SchStop setStringValue:[NSString stringWithFormat:@"%f", _pGlobals->pSchStop->meshcoord]];
    }
    bndStDefaults(_pGlobals, _pFlags);
    
    [_eLB setStringValue:[NSString stringWithFormat:@"%f", _pGlobals->eLB]];
    [_eUB setStringValue:[NSString stringWithFormat:@"%f", _pGlobals->eUB]];
    [_hhLB setStringValue:[NSString stringWithFormat:@"%f",- _pGlobals->hhLB]];
    [_hhUB setStringValue:[NSString stringWithFormat:@"%f", -_pGlobals->hhUB]];
    [_sohLB setStringValue:[NSString stringWithFormat:@"%f", -_pGlobals->sohLB]];
    [_sohUB setStringValue:[NSString stringWithFormat:@"%f", -_pGlobals->sohUB]];
}

- (IBAction)CancelBSt:(id)sender {
}

- (IBAction)RunBSt:(id)sender {
    
    int error_loc;
    NSString *inputPathNS;
    NSMutableString *inputPathTemp;
    inputPathTemp = [[NSMutableString alloc] init];
    NSURL *outputFileURL;
    NSString *outputFile;
    
    _pGlobals->realschstart = [[_SchStart stringValue] doubleValue];
    _pGlobals->realschstop = [[_SchStop stringValue] doubleValue];
    _pGlobals->eLB = [[_eLB stringValue] doubleValue];
    _pGlobals->eUB = [[_eUB stringValue] doubleValue];
    _pGlobals->hhLB = -[[_hhLB stringValue] doubleValue];
    _pGlobals->hhUB = -[[_hhUB stringValue] doubleValue];
    _pGlobals->sohLB = -[[_sohLB stringValue] doubleValue];
    _pGlobals->sohUB = -[[_sohUB stringValue] doubleValue];
    [self close];
    
    error_loc =boundstates(_pGlobals, _pFlags); // find the boundstates
    
    // Write output files
    inputPathNS = [NSString stringWithUTF8String:_pGlobals->inputPath];
    [inputPathTemp setString:inputPathNS];
    [inputPathTemp appendString:@"_out.txt"];
    outputFileURL = [NSURL URLWithString:inputPathTemp];
    outputFile= [NSString stringWithUTF8String:_pGlobals->OutputFile];
    NSData *fileData=[outputFile dataUsingEncoding: [NSString defaultCStringEncoding] ];
    Boolean result = [fileData writeToURL:outputFileURL atomically:YES];
    
    
    [inputPathTemp setString:inputPathNS];
    [inputPathTemp appendString:@"_status.txt"];
    outputFileURL = [NSURL URLWithString:inputPathTemp];
    outputFile= [NSString stringWithUTF8String:_pGlobals->StatusFile];
    fileData=[outputFile dataUsingEncoding: [NSString defaultCStringEncoding] ];
    result = [fileData writeToURL:outputFileURL atomically:YES];
    
    //[NSApp stopModal];
}
@end
