#include "XmlReader.h"

#include "../Js/Js.h"
#include "../Base/LibBase.h"

#include "../Item/ItemFactory.h"

#include <QFile>
#include <QTextStream>

#include <QDebug>

TXmlReader::TXmlReader()
{
    mIsOpen = false;
}

TXmlReader::~TXmlReader()
{

}

bool TXmlReader::fOpen(QString FileName)
{
    QFile mFile(FileName);

    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        return false;
    }

    if(!mDoc.setContent(&mFile, false/*, &errorStr, &errorLine, &errorColumn*/))
    {
        return false;
    }

    mIsOpen = true;

    return true;
}

bool TXmlReader::fIsOpen()
{
    return mIsOpen;
}

QDomElement TXmlReader::fGetRoot()
{
    return mDoc.documentElement();
}

TItem* TXmlReader::fAnalyItem(QDomElement mXmlNode)
{
    TItem *Item = NULL;

    int Type = 0;
    double StartX = 0;
    double StartY = 0;
    double StopX = 0;
    double StopY = 0;
    int LineWidth = 0;
    int LineStyle = 0;
    int LineColorR = 0;
    int LineColorG = 0;
    int LineColorB = 0;
    int LineColorA = 0;
    int BackColorR = 0;
    int BackColorG = 0;
    int BackColorB = 0;
    int BackColorA = 0;

    Type = mXmlNode.attribute(tr("Type")).toInt();

    StartX = mXmlNode.attribute(tr("StartX")).toDouble();
    StartY = mXmlNode.attribute(tr("StartY")).toDouble();
    StopX = mXmlNode.attribute(tr("StopX")).toDouble();
    StopY = mXmlNode.attribute(tr("StopY")).toDouble();
    LineWidth = mXmlNode.attribute(tr("LineWidth")).toInt();
    LineStyle = mXmlNode.attribute(tr("LineStyle")).toInt();

    QDomElement mXmlColor;
    mXmlColor = mXmlNode.firstChild().toElement();

    if(!mXmlColor.isNull())
    {
        bool OkColorA = false;
        LineColorR = mXmlColor.attribute(tr("r")).toInt();
        LineColorG = mXmlColor.attribute(tr("g")).toInt();
        LineColorB = mXmlColor.attribute(tr("b")).toInt();
        LineColorA = mXmlColor.attribute(tr("a")).toInt(&OkColorA);

        if(!OkColorA)
        {
            LineColorA = 255;
        }

        mXmlColor = mXmlColor.nextSibling().toElement();

        if(!mXmlColor.isNull())
        {
            BackColorR = mXmlColor.attribute(tr("r")).toInt();
            BackColorG = mXmlColor.attribute(tr("g")).toInt();
            BackColorB = mXmlColor.attribute(tr("b")).toInt();
            BackColorA = mXmlColor.attribute(tr("a")).toInt(&OkColorA);

            if(!OkColorA)
            {
                BackColorA = 255;
            }
        }
    }

    QColor ItemLineColor(LineColorR, LineColorG, LineColorB, LineColorA);
    QColor ItemBackColor(BackColorR, BackColorG, BackColorB, BackColorA);

    //////////////////////////////////////////////////////////////////////
    Item = TItemFactory::fFactory(Type, StartX, StartY, StopX, StopY,
                                  Qt::PenStyle(LineStyle), LineWidth, ItemLineColor, ItemBackColor);

    switch (Type)
    {
    case TItem::Group:
    {
        TGroup *Group = dynamic_cast<TGroup *>(Item);

        if(Group)
        {
            QDomElement XmlNode = mXmlColor.nextSibling().toElement();

            while(!XmlNode.isNull())
            {
                TItem *SubItem = fAnalyItem(XmlNode);

                Group->fAddItem(SubItem);

                delete SubItem;

                XmlNode = XmlNode.nextSibling().toElement();
            }
        }
    }
    break;

    case TItem::Polygon:
    {
        TPolygon *Polygon = dynamic_cast<TPolygon *>(Item);

        if(Polygon)
        {
            QPolygonF PolygonF;

            bool OkX, OkY;
            double X, Y;

            QDomElement XmlPointF;
            XmlPointF = mXmlColor.nextSibling().toElement();

            while (!XmlPointF.isNull())
            {
                X = XmlPointF.attribute(tr("x")).toDouble(&OkX);
                Y = XmlPointF.attribute(tr("y")).toDouble(&OkY);

                if(OkX && OkY)
                {
                    PolygonF.push_back(QPointF(X, Y));
                }

                XmlPointF = XmlPointF.nextSibling().toElement();
            }

            Polygon->fSetPolygonPoints(PolygonF);
        }
    }
    break;

    case TItem::Arc:
    {
        TArc *Arc = dynamic_cast<TArc *>(Item);

        if(Arc)
        {
            Arc->fSetMode(mXmlNode.attribute(tr("Mode")).toInt());
            Arc->fSetStartAngle(mXmlNode.attribute(tr("StartAngle")).toDouble());
            Arc->fSetCoverAngle(mXmlNode.attribute(tr("CoverAngle")).toDouble());
        }
    }
    break;

    case TItem::Text:
    {
        TText *Text = dynamic_cast<TText *>(Item);

        if(Text)
        {
            Text->fSetText(mXmlNode.attribute(tr("Text")));
            Text->fSetFontFamily(mXmlNode.attribute(tr("FontFamily")));
            Text->fSetFontSize(mXmlNode.attribute(tr("FontSize")).toInt());
            Text->fSetFontBlod(IntToBool(mXmlNode.attribute(tr("FontBlod")).toInt()));
            Text->fSetFontItalic(IntToBool(mXmlNode.attribute(tr("FontItalic")).toInt()));
            Text->fSetFontSpacing(mXmlNode.attribute(tr("FontSpacing")).toInt());
            Text->fSetFontUnderLine(IntToBool(mXmlNode.attribute(tr("FontUnderLine")).toInt()));
            Text->fSetFontStrikeOut(IntToBool(mXmlNode.attribute(tr("FontStrikeOut")).toInt()));
        }
    }
    break;

    case TItem::ValueText:
    {
        TValueText *ValueText = dynamic_cast<TValueText *>(Item);
        //qDebug()<<"Sssssssssssssssssssssssss";
        if(ValueText)
        {
            //qDebug()<<"Sssssssssssssssssssssssss"<<mXmlNode.attribute(tr("Text"));
            ValueText->fSetText(mXmlNode.attribute(tr("Text")));
            ValueText->fSetFontFamily(mXmlNode.attribute(tr("FontFamily")));
            ValueText->fSetFontSize(mXmlNode.attribute(tr("FontSize")).toInt());
            ValueText->fSetFontBlod(IntToBool(mXmlNode.attribute(tr("FontBlod")).toInt()));
            ValueText->fSetFontItalic(IntToBool(mXmlNode.attribute(tr("FontItalic")).toInt()));
            ValueText->fSetFontSpacing(mXmlNode.attribute(tr("FontSpacing")).toInt());
            ValueText->fSetFontUnderLine(IntToBool(mXmlNode.attribute(tr("FontUnderLine")).toInt()));
            ValueText->fSetFontStrikeOut(IntToBool(mXmlNode.attribute(tr("FontStrikeOut")).toInt()));
            ValueText->mUpAlarm=mXmlNode.attribute(tr("UpAlarm")).toDouble();
            ValueText->mDownAlarm=mXmlNode.attribute(tr("DownAlarm")).toDouble();

            ValueText->fSetMode(mXmlNode.attribute(tr("Mode")).toInt());
            ValueText->fSetIntLen(mXmlNode.attribute(tr("IntLen")).toInt());
            ValueText->fSetPointLen(mXmlNode.attribute(tr("PointLen")).toInt());
        }
    }
    break;

    case TItem::Pixmap:
    {
        TPixmap *Pixmap = dynamic_cast<TPixmap *>(Item);

        if(Pixmap)
        {
            Pixmap->mFileDir.clear();
            Pixmap->condiation.clear();

            Pixmap->mFileDir.push_back(mXmlNode.attribute(tr("FileDir")));
            //qDebug()<<mXmlNode.attribute(tr("FileDir"));
           // qDebug()<<mXmlNode.attribute(tr("FileDir1"));
            //qDebug()<<mXmlNode.attribute(tr("FileDir2"));

            Pixmap->mFileDir.push_back(mXmlNode.attribute(tr("FileDir1")));
            Pixmap->mFileDir.push_back(mXmlNode.attribute(tr("FileDir2")));
            Pixmap->condiation.push_back(mXmlNode.attribute(tr("condiation")).toInt());
            Pixmap->condiation.push_back(mXmlNode.attribute(tr("condiation1")).toInt());
            Pixmap->condiation.push_back(mXmlNode.attribute(tr("condiation2")).toInt());
            if(!Pixmap->mFileDir.isEmpty())
            {
                Pixmap->fSetFileDirList(Pixmap->mFileDir);
            }
        }

        break;
    }

    case TItem::Curve:
    {
        TCurve *Curve = dynamic_cast<TCurve *>(Item);

        if(Curve)
        {
            Curve->fSetDataSum(mXmlNode.attribute(tr("DataSum")).toInt());
            Curve->fSetInterval(mXmlNode.attribute(tr("Interval")).toInt());
            Curve->fSetTime(mXmlNode.attribute(tr("Time")).toInt());
        }
    }
    break;

    case TItem::Meter:
    {
        TMeter *Meter = dynamic_cast<TMeter *>(Item);

        if(Meter)
        {
            Meter->fSetMode(mXmlNode.attribute(tr("Mode")).toInt());
            Meter->fSetScaleSum(mXmlNode.attribute(tr("ScaleSum")).toInt());
            Meter->fSetStartAngle(mXmlNode.attribute(tr("StartAngle")).toDouble());
            Meter->fSetStepAngle(mXmlNode.attribute(tr("StepAngle")).toDouble());
            Meter->fSetStartValue(mXmlNode.attribute(tr("StartValue")).toDouble());
            Meter->fSetStepValue(mXmlNode.attribute(tr("StepValue")).toDouble());

            mXmlColor = mXmlColor.nextSibling().toElement();

            if(!mXmlColor.isNull())
            {
                bool OkColorA = false;

                BackColorR = mXmlColor.attribute(tr("r")).toInt();
                BackColorG = mXmlColor.attribute(tr("g")).toInt();
                BackColorB = mXmlColor.attribute(tr("b")).toInt();
                BackColorA = mXmlColor.attribute(tr("a")).toInt(&OkColorA);

                if(!OkColorA)
                {
                    BackColorA = 255;
                }

                QColor PointerColor(BackColorR, BackColorG, BackColorB, BackColorA);

                Meter->fSetPointerColor(PointerColor);
            }
        }
    }
    break;

    case TItem::ProcessBar:
    {
        TProcessBar *ProcessBar = dynamic_cast<TProcessBar *>(Item);

        if(ProcessBar)
        {
            ProcessBar->fSetMode(mXmlNode.attribute(tr("Mode")).toInt());
            ProcessBar->fSetScaleSum(mXmlNode.attribute(tr("ScaleSum")).toInt());
            ProcessBar->fSetStartValue(mXmlNode.attribute(tr("StartValue")).toDouble());
            ProcessBar->fSetStepValue(mXmlNode.attribute(tr("StepValue")).toDouble());

            mXmlColor = mXmlColor.nextSibling().toElement();

            if(!mXmlColor.isNull())
            {
                bool OkColorA = false;

                BackColorR = mXmlColor.attribute(tr("r")).toInt();
                BackColorG = mXmlColor.attribute(tr("g")).toInt();
                BackColorB = mXmlColor.attribute(tr("b")).toInt();
                BackColorA = mXmlColor.attribute(tr("a")).toInt(&OkColorA);

                if(!OkColorA)
                {
                    BackColorA = 255;
                }

                QColor ValueColor(BackColorR, BackColorG, BackColorB, BackColorA);

                ProcessBar->fSetValueColor(ValueColor);
            }
        }
    }
    break;

    case TItem::Table:
    {
        TTable *Table = dynamic_cast<TTable *>(Item);

        if(Table)
        {
            Table->fSetSize(mXmlNode.attribute(tr("RowSum")).toInt(),
                            mXmlNode.attribute(tr("ColSum")).toInt());

            Table->fSetRowLength(mXmlNode.attribute(tr("RowLength")).toInt());

            QDomElement XmlCol = mXmlColor.nextSibling().toElement();

            while (!XmlCol.isNull())
            {
                int Col = XmlCol.attribute(tr("Col")).toInt();
                Table->fSetColLength(Col, XmlCol.attribute(tr("Length")).toInt());

                QDomElement XmlRow = XmlCol.firstChild().toElement();

                while(!XmlRow.isNull())
                {
                    Table->fSetCell(XmlRow.attribute(tr("Row")).toInt(), Col,
                                    XmlRow.attribute(tr("Value")));

                    XmlRow = XmlRow.nextSibling().toElement();
                }

                XmlCol = XmlCol.nextSibling().toElement();
            }
        }
    }
    break;

    case TItem::Button:
    {

        TButton *Button = dynamic_cast<TButton *>(Item);

        if(Button)
        {
            Button->fSetText(mXmlNode.attribute(tr("Text")));
            bool OkTmp = false;
            int iTmp = 0;

            iTmp = mXmlNode.attribute(tr("Link")).toInt(&OkTmp);
            if(OkTmp)
            {
                Button->fSetLinkScene(iTmp);
            }
            Button->mcomNum=mXmlNode.attribute(tr("mcomNum")).toInt();

            Button->width=mXmlNode.attribute(tr("width")).toInt();
            Button->heigth=mXmlNode.attribute(tr("heigth")).toInt();
            Button->fSetFontSize(mXmlNode.attribute(tr("mFontSize")).toInt());
            int sssss= mXmlNode.attribute(tr("isShowTimer")).toInt();
            if(sssss==1) {
                Button->isShowTimer=true;
            } else {
                Button->isShowTimer=false;
            }

            Button->mBeginWeek=mXmlNode.attribute(tr("mBeginWeek")).toInt();
            Button->mEndWeek=mXmlNode.attribute(tr("mEndWeek")).toInt();
            Button->mBeginTime=QTime::fromString(mXmlNode.attribute(tr("mEndWeek")));
            Button->mEndTime=QTime::fromString(mXmlNode.attribute(tr("mEndTime")));

            int i=mXmlNode.attribute(tr("greaterEqualSize")).toInt();
            for(int j=0; j<i; j++)
            {
                TButton::link link;

                link.groupId=mXmlNode.attribute(tr("greaterEqualgroupId")+QString::number(j)).toInt();
                link.writeNum=mXmlNode.attribute(tr("greaterEqualwriteNum")+QString::number(j)).toDouble();
                Button->greaterEqual.append(link);
            }
            i=mXmlNode.attribute(tr("controlSize")).toInt();
            for(int j=0; j<i; j++)
            {
               TButton::Control contral;
               contral.servername=mXmlNode.attribute(tr("contralservername")+QString::number(j));
               contral.mainId=mXmlNode.attribute(tr("contralmainId")+QString::number(j)).toInt();
               contral.devId=mXmlNode.attribute(tr("contraldevId")+QString::number(j)).toInt();
               contral.startAddr=mXmlNode.attribute(tr("contralstartAddr")+QString::number(j)).toInt();
               contral.writeNum=mXmlNode.attribute(tr("contralwriteNum")+QString::number(j)).toDouble();
               Button->control.append(contral);
            }
           //qDebug()<<"controlSize"<<i;
           Button->mPixPath= mXmlNode.attribute(tr("backgroundPixmap"));

        }

    }
    break;

    case TItem::InputEdit:
    {
        TInputEdit *Input = dynamic_cast<TInputEdit *>(Item);

        if(Input)
        {
            Input->fSetText(mXmlNode.attribute(tr("Text")));
        }
    }
    break;

    default:
        break;
    }
    //////////////////////////////////////////////////////////////////////

    if(Item)
    {
        Item->fSetZValue(mXmlNode.attribute(tr("Z")).toDouble());
        Item->fSetRotateCenter(mXmlNode.attribute(tr("RotateX")).toDouble(),
                               mXmlNode.attribute(tr("RotateY")).toDouble());
        Item->fSetRotateAngle(mXmlNode.attribute(tr("Rotate")).toDouble());
    }

    TItemEx *ItemEx = dynamic_cast<TItemEx *>(Item);

    if(ItemEx)
    {
        ItemEx->fSetNameId(mXmlNode.attribute(tr("NameId")));
        ItemEx->fSetServiceId(mXmlNode.attribute(tr("ServiceId")).toInt());
        ItemEx->fSetHint(mXmlNode.attribute(tr("Hint")));

        ItemEx->fSetUserDataName1(mXmlNode.attribute(tr("UserDataName1")));
        ItemEx->fSetUserData1(mXmlNode.attribute(tr("UserData1")).toDouble());
        ItemEx->fSetUserDataName2(mXmlNode.attribute(tr("UserDataName2")));
        ItemEx->fSetUserData2(mXmlNode.attribute(tr("UserData2")).toDouble());
        ItemEx->showStart=mXmlNode.attribute(tr("showStart")).toDouble();
        ItemEx->showStop=mXmlNode.attribute(tr("showStop")).toDouble();
    }

    //////////////////////////////////////////////////////////////////////
    TGraphics *Graphics = dynamic_cast<TGraphics *>(Item);

    if(Graphics)
    {
        bool OkTmp = false;
        int iTmp = 0;

        iTmp = mXmlNode.attribute(tr("Link")).toInt(&OkTmp);
        if(OkTmp)
        {
            Graphics->fSetLinkScene(iTmp);
        }

        //
        Graphics->fSetNodeId(mXmlNode.attribute(tr("NodeId")).toInt());
        Graphics->fSetMainId(mXmlNode.attribute(tr("MainId")).toInt());
        //Graphics->fSetValue(mXmlNode.attribute(tr("Value")).toDouble());
        Graphics->fSetGroupId(mXmlNode.attribute(tr("GroupId")).toInt());
        Graphics->fSetGroupName(mXmlNode.attribute(tr("GroupName")));
        Graphics->fSetLinkScene(mXmlNode.attribute(tr("Link")).toInt());
        Graphics->startAddr=mXmlNode.attribute(tr("startAddr"), "-1").toInt();
        Graphics->threeDevName=mXmlNode.attribute(tr("modbuspro", "modbus rtu"));


        int i=mXmlNode.attribute(tr("greaterEqualSize")).toInt();
        for(int j=0; j<i; j++)
        {
            TGraphics::link link;
            link.currntNum=mXmlNode.attribute(tr("greaterEqualcurrntNum")+QString::number(j)).toDouble();
            link.groupId=mXmlNode.attribute(tr("greaterEqualgroupId")+QString::number(j)).toInt();
            link.writeNum=mXmlNode.attribute(tr("greaterEqualwriteNum")+QString::number(j)).toDouble();
            Graphics->greaterEqual.append(link);
        }
        //qDebug()<<Graphics->greaterEqual.size();
        i=mXmlNode.attribute(tr("lessThanSize")).toInt();
        for(int j=0; j<i; j++)
        {
            TGraphics::link link;
            link.currntNum=mXmlNode.attribute(tr("lessThancurrntNum")+QString::number(j)).toDouble();
            link.groupId=mXmlNode.attribute(tr("lessThangroupId")+QString::number(j)).toInt();
            link.writeNum=mXmlNode.attribute(tr("lessThanwriteNum")+QString::number(j)).toDouble();
            Graphics->lessThan.append(link);
        }


        bool Ok = false;
        double Coe = mXmlNode.attribute(tr("Coe")).toDouble(&Ok);

        if(!Ok)
        {
            Coe = 1;
        }

        Graphics->fSetCoe(Coe);

        // 设定mJs脚本的信息

    }

    ////////////////////////////////////////
    TWidgets *Widgets = dynamic_cast<TWidgets *>(Item);

    if(Widgets)
    {
        //Widgets->fSetNameId(mXmlNode.attribute(tr("NameId")));
        //Widgets->fSetServId(mXmlNode.attribute(tr("ServiceId")).toInt());
        Widgets->mJsWidgets.fSetName(mXmlNode.attribute(tr("JsWidgetsName")));
    }

    return Item;
}

