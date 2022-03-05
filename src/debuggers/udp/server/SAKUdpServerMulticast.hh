/*
 * Copyright 2022 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKUDPSERVERMULTICAST_HH
#define SAKUDPSERVERMULTICAST_HH

#include <QDialog>
#include <QSettings>
#include <QCheckBox>
#include <QLineEdit>
#include <QJsonArray>
#include <QJsonObject>

namespace Ui {
    class SAKUdpServerMulticast;
}

class SAKUdpServerMulticast : public QDialog
{
    Q_OBJECT
public:
    SAKUdpServerMulticast(QSettings *settings,
                          const QString settingsGroup,
                          QWidget *parent = Q_NULLPTR);
    ~SAKUdpServerMulticast();

    const QJsonArray parameters();
    void updateUiState(bool opened);
private:
    Ui::SAKUdpServerMulticast *ui;
    QSettings *mSettings;
    const QString mSettingsGroup;
private:
    void setParameters(const QJsonArray &parameters);
    void updateParameters();
    void writeParametersToini(const QJsonArray &parameters);
    const QJsonArray readParameterFromIni();
    QCheckBox *cellCheckBox(int row);
    QLineEdit *cellLineEdit(int row);
    QWidget *cellWidget(int row, int column);
signals:
    void parametersChanged(QJsonArray parameters);
};

#endif // SAKUDPSERVERMULTICAST_HH
