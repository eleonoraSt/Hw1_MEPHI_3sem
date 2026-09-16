#ifndef SHRDPTR_H
#define SHRDPTR_H

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
            delete ptr;
            delete count;
        }
    }

    ShrdPtr<T> operator=(const T* pointer) {
        (*count)--;
        if (count == 0) {
            delete ptr;
            delete count;
        }
        ptr = pointer;
        count = new unsigned int(1);
    }

    ShrdPtr<T> operator=(const ShrdPtr<T>& other) {
        (*count)--;
        if (count == 0) {
            delete ptr;
            delete count;
        }
        ptr = other.ptr;
        count = other.count;
        (*count)++;
    }

    ShrdPtr<T> operator=(const ShrdPtr<T>&& other) = delete;

    T& operator*() {return ptr;}

    T& operator->() {return ptr;}
};

#endif // SHRDPTR_H
