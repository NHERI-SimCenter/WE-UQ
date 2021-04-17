#ifndef REMOTECASESELECTOR_CPP
#define REMOTECASESELECTOR_CPP

#include <RemoteService.h>
#include <QDialog>
#include <QListWidget>
#include <QComboBox>
#include <QPushButton>

class RemoteCaseSelector : public QDialog
{
    Q_OBJECT
public:
    explicit RemoteCaseSelector(RemoteService* remoteService, QWidget *parent = nullptr);

signals:
    void caseSelected(const QString& remotePath);

public slots:
    void directoryClicked(const QModelIndex& index);
    void onStorageChanged(const QString& storageName);
    void onSelectButtonClicked();
    QString getSelectedCase();

private:
    RemoteService* agaveService;
    QListWidget* dirList;
    QJsonArray currentDirectories;
    QString currentPath;
    QComboBox* storageComboBox;
    QPushButton* selectButton;

    QString selectedCase;

    void setupConnections();
    void listDirectories(QJsonArray directories);
    QString GetRemotePath(QJsonObject& dirJson);

};


#endif // REMOTECASESELECTOR_CPP
