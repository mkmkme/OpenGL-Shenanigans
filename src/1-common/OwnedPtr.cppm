module;

#include <functional>
#include <utility>

export module OwnedPtr;

export template <typename T, T Invalid = {}>
class OwnedPtr {
    T ptr;
    std::function<void(T)> deleter;

public:
    OwnedPtr() noexcept
        : ptr(Invalid)
        , deleter(nullptr)
    {
    }

    OwnedPtr(T ptr, std::function<void(T)> deleter) noexcept
        : ptr(ptr)
        , deleter(std::move(deleter))
    {
    }

    OwnedPtr(const OwnedPtr &) = delete;
    OwnedPtr &operator=(const OwnedPtr &) = delete;

    OwnedPtr(OwnedPtr &&other) noexcept
        : OwnedPtr()
    {
        swap(other);
    }

    OwnedPtr &operator=(OwnedPtr &&other) noexcept
    {
        OwnedPtr tmp(std::move(other));
        swap(tmp);
        return *this;
    }

    void swap(OwnedPtr &other) noexcept
    {
        std::ranges::swap(ptr, other.ptr);
        std::ranges::swap(deleter, other.deleter);
    }

    ~OwnedPtr()
    {
        if (deleter != nullptr) {
            deleter(ptr);
        }
    }

    T get() const { return ptr; }
};
