#ifndef LINEEDITDELEGATE_H
#define LINEEDITDELEGATE_H

#include <QStyledItemDelegate>

class LineEditDelegate : public QStyledItemDelegate
{
public:
    explicit LineEditDelegate(QObject *parent = nullptr);
    void setreadOnly(bool r);
    void setmaxlength(int m);
    void setvalidator(QString r);

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
    bool readonly = false;
    int maxlength = 32767;
    QRegularExpression validator = QRegularExpression();
};

#endif // LINEEDITDELEGATE_H
