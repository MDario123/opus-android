package com.theeasiestway.opus

class OpusException(
    val errorCode: Int,
    message: String
) : RuntimeException("Opus error $errorCode: $message")
