/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef QMDASSISTANTOUTPUTSAVE2FILE_HH
#define QMDASSISTANTOUTPUTSAVE2FILE_HH

#include <QMutex>
#include <QThread>
#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QSettings>
#include <QPushButton>
#include <QRadioButton>
#include <QWaitCondition>

namespace Ui {
    class SAKDebuggerOutputSave2File;
}

/// @brief saving parameters editing dialog
class SAKDebuggerOutputSave2File : public QDialog
{
    Q_OBJECT
public:
    struct ParametersContext {
        enum TextFormat{Bin, Utf8, Hex}format;
        enum DataType {Read,Written}type;
        QString fileName;
        bool saveTimestamp;
    };

    SAKDebuggerOutputSave2File(QSettings *settings,
                               QString settingGroup,
                               QWidget *parent = Q_NULLPTR);
    ~SAKDebuggerOutputSave2File();

    class Save2FileThread : public QThread
    {
    public:
        Save2FileThread(QObject *parent = Q_NULLPTR);
        ~Save2FileThread();

        void writeDataToFile(QByteArray data,
                             SAKDebuggerOutputSave2File::ParametersContext parameters);
    protected:
        void run() final;
    private:
        struct DataInfoStruct {
            QByteArray data;
            SAKDebuggerOutputSave2File::ParametersContext parameters;
        };

        QList<DataInfoStruct> m_dataList;
        QMutex m_dataListMutex;
        QMutex m_treadMutex;
        QWaitCondition m_threadWaitCondition;
    private:
        void innerWriteDataToFile(
                QByteArray data,
                SAKDebuggerOutputSave2File::ParametersContext parameters);
        DataInfoStruct takeDataInfo();
        QString bytes2String(QByteArray bytes, int format);
    };

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
    QString m_defaultPath;
    Save2FileThread *m_saveOutputDataThread;
    QSettings *m_settings;
    QString m_settingsOutputPath;
    QString m_settingKeyReadData;
    QString m_settingKeyWrittenData;
    QString m_settingKeyTimestamp;
    QString m_settingKeyDataType;
private:
    ParametersContext parameters(ParametersContext::DataType type);
signals:
    void writeDataToFile(
            QByteArray data,
            SAKDebuggerOutputSave2File::ParametersContext mParametersContext);
private:
    Ui::SAKDebuggerOutputSave2File *ui;
    QLineEdit *m_pathLineEdit;
    QPushButton *m_selectPushButton;
    QCheckBox *m_readDataCheckBox;
    QCheckBox *m_writtenDataCheckBox;
    QCheckBox *m_timestampCheckBox;
    QRadioButton *m_binRadioButton;
    QRadioButton *m_hexRadioButton;
    QRadioButton *m_utf8RadioButton;
    QPushButton *m_okPushButton;
    QPushButton *m_truncatePushButton;
private slots:
    void onPathLineEditTextChanged(const QString &text);
    void onSelectPushButtonClicked();
    void onTruncatePushButtonClicked();
    void onReadDataCheckBoxClicked();
    void onWrittenDataCheckBoxClicked();
    void onTimestampCheckBoxClicked();
    void onBinRadioButtonClicked();
    void onHexRadioButtonClicked();
    void onUtf8RadioButtonClicked();
};
Q_DECLARE_METATYPE(SAKDebuggerOutputSave2File::ParametersContext);
#endif
