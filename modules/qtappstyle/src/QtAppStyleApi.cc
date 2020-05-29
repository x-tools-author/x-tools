/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */

#include <QApplication>
#include <QStyleFactory>

#include "QtAppStyleApi.hh"

QtAppStyleApi *QtAppStyleApi::_this = Q_NULLPTR;
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

    QAction *styleAction = Q_NULLPTR;
    QActionGroup *styleActionGroup = new QActionGroup(this);
    QStringList styleKeys = QStyleFactory::keys();
    for (QString style : styleKeys) {
        styleAction = new QAction(style, this);
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
