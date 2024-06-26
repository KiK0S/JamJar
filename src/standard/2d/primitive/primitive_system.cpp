#include "standard/2d/primitive/primitive_system.hpp"
#include "game.hpp"
#include "message/message_payload.hpp"
#include "standard/2d/primitive/primitive.hpp"
#include "standard/2d/render/render_system.hpp"
#include "standard/2d/render/renderable.hpp"
#include "standard/2d/transform/transform.hpp"
#include <array>
#include <memory>

const std::string JamJar::Standard::_2D::PrimitiveSystem::DEFAULT_PRIMITIVE_VERTEX_SHADER_NAME =
    "jamjar_default_primitive_vertex";
const std::string JamJar::Standard::_2D::PrimitiveSystem::DEFAULT_PRIMITIVE_FRAGMENT_SHADER_NAME =
    "jamjar_default_primitive_fragment";

JamJar::Standard::_2D::PrimitiveSystem::PrimitiveSystem(MessageBus *messageBus)
    : VectorSystem(messageBus, JamJar::Standard::_2D::PrimitiveSystem::evaluator) {
    this->messageBus->Subscribe(this, JamJar::Game::MESSAGE_PRE_RENDER);
#ifdef __EMSCRIPTEN__
    this->loadWebgl2Shaders();
#else
    this->loadOpenGLShaders();
#endif
}

#ifdef __EMSCRIPTEN__
#include "standard/2d/primitive/webgl2_default_primitive_shaders.hpp"
#include "standard/2d/webgl2/webgl2_system.hpp"
void JamJar::Standard::_2D::PrimitiveSystem::loadWebgl2Shaders() {
    // Load WebGL2 fragment shader
    auto fragShader = std::make_unique<WebGL2DefaultPrimitiveFragmentShader>(
        JamJar::Standard::_2D::WebGL2DefaultPrimitiveFragmentShader());
    this->messageBus->Publish(
        std::make_unique<JamJar::MessagePayload<std::unique_ptr<WebGL2DefaultPrimitiveFragmentShader>>>(
            JamJar::Standard::_2D::WebGL2System::MESSAGE_LOAD_SHADER, std::move(fragShader)));
    // Load WebGL2 vertex shader
    auto vertShader = std::make_unique<WebGL2DefaultPrimitiveVertexShader>(
        JamJar::Standard::_2D::WebGL2DefaultPrimitiveVertexShader());
    this->messageBus->Publish(
        std::make_unique<JamJar::MessagePayload<std::unique_ptr<WebGL2DefaultPrimitiveVertexShader>>>(
            JamJar::Standard::_2D::WebGL2System::MESSAGE_LOAD_SHADER, std::move(vertShader)));
}
#endif


#ifndef __EMSCRIPTEN__
#include "standard/2d/primitive/opengl2_default_primitive_shaders.hpp"
#include "standard/2d/opengl2/opengl2_system.hpp"
void JamJar::Standard::_2D::PrimitiveSystem::loadOpenGLShaders() {
    // Load OpenGL fragment shader
    auto fragShader = std::make_unique<OpenGLDefaultPrimitiveFragmentShader>(
        JamJar::Standard::_2D::OpenGLDefaultPrimitiveFragmentShader());
    this->messageBus->Publish(
        std::make_unique<JamJar::MessagePayload<std::unique_ptr<OpenGLDefaultPrimitiveFragmentShader>>>(
            JamJar::Standard::_2D::OpenGLSystem::MESSAGE_LOAD_SHADER, std::move(fragShader)));
    // Load OpenGL vertex shader
    auto vertShader = std::make_unique<OpenGLDefaultPrimitiveVertexShader>(
        JamJar::Standard::_2D::OpenGLDefaultPrimitiveVertexShader());
    this->messageBus->Publish(
        std::make_unique<JamJar::MessagePayload<std::unique_ptr<OpenGLDefaultPrimitiveVertexShader>>>(
            JamJar::Standard::_2D::OpenGLSystem::MESSAGE_LOAD_SHADER, std::move(vertShader)));
}
#endif

bool JamJar::Standard::_2D::PrimitiveSystem::evaluator(Entity *entity,
                                                       const std::vector<JamJar::Component *> &components) {
    bool hasPrimitive = false;
    bool hasTransform = false;
    for (const auto &component : components) {
        if (component->key == JamJar::Standard::_2D::Primitive::KEY) {
            hasPrimitive = true;
        }
        if (component->key == JamJar::Standard::_2D::Transform::KEY) {
            hasTransform = true;
        }
        if (hasPrimitive && hasTransform) {
            return true;
        }
    }
    return false;
}

void JamJar::Standard::_2D::PrimitiveSystem::OnMessage(JamJar::Message *message) {
    VectorSystem::OnMessage(message);
    switch (message->type) {
    case JamJar::Game::MESSAGE_PRE_RENDER: {
        auto *renderMessage = static_cast<JamJar::MessagePayload<float> *>(message);
        this->preRender(renderMessage->payload);
        break;
    }
    }
}

void JamJar::Standard::_2D::PrimitiveSystem::preRender(float alpha) {
    std::vector<Renderable> renderables;
    for (auto &entity : this->entities) {
        auto transform = entity.Get<JamJar::Standard::_2D::Transform>();
        auto primitive = entity.Get<JamJar::Standard::_2D::Primitive>();

        renderables.push_back({.vertices = primitive->polygon.PointsAsFloats(),
                               .modelMatrix = transform->InterpolatedMatrix4D(alpha),
                               .material = primitive->material,
                               .drawMode = primitive->drawMode});
    }
    auto msg = std::make_unique<JamJar::MessagePayload<std::vector<Renderable>>>(
        JamJar::Standard::_2D::RenderSystem::MESSAGE_LOAD_RENDERABLES, renderables);
    this->messageBus->Publish(std::move(msg));
}
