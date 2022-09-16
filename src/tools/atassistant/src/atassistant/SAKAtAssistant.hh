/******************************************************************************
 * Copyright 2022 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKATASSISTANT_HH
#define SAKATASSISTANT_HH

#include <QWidget>
#include <QComboBox>

namespace Ui {
    class SAKAtAssistant;
}

class SAKAtAssistant : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKAtAssistant(QWidget *parent = Q_NULLPTR);
    ~SAKAtAssistant();
private:
    struct {
        struct {
            QComboBox *portName;
        } device;
        struct {

        } input;
        struct {

        } output;
        struct {

        } command;
    } uiCtx_;
private:
    Ui::SAKAtAssistant *ui_;
};

#endif // SAKATASSISTANT_HH
