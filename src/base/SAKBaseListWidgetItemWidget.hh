/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKBASELISTWIDGETITEMWIDGET_HH
#define SAKBASELISTWIDGETITEMWIDGET_HH

#include <QWidget>

class SAKBaseListWidget;
class SAKBaseListWidgetItemWidget : public QWidget
{
    Q_OBJECT
    friend class SAKBaseListWidget;
public:
    explicit SAKBaseListWidgetItemWidget(QWidget *parent = Q_NULLPTR);
    explicit SAKBaseListWidgetItemWidget(quint64 id, QWidget *parent = Q_NULLPTR);

    quint64 id(){return mId;}
    bool enable(){return mEnable;}
    void setEnable(bool enable){mEnable = enable;}
protected:
    virtual void onBytesRead(QByteArray bytes){Q_UNUSED(bytes);}
private:
    quint64 mId;
    bool mEnable;
private:
    void onBytesReadPrivate(QByteArray bytes);
signals:
    void invokeWriteCookedBytes(QByteArray bytes);
};

#endif // SAKBASELISTWIDGETITEMWIDGET_HH
