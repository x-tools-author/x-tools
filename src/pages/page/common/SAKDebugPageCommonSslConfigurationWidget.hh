/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKDEBUGPAGECOMMONSSLCONFIGURATIONWIDGET_HH
#define SAKDEBUGPAGECOMMONSSLCONFIGURATIONWIDGET_HH

#include <QWidget>
#include <QComboBox>
#include <QSslEllipticCurve>
#include <QSslConfiguration>

namespace Ui {
    class SAKDebugPageCommonSslConfigurationWidget;
}

class SAKDebugPageCommonSslConfigurationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SAKDebugPageCommonSslConfigurationWidget(QWidget *parent = Q_NULLPTR);
    ~SAKDebugPageCommonSslConfigurationWidget();

    /**
     * @brief setupSslProtocolToComboBox: Add supported sll version to a combo box.
     * @param comboBox: Combo box which is used to show the ssl version list.
     */
    void setupSslProtocolToComboBox(QComboBox *comboBox);

    /**
     * @brief setupEllipticCurveToComboBox: Add supported elliptic curve to a combo box.
     * @param comboBox: Combo bo which is used to show the elliptic curve.
     */
    void setupEllipticCurveToComboBox(QComboBox *comboBox);
private:
    Ui::SAKDebugPageCommonSslConfigurationWidget *mUi;
    QComboBox *mSslProtocolComboBox;
    QComboBox *mEllipticCurveComboBox;
};

#endif // SAKSSLSOCKETCOMMONCONFIGURATION_HH
