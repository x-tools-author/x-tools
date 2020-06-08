/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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
