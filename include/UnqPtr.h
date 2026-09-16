#ifndef UNQPTR_H
#define UNQPTR_H

template <class T>
class UnqPtr {
private:
    T* ptr;
public:
    UnqPtr(): ptr(nullptr) {}

    UnqPtr(const T* pointer): ptr(pointer) {}

    UnqPtr(const UnqPtr<T>& other) = delete;

    UnqPtr(UnqPtr<T>&& other): ptr(other.ptr) {other = nullptr;}

    ~UnqPtr() {delete ptr;}

    UnqPtr<T> operator=(const T* pointer) {ptr = pointer;}

    UnqPtr<T> operator=(const UnqPtr<T>& other) = delete;

    UnqPtr<T> operator=(UnqPtr<T>&& other) {
        delete ptr;
        ptr = other.ptr;
        other.ptr = nullptr;
    }

    T& operator*() {return *ptr;}

    T& operator->() {return *ptr;}
};

#endif // UNQPTR_H
