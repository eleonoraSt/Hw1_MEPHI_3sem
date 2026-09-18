#ifndef SHRDPTR_H
#define SHRDPTR_H

#include <stdexcept>  // ошибки при nullptr

template <class T>
class ShrdPtr {
private:
    T* ptr;
    unsigned int* count;
public:
    ShrdPtr(): ptr(nullptr), count(new unsigned int(1)) {}

    template <class Derived>
    ShrdPtr(const Derived* pointer): ptr(dynamic_cast<T*>(pointer)), count(new unsigned int(1)) {}

    template <class Derived>
    ShrdPtr(const ShrdPtr<Derived>& other): ptr(dynamic_cast<T*>(other.ptr)), count(new unsigned int(1)) {
        (*count)++;
    }

    template <class Derived>
    ShrdPtr(const ShrdPtr<Derived>&& other) = delete;

    ~ShrdPtr() {
        (*count)--;
        if (count == 0) {
            if (ptr) delete ptr;
            delete count;
        }
    }

    template <class Derived>
    ShrdPtr<T> operator=(const Derived* pointer) {
        (*count)--;
        if (count == 0) {
            if (ptr) delete ptr;
            delete count;
        }
        ptr = dynamic_cast<T*>(pointer);
        count = new unsigned int(1);
    }

    template <class Derived>
    ShrdPtr<T> operator=(const ShrdPtr<Derived>& other) {
        (*count)--;
        if (count == 0) {
            if (ptr) delete ptr;
            delete count;
        }
        ptr = dynamic_cast<T*>(other);
        count = other.count;
        (*count)++;
    }

    template <class Derived>
    ShrdPtr<T> operator=(const ShrdPtr<Derived>&& other) = delete;

    T& operator*() {
        if (ptr == nullptr) throw std::invalid_argument("nullptr shared pointer");
        return ptr;
    }

    T& operator->() {
        if (ptr == nullptr) throw std::invalid_argument("nullptr shared pointer");
        return ptr;
    }
};

#endif // SHRDPTR_H
