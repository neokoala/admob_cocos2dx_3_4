//
//  GADInMobiExtras.h
//
//  Copyright (c) 2013 InMobi. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "GADAdNetworkExtras.h"
#import "InMobi.h"

#define ADMOB_ADAPTER_VERSION  @"4.5.0"

@interface GADInMobiExtras : NSObject <GADAdNetworkExtras>

#pragma mark Optional Parameters for targeted advertising during an Ad Request
/**
 * User income in USD.
 */
@property (nonatomic, assign) NSUInteger income;
/**
 * Age of the user may be used to deliver more relevant ads.
 */
@property (nonatomic, assign) NSUInteger age;
/**
 * Postal code of the user may be used to deliver more relevant ads.
 */
@property (nonatomic, copy) NSString *postalCode;
/**
 * Area code of the user may be used to deliver more relevant ads.
 */
@property (nonatomic, copy) NSString *areaCode;
/**
 * Education of the user may be used to deliver more relevant ads.
 */
@property (nonatomic, assign) IMEducation *educationType;
/**
 * Ethnicity of the user may be used to deliver more relevant ads.
 */
@property (nonatomic, assign) IMEthnicity *ethnicityType;
/**
 * Marital status of the user may be used to deliver more relevant ads.
 */
@property (nonatomic, assign) IMMaritalStatus *maritalStatus;
/**
 * Set whether user has any children, may be used to deliver more relevant ads.
 */
@property (nonatomic, assign) IMHasChildren *hasChildren;
/**
 * Sexual orientation of the user may be used to deliver more relevant ads.
 */
@property (nonatomic, assign) IMSexualOrientation *sexualOrientation;
/**
 * Language preference of the user may be used to deliver more relevant ads.
 */
@property (nonatomic, copy) NSString *language;

#pragma mark Setting Contextual Information
/**
 * Use contextually relevant strings to deliver more relevant ads.
 * Example: @"offers sale shopping"
 */
@property (nonatomic, copy) NSString *keywords;
/**
 * Ref-tag key to be passed to an ad instance.
 */
@property (nonatomic,copy) NSString *refTagKey;
/**
 * Ref-tag value to be passed to an ad instance.
 */
@property (nonatomic,copy) NSString *refTagValue;
/**
 * Use contextually relevant strings to deliver more relevant ads.
 * Example: @"cars bikes racing"
 */
@property (nonatomic, copy) NSString *interests;
/**
 * Provide additional values to be passed in the ad request as key-value pair.
 */
@property (nonatomic, retain) NSDictionary *additionalParameters;

#pragma mark Setting User Location
/**
 * Provide user's city in the format "city-state-country" for
 * city-level targetting.
 */
- (void)setLocationWithCity:(NSString *)_city
                      state:(NSString *)_state
                    country:(NSString *)_country;

#pragma mark Setting User IDs
/**
 * User ids such as facebook, twitter, etc may be provided to deliver more
 * relevant ids.
 */
@property (nonatomic, copy) NSString *loginId;
/**
 * Useful for maintaining different sessions with same login id.
 */
@property (nonatomic, copy) NSString *sessionId;

#pragma mark Setting Device Id Mask
/**
 * Device Id Mask to give flexibility on what device ids to be excluded from
 * InMobi device id collection.
 */
@property (nonatomic, assign) IMDeviceIdMask deviceIdMask;

/**
 * Returns the InMobi Admob adapter version
 */
+ (NSString *)adapterVersion;

@end

