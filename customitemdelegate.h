#pragma once

#include <QStyledItemDelegate>

class CustomItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CustomItemDelegate(QObject* parent = nullptr);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option,
                     const QModelIndex &index) override;
signals:
    void closeButtonClick(int mcc, int mnc);
    void itemDoubleClick(const QModelIndex &index);

private:
    QPoint closeIconPosition(const QStyleOptionViewItem &option,
                             const QModelIndex &index) const;
    void closeButtonEvent(QEvent *event, const QStyleOptionViewItem &option,
                     const QModelIndex &index);
    QPixmap m_closeIcon;

    static const int m_margins = 35;

    Q_DISABLE_COPY(CustomItemDelegate)
};
