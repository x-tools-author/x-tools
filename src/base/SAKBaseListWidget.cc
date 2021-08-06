/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QWidget>
#include <QListWidgetItem>

#include "SAKBaseListWidget.hh"

template <typename T>
SAKBaseListWidget<T>::SAKBaseListWidget()
    :QListWidget()
{

}

template <typename T>
void SAKBaseListWidget<T>::clearItems()
{
    QListWidget::clear();
}

template <typename T>
void SAKBaseListWidget<T>::importItems()
{
    QListWidgetItem *item = currentItem();
    if (item) {
        QWidget *itemWidget = QListWidget::itemWidget(item);
        if (itemWidget) {
            itemWidget->deleteLater();
        }
    } else {

    }
}

template <typename T>
void SAKBaseListWidget<T>::exportItems()
{

}

template <typename T>
void SAKBaseListWidget<T>::deleteItem()
{

}

template <typename T>
void SAKBaseListWidget<T>::addItem()
{

}
