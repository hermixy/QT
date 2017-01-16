#ifndef RUN_MODE_H
#define RUN_MODE_H

#include <QString>

class TRunMode
{
public:
    TRunMode();
    TRunMode(int Mode, QString Ip, int Port);
    ~TRunMode();

    int mRunMode;
    QString mIp;
    int mPort;
};

#endif // RUN_MODE_H
