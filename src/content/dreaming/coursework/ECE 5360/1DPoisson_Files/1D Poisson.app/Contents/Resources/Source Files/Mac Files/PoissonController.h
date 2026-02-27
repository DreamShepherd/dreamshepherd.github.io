//
//  PoissonController.h
//
//  Created by Greg Snider on 5/20/13.
//
//

#import <Foundation/Foundation.h>
#import "1Dpoi_type.h"

@class CPTController;
@class BndStDialogController;

@interface PoissonController : NSObject
{
}

- (BOOL)RunNow:(struct mtrl_data_type *) mtrl_data globals:(struct global_type *) pGlobals flags:(struct flag_type *) pFlags;
@property (strong) CPTController *graphController;
@property (strong) BndStDialogController *bndStController;

@end
