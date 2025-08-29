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
    { "INSPECTOR::BACKGROUND::CELL::ACTIVE::HALFBYTE", QColor(0, 255, 0, 255) },
    { "INSPECTOR::FOREGROUND::CELL::ACTIVE::HALFBYTE", QColor(255, 255, 255, 255) }
};

#endif // COLORS_H
