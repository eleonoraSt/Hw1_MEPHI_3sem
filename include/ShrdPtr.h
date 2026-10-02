#ifndef SHRDPTR_H
#define SHRDPTR_H

#include <stdexcept>  // ошибки при nullptr

#include "deleters.h"

template <class T>
class ShrdPtr {
private:
    T* ptr;
    unsigned int* count;
    void (*deleter)(T*);
public:
    ShrdPtr(): ptr(nullptr), count(new unsigned int(1)), deleter(default_delete<T>) {}

    ShrdPtr(T* pointer, void (*deleteFunc)(T*)=default_delete<T>): ptr(pointer), deleter(deleteFunc) {
        count = new unsigned int(1);
    }

    ShrdPtr(const ShrdPtr<T>& other): ptr(other.ptr), deleter(other.deleter), count(other.count) {
        (*count)++;
    }

    ShrdPtr(const ShrdPtr<T>&& other) = delete;

    ~ShrdPtr() {
        (*count)--;
        if (count == 0) {
            if (ptr) deleter(ptr);
            delete count;
        }
    }

    ShrdPtr<T> operator=(T* pointer) {
        (*count)--;
        if (count == 0) {
            if (ptr) deleter(ptr);
            delete count;
        }
        ptr = pointer;
        count = new unsigned int(1);
        deleter = default_delete<T>;  // небезопасный момент
    }

    ShrdPtr<T> operator=(const ShrdPtr<T>& other) {
        (*count)--;
        if (count == 0) {
            if (ptr) delete ptr;
            delete count;
        }
        ptr = other.ptr;
        count = other.count;
        deleter = other.deleter;
        (*count)++;
    }

    ShrdPtr<T> operator=(const ShrdPtr<T>&& other) = delete;

    T& operator*() {
        if (deleter == array_delete<T>) throw std::invalid_argument("* not defined for array shared pointers");
        if (ptr == nullptr) throw std::invalid_argument("nullptr shared pointer");
        return *ptr;
    }

    T& operator->() {
        if (deleter == array_delete<T>) throw std::invalid_argument("-> not defined for array shared pointers");
        if (ptr == nullptr) throw std::invalid_argument("nullptr shared pointer");
        return *ptr;
    }

    T& operator[](size_t index) {
        if (deleter == default_delete<T>) throw std::invalid_argument("[] not defined for non-array shared pointers");
        if (ptr == nullptr) throw std::invalid_argument("nullptr shared pointer");
        return ptr[index];
    }
};

template <class T, class... Args>
ShrdPtr<T> make_shrd(Args&&... args) {
    return ShrdPtr(new T(args...), default_delete<T>);
}

template <class T>
ShrdPtr<T> make_shrd_array(size_t size) {
    return ShrdPtr<T>(new T[size], array_delete<T>);
}

#endif // SHRDPTR_H
