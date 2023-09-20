/*********************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#ifndef SAKTOOLBOXUIFACTORY_HH
#define SAKTOOLBOXUIFACTORY_HH

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
