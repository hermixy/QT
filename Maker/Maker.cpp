#include "Maker.h"

//#include "../Scada/Dialog/RunMode/DialogRunMode.h"
#include "../Scada/Dialog/Style/DialogStyle.h"
#include "../Scada/Dialog/Style/dialogaddgroup.h"
#include "../Scada/Dialog/Play/DialogPlay.h"
//#include "../Scada/Dialog/Control/DialogControl.h"
#include "../Scada/Dialog/Linkage/DialogLinkage.h"
#include "../Scada/Dialog/Service/DialogService.h"
#include "../Scada/Dialog/JsHelper/DialogJsHelper.h"
#include "../Scada/Dialog/NewScene/DialogNewScene.h"
#include "../Scada/Dialog/AddLink/DialogAddLink.h"
#include "../Scada/Dialog/DataBase/DialogDataBase.h"
#include "../Scada/Dialog/Style/scenselistdialog.h"
#include "../Scada/Dialog/Timer/timeritemshow.h"
#include "../Scada/Src/Xml/XmlWriter.h"
#include "../Scada/Src/Xml/XmlReader.h"
#include "../Scada/Dialog/Style/tabwidgetreport.h"

#include "../Scada/Src/Item/ItemFactory.h"

#include "../Scada/Src/Undo/UndoAddDel.h"
#include "../Scada/Src/Undo/UndoMoveDrag.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

#include <QDebug>
#include <QInputDialog>

#if 1
#include <QMenuBar>
#include <QToolBar>
#include <QWidget>
#else
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#endif
#include <QtXml/QDomDocument>
#include <QDebug>
#include <ActiveQt/QAxWidget>
Maker * Maker::maker=nullptr;
Maker::Maker(QWidget *parent):
    QMainWindow(parent), mDefaltWinActSum(7) // 5个项 2个分隔符
{
    //mRunMode = new TRunMode(0, tr("127.0.0.1"), 6066);

    mProjectName = tr("");
    mCurrentScene = 0;
    mSceneId = 0;
    mSceneHost = 1;
    maker=this;

    // 初始化数据库信息(主机、用户名、密码等)
    mDBMsg = new TDataBaseMsg();

    mDataBase = NULL;             //数据库操作基类

    QWidget *mWidget = new QWidget(this);
    //mWidget->setObjectName(QStringLiteral("mWidget"));
    mWidget->setObjectName(tr("mWidget"));
    setCentralWidget(mWidget);              //

    mDrawSelected = NULL;                   //绘画选择null

    fCreateActions();
    fCreateMenus();
    fCreateToolBars();

    fUpdateItems();

    mView = new TView();
    //mView->setFixedSize(1000, 600);
    //mView->setObjectName(tr("mView"));

    //mView->setGeometry(QRect(0, 0, 800, 600));
    //mView->setScene(mScene);

    mView->setDragMode(QGraphicsView::RubberBandDrag);
    //mView->setRenderHints(QPainter::Antialiasing
                   //      | QPainter::TextAntialiasing);
    //mView->setContextMenuPolicy(Qt::ActionsContextMenu);
    connect(mView, SIGNAL(mMousePress(int,int)), this, SLOT(fViewClick(int,int)));

    mZMin = 0;
    mZMax = 0;
    setWindowTitle(tr("组态制作程序"));
    QVBoxLayout *mLayoutMain = new QVBoxLayout(mWidget);

    mLayoutMain->addWidget(mView);
    //mLayoutMain->addWidget(mGroupBoxStyle);
}

void Maker::fViewClick(int x, int y)
{
    Q_UNUSED(x);
    Q_UNUSED(y);

    //qDebug() << mCurrentScene;
    //qDebug() << mDrawSelected;

    if(NULL == mCurrentScene)
    {
        if(NULL == mDrawSelected)
        {
            QMessageBox::warning(this, tr("提示"), tr("请选择一个绘图工具，再进行绘图"));
        }
        else
        {
            QMessageBox::warning(this, tr("提示"), tr("请执行“文件--新建”，新建一个画布，再进行绘图"));
        }
    }
}

