/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QClipboard>
#include <QColorDialog>
#include <QComboBox>
#include <QDesktopServices>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPainter>
#include <QPushButton>
#include <QString>
#include <QTimer>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidgetAction>

#include "Common/xTools.h"
#include "SettingsMenu.h"
#include "SvgFileDataModel.h"
#include "SvgItemDelegate.h"
#include "xLicense/Source/LicenseManager.h"

namespace xSvg {

MainWindow::MainWindow(QWidget *parent)
    : xApp::MainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_model{nullptr}
{
    initToolBar();
    initMenuBar();

    m_centralWidget = new QWidget(this);
    ui->setupUi(m_centralWidget);
    ui->tableView->installEventFilter(this);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Fixed);
    ui->tableView->horizontalHeader()->hide();
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setItemDelegate(new SvgItemDelegate(this));

    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    QMenu *menu = new QMenu(ui->tableView);
    menu->addAction(m_actions.openPath);
    menu->addAction(m_actions.openFileLocation);
    menu->addAction(m_actions.saveAs);
    menu->addAction(m_actions.copyFullPath);
    menu->addAction(m_actions.copyFileName);
    connect(ui->tableView, &QWidget::customContextMenuRequested, this, [=](const QPoint &pos) {
        menu->exec(ui->tableView->mapToGlobal(pos));
    });

    setMinimumSize(1280, 680);
    setCentralWidget(m_centralWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showTips()
{
    QTimer::singleShot(500, this, [=]() {
        int ret = QMessageBox::information(this,
                                           tr("Import SVG Files"),
                                           tr("Select a folder to import SVG files now ?"),
                                           QMessageBox::Yes | QMessageBox::No);
        if (ret == QMessageBox::Yes) {
            openPath();
        }
    });
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->tableView && event->type() == QEvent::Resize) {
        updateView();
    }
    return xApp::MainWindow::eventFilter(obj, event);
}

void MainWindow::initMenuBar()
{
    m_exitAction->setIcon(materialIcon(QString("close.svg")));
    m_fileMenu->insertAction(m_exitAction, m_actions.openPath);
    m_fileMenu->insertAction(m_exitAction, m_actions.openFileLocation);
    //m_fileMenu->insertSeparator(m_exitAction);
    m_fileMenu->insertAction(m_exitAction, m_actions.saveAs);
    //m_fileMenu->insertSeparator(m_exitAction);
    m_fileMenu->insertAction(m_exitAction, m_actions.copyFullPath);
    m_fileMenu->insertAction(m_exitAction, m_actions.copyFileName);
}

void MainWindow::initToolBar()
{
    QToolBar *toolBar = new QToolBar(this);
    addToolBar(Qt::TopToolBarArea, toolBar);
    toolBar->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);
    toolBar->setMovable(false);
    toolBar->setFloatable(false);
    toolBar->setOrientation(Qt::Orientation::Horizontal);

    initToolBarActions(toolBar);
    toolBar->addSeparator();
    addSpacer(toolBar, 4);
    initToolBarFilter(toolBar);
    addSpacer(toolBar, 4);
    initToolBarFormat(toolBar);
    addSpacer(toolBar, 4);
    initToolBarSize(toolBar);
    addSpacer(toolBar, 4);
    initToolBarColor(toolBar);
    addSpacer(toolBar, 4);
    toolBar->addSeparator();
    initToolBarSettings(toolBar);
}

void MainWindow::initToolBarActions(QToolBar *toolBar)
{
    m_actions.openPath = toolBar->addAction(materialIcon("folder_open.svg"),
                                            tr("Open Path"),
                                            this,
                                            &MainWindow::openPath);

    m_actions.openFileLocation = toolBar->addAction(materialIcon("share_location.svg"),
                                                    tr("Open File Location"),
                                                    this,
                                                    &MainWindow::openFileLocation);
    toolBar->addSeparator();
    m_actions.saveAs = toolBar->addAction(materialIcon("exit_to_app.svg"),
                                          tr("Save As..."),
                                          this,
                                          &MainWindow::saveAs);
    m_actions.copyFullPath = toolBar->addAction(materialIcon("file_copy.svg"),
                                                tr("Copy Full Path"),
                                                this,
                                                &MainWindow::copyFullPath);
    m_actions.copyFileName = toolBar->addAction(materialIcon("copy_all.svg"),
                                                tr("Copy File Name"),
                                                this,
                                                &MainWindow::copyFileName);
}

