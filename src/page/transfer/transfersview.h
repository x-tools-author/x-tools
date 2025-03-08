/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QTabWidget>

class TransferView;
class TransfersView : public QTabWidget
{
    Q_OBJECT
public:
    explicit TransfersView(QWidget *parent = nullptr);
    ~TransfersView() override;

    void startAll();
    void stopAll();

    QVariantMap save() const;
    void load(const QVariantMap &data);

    void inputBytes(const QByteArray &bytes);
signals:
    void outputBytes(const QByteArray &bytes);

private:
    struct TransfersContext
    {
        QString name;
        TransferView *view;
    };
    QList<TransfersContext> m_transfersContextList;
};
