/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef SAKVERSION_H
#define SAKVERSION_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>

namespace Ui {
class SAKVersion;
}

class SAKVersion:public QDialog
{
    Q_OBJECT
public:
    SAKVersion();
    ~SAKVersion();
    static SAKVersion* instance(){return sakVersionSingleton;}
    QString getVersion(){return version->text();}
    QString getQQNum(){return qq->text();}
    QString getEmail(){return email->text();}
private:
    Ui::SAKVersion* ui          = nullptr;
    QLabel*         version     = nullptr;  /// 显示版本号
    QLabel*         datetime    = nullptr;  /// 显示编译时间日期
    QLabel*         author      = nullptr;  /// 软件作者
    QLabel*         email       = nullptr;  /// 联系邮箱
    QLabel*         blog        = nullptr;  /// 博客地址
    QLabel*         qq          = nullptr;  /// QQ交流群

    QPushButton*    copyQQ      = nullptr;  // 复制qq群号码按钮

    static SAKVersion* sakVersionSingleton;
protected:
    bool eventFilter(QObject *o, QEvent *e) final;
private:
    void openBlogUrl();
private slots:
    void copyQQNum();
};

#endif
