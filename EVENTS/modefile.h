#ifndef MODEFILE_H
#define MODEFILE_H

#include <QObject>
#include <QString>

class ModeFile : QObject
{
    Q_OBJECT

public:
    explicit ModeFile();
    explicit ModeFile(const QString &name);
    ~ModeFile();

    bool newFile(const QString &name);

    int numberOfModes() { return numModes; };
    int numberOfFloors() { return numFloors; };
    int numberOfLevels() { return numFloors; };
    int numberOfNodes() { return numNodes; };

    void writeFile(QString name);
    QString getFile();

private:
    bool parseFile();


    QString filename;
    QString file_contents;

    int numModes;
    int numFloors;
    int numNodes;

    bool status;
};

#endif // MODEFILE_H
