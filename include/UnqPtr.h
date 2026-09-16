#ifndef UNQPTR_H
#define UNQPTR_H

#include <stdexcept>  // ошибки при nullptr

template <class T>
class UnqPtr {
private:
    T* ptr;
public:
    UnqPtr(): ptr(nullptr) {}

    UnqPtr(const T* pointer): ptr(pointer) {}

    UnqPtr(const UnqPtr<T>& other) = delete;

    UnqPtr(UnqPtr<T>&& other): ptr(other.ptr) {other = nullptr;}

    ~UnqPtr() {if (ptr) delete ptr;}

    UnqPtr<T> operator=(const T* pointer) {
        if (ptr) delete ptr;
        ptr = pointer;
    }

    UnqPtr<T> operator=(const UnqPtr<T>& other) = delete;

    UnqPtr<T> operator=(UnqPtr<T>&& other) {
        if (ptr) delete ptr;
        ptr = other.ptr;
        other.ptr = nullptr;
    }

    T& operator*() {
        if (ptr == nullptr) throw std::invalid_argument("nullptr unique pointer");
        return *ptr;
    }

    T& operator->() {
        if (ptr == nullptr) throw std::invalid_argument("nullptr unique pointer");
        return *ptr;
    }
};

#endif // UNQPTR_H
