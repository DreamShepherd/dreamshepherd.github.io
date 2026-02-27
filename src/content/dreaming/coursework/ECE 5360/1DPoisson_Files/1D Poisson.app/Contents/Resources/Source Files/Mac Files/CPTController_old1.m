//
//  CPTController.m
//  DropPlot
//
//  Created by Greg Snider on 5/20/13.
//
//

#import "CPTController.h"
#import "1Dpoi_type.h"
#import "1Dpoi_def.h"

@implementation CPTController
@synthesize graphView;
@synthesize graphWindow;
@synthesize graph;
@synthesize pGlobals;

- (id)init
{
    self = [super initWithWindowNibName:@"CPTPlot"];
    //self = [super init];
    if ( self ) {
        dataPoints     = [[NSMutableArray alloc] init];
        zoomAnnotation = nil;
        dragStart      = CGPointZero;
        dragEnd        = CGPointZero;
    }
    return self;
}

- (void)windowDidLoad
{
	//extern struct global_type *pGlobals;
	struct mesh_type *pMesh;
    NSString *windowName;
    
    [super windowDidLoad];
    NSLog(@"Nib file is loaded");
    
    windowName = [NSString stringWithUTF8String:pGlobals->filename];
    [graphWindow setTitle:[windowName stringByAppendingString:@"_Plot"]];
    
    minimumValueForXAxis = MAXFLOAT;
    maximumValueForXAxis = -MAXFLOAT;
    
    minimumValueForYAxis = MAXFLOAT;
    maximumValueForYAxis = -MAXFLOAT;
    
	pMesh = pGlobals->pStartSim;
    do {
        //[fileContents getParagraphStart:&lineStart end:&lineEnd contentsEnd:&contentsEnd forRange:NSMakeRange(lineEnd, 0)];
        //currentRange = NSMakeRange(lineStart, contentsEnd - lineStart);
        //NSArray *columnValues = [[fileContents substringWithRange:currentRange] arrayByParsingCSVLine];
        
        double xValue = pMesh->meshcoord;
        double yValue = pMesh->v+pMesh->offset;
        if ( xValue < minimumValueForXAxis ) {
            minimumValueForXAxis = xValue;
        }
        if ( xValue > maximumValueForXAxis ) {
            maximumValueForXAxis = xValue;
        }
        if ( yValue < minimumValueForYAxis ) {
            minimumValueForYAxis = yValue;
        }
        if ( yValue > maximumValueForYAxis ) {
            maximumValueForYAxis = yValue;
        }
        
#ifdef USE_NSDECIMAL
        [dataPoints addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:[NSDecimalNumber decimalNumberWithString:[columnValues objectAtIndex:0]], @"x", [NSDecimalNumber decimalNumberWithString:[columnValues objectAtIndex:1]], @"y", nil]];
#else
        [dataPoints addObject:[NSMutableDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithDouble:xValue], @"x", [NSNumber numberWithDouble:yValue], @"y", nil]];
