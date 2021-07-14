/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKOUTPUTSAVE2FILEDIALOG_HH
#define SAKOUTPUTSAVE2FILEDIALOG_HH

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QSettings>
#include <QPushButton>
#include <QRadioButton>

namespace Ui {
    class SAKOutputSave2FileDialog;
}

class SAKDebugPage;
class SAKOutputSave2FileThread;
/// @brief saving parameters editing dialog
class SAKOutputSave2FileDialog : public QDialog
{
    Q_OBJECT
public:
    struct ParametersContext {
        enum TextFormat{Bin, Utf8, Hex}format;
        enum DataType {Read,Written}type;
        QString fileName;
        bool saveTimestamp;
    };

    SAKOutputSave2FileDialog(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKOutputSave2FileDialog();

    /**
     * @brief bytesRead: handle the read bytes
     * @param bytes: bytes need to be save to file
     */
    void bytesRead(QByteArray bytes);

    /**
     * @brief bytesWritten: handle the written bytes
     * @param bytes: bytes need to be save to file
     */
    void bytesWritten(QByteArray bytes);
private:
    QString mDefaultPath;
    SAKOutputSave2FileThread *mSaveOutputDataThread;
    QSettings *mSettings;
    SAKDebugPage *mDebugPage;
    QString mSettingsOutputPath;
    QString mSettingKeyReadData;
    QString mSettingKeyWrittenData;
    QString mSettingKeyTimestamp;
    QString mSettingKeyDataType;
private:
    ParametersContext parameters(ParametersContext::DataType type);
signals:
    void writeDataToFile(QByteArray data, ParametersContext mParametersContext);
private:
    Ui::SAKOutputSave2FileDialog *mUi;
    QLineEdit *mPathLineEdit;
    QPushButton *mSelectPushButton;
    QCheckBox *mReadDataCheckBox;
    QCheckBox *mWrittenDataCheckBox;
    QCheckBox *mTimestampCheckBox;
    QRadioButton *mBinRadioButton;
    QRadioButton *mHexRadioButton;
    QRadioButton *mUtf8RadioButton;
    QPushButton *mOkPushButton;
    QPushButton *mTruncatePushButton;
private slots:
    void on_pathLineEdit_textChanged(const QString &text);
    void on_selectPushButton_clicked();
    void on_truncatePushButton_clicked();
    void on_readDataCheckBox_clicked();
    void on_writtenDataCheckBox_clicked();
    void on_timestampCheckBox_clicked();
    void on_binRadioButton_clicked();
    void on_hexRadioButton_clicked();
    void on_utf8RadioButton_clicked();
};
Q_DECLARE_METATYPE(SAKOutputSave2FileDialog::ParametersContext);
#endif
