//
//  InfoController.h
//  principia
//
//  Created by Emil on 2013-12-09.
//  Copyright (c) 2013 Bithack AB. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface InfoController : UIViewController;
@property(nonatomic, assign) NSString *txt_title;
@property(nonatomic, assign) NSString *txt_descr;


- (IBAction)done_click:(id)sender;

@end