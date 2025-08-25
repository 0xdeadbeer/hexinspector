#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./inspector.h"
#include <QString>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QFile>
#include <QLabel>
#include <QChar>
#include <QDebug>
#include <QtMath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , inspector()
{
    ui->setupUi(this);

    mainPalette.setColor(QPalette::Window, QColor(0xCCCCCC));
    mainPalette.setColor(QPalette::Mid, QColor(0xEEEEEE));
    mainPalette.setColor(QPalette::HighlightedText, QColor(0, 0, 0));
    setPalette(mainPalette);

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::OpenFile);
    connect(this, &MainWindow::ContentsUpdated, &this->inspector, &Inspector::ContentsUpdated);

    ui->centralwidget->setLayout(new QVBoxLayout());
    ui->centralwidget->layout()->addWidget(&this->inspector);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OpenFile() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open file for hexdump"), QDir::homePath());
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray newContents = file.readAll();
    emit ContentsUpdated(newContents);
}
