/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QVBoxLayout>
#include <QWidget>

#include "utilities/serializable.h"

namespace Ui {
class LeftRightUi;
}

class LeftRightUi : public QWidget, public xTools::Serializable
{
    Q_OBJECT
public:
    explicit LeftRightUi(QWidget *parent = nullptr);
    ~LeftRightUi();

    QJsonObject save() override;
    void load(const QJsonObject &obj) override;
    void setupUi(const QString &leftTitle, QWidget *left, const QString &rightTitle, QWidget *right);

private:
    Ui::LeftRightUi *ui{nullptr};
    QWidget *m_left{nullptr};
    QWidget *m_right{nullptr};
    QVBoxLayout *m_leftLayout{nullptr};
    QVBoxLayout *m_rightLayout{nullptr};
};