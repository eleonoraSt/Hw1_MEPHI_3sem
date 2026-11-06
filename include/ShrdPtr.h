#ifndef SHRDPTR_H
#define SHRDPTR_H

#include <stdexcept>  // ошибки при nullptr
//#include "Triplet.h"

template <class T>
class ShrdPtr {
private:
    T* ptr;
    unsigned int* count;
    bool isArray;

    template <class U>
    friend class ShrdPtr;

    void detract() {
        (*count)--;
        if (*count == 0) {
            delete count;
            if (isArray) {delete[] ptr;} else {delete ptr;}
        }
    }
public:
    ShrdPtr(): ptr(nullptr), count(new unsigned int(1)), isArray(false) {}

    ShrdPtr(T* pointer, bool isItArray=false): ptr(pointer), count(new unsigned int(1)), isArray(isItArray) {}

    template <class Derived>
    ShrdPtr(Derived* pointer, bool isItArray=false): isArray(isItArray) {
        static_assert(std::is_base_of<T, Derived>::value, "Argument must be of the target class or derived from it");
        ptr = static_cast<T*>(pointer);
        count = new unsigned int(1);
    }

    ShrdPtr(const ShrdPtr<T>& other): ptr(other.ptr), count(other.count), isArray(other.isArray) {
        (*count)++;
    }

    template <class Derived>
    ShrdPtr(const ShrdPtr<Derived>& other) {
        static_assert(std::is_base_of<T, Derived>::value, "Argument must be of the target class or derived from it");
        ptr = static_cast<T*>(other.ptr);
        isArray = other.isArray;
        count = other.count;
        (*count)++;
    }

    ShrdPtr(const ShrdPtr<T>&& other) = delete;

    ~ShrdPtr() {
        detract();
    }

    ShrdPtr<T>& operator=(T* pointer) {
        detract();
        ptr = pointer;
        count = new unsigned int(1);
        isArray = false;
        return *this;
    }

    template <class Derived>
    ShrdPtr<T>& operator=(Derived* pointer) {
        static_assert(std::is_base_of<T, Derived>::value, "Argument must be of the target class or derived from it");
        detract();
        ptr = static_cast<T*>(pointer);
        count = new unsigned int(1);
        isArray = false;
        return *this;
    }

    ShrdPtr<T>& operator=(const ShrdPtr<T>& other) {
        detract();
        ptr = other.ptr;
        count = other.count;
        isArray = other.isArray;
        (*count)++;
        return *this;
    }

    template <class Derived>
    ShrdPtr<T>& operator=(const ShrdPtr<Derived>& other) {
        static_assert(std::is_base_of<T, Derived>::value, "Argument must be of the target class or derived from it");
        detract();
        ptr = static_cast<T*>(other.ptr);
        isArray = other.isArray;
        count = other.count;
        (*count)++;
        return *this;
    }

    ShrdPtr<T>& operator=(const ShrdPtr<T>&& other) = delete;

    T& operator*() const {
        if (isArray) throw std::invalid_argument("* not defined for array shared pointers");
        if (ptr == nullptr) throw std::invalid_argument("nullptr shared pointer");
        return *ptr;
    }

    T* operator->() const {
        if (isArray) throw std::invalid_argument("-> not defined for array shared pointers");
        if (ptr == nullptr) throw std::invalid_argument("nullptr shared pointer");
        return ptr;
    }

    T& operator[](size_t index) const {
        if (!isArray) throw std::invalid_argument("[] not defined for non-array shared pointers");
        if (ptr == nullptr) throw std::invalid_argument("nullptr shared pointer");
        return ptr[index];
    }
};

template <class T, class... Args>
ShrdPtr<T> make_shrd(Args&&... args) {
    return ShrdPtr<T>(new T(args...), false);
}

template <class T>
ShrdPtr<T> make_shrd_array(size_t size) {
    return ShrdPtr<T>(new T[size], true);
}

#endif // SHRDPTR_H
