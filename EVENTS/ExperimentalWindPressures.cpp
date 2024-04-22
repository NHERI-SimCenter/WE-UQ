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

// Written: fmckenna

#include "ExperimentalWindPressures.h"
#include "TapsInputDelegate.h"
#include "WindForceSpectrum.h"
#include "SC_DoubleLineEdit.h"
#include "SC_IntLineEdit.h"
#include "SC_StringLineEdit.h"

#include <sstream>
#include <QPushButton>
#include <QScrollArea>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>
#include <QFileDialog>
#include <QPushButton>
#include <SectionTitle.h>
#include <QFileInfo>
#include <QMovie>
#include <QPixmap>
#include <QIcon>
#include <RandomVariablesContainer.h>
#include <QRadioButton>
#include <QButtonGroup>
#include <QJsonArray>
#include <QComboBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QVector>
#include <LineEditRV.h>

#include <SimCenterPreferences.h>
#include <GeneralInformationWidget.h>

//#include <InputWidgetParameters.h>

// Written: fmckenna, Sang-ri

ExperimentalWindPressures::ExperimentalWindPressures(RandomVariablesContainer *theRandomVariableIW, QWidget *parent)
    : SimCenterAppWidget(parent), theRandomVariablesContainer(theRandomVariableIW)
{

    QGridLayout *layout = new QGridLayout();

    theExpWidget = new WindForceSpectrum(theRandomVariablesContainer);
    theExpWidget->hideCitation(false);

    windowSize = new SC_DoubleLineEdit("windowSize",4);
    overlapPerc = new SC_DoubleLineEdit("overlapPerc",50);
    cpsdGroupSize = new SC_IntLineEdit("cpsdGroupSize",50);
    selectedTaps= new TapsInputDelegate();
    //selectedTaps ->setMinimumWidth(400);
    //connect(selectedTaps,&QLineEdit::editingFinished,this,&ExperimentalWindPressures::selectTaps);

    layout->addWidget(theExpWidget,0,0,1,-1);
    layout->addWidget(new QLabel("Window Size"),1,0);
    layout->addWidget(windowSize,1,1);
    layout->addWidget(new QLabel("sec"),1,2);

    layout->addWidget(new QLabel("Overlap Percentage"),2,0);
    layout->addWidget(overlapPerc,2,1);
    layout->addWidget(new QLabel("%"),2,2);

    layout->addWidget(new QLabel("CPSD Group Size"),3,0);
    layout->addWidget(cpsdGroupSize,3,1);

    layout->addWidget(new QLabel("Selected Taps"),4,0);
    layout->addWidget(selectedTaps,4,1,1,-1);

    citationQuoteLabel = new QLabel("\nThe backend application used by this selection was provided by Prof. Seymour Spence and his students at the University of Michigan. Users should cite this work as follows:\nDuarte, T.G., Arunachalam, S., Subgranon, A. and Spence, S.M., (2023). Uncertainty Quantification and Simulation of Wind-Tunnel-Informed Stochastic Wind Loads. Wind, 3(3), pp.375-393.");
    citationQuoteLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    layout->addWidget(citationQuoteLabel,5,0,1,-1);

    layout->setRowStretch(8,1);
    layout->setColumnStretch(2,1);

    this->setLayout(layout);
}


ExperimentalWindPressures::~ExperimentalWindPressures()
{

}


void ExperimentalWindPressures::clear(void)
{
  //
}



bool
ExperimentalWindPressures::outputToJSON(QJsonObject &jsonObject)
{
    // just need to send the class type here.. type needed in object in case user screws up

//    modePercent->outputToJSON(jsonObject);
//    modelScale->outputToJSON(jsonObject);
//    fullScaleSpeed->outputToJSON(jsonObject);
//    fullScaleDuration->outputToJSON(jsonObject);
//    windowSize->outputToJSON(jsonObject);
//    overlapPerc->outputToJSON(jsonObject);
//    seed->outputToJSON(jsonObject);
//    filename->outputToJSON(jsonObject);
    theExpWidget->outputToJSON(jsonObject);
    windowSize->outputToJSON(jsonObject);
    overlapPerc->outputToJSON(jsonObject);
    windowSize->outputToJSON(jsonObject);
    overlapPerc->outputToJSON(jsonObject);
    cpsdGroupSize->outputToJSON(jsonObject);

    std::set<int> myIdxSet = selectedTaps->getSelectedComponentIDs();
    std::list<int> myIdxList(myIdxSet.begin(), myIdxSet.end());
    //QJsonArray myIdxJsonArray = myIdxList.array();
    QJsonArray myIdxArray;
    for (int num : myIdxList) {
        myIdxArray.append(num);
    }
    jsonObject["selectedTaps"] = QJsonArray(myIdxArray);

    //selectedTaps->outputToJSON(jsonObject);
    // Let us overwrite this
    jsonObject["type"]="ExperimentalWindPressures";
    jsonObject["EventClassification"]="Wind";

    return true;
}


