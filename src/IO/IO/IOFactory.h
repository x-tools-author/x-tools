/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QObject>

namespace xTools {

class Communication;
class IOFactory : public QObject
{
    Q_OBJECT
private:
    IOFactory();
    IOFactory(const IOFactory &) = delete;
    IOFactory &operator=(const IOFactory &) = delete;

public:
    static IOFactory &singleton();
    Q_INVOKABLE Communication *createDevice(int type);
};

} // namespace xTools
