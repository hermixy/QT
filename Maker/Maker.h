#ifndef MAKER_H
#define MAKER_H

#include "../Scada/Src/Item/Item.h"
#include "../Scada/Src/Graphics/GraphicsPub.h"
#include "../Scada/Src/Widgets/WidgetsPub.h"
#include "../Scada/Src/Action/Action.h"
#include "../Scada/Src/View/View.h"
#include "../Scada/Src/Scene/Scene.h"
#include "../Scada/Src/Linkage/Linkage.h"

//#include "../Scada/Src/RunMode/RunMode.h"
#include "../Scada/Src/Service/Service.h"
#include "../Scada/Src/DataBase/DataBasePub.h"
#include "../Scada/Src/Base/Base.h"
#include <QHash>

#include <QUndoStack>

#include <QMainWindow>

class QDomElement;
class QDomDocument;

class Maker : public QMainWindow
{
    Q_OBJECT

public:
    Maker(QWidget *parent = 0);
    static Maker *maker;
    void fSetCurrentScene(TScene *scene);

private slots:
    void fUpdateDrawAction();
    void fViewClick(int x, int y);
    void fDraw(double StartX, double StartY, double StopX, double StopY);
    void fItemDrop(TItem *Item, double StartDx, double StartDy, double StopDx, double StopDy);
    void fMoveItemList(TScene *Scene, QList<TItem *> ItemList,
                       double StartX, double StartY, double StopX, double StopY);
    void fStyle();
    void fCut();
    void fCopy();
    void fPaste();
    void fDel();
    void fMerge();
    void fAttribute();
    //void fLinkage();
    //void fRunMode();
    //void fJsHelper();
    void fTop();
    void fUpItems(TItem *Item);
    void fBottom();
    void fFinshPolygon();
    void fMove(double Dx1, double Dy1, double Dx2, double Dy2);
    void fAlignTop();
    void fAlignBottom();
    void fAlignLeft();
    void fAlignRight();
    void fUp();
    void fDown();
    void fLeft();
    void fRight();

    void fLineStyleChanged(int Style);
    void fLineWidthChanged(int Width);

    void fUpdateLineColor();
    void fUpdateBackColor();
    void fScenseList();
    void fService();
    void fGroupId();
    void fCopyScence();
    void fDbMsg();
    void fTimerMake();
    void fCurverConfigure();
    void fPieConfigure();
    void fNew();

    void fSave();
    void fSaveAs();
    void fSaveToDB();
    void fLoadFromDB();
    void fEdit();
    void fSaveItems();
    void fOpen();
    void fUpdateItems();           // 加载图元
    void fAddItemToBar(QString Dir, QString FileName); // 添加图元至绘图栏
    void fDrawItem();             // 点击图元，在屏幕上绘制
    void fClose();
    bool fCloseAll();

    void fUpdateCurrentScene(bool Checked = false);
    void fUpdateWindow(bool Checked = false);
    void fUpdateWindowAction(TAction *Action, bool Checked);

    void fZoomOut();
    void fZoomIn();
    void fZoomNormal();
    void fAddLink();
    //void fGotoLink(int LinkId);
    void fSetHost();
    void fAddActionToMenu(QString Name, TScene *Scene);

public:
    QString mProjectName;
    QWidget *mWidget;

    QLabel *mLabelLineStyleShow;
    QComboBox *mComboBoxLineStyle;
    QLabel *mLabelLineWidthShow;
    QLabel *mLabelLineWidth;
    QSpinBox *mSpinBoxLineWidth;
    QLabel *mLabelLineColorShow;
    QLabel *mLabelLineColor;
    QPushButton *mButtonLineColor;
    QLabel *mLabelBackColorShow;
    QLabel *mLabelBackColor;
    QPushButton *mButtonBackColor;

    int mSceneId;
    int mSceneHost;             // 主画面ID
    QList<TScene *> mSceneList; // 场景列表
    TScene *mCurrentScene;      // 当前场景
    TView *mView;
    //TRunMode* mRunMode;

    // 菜单栏
    QMenu *mFileMenu;
    QMenu *mDrawMenu;
    QMenu *mEditMenu;
    QMenu *mGraphicsMenu;
    QMenu *mManageMenu;
    QMenu *mWindowMenu;
    QMenu *mAboutMenu;

    // 工具栏
    QToolBar *mDrawToolBar;
    QToolBar *mEditToolBar;
    QToolBar *mItemToolBar;
    QToolBar *mStateToolBar;

    // 按钮
    TAction *mActionFileNew;
    TAction *mActionFileEdit;
    TAction *mActionFileClose;
    TAction *mActionFileCloseAll;
    //
    TAction *mActionFileOpen;
    TAction *mActionFileSave;
    TAction *mActionFileSaveAs;
    TAction *mActionFileOpenFromDB;
    TAction *mActionFileSaveToDB;
    TAction *mActionFileSaveAsDB;
    //
    TAction *mActionFileSaveItem;

