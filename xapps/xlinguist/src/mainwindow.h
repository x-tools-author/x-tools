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
#include <QTimer>

#include "x/xui.h"

namespace Ui {
class MainWindow;
}

class TsFile;
class Translator;
class MainWindow : public xUi
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    struct SettingKeys
    {
        const QString lastOpenedDirectory{"xLinguist/last_opened_directory"};
        const QString lastOpenedFile{"xLinguist/last_opened_file"};
    } m_keys;

private:
    Ui::MainWindow *ui;
    QString m_rootPath;
    QString m_lastOpenedFile;
    QList<Translator *> m_translators;
    QTimer *m_checkThreadPoolTimer{nullptr};

private:
    void onStartButtonClicked();
    void onStopButtonClicked();
    void onBrowseButtonClicked();
    void onOpenButtonClicked();
    void onRemoveButtonClicked();
    void onViewDoubleClicked(const QModelIndex &index);
    void onCheckThreadPoolTimeout();
    void onSavePushButtonClicked();

    void loadTranslationFiles(const QString &dir, const QString &specifiedFile = QString());
};