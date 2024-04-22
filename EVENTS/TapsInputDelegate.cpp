/* *****************************************************************************
Copyright (c) 2016-2021, The Regents of the University of California (Regents).
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
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
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

// Copied from AssetInputDelegate in R2D written by Stevan Gavrilovic

#include "TapsInputDelegate.h"

#include <QRegExpValidator>

#include <sstream>

TapsInputDelegate::TapsInputDelegate()
{
    // this->setMaximumWidth(1000);
    // this->setMinimumWidth(400);
    // this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    this->setPlaceholderText("e.g., 1, 3, 5-10, 12");

    // Create a regExp validator to make sure only '-' & ',' & ' ' & numbers are input
    QRegExp LERegExp ("((([0-9]*)|([0-9]+-[1-9][0-9]*))[ ]*,[ ]*)*([[0-9]+-[1-9][0-9]*|[0-9]*)");
    QRegExpValidator* LEValidator = new QRegExpValidator(LERegExp);
    this->setValidator(LEValidator);
}


int TapsInputDelegate::size()
{
    return selectedComponentIDs.size();
}


void TapsInputDelegate::clear()
{
    prevText.clear();
    selectedComponentIDs.clear();
    this->QLineEdit::clear();
}


void TapsInputDelegate::insertSelectedComponent(const int id)
{
    selectedComponentIDs.insert(id);

    // Reset the text on the line edit
    this->setText(this->getComponentAnalysisList());
}


void TapsInputDelegate::insertSelectedComponents(const QVector<int>& ids)
{
    for(auto&& id : ids)
        selectedComponentIDs.insert(id);

    // Reset the text on the line edit
    this->setText(this->getComponentAnalysisList());
}


void TapsInputDelegate::selectComponents()
{
    auto inputText = this->text();

    if(inputText==prevText)
        return;

    selectedComponentIDs.clear();

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
        // Handle the case where there is a range of Tapss separated by a '-'
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
                QString err = "Error in the range of Taps IDs provided in the Component Taps selection box";
                throw err;
                continue;
            }

            // Add the IDs to the set
            for(int ID = IDStart; ID<=IDEnd; ++ID)
                selectedComponentIDs.insert(ID);
        }
        else // Taps ID is given individually
        {
            auto ID = std::stoi(subStr);

            selectedComponentIDs.insert(ID);
        }
    }

    // Reset the text on the line edit
    auto sortedIds = this->getComponentAnalysisList();
    this->setText(sortedIds);

    prevText=sortedIds;

    emit componentSelectionComplete();
}


std::set<int> TapsInputDelegate::getSelectedComponentIDs() const
{
    return selectedComponentIDs;
}


QString TapsInputDelegate::getComponentAnalysisList()
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

