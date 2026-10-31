#ifndef PAIR_H
#define PAIR_H

template <class A, class B>
class Pair {
private:
    A itemA;
    B itemB;
public:
    Pair(A first, B second): itemA(first), itemB(second) {}

    A GetA() {return itemA;}

    B GetB() {return itemB;}
};

#endif // PAIR_H
