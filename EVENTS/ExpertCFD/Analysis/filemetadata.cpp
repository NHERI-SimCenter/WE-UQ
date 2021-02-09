/*********************************************************************************
**
** Copyright (c) 2017 The University of Notre Dame
** Copyright (c) 2017 The Regents of the University of California
**
** Redistribution and use in source and binary forms, with or without modification,
** are permitted provided that the following conditions are met:
**
** 1. Redistributions of source code must retain the above copyright notice, this
** list of conditions and the following disclaimer.
**
** 2. Redistributions in binary form must reproduce the above copyright notice, this
** list of conditions and the following disclaimer in the documentation and/or other
** materials provided with the distribution.
**
** 3. Neither the name of the copyright holder nor the names of its contributors may
** be used to endorse or promote products derived from this software without specific
** prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
** EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
** OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
** SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
** INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
** TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
** BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
** IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
** SUCH DAMAGE.
**
***********************************************************************************/

// Contributors:
// Written by Peter Sempolinski, for the Natural Hazard Modeling Laboratory, director: Ahsan Kareem, at Notre Dame

#include "filemetadata.h"

FileMetaData::FileMetaData()
{
    //Note: defaults are handled in the class header
}

FileMetaData& FileMetaData::operator=(const FileMetaData &toCopy)
{
    copyDataFrom(toCopy);
    return *this;
}

void FileMetaData::copyDataFrom(const FileMetaData &toCopy)
{
    fullContainingPath = toCopy.fullContainingPath;
    fileName = toCopy.fileName;
    fileSize = toCopy.fileSize;
    myType = toCopy.myType;
}

void FileMetaData::setFullFilePath(QString fullPath)
{
    QStringList pathParts;
    char divChar;
    if (fullPath.contains('/'))
    {
        pathParts = fullPath.split('/');
        divChar = '/';
    }
    else
    {
        pathParts = fullPath.split('\\');
        divChar = '\\';
    }
    fileName = "";

    while (fileName.isEmpty())
    {
        if (pathParts.size() == 0)
        {
            fullContainingPath = "";
            fullContainingPath = fullContainingPath.append(divChar);
            fileName = "";
            return;
        }
        fileName = pathParts.takeLast();
    }
    fullContainingPath = divChar;
    for (auto itr = pathParts.cbegin(); itr != pathParts.cend(); itr++)
    {
        if (!(*itr).isEmpty())
        {
            fullContainingPath.append(*itr);
            fullContainingPath.append(divChar);
        }
    }
}

void FileMetaData::setSize(int newSize)
{
    fileSize = newSize;
}

void FileMetaData::setType(FileType newType)
{
    myType = newType;
}

QString FileMetaData::getFullPath() const
{
    QString ret = fullContainingPath;
    ret.append(fileName);
    return ret;
}

QString FileMetaData::getFileName() const
{
    return fileName;
}

QString FileMetaData::getContainingPath() const
{
    return fullContainingPath;
}

int FileMetaData::getSize() const
{
    return fileSize;
}

FileType FileMetaData::getFileType() const
{
    return myType;
}

QString FileMetaData::getFileTypeString() const
{
    switch (myType)
    {
    case FileType::DIR : return "Folder";
    case FileType::FILE : return "File";
    case FileType::INVALID : return "Error";
    case FileType::SIM_LINK : return "Link";
    case FileType::NIL : return "Nil";
    }
    return "ERROR";
}

bool FileMetaData::isNil() const
{
    return (myType == FileType::NIL);
}

QStringList FileMetaData::getPathNameList(QString fullPath)
{
    QStringList fileNames = fullPath.split('/');

    QStringList ret;
    for (auto itr = fileNames.cbegin(); itr != fileNames.cend(); itr++)
    {
        if (!(*itr).isEmpty())
        {
            ret.append(*itr);
        }
    }
    return ret;
}

QString FileMetaData::cleanPathSlashes(QString fullPath)
{
    //TODO: maybe this should work for both types of slashes

    bool isFolder = false;
    if ((fullPath.at(fullPath.size() - 1)) == '/')
    {
        isFolder = true;
    }
    QStringList fileNameList = FileMetaData::getPathNameList(fullPath);
    QString ret;
    if (fileNameList.isEmpty()) return "/";

    for (auto itr = fileNameList.cbegin(); itr != fileNameList.cend(); itr++)
    {
        ret.append("/");
        ret.append(*itr);
    }

    if (isFolder){ret.append('/');}
    return ret;
}
