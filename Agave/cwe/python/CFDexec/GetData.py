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

Executable to extract structural data from files into SGF"""
from __future__ import print_function

import sys
from CFDmods.ParseParts import SgfObjs

def main(args):
    """Main function of data parser"""
    if len(args) < 3:
        print("Usage: getData [input] [output]")
        sys.exit(0)

    blank_geo_data = SgfObjs.SGFFileData()

    try:
        blank_geo_data.load_data_from_file(args[1])
    except IOError:
        print("Unable to read inputted geomentry file.")
        sys.exit(-1)

    try:
        blank_geo_data.emit_json_geometry(args[2])
    except IOError:
        print("Unable to write new geomentry file.")
        sys.exit(-1)

    print(("Geometry parsed and written to:" + args[2]))

if __name__ == '__main__':
    main(sys.argv)
