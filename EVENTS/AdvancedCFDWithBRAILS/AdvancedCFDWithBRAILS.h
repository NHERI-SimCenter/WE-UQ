#ifndef ADVANCED_CFD_WITH_BRAILS_H
#define ADVANCED_CFD_WITH_BRAILS_H

/* *****************************************************************************
Copyright (c) 2016-2017, The Regents of the University of California (Regents).
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS
PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT,
UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

*************************************************************************** */

// Written: fmk

#include <SimCenterAppWidget.h>

class RandomVariablesContainer;
class SC_IntLineEdit;
class SC_DoubleLineEdit;
class SC_StringLineEdit;
class SC_DirEdit;
class SC_ComboBox;
class QTableWidget;

class AdvancedCFDWithBRAILS : public SimCenterAppWidget
{
    Q_OBJECT

public:
    AdvancedCFDWithBRAILS(RandomVariablesContainer *theRandomVariableIW, bool isLaunchedAsTool=false, QWidget *parent = 0);
    ~AdvancedCFDWithBRAILS();

    //Methods
    bool initialize();
    bool isInitialize();
  
    bool inputFromJSON(QJsonObject &rvObject);
    bool outputToJSON(QJsonObject &rvObject);
    bool outputAppDataToJSON(QJsonObject &rvObject);
    bool inputAppDataFromJSON(QJsonObject &rvObject);
    bool copyFiles(QString &dirName);
    void clear(void);  

signals:

public slots:
   void runLocal(void);

private:
  bool initialized;
  SC_DirEdit *tinfProfile;
  SC_StringLineEdit *caseFolder;
  SC_DoubleLineEdit *windDirection;
  SC_IntLineEdit *numberOfProcessors;
  SC_DoubleLineEdit *latLR, *lonLR, *radiusLR;
  SC_DoubleLineEdit *minLat, *maxLat, *minLon, *maxLon;
  
  SC_ComboBox *scraper;
  SC_DoubleLineEdit *defaultHeight;
  
  SC_DoubleLineEdit *surfaceLevelLR, *surfaceLevelROI;
  SC_DoubleLineEdit *boundaryMeshCellSize, *kinematicViscosity;
  SC_IntLineEdit *numCellsBetweenLayers;
  SC_DoubleLineEdit *sideExtent, *topExtent, *inletExtent, *outletExtent;
  SC_ComboBox *sideStyle, *groundStyle, *roiStyle, *surroundStyle, *framework;
  SC_ComboBox *lesAlgorithm, *lesInflow;
  SC_DoubleLineEdit *Uref, *Href, *z0;  

  SC_DoubleLineEdit *end_timeRANS, *deltaT_simRANS, *deltaT_writeRANS;
  SC_DoubleLineEdit *end_timeLES, *initDeltaT_simLES, *deltaT_writeLES;
  SC_DoubleLineEdit *maxDeltaT_simLES, *maxCourantLES;
  SC_IntLineEdit *numWindProfiles, *numSectionPlanes;
  SC_ComboBox *adjustTimeLES;
  QTableWidget *refinementTable;
  
};

#endif // ADVANCED_CFD_WITH_BRAILS_H

