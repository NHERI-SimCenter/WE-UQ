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

A set of objects useful for parsing geometry files"""
from __future__ import print_function

import sys
import math

MARGIN = 0.0000001

MODE_NIL = 0
MODE_VERTS = 1
MODE_FACES = 2
MODE_VOLS = 3

class SGFvert(object):
    """Class for a vertex in sgf data"""

    def __init__(self, itemList):
        self.xval = float(itemList[0])
        self.yval = float(itemList[1])
        self.zval = 0.0
        self.is_spline = False
        self.in_use = False
        self.dim = 2
        if len(itemList) >= 3:
            self.zval = float(itemList[2])
            self.dim = 3
        self.idstr = "ERR"

        self.ref = []

    def set_spline(self):
        """Sets this point as a spline point"""
        self.is_spline = True

    def is_point(self, other_point):
        """Compares this point to another point"""
        if self.dim != other_point.dim:
            return False
        if self.param_same(self.xval, other_point.xval) is False:
            return False
        if self.param_same(self.yval, other_point.yval) is False:
            return False
        if self.param_same(self.zval, other_point.zval) is False:
            return False
        return True

    def param_same(self, val1, val2):
        """Compares two floating point coords to determine if they are the same"""
        if val1 > val2 + MARGIN:
            return False
        if val1 < val2 - MARGIN:
            return False
        return True

    def in_line_with(self, point1, point2):
        """Return true if self is on a line segment between point1 and point2 in 2D"""
        if self.param_same(self.yval, point1.yval):
            if self.param_same(self.yval, point2.yval):
                return True
            else:
                return False
        if self.param_same(self.xval, point1.xval):
            if self.param_same(self.xval, point2.xval):
                return True
            else:
                return False

        rise = point2.yval - point1.yval
        run = point2.xval - point1.xval
        factor = (self.xval - point1.xval) / run
        checkval = point1.yval + factor * rise

        if checkval > self.yval + MARGIN:
            return False
        if checkval < self.yval - MARGIN:
            return False
        return True

    def point_dist(self, other):
        """Returns distance to other point"""
        xdiff = other.xval - self.xval
        ydiff = other.yval - self.yval
        zdiff = other.zval - self.zval
        ret = xdiff * xdiff + ydiff * ydiff + zdiff * zdiff
        return math.sqrt(ret)

    def __str__(self):
        if self.dim == 2:
            return "(%f, %f)" % (self.xval, self.yval)
        return "(%f, %f, %f)" % (self.xval, self.yval, self.zval)


class SGFface(object):
    """Class for a face in sgf data"""
    def __init__(self, item_list, all_points):
        self.point_list = []
        self.idstr = "ERR"
        for item in item_list:
            if isinstance(item, (str, unicode)):
                try:
                    self.point_list.append(all_points[item])
                except IndexError:
                    print("ERROR: Vert Name not defined in face list")
                    sys.exit(2)
            else:
                self.point_list.append(item)


class SGFvol(object):
    """Class for a volumne in sgf data"""
    def __init__(self, item_list, all_faces):
        self.face_list = []
        self.idstr = "ERR"
        for item in item_list:
            if isinstance(item, (str, unicode)):
                try:
                    self.face_list.append(all_faces[item])
                except IndexError:
                    print("ERROR: Face Name not defined in vols list")
                    sys.exit(2)
            else:
                self.face_list.append(item)


class SGFsegment(object):
    """Data of a 2-point segment"""
    def __init__(self, point_list):
        self.points = point_list
        self.used = False

    def __str__(self):
        if len(self.points) == 2:
            return "(%s <=> %s)" % (self.points[0], self.points[1])
        return "(X <=> X)"


class Triangle(object):
    """Class stores a triangle pf points"""
    def __init__(self, a, b, c, num):
        self.num = num
        self.pta = a
        self.ptb = b
        self.ptc = c
        self.marked = -1


def doesnotcontain(pointlist, needle):
    """Returns true if no point in point list has same coords as needle"""
    for point in pointlist:
        if point.is_point(needle):
            return False
    return True


def containspoint(pointlist, needle):
    """Returns a point from the point list if it has the same coords as needle"""
    for point in pointlist:
        if point.is_point(needle):
            return point
    return None


def recenter_object(vert_list):
    """Recenters the verts in vert_list"""
    minz = float(vert_list[0].zval)
    minx = float(vert_list[0].xval)
    maxx = float(vert_list[0].xval)
    miny = float(vert_list[0].yval)
    maxy = float(vert_list[0].yval)

    for vert in vert_list:
        if float(vert.xval) < minx:
            minx = float(vert.xval)
        if float(vert.xval) > maxx:
            maxx = float(vert.xval)
        if float(vert.zval) < minz:
            minz = float(vert.zval)
        if float(vert.yval) < miny:
            miny = float(vert.yval)
        if float(vert.yval) > maxy:
            maxy = float(vert.yval)

    midx = ((maxx - minx) / 2) + minx
    midy = ((maxy - miny) / 2) + miny

    for vert in vert_list:
        vert.xval = vert.xval - midx
        vert.yval = vert.yval - midy
        vert.zval = vert.zval - minz


def get_file_lines(filename):
    """Return a list of lines from a given file name"""
    try:
        file_handle = open(filename, "r")
        file_lines = file_handle.readlines()
        file_handle.close()
    except IOError:
        print("ERROR: Unable to open input file")
        sys.exit(1)

    return file_lines


def parse_obj(file_lines, geo_object):
    """Parses some OBJ files into SGF data. So long as the faces are trianges.
    Assumes mm dimension from FreeCAD, converts to meters."""
    triangle_list = []
    vert_list = {}

    point_itr = 1

    for line in file_lines:
        if line[0] == '#':
            continue

        line_parts = line.split()
        if len(line_parts) < 4:
            continue

        if line_parts[0] == "v":
            cord_list = []
            for entry in range(1, 4):
                data = float(line_parts[entry])
                data /= 1000
                cord_list.append(data)

            temp_point = SGFvert(cord_list)
            apoint = containspoint(vert_list.values(), temp_point)
            if apoint is None:
                vert_list[str(point_itr)] = temp_point
            else:
                vert_list[str(point_itr)] = apoint
            point_itr += 1
            continue

        if line_parts[0] == "f":
            temp_face = Triangle(vert_list[extract_prefix_int_from_str(line_parts[1])],
                                 vert_list[extract_prefix_int_from_str(line_parts[2])],
                                 vert_list[extract_prefix_int_from_str(line_parts[3])], 0)
            triangle_list.append(temp_face)
            continue

    geo_object.idstr = "objTranslate"
    geo_object.dimension = 3

    itr = 0
    for point in vert_list.values():
        geo_object.vert_list[str(itr)] = point
        itr += 1

    itr = 0
    for itr in range(0, len(triangle_list)):
        new_face = SGFface([triangle_list[itr].pta,
                            triangle_list[itr].ptb,
                            triangle_list[itr].ptc], geo_object.vert_list)
        geo_object.face_list[str(itr)] = new_face
        itr += 1

    newvol = SGFvol(geo_object.face_list, geo_object.face_list)
    geo_object.vols_list["0"] = newvol

    for vertkey, vert in list(geo_object.vert_list.items()):
        vert.idstr = vertkey

    for facekey, face in list(geo_object.face_list.items()):
        face.idstr = facekey

    for volkey, vol in list(geo_object.vols_list.items()):
        vol.idstr = volkey

    #re-center the object
    recenter_object(geo_object.vert_list.values())


def extract_prefix_int_from_str(a_str):
    """Gets an int from the string, taking only the char before a non digit char"""
    ret_str = ""
    for a_char in a_str:
        if a_char.isdigit():
            ret_str += a_char
        else:
            return ret_str
    return ret_str


def parse_old_sgf(file_lines, geo_object):
    """Parses Data from old version SGF"""
    have_first = False
    in_mode = MODE_NIL

    num_verts = 0
    num_faces = 0
    num_vols = 0

    for line in file_lines:
        tokens = line.split()
        if len(tokens) == 0:
            continue
        if tokens[0] == "#Vertices":
            if len(tokens) != 2:
                print("ERROR: Vertices sysntax error")
                sys.exit(2)
            try:
                num_verts = int(tokens[1])
            except ValueError:
                print("ERROR: Vertices syntax error not an int")
                sys.exit(2)
            in_mode = MODE_VERTS
        elif tokens[0] == "#Faces":
            if len(tokens) != 2:
                print("ERROR: Faces sysntax error")
                sys.exit(2)
            try:
                num_faces = int(tokens[1])
            except ValueError:
                print("ERROR: Faces sysntax error not an int")
                sys.exit(2)
            in_mode = MODE_FACES
        elif tokens[0] == "#Volumes":
            if len(tokens) != 2:
                print("ERROR: Vols sysntax error")
                sys.exit(2)
            try:
                num_vols = int(tokens[1])
            except ValueError:
                print("ERROR: Volumes sysntax error not an int")
                sys.exit(2)
            if geo_object.dimension != 3:
                print("ERROR: Volumes defined in 2D object")
                sys.exit(2)
            in_mode = MODE_VOLS

        elif (in_mode is MODE_VERTS) and (len(tokens) > 0):
            try:
                dim_val = int(tokens[1])
            except ValueError:
                print("ERROR: Dimension Read Error")
                sys.exit(2)
            if dim_val != geo_object.dimension:
                print("ERROR: Dimension Mismatch")
                sys.exit(2)
            if dim_val > len(tokens) - 2:
                print("ERROR: Dimension Mismatch in Vert")
                sys.exit(2)
            if tokens[0] in geo_object.vert_list:
                print("ERROR: Vert Name used twice")
                sys.exit(2)
            new_vert = SGFvert(tokens[2:(dim_val + 2)])
            geo_object.vert_list[tokens[0]] = new_vert
            new_vert.idstr = tokens[0]
            if dim_val < len(tokens) - 2:
                if tokens[dim_val + 2][0] == 'S':
                    geo_object.vert_list[tokens[0]].set_spline()

        elif (in_mode is MODE_FACES) and (len(tokens) > 0):
            try:
                dim_val = int(tokens[1])
            except ValueError:
                print("ERROR: Dimension Read Error")
                sys.exit(2)
            if dim_val != len(tokens) - 2:
                print("ERROR: Dimension Mismatch in Face")
                sys.exit(2)
            if tokens[0] in geo_object.face_list:
                print("ERROR: Volume Name used twice")
                sys.exit(2)
            new_face = SGFface(tokens[2:len(tokens)], geo_object.vert_list)
            geo_object.face_list[tokens[0]] = new_face
            new_face.idstr = tokens[0]

        elif (in_mode is MODE_VOLS) and (len(tokens) > 0):
            try:
                dim_val = int(tokens[1])
            except ValueError:
                print("ERROR: Dimension Read Error")
                sys.exit(2)
            if dim_val != len(tokens) - 2:
                print("ERROR: Dimension Mismatch in Volumes")
                sys.exit(2)
            if tokens[0] in geo_object.vols_list:
                print("ERROR: Face Name used twice")
                sys.exit(2)
            new_vol = SGFvol(tokens[2:len(tokens)], geo_object.face_list)
            geo_object.vols_list[tokens[0]] = new_vol
            new_vol.idstr = tokens[0]

        elif (have_first is False) and (len(tokens) == 2):
            try:
                geo_object.idstr = tokens[0]
                geo_object.dimension = int(tokens[1])
            except ValueError:
                print("ERROR: Invalid file dimension (use 2 or 3)")
                sys.exit(2)
            have_first = True
            if (geo_object.dimension != 2) and (geo_object.dimension != 3):
                print("ERROR: Invalid file dimension (use 2 or 3)")
                sys.exit(2)

    if len(geo_object.vert_list) != num_verts:
        print("ERROR: Number of verts not equal to number of vert entries")
        sys.exit(2)

    if len(geo_object.face_list) != num_faces:
        print("ERROR: Number of faces not equal to number of face entries")
        sys.exit(2)

    if len(geo_object.vols_list) != num_vols:
        print("ERROR: Number of volumes not equal to number of volume entries")
        sys.exit(2)
