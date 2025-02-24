/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "../xIO.h"

namespace xTools {

class CommunicationUi;
class IOUiFactory
{
private:
    IOUiFactory();
    IOUiFactory(const IOUiFactory &) = delete;
    IOUiFactory &operator=(const IOUiFactory &) = delete;

public:
    static IOUiFactory &singleton();
    CommunicationUi *createDeviceUi(int type);
};

} // namespace xTools
