#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "framelesshelper.h"
#include "quihelper.h"
#include "vaccinetraccodeinput.h"

#include <QSqlError>
#include <QSqlRecord>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QString>
#include <QDateTime>

#include <QDebug>

mainWidget::mainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainWidget)
{
    ui->setupUi(this);
    initForm();
    initstylesheet();
    initDelegate();
}

mainWidget::~mainWidget()
{
    delete ui;
}

void mainWidget::initdata()
{
    sqltm = new QSqlTableModel(this);
    sqltm->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView->setModel(sqltm);

    sqltm_user = new QSqlTableModel(this);
    ui->tableView_user->setModel(sqltm_user);
    sqltm_user->setTable("用户管理");
    sqltm_user->select();

    //选择的模型
    selsqltm = ui->tableView->selectionModel();
    connect(selsqltm,&QItemSelectionModel::selectionChanged,this,&mainWidget::selected);
    lastsel = sqltm->index(0,0);

    selsqltm_user = ui->tableView_user->selectionModel();

    ui->toolButton_resident_manage->click();
}

void mainWidget::initForm()
{
    //设置窗口无边框
    QUIHelper::setFramelessForm(this);

    //设置窗口居中显示
    QUIHelper::setFormInCenter(this);

    //关联事件过滤器用于双击放大
    ui->top_menu->installEventFilter(this);

    //设置无边框窗体可拉伸
    setProperty("canMove", false);
    FramelessHelper *pHelper = new FramelessHelper(this);
    pHelper->activateOn(this);              //激活当前窗体
    //pHelper->setTitleHeight(30);            //设置窗体的标题栏高度
    pHelper->setWidgetMovable(true);        //设置窗体可移动
    pHelper->setWidgetResizable(true);      //设置窗体可缩放
    //pHelper->setRubberBandOnMove(true);     //设置橡皮筋效果-可移动
   // pHelper->setRubberBandOnResize(true);   //设置橡皮筋效果-可缩放

    //登录连接数据库成功
    connect(this,&mainWidget::connect_successfully,
            [this] ()
            {
                disconnect(loginwidget,&Login::login,this,&mainWidget::connect_mysql);
                this->loginwidget->close();
                initdata();
                show();
            });

    //创建登录窗口
    if(!create_login())
    {
        QMessageBox::warning(this,"错误","程序启动失败！");
        exit(0);
    }

    //设置顶部按钮属性
    ui->toolButton_size->setProperty("max",true);

    moremenu = new QMenu();
    moremenu->addAction("修改密码");
    moremenu->addAction("退出登录");
    //ui->toolButton_more->setMenu(moremenu);

    //添加自定义属性,用于切换ico用
    ui->toolButton_resident_manage->setProperty("icoName", "resident_manage");
    ui->toolButton_appointment_manage->setProperty("icoName", "appointment_manage");
    ui->toolButton_vaccination_information->setProperty("icoName", "vaccination_information");
    ui->toolButton_vaccination_site_manage->setProperty("icoName", "vaccination_site");
    ui->toolButton_vaccine_manage->setProperty("icoName", "vaccine_manage");
    ui->toolButton_system_manage->setProperty("icoName", "system_manage");

    QList<QToolButton *> btns = ui->left_menu->findChildren<QToolButton *>();
    foreach (QToolButton *btn, btns)
    {
        btn->installEventFilter(this);
        connect(btn, &QAbstractButton::clicked, this, &mainWidget::leftmenubuttonClick);
    }

    //设置控件属性
    ui->radioButton_advanced_search->click();

    //设置搜索编辑限制
    ui->lineEdit_name->setValidator(new QRegularExpressionValidator(QRegularExpression("^[\u4e00-\u9fa5]{0,20}$"),ui->lineEdit_name));
    ui->lineEdit_id->setValidator(new QRegularExpressionValidator(QRegularExpression("^\\d{0,5}$"),ui->lineEdit_id));
    ui->lineEdit_phone->setValidator(new QRegularExpressionValidator(QRegularExpression("^\\d{0,11}$"),ui->lineEdit_phone));
    ui->lineEdit_trace_code->setValidator(new QRegularExpressionValidator(QRegularExpression("^[\\d\\w]{0,5}$"),ui->lineEdit_trace_code));

    ui->toolButton_system_manage->setVisible(false);
}

void mainWidget::initstylesheet()
{
    //加载样式表
    QFile file(":/resource/qss/mainwidget.css");
    if (file.open(QFile::ReadOnly))
    {
        QString qss = QLatin1String(file.readAll());
        //QString paletteColor = qss.mid(20, 7);
        //qApp->setPalette(QPalette(paletteColor));
        this->setStyleSheet(qss);
        file.close();
    }
}

void mainWidget::initDelegate()
{
    textDelegate.setmaxlength(255);

    otherNameDelegate.setmaxlength(50);

    nameDelegate.setmaxlength(20);
    nameDelegate.setvalidator("^[\u4e00-\u9fa5]{0,20}$");

    idDelegate.setmaxlength(5);
    idDelegate.setvalidator("^\\d{0,5}$");

    phoneDelegate.setmaxlength(11);
    phoneDelegate.setvalidator("^\\d{0,11}$");

    appointmentStatusDelegate.setreadOnly(true);

    vaccinationTimesDelegate.setreadOnly(true);

    QStringList list = QStringList();

    list << "男" << "女";
    sexDelegate.setitems(list);
    list.clear();

    list << "灭活疫苗" << "腺病毒载体疫苗" << "重组亚单位疫苗";
    vaccineTypeDelegate.setitems(list);
    list.clear();

    list << "1" << "2" << "3";
    vaccinationNeedTimesDelegate.setitems(list);
    list.clear();

    ageDelegate.setnumrange(0,150);

    appointmentIdDelegate.setminnum(1);

    vaccineIdDelegate.setnumrange(1,5);

    vaccinationSiteIdDelegate.setminnum(1);

    appointmentDatetimeDelegate.setminidatetime(QDateTime::currentDateTime());

    vaccinationTimeDelegate.setdatetimeformat("HH:mm");
    vaccinationTimeDelegate.setcalendar(false);
}

