/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "mainwindow.h"

#include <QAction>
#include <QCloseEvent>
#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>

#include "./application.h"
#include "x/flow/nodeeditor/nodeeditor.h"
#include "x/flow/nodeeditor/nodeeditorview.h"
#include "x/flow/xflow.h"

struct MainWindowParameterKeys
{
    const QString flow{"flow"};
};

MainWindow::MainWindow(QWidget *parent)
    : xUi(parent)
{
    m_flow = new xFlow::xFlow(this);
    setCentralWidget(m_flow);

    QList<QAction *> actions = m_fileMenu->actions();
    QAction *a = m_fileMenu->addAction(tr("New Project"), this, &MainWindow::onNew);
    a->setShortcut(QKeySequence::New);
    a = m_fileMenu->addAction(tr("Save Project"), this, &MainWindow::onExport);
    a->setShortcut(QKeySequence::Save);
    a = m_fileMenu->addAction(tr("Load Project"), this, &MainWindow::onImport);
    a->setShortcut(QKeySequence::Open);
    for (int i = 0; i < actions.count(); i++) {
        m_fileMenu->addAction(actions.at(i));
    }
}

MainWindow::~MainWindow() {}

QString dataJsonFilePath()
{
    const QString path = xAPP->settingsPath();
    return path + "/data.json";
}

void MainWindow::load(const QString &fileName)
{
    QString filePath = fileName;
    if (filePath.isEmpty()) {
        filePath = dataJsonFilePath();
    }

    QFile file(filePath);
    if (!file.exists()) {
        m_flow->load(QJsonObject());
        return;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        m_flow->load(QJsonObject());
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    const MainWindowParameterKeys keys;
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    m_flow->load(obj.value(keys.flow).toObject());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    save();
    xUi::closeEvent(event);
}

void MainWindow::save(const QString &fileName)
{
    QJsonObject obj;
    const MainWindowParameterKeys keys;
    obj.insert(keys.flow, m_flow->save());

    QJsonDocument doc(obj);
    QString filePath = fileName;
    if (filePath.isEmpty()) {
        filePath = dataJsonFilePath();
    }

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.write(doc.toJson());
        file.close();
    }
}

void MainWindow::onNew()
{
    xFlow::NodeEditor *editor = m_flow->nodeEditor();
    xFlow::NodeEditorView *view = editor->view();
    view->clearAllNodes();
}

void MainWindow::onImport()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Import Project"),
                                                    QString(),
                                                    tr("JSON Files (*.json);;All Files (*)"));
    if (fileName.isEmpty()) {
        return;
    }

    save(fileName);
}

void MainWindow::onExport()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Export Project"),
                                                    QString(),
                                                    tr("JSON Files (*.json);;All Files (*)"));
    if (fileName.isEmpty()) {
        return;
    }

    load(fileName);
}
