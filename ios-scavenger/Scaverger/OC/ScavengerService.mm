//
//  ScavengerService.m
//  PreventionCheatKit
//
//  Created by Kerwin on 2023/6/29.
//

#import "ScavengerService.h"
#import "manager.h"
#import <memory>

@interface ScavengerService()

@property (nonatomic,strong) CMMotionManager *motionManager;

@property (nonatomic,strong) NSMutableArray *gyroDatas;
@property (nonatomic,assign) NSDate *startDate;

@property(nonatomic, strong) CMPedometer *stepCounter;
@property(nonatomic, assign) NSInteger stepCount;

@end

@implementation ScavengerService

static dispatch_once_t onceToken;
static ScavengerService *_instance = nil;

+ (instancetype)sharedInstance {
    dispatch_once(&onceToken, ^{
        _instance = [[self alloc] init];
    });
    return _instance;
}

Manager* manager;

Manager* getManager(){
    if (NULL == manager){
        manager = new Manager;
    }
    return  manager;
}


- (instancetype)init {
    if (self = [super init]) {
        _stepCounter = [[CMPedometer alloc] init];
        _gyroDatas = [NSMutableArray array];
    }
    return self;
}

- (void)start {
    
    [_gyroDatas removeAllObjects];
    
    _stepCount = 0;
    
    _startDate = [NSDate dateWithTimeIntervalSinceNow:0];
    
    [self startGyroUpdates];
}

- (void)clean {
    
//    NSLog(@"manager clean");
    
    if (_motionManager && [_motionManager isGyroActive]) {
        [_motionManager stopGyroUpdates];
    }
    
    [_stepCounter stopPedometerUpdates];
    
    _startDate = nil;
    
    getManager()->clean();
}

- (void)filterWithTimestamp:(long)timestamp latitude:(double)latitude longitude:(double)longitude completion:(void (^)(ScavengerResultModel *filterResult))completion{
    
    if (_startDate == nil) {
        [self start];
    }
    
    
    WEAKSELF(weakSelf);
    [self stepcountFrom:_startDate to:[NSDate dateWithTimeIntervalSinceNow:0] completion:^(NSInteger stepCount) {
        
//        NSLog(@"latitude>>%f longitude>>%f step>>%zd timestamp>>%ld",latitude,longitude,stepCount,timestamp);
    }];
    
//    NSLog(@"_stepCount>>%ld",_stepCount);
    
    long long value[weakSelf.gyroDatas.count][4];
    for (int i=0;i < weakSelf.gyroDatas.count ;i++) {
        GyroModel *temp = weakSelf.gyroDatas[i];
        value[i][0]= temp.x * 1000000;
        value[i][1]= temp.y * 1000000;
        value[i][2]= temp.z * 1000000;
        value[i][3]= temp.timestamp;
    }
    
    [weakSelf.gyroDatas removeAllObjects];
    
    CPoint *cpoint = new CPoint();
    cpoint->timestamp = timestamp;
    cpoint->latitude = int(latitude* 1000000);
    cpoint->longitude = int(longitude* 1000000);
    cpoint->step = int(_stepCount);
    cpoint->sensorValue = value;
    
    CPoint *resultCPoint;
    resultCPoint = getManager()->addPoint(cpoint);
    
    ScavengerResultModel *returnModel = [[ScavengerResultModel alloc]initWithTimestamp:resultCPoint->timestamp
                                                                              latitude:resultCPoint->latitude/1000000.0
                                                                             longitude:resultCPoint->longitude/1000000.0
                                                                                  type:resultCPoint->type];
    
//        dispatch_async(dispatch_get_main_queue(), ^{
        completion(returnModel);
//        });
}

//陀螺仪
- (void)startGyroUpdates {
    
    if (_motionManager) {
        [_motionManager stopGyroUpdates];
    }
    
    _motionManager = [[CMMotionManager alloc] init];
    
    /*
     CMRotationRate的X，Y,Z分别代表三个轴上的旋转速率，单位为弧度/秒
     1角度 = π/180 弧度
     1弧度 = 180/π 角度
     360角度 = 360 * π/180 = 2π弧度
     */
    
    WEAKSELF(weakSelf)
    if(_motionManager.gyroAvailable) {
        [_motionManager setGyroUpdateInterval:0.05];
        [_motionManager startGyroUpdatesToQueue:[NSOperationQueue mainQueue] withHandler:^(CMGyroData * gyroData, NSError * error) {
            CMRotationRate rotationRate = gyroData.rotationRate;
            
            double xAngle = [weakSelf clacRateAngle:rotationRate.x];
            double yAngle = [weakSelf clacRateAngle:rotationRate.y];
            double zAngle = [weakSelf clacRateAngle:rotationRate.z];
            
            NSTimeInterval timestamp = NSDate.date.timeIntervalSince1970;
            
            [weakSelf onGyroscopeChanged:xAngle withY:yAngle withZ:zAngle withTimeStamp:timestamp];
            
        }];
    } else {
        NSLog(@"gyro not available");
    }
}

//计算旋转角度
- (double)clacRateAngle:(double)rate {
    double angle = rate * 180/M_PI;
    return angle;
}

- (void)onGyroscopeChanged:(double) x withY:(double) y withZ:(double) z withTimeStamp:(long) timestamp{
    
    GyroModel *gyroModel = [[GyroModel alloc]initWithTimestamp:timestamp X:x Y:y Z:z];

    [_gyroDatas addObject:gyroModel];
}


- (void)stepcountFrom:(NSDate *)fromDate to:(NSDate *)toDate completion:(void (^)(NSInteger stepCount))completion{
    
    WEAKSELF(weakSelf)
    [_stepCounter queryPedometerDataFromDate:fromDate
                                     toDate:toDate
                                withHandler:^(CMPedometerData * _Nullable pedometerData, NSError * _Nullable error) {
        
        NSInteger systemStepCount = 0;
        if (!error && pedometerData) {
            systemStepCount = [pedometerData.numberOfSteps integerValue];
            weakSelf.stepCount = systemStepCount;
        }

//        dispatch_async(dispatch_get_main_queue(), ^{
//            completion(systemStepCount);
//        });
    }];

    
}

@end