QDomElement TXmlReader::fFindElement(QString Name)
{
    QDomElement XmlFind;
    XmlFind.clear();

    if(!fIsOpen())
    {
        //qDebug() << "fFindElement 1";
        return XmlFind;
    }

    QDomElement mXmlScada = fGetRoot();
    if(mXmlScada.isNull() || mXmlScada.tagName() != tr("scada"))
    {
        //qDebug() << "fFindElement 2";
        return XmlFind;
    }

    XmlFind = mXmlScada.firstChild().toElement();

    while((!XmlFind.isNull()) && (XmlFind.tagName() != Name))
    {
        XmlFind = XmlFind.nextSibling().toElement();
    }

    //qDebug() << "fFindElement 3";
    return XmlFind;
}

bool TXmlReader::fAnalyDB(TDataBaseMsg *DBMsg)
{
    if(0 == DBMsg)
    {
        return false;
    }

    QDomElement XmlDb = fFindElement(tr("db"));

    if(XmlDb.isNull())
    {
        qDebug() << "XmlServer.isNull()";
        return false;
    }

    DBMsg->fSetType(XmlDb.attribute(tr("Type")).toInt());
    DBMsg->fSetHost(XmlDb.attribute(tr("Host")));
    DBMsg->fSetDBName(XmlDb.attribute(tr("DBName")));
    DBMsg->fSetUserName(XmlDb.attribute(tr("User")));
    DBMsg->fSetPassWord(XmlDb.attribute(tr("PassWd")));

    return true;
}
bool TXmlReader::fAnalyCurver(QList<TScene::TimerStruct> &CurverStructList)
{
    QDomElement mXmlCurver = fFindElement(tr("Curver"));

    int i=mXmlCurver.attribute(tr("CurverStructList")).toInt();
    for(int j=0; j<i; j++)
    {
       TScene::TimerStruct contral;
       contral.TimerId=mXmlCurver.attribute(tr("CurverTimerId")+QString::number(j));
       contral.serverId=mXmlCurver.attribute(tr("CurverserverId")+QString::number(j)).toInt();
       contral.mainId=mXmlCurver.attribute(tr("CurvermainId")+QString::number(j)).toInt();
       contral.devId=mXmlCurver.attribute(tr("CurverdevId")+QString::number(j)).toInt();
       contral.startAddr=mXmlCurver.attribute(tr("CurverstartAddr")+QString::number(j)).toInt();

       CurverStructList.append(contral);
    }
    return true;
}
bool TXmlReader::fAnalyPie(QList<TScene::TimerStruct> &CurverStructList)
{
    QDomElement mXmlCurver = fFindElement(tr("Pie"));

    int i=mXmlCurver.attribute(tr("PieStructList")).toInt();
    for(int j=0; j<i; j++)
    {
       TScene::TimerStruct contral;
       contral.TimerId=mXmlCurver.attribute(tr("PieTimerId")+QString::number(j));
       contral.serverId=mXmlCurver.attribute(tr("PieserverId")+QString::number(j)).toInt();
       contral.mainId=mXmlCurver.attribute(tr("PiemainId")+QString::number(j)).toInt();
       contral.devId=mXmlCurver.attribute(tr("PiedevId")+QString::number(j)).toInt();
       contral.startAddr=mXmlCurver.attribute(tr("PiestartAddr")+QString::number(j)).toInt();

       CurverStructList.append(contral);
    }
    return true;
}

