//
//  CPTController.h
//  DropPlot
//
//  Created by Greg Snider on 5/20/13.
//
//

#import <Cocoa/Cocoa.h>
#import <CorePlot/CorePlot.h>
#import "1Dpoi_type.h"

@interface CPTController : NSWindowController
{
    
    double minimumValueForXAxis, maximumValueForXAxis, minimumValueForYAxis, maximumValueForYAxis;
    double majorIntervalLengthForX, majorIntervalLengthForY;
    NSMutableArray *graphList;
    NSMutableArray *dataPoints;
    
    CPTPlotSpaceAnnotation *zoomAnnotation;
    CGPoint dragStart, dragEnd;
}
@property (strong) IBOutlet NSWindow *graphWindow;

@property (strong)IBOutlet CPTGraphHostingView *graphView;
@property (strong) CPTXYGraph *graph;
@property (readwrite) struct global_type *pGlobals;

-(void)updateGraph;
-(void)loadData;
-(IBAction)zoomIn;
-(IBAction)zoomOut;

// PDF / image export
-(IBAction)exportToPDF:(id)sender;
-(IBAction)exportToPNG:(id)sender;

@end
