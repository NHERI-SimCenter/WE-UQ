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

Executable to slice 3D SGF into 2D SGF"""
from __future__ import print_function

import sys

from CFDmods.ParseParts import SgfObjs


def give_help_message():
    """Prints help message and exits"""
    print("""Usage: slice2D [input geometry] [output geometry]\
[sliceHeight] [slicePlane] [sliceAngle]""")
    sys.exit(0)


def main(args):
    """Main function of geometry slicer"""
    if len(args) < 4:
        give_help_message()

    try:
        slice_height = float(sys.argv[3])
    except ValueError:
        print("Slice height should be a number.")
        sys.exit(-1)

    slice_angle = 0.0
    slice_plane = 'z'

    if len(args) >= 5:
        if args[4] == 'z':
            slice_plane = 'z'
        elif args[4] == 'y':
            slice_plane = 'y'
        elif args[4] == 'x':
            slice_plane = 'x'
        else:
            print("Slice plane should be x,y or z.")
            sys.exit(-1)

    if len(sys.argv) >= 6:
        try:
            slice_angle = float(sys.argv[5])
        except ValueError:
            print("Slice angle should be a number.")
            sys.exit(-1)

    in_geo = SgfObjs.SGFFileData()
    in_geo.load_data_from_json(sys.argv[1])
    in_geo.perform_slice(slice_plane, slice_height, slice_angle)

    in_geo.emit_json_geometry(args[2])

    print("Success: Object Sliced")


if __name__ == '__main__':
    main(sys.argv)
