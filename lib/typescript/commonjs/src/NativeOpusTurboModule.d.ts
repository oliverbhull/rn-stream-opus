import type { TurboModule } from 'react-native';
export interface Spec extends TurboModule {
    decodeMultipleOpusPackets(packetsBase64: string, packetSize: number): Promise<{
        success: boolean;
        decodedDataBase64?: string;
        samplesDecoded?: number;
        packetsDecoded?: number;
        processingTimeMs?: number;
        error?: string;
    }>;
    resetDecoderState(): Promise<{
        success: boolean;
        error?: string;
    }>;
    saveDecodedDataAsWav(decodedDataBase64: string, filepath: string, sampleRate: number, channels: number): Promise<{
        success: boolean;
        filepath?: string;
        error?: string;
    }>;
}
declare const _default: Spec;
export default _default;
//# sourceMappingURL=NativeOpusTurboModule.d.ts.map