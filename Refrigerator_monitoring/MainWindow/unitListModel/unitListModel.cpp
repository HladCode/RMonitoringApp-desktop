#include "unitListModel.h"

unitModel::unitModel(QObject *parent)
    : QAbstractListModel(parent) {}

int unitModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return m_items.count();
}

QVariant unitModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_items.size())
        return QVariant();

    if (role == Qt::DisplayRole)
        return m_items.at(index.row());

    return QVariant();
}

Qt::ItemFlags unitModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void unitModel::addItem(const QString &item) {
    beginInsertRows(QModelIndex(), m_items.count(), m_items.count());
    m_items.append(item);
    endInsertRows();
}

void unitModel::clear() {
    if (m_items.isEmpty())
        return;

    beginResetModel();
    m_items.clear();
    endResetModel();
}
