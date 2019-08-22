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

#ifndef FILETREENODE_H
#define FILETREENODE_H

#include "filenoderef.h"

#include <QObject>
#include <QStandardItem>
#include <QDateTime>
#include <QPersistentModelIndex>

class FileStandardItem;

enum class NodeState {FILE_BUFF_LOADED, FILE_BUFF_RELOADING, FILE_BUFF_LOADING, FILE_KNOWN,
                      FILE_SPECULATE_IDLE, FILE_SPECULATE_LOADING,
                      FOLDER_CONTENTS_LOADED, FOLDER_CONTENTS_RELOADING, FOLDER_CONTENTS_LOADING, FOLDER_KNOWN_CONTENTS_NOT,
                      FOLDER_SPECULATE_IDLE, FOLDER_SPECULATE_LOADING,
                      INIT, ERROR, NON_EXTANT, DELETING};
//Note: Non-extant refers to nodes that do not exist. Deleting says the node is slated for removal.

enum class RequestState;
class FileMetaData;
class FileNodeRef;
class RemoteDataInterface;
class RemoteDataReply;
class FileOperator;

//Note: Quite a bit of this object is less well written than I would like
//Need to make data movements cleaner

class FileTreeNode : public QObject
{
    Q_OBJECT
public:
    FileTreeNode(FileMetaData contents, FileTreeNode * parent = nullptr);
    FileTreeNode(QString rootFolderName, FileOperator * theFileOperator, QObject * parent = nullptr); //This creates the default root folder
    ~FileTreeNode();

    bool isRootNode();
    NodeState getNodeState();
    FileNodeRef getFileData();
    QByteArray * getFileBuffer();
    FileTreeNode * getParentNode();
    QList<FileTreeNode *> getChildList();

    FileTreeNode * getNodeWithName(QString filename);
    FileTreeNode * getClosestNodeWithName(QString filename);
    FileTreeNode * getNodeReletiveToNodeWithName(QString searchPath);
    FileTreeNode * getChildNodeWithName(QString filename);

    bool isChildOf(FileTreeNode * possibleParent);

    bool haveLStask();
    void setLStask(RemoteDataReply * newTask);
    bool haveBuffTask();
    void setBuffTask(RemoteDataReply * newTask);

    void deleteFolderContentsData();
    void setFileBuffer(const QByteArray *newFileBuffer);

    QPersistentModelIndex getFirstModelIndex();

private slots:
    void deliverLSdata(RequestState taskState, QList<FileMetaData> dataList);
    void deliverBuffData(RequestState taskState, QByteArray bufferData);

private:
    void setNodeVisible();
    void recomputeNodeState();

    void changeNodeState(NodeState newState);
    void recomputeModelItems();
    void purgeModelItems();
    void updateModelItems(bool folderContentsLoaded);

    void settimestamps();

    FileTreeNode * pathSearchHelper(QString filename, bool stopEarly);
    FileTreeNode * pathSearchHelperFromAnyNode(QStringList filename, bool stopEarly);

    bool verifyControlNode(QList<FileMetaData> * newDataList);
    QString getControlAddress(QList<FileMetaData> * newDataList);
    void updateFileNodeData(QList<FileMetaData> * newDataList);

    void clearAllChildren();
    void insertFile(FileMetaData *newData);
    void purgeUnmatchedChildren(QList<FileMetaData> * newChildList);

    FileOperator * myFileOperator = nullptr;
    FileTreeNode * myParent = nullptr;

    FileNodeRef fileData;
    QList<FileTreeNode *> childList;

    QByteArray * fileDataBuffer = nullptr;

    RemoteDataReply * lsTask = nullptr;
    RemoteDataReply * bufferTask = nullptr;

    bool nodeVisible = false;
    bool folderContentsKnown = false;
    NodeState myState = NodeState::INIT;
    qint64 nodeTimestamp;

    QList<QPersistentModelIndex> modelItemList;
    QPersistentModelIndex decendantPlaceholderItem;
};

#endif // FILETREENODE_H
