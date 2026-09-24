#ifndef ARRAY_SEQUENCE_H
#define ARRAY_SEQUENCE_H

#include "Sequence.h"
#include "DynamicArray.h"
#include "UnqPtr.h"

#include <functional>  // map-reduce

#define CAPACITY_ADD 10  // Шаг увеличения capacity

template <class T> class ArraySequence: public Sequence<T> {
private:
    UnqPtr<DynamicArray<T>> seq;
    size_t size;  // capacity - это seq->GetSize()
public:
    ArraySequence(const T* items, size_t count): seq(new DynamicArray<T>(items, count)), size(count) {}

    // сейчас добавлять будут
    ArraySequence(): seq(new DynamicArray<T>(CAPACITY_ADD)), size(0) {}

    ArraySequence(const DynamicArray<T>& dynamicArray): seq(new DynamicArray<T>(dynamicArray)) {
        size = dynamicArray.size;
    }

    ArraySequence(DynamicArray<T>&& dynamicArray): seq(make_unq(dynamicArray)), size(dynamicArray.size) {
        dynamicArray = DynamicArray<T>(0);
    }

    ArraySequence(const ArraySequence<T>& other): seq(new DynamicArray<T>(*other.seq)), size(other.GetLength()) {}

    ArraySequence(ArraySequence<T>&& other): seq(other.seq), size(other.GetLength()) {
        other.seq = nullptr;
        other.size = 0;
    }

    ~ArraySequence() override {
        delete seq;
    }

    T GetFirst() const override {
        if (size == 0) throw std::out_of_range("array sequence: index error");
        return Get(0);
    }

    T GetLast() const override {
        if (size == 0) throw std::out_of_range("array sequence: index error");
        return Get(GetLength() - 1);
    }

    T Get(size_t index) const override {
        if (index >= GetLength()) throw std::out_of_range("array sequence: index error");
        return seq->Get(index);
    }

    Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const override {
        if (endIndex > GetLength() || endIndex < startIndex) {  // endIndex = startIndex - пустой срез
            throw std::out_of_range("array sequence: index error");
        }
        ArraySequence<T>* subseq = new ArraySequence<T>();
        subseq->seq->Resize(endIndex - startIndex);
        for (size_t index = startIndex; index < endIndex; index++) {
            subseq->seq->Set(index - startIndex, Get(index));
        }
        subseq->size = endIndex - startIndex;
        return (Sequence<T>*)subseq;
    }

    size_t GetLength() const override {
        return size;
    }

    Sequence<T>* Append(T item) override {
        if (seq->GetSize() == size) {
            seq->Resize(size + CAPACITY_ADD);
        }
        seq->Set(size, item);
        size++;
        return this;
    }

    Sequence<T>* Prepend(T item) override {
        if (seq->GetSize() == size) {
            seq->Resize(size + CAPACITY_ADD);
        }
        for (size_t index = size; index > 0; index--) {
            seq->Set(index, seq->Get(index - 1));
        }
        seq->Set(0, item);
        size++;
        return this;
    }

    Sequence<T>* InsertAt(T item, size_t index) override {
        if (index > size) throw std::out_of_range("array sequence: index error");
        if (seq->GetSize() == size) {
            seq->Resize(size + CAPACITY_ADD);
        }
        for (size_t current = size; current >= index; current--) {
            seq->Set(current, seq->Get(current - 1));
        }
        seq->Set(index, item);
        size++;
        return this;
    }

    Sequence<T>* Concat(Sequence<T>* list) override {
        size_t otherSize = list->GetLength();
        seq->Resize(GetLength() + otherSize);
        for (size_t index = 0; index < otherSize; index++) {
            seq->Set(size + index, list->Get(index));
        }
        size += otherSize;
        return this;
    }

    Sequence<T>* Map(std::function<T(T)> func) const override {
        ArraySequence<T>* mapped = new ArraySequence<T>();
        mapped->seq->Resize(GetLength());
        for (size_t index = 0; index < GetLength(); index++) {
            mapped->seq->Set(index, func(seq->Get(index)));
        }
        mapped->size = GetLength();
        return (Sequence<T>*)mapped;
    }

    Sequence<T>* Where(std::function<bool(T)> func) const override {
        ArraySequence<T>* filtered = new ArraySequence<T>();
        T item;
        for (size_t index = 0; index < GetLength(); index++) {
            item = seq->Get(index);
            if (func(item)) filtered->Append(item);
        }
        return (Sequence<T>*)filtered;
    }

    T Reduce(std::function<T(T, T)> func, T initial) const override {
        T result = initial;
        for (size_t index = 0; index < GetLength(); index++) {
            result = func(seq->Get(index), result);
        }
        return result;
    }

    ArraySequence<T> operator=(const DynamicArray<T>& other) {
        if (seq) delete seq;
        seq = new DynamicArray<T>(other);
        size = other.GetSize();
        return *this;
    }

    ArraySequence<T> operator=(const ArraySequence<T>& other) {
        if (this == &other) return *this;
        if (seq) delete seq;
        seq = new DynamicArray<T>(other.seq);
        size = other.GetLength();
        return *this;
    }

    ArraySequence<T> operator=(ArraySequence<T>&& other) {
        if (this == &other) return *this;
        if (seq) delete seq;
        seq = other.seq;
        size = other.GetLength();
        other.seq = nullptr;
        other.size = 0;
        return this;
    }

    bool operator==(const ArraySequence<T>& other) {
        bool equal = GetLength() == other.GetLength();
        for (size_t index = 0; index < GetLength() && equal; index++) {
            equal = Get(index) == other.Get(index);
        }
        return equal;
    }
};

#endif  // ARRAY_SEQUENCE_H
