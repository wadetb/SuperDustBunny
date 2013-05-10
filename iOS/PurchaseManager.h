
#import <StoreKit/StoreKit.h>

#define kInAppPurchaseManagerProductsFetchedNotification @"kInAppPurchaseManagerProductsFetchedNotification"

@interface InAppPurchaseManager : NSObject <SKProductsRequestDelegate>
{
    SKProduct *fullUpgradeProduct;
    SKProductsRequest *productsRequest;
} 

#define kInAppPurchaseManagerTransactionFailedNotification @"kInAppPurchaseManagerTransactionFailedNotification"
#define kInAppPurchaseManagerTransactionSucceededNotification @"kInAppPurchaseManagerTransactionSucceededNotification"

…

@interface InAppPurchaseManager : NSObject <SKProductsRequestDelegate, SKPaymentTransactionObserver>
{
    …
}

- (void)loadStore;
- (BOOL)canMakePurchases;
- (void)purchaseFullUpgrade;

@end 