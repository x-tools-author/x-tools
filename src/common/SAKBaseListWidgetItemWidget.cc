/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QDateTime>
#include "SAKBaseListWidgetItemWidget.hh"

SAKBaseListWidgetItemWidget::SAKBaseListWidgetItemWidget(QWidget *parent)
    :QWidget(parent)
    ,mEnable(false)
{
    mId = QDateTime::currentMSecsSinceEpoch();
}

SAKBaseListWidgetItemWidget::SAKBaseListWidgetItemWidget(quint64 id, QWidget *parent)
    :QWidget(parent)
    ,mId(id)
    ,mEnable(false)
{

}

void SAKBaseListWidgetItemWidget::onBytesReadPrivate(QByteArray bytes)
{
    if (enable()) {
        onBytesRead(bytes);
    }
}
