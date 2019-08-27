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

Classes and methods to convert from MSH (for gmsh 3.*) file to OpenFOAM(TM) mesh
Note: This project is and all code contributiors are entirely distinct from
OpenFOAM."""
from __future__ import print_function

import sys
import gzip
import json

from CFDmods.GmshUtil.GmshGeo import BoundaryInfo

TOLERANCE = 0.0000001


class MshVector(object):
    """Class for vector arithmetic. Use to make points clockwise for openFOAM"""
    def __init__(self, input_val):
        if isinstance(input_val, list):
            self.xcord = input_val[0]
            self.ycord = input_val[1]
            self.zcord = input_val[2]
        elif isinstance(input_val, MshVert):
            self.xcord = input_val.xval
            self.ycord = input_val.yval
            self.zcord = input_val.zval
        elif isinstance(input_val, MshVector):
            self.xcord = input_val.xcord
            self.ycord = input_val.ycord
            self.zcord = input_val.zcord

    def vec_subtract(self, vec_other):
        """Performs vector subtraction"""
        newz = self.zcord - vec_other.zcord
        newy = self.ycord - vec_other.ycord
        newx = self.xcord - vec_other.xcord
        return MshVector([newx, newy, newz])

    def dot_product(self, other_vector):
        """Returns scalar (dot) product between self and other vector"""
        ret = self.xcord * other_vector.xcord
        ret += self.ycord * other_vector.ycord
        ret += self.zcord * other_vector.zcord
        return ret

    def cross_prod(self, other_vector):
        """Return vector (cross) product of self and other vector"""
        new_coords = [0.0, 0.0, 0.0]
        new_coords[0] = (self.ycord * other_vector.zcord) - (self.zcord * other_vector.ycord)
        new_coords[1] = (self.zcord * other_vector.xcord) - (self.xcord * other_vector.zcord)
        new_coords[2] = (self.xcord * other_vector.ycord) - (self.ycord * other_vector.xcord)
        return MshVector(new_coords)

    def __str__(self):
        return "(%f, %f, %f)" % (self.xcord, self.ycord, self.zcord)


class MshVert(object):
    """Class for MSH point"""
    def __init__(self, coord_list, idNum):
        self.ref_element = []
        self.mirror_point = None

        self.idnum = idNum
        self.xval = 0.0
        self.yval = 0.0
        self.zval = 0.0

        try:
            if len(coord_list) > 0:
                self.xval = float(coord_list[0])
            if len(coord_list) > 1:
                self.yval = float(coord_list[1])
            if len(coord_list) > 2:
                self.zval = float(coord_list[2])
        except ValueError:
            print("ERROR: Coordinates must be floating point")
            sys.exit(2)

    def clone_point_up(self, z_val, new_id):
        """Clones point on other z plane for 2D meshes"""
        new_list = [self.xval, self.yval, z_val]
        self.mirror_point = MshVert(new_list, new_id)
        return self.mirror_point

    def has_element(self, to_find):
        """Return True if this object has a containing OpenFoam owner element"""
        for an_element in self.ref_element:
            if to_find is an_element:
                return True
        return False

    def __str__(self):
        return "(%f, %f, %f)" % (self.xval, self.yval, self.zval)


class MshElement(object):
    """Class for MSH element"""
    def __init__(self, raw_tokens, all_points, id_num):
        self.point_list = []
        self.my_faces = []
        self.idnum = id_num

        if len(raw_tokens) < 3:
            print("ERROR: element line format error")
            sys.exit(5)

        if int(raw_tokens[1]) == 2:
            self.dimension = 2
        elif int(raw_tokens[1]) == 4:
            self.dimension = 3
        else:
            print("ERROR: element line type not recognized")
            sys.exit(6)

        num_tags = int(raw_tokens[2])
        start_index = 3 + num_tags

        for item in raw_tokens[start_index:]:
            try:
                #Note: This will not work if msh file is 0 base element
                #or not consecutive indexes
                self.point_list.append(all_points[int(item) - 1])
            except ValueError:
                print("ERROR: vert indexes not as expected")
                sys.exit(2)
            except IndexError:
                print("ERROR: Vert Name not defined in face list")
                sys.exit(2)
        for a_point in self.point_list:
            a_point.ref_element.append(self)
        self.checked = False

    def setup_of_faces(self, settings):
        """Creates OpenFOAM faces from MSH element"""
        if self.dimension != settings.dimension:
            print("ERROR: Dimensional mismatch")
            sys.exit(3)

        if self.dimension == 2:
            return self.setup_2d_of_faces(settings)

        if self.dimension == 3:
            return self.setup_3d_of_faces(settings)

        print("ERROR: Dimension not recognized")
        sys.exit(4)

    def setup_2d_of_faces(self, settings):
        """Creates OpenFOAM faces from MSH element for 2D mesh"""
        new_faces = []

        high_list = []
        low_list = []
        for a_point in self.point_list:
            high_list.append(a_point.mirror_point)
            low_list.append(a_point)

        high_face = self.get_vert_of_face(high_list, settings, low_list[0])
        if high_face is not False:
            new_faces.append(high_face)

        low_face = self.get_vert_of_face(low_list, settings, high_list[0])
        if low_face is not False:
            new_faces.append(low_face)

        num_points = len(self.point_list)
        for i in range(num_points):
            tmp_point_list = []
            tmp_point_list.append(self.point_list[i - 1])
            tmp_point_list.append(self.point_list[i])
            tmp_point_list.append(self.point_list[i].mirror_point)
            tmp_point_list.append(self.point_list[i - 1].mirror_point)

            itr = 0
            check_point = self.point_list[itr]
            while (check_point is self.point_list[i - 1]) or \
                  (check_point is self.point_list[i]):
                itr += 1
                check_point = self.point_list[itr]

            tmp = self.get_vert_of_face(tmp_point_list, settings, check_point)
            if tmp is not False:
                new_faces.append(tmp)

        self.my_faces = self.upper_triangle_order(new_faces)

        self.checked = True

        return self.my_faces

    def setup_3d_of_faces(self, settings):
        """Creates OpenFOAM faces from MSH element for 3D mesh"""
        new_faces = []
        for i in range(len(self.point_list)):
            tmp_point_list = []

            for j in range(len(self.point_list)):
                if i != j:
                    tmp_point_list.append(self.point_list[j])

            tmp = self.get_vert_of_face(tmp_point_list, settings, self.point_list[i])
            if tmp is not False:
                new_faces.append(tmp)

        self.my_faces = self.upper_triangle_order(new_faces)

        self.checked = True

        return self.my_faces

    def get_vert_of_face(self, point_list, settings, clock_ref):
        """Returns OpenFOAM face for given point list. Creates one if not already extant"""
        found = True
        for point in point_list:
            if point.xval > settings.xmin + TOLERANCE:
                found = False

        if found is True:
            return OfFace(point_list, self, "inlet", clock_ref)

        found = True
        for point in point_list:
            if point.xval < settings.xmax - TOLERANCE:
                found = False

        if found is True:
            return OfFace(point_list, self, "outlet", clock_ref)

        found = True
        for point in point_list:
            if point.yval > settings.ymin + TOLERANCE:
                found = False

        if found is True:
            return OfFace(point_list, self, "lowYPlane", clock_ref)

        found = True
        for point in point_list:
            if point.yval < settings.ymax - TOLERANCE:
                found = False

        if found is True:
            return OfFace(point_list, self, "highYPlane", clock_ref)

        if settings.dimension == 3:
            found = True
            for point in point_list:
                if point.zval > settings.zmin + TOLERANCE:
                    found = False

            if found is True:
                return OfFace(point_list, self, "lowZPlane", clock_ref)

            found = True
            for point in point_list:
                if point.zval < settings.zmax - TOLERANCE:
                    found = False

            if found is True:
                return OfFace(point_list, self, "highZPlane", clock_ref)
        else:
            found = True
            for point in point_list:
                if point.zval > 0.1:
                    found = False

            if found is True:
                return OfFace(point_list, self, "lowZPlane", clock_ref)

            found = True
            for point in point_list:
                if point.zval < 0.1:
                    found = False

            if found is True:
                return OfFace(point_list, self, "highZPlane", clock_ref)

        face_check_list = point_list[0].ref_element[:]
        face_check_list.remove(self)

        if self.dimension == 2:
            verify_points = []
            for point in point_list:
                if point.zval < 0.1:
                    verify_points.append(point)
        else:
            verify_points = point_list

        for point in verify_points:
            for face in face_check_list[:]:
                if point.has_element(face) is False:
                    face_check_list.remove(face)

        if len(face_check_list) != 1:
            return OfFace(point_list, self, None, clock_ref)

        other_element = face_check_list[0]

        if other_element.checked is False:
            new_face = OfFace(point_list, self, None, clock_ref)
            new_face.my_neighbor = other_element
            new_face.has_neighbor = True
            return new_face

        return False

    def upper_triangle_order(self, face_list):
        """Puts inputted face list in upper triangle order: ordered by neighbor index"""
        ret = []

        tmp_list = []

        for a_face in face_list:
            if a_face.has_neighbor is True:
                tmp_list.append(a_face)
            else:
                ret.append(a_face)

        if len(tmp_list) == 0:
            return ret

        if len(tmp_list) == 1:
            ret.append(tmp_list[0])
            return ret

        while len(tmp_list) > 1:
            lowest = tmp_list[0]
            for a_face in tmp_list:
                if a_face.my_neighbor.idnum < lowest.my_neighbor.idnum:
                    lowest = a_face
            ret.append(lowest)
            tmp_list.remove(lowest)

        if len(tmp_list) == 1:
            ret.append(tmp_list[0])
        return ret


class OfFace(object):
    """Class for an OpenFoam Face element"""
    def __init__(self, point_list, owner, block, clock_ref):
        self.point_list = []
        for point in point_list:
            self.point_list.append(point)

        self.owner = owner
        self.defunct = False

        self.has_neighbor = False
        self.my_block = block

        self.my_neighbor = None

        #Now check that the points are arranged relative to reference point
        vec1 = MshVector(self.point_list[0])
        vec2 = MshVector(self.point_list[1])
        vec3 = MshVector(self.point_list[2])

        tmp1 = vec2.vec_subtract(vec1)
        tmp2 = vec3.vec_subtract(vec2)

        vec_cross = tmp1.cross_prod(tmp2)
        vec_ref = MshVector(clock_ref)

        vec_compare = vec_ref.vec_subtract(vec1)

        if vec_cross.dot_product(vec_compare) > 0.0:
            self.point_list.reverse()


    def emit_points(self):
        """Returns string containing list of point ids"""
        ret = str(len(self.point_list))
        ret = ret + "("
        for a_point in self.point_list:
            ret = ret + str(a_point.idnum) + " "
        ret = ret + ")\n"
        return ret


class MshInfo(object):
    """Class for reading, parsing and storing raw MSH data for later conversion"""
    MODE_NIL = 0
    MODE_VERTS = 1
    MODE_VERTS_NUM = 2
    MODE_FACES = 3
    MODE_FACES_NUM = 4

    def __init__(self, fileName):
        try:
            file_handle = open(fileName, "r")
            file_lines = file_handle.readlines()
            file_handle.close()
        except IOError:
            print("ERROR: Unable to open input file")
            sys.exit(1)

        in_mode = MshInfo.MODE_NIL

        self.vert_list = []
        self.element_list = []

        virt_itr = 0
        face_itr = 0

        for line in file_lines:
            if line == '$Nodes\n':
                in_mode = MshInfo.MODE_VERTS_NUM
            elif line == '$Elements\n':
                in_mode = MshInfo.MODE_FACES_NUM
            elif in_mode is MshInfo.MODE_VERTS_NUM:
                in_mode = MshInfo.MODE_VERTS
                self.vert_list = [None] * int(line)
            elif in_mode is MshInfo.MODE_FACES_NUM:
                in_mode = MshInfo.MODE_FACES
                self.element_list = [None] * int(line)
            else:
                tokens = line.split()
                if (in_mode is MshInfo.MODE_VERTS) and (len(tokens) > 2):
                    self.vert_list[virt_itr] = MshVert(tokens[1:], virt_itr)
                    virt_itr += 1
                if (in_mode is MshInfo.MODE_FACES) and (len(tokens) > 3):
                    self.element_list[face_itr] = MshElement(tokens,
                                                    self.vert_list, face_itr)
                    face_itr += 1


class OfBoundaryInfo(object):
    """Class for storing boundary info of MSH to OF conversion"""
    def __init__(self, point_list):
        self.xmin = float("inf")
        self.xmax = -float("inf")
        self.ymin = float("inf")
        self.ymax = -float("inf")
        self.zmin = float("inf")
        self.zmax = -float("inf")

        for point in point_list:
            if point.xval < self.xmin:
                self.xmin = point.xval
            if point.xval > self.xmax:
                self.xmax = point.xval
            if point.yval < self.ymin:
                self.ymin = point.yval
            if point.yval > self.ymax:
                self.ymax = point.yval
            if point.zval < self.zmin:
                self.zmin = point.zval
            if point.zval > self.zmax:
                self.zmax = point.zval

        if (self.zmax - self.zmin) < TOLERANCE:
            self.dimension = 2
        else:
            self.dimension = 3


class OfMeshInstance(object):
    """Class for converted OpenFOAM mesh instance"""
    def __init__(self, parsed_msh_info):
        self.msh_info = parsed_msh_info
        self.bound_info = OfBoundaryInfo(self.msh_info.vert_list)
        self.dimension = self.bound_info.dimension

        self.bound_params = None

    def set_boundary_params(self, file_name):
        """Retrives boundary info from a file and sets boundary paramters"""
        try:
            file_handle = open(file_name, "r")
        except IOError:
            print("ERROR: Unable to open setting file")
            sys.exit(6)
        raw_boundaries = json.load(file_handle)

        self.bound_params = BoundaryInfo([], raw_boundaries)

    def set_boundary_default(self):
        """Sets boundary solely from default values"""
        self.bound_params = BoundaryInfo([], {})

    def emit_open_foam_files(self, out_file_folder):
        """Outputs OpenFOAM mesh files into out_file_folder"""
        if self.dimension == 2:
            new_list = []
            big_itr = len(self.msh_info.vert_list)
            for a_point in self.msh_info.vert_list:
                new_list.append(a_point.clone_point_up(1.0, big_itr))
                big_itr += 1
            self.msh_info.vert_list.extend(new_list)

        all_offaces = []

        sys.stdout.write("Progress: 0")
        num_ele = len(self.msh_info.element_list)
        i = 1
        percent_target = 5
        twentith = num_ele / 20
        steps = twentith
        for an_element in self.msh_info.element_list:
            i += 1
            if i > steps:
                sys.stdout.write("...%d" % percent_target)
                percent_target += 5
                steps += twentith

            tmp_list = an_element.setup_of_faces(self.bound_info)
            if tmp_list is not False:
                all_offaces.extend(tmp_list)

        sys.stdout.write("\n")

        face_count = len(all_offaces)
        neighbor_count = 0
        for a_face in all_offaces:
            if a_face.has_neighbor is True:
                neighbor_count += 1
            elif a_face.my_block is None:
                a_face.my_block = "object"

        try:
            point_handle = gzip.open(out_file_folder + "/points.gz", "w")
            owner_handle = gzip.open(out_file_folder + "/owner.gz", "w")
            faces_handle = gzip.open(out_file_folder + "/faces.gz", "w")
            boundary_handle = gzip.open(out_file_folder + "/boundary.gz", "w")
            neighbour_handle = gzip.open(out_file_folder + "/neighbour.gz", "w")
        except IOError:
            print("ERROR: Unable to open output file")
            sys.exit(3)

        point_handle.write("FoamFile\n")
        point_handle.write("{\nversion  2.0;\n")
        point_handle.write("format  ascii;\nclass  vectorField;\n")
        point_handle.write("location  \"constant/polyMesh\";\n")
        point_handle.write("object  points;\n")
        point_handle.write("}\n\n")

        owner_handle.write("FoamFile\n")
        owner_handle.write("{\nversion  2.0;\n")
        owner_handle.write("format  ascii;\n")
        owner_handle.write("class  labelList;\n")
        owner_handle.write("location  \"constant/polyMesh\";\nobject  owner;\n")
        owner_handle.write("}\n\n")

        faces_handle.write("FoamFile\n")
        faces_handle.write("{\nversion  2.0;\n")
        faces_handle.write("format  ascii;\n")
        faces_handle.write("class  faceList;\n")
        faces_handle.write("location  \"constant/polyMesh\";\n")
        faces_handle.write("object  faces;\n")
        faces_handle.write("}\n\n")

        boundary_handle.write("FoamFile\n")
        boundary_handle.write("{\nversion  2.0;\n")
        boundary_handle.write("format  ascii;\n")
        boundary_handle.write("class  polyBoundaryMesh;\n")
        boundary_handle.write("location  \"constant/polyMesh\";\n")
        boundary_handle.write("object  boundary;\n")
        boundary_handle.write("}\n\n")

        neighbour_handle.write("FoamFile\n")
        neighbour_handle.write("{\nversion  2.0;\n")
        neighbour_handle.write("format  ascii;\n")
        neighbour_handle.write("class  labelList;\n")
        neighbour_handle.write("location  \"constant/polyMesh\";\n")
        neighbour_handle.write("object  neighbour;\n")
        neighbour_handle.write("}\n\n")

        point_handle.write(str(len(self.msh_info.vert_list)) + "\n(\n")
        for a_point in self.msh_info.vert_list:
            point_handle.write("(")
            point_handle.write(str(a_point.xval))
            point_handle.write(" ")
            point_handle.write(str(a_point.yval))
            point_handle.write(" ")
            point_handle.write(str(a_point.zval))
            point_handle.write(")\n")
        point_handle.write(")\n")
        point_handle.close()

        owner_handle.write(str(face_count) + "\n(\n")
        faces_handle.write(str(face_count) + "\n(\n")
        neighbour_handle.write(str(neighbor_count) + "\n(\n")
        boundary_handle.write("7\n(\n")

        inlet_count = 0
        outlet_count = 0
        high_ycount = 0
        low_ycount = 0
        low_zcount = 0
        high_zcount = 0
        object_count = 0

        internal_faces = []
        external_faces = []

        for a_face in all_offaces:
            if a_face.has_neighbor is True:
                internal_faces.append(a_face)
            else:
                external_faces.append(a_face)
                if a_face.my_block == "inlet":
                    inlet_count += 1
                elif a_face.my_block == "outlet":
                    outlet_count += 1
                elif a_face.my_block == "highYPlane":
                    high_ycount += 1
                elif a_face.my_block == "lowYPlane":
                    low_ycount += 1
                elif a_face.my_block == "lowZPlane":
                    low_zcount += 1
                elif a_face.my_block == "highZPlane":
                    high_zcount += 1
                elif a_face.my_block == "object":
                    object_count += 1

        for a_face in internal_faces:
            owner_handle.write(str(a_face.owner.idnum) + "\n")
            neighbour_handle.write(str(a_face.my_neighbor.idnum) + "\n")
            faces_handle.write(a_face.emit_points())

        neighbour_handle.write(")\n")
        neighbour_handle.close()

        running_count = neighbor_count
        bound_entry = OfMeshEdgeInfo("object", "wall", object_count, neighbor_count)
        bound_entry.emit_boundary_entry(external_faces, boundary_handle, owner_handle, faces_handle)
        running_count += object_count

        bound_entry = OfMeshEdgeInfo("inlet", "patch", inlet_count, running_count)
        bound_entry.emit_boundary_entry(external_faces, boundary_handle, owner_handle, faces_handle)
        running_count += inlet_count

        bound_entry = OfMeshEdgeInfo("outlet", "patch", outlet_count, running_count)
        bound_entry.emit_boundary_entry(external_faces, boundary_handle, owner_handle, faces_handle)
        running_count += outlet_count

        wall_type = self.translate_boundary_type(self.bound_params.low_yplane)
        bound_entry = OfMeshEdgeInfo("highYPlane", wall_type, high_ycount, running_count)
        bound_entry.emit_boundary_entry(external_faces, boundary_handle, owner_handle, faces_handle)
        running_count += high_ycount

        wall_type = self.translate_boundary_type(self.bound_params.high_yplane)
        bound_entry = OfMeshEdgeInfo("lowYPlane", wall_type, low_ycount, running_count)
        bound_entry.emit_boundary_entry(external_faces, boundary_handle, owner_handle, faces_handle)
        running_count += low_ycount

        if self.dimension == 2:
            wall_type = "empty"
        else:
            wall_type = self.translate_boundary_type(self.bound_params.low_zplane)

        bound_entry = OfMeshEdgeInfo("lowZPlane", wall_type, low_zcount, running_count)
        bound_entry.emit_boundary_entry(external_faces, boundary_handle, owner_handle, faces_handle)
        running_count += low_zcount

        if self.dimension == 2:
            wall_type = "empty"
        else:
            wall_type = self.translate_boundary_type(self.bound_params.high_zplane)

        bound_entry = OfMeshEdgeInfo("highZPlane", wall_type, high_zcount, running_count)
        bound_entry.emit_boundary_entry(external_faces, boundary_handle, owner_handle, faces_handle)

        owner_handle.write(")\n")
        faces_handle.write(")\n")
        boundary_handle.write(")\n")

        owner_handle.close()
        faces_handle.close()
        boundary_handle.close()

    def translate_boundary_type(self, boundary_str):
        """Translates an internal boundary designation into a real boundary string"""
        if boundary_str == "SYM_PLANE":
            return "symmetryPlane"
        if boundary_str == "WALL":
            return "wall"
        if boundary_str == "EMPTY":
            return "empty"
        return "symmetryPlane"


class OfMeshEdgeInfo(object):
    """Class for edges of Of mesh: boundary and object"""
    def __init__(self, obj_name, obj_type, obj_count, obj_start):
        self.obj_name = obj_name
        self.obj_type = obj_type
        self.obj_count = obj_count
        self.obj_start = obj_start

    def emit_boundary_entry(self, all_of_faces, boundary_handle, owner_handle, faces_handle):
        """Prints this boundary info to given files"""
        boundary_handle.write(self.obj_name + "\n{\n")
        boundary_handle.write("type  " + self.obj_type + ";\n")
        boundary_handle.write("nFaces  " + str(self.obj_count) + ";\n")
        boundary_handle.write("startFace  " + str(self.obj_start) + ";\n}\n")
        for a_face in all_of_faces:
            if ((a_face.defunct is False)
                and (a_face.has_neighbor is False)
                and (a_face.my_block == self.obj_name)):
                owner_handle.write(str(a_face.owner.idnum) + "\n")
                faces_handle.write(a_face.emit_points())

