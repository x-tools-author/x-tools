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

#include <QMutex>
#include <QThread>
#include <QtGlobal>
#include <QLoggingCategory>

#include "SAKTableModel.hh"

class SAKLog : public QThread
{
    Q_OBJECT
    Q_PROPERTY(qint64 logLifeCycle READ logLifeCycle WRITE setLogLifeCycle NOTIFY logLifeCycleChanged)
private:
    explicit SAKLog(QObject *parent = nullptr);

public:
    ~SAKLog();
    static SAKLog *instance();
    qint64 logLifeCycle();
    void setLogLifeCycle(qint64 t);

    QAbstractTableModel *tableModel();
    static void messageOutput(QtMsgType type,
                              const QMessageLogContext &context,
                              const QString &msg);

signals:
    void fileNameChanged();
    void logLifeCycleChanged();

protected:
    virtual void run() override;

private:
    struct LogContext {
        QtMsgType type;
        QString category;
        QString msg;
    };
    static QVector<LogContext> mLogContextVector;
    static QMutex mLogContextVectorMutex;

private:
    struct {
        const QString logFileLifeCycle{"logFileLifeCycle"};//(days)
    } mSettingsKey;
    const QLoggingCategory mLoggingCategory{"SAK.Log"};
    QString mLogPath;
    qint64 mLogLifeCycle;
    SAKTableModel *mTableModel{nullptr};

private:
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
    void messageOutputInner(QtMsgType type,
                            const QMessageLogContext &context,
                            const QString &msg);
};

#endif // SAKLOG_HH
