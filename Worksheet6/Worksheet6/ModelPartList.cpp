#include "ModelPartList.h"

ModelPartList::ModelPartList(const QString &data, QObject *parent)
    : QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    rootData << data << "Visible";
    m_rootItem = new ModelPart(rootData);
}

ModelPartList::~ModelPartList()
{
    delete m_rootItem;
}

int ModelPartList::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<ModelPart*>(parent.internalPointer())->columnCount();
    return m_rootItem->columnCount();
}

QVariant ModelPartList::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    auto *item = static_cast<ModelPart*>(index.internalPointer());
    return item->data(index.column());
}

Qt::ItemFlags ModelPartList::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QVariant ModelPartList::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_rootItem->data(section);

    return QVariant();
}

QModelIndex ModelPartList::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    ModelPart *parentItem = nullptr;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<ModelPart*>(parent.internalPointer());

    ModelPart *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);

    return QModelIndex();
}

QModelIndex ModelPartList::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    auto *childItem = static_cast<ModelPart*>(index.internalPointer());
    ModelPart *parentItem = childItem->parentItem();

    if (parentItem == m_rootItem || parentItem == nullptr)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int ModelPartList::rowCount(const QModelIndex &parent) const
{
    ModelPart *parentItem = nullptr;

    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<ModelPart*>(parent.internalPointer());

    return parentItem->childCount();
}

bool ModelPartList::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    auto *item = static_cast<ModelPart*>(index.internalPointer());
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index, {role});

    return result;
}

ModelPart *ModelPartList::getRootItem()
{
    return m_rootItem;
}