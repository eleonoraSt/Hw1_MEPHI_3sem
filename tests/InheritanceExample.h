#ifndef INHERITANCEEXAMPLE_H
#define INHERITANCEEXAMPLE_H

struct Parent {
    int field;
    Parent(int val): field(val) {}
    ~Parent() = default;
};

struct Child: public Parent {
    Child(int val): Parent(val) {}
};

struct Impostor {
    int field;
    Impostor(int val): field(val) {}
    ~Impostor() = default;
};

#endif // INHERITANCEEXAMPLE_H
