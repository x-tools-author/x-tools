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
#include "SAKInputDataItem.hh"
#include "SAKCRCInterface.hh"
#include "SAKInputDataFactory.hh"

#include "ui_SAKInputDataItem.h"

SAKInputDataItem::SAKInputDataItem(SAKDebugPage *debugPage, SAKDebugPageInputManager *inputManager, QWidget *parent)
    :QWidget(parent)
    ,debugPage(debugPage)
    ,inputManager(inputManager)
    ,ui(new Ui::SAKInputDataItem)
{
    ui->setupUi(this);

    textFormatComboBox = ui->textFormatComboBox;
    descriptionLineEdit = ui->descriptionLineEdit;
    inputDataTextEdit = ui->inputDataTextEdit;
    updatePushButton = ui->updatePushButton;
    SAKGlobal::initInputTextFormatComboBox(textFormatComboBox);

    menuPushButton = inputManager->sendPresetPushButton;
    addDataAction(menuPushButton);
    connect(descriptionLineEdit, &QLineEdit::textChanged, this, &SAKInputDataItem::updateActionTitle);
    connect(inputDataTextEdit, &QTextEdit::textChanged, this, &SAKInputDataItem::updateTextFormat);
#if 1
    connect(textFormatComboBox, &QComboBox::currentTextChanged, inputDataTextEdit, &QTextEdit::clear);
#else
    connect(textFormatComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        Q_UNUSED(index)
        inputDataTextEdit->clear();
    });
#endif
}

SAKInputDataItem::~SAKInputDataItem()
{
    removeDataAction(inputManager->presetPushButton);
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

void SAKInputDataItem::on_updatePushButton_clicked()
{

}
