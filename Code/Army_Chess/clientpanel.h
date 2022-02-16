#ifndef CLIENTPANEL_H
#define CLIENTPANEL_H

#include <QDialog>
#include <QRegularExpressionValidator> //#include <QRegExp> // Qt 6 //#include <QRegExpValidator> // Qt 5

namespace Ui {
class ClientPanel;
}

class ClientPanel : public QDialog
{
    Q_OBJECT

private slots:
    void on_pushButton_1_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_0_clicked();
    void on_pushButton_dot_clicked();
    void on_pushButton_delete_clicked();

public:
    explicit ClientPanel(QWidget *parent = nullptr);
    Ui::ClientPanel *ui;
    QString returnIP();
    ~ClientPanel();

private:
    QKeyEvent *event;
};

#endif // CLIENTPANEL_H
