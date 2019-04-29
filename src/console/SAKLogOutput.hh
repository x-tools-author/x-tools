/*
 * The file is encoding with utf-8 (with BOM)
 *
 * I write the comment with English, it's not because that I'm good at English,
 * but for "installing B".
 *
 * Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
 */
#ifndef APPMESSAGEOUTPUT_HH
#define APPMESSAGEOUTPUT_HH

#include <QThread>

class SAKLogOutput:public QThread
{
    Q_OBJECT
public:
    /**
     * @brief SAKLogOutput  -- 构造函数， nothing to say...
     * @param parent        -- 父控件
     */
    SAKLogOutput(QObject *parent = nullptr);

    /**
     * @brief outputMessage -- 将消息输出到文件保存
     * @param msg           -- 消息具体内容
     */
    void outputMessage(QString msg);
private:
    void run() final;
    void destroyThis();
};

#endif