void Maker::fCreateActions()
{
    mActionFileNew = fAddFileAction(tr("新建画布(&N)..."));
    mActionFileNew->setShortcut(QKeySequence::New);
    connect(mActionFileNew, SIGNAL(triggered()), this, SLOT(fNew()));
    mActionFileEdit = fAddFileAction(tr("编辑画布(&E)..."));
    connect(mActionFileEdit, SIGNAL(triggered()), this, SLOT(fEdit()));
    mActionFileClose = fAddFileAction(tr("删除画布(&C)"));
    connect(mActionFileClose, SIGNAL(triggered()), this, SLOT(fClose()));
    mActionFileCloseAll = fAddFileAction(tr("删除所有画布(&W)"));
    mActionFileCloseAll->setShortcut(QKeySequence::Close);
    connect(mActionFileCloseAll, SIGNAL(triggered()), this, SLOT(fCloseAll()));

    mActionFileOpen = fAddFileAction(tr("打开工程(&O)..."));
    mActionFileOpen->setShortcut(QKeySequence::Open);
    connect(mActionFileOpen, SIGNAL(triggered()), this, SLOT(fOpen()));
    mActionFileSave = fAddFileAction(tr("保存工程(&S)"));
    mActionFileSave->setShortcut(QKeySequence::Save);
    connect(mActionFileSave, SIGNAL(triggered()), this, SLOT(fSave()));
    mActionFileSaveAs = fAddFileAction(tr("工程另存为(&A)..."));
    mActionFileSaveAs->setShortcut(QKeySequence::SaveAs);
    connect(mActionFileSaveAs, SIGNAL(triggered()), this, SLOT(fSaveAs()));
   /* mActionFileOpenFromDB = fAddFileAction(tr("打开自数据库(&D)..."));
    connect(mActionFileOpenFromDB, SIGNAL(triggered()), this, SLOT(fLoadFromDB()));
    mActionFileSaveToDB = fAddFileAction(tr("保存至数据库(&B)"));
    connect(mActionFileSaveToDB, SIGNAL(triggered()), this, SLOT(fSaveToDB()));
    mActionFileSaveAsDB = fAddFileAction(tr("另保存为数据库(&G)..."));*/

    mActionFileSaveItem = fAddFileAction(tr("保存图元(&I)..."));
    connect(mActionFileSaveItem, SIGNAL(triggered()), this, SLOT(fSaveItems()));

    //////////////////////////////////////////////////
    mActionDrawFree = fAddDrawAction(tr("释放(&F)"), tr("://pic/draw/Free.png"), tr(""));
    mActionDrawFree->mType = TItem::Base;
    mActionDrawFree->setCheckable(false);// 不保持按下
    mActionDrawPoint = fAddDrawAction(tr("点(&D)"), tr("://pic/draw/Point.png"), tr(""));
    mActionDrawPoint->mType = TItem::Point;
    mActionDrawLine = fAddDrawAction(tr("线(&N)"), tr("://pic/draw/Line.png"), tr(""));
    mActionDrawLine->mType = TItem::Line;
    mActionDrawEllipse = fAddDrawAction(tr("椭圆(&E)"), tr("://pic/draw/Ellipse.png"), tr(""));
    mActionDrawEllipse->mType = TItem::Ellipse;
    mActionDrawRect = fAddDrawAction(tr("矩形(&R)"), tr("://pic/draw/Rect.png"), tr(""));
    mActionDrawRect->mType = TItem::Rect;
    mActionDrawPolygon = fAddDrawAction(tr("任意多边形(&P)"), tr("://pic/draw/Polygon.png"), tr(""));
    mActionDrawPolygon->mType = TItem::Polygon;
    connect(mActionDrawPolygon, SIGNAL(changed()), this, SLOT(fFinshPolygon()));
    mActionDrawArc = fAddDrawAction(tr("圆弧(&A)"), tr("://pic/draw/Arc.png"), tr(""));
    mActionDrawArc->mType = TItem::Arc;
    mActionDrawText = fAddDrawAction(tr("文字(&T)"), tr("://pic/draw/Text.png"), tr(""));
    mActionDrawText->mType = TItem::Text;
    mActionDrawValueText = fAddDrawAction(tr("值文字(&V)"), tr("://pic/draw/ValueText.png"), tr(""));
    mActionDrawValueText->mType = TItem::ValueText;
    mActionDrawPixmap = fAddDrawAction(tr("图片(&C)"), tr("://pic/draw/Pixmap.png"), tr(""));
    mActionDrawPixmap->mType = TItem::Pixmap;
    //mActionDrawPath = fAddDrawAction(tr("路径(&H)"), tr("://pic/draw/Path.png"), tr(""));
    //mActionDrawPath->mType = TItem::Path;

    mActionDrawButton = fAddDrawAction(tr("按钮(&U)"), tr("://pic/draw/Button.png"), tr(""));
    mActionDrawButton->mType = TItem::Button;
    mActionDrawTextBox = fAddDrawAction(tr("文本框(&I)"), tr("://pic/draw/TextBox.png"), tr(""));
    mActionDrawTextBox->mType = TItem::InputEdit;
    mActionDrawTable = fAddDrawAction(tr("表格(&B)"), tr("://pic/draw/Table.png"), tr(""));
    mActionDrawTable->mType = TItem::Table;
    mActionDrawProcessBar = fAddDrawAction(tr("进度条(&S)"), tr("://pic/draw/ProcessBar.png"), tr(""));
    mActionDrawProcessBar->mType = TItem::ProcessBar;
    mActionDrawMeter = fAddDrawAction(tr("表盘(&M)"), tr("://pic/draw/Clock.png"), tr(""));
    mActionDrawMeter->mType = TItem::Meter;
    mActionDrawLight = fAddDrawAction(tr("指示灯(&L)"), tr("://pic/draw/Light.png"), tr(""));
    mActionDrawLight->mType = TItem::Light;
    //mActionDrawCurve = fAddDrawAction(tr("实时曲线(&C)"), tr("://pic/draw/Curve.png"), tr(""));
    //mActionDrawCurve->mType = TItem::Curve;

    /////////////////////////////////////////////////
    mActionEditStyle = fAddEditAction(tr("样式(&S)..."), tr("://pic/edit/Style.png"), tr(""));
    connect(mActionEditStyle, SIGNAL(triggered()), this, SLOT(fStyle()));
    mActionEditCut = fAddEditAction(tr("剪切(&X)"), tr("://pic/edit/Cut.png"), tr(""));
    mActionEditCut->setShortcut(QKeySequence::Cut);
    connect(mActionEditCut, SIGNAL(triggered()), this, SLOT(fCut()));
    mActionEditCopy = fAddEditAction(tr("复制(&C)"), tr("://pic/edit/Copy.png"), tr(""));
    mActionEditCopy->setShortcut(QKeySequence::Copy);
    connect(mActionEditCopy, SIGNAL(triggered()), this, SLOT(fCopy()));
    mActionEditPaste = fAddEditAction(tr("粘贴(&P)"), tr("://pic/edit/Paste.png"), tr(""));
    mActionEditPaste->setShortcut(QKeySequence::Paste);
    connect(mActionEditPaste, SIGNAL(triggered()), this, SLOT(fPaste()));

    /*mActionEditUndo = fAddEditAction(tr("撤销"), tr("://pic/edit/Undo.png"), tr(""));
    mActionEditUndo->setShortcut(QKeySequence::Undo);
    mActionEditRedo = fAddEditAction(tr("重做"), tr("://pic/edit/Redo.png"), tr(""));
    mActionEditRedo->setShortcut(QKeySequence::Redo);*/

    mUndoStack = new QUndoStack(this);;
    mActionUndo = mUndoStack->createUndoAction(this, tr("撤销(&U)"));
    mActionUndo->setIcon(QIcon(tr("://pic/edit/Undo.png")));
    mActionUndo->setShortcut(QKeySequence::Undo);
    mActionRedo = mUndoStack->createRedoAction(this, tr("重做(&R)"));
    mActionRedo->setIcon(QIcon(tr("://pic/edit/Redo.png")));
    mActionRedo->setShortcut(QKeySequence::Redo);

    mActionEditDel = fAddEditAction(tr("删除(&D)"), tr("://pic/edit/Del.png"), tr(""));
    mActionEditDel->setShortcut(QKeySequence::Delete);
    connect(mActionEditDel, SIGNAL(triggered()), this, SLOT(fDel()));

    /////////////////////////////////////////////
    mActionGraphMerge = fAddEditAction(tr("合并(&M)"), tr("://pic/graph/Merge.png"), tr(""));
    connect(mActionGraphMerge, SIGNAL(triggered()), this, SLOT(fMerge()));
    mActionMoveToTop = fAddEditAction(tr("置于顶层(&T)"), tr("://pic/graph/MoveToTop.png"), tr(""));
    connect(mActionMoveToTop, SIGNAL(triggered()), this, SLOT(fTop()));
    mActionMoveToBottom = fAddEditAction(tr("置于底层(&D)"), tr("://pic/graph/MoveToBottom.png"), tr(""));
    connect(mActionMoveToBottom, SIGNAL(triggered()), this, SLOT(fBottom()));

    mActionAlignLeft = fAddEditAction(tr("左对齐(&L)"), tr("://pic/graph/AlignLeft.png"), tr(""));
    mActionAlignRight = fAddEditAction(tr("右对齐(&R)"), tr("://pic/graph/AlignRight.png"), tr(""));
    mActionAlignTop = fAddEditAction(tr("顶对齐(&T)"), tr("://pic/graph/AlignTop.png"), tr(""));
    mActionAlignBottom = fAddEditAction(tr("底对齐(&B)"), tr("://pic/graph/AlignBottom.png"), tr(""));

    connect(mActionAlignLeft, SIGNAL(triggered()), this, SLOT(fAlignLeft()));
    connect(mActionAlignRight, SIGNAL(triggered()), this, SLOT(fAlignRight()));
    connect(mActionAlignTop, SIGNAL(triggered()), this, SLOT(fAlignTop()));
    connect(mActionAlignBottom, SIGNAL(triggered()), this, SLOT(fAlignBottom()));

    mActionMoveUp = fAddEditAction(tr("上移"), tr("://pic/graph/Up.png"), tr(""));
    mActionMoveUp->setShortcut(QKeySequence::MoveToPreviousLine);
    connect(mActionMoveUp, SIGNAL(triggered()), this, SLOT(fUp()));
    mActionMoveDown = fAddEditAction(tr("下移"), tr("://pic/graph/Down.png"), tr(""));
    mActionMoveDown->setShortcut(QKeySequence::MoveToNextLine);
    connect(mActionMoveDown, SIGNAL(triggered()), this, SLOT(fDown()));
    mActionMoveLeft = fAddEditAction(tr("左移"), tr("://pic/graph/Left.png"), tr(""));
    mActionMoveLeft->setShortcut(QKeySequence::MoveToPreviousChar);
    connect(mActionMoveLeft, SIGNAL(triggered()), this, SLOT(fLeft()));
    mActionMoveRight = fAddEditAction(tr("右移"), tr("://pic/graph/Right.png"), tr(""));
    mActionMoveRight->setShortcut(QKeySequence::MoveToNextChar);
    connect(mActionMoveRight, SIGNAL(triggered()), this, SLOT(fRight()));

    /////////////////////////////////////////////////////////
    //mActionJsHelper = fAddFileAction(tr("脚本助手(&E)..."));
    //connect(mActionJsHelper, SIGNAL(triggered()), this, SLOT(fJsHelper()));
    mActionAttribute = fAddFileAction(tr("报表(&P)..."));
    connect(mActionAttribute, SIGNAL(triggered()), this, SLOT(fAttribute()));
    //mActionLinkage = fAddFileAction(tr("图形联动(&L)..."));
    //connect(mActionLinkage, SIGNAL(triggered()), this, SLOT(fLinkage()));
    //mActionRunMode = fAddFileAction(tr("运行模式"));
    //connect(mActionRunMode, SIGNAL(triggered()), this, SLOT(fRunMode()));
    mActionService = fAddFileAction(tr("系统服务(&S)..."));
    connect(mActionService, SIGNAL(triggered()), this, SLOT(fService()));
    mActionSceneList=fAddFileAction(tr("场景列表(&S)..."));
    connect(mActionSceneList, SIGNAL(triggered()), this, SLOT(fScenseList()));

    mActionGroupId =fAddFileAction(tr("添加组..."));
    connect(mActionGroupId, SIGNAL(triggered()), this, SLOT(fGroupId()));
    mActionCopyScence=fAddFileAction(tr("复制场景"));
    connect(mActionCopyScence, SIGNAL(triggered()), this, SLOT(fCopyScence()));
    mActionDB = fAddFileAction(tr("数据库配置(&D)..."));
    connect(mActionDB, SIGNAL(triggered()), this, SLOT(fDbMsg()));
    mActionTimer = fAddFileAction(tr("定时配置"));
    connect(mActionTimer, SIGNAL(triggered()), this, SLOT(fTimerMake()));
    mActionCurve=fAddFileAction(tr("实时曲线配置"));
    connect(mActionCurve, SIGNAL(triggered()), this, SLOT(fCurverConfigure()));

    mActionPie=fAddFileAction(tr("实时饼图配置"));
    connect(mActionPie, SIGNAL(triggered()), this, SLOT(fPieConfigure()));
    /////////////////////////////////////////////////////////////
    mActionZoomOut = fAddFileAction(tr("放大"));
    connect(mActionZoomOut, SIGNAL(triggered()), this, SLOT(fZoomOut()));
    mActionZoomIn = fAddFileAction(tr("缩小"));
    connect(mActionZoomIn, SIGNAL(triggered()), this, SLOT(fZoomIn()));
    mActionZoomNormal = fAddFileAction(tr("正常"));
    connect(mActionZoomNormal, SIGNAL(triggered()), this, SLOT(fZoomNormal()));
    mActionAddLink = fAddFileAction(tr("添加链接(&L)..."));
    connect(mActionAddLink, SIGNAL(triggered()), this, SLOT(fAddLink()));
    mActionSetHost = fAddFileAction(tr("设为主画面(&H)"));
    connect(mActionSetHost, SIGNAL(triggered()), this, SLOT(fSetHost()));
    ///////////////////////////////////////////////////////
    mActionAbout = fAddFileAction(tr("关于(&A)"));
}


TAction *Maker::fAddDrawAction(QString _Name, QString _IconDir, QString _ShutCut)
{
    TAction *Action = new TAction(_Name, this);
    Action->setIcon(QIcon(_IconDir));
    Action->setCheckable(true);

    if(_ShutCut.length() > 0)
    {
        Action->setShortcut(_ShutCut);
    }

    connect(Action, SIGNAL(triggered()), this, SLOT(fUpdateDrawAction()));

    return Action;
}

TAction *Maker::fAddEditAction(QString _Name, QString _IconDir, QString _ShutCut)
{
    TAction *Action = new TAction(_Name, this);
    Action->setIcon(QIcon(_IconDir));
    //Action->setCheckable(true);
    Action->mType = TItem::Base;

    if(_ShutCut.length() > 0)
    {
        Action->setShortcut(_ShutCut);
    }

    return Action;
}

TAction *Maker::fAddFileAction(QString _Name)
{
    TAction *Action = new TAction(_Name, this);

    return Action;
}

