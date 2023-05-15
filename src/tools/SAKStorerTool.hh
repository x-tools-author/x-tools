/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKSTORERTOOL_H
#define SAKSTORERTOOL_H

#include <QTimer>
#include <QMutex>

#include "SAKBaseTool.hh"

class SAKStorerTool : public SAKBaseTool
{
    Q_OBJECT
public:
    explicit SAKStorerTool(QObject *parent = nullptr);
    ~SAKStorerTool();

    virtual void inputBytes(
            const QByteArray &bytes,
            const QVariant &context = QJsonObject()) final;

    bool saveRx(){return mParameters.saveRx;}
    void setSaveRx(bool save){mParameters.saveRx = save; emit saveRxChanged();}

    bool saveTx(){return mParameters.saveTx;}
    void setSaveTx(bool save){mParameters.saveTx; emit saveTxChanged();}

    Q_INVOKABLE void setPath(const QString &path);
    Q_INVOKABLE void setFormat(int format);
    Q_INVOKABLE void setSaveDate(bool saveDate);
    Q_INVOKABLE void setSaveTime(bool saveTime);
    Q_INVOKABLE void setSaveMs(bool saveMs);

signals:
    void saveRxChanged();
    void saveTxChanged();

protected:
    virtual void run() final;

private:
    struct Parameters {
        QString file;
        int format;
        bool saveRx;
        bool saveTx;
        bool saveDate;
        bool saveTime;
        bool saveMs;
    } mParameters;
    QMutex mParametersMutex;

    struct InputContext {
        QByteArray bytes;
        QVariant context;
    };
    QList<InputContext> mInputContextList;
    QMutex mInputContextListMutex;

private:
    void write2file();
};

#endif // SAKSTORERTOOL_H
