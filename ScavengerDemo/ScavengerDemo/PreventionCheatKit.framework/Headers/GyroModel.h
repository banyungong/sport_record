//
//  GyroModel.h
//  PreventionCheatKit
//
//  Created by Kerwin on 2023/7/3.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface GyroModel : NSObject

- (instancetype)initWithTimestamp:(long)timestamp  X:(double)x Y:(double)y Z:(double)z;

@property (nonatomic , assign) double   x;
@property (nonatomic , assign) double   y;
@property (nonatomic , assign) double   z;
@property (nonatomic , assign) long     timestamp;

@end

@interface ScavengerResultModel : NSObject

- (instancetype)initWithTimestamp:(long)timestamp  latitude:(double)latitude longitude:(double)longitude type:(int)type;

@property (nonatomic , assign) double   latitude;
@property (nonatomic , assign) double   longitude;
@property (nonatomic , assign) int   type;
@property (nonatomic , assign) long     timestamp;

@end

NS_ASSUME_NONNULL_END
