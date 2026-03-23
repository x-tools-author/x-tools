/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "hexeditor.h"
#include "ui_hexeditor.h"

#include <QFile>
#include <QFileDialog>

#include "utilities/iconengine.h"

HexEditor::HexEditor(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HexEditor)
{
    ui->setupUi(this);
    ui->toolButtonSave->setIcon(xIcon(":/res/icons/save.svg"));
    ui->toolButtonSaveAs->setIcon(xIcon(":/res/icons/save_as.svg"));
    ui->toolButtonOpen->setIcon(xIcon(":/res/icons/file_open.svg"));
    ui->toolButtonNew->setIcon(xIcon(":/res/icons/add.svg"));
    ui->toolButtonSave->hide();
    setMinimumSize(680, 300);

    connect(ui->toolButtonOpen, &QToolButton::clicked, this, &HexEditor::onOpenButtonClicked);
    connect(ui->toolButtonSave, &QToolButton::clicked, this, &HexEditor::onSaveButtonClicked);
    connect(ui->toolButtonSaveAs, &QToolButton::clicked, this, &HexEditor::onSaveAsButtonClicked);
    connect(ui->toolButtonNew, &QToolButton::clicked, this, &HexEditor::onNewButtonClicked);
}

HexEditor::~HexEditor()
{
    delete ui;
}

void HexEditor::onOpenButtonClicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    QString fileName = dialog.getOpenFileName(this,
                                              tr("Open File"),
                                              QString(),
                                              tr("All Files (*.*)"));
    if (fileName.isEmpty()) {
        return;
    }

    openFile(fileName);
}

void HexEditor::onSaveButtonClicked()
{
    // Nothing to do yet...
}

void HexEditor::onSaveAsButtonClicked()
{
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    QString fileName = dialog.getSaveFileName(this,
                                              tr("Save File As"),
                                              QString(),
                                              tr("All Files (*.*)"));
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open file for writing:" << fileName
                   << ", error:" << file.errorString();
        return;
    }

    QByteArray data = ui->widget->data();
    qint64 bytesWritten = file.write(data);
    if (bytesWritten != data.size()) {
        qWarning() << "Failed to write all data to file:" << fileName
                   << ", error:" << file.errorString();
    }
    file.close();
    openFile(fileName); // Reopen the file to update the buffer and UI
}

void HexEditor::onNewButtonClicked()
{
    if (m_buffer) {
        m_buffer->close();
        delete m_buffer;
        m_buffer = nullptr;
    }

    int size = ui->spinBoxBytes->value();
    QByteArray data(size, '0');
    m_buffer = new QBuffer(this);
    m_buffer->setData(data);
    ui->widget->setData(*m_buffer);
    ui->widget->setReadOnly(false);
}

void HexEditor::openFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << fileName << ", error:" << file.errorString();
        return;
    }

    QBuffer *oldBuffer = m_buffer;
    QByteArray data = file.readAll();
    file.close();
    m_buffer = new QBuffer(this);
    m_buffer->setData(data);
    ui->widget->setData(*m_buffer);
    ui->widget->setReadOnly(false);

    if (oldBuffer) {
        oldBuffer->close();
        delete oldBuffer;
    }
}