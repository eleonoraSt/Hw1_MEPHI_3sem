#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "include/Sequence.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow: public QMainWindow {
    Q_OBJECT
private:
    Ui::MainWindow* ui;
    Sequence<int>* seq;

    void print();
    void clearMsg();
    void clearOutput();
    void modified();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
public slots:
    void inputSlot();
    void firstItemSlot();
    void lastItemSlot();
    void getItemSlot();
    void sliceSlot();
    void sizeSlot();
    void prependSlot();
    void appendSlot();
    void insertSlot();
};

#endif // MAINWINDOW_H
