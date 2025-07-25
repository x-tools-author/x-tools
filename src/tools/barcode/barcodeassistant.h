/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QCompleter>
#include <QStringListModel>
#include <QWidget>

namespace Ui {
class BarCodeAssistant;
}

class BarCodeAssistant : public QWidget
{
    Q_OBJECT
public:
    explicit BarCodeAssistant(QWidget *parent = nullptr);
    ~BarCodeAssistant();

    void onExportButtonClicked();
    void onRefreshButtonClicked();

private:
    Ui::BarCodeAssistant *ui;
    QCompleter *m_completer;
    QStringListModel *m_filterModel;
    QList<QPair<int, QString>> m_allItems;

private:
    void onTypeComboBoxTextChanged(const QString &text);

    void updateComboBoxItems(const QString &filter = QString());
    void setupTypeComboBox();
};