void Maker::fCreateMenus()
{
    QMenuBar *mMenuBar = new QMenuBar(this);
    //mMenuBar->setObjectName(QStringLiteral("mMenuBar"));
    mMenuBar->setObjectName(tr("mMenuBar"));
    //mMenuBar->setGeometry(QRect(0, 0, 1366, 23));
    this->setMenuBar(mMenuBar);

    mFileMenu = mMenuBar->addMenu(tr("文件(&F)"));
    mFileMenu->addAction(mActionFileNew);
    mFileMenu->addAction(mActionFileEdit);
    mFileMenu->addAction(mActionFileClose);
    mFileMenu->addAction(mActionFileCloseAll);
    mFileMenu->addSeparator();
    mFileMenu->addAction(mActionFileOpen);
    mFileMenu->addAction(mActionFileSave);
    mFileMenu->addAction(mActionFileSaveAs);
    mFileMenu->addSeparator();
    //mFileMenu->addAction(mActionFileOpenFromDB);
    //mFileMenu->addAction(mActionFileSaveToDB);
    //mFileMenu->addAction(mActionFileSaveAsDB);
    mFileMenu->addSeparator();
    mFileMenu->addAction(mActionFileSaveItem);

    mDrawMenu = mMenuBar->addMenu(tr("绘图(&D)"));
    mDrawMenu->addAction(mActionDrawFree);
    mDrawMenu->addSeparator();
    mDrawMenu->addAction(mActionDrawButton);
    mDrawMenu->addAction(mActionDrawTextBox);
    mDrawMenu->addAction(mActionDrawTable);
    mDrawMenu->addSeparator();
    mDrawMenu->addAction(mActionDrawValueText);
    mDrawMenu->addAction(mActionDrawProcessBar);
    mDrawMenu->addAction(mActionDrawMeter);
    mDrawMenu->addAction(mActionDrawLight);
    //mDrawMenu->addAction(mActionDrawCurve);
    mDrawMenu->addSeparator();
    mDrawMenu->addAction(mActionDrawPoint);
    mDrawMenu->addAction(mActionDrawLine);
    mDrawMenu->addAction(mActionDrawEllipse);
    mDrawMenu->addAction(mActionDrawRect);
    mDrawMenu->addAction(mActionDrawPolygon);
    mDrawMenu->addAction(mActionDrawArc);
    mDrawMenu->addAction(mActionDrawText);
    mDrawMenu->addAction(mActionDrawPixmap);
    //mDrawMenu->addAction(mActionDrawPath);

    mEditMenu = mMenuBar->addMenu(tr("编辑(&E)"));
    mEditMenu->addAction(mActionEditStyle);
    mEditMenu->addSeparator();
    mEditMenu->addAction(mActionEditCut);
    mEditMenu->addAction(mActionEditCopy);
    mEditMenu->addAction(mActionEditPaste);
    mEditMenu->addAction(mActionEditDel);
    mEditMenu->addSeparator();
    //mEditMenu->addAction(mActionEditUndo);
    //mEditMenu->addAction(mActionEditRedo);
    mEditMenu->addAction(mActionUndo);
    mEditMenu->addAction(mActionRedo);

    mGraphicsMenu = mMenuBar->addMenu(tr("图形(&G)"));
    mGraphicsMenu->addAction(mActionGraphMerge);
    mGraphicsMenu->addAction(mActionMoveToTop);
    mGraphicsMenu->addAction(mActionMoveToBottom);
    mGraphicsMenu->addSeparator();
    mGraphicsMenu->addAction(mActionAlignLeft);
    mGraphicsMenu->addAction(mActionAlignRight);
    mGraphicsMenu->addAction(mActionAlignTop);
    mGraphicsMenu->addAction(mActionAlignBottom);
    mGraphicsMenu->addSeparator();
    mGraphicsMenu->addAction(mActionMoveUp);
    mGraphicsMenu->addAction(mActionMoveDown);
    mGraphicsMenu->addAction(mActionMoveLeft);
    mGraphicsMenu->addAction(mActionMoveRight);

    mManageMenu = mMenuBar->addMenu(tr("管理(&M)"));
    //mManageMenu->addAction(mActionRunMode);
    mManageMenu->addAction(mActionAttribute);
    //mManageMenu->addAction(mActionLinkage);
    mManageMenu->addAction(mActionService);
    mManageMenu->addAction(mActionSceneList);
    mManageMenu->addAction(mActionGroupId);
    mManageMenu->addAction(mActionCopyScence);
    mManageMenu->addAction(mActionDB);
    mManageMenu->addAction(mActionTimer);
    mManageMenu->addAction(mActionCurve);
    mManageMenu->addAction(mActionPie);

    //mManageMenu->addAction(mActionJsHelper);

    mWindowMenu = mMenuBar->addMenu(tr("窗口(&U)"));
    mWindowMenu->addAction(mActionZoomOut);
    mWindowMenu->addAction(mActionZoomIn);
    mWindowMenu->addAction(mActionZoomNormal);
    mWindowMenu->addSeparator();
    mWindowMenu->addAction(mActionSetHost);
    mWindowMenu->addAction(mActionAddLink);
    mWindowMenu->addSeparator();

    mAboutMenu = mMenuBar->addMenu(tr("关于(&H)"));
    mAboutMenu->addAction(mActionAbout);
}

void Maker::fCreateToolBars()
{
    mDrawToolBar = new QToolBar(tr("绘图"));
    mDrawToolBar->setGeometry(QRect(0, 0, 16, 600));
    mDrawToolBar->addAction(mActionDrawFree);
    mDrawToolBar->addSeparator();
    mDrawToolBar->addAction(mActionDrawButton);
    mDrawToolBar->addAction(mActionDrawTextBox);
    mDrawToolBar->addAction(mActionDrawTable);
    mDrawToolBar->addSeparator();
    mDrawToolBar->addAction(mActionDrawValueText);
    mDrawToolBar->addAction(mActionDrawProcessBar);
    mDrawToolBar->addAction(mActionDrawMeter);
    mDrawToolBar->addAction(mActionDrawLight);
    //mDrawToolBar->addAction(mActionDrawCurve);
    mDrawToolBar->addSeparator();
    mDrawToolBar->addAction(mActionDrawPoint);
    mDrawToolBar->addAction(mActionDrawLine);
    mDrawToolBar->addAction(mActionDrawEllipse);
    mDrawToolBar->addAction(mActionDrawRect);
    mDrawToolBar->addAction(mActionDrawPolygon);
    mDrawToolBar->addAction(mActionDrawArc);
    mDrawToolBar->addAction(mActionDrawText);
    mDrawToolBar->addAction(mActionDrawPixmap);
    //mDrawToolBar->addAction(mActionDrawPath);
    addToolBar(Qt::LeftToolBarArea, mDrawToolBar);

    mEditToolBar = new QToolBar(tr("编辑"));
    mEditToolBar->setGeometry(QRect(0, 0, 16, 600));
    mEditToolBar->addAction(mActionEditStyle);
    mEditToolBar->addSeparator();
    mEditToolBar->addAction(mActionEditCut);
    mEditToolBar->addAction(mActionEditCopy);
    mEditToolBar->addAction(mActionEditPaste);
    mEditToolBar->addAction(mActionEditDel);
    mEditToolBar->addSeparator();
    //mEditToolBar->addAction(mActionEditUndo);
    //mEditToolBar->addAction(mActionEditRedo);
    mEditToolBar->addAction(mActionUndo);
    mEditToolBar->addAction(mActionRedo);
    mEditToolBar->addSeparator();
    mEditToolBar->addAction(mActionGraphMerge);
    mEditToolBar->addAction(mActionMoveToTop);
    mEditToolBar->addAction(mActionMoveToBottom);
    mEditToolBar->addSeparator();
    mEditToolBar->addAction(mActionAlignLeft);
    mEditToolBar->addAction(mActionAlignRight);
    mEditToolBar->addAction(mActionAlignTop);
    mEditToolBar->addAction(mActionAlignBottom);
    mEditToolBar->addSeparator();
    mEditToolBar->addAction(mActionMoveUp);
    mEditToolBar->addAction(mActionMoveDown);
    mEditToolBar->addAction(mActionMoveLeft);
    mEditToolBar->addAction(mActionMoveRight);
    addToolBar(Qt::RightToolBarArea, mEditToolBar);

    ///////////////////////////////////////////////////////////////
    /*mLabelItemFlagShow = TBase::fLabel(tr("      图形属性"), tr("mLabelItemFlagShow"));
    mComboBoxItemFlag = TBase::fComboBox(tr("mComboBoxItemFlag"));
    mComboBoxItemFlag->addItem(tr("可拖拽"));
    mComboBoxItemFlag->addItem(tr("可移动"));
    mComboBoxItemFlag->setCurrentIndex(1);
    connect(mComboBoxItemFlag, SIGNAL(currentIndexChanged(int)), this, SLOT(fItemFlag(int)));
    */
    mLabelLineStyleShow = TBase::fLabel(tr("      样式"), tr("mLabelLineStyleShow"));
    mComboBoxLineStyle = TBase::fLineStyleComboBox();
    mComboBoxLineStyle->setCurrentIndex(1);
    mLineStyle = Qt::SolidLine;
    connect(mComboBoxLineStyle, SIGNAL(currentIndexChanged(int)), this, SLOT(fLineStyleChanged(int)));

    mLabelLineWidthShow = TBase::fLabel(tr("宽度"), tr("mLabelLineWidthShow"));
    mLabelLineWidth     = TBase::fLabel(tr(""), tr("mLabelLineWidth"), 50, 22);

    mSpinBoxLineWidth   = TBase::fSpinBox(tr("mSpinBoxLineWidth"), 1, 100);
    mLineWidth = 1;
    fLineWidthChanged(1);
    connect(mSpinBoxLineWidth, SIGNAL(valueChanged(int)), this, SLOT(fLineWidthChanged(int)));

    mLabelLineColorShow = TBase::fLabel(tr("线条"), tr("mLabelLineColorShow"));
    mLabelLineColor     = TBase::fLabel(tr(""), tr("mLabelLineColor"), 50, 22);

    mLineColor = Qt::white;
    TBase::fColorLabel(mLabelLineColor, mLineColor);
    mButtonLineColor    = TBase::fButton(tr("浏览"), tr("mButtonLineColor"));
    connect(mButtonLineColor, SIGNAL(clicked()), this, SLOT(fUpdateLineColor()));

    mLabelBackColorShow = TBase::fLabel(tr("背景"), tr("mLabelBackColorShow"));
    mLabelBackColor     = TBase::fLabel(tr(""), tr("mLabelBackColor"), 50, 22);

    mBackColor = Qt::black;
    TBase::fColorLabel(mLabelBackColor, mBackColor);
    mButtonBackColor    = TBase::fButton(tr("浏览"), tr("mButtonLineColor"));
    connect(mButtonBackColor, SIGNAL(clicked()), this, SLOT(fUpdateBackColor()));

    mStateToolBar = new QToolBar(tr("状态"));
    mStateToolBar->setGeometry(QRect(0, 0, 16, 600));
    /*mStateToolBar->addWidget(mLabelItemFlagShow);
    mStateToolBar->addSeparator();
    mStateToolBar->addWidget(mComboBoxItemFlag);
    mStateToolBar->addSeparator();
    mStateToolBar->addSeparator();
    mStateToolBar->addSeparator();*/

    mStateToolBar->addWidget(mLabelLineStyleShow);
    mStateToolBar->addSeparator();
    mStateToolBar->addWidget(mComboBoxLineStyle);
    mStateToolBar->addSeparator();
    mStateToolBar->addSeparator();
    mStateToolBar->addSeparator();

    mStateToolBar->addWidget(mLabelLineWidthShow);
    mStateToolBar->addSeparator();
    mStateToolBar->addWidget(mLabelLineWidth);
    mStateToolBar->addSeparator();
    mStateToolBar->addWidget(mSpinBoxLineWidth);
    mStateToolBar->addSeparator();
    mStateToolBar->addSeparator();
    mStateToolBar->addSeparator();

    mStateToolBar->addWidget(mLabelLineColorShow);
    mStateToolBar->addSeparator();
    mStateToolBar->addWidget(mLabelLineColor);
    mStateToolBar->addSeparator();
    mStateToolBar->addWidget(mButtonLineColor);
    mStateToolBar->addSeparator();
    mStateToolBar->addSeparator();
    mStateToolBar->addSeparator();

    mStateToolBar->addWidget(mLabelBackColorShow);
    mStateToolBar->addSeparator();
    mStateToolBar->addWidget(mLabelBackColor);
    mStateToolBar->addSeparator();
    mStateToolBar->addWidget(mButtonBackColor);

    addToolBar(Qt::BottomToolBarArea, mStateToolBar);

    ////////////////////////////////////////////////////////////////
    mItemToolBar = new QToolBar(tr("图元"));
    mItemToolBar->setGeometry(QRect(0, 0, 16, 600));
    addToolBar(Qt::BottomToolBarArea, mItemToolBar);
}

