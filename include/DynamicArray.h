#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <cstdlib>  // size_t
#include <stdexcept>

template <class T>
class DynamicArray {
private:
    T* data;
    size_t size;
public:
    DynamicArray(const T* items, size_t count) {
        size = count;
        if (count == 0) {
            data = nullptr;
            return;
        }
        data = new T[count];
        for (size_t index = 0; index < count; index++) {
            data[index] = items[index];
        }
    }

    DynamicArray(size_t count) {
        size = count;
        if (count == 0) {
            data = nullptr;
            return;
        }
        data = new T[count];
    }

    DynamicArray(const DynamicArray<T>& dynamicArray) {
        size = dynamicArray.GetSize();
        data = new T[size];
        for (size_t index = 0; index < size; index++) {
            data[index] = dynamicArray.Get(index);
        }
    }

    ~DynamicArray() {
        if (data) delete[] data;
    }

    T Get(size_t index) const {
        if (index >= size) throw std::out_of_range("dynamic array: index error");
        return data[index];
    }

    size_t GetSize() const {
        return size;
    }

    void Set(size_t index, T value) {
        if (index >= size) throw std::out_of_range("dynamic array: index error");
        data[index] = value;
    }

    void Resize(size_t newSize) {
        T* newData = newSize == 0 ? nullptr : new T[newSize];
        size_t copyNum = size <= newSize ? size : newSize;
        for (size_t index = 0; index < copyNum; index++) {
            newData[index] = data[index];
        }
        delete[] data;
        data = newData;
        size = newSize;
    }

    DynamicArray<T> operator=(const DynamicArray<T> &other) {
        if (data && this != &other) delete[] data;
        Resize(other.size);
        for (size_t index = 0; index < size; index++) {
            data[size] = other.data[size];
        }
        return *this;
    }
};

#endif  // DYNAMIC_ARRAY_H
