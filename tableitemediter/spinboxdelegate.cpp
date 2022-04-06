#include "spinboxdelegate.h"
#include <QSpinBox>

SpinBoxDelegate::SpinBoxDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void SpinBoxDelegate::setminnum(int min)
{
    minnum = min;
}

void SpinBoxDelegate::setmaxnum(int max)
{
    maxnum = max;
}

void SpinBoxDelegate::setnumrange(int min, int max)
{
    minnum = min;
    maxnum = max;
}

void SpinBoxDelegate::setsuffix(QString s)
{
    suffix = s;
}

QWidget *SpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QSpinBox *editor = new QSpinBox(parent);
    editor->setMinimum(minnum);
    editor->setMaximum(maxnum);
    editor->setSuffix(suffix);
    //editor->setFrame(false);
    return editor;
}

void SpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int n = index.model()->data(index,Qt::EditRole).toInt();
    QSpinBox *s = static_cast<QSpinBox *>(editor);
    s->setValue(n);
}

void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox *s = static_cast<QSpinBox *>(editor);
    int n = s->value();
    model->setData(index,n,Qt::EditRole);
}

void SpinBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