    // 绘图
    TAction *mActionDrawButton;
    TAction *mActionDrawTextBox;
    TAction *mActionDrawTable;
    TAction *mActionDrawProcessBar;
    TAction *mActionDrawMeter;
    TAction *mActionDrawLight;
    TAction *mActionDrawCurve;

    TAction *mActionDrawFree;
    TAction *mActionDrawPoint;
    TAction *mActionDrawLine;
    TAction *mActionDrawEllipse;
    TAction *mActionDrawRect;
    TAction *mActionDrawPolygon;
    TAction *mActionDrawArc;
    TAction *mActionDrawText;
    TAction *mActionDrawValueText;
    TAction *mActionDrawPixmap;
    TAction *mActionDrawPath;

    // 编辑
    TAction *mActionEditStyle;
    TAction *mActionEditCut;
    TAction *mActionEditCopy;
    TAction *mActionEditPaste;
    //TAction *mActionEditUndo;
    //TAction *mActionEditRedo;
    TAction *mActionEditDel;

    // 图形
    TAction *mActionGraphMerge;
    TAction *mActionMoveToTop;
    TAction *mActionMoveToBottom;
    TAction *mActionAlignTop;
    TAction *mActionAlignLeft;
    TAction *mActionAlignRight;
    TAction *mActionAlignBottom;
    TAction *mActionMoveUp;
    TAction *mActionMoveDown;
    TAction *mActionMoveLeft;
    TAction *mActionMoveRight;

    // 撤销、重做
    QUndoStack * mUndoStack;
    QAction * mActionUndo;
    QAction * mActionRedo;

    // 管理
    //TAction *mActionRunMode;
    TAction *mActionAttribute;
    //TAction *mActionControl;
    TAction *mActionLinkage;
    TAction *mActionJsHelper;
    TAction *mActionService;
    TAction *mActionGroupId;
    TAction *mActionSceneList;
    TAction *mActionCopyScence;
    TAction *mActionDB;
    TAction *mActionTimer;
    TAction *mActionCurve;
    TAction *mActionPie;

    // 窗口
    TAction *mActionAddLink;
    TAction *mActionSetHost;
    TAction *mActionZoomOut;
    TAction *mActionZoomIn;
    TAction *mActionZoomNormal;
    const int mDefaltWinActSum;

    // 关于
    TAction *mActionAbout;

    // 选择的按钮
    TAction *mDrawSelected;

    void fCreateActions();
    void fCreateMenus();
    void fCreateToolBars();

    TAction* fAddFileAction(QString _Name);
    TAction* fAddDrawAction(QString _Name, QString _IconDir, QString _ShutCut);
    TAction* fAddEditAction(QString _Name, QString _IconDir, QString _ShutCut);

    TItem* fItemSelected() const;
    TGraphics* fGraphicsSelected() const;
    TWidgets* fWidgetsSelected() const;
    void fAddItemToScene(TItem *Item);
    void fCopyItemListToScene(QList<TItem *> &ItemList);
    void fAddItemList(TScene *Scene, QList<TItem *> &ItemList);

    int mZMax;// 置顶
    int mZMin;// 置底

    // 全局绘图属性
    Qt::PenStyle mLineStyle;        //线条的类型
    int mLineWidth;                //线条的宽度
    QColor mLineColor;             //线条的颜色
    QColor mBackColor;

    // 保存绘制多边形的点
    QPolygonF mPolygonPoints;

    // 剪切、复制列表
    QList <TItem *>mCopyList;

    // 服务列表
    QList<TService *>mServiceList;

    // 联动列表
    QList<TLinkage *>mLinkageList;

    // 数据库信息
    TDataBaseMsg *mDBMsg;

    // 保存数据库连接，用于打开和保存
    TDataBase *mDataBase;
    //组合组名
    QHash<QString, int> mGroupHash;

    TItem* fCopyItem(TItem *SourceItem);
    void fSetItem(TItem *Item);

    void UpdateAfterOpen();

    TScene* fNewScene(int SceneId, QString Name, QColor BackColor,
                      double PosX = 0, double PosY = 0, double Width = 1000, double Height = 600);
    void fSetScene(TScene *Scene);

    bool fSaveToFile(const QString FileName);
    void fUpdateProjectName();

    bool fConnectToDB();
    bool copyScense=false;     //
    // 报表
    QStringList reportName;
    QList<QStringList> content;

    QList<TScene::TimerStruct> CurverStructList;
    QList<TScene::TimerStruct> PieStructList;
};

#endif // MAKER_H
