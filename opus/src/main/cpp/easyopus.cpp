#include "include/opus.h"
#include <jni.h>

//
// Encoding
//

JNIEXPORT jlong JNICALL Java_com_theeasiestway_opus_Encoder_create(
    JNIEnv *env, jobject thiz, jint sampleRate, jint channels, jint application,
    jintArray jerror // optional: Java can pass this to receive error code
) {
  int error;
  OpusEncoder *encoder = opus_encoder_create(
      static_cast<opus_int32>(sampleRate), channels, application, &error);

  // Optionally write the error code back into the passed-in int array
  if (jerror != nullptr && env->GetArrayLength(jerror) > 0) {
    env->SetIntArrayRegion(jerror, 0, 1, &error);
  }

  // Return pointer as jlong (Java long)
  return reinterpret_cast<jlong>(encoder);
}

JNIEXPORT void JNICALL Java_com_theeasiestway_opus_Encoder_release(
    JNIEnv *env, jobject thiz, jlong handle) {
  OpusEncoder *encoder = reinterpret_cast<OpusEncoder *>(handle);
  if (encoder != nullptr) {
    opus_encoder_destroy(encoder);
  }
}

JNIEXPORT jint JNICALL Java_com_theeasiestway_opus_Encoder_encodeShort(
    JNIEnv *env, jobject thiz, jlong handle, jobject input, jint frameSize,
    jobject output) {
  OpusEncoder *encoder = reinterpret_cast<OpusEncoder *>(handle);
  if (encoder == nullptr) {
    return OPUS_INVALID_STATE;
  }
  if (input == nullptr || output == nullptr) {
    return OPUS_BAD_ARG;
  }

  void *inputData = env->GetDirectBufferAddress(input);

  if (inputData == nullptr) {
    return OPUS_BAD_ARG;
  }

  void *outputPtr = env->GetDirectBufferAddress(output);

  if (outputPtr == nullptr) {
    return OPUS_BAD_ARG;
  }

  jlong outputLength = env->GetDirectBufferCapacity(output);

  int result = opus_encode(encoder, static_cast<const opus_int16 *>(inputData),
                           frameSize, static_cast<unsigned char *>(outputPtr),
                           outputLength);

  return result;
}

JNIEXPORT jint JNICALL Java_com_theeasiestway_opus_Encoder_encodeFloat(
    JNIEnv *env, jobject thiz, jlong handle, jobject input, jint frameSize,
    jobject output) {
  OpusEncoder *encoder = reinterpret_cast<OpusEncoder *>(handle);
  if (encoder == nullptr) {
    return OPUS_INVALID_STATE;
  }
  if (input == nullptr || output == nullptr) {
    return OPUS_BAD_ARG;
  }

  void *inputData = env->GetDirectBufferAddress(input);

  if (inputData == nullptr) {
    return OPUS_BAD_ARG;
  }

  void *outputPtr = env->GetDirectBufferAddress(output);

  if (outputPtr == nullptr) {
    return OPUS_BAD_ARG;
  }

  jlong outputLength = env->GetDirectBufferCapacity(output);

  int result = opus_encode_float(
      encoder, static_cast<const float *>(inputData), frameSize,
      static_cast<unsigned char *>(outputPtr), outputLength);

  return result;
}

JNIEXPORT jint JNICALL Java_com_theeasiestway_opus_Encoder_setBitrate(
    JNIEnv *env, jobject thiz, jlong handle, jint bitrate) {
  OpusEncoder *encoder = reinterpret_cast<OpusEncoder *>(handle);
  if (encoder == nullptr) {
    return OPUS_INVALID_STATE;
  }
  return opus_encoder_ctl(encoder, OPUS_SET_BITRATE(bitrate));
}

//
// Decoding
//

JNIEXPORT jlong JNICALL Java_com_theeasiestway_opus_Decoder_create(
    JNIEnv *env, jobject thiz, jint sampleRate, jint channels,
    jintArray jerror // optional: Java can pass this to receive error code
) {
  int error;
  OpusDecoder *decoder = opus_decoder_create(
      static_cast<opus_int32>(sampleRate), channels, &error);

  // Optionally write the error code back into the passed-in int array
  if (jerror != nullptr && env->GetArrayLength(jerror) > 0) {
    env->SetIntArrayRegion(jerror, 0, 1, &error);
  }

  return reinterpret_cast<jlong>(decoder);
}

JNIEXPORT void JNICALL Java_com_theeasiestway_opus_Decoder_release(
    JNIEnv *env, jobject thiz, jlong handle) {
  OpusDecoder *decoder = reinterpret_cast<OpusDecoder *>(handle);
  if (decoder != nullptr) {
    opus_decoder_destroy(decoder);
  }
}

JNIEXPORT jint JNICALL Java_com_theeasiestway_opus_Decoder_decodeShort(
    JNIEnv *env, jobject thiz, jlong handle, jobject input, jint inputLength,
    jobject output, jint frameSize, jboolean decodeFEC) {
  OpusDecoder *decoder = reinterpret_cast<OpusDecoder *>(handle);
  if (decoder == nullptr) {
    return OPUS_INVALID_STATE;
  }
  if (output == nullptr) {
    return OPUS_BAD_ARG;
  }

  // It's fine if input is null, opus will handle it gracefully
  // If input is not null, then it should be a direct buffer
  void *inputData;
  if (input == nullptr) {
    inputData = nullptr;
  } else {
    inputData = env->GetDirectBufferAddress(input);
    if (inputData == nullptr) {
      return OPUS_BAD_ARG;
    }
  }

  void *outputData = env->GetDirectBufferAddress(output);
  if (outputData == nullptr) {
    return OPUS_BAD_ARG;
  }

  int result = opus_decode(
      decoder, static_cast<const unsigned char *>(inputData), inputLength,
      static_cast<opus_int16 *>(outputData), frameSize, decodeFEC ? 1 : 0);

  return result;
}

JNIEXPORT jint JNICALL Java_com_theeasiestway_opus_Decoder_decodeFloat(
    JNIEnv *env, jobject thiz, jlong handle, jbyteArray input, jint inputLength,
    jfloatArray output, jint frameSize, jboolean decodeFEC) {
  OpusDecoder *decoder = reinterpret_cast<OpusDecoder *>(handle);
  if (decoder == nullptr) {
    return OPUS_INVALID_STATE;
  }
  if (output == nullptr) {
    return OPUS_BAD_ARG;
  }

  // It's fine if input is null, opus will handle it gracefully
  // If input is not null, then it should be a direct buffer
  void *inputData;
  if (input == nullptr) {
    inputData = nullptr;
  } else {
    inputData = env->GetDirectBufferAddress(input);
    if (inputData == nullptr) {
      return OPUS_BAD_ARG;
    }
  }

  void *outputData = env->GetDirectBufferAddress(output);
  if (outputData == nullptr) {
    return OPUS_BAD_ARG;
  }

  int result = opus_decode_float(
      decoder, static_cast<const unsigned char *>(inputData), inputLength,
      static_cast<float *>(outputData), frameSize, decodeFEC ? 1 : 0);

  return result;
}
