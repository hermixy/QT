#include "DataBaseService.h"

#include "../Base/LibBase.h"
#include "../Graphics/GraphicsPub.h"
#include "../Widgets/WidgetsPub.h"
#include "../Item/ItemFactory.h"
#include "../DataBase/Access/Access.h"
#include "../DataBase/MySQL/MySQL.h"

#include <QDebug>

TDataBaseService::TDataBaseService()
{
}

TDataBaseService::~TDataBaseService()
{
}

bool TDataBaseService::fClear(TDataBase *DataBase)
{
    if(NULL == DataBase)
    {
        return false;
    }

    qDebug() << "TDataBaseService::fClear()";

    // 清空表
    DataBase->fDoSql(tr("DELETE FROM service"));

    DataBase->fDoSql(tr("DELETE FROM scene"));
    DataBase->fDoSql(tr("DELETE FROM item"));

    DataBase->fDoSql(tr("DELETE FROM graphics"));
    DataBase->fDoSql(tr("DELETE FROM js"));
    DataBase->fDoSql(tr("DELETE FROM arc"));
    DataBase->fDoSql(tr("DELETE FROM curve"));
    DataBase->fDoSql(tr("DELETE FROM meter"));
    DataBase->fDoSql(tr("DELETE FROM pixmap"));
    DataBase->fDoSql(tr("DELETE FROM polygon"));
    DataBase->fDoSql(tr("DELETE FROM processbar"));
    DataBase->fDoSql(tr("DELETE FROM text"));
    DataBase->fDoSql(tr("DELETE FROM value_text"));

    DataBase->fDoSql(tr("DELETE FROM widgets"));
    DataBase->fDoSql(tr("DELETE FROM input_edit"));
    DataBase->fDoSql(tr("DELETE FROM button"));
    DataBase->fDoSql(tr("DELETE FROM table"));
    DataBase->fDoSql(tr("DELETE FROM table_len"));
    DataBase->fDoSql(tr("DELETE FROM table_val"));

    DataBase->fDoSql(tr("DELETE FROM linkage"));

    return true;
}

bool TDataBaseService::fSaveService(TDataBase *DataBase, const QList<TService *>ServiceList)
{
    if(NULL == DataBase)
    {
        return false;
    }

    qDebug() << "TDataBaseService::fSaveService()";

    //service
    foreach(TService *Service, ServiceList)
    {
        switch(Service->fGetPort())
        {
        case TService::Port_Net_Client_Tcp:
        case TService::Port_Net_Client_Udp:
        {
            TClientData *ClientData = Service->fGetClientData();

            if(ClientData)
            {
                DataBase->fDoSql(tr("INSERT INTO service (id,name,port,agree_ment,net_ip,net_port) ")
                                  + tr("VALUES (%1,'%2',%3,%4,'%5',%6)")
                                  .arg(Service->fGetId())
                                  .arg(Service->fGetName())
                                  .arg(Service->fGetPort())
                                  .arg(Service->fGetAgreeMent())
                                  .arg(ClientData->fGetIp())
                                  .arg(ClientData->fGetPort())
                                  );
            }
        }
        break;

        case TService::Port_Net_Server_Tcp:
        case TService::Port_Net_Server_Udp:
        {
            TServerData *ServerData = Service->fGetServerData();

            if(ServerData)
            {
                DataBase->fDoSql(tr("INSERT INTO service (id,name,port,agree_ment,net_ip,net_port,server_io_sum,server_process_sum) ")
                                  + tr("VALUES (%1,'%2',%3,%4,'%5',%6,%7,%8)")
                                  .arg(Service->fGetId())
                                  .arg(Service->fGetName())
                                  .arg(Service->fGetPort())
                                  .arg(Service->fGetAgreeMent())
                                  .arg(ServerData->fGetIp())
                                  .arg(ServerData->fGetPort())
                                  .arg(ServerData->fGetIoThreadSum())
                                  .arg(ServerData->fGetProcessThreadSum())
                                  );
            }
        }
        break;

        case TService::Port_Com:
        {
            TComData *ComData = Service->fGetComData();

            if(ComData)
            {
                DataBase->fDoSql(tr("INSERT INTO service (id,name,port,agree_ment,com_name,com_rate,com_data,com_stop,com_check) ")
                                  + tr("VALUES (%1,'%2',%3,%4,'%5',%6,%7,%8,%9)")
                                  .arg(Service->fGetId())
                                  .arg(Service->fGetName())
                                  .arg(Service->fGetPort())
                                  .arg(Service->fGetAgreeMent())
                                  .arg(ComData->fGetName())
                                  .arg(ComData->fGetRate())
                                  .arg(ComData->fGetDataBits())
                                  .arg(ComData->fGetStopBits())
                                  .arg(ComData->fGetCheck())
                                  );
            }
        }
        break;

        default:
        break;
        }
    }

    return true;
}