int mainWidget::customMessageBox(QMessageBox::Icon icon, const QString &title, const QString &text, const QString &btn1, const QString &btn2, const QString &btn3, QWidget *parent)
{
    QMessageBox *box = new QMessageBox(icon, title, text,
                                       QMessageBox::Yes, parent);
    box->setButtonText(QMessageBox::Yes,btn1);
    if(!btn2.isEmpty())
    {
        box->addButton(QMessageBox::No);
        box->setButtonText(QMessageBox::No,btn2);
    }
    if(!btn3.isEmpty())
    {
        box->addButton(QMessageBox::Cancel);
        box->setButtonText(QMessageBox::Cancel,btn3);
    }
    box->setAttribute(Qt::WA_DeleteOnClose);
    return box->exec();
}

bool mainWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->top_menu)
    {
        if (event->type() == QEvent::MouseButtonDblClick)
            on_toolButton_size_clicked();
    }
    else if(watched->parent() == ui->left_menu && !((QToolButton*)watched)->isChecked())
    {
        if(event->type() == QEvent::HoverEnter)
           ((QToolButton*)watched)->setIcon(QIcon(":/resource/icon/"+ ((QToolButton*)watched)->property("icoName").toString() +"_focus.svg"));
        if(event->type() == QEvent::HoverLeave)
           ((QToolButton*)watched)->setIcon(QIcon(":/resource/icon/"+ ((QToolButton*)watched)->property("icoName").toString() +".svg"));
    }

    return QWidget::eventFilter(watched, event);
}

void mainWidget::on_toolButton_close_clicked()
{
    close();
}

void mainWidget::on_toolButton_size_clicked()
{
    if(ui->toolButton_size->property("max").toBool())
    {
        ui->toolButton_size->setIcon(QIcon(":/resource/icon/normalimize.svg"));
        showMaximized();
    }
    else
    {
        ui->toolButton_size->setIcon(QIcon(":/resource/icon/maximize.svg"));
        showNormal();
    }
    ui->toolButton_size->setProperty("max",!ui->toolButton_size->property("max").toBool());
}

void mainWidget::on_toolButton_minimize_clicked()
{
    showMinimized();
}

void mainWidget::on_toolButton_more_clicked()
{
    moremenu->move(this->pos().x()+this->size().width()-220,this->pos().y()+63);
    moremenu->show();
}

