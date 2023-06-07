/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKLOG_HH
#define SAKLOG_HH

#include <QThread>
#include <QLoggingCategory>

class SAKLog : public QThread
{
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
private:
    explicit SAKLog(QObject *parent = nullptr);

    QString fileName();
    void setFileName(const QString &name);

public:
    ~SAKLog();
    static SAKLog *instance();

signals:
    void fileNameChanged();

protected:
    virtual void run() override;

private:
    struct {
        const QString fileNmae{"fileNmae"};
    } mSettingsKey;
    const QLoggingCategory mLoggingCategory{"SAK.Log"};
};

#endif // SAKLOG_HH