void MainWindow::initToolBarFilter(QToolBar *toolBar)
{
    toolBar->addWidget(new QLabel(tr("Filter\nText")));
    addSpacer(toolBar, 4);
    m_searchLineEdit = new QLineEdit(this);
    m_searchLineEdit->setMinimumWidth(200);
    m_searchLineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_searchLineEdit->setClearButtonEnabled(true);
    toolBar->addWidget(m_searchLineEdit);
    addSpacer(toolBar, 4);

    QString text = g_xTools.settings()->value(m_settingKeys.filter).toString();
    m_searchLineEdit->setText(text);

    connect(m_searchLineEdit, &QLineEdit::textEdited, this, [this]() {
        g_xTools.settings()->setValue(m_settingKeys.filter, m_searchLineEdit->text().trimmed());
        this->updateView();
    });
}

void MainWindow::initToolBarFormat(QToolBar *toolBar)
{
    toolBar->addWidget(new QLabel(tr("Export\nFormat")));
    addSpacer(toolBar, 4);
    m_formatComboBox = new QComboBox(this);
    m_formatComboBox->setMinimumWidth(100);
    m_formatComboBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    toolBar->addWidget(m_formatComboBox);
    m_formatComboBox->addItems(QStringList() << "SVG"
                                             << "PNG"
                                             << "XPM"
                                             << "ICNS"
                                             << "ICO"
                                             << "TIFF"
                                             << "WEBP");

    QString format = g_xTools.settings()->value(m_settingKeys.format).toString();
    m_formatComboBox->setCurrentText(format);

    connect(m_formatComboBox, QOverload<int>::of(&QComboBox::activated), this, [this]() {
        g_xTools.settings()->setValue(m_settingKeys.format, m_formatComboBox->currentText());
    });
}

void MainWindow::initToolBarSize(QToolBar *toolBar)
{
    toolBar->addWidget(new QLabel(tr("Icon\nSize")));
    addSpacer(toolBar, 4);
    m_sizeComboBox = new QComboBox(this);
    m_sizeComboBox->setMinimumWidth(100);
    toolBar->addWidget(m_sizeComboBox);
    m_sizeComboBox->addItems(QStringList() << "16"
                                           << "24"
                                           << "32"
                                           << "48"
                                           << "64"
                                           << "128"
                                           << "256"
                                           << "512");

    QString size = g_xTools.settings()->value(m_settingKeys.size).toString();
    m_sizeComboBox->setCurrentText(size);
    SvgItemDelegate::setSize(size.toInt());

    connect(m_sizeComboBox, QOverload<int>::of(&QComboBox::activated), this, [this]() {
        g_xTools.settings()->setValue(m_settingKeys.size, m_sizeComboBox->currentText());
        SvgItemDelegate::setSize(m_sizeComboBox->currentText().toInt());
        updateView();
    });
}

