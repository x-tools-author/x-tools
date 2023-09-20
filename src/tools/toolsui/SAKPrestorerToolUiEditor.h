/*********************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#ifndef SAKPRESTORERTOOLUIEDITOR_HH
#define SAKPRESTORERTOOLUIEDITOR_HH

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class SAKPrestorerToolUiEditor;
}

class SAKPrestorerToolUiEditor : public QDialog
{
    Q_OBJECT
public:
    explicit SAKPrestorerToolUiEditor(QWidget *parent = nullptr);
    ~SAKPrestorerToolUiEditor();

    Q_INVOKABLE QJsonObject parameters();
    Q_INVOKABLE void setParameters(const QJsonObject &params);

private:
    Ui::SAKPrestorerToolUiEditor *ui{nullptr};
};

#endif // SAKPRESTORERTOOLUIEDITOR_HH
