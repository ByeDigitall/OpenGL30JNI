//
// Created by Raining on 2020/8/14.
// #I# 脸部点渲染图像
//

#include <GLES3/gl3.h>
#include <jni.h>
#include "esUtils.h"
#include <cmath>
#include <algorithm>

namespace example19 {
    // 人脸图像索引(702个点)
    static const GLushort FACE_VERTICES_INDEX[] = {
            // 一共234个三角形
            // 脸外索引(人脸顶部中心逆时针数) 44个三角形
            110, 114, 111,
            111, 114, 115,
            115, 111, 32,
            32, 115, 116,
            116, 32, 31,
            31, 116, 30,
            30, 116, 29,
            29, 116, 28,
            28, 116, 27,
            27, 116, 26,
            26, 116, 25,
            25, 116, 117,
            117, 25, 24,
            24, 117, 23,
            23, 117, 22,
            22, 117, 21,
            21, 117, 20,
            20, 117, 19,
            19, 117, 118,
            118, 19, 18,
            18, 118, 17,
            17, 118, 16,
            16, 118, 15,
            15, 118, 14,
            14, 118, 13,
            13, 118, 119,
            119, 13, 12,
            12, 119, 11,
            11, 119, 10,
            10, 119, 9,
            9, 119, 8,
            8, 119, 7,
            7, 119, 120,
            120, 7, 6,
            6, 120, 5,
            5, 120, 4,
            4, 120, 3,
            3, 120, 2,
            2, 120, 1,
            1, 120, 0,
            0, 120, 121,
            121, 0, 109,
            109, 121, 114,
            114, 109, 110,
            // 脸内部索引
            // 额头 14个三角形
            0, 33, 109,
            109, 33, 34,
            34, 109, 35,
            35, 109, 36,
            36, 109, 110,
            36, 110, 37,
            37, 110, 43,
            43, 110, 38,
            38, 110, 39,
            39, 110, 111,
            111, 39, 40,
            40, 111, 41,
            41, 111, 42,
            42, 111, 32,
            // 左眉毛  10个三角形
            33, 34, 64,
            64, 34, 65,
            65, 34, 107,
            107, 34, 35,
            35, 36, 107,
            107, 36, 66,
            66, 107, 65,
            66, 36, 67,
            67, 36, 37,
            37, 67, 43,
            // 右眉毛 10个三角形
            43, 38, 68,
            68, 38, 39,
            39, 68, 69,
            39, 40, 108,
            39, 108, 69,
            69, 108, 70,
            70, 108, 41,
            41, 108, 40,
            41, 70, 71,
            71, 41, 42,
            // 左眼 21个三角形
            0, 33, 52,
            33, 52, 64,
            52, 64, 53,
            64, 53, 65,
            65, 53, 72,
            65, 72, 66,
            66, 72, 54,
            66, 54, 67,
            54, 67, 55,
            67, 55, 78,
            67, 78, 43,
            52, 53, 57,
            53, 72, 74,
            53, 74, 57,
            74, 57, 73,
            72, 54, 104,
            72, 104, 74,
            74, 104, 73,
            73, 104, 56,
            104, 56, 54,
            54, 56, 55,
            // 右眼 21个三角形
            68, 43, 79,
            68, 79, 58,
            68, 58, 59,
            68, 59, 69,
            69, 59, 75,
            69, 75, 70,
            70, 75, 60,
            70, 60, 71,
            71, 60, 61,
            71, 61, 42,
            42, 61, 32,
            61, 60, 62,
            60, 75, 77,
            60, 77, 62,
            77, 62, 76,
            75, 77, 105,
            77, 105, 76,
            105, 76, 63,
            105, 63, 59,
            105, 59, 75,
            59, 63, 58,
            // 左脸颊 16个
            0, 52, 1,
            1, 52, 2,
            2, 52, 57,
            2, 57, 3,
            3, 57, 4,
            4, 57, 112,
            57, 112, 74,
            74, 112, 56,
            56, 112, 80,
            80, 112, 82,
            82, 112, 7,
            7, 112, 6,
            6, 112, 5,
            5, 112, 4,
            56, 80, 55,
            55, 80, 78,
            // 右脸颊 16个
            32, 61, 31,
            31, 61, 30,
            30, 61, 62,
            30, 62, 29,
            29, 62, 28,
            28, 62, 113,
            62, 113, 76,
            76, 113, 63,
            63, 113, 81,
            81, 113, 83,
            83, 113, 25,
            25, 113, 26,
            26, 113, 27,
            27, 113, 28,
            63, 81, 58,
            58, 81, 79,
            // 鼻子部分 16个
            78, 43, 44,
            43, 44, 79,
            78, 44, 80,
            79, 81, 44,
            80, 44, 45,
            44, 81, 45,
            80, 45, 46,
            45, 81, 46,
            80, 46, 82,
            81, 46, 83,
            82, 46, 47,
            47, 46, 48,
            48, 46, 49,
            49, 46, 50,
            50, 46, 51,
            51, 46, 83,
            // 鼻子和嘴巴中间三角形 14个
            7, 82, 84,
            82, 84, 47,
            84, 47, 85,
            85, 47, 48,
            48, 85, 86,
            86, 48, 49,
            49, 86, 87,
            49, 87, 88,
            88, 49, 50,
            88, 50, 89,
            89, 50, 51,
            89, 51, 90,
            51, 90, 83,
            83, 90, 25,
            // 上嘴唇部分 10个
            84, 85, 96,
            96, 85, 97,
            97, 85, 86,
            86, 97, 98,
            86, 98, 87,
            87, 98, 88,
            88, 98, 99,
            88, 99, 89,
            89, 99, 100,
            89, 100, 90,
            // 下嘴唇部分 10个
            90, 100, 91,
            100, 91, 101,
            101, 91, 92,
            101, 92, 102,
            102, 92, 93,
            102, 93, 94,
            102, 94, 103,
            103, 94, 95,
            103, 95, 96,
            96, 95, 84,
            // 唇间部分 8个
            96, 97, 103,
            97, 103, 106,
            97, 106, 98,
            106, 103, 102,
            106, 102, 101,
            106, 101, 99,
            106, 98, 99,
            99, 101, 100,
            // 嘴巴与下巴之间的部分(关键点7 到25 与嘴巴鼻翼围起来的区域) 24个
            7, 84, 8,
            8, 84, 9,
            9, 84, 10,
            10, 84, 95,
            10, 95, 11,
            11, 95, 12,
            12, 95, 94,
            12, 94, 13,
            13, 94, 14,
            14, 94, 93,
            14, 93, 15,
            15, 93, 16,
            16, 93, 17,
            17, 93, 18,
            18, 93, 92,
            18, 92, 19,
            19, 92, 20,
            20, 92, 91,
            20, 91, 21,
            21, 91, 22,
            22, 91, 90,
            22, 90, 23,
            23, 90, 24,
            24, 90, 25
    };

