/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "dataview.h"
#include "dataviewui.h"

DataView::DataView(QObject *parent)
    : BaseNode(parent)
    , m_embeddedWidget(nullptr)
{}

DataView::~DataView() {}

QString DataView::caption() const
{
    return tr("Data View");
}

QString DataView::name() const
{
    return QString("DataView");
}

unsigned int DataView::nPorts(QtNodes::PortType portType) const
{
    if (portType == QtNodes::PortType::In) {
        return 1;
    } else if (portType == QtNodes::PortType::Out) {
        return 0;
    } else {
        return 0;
    }
}

bool DataView::resizable() const
{
    return true;
}

QWidget *DataView::embeddedWidget()
{
    if (!m_embeddedWidget) {
        m_embeddedWidget = new DataViewUi(this);
    }

    return m_embeddedWidget;
}
