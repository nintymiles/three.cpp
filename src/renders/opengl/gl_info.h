//
// Created by SeanR on 2023/6/14.
//

#ifndef THREE_CPP_GL_INFO_H
#define THREE_CPP_GL_INFO_H

#include <vector>
#include <memory>

class GLProgram;

using programPtr = std::shared_ptr<GLProgram>;

class GLInfo {
    struct Memory {
        unsigned geometries;
        unsigned textures;
        Memory() :geometries(0), textures(0) {}
    };

    struct Render {
        unsigned frame;
        unsigned calls;
        unsigned triangles;
        unsigned points;
        unsigned lines;
        Render() :frame(0), calls(0), triangles(0), points(0), lines(0) {}
    };
public:
    using sptr = std::shared_ptr<GLInfo>;
    Memory memory;

    Render render;

    std::vector<programPtr> programs;

    bool autoReset = true;

    GLInfo() {}

    //virtual ~GLInfo() = default;

    void update(const unsigned count, const unsigned mode, unsigned instanceCount = 0);
    void reset();
};

//class GLProgram;
//
//class GLInfo{
//public:
//    using GLProgramPtr = std::shared_ptr<GLProgram>;
//
//    struct MemoryInfo{
//        unsigned geometries;
//        unsigned textures;
//
//        MemoryInfo() :geometries(0), textures(0) {}
//    };
//
//    struct RenderInfo{
//        unsigned frame;
//        unsigned calls;
//        unsigned triangles;
//        unsigned points;
//        unsigned lines;
//
//        RenderInfo() :frame(0), calls(0), triangles(0), points(0), lines(0) {}
//    };
//
//    MemoryInfo memory;
//    RenderInfo render;
//
//    std::vector<GLProgramPtr> programs;
//    bool autoReset = true;
//
//    using sptr = std::shared_ptr<GLInfo>;
//
//    GLInfo& reset(){
//        render.calls = 0;
//        render.frame = 0;
//        render.lines = 0;
//        render.points = 0;
//        render.triangles = 0;
//
//        return *this;
//    }
//
//    GLInfo& update(int count,GLenum mode,int instanceCount){
//        render.calls ++;
//        switch (mode) {
//            case GL_TRIANGLES:
//                render.triangles += instanceCount * ( count / 3 );
//                break;
//
//            case GL_LINES:
//                render.lines += instanceCount * ( count / 2 );
//                break;
//
//            case GL_LINE_STRIP:
//                render.lines += instanceCount * ( count - 1 );
//                break;
//
//            case GL_LINE_LOOP:
//                render.lines += instanceCount * count;
//                break;
//
//            case GL_POINTS:
//                render.points += instanceCount * count;
//                break;
//
//            default:
//                break;
//
//        }
//        return *this;
//    }
//
//
//};

#endif //THREE_CPP_GL_INFO_H
