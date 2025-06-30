# opus-android

Android wrapper around [libopus 1.3.1](https://opus-codec.org/release/stable/2019/04/12/libopus-1_3_1.html) written on C++ and Kotlin.

## Pending tasks

- [ ] Add support for all libopus features
- [ ] Add definition for constants

## Supported ABIs:

armeabi-v7a, arm64-v8a, x86, x86_64

## How to use

```kotlin
// Constants
val SAMPLE_RATE = 48000
val CHANNELS = 1
val APPLICATION = 2048 // VOIP
var FRAME_SIZE = 960

// Encode
val encoder = Encoder(SAMPLE_RATE, CHANNELS, APPLICATION)
assertNotNull(encoder)
val errorCode = encoder.setBitrate(16_000) // set bitrate to 16 kbps
assertEquals(errorCode, 0) {"Error ocurred: $errorCode"}

// It's important that the buffers are direct buffers
val inputBuffer = ByteBuffer.allocateDirect(Float.SIZE_BYTES * FRAME_SIZE * CHANNELS)
inputBuffer.order(ByteOrder.nativeOrder())
val inputBufferAsFloat = inputBuffer.asFloatBuffer()

val encodedBuffer = ByteBuffer.allocateDirect(1024)
encodedBuffer.order(ByteOrder.nativeOrder())

for (i in 0 until inputBufferAsFloat.remaining()) {
    inputBufferAsFloat.put(0.5f)
}

val amountEncoded = encoder.encodeFloat(inputBuffer.asFloatBuffer(), OPUS_FRAME_SIZE, encodedBuffer)

assert(amountEncoded > 0) {"Error ocurred: $amountEncoded"}

encoder.release()

// Decode
val decoder = Decoder(SAMPLE_RATE, CHANNELS)
assertNotNull(decoder)

val decoded = decoder.decodeFloat(encodedBuffer, inputBufferAsFloat)

assertEquals(decoded, OPUS_FRAME_SIZE)

decoder.release()
```

## Project structure

#### Compiled library:

- **opus.aar** - it's a compiled library of **opus** module that mentioned above, get it on release, you can easily add it to your project using gradle dependencies. First you have to place **opus.aar** in the libs folder of your project and then add to your build.gradle the following:

```groovy
dependencies {
    api fileTree(dir: 'libs', include: '*.jar')       // this line is necessary in order to allow gradle to take opus.aar from "libs" dir
    api files('libs/opus.aar')                        // dependency for opus.aar library
    ...                                               // other dependencies
}
```
