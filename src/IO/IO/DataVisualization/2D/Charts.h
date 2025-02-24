/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "IO/IO/AbstractIO.h"

namespace xTools {

class Charts : public AbstractIO
{
    Q_OBJECT
public:
    enum class DataFormat { BinaryY, BinaryXY, TextY, TextXY };
    Q_ENUM(DataFormat);

public:
    Charts(QObject *parent = Q_NULLPTR);
    virtual ~Charts() override;

    virtual void inputBytes(const QByteArray &bytes) override;

signals:
    void newValues(const QList<double> &values);
    void newPoints(const QList<QPointF> &points);

protected:
    void run() override;

    QByteArray handleBinaryY();
    QByteArray handleTextY();
    QByteArray handleBinaryXY();
    QByteArray handleTextXY();

    void handleBinaryY(QByteArray &bytes);
    void handleTextY(QByteArray &bytes);
    void handleBinaryXY(QByteArray &bytes);
    void handleTextXY(QByteArray &bytes);

    Q_SIGNAL void input2run(const QByteArray &bytes);

private:
    const QByteArray m_binaryTail;
    int m_testAngle{0};
};

} // namespace xTools
