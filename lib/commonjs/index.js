"use strict";

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports.decodeMultipleOpusPackets = decodeMultipleOpusPackets;
exports.resetDecoderState = resetDecoderState;
exports.saveDecodedDataAsWav = saveDecodedDataAsWav;
var _NativeOpusTurboModule = _interopRequireDefault(require("./NativeOpusTurboModule"));
function _interopRequireDefault(e) { return e && e.__esModule ? e : { default: e }; }
function decodeMultipleOpusPackets(packetsBase64, packetSize) {
  return _NativeOpusTurboModule.default.decodeMultipleOpusPackets(packetsBase64, packetSize);
}
function resetDecoderState() {
  return _NativeOpusTurboModule.default.resetDecoderState();
}
function saveDecodedDataAsWav(decodedDataBase64, filepath, sampleRate, channels) {
  return _NativeOpusTurboModule.default.saveDecodedDataAsWav(decodedDataBase64, filepath, sampleRate, channels);
}
//# sourceMappingURL=index.js.map