bool TDataBaseService::fSaveScene(TDataBase *DataBase, const QList<TScene *>SceneList)
{
    if(NULL == DataBase)
    {
        return false;
    }

    qDebug() << "TDataBaseService::fSaveScene()";

    int ItemIdIndex = 1;

    foreach(TScene* Scene, SceneList)
    {
        DataBase->fDoSql(tr("INSERT INTO scene (id,host,name,pos_x,pos_y,width,height,color_r,color_g,color_b) ")
                          + tr("VALUES (%1,%2,'%3',%4,%5,%6,%7,%8,%9,")
                          .arg(Scene->fGetId())
                          .arg(BoolToInt(Scene == SceneList.first()))
                          .arg(Scene->fGetName())
                          .arg(Scene->fGetPosX())
                          .arg(Scene->fGetPosY())
                          .arg(Scene->fGetWidth())
                          .arg(Scene->fGetHeight())
                          .arg(Scene->fGetBackColor().red())
                          .arg(Scene->fGetBackColor().green())
                          + tr("%1)").arg(Scene->fGetBackColor().blue()));

        // 插入该Scene中所有Item
        QList<TItemEx *> ItemList;
        Scene->fGetItemsList(ItemList);

        foreach(TItemEx *Item, ItemList)
        {
            //qDebug() << "insert itemex " << ItemIdIndex;
            fSaveItem(DataBase, Scene->fGetId(), Item, ItemIdIndex++);
        }
    }

    return true;
}