bool
ExperimentalWindPressures::inputFromJSON(QJsonObject &jsonObject)
{
    this->clear();

    theExpWidget->inputFromJSON(jsonObject);

//    modePercent->inputFromJSON(jsonObject);
//    modelScale->inputFromJSON(jsonObject);
//    fullScaleSpeed->inputFromJSON(jsonObject);
//    fullScaleDuration->inputFromJSON(jsonObject);
    windowSize->inputFromJSON(jsonObject);
    overlapPerc->inputFromJSON(jsonObject);
    cpsdGroupSize->inputFromJSON(jsonObject);
    QJsonArray jsonArray =jsonObject["selectedTaps"].toArray();

    QString combinedString;
    for (const QJsonValue& value : jsonArray) {
        if (value.isDouble()) {
            combinedString.append(QString::number(value.toInt()));
        }
        combinedString.append(", "); // Add a space between each integer
    }
    selectedTaps->setText(combinedString);
    selectedTaps->textEdited("");
    connect(selectedTaps,&QLineEdit::editingFinished,this,&ExperimentalWindPressures::selectTaps);
    selectedTaps->editingFinished();

//    seed->inputFromJSON(jsonObject);
//    filename->inputFromJSON(jsonObject);
    return true;
}

void ExperimentalWindPressures::selectTaps(void)
{
    try
    {
        selectedTaps->selectComponents();
    }
    catch (const QString msg)
    {
        this->errorMessage(msg);
    }
}

bool
ExperimentalWindPressures::outputAppDataToJSON(QJsonObject &jsonObject) {

    //
    // per API, need to add name of application to be called in AppLication
    // and all data to be used in ApplicationDate
    //

    jsonObject["EventClassification"]="Wind";
    jsonObject["Application"] = "ExperimentalWindPressures";
    QJsonObject dataObj;
    jsonObject["ApplicationData"] = dataObj;

    return true;
}
bool
ExperimentalWindPressures::inputAppDataFromJSON(QJsonObject &jsonObject) {

    Q_UNUSED(jsonObject);
    return true;
}


bool
ExperimentalWindPressures::copyFiles(QString &destDir) {
    theExpWidget->copyFiles(destDir);
    return true;
}

/*
void ExperimentalWindPressures::selectTaps()
{
    auto inputText = selectedTaps->text();

    if(inputText==prevText)
        return;

    //selectedComponentIDs.clear();

    // Quick return if the input text is empty
    if(inputText.isEmpty())
        return;

    // Remove any white space from the string
    inputText.remove(" ");

    // Split the incoming text into the parts delimited by commas
    std::vector<std::string> subStringVec;

    // Create string stream from the string
    std::stringstream s_stream(inputText.toStdString());

    // Split the input string to substrings at the comma
    while(s_stream.good()) {
        std:: string subString;
        getline(s_stream, subString, ',');
        if(!subString.empty())
            subStringVec.push_back(subString);
    }

    // Check for the case where the IDs are given as a range
    std::string dashDelim = "-";

    for(auto&& subStr : subStringVec)
    {
        // Handle the case where there is a range of assets separated by a '-'
        if (subStr.find(dashDelim) != std::string::npos)
        {
            auto pos = subStr.find(dashDelim);
            // Get the strings on either side of the '-' character
            std::string intStart = subStr.substr(0, pos);
            std::string intEnd = subStr.substr(pos + dashDelim.length());

            // Convert them into integers
            auto IDStart = std::stoi(intStart);
            auto IDEnd = std::stoi(intEnd);

            // Make sure that the end integer is greater than the first
            if(IDStart>IDEnd)
            {
                QString err = "Error in the range of asset IDs provided in the Component asset selection box";
                throw err;
                continue;
            }

            // Add the IDs to the set
            for(int ID = IDStart; ID<=IDEnd; ++ID)
                selectedComponentIDs.insert(ID);
        }
        else // Asset ID is given individually
        {
            auto ID = std::stoi(subStr);

            selectedComponentIDs.insert(ID);
        }
    }

    // Reset the text on the line edit
    auto sortedIds = this->getComponentAnalysisList();
    selectedTaps->setText(sortedIds);

    prevText=sortedIds;
}
std::set<int> ExperimentalWindPressures::getSelectedComponentIDs() const
{
    return selectedComponentIDs;
}


QString ExperimentalWindPressures::getComponentAnalysisList()
{
    QString stringList;

    std::set<int>::iterator it;
    for (it = selectedComponentIDs.begin(); it != selectedComponentIDs.end(); ++it)
    {
        int first = *it;
        int next = *it;
        while(std::next(it, 1) != selectedComponentIDs.end() && *std::next(it, 1) == *it +1 )
        {
            next = *std::next(it, 1);
            ++it;
        }

        if(next == first)
        {
            stringList.append(QString::number(*it)+",");
        }
        else
        {
            stringList.append(QString::number(first)+"-"+QString::number(next)+",");
        }
    }

    // Remove the last comma
    stringList.truncate(stringList.lastIndexOf(QChar(',')));

    return stringList;
}

*/
