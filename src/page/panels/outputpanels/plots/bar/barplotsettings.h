/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>

#include "../common/plotsettings.h"

namespace Ui {
class BarPlotSettings;
}

struct BarPlotSettingsKeys
{
    const QString dataFormat{"dataFormat"};
    const QString channels{"channels"};

    const struct
    {
        const QString channelName{"channelName"};
        const QString channelColor{"channelColor"};
    } channel;
};

class BarPlotSettings : public PlotSettings
{
    Q_OBJECT
public:
    explicit BarPlotSettings(QWidget *parent = Q_NULLPTR);
    ~BarPlotSettings() override;

    QVariantMap save() const override;
    void load(const QVariantMap &parameters) override;

    static int channelCount();

    int dataType();
    void setDataType(int type);

    void updateUiState(bool ioIsOpened);

signals:
    void invokeSetLegendVisible(bool visible);

    void dataFormatChanged(int dataFormat);
    void channelVisibleChanged(int channelIndex, bool visible);
    void channelTypeChanged(int channelIndex, int type);
    void channelColorChanged(int channelIndex, const QColor &color);
    void channelNameChanged(int channelIndex, const QString &name);

private:
    struct ChannelContext
    {
        QCheckBox *visibleCheckBox;
        QComboBox *typeComboBox;
        QPushButton *colorButton;
        QLineEdit *nameLineEdit;
    };
    Ui::BarPlotSettings *ui;
    QList<ChannelContext> m_channelContexts;

private:
    void setupColorButton(QPushButton *button, int channelIndex);
    void setupNameLineEdit(QLineEdit *lineEdit, int channelIndex);

    QString seriesTypeToString(int type) const;
};
