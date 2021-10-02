/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QSsl>
#include <QFile>
#include <QSslKey>
#include <QMetaEnum>
#include <QStandardItem>
#include <QSslCertificate>
#include <QStandardItemModel>

#include "SAKDebugPageCommonSslConfigurationWidget.hh"
#include "ui_SAKDebugPageCommonSslConfigurationWidget.h"

SAKDebugPageCommonSslConfigurationWidget::SAKDebugPageCommonSslConfigurationWidget(QSettings *settings, QWidget *parent)
    :QWidget(parent)
    ,mSettings(settings)
    ,mEnableSetting(false)
    ,mSettingKeySslProtocol(QString("SSL/protocol"))
    ,mSettingKeyEllipticCurve(QString("SSL/ellipticCurve"))
    ,mSettingKeyKeyAlgorithm(QString("SSL/keyAlgorithm"))
    ,mSettingKeyCipherSuite(QString("SSL/cipherSuite"))
    ,mSettingKeyEncodingFormat(QString("SSL/encodingFormat"))
    ,mSettingKeyCertification(QString("SSL/certification"))
    ,mSettingKeyKeyPath(QString("SSL/keyPath"))
    ,mSettingKeyUsingInnnerCert(QString("SSL/usingInnnerCert"))
    ,mSettingKeyUsingInnerKey(QString("SSL/usingInnerKey"))
    ,mSettingKeyVerifyMode(QString("SSL/peerVerifyMode"))
    ,mSettingKeyVerifyDepth(QString("SSL/peerVerifyDepth"))
    ,mSettingKeyPeerName(QString("SSL/peerName"))
    ,mUi(new Ui::SAKDebugPageCommonSslConfigurationWidget)
{
    mUi->setupUi(this);
    mSslProtocolComboBox = mUi->sslProtocolComboBox;
    mEllipticCurveComboBox = mUi->ellipticCurveComboBox;
    mKeyAlgorithmComboBox = mUi->keyAlgorithmComboBox;
    mCipherSuiteComboBox = mUi->cipherSuiteComboBox;
    mEncodingFormatComboBox = mUi->encodingFormatComboBox;
    mCertificationLineEdit = mUi->certificateLineEdit;
    mKeyPathLineEdit = mUi->keyPathLineEdit;
    mImportCertPushButton = mUi->importCertPushButton;
    mImportKeyPathPushButton = mUi->importKeyPushButton;
    mUsingInnnerCertCheckBox = mUi->usingInnerCertCheckBox;
    mUsingInnerKeyCheckBox = mUi->usingInnerKeyCheckBox;
    mVerifyModeComboBox = mUi->verifyModeComboBox;
    mVerifyDepthLineEdit = mUi->verifyDepthLineEdit;
    mOutportPushButton = mUi->outportPushButton;
    mLongNameCheckBox = mUi->longNameCheckBox;

    setupSslProtocolToComboBox(mSslProtocolComboBox);
    setupEllipticCurveToComboBox(mEllipticCurveComboBox, true);
    setupKeyAlgorithmToComboBox(mKeyAlgorithmComboBox);
    setupCipherSuiteToComboBox(mCipherSuiteComboBox);
    setupEncodingFormatToComboBox(mEncodingFormatComboBox);
    setupVerifyModeToComboBox(mVerifyModeComboBox);

    Q_ASSERT_X(mSettings, __FUNCTION__, "The parameter can not be a null value.");
    if (mSettings){
        mSslProtocolComboBox->setCurrentText(mSettings->value(mSettingKeySslProtocol).toString());
        mEllipticCurveComboBox->setCurrentText(mSettings->value(mSettingKeyEllipticCurve).toString());
        mKeyAlgorithmComboBox->setCurrentText(mSettings->value(mSettingKeyKeyAlgorithm).toString());
        mCipherSuiteComboBox->setCurrentText(mSettings->value(mSettingKeyCipherSuite).toString());
        mEncodingFormatComboBox->setCurrentText(mSettings->value(mSettingKeyEncodingFormat).toString());
        mCertificationLineEdit->setText(mSettings->value(mSettingKeyCertification).toString());
        mKeyPathLineEdit->setText(mSettings->value(mSettingKeyKeyPath).toString());
        mUsingInnnerCertCheckBox->setChecked(mSettings->value(mSettingKeyUsingInnnerCert).toBool());
        mUsingInnerKeyCheckBox->setChecked(mSettings->value(mSettingKeyUsingInnerKey).toBool());
        mVerifyModeComboBox->setCurrentText(mSettings->value(mSettingKeyVerifyMode).toString());
        mVerifyDepthLineEdit->setText(mSettings->value(mSettingKeyVerifyDepth).toString());
    }

    mEnableSetting = true;
}

