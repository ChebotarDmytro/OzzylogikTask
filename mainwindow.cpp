#include <QTreeView>
#include <QPushButton>
#include <QDebug>

#include "customitemmodel.h"
#include "customitemdelegate.h"
#include "operatoreditordialog.h"

#include "mainwindow.h"

static constexpr int SCREEN_WIDTH = 800;
static constexpr int SCREEN_HEIGHT = SCREEN_WIDTH;
static constexpr int MARGIN = 6;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(SCREEN_WIDTH, SCREEN_HEIGHT);

    m_treeView = new QTreeView;
    m_itemModel = new CustomItemModel(m_treeView);
    m_itemDelegate = new CustomItemDelegate(m_treeView);
    m_treeView->setItemDelegate(m_itemDelegate);
    m_treeView->setModel(m_itemModel);
    m_treeView->viewport()->setAttribute(Qt::WA_Hover);

    m_newItem = new QPushButton(QIcon{":/icons/add-new-64.png"}, {}, m_treeView);
    m_newItem->setIconSize({64, 64});
    m_newItem->setFixedSize(64, 64);
    m_newItem->setStyleSheet("border: 0px");
    m_newItem->move(this->width() - m_newItem->width() - MARGIN * 3,
                    this->height() - m_newItem->height() - MARGIN);

    connect(m_newItem, &QPushButton::clicked, [this]
    {
        m_editorDialog = new OperatorEditorDialog({}, m_itemModel);
        m_editorDialog->exec();
    });

    connect(m_itemDelegate, &CustomItemDelegate::closeButtonClick,
            [](int /*mcc*/, int /*mnc*/)
    {

    });

    connect(m_itemDelegate, &CustomItemDelegate::itemDoubleClick,
            [this](const QModelIndex& index)
    {
        m_editorDialog = new OperatorEditorDialog(index, m_itemModel);
        m_editorDialog->exec();
    });

    setCentralWidget(m_treeView);
}

MainWindow::~MainWindow()
{

}