void mainWidget::leftmenubuttonClick()
{
    QToolButton *b = (QToolButton *)sender();
    QString text = b->text();

    QList<QToolButton *> btns = ui->left_menu->findChildren<QToolButton *>();
    foreach (QToolButton *btn, btns) //改变图标
    {
        QString icoName = btn->property("icoName").toString();
        if (btn != b)
        {
            btn->setChecked(false);
            btn->setIcon(QIcon(QString(":/resource/icon/%1.svg").arg(icoName)));
        }
        else
        {
            btn->setChecked(true);
            btn->setIcon(QIcon(QString(":/resource/icon/%1_focus.svg").arg(icoName)));
        }
    }

    if(b == lastbutton) //点击相同按钮
        return;
    else
        lastbutton = b;

    if(text != "系统管理")
    {
        ui->stackedWidget->setCurrentIndex(0);
        sqltm->setTable(text);
        setDelegate(); //设置表格代理
        sqltm->select();

        //设置控件是否启用
        all_button_disable();
        if(b == ui->toolButton_resident_manage) //居民管理
        {
            //上方按钮
            ui->toolButton_del->setProperty("canenable",true);
            ui->toolButton_appoint->setProperty("canenable",true);
            ui->toolButton_complete_app->setProperty("canenable",false);
            ui->toolButton_cancel_app->setProperty("canenable",false);
            ui->checkBox_resident_vaccination_information->setProperty("canenable",true);

            ui->toolButton_add->setEnabled(true);
            ui->toolButton_save->setEnabled(true);
            ui->toolButton_cancel->setEnabled(true);
            ui->checkBox_resident_vaccination_information->setEnabled(true);

            ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked |
                                           QAbstractItemView::EditKeyPressed |
                                           QAbstractItemView::AnyKeyPressed);

            //搜索栏
            QList<QLineEdit *> btns_1 = ui->widget_resident->findChildren<QLineEdit *>();
            foreach (QLineEdit *btn, btns_1)
            {
                btn->setEnabled(true);
            }
            btns_1.clear();

            QList<QCheckBox *> btns_2 = ui->widget_resident->findChildren<QCheckBox *>();
            foreach (QCheckBox *btn, btns_2)
            {
                btn->setEnabled(true);
            }
            btns_2.clear();

            QList<QComboBox *> btns_3 = ui->widget_resident->findChildren<QComboBox *>();
            foreach (QComboBox *btn, btns_3)
            {
                btn->setEnabled(true);
            }
            btns_3.clear();

            QList<QSpinBox *> btns_4 = ui->widget_resident->findChildren<QSpinBox *>();
            foreach (QSpinBox *btn, btns_4)
                btn->setEnabled(true);
            btns_4.clear();
        }
        else if(b == ui->toolButton_appointment_manage) //预约管理
        {
            //上方按钮
            ui->toolButton_del->setProperty("canenable",false);
            ui->toolButton_appoint->setProperty("canenable",true);
            ui->toolButton_complete_app->setProperty("canenable",true);
            ui->toolButton_cancel_app->setProperty("canenable",true);
            ui->checkBox_resident_vaccination_information->setProperty("canenable",false);

            ui->toolButton_add->setEnabled(true);

            ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

            //搜索栏
            QList<QLineEdit *> btns_1 = ui->widget_appointment->findChildren<QLineEdit *>();
            foreach (QLineEdit *btn, btns_1)
            {
                btn->setEnabled(true);
            }
            btns_1.clear();

            QList<QCheckBox *> btns_2 = ui->widget_appointment->findChildren<QCheckBox *>();
            foreach (QCheckBox *btn, btns_2)
            {
                btn->setEnabled(true);
            }
            btns_2.clear();

            QList<QComboBox *> btns_3 = ui->widget_appointment->findChildren<QComboBox *>();
            foreach (QComboBox *btn, btns_3)
            {
                btn->setEnabled(true);
            }
            btns_3.clear();

            QList<QSpinBox *> btns_4 = ui->widget_appointment->findChildren<QSpinBox *>();
            foreach (QSpinBox *btn, btns_4)
                btn->setEnabled(true);
            btns_4.clear();

            QList<QDateTimeEdit *> btns_5 = ui->widget_appointment->findChildren<QDateTimeEdit *>();
            foreach (QDateTimeEdit *btn, btns_5)
                btn->setEnabled(true);
            btns_5.clear();

            ui->lineEdit_name->setEnabled(true);
            ui->lineEdit_id->setEnabled(true);
            ui->checkBox_vaccination_site_id->setEnabled(true);
            ui->spinBox_vaccination_site_id->setEnabled(true);
            ui->lineEdit_vaccination_site_name->setEnabled(true);
            ui->lineEdit_vaccination_site_address->setEnabled(true);
            ui->checkBox_vaccine_id->setEnabled(true);
            ui->spinBox_vaccine_id->setEnabled(true);
            ui->lineEdit_vaccine_name->setEnabled(true);
        }
        else
        {
            //上方按钮
            ui->toolButton_appoint->setProperty("canenable",false);
            ui->toolButton_complete_app->setProperty("canenable",false);
            ui->toolButton_cancel_app->setProperty("canenable",false);
            ui->checkBox_resident_vaccination_information->setProperty("canenable",false);

            ui->toolButton_add->setEnabled(true);
            ui->toolButton_save->setEnabled(true);
            ui->toolButton_cancel->setEnabled(true);

            //搜索栏
            if(b == ui->toolButton_vaccination_information) //接种信息
            {
                ui->toolButton_del->setProperty("canenable",false);
                ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

                ui->toolButton_add->setEnabled(false);
                ui->toolButton_save->setEnabled(false);
                ui->toolButton_cancel->setEnabled(false);

                QList<QLineEdit *> btns_1 = ui->widget_vaccination->findChildren<QLineEdit *>();
                foreach (QLineEdit *btn, btns_1)
                {
                    btn->setEnabled(true);
                }
                btns_1.clear();

                QList<QCheckBox *> btns_2 = ui->widget_vaccination->findChildren<QCheckBox *>();
                foreach (QCheckBox *btn, btns_2)
                {
                    btn->setEnabled(true);
                }
                btns_2.clear();

                QList<QComboBox *> btns_3 = ui->widget_vaccination->findChildren<QComboBox *>();
                foreach (QComboBox *btn, btns_3)
                {
                    btn->setEnabled(true);
                }
                btns_3.clear();

                QList<QSpinBox *> btns_4 = ui->widget_vaccination->findChildren<QSpinBox *>();
                foreach (QSpinBox *btn, btns_4)
                    btn->setEnabled(true);
                btns_4.clear();

                QList<QDateTimeEdit *> btns_5 = ui->widget_vaccination->findChildren<QDateTimeEdit *>();
                foreach (QDateTimeEdit *btn, btns_5)
                    btn->setEnabled(true);
                btns_5.clear();

                ui->lineEdit_name->setEnabled(true);
                ui->lineEdit_id->setEnabled(true);
                ui->lineEdit_vaccine_name->setEnabled(true);
                ui->spinBox_vaccination_site_id->setEnabled(true);
                ui->lineEdit_vaccination_site_name->setEnabled(true);
                ui->checkBox_vaccine_id->setEnabled(true);
                ui->spinBox_vaccine_id->setEnabled(true);
                ui->comboBox_vaccination_times->setEnabled(true);
            }
            else if(b == ui->toolButton_vaccination_site_manage) //接种点管理
            {
                ui->toolButton_del->setProperty("canenable",true);
                ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked |
                                               QAbstractItemView::EditKeyPressed |
                                               QAbstractItemView::AnyKeyPressed);

                QList<QLineEdit *> btns_1 = ui->widget_vaccination_site->findChildren<QLineEdit *>();
                foreach (QLineEdit *btn, btns_1)
                {
                    btn->setEnabled(true);
                }
                btns_1.clear();

                QList<QCheckBox *> btns_2 = ui->widget_vaccination_site->findChildren<QCheckBox *>();
                foreach (QCheckBox *btn, btns_2)
                {
                    btn->setEnabled(true);
                }
                btns_2.clear();

                QList<QComboBox *> btns_3 = ui->widget_vaccination_site->findChildren<QComboBox *>();
                foreach (QComboBox *btn, btns_3)
                {
                    btn->setEnabled(true);
                }
                btns_3.clear();

                QList<QSpinBox *> btns_4 = ui->widget_vaccination_site->findChildren<QSpinBox *>();
                foreach (QSpinBox *btn, btns_4)
                    btn->setEnabled(true);
                btns_4.clear();

                QList<QDateTimeEdit *> btns_5 = ui->widget_vaccination_site->findChildren<QDateTimeEdit *>();
                foreach (QDateTimeEdit *btn, btns_5)
                    btn->setEnabled(true);
                btns_5.clear();
            }
            else if(b == ui->toolButton_vaccine_manage) //疫苗管理
            {
                ui->toolButton_del->setProperty("canenable",true);
                ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked |
                                               QAbstractItemView::EditKeyPressed |
                                               QAbstractItemView::AnyKeyPressed);
                QList<QLineEdit *> btns_1 = ui->widget_vaccine->findChildren<QLineEdit *>();
                foreach (QLineEdit *btn, btns_1)
                {
                    btn->setEnabled(true);
                }
                btns_1.clear();

                QList<QCheckBox *> btns_2 = ui->widget_vaccine->findChildren<QCheckBox *>();
                foreach (QCheckBox *btn, btns_2)
                {
                    btn->setEnabled(true);
                }
                btns_2.clear();

                QList<QComboBox *> btns_3 = ui->widget_vaccine->findChildren<QComboBox *>();
                foreach (QComboBox *btn, btns_3)
                {
                    btn->setEnabled(true);
                }
                btns_3.clear();

                QList<QSpinBox *> btns_4 = ui->widget_vaccine->findChildren<QSpinBox *>();
                foreach (QSpinBox *btn, btns_4)
                    btn->setEnabled(true);
                btns_4.clear();
            }
        }
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
}

