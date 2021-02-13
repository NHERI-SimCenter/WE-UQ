#-------------------------------------------------
#
# Project created by fmk
#
#-------------------------------------------------

#message($$_PRO_FILE_PWD_)
#message($$PWD)

INCLUDEPATH += $$PWD/EVENTS
INCLUDEPATH += $$PWD/EVENTS/StochasticMotionInput/include
INCLUDEPATH += StochasticWindModel/include
INCLUDEPATH += Inflow
INCLUDEPATH += $$PWD/EVENTS/BasicCFD
INCLUDEPATH += $$PWD/EVENTS/ExpertCFD
INCLUDEPATH += $$PWD/EVENTS/ExpertCFD/UI
INCLUDEPATH += $$PWD/EVENTS/ExpertCFD/Analysis
INCLUDEPATH += $$PWD/EVENTS/ExpertCFD/CFDanalysis
INCLUDEPATH += $$PWD/EVENTS/ExpertCFD/SimCenter_widgets

SOURCES += \
    $$PWD/EVENTS/ExpertCFD/CFDanalysis/CFDanalysisType.cpp \
    $$PWD/EVENTS/ExpertCFD/CFDanalysis/CFDcaseInstance.cpp \
    $$PWD/EVENTS/ExpertCFD/CFDExpertWidget.cpp \
    $$PWD/EVENTS/ExpertCFD/UI/CWE3DView.cpp \
    $$PWD/EVENTS/ExpertCFD/UI/Edged3DBox.cpp \
    $$PWD/EVENTS/ExpertCFD/UI/GeometryHelper.cpp \
    $$PWD/EVENTS/ExpertCFD/UI/CFDTemplateWidget.cpp \
    $$PWD/EVENTS/ExpertCFD/UI/Graphics3DAxes.cpp \
    $$PWD/EVENTS/ExpertCFD/UI/PatchesSelector.cpp \
    $$PWD/EVENTS/ExpertCFD/UI/RemoteCaseSelector.cpp \
    $$PWD/EVENTS/ExpertCFD/UI/SubdomainsModel.cpp \
    $$PWD/EVENTS/ExpertCFD/cfd_globals.cpp \
    $$PWD/EVENTS/ExpertCFD/cwe_guiWidgets/cwe_parameters.cpp \
    $$PWD/EVENTS/ExpertCFD/cwe_guiWidgets/cwe_state_label.cpp \
    $$PWD/EVENTS/ExpertCFD/cwe_guiWidgets/cwe_tabwidget/cwe_groupswidget.cpp \
    $$PWD/EVENTS/ExpertCFD/cwe_guiWidgets/cwe_tabwidget/cwe_parampanel.cpp \
    $$PWD/EVENTS/ExpertCFD/cwe_guiWidgets/cwe_tabwidget/cwe_stagestatustab.cpp \
    $$PWD/EVENTS/ExpertCFD/cwe_guiWidgets/cwe_tabwidget/cwe_tabwidget.cpp \
    $$PWD/EVENTS/ExpertCFD/OpenFoamHelper/openfoamhelper.cpp \
    $$PWD/EVENTS/Inflow/inflowparameterwidget.cpp \
    $$PWD/EVENTS/WindEventSelection.cpp \
    $$PWD/EVENTS/DEDM_HRP.cpp \
    $$PWD/EVENTS/BasicCFD/BasicCFD.cpp \
    $$PWD/EVENTS/BasicCFD/MeshParametersCWE.cpp \
    $$PWD/EVENTS/BasicCFD/SimulationParametersCWE.cpp \
    $$PWD/EVENTS/LowRiseTPU.cpp \
    $$PWD/EVENTS/WindTunnelExperiment.cpp \
    $$PWD/EVENTS/StochasticWindModel/src/WittigSinha.cpp \
    $$PWD/EVENTS/StochasticWindModel/src/StochasticWindInput.cpp \
    $$PWD/EVENTS/ExpertCFD/Analysis/filemetadata.cpp \
    $$PWD/EVENTS/ExpertCFD/SimCenter_widgets/sctrbooldatawidget.cpp \
    $$PWD/EVENTS/ExpertCFD/SimCenter_widgets/sctrchoicedatawidget.cpp \
    $$PWD/EVENTS/ExpertCFD/SimCenter_widgets/sctrfiledatawidget.cpp \
    $$PWD/EVENTS/ExpertCFD/SimCenter_widgets/sctrmasterdatawidget.cpp \
    $$PWD/EVENTS/ExpertCFD/SimCenter_widgets/sctrstddatawidget.cpp \
    $$PWD/EVENTS/ExpertCFD/SimCenter_widgets/sctrtextdatawidget.cpp \
    $$PWD/EVENTS/ExpertCFD/Analysis/remotejobdata.cpp \
    $$PWD/EVENTS/initialmodalinfo.cpp \
    $$PWD/EVENTS/usermodeshapes.cpp

