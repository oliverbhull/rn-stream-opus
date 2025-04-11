#include <jni.h>
#include "react-native-opus.h"

extern "C"
JNIEXPORT jdouble JNICALL
Java_com_opus_OpusModule_nativeMultiply(JNIEnv *env, jclass type, jdouble a, jdouble b) {
    return opus::multiply(a, b);
}