#endif
        // Create a dictionary of the items, keyed to the header titles
        //			NSDictionary *keyedImportedItems = [[NSDictionary alloc] initWithObjects:columnValues forKeys:columnHeaders];
        // Process this
    } while ((pMesh = pMesh->pDownMesh) != pGlobals->pBottomMesh);
    
    majorIntervalLengthForX = (maximumValueForXAxis - minimumValueForXAxis) / 5.0;
    if ( majorIntervalLengthForX > 0.0 ) {
        majorIntervalLengthForX = pow( 10.0, ceil( log10(majorIntervalLengthForX) ) );
    }
    
    majorIntervalLengthForY = (maximumValueForYAxis - minimumValueForYAxis) / 10.0;
    if ( majorIntervalLengthForY > 0.0 ) {
        majorIntervalLengthForY = pow( 10.0, ceil( log10(majorIntervalLengthForY) ) );
    }
    
    minimumValueForXAxis = floor(minimumValueForXAxis / majorIntervalLengthForX) * majorIntervalLengthForX;
    minimumValueForYAxis = floor(minimumValueForYAxis / majorIntervalLengthForY) * majorIntervalLengthForY;
    
    // Create graph from theme
    graph = [(CPTXYGraph *)[CPTXYGraph alloc] initWithFrame:CGRectZero];
    //CPTTheme *theme = [CPTTheme themeNamed:kCPTDarkGradientTheme];
    CPTTheme *theme = [CPTTheme themeNamed:kCPTPlainWhiteTheme];
    [graph applyTheme:theme];
    self.graphView.hostedGraph = graph;
    
    graph.paddingLeft   = 0.0;
    graph.paddingTop    = 0.0;
    graph.paddingRight  = 0.0;
    graph.paddingBottom = 0.0;
    
    graph.plotAreaFrame.paddingLeft   = 55.0;
    graph.plotAreaFrame.paddingTop    = 40.0;
    graph.plotAreaFrame.paddingRight  = 40.0;
    graph.plotAreaFrame.paddingBottom = 35.0;
    
    graph.plotAreaFrame.plotArea.fill = graph.plotAreaFrame.fill;
    graph.plotAreaFrame.fill          = nil;
    
    graph.plotAreaFrame.borderLineStyle = nil;
    graph.plotAreaFrame.cornerRadius    = 0.0;
    
    // Setup plot space
    CPTXYPlotSpace *plotSpace = (CPTXYPlotSpace *)graph.defaultPlotSpace;
    plotSpace.xRange = [CPTPlotRange plotRangeWithLocation:CPTDecimalFromDouble(minimumValueForXAxis)
                                                    length:CPTDecimalFromDouble(ceil( (maximumValueForXAxis - minimumValueForXAxis) / majorIntervalLengthForX ) * majorIntervalLengthForX)];
    plotSpace.yRange = [CPTPlotRange plotRangeWithLocation:CPTDecimalFromDouble(minimumValueForYAxis)
                                                    length:CPTDecimalFromDouble(ceil( (maximumValueForYAxis - minimumValueForYAxis) / majorIntervalLengthForY ) * majorIntervalLengthForY)];
    
    // this allows the plot to respond to mouse events
    [plotSpace setDelegate:self];
    [plotSpace setAllowsUserInteraction:YES];
    
    CPTXYAxisSet *axisSet = (CPTXYAxisSet *)graph.axisSet;
    
    CPTXYAxis *x = axisSet.xAxis;
    x.minorTicksPerInterval = 9;
    x.majorIntervalLength   = CPTDecimalFromDouble(majorIntervalLengthForX);
    x.labelOffset           = 5.0;
    x.axisConstraints       = [CPTConstraints constraintWithLowerOffset:0.0];
    
    CPTXYAxis *y = axisSet.yAxis;
    y.minorTicksPerInterval = 9;
    y.majorIntervalLength   = CPTDecimalFromDouble(majorIntervalLengthForY);
    y.labelOffset           = 5.0;
    y.axisConstraints       = [CPTConstraints constraintWithLowerOffset:0.0];
    
    // Create the main plot for the delimited data
    //CPTScatterPlot *dataSourceLinePlot = [[(CPTScatterPlot *)[CPTScatterPlot alloc] initWithFrame:graph.bounds] autorelease];
    CPTScatterPlot *dataSourceLinePlot = [(CPTScatterPlot *)[CPTScatterPlot alloc] initWithFrame:graph.bounds];
    dataSourceLinePlot.identifier = @"Data Source Plot";
    
    //CPTMutableLineStyle *lineStyle = [[dataSourceLinePlot.dataLineStyle mutableCopy] autorelease];
    CPTMutableLineStyle *lineStyle = [dataSourceLinePlot.dataLineStyle mutableCopy];
    lineStyle.lineWidth              = 1.0;
    lineStyle.lineColor              = [CPTColor blackColor];
    dataSourceLinePlot.dataLineStyle = lineStyle;
    
    dataSourceLinePlot.dataSource = self;
    [graph addPlot:dataSourceLinePlot];
}