bool TDataBaseService::fSaveItem(TDataBase *DataBase, int SceneId, TItemEx *Item, int ItemIdIndex, TGroup *GroupParent, int GroupId)
{
    if(NULL == DataBase || NULL == Item)
    {
        return false;
    }

    double StartX = 0;
    double StartY = 0;
    double StopX = 0;
    double StopY = 0;

    if(GroupParent)
    {
        // Group中的图形，进行坐标变换
        StartX = GroupParent->fGetItemStartPos(Item).x();
        StartY = GroupParent->fGetItemStartPos(Item).y();
        StopX = GroupParent->fGetItemStopPos(Item).x();
        StopY = GroupParent->fGetItemStopPos(Item).y();
    }
    else
    {
        StartX = Item->fGetStartX();
        StartY = Item->fGetStartY();
        StopX = Item->fGetStopX();
        StopY = Item->fGetStopY();
    }

    DataBase->fDoSql(tr("INSERT INTO item (scene_id,item_id,type,start_x,start_y,stop_x,stop_y,line_width,line_style")
                      + tr(",line_color_r,line_color_g,line_color_b,line_color_a,back_color_r,back_color_g,back_color_b,back_color_a,z")
                      + tr(",rotate_angle,rotate_center_x,rotate_center_y,move_able,select_able,drag_able,group_id,name_id,service_id")
                      + tr(",hint,user_data_name_1,user_data_1,user_data_name_2,user_data_2) ")
                      + tr("VALUES (%1,%2,%3,%4,%5,%6,%7,%8,%9,")
                      .arg(SceneId).arg(ItemIdIndex).arg(Item->fGetType())
                      .arg(StartX).arg(StartY).arg(StopX).arg(StopY)
                      .arg(Item->fGetLineWidth()).arg(Item->fGetLineStyle())
                      + tr("%1,%2,%3,%4,%5,%6,%7,%8,%9,")
                      .arg(Item->fGetLineColor().red())
                      .arg(Item->fGetLineColor().green())
                      .arg(Item->fGetLineColor().blue())
                      .arg(Item->fGetLineColor().alpha())
                      .arg(Item->fGetBackGroundColor().red())
                      .arg(Item->fGetBackGroundColor().green())
                      .arg(Item->fGetBackGroundColor().blue())
                      .arg(Item->fGetBackGroundColor().alpha())
                      .arg(Item->fGetZValue())
                      + tr("%1,%2,%3,%4,%5,%6,%7,'%8',%9,")
                      .arg(Item->fGetRotateAngle())
                      .arg(Item->fGetRotateCenterX())
                      .arg(Item->fGetRotateCenterY())
                      .arg(BoolToInt(Item->fGetMoveAble()))
                      .arg(BoolToInt(Item->fGetSelectAble()))
                      .arg(BoolToInt(Item->fGetDragAble()))
                      .arg(GroupId)
                      .arg(Item->fGetNameId())
                      .arg(Item->fGetServiceId())
                      + tr("'%1','%2',%3,'%4',%5)")
                      .arg(Item->fGetHint())
                      .arg(Item->fGetUserDataName1())
                      .arg(Item->fGetUserData1())
                      .arg(Item->fGetUserDataName2())
                      .arg(Item->fGetUserData2())
                      );



    TGraphics *Graphics = dynamic_cast<TGraphics *>(Item);

    if(Graphics)
    {
        DataBase->fDoSql(tr("INSERT INTO graphics (item_id,link_scene,node_id,value,coe,auto_type,auto_time) ")
                          + tr("VALUES (%1,%2,%3,%4,%5,%6,%7)")
                            .arg(ItemIdIndex)
                            .arg(Graphics->fGetLinkScene())
                            .arg(Graphics->fGetNodeId())
                            .arg(Graphics->fGetValue())
                            .arg(Graphics->fGetCoe())
                            .arg(Graphics->fGetAutoType())
                            .arg(Graphics->fGetAutoTime())
                          );


    }

    TWidgets *Widgets = dynamic_cast<TWidgets *>(Item);

    if(Widgets)
    {
        DataBase->fDoSql(tr("INSERT INTO widgets (item_id,js_name) ")
                          + tr("VALUES (%1,'%2')")
                          .arg(ItemIdIndex)
                          .arg(Widgets->mJsWidgets.fGetName())
                          );
    }

    switch(Item->fGetType())
    {
    case TItem::Group:
    {
        TGroup *Group = dynamic_cast<TGroup *>(Item);
        // 添加的是Group
        if(Group)
        {
            foreach(TItem *ItemInGroup, Group->fGetItemsList())
            {
                TItemEx *ItemExInGroup = dynamic_cast<TItemEx *>(ItemInGroup);

                if(ItemExInGroup)
                {
                    fSaveItem(DataBase, SceneId, ItemExInGroup, 0, Group, ItemIdIndex);
                }
            }
        }
    }
    break;

    case TItem::Point:
    case TItem::Line:
    case TItem::Ellipse:
    case TItem::Rect:
    case TItem::Light:
    {
    }
    break;

    case TItem::Polygon:
    {
        TPolygon *Polygon = dynamic_cast<TPolygon *>(Item);

        if(Polygon)
        {
            QPolygonF PolygonF =  Polygon->fGetPolygonPoints();

            for(int i = 0; i < PolygonF.count(); ++i)
            {
                DataBase->fDoSql(tr("INSERT INTO polygon (item_id,point_id,x,y) ")
                                  + tr("VALUES (%1,%2,%3,%4)")
                                  .arg(ItemIdIndex)
                                  .arg(i)
                                  .arg(PolygonF.at(i).x())
                                  .arg(PolygonF.at(i).y())
                                  );
            }
        }
    }
    break;

    case TItem::Arc:
    {
        TArc *Arc = dynamic_cast<TArc *>(Item);
        if(Arc)
        {
            DataBase->fDoSql(tr("INSERT INTO arc (item_id,mode,start_angle,cover_angle) ")
                              + tr("VALUES (%1,%2,%3,%4)")
                              .arg(ItemIdIndex)
                              .arg(Arc->fGetMode())
                              .arg(Arc->fGetStartAngle())
                              .arg(Arc->fGetCoverAngle())
                              );
        }
    }
    break;

    case TItem::Text:
    {
        TText *Text = dynamic_cast<TText *>(Item);
        if(Text)
        {
            DataBase->fDoSql(tr("INSERT INTO text (item_id,text,family,size,blod,italic,spacing,under_line,strike_out) ")
                              + tr("VALUES (%1,'%2','%3',%4,%5,%6,%7,%8,%9)")
                                .arg(ItemIdIndex)
                                .arg(Text->fGetText())
                                .arg(Text->fGetFontFamily())
                                .arg(Text->fGetFontSize())
                                .arg(BoolToInt(Text->fGetFontBlod()))
                                .arg(BoolToInt(Text->fGetFontItalic()))
                                .arg(Text->fGetFontSpacing())
                                .arg(BoolToInt(Text->fGetFontUnderLine()))
                                .arg(BoolToInt(Text->fGetFontStrikeOut()))
            );
        }
    }
    break;

    case TItem::ValueText:
    {
        TValueText *ValueText = dynamic_cast<TValueText *>(Item);

        if(ValueText)
        {
            DataBase->fDoSql(tr("INSERT INTO text (item_id,text,family,size,blod,italic,spacing,under_line,strike_out) ")
                              + tr("VALUES (%1,'%2','%3',%4,%5,%6,%7,%8,%9)")
                                .arg(ItemIdIndex)
                                .arg(ValueText->fGetText())
                                .arg(ValueText->fGetFontFamily())
                                .arg(ValueText->fGetFontSize())
                                .arg(BoolToInt(ValueText->fGetFontBlod()))
                                .arg(BoolToInt(ValueText->fGetFontItalic()))
                                .arg(ValueText->fGetFontSpacing())
                                .arg(BoolToInt(ValueText->fGetFontUnderLine()))
                                .arg(BoolToInt(ValueText->fGetFontStrikeOut()))
            );

            DataBase->fDoSql(tr("INSERT INTO value_text (item_id,mode,int_len,point_len) ")
                              + tr("VALUES (%1,%2,%3,%4)")
                                .arg(ItemIdIndex)
                                .arg(ValueText->fGetMode())
                                .arg(ValueText->fGetIntLen())
                                .arg(ValueText->fGetPointLen())
                                );
        }
    }
    break;

    case TItem::Pixmap:
    {
        TPixmap *Pixmap = dynamic_cast<TPixmap *>(Item);
        if(Pixmap)
        {
            DataBase->fDoSql(tr("INSERT INTO pixmap (item_id,dir) ")
                              + tr("VALUES (%1,'%2')")
                              .arg(ItemIdIndex)
                              .arg(Pixmap->fGetFileDir())
                              );
        }
    }
    break;

    case TItem::Curve:
    {
        TCurve *Curve = dynamic_cast<TCurve *>(Item);
        if(Curve)
        {
            DataBase->fDoSql(tr("INSERT INTO curve (item_id,data_sum,interval,time) ")
                              + tr("VALUES (%1,%2,%3,%4)")
                              .arg(ItemIdIndex)
                              .arg(Curve->fGetDataSum())
                              .arg(Curve->fGetInterval())
                              .arg(Curve->fGetTime())
                              );
        }
    }
    break;

    case TItem::Meter:
    {
        TMeter *Meter = dynamic_cast<TMeter *>(Item);
        if(Meter)
        {
            DataBase->fDoSql(tr("INSERT INTO meter (item_id,mode,scale_sum,start_angle,step_angle,start_value,step_value,pointer_color_r,pointer_color_g,pointer_color_b) ")
                              + tr("VALUES (%1,%2,%3,%4,%5,%6,%7,%8,%9")
                              .arg(ItemIdIndex)
                              .arg(Meter->fGetMode())
                              .arg(Meter->fGetScaleSum())
                              .arg(Meter->fGetStartAngle())
                              .arg(Meter->fGetStepAngle())
                              .arg(Meter->fGetStartValue())
                              .arg(Meter->fGetStepValue())
                              .arg(Meter->fGetPointerColor().red())
                              .arg(Meter->fGetPointerColor().green())
                              + tr(",%1)")
                              .arg(Meter->fGetPointerColor().blue())
                              );
        }
    }
    break;

    case TItem::ProcessBar:
    {
        TProcessBar *ProcessBar = dynamic_cast<TProcessBar *>(Item);
        if(ProcessBar)
        {
            DataBase->fDoSql(tr("INSERT INTO processbar (item_id,mode,scale_sum,start_value,step_value,value_color_r,value_color_g,value_color_b) ")
                              + tr("VALUES (%1,%2,%3,%4,%5,%6,%7,%8)")
                              .arg(ItemIdIndex)
                              .arg(ProcessBar->fGetMode())
                              .arg(ProcessBar->fGetScaleSum())
                              .arg(ProcessBar->fGetStartValue())
                              .arg(ProcessBar->fGetStepValue())
                              .arg(ProcessBar->fGetValueColor().red())
                              .arg(ProcessBar->fGetValueColor().green())
                              .arg(ProcessBar->fGetValueColor().blue())
                              );
        }
    }
    break;

    //
    case TItem::Button:
    {
        TButton *Button = dynamic_cast<TButton *>(Item);

        if(Button)
        {
            DataBase->fDoSql(tr("INSERT INTO button (item_id,text) ")
                              + tr("VALUES (%1,'%2')")
                              .arg(ItemIdIndex)
                              .arg(Button->fGetText())
                              );
        }
    }
    break;

    case TItem::InputEdit:
    {
        TInputEdit *InputEdit = dynamic_cast<TInputEdit *>(Item);

        if(InputEdit)
        {
            DataBase->fDoSql(tr("INSERT INTO input_edit (item_id,text) ")
                              + tr("VALUES (%1,'%2')")
                              .arg(ItemIdIndex)
                              .arg(InputEdit->fGetText())
                              );
        }
    }
    break;

    case TItem::Table:
    {
        TTable *Table = dynamic_cast<TTable *>(Item);

        if(Table)
        {
            DataBase->fDoSql(tr("INSERT INTO table_base (item_id,col_sum,row_sum) ")
                              + tr("VALUES (%1,%2,%3)")
                              .arg(ItemIdIndex)
                              .arg(Table->fGetColSum())
                              .arg(Table->fGetRowSum())
                              );

            // 插入行高
            for(int i = 0; i < Table->fGetRowSum(); i++)
            {
                DataBase->fDoSql(tr("INSERT INTO table_len (item_id,type,num,length) ")
                                  + tr("VALUES (%1,%2,%3,%4)")
                                  .arg(ItemIdIndex)
                                  .arg(1)
                                  .arg(i)
                                  .arg(Table->fGetRowLength())
                                  );
            }

            // 插入列宽
            for(int j = 0; j < Table->fGetColSum(); j++)
            {
                DataBase->fDoSql(tr("INSERT INTO table_len (item_id,type,num,length) ")
                                  + tr("VALUES (%1,%2,%3,%4)")
                                  .arg(ItemIdIndex)
                                  .arg(2)
                                  .arg(j)
                                  .arg(Table->fGetColLength(j))
                                  );
            }

            // 插入单元格值
            for(int i = 0; i < Table->fGetRowSum(); i++)
            {
                for(int j = 0; j < Table->fGetColSum(); j++)
                {
                    DataBase->fDoSql(tr("INSERT INTO table_val (item_id,row,col,value) ")
                                      + tr("VALUES (%1,%2,%3,'%4')")
                                      .arg(ItemIdIndex)
                                      .arg(i)
                                      .arg(j)
                                      .arg(Table->fGetCell(i, j))
                                      );
                }
            }
        }
    }
    break;

    default:
    break;
    }

    return true;
}

