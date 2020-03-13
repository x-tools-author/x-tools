/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QFile>
#include <QMapIterator>
#include <QActionGroup>
#include <QApplication>

#include "QtStyleSheetApi.hh"

QtStyleSheetApi *QtStyleSheetApi::_this = nullptr;
QtStyleSheetApi *QtStyleSheetApi::instance()
{
    if (_this){
        return _this;
    }else{
        new QtStyleSheetApi;
        return _this;
    }
}

QList<QAction *> QtStyleSheetApi::actions()
{
    return styleSheetActions;
}

void QtStyleSheetApi::setStyleSheet(QString styleSheetName)
{
    if (styleSheetName.isEmpty()){
        return;
    }


    QMapIterator<QString, QString> iterator(styleSheetMap);
    while(iterator.hasNext()){
        iterator.next();

        if(iterator.key() == styleSheetName){
            for(auto var:_this->styleSheetActions){
                if (var->objectName() == iterator.value()){
                    var->setChecked(true);
                    emit var->triggered();
                    return;
                }
            }
        }
    }
}

QtStyleSheetApi::QtStyleSheetApi(QObject *parent)
    :QObject (parent)
{
    _this = this;

    styleSheetMap.insert(tr("黑色"), ":/qss/black.css");
    styleSheetMap.insert(tr("蓝黑色"), ":/qss/blackblue.css");
    styleSheetMap.insert(tr("视频黑色"), ":/qss/blackvideo.css");
    styleSheetMap.insert(tr("蓝色"), ":/qss/blue.css");
    styleSheetMap.insert(tr("深黑色"), ":/qss/darkblack.css");
    styleSheetMap.insert(tr("深蓝色"), ":/qss/darkblue.css");
    styleSheetMap.insert(tr("深灰色"), ":/qss/darkgray.css");
    styleSheetMap.insert(tr("扁平黑色"), ":/qss/flatblack.css");
    styleSheetMap.insert(tr("扁平蓝色"), ":/qss/flatblue.css");
    styleSheetMap.insert(tr("扁平白色"), ":/qss/flatwhite.css");
    styleSheetMap.insert(tr("灰色"), ":/qss/gray.css");
    styleSheetMap.insert(tr("浅黑色"), ":/qss/lightblack.css");
    styleSheetMap.insert(tr("浅蓝色"), ":/qss/lightblue.css");
    styleSheetMap.insert(tr("浅灰色"), ":/qss/lightgray.css");
    styleSheetMap.insert(tr("PS黑色"), ":/qss/psblack.css");
    styleSheetMap.insert(tr("紫色"), ":/qss/purple.css");
    styleSheetMap.insert(tr("银色"), ":/qss/silvery.css");

    QMapIterator<QString, QString> iterator(styleSheetMap);
    QActionGroup *actionGroup = new QActionGroup(this);
    while(iterator.hasNext()){
        iterator.next();

        QAction *action = new QAction(iterator.key(), this);
        action->setCheckable(true);
        action->setObjectName(iterator.value());
        actionGroup->addAction(action);
        styleSheetActions.append(action);
        connect(action, &QAction::triggered, this, &QtStyleSheetApi::changeStyleSheet);
    }
}

void QtStyleSheetApi::changeStyleSheet()
{
    QObject *obj = sender();
    QString fileName = obj->objectName();
    if (fileName.isEmpty()){
        return;
    }

    QFile file(fileName);
    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setStyleSheet(qss);
        file.close();

        emit styleSheetChanged(styleSheetMap.key(fileName));
    }
}
