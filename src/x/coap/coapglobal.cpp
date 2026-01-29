/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "coapglobal.h"
#include "ui_coapglobal.h"

#include <QAction>
#include <QMenu>
#include <QScreen>
#include <QToolButton>

namespace Ui {
class CoAPGlobal;
}

namespace xCoAP {

struct CoAPGlobalSettingKeys
{
    const QString clientViewVisible{"clientViewVisible"};
    const QString serverViewVisible{"serverViewVisible"};
    const QString client{"client"};
    const QString server{"server"};
};

class CoAPGlobalPrivate : public QObject
{
public:
    CoAPGlobalPrivate(CoAPGlobal* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {
        ui = new Ui::CoAPGlobal();
        ui->setupUi(q);

        connect(ui->buttonBox, &QDialogButtonBox::accepted, q, [=]() { onAccepted(); });
        connect(ui->buttonBox, &QDialogButtonBox::rejected, q, [=]() { onRejected(); });
    }
    ~CoAPGlobalPrivate() { delete ui; }

public:
    Ui::CoAPGlobal* ui{nullptr};
    QJsonObject m_cachedParameters;

public:
    void onAccepted() { q->close(); }
    void onRejected()
    {
        q->close();
        q->load(m_cachedParameters);
    }

private:
    CoAPGlobal* q{nullptr};
};

struct CoAPGlobalParameterKeys
{
    const QString enableClientName{"enableClientName"};
    const QString clientName{"clientName"};
    const QString enableToken{"enableToken"};
    const QString tokenLength{"tokenLength"};

    const QString enableServerName{"enableServerName"};
    const QString serverName{"serverName"};
};

CoAPGlobal::CoAPGlobal(QWidget* parent)
    : QWidget(parent)
{
    d = new CoAPGlobalPrivate(this);
    setAttribute(Qt::WA_ShowModal, true);
    adjustSize();
    setWindowFlag(Qt::WindowType::WindowMaximizeButtonHint, false);
    setWindowFlag(Qt::WindowType::WindowMinimizeButtonHint, false);
}

CoAPGlobal::~CoAPGlobal() {}

CoAPGlobal& CoAPGlobal::singleton()
{
    static CoAPGlobal instance;
    return instance;
}

void CoAPGlobal::showThenMoveToCenter()
{
    show();
    QRect screenGeometry = screen()->geometry();
    move(screenGeometry.center() - rect().center());
}

QJsonObject CoAPGlobal::save()
{
    CoAPGlobalParameterKeys keys;
    QJsonObject obj;
    obj.insert(keys.enableClientName, d->ui->checkBoxClientName->isChecked());
    obj.insert(keys.clientName, d->ui->lineEditClientName->text().trimmed());
    obj.insert(keys.enableToken, d->ui->checkBoxTokenLength->isChecked());
    obj.insert(keys.tokenLength, d->ui->spinBoxTokenLength->value());
    obj.insert(keys.enableServerName, d->ui->checkBoxServerName->isChecked());
    obj.insert(keys.serverName, d->ui->lineEditServerName->text().trimmed());
    return obj;
}

void CoAPGlobal::load(const QJsonObject& obj)
{
    CoAPGlobalParameterKeys keys;
    const QString defaultClientName = QString("com.x-tools.x-coap.client");
    const QString defaultServerName = QString("com.x-tools.x-coap.server");
    d->ui->checkBoxClientName->setChecked(obj.value(keys.enableClientName).toBool(false));
    d->ui->lineEditClientName->setText(obj.value(keys.clientName).toString(defaultClientName));
    d->ui->checkBoxTokenLength->setChecked(obj.value(keys.enableToken).toBool(false));
    d->ui->spinBoxTokenLength->setValue(obj.value(keys.tokenLength).toInt(0));
    d->ui->checkBoxServerName->setChecked(obj.value(keys.enableServerName).toBool(false));
    d->ui->lineEditServerName->setText(obj.value(keys.serverName).toString(defaultServerName));
    d->m_cachedParameters = save();
}

bool CoAPGlobal::isClientNameEnabled() const
{
    return d->ui->checkBoxClientName->isChecked();
}

QString CoAPGlobal::clientName() const
{
    return d->ui->lineEditClientName->text().trimmed();
}

bool CoAPGlobal::isTokenEnabled() const
{
    return d->ui->checkBoxTokenLength->isChecked();
}

int CoAPGlobal::tokenLength() const
{
    return d->ui->spinBoxTokenLength->value();
}

bool CoAPGlobal::isServerNameEnabled() const
{
    return d->ui->checkBoxServerName->isChecked();
}

QString CoAPGlobal::serverName() const
{
    return d->ui->lineEditServerName->text().trimmed();
}

} // namespace xCoAP
