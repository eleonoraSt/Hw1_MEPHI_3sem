#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <cstdlib>  // size_t
#include <stdexcept>

template <class T> class LinkedList {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
    };

    Node* head;
    Node* tail;
public:
    LinkedList(const T* items, size_t count): head(nullptr), tail(nullptr) {
        for (size_t index = 0; index < count; index++) {
            Append(items[index]);
        }
    }

    LinkedList(): head(nullptr), tail(nullptr) {}

    LinkedList(const LinkedList<T>& list): head(nullptr), tail(nullptr) {
        Node* copyFrom = list.head;
        while (copyFrom != nullptr) {
            Append(copyFrom->data);
            copyFrom = copyFrom->next;
        }
    }

    LinkedList(LinkedList<T>&& list): head(nullptr), tail(nullptr) {
        Node* copyFrom = list.head;
        list.head = nullptr;
        Node* next = nullptr;
        while (copyFrom != nullptr) {
            Append(copyFrom->data);
            copyFrom = copyFrom->next;
            delete copyFrom->prev;
        }
    }

    ~LinkedList() {
        Node* prev;
        while (tail != nullptr) {
            prev = tail->prev;
            delete tail;
            tail = prev;
        }
    }

    T GetFirst() const {
        if (head == nullptr) throw std::out_of_range("linked list: index error");  // Пустой список
        return head->data;
    }

    T GetLast() const {
        if (tail == nullptr) throw std::out_of_range("linked list: index error");  // Пустой список
        return tail->data;
    }

    T Get(size_t index) const {
        if (head == nullptr) throw std::out_of_range("linked list: index error");  // Пустой список
        Node* item = head;
        for (size_t current = 0; current < index; current++) {
            item = item->next;
            if (item == nullptr) throw std::out_of_range("linked list: index error");  // Индекс больше размера массива
        }
        return item->data;
    }

    LinkedList<T>* GetSubList(size_t startIndex, size_t endIndex) const {
        if (startIndex > endIndex) throw std::out_of_range("linked list: index error");
        LinkedList<T>* sublist = new LinkedList<T>();
        Node* item = head;
        size_t index;
        for (index = 0; index < startIndex; index++) {
            if (item == nullptr) {  // Начальный индекс больше размера массива
                delete sublist;
                std::out_of_range("linked list: index error");
            }
            item = item->next;
        }
        for (index = startIndex; index <= endIndex; index++) {
            if (item == nullptr) {  // Конечный индекс больше размера массива
                delete sublist;
                throw std::out_of_range("linked list: index error");
            }
            sublist->Append(item->data);
            item = item->next;
        }
        return sublist;
    }

    size_t GetLength() const {
        Node* item = head;
        size_t count = 0;
        while (item != nullptr) {
            count++;
            item = item->next;
        }
        return count;
    }

    void Append(T item) {
        Node* node = new Node({item, nullptr, nullptr});
        node->prev = tail;
        if (head == nullptr) {  // Пустой список
            head = node;
        } else {
            tail->next = node;
        }
        tail = node;
    }

    void Prepend(T item) {
        Node* node = new Node({item, nullptr, nullptr});
        node->next = head;
        if (tail == nullptr) {  // Пустой список
            tail = node;
        } else {
            head->prev = node;
        }
        head = node;
    }

    void InsertAt(T item, size_t index) {
        if (index == 0) {
            Prepend(item);
            return;
        }
        Node* prev = head;
        if (prev == nullptr) throw std::out_of_range("linked list: index error");
        for (size_t current = 0; current < index - 1; current++) {
            if (prev == nullptr) throw std::out_of_range("linked list: index error");  // передан индекс больше размера списка
            prev = prev->next;
        }
        Node* node = new Node({item, nullptr, nullptr});
        node->next = prev->next;
        node->prev = prev;
        if (prev->next != nullptr) prev->next->prev = node;
        prev->next = node;
    }

    LinkedList<T>* Concat(LinkedList<T>* list) {
        LinkedList<T>* concatenated = new LinkedList<T>(this);
        Node* otherNode = list->head;
        while (otherNode != nullptr) {
            concatenated->Append(otherNode->data);
        }
        return concatenated;
    }

    LinkedList<T> operator=(const LinkedList<T>& other) {
        if (this != &other) {
            Node* next;
            while (head) {
                next = head->next;
                delete head;
                head = next;
            }
            for (Node* otherHead = other.head; otherHead != nullptr; otherHead = otherHead->next) {
                Append(otherHead->data);
            }
        }
    }

    LinkedList<T> operator=(LinkedList<T>&& other) {
        if (this != &other) {
            Node* next;
            while (head) {
                next = head->next;
                delete head;
                head = next;
            }
            while (other.head) {
                Append(other.head->data);
                next = other.head->next;
                delete other.head;
                other.head = next;
            }
        }
    }
};

#endif  // LINKED_LIST_H
