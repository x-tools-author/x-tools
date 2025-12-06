/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "mainwindow.h"

#include <QCloseEvent>
#include <QFile>
#include <QJsonDocument>

#include "./application.h"
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
}

MainWindow::~MainWindow() {}

QString dataJsonFilePath()
{
    const QString path = xAPP->settingsPath();
    return path + "/data.json";
}

void MainWindow::load()
{
    QString filePath = dataJsonFilePath();
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

void MainWindow::save()
{
    QJsonObject obj;
    const MainWindowParameterKeys keys;
    obj.insert(keys.flow, m_flow->save());

    QJsonDocument doc(obj);
    QFile file(dataJsonFilePath());
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.write(doc.toJson());
        file.close();
    }
}