"use strict";

import OpusTurboModule from './NativeOpusTurboModule';
export function decodeMultipleOpusPackets(packetsBase64, packetSize) {
  return OpusTurboModule.decodeMultipleOpusPackets(packetsBase64, packetSize);
}
export function resetDecoderState() {
  return OpusTurboModule.resetDecoderState();
}
export function saveDecodedDataAsWav(decodedDataBase64, filepath, sampleRate, channels) {
  return OpusTurboModule.saveDecodedDataAsWav(decodedDataBase64, filepath, sampleRate, channels);
}
//# sourceMappingURL=index.js.map