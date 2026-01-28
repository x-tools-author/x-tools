/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "coapclientui.h"
#include "ui_coapclientui.h"

#include <QHBoxLayout>
#include <QObject>

#include <coap3/coap.h>

#include "coapclient.h"
#include "coapcommon.h"
#include "coapmsgview.h"
#include "coapoptioneditor.h"
#include "coapoptionview.h"
#include "coappayloadview.h"
#include "coapresourceview.h"

#include "utilities/iconengine.h"

namespace Ui {
class CoAPClientUi;
}

namespace xCoAP {

class CoAPClientUiPrivate : QObject
{
public:
    CoAPClientUiPrivate(CoAPClientUi* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {
        ui = new Ui::CoAPClientUi();
        ui->setupUi(q_ptr);
        setupPayload();
        setupResource();
        setupOption();
        setupToken();
        ui->pushButtonClose->setEnabled(false);

        // clang-format off
        connect(ui->pushButtonOpen, &QPushButton::clicked, this, [=]() { onOpenButtonClicked(); });
        connect(ui->pushButtonClose, &QPushButton::clicked, this, [=]() { onCloseButtonClicked(); });
        connect(ui->pushButtonGet, &QPushButton::clicked, this, [=]() { sendMessage(COAP_REQUEST_GET); });
        connect(ui->pushButtonPost, &QPushButton::clicked, this, [=]() { sendMessage(COAP_REQUEST_POST); });
        connect(ui->pushButtonPut, &QPushButton::clicked, this, [=]() { sendMessage(COAP_REQUEST_PUT); });
        connect(ui->pushButtonDelete, &QPushButton::clicked, this, [=]() { sendMessage(COAP_REQUEST_DELETE); });
        connect(ui->pushButtonFetch, &QPushButton::clicked, this, [=]() { sendMessage(COAP_REQUEST_FETCH); });
        connect(ui->pushButtonPatch, &QPushButton::clicked, this, [=]() { sendMessage(COAP_REQUEST_PATCH); });
        connect(ui->pushButtonIPatch, &QPushButton::clicked, this, [=]() { sendMessage(COAP_REQUEST_IPATCH); });
        // clang-format on

        m_client = new CoAPClient(q);
        connect(m_client, &CoAPClient::started, this, [=]() { onStarted(); });
        connect(m_client, &CoAPClient::finished, this, [=]() { onFinished(); });
        connect(m_client,
                &CoAPClient::messageReceived,
                this,
                [=](std::shared_ptr<CoAPMsgItem> request, std::shared_ptr<CoAPMsgItem> response) {
                    onMessageReceived(request, response);
                });

        m_msgView = new CoAPMsgView(q);
        QHBoxLayout* layout = new QHBoxLayout(ui->widget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(4);
        layout->addWidget(m_msgView);
#if 0
        m_resourceView = new CoAPResourceView(q);
#endif
        m_payloadView = new CoAPPayloadView(q);
        m_optionView = new CoAPOptionView(q);
    }
    ~CoAPClientUiPrivate() { delete ui; }

public:
    Ui::CoAPClientUi* ui{nullptr};
    CoAPMsgView* m_msgView{nullptr};
    CoAPResourceView* m_resourceView{nullptr};
    CoAPPayloadView* m_payloadView{nullptr};
    CoAPOptionView* m_optionView{nullptr};

public:
    void onPayloadTextChanged()
    {
        QString payload = ui->textEditPayload->toPlainText();
        if (!payload.contains('\t')) {
            return;
        }

        QTextCursor cursor = ui->textEditPayload->textCursor();
        int pos = cursor.position();

        // Replace tab with spaces
        payload.replace("\t", "    ");
        ui->textEditPayload->blockSignals(true);
        ui->textEditPayload->setPlainText(payload);
        ui->textEditPayload->blockSignals(false);
        cursor.setPosition(pos + 3);
        ui->textEditPayload->setTextCursor(cursor);
    }

private:
    CoAPClientUi* q{nullptr};
    CoAPClient* m_client{nullptr};

private:
    void setupPayload()
    {
        ui->toolButtonPayloadFormat->setIcon(xIcon(":res/icons/transform.svg"));
        ui->toolButtonPayloadEdit->setIcon(xIcon(":res/icons/edit_note.svg"));
        ui->toolButtonPayloadLoad->setIcon(xIcon(":res/icons/file_open.svg"));
        ui->toolButtonPayloadSave->setIcon(xIcon(":res/icons/save.svg"));
        connect(ui->textEditPayload, &QTextEdit::textChanged, q, [=]() { onPayloadTextChanged(); });
    }
    void setupToken()
    {
        ui->toolButtonTokenFormat->setIcon(xIcon(":res/icons/transform.svg"));
        ui->toolButtonTokenRemove->setIcon(xIcon(":res/icons/save.svg"));
        ui->toolButtonTokenAdd->setIcon(xIcon(":res/icons/file_open.svg"));
        ui->toolButtonTokenEdit->setIcon(xIcon(":res/icons/edit_note.svg"));
    }
    void setupResource()
    {
        ui->toolButtonResourceFormat->setIcon(xIcon(":res/icons/transform.svg"));
        ui->toolButtonResourceRemove->setIcon(xIcon(":res/icons/save.svg"));
        ui->toolButtonResourceAdd->setIcon(xIcon(":res/icons/file_open.svg"));
        ui->toolButtonResourceEdit->setIcon(xIcon(":res/icons/edit_note.svg"));
    }
    void setupOption()
    {
        ui->toolButtonOptionFormat->setIcon(xIcon(":res/icons/transform.svg"));
        ui->toolButtonOptionRemove->setIcon(xIcon(":res/icons/save.svg"));
        ui->toolButtonOptionAdd->setIcon(xIcon(":res/icons/file_open.svg"));
        ui->toolButtonOptionEdit->setIcon(xIcon(":res/icons/edit_note.svg"));
        connect(ui->toolButtonOptionEdit, &QPushButton::clicked, this, [=]() {
            onOptionButtonClicked();
        });
    }
    void sendMessage(coap_request_t requestType)
    {
        QByteArray payload = ui->textEditPayload->toPlainText().toUtf8();
        QByteArray token = ui->comboBoxToken->currentText().toUtf8();
        QByteArray resource = ui->comboBoxResource->currentText().toUtf8();
        QByteArray option = ui->comboBoxOption->currentText().toUtf8();
        emit m_client->invokeSendMessage(payload,
                                         token,
                                         resource,
                                         option,
                                         static_cast<int>(requestType));
    }

private:
    void onStarted()
    {
        ui->pushButtonClose->setEnabled(true);
        ui->pushButtonOpen->setEnabled(false);
    }
    void onFinished()
    {
        ui->pushButtonClose->setEnabled(false);
        ui->pushButtonOpen->setEnabled(true);
    }
    void onOpenButtonClicked()
    {
        ui->pushButtonClose->setEnabled(false);
        ui->pushButtonOpen->setEnabled(false);
        onCloseButtonClicked();
        m_client->startClient(q->save());
    }
    void onCloseButtonClicked()
    {
        ui->pushButtonClose->setEnabled(false);
        ui->pushButtonOpen->setEnabled(false);
        m_client->stopClient();
    }
    void onOptionButtonClicked()
    {
        CoAPOptionEditor editor(q);
        editor.load(ui->comboBoxOption);
        int ret = editor.exec();
        if (ret == QDialog::Accepted) {
            editor.save(ui->comboBoxOption);
        }
    }
    void onMessageReceived(std::shared_ptr<CoAPMsgItem> request,
                           std::shared_ptr<CoAPMsgItem> response)
    {
        m_msgView->addMessage(request, response);
    }
};

struct CoAPClientUiParameterKeys
{
    const QString payload{"payload"};
    const QString token{"token"};
    const QString resource{"resource"};
    const QString option{"option"};

    const QString message{"message"};
};

CoAPClientUi::CoAPClientUi(QWidget* parent)
    : QWidget(parent)
{
    d = new CoAPClientUiPrivate(this);

    CoAPCommon::setupSocketAddress(d->ui->comboBoxServerIp);
    CoAPCommon::setupSocketPort(d->ui->spinBoxServerPort);
    CoAPCommon::setupSocketProtocol(d->ui->comboBoxProtocol);
}

CoAPClientUi::~CoAPClientUi() {}

QJsonObject CoAPClientUi::save()
{
    CoAPCommon::ClientParameterKeys keys;
    QJsonObject obj;
    obj[keys.serverAddress] = d->ui->comboBoxServerIp->currentText();
    obj[keys.serverPort] = d->ui->spinBoxServerPort->value();
    obj[keys.protocol] = d->ui->comboBoxProtocol->currentData().toInt();

    CoAPClientUiParameterKeys uiKeys;
    obj[uiKeys.payload] = d->ui->textEditPayload->toPlainText();
    obj[uiKeys.token] = d->ui->comboBoxToken->currentText();
    obj[uiKeys.resource] = d->ui->comboBoxResource->currentText();
    obj[uiKeys.option] = d->ui->comboBoxOption->currentText();
    obj[uiKeys.message] = d->m_msgView->save();

    return obj;
}

QString defaultPayload()
{
    QJsonObject obj;
    obj.insert("project", "xTools");
    obj.insert("subProject", "xCoAP");
    obj.insert("author", "x-tools-author");
    obj.insert("email", "x-tools@outlook.com");
    return QString::fromUtf8(QJsonDocument(obj).toJson(QJsonDocument::Indented));
}

void CoAPClientUi::load(const QJsonObject& obj)
{
    CoAPCommon::ClientParameterKeys keys;
    CoAPCommon::ClientParameters params = CoAPCommon::jsonObject2ClientParameters(obj);
    int index = d->ui->comboBoxServerIp->findText(params.serverAddress);
    if (index != -1) {
        d->ui->comboBoxServerIp->setCurrentIndex(index);
    }
    d->ui->spinBoxServerPort->setValue(params.serverPort);
    index = d->ui->comboBoxProtocol->findData(params.protocol);
    if (index != -1) {
        d->ui->comboBoxProtocol->setCurrentIndex(index);
    }

    CoAPClientUiParameterKeys uiKeys;
    const QString defaultResource = QString("/hello");
    const QString defaultToken = QString("xTools.xCoAP.Client");
    const QString defaultOption = QString("accept: application/json");
    QString payload = obj.value(uiKeys.payload).toString(defaultPayload());
    payload = payload.isEmpty() ? defaultPayload() : payload;
    d->ui->textEditPayload->setPlainText(payload);
    d->ui->comboBoxResource->setCurrentText(obj.value(uiKeys.resource).toString(defaultResource));
    d->ui->comboBoxToken->setCurrentText(obj.value(uiKeys.token).toString(defaultToken));
    d->ui->comboBoxOption->setCurrentText(obj.value(uiKeys.option).toString(defaultOption));
    d->m_msgView->load(obj.value(uiKeys.message).toObject());
}

QWidget* CoAPClientUi::resourceView()
{
    return d->m_resourceView;
}

QWidget* CoAPClientUi::payloadView()
{
    return d->m_payloadView;
}

QWidget* CoAPClientUi::optionView()
{
    return d->m_optionView;
}

} // namespace xCoAP
