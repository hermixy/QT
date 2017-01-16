#include "XmlWriter.h"

#include "../Base/LibBase.h"
#include <QFile>
#include <QTextStream>

TXmlWriter::TXmlWriter()
{
}

TXmlWriter::~TXmlWriter()
{
}

void TXmlWriter::fAddHead()
{
    QDomProcessingInstruction instruction;
    instruction = mDoc.createProcessingInstruction(tr("xml"), tr("version=\"1.0\" encoding=\"UTF-8\""));
    mDoc.appendChild(instruction);
}

QDomElement TXmlWriter::fAddRoot(const QString Root)
{
    QDomElement XmlRoot = mDoc.createElement(Root);
    mDoc.appendChild(XmlRoot);

    return XmlRoot;
}

QDomElement TXmlWriter::fAddChild(QDomElement &Parent, const QString Name)
{
    QDomElement XmlNode = mDoc.createElement(Name);
    Parent.appendChild(XmlNode);

    return XmlNode;
}

bool TXmlWriter::fInsertGraphics(QDomElement &XmlItem, TItem *Item)
{
    TGraphics *Graphics = dynamic_cast<TGraphics *>(Item);

    if(NULL == Graphics || XmlItem.isNull())
    {
        return false;
    }

    XmlItem.setAttribute(tr("NodeId"), Graphics->fGetNodeId());
    XmlItem.setAttribute(tr("MainId"), Graphics->fGetMainId());
    XmlItem.setAttribute(tr("GroupId"), Graphics->fGetGroupId());
    XmlItem.setAttribute(tr("GroupName"), Graphics->fGetGroupName());
    XmlItem.setAttribute(tr("Value"), Graphics->fGetValue());
    XmlItem.setAttribute(tr("Coe"), Graphics->fGetCoe());
    XmlItem.setAttribute(tr("Link"), Graphics->fGetLinkScene());
    XmlItem.setAttribute(tr("startAddr"), Graphics->startAddr);
    XmlItem.setAttribute(tr("modbuspro"), Graphics->threeDevName);
    XmlItem.setAttribute(tr("greaterEqualSize"), Graphics->greaterEqual.size());
    XmlItem.setAttribute(tr("lessThanSize"), Graphics->lessThan.size());
    int i=0;
    foreach (TGraphics::link link, Graphics->greaterEqual) {

        XmlItem.setAttribute(tr("greaterEqualcurrntNum")+QString::number(i), link.currntNum);
        XmlItem.setAttribute(tr("greaterEqualgroupId")+QString::number(i), link.groupId);
        XmlItem.setAttribute(tr("greaterEqualwriteNum")+QString::number(i), link.writeNum);
        i++;
    }
    i=0;
    foreach (TGraphics::link link, Graphics->lessThan) {

        XmlItem.setAttribute(tr("lessThancurrntNum")+QString::number(i), link.currntNum);
        XmlItem.setAttribute(tr("lessThangroupId")+QString::number(i), link.groupId);
        XmlItem.setAttribute(tr("lessThanwriteNum")+QString::number(i), link.writeNum);
        i++;
    }

    return true;
}

bool TXmlWriter::fInsertGroup(QDomElement &XmlItem, TItem *Item)
{
    TGroup *Group = dynamic_cast<TGroup *>(Item);

    if(NULL == Group || XmlItem.isNull())
    {
        return false;
    }

    foreach (TItem *Item, Group->fGetItemsList())
    {
        fAddItem(XmlItem, Item, Group);
    }

    return true;
}

bool TXmlWriter::fInsertPolygon(QDomElement &XmlItem, TItem *Item)
{
    TPolygon *Polygon = dynamic_cast<TPolygon *>(Item);

    if(NULL == Polygon || XmlItem.isNull())
    {
        return false;
    }

    QPolygonF PolygonF = Polygon->fGetPolygonPoints();

    for(int i = 0; i < PolygonF.count(); i++)
    {
        QDomElement XmlPoint = fAddChild(XmlItem, tr("Point%1").arg(i));

        XmlPoint.setAttribute(tr("x"), QString::number(PolygonF.at(i).x()));
        XmlPoint.setAttribute(tr("y"), QString::number(PolygonF.at(i).y()));
    }

    return true;
}

