/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class CommunicationSettings;
}
QT_END_NAMESPACE

class SaveThread;
class CommunicationSettings : public QWidget
{
    Q_OBJECT
public:
    CommunicationSettings(QWidget *parent = nullptr);
    ~CommunicationSettings();

    void saveData(const QByteArray &data, bool isRx);
    QVariantMap save();
    void load(const QVariantMap &data);
    void addWidgets(QList<QWidget *> widgets);

private:
    struct
    {
        const QString saveToFilePath{"saveToFilePath"};
    } m_settingKeys;

private:
    Ui::CommunicationSettings *ui;
    SaveThread *m_saveThread;
    QString m_fileName;

private:
    void onBrowserButtonClicked();
};
