#pragma once

#include <QDialog>
#include <QModelIndex>

class QLabel;
class QLineEdit;
class CustomItemModel;
class DatabaseManager;

class OperatorEditorDialog : public QDialog
{
    Q_OBJECT
public:
    explicit OperatorEditorDialog(const QModelIndex& index = {},
                                  CustomItemModel *model = nullptr,
                                  QWidget* parent = nullptr);
public slots:
    void onSaveClicked();
    void loadIcon(const QString& mcc);

private:
    bool checkEmptyFields() const;
    QModelIndex m_index;
    CustomItemModel *m_model = nullptr;
    DatabaseManager& m_DatabaseManager;
    QLabel *m_nameOperator = nullptr;
    QLabel *m_mccLabel = nullptr;
    QLabel *m_mncLabel = nullptr;
    QLabel *m_iconCountry = nullptr;
    QLineEdit *m_nameEdit = nullptr;
    QLineEdit *m_mccEdit = nullptr;
    QLineEdit *m_mncEdit = nullptr;
    QPushButton *m_saveButton = nullptr;
    QPushButton *m_cancelButton = nullptr;
};

