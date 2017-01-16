#ifndef TPIXMAP_H
#define TPIXMAP_H

#include "../Graphics/Graphics.h"
#include <QMovie>
#include <QGraphicsProxyWidget>
#include <ActiveQt/QAxWidget>
#include <QGraphicsProxyWidget>
#include <QtWebView/QtWebView>
#include <QVBoxLayout>
/////////////////////////////////////////////////  图片  /////////////////////////////////////////////////////
class TPixmap : public TGraphics
{
    Q_OBJECT
public:
    TPixmap(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
            const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
            const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white,
            const QString &FileDir = tr("://pic/draw/Pixmap.png"));

    virtual ~TPixmap();

public slots:

    TItem *fCopy();
    void fCopy(TPixmap *PixmapFrom);

    void fSetFileDir(const QString FileDir);

    QString fGetFileDir() const;
    virtual void fSetValue(const double Value);

    void fSetFileDirList(QList<QString> fileDirList);

    void updateCurrentPixmap(int pixnum);
    void updateFlash();

public:
    QString currentPixmapPath;
    bool isPixmap=false;
    QList<QString> mFileDir;                     //文件路径链表
    QList<int> condiation;
    QMovie *movie;
    QPixmap currentPixmap;
    //void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void fDraw(QPainter *painter);

    QGraphicsProxyWidget *proxy;
    QWidget *w;
    QVBoxLayout *vbox;
    QAxWidget *flash;
    QTimer *timer;
};
#endif // TPIXMAP_H
