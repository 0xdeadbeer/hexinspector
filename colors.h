#ifndef COLORS_H
#define COLORS_H


#include <QColor>

static std::unordered_map<std::string, QColor> ColorMap = {
    { "WINDOW::BACKGROUND", QColor(200, 200, 200, 255) },
    { "INSPECTOR::BACKGROUND", QColor(180, 180, 180, 255) },
    { "INSPECTOR::FOREGROUND", QColor(40, 40, 40, 255) },
    { "INSPECTOR::BACKGROUND::COLUMN::ACTIVE", QColor(150, 150, 150, 255) },
    { "INSPECTOR::BACKGROUND::ROW::ACTIVE", QColor(150, 150, 150, 255) },
    { "INSPECTOR::FOREGROUND::CELL", QColor(255, 255, 255, 255) },
    { "INSPECTOR::FOREGROUND::CELL::ACTIVE", QColor(0, 0, 0, 255) },
    { "INSPECTOR::BACKGROUND::CELL::ACTIVE::HALFBYTE", QColor(60, 60, 60, 255) },
    { "INSPECTOR::FOREGROUND::CELL::ACTIVE::HALFBYTE", QColor(255, 255, 255, 255) },
    { "INSPECTOR::BACKGROUND::MODE::NORMAL", QColor(0x79, 0x80, 0x87, 0xff) },
    { "INSPECTOR::BACKGROUND::MODE::INSERT", QColor(0x0c, 0xa6, 0x78, 0xff) },
    { "INSPECTOR::BACKGROUND::MODE::VISUAL", QColor(0x70, 0x48, 0xe8, 0xff) }
};

#endif // COLORS_H
