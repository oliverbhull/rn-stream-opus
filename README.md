# react-native-opus

A library for handling Opus audio data in React Native applications, particularly for Bluetooth Low Energy (BLE) audio processing.

## Installation

```sh
npm install react-native-opus
```

## Usage

The `react-native-opus` library provides functions to decode Opus audio packets and process PCM data for voice activity detection (VAD). Below is an example of how to use the library in a BLE audio streaming context, as demonstrated in the provided code.

### Example: Monitoring and Processing BLE Audio Data

```js
import { Device } from "react-native-ble-plx";
import { decodeMultipleOpusPackets } from 'react-native-opus';
import { processMultiplePcm } from 'react-native-webrtc-vad';
import { Buffer } from "buffer";

// BLE-related imports (custom to your app)
import {
  getMyAudioDataCharacteristicUUID,
  getServiceUUID,
} from "./ble-characteristics";
import { getCompassEventEmitter } from "./ble-events";

// Monitor audio data from a BLE device
export async function monitorAudioDataCharacteristic(device: Device, codec: string) {
  const bufferMultiplier = 1; // Adjust based on app state (e.g., 6 for background)
  const baseSize = codec === "opus" ? 166 : 1300;
  const bufferLength = Math.round(baseSize * bufferMultiplier);

  let bytes1 = new Array(bufferLength);
  let bytes2 = new Array(bufferLength);
  let pos1 = 0;
  let pos2 = 0;
  let idx = 0;

  const receiveAudioCallback = async (error: any, characteristic: any) => {
    if (error) {
      console.error("Error monitoring characteristic: ", error);
      return;
    }

    if (!characteristic) {
      console.log("No characteristic found");
      return;
    }

    // Decode Opus packets
    const decodedPackets = await decodeMultipleOpusPackets(characteristic.value, 40);
    console.log("Decoded packets: ", decodedPackets);

    // Process PCM data for voice activity detection
    const processedPackets = await processMultiplePcm(decodedPackets.decodedDataBase64, 16000, 640);
    const results = processedPackets.results;
    console.log(
      "Processed packets: ",
      results?.filter((packet: any) => packet === false).length, "false, ",
      results?.filter((packet: any) => packet === true).length, "true"
    );

    // Buffer and send audio data
    if (idx === 0) {
      bytes1[pos1++] = characteristic?.value!;
      if (pos1 === bufferLength) {
        idx = 1;
        sendAudioData(bytes1); // Custom function to send data to server
        pos1 = 0;
      }
    } else {
      bytes2[pos2++] = characteristic?.value!;
      if (pos2 === bufferLength) {
        idx = 0;
        sendAudioData(bytes2); // Custom function to send data to server
        pos2 = 0;
      }
    }
  };

  // Start monitoring BLE characteristic
  device.monitorCharacteristicForService(
    getServiceUUID(),
    getMyAudioDataCharacteristicUUID(codec),
    receiveAudioCallback
  );
}

// Example function to send audio data to a server
async function sendAudioData(bytes: string[]) {
  const binaryChunks = bytes.map(base64String => Buffer.from(base64String, 'base64'));
  const blob = new Blob([Buffer.concat(binaryChunks)]);

  const formData = new FormData();
  formData.append('file', blob);
  formData.append('sampleRate', '16000');

  // Example: Send to server (adjust URL and headers as needed)
  await fetch("https://your-server/api/v1/audio/process", {
    method: "POST",
    body: formData,
  });
}
```

### Key Functions

- **`decodeMultipleOpusPackets(base64String: string, frameSize: number)`**: Decodes multiple Opus packets from a base64-encoded string. The `frameSize` parameter specifies the frame size in milliseconds (e.g., 40ms).

  ```js
  const decodedPackets = await decodeMultipleOpusPackets(characteristic.value, 40);
  ```

- **`processMultiplePcm(base64String: string, sampleRate: number, frameSize: number)`**: Processes PCM data for voice activity detection. Returns an object with VAD results.

  ```js
  const processedPackets = await processMultiplePcm(decodedPackets.decodedDataBase64, 16000, 640);
  ```

### Configuration

- **Buffer Size**: Adjust buffer size based on the codec and app state (e.g., foreground vs. background). For Opus, the base buffer size is 166 samples, but it can be scaled using a `bufferMultiplier`.

  ```js
  const bufferMultiplier = appState === 'background' ? 6 : 1;
  const bufferSize = Math.round((codec === "opus" ? 166 : 1300) * bufferMultiplier);
  ```

- **BLE Integration**: Use with `react-native-ble-plx` to monitor audio characteristics. Ensure your BLE service and characteristic UUIDs are correctly configured.

### Dependencies

- `react-native-ble-plx`: For BLE communication.
- `react-native-webrtc-vad`: For voice activity detection.
- `buffer`: For handling binary data.

Install these dependencies:

```sh
npm install react-native-ble-plx react-native-webrtc-vad buffer
```

## Contributing

See the [contributing guide](CONTRIBUTING.md) to learn how to contribute to the repository and the development workflow.

## License

MIT

---

Made with [create-react-native-library](https://github.com/callstack/react-native-builder-bob)
