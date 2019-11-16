/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKVERSION_HH
#define SAKVERSION_HH

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
    SAKVersion(QWidget *parent = nullptr);
    ~SAKVersion();
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
    QLabel*         copyrightLabel;

    QPushButton*    copyQQ      = nullptr;  // 复制qq群号码按钮
protected:
    bool eventFilter(QObject *o, QEvent *e) final;
private:
    void openBlogUrl();
private slots:
    void copyQQNum();
};

#endif
