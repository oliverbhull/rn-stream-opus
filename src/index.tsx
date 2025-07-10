import OpusTurboModule from './NativeOpusTurboModule';

export function decodeMultipleOpusPackets(
  packetsBase64: string,
  packetSize: number
): Promise<{
  success: boolean;
  decodedDataBase64?: string;
  samplesDecoded?: number;
  packetsDecoded?: number;
  processingTimeMs?: number;
  error?: string;
}> {
  return OpusTurboModule.decodeMultipleOpusPackets(packetsBase64, packetSize);
}

export function resetDecoderState(): Promise<{ success: boolean; error?: string }> {
  return OpusTurboModule.resetDecoderState();
}

export function saveDecodedDataAsWav(
  decodedDataBase64: string,
  filepath: string,
  sampleRate: number,
  channels: number
): Promise<{
  success: boolean;
  filepath?: string;
  error?: string;
}> {
  return OpusTurboModule.saveDecodedDataAsWav(decodedDataBase64, filepath, sampleRate, channels);
}

// Frame-by-frame streaming API
export function initializeStreamDecoder(
  sampleRate: number,
  channels: number
): Promise<{ success: boolean; error?: string }> {
  return OpusTurboModule.initializeStreamDecoder(sampleRate, channels);
}

export function decodeOpusFrame(
  frameData: Uint8Array
): Promise<{
  success: boolean;
  pcmData?: Float32Array;
  samplesDecoded?: number;
  error?: string;
}> {
  // Convert Uint8Array to base64 string for native module
  const base64Frame = Buffer.from(frameData).toString('base64');
  return OpusTurboModule.decodeOpusFrame(base64Frame);
}

export function resetOpusStreamDecoder(): Promise<{ success: boolean; error?: string }> {
  return OpusTurboModule.resetOpusStreamDecoder();
}