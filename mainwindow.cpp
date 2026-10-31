#include <string>
#include <sstream>
#include <QString>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
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

void MainWindow::inputSlot() {
    std::string seqText = ui->textEdit->toPlainText().toStdString();
    std::istringstream seqStream(seqText);
    int buf;
    while (seqStream >> buf) {
        seq->Append(buf);
    }
}
