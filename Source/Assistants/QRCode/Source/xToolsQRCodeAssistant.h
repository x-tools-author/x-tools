/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QPixmap>
#include <QWidget>

namespace Ui {
class xToolsQRCodeAssistant;
}

class xToolsQRCodeAssistant : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE xToolsQRCodeAssistant(QWidget* parent = nullptr);
    ~xToolsQRCodeAssistant();

private:
    Ui::xToolsQRCodeAssistant* ui;
    QPixmap m_pixMap;

private slots:
    void onGeneratePushButtonClicked();
    void onExportPushButtonClicked();
};