#ifndef MESSAGE_LISTENER_HPP
#define MESSAGE_LISTENER_HPP

#include "message/message.hpp"
#include <cstdint>

namespace JamJar {
class Listener {
  private:
    static uint32_t ID;

  public:
    Listener();
    uint32_t m_listenerID;
    virtual void OnMessage(Message *message) = 0;
};
}; // namespace JamJar

#endif