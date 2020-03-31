#pragma once

#include <QMainWindow>

class QPushButton;
class QTreeView;
class CustomItemModel;
class CustomItemDelegate;
class OperatorEditorDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QPushButton *m_newItem = nullptr;
    QTreeView *m_treeView = nullptr;
    CustomItemModel *m_itemModel = nullptr;
    CustomItemDelegate *m_itemDelegate = nullptr;
    OperatorEditorDialog *m_editorDialog = nullptr;
};
