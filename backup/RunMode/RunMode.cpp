#include "RunMode.h"

TRunMode::TRunMode()
{
    mRunMode = 0;
}

TRunMode::TRunMode(int Mode, QString Ip, int Port)
{
    mRunMode = Mode;
    mIp = Ip;
    mPort = Port;
}

TRunMode::~TRunMode()
{}


