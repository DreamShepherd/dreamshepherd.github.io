//
//  AppController.h
//  DropPlot
//
//  Created by Greg Snider on 5/20/13.
//
//

#import <Foundation/Foundation.h>

@class CPTController;
@class PoissonController;

@interface AppController : NSObject
{
    CPTController * graphController;
}
- (IBAction)ChooseMtrlFile:(id)sender;
- (void)awakeFromNib;

@property (strong)PoissonController *poiController;

@end