bool TDataBaseService::fSaveLinkage(TDataBase *DataBase, const QList<TLinkage *> LinkageList)
{
    if(NULL == DataBase)
    {
        return false;
    }

    // 联动信息
    foreach(TLinkage *Linkage, LinkageList)
    {
        DataBase->fDoSql(tr("INSERT INTO linkage (src_service_id,src_node_id,dst_service_id,dst_node_id) ")
                          + tr("VALUES (%1,%2,%3,%4)")
                          .arg(Linkage->mSrcServiceId)
                          .arg(Linkage->mSrcNodeId)
                          .arg(Linkage->mDstServiceId)
                          .arg(Linkage->mDstNodeId)
                          );
    }

    return true;
}

bool TDataBaseService::fLoadService(TDataBase *DataBase, QList<TService *> &ServiceList)
{
    if(NULL == DataBase)
    {
        return false;
    }

    // 解析服务
    QList< QList<QVariant> > ResultServices = DataBase->fDoSqlResult(tr("SELECT id,name,port,agree_ment,net_ip,net_port,server_io_sum,server_process_sum,com_name,com_rate,com_data,com_stop,com_check FROM service"));

    qDebug() << "ResultServices.length()" << ResultServices.length();

    foreach(QList<QVariant> SqlService, ResultServices)
    {
        if(SqlService.length() < 13)
        {
            continue;
        }

        TService *Service = new TService();

        if(NULL == Service)
        {
            continue;
        }

        Service->fSetId(SqlService.at(0).toInt());
        Service->fSetName(SqlService.at(1).toString());
        Service->fSetPort(SqlService.at(2).toInt());
        Service->fSetAgreeMent(SqlService.at(3).toInt());

        switch(Service->fGetPort())
        {
        //网络
        case TService::Port_Net_Client_Tcp:
        case TService::Port_Net_Client_Udp:
        {
            TClientData *ClientData = Service->fGetClientData();

            if(ClientData)
            {
                ClientData->fSetIp(SqlService.at(4).toString());
                ClientData->fSetPort(SqlService.at(5).toInt());
            }
        }
        break;

        case TService::Port_Net_Server_Tcp:
        case TService::Port_Net_Server_Udp:
        {
            TServerData *ServerData = Service->fGetServerData();

            if(ServerData)
            {
                ServerData->fSetIp(SqlService.at(4).toString());
                ServerData->fSetPort(SqlService.at(5).toInt());
                ServerData->fSetIoThreadSum(SqlService.at(6).toInt());
                ServerData->fSetProcessThreadSum(SqlService.at(7).toInt());
            }
        }
        break;

        // 串口
        case TService::Port_Com:
        {
            TComData *ComData = Service->fGetComData();

            if(ComData)
            {
                ComData->fSetName(SqlService.at(8).toString());
                ComData->fSetRate(SqlService.at(9).toInt());
                ComData->fSetDataBits(SqlService.at(10).toInt());
                ComData->fSetStopBits(SqlService.at(11).toInt());
                ComData->fSetCheck(SqlService.at(12).toInt());
            }
        }
        break;

        default:
            break;
        }

        ServiceList.push_back(Service);
    }

    return true;
}

