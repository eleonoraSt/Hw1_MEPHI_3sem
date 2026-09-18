#ifndef UNQPTR_H
#define UNQPTR_H

#include <stdexcept>  // ошибки при nullptr

template <class T>
class UnqPtr {
private:
    T* ptr;
public:
    UnqPtr(): ptr(nullptr) {}

    template <class Derived>
    UnqPtr(const Derived* pointer): ptr(dynamic_cast<T*>(pointer)) {}

    template <class Derived>
    UnqPtr(const UnqPtr<Derived>& other) = delete;

    template <class Derived>
    UnqPtr(UnqPtr<Derived>&& other): ptr(dynamic_cast<T*>(other.ptr)) {other = nullptr;}

    ~UnqPtr() {if (ptr) delete ptr;}

    template <class Derived>
    UnqPtr<T> operator=(const Derived* pointer) {
        if (ptr) delete ptr;
        ptr = dynamic_cast<T*>(pointer);
    }

    template <class Derived>
    UnqPtr<T> operator=(const UnqPtr<Derived>& other) = delete;

    template <class Derived>
    UnqPtr<T> operator=(UnqPtr<Derived>&& other) {
        if (ptr) delete ptr;
        ptr = dynamic_cast<T*>(other.ptr);
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
