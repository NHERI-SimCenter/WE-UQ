#ifndef CFDEXPERTWIDGET_H
#define CFDEXPERTWIDGET_H

#include <SimCenterAppWidget.h>
#include <RandomVariablesContainer.h>
#include <RemoteService.h>
#include <Inflow/inflowparameterwidget.h>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include "usermodeshapes.h"
#include <QDir>

class CFDExpertWidget : public SimCenterAppWidget
{
    Q_OBJECT
public:
    explicit CFDExpertWidget(RandomVariablesContainer *theRandomVariableIW, RemoteService* remoteService, QWidget *parent = nullptr);

    bool outputAppDataToJSON(QJsonObject &jsonObject) override;
    bool outputToJSON(QJsonObject &rvObject) override;
    bool inputFromJSON(QJsonObject &rvObject) override;
    bool copyFiles(QString &path) override;
    bool supportsLocalRun() override;

signals:

public slots:
    void selectButtonPushed();
    void selectPatchesPushed();
    void on_couplingGroup_checked(bool checked);

private:
    QLineEdit* caseEditBox;
    QComboBox* solverComboBox;
    RemoteService* remoteService;
    InflowParameterWidget* inflowWidget;
    QCheckBox* inflowCheckBox;
    QComboBox* forceComboBox;
    UserModeShapes* couplingGroup;
    QPushButton* caseSelectButton;
    QLabel* loginRequiredLabel;
    QDoubleSpinBox* startTimeBox;
    QLineEdit* patchesEditBox;
    QPushButton* selectPatchesButton;
    QComboBox* meshingComboBox;
    QSpinBox* processorsBox;
    QPushButton* refreshButton;

    QFrame *container;
    QScrollArea *scrollArea;

    QString originalUFilePath;
    QString originalControlDictPath;
    QStringList patchesList;

    QDir oldLocation = QDir(".");
    QDir newLocation = QDir(".");

    bool shown;


    void downloadRemoteCaseFiles();
    void ensureUFileExists();
    QStringList getRemoteFilesPaths();
    void initializeUI();
    void setupConnections();
    void parseBoundaryPatches(QString uFilePath);
    void processBuildingPatches();
    bool validateSelectedPatches();
    void autoSelectPatches();
    bool buildFiles(QString &dirName);

    /* *******************************************************************
     *     migrated from InflowWidget
     * *******************************************************************/

    bool readUfile(QString);
    void processUfile();
    void exportUFile(QString);

    bool readControlDict(QString);
    void exportControlDictFile(QString, QString);

    bool getLine(QStringList &);
    QMap<QString, QString> *readParameters(void);

    QByteArray CDictContents;

    QFile UFile;
    QList<QByteArray> UFileList;
    QListIterator<QByteArray> *UIter;
    QMap<QString, QMap<QString, QString> * > boundaries;

    QString UFilePath;
    QByteArray UFileContents;
    QByteArray UFileHead = "";
    QByteArray UFileTail = "";


    // QWidget interface
    void resizeEvent(QResizeEvent *event);

protected:
    void showEvent(QShowEvent *event) override;
};

#endif // CFDEXPERTWIDGET_H
