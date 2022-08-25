#include "RemoteCaseSelector.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QFormLayout>
#include <QComboBox>

const QString PARENT_DIR_LABEL = QObject::tr("(Up to parent directory)");

RemoteCaseSelector::RemoteCaseSelector(RemoteService* remoteService, QWidget *parent) :
    QDialog(parent), agaveService(remoteService)
{
  qDebug() << "RemoteCaseSelector::constructor()";
  
    setWindowTitle("Select remote OpenFOAM case");
    QVBoxLayout* layout = new QVBoxLayout(this);

    auto formLayout = new QFormLayout();
    storageComboBox = new QComboBox(this);
    storageComboBox->addItem("Default");
    storageComboBox->addItem("Community");
    formLayout->addRow("Cloud Storage System", storageComboBox);
    layout->addLayout(formLayout);

    dirList = new QListWidget(this);
    dirList->setMouseTracking(true);
    layout->addWidget(dirList);

    selectButton = new QPushButton("Select Case");
    selectButton->setToolTip(tr("Select Current Directory as the OpenFOAM Case Directory"));
    selectButton->setStyleSheet("QPushButton:disabled {background-color: #C7C6C6}");
    layout->addWidget(selectButton);

    if(parent)
        this->resize(parent->width()/2, parent->height()/2);

    setupConnections();
    remoteService->remoteLSCall("");
}

void RemoteCaseSelector::directoryClicked(const QModelIndex& index)
{
    auto dirName = dirList->model()->data(index).toString();

    if(0 == dirName.compare(PARENT_DIR_LABEL))
    {
        currentPath = currentPath.left(currentPath.lastIndexOf('/') + 1);
        agaveService->remoteLSCall(currentPath);
        return;
    }

    for(auto directory: currentDirectories)
    {
        auto directoryJson = directory.toObject();
        if(0 == directoryJson["name"].toString().compare(dirName))
        {
            currentPath = GetRemotePath(directoryJson);
            agaveService->remoteLSCall(currentPath);
            break;
        }
    }

}

void RemoteCaseSelector::onStorageChanged(const QString &storageName)
{
    if(0 == storageName.compare("Default"))
        agaveService->remoteLSCall("");
    else if(0 == storageName.compare("Community"))
        agaveService->remoteLSCall("system/designsafe.storage.community");
}

void RemoteCaseSelector::onSelectButtonClicked()
{
    selectedCase = currentPath;
    if(currentPath.startsWith("system/"))
        selectedCase = "agave://" + selectedCase.remove("system/");
    else
        selectedCase = "agave://designsafe.storage.default" + selectedCase;

    this->accept();
}

QString RemoteCaseSelector::getSelectedCase()
{
    return selectedCase;
}

void RemoteCaseSelector::setupConnections()
{
    connect(agaveService, &RemoteService::remoteLSReturn, this, &RemoteCaseSelector::listDirectories);

    connect(dirList, &QListWidget::doubleClicked, this, &RemoteCaseSelector::directoryClicked);

    connect(storageComboBox, &QComboBox::currentTextChanged, this, &RemoteCaseSelector::onStorageChanged);

    connect(selectButton, &QPushButton::clicked, this, &RemoteCaseSelector::onSelectButtonClicked);
}

void RemoteCaseSelector::listDirectories(QJsonArray directories)
{
    dirList->clear();

    while(currentDirectories.count())
    {
        currentDirectories.pop_back();
    }

    dirList->addItem(PARENT_DIR_LABEL);
    QStringList subDirectories;

    for(auto directory: directories)
    {
        auto directoryJson = directory.toObject();

        if(0 == directoryJson["type"].toString().compare("dir"))
        {
            QString dirName = directoryJson["name"].toString();
            if(0 == dirName.compare("."))
                continue;

            dirList->addItem(dirName);
            currentDirectories.push_back(directory);
            subDirectories << dirName;
        }
    }

    if (subDirectories.contains("0") && subDirectories.contains("constant")
            && subDirectories.contains("system"))
        selectButton->setDisabled(false);

    else
        selectButton->setDisabled(true);
}

QString RemoteCaseSelector::GetRemotePath(QJsonObject &directoryJson)
{
    QString relativePath = directoryJson["path"].toString();
    QString systemId = directoryJson["system"].toString();

    if(0 == systemId.compare("designsafe.storage.default"))
        return relativePath;
    else
        return "system/" + systemId + relativePath;
}
