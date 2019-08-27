"""
Copyright (c) 2018 The University of Notre Dame
Copyright (c) 2018 The Regents of the University of California

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this
list of conditions and the following disclaimer in the documentation and/or other
materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may
be used to endorse or promote products derived from this software without specific
prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.

Contributors:
Written by Peter Sempolinski, for the Natural Hazard Modeling Laboratory, director: Ahsan Kareem, at Notre Dame

Executable for conversion from raw geometry to GEO file"""
from __future__ import print_function

import sys

from CFDmods.GmshUtil import GmshGeo
from CFDmods.ParseParts import SgfObjs


def give_help_message():
    """Prints help text and exits"""
    print("Usage: sf2geo [geometry input] [output] [optional boundary input]")
    sys.exit(0)

def main(args):
    """Main procedure for data2geo program"""
    if len(args) < 3:
        give_help_message()

    in_file = SgfObjs.SGFFileData()
    in_file.load_data_from_file(args[1])

    geo_file_data = GmshGeo.GeoInstance()
    geo_file_data.geo_info = in_file

    if len(args) > 3:
        geo_file_data.set_boundary_info(args[3])
    else:
        geo_file_data.set_boundary_default()

    geo_file_data.emit_geo_file(args[2])

    print("Success: Mesh Generated")

if __name__ == '__main__':
    main(sys.argv)
