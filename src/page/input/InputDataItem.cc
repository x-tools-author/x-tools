/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include "SAKGlobal.hh"
#include "SAKDebugPage.hh"
#include "InputDataItem.hh"
#include "SAKCRCInterface.hh"
#include "InputDataFactory.hh"

#include "ui_InputDataItem.h"

#include <QMenu>
#include <QDebug>

InputDataItem::InputDataItem(SAKDebugPage *debugPage, DebugPageInputManager *inputManager, QWidget *parent)
    :QWidget (parent)
    ,ui (new Ui::InputDataItem)
    ,debugPage (debugPage)
    ,inputManager (inputManager)
{
    ui->setupUi(this);

    textFormatComboBox  = ui->textFormatComboBox;
    descriptionLineEdit = ui->descriptionLineEdit;
    inputDataTextEdit   = ui->inputDataTextEdit;
    SAKGlobal::initInputTextFormatComboBox(textFormatComboBox);

    menuPushButton = inputManager->sendPresetPushButton;
    addDataAction(menuPushButton);
    connect(descriptionLineEdit, &QLineEdit::textChanged, this, &InputDataItem::updateActionTitle);
    connect(inputDataTextEdit, &QTextEdit::textChanged, this, &InputDataItem::updateTextFormat);

    connect(textFormatComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        Q_UNUSED(index)
        inputDataTextEdit->clear();
    });
}

InputDataItem::~InputDataItem()
{
    removeDataAction(inputManager->presetPushButton);
}

void InputDataItem::addDataAction(QPushButton *menuPushButton)
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
    connect(action, &QAction::triggered, this, &InputDataItem::sendRawData);
}

void InputDataItem::removeDataAction(QPushButton *menuPushButton)
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

void InputDataItem::updateActionTitle(const QString &title)
{
    action->setText(title);
}

void InputDataItem::updateTextFormat()
{
    inputManager->formattingInputText(inputDataTextEdit, textFormatComboBox->currentData().toInt());
}

void InputDataItem::sendRawData()
{
    QString data = inputDataTextEdit->toPlainText();
    int format = textFormatComboBox->currentData().toInt();

    if (!data.isEmpty()){
        debugPage->writeRawData(data, format);
    }
}
