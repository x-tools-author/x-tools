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
#include <QInputDialog>
#include <QJsonArray>
#include <QMessageBox>
#include <QObject>

#include <coap3/coap.h>

#include "utilities/highlighter/syntaxhighlighterjson.h"
#include "utilities/iconengine.h"

#include "coapclient.h"
#include "coapcommon.h"
#include "coapmsgview.h"
#include "coappayloadview.h"
#include "coapresourceview.h"

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
        setupOptionUriPath();
        setupOptionContextFormat();
        ui->pushButtonClose->setEnabled(false);
        ui->toolButtonClient->hide();
#if 0
        m_syntaxHighlighter = new xTools::SyntaxHighlighterJson(ui->textEditPayload->document());
#endif

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
        connect(ui->toolButtonPayloadEdit, &QToolButton::clicked, this, [=]() { emit q->invokeChangeTabIndex(1); });
        // clang-format on

        // client
        m_client = new CoAPClient(q);
        connect(m_client, &CoAPClient::started, this, [=]() { onStarted(); });
        connect(m_client, &CoAPClient::finished, this, [=]() { onFinished(); });
        connect(m_client,
                &CoAPClient::messageReceived,
                this,
                [=](std::shared_ptr<CoAPMsgItem> request, std::shared_ptr<CoAPMsgItem> response) {
                    onMessageReceived(request, response);
                });

        // message view
        m_msgView = new CoAPMsgView(q);
        auto* layout = new QHBoxLayout(ui->widget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(4);
        layout->addWidget(m_msgView);

        // payload view
        m_payloadView = new CoAPPayloadView(q);
        connect(ui->toolButtonPayloadSave, &QToolButton::clicked, q, [=]() {
            onSavePayloadBtnClicked();
        });
        connect(m_payloadView,
                &CoAPPayloadView::payloadActivated,
                q,
                [=](int format, const QByteArray& payload) { onPayloadActivated(format, payload); });
    }
    ~CoAPClientUiPrivate() override { delete ui; }

public:
    Ui::CoAPClientUi* ui{nullptr};
    CoAPMsgView* m_msgView{nullptr};
    CoAPResourceView* m_resourceView{nullptr};
    CoAPPayloadView* m_payloadView{nullptr};
    xTools::SyntaxHighlighterJson* m_syntaxHighlighter{nullptr};

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
        ui->toolButtonPayloadEdit->setIcon(xIcon(":res/icons/edit_note.svg"));
        ui->toolButtonPayloadLoad->setIcon(xIcon(":res/icons/file_open.svg"));
        ui->toolButtonPayloadSave->setIcon(xIcon(":res/icons/save.svg"));
        connect(ui->textEditPayload, &QTextEdit::textChanged, q, [=]() { onPayloadTextChanged(); });
    }
    void setupOptionUriPath()
    {
        ui->toolButtonUriPathAdd->setIcon(xIcon(":res/icons/add.svg"));
        ui->toolButtonUriPathRemove->setIcon(xIcon(":res/icons/remove.svg"));
        connect(ui->toolButtonUriPathAdd, &QToolButton::clicked, this, [=]() {
            const QString txt = ui->comboBoxOptionUriPath->currentText();
            int index = this->ui->comboBoxOptionUriPath->findText(txt);
            if (index == -1 && !txt.isEmpty()) {
                this->ui->comboBoxOptionUriPath->addItem(txt);
            }
        });
        connect(ui->toolButtonUriPathRemove, &QToolButton::clicked, this, [=]() {
            const QString txt = ui->comboBoxOptionUriPath->currentText();
            int index = this->ui->comboBoxOptionUriPath->findText(txt);
            if (index != -1) {
                this->ui->comboBoxOptionUriPath->removeItem(index);
            }
        });
    }
    void setupOptionContextFormat()
    {
        CoAPCommon::setupContextFormat(ui->comboBoxOptionContentFormat);
    }
    void sendMessage(const coap_request_t requestType) const
    {
        QByteArray payload = ui->textEditPayload->toPlainText().toUtf8();
        QByteArray resource = ui->comboBoxOptionUriPath->currentText().toUtf8();
        int contextFormat = ui->comboBoxOptionContentFormat->currentData().toInt();
        emit m_client->invokeSendMessage(payload,
                                         resource,
                                         contextFormat,
                                         static_cast<int>(requestType));
    }

