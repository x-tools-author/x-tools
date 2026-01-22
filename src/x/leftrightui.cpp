/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "leftrightui.h"
#include "ui_leftrightui.h"

LeftRightUi::LeftRightUi(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LeftRightUi)
{
    ui->setupUi(this);
}

LeftRightUi::~LeftRightUi()
{
    delete ui;
}

QJsonObject LeftRightUi::save()
{
    QJsonObject obj;
    // Currently, no specific state to save for left/right widgets.
    return obj;
}

void LeftRightUi::load(const QJsonObject &obj)
{
    Q_UNUSED(obj);
    // Currently, no specific state to load for left/right widgets.
}

void LeftRightUi::setupUi(const QString &leftTitle,
                          QWidget *left,
                          const QString &rightTitle,
                          QWidget *right)
{
    ui->pushButtonLeft->setText(leftTitle);
    ui->pushButtonRight->setText(rightTitle);

    if (left && !m_left) {
        m_left = left;
        m_leftLayout = new QVBoxLayout();
        m_leftLayout->setContentsMargins(0, 0, 0, 0);
        m_leftLayout->setSpacing(0);
        ui->widgetLeft->setLayout(m_leftLayout);
        m_leftLayout->addWidget(m_left);
    }

    if (right && !m_right) {
        m_right = right;
        m_rightLayout = new QVBoxLayout();
        m_rightLayout->setContentsMargins(0, 0, 0, 0);
        m_rightLayout->setSpacing(0);
        ui->widgetRight->setLayout(m_rightLayout);
        m_rightLayout->addWidget(m_right);
    }
}