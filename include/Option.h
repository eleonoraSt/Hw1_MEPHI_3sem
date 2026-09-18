#ifndef OPTION_H
#define OPTION_H

template <class T>
class Option {
private:
    T* obj;
public:
    Option(): obj(nullptr) {}

    Option(T object): obj(new T(object)) {}

    Option(Option<T>& option) {
        obj = option.IsValid() ? option.Get() : nullptr;
    }

    Option(Option<T>&& option) {
        obj = option.IsValid() ? option.Get() : nullptr;
        option = nullptr;
    }

    bool IsValid() {return obj != nullptr;}

    T Get() {
        return IsValid() ? &obj : T();
    }

    Option<T> operator=(T object) {
        if (obj) delete obj;
        obj = new T(object);
    }

    Option<T> operator=(Option<T>& option) {
        if (obj) delete obj;
        obj = option.IsValid() ? option.Get() : nullptr;
    }

    Option<T> operator=(Option<T>&& option) {
        if (obj) delete obj;
        obj = option.IsValid() ? option.Get() : nullptr;
        option = nullptr;
    }
};

#endif // OPTION_H