private:
    void onStarted() const
    {
        ui->pushButtonClose->setEnabled(true);
        ui->pushButtonOpen->setEnabled(false);
    }
    void onFinished() const
    {
        ui->pushButtonClose->setEnabled(false);
        ui->pushButtonOpen->setEnabled(true);
    }
    void onOpenButtonClicked() const
    {
        ui->pushButtonClose->setEnabled(false);
        ui->pushButtonOpen->setEnabled(false);
        onCloseButtonClicked();
        m_client->startClient(q->save());
    }
    void onCloseButtonClicked() const
    {
        ui->pushButtonClose->setEnabled(false);
        ui->pushButtonOpen->setEnabled(false);
        m_client->stopClient();
    }
    void onMessageReceived(std::shared_ptr<CoAPMsgItem> request,
                           std::shared_ptr<CoAPMsgItem> response) const
    {
        m_msgView->addMessage(request, response);
    }
    void onSavePayloadBtnClicked() const
    {
        QString description = QInputDialog::getText(q,
                                                    tr("Payload Description"),
                                                    tr("Please enter the payload description:"),
                                                    QLineEdit::Normal,
                                                    QString(tr("Payload description")));
        if (description.isEmpty()) {
            return;
        }

        int contextFormat = ui->comboBoxOptionContentFormat->currentData().toInt();
        QByteArray payload = ui->textEditPayload->toPlainText().toUtf8();
        QString errStr = m_payloadView->addPayload(description, contextFormat, payload);
        if (!errStr.isEmpty()) {
            QMessageBox::warning(q,
                                 tr("Save Payload Failed"),
                                 tr("Failed to save payload: %1").arg(errStr));
        }
    }
    void onPayloadActivated(int format, const QByteArray& payload) const
    {
        int index = ui->comboBoxOptionContentFormat->findData(format);
        if (index == -1) {
            QMessageBox::warning(q,
                                 tr("Unknown Content Format"),
                                 tr("The content format %1 is unknown.").arg(format));
            return;
        }
        ui->comboBoxOptionContentFormat->setCurrentIndex(index);
        ui->textEditPayload->setPlainText(QString::fromUtf8(payload));
    }
};

struct CoAPClientUiParameterKeys
{
    const QString payload{"payload"};
    const QString uriPath{"uriPath"};
    const QString uriPaths{"uriPaths"};
    const QString contextFormat{"contextFormat"};

    const QString message{"message"};
    const QString payloads{"payloads"};
};

CoAPClientUi::CoAPClientUi(QWidget* parent)
    : QWidget(parent)
{
    d = new CoAPClientUiPrivate(this);

    CoAPCommon::setupSocketAddress(d->ui->comboBoxServerIp);
    CoAPCommon::setupSocketPort(d->ui->spinBoxServerPort);
    CoAPCommon::setupSocketProtocol(d->ui->comboBoxProtocol);

    d->ui->toolButtonPayloadLoad->setMenu(d->m_payloadView->toolButtonMenu());
    d->ui->toolButtonPayloadLoad->setPopupMode(QToolButton::MenuButtonPopup);
}

CoAPClientUi::~CoAPClientUi()
{
    // Nothing to do yet
}

QJsonObject CoAPClientUi::save()
{
    CoAPCommon::ClientParameterKeys keys;
    QJsonObject obj;
    obj[keys.serverAddress] = d->ui->comboBoxServerIp->currentText();
    obj[keys.serverPort] = d->ui->spinBoxServerPort->value();
    obj[keys.protocol] = d->ui->comboBoxProtocol->currentData().toInt();

    CoAPClientUiParameterKeys uiKeys;
    obj[uiKeys.payload] = d->ui->textEditPayload->toPlainText();
    QJsonArray uriPaths;
    for (int i = 0; i < d->ui->comboBoxOptionUriPath->count(); ++i) {
        uriPaths.append(d->ui->comboBoxOptionUriPath->itemText(i));
    }
    obj[uiKeys.uriPaths] = uriPaths;
    obj[uiKeys.uriPath] = d->ui->comboBoxOptionUriPath->currentText();
    obj[uiKeys.contextFormat] = d->ui->comboBoxOptionContentFormat->currentData().toInt();
    obj[uiKeys.message] = d->m_msgView->save();
    obj[uiKeys.payloads] = d->m_payloadView->save();

    return obj;
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
    CoAPCommon::PayloadContext ctx = CoAPCommon::defaultPayloadContext();
    // Payload
    QString payload = obj.value(uiKeys.payload).toString(QString::fromUtf8(ctx.data));
    payload = payload.isEmpty() ? QString::fromUtf8(ctx.data) : payload;
    d->ui->textEditPayload->setPlainText(payload);
    // Options: URI Path
    const QJsonArray uriPaths = obj.value(uiKeys.uriPaths).toArray();
    d->ui->comboBoxOptionUriPath->clear();
    for (const QJsonValue& val : uriPaths) {
        QString path = val.toString();
        if (!path.isEmpty()) {
            d->ui->comboBoxOptionUriPath->addItem(path);
        }
    }
    const QString defaultResource = QString("path/to/resource");
    QString uriPath = obj.value(uiKeys.uriPath).toString(defaultResource);
    uriPath = uriPath.isEmpty() ? defaultResource : uriPath;
    d->ui->comboBoxOptionUriPath->setCurrentText(uriPath);
    // Options: Content Format
    int defaultFormat = COAP_MEDIATYPE_APPLICATION_JSON;
    int format = obj.value(uiKeys.contextFormat).toInt(defaultFormat);
    index = d->ui->comboBoxOptionContentFormat->findData(format);
    index = index == -1 ? d->ui->comboBoxOptionContentFormat->findData(defaultFormat) : index;
    d->ui->comboBoxOptionContentFormat->setCurrentIndex(index);
    // Pages
    d->m_msgView->load(obj.value(uiKeys.message).toObject());
    d->m_payloadView->load(obj.value(uiKeys.payloads).toObject());
}

QWidget* CoAPClientUi::resourceView() const
{
    return d->m_resourceView;
}

QWidget* CoAPClientUi::payloadView() const
{
    return d->m_payloadView;
}

} // namespace xCoAP
