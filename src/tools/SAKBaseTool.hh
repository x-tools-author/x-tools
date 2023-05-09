/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKBASETOOL_H
#define SAKBASETOOL_H

#include <atomic>
#include <QThread>
#include <QJsonObject>
#include <QLoggingCategory>

class SAKBaseTool : public QThread
{
    Q_OBJECT
    Q_PROPERTY(bool isWorking READ isWorking NOTIFY isWorkingChanged)
    Q_PROPERTY(bool enable READ enable WRITE setEnable NOTIFY enableChanged)
public:
    explicit SAKBaseTool(const char *logCategory, QObject *parent = Q_NULLPTR);
    virtual ~SAKBaseTool();

protected:
    /**
     * @brief inputBytes: The entrance of data.
     * @param bytes: The bytes input.
     * @param context: The extension parameters.
     */
    virtual void inputBytes(const QByteArray &bytes,
                            const QVariant &context = QJsonObject());

signals:
    void bytesInputted(const QByteArray &bytes, const QVariant &context);
    void bytesOutputted(const QByteArray &bytes, const QVariant &context);
    void errorOccure(const QString &errorString);

protected:
    void outputMessage(int type, const QString &info);

private:
    QLoggingCategory mLoggingCategory;
    QString mToolName{""};

protected:
    bool mIsWorking;
    bool isWorking(){return mIsWorking;}
    Q_SIGNAL void isWorkingChanged();

    std::atomic_bool mEnable{true};
    bool enable(){return mEnable;}
    void setEnable(bool enable){mEnable = enable; emit enableChanged();}
    Q_SIGNAL void enableChanged();
};

#endif // SAKBASETOOL_H
