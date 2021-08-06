/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKBASELISTWIDGET_HH
#define SAKBASELISTWIDGET_HH

#include <QListWidget>

template <typename T>
class SAKBaseListWidget : public QListWidget
{
    Q_OBJECT
public:
    SAKBaseListWidget();

    void clearItems();
    void importItems();
    void exportItems();
    void deleteItem();
    void addItem();
};

#endif // SAKBASELISTWIDGET_HH
