# react-native-opus

A library for decoding Opus audio packets in React Native with both batch and frame-by-frame streaming capabilities.

## Installation

```sh
npm install react-native-opus
```

## Usage

The `react-native-opus` library provides functions to decode Opus audio packets from base64-encoded strings with both batch and frame-by-frame streaming capabilities.

## API Methods

### Batch Decoding

- **`decodeMultipleOpusPackets(base64String: string, frameSize: number)`**: Decodes multiple Opus packets from a base64-encoded string. The `frameSize` parameter specifies the frame size in bytes.

### Frame-by-Frame Streaming

- **`initializeStreamDecoder(sampleRate: number, channels: number)`**: Initializes the stream decoder with specified sample rate and channel count.
- **`decodeOpusFrame(frameData: Uint8Array)`**: Decodes a single Opus frame and returns Float32Array PCM data.
- **`resetOpusStreamDecoder()`**: Resets the stream decoder state.

## Examples

### Example: Batch Decoding

```js
import { decodeMultipleOpusPackets } from 'react-native-opus';

// Hardcoded base64 string (100 characters)
const base64String = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";

// Decode the Opus packet
async function decodeAudio() {
  try {
    const decodedPackets = await decodeMultipleOpusPackets(base64String, 40);
    console.log("Decoded packets:", decodedPackets);
  } catch (error) {
    console.error("Error decoding audio:", error);
  }
}

decodeAudio();
```

### Example: Frame-by-Frame Streaming

```js
import { 
  initializeStreamDecoder, 
  decodeOpusFrame, 
  resetOpusStreamDecoder 
} from 'react-native-opus';

async function streamDecodeAudio() {
  try {
    // Initialize stream decoder for 16kHz mono audio
    const initResult = await initializeStreamDecoder(16000, 1);
    if (!initResult.success) {
      throw new Error(initResult.error);
    }

    // Decode individual frames
    const frameData = new Uint8Array([/* your opus frame bytes */]);
    const decodeResult = await decodeOpusFrame(frameData);
    
    if (decodeResult.success && decodeResult.pcmData) {
      console.log(`Decoded ${decodeResult.samplesDecoded} samples`);
      console.log('PCM data:', decodeResult.pcmData); // Float32Array
    }

    // Reset decoder when done
    await resetOpusStreamDecoder();
  } catch (error) {
    console.error("Error streaming decode:", error);
  }
}

streamDecodeAudio();
```

## Key Features

- **Batch Processing**: Decode multiple packets at once for efficiency
- **Frame-by-Frame Streaming**: Real-time decoding with persistent decoder state
- **Float32Array Output**: Direct PCM data for audio processing
- **Cross-Platform**: Works on both iOS and Android
- **Memory Efficient**: Optimized for mobile devices

## Contributing

See the [contributing guide](CONTRIBUTING.md) for details on contributing.

## License

MIT
