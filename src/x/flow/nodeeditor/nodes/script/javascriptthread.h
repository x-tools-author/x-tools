/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QtNodes/Definitions>
#include <QtNodes/internal/NodeData.hpp>

#include <QJSEngine>
#include <QThread>

class JavaScript;
class JavaScriptThread : public QThread
{
    Q_OBJECT
public:
    JavaScriptThread(JavaScript *js);
    ~JavaScriptThread() override;

private:
    JavaScript *m_js;

protected:
    void run() override;

    void onInput2Thread(QJSEngine *engine,
                        std::shared_ptr<QtNodes::NodeData> nodeData,
                        QtNodes::PortIndex const);
};
