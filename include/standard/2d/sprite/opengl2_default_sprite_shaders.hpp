#pragma once
#ifndef __EMSCRIPTEN__
#include "standard/2d/opengl2/opengl2_shader.hpp"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>

namespace JamJar::Standard::_2D {

class OpenGLDefaultSpriteVertexShader : public OpenGLShader {
  public:
    explicit OpenGLDefaultSpriteVertexShader();
    void PerProgram(OpenGLShaderContext *context) override;
    void PerTexture(OpenGLShaderContext *context, JamJar::Texture *texture, GLuint textureRef) override;
    void PerRenderable(OpenGLShaderContext *context, JamJar::Texture *texture, Renderable *renderable,
                       GLuint textureRef) override;
};

class OpenGLDefaultSpriteFragmentShader : public OpenGLShader {
  public:
    explicit OpenGLDefaultSpriteFragmentShader();
    void PerProgram(OpenGLShaderContext *context) override;
    void PerTexture(OpenGLShaderContext *context, JamJar::Texture *texture, GLuint textureRef) override;
    void PerRenderable(OpenGLShaderContext *context, JamJar::Texture *texture, Renderable *renderable,
                       GLuint textureRef) override;
};
}; // namespace JamJar::Standard::_2D

#endif
