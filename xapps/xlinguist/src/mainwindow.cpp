/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded in "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "mainwindow.h"

#include <QMenuBar>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent)
    : xUi(parent)
{
    setWindowTitle(tr("xLinguist"));
    setMinimumSize(800, 600);
    setWindowIcon(QIcon(":/res/icons/logo.svg"));

    initFileMenu();
    initToolBar();
}

MainWindow::~MainWindow() {}

void MainWindow::initFileMenu()
{
    m_fileMenu = new QMenu(tr("File"), this);
    menuBar()->insertMenu(m_optionMenu->menuAction(), m_fileMenu);

    QAction *openAction = m_fileMenu->addAction(tr("Open"), this, &MainWindow::onOpen);
    openAction->setShortcut(QKeySequence::Open);
    QAction *saveAction = m_fileMenu->addAction(tr("Save"), this, &MainWindow::onSave);
    saveAction->setShortcut(QKeySequence::Save);
    QAction *saveAsAction = m_fileMenu->addAction(tr("Save As"), this, &MainWindow::onSaveAs);
    saveAsAction->setShortcut(QKeySequence::SaveAs);

    m_fileMenu->addSeparator();
    initFileMenuRecentFiles();
    m_fileMenu->addSeparator();

    QAction *exitAction = m_fileMenu->addAction(tr("Exit"), this, &MainWindow::close);
    exitAction->setShortcut(QKeySequence::Quit);
}

void MainWindow::initFileMenuRecentFiles()
{
    QMenu *recentFilesMenu = m_fileMenu->addMenu(tr("Recent Files"));
    recentFilesMenu->addAction(tr("Clear Recent Files"), this, [this]() {

    });
    recentFilesMenu->addSeparator();
}

void MainWindow::initToolBar() {}

void MainWindow::onOpen() {}

void MainWindow::onSave() {}

void MainWindow::onSaveAs() {}
