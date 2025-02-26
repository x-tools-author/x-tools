/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "statisticianui.h"
#include "ui_statisticianui.h"

#include "statistician.h"

namespace xTools {

StatisticianUi::StatisticianUi(QWidget *parent)
    : AbstractIOUi{parent}
    , ui(new Ui::StatisticianUi)
    , m_statistician{nullptr}
{
    ui->setupUi(this);
    updateInfo();
}

StatisticianUi::~StatisticianUi()
{
    delete ui;
}

QVariantMap StatisticianUi::save() const
{
    return {};
}

void StatisticianUi::load(const QVariantMap &parameters)
{
    Q_UNUSED(parameters);
}

void StatisticianUi::setupIO(AbstractIO *io)
{
    if (m_statistician) {
        disconnect(m_statistician, nullptr, this, nullptr);
    }

    m_statistician = qobject_cast<Statistician *>(io);
    if (!m_statistician) {
        return;
    }

    connect(m_statistician, &Statistician::bytesChanged, this, &StatisticianUi::updateInfo);
    connect(m_statistician, &Statistician::framesChanged, this, &StatisticianUi::updateInfo);
    connect(m_statistician, &Statistician::speedChanged, this, &StatisticianUi::updateInfo);
}

void StatisticianUi::updateInfo()
{
    int frame = 0;
    int bytes = 0;
    int speed = 0;

    if (m_statistician) {
        frame = m_statistician->frames();
        bytes = m_statistician->bytes();
        speed = m_statistician->speed();
    }

    QString info = tr("%1 frames, %2 bytes, %3B/s").arg(frame).arg(bytes).arg(speed);
    ui->label->setText(info);
}

} // namespace xTools
