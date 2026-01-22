/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QThread>

namespace xCoAP {

class CoAPClient : public QThread
{
    Q_OBJECT
public:
    explicit CoAPClient(QObject* parent = nullptr);
    ~CoAPClient();

protected:
    void run() override;
};

} // namespace xCoAP
