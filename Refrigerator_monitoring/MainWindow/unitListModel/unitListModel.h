#ifndef UNITLISTMODEL_H
#define UNITLISTMODEL_H

#include <QAbstractListModel>
#include <QStringList>

class unitModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit unitModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void addItem(const QString &item);
    void clear();

private:
    QStringList m_items;
};
#endif // UNITLISTMODEL_H
