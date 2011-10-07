//
//  URLRequestStore.h
//  SuperDustBunny
//
//  Created by Wade Brainerd on 10/1/11.
//  Copyright (c) 2011 Self. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <Foundation/NSURLConnection.h>

#if TARGET_OS_IPHONE
@interface URLRequestStore : NSObject < NSURLConnectionDelegate > {
#else
@interface URLRequestStore : NSObject < NSConnectionDelegate > {
#endif
    NSString *path;
    NSURLRequest *request;
}

- (id) initWithRequest:(NSURLRequest *)_request;

+ (void) submitRequest:(NSURLRequest *)request;
+ (void) processStoredRequests;

@end
