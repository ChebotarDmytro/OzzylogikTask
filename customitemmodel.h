#pragma once

#include <QAbstractItemModel>
#include <QList>
#include <QString>

class CustomItem
{
public:
    struct InitItem
    {
        QString imagePath;
        QString fullName;
        QString name;
        QString mcc;
        QString mnc;
    };

    CustomItem(const InitItem& data = {});
    ~CustomItem();

    int row() const;
    CustomItem *childAt(int row) const;
    int childCount() const;
    CustomItem *parent() const;

    QString getImagePath() const;
    void setImagePath(const QString &imagePath);

    QString getName() const;
    void setName(const QString &name);

    QString getMcc() const;
    void setMcc(const QString &mcc);

    QString getMnc() const;
    void setMnc(const QString &mnc);

    QString getFullName() const;
    void setFullName(const QString &fullName);

protected:
    QString m_imagePath;
    QString m_fullName;
    QString m_name;
    QString m_mcc;
    QString m_mnc;

private:
    CustomItem* m_parent;
    QList<CustomItem*> children;

    friend class CustomItemModel;
};

class QSqlQuery;
class DatabaseManager;

class CustomItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum CustomRole
    {
        Mcc = Qt::UserRole,
        Mnc,
        Name
    };

    CustomItemModel(QObject* parent = nullptr);
    ~CustomItemModel() override;

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
             int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
               const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    void setRoot(CustomItem* item);
    CustomItem* itemFromIndex(const QModelIndex& index) const;
    void updateModel();
private:
    void setupModel(CustomItem* parent);
    CustomItem* rootItem;

    DatabaseManager &m_DatabaseManager;
};
