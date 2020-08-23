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
#include <QCheckBox>
#include <QSettings>
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
    explicit SAKDebugPageCommonSslConfigurationWidget(QSettings *settings, QWidget *parent = Q_NULLPTR);
    ~SAKDebugPageCommonSslConfigurationWidget();

    /**
     * @brief sslConfiguration: Generate a ssl configuration from settings.
     * @return A QSslConfiguration instance.
     */
    QSslConfiguration sslConfiguration();

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

    /**
     * @brief setupVerifyModeToComboBox: Add supported verify mode to a combo box.
     * @param comboBox: Combo box which is used to show the verify mode list.
     */
    void setupVerifyModeToComboBox(QComboBox *comboBox);
private:
    QSettings *mSettings;
    bool mEnableSetting;
    QString mSettingKeySslProtocol;
    QString mSettingKeyEllipticCurve;
    QString mSettingKeyKeyAlgorithm;
    QString mSettingKeyCipherSuite;
    QString mSettingKeyEncodingFormat;
    QString mSettingKeyCertification;
    QString mSettingKeyKeyPath;
    QString mSettingKeyUsingInnnerCert;
    QString mSettingKeyUsingInnerKey;
    QString mSettingKeyVerifyMode;
    QString mSettingKeyVerifyDepth;
    QString mSettingKeyPeerName;
private:
    Ui::SAKDebugPageCommonSslConfigurationWidget *mUi;
    QComboBox *mSslProtocolComboBox;
    QComboBox *mEllipticCurveComboBox;
    QComboBox *mKeyAlgorithmComboBox;
    QComboBox *mCipherSuiteComboBox;
    QComboBox *mEncodingFormatComboBox;
    QLineEdit *mCertificationLineEdit;
    QLineEdit *mKeyPathLineEdit;
    QPushButton *mImportCertPushButton;
    QPushButton *mImportKeyPathPushButton;
    QCheckBox *mUsingInnnerCertCheckBox;
    QCheckBox *mUsingInnerKeyCheckBox;
    QComboBox *mVerifyModeComboBox;
    QLineEdit *mVerifyDepthLineEdit;
    QPushButton *mOutportPushButton;
    QCheckBox *mLongNameCheckBox;
private slots:
    void on_sslProtocolComboBox_currentTextChanged(const QString &arg1);
    void on_keyAlgorithmComboBox_currentTextChanged(const QString &arg1);
    void on_encodingFormatComboBox_currentTextChanged(const QString &arg1);
    void on_certificateLineEdit_textChanged(const QString &arg1);
    void on_keyPathLineEdit_textChanged(const QString &arg1);
    void on_importCertPushButton_clicked();
    void on_importKeyPushButton_clicked();
    void on_usingInnerCertCheckBox_clicked();
    void on_usingInnerKeyCheckBox_clicked();
    void on_verifyModeComboBox_currentTextChanged(const QString &arg1);
    void on_verifyDepthLineEdit_textChanged(const QString &arg1);
    void on_outportPushButton_clicked();
    void on_longNameCheckBox_clicked();
};

#endif // SAKSSLSOCKETCOMMONCONFIGURATION_HH
