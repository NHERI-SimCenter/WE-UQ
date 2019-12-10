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
INCLUDEPATH += $$PWD/EVENTS/CFD
INCLUDEPATH += $$PWD/EVENTS/CFD/UI
INCLUDEPATH += $$PWD/EVENTS/CFD/Analysis
INCLUDEPATH += $$PWD/EVENTS/CFD/CFDanalysis
INCLUDEPATH += $$PWD/EVENTS/CFD/SimCenter_widgets

SOURCES += \
    $$PWD/EVENTS/CFD/CFDanalysis/CFDanalysisType.cpp \
    $$PWD/EVENTS/CFD/CFDanalysis/CFDcaseInstance.cpp \
    $$PWD/EVENTS/CFD/UI/CFDExpertWidget.cpp \
    $$PWD/EVENTS/CFD/UI/CWE3DView.cpp \
    $$PWD/EVENTS/CFD/UI/Edged3DBox.cpp \
    $$PWD/EVENTS/CFD/UI/GeometryHelper.cpp \
    $$PWD/EVENTS/CFD/UI/CFDTemplateWidget.cpp \
    $$PWD/EVENTS/CFD/UI/Graphics3DAxes.cpp \
    $$PWD/EVENTS/CFD/UI/PatchesSelector.cpp \
    $$PWD/EVENTS/CFD/UI/RemoteCaseSelector.cpp \
    $$PWD/EVENTS/CFD/UI/SubdomainsModel.cpp \
    $$PWD/EVENTS/CFD/cfd_globals.cpp \
    $$PWD/EVENTS/CFD/cwe_guiWidgets/cwe_parameters.cpp \
    $$PWD/EVENTS/CFD/cwe_guiWidgets/cwe_state_label.cpp \
    $$PWD/EVENTS/CFD/cwe_guiWidgets/cwe_tabwidget/cwe_groupswidget.cpp \
    $$PWD/EVENTS/CFD/cwe_guiWidgets/cwe_tabwidget/cwe_parampanel.cpp \
    $$PWD/EVENTS/CFD/cwe_guiWidgets/cwe_tabwidget/cwe_stagestatustab.cpp \
    $$PWD/EVENTS/CFD/cwe_guiWidgets/cwe_tabwidget/cwe_tabwidget.cpp \
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
    $$PWD/EVENTS/CFD/Analysis/filemetadata.cpp \
    $$PWD/EVENTS/CFD/SimCenter_widgets/sctrbooldatawidget.cpp \
    $$PWD/EVENTS/CFD/SimCenter_widgets/sctrchoicedatawidget.cpp \
    $$PWD/EVENTS/CFD/SimCenter_widgets/sctrfiledatawidget.cpp \
    $$PWD/EVENTS/CFD/SimCenter_widgets/sctrmasterdatawidget.cpp \
    $$PWD/EVENTS/CFD/SimCenter_widgets/sctrstddatawidget.cpp \
    $$PWD/EVENTS/CFD/SimCenter_widgets/sctrtextdatawidget.cpp \
    $$PWD/EVENTS/CFD/Analysis/remotejobdata.cpp

HEADERS  += \
    $$PWD/EVENTS/CFD/CFDanalysis/CFDanalysisType.h \
    $$PWD/EVENTS/CFD/CFDanalysis/CFDcaseInstance.h \
    $$PWD/EVENTS/CFD/SimCenter_widgets/sctrstates.h \
    $$PWD/EVENTS/CFD/UI/CFDTemplateWidget.h \
    $$PWD/EVENTS/CFD/UI/CFDExpertWidget.h \
    $$PWD/EVENTS/CFD/UI/CWE3DView.h \
    $$PWD/EVENTS/CFD/UI/Edged3DBox.h \
    $$PWD/EVENTS/CFD/UI/GeometryHelper.h \
    $$PWD/EVENTS/CFD/UI/Graphics3DAxes.h \
    $$PWD/EVENTS/CFD/UI/PatchesSelector.h \
    $$PWD/EVENTS/CFD/UI/RemoteCaseSelector.h \
    $$PWD/EVENTS/CFD/UI/SubdomainsModel.h \
    $$PWD/EVENTS/CFD/cfd_globals.h \
    $$PWD/EVENTS/CFD/cwe_guiWidgets/cwe_parameters.h \
    $$PWD/EVENTS/CFD/cwe_guiWidgets/cwe_state_label.h \
    $$PWD/EVENTS/CFD/cwe_guiWidgets/cwe_tabwidget/cwe_groupswidget.h \
    $$PWD/EVENTS/CFD/cwe_guiWidgets/cwe_tabwidget/cwe_parampanel.h \
    $$PWD/EVENTS/CFD/cwe_guiWidgets/cwe_tabwidget/cwe_stagestatustab.h \
    $$PWD/EVENTS/CFD/cwe_guiWidgets/cwe_tabwidget/cwe_tabwidget.h \
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
    $$PWD/EVENTS/CFD/Analysis/filemetadata.h \
    $$PWD/EVENTS/CFD/SimCenter_widgets/sctrbooldatawidget.h \
    $$PWD/EVENTS/CFD/SimCenter_widgets/sctrchoicedatawidget.h \
    $$PWD/EVENTS/CFD/SimCenter_widgets/sctrfiledatawidget.h \
    $$PWD/EVENTS/CFD/SimCenter_widgets/sctrmasterdatawidget.h \
    $$PWD/EVENTS/CFD/SimCenter_widgets/sctrstddatawidget.h \
    $$PWD/EVENTS/CFD/SimCenter_widgets/sctrtextdatawidget.h \
    $$PWD/EVENTS/CFD/Analysis/remotejobdata.h

FORMS    += \
    $$PWD/EVENTS/CFD/cwe_guiWidgets/cwe_parameters.ui \
    $$PWD/EVENTS/CFD/cwe_guiWidgets/cwe_tabwidget/cwe_parampanel.ui \
    $$PWD/EVENTS/CFD/cwe_guiWidgets/cwe_tabwidget/cwe_stagestatustab.ui \
    $$PWD/EVENTS/CFD/cwe_guiWidgets/cwe_tabwidget/cwe_tabwidget.ui \
    $$PWD/EVENTS/Inflow/inflowparameterwidget.ui

