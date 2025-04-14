/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 *
 * @format
 */

import React, { useState } from 'react';
import {
  SafeAreaView,
  StatusBar,
  StyleSheet,
  Text,
  View,
  Pressable,
  Alert,
  useColorScheme,
} from 'react-native';
import { decodeMultipleOpusPackets } from 'react-native-opus';

// Hardcoded base64 Opus string (replace with actual data if needed)
const BASE64_OPUS_STRING =
  'uA3h1qD6BNyQQaeaz4swEG+XHNgqhWbncGOgmzi4gqsM1711/j5+abgOBe9pOrbHBo5lmump9wE+3x91yI4/Yk4pK1LZUT11nOc50WV/1PS4DhEgJfmFU2BhZaj1QwuXqwdkP0mEby+/mm+pccXy9n7FSw/IJ6MsuBomEXSjlOJ238qLOTUfsRf0CLMq3247Amh+KA0LHBBJ1+GKZqVWWrgantUAwgPUMZuL+tVTI7gBTwupR3ea48LzdFuPS3yUMyHZ5FsAkSm4Dl6pFFnRR6LY+UCoMTBNb2DZHplGKtv/F+PK2XbW9fwgM4/PRK4q';

function App(): React.JSX.Element {
  const isDarkMode = useColorScheme() === 'dark';
  const [decodedResult, setDecodedResult] = useState<string | null>(null);
  const [isLoading, setIsLoading] = useState(false);

  const backgroundStyle = {
    backgroundColor: isDarkMode ? '#000000' : '#F5F5F5', // Black or light gray
    flex: 1,
  };

  const handleDecodePress = async () => {
    setIsLoading(true);
    setDecodedResult(null);
    console.log('Attempting to decode...');
    try {
      // Assuming the function returns a base64 string or similar serializable result
      const result = await decodeMultipleOpusPackets(BASE64_OPUS_STRING, 40);
      console.log('Decoding successful:', result);
      // Displaying raw PCM data might not be meaningful, log it instead or process further
      setDecodedResult('Decoding succeeded! Check console for raw data.');
      // If result is directly displayable (e.g., string):
      // setDecodedResult(typeof result === 'string' ? result : JSON.stringify(result));
    } catch (error) {
      console.error('Decoding failed:', error);
      setDecodedResult(null); // Clear previous results on error
      Alert.alert(
        'Decoding Error',
        error instanceof Error ? error.message : 'An unknown error occurred'
      );
    } finally {
      setIsLoading(false);
    }
  };

  return (
    <SafeAreaView style={backgroundStyle}>
      <StatusBar barStyle={isDarkMode ? 'light-content' : 'dark-content'} backgroundColor={backgroundStyle.backgroundColor} />
      <View style={styles.container}>
        <Pressable
          style={({ pressed }) => [
            styles.button,
            isDarkMode ? styles.buttonDark : styles.buttonLight,
            pressed && (isDarkMode ? styles.buttonDarkPressed : styles.buttonLightPressed),
            isLoading && styles.buttonDisabled,
          ]}
          onPress={handleDecodePress}
          disabled={isLoading}
        >
          <Text style={[styles.buttonText, isDarkMode ? styles.buttonTextDark : styles.buttonTextLight]}>
            {isLoading ? 'Decoding...' : 'Decode Opus String'}
          </Text>
        </Pressable>
        {decodedResult && (
           <Text style={[styles.resultText, isDarkMode ? styles.resultTextDark : styles.resultTextLight]}>
             {decodedResult}
           </Text>
        )}
      </View>
    </SafeAreaView>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    padding: 20,
  },
  button: {
    borderRadius: 12, // rounded-2xl equivalent
    paddingVertical: 12,
    paddingHorizontal: 32,
    marginBottom: 20,
    shadowOffset: { width: 0, height: 4 }, // Subtle shadow
    shadowOpacity: 0.1,
    shadowRadius: 6,
    elevation: 5, // for Android
  },
  buttonDark: {
    backgroundColor: '#FFFFFF', // White button on dark bg
    shadowColor: '#FFFFFF', // White shadow
  },
  buttonLight: {
    backgroundColor: '#000000', // Black button on light bg
    shadowColor: '#000000', // Black shadow
  },
  buttonDarkPressed: {
     backgroundColor: '#EAEAEA', // Slightly darker white on press
     transform: [{ scale: 0.98 }], // Subtle scale down
  },
  buttonLightPressed: {
     backgroundColor: '#333333', // Slightly lighter black on press
     transform: [{ scale: 0.98 }],
  },
  buttonDisabled: {
    opacity: 0.6,
  },
  buttonText: {
    fontSize: 16,
    fontWeight: '600', // semibold
    letterSpacing: 0.5, // tracking-wide approximation
    textAlign: 'center',
  },
  buttonTextDark: {
    color: '#000000', // Black text
  },
  buttonTextLight: {
     color: '#FFFFFF', // White text
  },
  resultText: {
    marginTop: 20,
    fontSize: 14,
    textAlign: 'center',
    paddingHorizontal: 10,
  },
  resultTextDark: {
    color: '#D8D8D8', // Light gray text
  },
  resultTextLight: {
    color: '#333333', // Dark gray text
  },
});

export default App;
