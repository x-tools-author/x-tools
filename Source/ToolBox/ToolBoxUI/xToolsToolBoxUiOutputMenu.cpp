/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsToolBoxUiOutputMenu.h"
#include "ui_xToolsToolBoxUiOutputMenu.h"

#include <QWidgetAction>

#include "xToolsCompatibility.h"
#include "xToolsHighlighter.h"
#include "xToolsSettings.h"

xToolsToolBoxUiOutputMenu::xToolsToolBoxUiOutputMenu(const QString& settingsGroup,
                                               QTextDocument* doc,
                                               QWidget* parent)
    : QMenu(parent)
    , ui(new Ui::xToolsToolBoxUiOutputMenu)
{
    QWidget* w = new QWidget(this);
    ui->setupUi(w);
    QWidgetAction* action = new QWidgetAction(this);
    action->setDefaultWidget(w);
    addAction(action);

    const QString key = settingsGroup + "/highlighter/items";
    QString txt = xToolsSettings::instance()->value(key).toString();
    txt = QString::fromUtf8(QByteArray::fromHex(txt.toLatin1()));
    ui->keyword->setText(txt);

    xToolsHighlighter* highlighter = new xToolsHighlighter(this);
    auto updateDoc = [=]() {
        QString text = ui->keyword->text();
        QStringList list = text.split(";", xToolsSkipEmptyParts);
        highlighter->removeKeyWord("");
        highlighter->setKeyWords(list);
    };

    highlighter->setDoc(QVariant::fromValue(doc));
    connect(ui->keyword, &QLineEdit::textChanged, this, [=]() {
        QString text = ui->keyword->text();
        QString hex = QString::fromLatin1(text.toUtf8().toHex());
        xToolsSettings::instance()->setValue(key, hex);

        updateDoc();
    });

    updateDoc();

    const QString fillterKey = settingsGroup + "/fillter";
    txt = xToolsSettings::instance()->value(fillterKey).toString();
    ui->filter->setText(txt);
    connect(ui->filter, &QLineEdit::editingFinished, this, [=]() {
        xToolsSettings::instance()->setValue(fillterKey, ui->filter->text().trimmed());
    });
}

xToolsToolBoxUiOutputMenu::~xToolsToolBoxUiOutputMenu()
{
    delete ui;
}

QString xToolsToolBoxUiOutputMenu::filter()
{
    return ui->filter->text().trimmed();
}
