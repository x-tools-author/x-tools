/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "nodeeditor/nodes/common/basenode.h"

struct FrameAnalyzerParametersKeys
{
    const QString fixed = QStringLiteral("fixed");
    const QString length = QStringLiteral("length");
    const QString splitFlag = QStringLiteral("splitFlag");
    const QString maxCacheBytes = QStringLiteral("maxCacheBytes");
};

class FrameAnalyzer : public BaseNode
{
    Q_OBJECT
public:
    FrameAnalyzer(QObject *parent = nullptr);
    ~FrameAnalyzer() override;

    QString caption() const override;
    QString name() const override;
    unsigned int nPorts(QtNodes::PortType portType) const override;
    QWidget *embeddedWidget() override;

    void analyze(QByteArray &bytes);
    void analyzeFixed(QByteArray &bytes, int length);
    void analyzeSeparationMark(QByteArray &bytes, const QByteArray &splitFlag);
    void clearCacheBytes();

signals:
    void invokeClearCache();

private:
    QWidget *m_embeddedWidget{nullptr};
};
