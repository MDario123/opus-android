package com.theeasiestway.opus

import java.nio.ByteBuffer
import java.nio.FloatBuffer
import java.nio.ShortBuffer

class Encoder {
  private var nativeHandler: Long

  init {
    OpusLibLoader
  }

  // # Constructor

  private external fun create(sampleRate: Int, channels: Int, application: Int, error: IntArray?): Long

  constructor(sampleRate: Int, channels: Int, application: Int) {
    val error = IntArray(1)
    nativeHandler = create(sampleRate, channels, application, error)
    if (error[0] != 0) {
      throw IllegalStateException("Failed to create encoder: error code $error")
    }
  }

  // # Destructor

  private external fun release(nativeHandler: Long)

  public fun release() {
    if (nativeHandler != 0L) {
      release(nativeHandler)
      nativeHandler = 0L
    }
  }

  // # Configuration methods

  private external fun setBitrate(nativeHandler: Long, bitrate: Int): Int

  fun setBitrate(bitrate: Int): Int {
    return setBitrate(nativeHandler, bitrate)
  }

  // # Encoder methods

  private external fun encodeShort(
    nativeHandler: Long,
    input: ShortBuffer,
    frameSize: Int,
    output: ByteBuffer,
  ): Int

  // You must guarantee that output has enough space to hold the encoded data. Otherwise this is undefined behavior.
  fun encodeShort(
    input: ShortBuffer,
    frameSize: Int,
    output: ByteBuffer,
  ): Int {
    return encodeShort(nativeHandler, input, frameSize, output)
  }

  private external fun encodeFloat(
    nativeHandler: Long,
    input: FloatBuffer,
    frameSize: Int,
    output: ByteBuffer,
  ): Int

  // You must guarantee that output has enough space to hold the encoded data. Otherwise this is undefined behavior.
  fun encodeFloat(
    input: FloatBuffer,
    frameSize: Int,
    output: ByteBuffer,
  ): Int {
    return encodeFloat(nativeHandler, input, frameSize, output)
  }
}
