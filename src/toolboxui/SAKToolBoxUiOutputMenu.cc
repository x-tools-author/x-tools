/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QWidgetAction>

#include "SAKSettings.hh"
#include "SAKHighlighter.hh"
#include "SAKToolBoxUiOutputMenu.hh"
#include "ui_SAKToolBoxUiOutputMenu.h"

SAKToolBoxUiOutputMenu::SAKToolBoxUiOutputMenu(const QString &settingsGroup,
                                               QTextDocument *doc,
                                               QWidget *parent)
    : QMenu(parent)
    , ui(new Ui::SAKToolBoxUiOutputMenu)
{
    QWidget *w = new QWidget(this);
    ui->setupUi(w);
    QWidgetAction *action = new QWidgetAction(this);
    action->setDefaultWidget(w);
    addAction(action);

    const QString key = settingsGroup + "/highlighter/items";
    QString txt = SAKSettings::instance()->value(key).toString();
    txt = QString::fromUtf8(QByteArray::fromHex(txt.toLatin1()));
    ui->lineEdit->setText(txt);

    SAKHighlighter *highlighter = new SAKHighlighter(this);
    auto updateDoc = [=](){
        QString text = ui->lineEdit->text();
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
        QStringList list = text.split(";", Qt::SkipEmptyParts);
#else
        QStringList list = text.split(QString(";"), QString::SkipEmptyParts);
#endif
        highlighter->removeKeyWord("");
        highlighter->setKeyWords(list);
    };

    highlighter->setDoc(QVariant::fromValue(doc));
    connect(ui->lineEdit, &QLineEdit::textChanged, this, [=](){
        QString text = ui->lineEdit->text();
        QString hex = QString::fromLatin1(text.toUtf8().toHex());
        SAKSettings::instance()->setValue(key, hex);

        updateDoc();
    });

    updateDoc();
}

SAKToolBoxUiOutputMenu::~SAKToolBoxUiOutputMenu()
{
    delete ui;
}
