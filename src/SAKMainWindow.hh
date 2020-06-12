/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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
/// @brief 程序主窗口
class SAKMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit SAKMainWindow(QWidget *parent = Q_NULLPTR);
    ~SAKMainWindow();
protected:
      bool eventFilter(QObject *obj, QEvent *event);
private:
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

    void createCRCCalculator();
private:
    QTabWidget *tabWidget;
    QMenu *toolsMenu;
    QAction *defaultStyleSheetAction;
    SAKUpdateManager *updateManager;
    SAKMoreInformation *moreInformation;
    SAKQRCodeDialog *qrCodeDialog;

    QTranslator qtTranslator;
    QTranslator qtBaeTranslator;
    QTranslator sakTranslator;
private:
    Ui::SAKMainWindow *ui;
};

#endif // MAINWINDOW_H
