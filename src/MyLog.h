#pragma once
#include<GLEW/glew.h>
#include<iostream>
#define CheckGL(x) ErrorClear();x;GetError(#x,__FILE__,__LINE__);
void static ErrorClear() {
    while (glGetError() != GL_NO_ERROR);
}
void static GetError(const std::string file, const std::string function, int line) {
    GLenum error;
    if ((error = glGetError()))
    {
        std::cout << "OpenGL error:" << error << ",in" << file << "," << function << ", line" << line << std::endl;
    }
}

