/***************************************************************************************************
 * Copyright 2025-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "leftrightui.h"
#include "ui_leftrightui.h"

#include <QTabBar>

namespace Ui {
class LeftRightUi;
}

class LeftRightUiPrivate : public QObject
{
public:
    LeftRightUiPrivate(LeftRightUi *q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {
        ui = new Ui::LeftRightUi;
        ui->setupUi(q);

        m_leftTabBar = new QTabBar(q);
        m_rightTabBar = new QTabBar(q);
        ui->horizontalLayout->insertWidget(1, m_leftTabBar);
        ui->horizontalLayout->insertWidget(3, m_rightTabBar);
    }
    ~LeftRightUiPrivate() override { delete ui; }

public:
    Ui::LeftRightUi *ui{nullptr};
    QWidget *m_left{nullptr};
    QWidget *m_right{nullptr};
    QVBoxLayout *m_leftLayout{nullptr};
    QVBoxLayout *m_rightLayout{nullptr};
    QTabBar *m_leftTabBar{nullptr};
    QTabBar *m_rightTabBar{nullptr};

private:
    LeftRightUi *q{nullptr};
};

LeftRightUi::LeftRightUi(QWidget *parent)
    : QWidget(parent)
{
    d = new LeftRightUiPrivate(this);
}

LeftRightUi::~LeftRightUi() {}

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
    d->ui->pushButtonLeft->setText(leftTitle);
    d->ui->pushButtonRight->setText(rightTitle);

    if (left && !d->m_left) {
        d->m_left = left;
        d->m_leftLayout = new QVBoxLayout();
        d->m_leftLayout->setContentsMargins(0, 0, 0, 0);
        d->m_leftLayout->setSpacing(0);
        d->ui->widgetLeft->setLayout(d->m_leftLayout);
        d->m_leftLayout->addWidget(d->m_left);
    }

    if (right && !d->m_right) {
        d->m_right = right;
        d->m_rightLayout = new QVBoxLayout();
        d->m_rightLayout->setContentsMargins(0, 0, 0, 0);
        d->m_rightLayout->setSpacing(0);
        d->ui->widgetRight->setLayout(d->m_rightLayout);
        d->m_rightLayout->addWidget(d->m_right);
    }
}

void LeftRightUi::addLeftTab(const QString &title)
{
    if (d->m_leftTabBar) {
        d->m_leftTabBar->addTab(title);
    }
}

void LeftRightUi::addRightTab(const QString &title)
{
    if (d->m_rightTabBar) {
        d->m_rightTabBar->addTab(title);
    }
}