#pragma mark -
#pragma mark Zoom Methods

-(IBAction)zoomIn
{
    CPTXYPlotSpace *plotSpace = (CPTXYPlotSpace *)graph.defaultPlotSpace;
    CPTPlotArea *plotArea     = graph.plotAreaFrame.plotArea;
    
    // convert the dragStart and dragEnd values to plot coordinates
    CGPoint dragStartInPlotArea = [graph convertPoint:dragStart toLayer:plotArea];
    CGPoint dragEndInPlotArea   = [graph convertPoint:dragEnd toLayer:plotArea];
    
    double start[2], end[2];
    
    // obtain the datapoints for the drag start and end
    [plotSpace doublePrecisionPlotPoint:start forPlotAreaViewPoint:dragStartInPlotArea];
    [plotSpace doublePrecisionPlotPoint:end forPlotAreaViewPoint:dragEndInPlotArea];
    
    // recalculate the min and max values
    minimumValueForXAxis = MIN(start[CPTCoordinateX], end[CPTCoordinateX]);
    maximumValueForXAxis = MAX(start[CPTCoordinateX], end[CPTCoordinateX]);
    minimumValueForYAxis = MIN(start[CPTCoordinateY], end[CPTCoordinateY]);
    maximumValueForYAxis = MAX(start[CPTCoordinateY], end[CPTCoordinateY]);
    
    // now adjust the plot range and axes
    plotSpace.xRange = [CPTPlotRange plotRangeWithLocation:CPTDecimalFromDouble(minimumValueForXAxis)
                                                    length:CPTDecimalFromDouble(maximumValueForXAxis - minimumValueForXAxis)];
    plotSpace.yRange = [CPTPlotRange plotRangeWithLocation:CPTDecimalFromDouble(minimumValueForYAxis)
                                                    length:CPTDecimalFromDouble(maximumValueForYAxis - minimumValueForYAxis)];
    
    CPTXYAxisSet *axisSet = (CPTXYAxisSet *)graph.axisSet;
    axisSet.xAxis.labelingPolicy = CPTAxisLabelingPolicyAutomatic;
    axisSet.yAxis.labelingPolicy = CPTAxisLabelingPolicyAutomatic;
}

-(IBAction)zoomOut
{
    double xval, yval;
    
    minimumValueForXAxis = MAXFLOAT;
    maximumValueForXAxis = -MAXFLOAT;
    
    minimumValueForYAxis = MAXFLOAT;
    maximumValueForYAxis = -MAXFLOAT;
    
    // get the ful range min and max values
    for ( NSDictionary *xyValues in dataPoints ) {
        xval = [[xyValues valueForKey:@"x"] doubleValue];
        
        minimumValueForXAxis = fmin(xval, minimumValueForXAxis);
        maximumValueForXAxis = fmax(xval, maximumValueForXAxis);
        
        yval = [[xyValues valueForKey:@"y"] doubleValue];
        
        minimumValueForYAxis = fmin(yval, minimumValueForYAxis);
        maximumValueForYAxis = fmax(yval, maximumValueForYAxis);
    }
    
    minimumValueForXAxis = floor(minimumValueForXAxis / majorIntervalLengthForX) * majorIntervalLengthForX;
    minimumValueForYAxis = floor(minimumValueForYAxis / majorIntervalLengthForY) * majorIntervalLengthForY;
    
    // now adjust the plot range and axes
    CPTXYPlotSpace *plotSpace = (CPTXYPlotSpace *)graph.defaultPlotSpace;
    
    plotSpace.xRange = [CPTPlotRange plotRangeWithLocation:CPTDecimalFromDouble(minimumValueForXAxis)
                                                    length:CPTDecimalFromDouble(ceil( (maximumValueForXAxis - minimumValueForXAxis) / majorIntervalLengthForX ) * majorIntervalLengthForX)];
    plotSpace.yRange = [CPTPlotRange plotRangeWithLocation:CPTDecimalFromDouble(minimumValueForYAxis)
                                                    length:CPTDecimalFromDouble(ceil( (maximumValueForYAxis - minimumValueForYAxis) / majorIntervalLengthForY ) * majorIntervalLengthForY)];
    CPTXYAxisSet *axisSet = (CPTXYAxisSet *)graph.axisSet;
    axisSet.xAxis.labelingPolicy = CPTAxisLabelingPolicyFixedInterval;
    axisSet.yAxis.labelingPolicy = CPTAxisLabelingPolicyFixedInterval;
}

