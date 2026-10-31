#ifndef TRIPLET_H
#define TRIPLET_H

template <class A, class B, class C>
class Triplet {
private:
    A itemA;
    B itemB;
    C itemC;
public:
    Triplet(A first, B second, C third): itemA(first), itemB(second), itemC(third) {}

    A GetA() {return itemA;}

    B GetB() {return itemB;}

    C GetC() {return itemC;}
};

#endif // TRIPLET_H
