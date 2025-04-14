export declare function decodeMultipleOpusPackets(packetsBase64: string, packetSize: number): Promise<{
    success: boolean;
    decodedDataBase64?: string;
    samplesDecoded?: number;
    packetsDecoded?: number;
    processingTimeMs?: number;
    error?: string;
}>;
export declare function resetDecoderState(): Promise<{
    success: boolean;
    error?: string;
}>;
export declare function saveDecodedDataAsWav(decodedDataBase64: string, filepath: string, sampleRate: number, channels: number): Promise<{
    success: boolean;
    filepath?: string;
    error?: string;
}>;
//# sourceMappingURL=index.d.ts.map