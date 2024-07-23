/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QPushButton>
#include <QTimer>
#include <QVariantMap>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class IOPage;
}
QT_END_NAMESPACE

class InputSettings;
class OutputSettings;
class Communication;
class CommunicationUi;
class SyntaxHighlighter;
class CommunicationSettings;
class IOPage : public QWidget
{
    Q_OBJECT
public:
    explicit IOPage(QWidget *parent = nullptr);
    ~IOPage();

    QVariantMap save();
    void load(const QVariantMap &parameters);

private:
    struct
    {
        const QString communicationType{"communicationType"};
        const QString communicationSettings{"communicationSettings"};

        const QString outputFormat{"outputFormat"};
        const QString outputRx{"outputRx"};
        const QString outputTx{"outputTx"};
        const QString outputFlag{"outputFlag"};
        const QString outputDate{"outputDate"};
        const QString outputTime{"outputTime"};
        const QString outputMs{"outputMs"};
        const QString outputSettings{"outputSettings"};

        const QString cycleInterval{"cycleInterval"};
        const QString inputFormat{"inputFormat"};
        const QString inputSettings{"inputSettings"};
    } m_keys;

private:
    Ui::IOPage *ui;
    Communication *m_io;
    CommunicationUi *m_ioUi;
    CommunicationSettings *m_ioSettings;
    OutputSettings *m_outputSettings;
    InputSettings *m_inputSettings;
    QTimer *m_writeTimer;
    QTimer *m_updateLabelnfoTimer;
    SyntaxHighlighter *m_highlighter;

private:
    void initUi();
    void initUiCommunication();
    void initUiOutputControl();
    void initUiInputControl();
    void initUiOutput();
    void initUiInput();

    void onCommunicationTypeChanged();
    void onCycleIntervalChanged();
    void onInputFormatChanged();
    void onOpenButtonClicked();
    void onHighlighterEnableChanged();
    void onHighlighterKeywordsChanged();

    void onOpened();
    void onClosed();
    void onErrorOccurred(const QString &error);
    void onWarningOccurred(const QString &warning);
    void onBytesRead(const QByteArray &bytes, const QString &from);
    void onBytesWritten(const QByteArray &bytes, const QString &to);

    void open();
    void close();
    void writeBytes();
    void updateLabelInfo();
    void setupMenu(QPushButton *target, QWidget *actionWidget);
    void setUiEnabled(bool enabled);
    void outputText(const QByteArray &bytes, const QString &flag, bool isRx);

    QByteArray payload() const;
    QByteArray crc(const QByteArray &payload) const;
};
