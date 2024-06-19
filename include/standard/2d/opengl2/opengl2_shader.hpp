#pragma once
#ifndef __EMSCRIPTEN__

#include "render/texture.hpp"
#include "standard/2d/camera/camera.hpp"
#include "standard/2d/render/renderable.hpp"
#include "standard/2d/transform/transform.hpp"
#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <optional>
#include <string>

namespace JamJar::Standard::_2D {

enum OpenGLShaderType { VERTEX, FRAGMENT };

struct OpenGLShaderContext {
    GLuint program;
    Camera *camera;
    Transform *transform;
};

class OpenGLShader {
  public:
    explicit OpenGLShader(OpenGLShaderType type, std::string name, std::string source);
    OpenGLShaderType type;
    std::string name;
    std::string source;
    virtual void PerProgram(OpenGLShaderContext *context) = 0;
    virtual void PerTexture(OpenGLShaderContext *context, JamJar::Texture *texture, GLuint textureRef) = 0;
    virtual void PerRenderable(OpenGLShaderContext *context, JamJar::Texture *texture, Renderable *renderable,
                               GLuint textureRef) = 0;
    virtual ~OpenGLShader(){};
};
}; // namespace JamJar::Standard::_2D

#endif