/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
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

namespace Ui {
class ChartsUiSettings;
}

namespace xTools {

class ChartsUiSettings : public QWidget
{
    Q_OBJECT
public:
    explicit ChartsUiSettings(QWidget *parent = Q_NULLPTR);
    ~ChartsUiSettings() override;

    static int channelCount();

    int dataType();
    void setDataType(int type);

    bool legendVisible();
    void setLegendVisible(bool visible);

    int cachePoints();
    void setCachePoints(int points);

    void load(const QVariantMap &parameters);

signals:
    void invokeSetDataType(int type);
    void invokeSetLegendVisible(bool visible);
    void invokeClearChannels();
    void invokeImportChannels();
    void invokeExportChannels();
    void invokeSetChannelVisible(int channelIndex, bool visible);
    void invokeSetChannelType(int channelIndex, int type);
    void invokeSetChannelColor(int channelIndex, const QColor &color);
    void invokeSetChannelName(int channelIndex, const QString &name);

private:
    struct ChannelContext
    {
        QCheckBox *visibleCheckBox;
        QComboBox *typeComboBox;
        QPushButton *colorButton;
        QLineEdit *nameLineEdit;
    };
    Ui::ChartsUiSettings *ui;
    QList<ChannelContext> m_channelContexts;

private:
    void setupVisibleCheckBox(QCheckBox *checkBox, int channelIndex);
    void setupTypeComboBox(QComboBox *comboBox, int channelIndex);
    void setupColorButton(QPushButton *button, int channelIndex);
    void setupNameLineEdit(QLineEdit *lineEdit, int channelIndex);

    QString seriesTypeToString(int type) const;
};

} // namespace xTools
