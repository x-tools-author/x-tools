/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKTOOLBOXUIFACTORY_H
#define SAKTOOLBOXUIFACTORY_H

#include <QObject>

class SAKToolBoxUi;
class SAKToolBoxUiFactory : public QObject
{
    Q_OBJECT
private:
    explicit SAKToolBoxUiFactory(QObject *parent = nullptr);

public:
    static SAKToolBoxUiFactory *instance();
    QList<int> supportedTools();
    SAKToolBoxUi *createToolBoxUi(int type);

private:
};

#endif // SAKTOOLBOXUIFACTORY_H
