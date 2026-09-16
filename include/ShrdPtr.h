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

    ShrdPtr(const T* pointer): ptr(pointer), count(new unsigned int(1)) {}

    ShrdPtr(const ShrdPtr<T>& other): ptr(other.ptr), count(other.count) {(*count)++;}

    ShrdPtr(const ShrdPtr<T>&& other) = delete;

    ~ShrdPtr() {
        (*count)--;
        if (count == 0) {
            if (ptr) delete ptr;
            delete count;
        }
    }

    ShrdPtr<T> operator=(const T* pointer) {
        (*count)--;
        if (count == 0) {
            if (ptr) delete ptr;
            delete count;
        }
        ptr = pointer;
        count = new unsigned int(1);
    }

    ShrdPtr<T> operator=(const ShrdPtr<T>& other) {
        (*count)--;
        if (count == 0) {
            if (ptr) delete ptr;
            delete count;
        }
        ptr = other.ptr;
        count = other.count;
        (*count)++;
    }

    ShrdPtr<T> operator=(const ShrdPtr<T>&& other) = delete;

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