TItem* TDataBaseService::fCreateItem(TDataBase *DataBase, int SceneId, const QList<QVariant>& SqlItem)
{
    if(SqlItem.count() < 30)
    {
        return NULL;
    }

    TItem* Item = NULL;

    int ItemId = SqlItem.at(0).toInt();
    int Type = SqlItem.at(1).toInt();
    double StartX = SqlItem.at(2).toDouble();
    double StartY = SqlItem.at(3).toDouble();
    double StopX = SqlItem.at(4).toDouble();
    double StopY = SqlItem.at(5).toDouble();
    int LineWidth = SqlItem.at(6).toInt();
    int LineStyle = SqlItem.at(7).toInt();

    QColor ItemLineColor(SqlItem.at(8).toInt(), SqlItem.at(9).toInt(), SqlItem.at(10).toInt(), SqlItem.at(11).toInt());
    QColor ItemBackColor(SqlItem.at(12).toInt(), SqlItem.at(13).toInt(), SqlItem.at(14).toInt(), SqlItem.at(15).toInt());

    Item = TItemFactory::fFactory(Type, StartX, StartY, StopX, StopY, Qt::PenStyle(LineStyle),
                                  LineWidth, ItemLineColor, ItemBackColor);

    switch(Type)
    {
    case TItem::Group:
    {
        TGroup *Group = dynamic_cast<TGroup *>(Item);

        if(Group)
        {
            QList<TItem *> ItemList = fLoadItems(DataBase, SceneId, ItemId);

            Group->fAddItems(ItemList);

            foreach(TItem *ItemInGroup, ItemList)
            {
                ItemList.removeOne(ItemInGroup);
                delete ItemInGroup;
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
            QList< QList<QVariant> > ResultPolygons = DataBase->fDoSqlResult(tr("SELECT x,y FROM polygon WHERE item_id = %1 ORDER BY point_id").arg(ItemId));

            foreach(const QList<QVariant>& SqlPolygon, ResultPolygons)
            {
                if(SqlPolygon.length() < 2)
                {
                    continue;
                }

                PolygonF.push_back(QPointF(SqlPolygon.at(0).toDouble(), SqlPolygon.at(1).toDouble()));
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
            QList<QVariant> SqlArc = DataBase->fDoSqlOneResult(tr("SELECT mode,start_angle,cover_angle FROM arc WHERE item_id = %1").arg(ItemId));

            Arc->fSetMode(SqlArc.at(0).toInt());
            Arc->fSetStartAngle(SqlArc.at(1).toDouble());
            Arc->fSetCoverAngle(SqlArc.at(2).toDouble());
        }
    }
    break;

    case TItem::Text:
    {
        TText *Text = dynamic_cast<TText *>(Item);

        if(Text)
        {
            QList<QVariant> SqlText = DataBase->fDoSqlOneResult(tr("SELECT text,family,size,blod,italic,spacing,under_line,strike_out FROM text WHERE item_id = %1").arg(ItemId));

            if(SqlText.length() >= 8)
            {
                Text->fSetText(SqlText.at(0).toString());
                Text->fSetFontFamily(SqlText.at(1).toString());
                Text->fSetFontSize(SqlText.at(2).toInt());
                Text->fSetFontBlod(IntToBool(SqlText.at(3).toInt()));
                Text->fSetFontItalic(IntToBool(SqlText.at(4).toInt()));
                Text->fSetFontSpacing(SqlText.at(5).toInt());
                Text->fSetFontUnderLine(IntToBool(SqlText.at(6).toInt()));
                Text->fSetFontStrikeOut(IntToBool(SqlText.at(7).toInt()));
            }
        }
    }
    break;

    case TItem::ValueText:
    {
        TValueText *ValueText = dynamic_cast<TValueText *>(Item);

        if(ValueText)
        {
            QList<QVariant> SqlValueText = DataBase->fDoSqlOneResult(tr("SELECT mode,int_len,point_len FROM value_text WHERE item_id = %1").arg(ItemId));

            if(SqlValueText.length() >= 3)
            {
                ValueText->fSetMode(SqlValueText.at(0).toInt());
                ValueText->fSetIntLen(SqlValueText.at(1).toInt());
                ValueText->fSetPointLen(SqlValueText.at(2).toInt());

                QList<QVariant> SqlText = DataBase->fDoSqlOneResult(tr("SELECT text,family,size,blod,italic,spacing,under_line,strike_out FROM text WHERE item_id = %1").arg(ItemId));

                if(SqlText.length() >= 8)
                {
                    ValueText->fSetFontFamily(SqlText.at(1).toString());
                    ValueText->fSetFontSize(SqlText.at(2).toInt());
                    ValueText->fSetFontBlod(IntToBool(SqlText.at(3).toInt()));
                    ValueText->fSetFontItalic(IntToBool(SqlText.at(4).toInt()));
                    ValueText->fSetFontSpacing(SqlText.at(5).toInt());
                    ValueText->fSetFontUnderLine(IntToBool(SqlText.at(6).toInt()));
                    ValueText->fSetFontStrikeOut(IntToBool(SqlText.at(7).toInt()));
                }
            }
        }
    }
    break;

    case TItem::Pixmap:
    {
        TPixmap *Pixmap = dynamic_cast<TPixmap *>(Item);

        if(Pixmap)
        {
            QList<QVariant> SqlPixmap = DataBase->fDoSqlOneResult(tr("SELECT dir FROM pixmap WHERE item_id = %1").arg(ItemId));

            if(SqlPixmap.length() >= 1)
            {
                Pixmap->fSetFileDir(SqlPixmap.at(0).toString());
            }
        }
    }
    break;

    case TItem::Curve:
    {
        TCurve *Curve = dynamic_cast<TCurve *>(Item);

        if(Curve)
        {
            QList<QVariant> SqlCurve = DataBase->fDoSqlOneResult(tr("SELECT data_sum,interval,time FROM curve WHERE item_id = %1").arg(ItemId));

            if(SqlCurve.length() >= 3)
            {
                Curve->fSetDataSum(SqlCurve.at(0).toInt());
                Curve->fSetInterval(SqlCurve.at(1).toInt());
                Curve->fSetTime(SqlCurve.at(2).toInt());
            }
        }
    }
    break;

    case TItem::Meter:
    {
        TMeter *Meter = dynamic_cast<TMeter *>(Item);

        if(Meter)
        {
            QList<QVariant> SqlMeter = DataBase->fDoSqlOneResult(tr("SELECT mode,scale_sum,start_angle,step_angle,start_value,step_value,pointer_color_r,pointer_color_g,pointer_color_b FROM meter WHERE item_id = %1").arg(ItemId));

            if(SqlMeter.length() >= 9)
            {
                Meter->fSetMode(SqlMeter.at(0).toInt());
                Meter->fSetScaleSum(SqlMeter.at(1).toInt());
                Meter->fSetStartAngle(SqlMeter.at(2).toDouble());
                Meter->fSetStepAngle(SqlMeter.at(3).toDouble());
                Meter->fSetStartValue(SqlMeter.at(4).toDouble());
                Meter->fSetStepValue(SqlMeter.at(5).toDouble());
                Meter->fSetPointerColor(QColor(SqlMeter.at(6).toInt(), SqlMeter.at(7).toInt(), SqlMeter.at(8).toInt()));
            }
        }
    }
    break;

    case TItem::ProcessBar:
    {
        TProcessBar *ProcessBar = dynamic_cast<TProcessBar *>(Item);

        if(ProcessBar)
        {
            QList<QVariant> SqlProcessBar = DataBase->fDoSqlOneResult(tr("SELECT mode,scale_sum,start_value,step_value,value_color_r,value_color_g,value_color_b FROM processbar WHERE item_id = %1").arg(ItemId));

            if(SqlProcessBar.length() >= 7)
            {
                ProcessBar->fSetMode(SqlProcessBar.at(0).toInt());
                ProcessBar->fSetScaleSum(SqlProcessBar.at(1).toInt());
                ProcessBar->fSetStartValue(SqlProcessBar.at(2).toDouble());
                ProcessBar->fSetStepValue(SqlProcessBar.at(3).toDouble());
                ProcessBar->fSetValueColor(QColor(SqlProcessBar.at(4).toInt(), SqlProcessBar.at(5).toInt(), SqlProcessBar.at(6).toInt()));
            }
        }
    }
    break;

    case TItem::Table:
    {
        TTable *Table = dynamic_cast<TTable *>(Item);

        if(Table)
        {
            QList<QVariant> SqlTable = DataBase->fDoSqlOneResult(tr("SELECT col_sum,row_sum FROM table_base WHERE item_id = %1").arg(ItemId));

            if(SqlTable.length() >= 2)
            {
                Table->fSetSize(SqlTable.at(0).toInt(), SqlTable.at(1).toInt());

                QList< QList<QVariant> > ResultTableLen = DataBase->fDoSqlResult(tr("SELECT type,num,length FROM table_len WHERE item_id = %1").arg(ItemId));

                foreach(const QList<QVariant> &SqlTableLen, ResultTableLen)
                {
                    if(SqlTableLen.length() >= 3)
                    {
                        int Type = SqlTableLen.at(0).toInt();

                        if(1 == Type)
                        {
                            Table->fSetRowLength(SqlTableLen.at(2).toInt());
                        }
                        else if(2 == Type)
                        {
                            Table->fSetColLength(SqlTableLen.at(1).toInt(), SqlTableLen.at(2).toInt());
                        }
                    }
                }

                // 数据
                QList< QList<QVariant> > ResultTableData = DataBase->fDoSqlResult(tr("SELECT row,col,value FROM table_val WHERE item_id = %1").arg(ItemId));

                foreach(const QList<QVariant> &SqlTableData, ResultTableData)
                {
                    if(SqlTableData.length() >= 3)
                    {
                        Table->fSetCell(SqlTableData.at(0).toInt(), SqlTableData.at(1).toInt(), SqlTableData.at(2).toString());
                    }
                }
            }
        }
    }
    break;

    case TItem::Button:
    {
        TButton *Button = dynamic_cast<TButton *>(Item);

        if(Button)
        {
            QList<QVariant> SqlButton = DataBase->fDoSqlOneResult(tr("SELECT text FROM button WHERE item_id = %1").arg(ItemId));

            if(SqlButton.length() >= 1)
            {
                Button->fSetText(SqlButton.at(0).toString());
            }
        }
    }
    break;

    case TItem::InputEdit:
    {
        TInputEdit *Input = dynamic_cast<TInputEdit *>(Item);

        if(Input)
        {
            QList<QVariant> SqlInputText = DataBase->fDoSqlOneResult(tr("SELECT text FROM input_edit WHERE item_id = %1").arg(ItemId));

            if(SqlInputText.length() >= 1)
            {
                Input->fSetText(SqlInputText.at(0).toString());
            }
        }
    }
    break;

    default:
        break;
    } // end of switch(type)

    if(Item)
    {
        Item->fSetZValue(SqlItem.at(16).toDouble());
        Item->fSetRotateAngle(SqlItem.at(17).toDouble());
        Item->fSetRotateCenter(SqlItem.at(18).toDouble(), SqlItem.at(19).toDouble());
        Item->fSetMoveAble(IntToBool(SqlItem.at(20).toInt()));
        Item->fSetSelectAble(IntToBool(SqlItem.at(21).toInt()));
        Item->fSetDragAble(IntToBool(SqlItem.at(22).toInt()));
    }

    TItemEx *ItemEx = dynamic_cast<TItemEx *>(Item);

    if(ItemEx)
    {
        ItemEx->fSetNameId(SqlItem.at(23).toString());
        ItemEx->fSetServiceId(SqlItem.at(24).toInt());
        ItemEx->fSetHint(SqlItem.at(25).toString());
        ItemEx->fSetUserDataName1(SqlItem.at(26).toString());
        ItemEx->fSetUserData1(SqlItem.at(27).toDouble());
        ItemEx->fSetUserDataName2(SqlItem.at(28).toString());
        ItemEx->fSetUserData2(SqlItem.at(29).toDouble());
    }

    TGraphics *Graphics = dynamic_cast<TGraphics *>(Item);

    if(Graphics)
    {
        QList<QVariant> SqlGraphics = DataBase->fDoSqlOneResult(tr("SELECT link_scene,node_id,value,coe,auto_type,auto_time FROM graphics WHERE item_id = %1").arg(ItemId));

        if(SqlGraphics.length() >= 6)
        {
            Graphics->fSetLinkScene(SqlGraphics.at(0).toInt());
            Graphics->fSetNodeId(SqlGraphics.at(1).toInt());
            Graphics->fSetValue(SqlGraphics.at(2).toDouble());
            Graphics->fSetCoe(SqlGraphics.at(3).toDouble());
            Graphics->fSetAutoType(SqlGraphics.at(4).toInt());
            Graphics->fSetAutoTime(SqlGraphics.at(5).toInt());

            QList< QList<QVariant> >ResultJs = DataBase->fDoSqlResult(tr("SELECT link_scene,node_id,value,coe,auto_type,auto_time FROM graphics WHERE item_id = %1").arg(ItemId));



        }
    }

    TWidgets *Widgets = dynamic_cast<TWidgets *>(Item);

    if(Widgets)
    {
        QList<QVariant> SqlWidgets = DataBase->fDoSqlOneResult(tr("SELECT js_name FROM widgets WHERE item_id = %1").arg(ItemId));

        if(SqlWidgets.length() >= 1)
        {
            Widgets->mJsWidgets.fSetName(SqlWidgets.at(0).toString());
        }
    }

    return Item;
}

QList<TItem *> TDataBaseService::fLoadItems(TDataBase *DataBase, int SceneId, int GroupId)
{
    QList<TItem *>ItemList;

    QList< QList<QVariant> > ResultItems = DataBase->fDoSqlResult(tr("SELECT item_id,type,start_x,start_y,stop_x,stop_y,line_width,line_style,line_color_r,line_color_g,line_color_b,line_color_a,back_color_r,back_color_g,back_color_b,back_color_a,z")
                                                                   + tr(",rotate_angle,rotate_center_x,rotate_center_y,move_able,select_able,drag_able,name_id,service_id,hint,user_data_name_1,user_data_1,user_data_name_2,user_data_2")
                                                                   + tr(" FROM item WHERE scene_id = %1 AND group_id = %2").arg(SceneId).arg(GroupId));

    foreach(const QList<QVariant>& SqlItem, ResultItems)
    {
        TItem* Item = fCreateItem(DataBase, SceneId, SqlItem);

        //qDebug() << "TDataBaseService::fLoadItems" << (void *)Item;

        if(Item)
        {
            ItemList.push_back(Item);
        }
    }

    return ItemList;
}

bool TDataBaseService::fLoadScene(TDataBase *DataBase, QList<TScene *> &SceneList)
{
    if(NULL == DataBase)
    {
        return false;
    }

    //解析界面图形
    QList< QList<QVariant> > ResultScenes = DataBase->fDoSqlResult(tr("SELECT id,host,name,pos_x,pos_y,width,height,color_r,color_g,color_b FROM scene"));

    //qDebug() << "ResultScenes.length()" << ResultScenes.length();

    if(ResultScenes.length() < 1)
    {
        return false;
    }

    foreach(const QList<QVariant>& SqlScene, ResultScenes)
    {
        if(SqlScene.length() < 10)
        {
            continue;
        }

        TScene *Scene = new TScene(SqlScene.at(3).toDouble(),
                                   SqlScene.at(4).toDouble(),
                                   SqlScene.at(5).toDouble(),
                                   SqlScene.at(6).toDouble());

        if(NULL == Scene)
        {
            continue;
        }

        Scene->fSetId(SqlScene.at(0).toInt());
        Scene->fSetName(SqlScene.at(2).toString());

        Scene->fSetBackColor(SqlScene.at(7).toInt(),
                                   SqlScene.at(8).toInt(),
                                   SqlScene.at(9).toInt());

        //Scene->setBackgroundBrush(Scene->fGetBackColor());

        if(SqlScene.at(1).toInt() > 0)
        {
            // 保证主视图是第一个
            SceneList.push_front(Scene);
        }
        else
        {
            SceneList.push_back(Scene);
        }

        // 开始解析该scene中的item
        QList<TItem *> ItemList = fLoadItems(DataBase, Scene->fGetId());

        //qDebug() << "TDataBaseService::fLoadScene Item sum" << ItemList.length();

        foreach(TItem *Item, ItemList)
        {
            Scene->addItem(Item);
        }
    }

    return true;
}

bool TDataBaseService::fLoadLinkage(TDataBase *DataBase, QList<TLinkage *> &LinkageList)
{
    if(NULL == DataBase)
    {
        return false;
    }

    // 解析联动信息
    QList< QList<QVariant> > ResultLinkages = DataBase->fDoSqlResult(tr("SELECT src_service_id,src_node_id,dst_service_id,dst_node_id FROM linkage"));

    qDebug() << "ResultLinkages.length()" << ResultLinkages.length();

    foreach(QList<QVariant> SqlLinkage, ResultLinkages)
    {
        if(SqlLinkage.length() < 4)
        {
            continue;
        }

        TLinkage *Linkage = new TLinkage();

        if(NULL == Linkage)
        {
            continue;
        }

        Linkage->mSrcServiceId = SqlLinkage.at(0).toInt();
        Linkage->mSrcNodeId = SqlLinkage.at(1).toInt();
        Linkage->mDstServiceId = SqlLinkage.at(2).toInt();
        Linkage->mDstNodeId = SqlLinkage.at(3).toInt();

        LinkageList.push_back(Linkage);
    }

    return true;
}

TDataBase* TDataBaseService::fCreateDataBase(TDataBaseMsg *DataBaseMsg)
{
    if(NULL == DataBaseMsg)
    {
        return NULL;
    }

    qDebug() << "DBMsg->mType" << DataBaseMsg->fGetType();

    TDataBase *DataBase = NULL;

    switch(DataBaseMsg->fGetType())
    {
    // MySQL
    case 0:
    {
        DataBase = new TMySQL();
        //fInit(tr("localhost"), tr("znkg"), tr("root"), tr("root")))
    }
    break;

    // Access
    case 1:
    {
        DataBase = new TAccess();
        //fInit(tr("D:\\Qt\\Scada\\db\\access\\"), tr("scada.mdb"), tr(""), tr("")))
    }
    break;

    // Oracle
    case 2:
        break;

    // SQL Server
    case 3:
        break;

    default:
        break;
    }

    return DataBase;
}