bool TXmlWriter::fInsertArc(QDomElement &XmlItem, TItem *Item)
{
    TArc *Arc = dynamic_cast<TArc *>(Item);

    if(NULL == Arc || XmlItem.isNull())
    {
        return false;
    }

    XmlItem.setAttribute(tr("Mode"), Arc->fGetMode());
    XmlItem.setAttribute(tr("StartAngle"), Arc->fGetStartAngle());
    XmlItem.setAttribute(tr("CoverAngle"), Arc->fGetCoverAngle());

    return true;
}

bool TXmlWriter::fInsertText(QDomElement &XmlItem, TItem *Item)
{
    TText *Text = dynamic_cast<TText *>(Item);

    if(NULL == Text || XmlItem.isNull())
    {
        return false;
    }

    XmlItem.setAttribute(tr("Text"), Text->fGetText());
    XmlItem.setAttribute(tr("FontFamily"), Text->fGetFontFamily());
    XmlItem.setAttribute(tr("FontSize"), Text->fGetFontSize());
    XmlItem.setAttribute(tr("FontBlod"), BoolToInt(Text->fGetFontBlod()));
    XmlItem.setAttribute(tr("FontItalic"), BoolToInt(Text->fGetFontItalic()));
    XmlItem.setAttribute(tr("FontSpacing"), Text->fGetFontSpacing());
    XmlItem.setAttribute(tr("FontUnderLine"), BoolToInt(Text->fGetFontUnderLine()));
    XmlItem.setAttribute(tr("FontStrikeOut"), BoolToInt(Text->fGetFontStrikeOut()));

    return true;
}

bool TXmlWriter::fInsertValueText(QDomElement &XmlItem, TItem *Item)
{
    TValueText *ValueText = dynamic_cast<TValueText *>(Item);

    if(NULL == ValueText || XmlItem.isNull())
    {
        return false;
    }

    XmlItem.setAttribute(tr("Mode"), ValueText->fGetMode());
    XmlItem.setAttribute(tr("IntLen"), ValueText->fGetIntLen());
    XmlItem.setAttribute(tr("PointLen"), ValueText->fGetPointLen());
    XmlItem.setAttribute(tr("UpAlarm"), ValueText->mUpAlarm);
    XmlItem.setAttribute(tr("DownAlarm"), ValueText->mDownAlarm);


    return true;
}

bool TXmlWriter::fInsertPixmap(QDomElement &XmlItem, TItem *Item)
{
    TPixmap *Pixmap = dynamic_cast<TPixmap *>(Item);

    if(NULL == Pixmap || XmlItem.isNull())
    {
        return false;
    }
    //qDebug()<<Pixmap->mFileDir.size()<<Pixmap->mFileDir.at(0);
    if(Pixmap->mFileDir.size()==3)
    {
        XmlItem.setAttribute(tr("FileDir"), Pixmap->mFileDir.at(0));
        XmlItem.setAttribute(tr("FileDir1"), Pixmap->mFileDir.at(1));
        XmlItem.setAttribute(tr("FileDir2"), Pixmap->mFileDir.at(2));

        XmlItem.setAttribute(tr("condiation"), Pixmap->condiation.at(0));
        XmlItem.setAttribute(tr("condiation1"), Pixmap->condiation.at(1));
        XmlItem.setAttribute(tr("condiation2"), Pixmap->condiation.at(2));
    }
    else
    {
        XmlItem.setAttribute(tr("FileDir"), "");
        XmlItem.setAttribute(tr("FileDir1"), "");
        XmlItem.setAttribute(tr("FileDir2"), "");

        XmlItem.setAttribute(tr("condiation"), 0);
        XmlItem.setAttribute(tr("condiation1"), 0);
        XmlItem.setAttribute(tr("condiation2"), 0);
    }

    return true;
}

