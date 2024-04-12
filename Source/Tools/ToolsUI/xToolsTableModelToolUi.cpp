/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsTableModelToolUi.h"
#include "ui_xToolsTableModelToolUi.h"

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMessageBox>
#include <QStandardItemModel>

#include "xToolsApplication.h"
#include "xToolsMenu.h"
#include "xToolsSettings.h"
#include "xToolsTableModelTool.h"

xToolsTableModelToolUi::xToolsTableModelToolUi(QWidget *parent)
    : xToolsBaseToolUi{parent}
    , ui(new Ui::xToolsTableModelToolUi)
{
    ui->setupUi(this);
    QHeaderView *vHeaderView = ui->tableView->verticalHeader();
    vHeaderView->hide();

    connect(ui->pushButtonEdit,
            &QPushButton::clicked,
            this,
            &xToolsTableModelToolUi::onPushButtonEditClicked);
    connect(ui->pushButtonClear,
            &QPushButton::clicked,
            this,
            &xToolsTableModelToolUi::onPushButtonClearClicked);
    connect(ui->pushButtonDelete,
            &QPushButton::clicked,
            this,
            &xToolsTableModelToolUi::onPushButtonDeleteClicked);
    connect(ui->pushButtonImport,
            &QPushButton::clicked,
            this,
            &xToolsTableModelToolUi::onPushButtonImportClicked);
    connect(ui->pushButtonExport,
            &QPushButton::clicked,
            this,
            &xToolsTableModelToolUi::onPushButtonExportClicked);
    connect(ui->pushButtonAppend,
            &QPushButton::clicked,
            this,
            &xToolsTableModelToolUi::onPushButtonAppendClicked);

    connect(ui->tableView, &QTableView::doubleClicked, this, [=](const QModelIndex &index) {
        Q_UNUSED(index)
        onPushButtonEditClicked();
    });
}

xToolsTableModelToolUi::~xToolsTableModelToolUi()
{
    delete ui;
}

void xToolsTableModelToolUi::setStretchSections(QList<int> columns)
{
    QTableView *tableView = ui->tableView;
    QHeaderView *headerView = tableView->horizontalHeader();
    for (int &column : columns) {
        headerView->setSectionResizeMode(column, QHeaderView::Stretch);
    }
}

void xToolsTableModelToolUi::setSectionResizeModeToStretch()
{
    QTableView *tableView = ui->tableView;
    QHeaderView *headerView = tableView->horizontalHeader();
    headerView->setSectionResizeMode(QHeaderView::Stretch);
}

void xToolsTableModelToolUi::setColumnVisible(int column, bool visible)
{
    QTableView *tableView = ui->tableView;
    if (visible) {
        tableView->showColumn(column);
    } else {
        tableView->hideColumn(column);
    }
}

