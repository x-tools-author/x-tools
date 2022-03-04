/*
 * Copyright 2022 Qsaker(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKUDPCLIENTADVANCEDMULTICAST_HH
#define SAKUDPCLIENTADVANCEDMULTICAST_HH

#include <QDialog>

namespace Ui {
    class SAKUdpClientAdvancedMulticast;
};

class SAKUdpClientAdvancedMulticast : public QDialog
{
    Q_OBJECT
public:
    SAKUdpClientAdvancedMulticast(QWidget *parent = Q_NULLPTR);
    ~SAKUdpClientAdvancedMulticast();

    QString address();
    quint16 port();
private:
    Ui::SAKUdpClientAdvancedMulticast *ui;
};

#endif // SAKUDPCLIENTADVANCEDMULTICAST_HH
