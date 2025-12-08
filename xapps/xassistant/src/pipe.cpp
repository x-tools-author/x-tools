/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "pipe.h"

#include <QSettings>

#include "application.h"
#include "page/page.h"

Pipe::Pipe(Page *leftPage, Page *rightPage, QObject *parent)
    : QObject(parent)
    , m_leftPage(leftPage)
    , m_rightPage(rightPage)
{
    m_leftButton = new QToolButton();
    m_rightButton = new QToolButton();
    m_leftButton->setCheckable(true);
    m_rightButton->setCheckable(true);

    Application *application = qobject_cast<Application *>(qApp);
    m_leftButton->setToolTip(application->left2rightTips());
    m_rightButton->setToolTip(application->right2leftTips());
    leftPage->appendOutputControl(m_leftButton);
    rightPage->prependOutputControl(m_rightButton);

    connect(m_leftButton, &QToolButton::clicked, this, &Pipe::onLeftButtonClicked);
    connect(m_rightButton, &QToolButton::clicked, this, &Pipe::onRightButtonClicked);

    connect(leftPage, &Page::bytesRead, this, &Pipe::onLeftPageBytesRead);
    connect(rightPage, &Page::bytesRead, this, &Pipe::onRightPageBytesRead);

    QSettings *settings = application->settings();
    bool left2right = settings->value(m_keys.left2right, false).toBool();
    bool right2left = settings->value(m_keys.right2left, false).toBool();
    m_leftButton->setChecked(left2right);
    m_rightButton->setChecked(right2left);

    onLeftButtonClicked();
    onRightButtonClicked();
}

Pipe::~Pipe() {}

void Pipe::onLeftButtonClicked()
{
    if (m_leftButton->isChecked()) {
        m_leftButton->setIcon(QIcon(":/res/icons/right_gray.svg"));
    } else {
        m_leftButton->setIcon(QIcon(":/res/icons/right_blue.svg"));
    }

    Application *application = qobject_cast<Application *>(qApp);
    application->settings()->setValue(m_keys.left2right, m_leftButton->isChecked());
}

void Pipe::onRightButtonClicked()
{
    if (m_rightButton->isChecked()) {
        m_rightButton->setIcon(QIcon(":/res/icons/left_gray.svg"));
    } else {
        m_rightButton->setIcon(QIcon(":/res/icons/left_blue.svg"));
    }

    Application *application = qobject_cast<Application *>(qApp);
    application->settings()->setValue(m_keys.right2left, m_rightButton->isChecked());
}

void Pipe::onLeftPageBytesRead(const QByteArray &bytes, const QString &from)
{
    Q_UNUSED(from);
    if (m_leftButton->isChecked()) {
        m_rightPage->inputBytes(bytes);
    }
}

void Pipe::onRightPageBytesRead(const QByteArray &bytes, const QString &from)
{
    Q_UNUSED(from);
    if (m_rightButton->isChecked()) {
        m_leftPage->inputBytes(bytes);
    }
}
