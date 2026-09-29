#ifndef SHRDPTR_H
#define SHRDPTR_H

#include <stdexcept>  // ошибки при nullptr
#include <functional>  // deleters

#include "deleters.h"

template <class T>
class ShrdPtr {
private:
    T* ptr;
    unsigned int* count;
    std::function<void(T*)> deleter;
public:
    ShrdPtr(): ptr(nullptr), count(new unsigned int(1)), deleter(default_delete<T>) {}

    template <class Derived>
    ShrdPtr(Derived* pointer, std::function<void(T*)> deleteFunc=default_delete<T>) {
        ptr = dynamic_cast<T*>(pointer);
        if (ptr == nullptr) throw std::invalid_argument("shared pointer conversion failed");
        count = new unsigned int(1);
        deleter = deleteFunc;
    }

    template <class Derived>
    ShrdPtr(const ShrdPtr<Derived>& other): ptr(dynamic_cast<T*>(other.ptr)), count(new unsigned int(1)) {
        if (ptr == nullptr) throw std::invalid_argument("shared pointer conversion failed");
        deleter = other.deleter;
        (*count)++;
    }

    template <class Derived>
    ShrdPtr(const ShrdPtr<Derived>&& other) = delete;

    ~ShrdPtr() {
        (*count)--;
        if (count == 0) {
            if (ptr) deleter(ptr);
            delete count;
        }
    }

    template <class Derived>
    ShrdPtr<T> operator=(Derived* pointer) {
        (*count)--;
        if (count == 0) {
            if (ptr) deleter(ptr);
            delete count;
        }
        ptr = dynamic_cast<T*>(pointer);
        if (ptr == nullptr) throw std::invalid_argument("shared pointer conversion failed");
        count = new unsigned int(1);
        deleter = default_delete<T>;  // небезопасный момент
    }

    template <class Derived>
    ShrdPtr<T> operator=(const ShrdPtr<Derived>& other) {
        (*count)--;
        if (count == 0) {
            if (ptr) delete ptr;
            delete count;
        }
        ptr = dynamic_cast<T*>(other);
        if (ptr == nullptr) throw std::invalid_argument("shared pointer conversion failed");
        count = other.count;
        deleter = other.deleter;
        (*count)++;
    }

    template <class Derived>
    ShrdPtr<T> operator=(const ShrdPtr<Derived>&& other) = delete;

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
