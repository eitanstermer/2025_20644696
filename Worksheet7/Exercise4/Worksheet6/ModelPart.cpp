#include "ModelPart.h"

#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkSTLReader.h>

ModelPart::ModelPart(const QList<QVariant>& data, ModelPart* parentItem)
    : m_itemData(data)
    , m_parentItem(parentItem)
{
}

ModelPart::~ModelPart()
{
    qDeleteAll(m_childItems);
}

void ModelPart::appendChild(ModelPart* child)
{
    m_childItems.append(child);
}

ModelPart* ModelPart::child(int row)
{
    return m_childItems.value(row);
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
    return m_itemData.value(column);
}

bool ModelPart::setData(int column, const QVariant& value)
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

ModelPart* ModelPart::parentItem()
{
    return m_parentItem;
}

QString ModelPart::name() const
{
    return m_itemData.value(0).toString();
}

bool ModelPart::isVisible() const
{
    return m_itemData.value(1).toBool();
}

void ModelPart::setName(const QString& name)
{
    if (m_itemData.size() > 0)
        m_itemData[0] = name;
}

void ModelPart::setIsVisible(bool visible)
{
    if (m_itemData.size() > 1)
        m_itemData[1] = visible;
}

void ModelPart::setColour(const QColor& colour)
{
    m_colour = colour;
}

QColor ModelPart::colour() const
{
    return m_colour;
}

// ================= Worksheet 7 Exercise 4 =================

void ModelPart::loadSTL(const QString& fileName)
{
    // STL reader
    stlReader = vtkSmartPointer<vtkSTLReader>::New();
    stlReader->SetFileName(fileName.toStdString().c_str());
    stlReader->Update();

    // Mapper
    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(stlReader->GetOutputPort());

    // Actor
    actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
}

vtkSmartPointer<vtkActor> ModelPart::getActor() const
{
    return actor;
}