void Maker::fStyle()
{
    DialogStyle *mStyle = new DialogStyle(fItemSelected(), mServiceList, this);

    if(mStyle->mInit)
    {
        mStyle->exec();
    }
    else
    {
        QMessageBox::warning(this, tr("提示"), tr("选择一个图形再进行操作"));
    }
qDebug()<<"fdsfdsg";
    delete mStyle;
}

void Maker::fCut()
{
    if(NULL == mCurrentScene)
    {
        return;
    }

    mCurrentScene->fGetItemsSelected(mCopyList);

    foreach (TItem *mItem, mCopyList)
    {
        mCurrentScene->removeItem(mItem);
    }
}

void Maker::fCopy()
{
    if(NULL == mCurrentScene)
    {
        return;
    }

    mCurrentScene->fGetItemsSelected(mCopyList);
}

void Maker::fPaste()
{
    if(NULL == mCurrentScene)
    {
        return;
    }

    fCopyItemListToScene(mCopyList);
}

void Maker::fDel()
{
    if(NULL == mCurrentScene)
    {
        return;
    }

    QList<TItem *> items;
    mCurrentScene->fGetItemsSelected(items);

    TUndoAddDel *Undo = new TUndoAddDel(mCurrentScene, items);
    Undo->fSetOperation(TUndoAddDel::OPERATION_DEL);
    // Undo模块添加时会自动执行一次，把item从scene删除
    mUndoStack->push(Undo);

    // 加入Undo以后，删除以后不释放
    /*
    // 更新复制列表
    if(mCopyList.count() > 0)
    {
        foreach (TItem *mItem, items)
        {
            mCopyList.removeOne(mItem);
        }
    }

    qDeleteAll(items);
    */
}

void Maker::fMerge()
{
    if(NULL == mCurrentScene)
    {
        return;
    }

    QList<TItem *> ItemsList;
    mCurrentScene->fGetItemsSelected(ItemsList);

    //qDebug() << items.length() << items.size();

    if(ItemsList.size() <= 1)
    {
        return;
    }    

    TGroup *mGroup = new TGroup();

    if(mGroup)
    {
        mGroup->fAddItems(ItemsList);
        fUpItems(mGroup);
        fAddItemToScene(mGroup);
    }
}

void Maker::fDraw(double StartX, double StartY, double StopX, double StopY)
{
    TItem *Item = NULL;

    if(mCurrentScene && NULL != mDrawSelected)
    {
        if(StartX == StopX && StartY == StopY)
        {
            StopX = StartX + 50;
            StartY = StartY + 50;
            return;
        }
        if(mCurrentScene->selectedItems().size())
        {
            return;
        }

        if(TItem::Polygon != mDrawSelected->mType)
        {
            Item = TItemFactory::fFactory(mDrawSelected->mType,
                                      StartX, StartY, StopX, StopY,
                                      mLineStyle, mLineWidth, mLineColor, mBackColor);
        }
        else if(mPolygonPoints.count() < 64)
        {
            mPolygonPoints.push_front(QPointF(StartX, StartY));
        }

        if(NULL == Item)
        {
            return;
        }

        //qDebug() << mDrawSelected << mDrawSelected->mType;
        switch(Item->fGetType())
        {
        case TItem::Table:
        {
            TTable* Table = dynamic_cast<TTable *>(Item);

            if(Table)
            {
                Table->fSetSize(5, 5);
            }
        }
        break;

        default:
            break;
        }
    }

    fAddItemToScene(Item);
}

void Maker::fMoveItemList(TScene *Scene, QList<TItem *> ItemList,
                          double StartX, double StartY, double StopX, double StopY)
{
    if(NULL == Scene || ItemList.length() <= 0)
    {
        return;
    }

    //qDebug() << "Maker::fMoveItemList" << StartX << StartY << StopX << StopY;

    TUndoMoveDrag *Undo = new TUndoMoveDrag(Scene, ItemList);
    mUndoStack->push(Undo);

    double dx = StopX - StartX;
    double dy = StopY - StartY;
    Undo->fSetVector(dx, dy, dx, dy);
}

TItem* Maker::fItemSelected() const
{
    if(NULL == mCurrentScene)
    {
        return NULL;
    }

    QList<TItem *> items;

    mCurrentScene->fGetItemsSelected(items);

    if(items.count() > 0)
    {
        return items.first();
    }
    else
    {
        return NULL;
    }
}

TGraphics* Maker::fGraphicsSelected() const
{
    if(NULL == mCurrentScene)
    {
        return NULL;
    }

    QList<TGraphics *> items;

    mCurrentScene->fGetItemsSelected(items);

    if(items.count() > 0)
    {
        return items.first();
    }
    else
    {
        return NULL;
    }
}

TWidgets* Maker::fWidgetsSelected() const
{
    if(NULL == mCurrentScene)
    {
        return NULL;
    }

    QList<TWidgets *> items;

    mCurrentScene->fGetItemsSelected(items);

    if(items.count() > 0)
    {
        return items.first();
    }
    else
    {
        return NULL;
    }
}

void Maker::fUpdateDrawAction()
{
    TAction *mClick = (TAction *)sender();

    if(NULL != mDrawSelected)
    {        
        mDrawSelected->setChecked(false);
    }

    if(mClick == mDrawSelected)
    {
        //mView->setDragMode(QGraphicsView::NoDrag);
        mDrawSelected = NULL;
    }
    else
    {
        mDrawSelected = mClick;
#if false
        if(NULL != mDrawSelected)
        {
            if(TAction::Free != mDrawSelected->mType)
            {
                mView->setDragMode(QGraphicsView::RubberBandDrag);
            }
            else
            {
                mView->setDragMode(QGraphicsView::NoDrag);
            }
        }
#endif
    }
}

void Maker::fAttribute()
{
    /*TItem *Item = fItemSelected();

    if(NULL == Item)
    {
        QMessageBox::warning(this, tr("提示"), tr("未选中图形，或者选中图形不支持该操作"));
        return;
    }

    DialogPlay *Play = new DialogPlay(Item, mServiceList, this);

    if(Play)
    {
        if(Play->mInit)
        {
            Play->exec();
        }
        else
        {
            QMessageBox::warning(this, tr("提示"), tr("未知错误，请重试"));
        }

        delete Play;
    }*/
    TabWidgetReport *widget=new TabWidgetReport(0);
    widget->reportName=this->reportName;
    widget->content=this->content;
    widget->exec();
    this->reportName=widget->reportName;
    this->content=widget->content;

    delete widget;

}