void MainWindow::initToolBarColor(QToolBar *toolBar)
{
    toolBar->addWidget(new QLabel(tr("Fill\nColor")));
    addSpacer(toolBar, 4);
    m_colorButton = new QPushButton(this);
    m_colorButton->setMinimumWidth(100);
    m_colorButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    toolBar->addWidget(m_colorButton);

    QString color = g_xTools.settings()->value(m_settingKeys.color).toString();
    SvgItemDelegate::setColor(color);
    if (color.isEmpty()) {
        m_colorButton->setStyleSheet(QString("background-color:%1").arg("#00000000"));
    } else {
        m_colorButton->setStyleSheet(QString("background-color:%1").arg(color));
    }

    connect(m_colorButton, &QPushButton::clicked, this, [this]() {
        QColor color = QColorDialog::getColor(Qt::white, this, tr("Select Color"));
        if (!color.isValid()) {
            SvgItemDelegate::setColor("");
            g_xTools.settings()->setValue(m_settingKeys.color, "");
            m_colorButton->setStyleSheet(QString("background-color:#00000000"));
        } else {
            SvgItemDelegate::setColor(color.name());
            g_xTools.settings()->setValue(m_settingKeys.color, color.name());
            m_colorButton->setStyleSheet(QString("background-color:%1").arg(color.name()));
        }

        updateView();
    });
}

void MainWindow::initToolBarSettings(QToolBar *toolBar)
{
    m_settingsMenu = new SettingsMenu(this);
    QWidgetAction *action = new QWidgetAction(this);
    action->setDefaultWidget(m_settingsMenu);

    QMenu *menu = new QMenu(this);
    menu->setIcon(materialIcon("settings.svg"));
    menu->addAction(action);
    toolBar->addAction(menu->menuAction());
}

void MainWindow::openPath()
{
    QString path = g_xTools.settings()->value(m_settingKeys.path).toString();
    path = QFileDialog::getExistingDirectory(this, tr("Open SVG Path"), path);
    if (path.isEmpty()) {
        return;
    }

    m_path = path;
    g_xTools.settings()->setValue(m_settingKeys.path, path);
    updateView();
}

void MainWindow::openFileLocation()
{
    QModelIndexList list = ui->tableView->selectionModel()->selectedIndexes();
    if (list.isEmpty()) {
        return;
    }

    QStringList pathList;
    for (const QModelIndex &index : list) {
        QString fileName = m_model->data(index, Qt::DisplayRole).toString();
        QString filePath = fileName.left(fileName.lastIndexOf('/'));
        pathList.append(filePath);
    }

    pathList.removeDuplicates();
    for (const QString &path : pathList) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    }
}

void MainWindow::copyTo(const QModelIndexList &list, const QString &path)
{
    for (const QModelIndex &index : list) {
        QString oldName = m_model->data(index, Qt::DisplayRole).toString();
        int lastIndexOf = oldName.lastIndexOf('/');
        QString newName = path + "/" + oldName.right(oldName.length() - lastIndexOf);
        if (QFile::copy(oldName, newName)) {
            qInfo() << QString("Copy %1 to %2 successfully").arg(oldName, newName);
        } else {
            qWarning() << QString("Copy %1 to %2 failed").arg(oldName, newName);
        }
    }
}

