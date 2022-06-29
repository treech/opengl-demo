package com.treech.opengl.demo

import android.content.Context
import android.content.res.AssetManager
import android.opengl.GLSurfaceView
import android.util.Log
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MyRender(context: Context) : GLSurfaceView.Renderer {

    companion object {
        private const val TAG = "MyRender"

        init {
            System.loadLibrary("demo")
        }
    }

    private var assetManager: AssetManager = context.assets

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        Log.e(TAG, "onSurfaceCreated")
        loadShaderFile(assetManager)
        glesInit()
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        Log.e(TAG, "onSurfaceChanged")
        glesResize(width, height)
    }

    override fun onDrawFrame(gl: GL10?) {
        Log.e(TAG, "onDrawFrame")
        glesRender()
    }

    external fun loadShaderFile(assetManager: AssetManager)

    external fun glesInit()

    external fun glesResize(width: Int, height: Int)

    external fun glesRender()
}