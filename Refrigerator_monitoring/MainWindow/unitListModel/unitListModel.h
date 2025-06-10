#ifndef UNITLISTMODEL_H
#define UNITLISTMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QList>

class unitModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit unitModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void addItem(const QString &item, const int& sensor_id);
    int sensor_index(const QModelIndex& index);
    void clear();

private:
    QStringList m_items;
    QList<int> m_sensors_id;
};
#endif // UNITLISTMODEL_H
