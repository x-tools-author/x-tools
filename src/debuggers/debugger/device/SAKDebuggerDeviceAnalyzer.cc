/****************************************************************************************
 * Copyright 2020-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QDebug>
#include <QRegularExpressionValidator>

#include "SAKCommonInterface.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKDebuggerDeviceAnalyzer.hh"

#include "ui_SAKDebuggerDeviceAnalyzer.h"

SAKDebuggerDeviceAnalyzer::SAKDebuggerDeviceAnalyzer(QSettings *settings,
                                                     const QString &settingsGroup,
                                                     QWidget *parent)
    :QDialog(parent)
    ,mUi(new Ui::SAKDebuggerDeviceAnalyzer)
{
    mUi->setupUi(this);

    QString analyzerSettingsGroup = settingsGroup + "/analyzer/";
    mSettingsKeyContext.enable = analyzerSettingsGroup + "enable";
    mSettingsKeyContext.fixedLength = analyzerSettingsGroup + "fixedLength";
    mSettingsKeyContext.frameLength = analyzerSettingsGroup + "length";
    mSettingsKeyContext.startFlags = analyzerSettingsGroup + "startFlags";
    mSettingsKeyContext.endFlags = analyzerSettingsGroup + "endFlags";

    SAKCommonDataStructure::setLineEditTextFormat(mUi->endLineEdit,
                                                SAKCommonDataStructure::InputFormatHex);
    SAKCommonDataStructure::setLineEditTextFormat(mUi->startLineEdit,
                                                SAKCommonDataStructure::InputFormatHex);


    if (settings) {
#if 0
        bool enable = settings->value(mSettingsKeyContext.enable).toBool();
        mUi->disableCheckBox->setChecked(!enable);
#endif
        bool fixedLenght = settings->value(mSettingsKeyContext.fixedLength).toBool();
        mUi->fixedLengthCheckBox->setChecked(fixedLenght);

        int frameLength = settings->value(mSettingsKeyContext.frameLength).toInt();
        mUi->frameLengthSpinBox->setValue(frameLength);

        QString startBytes = settings->value(mSettingsKeyContext.startFlags).toString();
        mUi->startLineEdit->setText(startBytes);

        QString endBytes = settings->value(mSettingsKeyContext.endFlags).toString();
        mUi->endLineEdit->setText(endBytes);


        connect(mUi->disableCheckBox, &QCheckBox::clicked,
                this,
                [=](){
            bool checked = mUi->disableCheckBox->isChecked();
            settings->setValue(mSettingsKeyContext.enable, !checked);
            emit parametersChanged();
        });

        connect(mUi->fixedLengthCheckBox, &QCheckBox::clicked,
                this,
                [=](){
            bool checked = mUi->fixedLengthCheckBox->isChecked();
            settings->setValue(mSettingsKeyContext.fixedLength, checked);
            emit parametersChanged();
        });

        connect(mUi->frameLengthSpinBox,
                static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                this,
                [=](int frameLength){
            settings->setValue(mSettingsKeyContext.frameLength, frameLength);
            emit parametersChanged();
        });

        connect(mUi->startLineEdit, &QLineEdit::textChanged,
                this,
                [=](const QString &text){
            settings->setValue(mSettingsKeyContext.startFlags, text);
            emit parametersChanged();
        });

        connect(mUi->endLineEdit, &QLineEdit::textChanged,
                this,
                [=](const QString &text){
            settings->setValue(mSettingsKeyContext.endFlags, text);
            emit parametersChanged();
        });
    }


    connect(mUi->clearPushButton, &QPushButton::clicked,
            this, &SAKDebuggerDeviceAnalyzer::clearTemp);
    setModal(true);
}

SAKDebuggerDeviceAnalyzer::~SAKDebuggerDeviceAnalyzer()
{
    delete mUi;
}

SAKDebuggerDeviceAnalyzer::SAKStructAnalyzerContext
SAKDebuggerDeviceAnalyzer::parametersContext()
{
    SAKStructAnalyzerContext ctx;
    ctx.enable = !mUi->disableCheckBox->isChecked();
    QString endBytesString = mUi->endLineEdit->text();
    QByteArray endBytes = SAKCommonDataStructure::stringToByteArray(
                endBytesString,
                SAKCommonDataStructure::InputFormatHex
                );
    ctx.endFlags = endBytes;
    QString startBytesString = mUi->startLineEdit->text();
    QByteArray startBytes = SAKCommonDataStructure::stringToByteArray(
                startBytesString,
                SAKCommonDataStructure::InputFormatHex
                );
    ctx.startFlags = startBytes;
    ctx.fixedLength = mUi->fixedLengthCheckBox->isChecked();
    ctx.length = mUi->frameLengthSpinBox->value();

    return ctx;
}
