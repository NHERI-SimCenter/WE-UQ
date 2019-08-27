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
// modified by Peter Mackenzie-Helnwein,UW Seattle

#ifndef CFD_GLOBALS_H
#define CFD_GLOBALS_H

#include <QMessageBox>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(agaveAppLayer)

class AgaveSetupDriver;
class RemoteDataInterface;
class FileOperator;
class JobOperator;

/*! \brief The cfd_globals are a set of static methods, intended as global functions for CFD in the WE-UQ programs.
 *
 *  These utility functions include error popups
 */

class cfd_globals
{
public:
    /*! \brief cfd_globals is a static class. The constructor should never be used.
     */
    cfd_globals();

    /*! \brief This will display an informational popup and kill the program.
     *
     *  \param message This is the content of the message displayed in the popup.
     *  \param header This short text appears in the header of the popup. The default is: "Critical Error"
     *
     *  After construction, use show() or exec() to display the window.
     */
    [[ noreturn ]] static void displayFatalPopup(QString message, QString header = "Critical Error");

    static void displayPopup(QString message, QString header = "Error");

private:
};

#endif // CFD_GLOBALS_H
