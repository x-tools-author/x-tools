/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QMenu>
#include <QDebug>

#include "SAKGlobal.hh"
#include "SAKDebugPage.hh"
#include "SAKDataStruct.hh"
#include "SAKCRCInterface.hh"
#include "SAKInputDataFactory.hh"
#include "SAKInputDataPresetItem.hh"
#include "SAKDebugPageDatabaseInterface.hh"

#include "ui_SAKInputDataPresetItem.h"

SAKInputDataPresetItem::SAKInputDataPresetItem(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget(parent)
    ,debugPage(debugPage)
    ,ui(new Ui::SAKInputDataPresetItem)
{
    initUi();
    inputManager = debugPage->inputController();
    id = QDateTime::currentMSecsSinceEpoch();
}

SAKInputDataPresetItem::SAKInputDataPresetItem(quint64 id,
                                   quint32 format,
                                   QString comment,
                                   quint32 classify,
                                   QString data,
                                   SAKDebugPage *debugPage,
                                   QWidget *parent)
    :QWidget(parent)
    ,debugPage(debugPage)
    ,id(id)
    ,ui(new Ui::SAKInputDataPresetItem)
{
    initUi();
    inputManager = debugPage->inputController();
    textFormatComboBox->setCurrentIndex(format);
    descriptionLineEdit->setText(comment);
    Q_UNUSED(classify);
    inputDataTextEdit->setText(data);
}

SAKInputDataPresetItem::~SAKInputDataPresetItem()
{
    delete ui;
}

quint64 SAKInputDataPresetItem::parameterID()
{
    return id;
}

quint32 SAKInputDataPresetItem::parameterFormat()
{
    return textFormatComboBox->currentIndex();
}

QString SAKInputDataPresetItem::parameterComment()
{
    return descriptionLineEdit->text();
}

quint32 SAKInputDataPresetItem::parameterClassify()
{
    quint32 ret = 0;
    return ret;
}

QString SAKInputDataPresetItem::parameterData()
{
    return inputDataTextEdit->toPlainText();
}

void SAKInputDataPresetItem::addDataAction(QPushButton *menuPushButton)
{
    if (!menuPushButton){
        return;
    }

    QMenu *menu = menuPushButton->menu();
    if (!menu){
        menu = new QMenu(menuPushButton);
        menuPushButton->setMenu(menu);
    }

    action = new QAction(descriptionLineEdit->text(), this);
    menu->addAction(action);
    connect(action, &QAction::triggered, this, &SAKInputDataPresetItem::sendRawData);
}

void SAKInputDataPresetItem::removeDataAction(QPushButton *menuPushButton)
{
    if (!menuPushButton){
        return;
    }

    QMenu *menu = menuPushButton->menu();
    if (menu){
        QList<QAction*> actions = menu->actions();
        if (!actions.isEmpty()){
            for(auto var:actions){
                if (action == var){
                    menu->removeAction(action);
                }
            }
        }
    }
}

void SAKInputDataPresetItem::updateActionTitle(const QString &title)
{
    action->setText(title);
}

void SAKInputDataPresetItem::updateTextFormat()
{
    inputManager->formattingInputText(inputDataTextEdit, textFormatComboBox->currentData().toInt());
}

void SAKInputDataPresetItem::sendRawData()
{
    QString data = inputDataTextEdit->toPlainText();
    int format = textFormatComboBox->currentData().toInt();

    if (!data.isEmpty()){
        debugPage->writeRawData(data, format);
    }
}

void SAKInputDataPresetItem::initUi()
{
    ui->setupUi(this);

    textFormatComboBox = ui->textFormatComboBox;
    descriptionLineEdit = ui->descriptionLineEdit;
    inputDataTextEdit = ui->inputDataTextEdit;
    SAKGlobal::initInputTextFormatComboBox(textFormatComboBox);

    menuPushButton = inputManager->sendPresetPushButton;
    addDataAction(menuPushButton);
    connect(descriptionLineEdit, &QLineEdit::textChanged, this, &SAKInputDataPresetItem::updateActionTitle);
    connect(inputDataTextEdit, &QTextEdit::textChanged, this, &SAKInputDataPresetItem::updateTextFormat);
    connect(textFormatComboBox, &QComboBox::currentTextChanged, inputDataTextEdit, &QTextEdit::clear);
}

//void SAKInputDataPresetItem::on_updatePushButton_clicked()
//{
//    QString tableName = SAKDataStruct::presettingDataTableName(debugPage->pageType());
//    SAKDataStruct::SAKStructPresettingDataItem item;
//    item.id = parameterID();
//    item.data = parameterData();
//    item.format = parameterFormat();
//    item.comment = parameterComment();
//    item.classify = parameterClassify();
//    SAKDebugPageDatabaseInterface::instance()->updatePresettingDataItem(tableName, item);
//}
