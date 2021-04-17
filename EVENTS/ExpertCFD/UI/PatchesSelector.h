#ifndef PATCHESSELECTOR_H
#define PATCHESSELECTOR_H

#include <QWidget>
#include <QDialog>
#include <RemoteService.h>
#include <QListWidget>
#include <QPushButton>

class PatchesSelector : public QDialog
{
public:
    explicit PatchesSelector(QStringList patches, QString selectedPatches, QWidget *parent = nullptr);
    QString getPatches();
private:
    QListWidget* patchesList;
    QPushButton* selectButton;
    QStringList patches;

    void setupUI(QStringList patches, QString selectedPatches);
    void setupConnections();
};

#endif // PATCHESSELECTOR_H
