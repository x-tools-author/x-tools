/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded in "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source code
 * directory.
 **************************************************************************************************/
#include "MainWindow.h"

#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QDesktopServices>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPainter>
#include <QProcess>
#include <QScreen>
#include <QStyle>
#include <QStyleFactory>
#include <QStyleHints>
#include <QSvgRenderer>
#include <QTimer>
#include <QUrl>

#include "xtools.h"

#ifdef X_TOOLS_ENABLE_QSS
#include "qss/QssMgr.h"
#endif

namespace xTools {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_fileMenu(nullptr)
    , m_optionMenu(nullptr)
    , m_languageMenu(nullptr)
    , m_helpMenu(nullptr)
    , m_exitAction(nullptr)
    , m_gitHubAction(nullptr)
    , m_giteeAction(nullptr)
    , m_qqGroupAction(nullptr)
    , m_aboutAction(nullptr)
    , m_aboutQtAction(nullptr)
    , m_appStyleActionGroup(nullptr)
    , m_languageActionGroup(nullptr)
    , m_appPaletteActionGroup(nullptr)
{
    m_appStyleActionGroup = new QActionGroup(this);
    m_languageActionGroup = new QActionGroup(this);
    m_appPaletteActionGroup = new QActionGroup(this);

    initMenuFile();
    initMenuOption();
    initMenuLanguage();
    initMenuHelp();

    MainWindow::updateWindowTitle();
}



} // namespace xTools
