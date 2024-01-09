/***************************************************************************************************
 * Copyright 2020-2024 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKNUMBERASSISTANT_H
#define SAKNUMBERASSISTANT_H

#include <QWidget>

namespace Ui {
class SAKNumberAssistant;
}

class SAKInterface;
class SAKNumberAssistant : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKNumberAssistant(QWidget* parent = Q_NULLPTR);
    ~SAKNumberAssistant();

private:
    Ui::SAKNumberAssistant* ui;
    SAKInterface* m_interface;

private:
    void fixedLength(QStringList& stringList);

private slots:
    void onHexRawDataCheckBoxClicked();
    void onCreatePushButtonClicked();
    void onRawDataLineEditTextChanged(const QString& text);
    void onBigEndianCheckBoxClicked();
    void onFloatRadioButtonClicked();
    void onDoubleRadioButtonClicked();
};

#endif // SAKNUMBERASSISTANT_H
