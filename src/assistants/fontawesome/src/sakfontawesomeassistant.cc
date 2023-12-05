/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakfontawesomeassistant.h"
#include "ui_sakfontawesomeassistant.h"

#include "flowlayout.h"
#include "sakfontawesome.h"
#include "sakfontawesomeicon.h"

SAKFontAwesomeAssistant::SAKFontAwesomeAssistant(QWidget *parent)
    : QWidget{parent}
    , ui(new Ui::SAKFontAwesomeAssistant)
{
    ui->setupUi(this);
    auto *l = new FlowLayout();
    ui->widget->setLayout(l);

    auto icons = SAKFontAwesome::instance()->supportedIcons();
    for (auto &icon : icons) {
        auto iconWidget = new SAKFontAwesomeIcon(icon);
        l->addWidget(iconWidget);
        connect(ui->lineEditFilter, &QLineEdit::editingFinished, this, [=]() {
            iconWidget->setFilter(ui->lineEditFilter->text().trimmed().toUpper());
        });
        iconWidget->setFilter(ui->lineEditFilter->text().trimmed().toUpper());
    }
}
