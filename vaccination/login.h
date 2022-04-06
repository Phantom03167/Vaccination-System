#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QDialog>
#include <QString>

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    void initForm();
    void initstylesheet();

    int width;
    int height;

signals:
    void login(QString username,QString password);

protected:

private slots:
    void on_toolButton_close_clicked();

    void on_toolButton_minimize_clicked();

    void on_pushButton_forget_clicked();

    void on_pushButton_login_clicked();

    void on_pushButton_quit_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
