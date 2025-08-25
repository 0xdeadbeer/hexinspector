#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <QObject>
#include <QWidget>
#include <QByteArray>
#include <QPainter>
#include <QAbstractScrollArea>

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

private:
    void drawColumn(QPainter *painter, QRect rect);
    void drawRow(QPainter *painter, QRect rect);
    void drawCell(QPainter *painter, QString contents, QPoint point);
    void drawSelectedCell(QPainter *painter, QString contents, QPoint point);
    void scroll(int direction);

public slots:
    void ContentsUpdated(QByteArray newContents);
    void CursorMoveRight();
    void CursorMoveLeft();
    void CursorMoveDown();
    void CursorMoveUp();

private:
    QByteArray fileContents;
    InspectorGrid grid;

signals:

};

#endif // INSPECTOR_H
