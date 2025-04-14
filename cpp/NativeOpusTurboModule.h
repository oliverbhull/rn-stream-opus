#pragma once

#include <jsi/jsi.h>
#include <ReactCommon/CallInvoker.h>
#include <vector>
#include <string>

#if __has_include(<React-Codegen/RNOpusSpecJSI.h>)
#include <React-Codegen/RNOpusSpecJSI.h>
#elif __has_include("RNOpusSpecJSI.h")
#include "RNOpusSpecJSI.h"
#else
#error "Could not find RNOpusSpecJSI.h"
#endif

// Include opus headers
#if __has_include("opus/opus.h")
#include "opus/opus.h"
#elif __has_include("opus.h") // Fallback if directly available
#include "opus.h"
#else
#error "Could not find opus.h"
#endif

namespace facebook::react {
class NativeOpusTurboModule: public NativeOpusTurboModuleCxxSpec<NativeOpusTurboModule> {
public:
    static constexpr const char* kModuleName = "OpusTurbo";
    
    NativeOpusTurboModule(std::shared_ptr<CallInvoker> jsInvoker);
    ~NativeOpusTurboModule();
    
    jsi::Value decodeMultipleOpusPackets(jsi::Runtime &rt, std::string packetsBase64, double packetSize);
    jsi::Value resetDecoderState(jsi::Runtime &rt);
    jsi::Value saveDecodedDataAsWav(jsi::Runtime &rt, std::string decodedDataBase64, std::string filepath, double sampleRate, double channels);

private:
    static std::string base64_encode(const std::vector<uint8_t>& input);
    static std::vector<uint8_t> base64_decode(const std::string& input);
        
    OpusDecoder* opusDecoder = nullptr;
    static constexpr opus_int32 DEFAULT_SAMPLE_RATE = 16000;
    static constexpr int DEFAULT_CHANNELS = 1;
};

} // namespace facebook::react
