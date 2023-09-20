/*********************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#ifndef SAKDATASTRUCTURE_H
#define SAKDATASTRUCTURE_H

#include <QObject>
#include <Qt>

class SAKDataStructure : public QObject {
  Q_OBJECT
 public:
  explicit SAKDataStructure(QObject* parent = nullptr);

  enum SAKEnumTextFormat {
    TextFormatBin,
    TextFormatOct,
    TextFormatDec,
    TextFormatHex,
    TextFormatAscii,
    TextFormatUtf8
  };
  Q_ENUM(SAKEnumTextFormat)

  enum SAKEnumEscapeCharacterOption {
    EscapeCharacterOptionNone,
    EscapeCharacterOptionR,
    EscapeCharacterOptionN,
    EscapeCharacterOptionRN,
    EscapeCharacterOptionNR,
    EscapeCharacterOptionRAndN
  };
  Q_ENUM(SAKEnumEscapeCharacterOption)

  enum SAKEnumAffixes { AffixesNone, AffixesR, AffixesN, AffixesRN, AffixesNR };
  Q_ENUM(SAKEnumAffixes)

  enum EDEnumResponseOptions {
    ResponseOptionDisable,
    ResponseOptionEcho,
    ResponseOptionAlways,
    ResponseOptionInputEqualReference,
    ResponseOptionInputContainReference,
    ResponseOptionInputDiscontainReference
  };
  Q_ENUM(EDEnumResponseOptions)

  enum SAKEnumPalette {
    PaletteSystem,
    PaletteLight,
    PaletteDark,
    PaletteCustom
  };
  Q_ENUM(SAKEnumPalette)
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
  enum SAKHdpiPolicy {
    HdpiPolicyRound = int(Qt::HighDpiScaleFactorRoundingPolicy::Round),
    HdpiPolicyCeil = int(Qt::HighDpiScaleFactorRoundingPolicy::Ceil),
    HdpiPolicyFloor = int(Qt::HighDpiScaleFactorRoundingPolicy::Floor),
    HdpiPolicyRoundPreferFloor =
        int(Qt::HighDpiScaleFactorRoundingPolicy::RoundPreferFloor),
    HdpiPolicyPassThrough =
        int(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough),
    HdpiPolicySystem = 999
  };
  Q_ENUM(SAKHdpiPolicy)
#endif

  struct EDStructDataItem {
    int itemTextFormat;
    int itemTextEscapeChracter;
    QString itemText;
    int itemPrefix;
    int itemSuffix;

    bool itemCrcEnable;
    int itemCrcAlgorithm;
    int itemCrcStartIndex;
    int itemCrcEndIndex;
  };

 public:
  static QString affixesName(int affixes);
  static QByteArray affixesData(int affixes);
  static QString cookedString(int escapeCharacter, const QString& str);
  static QByteArray dataItemBytes(const EDStructDataItem& item);
  Q_INVOKABLE static QString cookEscapeCharacter(int option,
                                                 const QString& str);
};

#endif  // SAKDATASTRUCTURE_H
