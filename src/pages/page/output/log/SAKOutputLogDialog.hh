#ifndef SAKOUTPUTLOGDIALOG_HH
#define SAKOUTPUTLOGDIALOG_HH

#include <QDialog>
#include <QPushButton>
#include <QTextBrowser>

namespace Ui {
    class SAKOutputLogDialog;
}

class SAKOutputLogDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SAKOutputLogDialog(QWidget *parent = Q_NULLPTR);
    ~SAKOutputLogDialog();
    void outputMessage(QString msg, bool isInfo = true);
private:
    Ui::SAKOutputLogDialog *mUi;
    QPushButton *mClosePushButton;
    QPushButton *mSaveLogPushButton;
    QPushButton *mClearLogPushButton;
    QTextBrowser *mLogTextBrowser;
private:
    void onClosePushButtonClicked();
    void onSaveLogPushButtonClicked();
    void onClearLogPushButtonClicked();
};

#endif // SAKOUTPUTLOGDIALOG_HH
