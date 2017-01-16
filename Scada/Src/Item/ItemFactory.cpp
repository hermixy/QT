#include "ItemFactory.h"

#include "../Graphics/GraphicsPub.h"
#include "../Widgets/WidgetsPub.h"

TItemFactory::TItemFactory()
{}

TItemFactory::~TItemFactory()
{}

TItem* TItemFactory::fFactory(int Type, double StartX, double StartY, double StopX, double StopY,
                               const Qt::PenStyle &LineStyle, const int LineWidth,
                               const QColor &LineColor, const QColor &BackColor)
{
    TItem *Item = NULL;

    switch (Type)
    {

    case TItem::Group:
    {
        Item = new TGroup();
    }
    break;

    case TItem::Point:
    {
        Item = new TPoint(StartX, StartY, LineStyle,
                           LineWidth, LineColor, BackColor);
    }
    break;

    case TItem::Line:
    {
        Item = new TLine(StartX, StartY, StopX, StopY, LineStyle,
                          LineWidth, LineColor, BackColor);
    }
    break;

    case TItem::Ellipse:
    {
        Item = new TEllipse(StartX, StartY, StopX, StopY, LineStyle,
                             LineWidth, LineColor, BackColor);
    }
    break;

    case TItem::Rect:
    {
        Item = new TRect(StartX, StartY, StopX, StopY,
                         LineStyle, LineWidth, LineColor, BackColor);
    }
    break;

    case TItem::Polygon:
    {
        Item = new TPolygon(StartX, StartY, StopX, StopY,
                            LineStyle, LineWidth, LineColor, BackColor);
        break;
    }

    case TItem::Arc:
    {
        Item = new TArc(StartX, StartY, StopX, StopY,
                        LineStyle, LineWidth, LineColor, BackColor);
    }
    break;

    case TItem::Text:
    {
        Item = new TText(StartX, StartY, StopX, StopY,
                                LineStyle, LineWidth, LineColor, BackColor);
    }
    break;

    case TItem::ValueText:
    {
        Item = new TValueText(StartX, StartY, StopX, StopY,
                              LineStyle, LineWidth, LineColor, BackColor);
    }
    break;

    case TItem::Pixmap:
    {
        Item = new TPixmap(StartX, StartY, StopX, StopY,
                            LineStyle, LineWidth, LineColor, BackColor);
    }
    break;

    case TItem::Light:
    {
        Item = new TLight(StartX, StartY, StopX, StopY,
                          LineStyle, LineWidth, LineColor, BackColor);
    }
    break;

    case TItem::Curve:
    {
        Item = new TCurve(StartX, StartY, StopX, StopY,
                          LineStyle, LineWidth, LineColor, BackColor);
    }
    break;

    case TItem::Meter:
    {
        Item = new TMeter(StartX, StartY, StopX, StopY,
                                   LineStyle, LineWidth, LineColor, BackColor);
    }
    break;

    case TItem::ProcessBar:
    {
        Item = new TProcessBar(StartX, StartY, StopX, StopY,
                               LineStyle, LineWidth, LineColor, BackColor);
    }
    break;

    case TItem::Table:
    {
        Item = new TTable(StartX, StartY, StopX, StopY,
                          LineStyle, LineWidth, LineColor, BackColor);
    }
    break;

    case TItem::Button:
    {
        Item = new TButton(StartX, StartY, StopX, StopY,
                           LineStyle, LineWidth, LineColor, BackColor);
    }
    break;

    case TItem::InputEdit:
    {
        Item = new TInputEdit(StartX, StartY, StopX, StopY,
                              LineStyle, LineWidth, LineColor, BackColor);
    }
    break;

    default:
        break;
    }

    return Item;
}

