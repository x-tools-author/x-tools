/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QChart>
#include <QMenu>
#include <QValueAxis>
#include <QXYSeries>

#include "IO/UI/AbstractIOUi.h"

namespace Ui {
class ChartsUi;
}

namespace xTools {

struct ChartsUiDataKeys
{
    const QString dataType{"dataType"};
    const QString testData{"testData"};
    const QString channels{"channels"};
    const QString legendVisible{"legendVisible"};
    const QString cachePoints{"cachePoints"};

    const QString channelName{"channelName"};
    const QString channelVisible{"channelVisible"};
    const QString channelColor{"channelColor"};
    const QString channelType{"channelType"};
};

class ChartsUiSettings;
class ChartsUi : public AbstractIOUi
{
    Q_OBJECT
public:
    explicit ChartsUi(QWidget *parent = Q_NULLPTR);
    ~ChartsUi() override;

    QVariantMap save() const override;
    void load(const QVariantMap &parameters) override;
    void setupIO(AbstractIO *io) override;

    QMenu *settingsMenu() const;

private:
    Ui::ChartsUi *ui;
    ChartsUiSettings *m_settings;
    QMenu *m_settingsMenu;
    QList<QXYSeries *> m_series;
    QChart *m_chart;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
    AbstractIO *m_io{nullptr};

private:
    void onNewValues(const QList<double> &values);
    void onNewPoints(const QList<QPointF> &points);

    void onSetDataType(int type);
    void onSetLegendVisible(bool visible);
    void onClearChannels();
    void onImportChannels();
    void onExportChannels();
    void onSetChannelVisible(int channelIndex, bool visible);
    void onSetChannelType(int channelIndex, int type);
    void onSetChannelColor(int channelIndex, const QColor &color);
    void onSetChannelName(int channelIndex, const QString &name);
};

} // namespace xTools
