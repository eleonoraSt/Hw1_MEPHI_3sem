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
    std::string seqText;
    std::ostringstream seqStream(seqText);
    size_t size = seq->GetLength();
    for (size_t index = 0; index < size; index++) {
        seqStream << seq->Get(index) << ' ';
    }
    ui->textEdit->setText(QString::fromStdString(seqText));
}

void MainWindow::clearMsg() {
    ui->label->setText("");
}

void MainWindow::clearOutput() {
    ui->firstItemOut->setText("");
    ui->lastItemOut->setText("");
    ui->getItemOut->setText("");
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
    } catch (std::out_of_range msg) {
        ui->label->setText(msg.what());
        return;
    }
    ui->firstItemOut->setText(QString::number(first));
    clearMsg();
}