TScene* TXmlReader::fAnalyScene(QList<TScene *> &SceneList)
{

    int mHostId = 0;
    TScene *mSceneHost = 0;

    QDomElement mXmlScenes = fFindElement(tr("scenes"));

    if(mXmlScenes.isNull())
    {
    return NULL;
    }

    mHostId = mXmlScenes.attribute(tr("Host")).toInt();

    // scene
    QDomElement mXmlScene = mXmlScenes.firstChild().toElement();

    while(!mXmlScene.isNull())
    {
        int Id = 0;
        QString Name;
        double PosX = 0;
        double PosY = 0;
        double Width = 0;
        double Height = 0;
        int r = 0, g = 0, b = 0;
        QList<TScene::TimerStruct> timerList;

        bool OkId = false;
        bool OkPosX = false;
        bool OkPosY = false;
        bool OkWidth = false;
        bool OkHeight = false;
        bool OkR = false;
        bool OkG = false;
        bool OkB = false;

        Id = mXmlScene.attribute(tr("Id")).toInt(&OkId, 10);
        Name = mXmlScene.attribute(tr("Name"));
        PosX = mXmlScene.attribute(tr("PosX")).toDouble(&OkPosX);
        PosY = mXmlScene.attribute(tr("PosY")).toDouble(&OkPosY);
        Width = mXmlScene.attribute(tr("Width")).toDouble(&OkWidth);
        Height = mXmlScene.attribute(tr("Height")).toDouble(&OkHeight);
       /*********************************************************/

        int i=mXmlScene.attribute(tr("TImerStructList")).toInt();
        for(int j=0; j<i; j++)
        {
           TScene::TimerStruct contral;
           contral.TimerId=mXmlScene.attribute(tr("sceneTimerId")+QString::number(j));
           contral.serverId=mXmlScene.attribute(tr("sceneserverId")+QString::number(j)).toInt();
           contral.mainId=mXmlScene.attribute(tr("scenemainId")+QString::number(j)).toInt();
           contral.devId=mXmlScene.attribute(tr("scenedevId")+QString::number(j)).toInt();
           contral.startAddr=mXmlScene.attribute(tr("scenestartAddr")+QString::number(j)).toInt();
           //contral.writeNum=mXmlScene.attribute(tr("scenewriteNum")+QString::number(j)).toDouble();
           timerList.append(contral);
        }

        QDomElement mXmlBackColor = mXmlScene.firstChild().toElement();

        if(mXmlBackColor.isNull())
        {
            mXmlScene = mXmlScene.nextSibling().toElement();
            continue;
        }
        else
        {
            r = mXmlBackColor.attribute(tr("r")).toInt(&OkR, 10);
            g = mXmlBackColor.attribute(tr("g")).toInt(&OkG, 10);
            b = mXmlBackColor.attribute(tr("b")).toInt(&OkB, 10);
        }

        TScene *mScene = 0;

        if(OkId && OkPosX && OkPosY && OkWidth && OkHeight && OkR && OkG && OkB)
        {
            //qDebug() << "xml new Scene:" << PosX << PosY << Width << Height;
            mScene = new TScene(PosX, PosY, Width, Height);
            mScene->fSetId(Id);
            mScene->fSetName(Name);
            mScene->TImerStructList=timerList;

            mScene->fSetBackColor(r, g, b);
            //mScene->setBackgroundBrush(mScene->fGetBackColor());

            if(Id == mHostId)
            {
                // 保证主视图是第一个
                mSceneHost = mScene;
                SceneList.push_front(mScene);
            }
            else
            {
                SceneList.push_back(mScene);
            }

            // item
            QDomElement mXmlNode = mXmlBackColor.nextSibling().toElement();

            while(!mXmlNode.isNull())
            {

                TItem *Item = fAnalyItem(mXmlNode);

                if(Item)
                {
                    mScene->addItem(Item);
                }

                mXmlNode = mXmlNode.nextSibling().toElement();
            }

        }

        mXmlScene = mXmlScene.nextSibling().toElement();
    }

    return mSceneHost;

}