/*void Maker::fLinkage()
{
    if(mSceneList.length() <= 0 || mServiceList.length() <= 0)
    {
        QMessageBox::warning(this, tr("提示"), tr("请至少新建一个画布和一个服务"));
        return;
    }

    DialogLinkage *ShowLinkage = new DialogLinkage(mSceneList, mServiceList, mLinkageList, this);

    if(ShowLinkage->mInit)
    {
        ShowLinkage->exec();

        if(ShowLinkage->mSave)
        {
            foreach(TLinkage *Linkage, mLinkageList)
            {
                mLinkageList.removeOne(Linkage);
                delete Linkage;
            }

            //qDebug() << "fLinkage" << "ShowLinkage->mLinkageList" << ShowLinkage->mLinkageList.length();
            foreach(TLinkage *Linkage, ShowLinkage->mLinkageList)
            {
                mLinkageList.push_back(new TLinkage(Linkage));
            }
        }
    }
    else
    {
        QMessageBox::warning(this, tr("提示"), tr("未知错误，请重试"));
    }

    delete ShowLinkage;
}

void Maker::fJsHelper()
{
    DialogJsHelper *JsHelper = new DialogJsHelper(this);

    if(JsHelper)
    {
        JsHelper->exec();

        delete JsHelper;
    }
}
*/
void Maker::fUpdateLineColor()
{
    if(TBase::fChooseColor(mLineColor, this))
    {
        TBase::fColorLabel(mLabelLineColor, mLineColor);
    }
}

void Maker::fUpdateBackColor()
{
    if(TBase::fChooseColor(mBackColor, this))
    {
        TBase::fColorLabel(mLabelBackColor, mBackColor);

    }
}

void Maker::fTop()
{
    if(NULL == mCurrentScene)
    {
        return;
    }

    QList<TItem *> ItemList;
    mCurrentScene->fGetItemsSelected(ItemList);

    foreach(TItem *Item, ItemList)
    {
        fUpItems(Item);
    }
}

void Maker::fUpItems(TItem *Item)
{
    Item->fSetZValue(++mZMax);
}

void Maker::fBottom()
{
    if(NULL == mCurrentScene)
    {
        return;
    }

    QList<TItem *> ItemList;
    mCurrentScene->fGetItemsSelected(ItemList);

    foreach(TItem *Item, ItemList)
    {
        Item->fSetZValue(--mZMin);
    }
}

void Maker::fMove(double Dx1, double Dy1, double Dx2, double Dy2)
{
    if(NULL == mCurrentScene)
    {
        return;
    }

    QList<TItem *> ItemList;
    mCurrentScene->fGetItemsSelected(ItemList);

    foreach(TItem *Item, ItemList)
    {
        Item->fMoveTo(Item->fGetStartX() + Dx1, Item->fGetStartY() + Dy1,
                      Item->fGetStopX() + Dx2, Item->fGetStopY() + Dy2);
    }
}
void Maker::fAlignBottom()
{
    if(NULL == mCurrentScene)
    {
        return;
    }
    double maxStopY=0;
    QList<TItem *> ItemList;
    mCurrentScene->fGetItemsSelected(ItemList);
    maxStopY=ItemList.first()->fGetStopY();
    foreach(TItem *Item, ItemList)
    {
        if(Item->fGetStopY()>maxStopY)
        {
            maxStopY=Item->fGetStopY();
        }
    }
    foreach(TItem *Item, ItemList)
    {
        Item->fMoveTo(Item->fGetStartX(), Item->fGetStartY() + maxStopY-Item->fGetStopY(),
                      Item->fGetStopX(), Item->fGetStopY() + maxStopY-Item->fGetStopY());
    }

}
void Maker::fAlignLeft()
{
    if(NULL == mCurrentScene)
    {
        return;
    }
    double maxStartX=0;
    QList<TItem *> ItemList;
    mCurrentScene->fGetItemsSelected(ItemList);
    maxStartX=ItemList.first()->fGetStartX();
    foreach(TItem *Item, ItemList)
    {
        if(Item->fGetStartX()<maxStartX)
        {
            maxStartX=Item->fGetStartX();
        }
    }
    foreach(TItem *Item, ItemList)
    {
        Item->fMoveTo(Item->fGetStartX()-(Item->fGetStartX()-maxStartX), Item->fGetStartY() ,
                      Item->fGetStopX()-(Item->fGetStartX()-maxStartX), Item->fGetStopY());
    }
}
void Maker::fAlignTop()
{
    if(NULL == mCurrentScene)
    {
        return;
    }
    double maxStartY=0;
    QList<TItem *> ItemList;
    mCurrentScene->fGetItemsSelected(ItemList);
    maxStartY=ItemList.first()->fGetStartY();
    foreach(TItem *Item, ItemList)
    {
        if(Item->fGetStartY()<maxStartY)
        {
            maxStartY=Item->fGetStartY();
        }
    }
    foreach(TItem *Item, ItemList)
    {
        Item->fMoveTo(Item->fGetStartX(), Item->fGetStartY() - (Item->fGetStartY()-maxStartY),
                      Item->fGetStopX(), Item->fGetStopY() - (Item->fGetStartY()-maxStartY));
    }
}
void Maker::fAlignRight()
{
    if(NULL == mCurrentScene)
    {
        return;
    }
    double maxStartX=0;
    QList<TItem *> ItemList;
    mCurrentScene->fGetItemsSelected(ItemList);
    maxStartX=ItemList.first()->fGetStopX();
    foreach(TItem *Item, ItemList)
    {
        if(Item->fGetStopX()>maxStartX)
        {
            maxStartX=Item->fGetStopX();
        }
    }
    foreach(TItem *Item, ItemList)
    {
        Item->fMoveTo(Item->fGetStartX()+(maxStartX-Item->fGetStopX()), Item->fGetStartY() ,
                      Item->fGetStopX()+(maxStartX-Item->fGetStopX()), Item->fGetStopY());
    }
}
void Maker::fUp()
{
    fMove(0, -1, 0, -1);
}

void Maker::fDown()
{
    fMove(0, 1, 0, 1);
}

void Maker::fLeft()
{
    fMove(-1, 0, -1, 0);
}

void Maker::fRight()
{
    fMove(1, 0, 1, 0);
}

void Maker::fLineStyleChanged(int Style)
{
    mLineStyle = Qt::PenStyle(Style);
}

void Maker::fLineWidthChanged(int Width)
{
    mLineWidth = Width;
    TBase::fColorLabel(mLabelLineWidth, Qt::black, mLabelLineWidth->width(), mLineWidth);
}

/*void Maker::fRunMode()
{
    DialogRunMode *RunModeShow = new DialogRunMode(mRunMode, this);

    if(RunModeShow)
    {
        RunModeShow->exec();

        delete RunModeShow;
    }
}*/
void Maker::fScenseList()
{
    ScenseListDialog *dialog=new ScenseListDialog(mSceneList, this);
    dialog->exec();
    delete dialog;
}

void Maker::fService()
{
    if(mSceneList.length() <= 0)
    {
        QMessageBox::warning(this, tr("提示"), tr("请至少新建一个画布"));
        return;
    }

    //qDebug() << "mServiceList.length" << mServiceList.length();

    DialogService *ShowService = new DialogService(mServiceList, this);

    if(ShowService)
    {
        ShowService->exec();

        if(ShowService->mSave)
        {
            foreach(TService *Service, mServiceList)
            {
                mServiceList.removeOne(Service);
                delete Service;
            }

            //qDebug() << "ShowService->mServiceList.length" <<ShowService->mServiceList.length();

            foreach(TService *Service, ShowService->mServiceList)   //q
            {
                mServiceList.push_back(new TService(Service));
            }

            //qDebug() << "save mServiceList.length" << mServiceList.length();
        }

        delete ShowService;
    }
}
void Maker::fGroupId()
{
    if(NULL == mCurrentScene)
    {
        return;
    }

    QList<TGraphics *> items;
    mCurrentScene->fGetItemsSelected(items);
    if(items.size()>0)
    {
        //int groupId=QInputDialog::getInt(this, tr("输入组号"), tr("组号"), 0);
        DialogAddGroup *addGroup=new DialogAddGroup(this);
        addGroup->exec();
        if(addGroup->groupId>0)
        {
            foreach(TGraphics *grap, items)
            {
                grap->fSetGroupId(addGroup->groupId);
                grap->fSetGroupName(addGroup->groupName);
            }
        }
        else
        {
            QMessageBox::warning(this, tr("提示"), tr("请输入大于0的组号"));
        }
        delete addGroup;
    }
    else
    {
        QMessageBox::warning(this, tr("提示"), tr("请选择至少一个图形项"));
    }
}
void Maker::fCopyScence()
{
    copyScense=true;
    TScene *AddScene = fNewScene(++mSceneId, mCurrentScene->fGetName()+tr("副本")+QString::number(mSceneId), mCurrentScene->fGetBackColor());

    if(AddScene)
    {
        QList<TItem *>mCopyList;
        if(mCurrentScene!=NULL)
        {

            mCurrentScene->fGetItemsList(mCopyList);

            //fCopyItemListToScene(mCopyList);
            QList<TItem *> List;

            foreach(TItem *Item, mCopyList)
            {
                List.push_back(fCopyItem(Item));
            }

            fAddItemList(AddScene, List);

        }
    }
    copyScense=false;
}

