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
#include <QMetaEnum>
#include "SAKDebugPageCommonSslConfigurationWidget.hh"
#include "ui_SAKDebugPageCommonSslConfigurationWidget.h"

SAKDebugPageCommonSslConfigurationWidget::SAKDebugPageCommonSslConfigurationWidget(QWidget *parent)
    :QWidget(parent)
    ,mUi(new Ui::SAKDebugPageCommonSslConfigurationWidget)
{
    mUi->setupUi(this);
    mSslProtocolComboBox = mUi->sslProtocolComboBox;
    mEllipticCurveComboBox = mUi->ellipticCurveComboBox;

    setupSslProtocolToComboBox(mSslProtocolComboBox);
    setupEllipticCurveToComboBox(mEllipticCurveComboBox);
}

SAKDebugPageCommonSslConfigurationWidget::~SAKDebugPageCommonSslConfigurationWidget()
{
    delete mUi;
}
void SAKDebugPageCommonSslConfigurationWidget::setupSslProtocolToComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->addItem(QString("SslV3"), (QSsl::SslV3));
        comboBox->addItem(QString("SslV2"), (QSsl::SslV2));
        comboBox->addItem(QString("TlsV1_0"), (QSsl::TlsV1_0));
        comboBox->addItem(QString("TlsV1_1"), (QSsl::TlsV1_1));
        comboBox->addItem(QString("TlsV1_2"), (QSsl::TlsV1_2));
        comboBox->addItem(QString("AnyProtocol"), (QSsl::AnyProtocol));
        comboBox->addItem(QString("TlsV1SslV3"), (QSsl::TlsV1SslV3));
        comboBox->addItem(QString("SecureProtocols"), (QSsl::SecureProtocols));
        comboBox->addItem(QString("TlsV1_0OrLater"), (QSsl::TlsV1_0OrLater));
        comboBox->addItem(QString("TlsV1_1OrLater"), (QSsl::TlsV1_1OrLater));
        comboBox->addItem(QString("TlsV1_2OrLater"), (QSsl::TlsV1_2OrLater));
        comboBox->addItem(QString("DtlsV1_0"), (QSsl::DtlsV1_0));
        comboBox->addItem(QString("DtlsV1_0OrLater"), (QSsl::DtlsV1_0OrLater));
        comboBox->addItem(QString("DtlsV1_2"), (QSsl::DtlsV1_2));
        comboBox->addItem(QString("DtlsV1_2OrLater"), (QSsl::DtlsV1_2OrLater));
        comboBox->addItem(QString("TlsV1_3"), (QSsl::TlsV1_3));
        comboBox->addItem(QString("TlsV1_3OrLater"), (QSsl::TlsV1_3OrLater));
    }else{
        Q_ASSERT_X(false, __FUNCTION__, "The parameter can not be a null value.");
    }
}

void SAKDebugPageCommonSslConfigurationWidget::setupEllipticCurveToComboBox(QComboBox *comboBox)
{
    if (comboBox){
        for (auto var : QSslConfiguration::supportedEllipticCurves()){
            comboBox->addItem(var.shortName(), var.shortName());
        }
    }else{
        Q_ASSERT_X(false, __FUNCTION__, "The parameter can not be a null value.");
    }
}