HEADERS  += \
    $$PWD/EVENTS/ExpertCFD/CFDanalysis/CFDanalysisType.h \
    $$PWD/EVENTS/ExpertCFD/CFDanalysis/CFDcaseInstance.h \
    $$PWD/EVENTS/ExpertCFD/SimCenter_widgets/sctrstates.h \
    $$PWD/EVENTS/ExpertCFD/UI/CFDTemplateWidget.h \
    $$PWD/EVENTS/ExpertCFD/CFDExpertWidget.h \
    $$PWD/EVENTS/ExpertCFD/UI/CWE3DView.h \
    $$PWD/EVENTS/ExpertCFD/UI/Edged3DBox.h \
    $$PWD/EVENTS/ExpertCFD/UI/GeometryHelper.h \
    $$PWD/EVENTS/ExpertCFD/UI/Graphics3DAxes.h \
    $$PWD/EVENTS/ExpertCFD/UI/PatchesSelector.h \
    $$PWD/EVENTS/ExpertCFD/UI/RemoteCaseSelector.h \
    $$PWD/EVENTS/ExpertCFD/UI/SubdomainsModel.h \
    $$PWD/EVENTS/ExpertCFD/cfd_globals.h \
    $$PWD/EVENTS/ExpertCFD/cwe_guiWidgets/cwe_parameters.h \
    $$PWD/EVENTS/ExpertCFD/cwe_guiWidgets/cwe_state_label.h \
    $$PWD/EVENTS/ExpertCFD/cwe_guiWidgets/cwe_tabwidget/cwe_groupswidget.h \
    $$PWD/EVENTS/ExpertCFD/cwe_guiWidgets/cwe_tabwidget/cwe_parampanel.h \
    $$PWD/EVENTS/ExpertCFD/cwe_guiWidgets/cwe_tabwidget/cwe_stagestatustab.h \
    $$PWD/EVENTS/ExpertCFD/cwe_guiWidgets/cwe_tabwidget/cwe_tabwidget.h \
    $$PWD/EVENTS/ExpertCFD/OpenFoamHelper/openfoamhelper.h \
    $$PWD/EVENTS/Inflow/inflowparameterwidget.h \
    $$PWD/EVENTS/WindEventSelection.h \
    $$PWD/EVENTS/WindTunnelExperiment.h \
    $$PWD/EVENTS/DEDM_HRP.h \
    $$PWD/EVENTS/BasicCFD/BasicCFD.h \
    $$PWD/EVENTS/BasicCFD/MeshParametersCWE.h \
    $$PWD/EVENTS/BasicCFD/SimulationParametersCWE.h \
    $$PWD/EVENTS/LowRiseTPU.h \
    $$PWD/EVENTS/StochasticWindModel/include/WittigSinha.h \
    $$PWD/EVENTS/StochasticWindModel/include/StochasticWindInput.h \
    $$PWD/EVENTS/ExpertCFD/Analysis/filemetadata.h \
    $$PWD/EVENTS/ExpertCFD/SimCenter_widgets/sctrbooldatawidget.h \
    $$PWD/EVENTS/ExpertCFD/SimCenter_widgets/sctrchoicedatawidget.h \
    $$PWD/EVENTS/ExpertCFD/SimCenter_widgets/sctrfiledatawidget.h \
    $$PWD/EVENTS/ExpertCFD/SimCenter_widgets/sctrmasterdatawidget.h \
    $$PWD/EVENTS/ExpertCFD/SimCenter_widgets/sctrstddatawidget.h \
    $$PWD/EVENTS/ExpertCFD/SimCenter_widgets/sctrtextdatawidget.h \
    $$PWD/EVENTS/ExpertCFD/Analysis/remotejobdata.h \
    $$PWD/EVENTS/initialmodalinfo.h \
    $$PWD/EVENTS/usermodeshapes.h

FORMS    += \
    $$PWD/EVENTS/ExpertCFD/cwe_guiWidgets/cwe_parameters.ui \
    $$PWD/EVENTS/ExpertCFD/cwe_guiWidgets/cwe_tabwidget/cwe_parampanel.ui \
    $$PWD/EVENTS/ExpertCFD/cwe_guiWidgets/cwe_tabwidget/cwe_stagestatustab.ui \
    $$PWD/EVENTS/ExpertCFD/cwe_guiWidgets/cwe_tabwidget/cwe_tabwidget.ui \
    $$PWD/EVENTS/Inflow/inflowparameterwidget.ui \
    $$PWD/EVENTS/usermodeshapes.ui

QT += widgets

