/******************************************************************************
 * Copyright 2022-2023 Qsaker(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 *****************************************************************************/

#include "sakassistantsfactory.h"

#include <QCoreApplication>

#ifdef SAK_IMPORT_MODULE_FILECHECKASSISTANT
#include "SAKToolFileCheckAssistant.h"
#endif
#ifdef SAK_IMPORT_MODULE_CRCASSISTANT
#include "SAKToolCRCAssistant.h"
#endif
#ifdef SAK_IMPORT_MODULE_QRCODEASSISTANT
#include "SAKToolQRCodeCreator.h"
#endif
#ifdef SAK_IMPORT_MODULE_FLOATASSISTANT
#include "SAKToolFloatAssistant.h"
#endif
#ifdef SAK_IMPORT_MODULE_STRINGASSISTANT
#include "SAKToolStringAssistant.h"
#endif
#ifdef SAK_IMPORT_MODULE_ATASSISTANT
#include "SAKAtAssistant.h"
#endif
#ifdef SAK_IMPORT_MODULE_ASCIIASSISTANT
#include "SAKToolAsciiAssistant.h"
#endif
#ifdef SAK_IMPORT_MODULE_BROADCASTASSISTANT
#include "SAKToolBroadcastAssistant.h"
#endif
#ifdef SAK_IMPORT_MODULE_BASE64ASSISTANT
#include "sakbase64assistant.h"
#endif

SAKAssistantsFactory::SAKAssistantsFactory(QObject* parent) : QObject(parent) {
  type_name_map_.insert(kCrcAssistant, tr("CRC Assistant"));
  type_name_map_.insert(kFileCheckAssistant, tr("File Assistant"));
  type_name_map_.insert(kAsciiAssistant, tr("ASCII Assistant"));
  type_name_map_.insert(kNumberAssistant, tr("Float Assistant"));
  type_name_map_.insert(kStringAssistant, tr("String Assistant"));
  type_name_map_.insert(kBroadcastAssistant, tr("Broadcast Assistant"));
  type_name_map_.insert(kBase64Assistant, tr("Base64 Assistant"));
}

QList<int> SAKAssistantsFactory::SupportedAssistants() {
  QList<int> ret = type_name_map_.keys();
  QList<int> cooked;
  for (int i = 0; i < ret.count(); i++) {
    cooked.append(ret.at(i));
  }

  return cooked;
}

QString SAKAssistantsFactory::AssistantName(int type) const {
  if (type_name_map_.contains(type)) {
    return type_name_map_.value(type);
  }

  QString name = QString("UnknowType(%1)").arg(type);
  return name;
}

SAKAssistantsFactory* SAKAssistantsFactory::Instance() {
  static SAKAssistantsFactory* factory = nullptr;
  if (!factory) {
    factory = new SAKAssistantsFactory(qApp);
  }

  return factory;
}

QWidget* SAKAssistantsFactory::NewAssistant(int type) {
  if (type == kCrcAssistant) {
    return new SAKToolCRCAssistant();
  } else if (type == kFileCheckAssistant) {
    return new SAKToolFileCheckAssistant();
  } else if (type == kAsciiAssistant) {
    return new SAKToolAsciiAssistant;
  } else if (type == kNumberAssistant) {
    return new SAKToolFloatAssistant;
  } else if (type == kStringAssistant) {
    return new SAKToolStringAssistant;
  } else if (type == kBroadcastAssistant) {
    return new SAKToolBroadcastAssistant;
  } else if (type == kBase64Assistant) {
    return new SAKBase64Assisatnt;
  } else {
    return nullptr;
  }
}
