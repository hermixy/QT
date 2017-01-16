#ifndef TVIEW_H
#define TVIEW_H

#include <QGraphicsView>
class QWidget;

class TView : public QGraphicsView
{
    Q_OBJECT

public:
    TView(QWidget *parent = 0);
    void fZoom(double _Multiple);
    void fSetZoom(double _Zoom);
    double fGetZoom() const;

Q_SIGNALS:
    void mMousePress(int x, int y);

protected:
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    double mZoom;
};

#endif // TVIEW_H
