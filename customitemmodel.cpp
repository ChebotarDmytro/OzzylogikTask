#include "customitemmodel.h"
#include <databasemanager.h>

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QIcon>

#include <QDebug>

//-------------------------CustomItem-----------------------------------------//

CustomItem::CustomItem(const InitItem &data)
    : m_imagePath(data.imagePath),
      m_fullName(data.fullName),
      m_name(data.name),
      m_mcc(data.mcc),
      m_mnc(data.mnc),
      m_parent(nullptr)
{

}

CustomItem::~CustomItem()
{
    qDeleteAll(children);
}

int CustomItem::row() const
{
    return m_parent
        ? m_parent->children.indexOf(const_cast<CustomItem*>(this))
        : 0;
}

CustomItem *CustomItem::childAt(int row) const
{
    return children.value(row);
}

int CustomItem::childCount() const
{
    return children.count();
}

CustomItem *CustomItem::parent() const
{
    return m_parent;
}

QString CustomItem::getImagePath() const
{
    return m_imagePath;
}

void CustomItem::setImagePath(const QString &imagePath)
{
    m_imagePath = imagePath;
}

QString CustomItem::getName() const
{
    return m_name;
}

void CustomItem::setName(const QString &name)
{
    m_name = name;
}

QString CustomItem::getMcc() const
{
    return m_mcc;
}

void CustomItem::setMcc(const QString &mcc)
{
    m_mcc = mcc;
}

QString CustomItem::getMnc() const
{
    return m_mnc;
}

void CustomItem::setMnc(const QString &mnc)
{
    m_mnc = mnc;
}

QString CustomItem::getFullName() const
{
    return m_fullName;
}

void CustomItem::setFullName(const QString &fullName)
{
    m_fullName = fullName;
}

//-------------------------CustomItemModel------------------------------------//

CustomItemModel::CustomItemModel(QObject* parent)
    : QAbstractItemModel(parent),
      rootItem(nullptr),
      m_DatabaseManager(DatabaseManager::instance())
{
    rootItem = new CustomItem;
    setupModel(rootItem);
}

CustomItemModel::~CustomItemModel()
{
    delete rootItem;
}

void CustomItemModel::setRoot(CustomItem* item)
{
    delete rootItem;
    rootItem = item;
}

QModelIndex CustomItemModel::index(int row, int column,
                                   const QModelIndex& parent) const
{
    auto parentItem = itemFromIndex(parent);
    Q_ASSERT(parentItem);
    if(auto item = parentItem->childAt(row))
        return createIndex( row, column, item );

    return {};
}

CustomItem* CustomItemModel::itemFromIndex(const QModelIndex& index) const
{
    if(index.isValid())
        if(auto item = static_cast<CustomItem*>(index.internalPointer()))
            return item;

    return rootItem;
}

int CustomItemModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return 0;

    auto parentItem = itemFromIndex(parent);
    return parentItem ? parentItem->childCount() : 0;
}

int CustomItemModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 1;
}

QModelIndex CustomItemModel::parent(const QModelIndex& index) const
{
    if(auto childItem = itemFromIndex(index))
    {
        if(auto parentItem = childItem->parent())
        {
            if(parentItem == rootItem)
                return {};

            return createIndex(parentItem->row(), 0, parentItem);
        }
    }

    return {};
}

QVariant CustomItemModel::data(const QModelIndex& index, int role) const
{
    if(!rootItem || !index.isValid() || index.column() < 0)
        return {};

    if(auto item = itemFromIndex(index))
    {
        if(role == Qt::DisplayRole)
            return item->getFullName();

        if(role == Qt::DecorationRole)
            return QIcon(item->getImagePath());

        if(role == CustomRole::Mcc)
            return item->getMcc();

        if(role == CustomRole::Mnc)
            return item->getMnc();

        if(role == CustomRole::Name)
            return item->getName();
    }

    return {};
}

bool CustomItemModel::setData(const QModelIndex &index, const QVariant &value,
                              int role)
{
    if(!index.isValid())
        return false;

    if(auto item = itemFromIndex(index))
    {
        if(role == Qt::EditRole)
            item->setFullName(value.toString());
        else if(role == CustomRole::Name)
            item->setName(value.toString());
        else return false;
    }

    emit dataChanged(index, index);
    return true;
}

QVariant CustomItemModel::headerData(int section,
                                     Qt::Orientation orientation,
                                     int role) const
{
    if(section == 0 && orientation == Qt::Horizontal && role == Qt::DisplayRole)
    return "Countries";

    return {};
}

Qt::ItemFlags CustomItemModel::flags(const QModelIndex& index) const
{
    return index.isValid()
           ? Qt::ItemIsEnabled | Qt::ItemIsSelectable
           : Qt::ItemIsEnabled;
}

void CustomItemModel::setupModel(CustomItem* parent)
{
    if(!parent || !parent->children.isEmpty())
        return;

    QList<CustomItem*> parents;
    QList<int> levels;
    parents << parent;
    levels << 0;

    auto queryCountries = m_DatabaseManager.countries().readData();
    int countryIndex = queryCountries.record().indexOf("Name");
    int mccIndex = queryCountries.record().indexOf("Mcc");
    int iconCountryIndex = queryCountries.record().indexOf("Code");

    int level = 0;
    CustomItem::InitItem data;
    QString iconCountry;
    QString nameCountry;
    QString nameOperator;
    QString iconOperator;
    QSqlQuery queryOperators;

    while(queryCountries.next())
    {
        if(level > levels.last())
        {
            if(parents.last()->children.count() > 0)
            {
                auto mcc = queryCountries.value(mccIndex).toInt();
                queryOperators = m_DatabaseManager.operators().readData(mcc);

                parents << parents.last()->children.value(
                               parents.last()->children
                               .count() - 1);
                levels << level;
            }
        }

        if(queryOperators.isSelect())
        {
            while (queryOperators.next())
            {
                data.mcc = queryOperators.value(0).toString();
                data.mnc = queryOperators.value(1).toString();
                data.fullName = QString("%1 (%2, %3)")
                        .arg(queryOperators.value(2).toString())
                        .arg(data.mcc).arg(data.mnc);
                data.name = queryOperators.value(2).toString();

                data.imagePath = QString(":/icons/Operators/%1_%2.png")
                        .arg(data.mcc).arg(data.mnc);

                auto item = new CustomItem(data);
                item->m_parent = parents.last();
                parents.last()->children.append(item);
            }
            parents.pop_back();
            levels.pop_back();
            queryOperators.clear();
            --level;
        }
        else
        {
            data.fullName = queryCountries.value(countryIndex).toString();
            data.imagePath = QString(":/icons/Countries/%1.png")
                    .arg(queryCountries.value(iconCountryIndex).toString());
            auto item = new CustomItem(data);
            item->m_parent = parents.last();
            parents.last()->children.append(item);
            ++level;
        }
    }
}

void CustomItemModel::updateModel()
{
    auto newRoot = new CustomItem;
    setRoot(newRoot);
    setupModel(newRoot);
}
