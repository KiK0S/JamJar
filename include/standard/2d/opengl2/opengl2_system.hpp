#pragma once
#ifndef __EMSCRIPTEN__

#include "component/component.hpp"
#include "entity/entity.hpp"
#include "hash.hpp"
#include "message/listener.hpp"
#include "message/message.hpp"
#include "message/message_bus.hpp"
#include "render/texture.hpp"
#include "standard/2d/render/render_system.hpp"
#include "standard/2d/opengl2/opengl2_shader.hpp"
#include "standard/file_texture/file_texture_response.hpp"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <cstdint>
#include <string>
#include <unordered_map>

namespace JamJar::Standard::_2D {
struct OpenGLLoadedShader {
    std::unique_ptr<OpenGLShader> definition;
    GLuint shader;
};

class OpenGLSystem : public RenderSystem {
  private:
    SDL_Window *window;
    std::unordered_map<std::uint32_t, GLuint> textures;
    GLuint m_defaultTextureRef;
    JamJar::Texture m_defaultTexture;
    std::unordered_map<std::string, std::unique_ptr<OpenGLLoadedShader>> m_shaders;
    std::unordered_map<std::string, GLuint> m_programs;
    static bool evaluator(Entity *entity, std::vector<JamJar::Component *> components);
    void loadTexture(FileTextureResponse *response);
    GLuint createTexture(const int width, const int height, JamJar::TextureProperties properties, const void *data);
    void loadShader(std::unique_ptr<OpenGLShader> shader);
    GLuint getProgram(std::string identifier, GLuint fragment, GLuint vertex);

  protected:
    void render(float deltaTime) override;

  public:
    constexpr static uint32_t MESSAGE_LOAD_SHADER = hash("jamjar_2d_opengl_load_shader");
    explicit OpenGLSystem(MessageBus *messageBus, SDL_Window *window);
    void OnMessage(JamJar::Message *message) override;
};
}; // namespace JamJar::Standard::_2D

#endif