bool TXmlWriter::fInsertWidgets(QDomElement &XmlItem, TItem *Item)
{
    TWidgets *Widgets = dynamic_cast<TWidgets *>(Item);

    if(NULL == Widgets || XmlItem.isNull())
    {
        return false;
    }

    //XmlItem.setAttribute(tr("NameId"), Widgets->fGetNameId());
    //XmlItem.setAttribute(tr("ServiceId"), Widgets->fGetServId());
    XmlItem.setAttribute(tr("JsWidgetsName"), Widgets->mJsWidgets.fGetName());

    return true;
}

bool TXmlWriter::fInsertTable(QDomElement &XmlItem, TItem *Item)
{
    TTable *Table = dynamic_cast<TTable *>(Item);

    if(NULL == Table || XmlItem.isNull())
    {
        return false;
    }

    int ColSum = Table->fGetColSum();
    int RowSum = Table->fGetRowSum();

    XmlItem.setAttribute(tr("ColSum"), ColSum);
    XmlItem.setAttribute(tr("RowSum"), RowSum);
    XmlItem.setAttribute(tr("RowLength"), Table->fGetRowLength());

    for(int i = 0; i < ColSum; i++)
    {
        QDomElement XmlCol = fAddChild(XmlItem, tr("Col%1").arg(i));
        XmlCol.setAttribute(tr("Col"), i);
        XmlCol.setAttribute(tr("Length"), Table->fGetColLength(i));

        for(int j = 0; j < RowSum; j++)
        {
            QDomElement XmlRow = fAddChild(XmlCol, tr("Row%1").arg(j));
            XmlRow.setAttribute(tr("Row"), j);
            XmlRow.setAttribute(tr("Value"), Table->fGetCell(j, i));
        }
    }

    return true;
}

bool TXmlWriter::fInsertButton(QDomElement &XmlItem, TItem *Item)
{
    TButton *Button = dynamic_cast<TButton *>(Item);

    if(NULL == Button || XmlItem.isNull())
    {
        return false;
    }
    XmlItem.setAttribute(tr("Text"), Button->fGetText());
    XmlItem.setAttribute(tr("Link"), Button->fGetLinkScene());
    XmlItem.setAttribute(tr("mcomNum"), Button->mcomNum);
    XmlItem.setAttribute(tr("width"), Button->width);
    XmlItem.setAttribute(tr("heigth"), Button->heigth);
    XmlItem.setAttribute(tr("mBeginWeek"), Button->mBeginWeek);
    XmlItem.setAttribute(tr("mEndWeek"), Button->mEndWeek);
    XmlItem.setAttribute(tr("mBeginTime"), Button->mBeginTime.toString());
    XmlItem.setAttribute(tr("mEndTime"), Button->mEndTime.toString());
    XmlItem.setAttribute(tr("mFontSize"), Button->fGetFontSize());
    XmlItem.setAttribute(tr("isShowTimer"), Button->isShowTimer?1:0);


    XmlItem.setAttribute(tr("greaterEqualSize"), Button->greaterEqual.size());

    int i=0;
    foreach (TButton::link link, Button->greaterEqual) {


        XmlItem.setAttribute(tr("greaterEqualgroupId")+QString::number(i), link.groupId);
        XmlItem.setAttribute(tr("greaterEqualwriteNum")+QString::number(i), link.writeNum);
        i++;
    }
    XmlItem.setAttribute(tr("controlSize"), Button->control.size());
    i=0;
    foreach (TButton::Control contral, Button->control) {
        XmlItem.setAttribute(tr("contralservername")+QString::number(i), contral.servername);
        XmlItem.setAttribute(tr("contralmainId")+QString::number(i), contral.mainId);
        XmlItem.setAttribute(tr("contraldevId")+QString::number(i), contral.devId);
        XmlItem.setAttribute(tr("contralstartAddr")+QString::number(i), contral.startAddr);
        XmlItem.setAttribute(tr("contralwriteNum")+QString::number(i), contral.writeNum);
        i++;
    }
    XmlItem.setAttribute(tr("backgroundPixmap"), Button->mPixPath);

    return true;
}

