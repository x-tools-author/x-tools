/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKOTHERTRANSMISSIONPAGEVIEWER_HH
#define SAKOTHERTRANSMISSIONPAGEVIEWER_HH

#include <QWidget>
#include <QPushButton>

namespace Ui {
class SAKOtherTransmissionPageViewer;
}

class SAKDebugPage;
class SAKOtherTransmissionPage;
class SAKOtherTransmissionPageViewer:public QWidget
{
    Q_OBJECT
public:
    SAKOtherTransmissionPageViewer(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKOtherTransmissionPageViewer();
private:
    QWidget *mSerialPortWidget;
    QWidget *mUdpWidget;
    QWidget *mTcpWidget;
    SAKDebugPage *mDebugPage;
    Ui::SAKOtherTransmissionPageViewer *mUi;

    SAKOtherTransmissionPage *mUdpTransmission;
    SAKOtherTransmissionPage *mTcpTransmission;
    SAKOtherTransmissionPage *mSerialPortTransmission;
};

#endif
