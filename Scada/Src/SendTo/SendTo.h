#ifndef SEND_TO_H
#define SEND_TO_H

#include <QObject>
#include <QString>

class TSendTo
{
public:
    TSendTo();
    TSendTo(QObject *Sender, int Type, double ValueFloat,
                int ValueInt, QString Text);

    ~TSendTo();

    enum SendToType
    {
        TYPE_UNKNOWEN       = 0,
        TYPE_YK_ONYZ        = 1,    // 遥控 预置合
        TYPE_YK_ON          = 2,    // 遥控 合
        TYPE_YK_NoYZ        = 3,    // 遥控 取消预置
        TYPE_YK_OFFYZ       = 4,    // 遥控 预置分
        TYPE_YK_OFF         = 5,    // 遥控 分
        TYPE_Start          = 101,  // 启动
        TYPE_Stop           = 102,  // 停止
        TYPE_Left           = 201,  // 左
        TYPE_Right          = 202,  // 右
        TYPE_StopMove       = 203,  // 停止移动
        TYPE_MESSAGE_INFO   = 1001, // 提示框，仅提示信息，只有一个确认键
        TYPE_MESSAGE_INPUT  = 1002  // 确认框，假如需要确认操作，需要输入确认信息，可以直接取消
    };

    void fCopy(TSendTo *Src);

    void fSetSender(QObject *Sender);
    QObject* fGetSender();
    void fSetType(int Type);
    int fGetType();
    void fSetValueFloat(double Value);
    double fGetValueFloat();
    void fSetValueInt(int ValueInt);
    int fGetValueInt();
    void fSetText(const QString &Text);
    QString fGetText();

private:
    QObject *mSender;
    int mType;
    double mValueFloat;
    int mValueInt;
    QString mText;
};

#endif // SEND_TO_H
