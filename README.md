# react-native-opus

A library for decoding Opus audio packets in React Native.

## Installation

```sh
npm install react-native-opus
```

## Usage

The `react-native-opus` library provides a function to decode Opus audio packets from base64-encoded strings.

### Example: Decoding an Opus Packet

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

### Key Function

- **`decodeMultipleOpusPackets(base64String: string, frameSize: number)`**: Decodes a base64-encoded Opus packet. The `frameSize` parameter specifies the frame size in milliseconds (e.g., 40ms).

## Contributing

See the [contributing guide](CONTRIBUTING.md) for details on contributing.

## License

MIT