bool TXmlWriter::fInsertInput(QDomElement &XmlItem, TItem *Item)
{
    TInputEdit *Input = dynamic_cast<TInputEdit *>(Item);

    if(NULL == Input || XmlItem.isNull())
    {
        return false;
    }

    XmlItem.setAttribute(tr("Text"), Input->fGetText());

    return true;
}

bool TXmlWriter::fInsertCurve(QDomElement &XmlItem, TItem *Item)
{
    TCurve *Curve = dynamic_cast<TCurve *>(Item);

    if(NULL == Curve || XmlItem.isNull())
    {
        return false;
    }

    XmlItem.setAttribute(tr("DataSum"), Curve->fGetDataSum());
    XmlItem.setAttribute(tr("Interval"), Curve->fGetInterval());
    XmlItem.setAttribute(tr("Time"), Curve->fGetTime());

    return true;
}

bool TXmlWriter::fInsertMeter(QDomElement &XmlItem, TItem *Item)
{
    TMeter *Meter = dynamic_cast<TMeter *>(Item);

    if(NULL == Meter || XmlItem.isNull())
    {
        return false;
    }

    XmlItem.setAttribute(tr("Mode"), Meter->fGetMode());
    XmlItem.setAttribute(tr("ScaleSum"), Meter->fGetScaleSum());
    XmlItem.setAttribute(tr("StartAngle"), Meter->fGetStartAngle());
    XmlItem.setAttribute(tr("StepAngle"), Meter->fGetStepAngle());
    XmlItem.setAttribute(tr("StartValue"), Meter->fGetStartValue());
    XmlItem.setAttribute(tr("StepValue"), Meter->fGetStepValue());

    QDomElement XmlColor = fAddChild(XmlItem, tr("PointerColor"));
    QColor mColor = Meter->fGetPointerColor();

    XmlColor.setAttribute(tr("r"), mColor.red());
    XmlColor.setAttribute(tr("g"), mColor.green());
    XmlColor.setAttribute(tr("b"), mColor.blue());
    XmlColor.setAttribute(tr("a"), mColor.alpha());

    return true;
}

bool TXmlWriter::fInsertProcessBar(QDomElement &XmlItem, TItem *Item)
{
    TProcessBar *ProcessBar = dynamic_cast<TProcessBar *>(Item);

    if(NULL == ProcessBar)
    {
        return false;
    }

    XmlItem.setAttribute(tr("Mode"), ProcessBar->fGetMode());
    XmlItem.setAttribute(tr("ScaleSum"), ProcessBar->fGetScaleSum());
    XmlItem.setAttribute(tr("StartValue"), ProcessBar->fGetStartValue());
    XmlItem.setAttribute(tr("StepValue"), ProcessBar->fGetStepValue());

    QDomElement XmlColor = fAddChild(XmlItem, tr("ValueColor"));
    QColor mColor = ProcessBar->fGetValueColor();

    XmlColor.setAttribute(tr("r"), mColor.red());
    XmlColor.setAttribute(tr("g"), mColor.green());
    XmlColor.setAttribute(tr("b"), mColor.blue());
    XmlColor.setAttribute(tr("a"), mColor.alpha());

    return true;
}

bool TXmlWriter::fInsertItemEx(QDomElement &XmlItem, TItem *Item)
{
    TItemEx *ItemEx = dynamic_cast<TItemEx *>(Item);

    if(NULL == ItemEx)
    {
        return false;
    }

    XmlItem.setAttribute(tr("NameId"), ItemEx->fGetNameId());
    XmlItem.setAttribute(tr("ServiceId"), ItemEx->fGetServiceId());
    XmlItem.setAttribute(tr("Hint"), ItemEx->fGetHint());
    XmlItem.setAttribute(tr("UserDataName1"), ItemEx->fGetUserDataName1());
    XmlItem.setAttribute(tr("UserData1"), ItemEx->fGetUserData1());
    XmlItem.setAttribute(tr("UserDataName2"), ItemEx->fGetUserDataName2());
    XmlItem.setAttribute(tr("UserData2"), ItemEx->fGetUserData2());
    XmlItem.setAttribute(tr("showStart"), ItemEx->showStart);
    XmlItem.setAttribute(tr("showStop"), ItemEx->showStop);

    return true;
}

