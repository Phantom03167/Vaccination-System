#include "vaccinetraccodeinput.h"
#include "ui_vaccinetraccodeinput.h"

#include <QRegularExpressionValidator>
#include <QMessageBox>

VaccineTracCodeInput::VaccineTracCodeInput(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VaccineTracCodeInput)
{
    ui->setupUi(this);
    //setAttribute(Qt::WA_DeleteOnClose);
    ui->lineEdit_trace_code->setValidator(new QRegularExpressionValidator(QRegularExpression("^[a-zA-Z0-9]{0,5}$"),ui->lineEdit_trace_code));
}

VaccineTracCodeInput::~VaccineTracCodeInput()
{
    delete ui;
}

QString VaccineTracCodeInput::gettracecode()
{
    return ui->lineEdit_trace_code->text();
}

void VaccineTracCodeInput::on_pushButton_ok_clicked()
{
    //emit Ok();
    if(ui->lineEdit_trace_code->text().length()<5)
    {
        hide();
        QMessageBox::warning(this,"错误","疫苗追溯码错误！\t\t\n\n","确定");
        show();
    }
    else
        accept();
}


void VaccineTracCodeInput::on_pushButton_cancel_clicked()
{
    //emit Cancel();
    reject();
}