void xToolsTableModelToolUi::onBaseToolUiInitialized(xToolsBaseTool *tool,
                                                     const QString &settingGroup)
{
    if (!tool) {
        qWarning() << "The value of tool is nullptr!";
        return;
    }

    if (!tool->inherits("xToolsTableModelTool")) {
        qWarning() << "The tool does not inherits xToolsTableModelTool!";
        return;
    }

    m_TableModelTool = dynamic_cast<xToolsTableModelTool *>(tool);
    Q_ASSERT_X(m_TableModelTool, Q_FUNC_INFO, "The tool is not xToolsTableModelTool!");

    mTableModel = m_TableModelTool->tableModel().value<QAbstractTableModel *>();
    QTableView *tableView = ui->tableView;
    QHeaderView *headerView = tableView->horizontalHeader();
    int columnCount = mTableModel->columnCount();
    QStringList headers;
    QStringList rawHeaders;
    for (int i = 0; i < columnCount; i++) {
        auto orientation = Qt::Orientation::Horizontal;
        QString str = mTableModel->headerData(i, orientation).toString();
        rawHeaders.append(str);
        str = m_TableModelTool->cookHeaderString(str);
        headers.append(str);
    }

    QStandardItemModel *headerViewModel = new QStandardItemModel(headerView);

    tableView->setHorizontalHeader(headerView);
    tableView->setModel(mTableModel);

    headerViewModel->setColumnCount(headers.count());
    headerViewModel->setHorizontalHeaderLabels(headers);
    headerView->setModel(headerViewModel);

    m_menu = new xToolsMenu(ui->pushButtonVisible);
    ui->pushButtonVisible->setMenu(m_menu);
    auto settings = xToolsSettings::instance();
    auto hideColumns = defaultHideColumns();
    for (int i = 0; i < headers.count(); i++) {
        QAction *ret = m_menu->addAction(headers.at(i));
        connect(ret, &QAction::triggered, this, [=]() {
            if (ret->isChecked()) {
                tableView->showColumn(i);
            } else {
                tableView->hideColumn(i);
            }
            settings->setValue(settingGroup + "/" + rawHeaders.at(i), ret->isChecked());
        });
        ret->setCheckable(true);

        QVariant var = settings->value(settingGroup + "/" + rawHeaders.at(i));
        if (var.isValid()) {
            if (!var.toBool()) {
                tableView->hideColumn(i);
            }
            ret->setChecked(var.toBool());
        } else {
            if (hideColumns.contains(i)) {
                tableView->hideColumn(i);
                ret->setChecked(false);
            } else {
                ret->setChecked(true);
            }
        }
    }

    mItemsKey = settingGroup + "/items";
    QString items = settings->value(mItemsKey).toString();
    QByteArray json = QByteArray::fromHex(items.toLatin1());
    importFromJson(QJsonDocument::fromJson(json).toJson());
}

QList<int> xToolsTableModelToolUi::defaultHideColumns()
{
    QList<int> list;
    return list;
}

void xToolsTableModelToolUi::afterRowEdited(int row)
{
    Q_UNUSED(row)
}

void xToolsTableModelToolUi::clear()
{
    int rowCount = mTableModel->rowCount();
    mTableModel->removeRows(0, rowCount);
}

void xToolsTableModelToolUi::remove(const QModelIndex &index)
{
    if (index.isValid()) {
        mTableModel->removeRow(index.row());
    }
}

void xToolsTableModelToolUi::importFromJson(const QByteArray &json)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(json);
    QJsonArray jsonArray = jsonDoc.array();
    for (int i = 0; i < jsonArray.count(); i++) {
        QJsonObject jsonObj = jsonArray.at(i).toObject();
        QJsonDocument jd;
        jd.setObject(jsonObj);
        QString item = QString::fromUtf8(jd.toJson());
        m_TableModelTool->addItem(item);
    }
}

QByteArray xToolsTableModelToolUi::exportAsJson()
{
    auto items = m_TableModelTool->itemsContext();
    QJsonArray jsonArray = items.toJsonArray();
    QJsonDocument jsonDoc;
    jsonDoc.setArray(jsonArray);
    QByteArray json = jsonDoc.toJson();
    return json;
}

void xToolsTableModelToolUi::edit(const QModelIndex &index)
{
    QVariant var = m_TableModelTool->itemContext(index.row());
    QJsonObject jsonObj = var.toJsonObject();
    QDialog *editor = itemEditor();
    QGenericReturnArgument ret;
    QMetaObject::invokeMethod(editor,
                              "setParameters",
                              Qt::DirectConnection,
                              ret,
                              Q_ARG(QJsonObject, jsonObj));
    qInfo() << "the parameter of setParameters() is:" << jsonObj;
    Q_UNUSED(ret);
    editor->show();

    if (QDialog::Accepted == editor->exec()) {
        QJsonObject params;
        QMetaObject::invokeMethod(editor,
                                  "parameters",
                                  Qt::DirectConnection,
                                  Q_RETURN_ARG(QJsonObject, params));
        qInfo() << "the parameter of parameters() is:" << params;
        QJsonDocument jsonDoc;
        jsonDoc.setObject(params);
        QString str = QString::fromUtf8(jsonDoc.toJson());
        m_TableModelTool->addItem(str, index.row());

        afterRowEdited(index.row());
    }
}

