#include "NativeOpusTurboModule.h"
#include <stdexcept> // For runtime_error
#include <chrono> // For timing
#include <vector> // Ensure vector is included

namespace facebook::react {

// Constructor: Create the single decoder instance
NativeOpusTurboModule::NativeOpusTurboModule(std::shared_ptr<CallInvoker> jsinvoker)
    : NativeOpusTurboModuleCxxSpec(std::move(jsinvoker)) {
    int error = 0;
    opusDecoder = opus_decoder_create(DEFAULT_SAMPLE_RATE, DEFAULT_CHANNELS, &error);
    if (error != OPUS_OK || !opusDecoder) {
        // Handle error appropriately - maybe log or throw an exception
        // For now, we'll leave it null, and methods will check
        opusDecoder = nullptr;
        // Consider logging: fprintf(stderr, "Failed to create Opus decoder: %s\n", opus_strerror(error));
    }
}

// Destructor: Clean up the decoder
NativeOpusTurboModule::~NativeOpusTurboModule() {
    if (opusDecoder) {
        opus_decoder_destroy(opusDecoder);
        opusDecoder = nullptr;
    }
    if (streamDecoder) {
        opus_decoder_destroy(streamDecoder);
        streamDecoder = nullptr;
    }
}

// Base64 encoding/decoding utility methods
std::string NativeOpusTurboModule::base64_encode(const std::vector<uint8_t>& input) {
    static const char* encoding_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    static const int mod_table[] = {0, 2, 1};
    
    size_t input_length = input.size();
    size_t output_length = 4 * ((input_length + 2) / 3);
    
    std::string encoded_data(output_length, '\0');
    size_t i, j;
    
    for (i = 0, j = 0; i < input_length;) {
        uint32_t octet_a = i < input_length ? input[i++] : 0;
        uint32_t octet_b = i < input_length ? input[i++] : 0;
        uint32_t octet_c = i < input_length ? input[i++] : 0;
        
        uint32_t triple = (octet_a << 16) + (octet_b << 8) + octet_c;
        
        encoded_data[j++] = encoding_table[(triple >> 18) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 12) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 6) & 0x3F];
        encoded_data[j++] = encoding_table[triple & 0x3F];
    }
    
    for (i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[output_length - 1 - i] = '=';
    
    return encoded_data;
}

std::vector<uint8_t> NativeOpusTurboModule::base64_decode(const std::string& input) {
    static const int decoding_table[256] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1,
        -1, -1, -1, -1, -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28,
        29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
        49, 50, 51, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };
    
    size_t input_length = input.size();
    if (input_length % 4 != 0) return {};
    
    size_t output_length = input_length / 4 * 3;
    if (input.length() >= 1 && input[input_length - 1] == '=') output_length--;
    if (input.length() >= 2 && input[input_length - 2] == '=') output_length--;
    
    std::vector<uint8_t> decoded_data(output_length);
    
    for (size_t i = 0, j = 0; i < input_length;) {
        uint32_t sextet_a = input[i] == '=' ? 0 & i++ : decoding_table[static_cast<int>(input[i++])];
        uint32_t sextet_b = input[i] == '=' ? 0 & i++ : decoding_table[static_cast<int>(input[i++])];
        uint32_t sextet_c = input[i] == '=' ? 0 & i++ : decoding_table[static_cast<int>(input[i++])];
        uint32_t sextet_d = input[i] == '=' ? 0 & i++ : decoding_table[static_cast<int>(input[i++])];
        
        uint32_t triple = (sextet_a << 18) + (sextet_b << 12) + (sextet_c << 6) + sextet_d;
        
        if (j < output_length) decoded_data[j++] = (triple >> 16) & 0xFF;
        if (j < output_length) decoded_data[j++] = (triple >> 8) & 0xFF;
        if (j < output_length) decoded_data[j++] = triple & 0xFF;
    }
    
    return decoded_data;
}

