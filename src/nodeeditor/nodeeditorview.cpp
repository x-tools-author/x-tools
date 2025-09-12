/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "nodeeditorview.h"

#include "nodeeditormodel.h"
#include "nodeeditorscene.h"

namespace xFlow {

NodeEditorView::NodeEditorView(QWidget *parent)
    : QtNodes::GraphicsView(parent)
{
    m_model = new NodeEditorModel();
    m_scene = new NodeEditorScene(*m_model, this);
    setScene(m_scene);
}

NodeEditorView::~NodeEditorView()
{
    delete m_model;
}

} // namespace xFlow