bool TXmlReader::fAnalyLinkage(QList<TLinkage *> &LinkageList)
{
    QDomElement XmlLinkagesHead = fFindElement(tr("Linkages"));

    if(XmlLinkagesHead.isNull())
    {
        return false;
    }

    QDomElement XmlLinkage = XmlLinkagesHead.firstChild().toElement();

    while(!XmlLinkage.isNull())
    {
        TLinkage *Linkage = new TLinkage();
        Linkage->mSrcServiceId = XmlLinkage.attribute(tr("SrcServiceId")).toInt();
        Linkage->mSrcNodeId = XmlLinkage.attribute(tr("SrcNodeId")).toInt();

        Linkage->mDstServiceId = XmlLinkage.attribute(tr("DstServiceId")).toInt();
        Linkage->mDstNodeId = XmlLinkage.attribute(tr("DstNodeId")).toInt();

        LinkageList.push_back(Linkage);

        XmlLinkage = XmlLinkage.nextSibling().toElement();
    }

    return true;
}

bool TXmlReader::fAnalyService(QList<TService *> &ServiceList)
{
    QDomElement mXmlServices = fFindElement(tr("services"));

    if(mXmlServices.isNull())
    {
        return false;
    }

    QDomElement mXmlService = mXmlServices.firstChild().toElement();

    while(!mXmlService.isNull())
    {
        TService *Service = new TService();
        Service->fSetId(mXmlService.attribute(tr("Id")).toInt());
        Service->fSetName(mXmlService.attribute(tr("Name")));
        Service->fSetPort(mXmlService.attribute(tr("Port")).toInt());
        Service->fSetConnectMode(mXmlService.attribute(tr("ConnectMode")).toInt());
        Service->fSetRecvMode(mXmlService.attribute(tr("RecvMode")).toInt());
        Service->fSetAgreeMent(mXmlService.attribute(tr("AgreeMent")).toInt());

        switch(Service->fGetPort())
        {
        //网络
        case TService::Port_Net_Client_Tcp:
        case TService::Port_Net_Client_Udp:
        {
            TClientData *ClientData = Service->fGetClientData();
            if(ClientData)
            {
                ClientData->fSetIp(mXmlService.attribute(tr("NetIp")));
                ClientData->fSetPort(mXmlService.attribute(tr("NetPort")).toInt());
            }
        }
        break;

        case TService::Port_Net_Server_Tcp:
        case TService::Port_Net_Server_Udp:
        {
            TServerData *ServerData = Service->fGetServerData();
            if(ServerData)
            {
                ServerData->fSetIp(mXmlService.attribute(tr("NetIp")));
                ServerData->fSetPort(mXmlService.attribute(tr("NetPort")).toInt());
                ServerData->fSetIoThreadSum(mXmlService.attribute(tr("IoThreadSum")).toInt());
                ServerData->fSetProcessThreadSum(mXmlService.attribute(tr("ProcessThreadSum")).toInt());
            }
        }
        break;

        // 串口
        case TService::Port_Com:
        {
            TComData *ComData = Service->fGetComData();

            if(ComData)
            {
                //ComData->mMode = mXmlService.attribute(tr("ComMode")).toInt();
                ComData->fSetName(mXmlService.attribute(tr("ComName")));
                ComData->fSetRate(mXmlService.attribute(tr("ComRate")).toInt());
                ComData->fSetDataBits(mXmlService.attribute(tr("ComData")).toInt());
                ComData->fSetStopBits(mXmlService.attribute(tr("ComStop")).toInt());
                ComData->fSetCheck(mXmlService.attribute(tr("ComCheck")).toInt());
            }
        }
        break;

        default:
            break;
        }

        switch(Service->fGetAgreeMent())
        {
        case TService::AgreeMent_Iec104:
        {
            TIec104Data *Iec104Data = Service->fGetIec104Data();

            if(Iec104Data)
            {
                Iec104Data->mAddr = mXmlService.attribute(tr("Iec104Addr")).toInt();
                Iec104Data->mUnMaxSum = mXmlService.attribute(tr("Iec104UnMaxSum")).toInt();
                Iec104Data->mUnMaxTime = mXmlService.attribute(tr("Iec104UnMaxTime")).toInt();
                Iec104Data->mAppAddr = mXmlService.attribute(tr("Iec104AppAddr")).toInt();
                Iec104Data->mMsgAddr = mXmlService.attribute(tr("Iec104MsgAddr")).toInt();
                Iec104Data->mSendReason = mXmlService.attribute(tr("Iec104SendReason")).toInt();
                Iec104Data->mTimeLen = mXmlService.attribute(tr("Iec104TimeLen")).toInt();
            }
        }
        break;

        case TService::AgreeMent_Modebus_RTU:
        {
            TModebusRtuData *RtuData = Service->fGetModebusRtuData();

            if(RtuData)
            {
                QDomElement XmlHost = mXmlService.firstChild().toElement();

                while(!XmlHost.isNull())
                {
                    RtuData->fAddHost(XmlHost.attribute(tr("Host")).toInt(),
                                      XmlHost.attribute(tr("Fun")).toInt(),
                                      XmlHost.attribute(tr("StartAddr")).toInt(),
                                      XmlHost.attribute(tr("RegCount")).toInt(),
                                      XmlHost.attribute(tr("DataLen")).toInt(),
                                      XmlHost.attribute(tr("Type")).toInt());
                    XmlHost = XmlHost.nextSibling().toElement();
                }
            }
        }

        default:
            break;
        }

        ServiceList.push_back(Service);

        mXmlService = mXmlService.nextSibling().toElement();
    }

    return true;
}
