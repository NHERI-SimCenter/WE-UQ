/*********************************************************************************
**
** Copyright (c) 2018 The University of Notre Dame
** Copyright (c) 2018 The Regents of the University of California
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

#include "filenoderef.h"

#include "fileoperator.h"
#include "filetreenode.h"

FileNodeRef::FileNodeRef() : FileMetaData() {}

FileNodeRef& FileNodeRef::operator=(const FileNodeRef &toCopy)
{
    copyDataFrom(toCopy);
    timestamp = toCopy.timestamp;
    myFileOperator = toCopy.myFileOperator;
    return *this;
}

bool FileNodeRef::fileNodeExtant() const
{
    if (myFileOperator == nullptr) return false;
    if (getFileType() == FileType::NIL) return false;

    return myFileOperator->fileStillExtant(*this);
}

NodeState FileNodeRef::getNodeState() const
{
    if (myFileOperator == nullptr) return NodeState::NON_EXTANT;
    return myFileOperator->getFileNodeState(*this);
}

bool FileNodeRef::isAncestorOf(const FileNodeRef &child) const
{
    if (getFileType() == FileType::NIL) return false;
    if (child.getFileType() == FileType::NIL) return false;
    if (myFileOperator == nullptr)
    {
        qCDebug(fileManager, "ERROR: attempted use of FileNodeRef without file operator.(1)");
        return false;
    }
    if (myFileOperator != child.myFileOperator)
    {
        qCDebug(fileManager, "ERROR: attempted use of FileNodeRef with mismatched file operator.");
        return false;
    }
    return myFileOperator->isAncestorOf(*this, child);
}

const FileNodeRef FileNodeRef::getChildWithName(QString childName) const
{
    if (myFileOperator == nullptr)
    {
        if (getFileType() != FileType::NIL) qCDebug(fileManager, "ERROR: attempted use of FileNodeRef without file operator.(2)");
        return FileNodeRef::nil();
    }
    return myFileOperator->getChildWithName(*this, childName);
}

bool FileNodeRef::fileBufferLoaded() const
{
    if (myFileOperator == nullptr)
    {
        if (getFileType() != FileType::NIL) qCDebug(fileManager, "ERROR: attempted use of FileNodeRef without file operator.(3)");
        return false;
    }
    return (myFileOperator->getFileNodeState(*this) == NodeState::FILE_BUFF_LOADED);
}

const QByteArray FileNodeRef::getFileBuffer() const
{
    if (myFileOperator == nullptr)
    {
        if (getFileType() != FileType::NIL) qCDebug(fileManager, "ERROR: attempted use of FileNodeRef without file operator.(4)");
        return QByteArray();
    }
    return myFileOperator->getFileBuffer(*this);
}

void FileNodeRef::setFileBuffer(const QByteArray * toSet) const
{
    if (myFileOperator == nullptr)
    {
        if (getFileType() != FileType::NIL) qCDebug(fileManager, "ERROR: attempted use of FileNodeRef without file operator.(5)");
        return;
    }
    myFileOperator->setFileBuffer(*this, toSet);
}

bool FileNodeRef::folderContentsLoaded() const
{
    if (myFileOperator == nullptr)
    {
        if (getFileType() != FileType::NIL) qCDebug(fileManager, "ERROR: attempted use of FileNodeRef without file operator.(6)");
        return false;
    }
    return (myFileOperator->getFileNodeState(*this) == NodeState::FOLDER_CONTENTS_LOADED);
}

FileNodeRef FileNodeRef::getParent() const
{
    if (myFileOperator == nullptr)
    {
        if (getFileType() != FileType::NIL) qCDebug(fileManager, "ERROR: attempted use of FileNodeRef without file operator.(7)");
        return FileNodeRef::nil();
    }
    return (myFileOperator->getParent(*this));
}

QList<FileNodeRef> FileNodeRef::getChildList() const
{
    if (myFileOperator == nullptr)
    {
        if (getFileType() != FileType::NIL) qCDebug(fileManager, "ERROR: attempted use of FileNodeRef without file operator.(8)");
        return QList<FileNodeRef>();
    }
    return (myFileOperator->getChildList(*this));
}

bool FileNodeRef::isRootNode() const
{
    if (myFileOperator == nullptr)
    {
        if (getFileType() != FileType::NIL) qCDebug(fileManager, "ERROR: attempted use of FileNodeRef without file operator.(9)");
        return false;
    }
    return (myFileOperator->nodeIsRoot(*this));
}

QPersistentModelIndex FileNodeRef::getFirstModelIndex() const
{
    if (myFileOperator == nullptr)
    {
        if (getFileType() != FileType::NIL) qCDebug(fileManager, "ERROR: attempted use of FileNodeRef without file operator.(10)");
        return QPersistentModelIndex();
    }
    return (myFileOperator->getModelIndexByFile(*this));
}

void FileNodeRef::enactFolderRefresh(bool clearData) const
{
    if (myFileOperator == nullptr)
    {
        if (getFileType() != FileType::NIL) qCDebug(fileManager, "ERROR: attempted use of FileNodeRef without file operator.(11)");
        return;
    }
    myFileOperator->enactFolderRefresh(*this, clearData);
}

FileNodeRef FileNodeRef::nil()
{
    FileNodeRef ret;
    return ret;
}

void FileNodeRef::setFileOperator(FileOperator * newFileOperator)
{
    myFileOperator = newFileOperator;
}

bool FileNodeRef::usesFileOperator(FileOperator * toCompare)
{
    return (myFileOperator == toCompare);
}

void FileNodeRef::setTimestamp(qint64 newTimestamp)
{
    timestamp = newTimestamp;
}

qint64 FileNodeRef::getTimestamp() const
{
    return timestamp;
}
