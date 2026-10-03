#ifndef UNQPTR_H
#define UNQPTR_H

#include <stdexcept>  // ошибки при nullptr

#include "deleters.h"

template <class T>
class UnqPtr {
private:
    T* ptr;
    void (*deleter)(T*);
public:
    UnqPtr(): ptr(nullptr), deleter(default_delete<T>) {}

    UnqPtr(T* pointer, void (*deleteFunc)(T*)=default_delete<T>): ptr(pointer), deleter(deleteFunc) {}

    UnqPtr(const UnqPtr<T>& other) = delete;

    UnqPtr(UnqPtr<T>&& other): ptr(other.ptr), deleter(other.deleter) {
        other.ptr = nullptr;
    }

    ~UnqPtr() {deleter(ptr);}

    UnqPtr<T>& operator=(T* pointer) {
        deleter(ptr);
        ptr = pointer;
        deleter = default_delete<T>;  // небезопасный момент
        return *this;  // конструктора по lvalue-ссылке нет
    }

    UnqPtr<T>& operator=(const UnqPtr<T>& other) = delete;

    UnqPtr<T>& operator=(UnqPtr<T>&& other) {
        deleter(ptr);
        ptr = other.ptr;
        deleter = other.deleter;
        other.ptr = nullptr;
        return *this;
    }

    T& operator*() {
        if (deleter == array_delete<T>) throw std::invalid_argument("* not defined for array unique pointers");
        if (ptr == nullptr) throw std::invalid_argument("nullptr unique pointer");
        return *ptr;
    }

    T& operator->() {
        if (deleter == array_delete<T>) throw std::invalid_argument("-> not defined for array unique pointers");
        if (ptr == nullptr) throw std::invalid_argument("nullptr unique pointer");
        return *ptr;
    }

    T& operator[](size_t index) {
        if (deleter == default_delete<T>) throw std::invalid_argument("[] not defined for non-array unique pointers");
        if (ptr == nullptr) throw std::invalid_argument("nullptr unique pointer");
        return ptr[index];
    }
};

template <class T, class... Args>
UnqPtr<T> make_unq(Args&&... args) {
    return UnqPtr<T>(new T(args...), default_delete<T>);
}

template <class T>
UnqPtr<T> make_unq_array(size_t size) {
    return UnqPtr<T>(new T[size], array_delete<T>);
}

#endif // UNQPTR_H
