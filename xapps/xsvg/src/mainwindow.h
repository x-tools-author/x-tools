/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <atomic>

#include <QModelIndexList>

#include "xApp/Source/MainWindow.h"

namespace Ui {
class MainWindow;
}

class SettingsMenu;
class SvgFileDataModel;

namespace xSvg {

class MainWindow : public xApp::MainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = Q_NULLPTR);
    ~MainWindow() override;
    void showTips();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    const struct
    {
        const QString path = "path";
        const QString filter = "filter";
        const QString format = "format";
        const QString size = "size";
        const QString color = "color";
    } m_settingKeys;

private:
    Ui::MainWindow* ui;
    QWidget* m_centralWidget;
    SvgFileDataModel* m_model{nullptr};
    QString m_path;
    std::atomic_bool m_isUpdating{false};
    std::atomic_bool m_needUpdateAgain{false};
    SettingsMenu* m_settingsMenu{nullptr};
    QLineEdit* m_searchLineEdit{nullptr};
    QComboBox* m_sizeComboBox{nullptr};
    QComboBox* m_formatComboBox{nullptr};
    QPushButton* m_colorButton{nullptr};

    struct
    {
        QAction* openPath;
        QAction* openFileLocation;
        QAction* saveAs;
        QAction* copyFullPath;
        QAction* copyFileName;
        QAction* settingsAction;
    } m_actions;

private:
    void initMenuBar();
    void initToolBar();
    void initToolBarActions(QToolBar* toolBar);
    void initToolBarFilter(QToolBar* toolBar);
    void initToolBarFormat(QToolBar* toolBar);
    void initToolBarSize(QToolBar* toolBar);
    void initToolBarColor(QToolBar* toolBar);
    void initToolBarSettings(QToolBar* toolBar);

    void openPath();
    void openFileLocation();
    void copyTo(const QModelIndexList& list, const QString& path);
    void exportTo(const QModelIndexList& list, const QString& path, int padding = 0);
    void saveAs();
    void copyFullPath();
    void copyFileName();
    void updateView();    
};

} // namespace xSvg
