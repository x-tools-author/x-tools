/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKDEBUGPAGECOMMONSSLCONFIGURATIONWIDGET_HH
#define SAKDEBUGPAGECOMMONSSLCONFIGURATIONWIDGET_HH

#include <QWidget>

namespace Ui {
    class SAKDebugPageCommonSslConfigurationWidget;
}

class SAKDebugPageCommonSslConfigurationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SAKDebugPageCommonSslConfigurationWidget(QWidget *parent = Q_NULLPTR);
    ~SAKDebugPageCommonSslConfigurationWidget();
private:
    Ui::SAKDebugPageCommonSslConfigurationWidget *mUi;
};

#endif // SAKSSLSOCKETCOMMONCONFIGURATION_HH