bool mainWidget::create_login()
{
    loginwidget = new Login();
    if(loginwidget==NULL)
        return false;

    //绑定登录窗口槽函数
    //connect(loginwidget,&QObject::destroyed,this,&QWidget::close);
    connect(loginwidget,&Login::login,this,&mainWidget::connect_mysql);

    loginwidget->show();
    return true;
}

void mainWidget::connect_mysql(QString username,QString password)
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("127.0.0.1");
    db.setUserName(username);
    db.setPassword(password);
    db.setDatabaseName("新冠疫苗接种管理");

    if(!db.open())
        QMessageBox::warning(this,"打开失败",db.lastError().text());
    else
        emit connect_successfully();
}

void mainWidget::setDelegate()
{
    QString table = sqltm->tableName();
    if(table == "居民管理")
    {
        ui->tableView->setItemDelegateForColumn(0,&nameDelegate);
        ui->tableView->setItemDelegateForColumn(1,&sexDelegate);
        ui->tableView->setItemDelegateForColumn(2,&ageDelegate);
        ui->tableView->setItemDelegateForColumn(3,&idDelegate);
        ui->tableView->setItemDelegateForColumn(4,&phoneDelegate);
        ui->tableView->setItemDelegateForColumn(5,&textDelegate);
        ui->tableView->setItemDelegateForColumn(7,&textDelegate);
    }
    else if(table == "预约记录管理")
    {
        //ui->tableView->setItemDelegateForColumn(0,&appointmentIdDelegate);
        ui->tableView->setItemDelegateForColumn(0,&idDelegate);
        //ui->tableView->setItemDelegateForColumn(2,&vaccinationTimesDelegate);
        ui->tableView->setItemDelegateForColumn(1,&vaccineIdDelegate);
        ui->tableView->setItemDelegateForColumn(2,&vaccinationSiteIdDelegate);
        ui->tableView->setItemDelegateForColumn(3,&appointmentDatetimeDelegate);
        //ui->tableView->setItemDelegateForColumn(6,&appointmentStatusDelegate);
    }
    else if(table == "接种点管理")
    {
        ui->tableView->setItemDelegateForColumn(0,&vaccinationSiteIdDelegate);
        ui->tableView->setItemDelegateForColumn(1,&otherNameDelegate);
        ui->tableView->setItemDelegateForColumn(2,&textDelegate);
        ui->tableView->setItemDelegateForColumn(3,&vaccinationTimeDelegate);
        ui->tableView->setItemDelegateForColumn(4,&vaccinationTimeDelegate);
    }
    else if(table == "疫苗管理")
    {
        ui->tableView->setItemDelegateForColumn(0,&vaccineIdDelegate);
        ui->tableView->setItemDelegateForColumn(1,&otherNameDelegate);
        ui->tableView->setItemDelegateForColumn(2,&vaccineTypeDelegate);
        ui->tableView->setItemDelegateForColumn(3,&otherNameDelegate);
        ui->tableView->setItemDelegateForColumn(4,&vaccinationNeedTimesDelegate);
        ui->tableView->setItemDelegateForColumn(5,&textDelegate);
    }
}

void mainWidget::all_button_disable()
{
    QList<QToolButton *> btns_1 = ui->above_menu->findChildren<QToolButton *>();
    foreach (QToolButton *btn, btns_1)
        btn->setEnabled(false);

    ui->checkBox_resident_vaccination_information->setCheckState(Qt::Unchecked);
    ui->checkBox_resident_vaccination_information->setEnabled(false);

    QList<QCheckBox *> btns_2 = ui->scrollArea->findChildren<QCheckBox *>();
    foreach (QCheckBox *btn, btns_2)
    {
        btn->setCheckState(Qt::Unchecked);
        btn->setEnabled(false);
    }

    QList<QComboBox *> btns_3 = ui->scrollArea->findChildren<QComboBox *>();
    foreach (QComboBox *btn, btns_3)
        btn->setEnabled(false);

    QList<QSpinBox *> btns_4 = ui->scrollArea->findChildren<QSpinBox *>();
    foreach (QSpinBox *btn, btns_4)
        btn->setEnabled(false);

    QList<QLineEdit *> btns_5 = ui->scrollArea->findChildren<QLineEdit *>(QRegularExpression("^lineEdit"));
    foreach (QLineEdit *btn, btns_5)
        btn->setEnabled(false);
    QList<QDateTimeEdit *> btns_6 = ui->scrollArea->findChildren<QDateTimeEdit *>();
    foreach (QDateTimeEdit *btn, btns_6)
        btn->setEnabled(false);
}

