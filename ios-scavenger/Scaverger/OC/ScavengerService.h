//
//  ScavengerService.h
//  PreventionCheatKit
//
//  Created by Kerwin on 2023/6/29.
//

#import <Foundation/Foundation.h>
#import <PreventionCheatKit/GyroModel.h>
#import <CoreMotion/CoreMotion.h>

#define WEAKSELF(weakSelf)  __weak __typeof(&*self)weakSelf = self;

NS_ASSUME_NONNULL_BEGIN

@interface ScavengerService : NSObject

+ (instancetype)sharedInstance;

- (void)start;

- (void)clean;

-(void)filterWithTimestamp:(long)timestamp latitude:(double)latitude longitude:(double)longitude
   completion:(void (^)(ScavengerResultModel *filterResult))completion;


@end

NS_ASSUME_NONNULL_END
