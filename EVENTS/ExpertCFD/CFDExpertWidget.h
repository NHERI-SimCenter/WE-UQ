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

#ifdef ENDLN
#undef ENDLN
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
#define ENDLN endl
#else
#define ENDLN Qt::endl
#endif

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

protected:
    void showEvent(QShowEvent *event) override;

private:
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
    void exportgeneralizedMotionStateFile(QString, QString);

    bool getLine(QStringList &);
    QMap<QString, QString> *readParameters(void);

    // QWidget interface
    void resizeEvent(QResizeEvent *event);

    // variables used in the GUI

    QLineEdit*      caseEditBox;
    QComboBox*      solverComboBox;
    RemoteService*  remoteService;
    InflowParameterWidget* inflowWidget;
    QCheckBox*      inflowCheckBox;
    QComboBox*      forceComboBox;
    UserModeShapes* couplingGroup;
    QPushButton*    caseSelectButton;
    QLabel*         loginRequiredLabel;
    QDoubleSpinBox* startTimeBox;
    QLineEdit*      patchesEditBox;
    QPushButton*    selectPatchesButton;
    QComboBox*      meshingComboBox;
    QSpinBox*       processorsBox;
    QPushButton*    refreshButton;

    QFrame          *container;
    QScrollArea     *scrollArea;

    // variables used to indicate state of the app

    bool shown;

    // variables used for processing

    QString         originalUFilePath;
    QString         originalControlDictPath;
    QString         originalgeneralizedMotionStatePath;
    QString         originalfvSolutionPath;
    QStringList     patchesList;

    QDir oldLocation = QDir(".");
    QDir newLocation = QDir(".");

    QByteArray CDictContents;
    QByteArray TemplateContents;

    QFile       UFile;

    QList<QByteArray> UFileList;
    QListIterator<QByteArray> *UIter;
    QMap<QString, QMap<QString, QString> * > boundaries;

    QString     UFilePath;
    QByteArray  UFileContents;
    QByteArray  UFileHead = "";
    QByteArray  UFileTail = "";

};

#endif // CFDEXPERTWIDGET_H
