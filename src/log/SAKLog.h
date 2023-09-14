/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKLOG_HH
#define SAKLOG_HH

#include <QMutex>
#include <QThread>
#include <QtGlobal>
#include <QLoggingCategory>

#include "SAKTableModel.h"

class SAKLog : public QThread
{
    Q_OBJECT
    Q_PROPERTY(int logLifeCycle READ logLifeCycle WRITE setLogLifeCycle
               NOTIFY logLifeCycleChanged)
    Q_PROPERTY(int logLevel READ logLevel WRITE setLogLevel
               NOTIFY logLevelChanged)
    Q_PROPERTY(bool isPaused READ isPaused WRITE setIsPaused
               NOTIFY isPausedChanged)
private:
    explicit SAKLog(QObject *parent = nullptr);

public:
    ~SAKLog();
    static SAKLog *instance();
    int logLifeCycle();
    void setLogLifeCycle(int t);
    int logLevel();
    void setLogLevel(int level);
    bool isPaused();
    void setIsPaused(bool paused);

    QAbstractTableModel *tableModel();
    static void messageOutput(QtMsgType type,
                              const QMessageLogContext &context,
                              const QString &msg);
    void clear();
    QString logPath();

signals:    
    void logLifeCycleChanged();
    void logLevelChanged();
    void isPausedChanged();

protected:
    virtual void run() override;

private:
    struct LogContext {
        int type;
        QString category;
        QString msg;
    };
    static QVector<LogContext> mLogContextVector;
    static QMutex mLogContextVectorMutex;
    QVector<LogContext> mTemp;
    QMutex mTempMutex;

private:
    const QLoggingCategory mLoggingCategory{"sak.log"};
    SAKTableModel *mTableModel{nullptr};

    struct {
        int logLevel;
        int logLifeCycle;
        bool isPaused;
    } mParameters;
    QMutex mParametersMutex;
    const int mMaxTemp;

private slots:
    void onInvokeGetRowCount(int &count);
    void onInvokeGetColumnCount(int &count);
    void onInvokeGetData(QVariant &data,
                         const QModelIndex &index,
                         int role = Qt::DisplayRole);
    void onInvokeSetData(bool &result,
                         const QModelIndex &index,
                         const QVariant &value,
                         int role = Qt::EditRole);
    void onInvokeInsertRows(bool &result,
                            int row,
                            int count,
                            const QModelIndex &parent = QModelIndex());
    void onInvokeRemoveRows(bool &result,
                            int row,
                            int count,
                            const QModelIndex &parent = QModelIndex());
    void onInvokeGetHeaderData(QVariant &data,
                               int section,
                               Qt::Orientation orientation,
                               int role = Qt::DisplayRole);

private:
    void writeLog();
    void clearLog();
    QString logTypeFlag(int type);
};

#endif // SAKLOG_HH
