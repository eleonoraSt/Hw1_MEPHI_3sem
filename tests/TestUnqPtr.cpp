#include <QTest>

#include "..\include\UnqPtr.h"

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
};

QTEST_MAIN(TestUnqPtr)
#include "TestUnqPtr.moc"
