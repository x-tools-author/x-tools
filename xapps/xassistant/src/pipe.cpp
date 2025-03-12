/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "pipe.h"

#include "device/device.h"
#include "page/page.h"

Pipeline::Pipeline(Page *leftPage, Page *rightPage, QObject *parent)
    : QObject(parent)
    , m_leftPage(leftPage)
    , m_rightPage(rightPage)
{
    // QToolButton *leftPageLeftButton = m_leftPage->outputPanel()->leftButton();
    // m_left2rightButton = m_leftPage->outputPanel()->rightButton();
    // m_right2leftButton = m_rightPage->outputPanel()->leftButton();
    // QToolButton *rightPageRightButton = m_rightPage->outputPanel()->rightButton();
    // leftPageLeftButton->setVisible(false);
    // rightPageRightButton->setVisible(false);
    // m_left2rightButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    // m_right2leftButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    // m_left2rightButton->setCheckable(true);
    // m_right2leftButton->setCheckable(true);
    // m_left2rightButton->setIcon(QIcon(":/Resource/Icons/RightGray.svg"));
    // m_right2leftButton->setIcon(QIcon(":/Resource/Icons/LeftGray.svg"));
    // m_left2rightButton->setToolTip(tr("Transmit the data received by the right page"));
    // m_right2leftButton->setToolTip(tr("Transmit the data received by the left page"));
    // connect(m_left2rightButton, &QToolButton::clicked, this, &Pipeline::onLeft2RightButtonClicked);
    // connect(m_right2leftButton, &QToolButton::clicked, this, &Pipeline::onRight2LeftButtonClicked);

    // connect(m_leftPage, &Page::bytesRead, this, &Pipeline::onLeftPageBytesRead);
    // connect(m_rightPage, &Page::bytesRead, this, &Pipeline::onRightPageBytesRead);
}

Pipeline::~Pipeline() {}

void Pipeline::onLeft2RightButtonClicked()
{
    if (m_left2rightButton->isChecked()) {
        m_left2rightButton->setIcon(QIcon(":/Resource/Icons/RightGreen.svg"));
    } else {
        m_left2rightButton->setIcon(QIcon(":/Resource/Icons/RightGray.svg"));
    }
}

void Pipeline::onRight2LeftButtonClicked()
{
    if (m_right2leftButton->isChecked()) {
        m_right2leftButton->setIcon(QIcon(":/Resource/Icons/LeftGreen.svg"));
    } else {
        m_right2leftButton->setIcon(QIcon(":/Resource/Icons/LeftGray.svg"));
    }
}

void Pipeline::onLeftPageBytesRead(const QByteArray &bytes)
{
    // if (m_left2rightButton->isChecked()) {
    //     Device *device = m_leftPage->device();
    //     if (device) {
    //         device->writeBytes(bytes);
    //     }
    // }
}

void Pipeline::onRightPageBytesRead(const QByteArray &bytes)
{
    // if (m_right2leftButton->isChecked()) {
    //     Device *device = m_rightPage->device();
    //     if (device) {
    //         device->writeBytes(bytes);
    //     }
    // }
}
