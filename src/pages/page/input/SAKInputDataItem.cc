/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QMenu>
#include <QDebug>

#include "SAKGlobal.hh"
#include "SAKDebugPage.hh"
#include "SAKDataStruct.hh"
#include "SAKCRCInterface.hh"
#include "SAKInputDataItem.hh"
#include "SAKInputDataFactory.hh"
#include "SAKDebugPageDatabaseInterface.hh"

#include "ui_SAKInputDataItem.h"

SAKInputDataItem::SAKInputDataItem(SAKDebugPage *debugPage, SAKDebugPageInputManager *inputManager, QWidget *parent)
    :QWidget(parent)
    ,debugPage(debugPage)
    ,inputManager(inputManager)
    ,ui(new Ui::SAKInputDataItem)
{
    initUi();
    id = QDateTime::currentMSecsSinceEpoch();
}

SAKInputDataItem::SAKInputDataItem(quint64 id,
                                   quint32 format,
                                   QString comment,
                                   quint32 classify,
                                   QString data,
                                   SAKDebugPage *debugPage,
                                   SAKDebugPageInputManager *inputManager,
                                   QWidget *parent)
    :QWidget(parent)
    ,debugPage(debugPage)
    ,inputManager(inputManager)
    ,id(id)
    ,ui(new Ui::SAKInputDataItem)
{
    initUi();
    textFormatComboBox->setCurrentIndex(format);
    descriptionLineEdit->setText(comment);
    classifyComboBox->setCurrentIndex(classify);
    inputDataTextEdit->setText(data);
}

SAKInputDataItem::~SAKInputDataItem()
{
    removeDataAction(inputManager->presetPushButton);
}

quint64 SAKInputDataItem::parameterID()
{
    return id;
}

quint32 SAKInputDataItem::parameterFormat()
{
    return textFormatComboBox->currentIndex();
}

QString SAKInputDataItem::parameterComment()
{
    return descriptionLineEdit->text();
}

quint32 SAKInputDataItem::parameterClassify()
{
    quint32 ret;
    if (classifyComboBox->count()){
        ret = classifyComboBox->currentIndex();
    }else{
        ret = 0;
    }
    return ret;
}

QString SAKInputDataItem::parameterData()
{
    return inputDataTextEdit->toPlainText();
}

void SAKInputDataItem::addDataAction(QPushButton *menuPushButton)
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
    connect(action, &QAction::triggered, this, &SAKInputDataItem::sendRawData);
}

void SAKInputDataItem::removeDataAction(QPushButton *menuPushButton)
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

void SAKInputDataItem::updateActionTitle(const QString &title)
{
    action->setText(title);
}

void SAKInputDataItem::updateTextFormat()
{
    inputManager->formattingInputText(inputDataTextEdit, textFormatComboBox->currentData().toInt());
}

void SAKInputDataItem::sendRawData()
{
    QString data = inputDataTextEdit->toPlainText();
    int format = textFormatComboBox->currentData().toInt();

    if (!data.isEmpty()){
        debugPage->writeRawData(data, format);
    }
}

void SAKInputDataItem::initUi()
{
    ui->setupUi(this);

    textFormatComboBox = ui->textFormatComboBox;
    descriptionLineEdit = ui->descriptionLineEdit;
    inputDataTextEdit = ui->inputDataTextEdit;
    updatePushButton = ui->updatePushButton;
    classifyComboBox = ui->classifyComboBox;
    classifyComboBox->addItem(tr("默认"));
    SAKGlobal::initInputTextFormatComboBox(textFormatComboBox);

    menuPushButton = inputManager->sendPresetPushButton;
    addDataAction(menuPushButton);
    connect(descriptionLineEdit, &QLineEdit::textChanged, this, &SAKInputDataItem::updateActionTitle);
    connect(inputDataTextEdit, &QTextEdit::textChanged, this, &SAKInputDataItem::updateTextFormat);
    connect(textFormatComboBox, &QComboBox::currentTextChanged, inputDataTextEdit, &QTextEdit::clear);
}

void SAKInputDataItem::on_updatePushButton_clicked()
{
    QString tableName = SAKDataStruct::presettingDataTableName(debugPage->pageType());
    SAKDataStruct::SAKStructPresettingDataItem item;
    item.id = parameterID();
    item.data = parameterData();
    item.format = parameterFormat();
    item.comment = parameterComment();
    item.classify = parameterClassify();
    SAKDebugPageDatabaseInterface::instance()->updatePresettingDataItem(tableName, item);
}
