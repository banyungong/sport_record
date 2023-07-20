//
//  GyroModel.m
//  PreventionCheatKit
//
//  Created by Kerwin on 2023/7/3.
//

#import "GyroModel.h"

@implementation GyroModel

- (instancetype)initWithTimestamp:(long)timestamp  X:(double)x Y:(double)y Z:(double)z {
    self = [super init];
    if (self) {
        self.x = x;
        self.y = y;
        self.z = z;
        self.timestamp = timestamp;
    }
    
    return  self;
}

@end


@implementation ScavengerResultModel

- (instancetype)initWithTimestamp:(long)timestamp  latitude:(double)latitude longitude:(double)longitude type:(int)type{
    self = [super init];
    if (self) {
        self.type = type;
        self.latitude = latitude;
        self.longitude = longitude;
        self.timestamp = timestamp;
    }
    
    return  self;
}

@end