void mainWidget::on_toolButton_add_clicked()
{
    if(sqltm->isDirty())
    {
        switch(customMessageBox(QMessageBox::Warning, "未保存的修改",
                                "是否保存修改的内容？\t\t\n\n",
                                "保存", "不保存", "取消", this))
        {
        case QMessageBox::Yes:
            qDebug()<<"11111";
            ui->toolButton_save->click();
            if(sqltm->isDirty())
                return;
            else
                break;
        case QMessageBox::No:
            ui->toolButton_cancel->click();
            if(sqltm->isDirty())
                return;
            else
                break;
        case QMessageBox::Cancel:
            qDebug()<<"33333";
            return;
        }
    }

    if(ui->toolButton_appointment_manage->isChecked())
    {
        ui->toolButton_save->setEnabled(true);
        ui->toolButton_cancel->setEnabled(true);
    }

    //获取空记录
    QSqlRecord empty_record = sqltm->record();

    //插入空记录
    sqltm->insertRecord(0,empty_record);

    //选择新插入的行
    selsqltm->clear();
    selsqltm->setCurrentIndex(sqltm->index(0,0),QItemSelectionModel::SelectCurrent|QItemSelectionModel::Rows);
}

void mainWidget::on_toolButton_del_clicked()
{
    switch(customMessageBox(QMessageBox::Warning,"删除",
                            "是否确认删除选中的的内容？\t\t\n\n",
                            "删除", "取消","",this))
    {
    case QMessageBox::Yes:
    {
        //获取选中模型的索引列表
        QModelIndexList indexlist = selsqltm->selectedRows();
        int size=indexlist.size();
        for(int i=0;i<size;i++)
        {
            if(!sqltm->removeRow(indexlist.at(i).row()))
                QMessageBox::warning(this,"删除失败",sqltm->lastError().databaseText());
        }
        if(!sqltm->submitAll())
            QMessageBox::warning(this,"删除错误",sqltm->lastError().databaseText());
        break;
    }
    case QMessageBox::No:
        return;
    }
}

void mainWidget::on_toolButton_save_clicked()
{
    if(!sqltm->submitAll())
    {
        QMessageBox::warning(this,"保存失败",sqltm->lastError().databaseText());
    }

    if(ui->toolButton_appointment_manage->isChecked())
    {
        ui->toolButton_save->setEnabled(false);
        ui->toolButton_cancel->setEnabled(false);

        sqltm->setTable("预约管理");
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        sqltm->select();
    }
}

void mainWidget::on_toolButton_cancel_clicked()
{
    sqltm->revertAll();

    if(ui->toolButton_appointment_manage->isChecked())
    {
        ui->toolButton_save->setEnabled(false);
        ui->toolButton_cancel->setEnabled(false);

        sqltm->setTable("预约管理");
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        sqltm->select();
    }
}

void mainWidget::on_toolButton_appoint_clicked()
{
    QString id = sqltm->data(sqltm->index(selsqltm->currentIndex().row(),3)).toString(); //获取选择居民的身份证号

    //转到预约管理
    ui->toolButton_appointment_manage->click();

    //切换表
    sqltm->setTable("预约记录管理");
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked |
                                   QAbstractItemView::EditKeyPressed |
                                   QAbstractItemView::AnyKeyPressed);

    //设置高级搜索
    ui->radioButton_advanced_search->click();

    //设置过滤器
    //sqltm->setFilter(id);

    //设置搜索词
    ui->lineEdit_id->setText(id);

    //搜索
    ui->toolButton_search->click();
    setDelegate();
    sqltm->select();

    //添加记录
    ui->toolButton_add->click();
    sqltm->setData(sqltm->index(selsqltm->currentIndex().row(),0),id);
}

void mainWidget::on_toolButton_complete_app_clicked()
{
    QString appointment_id = sqltm->data(sqltm->index(selsqltm->currentIndex().row(),0)).toString();

    VaccineTracCodeInput *tracecodeinput = new VaccineTracCodeInput(this);
    switch(tracecodeinput->exec())
    {
    case QDialog::Accepted:
    {
        QSqlQuery *sql_update = new QSqlQuery();
        sql_update->prepare("UPDATE `新冠疫苗接种`.`预约记录` SET `预约状态` = '完成预约' WHERE `预约编号` = :appointment_id");
        sql_update->bindValue(":appointment_id",appointment_id);

        if(!sql_update->exec())
        {
            QMessageBox::warning(this,"状态更改失败",sql_update->lastError().databaseText());
            sqltm->select();
            delete sql_update;
            break;
        }
        else
        {
            sqltm->select();
            delete sql_update;
        }


        QSqlQuery *sql_insert = new QSqlQuery();
        sql_insert->prepare("INSERT INTO `新冠疫苗接种`.`接种记录` (`预约编号`, `疫苗追溯码`) VALUES ("+appointment_id+", '"+tracecodeinput->gettracecode()+"')");
        //sql_insert->bindValue(0,appointment_id);
        //sql_insert->bindValue(1,);

//        if(!sql_insert->exec())
//        {
//            QMessageBox::warning(this,"写入接种信息失败",sql_insert->lastError().text());
//            sqltm->select();
//            delete sql_insert;
//            break;
//        }
        while(!sql_insert->exec())
        {
            QMessageBox::warning(this,"写入接种信息失败",sql_insert->lastError().text());
            tracecodeinput->exec();
            sql_insert->prepare("INSERT INTO `新冠疫苗接种`.`接种记录` (`预约编号`, `疫苗追溯码`) VALUES ("+appointment_id+", '"+tracecodeinput->gettracecode()+"')");
        }
        sqltm->select();
        delete sql_insert;
    }

    case QDialog::Rejected:
        break;
    }
    delete tracecodeinput;
}

