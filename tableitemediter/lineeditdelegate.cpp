#include "lineeditdelegate.h"
#include <QLineEdit>

LineEditDelegate::LineEditDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}
void LineEditDelegate::setreadOnly(bool r)
{
    readonly = r;
}

void LineEditDelegate::setmaxlength(int m)
{
    maxlength = m;
}


void LineEditDelegate::setvalidator(QString r)
{
    validator = QRegularExpression(r);
}

QWidget *LineEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QLineEdit *editor = new QLineEdit(parent);
    editor->setReadOnly(readonly);
    editor->setMaxLength(maxlength);
    editor->setValidator(new QRegularExpressionValidator(validator));
    //editor->setFrame(false);
    return editor;
}

void LineEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString str = index.model()->data(index,Qt::EditRole).toString();
    QLineEdit *l = static_cast<QLineEdit *>(editor);
    l->setText(str);
}

void LineEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *l = static_cast<QLineEdit *>(editor);
    QString str = l->text();
    model->setData(index,str,Qt::EditRole);
}

void LineEditDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
