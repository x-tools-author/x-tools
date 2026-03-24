/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAction>
#include <QFileDialog>
#include <QMenu>
#include <QMutex>
#include <QScreen>
#include <QToolButton>

#include "coapglobal.h"
#include "common/xapp.h"
#include "ui_coapglobal.h"
#include "utilities/iconengine.h"

namespace Ui {
class CoAPGlobal;
}

namespace xCoAP {

class CoAPGlobalPrivate : public QObject
{
    Q_OBJECT
public:
    CoAPGlobalPrivate(CoAPGlobal* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {
        ui = new Ui::CoAPGlobal();
        ui->setupUi(q);
        ui->lineEditServerCachePath->setReadOnly(true);
        ui->toolButtonServerCachePath->setIcon(xIcon(":res/icons/folder.svg"));

        connect(ui->buttonBox, &QDialogButtonBox::accepted, q, [=]() { onAccepted(); });
        connect(ui->buttonBox, &QDialogButtonBox::rejected, q, [=]() { onRejected(); });
        connect(ui->toolButtonServerCachePath, &QToolButton::clicked, q, [=]() { onBrowse(); });

        // Hide something not used yet...
        const int count = ui->horizontalLayoutCache->count();
        for (int i = 0; i < count; ++i) {
            QWidget* w = ui->horizontalLayoutCache->itemAt(i)->widget();
            if (w) {
                w->hide();
            }
        }
        ui->checkBoxServerCache->hide();
        q->adjustSize();
    }
    ~CoAPGlobalPrivate() { delete ui; }

public:
    Ui::CoAPGlobal* ui{nullptr};
    QJsonObject m_cachedParameters;
    QMutex m_mutex;

public:
    void onAccepted() { q->close(); }
    void onRejected()
    {
        q->close();
        q->load(m_cachedParameters);
    }
    void onBrowse()
    {
        m_mutex.lock();
        const QString title = tr("Select CoAP Server Cache Directory");
        const QString path = ui->lineEditServerCachePath->text().trimmed();
        QFileDialog::Options options = QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks;
        QString dir = QFileDialog::getExistingDirectory(q, title, path, options);
        if (!dir.isEmpty()) {
            ui->lineEditServerCachePath->setText(dir);
        }
        m_mutex.unlock();
    }

private:
    CoAPGlobal* q{nullptr};
};

} // namespace xCoAP
