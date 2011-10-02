//
//  URLRequestStore.m
//  SuperDustBunny
//
//  Created by Wade Brainerd on 10/1/11.
//  Copyright (c) 2011 Self. All rights reserved.
//

#import "URLRequestStore.h"

#import <CommonCrypto/CommonDigest.h>

@implementation URLRequestStore 

- (id) initWithRequest:(NSURLRequest *)_request
{
    if (self = [super init])
    {
        request = [_request retain];
        path = nil;
    }
    
    return self;
}

- (id) initWithRequest:(NSURLRequest *)_request withPath:(NSString *)_path
{
    if (self = [super init])
    {
        request = [_request retain];
        path = [_path retain];
    }
    
    return self;
}

- (void) dealloc
{
    if (request) {
        [request release];
        request = nil;
    }

    if (path) {
        [path release];
        path = nil;
    }
}

- (void) connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    NSString* resultText = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    NSLog(@"URL request response data:\n%@\n\n", resultText);
    [resultText release];
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection
{
    NSLog(@"URL request completed.\n");
    
    if (path) {
        NSLog(@"Removing stored URL request '%@'.\n", path);

        NSError *removeItemError;
        BOOL removeItemResult = [[NSFileManager defaultManager] removeItemAtPath:path error:&removeItemError];
        if (removeItemResult == NO) {
            NSLog(@"Error removing stored URL request '%@':\n%@\n", path, [removeItemError localizedDescription]);
            return;
        }
        
        [path release];
        path = nil;
    }
    
    [request release];
    request = nil;
}

- (void) connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
    NSLog(@"URL request failed: %@\n", [error localizedDescription]);

    @try {
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentDirectory = [paths objectAtIndex:0];
        
        NSString *storeDirectory = [documentDirectory stringByAppendingPathComponent:@"RequestStore"];
        if (![[NSFileManager defaultManager] fileExistsAtPath:storeDirectory])
        {
            NSLog(@"Creating stored URL request directory '%@'...\n", storeDirectory);
            NSError *createDirectoryError;
            BOOL createDirectoryResult = [[NSFileManager defaultManager] createDirectoryAtPath:storeDirectory withIntermediateDirectories:YES attributes:nil error:&createDirectoryError];
            if (createDirectoryResult == NO)
            {
                NSLog(@"Error creating stored request directory: %@\n", [createDirectoryError localizedDescription]);
                return;
            }
        }
        
        NSData *data = [request HTTPBody];
        
        unsigned char digest[16];
        CC_MD5([data bytes], (CC_LONG)[data length], digest);
        
        NSString *fileName = [NSString stringWithFormat:@"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x.request",
                              digest[0], digest[1], digest[2], digest[3], digest[4], digest[5], digest[6], digest[7], digest[8], digest[9], digest[10], digest[11], digest[12], digest[13], digest[14], digest[15]];
        
        NSString *filePath = [storeDirectory stringByAppendingPathComponent:fileName];
        
        BOOL archiveResult = [NSKeyedArchiver archiveRootObject:request toFile:filePath];    
        if (archiveResult == NO) {
            NSLog(@"Failed to archive URL request '%@'; skipping.\n", filePath);
        } else {
            NSLog(@"Stored request '%@'.\n", filePath);
        }
    }
    @catch (NSException *e)
    {
        NSLog(@"Exception while saving URL request:\n%@\n%@\n", [e name], [e reason]);
    }
    @finally {
        [request release];
        request = nil;
        
        [path release];
        path = nil;
    }
}

+ (void) submitRequest:(NSURLRequest *)request
{
    [NSURLConnection connectionWithRequest:request delegate:[[[URLRequestStore alloc] initWithRequest:request] autorelease]];    
}

+ (void) processedStoredRequests
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentDirectory = [paths objectAtIndex:0];
    
    NSString *storeDirectory = [documentDirectory stringByAppendingPathComponent:@"RequestStore"];
    if (![[NSFileManager defaultManager] fileExistsAtPath:storeDirectory])
        return;
    
    NSLog(@"Processing stored URL requests...\n");
    
    NSError *contentsError;
    NSArray *requestNames = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:storeDirectory error:&contentsError];
    if (requestNames == nil) {
        NSLog(@"Error getting contents of request store directory '%@':\n%@\n", storeDirectory, [contentsError localizedDescription]);
        return;
    }
    
    for (NSString *requestName in requestNames) {
        @try {
            NSString *requestPath = [storeDirectory stringByAppendingPathComponent:requestName];
            
            NSURLRequest *request = [NSKeyedUnarchiver unarchiveObjectWithFile:requestPath];
            if (request == nil || ![request isKindOfClass:[NSURLRequest class]]) {
                NSLog(@"Error unarchiving stored URL request '%@'; skipping.\n", requestPath);
            } else {
                NSLog(@"Submitting stored URL request '%@'.\n", requestPath);
                
                [NSURLConnection connectionWithRequest:request 
                                              delegate:[[[URLRequestStore alloc] initWithRequest:request withPath:requestPath] autorelease]];    
            }            
        }
        @catch (NSException *exception) {
            NSLog(@"Exception while processing stored URL request '%@':\n%@\n%@\n", requestName, [exception name], [exception reason]);
        }
    }
}

@end
