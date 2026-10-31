#ifndef UNQPTR_H
#define UNQPTR_H

#include <stdexcept>  // ошибки при nullptr
#include "Pair.h"

template <class T>
class UnqPtr {
private:
    T* ptr;
    bool isArray;

    void clear() {
        if (isArray) {delete[] ptr;} else {delete ptr;}
    }
public:
    UnqPtr(): ptr(nullptr), isArray(false) {}

    UnqPtr(T* pointer, bool isItArray=false): ptr(pointer), isArray(isItArray) {}

    template <class Derived>
    UnqPtr(Derived* pointer, bool isItArray=false): isArray(isItArray) {
        static_assert(std::is_base_of<T, Derived>::value, "Argument must be of the target class or derived from it");
        ptr = dynamic_cast<T*>(pointer);
    }

    UnqPtr(const UnqPtr<T>& other) = delete;

    UnqPtr(UnqPtr<T>&& other): ptr(other.ptr), isArray(other.isArray) {
        other.ptr = nullptr;
        other.isArray = false;
    }

    template <class Derived>
    UnqPtr(UnqPtr<Derived>&& other) {
        static_assert(std::is_base_of<T, Derived>::value, "Argument must be of the target class or derived from it");
        Pair<Derived*, bool> derivedInfo = other.Get();
        ptr = dynamic_cast<T*>(derivedInfo.GetA());
        isArray = derivedInfo.GetB();
        other.ForceReset();
    }

    ~UnqPtr() {clear();}

    UnqPtr<T>& operator=(T* pointer) {
        clear();
        ptr = pointer;
        isArray = false;  // небезопасный момент
        return *this;  // конструктора по lvalue-ссылке нет
    }

    template <class Derived>
    UnqPtr<T>& operator=(Derived* pointer) {
        static_assert(std::is_base_of<T, Derived>::value, "Argument must be of the target class or derived from it");
        clear();
        ptr = dynamic_cast<T*>(pointer);
        isArray = false;  // небезопасный момент
        return *this;  // конструктора по lvalue-ссылке нет
    }

    UnqPtr<T>& operator=(const UnqPtr<T>& other) = delete;

    UnqPtr<T>& operator=(UnqPtr<T>&& other) {
        if (&other == this) return *this;
        clear();
        ptr = other.ptr;
        isArray = other.isArray;
        other.ptr = nullptr;
        other.isArray = false;
        return *this;
    }

    template <class Derived>
    UnqPtr<T>& operator=(UnqPtr<Derived>&& other) {
        static_assert(std::is_base_of<T, Derived>::value, "Argument must be of the target class or derived from it");
        clear();
        Pair<Derived*, bool> derivedInfo = other.Get();
        ptr = dynamic_cast<T*>(derivedInfo.GetA());
        isArray = derivedInfo.GetB();
        other.ForceReset();
        return *this;
    }

    T& operator*() {
        if (isArray) throw std::invalid_argument("-> not defined for array unique pointers");
        if (ptr == nullptr) throw std::invalid_argument("nullptr unique pointer");
        return *ptr;
    }

    T& operator->() {
        if (isArray) throw std::invalid_argument("-> not defined for array unique pointers");
        if (ptr == nullptr) throw std::invalid_argument("nullptr unique pointer");
        return *ptr;
    }

    T& operator[](size_t index) {
        if (!isArray) throw std::invalid_argument("[] not defined for non-array unique pointers");
        if (ptr == nullptr) throw std::invalid_argument("nullptr unique pointer");
        return ptr[index];
    }

    Pair<T*, bool> Get() {return Pair<T*, bool>(ptr, isArray);}

    void ForceReset() {
        ptr = nullptr;
        isArray = false;
    }
};

template <class T, class... Args>
UnqPtr<T> make_unq(Args&&... args) {
    return UnqPtr<T>(new T(args...), false);
}

template <class T>
UnqPtr<T> make_unq_array(size_t size) {
    return UnqPtr<T>(new T[size], true);
}

#endif // UNQPTR_H
