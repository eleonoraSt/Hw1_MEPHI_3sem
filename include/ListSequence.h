#ifndef LIST_SEQUENCE_H
#define LIST_SEQUENCE_H

#include "Sequence.h"
#include "LinkedList.h"
#include "UnqPtr.h"

template <class T> class ListSequence: public Sequence<T> {
private:
    UnqPtr<LinkedList<T>> seq;
    size_t length;
public:
    ListSequence(const T* items, size_t count): seq(new LinkedList<T>(items, count)), length(count) {}

    ListSequence(): seq(new LinkedList<T>()), length(0) {}

    ListSequence(const LinkedList<T>& linkedList): seq(new LinkedList<T>(linkedList)) {
        length = linkedList.GetLength();
    }

    ListSequence(LinkedList<T>&& linkedList): seq(make_unq(linkedList)), length(linkedList.GetLength()) {
        linkedList = LinkedList<T>();
    }

    ListSequence(const ListSequence<T>& other): seq(new LinkedList<T>(*other.seq)), length(other.length) {}

    ListSequence(ListSequence<T>&& other): seq(std::move(other.seq)), length(other.length) {
        other.length = 0;
    }

    ~ListSequence() override {
        delete seq;
    }

    T GetFirst() const override {
        if (length == 0) throw std::out_of_range("list sequence: index error");
        return seq->GetFirst();
    }

    T GetLast() const override {
        if (length == 0) throw std::out_of_range("list sequence: index error");
        return seq->GetLast();
    }

    T Get(size_t index) const override {
        if (index >= length) throw std::out_of_range("list sequence: index error");
        return seq->Get(index);
    }

    Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const override {
        if (endIndex >= length || endIndex < startIndex) {
            throw std::out_of_range("list sequence: index error");
        }
        ListSequence<T>* subseq = new ListSequence<T>();
        subseq->seq = seq->GetSubList(startIndex, endIndex);
        subseq->length = endIndex - startIndex;
        return (Sequence<T>*)subseq;
    }

    size_t GetLength() const override {
        return length;
    }

    Sequence<T>* Append(T item) override {
        seq->Append(item);
        length++;
        return this;
    }

    Sequence<T>* Prepend(T item) override {
        seq->Prepend(item);
        length++;
        return this;
    }

    Sequence<T>* InsertAt(T item, size_t index) override {
        if (index > length) throw std::out_of_range("list sequence: index error");
        seq->InsertAt(item, index);
        length++;
        return this;
    }

    Sequence<T>* Concat(Sequence<T>* list) override {
        size_t otherLength = list->GetLength();
        for (size_t index = 0; index < otherLength; index++) {
            seq->Append(list->Get(index));
        }
        length += otherLength;
        return this;
    }

    Sequence<T>* Map(std::function<T(T)> func) const override {
        ListSequence<T>* mapped = new ListSequence<T>();
        for (size_t index = 0; index < length; index++) {
            mapped->Append(func(seq->Get(index)));
        }
        return (Sequence<T>*)mapped;
    }

    Sequence<T>* Where(std::function<bool(T)> func) const override {
        ListSequence<T>* filtered = new ListSequence<T>();
        T item;
        for (size_t index = 0; index < length; index++) {
            item = seq->Get(index);
            if (func(item)) filtered->Append(item);
        }
        return (Sequence<T>*)filtered;
    }

    T Reduce(std::function<T(T, T)> func, T initial) const override {
        T result = initial;
        for (size_t index = 0; index < length; index++) {
            result = func(seq->Get(index), result);
        }
        return result;
    }

    ListSequence<T> operator=(const LinkedList<T>& other) {
        if (seq) delete seq;
        seq = new LinkedList<T>(other);
        length = other.GetLength();
        return *this;
    }

    ListSequence<T> operator=(LinkedList<T>&& other) {
        if (seq) delete seq;
        seq = make_unq(other);
        length = other.GetLength();
        other = LinkedList<T>();
        return *this;
    }

    ListSequence<T> operator=(const ListSequence<T>& other) {
        if (this == &other) return *this;
        if (seq) delete seq;
        seq = new LinkedList<T>(other.seq);
        length = other.length;
        return *this;
    }

    ListSequence<T> operator=(ListSequence<T>&& other) {
        if (this == &other) return *this;
        if (seq) delete seq;
        seq = std::move(other.seq);
        length = other.length;
        other.length = 0;
        return *this;
    }

    bool operator==(const Sequence<T>& other) {
        bool equal = GetLength() == other.GetLength();
        for (size_t index = 0; index < GetLength() && equal; index++) {
            equal = Get(index) == other.Get(index);
        }
        return equal;
    }
};

#endif // LIST_SEQUENCE_H
