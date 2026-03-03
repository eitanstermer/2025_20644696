#include "ModelPartList.h"

#include <QStringList>

ModelPartList::ModelPartList(const QString& data, QObject *parent)
    : QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    rootData << data << "Visible";

    rootItem = new ModelPart(rootData);
}

ModelPartList::~ModelPartList()
{
    delete rootItem;
}

ModelPart* ModelPartList::getRootItem() const
{
    return rootItem;
}

int ModelPartList::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        ModelPart* parentItem = static_cast<ModelPart*>(parent.internalPointer());
        return parentItem->columnCount();
    }
    return rootItem->columnCount();
}

int ModelPartList::rowCount(const QModelIndex& parent) const
{
    ModelPart* parentItem;

    if (!parent.isValid()) {
        parentItem = rootItem;
    } else {
        parentItem = static_cast<ModelPart*>(parent.internalPointer());
    }

    return parentItem->childCount();
}

QModelIndex ModelPartList::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    ModelPart* parentItem;

    if (!parent.isValid()) {
        parentItem = rootItem;
    } else {
        parentItem = static_cast<ModelPart*>(parent.internalPointer());
    }

    ModelPart* childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);

    return QModelIndex();
}

QModelIndex ModelPartList::parent(const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();

    ModelPart* childItem = static_cast<ModelPart*>(index.internalPointer());
    ModelPart* parentItem = childItem->parentItem();

    if (parentItem == rootItem || parentItem == nullptr)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

QVariant ModelPartList::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    ModelPart* item = static_cast<ModelPart*>(index.internalPointer());

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        // Column 0: Name
        if (index.column() == 0) {
            return item->data(0);
        }

        // Column 1: Visible (show as "true"/"false")
        if (index.column() == 1) {
            bool v = item->data(1).toBool();
            return v ? "true" : "false";
        }
    }

    return QVariant();
}

bool ModelPartList::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid())
        return false;

    if (role != Qt::EditRole)
        return false;

    ModelPart* item = static_cast<ModelPart*>(index.internalPointer());
    bool ok = false;

    // Column 0: Name
    if (index.column() == 0) {
        ok = item->setData(0, value.toString());
    }

    // Column 1: Visible
    if (index.column() == 1) {
        ok = item->setData(1, value.toBool());
    }

    if (ok) {
        emit dataChanged(index, index, { Qt::DisplayRole, Qt::EditRole });
    }

    return ok;
}

Qt::ItemFlags ModelPartList::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags f = QAbstractItemModel::flags(index);

    // Allow editing of both columns (needed for setData to be used properly)
    if (index.column() == 0 || index.column() == 1) {
        f |= Qt::ItemIsEditable;
    }

    return f;
}

QVariant ModelPartList::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return rootItem->data(section);
    }

    return QVariant();
}
