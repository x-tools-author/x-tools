/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QList>
#include <QDebug>
#include <QMetaEnum>
#include <QLineEdit>

#include "SAKCommonDataStructure.hh"
#include "SAKTestDebuggerController.hh"
#include "ui_SAKTestDebuggerController.h"

SAKTestDebuggerController::SAKTestDebuggerController(QSettings *settings,
                                                     const QString &settingsGroup,
                                                     QWidget *parent)
    :SAKDebuggerController(settings, settingsGroup, parent)
    ,mUi(new Ui::SAKTestDebuggerController)
{
    mUi->setupUi(this);

    connect(mUi->openFailedCheckBox, &QCheckBox::clicked,
            this, &SAKTestDebuggerController::openFailedChanged);

    connect(mUi->errorStringLineEdit, &QLineEdit::textChanged,
            this, &SAKTestDebuggerController::errorStringChanged);

    connect(mUi->readCyclicCheckBox, &QCheckBox::clicked,
            this, &SAKTestDebuggerController::readCircularlyChanged);

    connect(mUi->readIntervalLineEdit, &QLineEdit::textChanged,
            this, [&](const QString &text){
        emit readIntervalChanged(text.toInt());
    });

    connect(mUi->writeCyclicCheckBox, &QCheckBox::clicked,
            this, &SAKTestDebuggerController::writeCircularlyChanged);

    connect(mUi->writtenIntervalLineEdit, &QLineEdit::textChanged,
            this, [&](const QString &text){
        emit writtingIntervalChanged(text.toInt());
    });
}

SAKTestDebuggerController::~SAKTestDebuggerController()
{
    delete mUi;
}

void SAKTestDebuggerController::updateUiState(bool opened)
{
    mUi->openFailedCheckBox->setEnabled(!opened);
    mUi->errorStringLineEdit->setEnabled(!opened);
#if 0
    mUi->readCyclicCheckBox->setEnabled(!opened);
    mUi->readIntervalLineEdit->setEnabled(!opened);
    mUi->writeCyclicCheckBox->setEnabled(!opened);
    mUi->writtenIntervalLineEdit->setEnabled(!opened);
#endif
}

void SAKTestDebuggerController::refreshDevice()
{
    qDebug() << __FUNCTION__ << __LINE__;
}

QVariant SAKTestDebuggerController::parametersContext()
{
    SAKCommonDataStructure::SAKStructTestParametersContext ctx;
    ctx.openFailed = mUi->openFailedCheckBox->isChecked();
    ctx.errorString = mUi->errorStringLineEdit->text();
    ctx.readCircularly = mUi->readCyclicCheckBox->isChecked();
    ctx.readInterval = mUi->readIntervalLineEdit->text().toInt();
    ctx.writeCircularly = mUi->writeCyclicCheckBox->isChecked();
    ctx.writtingInterval = mUi->writtenIntervalLineEdit->text().toInt();
    return QVariant::fromValue(ctx);
}
