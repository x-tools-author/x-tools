/*
 * Copyright 2022 Qsaker(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKUDPCLIENTADVANCED_HH
#define SAKUDPCLIENTADVANCED_HH

#include <QMutex>
#include <QDialog>
#include <QSettings>
#include <QJsonObject>

namespace Ui {
    class SAKUdpClientAdvanced;
};

class SAKUdpClientAdvanced : public QDialog
{
    Q_OBJECT
public:
    SAKUdpClientAdvanced(QSettings *settings,
                         const QString settingsGroup,
                         QWidget *parent = Q_NULLPTR);
    ~SAKUdpClientAdvanced();
    const QJsonObject parameters();
private:
    Ui::SAKUdpClientAdvanced *ui;
    QSettings *mSettings;
    QString mGroup;
private:
    void updateParameters();
    void setParameters(const QJsonObject &parameters);
    void exportParameters();
    void importParameters();
    void clearUnicast();
    void clearMulticast();
    void clearBroadCast();
    void writeParametersToini(const QJsonObject &parameters);
    QJsonObject readParameterFromIni();
    int showClearMessageBox();
signals:
    void parametersUpdated(const QJsonObject &parameters);
};

#endif // SAKUDPCLIENTADVANCED_HH
