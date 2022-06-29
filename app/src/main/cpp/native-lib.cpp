#include <jni.h>
#include <string>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <GLES3/gl3.h>

#define LOG_TAG "opengl-demo"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

AAssetManager *g_pAssetManager = NULL;
GLint g_ShaderProgram;
jint g_width;
jint g_height;

char *readShaderFile(char *shaderFile) {
    if (NULL == g_pAssetManager) {
        LOGE("pAssetManager is null!");
        return NULL;
    }
    AAsset *pAsset = NULL;
    off_t size = -1;
    char *pBuffer = NULL;
    int numberByte = -1;

    pAsset = AAssetManager_open(g_pAssetManager, shaderFile, AASSET_MODE_UNKNOWN);
    size = AAsset_getLength(pAsset);
    LOGI("after AAssetManager_open");
    pBuffer = (char *) malloc(size + 1);
    pBuffer[size] = '\0';
    numberByte = AAsset_read(pAsset, pBuffer, size);
    LOGI("shaderFile:%s,pBuffer:%s", shaderFile, pBuffer);
    AAsset_close(pAsset);
    return pBuffer;
}

GLuint loadShader(GLenum type, const char *shaderSrc) {
    GLuint shader;
    GLint compiled;
    // Create the shader object
    shader = glCreateShader(type);
    if (shader == 0) {
        return 0;
    }
    // Load the shader source
    glShaderSource(shader, 1, &shaderSrc, NULL);

    // Compile the shader
    glCompileShader(shader);

    // Check the compile status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char *infoLog = (char *) malloc(sizeof(char) * infoLen);
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            LOGE("Error compiling shader:[%s]", infoLog);
            free(infoLog);
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}


extern "C" JNIEXPORT void JNICALL
Java_com_treech_opengl_demo_MyRender_loadShaderFile(
        JNIEnv *env,
        jobject self,
        jobject assetManager) {
    if (assetManager && env) {
        LOGI("before AAssetManager_fromJava");
        g_pAssetManager = AAssetManager_fromJava(env, assetManager);
        LOGI("after AAssetManager_fromJava");
        if (g_pAssetManager == NULL) {
            LOGE("AAssetManager_fromJava() return null !");
        }
    } else {
        LOGE("assetManager is null !");
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_treech_opengl_demo_MyRender_glesInit(
        JNIEnv *env,
        jobject self) {
    char *pVertexShader = readShaderFile("shader/vs.glsl");
    char *pFragmentShader = readShaderFile("shader/fs.glsl");

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint shaderProgram;
    GLint linkStatus;

    vertexShader = loadShader(GL_VERTEX_SHADER, pVertexShader);
    fragmentShader = loadShader(GL_FRAGMENT_SHADER, pFragmentShader);

    // Create the program object
    shaderProgram = glCreateProgram();
    if (shaderProgram == 0) {
        LOGE("shaderProgram create error!");
        return;
    }
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    // Link the program
    glLinkProgram(shaderProgram);

    // Check the link status
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);

    if (!linkStatus) {
        GLint infoLen = 0;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char *infoLog = (char *) malloc(sizeof(char) * infoLen);
            glGetShaderInfoLog(shaderProgram, infoLen, NULL, infoLog);
            LOGE("Error linking program:[%s]", infoLog);
            free(infoLog);
        }
        glDeleteProgram(shaderProgram);
        return;
    }
    // Store the program object
    g_ShaderProgram = shaderProgram;
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

}

extern "C" JNIEXPORT void JNICALL
Java_com_treech_opengl_demo_MyRender_glesResize(
        JNIEnv *env,
        jobject self,
        jint width,
        jint height) {
    g_width = width;
    g_height = height;
}

extern "C" JNIEXPORT void JNICALL
Java_com_treech_opengl_demo_MyRender_glesRender(
        JNIEnv *env,
        jobject self) {
    // Set the viewport
    glViewport(0, 0, g_width, g_height);

    // Use the program object
    glUseProgram(g_ShaderProgram);

    // Load the vertex data
    GLfloat vVertices[] = {
            0.0f, 0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
    };
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}