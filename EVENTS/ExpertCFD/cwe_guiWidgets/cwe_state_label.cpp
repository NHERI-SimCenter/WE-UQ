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

#include "cwe_state_label.h"

#include "CFDanalysis/CFDcaseInstance.h"

cwe_state_label::cwe_state_label(QWidget *parent) : QLabel(parent)
{
    this->setText("No Case Selected");
}

void cwe_state_label::setCurrentCase(CFDcaseInstance * newCase)
{
    if (currentCase != NULL)
    {
        QObject::disconnect(currentCase, 0, this, 0);
    }
    currentCase = newCase;
    if (currentCase == NULL)
    {
        this->setText("No Case Selected");
        return;
    }
    QObject::connect(currentCase, SIGNAL(haveNewState(CaseState)),
                    this, SLOT(setNewState(CaseState)));
    CaseState currentState = currentCase->getCaseState();
    setNewState(currentState);
}

void cwe_state_label::setNewState(CaseState newState)
{
    if (newState == CaseState::RUNNING)
    {
        this->setText("Running Remote Task . . . Please Wait");
        return;
    }
    if (newState == CaseState::OP_INVOKE)
    {
        this->setText("Updating remote files . . . Please Wait");
        return;
    }
    if (newState == CaseState::DOWNLOAD)
    {
        this->setText("Downloading remote case . . . Please Wait");
        return;
    }
    if (newState == CaseState::DEFUNCT)
    {
        this->setText("No Case Selected");
        return;
    }
    if (newState == CaseState::ERROR)
    {
        this->setText("ERROR");
        return;
    }
    if (newState == CaseState::INVALID)
    {
        this->setText("No Case Selected");
        return;
    }
    if (newState == CaseState::LOADING)
    {
        this->setText("Getting remote data  . . . Please Wait");
        return;
    }
    if (newState == CaseState::EXTERN_OP)
    {
        this->setText("File Operation in Progress . . . Please Wait");
        return;
    }
    if (newState == CaseState::READY)
    {
        this->setText("Ready for user input");
        return;
    }
    if (newState == CaseState::READY_ERROR)
    {
        this->setText("Error in remote task process.");
        return;
    }
    if (newState == CaseState::OFFLINE)
    {
        this->setText("OFFLINE MODE");
        return;
    }
    if (newState == CaseState::PARAM_SAVE)
    {
        this->setText("Saving Parameters");
        return;
    }
    this->setText("ERROR");
}
