import React, { useState } from 'react';
import { Text, View, StyleSheet, Button, Alert } from 'react-native';
import { decodeMultipleOpusPackets } from 'react-native-opus';

const base64Opus = "uA3h1qD6BNyQQaeaz4swEG+XHNgqhWbncGOgmzi4gqsM1711/j5+abgOBe9pOrbHBo5lmump9wE+3x91yI4/Yk4pK1LZUT11nOc50WV/1PS4DhEgJfmFU2BhZaj1QwuXqwdkP0mEby+/mm+pccXy9n7FSw/IJ6MsuBomEXSjlOJ238qLOTUfsRf0CLMq3247Amh+KA0LHBBJ1+GKZqVWWrgantUAwgPUMZuL+tVTI7gBTwupR3ea48LzdFuPS3yUMyHZ5FsAkSm4Dl6pFFnRR6LY+UCoMTBNb2DZHplGKtv/F+PK2XbW9fwgM4/PRK4q";
const PACKET_SIZE = 40;

export default function App() {
  const [decodedResult, setDecodedResult] = useState<string | null>(null);
  const [error, setError] = useState<string | null>(null);
  const [isLoading, setIsLoading] = useState(false);

  const handleDecodePress = async () => {
    setIsLoading(true);
    setError(null);
    setDecodedResult(null);
    console.log('Attempting to decode...');
    try {
      const result = await decodeMultipleOpusPackets(base64Opus, PACKET_SIZE);
      console.log('Decode Result:', result);
      if (result.success) {
        setDecodedResult(
          `Success! Decoded ${result.packetsDecoded} packets (${result.samplesDecoded} samples) in ${result.processingTimeMs?.toFixed(2)}ms. Output Base64 length: ${result.decodedDataBase64?.length}`
        );
      } else {
        setError(`Decoding failed: ${result.error}`);
        Alert.alert('Decoding Error', result.error || 'Unknown error');
      }
    } catch (e: any) {
      console.error('Decoding Exception:', e);
      const errorMessage = e.message || 'An unexpected error occurred.';
      setError(`Exception: ${errorMessage}`);
      Alert.alert('Decoding Exception', errorMessage);
    }
    setIsLoading(false);
  };

  return (
    <View style={styles.container}>
      <Text style={styles.title}>React Native Opus Decode Test</Text>
      <Button
        title={isLoading ? 'Decoding...' : 'Decode Hardcoded Opus'}
        onPress={handleDecodePress}
        disabled={isLoading}
      />
      {decodedResult && (
        <Text style={styles.resultText}>Result: {decodedResult}</Text>
      )}
      {error && <Text style={styles.errorText}>Error: {error}</Text>}
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
    padding: 20,
  },
  title: {
    fontSize: 18,
    fontWeight: 'bold',
    marginBottom: 20,
  },
  resultText: {
    marginTop: 15,
    color: 'green',
  },
  errorText: {
    marginTop: 15,
    color: 'red',
  },
});
