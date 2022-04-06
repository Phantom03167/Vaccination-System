#include "login.h"
#include "ui_login.h"
#include "quihelper.h"
//#include "iconhelper.h"

#include <QObject>
#include <QDebug>

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    initForm();
    initstylesheet();
}

Login::~Login()
{
    delete ui;
}

void Login::initForm()
{
    //设置窗口无边框
    QUIHelper::setFramelessForm(this);

    //设置窗口居中显示
    QUIHelper::setFormInCenter(this);

    //设置关闭后释放窗口
    setAttribute(Qt::WA_DeleteOnClose);

    connect(ui->lineEdit_password,&QLineEdit::returnPressed,this,&Login::on_pushButton_login_clicked);
}

void Login::initstylesheet()
{
    //加载样式表
    QFile file(":/resource/qss/login.css");
    if (file.open(QFile::ReadOnly))
    {
        QString qss = QLatin1String(file.readAll());
        //QString paletteColor = qss.mid(20, 7);
        //qApp->setPalette(QPalette(paletteColor));
        this->setStyleSheet(qss);
        file.close();
    }
}

void Login::on_toolButton_close_clicked()
{
    close();
}


void Login::on_toolButton_minimize_clicked()
{
    showMinimized();
}


void Login::on_pushButton_forget_clicked()
{

}


void Login::on_pushButton_login_clicked()
{
    emit login(ui->lineEdit_username->text(),ui->lineEdit_password->text());
}


void Login::on_pushButton_quit_clicked()
{
    close();
}

