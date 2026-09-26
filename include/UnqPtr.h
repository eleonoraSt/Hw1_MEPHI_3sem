#ifndef UNQPTR_H
#define UNQPTR_H

#include <stdexcept>  // ошибки при nullptr
#include <functional>  // deleters

#include "deleters.h"

template <class T>
class UnqPtr {
private:
    T* ptr;
    std::function<void(T*)> deleter;
public:
    UnqPtr(): ptr(nullptr), deleter(default_delete) {}

    template <class Derived>
    UnqPtr(const Derived* pointer, std::function<void(T*)> deleteFunc=default_delete){
        ptr = dynamic_cast<T*>(pointer);
        deleter = deleteFunc;
    }

    template <class Derived>
    UnqPtr(const UnqPtr<Derived>& other) = delete;

    template <class Derived>
    UnqPtr(UnqPtr<Derived>&& other): ptr(dynamic_cast<T*>(other.ptr)), deleter(other.deleter) {
        other = nullptr;
    }

    ~UnqPtr() {if (ptr) deleter(ptr);}

    template <class Derived>
    UnqPtr<T> operator=(const Derived* pointer) {
        if (ptr) deleter(ptr);
        ptr = dynamic_cast<T*>(pointer);
        deleter = default_delete;  // небезопасный момент
    }

    template <class Derived>
    UnqPtr<T> operator=(const UnqPtr<Derived>& other) = delete;

    template <class Derived>
    UnqPtr<T> operator=(UnqPtr<Derived>&& other) {
        if (ptr) deleter(ptr);
        ptr = dynamic_cast<T*>(other.ptr);
        deleter = other.deleter;
        other.ptr = nullptr;
    }

    T& operator*() {
        if (deleter == array_delete) throw std::invalid_argument("* not defined for array unique pointers");
        if (ptr == nullptr) throw std::invalid_argument("nullptr unique pointer");
        return *ptr;
    }

    T& operator->() {
        if (deleter == array_delete) throw std::invalid_argument("-> not defined for array unique pointers");
        if (ptr == nullptr) throw std::invalid_argument("nullptr unique pointer");
        return *ptr;
    }

    T& operator[](size_t index) {
        if (deleter == default_delete) throw std::invalid_argument("[] not defined for non-array unique pointers");
        if (ptr == nullptr) throw std::invalid_argument("nullptr unique pointer");
        return ptr[index];
    }
};

template <class T, class... Args>
UnqPtr<T> make_unq(Args&... args) {
    return UnqPtr<T>(new T(args...), default_delete);
}

template <class T>
UnqPtr<T> make_unq_array(size_t size) {
    return UnqPtr<T>(new T[size], array_delete);
}

#endif // UNQPTR_H
