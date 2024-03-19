/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "saktoolboxuioutputmenu.h"

#include <QWidgetAction>

#include "sakhighlighter.h"
#include "saksettings.h"
#include "ui_saktoolboxuioutputmenu.h"

SAKToolBoxUiOutputMenu::SAKToolBoxUiOutputMenu(const QString& settingsGroup,
                                               QTextDocument* doc,
                                               QWidget* parent)
    : QMenu(parent)
    , ui(new Ui::SAKToolBoxUiOutputMenu)
{
    QWidget* w = new QWidget(this);
    ui->setupUi(w);
    QWidgetAction* action = new QWidgetAction(this);
    action->setDefaultWidget(w);
    addAction(action);

    const QString key = settingsGroup + "/highlighter/items";
    QString txt = SAKSettings::instance()->value(key).toString();
    txt = QString::fromUtf8(QByteArray::fromHex(txt.toLatin1()));
    ui->keyword->setText(txt);

    SAKHighlighter* highlighter = new SAKHighlighter(this);
    auto updateDoc = [=]() {
        QString text = ui->keyword->text();
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
        QStringList list = text.split(";", Qt::SkipEmptyParts);
#else
        QStringList list = text.split(QString(";"), QString::SkipEmptyParts);
#endif
        highlighter->removeKeyWord("");
        highlighter->setKeyWords(list);
    };

    highlighter->setDoc(QVariant::fromValue(doc));
    connect(ui->keyword, &QLineEdit::textChanged, this, [=]() {
        QString text = ui->keyword->text();
        QString hex = QString::fromLatin1(text.toUtf8().toHex());
        SAKSettings::instance()->setValue(key, hex);

        updateDoc();
    });

    updateDoc();

    const QString fillterKey = settingsGroup + "/fillter";
    txt = SAKSettings::instance()->value(fillterKey).toString();
    ui->filter->setText(txt);
    connect(ui->filter, &QLineEdit::editingFinished, this, [=]() {
        SAKSettings::instance()->setValue(fillterKey, ui->filter->text().trimmed());
    });
}

SAKToolBoxUiOutputMenu::~SAKToolBoxUiOutputMenu()
{
    delete ui;
}

QString SAKToolBoxUiOutputMenu::filter()
{
    return ui->filter->text().trimmed();
}
