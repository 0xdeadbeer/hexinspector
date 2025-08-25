#include "inspector.h"
#include <QByteArray>
#include <QAbstractScrollArea>
#include <QPainter>
#include <QPaintEvent>
#include <QShortcut>
#include <QtGlobal>
#include <QPoint>
#include <QFont>

Inspector::Inspector(QWidget *parent)
    : QAbstractScrollArea{parent}
    , grid(16, 12, 30, 16*2, QFont("Hack Nerd Font Mono", 12))
{
    viewport()->update();

    QObject::connect(new QShortcut(QKeySequence("l"), this), SIGNAL(activated()), this, SLOT(CursorMoveRight()));
    QObject::connect(new QShortcut(QKeySequence("h"), this), SIGNAL(activated()), this, SLOT(CursorMoveLeft()));
    QObject::connect(new QShortcut(QKeySequence("j"), this), SIGNAL(activated()), this, SLOT(CursorMoveDown()));
    QObject::connect(new QShortcut(QKeySequence("k"), this), SIGNAL(activated()), this, SLOT(CursorMoveUp()));
}

void Inspector::ContentsUpdated(QByteArray buff) {
    fileContents = buff;
    viewport()->repaint();
}

void Inspector::paintEvent(QPaintEvent *event) {
    QPainter painter(viewport());

    painter.setFont(grid.FontFamily);
    painter.fillRect(event->rect(), viewport()->palette().color(QPalette::Mid));

    QString strData = this->fileContents.toUpper().toHex(' ');
    QStringList hexCodes = strData.split(" ", Qt::SkipEmptyParts);

    int totalRows = hexCodes.length()/grid.Columns;
    int leftRows = totalRows-grid.DisplayRow >= grid.Rows ? grid.Rows : totalRows-grid.DisplayRow;

    int hpadding = 3;
    int vpadding = 5;
    int cursorColumn = grid.IndexCursor % grid.Columns;
    int cursorRow = qFloor(grid.IndexCursor / grid.Columns);
    QRect cursorColumnRect(cursorColumn*grid.HSpacing+grid.Margin-hpadding,
                           0,
                           grid.FontMetrics.tightBoundingRect("AA").width()+2*hpadding,
                           grid.Margin+grid.Rows*(grid.VSpacing));
    drawColumn(&painter, cursorColumnRect);

    QRect cursorRowRect(0,
                        (cursorRow*grid.VSpacing)-grid.FontMetrics.tightBoundingRect("AA").height()+grid.Margin-vpadding,
                        grid.Margin+grid.Columns*grid.HSpacing,
                        grid.FontMetrics.tightBoundingRect("AA").height()+2*vpadding);
    drawRow(&painter, cursorRowRect);

    for (int y = grid.DisplayRow; y < leftRows; y++) {
        for (int x = 0; x < grid.Columns; x++) {
            int cell = y*grid.Columns+x;
            if (cell >= hexCodes.length()) {
                return;
            }

            QString cellContents = hexCodes.at(y*grid.Columns+x);
            QPoint point(x*grid.HSpacing+grid.Margin, y*grid.VSpacing+grid.Margin);
            if (cell == grid.IndexCursor) {
                drawSelectedCell(&painter, cellContents, point);
                continue;
            }


            drawCell(&painter, cellContents, point);
        }
    }
}

void Inspector::drawColumn(QPainter *painter, QRect rect) {
    painter->fillRect(rect, QColor(0xaa, 0xaa, 0xaa, 0xaa));
}

void Inspector::drawRow(QPainter *painter, QRect rect) {
    painter->fillRect(rect, QColor(0xaa, 0xaa, 0xaa, 0xaa));
}

void Inspector::drawCell(QPainter *painter, QString contents, QPoint point) {
    painter->drawText(point, contents);
}

void Inspector::drawSelectedCell(QPainter *painter, QString contents, QPoint point) {
    QRect boundingBox = grid.FontMetrics.tightBoundingRect("A");
    const int padding = 1;

    QPoint rectPoint(point);
    if (grid.OffsetCursor) {
        rectPoint.setX(rectPoint.x()+boundingBox.width());
    } else {
        point.setX(point.x()+boundingBox.width());
    }

    painter->fillRect(rectPoint.x(), rectPoint.y()-boundingBox.height()-4*padding, boundingBox.width(), boundingBox.height()+8*padding, palette().color(QPalette::HighlightedText));

    painter->setPen(Qt::white);
    drawCell(painter, contents.at(grid.OffsetCursor), rectPoint);

    painter->setPen(Qt::black);
    drawCell(painter, contents.at(grid.OffsetCursor ? 0 : 1), point);
}

void Inspector::scroll(int direction) {
    if (direction == 0) {
        return;
    }

    if (direction == 1) {
        return;
    }
}

void Inspector::CursorMoveRight() {
    this->grid.OffsetCursor = (this->grid.OffsetCursor + 1) % 2;
    if (this->grid.OffsetCursor % 2 == 0) {
        this->grid.IndexCursor = qBound(0, this->grid.IndexCursor+1, fileContents.length()-1);
    }
    viewport()->update();
}

void Inspector::CursorMoveLeft() {
    this->grid.OffsetCursor = (this->grid.OffsetCursor + 1) % 2;
    if (this->grid.OffsetCursor % 2 != 0) {
        this->grid.IndexCursor = qBound(0, this->grid.IndexCursor-1, fileContents.length()-1);
    }
    viewport()->update();
}

void Inspector::CursorMoveDown() {
    this->grid.IndexCursor = qBound(0, this->grid.IndexCursor+grid.Columns, fileContents.length()-1);
    viewport()->update();
}

void Inspector::CursorMoveUp() {
    this->grid.IndexCursor = qBound(0, this->grid.IndexCursor-grid.Columns, fileContents.length()-1);
    viewport()->update();
}
