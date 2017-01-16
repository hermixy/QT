#ifndef DIALOG_STYLE_H
#define DIALOG_STYLE_H
#include "../../Src/Service/Service.h"
#include "../Scada/Src/Item/Item.h"
#include "../Scada/Src/TableView/TableView.h"
#include "../Scada/Src/Base/Base.h"
#include <QFileDialog>
#include <QFontComboBox>

class DialogStyle : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogStyle(TItem *Item, const QList<TService *> &ServiceList, QWidget *parent = 0);
    ~DialogStyle();

    bool mInit;
    void updateLabel();

private slots:
    void fChooseLineColor();
    void fChooseLineWidth(int Width);
    void fChooseBackColor();
    void fChooseValueColor();
    void fChoosePointerColor();
    void fClose();
    void fApply();
    void fTableSizeChange(int Value);
    void fRowLengthChange(int Len);
    void fPixmapPathChange();
    void deleteContral();
    void modSendOrder();
private:
    QTabWidget *TabWidget;
    TItem *mItem;

    // 坐标位置
    QGroupBox *mGroupBoxPos;
    QLabel *mLabelStartX;
    QDoubleSpinBox *mSpinBoxStartX;
    QLabel *mLabelStartY;
    QDoubleSpinBox *mSpinBoxStartY;
    QLabel *mlabelStopX;
    QDoubleSpinBox *mSpinBoxStopX;
    QLabel *mLabelStopY;
    QDoubleSpinBox *mSpinBoxStopY;
    // 中心
    QLabel *mLabelCenterX;
    QDoubleSpinBox *mSpinBoxCenterX;
    QLabel *mLabelCenterY;
    QDoubleSpinBox *mSpinBoxCenterY;

    QLabel *mLabelZ;
    QSpinBox *mSpinBoxZ;
    QLabel *mLabelRotateAngleShow;
    QSpinBox *mSpinBoxRotateAngle;
    QLabel *mLabelRotateXShow;
    QDoubleSpinBox *mSpinBoxRotateX;
    QLabel *mLabelRotateYShow;
    QDoubleSpinBox *mSpinBoxRotateY;

    // 样式
    QGroupBox *mGroupBoxStyle;
    //QGridLayout *mLayoutStyle;
    QLabel *mLabelLineStyleShow;
    QComboBox *mComboBoxLineStyle;
    QLabel *mLabelLineWidthShow;
    QLabel *mLabelLineWidth;
    QSpinBox *mSpinBoxLineWidth;
    QColor mLineColor;
    QLabel *mLabelLineColorShow;
    QLabel *mLabelLineColor;
    QPushButton *mButtonLineColor;
    QLabel *mLabelLineAlpha;
    QSpinBox *mSpinBoxLineAlpha;
    QColor mBackColor;
    QLabel *mLabelBackColorShow;
    QLabel *mLabelBackColor;
    QPushButton *mButtonBackColor;
    QLabel *mLabelBackAlpha;
    QSpinBox *mSpinBoxBackAlpha;

    //绑定设备
    QGroupBox *mGroupBoxConfig;
    QLabel *mLabelNameIdShow;
    QLineEdit *mLineEditNameId;
    QLabel *mLabelServiceShow;
    QComboBox *mComboBoxService;

    QLabel *mLabelModbusPro;
    QComboBox *mComboBoxModbusPro;
    QCheckBox *crcCheckBox;

    QLabel *mLabelHintShow;
    QTextEdit *mTextEditHint;
    QLabel *mLabelSubDeviceNum;
    QSpinBox *mLineEditSubDeviceNum;

    QLabel *mLabelGreaterEqual;
    QDoubleSpinBox *mLineEditGreaterEqual;
    QSpinBox *mGroupNum;
    QLabel *mLabelGreaterWrite;
    QDoubleSpinBox *mLineEditWrite;

    QLabel *mLabelLess;
    QDoubleSpinBox *mLineEditLess;
    QSpinBox *mGroupLessNum;
    QLabel *mLabelLessWrite;
    QDoubleSpinBox *mLineEditLessWrite;
    //show range
    QLabel *mLabelShowRange;
    QLabel *mLabelStartRange;
    QDoubleSpinBox *msipnBoxStartRange;
    QLabel *mLabelStopRange;
    QDoubleSpinBox *msipnBoxStopRange;

    QLabel *mLabelMainDeviceNum;
    QSpinBox *mLineEditMainDeviceNum;

    QLabel *mLabelGroupId;
    QLabel *mStartAddr;
    QLabel *mGroupName;
    QLineEdit *mLabelGroupName;
    QLineEdit *mLineEditStartAddr;
    QSpinBox *mSpinBoxGroupId;
    //联动列表
    QGroupBox *mLinkList;


    // 文字
    QGroupBox *mGroupBoxText;
    //QGridLayout *mLayoutText;
    QLabel *mLabelTextShow;
    QTextEdit *mTextEditText;
    QLabel *mLabelFontFamilyShow;
    QFontComboBox *mComboBoxFontFamily;
    QLabel *mLabelFontSizeShow;
    QSpinBox *mSpinBoxFontSize;
    QLabel *mLabelSpacingShow;
    QSpinBox *mSpinBoxSpacing;
    QCheckBox *mCheckBoxFontBlob;
    QCheckBox *mCheckBoxFontItalic;
    QCheckBox *mCheckBoxFontUnderLine;
    QCheckBox *mCheckBoxFontStrikeOut;

    // 值文字
    QGroupBox *mGroupBoxValueText;
    //QGridLayout *mLayoutValueText;
    QLabel *mLabelModeShow;
    QComboBox *mComboBoxTextMode;
    QLabel *mLabelIntLenShow;
    QSpinBox *mSpinBoxTextIntLen;
    QLabel *mLabelPointLenShow;
    QSpinBox *mSpinBoxTextPointLen;

    QLabel *mLabelUpAlarm;
    QDoubleSpinBox *mSpinBoxUpAlarm;
    QLabel *mLabelDownAlarm;
    QDoubleSpinBox *mSpinBoxDownAlarm;

    // 图片
    QGroupBox *mGroupBoxPixmap;
    //QHBoxLayout *mLayoutPixmap;
    QLabel *mLabelPixmapDirShow;
    QLineEdit *mLineEditPixmapDir;
    QPushButton *mButtonPixmap;
    QLabel *mLabelCondition;
    QComboBox *mComboxCondition;

    QLabel *mLabelPixmapDirShow1;
    QLineEdit *mLineEditPixmapDir1;
    QPushButton *mButtonPixmap1;
    QLabel *mLabelCondition1;
    QComboBox *mComboxCondition1;

    QLabel *mLabelPixmapDirShow2;
    QLineEdit *mLineEditPixmapDir2;
    QPushButton *mButtonPixmap2;
    QLabel *mLabelCondition2;
    QComboBox *mComboxCondition2;

    // 角度
    QGroupBox *mGroupBoxAngle;
    //QHBoxLayout *mLayoutAngle;
    QLabel *mLabelArcMode;
    QComboBox *mComboBoxArcMode;
    QLabel *mLabelStartAngleShow;
    QDoubleSpinBox *mSpinBoxStartAngle;
    QLabel *mLabelCoverAngleShow;
    QDoubleSpinBox *mSpinBoxCoverAngle;

    // 表格
    QGroupBox *mGroupBoxTable;
    TTableView *mTableView;
    QLabel *mLabelTableRowShow;
    QSpinBox *mSpinBoxTableRow;
    QLabel *mLabelTableRowLenShow;
    QSpinBox *mSpinBoxRowLen;
    QLabel *mLabelTableColShow;
    QSpinBox *mSpinBoxTableCol;
    QLabel *mLabelShow;

    // 按钮
    QGroupBox *mGroupBoxButton;
    QLabel *mLabelButtonTextShow;
    QLineEdit *mEditButtonText;
    QLabel *mLabelButtonBeginTime;
    QComboBox *mComBoxBeginTime;
    QTimeEdit *mTimeEditBeginTime;
    QLabel *mLabelButtonEndTime;
    QComboBox *mComBoxEndTime;
    QTimeEdit *mTimeEditEndTime;
    QLabel *mLabelButtonAutoSend;
    QComboBox *mComBoxOpenClose;

    QLabel *mLabelPixmapButtonShow;
    QLineEdit *mLineEditPixmapButtonDir;
    QPushButton *mTbuttonPixmap;

    QCheckBox *isShowTimre;

    QSpinBox *mSpinBoxViewWidth;
    QLabel *mLabelX;
    QSpinBox *mSpinBoxViewHeigth;

    // 仪表盘
    QGroupBox *mGroupBoxMeter;
    QLabel *mLabelMeterModeShow;
    QComboBox *mComboBoxMeterMode;
    QLabel *mLabelScaleSumShow;
    QSpinBox *mSpinBoxScaleSum;
    QLabel *mLabelMeterStartAngleShow;
    QDoubleSpinBox *mSpinBoxMeterStartAngle;
    QLabel *mLabelMeterStepAngleShow;
    QDoubleSpinBox *mSpinBoxMeterStepAngle;
    QLabel *mLabelMeterStartValueShow;
    QDoubleSpinBox *mSpinBoxMeterStartValue;
    QLabel *mLabelMeterStepValueShow;
    QDoubleSpinBox *mSpinBoxMeterStepValue;
    QLabel *mLabelMeterPointerColorShow;
    QLabel *mLabelMeterPointerColor;
    QPushButton *mButtonMeterPointColor;
    QColor mMeterPointerColor;

    // 进度条
    QGroupBox *mGroupBoxProcessBar;
    QLabel *mLabelProcessBarModeShow;
    QComboBox *mComboBoxProcessBarMode;
    QLabel *mLabelProcessBarScaleSumShow;
    QSpinBox *mSpinBoxProcessBarScaleSum;
    QLabel *mLabelProcessBarStartValueShow;
    QDoubleSpinBox *mSpinBoxProcessBarStartValue;
    QLabel *mLabelProcessBarStepValueShow;
    QDoubleSpinBox *mSpinBoxProcessBarStepValue;
    QLabel *mLabelProcessBarValueColorShow;
    QLabel *mLabelProcessBarValueColor;
    QPushButton *mButtonProcessBarValueColor;
    QColor mProcessBarValueColor;

    // 窗口
    QPushButton *mButtonOK;
    QPushButton *mButtonCancel;

    void fChooseColor(QLabel *Label, QColor &Color);

    void fInit();
    bool fCreateItem(const QList<TService *> &ServiceList);
    bool fCreateText();
    bool fCreateValueText();
    bool fCreatePixmap();
    bool fCreateAngle();
    bool fCreateTable();
    bool fCreateButton();
    bool fCreateMeter();
    bool fCreateProcessBar();

    bool fSaveItem();
    bool fSaveText();
    bool fSaveValueText();
    bool fSavePixmap();
    bool fSaveAngle();
    bool fSaveTable();
    bool fSaveButton();
    bool fSaveMeter();
    bool fSaveProcessBar();
};

#endif // DIALOG_STYLE_H