void mainWidget::on_toolButton_cancel_app_clicked()
{
    QSqlQuery *sql = new QSqlQuery();
    sql->prepare("UPDATE `新冠疫苗接种`.`预约记录` SET `预约状态` = '取消预约' WHERE `预约编号` = :appointment_id");
    sql->bindValue(":appointment_id",sqltm->data(sqltm->index(selsqltm->currentIndex().row(),0)).toString());

    if(!sql->exec())
        QMessageBox::warning(this,"状态更改失败",sql->lastError().driverText());
    sqltm->select();
    delete sql;
}

void mainWidget::on_checkBox_resident_vaccination_information_stateChanged(int arg1)
{
    switch (arg1)
    {
    case Qt::Unchecked:
        ui->toolButton_add->setEnabled(true);
        ui->toolButton_del->setProperty("canenable",true);
        ui->toolButton_save->setEnabled(true);
        ui->toolButton_cancel->setEnabled(true);
        ui->toolButton_appoint->setEnabled(false);
        sqltm->setTable("居民管理");
        ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked |
                                       QAbstractItemView::EditKeyPressed |
                                       QAbstractItemView::AnyKeyPressed);
        setDelegate(); //设置表格代理
        sqltm->select();
        break;
    case Qt::Checked:
        ui->toolButton_add->setEnabled(false);
        ui->toolButton_del->setProperty("canenable",false);
        ui->toolButton_del->setEnabled(false);
        ui->toolButton_save->setEnabled(false);
        ui->toolButton_cancel->setEnabled(false);
        ui->toolButton_appoint->setEnabled(false);
        sqltm->setTable("居民接种信息");
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        sqltm->select();
        break;
    }
}

void mainWidget::on_radioButton_fuzzy_search_clicked(bool checked)
{
    if(checked)
    {
        ui->widget_advanced_search->setEnabled(false);
        ui->lineEdit_fuzzy_search_content->setEnabled(true);
    }
}

void mainWidget::on_radioButton_advanced_search_clicked(bool checked)
{
    if(checked)
    {
        ui->widget_advanced_search->setEnabled(true);
        ui->lineEdit_fuzzy_search_content->setEnabled(false);
    }
}

void mainWidget::selected()
{
    //判断是否有修改的数据未保存
    if(!selsqltm->selection().isEmpty() && selsqltm->currentIndex().row()!= lastsel.row() && sqltm->isDirty(lastsel))
    {
        //qDebug()<<selsqltm->currentIndex()<<";"<<selsqltm->selection().isEmpty();
        //QMessageBox::information(this,"未保存的修改","选项切换");
        switch(customMessageBox(QMessageBox::Warning, "未保存的修改",
                                "是否保存修改的内容？\t\t\n\n",
                                "保存", "不保存", "取消", this))
        {
        case QMessageBox::Yes:
            lastsel = selsqltm->currentIndex();
            ui->toolButton_save->click();
            break;
        case QMessageBox::No:
            lastsel = selsqltm->currentIndex();
            ui->toolButton_cancel->click();
            break;
        case QMessageBox::Cancel:
            selsqltm->setCurrentIndex(lastsel,QItemSelectionModel::SelectCurrent|QItemSelectionModel::Rows);
            break;
        }
    }
    else
    {
        if(!selsqltm->selection().isEmpty())
            lastsel = selsqltm->currentIndex();
    }

    int size = selsqltm->selectedRows().size();
    if(size < 1) //未选中
    {
        ui->toolButton_del->setEnabled(false);
        ui->toolButton_appoint->setEnabled(false);
        ui->toolButton_complete_app->setEnabled(false);
        ui->toolButton_cancel_app->setEnabled(false);
    }
    else
    {
        if(ui->toolButton_del->property("canenable").toBool())
            ui->toolButton_del->setEnabled(true);

        if(size > 1) //选中多个
        {
            ui->toolButton_appoint->setEnabled(false);
            ui->toolButton_complete_app->setEnabled(false);
        }
        else //只选中一个
        {
            if(ui->toolButton_appoint->property("canenable").toBool())
                ui->toolButton_appoint->setEnabled(true);
            if(sqltm->tableName() == "预约管理")
            {
                if(sqltm->data(sqltm->index(selsqltm->currentIndex().row(),11)).toString()=="预约成功")
                {
                    if(ui->toolButton_complete_app->property("canenable").toBool())
                        ui->toolButton_complete_app->setEnabled(true);
                    if(ui->toolButton_cancel_app->property("canenable").toBool())
                        ui->toolButton_cancel_app->setEnabled(true);
                }
                else
                {
                    ui->toolButton_complete_app->setEnabled(false);
                    ui->toolButton_cancel_app->setEnabled(false);
                }

            }
        }
    }
}

