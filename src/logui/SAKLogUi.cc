#include <QStandardItemModel>

#include "SAKLogUi.hh"
#include "ui_SAKLogUi.h"

SAKLogUi::SAKLogUi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SAKLogUi)
{
    ui->setupUi(this);
    QAbstractTableModel *tableModel = SAKLog::instance()->tableModel();
    QTableView *tableView = ui->tableView;
    QHeaderView *headerView = tableView->horizontalHeader();

    int columnCount = tableModel->columnCount();
    QStringList headers;
    for (int i = 0; i < columnCount; i++) {
        auto orientation = Qt::Orientation::Horizontal;
        QString str = tableModel->headerData(i, orientation).toString();
        headers.append(str);
    }
    QStandardItemModel *headerViewModel = new QStandardItemModel(headerView);

    tableView->setHorizontalHeader(headerView);
    tableView->setModel(tableModel);
    tableView->setAlternatingRowColors(true);

    headerViewModel->setColumnCount(headers.count());
    headerViewModel->setHorizontalHeaderLabels(headers);
    headerView->setModel(headerViewModel);
    headerView->setDefaultAlignment(Qt::AlignLeft);
    headerView->setSectionResizeMode(2, QHeaderView::Stretch);
}

SAKLogUi::~SAKLogUi()
{
    delete ui;
}
