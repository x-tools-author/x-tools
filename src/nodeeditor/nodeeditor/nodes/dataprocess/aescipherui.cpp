/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "aescipherui.h"
#include "ui_aescipherui.h"

#include <qaesencryption.h>

#include "aescipher.h"

AesCipherUi::AesCipherUi(BaseNode *node, QWidget *parent)
    : BaseNodeUi(node, parent)
    , ui(new Ui::AesCipherUi)
{
    QWidget *w = new QWidget(this);
    ui->setupUi(w);
    setEmbeddedWidget(w);

    ui->comboBoxWay->addItem(tr("Encrypt"), static_cast<int>(AesCipher::WorkWay::Encrypt));
    ui->comboBoxWay->addItem(tr("Decrypt"), static_cast<int>(AesCipher::WorkWay::Decrypt));

    ui->comboBoxLevel->addItem("AES_128", static_cast<int>(QAESEncryption::AES_128));
    ui->comboBoxLevel->addItem("AES_192", static_cast<int>(QAESEncryption::AES_192));
    ui->comboBoxLevel->addItem("AES_256", static_cast<int>(QAESEncryption::AES_256));

    ui->comboBoxMode->addItem("ECB", static_cast<int>(QAESEncryption::ECB));
    ui->comboBoxMode->addItem("CBC", static_cast<int>(QAESEncryption::CBC));
    ui->comboBoxMode->addItem("CFB", static_cast<int>(QAESEncryption::CFB));
    ui->comboBoxMode->addItem("OFB", static_cast<int>(QAESEncryption::OFB));

    ui->comboBoxPaddingStandard->addItem("Zero", static_cast<int>(QAESEncryption::ZERO));
    ui->comboBoxPaddingStandard->addItem("PKCS7", static_cast<int>(QAESEncryption::PKCS7));
    ui->comboBoxPaddingStandard->addItem("ISO", static_cast<int>(QAESEncryption::ISO));
    ui->comboBoxPaddingStandard->setCurrentIndex(2);

    ui->lineEditKey->setText("0123456789abcdef");
    ui->lineEditIv->setText("0123456789abcdef");
}

AesCipherUi::~AesCipherUi()
{
    delete ui;
}

QJsonObject AesCipherUi::save() const
{
    QJsonObject parameters = BaseNodeUi::save();
    AesCipherParametersKeys keys;
    parameters.insert(keys.workWay, ui->comboBoxWay->currentData().toInt());
    parameters.insert(keys.key, ui->lineEditKey->text());
    parameters.insert(keys.iv, ui->lineEditIv->text());
    parameters.insert(keys.mode, ui->comboBoxMode->currentData().toInt());
    parameters.insert(keys.padding, ui->comboBoxPaddingStandard->currentData().toInt());
    parameters.insert(keys.level, ui->comboBoxLevel->currentData().toInt());
    return parameters;
}

void AesCipherUi::load(const QJsonObject &parameters)
{
    BaseNodeUi::load(parameters);
    AesCipherParametersKeys keys;
    QVariantMap parametersMap = parameters.toVariantMap();

    int defaultWorkWay = static_cast<int>(AesCipher::WorkWay::Encrypt);
    QString key = parametersMap.value(keys.key, QString("0123456789abcdef")).toString();
    QString iv = parametersMap.value(keys.iv, QString("0123456789abcdef")).toString();
    int workWay = parametersMap.value(keys.workWay, defaultWorkWay).toInt();
    int mode = parametersMap.value(keys.mode, static_cast<int>(QAESEncryption::ECB)).toInt();
    int padding = parametersMap.value(keys.padding, static_cast<int>(QAESEncryption::ZERO)).toInt();
    int level = parametersMap.value(keys.level, static_cast<int>(QAESEncryption::AES_128)).toInt();

    ui->lineEditKey->setText(key);
    ui->lineEditIv->setText(iv);

    int index = ui->comboBoxWay->findData(workWay);
    ui->comboBoxWay->setCurrentIndex(index < 0 ? 0 : index);
    index = ui->comboBoxLevel->findData(level);
    ui->comboBoxMode->setCurrentIndex(index < 0 ? 0 : index);
    index = ui->comboBoxPaddingStandard->findData(padding);
    ui->comboBoxPaddingStandard->setCurrentIndex(index < 0 ? 0 : index);
    index = ui->comboBoxLevel->findData(level);
    ui->comboBoxLevel->setCurrentIndex(index < 0 ? 0 : index);
    index = ui->comboBoxMode->findData(mode);
    ui->comboBoxMode->setCurrentIndex(index < 0 ? 0 : index);
}
