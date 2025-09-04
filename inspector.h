#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <QObject>
#include <QWidget>
#include <QByteArray>
#include <QPainter>
#include <QAbstractScrollArea>

#define INSPECTOR_MODE_NORMAL 0b00000001
#define INSPECTOR_MODE_INSERT 0b00000010
#define INSPECTOR_MODE_VISUAL 0b00000100

struct InspectorGrid {
    InspectorGrid(int columns, int rows, int vSpacing, int hSpacing, QFont fontFamily)
        : Columns(columns)
        , Rows(rows)
        , VSpacing(vSpacing)
        , HSpacing(hSpacing)
        , FontFamily(fontFamily)
        , FontMetrics(FontFamily)
        , Margin(20) {
        IndexCursor = 0;
        OffsetCursor = 0;
        DisplayRow = 0;
    }

    int Columns;
    int Rows;
    int VSpacing;
    int HSpacing;
    int IndexCursor;
    int OffsetCursor;
    int DisplayRow;
    int Margin;
    QFont FontFamily;
    QFontMetrics FontMetrics;
};

class Inspector : public QAbstractScrollArea
{
    Q_OBJECT
public:
    explicit Inspector(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void handleDefaultEvent(int key);
    void handleNormalEvent(int key);
    void handleInsertEvent(int key);
    void handleVisualEvent(int key);
    QSize GetGridDimensions();

private:
    void drawColumn(QPainter *painter, QRect rect);
    void drawRow(QPainter *painter, QRect rect);
    void drawCell(QPainter *painter, QString contents, QPoint point);
    void drawCell(QPainter *painter, QString contents, QPoint point, QString color);
    void drawSelectedCell(QPainter *painter, QString contents, QPoint point);
    void scroll(int direction);

public slots:
    void DisplayWelcomeScreen();
    void ContentsUpdated(QByteArray newContents);
    void CursorMoveRight();
    void CursorMoveLeft();
    void CursorMoveDown();
    void CursorMoveUp();

signals:
    void SignalWelcomeScreen();

private:
    bool active;
    QByteArray fileContents;
    InspectorGrid grid;

    int mode;

signals:

};

#endif // INSPECTOR_H