void Maker::fDbMsg()
{
    DialogDataBase *DataBaseShow = new DialogDataBase(mDBMsg, this);

    if(DataBaseShow)
    {
        DataBaseShow->exec();
        delete DataBaseShow;
    }
}
void Maker::fTimerMake()
{
    if(!mCurrentScene) {
        return;
    }
    TimerItemShow *itemSHow=new TimerItemShow(mCurrentScene->TImerStructList, tr("定时配置"),this);
    itemSHow->exec();
    if(itemSHow->mSave) {
        mCurrentScene->TImerStructList=itemSHow->timerstruct;
    }
    delete itemSHow;
    //qDebug()<<mCurrentScene->TImerStructList.size();
}
void Maker::fCurverConfigure()
{
    TimerItemShow *itemSHow=new TimerItemShow(this->CurverStructList, tr("曲线配置"),this);
    itemSHow->exec();
    if(itemSHow->mSave) {
        this->CurverStructList=itemSHow->timerstruct;
    }
    delete itemSHow;
}
void Maker::fPieConfigure()
{
    TimerItemShow *itemSHow=new TimerItemShow(this->PieStructList, tr("饼图配置"),this);
    itemSHow->exec();
    if(itemSHow->mSave) {
        this->PieStructList=itemSHow->timerstruct;
    }
    delete itemSHow;
}

void Maker::fFinshPolygon()
{
    if(!mActionDrawPolygon->isChecked())
    {
        TItem *Item = new TPolygon(mPolygonPoints, mLineStyle, mLineWidth, mLineColor, mBackColor);

        mPolygonPoints.clear();

        fAddItemToScene(Item);
    }
}

bool Maker::fSaveToFile(const QString FileName)
{
    TXmlWriter mXml;

    mXml.fAddHead();

    // 添加正文
    // scada
    QDomElement mXmlRootScada = mXml.fAddRoot(tr("scada"));

    // DB
    QDomElement mXmlDb = mXml.fAddChild(mXmlRootScada, tr("db"));
    if(mDBMsg)
    {
        mXmlDb.setAttribute(tr("Type"), mDBMsg->fGetType());
        mXmlDb.setAttribute(tr("Host"), mDBMsg->fGetHost());
        mXmlDb.setAttribute(tr("DBName"), mDBMsg->fGetDBName());
        mXmlDb.setAttribute(tr("User"), mDBMsg->fGetUserName());
        mXmlDb.setAttribute(tr("PassWd"), mDBMsg->fGetPassWord());
    }
    //Curver
    QDomElement mXmlCurver = mXml.fAddChild(mXmlRootScada, tr("Curver"));
    mXmlCurver.setAttribute(tr("CurverStructList"), this->CurverStructList.size());
    int i=0;
    foreach (TScene::TimerStruct contral, this->CurverStructList) {
        mXmlCurver.setAttribute(tr("CurverTimerId")+QString::number(i), contral.TimerId);
        mXmlCurver.setAttribute(tr("CurverserverId")+QString::number(i), contral.serverId);
        mXmlCurver.setAttribute(tr("CurvermainId")+QString::number(i), contral.mainId);
        mXmlCurver.setAttribute(tr("CurverdevId")+QString::number(i), contral.devId);
        mXmlCurver.setAttribute(tr("CurverstartAddr")+QString::number(i), contral.startAddr);
        i++;
    }
    // Pie
    QDomElement mXmlPie = mXml.fAddChild(mXmlRootScada, tr("Pie"));
    mXmlPie.setAttribute(tr("PieStructList"), this->PieStructList.size());
    i=0;
    foreach (TScene::TimerStruct contral, this->CurverStructList) {
        mXmlPie.setAttribute(tr("PieTimerId")+QString::number(i), contral.TimerId);
        mXmlPie.setAttribute(tr("PieserverId")+QString::number(i), contral.serverId);
        mXmlPie.setAttribute(tr("PiemainId")+QString::number(i), contral.mainId);
        mXmlPie.setAttribute(tr("PiedevId")+QString::number(i), contral.devId);
        mXmlPie.setAttribute(tr("PiestartAddr")+QString::number(i), contral.startAddr);
        i++;
    }
    // scene
    QDomElement mXmlScenes = mXml.fAddChild(mXmlRootScada, tr("scenes"));
    mXmlScenes.setAttribute(tr("Host"), mSceneHost);

    foreach (TScene *AddScene, mSceneList)
    {
        QDomElement mXmlScene = mXml.fAddChild(mXmlScenes, tr("scene"));
        mXmlScene.setAttribute(tr("Id"), AddScene->fGetId());
        mXmlScene.setAttribute(tr("Name"), AddScene->fGetName());
        mXmlScene.setAttribute(tr("PosX"), AddScene->fGetPosX());
        mXmlScene.setAttribute(tr("PosY"), AddScene->fGetPosY());
        mXmlScene.setAttribute(tr("Width"), AddScene->fGetWidth());
        mXmlScene.setAttribute(tr("Height"), AddScene->fGetHeight());
        /**********************************************************/
        mXmlScene.setAttribute(tr("TImerStructList"), AddScene->TImerStructList.size());
        int i=0;
        foreach (TScene::TimerStruct contral, AddScene->TImerStructList) {
            mXmlScene.setAttribute(tr("sceneTimerId")+QString::number(i), contral.TimerId);
            mXmlScene.setAttribute(tr("sceneserverId")+QString::number(i), contral.serverId);
            mXmlScene.setAttribute(tr("scenemainId")+QString::number(i), contral.mainId);
            mXmlScene.setAttribute(tr("scenedevId")+QString::number(i), contral.devId);
            mXmlScene.setAttribute(tr("scenestartAddr")+QString::number(i), contral.startAddr);
            //mXmlScene.setAttribute(tr("scenewriteNum")+QString::number(i), contral.writeNum);
            i++;
        }
        /**********************************************************/

        QDomElement XmlColor = mXml.fAddChild(mXmlScene, tr("BackColor"));
        XmlColor.setAttribute(tr("r"), AddScene->fGetBackColor().red());
        XmlColor.setAttribute(tr("g"), AddScene->fGetBackColor().green());
        XmlColor.setAttribute(tr("b"), AddScene->fGetBackColor().blue());

        QList<TItem *>ItemList;
        AddScene->fGetItemsList(ItemList);

        foreach(TItem *Item, ItemList)
        {
            mXml.fAddItem(mXmlScene, Item);
        }
    }

    // Service
    QDomElement mXmlServices = mXml.fAddChild(mXmlRootScada, tr("services"));
    for(int i = 0; i < mServiceList.count(); ++i)
    {
        TService *ServiceMsg = mServiceList.at(i);
        QDomElement mXmlAddService = mXml.fAddChild(mXmlServices, tr("service"));
        mXmlAddService.setAttribute(tr("Name"), ServiceMsg->fGetName());
        mXmlAddService.setAttribute(tr("Id"), ServiceMsg->fGetId());
        mXmlAddService.setAttribute(tr("Port"), ServiceMsg->fGetPort());
        mXmlAddService.setAttribute(tr("ConnectMode"), ServiceMsg->fGetConnectMode());
        mXmlAddService.setAttribute(tr("RecvMode"), ServiceMsg->fGetRecvMode());
        mXmlAddService.setAttribute(tr("AgreeMent"), ServiceMsg->fGetAgreeMent());

        switch(ServiceMsg->fGetPort())
        {
            // 网络
        case TService::Port_Net_Client_Tcp:
        case TService::Port_Net_Client_Udp:
        {
            TClientData *ClientData = ServiceMsg->fGetClientData();
            if(ClientData)
            {
                mXmlAddService.setAttribute(tr("NetIp"), ClientData->fGetIp());
                mXmlAddService.setAttribute(tr("NetPort"), ClientData->fGetPort());
            }
        }
        break;

        case TService::Port_Net_Server_Tcp:
        case TService::Port_Net_Server_Udp:
        {
            TServerData *ServerData = ServiceMsg->fGetServerData();
            if(ServerData)
            {
                mXmlAddService.setAttribute(tr("NetIp"), ServerData->fGetIp());
                mXmlAddService.setAttribute(tr("NetPort"), ServerData->fGetPort());
                mXmlAddService.setAttribute(tr("IoThreadSum"), ServerData->fGetIoThreadSum());
                mXmlAddService.setAttribute(tr("ProcessThreadSum"), ServerData->fGetProcessThreadSum());
            }
        }
        break;

            // 串口
        case TService::Port_Com:
        {
            TComData *ComData = ServiceMsg->fGetComData();

            if(ComData)
            {
                //mXmlAddService.setAttribute(tr("ComMode"), ComData->mMode);
                mXmlAddService.setAttribute(tr("ComName"), ComData->fGetName());
                mXmlAddService.setAttribute(tr("ComRate"), ComData->fGetRate());
                mXmlAddService.setAttribute(tr("ComData"), ComData->fGetDataBits());
                mXmlAddService.setAttribute(tr("ComStop"), ComData->fGetStopBits());
                mXmlAddService.setAttribute(tr("ComCheck"), ComData->fGetCheck());
            }
         }
         break;

            default:
            break;
        }

        switch(ServiceMsg->fGetAgreeMent())
        {
        case TService::AgreeMent_Iec104:
        {
            TIec104Data *Iec104Data = ServiceMsg->fGetIec104Data();

            if(Iec104Data)
            {
                mXmlAddService.setAttribute(tr("Iec104Addr"), Iec104Data->mAddr);
                mXmlAddService.setAttribute(tr("Iec104UnMaxSum"), Iec104Data->mUnMaxSum);
                mXmlAddService.setAttribute(tr("Iec104UnMaxTime"), Iec104Data->mUnMaxTime);
                mXmlAddService.setAttribute(tr("Iec104AppAddr"), Iec104Data->mAppAddr);
                mXmlAddService.setAttribute(tr("Iec104MsgAddr"), Iec104Data->mMsgAddr);
                mXmlAddService.setAttribute(tr("Iec104SendReason"), Iec104Data->mSendReason);
                mXmlAddService.setAttribute(tr("Iec104TimeLen"), Iec104Data->mTimeLen);
            }
        }
        break;

        case TService::AgreeMent_Modebus_RTU:
        {
            TModebusRtuData *RtuData = ServiceMsg->fGetModebusRtuData();

            if(RtuData)
            {
                int i = 0;
                foreach(TModebusRtuNode *Node, RtuData->mList)
                {
                    QDomElement XmlHost = mXml.fAddChild(mXmlAddService, tr("Host%1").arg(i++));

                    XmlHost.setAttribute(tr("Host"), Node->mHost);
                    XmlHost.setAttribute(tr("Fun"), Node->mFun);
                    XmlHost.setAttribute(tr("StartAddr"), Node->mStartAddr);
                    XmlHost.setAttribute(tr("RegCount"), Node->mRegCount);
                    XmlHost.setAttribute(tr("DataLen"), Node->mDataLen);
                    XmlHost.setAttribute(tr("Type"), Node->mType);
                }
            }
        }
        break;

        default:
            break;
        }
    }

    // Linkage
    QDomElement mXmlLinkages = mXml.fAddChild(mXmlRootScada, tr("Linkages"));

    foreach(TLinkage *Linkage, mLinkageList)
    {
        QDomElement XmlAddLinkage = mXml.fAddChild(mXmlLinkages, tr("linkage"));
        XmlAddLinkage.setAttribute(tr("SrcServiceId"), Linkage->mSrcServiceId);
        XmlAddLinkage.setAttribute(tr("SrcNodeId"), Linkage->mSrcNodeId);
        XmlAddLinkage.setAttribute(tr("DstServiceId"), Linkage->mDstServiceId);
        XmlAddLinkage.setAttribute(tr("DstNodeId"), Linkage->mDstNodeId);
    }

    if(!mXml.fSave(FileName))
    {
        QMessageBox::warning(this, tr("保存"), tr("保存失败，请重试"));
    }

    fUpdateProjectName();

    return true;
}

