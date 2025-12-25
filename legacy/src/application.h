#pragma once

#include "common/xapp.h"

class Application : public xApp
{
    Q_OBJECT
public:
    Application(int &argc, char **argv);
    ~Application() override;
};