/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QListWidgetItem>

#include "x/xui.h"

namespace Ui {
class MainWindow;
}

class TsFile;
class MainWindow : public xUi
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    struct SettingKeys
    {
        const QString lastOpenedDirectory{"xLinguist/last_opened_directory"};
    } m_keys;

private:
    Ui::MainWindow *ui;
    QString m_rootPath;
    QList<TsFile *> m_tsFiles;

private:
    void onStartButtonClicked();
    void onStopButtonClicked();
    void onBrowseButtonClicked();
    void onRemoveButtonClicked();
    void onListWidgetItemDoubleClicked(QListWidgetItem *item);

    void loadTranslationFiles(const QString &dir);
};