// Modified decodeMultipleOpusPackets (Base64 version)
jsi::Value NativeOpusTurboModule::decodeMultipleOpusPackets(jsi::Runtime &rt, std::string packetsBase64, double packetSize) {
    int packetSizeInt = (int)packetSize;
    jsi::Object result = jsi::Object(rt);

    if (!opusDecoder) {
        result.setProperty(rt, "success", false);
        result.setProperty(rt, "error", jsi::String::createFromUtf8(rt, "Decoder not initialized"));
        return result;
    }

    try {
        std::vector<uint8_t> inputData = base64_decode(packetsBase64);
        if (inputData.empty() && !packetsBase64.empty()) { // Handle invalid base64
             result.setProperty(rt, "success", false);
             result.setProperty(rt, "error", jsi::String::createFromUtf8(rt, "Invalid base64 input"));
             return result;
        }
         if (inputData.empty() && packetsBase64.empty()) { // Handle empty input explicitly
             result.setProperty(rt, "success", true);
             result.setProperty(rt, "decodedDataBase64", jsi::String::createFromUtf8(rt, ""));
             result.setProperty(rt, "samplesDecoded", 0);
             result.setProperty(rt, "packetsDecoded", 0);
             result.setProperty(rt, "processingTimeMs", 0.0);
             return result;
         }

        // ---- Start of logic moved from processRawOpusBytes ----
        const unsigned char* inputBytes = inputData.data();
        size_t inputSize = inputData.size();

        auto startTime = std::chrono::high_resolution_clock::now();

        std::vector<opus_int16> outputBuffer;
        outputBuffer.reserve(inputSize * 4); // Approximate reserve

        int totalSamplesDecoded = 0;
        int packetsDecoded = 0;

        for (size_t offset = 0; offset < inputSize; offset += packetSizeInt) {
            size_t packetBytes = std::min((size_t)packetSizeInt, inputSize - offset);

            if (packetBytes < packetSizeInt && offset > 0) {
                break;
            }

            opus_int16 tempBuffer[960];
            int samplesDecoded = opus_decode(
                opusDecoder,
                inputBytes + offset,
                packetBytes,
                tempBuffer,
                960, // Max frame size per channel for opus
                0
            );

            if (samplesDecoded < 0) {
                continue;
            }

            outputBuffer.insert(
                outputBuffer.end(),
                tempBuffer,
                // samplesDecoded is per channel, multiply by number of channels
                tempBuffer + samplesDecoded * DEFAULT_CHANNELS
            );

            totalSamplesDecoded += samplesDecoded;
            packetsDecoded++;
        }

        size_t outputSizeBytes = outputBuffer.size() * sizeof(opus_int16);
        std::vector<uint8_t> outputBytesVec(
            reinterpret_cast<uint8_t*>(outputBuffer.data()),
            reinterpret_cast<uint8_t*>(outputBuffer.data()) + outputSizeBytes
        );
        // Call the base64 encode method directly as we are inside the class scope
        std::string outputBase64 = base64_encode(outputBytesVec);

        auto endTime = std::chrono::high_resolution_clock::now();
        double processingTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();

        result.setProperty(rt, "success", true);
        result.setProperty(rt, "decodedDataBase64", jsi::String::createFromUtf8(rt, outputBase64));
        result.setProperty(rt, "samplesDecoded", totalSamplesDecoded);
        result.setProperty(rt, "packetsDecoded", packetsDecoded);
        result.setProperty(rt, "processingTimeMs", processingTime);
        // ---- End of logic moved from processRawOpusBytes ----

        return result; // Return success result

    } catch (const std::exception& e) {
        result.setProperty(rt, "success", false);
        result.setProperty(rt, "error", jsi::String::createFromUtf8(rt, e.what()));
        return result;
    }
}

