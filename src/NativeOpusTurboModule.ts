import type { TurboModule } from 'react-native';
import { TurboModuleRegistry } from 'react-native';

export interface Spec extends TurboModule {

  decodeMultipleOpusPackets(
    packetsBase64: string,
    packetSize: number
  ): Promise<{
    success: boolean;
    decodedDataBase64?: string;
    samplesDecoded?: number;
    packetsDecoded?: number;
    processingTimeMs?: number;
    error?: string;
  }>;

  resetDecoderState(): Promise<{ success: boolean; error?: string }>;

  saveDecodedDataAsWav(
    decodedDataBase64: string,
    filepath: string,
    sampleRate: number,
    channels: number
  ): Promise<{
    success: boolean;
    filepath?: string;
    error?: string;
  }>;

  // Frame-by-frame streaming methods
  initializeStreamDecoder(
    sampleRate: number,
    channels: number
  ): Promise<{ success: boolean; error?: string }>;

  decodeOpusFrame(
    base64Frame: string
  ): Promise<{
    success: boolean;
    pcmData?: Float32Array;
    samplesDecoded?: number;
    error?: string;
  }>;

  resetOpusStreamDecoder(): Promise<{ success: boolean; error?: string }>;
}

export default TurboModuleRegistry.getEnforcing<Spec>('OpusTurbo');
