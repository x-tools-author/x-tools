/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
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

#include "../common/chartsettings.h"

namespace Ui {
class LineChartSettings;
}

class LineChartSettings : public ChartSettings
{
    Q_OBJECT
public:
    explicit LineChartSettings(QWidget *parent = Q_NULLPTR);
    ~LineChartSettings() override;

    QVariantMap save() const override;
    void load(const QVariantMap &parameters) override;

    static int channelCount();

    int dataType();
    void setDataType(int type);

    bool legendVisible();
    void setLegendVisible(bool visible);

    int cachePoints();
    void setCachePoints(int points);

    void updateUiState(bool ioIsOpened);

signals:
    void dataFormatChanged(int type);
    void invokeSetLegendVisible(bool visible);
    void channelVisibleChanged(int channelIndex, bool visible);
    void channelTypeChanged(int channelIndex, int type);
    void channelColorChanged(int channelIndex, const QColor &color);
    void channelNameChanged(int channelIndex, const QString &name);

    void invokeClearChannels();
    void invokeImportChannels();
    void invokeExportChannels();

private:
    struct ChannelContext
    {
        QCheckBox *visibleCheckBox;
        QComboBox *typeComboBox;
        QPushButton *colorButton;
        QLineEdit *nameLineEdit;
    };
    Ui::LineChartSettings *ui;
    QList<ChannelContext> m_channelContexts;

private:
    void setupVisibleCheckBox(QCheckBox *checkBox, int channelIndex);
    void setupTypeComboBox(QComboBox *comboBox, int channelIndex);
    void setupColorButton(QPushButton *button, int channelIndex);
    void setupNameLineEdit(QLineEdit *lineEdit, int channelIndex);

    QString seriesTypeToString(int type) const;
};
