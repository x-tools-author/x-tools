/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qter188/QtSwissArmyKnife
 *     https://gitee.com/qter188/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is qter188@outlook.com.
 */

#include <QApplication>
#include <QStyleFactory>

#include "QtAppStyleApi.hh"

QtAppStyleApi *QtAppStyleApi::_this = nullptr;
QtAppStyleApi *QtAppStyleApi::instance()
{
    if (_this){
        return _this;
    }else{
        new QtAppStyleApi;
        return _this;
    }
}

QList<QAction *> QtAppStyleApi::actions()
{
    return appStyleActions;
}

QtAppStyleApi::QtAppStyleApi(QObject *parent)
    :QObject (parent)
{
    _this = this;

    QAction *styleAction = nullptr;
    QActionGroup *styleActionGroup = new QActionGroup(this);
    QStringList styleKeys = QStyleFactory::keys();
    for (QString style : styleKeys) {
        styleAction = new QAction(style);
        styleAction->setCheckable(true);
        styleAction->setObjectName(style);
        styleAction->setActionGroup(styleActionGroup);

        appStyleActions.append(styleAction);
        connect(styleAction, &QAction::triggered, this, &QtAppStyleApi::changeAppStyle);
    }
}

void QtAppStyleApi::setStyle(QString style)
{
    if (style.isEmpty()){
        style = QStyleFactory::keys().first();
    }

    for(auto var:_this->appStyleActions){
        if (var->objectName() == style){
            var->setChecked(true);
            emit var->triggered();
        }
    }
}

void QtAppStyleApi::changeAppStyle()
{
    QString styleName = sender()->objectName();
    if (styleName.isEmpty()){
        return;
    }

    QStyle *style = QStyleFactory::create(styleName);
    if (style){
        QApplication::setStyle(style);
        emit appStyleChanged(styleName);
    }
}
