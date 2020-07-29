/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTRANSMISSIONSETTINGS_HH
#define SAKTRANSMISSIONSETTINGS_HH

#include <QWidget>
#include <QPushButton>

class SAKDebugPage;
class SAKOtherTransmissionPage;

namespace Ui {
class SAKOtherTransmissionPageViewer;
}

class SAKOtherTransmissionPageViewer:public QWidget
{
    Q_OBJECT
public:
    SAKOtherTransmissionPageViewer(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKOtherTransmissionPageViewer();
private:
    QWidget                     *serialPortWidget       = Q_NULLPTR;
    QWidget                     *udpWidget              = Q_NULLPTR;
    QWidget                     *tcpWidget              = Q_NULLPTR;
    SAKDebugPage                *_debugPage             = Q_NULLPTR;
    Ui::SAKOtherTransmissionPageViewer    *ui                     = Q_NULLPTR;

    SAKOtherTransmissionPage            *udpTransmission        = Q_NULLPTR;
    SAKOtherTransmissionPage            *tcpTransmission        = Q_NULLPTR;
    SAKOtherTransmissionPage            *serialPortTransmission = Q_NULLPTR;
};

#endif
