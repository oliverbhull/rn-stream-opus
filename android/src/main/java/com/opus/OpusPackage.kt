package com.opus

import com.facebook.react.TurboReactPackage
import com.facebook.react.bridge.NativeModule
import com.facebook.react.bridge.ReactApplicationContext
import com.facebook.react.module.model.ReactModuleInfo
import com.facebook.react.module.model.ReactModuleInfoProvider

class OpusPackage : TurboReactPackage() {
  override fun getModule(name: String, reactContext: ReactApplicationContext): NativeModule? {
    // Module is registered in C++
    return null
  }
  
  override fun getReactModuleInfoProvider(): ReactModuleInfoProvider {
    // For C++ TurboModules, the C++ infrastructure provides the necessary info.
    // We don't need to specify module info here.
    return ReactModuleInfoProvider { emptyMap() }
  }
} 