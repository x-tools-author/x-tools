/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakfontawesomeicon.h"
#include "ui_sakfontawesomeicon.h"

#include <QApplication>
#include <QClipboard>
#include <QFont>

SAKFontAwesomeIcon::SAKFontAwesomeIcon(const SAKFontAwesomeIconContext &ctx, QWidget *parent)
    : QWidget{parent}
    , ui(new Ui::SAKFontAwesomeIcon)
    , m_ctx(ctx)
{
    ui->setupUi(this);
    ui->labelUnicode->setText(QString::number(m_ctx.unicode));
    ui->labelIcon->setText(QString(QChar(m_ctx.unicode)));
    QFont font = ui->labelIcon->font();
    font.setPixelSize(48);
    font.setFamily(m_ctx.family);
    ui->labelIcon->setFont(font);
    setFixedSize(96, 96);

    QString toolTip = QString("unicode: %1\n").arg(ctx.unicode);
    toolTip.append(QString("label: %1\n").arg(ctx.label));
    toolTip.append(QString("terms: %1\n").arg(ctx.terms.join(",")).toLower());
    toolTip.append(QString("family: %1").arg(ctx.family));
    ui->pushButton->setToolTip(toolTip);
    connect(ui->pushButton, &QPushButton::clicked, this, [=]() {
        QApplication::clipboard()->setText(toolTip);
    });
}

void SAKFontAwesomeIcon::setFilter(const QString &filter)
{
    if (filter.isEmpty()) {
        show();
        return;
    }

    for (auto &term : m_ctx.terms) {
        if (term.contains(filter)) {
            show();
            return;
        }
    }

    hide();
}