void mainWidget::on_toolButton_search_clicked()
{
    QRegularExpression *sqlregex = new QRegularExpression("(['\\\\])");
    QString *filter = new QString("1");

    if(ui->radioButton_advanced_search->isChecked()) //高级搜索
    {
        if(ui->toolButton_resident_manage->isChecked()) //居民管理
        {
            //姓名
            if(!ui->lineEdit_name->text().isEmpty())
                *filter = *filter + QString(" and `姓名`='%1'").arg(ui->lineEdit_name->text().replace(*sqlregex,"\\\\1"));

            //性别
            if(ui->checkBox_sex->checkState() == Qt::Checked)
                *filter = *filter + QString(" and `性别`='%1'").arg(ui->comboBox_sex->currentText());

            //年龄
            if(ui->checkBox_age->checkState() == Qt::Checked)
                *filter = *filter + QString(" and `年龄`=%1").arg(ui->spinBox_age->text());

            //身份证号
            if(!ui->lineEdit_id->text().isEmpty())
                *filter = *filter + QString(" and `身份证号`='%1'").arg(ui->lineEdit_id->text().replace(*sqlregex,"\\\\1"));

            //联系方式
            if(!ui->lineEdit_phone->text().isEmpty())
                *filter = *filter + QString(" and `联系方式`='%1'").arg(ui->lineEdit_phone->text().replace(*sqlregex,"\\\\1"));

            //现居地址
            if(!ui->lineEdit_address->text().isEmpty())
                *filter = *filter + QString(" and `现居地址`='%1'").arg(ui->lineEdit_address->text().replace(*sqlregex,"\\\\1"));

            //备注
            if(!ui->lineEdit_remark->text().isEmpty())
                *filter = *filter + QString(" and `备注`='%1'").arg(ui->lineEdit_remark->text().replace(*sqlregex,"\\\\1"));
        }
        else if(ui->toolButton_appointment_manage->isChecked()) //预约管理
        {
            //预约编号
            if(ui->checkBox_appointment_id->checkState() == Qt::Checked)
                *filter = *filter + QString(" and `预约编号`='%1'").arg(ui->spinBox_appointment_id->text());

            //姓名
            if(!ui->lineEdit_name->text().isEmpty())
                *filter = *filter + QString(" and `姓名`='%1'").arg(ui->lineEdit_name->text().replace(*sqlregex,"\\\\1"));

            //预约剂次
            if(ui->checkBox_vaccination_times->checkState() == Qt::Checked)
                *filter = *filter + QString(" and `预约剂次`=%1").arg(ui->comboBox_vaccination_times->currentText());

            //身份证号
            if(!ui->lineEdit_id->text().isEmpty())
                *filter = *filter + QString(" and `身份证号`='%1'").arg(ui->lineEdit_id->text().replace(*sqlregex,"\\\\1"));

            //预约疫苗编号
            if(ui->checkBox_vaccine_id->checkState() == Qt::Checked)
                *filter = *filter + QString(" and `预约疫苗编号`='%1'").arg(ui->spinBox_vaccine_id->text());

            //预约疫苗名称
            if(!ui->lineEdit_vaccine_name->text().isEmpty())
                *filter = *filter + QString(" and `预约疫苗名称`='%1'").arg(ui->lineEdit_vaccine_name->text().replace(*sqlregex,"\\\\1"));

            //预约接种点编号
            if(ui->checkBox_vaccination_site_id->checkState() == Qt::Checked)
                *filter = *filter + QString(" and `预约接种点编号`=%1").arg(ui->spinBox_vaccination_site_id->text());

            //预约接种点名称
            if(!ui->lineEdit_vaccination_site_name->text().isEmpty())
                *filter = *filter + QString(" and `预约接种点名称`='%1'").arg(ui->lineEdit_vaccination_site_name->text().replace(*sqlregex,"\\\\1"));

            //预约接种点地址
            if(!ui->lineEdit_vaccination_site_address->text().isEmpty())
                *filter = *filter + QString(" and `预约接种点地址`='%1'").arg(ui->lineEdit_vaccination_site_address->text().replace(*sqlregex,"\\\\1"));

            //预约时间
            if(ui->checkBox_appointment_datetime->checkState() == Qt::Checked)
                *filter = *filter + QString(" and `预约时间`='%1'").arg(ui->dateTimeEdit_appointment_datetime->text());

            //预约状态
            if(ui->checkBox_appointment_status->checkState() == Qt::Checked)
                *filter = *filter + QString(" and `预约状态`='%1'").arg(ui->comboBox_appointment_status->currentText());
        }
        else if(ui->toolButton_vaccination_information->isChecked()) //接种信息
        {
            //接种编号
            if(ui->checkBox_vaccination_site_id->checkState() == Qt::Checked)
                *filter = *filter + QString(" and `接种编号`='%1'").arg(ui->spinBox_vaccination_site_id->text());

            //姓名
            if(!ui->lineEdit_name->text().isEmpty())
                *filter = *filter + QString(" and `姓名`='%1'").arg(ui->lineEdit_name->text().replace(*sqlregex,"\\\\1"));

            //身份证号
            if(!ui->lineEdit_id->text().isEmpty())
                *filter = *filter + QString(" and `身份证号`='%1'").arg(ui->lineEdit_id->text().replace(*sqlregex,"\\\\1"));

            //接种疫苗编号
            if(ui->checkBox_vaccine_id->checkState() == Qt::Checked)
                *filter = *filter + QString(" and `接种疫苗编号`=%1").arg(ui->spinBox_vaccine_id->text());

            //接种疫苗名称
            if(!ui->lineEdit_vaccine_name->text().isEmpty())
                *filter = *filter + QString(" and `接种疫苗名称`='%1'").arg(ui->lineEdit_vaccine_name->text().replace(*sqlregex,"\\\\1"));

            //接种剂次
            if(ui->checkBox_vaccination_times->checkState() == Qt::Checked)
                *filter = *filter + QString(" and `接种剂次`=%1").arg(ui->comboBox_vaccination_times->currentText());

            //疫苗追溯码
            if(!ui->lineEdit_trace_code->text().isEmpty())
                *filter = *filter + QString(" and `疫苗追溯码`='%1'").arg(ui->lineEdit_trace_code->text());

            //接种点编号
            if(ui->checkBox_vaccination_site_id->checkState() == Qt::Checked)
                *filter = *filter + QString(" and `接种点编号`='%1'").arg(ui->spinBox_vaccination_site_id->text());

            //接种点名称
            if(!ui->lineEdit_vaccination_site_name->text().isEmpty())
                *filter = *filter + QString(" and `接种点名称`='%1'").arg(ui->lineEdit_vaccination_site_name->text().replace(*sqlregex,"\\\\1"));

            //接种日期
            if(ui->checkBox_vaccination_date->checkState() == Qt::Checked)
                *filter = *filter + QString(" and `预约状态`='%1'").arg(ui->dateEdit_vaccination_date->text());
        }
        else if(ui->toolButton_vaccination_site_manage->isChecked()) //接种点管理
        {
            //接种点编号
            if(ui->checkBox_vaccination_site_id->checkState() == Qt::Checked)
                *filter = *filter + QString(" and `接种点编号`='%1'").arg(ui->spinBox_vaccination_site_id->text());

            //接种点名称
            if(!ui->lineEdit_vaccination_site_name->text().isEmpty())
                *filter = *filter + QString(" and `接种点名称`='%1'").arg(ui->lineEdit_vaccination_site_name->text().replace(*sqlregex,"\\\\1"));

            //接种点地址
            if(!ui->lineEdit_vaccination_site_address->text().isEmpty())
                *filter = *filter + QString(" and `接种点地址`='%1'").arg(ui->lineEdit_vaccination_site_address->text().replace(*sqlregex,"\\\\1"));

            //接种开始时间
            if(ui->checkBox_vaccination_begin_time->checkState() == Qt::Checked)
                *filter = *filter + QString(" and `接种开始时间`='%1'").arg(ui->timeEdit_vaccination_begin_time->text());
            if(ui->checkBox_vaccination_begin_timerange->checkState() == Qt::Checked)
                *filter = *filter + QString(" and `接种开始时间` between '%1' and '%2'").arg(ui->timeEdit_vaccination_begin_timerange_begin->text())
                                                                                      .arg(ui->timeEdit_vaccination_begin_timerange_end->text());

            //接种结束时间
            if(ui->checkBox_vaccination_end_time->checkState() == Qt::Checked)
                *filter = *filter + QString(" and `接种结束时间`='%1'").arg(ui->timeEdit_vaccination_end_time->text());
            if(ui->checkBox_vaccination_end_timerange->checkState() == Qt::Checked)
                *filter = *filter + QString(" and `接种结束时间` between '%1' and '%2'").arg(ui->timeEdit_vaccination_end_timerange_begin->text())
                                                                                      .arg(ui->timeEdit_vaccination_end_timerange_end->text());
        }
        else if(ui->toolButton_vaccine_manage) //疫苗管理
        {
            //疫苗编号
            if(ui->checkBox_vaccine_id->checkState() == Qt::Checked)
                *filter = *filter + QString(" and `疫苗编号`='%1'").arg(ui->spinBox_vaccine_id->text());

            //疫苗名称
            if(!ui->lineEdit_vaccine_name->text().isEmpty())
                *filter = *filter + QString(" and `疫苗名称`='%1'").arg(ui->lineEdit_vaccine_name->text().replace(*sqlregex,"\\\\1"));

            //疫苗类型
            if(ui->checkBox_vaccine_type->checkState() == Qt::Checked)
                *filter = *filter + QString(" and `疫苗类型`='%1'").arg(ui->comboBox_vaccine_type->currentText());

            //生产企业
            if(!ui->lineEdit_production_company->text().isEmpty())
                *filter = *filter + QString(" and `生产企业`='%1'").arg(ui->lineEdit_production_company->text().replace(*sqlregex,"\\\\1"));

            //需接种剂次
            if(ui->checkBox_need_vaccination_times->checkState() == Qt::Checked)
                *filter = *filter + QString(" and `需接种剂次`='%1'").arg(ui->spinBox_need_vaccination_times->text());

            //疫苗说明
            if(!ui->lineEdit_vaccine_description->text().isEmpty())
                *filter = *filter + QString(" and `疫苗说明`='%1'").arg(ui->lineEdit_vaccine_description->text().replace(*sqlregex,"\\\\1"));
        }
    }
    else //模糊搜索
    {

    }

    sqltm->setFilter(*filter);

    delete sqlregex;
    delete filter;
}

