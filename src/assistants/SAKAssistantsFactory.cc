/******************************************************************************
 * Copyright 2022-2023 Qsaker(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 *****************************************************************************/
#include "SAKAssistantsFactory.hh"

#include <QCoreApplication>

#ifdef SAK_IMPORT_MODULE_FILECHECKASSISTANT
#include "SAKToolFileCheckAssistant.hh"
#endif
#ifdef SAK_IMPORT_MODULE_CRCASSISTANT
#include "SAKToolCRCAssistant.hh"
#endif
#ifdef SAK_IMPORT_MODULE_QRCODEASSISTANT
#include "SAKToolQRCodeCreator.hh"
#endif
#ifdef SAK_IMPORT_MODULE_FLOATASSISTANT
#include "SAKToolFloatAssistant.hh"
#endif
#ifdef SAK_IMPORT_MODULE_STRINGASSISTANT
#include "SAKToolStringAssistant.hh"
#endif
#ifdef SAK_IMPORT_MODULE_ATASSISTANT
#include "SAKAtAssistant.hh"
#endif
#ifdef SAK_IMPORT_MODULE_ASCIIASSISTANT
#include "SAKToolAsciiAssistant.h"
#endif
#ifdef SAK_IMPORT_MODULE_BROADCASTASSISTANT
#include "SAKToolBroadcastAssistant.h"
#endif

SAKAssistantsFactory::SAKAssistantsFactory(QObject *parent) : QObject(parent) {
  mTypeNameMap.insert(AssistantCrc, tr("CRC Assistant"));
  mTypeNameMap.insert(AssistantFile, tr("File Assistant"));
  mTypeNameMap.insert(AssistantAscii, tr("ASCII Assistant"));
  mTypeNameMap.insert(AssistantFloat, tr("Float Assistant"));
  mTypeNameMap.insert(AssistantString, tr("String Assistant"));
  mTypeNameMap.insert(AssistantBroadcast, tr("Broadcast Assistant"));
}

QVector<int> SAKAssistantsFactory::supportedAssistants() {
  // QList is not same as QVector in some Qt version.
  QList<int> ret = mTypeNameMap.keys();
  QVector<int> cooked;
  for (int i = 0; i < ret.count(); i++) {
    cooked.append(ret.at(i));
  }

  return cooked;
}

QString SAKAssistantsFactory::assistantName(int type) const {
  if (mTypeNameMap.contains(type)) {
    return mTypeNameMap.value(type);
  }

  QString name = QString("UnknowType(%1)").arg(type);
  return name;
}

SAKAssistantsFactory *SAKAssistantsFactory::instance() {
  static SAKAssistantsFactory f;
  return &f;
}

QWidget *SAKAssistantsFactory::newAssistant(int type) {
  if (type == AssistantCrc) {
    return new SAKToolCRCAssistant();
  } else if (type == AssistantFile) {
    return new SAKToolFileCheckAssistant();
  } else if (type == AssistantAscii) {
    return new SAKToolAsciiAssistant;
  } else if (type == AssistantFloat) {
    return new SAKToolFloatAssistant;
  } else if (type == AssistantString) {
    return new SAKToolStringAssistant;
  } else if (type == AssistantBroadcast) {
    return new SAKToolBroadcastAssistant;
  } else {
    return nullptr;
  }
}
