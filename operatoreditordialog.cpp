#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QApplication>
#include <QStyle>
#include <QAction>
#include <QGridLayout>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>

#include "operatoreditordialog.h"
#include "customitemmodel.h"
#include "databasemanager.h"

OperatorEditorDialog::OperatorEditorDialog(const QModelIndex &index,
                                           CustomItemModel *model,
                                           QWidget *parent)
    : QDialog(parent),
      m_index(index),
      m_model(model),
      m_DatabaseManager(DatabaseManager::instance())
{
    auto style = qApp->style();
    setWindowTitle("Operator editor");
    setWindowIcon(index.isValid()
                  ? index.data(Qt::DecorationRole).value<QIcon>()
                  : style->standardIcon(QStyle::SP_MessageBoxQuestion));

    m_nameEdit = new QLineEdit(index.isValid()
                               ? index.data(CustomItemModel::Name).toString()
                               : "");
    auto closeIcon = style->standardIcon(QStyle::SP_DialogCloseButton);
    auto closeAction = m_nameEdit->addAction(closeIcon,
                                             QLineEdit::TrailingPosition);
    connect(closeAction, &QAction::triggered,
            m_nameEdit, &QLineEdit::clear);
    connect(m_nameEdit, &QLineEdit::textEdited, [this](const QString& /*text*/)
    {
        m_saveButton->setEnabled(checkEmptyFields());
    });

    m_nameOperator = new QLabel("Name");
    m_nameOperator->setBuddy(m_nameEdit);

    m_mccEdit = new QLineEdit(index.isValid()
                              ? index.data(CustomItemModel::Mcc).toString()
                              : "");
    m_mccEdit->setEnabled(index.isValid() ? false : true);
    connect(m_mccEdit, &QLineEdit::textEdited, [this](const QString& mcc)
    {
        loadIcon(mcc);
        m_saveButton->setEnabled(checkEmptyFields());
    });
    m_mccLabel = new QLabel("Mcc");
    m_mccLabel->setBuddy(m_mccEdit);

    m_iconCountry = new QLabel;

    if(index.isValid())
    {
        auto item = static_cast<CustomItem*>(index.parent().internalPointer());
        m_iconCountry->setPixmap(QPixmap(item->getImagePath()));
    }
    else
    {
        m_iconCountry->setPixmap(style->standardPixmap(
                                     QStyle::SP_MessageBoxQuestion));
    }

    m_mncEdit = new QLineEdit(index.isValid()
                              ? index.data(CustomItemModel::Mnc).toString()
                              : "");
    m_mncEdit->setEnabled(index.isValid() ? false : true);
    connect(m_mncEdit, &QLineEdit::textEdited, [this](const QString& /*text*/)
    {
        m_saveButton->setEnabled(checkEmptyFields());
    });
    m_mncLabel = new QLabel("Mnc");
    m_mncLabel->setBuddy(m_mncEdit);

    m_saveButton = new QPushButton("Save");
    m_saveButton->setEnabled(checkEmptyFields());
    connect(m_saveButton, &QPushButton::clicked,
            this, &OperatorEditorDialog::onSaveClicked);

    m_cancelButton = new QPushButton("Cancel");
    connect(m_cancelButton, &QPushButton::clicked,
            this, &OperatorEditorDialog::close);

    auto layout = new QGridLayout;
    layout->addWidget(m_nameOperator, 0, 0);
    layout->addWidget(m_nameEdit, 0, 1);
    layout->addWidget(m_mccLabel, 1, 0);
    layout->addWidget(m_mccEdit, 1, 1);
    layout->addWidget(m_iconCountry, 1, 2);
    layout->addWidget(m_mncLabel, 2, 0);
    layout->addWidget(m_mncEdit, 2, 1);

    auto buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(m_saveButton);
    buttonsLayout->addWidget(m_cancelButton);
    layout->addLayout(buttonsLayout, 3, 1);

    setLayout(layout);
}

void OperatorEditorDialog::onSaveClicked()
{
    auto fullName = QString("%1 (%2, %3)").arg(m_nameEdit->text())
                                          .arg(m_mccEdit->text())
                                          .arg(m_mncEdit->text());

    if(m_index.isValid())
    {
        m_model->setData(m_index, fullName);
        m_model->setData(m_index, m_nameEdit->text(), CustomItemModel::Name);

        m_DatabaseManager.operators().updateData(m_index);
    }
    else
    {
        m_DatabaseManager.operators().insertData({m_mccEdit->text(),
                                                  m_mncEdit->text(),
                                                  m_nameEdit->text()});
        // TODO don`t work
//        m_model->updateModel();
    }

    close();
}

void OperatorEditorDialog::loadIcon(const QString &mcc)
{
    auto query = m_DatabaseManager.countries().readData(mcc);
    query.first();

    if(query.isValid())
    {
        int codeIndex = query.record().indexOf("Code");

        if(codeIndex != -1)
        {
            auto code = query.value(codeIndex).toString();
            auto iconPath = QString(":/icons/Countries/%1.png").arg(code);
            auto icon = QPixmap(iconPath);

            if(!icon.isNull())
            {
                m_iconCountry->setPixmap(icon);
                setWindowIcon(QIcon(iconPath));
            }

            return;
        }
    }

    m_iconCountry->setPixmap(qApp->style()->standardPixmap(
                                 QStyle::SP_MessageBoxQuestion));
    setWindowIcon(qApp->style()->standardIcon(
                      QStyle::SP_MessageBoxQuestion));
}

bool OperatorEditorDialog::checkEmptyFields() const
{
    return (!m_nameEdit->text().isEmpty() &&
            !m_mccEdit->text().isEmpty() &&
            !m_mncEdit->text().isEmpty());
}
