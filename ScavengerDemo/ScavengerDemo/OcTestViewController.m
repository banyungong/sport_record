//
//  OcTestViewController.m
//  TestProject
//
//  Created by Kerwin on 2023/7/4.
//

#import "OcTestViewController.h"
#import <PreventionCheatKit/PreventionCheatKit.h>

@interface OcTestViewController ()

@end

@implementation OcTestViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    [[ScavengerService sharedInstance]filterWithTimestamp:123 latitude:1.11 longitude:1.11 completion:^(ScavengerResultModel * _Nonnull filterResult) {
        
    }];
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
