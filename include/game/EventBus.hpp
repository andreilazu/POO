#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

class EventBus {
public:
    using Handler = std::function<void(const std::string&)>;

    void subscribe(const std::string& eventName, Handler handler);
    void publish(const std::string& eventName, const std::string& payload) const;

private:
    std::unordered_map<std::string, std::vector<Handler>> handlers_;
};