    static float FACE_POINTS[] = {-0.41607684f, 0.02666843f, -0.4268434f, 0.06034112f, -0.43971097f,
                                  0.09437966f, -0.4520738f, 0.12887442f, -0.4613266f, 0.16391492f,
                                  -0.46486378f, 0.19959104f, -0.4600796f, 0.23599291f, -0.4529044f,
                                  0.2724402f, -0.44499326f, 0.30819762f, -0.4356215f, 0.34361434f,
                                  -0.42406446f, 0.37903976f, -0.40959746f, 0.41482258f,
                                  -0.39072978f,
                                  0.44959092f, -0.37770522f, 0.48690987f, -0.36080658f, 0.5233824f,
                                  -0.3303163f, 0.5556115f, -0.2765174f, 0.58020055f, -0.18581086f,
                                  0.5954975f, -0.0939731f, 0.5963398f, -0.0037860274f, 0.58619654f,
                                  0.08196843f, 0.56853676f, 0.1605084f, 0.5468298f, 0.23528206f,
                                  0.5212915f, 0.30362582f, 0.4915533f, 0.36413372f, 0.45753717f,
                                  0.41539955f, 0.41916502f, 0.45601773f, 0.37635875f, 0.4872223f,
                                  0.33116317f, 0.5114963f, 0.2852968f, 0.529922f, 0.23886192f,
                                  0.54358125f, 0.19196069f, 0.5535563f, 0.14469528f, 0.56092906f,
                                  0.09716773f, -0.4197306f, -0.020263314f, -0.3762017f,
                                  -0.03779477f,
                                  -0.32228768f, -0.030806363f, -0.26802963f, -0.013278782f,
                                  -0.22254944f, 0.007016182f, -0.065100014f, 0.005636096f,
                                  0.020704746f, -0.0085808635f, 0.12354565f, -0.013681054f,
                                  0.22806025f, -0.002231419f, 0.31234872f, 0.03733945f, -0.1476525f,
                                  0.074893355f, -0.19111615f, 0.13800204f, -0.23457962f, 0.2011106f,
                                  -0.27804327f, 0.26421928f, -0.3335632f, 0.28640628f, -0.31228703f,
                                  0.30499935f, -0.2586621f, 0.31973672f, -0.18040603f, 0.3208021f,
                                  -0.14188808f, 0.3161764f, -0.39120162f, 0.060181975f,
                                  -0.35182935f,
                                  0.052343607f, -0.26695675f, 0.06657672f, -0.24202561f, 0.0878551f,
                                  -0.28185135f, 0.08731961f, -0.36193204f, 0.07364309f,
                                  -0.003924966f, 0.110797286f, 0.045706153f, 0.09257674f,
                                  0.16361117f, 0.09696889f, 0.21205103f, 0.11447346f, 0.16095483f,
                                  0.122215986f, 0.048149467f, 0.118716f, -0.38395828f,
                                  -0.006072879f,
                                  -0.33818173f, 0.004476905f, -0.28603107f, 0.012991667f,
                                  -0.2369507f, 0.028032541f, -0.06116271f, 0.034684896f,
                                  0.025626302f, 0.027729988f, 0.117981315f, 0.029453516f,
                                  0.21348822f, 0.03380215f, -0.30764872f, 0.05427766f, -0.3239597f,
                                  0.08334923f, -0.305974f, 0.06963539f, 0.10589993f, 0.08866787f,
                                  0.10444236f, 0.12411618f, 0.10315454f, 0.1048094f, -0.21641707f,
                                  0.09033024f, -0.06338298f, 0.10387051f, -0.31865335f, 0.21534562f,
                                  -0.08118874f, 0.24477315f, -0.36504382f, 0.2639575f, -0.06607175f,
                                  0.30585945f, -0.38998228f, 0.41900563f, -0.36954975f, 0.40027583f,
                                  -0.32491267f, 0.39387953f, -0.29209626f, 0.40457928f, -0.2510705f,
                                  0.4040773f, -0.17375386f, 0.4284761f, -0.10671067f, 0.46086657f,
                                  -0.16337061f, 0.47787702f, -0.22969055f, 0.48638022f, -0.3032189f,
                                  0.48163688f, -0.34993017f, 0.4657514f, -0.37417328f, 0.443514f,
                                  -0.37554967f, 0.42172122f, -0.34900528f, 0.41713595f,
                                  -0.29389447f,
                                  0.42451608f, -0.19870704f, 0.438174f, -0.13476866f, 0.45730317f,
                                  -0.20088011f, 0.45697927f, -0.29363436f, 0.44970465f,
                                  -0.34650302f,
                                  0.43441033f, -0.305974f, 0.06963539f, 0.10315454f, 0.1048094f,
                                  -0.2937644f, 0.4371103f, -0.3302347f, -0.013164699f, 0.12076354f,
                                  0.007886171f, -0.19129878f, -0.084002316f, -0.006395817f,
                                  -0.13020974f, 0.110832214f, -0.06622058f, -0.41318518f,
                                  0.21393621f, 0.22271216f, 0.29557824f, 0.0f, -1.0f, 1.3333333f,
                                  -1.0f, 1.3333333f, 0.0f, 1.3333333f, 1.0f, 0.0f, 1.0f,
                                  -1.3333334f,
                                  1.0f, -1.3333334f, 0.0f, -1.3333334f, -1.0f};

