/***************************************************************************************************
 * Copyright 2024-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
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

class EmitterView;
class PresetPanel;
class ResponderView;
class TransfersView;
class ScriptsManager;
class DataRecordsView;
class SearchPanel;
class BarPanel;
class LinePanel;
class LuaView;

class Device;
class DeviceUi;
class Statistician;
class InputSettings;
class OutputSettings;
class DeviceSettings;
class SyntaxHighlighter;

class PagePrivate;
class Page : public QWidget
{
    Q_OBJECT
    PagePrivate *d{nullptr};

public:
    enum ControllerDirection { Left, Right };

public:
    explicit Page(ControllerDirection direction, QSettings *settings, QWidget *parent = nullptr);
    ~Page() override;

    QVariantMap save() const;
    void load(const QVariantMap &parameters);
    QTabWidget *tabWidget();
    QToolButton *presetToolButton();
    void inputBytes(const QByteArray &bytes);
    void prependOutputControl(QWidget *widget);
    void appendOutputControl(QWidget *widget);
    void hideTransferWidgets();
    void removeTestDevices();
    void aboutToClose();
    void showLiteMode();

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
    void onTerminalModeChanged();
    void onExternalPanelButtonClicked(bool checked);

    void openDevice();
    void closeDevice();
    void setupDevice(Device *device);
    void writeBytes();
    void writeSpecifiedBytes(const QByteArray &bytes);
    void updateLabelInfo();
    void setupMenu(QPushButton *target, QWidget *actionWidget);
    void setUiEnabled(bool enabled);
    void outputText(const QByteArray &bytes, const QString &flag, bool isRx);
    void saveControllerParameters();
    void loadControllerParameters();

    void addTab(const QString &name, QWidget *widget);
    void setTab(QWidget *widget);

    QByteArray payload() const;
    QByteArray crc(const QByteArray &payload) const;

private:
    Ui::Page *ui;
    DeviceUi *m_deviceController;
    DeviceSettings *m_ioSettings;
    OutputSettings *m_outputSettings;
    InputSettings *m_inputSettings;
    SyntaxHighlighter *m_highlighter;
    Statistician *m_rxStatistician;
    Statistician *m_txStatistician;

    PresetPanel *m_presetPanel{nullptr};
    EmitterView *m_emitterView{nullptr};
    ResponderView *m_responderView{nullptr};
    TransfersView *m_transfersView{nullptr};
    ScriptsManager *m_scriptsManager{nullptr};
    DataRecordsView *m_dataRecordsView{nullptr};
    SearchPanel *m_filterView{nullptr};
    BarPanel *m_barPanel{nullptr};
    LinePanel *m_linePanel{nullptr};
    LuaView *m_luaView{nullptr};

    QList<QToolButton *> m_tabToolButtons;

    QTimer *m_writeTimer;
    QTimer *m_updateLabelInfoTimer;
    QSettings *m_settings;
};
