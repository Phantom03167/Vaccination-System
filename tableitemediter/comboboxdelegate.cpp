#include "comboboxdelegate.h"
#include <QComboBox>

ComboBoxDelegate::ComboBoxDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void ComboBoxDelegate::setitems(QStringList items)
{
    itemlist = QStringList(items);
}

void ComboBoxDelegate::seteditable(bool r)
{
    editable = r;
}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QComboBox *editor = new QComboBox(parent);
//    int n = itemlist.size();
//    for(int i = 0; i < n; i++)
//        editor->addItem(itemlist.at(i));

    editor->addItems(itemlist);
    editor->setEditable(editable);
    //editor->setFrame(false);
    return editor;
}

void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString str = index.model()->data(index,Qt::EditRole).toString();
    QComboBox *c = static_cast<QComboBox *>(editor);
    c->setCurrentText(str);
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *c = static_cast<QComboBox *>(editor);
    QString str = c->currentText();
    model->setData(index,str,Qt::EditRole);
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

