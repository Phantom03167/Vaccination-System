#ifndef VACCINETRACCODEINPUT_H
#define VACCINETRACCODEINPUT_H

#include <QDialog>

namespace Ui {
class VaccineTracCodeInput;
}

class VaccineTracCodeInput : public QDialog
{
    Q_OBJECT

public:
    explicit VaccineTracCodeInput(QWidget *parent = nullptr);
    ~VaccineTracCodeInput();
    QString gettracecode();

signals:
    void Ok(QString trace_code);
    void Cancel();

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::VaccineTracCodeInput *ui;
};

#endif // VACCINETRACCODEINPUT_H
