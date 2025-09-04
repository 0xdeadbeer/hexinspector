#include "inspector.h"
#include <QByteArray>
#include <QAbstractScrollArea>
#include <QPainter>
#include <QPaintEvent>
#include <QShortcut>
#include <QtGlobal>
#include <QPoint>
#include <QFont>
#include <iostream>
#include "colors.h"

Inspector::Inspector(QWidget *parent)
    : QAbstractScrollArea{parent}
    , grid(16, 12, 30, 16*2, QFont("Hack Nerd Font Mono", 12))
    , active(false)
    , mode(INSPECTOR_MODE_NORMAL)
{
    viewport()->update();

    QObject::connect(this, SIGNAL(SignalWelcomeScreen()), this, SLOT(DisplayWelcomeScreen()));
}

void Inspector::DisplayWelcomeScreen() {
    QPainter painter(viewport());

    painter.setFont(grid.FontFamily);
    drawCell(&painter, "Welcome to hexinspector!", QPoint(50, 50));
    drawCell(&painter, "This is a pet project", QPoint(50, 75));
    drawCell(&painter, "written by @0xdeadbeer", QPoint(50, 100));
}


void Inspector::paintEvent(QPaintEvent *event) {
    QPainter painter(viewport());
    painter.fillRect(event->rect(), ColorMap.at("INSPECTOR::BACKGROUND"));
    if (!this->fileContents.length()) {
        active = false;
        emit SignalWelcomeScreen();
        return;
    }

    active = true;

    painter.setFont(grid.FontFamily);

    QString strData = this->fileContents.toHex(' ');
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
                           this->GetGridDimensions().height());
    drawColumn(&painter, cursorColumnRect);

    QRect cursorRowRect(0,
                        (cursorRow*grid.VSpacing)-grid.FontMetrics.tightBoundingRect("AA").height()+grid.Margin-vpadding,
                        this->GetGridDimensions().width(),
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

void Inspector::handleDefaultEvent(int key) {
    if (key == Qt::Key_Escape) {
        this->mode = INSPECTOR_MODE_NORMAL;
        return;
    }

    if (key == Qt::Key_Space) {
        this->fileContents.insert(grid.IndexCursor, (char) 0x00);
        viewport()->update();
        return;
    }

    if (key == Qt::Key_Backspace) {
        this->CursorMoveLeft();
        return;
    }

    if (this->mode == INSPECTOR_MODE_INSERT) {
        return;
    }

    if (this->mode == INSPECTOR_MODE_VISUAL) {
        return;
    }

    if (key == Qt::Key_I) {
        this->mode = INSPECTOR_MODE_INSERT;
    }

    if (key == Qt::Key_V) {
        this->mode = INSPECTOR_MODE_VISUAL;
        return;
    }
}

void Inspector::handleNormalEvent(int key) {
    switch (key) {
    case Qt::Key_H:
        this->CursorMoveLeft();
        break;
    case Qt::Key_J:
        this->CursorMoveDown();
        break;
    case Qt::Key_K:
        this->CursorMoveUp();
        break;
    case Qt::Key_L:
        this->CursorMoveRight();
        break;
    case Qt::Key_X:
        this->fileContents.remove(grid.IndexCursor, 1);
        viewport()->update();
        break;
    case Qt::Key_Space:
        break;
    }
}

void Inspector::handleInsertEvent(int key) {
    bool isNumber = key >= Qt::Key_0 && key <= Qt::Key_9;
    bool isChar = key >= Qt::Key_A && key <= Qt::Key_F;
    bool isNotHexDigit = !isNumber && !isChar;
    if (isNotHexDigit) {
        return;
    }

    char value = 0;
    if (isNumber) {
        value = key - Qt::Key_0;
    }
    if (isChar) {
        value = 0xA + (key - Qt::Key_A);
    }

    char halfByteValue = this->fileContents.at(grid.IndexCursor);
    int writeByte = 1-grid.OffsetCursor;
    int readByte = 1-writeByte;
    int newValue = 0;

    newValue |= halfByteValue & (0b1111 << (readByte * 4)) ;
    newValue |= (value & 0b1111) << (writeByte * 4);

    this->fileContents[grid.IndexCursor] = newValue;

    viewport()->repaint();

    this->CursorMoveRight();
}

void Inspector::handleVisualEvent(int key) {
    switch (key) {
    default:
        break;
    }
}

void Inspector::keyPressEvent(QKeyEvent *event) {
    if (!this->active) {
        return;
    }

    int key = event->key();
    this->handleDefaultEvent(key);

    switch (this->mode) {
    case INSPECTOR_MODE_NORMAL:
        this->handleNormalEvent(key);
        break;
    case INSPECTOR_MODE_INSERT:
        this->handleInsertEvent(key);
        break;
    case INSPECTOR_MODE_VISUAL:
        this->handleVisualEvent(key);
        break;
    }
}

QSize Inspector::GetGridDimensions() {
    return QSize(2*grid.Margin+grid.Columns*grid.HSpacing, 2+grid.Margin+grid.Rows*grid.VSpacing);
}

void Inspector::ContentsUpdated(QByteArray buff) {
    fileContents = buff;
    viewport()->repaint();
}

void Inspector::drawColumn(QPainter *painter, QRect rect) {
    painter->fillRect(rect, ColorMap.at("INSPECTOR::BACKGROUND::COLUMN::ACTIVE"));
}

void Inspector::drawRow(QPainter *painter, QRect rect) {
    painter->fillRect(rect, ColorMap.at("INSPECTOR::BACKGROUND::ROW::ACTIVE"));
}

void Inspector::drawCell(QPainter *painter, QString contents, QPoint point) {
    painter->setPen("INSPECTOR::FOREGROUND");
    painter->drawText(point, contents);
}

void Inspector::drawCell(QPainter *painter, QString contents, QPoint point, QString fgColor) {
    painter->setPen(ColorMap.at(fgColor.toStdString()));
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

    painter->fillRect(rectPoint.x(), rectPoint.y()-boundingBox.height()-4*padding, boundingBox.width(), boundingBox.height()+8*padding, ColorMap.at("INSPECTOR::BACKGROUND::CELL::ACTIVE::HALFBYTE"));

    drawCell(painter, contents.at(grid.OffsetCursor), rectPoint, "INSPECTOR::FOREGROUND::CELL::ACTIVE::HALFBYTE");
    drawCell(painter, contents.at(grid.OffsetCursor ? 0 : 1), point, "INSPECTOR::FOREGROUND::CELL::ACTIVE");
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
