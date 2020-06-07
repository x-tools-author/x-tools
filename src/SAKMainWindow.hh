/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMenu>
#include <QLabel>
#include <QAction>
#include <QMetaEnum>
#include <QTabWidget>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QTranslator>

class SAKQRCodeDialog;
class SAKUpdateManager;
class SAKMoreInformation;

namespace Ui {
class SAKMainWindow;
}

class SAKMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SAKMainWindow(QWidget *parent = Q_NULLPTR);
    ~SAKMainWindow();

protected:
      bool eventFilter(QObject *obj, QEvent *event);
private:
    QTabWidget *tabWidget               = Q_NULLPTR;
    Ui::SAKMainWindow *ui               = Q_NULLPTR;
    QMenu *toolsMenu                    = Q_NULLPTR;
    QAction *defaultStyleSheetAction    = Q_NULLPTR;
    SAKUpdateManager *updateManager     = Q_NULLPTR;
    SAKMoreInformation *moreInformation = Q_NULLPTR;
    SAKQRCodeDialog *qrCodeDialog       = Q_NULLPTR;

    QTranslator qtTranslator;
    QTranslator qtBaeTranslator;
    QTranslator sakTranslator;

    /// ----------------------------------------------------
    void addTab();    
    void addTool();

    void addTool(QString toolName, QWidget *toolWidget);
    void changeStylesheet(QString styleSheetName);
    void changeAppStyle(QString appStyle);

    void initMenu();
    void initFileMenu();
    void initToolMenu();
    void initOptionMenu();
    void initLanguageMenu();
    void initHelpMenu();
    void initLinksMenu();

    void installLanguage();
    void addRemovablePage();
    void openIODeviceWindow();
    QWidget *getDebugPage(int type);
    void closeDebugPage(int index);
    void about();
private:
    void createCRCCalculator();
};

#endif // MAINWINDOW_H