// New method to reset decoder state
jsi::Value NativeOpusTurboModule::resetDecoderState(jsi::Runtime &rt) {
    jsi::Object result = jsi::Object(rt);
    if (!opusDecoder) {
        result.setProperty(rt, "success", false);
        result.setProperty(rt, "error", jsi::String::createFromUtf8(rt, "Decoder not initialized"));
        return result;
    }

    int error = opus_decoder_ctl(opusDecoder, OPUS_RESET_STATE);
    if (error == OPUS_OK) {
        result.setProperty(rt, "success", true);
    } else {
        result.setProperty(rt, "success", false);
        result.setProperty(rt, "error", jsi::String::createFromUtf8(rt, opus_strerror(error)));
    }
    return result;
}

jsi::Value NativeOpusTurboModule::saveDecodedDataAsWav(jsi::Runtime &rt, std::string decodedDataBase64, std::string filepath, double sampleRate, double channels) {
    jsi::Object result = jsi::Object(rt);
    
    try {
        // Decode base64 to PCM data
        std::vector<uint8_t> decodedBytes = base64_decode(decodedDataBase64);
        
        // PCM data is 16-bit samples
        // size_t numSamples = decodedBytes.size() / 2;
        
        // Open file for writing
        FILE* file = fopen(filepath.c_str(), "wb");
        if (!file) {
            result.setProperty(rt, "success", false);
            result.setProperty(rt, "error", jsi::String::createFromUtf8(rt, "Failed to open output file"));
            return result;
        }
        
        // WAV Header constants
        const char* RIFF = "RIFF";
        const char* WAVE = "WAVE";
        const char* FMT = "fmt ";
        const char* DATA = "data";
        
        // WAV file header calculation
        int sampleRateInt = static_cast<int>(sampleRate);
        int channelsInt = static_cast<int>(channels);
        int bitsPerSample = 16;  // PCM 16-bit
        int byteRate = sampleRateInt * channelsInt * bitsPerSample / 8;
        int blockAlign = channelsInt * bitsPerSample / 8;
        int dataSize = decodedBytes.size();
        int fileSize = 36 + dataSize;  // 36 is the size of the WAV header minus 8
        
        // Write WAV header
        // RIFF chunk
        fwrite(RIFF, 1, 4, file);
        fwrite(&fileSize, 4, 1, file);
        fwrite(WAVE, 1, 4, file);
        
        // fmt chunk
        fwrite(FMT, 1, 4, file);
        int fmtSize = 16;  // PCM format size
        fwrite(&fmtSize, 4, 1, file);
        short audioFormat = 1;  // PCM = 1
        fwrite(&audioFormat, 2, 1, file);
        fwrite(&channelsInt, 2, 1, file);
        fwrite(&sampleRateInt, 4, 1, file);
        fwrite(&byteRate, 4, 1, file);
        fwrite(&blockAlign, 2, 1, file);
        fwrite(&bitsPerSample, 2, 1, file);
        
        // data chunk
        fwrite(DATA, 1, 4, file);
        fwrite(&dataSize, 4, 1, file);
        
        // Write the actual PCM data
        fwrite(decodedBytes.data(), 1, decodedBytes.size(), file);
        
        fclose(file);
        
        result.setProperty(rt, "success", true);
        result.setProperty(rt, "filepath", jsi::String::createFromUtf8(rt, filepath));
        
    } catch (const std::exception& e) {
        result.setProperty(rt, "success", false);
        result.setProperty(rt, "error", jsi::String::createFromUtf8(rt, e.what()));
    }
    
    return result;
}

// Frame-by-frame streaming methods
jsi::Value NativeOpusTurboModule::initializeStreamDecoder(jsi::Runtime &rt, double sampleRate, double channels) {
    jsi::Object result = jsi::Object(rt);
    
    try {
        // Clean up existing stream decoder if any
        if (streamDecoder) {
            opus_decoder_destroy(streamDecoder);
            streamDecoder = nullptr;
        }
        
        // Create new stream decoder with specified parameters
        streamSampleRate = static_cast<opus_int32>(sampleRate);
        streamChannels = static_cast<int>(channels);
        
        int error = 0;
        streamDecoder = opus_decoder_create(streamSampleRate, streamChannels, &error);
        
        if (error != OPUS_OK || !streamDecoder) {
            result.setProperty(rt, "success", false);
            result.setProperty(rt, "error", jsi::String::createFromUtf8(rt, opus_strerror(error)));
            streamInitialized = false;
            return result;
        }
        
        streamInitialized = true;
        result.setProperty(rt, "success", true);
        
    } catch (const std::exception& e) {
        result.setProperty(rt, "success", false);
        result.setProperty(rt, "error", jsi::String::createFromUtf8(rt, e.what()));
        streamInitialized = false;
    }
    
    return result;
}

