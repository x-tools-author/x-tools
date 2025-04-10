/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QButtonGroup>
#include <QPushButton>
#include <QRegularExpression>
#include <QSettings>
#include <QTabWidget>
#include <QTimer>
#include <QToolButton>
#include <QVariantMap>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Page;
}
QT_END_NAMESPACE

#ifdef X_ENABLE_CHARTS
class ChartsView;
#endif

class Statistician;
class InputSettings;
class OutputSettings;
class DeviceSettings;
class SyntaxHighlighter;

class Device;
class DeviceUi;
class Page : public QWidget
{
    Q_OBJECT
public:
    enum ControllerDirection { Left, Right };

public:
    explicit Page(ControllerDirection direction, QSettings *settings, QWidget *parent = nullptr);
    ~Page() override;

    QVariantMap save();
    void load(const QVariantMap &parameters);
    QTabWidget *tabWidget();
    QToolButton *presetToolButton();
    void inputBytes(const QByteArray &bytes);
    void prependOutputControl(QWidget *widget);
    void appendOutputControl(QWidget *widget);
    void hideChartsWidgets();
    void hideTransferWidgets();
    void removeTestDevices();

signals:
    void bytesWritten(const QByteArray &bytes, const QString &to);
    void bytesRead(const QByteArray &bytes, const QString &from);

private:
    void initUi();
    void initUiDeviceControl();
    void initUiOutputControl();
    void initUiInputControl();
    void initUiOutput();
    void initUiInput();

    void onDeviceTypeChanged();
    void onCycleIntervalChanged();
    void onInputFormatChanged();
    void onOpenButtonClicked();
    void onHighlighterEnableChanged();
    void onHighlighterKeywordsChanged();
    void onShowStatisticianChanged(bool checked);

    void onOpened();
    void onClosed();
    void onErrorOccurred(const QString &error);
    void onWarningOccurred(const QString &warning);
    void onBytesRead(const QByteArray &bytes, const QString &from);
    void onBytesWritten(const QByteArray &bytes, const QString &to);
    void onWrapModeChanged();
    void onSearchButtonClicked();
    void onSearchTextChanged();
    void performSearch();
    void highlightSearchResults(const QString &text, const QRegularExpression &regex);
    void highlightSearchResultsForLine(const QString &line, const QRegularExpression &regex);
    void performSearch(QString &line);

    void openDevice();
    void closeDevice();
    void setupDevice(Device *device);
    void writeBytes();
    void updateLabelInfo();
    void setupMenu(QPushButton *target, QWidget *actionWidget);
    void setUiEnabled(bool enabled);
    void outputText(const QByteArray &bytes, const QString &flag, bool isRx);
    void saveControllerParameters();
    void loadControllerParameters();
    void updateChartUi();

    QByteArray payload() const;
    QByteArray crc(const QByteArray &payload) const;
    DeviceUi *newDeviceUi(int type);

private:
    Ui::Page *ui;
    DeviceUi *m_deviceController;
    DeviceSettings *m_ioSettings;
    OutputSettings *m_outputSettings;
    InputSettings *m_inputSettings;
    SyntaxHighlighter *m_highlighter;
    Statistician *m_rxStatistician;
    Statistician *m_txStatistician;

    QTimer *m_writeTimer;
    QTimer *m_updateLabelInfoTimer;
    QSettings *m_settings;
#ifdef X_ENABLE_CHARTS
    ChartsView *m_chartsView;
    bool m_enableChars{true};
#endif
};