#pragma mark -
#pragma mark PDF / image export

-(IBAction)exportToPDF:(id)sender;
{
    NSSavePanel *pdfSavingDialog = [NSSavePanel savePanel];
    [pdfSavingDialog setAllowedFileTypes:[NSArray arrayWithObject:@"pdf"]];
    
    if ( [pdfSavingDialog runModal] == NSOKButton ) {
        NSData *dataForPDF = [graph dataForPDFRepresentationOfLayer];
        [dataForPDF writeToURL:[pdfSavingDialog URL] atomically:NO];
    }
}

-(IBAction)exportToPNG:(id)sender;
{
    NSSavePanel *pngSavingDialog = [NSSavePanel savePanel];
    [pngSavingDialog setAllowedFileTypes:[NSArray arrayWithObject:@"png"]];
    
    if ( [pngSavingDialog runModal] == NSOKButton ) {
        NSImage *image            = [graph imageOfLayer];
        NSData *tiffData          = [image TIFFRepresentation];
        NSBitmapImageRep *tiffRep = [NSBitmapImageRep imageRepWithData:tiffData];
        NSData *pngData           = [tiffRep representationUsingType:NSPNGFileType properties:nil];
        [pngData writeToURL:[pngSavingDialog URL] atomically:NO];
    }
}

#pragma mark -
#pragma mark Plot Data Source Methods

-(NSUInteger)numberOfRecordsForPlot:(CPTPlot *)plot
{
    return [dataPoints count];
}

-(NSNumber *)numberForPlot:(CPTPlot *)plot field:(NSUInteger)fieldEnum recordIndex:(NSUInteger)index
{
    NSString *key = (fieldEnum == CPTScatterPlotFieldX ? @"x" : @"y");
    NSNumber *num = [[dataPoints objectAtIndex:index] valueForKey:key];
    
    return num;
}


#pragma mark -
#pragma mark Plot Space Delegate Methods

-(BOOL)plotSpace:(CPTPlotSpace *)space shouldHandlePointingDeviceDraggedEvent:(id)event atPoint:(CGPoint)interactionPoint
{
    if ( zoomAnnotation ) {
        CPTPlotArea *plotArea = graph.plotAreaFrame.plotArea;
        CGRect plotBounds     = plotArea.bounds;
        
        // convert the dragStart and dragEnd values to plot coordinates
        CGPoint dragStartInPlotArea = [graph convertPoint:dragStart toLayer:plotArea];
        CGPoint dragEndInPlotArea   = [graph convertPoint:interactionPoint toLayer:plotArea];
        
        // create the dragrect from dragStart to the current location
        CGFloat endX      = MAX( MIN( dragEndInPlotArea.x, CGRectGetMaxX(plotBounds) ), CGRectGetMinX(plotBounds) );
        CGFloat endY      = MAX( MIN( dragEndInPlotArea.y, CGRectGetMaxY(plotBounds) ), CGRectGetMinY(plotBounds) );
        CGRect borderRect = CGRectMake( dragStartInPlotArea.x, dragStartInPlotArea.y,
                                       (endX - dragStartInPlotArea.x),
                                       (endY - dragStartInPlotArea.y) );
        
        zoomAnnotation.contentAnchorPoint = CGPointMake(dragEndInPlotArea.x >= dragStartInPlotArea.x ? 0.0 : 1.0,
                                                        dragEndInPlotArea.y >= dragStartInPlotArea.y ? 0.0 : 1.0);
        zoomAnnotation.contentLayer.frame = borderRect;
    }
    
    return NO;
}