jsi::Value NativeOpusTurboModule::decodeOpusFrame(jsi::Runtime &rt, std::string base64Frame) {
    jsi::Object result = jsi::Object(rt);
    
    if (!streamInitialized || !streamDecoder) {
        result.setProperty(rt, "success", false);
        result.setProperty(rt, "error", jsi::String::createFromUtf8(rt, "Stream decoder not initialized"));
        return result;
    }
    
    try {
        // Decode base64 frame data
        std::vector<uint8_t> frameData = base64_decode(base64Frame);
        
        if (frameData.empty() && !base64Frame.empty()) {
            result.setProperty(rt, "success", false);
            result.setProperty(rt, "error", jsi::String::createFromUtf8(rt, "Invalid base64 frame data"));
            return result;
        }
        
        // Prepare output buffer for decoded PCM data
        float pcmBuffer[MAX_FRAME_SIZE * streamChannels];
        
        // Decode the Opus frame
        int samplesDecoded = opus_decode_float(
            streamDecoder,
            frameData.data(),
            static_cast<opus_int32>(frameData.size()),
            pcmBuffer,
            MAX_FRAME_SIZE,
            0
        );
        
        if (samplesDecoded < 0) {
            result.setProperty(rt, "success", false);
            result.setProperty(rt, "error", jsi::String::createFromUtf8(rt, opus_strerror(samplesDecoded)));
            return result;
        }
        
        // Create Float32Array from the decoded PCM data
        size_t totalSamples = samplesDecoded * streamChannels;
        size_t bufferSize = totalSamples * sizeof(float);
        
        auto arrayBuffer = jsi::ArrayBuffer(rt, bufferSize);
        std::memcpy(arrayBuffer.data(rt), pcmBuffer, bufferSize);
        
        jsi::Object typedArray = rt.global().getPropertyAsObject(rt, "Float32Array");
        jsi::Value args[] = {
            jsi::Value(rt, arrayBuffer),
            jsi::Value(0),
            jsi::Value(static_cast<double>(totalSamples))
        };
        jsi::Object float32Array = typedArray.asFunction(rt).callAsConstructor(rt, args, 3).asObject(rt);
        
        result.setProperty(rt, "success", true);
        result.setProperty(rt, "pcmData", float32Array);
        result.setProperty(rt, "samplesDecoded", samplesDecoded);
        
    } catch (const std::exception& e) {
        result.setProperty(rt, "success", false);
        result.setProperty(rt, "error", jsi::String::createFromUtf8(rt, e.what()));
    }
    
    return result;
}

jsi::Value NativeOpusTurboModule::resetOpusStreamDecoder(jsi::Runtime &rt) {
    jsi::Object result = jsi::Object(rt);
    
    if (!streamInitialized || !streamDecoder) {
        result.setProperty(rt, "success", false);
        result.setProperty(rt, "error", jsi::String::createFromUtf8(rt, "Stream decoder not initialized"));
        return result;
    }
    
    int error = opus_decoder_ctl(streamDecoder, OPUS_RESET_STATE);
    if (error == OPUS_OK) {
        result.setProperty(rt, "success", true);
    } else {
        result.setProperty(rt, "success", false);
        result.setProperty(rt, "error", jsi::String::createFromUtf8(rt, opus_strerror(error)));
    }
    
    return result;
}

} // namespace facebook::react