    /*static float FACE_POINTS[] = {
            1.0f, 1.0f, // 右上角
            1.0f, -1.0f, // 右下角
            -1.0f, -1.0f, // 左下角
            -1.0f, 1.0f, // 左上角
    };

    static GLushort FACE_VERTICES_INDEX[] = { // 注意索引从0开始!
            0, 1, 3, // 第一个三角形
            1, 2, 3  // 第二个三角形
    };*/

    static esUtils::Shader *shader = nullptr;
    static GLuint vertexEBO;
}

using namespace example19;

extern "C"
JNIEXPORT void JNICALL
Java_com_ideacarry_example19_GLRenderer_surfaceCreated(JNIEnv *env, jobject thiz, jobject context, jint bg_color) {
    shader = new esUtils::Shader(env, context, "example19/vertex.glsl",
                                 "example19/fragment.glsl");

    //索引点
    glGenBuffers(1, &vertexEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(FACE_VERTICES_INDEX),
                 FACE_VERTICES_INDEX, GL_STATIC_DRAW);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_ideacarry_example19_GLRenderer_surfaceChanged(JNIEnv *env, jobject thiz, jint width, jint height) {
    glViewport(0, 0, width, height);
}

void logicalPointChangeRange(float *points, int count, float srcRangeA, float srcRangeB, float dstRangeA, float dstRangeB) {
    if (points && count > 0) {
        float srcRange = std::abs(srcRangeA - srcRangeB);
        float srcOffset = std::min(srcRangeA, srcRangeB);
        float dstRange = std::abs(dstRangeA - dstRangeB);
        float dstOffset = std::min(dstRangeA, dstRangeB);
        for (int i = 0; i < count; i++) {
            points[i] = (points[i] - srcOffset) / srcRange * dstRange + dstOffset;
        }
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_ideacarry_example19_GLRenderer_drawFrame(JNIEnv *env, jobject thiz, jobject bmp) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader->use();
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexEBO);

    //顶点坐标系
    GLuint vertexVBO;
    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(FACE_POINTS), FACE_POINTS, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    GLuint texture = loadAssetsTexture2D(env, bmp, GL_CLAMP_TO_EDGE);
    //绑定图像texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    //绘制
    glDrawElements(GL_TRIANGLES, sizeof(FACE_VERTICES_INDEX) / sizeof(GLushort),
                   GL_UNSIGNED_SHORT, nullptr);

    glDeleteTextures(1, &texture);
    glDeleteBuffers(1, &vertexVBO);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}