bool TXmlWriter::fAddItem(QDomElement &DomParent, TItem *Item, TGroup *GroupParent)
{
    if(NULL == Item || DomParent.isNull())
    {
        return false;
    }

    QDomElement XmlItem;
    QDomElement XmlColor;
    QColor mColor;

    XmlItem = fAddChild(DomParent, tr("item"));

    XmlItem.setAttribute(tr("Type"), Item->fGetType());

    if(GroupParent)
    {
        // Group中的图形，进行坐标变换
        XmlItem.setAttribute(tr("StartX"), GroupParent->fGetItemStartPos(Item).x());
        XmlItem.setAttribute(tr("StartY"), GroupParent->fGetItemStartPos(Item).y());
        XmlItem.setAttribute(tr("StopX"), GroupParent->fGetItemStopPos(Item).x());
        XmlItem.setAttribute(tr("StopY"), GroupParent->fGetItemStopPos(Item).y());
    }
    else
    {
        XmlItem.setAttribute(tr("StartX"), Item->fGetStartX());
        XmlItem.setAttribute(tr("StartY"), Item->fGetStartY());
        XmlItem.setAttribute(tr("StopX"), Item->fGetStopX());
        XmlItem.setAttribute(tr("StopY"), Item->fGetStopY());
    }

    XmlItem.setAttribute(tr("LineWidth"), Item->fGetLineWidth());
    XmlItem.setAttribute(tr("LineStyle"), (int)(Item->fGetLineStyle()));
    XmlItem.setAttribute(tr("Z"), Item->fGetZValue());
    XmlItem.setAttribute(tr("Rotate"), Item->fGetRotateAngle());
    XmlItem.setAttribute(tr("RotateX"), Item->fGetRotateCenterX());
    XmlItem.setAttribute(tr("RotateY"), Item->fGetRotateCenterY());

    XmlColor = fAddChild(XmlItem, tr("LineColor"));
    mColor = Item->fGetLineColor();
    XmlColor.setAttribute(tr("r"), mColor.red());
    XmlColor.setAttribute(tr("g"), mColor.green());
    XmlColor.setAttribute(tr("b"), mColor.blue());    
    XmlColor.setAttribute(tr("a"), mColor.alpha());

    XmlColor = fAddChild(XmlItem, tr("BackColor"));
    mColor = Item->fGetBackGroundColor();
    XmlColor.setAttribute(tr("r"), mColor.red());
    XmlColor.setAttribute(tr("g"), mColor.green());
    XmlColor.setAttribute(tr("b"), mColor.blue());
    XmlColor.setAttribute(tr("a"), mColor.alpha());

    //////////////
    fInsertItemEx(XmlItem, Item);

    // 插入图形的所有属性 ///////////////////////////////////
    fInsertGraphics(XmlItem, Item);

    // 插入群组的所有属性
    fInsertGroup(XmlItem, Item);

    // 插入多边形属性
    fInsertPolygon(XmlItem, Item);

    // 插入角度属性
    fInsertArc(XmlItem, Item);

    // 插入文本属性
    fInsertText(XmlItem, Item);

    // 插入值文本属性
    fInsertValueText(XmlItem, Item);

    // 插入图片属性
    fInsertPixmap(XmlItem, Item);

    // 插入实时曲线
    fInsertCurve(XmlItem, Item);

    // 插入仪表属性
    fInsertMeter(XmlItem, Item);

    // 插入刻度条
    fInsertProcessBar(XmlItem, Item);

    // 插入控件属性 ////////////////////////////////////////////
    fInsertWidgets(XmlItem, Item);

    // 插入表格属性
    fInsertTable(XmlItem, Item);

    // 插入按钮属性
    fInsertButton(XmlItem, Item);

    // 插入输入框控件
    fInsertInput(XmlItem, Item);

    return true;
}

bool TXmlWriter::fSave(QString FileName)
{
    QFile mFile(FileName);

    if(!mFile.open( QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return false;
    }

    QTextStream mOut(&mFile);
    mOut.setCodec("UTF-8");
    mDoc.save(mOut, 4);

    return true;
}
