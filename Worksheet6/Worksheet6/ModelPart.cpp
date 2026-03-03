#include "ModelPart.h"

ModelPart::ModelPart(const QVector<QVariant> &data, ModelPart *parentItem)
    : m_itemData(data)
    , m_parentItem(parentItem)
    , m_colour(200, 200, 200) // default grey
{
}

ModelPart::~ModelPart()
{
    qDeleteAll(m_childItems);
}

void ModelPart::appendChild(ModelPart *child)
{
    m_childItems.append(child);
}

ModelPart *ModelPart::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int ModelPart::childCount() const
{
    return m_childItems.count();
}

int ModelPart::columnCount() const
{
    return m_itemData.count();
}

QVariant ModelPart::data(int column) const
{
    if (column < 0 || column >= m_itemData.size())
        return QVariant();
    return m_itemData.at(column);
}

bool ModelPart::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= m_itemData.size())
        return false;

    m_itemData[column] = value;
    return true;
}

int ModelPart::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<ModelPart*>(this));
    return 0;
}

ModelPart *ModelPart::parentItem()
{
    return m_parentItem;
}

// ---- Convenience property helpers ----
QString ModelPart::name() const
{
    return data(0).toString();
}

void ModelPart::setName(const QString &name)
{
    setData(0, name);
}

bool ModelPart::isVisible() const
{
    // If old data was "true"/"false" string, toBool() still works fine:
    return data(1).toBool();
}

void ModelPart::setIsVisible(bool visible)
{
    setData(1, visible);
}

QColor ModelPart::colour() const
{
    return m_colour;
}

void ModelPart::setColour(const QColor &c)
{
    m_colour = c;
}
