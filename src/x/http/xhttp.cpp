/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xhttp.h"
#include "ui_xhttp.h"

#include <QJsonObject>

namespace xHttp {

xHttp::xHttp(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::xHttp)
{
    ui->setupUi(this);
}

xHttp::~xHttp()
{
    delete ui;
}

QJsonObject xHttp::save()
{
    QJsonObject obj;
    return obj;
}

void xHttp::load(const QJsonObject& obj)
{
    Q_UNUSED(obj);
}

} // namespace xHttp