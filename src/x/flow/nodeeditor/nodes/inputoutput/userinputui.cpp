/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "userinputui.h"
#include "ui_userinputui.h"

#include <QKeyEvent>
#include <QTimer>

#include "common/common.h"
#include "nodeeditor/nodes/common/basenode.h"
#include "nodeeditor/nodes/common/basenodedata.h"
#include "userinput.h"

UserInputUi::UserInputUi(BaseNode *node, QWidget *parent)
    : BaseNodeUi(node, parent)
    , ui(new Ui::UserInputUi)
{
    QWidget *w = new QWidget(this);
    ui->setupUi(w);
    setEmbeddedWidget(w);

    setupTextFormat(ui->comboBoxFormat);
    setupAddition(ui->comboBoxPrefix);
    setupAddition(ui->comboBoxSuffix);
    setupEscapeCharacter(ui->comboBoxEscape);

    ui->lineEditInput->installEventFilter(this);
    ui->comboBoxTiming->addItem(tr("Disable"), -1);
    ui->comboBoxTiming->addItem("50", 50);
    for (int i = 100; i <= 1000; i += 100) {
        ui->comboBoxTiming->addItem(QString::number(i), i);
    }

    connect(ui->pushButtonSend, &QPushButton::clicked, this, &UserInputUi::output);
    connect(ui->comboBoxFormat, &QComboBox::currentIndexChanged, this, &UserInputUi::updateFormat);

    m_cycleTimer = new QTimer(this);
    connect(m_cycleTimer, &QTimer::timeout, this, &UserInputUi::output);
    connect(ui->comboBoxTiming, &QComboBox::activated, this, &UserInputUi::updateCycleTimer);
}

UserInputUi::~UserInputUi()
{
    delete ui;
}

UserInput::Data getUserInputData(Ui::UserInputUi *ui)
{
    UserInput::Data data;
    data.format = ui->comboBoxFormat->currentData().toInt();
    data.prefix = ui->comboBoxPrefix->currentData().toInt();
    data.suffix = ui->comboBoxSuffix->currentData().toInt();
    data.escape = ui->comboBoxEscape->currentData().toInt();
    data.interval = ui->comboBoxTiming->currentData().toInt();
    data.text = ui->lineEditInput->text();
    return data;
}

QJsonObject UserInputUi::save() const
{
    QJsonObject parameters = BaseNodeUi::save();
    UserInput::DataKeys keys;

    parameters.insert(keys.format, ui->comboBoxFormat->currentData().toInt());
    parameters.insert(keys.escape, ui->comboBoxEscape->currentData().toInt());
    parameters.insert(keys.prefix, ui->comboBoxPrefix->currentData().toInt());
    parameters.insert(keys.suffix, ui->comboBoxSuffix->currentData().toInt());
    parameters.insert(keys.interval, ui->comboBoxTiming->currentData().toInt());
    parameters.insert(keys.text, ui->lineEditInput->text());

    return parameters;
}

void UserInputUi::load(const QJsonObject &parameters)
{
    BaseNodeUi::load(parameters);
    UserInput::Data data = UserInput::loadData(parameters);

    int index = ui->comboBoxFormat->findData(data.format);
    ui->comboBoxFormat->setCurrentIndex(index < 0 ? 0 : index);
    index = ui->comboBoxPrefix->findData(data.prefix);
    ui->comboBoxPrefix->setCurrentIndex(index < 0 ? 0 : index);
    index = ui->comboBoxSuffix->findData(data.suffix);
    ui->comboBoxSuffix->setCurrentIndex(index < 0 ? 0 : index);
    index = ui->comboBoxEscape->findData(data.escape);
    ui->comboBoxEscape->setCurrentIndex(index < 0 ? 0 : index);
    index = ui->comboBoxTiming->findData(data.interval);
    ui->comboBoxTiming->setCurrentIndex(index < 0 ? 0 : index);

    ui->lineEditInput->setText(data.text);
    updateCycleTimer();
}

bool UserInputUi::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->lineEditInput) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            int key = keyEvent->key();
            if (key == Qt::Key_Return || key == Qt::Key_Enter) {
                output();
            }
        }
    }

    return BaseNodeUi::eventFilter(obj, event);
}

void UserInputUi::output()
{
    auto node = dynamic_cast<UserInput *>(m_node);
    if (!node) {
        return;
    }

    UserInput::Data data = getUserInputData(ui);
    if (data.text.isEmpty()) {
        data.text = bytes2string(QByteArray("(null)"), data.format);
    }

    QByteArray bytes = UserInput::data2Bytes(data);
    std::shared_ptr<BaseNodeData> nodeData = std::make_shared<BaseNodeData>(bytes);
    m_node->setInData(std::move(nodeData), 0);
}

void UserInputUi::updateFormat()
{
    int format = ui->comboBoxFormat->currentData().toInt();
    ui->lineEditInput->clear();
    setupTextFormatValidator(ui->lineEditInput, format);
}

void UserInputUi::updateCycleTimer()
{
    int currentInterval = this->ui->comboBoxTiming->currentData().toInt();
    if (currentInterval == -1) {
        this->m_cycleTimer->stop();
    } else {
        this->m_cycleTimer->setInterval(currentInterval);
        this->m_cycleTimer->start();
    }
}
