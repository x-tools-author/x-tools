/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QFile>
#include <QMapIterator>
#include <QActionGroup>
#include <QApplication>

#include "QtStyleSheetApi.hh"

QtStyleSheetApi *QtStyleSheetApi::_this = Q_NULLPTR;
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

    styleSheetMap.insert(tr("Black"), ":/qss/black.css");
    styleSheetMap.insert(tr("Black Blue"), ":/qss/blackblue.css");
    styleSheetMap.insert(tr("Video Black"), ":/qss/blackvideo.css");
    styleSheetMap.insert(tr("Blue"), ":/qss/blue.css");
    styleSheetMap.insert(tr("Aterrimus"), ":/qss/darkblack.css");
    styleSheetMap.insert(tr("Dark Blue"), ":/qss/darkblue.css");
    styleSheetMap.insert(tr("Dark Grey"), ":/qss/darkgray.css");
    styleSheetMap.insert(tr("Flat Black"), ":/qss/flatblack.css");
    styleSheetMap.insert(tr("Flat Blue"), ":/qss/flatblue.css");
    styleSheetMap.insert(tr("Flat White"), ":/qss/flatwhite.css");
    styleSheetMap.insert(tr("Gray"), ":/qss/gray.css");
    styleSheetMap.insert(tr("Light Black"), ":/qss/lightblack.css");
    styleSheetMap.insert(tr("Wathet"), ":/qss/lightblue.css");
    styleSheetMap.insert(tr("French Grey"), ":/qss/lightgray.css");
    styleSheetMap.insert(tr("Photoshop Black"), ":/qss/psblack.css");
    styleSheetMap.insert(tr("Purple"), ":/qss/purple.css");
    styleSheetMap.insert(tr("Silver"), ":/qss/silvery.css");

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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        qApp->setPalette(QPalette(QColor(paletteColor)));
#endif
        qApp->setStyleSheet(qss);
        file.close();

        emit styleSheetChanged(styleSheetMap.key(fileName));
    }
}
