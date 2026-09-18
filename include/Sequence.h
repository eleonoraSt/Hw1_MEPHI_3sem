#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <cstdlib>  // size_t

template <class T> class Sequence {
public:
    /*
     * Ссылка возвращается, когда редактируется данный экземпляр
     * Указатель возвращается, когда создаётся новый экземпляр
     */

    virtual ~Sequence() {}

    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual T Get(size_t index) const = 0;
    virtual Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const = 0;
    virtual size_t GetLength() const = 0;

    virtual Sequence<T>& Append(T item) = 0;
    virtual Sequence<T>& Prepend(T item) = 0;
    virtual Sequence<T>& InsertAt(T item, size_t index) = 0;
    virtual Sequence<T>& Concat(Sequence<T>& list) = 0;

    virtual Sequence<T>* Map(T (*func)(T)) const = 0;
    virtual Sequence<T>* Where(bool (*func)(T)) const = 0;
    virtual T Reduce(T (*func)(T, T), T initial) const = 0;

    virtual Sequence<T> operator=(const Sequence<T>& other) = 0;
};

#endif  // SEQUENCE_H
