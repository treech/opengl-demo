package com.treech.opengl.demo

import android.app.ActivityManager
import android.content.Context
import android.opengl.GLSurfaceView
import android.opengl.GLSurfaceView.RENDERMODE_WHEN_DIRTY
import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import com.treech.opengl.demo.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding
    private lateinit var glSurfaceView: GLSurfaceView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        glSurfaceView = GLSurfaceView(this)
        if (checkSupportGLES30()) {
            glSurfaceView.setEGLContextClientVersion(3)
            glSurfaceView.setRenderer(MyRender(this))
            glSurfaceView.renderMode = RENDERMODE_WHEN_DIRTY
            setContentView(glSurfaceView)
        } else {
            setContentView(binding.root)
            Log.e(TAG, "OpenGL ES 3.0 not supported on device.")
        }

        // Example of a call to a native method
        binding.sampleText.text = "test"
    }

    override fun onResume() {
        super.onResume()
        glSurfaceView.onResume()
    }

    override fun onPause() {
        super.onPause()
        glSurfaceView.onPause()
    }

    private fun checkSupportGLES30(): Boolean {
        val activityManager = getSystemService(Context.ACTIVITY_SERVICE) as ActivityManager
        return activityManager.deviceConfigurationInfo.reqGlEsVersion >= 0x30000
    }

    companion object {
        private const val TAG = "MainActivity"
    }
}