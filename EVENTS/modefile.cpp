#include "modefile.h"

#include <QString>


ModeFile::ModeFile()
    : QObject()
{
    status = false;  // have no file

    filename = "";
    file_contents = "";

    numModes = 0;
    numFloors = 0;
    numNodes = 0;
}

ModeFile::ModeFile(const QString & name)
    : QObject()
{
    ModeFile();
    //newFile(name);
}

ModeFile::~ModeFile()
{
    // nothing to do here
}

/* */
bool ModeFile::newFile(const QString &name)
{
    return true;
}
/* */

void ModeFile::writeFile(QString name)
{

}

QString ModeFile::getFile()
{
    QString theContents;

    return theContents;
}

bool ModeFile::parseFile()
{
    return true;
}