-(BOOL)plotSpace:(CPTPlotSpace *)space shouldHandlePointingDeviceDownEvent:(id)event atPoint:(CGPoint)interactionPoint
{
    if ( !zoomAnnotation ) {
        dragStart = interactionPoint;
        
        CPTPlotArea *plotArea       = graph.plotAreaFrame.plotArea;
        CGPoint dragStartInPlotArea = [graph convertPoint:dragStart toLayer:plotArea];
        
        if ( CGRectContainsPoint(plotArea.bounds, dragStartInPlotArea) ) {
            // create the zoom rectangle
            // first a bordered layer to draw the zoomrect
            CPTBorderedLayer *zoomRectangleLayer = [[CPTBorderedLayer alloc] initWithFrame:CGRectNull];
            
            CPTMutableLineStyle *lineStyle = [CPTMutableLineStyle lineStyle];
            lineStyle.lineColor                = [CPTColor darkGrayColor];
            lineStyle.lineWidth                = 1.0;
            zoomRectangleLayer.borderLineStyle = lineStyle;
            
            CPTColor *transparentFillColor = [[CPTColor blueColor] colorWithAlphaComponent:0.2];
            zoomRectangleLayer.fill = [CPTFill fillWithColor:transparentFillColor];
            
            double start[2];
            [graph.defaultPlotSpace doublePrecisionPlotPoint:start forPlotAreaViewPoint:dragStartInPlotArea];
            NSArray *anchorPoint = [NSArray arrayWithObjects:
                                    [NSNumber numberWithDouble:start[CPTCoordinateX]],
                                    [NSNumber numberWithDouble:start[CPTCoordinateY]],
                                    nil];
            
            // now create the annotation
            zoomAnnotation              = [[CPTPlotSpaceAnnotation alloc] initWithPlotSpace:graph.defaultPlotSpace anchorPlotPoint:anchorPoint];
            zoomAnnotation.contentLayer = zoomRectangleLayer;
            
            [graph.plotAreaFrame.plotArea addAnnotation:zoomAnnotation];
        }
    }
    
    return NO;
}

-(BOOL)plotSpace:(CPTPlotSpace *)space shouldHandlePointingDeviceUpEvent:(id)event atPoint:(CGPoint)interactionPoint
{
    if ( zoomAnnotation ) {
        dragEnd = interactionPoint;
        
        // double-click to completely zoom out
        if ( [event clickCount] == 2 ) {
            CPTPlotArea *plotArea     = graph.plotAreaFrame.plotArea;
            CGPoint dragEndInPlotArea = [graph convertPoint:interactionPoint toLayer:plotArea];
            
            if ( CGRectContainsPoint(plotArea.bounds, dragEndInPlotArea) ) {
                [self zoomOut];
            }
        }
        else if ( !CGPointEqualToPoint(dragStart, dragEnd) ) {
            // no accidental drag, so zoom in
            [self zoomIn];
        }
        
        // and we're done with the drag
        [graph.plotAreaFrame.plotArea removeAnnotation:zoomAnnotation];
        //[zoomAnnotation release];
        zoomAnnotation = nil;
        
        dragStart = CGPointZero;
        dragEnd   = CGPointZero;
    }
    
    return NO;
}

-(BOOL)plotSpace:(CPTPlotSpace *)space shouldHandlePointingDeviceCancelledEvent:(id)event atPoint:(CGPoint)interactionPoint
{
    if ( zoomAnnotation ) {
        [graph.plotAreaFrame.plotArea removeAnnotation:zoomAnnotation];
        //[zoomAnnotation release];
        zoomAnnotation = nil;
        
        dragStart = CGPointZero;
        dragEnd   = CGPointZero;
    }
    
    return NO;
}
@end
