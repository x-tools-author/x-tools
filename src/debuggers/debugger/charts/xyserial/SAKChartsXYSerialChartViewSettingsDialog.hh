/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKCHARTSXYSERIALCHARTVIEWSETTINGSDIALOG_HH
#define SAKCHARTSXYSERIALCHARTVIEWSETTINGSDIALOG_HH

#include <QMap>
#include <QDialog>
#include <QSpinBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>

namespace Ui {
    class SAKChartsXYSerialChartViewSettingsDialog;
};

/// @brief Charts parameters settings
class SAKChartsXYSerialChartViewSettingsDialog:public QDialog
{
    Q_OBJECT
public:
    SAKChartsXYSerialChartViewSettingsDialog(QWidget *parent = Q_NULLPTR);
    ~SAKChartsXYSerialChartViewSettingsDialog();
private:
    Ui::SAKChartsXYSerialChartViewSettingsDialog *mUi;
};

#endif
