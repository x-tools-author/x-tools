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
#include <QSslCipher>
#include <QPushButton>
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
     * @param comboBox: Combo bo which is used to show the elliptic curve list.
     * @param longName: If the value is true, long name will be append to combo box.
     */
    void setupEllipticCurveToComboBox(QComboBox *comboBox, bool longName = false);

    /**
     * @brief setupKeyAlgorithmToComboBox: Add supported key algorithm to a combo box.
     * @param comboBox: Combo box which is used to show the key algorithm list.
     */
    void setupKeyAlgorithmToComboBox(QComboBox *comboBox);

    /**
     * @brief setupCipherSuiteToComboBox: Add supported cipher suite to a combo box.
     * @param comboBox: Combo box which is used to show the cipher suite list.
     */
    void setupCipherSuiteToComboBox(QComboBox *comboBox);

    /**
     * @brief setupEncodingFormatToComboBox: Add supported encoding format to a combo box.
     * @param comboBox: Combo box which is used to show the encoding format list.
     */
    void setupEncodingFormatToComboBox(QComboBox *comboBox);
private:
    Ui::SAKDebugPageCommonSslConfigurationWidget *mUi;
    QComboBox *mSslProtocolComboBox;
    QComboBox *mEllipticCurveComboBox;
    QComboBox *mKeyAlgorithmComboBox;
    QComboBox *mCipherSuiteComboBox;
    QComboBox *mEncodingFormatComboBox;
    QLineEdit *mCertificationLineEdit;
    QLineEdit *keyPathLineEdit;
    QPushButton *importCertPushButton;
    QPushButton *importKeyPathPushButton;
};

#endif // SAKSSLSOCKETCOMMONCONFIGURATION_HH
