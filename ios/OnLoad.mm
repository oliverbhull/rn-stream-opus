#import "NativeOpusTurboModule.h"
#import <Foundation/Foundation.h>
#import <ReactCommon/CxxTurboModuleUtils.h>

@interface OnLoad : NSObject // Class name doesn't strictly matter
@end
@implementation OnLoad

+(void) load {
    facebook::react::registerCxxModuleToGlobalModuleMap(
                                                        std::string(facebook::react::NativeOpusTurboModule::kModuleName),
                                                        [](std::shared_ptr<facebook::react::CallInvoker> jsInvoker) {
                                                            return std::make_shared<facebook::react::NativeOpusTurboModule>(jsInvoker);
                                                        }
                                                        );
}

@end