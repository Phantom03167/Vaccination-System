#ifndef DATETIMEEDITDELEGATE_H
#define DATETIMEEDITDELEGATE_H

#include <QStyledItemDelegate>
#include <QDateTime>

class DatetimeEditDelegate : public QStyledItemDelegate
{
public:
    explicit DatetimeEditDelegate(QObject *parent = nullptr);
    void setdatetimeformat(QString f);
    void setminidatetime(QDateTime dt);
    void setcalendar(bool c);

    // editing
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

private:
    QString datetimeformat = "yyyy-MM-dd HH:mm";
    QDateTime minidatetime = QDateTime();
    bool calendar = true;
};

#endif // DATETIMEEDITDELEGATE_H
