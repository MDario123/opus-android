package com.theeasiestway.opus

import android.util.Log

object OpusLibLoader {
    init {
        try {
            System.loadLibrary("easyopus")
        } catch (e: Exception) {
            Log.e("OpusLibLoader", "Couldn't load opus library: $e")
        }
    }
}
