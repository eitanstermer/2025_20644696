#pragma once

#include <QList>
#include <QVariant>
#include <QVector>
#include <QColor>

class ModelPart
{
public:
    explicit ModelPart(const QVector<QVariant> &data, ModelPart *parentItem = nullptr);
    ~ModelPart();

    void appendChild(ModelPart *child);

    ModelPart *child(int row);
    int childCount() const;
    int columnCount() const;

    QVariant data(int column) const;
    bool setData(int column, const QVariant &value);

    int row() const;
    ModelPart *parentItem();

    // ---- Convenience getters/setters for WS6 properties ----
    QString name() const;
    void setName(const QString &name);

    bool isVisible() const;
    void setIsVisible(bool visible);

    QColor colour() const;
    void setColour(const QColor &c);

private:
    QList<ModelPart*> m_childItems;
    QVector<QVariant> m_itemData;
    ModelPart *m_parentItem;

    // Stored separately so we don't add extra tree columns
    QColor m_colour;
};
