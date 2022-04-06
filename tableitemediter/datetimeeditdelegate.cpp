#include "datetimeeditdelegate.h"
#include <QDateTimeEdit>

DatetimeEditDelegate::DatetimeEditDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void DatetimeEditDelegate::setdatetimeformat(QString f)
{
    datetimeformat = f;
}

void DatetimeEditDelegate::setminidatetime(QDateTime dt)
{
    minidatetime = dt;
}

void DatetimeEditDelegate::setcalendar(bool c)
{
    calendar = c;
}

QWidget *DatetimeEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QDateTimeEdit *editor = new QDateTimeEdit(parent);
    editor->setDisplayFormat(datetimeformat);
    if(minidatetime.isValid())
        editor->setMinimumDateTime(minidatetime);
    editor->setCalendarPopup(calendar);
    //editor->setFrame(false);
    return editor;
}

void DatetimeEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QDateTime datetime = index.model()->data(index,Qt::EditRole).toDateTime();
    if(datetime.time().minute()!=0)
        datetime.setTime(QTime(datetime.time().hour(),0));
    QDateTimeEdit *d = static_cast<QDateTimeEdit *>(editor);
    d->setDateTime(datetime);
}

void DatetimeEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDateTimeEdit *d = static_cast<QDateTimeEdit *>(editor);
    //QString str = d->text();
    if(d->displayFormat().length()>5)
    {
        QDateTime datetime = d->dateTime();
        if(datetime.time().minute()!=0)
            datetime.setTime(QTime(datetime.time().hour(),0));
        model->setData(index,datetime,Qt::EditRole);
    }
    else
    {
        QDateTime datetime = d->dateTime();
        if(datetime.time().minute()!=0)
            datetime.setTime(QTime(datetime.time().hour(),0));
        model->setData(index,datetime.time(),Qt::EditRole);
    }

}

void DatetimeEditDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
