/***************************************************************************************************
 * Copyright 2018-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QUrl>

#include "common/xui.h"

class xPageManager;
class MainWindow : public xUi
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = Q_NULLPTR);
    ~MainWindow() override;

    void load(const QString& fileName = QString());
    void save(const QString& fileName = QString()) const;
    void moveToCenter();
    void showLiteMode();

protected:
    void closeEvent(QCloseEvent* event) override;

protected:
    virtual QUrl storeUrl() const;

protected:
    xPageManager* m_xPageMgr{nullptr};

private:
    void initMenuBar();
    void initFileMenu();
    void initToolMenu();
    void initOptionMenu();
    void initHelpMenu();

    void showHistory();
    void showQrCode();
    QString defalutDataJsonFile() const;

    // HDPI Policy for Windows
    QString qtConfFileName();
    void createQtConf();

    void onSaveActionTriggered() const;
    void onImportActionTriggered();
    void onExportActionTriggered();
    void onAboutActionTriggered();
};
