#include "game/EventBus.hpp"

void EventBus::subscribe(const std::string& eventName, Handler handler) {
    handlers_[eventName].push_back(std::move(handler));
}

void EventBus::publish(const std::string& eventName, const std::string& payload) const {
    const auto found = handlers_.find(eventName);
    if (found == handlers_.end()) {
        return;
    }
    for (const auto& handler : found->second) {
        handler(payload);
    }
}
