#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <cstdlib>  // size_t
#include <functional>  // map-reduce

template <class T>
class Sequence {
public:
    virtual ~Sequence() {}

    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual T Get(size_t index) const = 0;
    virtual Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const = 0;  // не включая endIndex
    virtual size_t GetLength() const = 0;

    virtual Sequence<T>* Append(T item) = 0;
    virtual Sequence<T>* Prepend(T item) = 0;
    virtual Sequence<T>* InsertAt(T item, size_t index) = 0;
    virtual Sequence<T>* Concat(Sequence<T>& list) = 0;

    virtual Sequence<T>* Map(std::function<T(T)> func) const = 0;
    virtual Sequence<T>* Where(std::function<bool(T)> func) const = 0;
    virtual T Reduce(std::function<T(T, T)> func, T initial) const = 0;
};

#endif  // SEQUENCE_H
