#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "login.h"
#include "lineeditdelegate.h"
#include "spinboxdelegate.h"
#include "comboboxdelegate.h"
#include "datetimeeditdelegate.h"

#include <QWidget>
#include <QToolButton>
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlTableModel>
#include <QItemSelectionModel>

QT_BEGIN_NAMESPACE
namespace Ui { class mainWidget; }
QT_END_NAMESPACE

class mainWidget : public QWidget
{
    Q_OBJECT

public:
    mainWidget(QWidget *parent = nullptr);
    ~mainWidget();
    int customMessageBox(QMessageBox::Icon icon, const QString &title,
                         const QString &text, const QString &btn1,
                         const QString &btn2 = nullptr, const QString &btn3 = nullptr,
                         QWidget *parent = nullptr);

signals:
    void connect_successfully();
protected:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void on_toolButton_close_clicked();

    void on_toolButton_size_clicked();

    void on_toolButton_minimize_clicked();

    void on_toolButton_more_clicked();

    void leftmenubuttonClick();

    void on_toolButton_add_clicked();

    void on_toolButton_del_clicked();

    void on_toolButton_save_clicked();

    void on_toolButton_cancel_clicked();

    void on_toolButton_appoint_clicked();

    void on_toolButton_complete_app_clicked();

    void on_toolButton_cancel_app_clicked();

    void on_checkBox_resident_vaccination_information_stateChanged(int arg1);

    void on_radioButton_fuzzy_search_clicked(bool checked);

    void on_radioButton_advanced_search_clicked(bool checked);

    void on_toolButton_search_clicked();

    void on_pushButton_reset_search_clicked();

    void on_checkBox_vaccination_begin_time_stateChanged(int arg1);

    void on_checkBox_vaccination_begin_timerange_stateChanged(int arg1);

    void on_checkBox_vaccination_end_time_stateChanged(int arg1);

    void on_checkBox_vaccination_end_timerange_stateChanged(int arg1);

    void selected();


private:
    Ui::mainWidget *ui;
    QMenu* moremenu;
    Login *loginwidget;
    QToolButton *lastbutton;
    QSqlDatabase db;
    QSqlTableModel *sqltm;
    QSqlTableModel *sqltm_user;
    QItemSelectionModel *selsqltm;
    QItemSelectionModel *selsqltm_user;
    QModelIndex lastsel;

    LineEditDelegate textDelegate;
    LineEditDelegate otherNameDelegate;
    LineEditDelegate nameDelegate;
    LineEditDelegate idDelegate;
    LineEditDelegate phoneDelegate;
    LineEditDelegate appointmentStatusDelegate;
    LineEditDelegate vaccinationTimesDelegate;

    ComboBoxDelegate sexDelegate;
    ComboBoxDelegate vaccineTypeDelegate;
    ComboBoxDelegate vaccinationNeedTimesDelegate;

    SpinBoxDelegate ageDelegate;
    SpinBoxDelegate appointmentIdDelegate;
    SpinBoxDelegate vaccineIdDelegate;
    SpinBoxDelegate vaccinationSiteIdDelegate;

    DatetimeEditDelegate appointmentDatetimeDelegate;
    DatetimeEditDelegate vaccinationTimeDelegate;

    void initdata();
    void initForm();
    void initstylesheet();
    void initDelegate();
    bool create_login();
    void connect_mysql(QString username,QString password);
    void setDelegate();
    void all_button_disable();
};
#endif // MAINWIDGET_H
