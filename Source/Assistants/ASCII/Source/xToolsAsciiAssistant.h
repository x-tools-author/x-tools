/***************************************************************************************************
 * Copyright 2022-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QMap>
#include <QWidget>

namespace Ui {
class xToolsAsciiAssistant;
}

class xToolsAsciiAssistant : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE xToolsAsciiAssistant(QWidget* parent = Q_NULLPTR);
    ~xToolsAsciiAssistant();

private:
    Ui::xToolsAsciiAssistant* ui;
    QMap<int, QString> m_descirption;

private:
    void initDescirption();
    void setupFilter(const QString& text);
};
