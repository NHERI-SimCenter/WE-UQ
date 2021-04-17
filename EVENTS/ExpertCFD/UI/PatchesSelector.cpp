#include "PatchesSelector.h"
#include <QVBoxLayout>

PatchesSelector::PatchesSelector(QStringList patches, QString selectedPatches, QWidget *parent):
    QDialog(parent)
{
    setupUI(patches, selectedPatches);
    setupConnections();
}

QString PatchesSelector::getPatches()
{
    return patches.join(',');
}

void PatchesSelector::setupUI(QStringList patches, QString selectedPatches)
{
    setWindowTitle("Select Building Patches");
    auto layout = new QVBoxLayout();
    patchesList = new QListWidget();

    auto selectedPatchesList = selectedPatches.split(',');
    for(auto patch: patches)
    {
        auto box = new QListWidgetItem(patch, patchesList);
        box->setFlags(box->flags() | Qt::ItemIsUserCheckable);
        if(selectedPatchesList.contains(patch))
            box->setCheckState(Qt::Checked);
        else
            box->setCheckState(Qt::Unchecked);
    }

    selectButton = new QPushButton("Select Patches");
    layout->addWidget(patchesList);
    layout->addWidget(selectButton);

    this->setLayout(layout);
}

void PatchesSelector::setupConnections()
{
    connect(selectButton, &QPushButton::clicked, this, [this]()
    {
        patches.clear();

        for (int i = 0; i < patchesList->count(); i++)
        {
            auto item = patchesList->item(i);
            if (item->checkState() == Qt::CheckState::Checked)
                patches << item->text();
        }

        this->accept();
    });
}
