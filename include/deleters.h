#ifndef DELETERS_H
#define DELETERS_H

template <class T>
void default_delete(T* normalPtr) {delete normalPtr;}

template <class T>
void array_delete(T* arrayPtr) {delete[] arrayPtr;}

#endif // DELETERS_H
