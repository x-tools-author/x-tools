/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "themeiconview.h"

#include <QGridLayout>
#include <QIcon>
#include <QMetaEnum>
#include <QPushButton>

ThemeIconView::ThemeIconView(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QGridLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(5);
    setLayout(layout);

    int from = static_cast<int>(QIcon::ThemeIcon::AddressBookNew);
    int to = static_cast<int>(QIcon::ThemeIcon::NThemeIcons);
    const int columns = 10;
    for (int i = from; i <= to; ++i) {
        QIcon::ThemeIcon value = static_cast<QIcon::ThemeIcon>(i);
        QPushButton *button = new QPushButton(QString::number(i), this);
        button->setIcon(QIcon::fromTheme(value));
        layout->addWidget(button, i / columns, i % columns);
    }
}

ThemeIconView::~ThemeIconView() {}