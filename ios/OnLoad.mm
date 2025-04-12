#import "NativeOpusTurboModule.h"
#import <Foundation/Foundation.h>
#import <ReactCommon/CxxTurboModuleUtils.h>

@interface OpusTurboModuleRegistration : NSObject // Class name doesn't strictly matter
@end

@implementation OpusTurboModuleRegistration

+ (void)load {
    // Use the kModuleName defined in your C++ header
    const char *moduleName = facebook::react::NativeOpusTurboModule::kModuleName;

    // Register the C++ module factory
    facebook::react::registerCxxModuleToGlobalModuleMap(
        std::string(moduleName),
        [](std::shared_ptr<facebook::react::CallInvoker> jsInvoker) {
            // Return a shared pointer to a new instance of your C++ module
            return std::make_shared<facebook::react::NativeOpusTurboModule>(jsInvoker);
        }
    );
}

@end