void MainWindow::exportTo(const QModelIndexList &list, const QString &path, int padding)
{
    for (const QModelIndex &index : list) {
        QString fileName = m_model->data(index, Qt::DisplayRole).toString();
        QPixmap pixmap(fileName);
        if (pixmap.isNull()) {
            qWarning() << QString("Load %1 failed").arg(fileName);
            continue;
        }

        int size = m_sizeComboBox->currentText().toInt();
        pixmap = pixmap.scaled(QSize(size, size), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QUrl url(fileName);
        QString newFileName = url.fileName();
        newFileName = newFileName.left(newFileName.lastIndexOf('.'));
        newFileName = path + "/" + newFileName;

        if (padding > 0) {
            QPixmap newPixmap(size, size);
            newPixmap.fill(Qt::transparent);
            QPainter painter(&newPixmap);
            auto cookedPadding = QMargins(padding, padding, padding, padding);
            painter.drawPixmap(newPixmap.rect() - cookedPadding, pixmap);
        }

#if 1
        QString fullPath = newFileName + "." + m_formatComboBox->currentText().toLower();
        if (!pixmap.save(fullPath, m_formatComboBox->currentText().toStdString().c_str())) {
            qWarning() << QString("Save as %1 failed").arg(fullPath);
        }
#else
        // Save all formats for testing...
        for (int i = 0; i < m_formatComboBox->count(); ++i) {
            QString format = m_formatComboBox->itemText(i).toLower();
            if (!pixmap.save(newFileName + "." + format, format.toStdString().c_str())) {
                qWarning() << QString("Save as %1 failed").arg(newFileName + "." + format);
            }
        }
#endif
    }
}

void MainWindow::saveAs()
{
    if (xApp::LicenseManager::singleton().isTrial()) {
        int ret = QMessageBox::warning(
            this,
            tr("Trial Limit"),
            tr("The operation is not supported by trial edition, please purchase full license."),
            QMessageBox::Ok | QMessageBox::Cancel);
        if (ret == QMessageBox::Ok) {
            purchase();
        }
        return;
    }

    QModelIndexList list = ui->tableView->selectionModel()->selectedIndexes();
    if (list.isEmpty()) {
        return;
    }

    QString path = QFileDialog::getExistingDirectory(this, tr("Copy to..."), QString());
    if (path.isEmpty()) {
        return;
    }

    if (m_formatComboBox->currentText() == "SVG") {
        copyTo(list, path);
    } else {
        if (m_settingsMenu->enablePadding()) {
            exportTo(list, path, m_settingsMenu->padding());
        } else {
            exportTo(list, path);
        }
    }
}

void MainWindow::copyFullPath()
{
    QModelIndexList list = ui->tableView->selectionModel()->selectedIndexes();
    if (list.isEmpty()) {
        return;
    }

    QStringList fileNames;
    for (const QModelIndex &index : list) {
        QString name = m_model->data(index, Qt::DisplayRole).toString();
        fileNames.append(name);
    }

    return qApp->clipboard()->setText(fileNames.join('\n'));
}

void MainWindow::copyFileName()
{
    QModelIndexList list = ui->tableView->selectionModel()->selectedIndexes();
    if (list.isEmpty()) {
        return;
    }

    QStringList fileNames;
    for (const QModelIndex &index : list) {
        QString name = m_model->data(index, Qt::DisplayRole).toString();
        int lastIndexOf = name.lastIndexOf('/');
        name = name.right(name.length() - lastIndexOf - 1);
        fileNames.append(name);
    }

    return qApp->clipboard()->setText(fileNames.join(' '));
}

void MainWindow::updateView()
{
    if (m_path.isEmpty()) {
        return;
    }

    if (m_isUpdating) {
        // 在更新过程中会忽略后续的更新请求，但更新完成后，如果需要跟新，会再次更新
        m_needUpdateAgain = true;
        return;
    }

    m_isUpdating = true;
    m_needUpdateAgain = false;
    if (ui->tableView->model()) {
        ui->tableView->model()->deleteLater();
    }
    ui->tableView->setModel(nullptr);

    int column = ui->tableView->width() / m_sizeComboBox->currentText().toInt();
    m_model = new SvgFileDataModel(column, this);
    connect(m_model, &SvgFileDataModel::dataReady, this, [this, column]() {
        this->m_isUpdating = false;
        if (m_model->svgFiles().isEmpty()) {
            QMessageBox::warning(this,
                                 tr("NO SVG FOUND"),
                                 tr("The path you selected has no svg file(s), please selected one "
                                    "has svg file(s) then try again"));
            return;
        }

        ui->tableView->setModel(m_model);
        for (int i = 0; i < column; ++i) {
            ui->tableView->setColumnWidth(i, this->m_sizeComboBox->currentText().toInt());
        }
        for (int i = 0; i < m_model->rowCount(); ++i) {
            ui->tableView->setRowHeight(i, this->m_sizeComboBox->currentText().toInt());
        }
        qInfo() << "Update view done, the with of column is "
                << this->m_sizeComboBox->currentText().toInt();
    });
    m_model->update(m_path, m_searchLineEdit->text().trimmed());
}

} // namespace xSvg