void Maker::fSaveAs()
{
    QString FileDir = QFileDialog::getSaveFileName(this, tr("另存为"));

    if(FileDir.length() <= 0)
    {
        return;
    }

    if(!FileDir.endsWith(tr(".xml")))
    {
        FileDir += tr(".xml");
    }

    mProjectName = FileDir;

    fSaveToFile(mProjectName);
}

bool Maker::fConnectToDB()
{
    bool Ret = false;

    if(mDataBase && mDataBase->fIsOpen())
    {
        return true;
    }
    else if(mDataBase)
    {
        delete mDataBase;
        mDataBase = NULL;
    }

    if(!mDBMsg->fIsAvailable())
    {
        fDbMsg();
    }

    mDataBase = TDataBaseService::fCreateDataBase(mDBMsg);

    if(mDataBase)
    {
        Ret = mDataBase->fInit(mDBMsg->fGetHost(),
                               mDBMsg->fGetDBName(),
                               mDBMsg->fGetUserName(),
                               mDBMsg->fGetPassWord());
    }

    return Ret;
}

void Maker::fSaveToDB()
{
    if(fConnectToDB())
    {
        TDataBaseService::fClear(mDataBase);
        TDataBaseService::fSaveService(mDataBase, mServiceList);
        TDataBaseService::fSaveScene(mDataBase, mSceneList);
        TDataBaseService::fSaveLinkage(mDataBase, mLinkageList);
    }
}

void Maker::fLoadFromDB()
{
    if(!fCloseAll())
    {
        return;
    }

    if(fConnectToDB())
    {
        TDataBaseService::fLoadService(mDataBase, mServiceList);
        TDataBaseService::fLoadScene(mDataBase, mSceneList);
        TDataBaseService::fLoadLinkage(mDataBase, mLinkageList);

        UpdateAfterOpen();
    }
}

void Maker::fUpdateProjectName()
{
    setWindowTitle(tr("组态制作程序 (") + mProjectName + tr(")"));
}

void Maker::fSave()
{
    QString SaveName = mProjectName;

    if(SaveName.length() <= 0)
    {
        fSaveAs();
    }
    else
    {
        fSaveToFile(mProjectName);
    }
}

void Maker::fSaveItems()
{
    QList <TItem *>ItemList;
    mCurrentScene->fGetItemsSelected(ItemList);

    if(ItemList.count() < 1)
    {
        QMessageBox::warning(this, tr("提示"), tr("选中要保存的图形，再进行操作"));
        return;
    }

    QString mFileDir = QFileDialog::getSaveFileName(this, tr("保存"), QApplication::applicationDirPath() + tr("//items//"), tr("*.xml"));

    if(mFileDir.length() <= 0)
    {
        return;
    }

    if(!mFileDir.endsWith(tr(".xml")))
    {
        mFileDir += tr(".xml");
    }

    TXmlWriter mXml;

    mXml.fAddHead();
    QDomElement mXmlItems = mXml.fAddRoot(tr("items"));

    foreach(TItem *mItem, ItemList)
    {
        mXml.fAddItem(mXmlItems, mItem);
    }

    if(!mXml.fSave(mFileDir))
    {
        QMessageBox::warning(this, tr("保存图元"), tr("保存失败，请重试"));
    }
    else
    {
        fUpdateItems();
    }
}

void Maker::UpdateAfterOpen()
{
    // 解析完成,添加动作
    foreach(TScene *Scene, mSceneList)
    {
        if(Scene->fGetId() > mSceneId)
        {
            mSceneId = Scene->fGetId();
        }

        fSetScene(Scene);

        QList<TItem *>ItemList;
        Scene->fGetItemsList(ItemList);

        foreach(TItem *Item, ItemList)
        {
            int zLayer = Item->fGetZValue();

            fSetItem(Item);

            if(zLayer > mZMax)
            {
                mZMax = zLayer;
            }
            else if(zLayer < mZMin)
            {
                mZMin = zLayer;
            }
        }
    }

    mCurrentScene = mSceneList.first();

    if(mCurrentScene)
    {
        mSceneHost = mCurrentScene->fGetId();
        mView->setScene(mCurrentScene);

        fUpdateWindowAction((TAction *)mCurrentScene->fGetUserData(), true);
    }
}

void Maker::fOpen()
{
    if(!fCloseAll())
    {
        return;
    }

    //开始打开操作/////////////////////////////////////////////

    QString FileDir = QFileDialog::getOpenFileName(this, tr("打开"),
                                                   QApplication::applicationDirPath(),
                                                   tr("图形文件(*.xml);;所有文件(*.*)"));

    qDebug() << "mFileDir" << FileDir;

    if(tr("") == FileDir)
    {
        return;
    }

    TXmlReader mXml;

    if(!mXml.fOpen(FileDir))
    {
        QMessageBox::warning(this, tr("初始化"), tr("打开或解析错误"));
        return;
    }

    mProjectName = FileDir;
    fUpdateProjectName();

    //解析界面图形
    mXml.fAnalyCurver(this->CurverStructList);
    mXml.fAnalyPie(this->PieStructList);
    mXml.fAnalyScene(mSceneList);

    UpdateAfterOpen();

    mXml.fAnalyService(mServiceList);

    mXml.fAnalyLinkage(mLinkageList);

    mXml.fAnalyDB(mDBMsg);

    //mXml.fAnalyRunMode(mRunMode);

    //mXml.fAnalySubWidgets(mSceneList);
}

void Maker::fAddItemToBar(QString Dir, QString FileName)
{
    TXmlReader mXml;

    if(!mXml.fOpen(Dir + FileName))
    {
        return;
    }

    QDomElement mXmlNode = mXml.fGetRoot();

    if(mXmlNode.tagName() != tr("items"))
    {
        return;
    }

    mXmlNode = mXmlNode.firstChild().toElement();

    // 图元名 <= 4
    QString ItemName;

    if(FileName.length() <= 8)
    {
        // -4 表示减去.xml的长度
        ItemName = FileName.mid(0, FileName.length() - 4);
    }
    else
    {
        ItemName = FileName.mid(0, 4);
    }

    TAction *mAction = fAddFileAction(ItemName);
    while(!mXmlNode.isNull())
    {
        TItem *mItem = mXml.fAnalyItem(mXmlNode);

        if(mItem)
        {
            mAction->mItemList.push_back(mItem);
        }

        mXmlNode = mXmlNode.nextSibling().toElement();
    }

    connect(mAction, SIGNAL(triggered()), this, SLOT(fDrawItem()));
    mItemToolBar->addAction(mAction);
}

void Maker::fUpdateItems()
{
    if(!mItemToolBar)
    {
        return;
    }
    else if(mItemToolBar->actions().length() > 0)
    {
        foreach(QAction *Action, mItemToolBar->actions())
        {
            TAction *mAction = dynamic_cast<TAction *>(Action);
            mItemToolBar->removeAction(Action);
            delete mAction;
        }
    }

    QString Dir = QApplication::applicationDirPath() + tr("//items//");
    qDebug()<<Dir;

    QDir scriptsDir(Dir);
    QStringList FileNames = scriptsDir.entryList(QStringList("*.xml"), QDir::Files);    

    foreach (QString FileName, FileNames)
    {
        //fAddItemToBar(Dir, FileName);
    }
}

void Maker::fDrawItem()
{
    TAction *mAction = qobject_cast<TAction *>(sender());

    if(mAction && mCurrentScene)
    {
        fCopyItemListToScene(mAction->mItemList);
    }
}

void Maker::fClose()
{
    if(0 != QMessageBox::information(this, tr("提示"), tr("关闭当前页面，将丢失页面上所有图形，且不可恢复，是否继续关闭"), tr("是"), tr("否")))
    {
        return;
    }

    if(mSceneHost == mCurrentScene->fGetId())
    {
        mSceneHost = 0;
    }

    TAction *FileAction = (TAction *)mCurrentScene->fGetUserData();

    if(FileAction)
    {
        mWindowMenu->actions().removeOne(FileAction);
        delete FileAction;
    }

    //qDebug() << "fClose remove scene";
    mSceneList.removeOne(mCurrentScene);
    delete mCurrentScene;
    mCurrentScene = 0;

    //qDebug() << "fClose " << mWindowMenu->actions().length();

    if(mWindowMenu->actions().length() > mDefaltWinActSum)
    {
        TAction *Action = dynamic_cast<TAction *>(mWindowMenu->actions().at(mDefaltWinActSum));

        if(Action)
        {
            Action->setChecked(true);
            mCurrentScene = Action->mScene;
            mView->setScene(mCurrentScene);
        }
    }
    else
    {
        mSceneHost = 0;
        mSceneId = 0;
    }
}