bool xToolsTableModelToolUi::append()
{
    QJsonObject jsonObj = m_TableModelTool->itemContext(-1).toJsonObject();
    QDialog *editor = itemEditor();
    QGenericReturnArgument ret;
    QMetaObject::invokeMethod(editor,
                              "setParameters",
                              Qt::DirectConnection,
                              ret,
                              Q_ARG(QJsonObject, jsonObj));
    qInfo() << "the parameter of setParameters() is:" << jsonObj;
    Q_UNUSED(ret);

    editor->show();
    if (!(QDialog::Accepted == editor->exec())) {
        return false;
    }

    QMetaObject::invokeMethod(editor,
                              "parameters",
                              Qt::DirectConnection,
                              Q_RETURN_ARG(QJsonObject, jsonObj));
    qInfo() << "the parameter of parameters() is:" << jsonObj;
    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonObj);
    QString str = QString::fromUtf8(jsonDoc.toJson());
    m_TableModelTool->addItem(str, -1);
    return true;
}

QModelIndex xToolsTableModelToolUi::currentIndex()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(xToolsApplication::mainWindow(),
                             tr("Please Select an Item"),
                             tr("Please select an item first, then try again!"));
    }
    return index;
}

void xToolsTableModelToolUi::writeToSettingsFile()
{
    QByteArray json = exportAsJson();
    xToolsSettings::instance()->setValue(mItemsKey, QString::fromLatin1(json.toHex()));
}

bool xToolsTableModelToolUi::isInitialized()
{
    if (!m_TableModelTool) {
        QMessageBox::warning(xToolsApplication::mainWindow(),
                             tr("Invalid Parameter"),
                             tr("The value of mTableModelTool is nullptr,"
                                " you must called initialize() first!"));
        return false;
    }

    return true;
}

void xToolsTableModelToolUi::onPushButtonEditClicked()
{
    if (!isInitialized()) {
        return;
    }

    QModelIndex index = currentIndex();
    if (index.isValid()) {
        edit(index);
        writeToSettingsFile();
    }
}

void xToolsTableModelToolUi::onPushButtonClearClicked()
{
    if (!isInitialized()) {
        return;
    }

    int ret = QMessageBox::warning(xToolsApplication::mainWindow(),
                                   tr("Clear Data"),
                                   tr("The data will be empty from settings file, are you sure?"),
                                   QMessageBox::No | QMessageBox::Ok);
    if (ret == QMessageBox::Ok) {
        clear();
        writeToSettingsFile();
    }
}

void xToolsTableModelToolUi::onPushButtonDeleteClicked()
{
    if (!isInitialized()) {
        return;
    }

    int ret = QMessageBox::warning(xToolsApplication::mainWindow(),
                                   tr("Delete Data"),
                                   tr("The data will be delete from settings file, are you sure?"),
                                   QMessageBox::Cancel | QMessageBox::Ok);

    if (ret != QMessageBox::Ok) {
        return;
    }

    QModelIndex index = currentIndex();
    if (index.isValid()) {
        remove(index);
        writeToSettingsFile();
    }
}

void xToolsTableModelToolUi::onPushButtonImportClicked()
{
    if (!isInitialized()) {
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(xToolsApplication::mainWindow(),
                                                    tr("Import data"),
                                                    ".",
                                                    tr("JSON (*.json);;All (*)"));
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (file.open(QFile::ReadOnly)) {
        QByteArray json = file.readAll();
        file.close();
        ;

        importFromJson(json);
        writeToSettingsFile();
    } else {
        qWarning() << "Can not open file:" << file.errorString();
    }
}

void xToolsTableModelToolUi::onPushButtonExportClicked()
{
    if (!isInitialized()) {
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(xToolsApplication::mainWindow(),
                                                    tr("Import data"),
                                                    ".",
                                                    tr("JSON (*.json);;All (*)"));
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (file.open(QFile::WriteOnly)) {
        QTextStream out(&file);
        out << exportAsJson();
        file.close();
    } else {
        qWarning() << "Can not open file:" << file.errorString();
    }
}

void xToolsTableModelToolUi::onPushButtonAppendClicked()
{
    if (!isInitialized()) {
        return;
    }

    if (append()) {
        writeToSettingsFile();
    }
}
