#include "View.h"

#include <QtGui>
#include <cmath>

TView::TView(QWidget *parent)
    : QGraphicsView(parent)
{
    //setDragMode(ScrollHandDrag);
    mZoom = 1;
}

void TView::wheelEvent(QWheelEvent *event)
{
    double numDegrees = event->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    double factor = std::pow(1.125, numSteps);

    fZoom(factor);
}

void TView::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit mMousePress(event->pos().x(), event->pos().y());

    QGraphicsView::mousePressEvent(event);
}

void TView::fZoom(double _Multiple)
{
    double NewZoom = mZoom * _Multiple;

    if(NewZoom > 0.1 && NewZoom < 10)
    {
        scale(_Multiple, _Multiple);

        mZoom = NewZoom;
    }
}

void TView::fSetZoom(double _Zoom)
{
    if(_Zoom > 0.1 && _Zoom < 10)
    {
        double Multiple = _Zoom / mZoom;

        scale(Multiple, Multiple);

        mZoom = _Zoom;
    }
}

double TView::fGetZoom() const
{
    return mZoom;
}
