#ifndef __EMSCRIPTEN__
#include "standard/2d/opengl2/opengl2_shader.hpp"

JamJar::Standard::_2D::OpenGLShader::OpenGLShader(OpenGLShaderType type, std::string name, std::string source)
    : type(type), name(name), source(source){};
#endif