bool Maker::fCloseAll()
{
    if(mSceneList.count() > 0)
    {
        if(0 != QMessageBox::information(this, tr("提示"), tr("执行该操作，将丢失当前所有图像和服务设置，且不可恢复，是否继续"), tr("是"), tr("否")))
        {
            return false;
        }
    }

    mProjectName = tr("");
    fUpdateProjectName();

    // 清空Undo列表
    mUndoStack->clear();

    // 清空Scene列表
    foreach(TScene *Scene, mSceneList)
    {
        mSceneList.removeOne(Scene);
        delete Scene;
    }

    mCurrentScene = NULL;

    // 清空服务列表
    foreach(TService *Service, mServiceList)
    {
        mServiceList.removeOne(Service);
        delete Service;
    }

    // 清空联动列表
    foreach(TLinkage *Linkage, mLinkageList)
    {
        mLinkageList.removeOne(Linkage);
        delete Linkage;
    }

    //qDebug() << "actions count 0" << mWindowMenu->actions().count();

    while(mWindowMenu->actions().count() > mDefaltWinActSum)
    {
        //qDebug() << "actions count 1" << mWindowMenu->actions().count();

        TAction *Action = dynamic_cast<TAction *>(mWindowMenu->actions().at(mDefaltWinActSum));

        //qDebug() << "Action" << Action;

        if(Action)
        {
            mWindowMenu->removeAction(Action);

            delete Action;
        }
    }

    //qDebug() << "actions count 2" << mWindowMenu->actions().count();

    mSceneHost = 0;
    mSceneId = 0;

    return true;
}

TItem* Maker::fCopyItem(TItem *SourceItem)
{
    if(NULL == SourceItem)
    {
        return NULL;
    }

    TItem *Item = SourceItem->fCopy();

    fSetItem(Item);

    return Item;
}

void Maker::fSetItem(TItem *Item)
{
    if(NULL == Item)
    {
        return;
    }

    connect(Item, SIGNAL(mDrop(TItem*,double,double,double,double)),
            this, SLOT(fItemDrop(TItem*,double,double,double,double)));
}

void Maker::fItemDrop(TItem *Item, double StartDx, double StartDy, double StopDx, double StopDy)
{
    if(NULL == mCurrentScene || NULL == Item)
    {
        return;
    }

    //qDebug() << "Maker::fItemDrop" << StartDx << StartDy << StopDx << StopDy;

    TUndoMoveDrag *Undo = new TUndoMoveDrag();
    mUndoStack->push(Undo);
    Undo->fSetScene(mCurrentScene);
    Undo->fAddItem(Item);
    Undo->fSetVector(StartDx, StartDy, StopDx, StopDy);
}

void Maker::fAddItemToScene(TItem *Item)
{
    if(NULL == mCurrentScene || NULL == Item)
    {
        return;
    }

    fSetItem(Item);

    QList<TItem *> ItemList;
    ItemList.push_back(Item);

    fAddItemList(mCurrentScene, ItemList);
}

void Maker::fCopyItemListToScene(QList<TItem *> &ItemList)
{
    QList<TItem *> List;

    foreach(TItem *Item, ItemList)
    {
        List.push_back(fCopyItem(Item));
    }

    fAddItemList(mCurrentScene, List);
}

void Maker::fAddItemList(TScene *Scene, QList<TItem *> &ItemList)
{
    if(NULL == Scene)
    {
        return;
    }

    Scene->clearSelection();

    foreach(TItem *Item, ItemList)
    {
        Item->setSelected(true);
    }

    TUndoAddDel *Undo = new TUndoAddDel(Scene, ItemList);
    Undo->fSetOperation(TUndoAddDel::OPERATION_ADD);
    // push时，执行一次，完成添加
    mUndoStack->push(Undo);
}

void Maker::fAddActionToMenu(QString Name, TScene *Scene)
{
    TAction *AddAction = fAddFileAction(Name.mid(0, 10));
    AddAction->mScene = Scene;
    Scene->fSetUserData(AddAction);
    connect(AddAction, SIGNAL(triggered(bool)), this, SLOT(fUpdateCurrentScene(bool)));
    connect(AddAction, SIGNAL(triggered(bool)), this, SLOT(fUpdateWindow(bool)));
    AddAction->setCheckable(true);
    if(!copyScense)
    {
        AddAction->setChecked(true);
        fUpdateWindowAction(AddAction, true);
    }
    mWindowMenu->addAction(AddAction);

    Scene->fSetUserData(AddAction);
}

TScene* Maker::fNewScene(int SceneId, QString Name, QColor BackColor,
                  double PosX, double PosY, double Width, double Height)
{
    TScene *AddScene = new TScene(PosX, PosY, Width, Height);
    AddScene->fSetId(SceneId);
    AddScene->fSetName(Name);
    AddScene->fSetBackColor(BackColor);
    //AddScene->setBackgroundBrush(BackColor);
    mSceneList.push_back(AddScene);

    fSetScene(AddScene);

    return AddScene;
}

void Maker::fSetScene(TScene *Scene)
{
    if(NULL == Scene)
    {
        return;
    }

    connect(Scene, SIGNAL(mDropFinsh(double, double, double, double)),
                        this, SLOT(fDraw(double, double, double, double)));

    fAddActionToMenu(Scene->fGetName(), Scene);

    connect(Scene, SIGNAL(mMoveItemList(TScene*,QList<TItem*>,double,double,double,double)),
            this, SLOT(fMoveItemList(TScene*,QList<TItem*>,double,double,double,double)));
}

void Maker::fNew()
{
    DialogNewScene *NewSceneShow = new DialogNewScene(this);

    if(NewSceneShow)
    {
        NewSceneShow->exec();

        if(NewSceneShow->mSave)
        {
            TScene *AddScene = fNewScene(++mSceneId, NewSceneShow->fGetName(), NewSceneShow->fGetBackColor());

            if(AddScene)
            {
                mView->setScene(AddScene);
                mCurrentScene = AddScene;
            }
        }

        delete NewSceneShow;
    }
}
void Maker::fSetCurrentScene(TScene *Scene)
{
    mView->setScene(Scene);
    mCurrentScene = Scene;
    TAction *FileAction = (TAction *)mCurrentScene->fGetUserData();

    if(FileAction)
    {
       fUpdateWindowAction(FileAction, true);
    }

}

void Maker::fEdit()
{
    if(mCurrentScene)
    {
        DialogNewScene *EditSceneShow = new DialogNewScene(this);

        if(EditSceneShow)
        {
            EditSceneShow->fSetName(mCurrentScene->fGetName());
            EditSceneShow->fSetBackColor(mCurrentScene->fGetBackColor());
            EditSceneShow->fSetTitle(tr("编辑"));

            EditSceneShow->exec();

            if(EditSceneShow->mSave)
            {
                QString NewName =  EditSceneShow->fGetName();
                if(mCurrentScene->fGetName() != NewName)
                {
                    mCurrentScene->fSetName(NewName);
                    TAction *FileAction = (TAction *)mCurrentScene->fGetUserData();

                    if(FileAction)
                    {
                        FileAction->setText(NewName);
                    }
                }

                mCurrentScene->fSetBackColor(EditSceneShow->fGetBackColor());
                //mCurrentScene->setBackgroundBrush(mCurrentScene->fGetBackColor());
            }

            delete EditSceneShow;
        }
    }
}

void Maker::fUpdateWindowAction(TAction *Action, bool Checked)
{
    if(Action)
    {
        foreach (QAction *Find, mWindowMenu->actions())
        {
            TAction *mFileAction = dynamic_cast<TAction *>(Find);

            if(mFileAction)
            {
                mFileAction->setChecked(false);
                //mFileAction->setVisible(false);  //设置不可见
            }
        }

        Action->setChecked(Checked);
       // Action->setVisible(true);
    }
}

void Maker::fUpdateCurrentScene(bool Checked)
{
    if(!Checked)
    {
        return;
    }

    TAction *Action = qobject_cast<TAction *>(sender());

    if(Action && Action->mScene)
    {
        mCurrentScene = Action->mScene;
        mView->setScene(mCurrentScene);
    }
}

void Maker::fUpdateWindow(bool Checked)
{
    fUpdateWindowAction(qobject_cast<TAction *>(sender()), Checked);
}

void Maker::fZoomOut()
{
    if(mView)
    {
        mView->fZoom(1.125);
    }
}

void Maker::fZoomIn()
{
    if(mView)
    {
        mView->fZoom(0.8);
    }
}

void Maker::fZoomNormal()
{
    if(mView)
    {
        mView->fSetZoom(1);
    }
}

void Maker::fAddLink()
{
    DialogAddLink *mLink = new DialogAddLink(fWidgetsSelected(), mSceneList, this);

    if(mLink->mInit)
    {
        mLink->exec();
    }
    else
    {
        QMessageBox::warning(this, tr("提示"), tr("选择一个按钮再进行操作"));
    }

    delete mLink;
}

/*void Maker::fGotoLink(int LinkId)
{
    foreach(TScene *Scene, mSceneList)
    {
        if(LinkId == Scene->fGetId())
        {
            mCurrentScene = Scene;
            mView->setScene(mCurrentScene);
        }
    }
}*/

void Maker::fSetHost()
{
    if(mCurrentScene)
    {
        mSceneHost = mCurrentScene->fGetId();
    }
}
