#pragma once

#include <memory>
#include <vector>

template <typename T>
class ObjectPool {
public:
    explicit ObjectPool(std::size_t capacity = 64) : storage_(), freeList_() {
        storage_.reserve(capacity);
        freeList_.reserve(capacity);
    }

    template <typename... Args>
    T* acquire(Args&&... args) {
        if (!freeList_.empty()) {
            T* object = freeList_.back();
            freeList_.pop_back();
            *object = T(std::forward<Args>(args)...);
            return object;
        }
        storage_.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        return storage_.back().get();
    }

    void release(T* object) {
        if (object != nullptr) {
            freeList_.push_back(object);
        }
    }

    std::size_t activeCount() const {
        return storage_.size() - freeList_.size();
    }

private:
    std::vector<std::unique_ptr<T>> storage_;
    std::vector<T*> freeList_;
};
