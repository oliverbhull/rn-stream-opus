import OpusTurboModule from './NativeOpusTurboModule';

export const decodeMultipleOpusPackets = OpusTurboModule.decodeMultipleOpusPackets;
export const resetDecoderState = OpusTurboModule.resetDecoderState;
export const saveDecodedDataAsWav = OpusTurboModule.saveDecodedDataAsWav;

export type { Spec as OpusTurboModuleSpec } from './NativeOpusTurboModule';