SAKDebugPageCommonSslConfigurationWidget::~SAKDebugPageCommonSslConfigurationWidget()
{
    delete mUi;
}

QSslConfiguration SAKDebugPageCommonSslConfigurationWidget::sslConfiguration()
{
    QSsl::SslProtocol protocol = static_cast<QSsl::SslProtocol>(mSslProtocolComboBox->currentData().toInt());
    QSsl::KeyAlgorithm keyAlgorithm = static_cast<QSsl::KeyAlgorithm>(mKeyAlgorithmComboBox->currentData().toInt());
    QSsl::EncodingFormat encodingFormat = static_cast<QSsl::EncodingFormat>(mEncodingFormatComboBox->currentData().toInt());
    QSslSocket::PeerVerifyMode peerVerifyMode = static_cast<QSslSocket::PeerVerifyMode>(mVerifyModeComboBox->currentData().toInt());

    QSslConfiguration sslConfiguration;
    sslConfiguration.setProtocol(protocol);
    QFile certFile(QString(mCertificationLineEdit->text()));
    QFile keyFile(QString(mKeyPathLineEdit->text()));
    certFile.open(QIODevice::ReadOnly);
    keyFile.open(QIODevice::ReadOnly);
    QSslCertificate certificate(&certFile, encodingFormat);
    QSslKey sslKey(&keyFile, keyAlgorithm, encodingFormat);
    certFile.close();
    keyFile.close();
    sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConfiguration.setLocalCertificate(certificate);
    sslConfiguration.setPrivateKey(sslKey);
    sslConfiguration.setPeerVerifyMode(peerVerifyMode);

    return sslConfiguration;
}

void SAKDebugPageCommonSslConfigurationWidget::setupSslProtocolToComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
        comboBox->addItem(QString("SslV3"), (QSsl::SslV3));
        comboBox->addItem(QString("SslV2"), (QSsl::SslV2));
#endif
        comboBox->addItem(QString("TlsV1_0"), (QSsl::TlsV1_0));
        comboBox->addItem(QString("TlsV1_1"), (QSsl::TlsV1_1));
        comboBox->addItem(QString("TlsV1_2"), (QSsl::TlsV1_2));
        comboBox->addItem(QString("AnyProtocol"), (QSsl::AnyProtocol));
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
        comboBox->addItem(QString("TlsV1SslV3"), (QSsl::TlsV1SslV3));
#endif
        comboBox->addItem(QString("SecureProtocols"), (QSsl::SecureProtocols));
        comboBox->addItem(QString("TlsV1_0OrLater"), (QSsl::TlsV1_0OrLater));
        comboBox->addItem(QString("TlsV1_1OrLater"), (QSsl::TlsV1_1OrLater));
        comboBox->addItem(QString("TlsV1_2OrLater"), (QSsl::TlsV1_2OrLater));
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        comboBox->addItem(QString("DtlsV1_0"), (QSsl::DtlsV1_0));
        comboBox->addItem(QString("DtlsV1_0OrLater"), (QSsl::DtlsV1_0OrLater));
        comboBox->addItem(QString("DtlsV1_2"), (QSsl::DtlsV1_2));
        comboBox->addItem(QString("DtlsV1_2OrLater"), (QSsl::DtlsV1_2OrLater));
        comboBox->addItem(QString("TlsV1_3"), (QSsl::TlsV1_3));
        comboBox->addItem(QString("TlsV1_3OrLater"), (QSsl::TlsV1_3OrLater));
#endif
        comboBox->setCurrentText(QString("SecureProtocols"));
    }else{
        Q_ASSERT_X(false, __FUNCTION__, "The parameter can not be a null value.");
    }
}

void SAKDebugPageCommonSslConfigurationWidget::setupEllipticCurveToComboBox(QComboBox *comboBox, bool longName)
{
    if (comboBox){
        comboBox->clear();
        for (auto &var : QSslConfiguration::supportedEllipticCurves()){
            comboBox->addItem(longName ? var.longName() : var.shortName());
        }
    }else{
        Q_ASSERT_X(false, __FUNCTION__, "The parameter can not be a null value.");
    }
}

void SAKDebugPageCommonSslConfigurationWidget::setupKeyAlgorithmToComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
        comboBox->addItem(QString("Rsa"), QSsl::Rsa);
        comboBox->addItem(QString("Dsa"), QSsl::Dsa);
        comboBox->addItem(QString("Ec"), QSsl::Ec);
        comboBox->addItem(QString("Opaque"), QSsl::Opaque);
    }else{
        Q_ASSERT_X(false, __FUNCTION__, "The parameter can not be a null value.");
    }
}

