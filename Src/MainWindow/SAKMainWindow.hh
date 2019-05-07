/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QLabel>
#include <QMetaEnum>

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
private slots:
    void AboutQt();
    void About();
    void styleActionTriggered();
private:
    void createCRCCalculator();
};

#endif // MAINWINDOW_H
