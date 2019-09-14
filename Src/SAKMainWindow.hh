/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 * Welcome to bother.
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
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

class SAKVersion;
class UpdateManager;
class MoreInformation;

namespace Ui {
class SAKMainWindow;
}

class SAKMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SAKMainWindow(QWidget *parent = nullptr);
    ~SAKMainWindow();

    enum SAKStyleSheet{
        QtDefault,
        FlatWhite,
        LightBlue,
        PSBlack
    };
    Q_ENUM(SAKStyleSheet)

    static const char* appStyle(){return appStyleKey;}
private:
    QTabWidget* mpTabWidget             = nullptr;
    Ui::SAKMainWindow* ui               = nullptr;
    QMenu* toolsMenu                    = nullptr;
    SAKVersion* versionDialog           = nullptr;
    UpdateManager* updateManager        = nullptr;
    MoreInformation* moreInformation    = nullptr;

    static const char* appStyleKey;
    const char* appStylesheetKey = "Universal/appStylesheet";
    QMetaEnum skins;
    /// ----------------------------------------------------
    void AddTab();
    void InitMenu();
    void AddTool();

    void addTool(QString toolName, QWidget *toolWidget);
    void changeStylesheet();

    /**
     * @brief initSkinMenu  -- 创建皮肤切换菜单
     */
    void initSkinMenu(QMenu* optionMenu);

    /**
     * @brief addRemovablePage  -- 添加可删除的设备页面（只能通过信号来调用）
     */
    void addRemovablePage();

    /**
     * @brief openIODeviceWindow    -- 打开一个设备窗口（只能通过信号来调用）
     */
    void openIODeviceWindow();
    /**
     * @brief getDebugPage 获取调试页面窗口
     * @param type 窗口类型
     * @return  调试页面
     */
    QWidget *getDebugPage(int type);

    /**
     * @brief closeDebugPage 关闭调试页面
     * @param index 需要关闭的调试页面
     */
    void closeDebugPage(int index);
private slots:
    void AboutQt();
    void About();

private:
    void createCRCCalculator();
};

#endif // MAINWINDOW_H
