#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "inspector.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void OpenFile();

signals:
    void ContentsUpdated(QByteArray newContents);

private:
    Ui::MainWindow *ui;
    QByteArray fileContents;
    Inspector inspector;
    QPalette mainPalette;
    int windowHPadding;
    int windowVPadding;
};
#endif // MAINWINDOW_H
