//
//  URLRequestStore.h
//  SuperDustBunny
//
//  Created by Wade Brainerd on 10/1/11.
//  Copyright (c) 2011 Self. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <Foundation/NSURLConnection.h>

@interface URLRequestStore : NSObject < NSConnectionDelegate > {
    NSString *path;
    NSURLRequest *request;
}

- (id) initWithRequest:(NSURLRequest *)_request;

+ (void) submitRequest:(NSURLRequest *)request;
+ (void) processedStoredRequests;

@end