void mainWidget::on_pushButton_reset_search_clicked()
{
    sqltm->setFilter(nullptr);
    sqltm->select();

    QList<QLineEdit *> btns_1 = ui->widget_search->findChildren<QLineEdit *>();
    foreach (QLineEdit *btn, btns_1)
        btn->clear();
    btns_1.clear();

    QList<QCheckBox *> btns_2= ui->scrollArea->findChildren<QCheckBox *>();
    foreach (QCheckBox *btn, btns_2)
        btn->setCheckState(Qt::Unchecked);
    btns_2.clear();
}

void mainWidget::on_checkBox_vaccination_begin_time_stateChanged(int arg1)
{
    if(arg1)
        ui->checkBox_vaccination_begin_timerange->setCheckState(Qt::Unchecked);
}

void mainWidget::on_checkBox_vaccination_begin_timerange_stateChanged(int arg1)
{
    if(arg1)
        ui->checkBox_vaccination_begin_time->setCheckState(Qt::Unchecked);
}

void mainWidget::on_checkBox_vaccination_end_time_stateChanged(int arg1)
{
    if(arg1)
        ui->checkBox_vaccination_end_timerange->setCheckState(Qt::Unchecked);
}

void mainWidget::on_checkBox_vaccination_end_timerange_stateChanged(int arg1)
{
    if(arg1)
        ui->checkBox_vaccination_end_time->setCheckState(Qt::Unchecked);
}
