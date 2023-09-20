/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#ifndef SAKINTERFACE_H
#define SAKINTERFACE_H

#include <QObject>
#include <QVariant>

#define SAK_STATIC Q_INVOKABLE static

class SAKInterface : public QObject {
  Q_OBJECT
 public:
  explicit SAKInterface(QObject* parent = nullptr);

  Q_INVOKABLE static void setMaximumBlockCount(QVariant doc, int maximum);
  Q_INVOKABLE static void setAppFont(const QString& fontFamily);
  Q_INVOKABLE static void setClipboardText(const QString& text);

  Q_INVOKABLE static bool isQtHighDpiScalePolicy(int policy);

  Q_INVOKABLE static QString arrayToString(const QByteArray& array, int format);
  Q_INVOKABLE static QString dateTimeString(const QString& format);
  Q_INVOKABLE static QString cookedFileName(const QString& fileName);
  Q_INVOKABLE static QString string2hexString(const QString& str);
  Q_INVOKABLE static QString hexString2String(const QString& str);
  Q_INVOKABLE static QString buildDateTime(const QString& format);
  Q_INVOKABLE static QString dateFormat();
  Q_INVOKABLE static QString timeFormat();

  Q_INVOKABLE static QByteArray string2array(const QString& input, int format);
  Q_INVOKABLE static QByteArray arrayAppendArray(const QByteArray& a1,
                                                 const QByteArray& a2);
  Q_INVOKABLE static QByteArray arrayToHex(const QByteArray& array,
                                           char separator = '\0');
};

#endif  // SAKINTERFACE_H
