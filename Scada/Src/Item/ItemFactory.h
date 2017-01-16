#ifndef TITEM_FACTORY_H
#define TITEM_FACTORY_H

#include "../Item/Item.h"

class TItemFactory
{
public:
    static TItem* fFactory(int Type, double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
                           const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
                           const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);

private:
    TItemFactory();
    ~TItemFactory();
};

#endif // TITEM_FACTORY_H
