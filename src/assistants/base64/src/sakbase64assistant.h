/********************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 ******************************************************************************/
#ifndef SAKBASE64ASSISTANT_H
#define SAKBASE64ASSISTANT_H

#include <QWidget>

namespace Ui {
class SAKBase64Assisatnt;
}

class SAKBase64Assisatnt : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKBase64Assisatnt(QWidget* parent = Q_NULLPTR);
    ~SAKBase64Assisatnt();

private:
    Ui::SAKBase64Assisatnt* ui_;

private:
    void OnImageClicked();
    void OnEncryptClicked();
    void OnDecryptClicked();
};

#endif // SAKBASE64ASSISTANT_H
