#include <memory>
#include <ctime>
#include <iostream>
#include <array>
#include <QTest>

#include "..\include\UnqPtr.h"

template <long Size>
void unqptr_load() {
    std::cout << "Time & memory test for array of UnqPtr of size " << Size << "\n";
    clock_t time = clock();
    std::array<UnqPtr<long>, Size> myVers;
    for (long index = 0; index < Size; index++) myVers[index] = make_unq<long>(index);
    size_t memory = sizeof myVers;
    time = clock() - time;
    std::cout << "Memory usage: " << memory << " bytes\n";
    std::cout << "Time usage: " << ((float)time) / CLOCKS_PER_SEC << " seconds\n";
}

template <long Size>
void unique_ptr_load() {
    std::cout << "Time & memory test for array of std::unique_ptr of size " << Size << "\n";
    clock_t time = clock();
    std::array<std::unique_ptr<long>, Size> stlVers;
    for (long index = 0; index < Size; index++) stlVers[index] = std::make_unique<long>(index);
    size_t memory = sizeof stlVers;
    time = clock() - time;
    std::cout << "Memory usage: " << memory << " bytes\n";
    std::cout << "Time usage: " << ((float)time) / CLOCKS_PER_SEC << " seconds\n";
}

class TestUnqPtr: public QObject {
    Q_OBJECT
private slots:
    void null() {
        UnqPtr<int> pointer;
        QVERIFY_THROWS_EXCEPTION(std::invalid_argument, *pointer);
        QVERIFY_THROWS_EXCEPTION(std::invalid_argument, pointer[0]);
    }

    void make_unique() {
        UnqPtr<int> pointer = make_unq<int>(2);
        QCOMPARE(*pointer, 2);
        QVERIFY_THROWS_EXCEPTION(std::invalid_argument, pointer[0]);
        *pointer = 5;
        QCOMPARE(*pointer, 5);
    }

    void make_unique_array() {
        UnqPtr<int> pointer(make_unq_array<int>(5));
        QVERIFY_THROWS_EXCEPTION(std::invalid_argument, *pointer);
        try {
            pointer[0] = 10;
        } catch (std::exception exc) {
            QFAIL(exc.what());
        }
        QCOMPARE(pointer[0], 10);
    }

    void ptr_constructor() {
        UnqPtr<int> ptr(new int(2));
        QCOMPARE(*ptr, 2);
    }

    void array_constructor() {
        int* constructFrom = new int[5];
        constructFrom[2] = 10;
        UnqPtr<int> ptr(constructFrom, array_delete<int>);
        QCOMPARE(ptr[2], 10);
    }

    void ptr_assignment() {
        UnqPtr<int> ptr;
        ptr = new int(2);
        QCOMPARE(*ptr, 2);
    }

    void move_constructor() {
        UnqPtr<int> moveFrom = make_unq<int>(2);
        UnqPtr<int> moveTo(std::move(moveFrom));
        QCOMPARE(*moveTo, 2);
        QVERIFY_THROWS_EXCEPTION(std::invalid_argument, *moveFrom);
    }

    void move_assignment() {
        UnqPtr<int> moveFrom = make_unq<int>(2);
        UnqPtr<int> moveTo;
        moveTo = std::move(moveFrom);
        QCOMPARE(*moveTo, 2);
        QVERIFY_THROWS_EXCEPTION(std::invalid_argument, *moveFrom);
    }

    void load_comparison_10() {
        unqptr_load<10>();
        unique_ptr_load<10>();
    }

    void load_comparison_100() {
        unqptr_load<100>();
        unique_ptr_load<100>();
    }

    void load_comparison_1e3() {
        unqptr_load<1000>();
        unique_ptr_load<1000>();
    }

    void load_comparison_1e4() {
        unqptr_load<10000>();
        unique_ptr_load<10000>();
    }

    void load_comparison_1e5() {
        unqptr_load<100000>();
        unique_ptr_load<100000>();
    }

    void load_comparison_1e6() {
        unqptr_load<1000000>();
        unique_ptr_load<1000000>();
    }
};

QTEST_MAIN(TestUnqPtr)
#include "TestUnqPtr.moc"
