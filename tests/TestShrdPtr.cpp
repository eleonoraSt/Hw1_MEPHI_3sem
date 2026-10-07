#include <memory>
#include <ctime>
#include <iostream>
#include <array>
#include <QTest>

#include "..\include\ShrdPtr.h"

template <long Size>
void shrdptr_load() {
    std::cout << "Time & memory test for array of ShrdPtr of size " << Size << "\n";
    clock_t time = clock();
    std::array<ShrdPtr<long>, Size> myVers;
    ShrdPtr<long> myVersBuf = make_shrd<long>(5);
    for (long index = 0; index < Size; index++) myVers[index] = myVersBuf;
    size_t memory = sizeof myVers;
    time = clock() - time;
    std::cout << "Memory usage: " << memory << " bytes\n";
    std::cout << "Time usage: " << ((float)time) / CLOCKS_PER_SEC << " seconds\n";
}

template <long Size>
void shared_ptr_load() {
    std::cout << "Time & memory test for array of std::shared_ptr of size " << Size << "\n";
    clock_t time = clock();
    std::array<std::shared_ptr<long>, Size> stlVers;
    std::shared_ptr<long> stlVersBuf = std::make_shared<long>(5);
    for (long index = 0; index < Size; index++) stlVers[index] = stlVersBuf;
    size_t memory = sizeof stlVers;
    time = clock() - time;
    std::cout << "Memory usage: " << memory << " bytes\n";
    std::cout << "Time usage: " << ((float)time) / CLOCKS_PER_SEC << " seconds\n";
}

class TestShrdPtr: public QObject {
    Q_OBJECT
private slots:
    void null() {
        ShrdPtr<int> pointer;
        QVERIFY_THROWS_EXCEPTION(std::invalid_argument, *pointer);
        QVERIFY_THROWS_EXCEPTION(std::invalid_argument, pointer[0]);
    }

    void make_shared() {
        ShrdPtr<int> pointer = make_shrd<int>(2);
        QCOMPARE(*pointer, 2);
        QVERIFY_THROWS_EXCEPTION(std::invalid_argument, pointer[0]);
        *pointer = 5;
        QCOMPARE(*pointer, 5);
    }

    void make_shared_array() {
        ShrdPtr<int> pointer(make_shrd_array<int>(5));
        QVERIFY_THROWS_EXCEPTION(std::invalid_argument, *pointer);
        try {
            pointer[0] = 10;
        } catch (std::exception exc) {
            QFAIL(exc.what());
        }
        QCOMPARE(pointer[0], 10);
    }

    void ptr_constructor() {
        ShrdPtr<int> ptr(new int(2));
        QCOMPARE(*ptr, 2);
    }

    void array_constructor() {
        int* constructFrom = new int[5];
        constructFrom[2] = 10;
        ShrdPtr<int> ptr(constructFrom, array_delete<int>);
        QCOMPARE(ptr[2], 10);
    }

    void ptr_assignment() {
        ShrdPtr<int> ptr;
        ptr = new int(2);
        QCOMPARE(*ptr, 2);
    }

    void copy_constructor() {
        ShrdPtr<int>* first_ptr = new ShrdPtr<int>(make_shrd<int>(2));
        ShrdPtr<int> second(*first_ptr);
        QCOMPARE(*second, 2);
        *(*first_ptr) = 3;
        delete first_ptr;
        QCOMPARE(*second, 3);
    }

    void copy_assignment() {
        ShrdPtr<int>* first_ptr = new ShrdPtr<int>(make_shrd<int>(2));
        ShrdPtr<int> second;
        second = *first_ptr;
        QCOMPARE(*second, 2);
        *(*first_ptr) = 3;
        delete first_ptr;
        QCOMPARE(*second, 3);
    }

    void load_comparison_10() {
        shrdptr_load<10>();
        shared_ptr_load<10>();
    }

    void load_comparison_100() {
        shrdptr_load<100>();
        shared_ptr_load<100>();
    }

    void load_comparison_1e3() {
        shrdptr_load<1000>();
        shared_ptr_load<1000>();
    }

    void load_comparison_1e4() {
        shrdptr_load<10000>();
        shared_ptr_load<10000>();
    }

    void load_comparison_1e5() {
        shrdptr_load<100000>();
        shared_ptr_load<100000>();
    }

    void load_comparison_1e6() {
        shrdptr_load<1000000>();
        shared_ptr_load<1000000>();
    }
};

QTEST_MAIN(TestShrdPtr)
#include "TestShrdPtr.moc"
