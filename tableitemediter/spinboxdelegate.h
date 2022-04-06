#ifndef SPINBOXDELEGATE_H
#define SPINBOXDELEGATE_H

#include <QStyledItemDelegate>

class SpinBoxDelegate : public QStyledItemDelegate
{
public:
    explicit SpinBoxDelegate(QObject *parent = nullptr);
    void setminnum(int min);
    void setmaxnum(int max);
    void setnumrange(int min, int max);
    void setsuffix(QString s);

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
    int minnum = INT_MIN;
    int maxnum = INT_MAX;
    QString suffix = nullptr;
};

#endif // SPINBOXDELEGATE_H