void SAKDebugPageCommonSslConfigurationWidget::setupCipherSuiteToComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
        QStandardItemModel *itemModel = new QStandardItemModel(comboBox);
        QList<QSslCipher> sslCipher = QSslConfiguration::supportedCiphers();
        for (auto &var : sslCipher){
            QStandardItem *item = new QStandardItem(var.name());
            item->setToolTip(var.name());
            itemModel->appendRow(item);
        }
        comboBox->setModel(itemModel);
    }else{
        Q_ASSERT_X(false, __FUNCTION__, "The parameter can not be a null value.");
    }
}

void SAKDebugPageCommonSslConfigurationWidget::setupEncodingFormatToComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
        comboBox->addItem(QString("Pem"), QSsl::Pem);
        comboBox->addItem(QString("Der"), QSsl::Der);
    }else{
        Q_ASSERT_X(false, __FUNCTION__, "The parameter can not be a null value.");
    }
}

void SAKDebugPageCommonSslConfigurationWidget::setupVerifyModeToComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
        comboBox->addItem(QString("VerifyNone"), QSslSocket::VerifyNone);
        comboBox->addItem(QString("QueryPeer"), QSslSocket::QueryPeer);
        comboBox->addItem(QString("VerifyPeer"), QSslSocket::VerifyPeer);
        comboBox->addItem(QString("AutoVerifyPeer"), QSslSocket::AutoVerifyPeer);
    }else{
        Q_ASSERT_X(false, __FUNCTION__, "The parameter can not be a null value.");
    }
}

void SAKDebugPageCommonSslConfigurationWidget::on_sslProtocolComboBox_currentTextChanged(const QString &arg1)
{
    if (mEnableSetting && mSettings){
        mSettings->setValue(mSettingKeySslProtocol, arg1);
    }
}

void SAKDebugPageCommonSslConfigurationWidget::on_keyAlgorithmComboBox_currentTextChanged(const QString &arg1)
{
    if (mEnableSetting && mSettings){
        mSettings->setValue(mSettingKeyKeyAlgorithm, arg1);
    }
}

void SAKDebugPageCommonSslConfigurationWidget::on_encodingFormatComboBox_currentTextChanged(const QString &arg1)
{
    if (mEnableSetting && mSettings){
        mSettings->setValue(mSettingKeyEncodingFormat, arg1);
    }
}

void SAKDebugPageCommonSslConfigurationWidget::on_certificateLineEdit_textChanged(const QString &arg1)
{
    if (mEnableSetting && mSettings){
        mSettings->setValue(mSettingKeyCertification, arg1);
    }
}

void SAKDebugPageCommonSslConfigurationWidget::on_keyPathLineEdit_textChanged(const QString &arg1)
{
    if (mEnableSetting && mSettings){
        mSettings->setValue(mSettingKeyKeyPath, arg1);
    }
}

void SAKDebugPageCommonSslConfigurationWidget::on_importCertPushButton_clicked()
{

}

void SAKDebugPageCommonSslConfigurationWidget::on_importKeyPushButton_clicked()
{

}

void SAKDebugPageCommonSslConfigurationWidget::on_usingInnerCertCheckBox_clicked()
{
    if (mEnableSetting && mSettings){
        mSettings->setValue(mSettingKeyUsingInnnerCert, mUsingInnnerCertCheckBox->isChecked());
    }
}

void SAKDebugPageCommonSslConfigurationWidget::on_usingInnerKeyCheckBox_clicked()
{
    if (mEnableSetting && mSettings){
        mSettings->setValue(mSettingKeyUsingInnerKey, mUsingInnerKeyCheckBox->isChecked());
    }
}

void SAKDebugPageCommonSslConfigurationWidget::on_verifyModeComboBox_currentTextChanged(const QString &arg1)
{
    if (mEnableSetting && mSettings){
        mSettings->setValue(mSettingKeyVerifyMode, arg1);
    }
}

void SAKDebugPageCommonSslConfigurationWidget::on_verifyDepthLineEdit_textChanged(const QString &arg1)
{
    if (mEnableSetting && mSettings){
        mSettings->setValue(mSettingKeyVerifyDepth, arg1);
    }
}

void SAKDebugPageCommonSslConfigurationWidget::on_outportPushButton_clicked()
{

}

void SAKDebugPageCommonSslConfigurationWidget::on_longNameCheckBox_clicked()
{
    setupEllipticCurveToComboBox(mEllipticCurveComboBox, mLongNameCheckBox->isChecked());
}
