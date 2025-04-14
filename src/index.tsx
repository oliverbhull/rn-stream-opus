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