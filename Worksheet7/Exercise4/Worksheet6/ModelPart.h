#pragma once

#include <QList>
#include <QVariant>
#include <QString>
#include <QColor>

#include <vtkSmartPointer.h>

class vtkActor;
class vtkPolyDataMapper;
class vtkSTLReader;

class ModelPart
{
public:
    ModelPart(const QList<QVariant>& data, ModelPart* parentItem = nullptr);
    ~ModelPart();

    void appendChild(ModelPart* child);

    ModelPart* child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    ModelPart* parentItem();

    bool setData(int column, const QVariant& value);

    QString name() const;
    bool isVisible() const;

    // ===== Functions required by OptionDialog =====
    void setName(const QString& name);
    void setColour(const QColor& colour);
    void setIsVisible(bool visible);

    QColor colour() const;

    // ===== Worksheet 7 Exercise 4 =====
    void loadSTL(const QString& fileName);
    vtkSmartPointer<vtkActor> getActor() const;

private:
    QList<ModelPart*> m_childItems;
    QList<QVariant> m_itemData;
    ModelPart* m_parentItem;

    QColor m_colour;

    vtkSmartPointer<vtkSTLReader> stlReader;
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkActor> actor;
};
