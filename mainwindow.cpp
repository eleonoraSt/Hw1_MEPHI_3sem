#include <string>
#include <sstream>
#include <QString>

#include "include/ArraySequence.h"
#include "include/ListSequence.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    seq = new ListSequence<int>();
}

MainWindow::~MainWindow() {
    delete ui;
    delete seq;
}

void MainWindow::print() {
    std::ostringstream seqStream;
    size_t size = seq->GetLength();
    for (size_t index = 0; index < size; index++) {
        seqStream << seq->Get(index) << " ";
    }
    ui->textEdit->setText(QString::fromStdString(seqStream.str()));
}

void MainWindow::clearMsg() {
    ui->label->setText("");
}

void MainWindow::clearOutput() {
    ui->firstItemOut->setText("");
    ui->lastItemOut->setText("");
    ui->getItemOut->setText("");
    ui->sizeOut->setText("");
}

void MainWindow::modified() {
    print();
    clearMsg();
    clearOutput();
}

void MainWindow::inputSlot() {
    delete seq;
    if (ui->listRadioButton->isChecked()) {
        seq = new ListSequence<int>();
    } else {
        seq = new ArraySequence<int>();
    }
    std::string seqText = ui->textEdit->toPlainText().toStdString();
    std::istringstream seqStream(seqText);
    int buf;
    while (seqStream >> buf) {
        seq->Append(buf);
    }
    clearMsg();
    clearOutput();
}

void MainWindow::firstItemSlot() {
    int first;
    try {
        first = seq->GetFirst();
    } catch (std::out_of_range err) {
        ui->label->setText("Пустая последовательность");
        return;
    }
    ui->firstItemOut->setText(QString::number(first));
    clearMsg();
}

void MainWindow::lastItemSlot() {
    int last;
    try {
        last = seq->GetLast();
    } catch (std::out_of_range err) {
        ui->label->setText("Пустая последовательность");
        return;
    }
    ui->lastItemOut->setText(QString::number(last));
    clearMsg();
}

void MainWindow::getItemSlot() {
    int item;
    bool ok;
    size_t index = ui->getItemIn->text().toULongLong(&ok);
    if (!ok) {
        ui->label->setText("Введите неотрицательное целое число в качестве индекса");
        return;
    }
    try {
        item = seq->Get(index);
    } catch (std::out_of_range err) {
        ui->label->setText("Индекс больше длины последовательности");
        return;
    }
    ui->getItemOut->setText(QString::number(item));
    clearMsg();
}

void MainWindow::sliceSlot() {
    Sequence<int>* subseq;
    bool okStart, okEnd;
    size_t start = ui->sliceStartIn->text().toULongLong(&okStart);
    size_t end = ui->sliceEndIn->text().toULongLong(&okEnd);
    if (!(okStart && okEnd)) {
        ui->label->setText("Введите два неотрицательных целых числа в качестве индексов");
        return;
    }
    try {
        subseq = seq->GetSubsequence(start, end);
    } catch (std::out_of_range err) {
        ui->label->setText("Введите индекс конца больше индекса начала и не меньше длины");
        return;
    }
    delete seq;
    seq = subseq;
    modified();
}

void MainWindow::sizeSlot() {
    size_t size = seq->GetLength();
    ui->sizeOut->setText(QString::number(size));
}

void MainWindow::prependSlot() {
    bool ok;
    int item = ui->prependIn->text().toInt(&ok);
    if (!ok) {
        ui->label->setText("Введите целое число");
        return;
    }
    seq->Prepend(item);
    modified();
}

void MainWindow::appendSlot() {
    bool ok;
    int item = ui->appendIn->text().toInt(&ok);
    if (!ok) {
        ui->label->setText("Введите целое число");
        return;
    }
    seq->Append(item);
    modified();
}

void MainWindow::insertSlot() {
    bool okItem, okIndex;
    int item = ui->insertItemIn->text().toInt(&okItem);
    size_t index = ui->insertIndexIn->text().toULongLong(&okIndex);
    if (!(okItem && okIndex)) {
        ui->label->setText("Введите целое число и индекс в виде целого неотрицательного числа");
        return;
    }
    try {
        seq->InsertAt(item, index);
    } catch (std::out_of_range err) {
        ui->label->setText("Индекс больше длины последовательности");
        return;
    }
    modified();
}
