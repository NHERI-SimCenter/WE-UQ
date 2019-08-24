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

#ifndef SCTRSTATES_H
#define SCTRSTATES_H

enum class SimCenterViewState { visible,
                                editable,
                                hidden };

/*
 * we want to use this enum as binary selector
 *
 * use: SimCenterButtonMode_ALL == SimCenterButtonMode_RUN|SimCenterButtonMode_CANCEL
 *                                     |SimCenterButtonMode_RESULTS|SimCenterButtonMode_RESET
 *
 * While this enum class is initially used for the 4 buttons in the CWE_TabWidget,
 * it may be extended to more, maybe all buttons of the CWE tool.
 */
#define SimCenterButtonMode  std::uint32_t

/*
 * SimCenterButtonMode_NONE      0000 0000 0000 0000
 * SimCenterButtonMode_RUN       0000 0000 0000 0001
 * SimCenterButtonMode_CANCEL    0000 0000 0000 0010
 * SimCenterButtonMode_RESET     0000 0000 0000 0100
 * SimCenterButtonMode_RESULTS   0000 0000 0000 1000
 * SimCenterButtonMode_ALL       0000 0001 0000 1111
 * SimCenterButtonMode_SAVE_ALL  0000 0001 0000 0000
 */

#define SimCenterButtonMode_NONE      0x0000u
#define SimCenterButtonMode_RUN       0x0001u
#define SimCenterButtonMode_CANCEL    0x0002u
#define SimCenterButtonMode_RESET     0x0004u
#define SimCenterButtonMode_RESULTS   0x0008u
#define SimCenterButtonMode_ALL       0x010fu
#define SimCenterButtonMode_SAVE_ALL  0x0100u

enum class SimCenterDataType { integer,
                               floatingpoint,
                               boolean,
                               string,
                               selection,
                               file,
                               tensor2D,
                               tensor3D,
                               vector2D,
                               vector3D,
                               unknown};

#endif // SCTRSTATES_H
