package com.theeasiestway.opus

import java.nio.ByteBuffer
import java.nio.FloatBuffer
import java.nio.ShortBuffer

class Decoder {
  private var nativeHandler: Long
  private var channels: Int

  init {
    OpusLibLoader
  }

  // # Constructor

  private external fun create(sampleRate: Int, channels: Int, error: IntArray?): Long

  constructor(sampleRate: Int, channels: Int) {
    val error = IntArray(1)
    nativeHandler = create(sampleRate, channels, error)
    if (error[0] != 0) {
      throw IllegalStateException("Failed to create encoder: error code $error")
    }
    this.channels = channels
  }

  // # Destructor

  private external fun release(nativeHandler: Long)

  public fun release() {
    if (nativeHandler != 0L) {
      release(nativeHandler)
      nativeHandler = 0L
    }
  }

  // # Decoder methods
  
  private external fun decodeShort(
    nativeHandler: Long,
    input: ByteBuffer?,
    inputSize: Int,
    output: ShortBuffer,
    frameSize: Int,
    fec: Boolean,
  ): Int

  // You must guarantee that output has enough space to hold the decoded data. Otherwise this is undefined behavior.
  fun decodeShort(
    input: ByteBuffer?,
    output: ShortBuffer,
    inputSize: Int? = null,
    frameSize: Int? = null,
    fec: Boolean = false,
  ): Int {
    val inputSize = inputSize ?: input?.remaining() ?: 0
    val frameSize = frameSize ?: output.capacity() / channels
    return decodeShort(nativeHandler, input, inputSize, output, frameSize, fec)
  }

  private external fun decodeFloat(
    nativeHandler: Long,
    input: ByteBuffer?,
    inputSize: Int,
    output: FloatBuffer,
    frameSize: Int,
    fec: Boolean,
  ): Int

  // You must guarantee that output has enough space to hold the decoded data. Otherwise this is undefined behavior.
  fun decodeFloat(
    input: ByteBuffer?,
    output: FloatBuffer,
    inputSize: Int? = null,
    frameSize: Int? = null,
    fec: Boolean = false,
  ): Int {
    val inputSize = inputSize ?: input?.remaining() ?: 0
    val frameSize = frameSize ?: output.capacity() / channels
    return decodeFloat(nativeHandler, input, inputSize, output, frameSize, fec)
  }

}
