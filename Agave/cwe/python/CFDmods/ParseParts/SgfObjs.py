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

Classes and Methods to store and trandform SGF data"""
from __future__ import print_function

import sys
import math
import json

from CFDmods.ParseParts import ParseObjs


class SGFFileData(object):
    """Class to store data for SGF files"""
    def __init__(self):
        self.idstr = "ERROR"
        self.dimension = 3

        self.vert_list = {}
        self.face_list = {}
        self.vols_list = {}

    def load_data_from_file(self, filename):
        """Loads SGF data from JSON,SGF-old, STL or DAE file"""
        broken_name = filename.split('.')
        file_ext = broken_name[len(broken_name) - 1]

        if file_ext == "json":
            self.load_data_from_json(filename)
        #elif file_ext == "stl":
        #    ParseObjs.parse_stl(ParseObjs.get_file_lines(filename), self)
        #elif file_ext == "dae":
        #    ParseObjs.parse_dae(filename, self)
        elif file_ext == "sgf":
            ParseObjs.parse_old_sgf(ParseObjs.get_file_lines(filename), self)
        elif file_ext == "obj":
            ParseObjs.parse_obj(ParseObjs.get_file_lines(filename), self)
        else:
            self.load_data_from_json(filename)

    def load_data_from_json(self, filename):
        """Loads SGF data from JSON-style SGF format"""
        try:
            filehandle = open(filename, "r")
        except IOError:
            print("ERROR: Unable to open geometry file")
            sys.exit(1)
        rawgeo = json.load(filehandle)
        filehandle.close()

        #Note: should probably add more error checking
        self.idstr = rawgeo['id']
        self.dimension = rawgeo['dimensions']

        raw_points = rawgeo['data']['vertices']
        raw_faces = rawgeo['data']['faces']
        if self.dimension == 3:
            raw_volumes = rawgeo['data']['volumes']

        raw_splines = []
        if rawgeo['data'].get('splines') is not None:
            raw_splines = rawgeo['data']['splines']

        for pointkey, pointvals in list(raw_points.items()):
            self.vert_list[pointkey] = ParseObjs.SGFvert(pointvals)
            self.vert_list[pointkey].idstr = pointkey
        for facekey, facevals in list(raw_faces.items()):
            self.face_list[facekey] = ParseObjs.SGFface(facevals, self.vert_list)
            self.face_list[facekey].idstr = facekey
        if self.dimension == 3:
            for volkey, volvals in list(raw_volumes.items()):
                self.vols_list[volkey] = ParseObjs.SGFvol(volvals, self.face_list)
                self.vols_list[volkey].idstr = volkey

        for spline_item in list(raw_splines):
            self.vert_list[spline_item].is_spline = True

    def emit_json_geometry(self, filename):
        """Outputs file in JSON with current geometry"""
        try:
            filehandle = open(filename, 'w')
        except IOError:
            print("ERROR: Unable to open output file")
            sys.exit(3)

        vert_json = {}
        face_json = {}
        vols_json = {}

        spline_list = []

        for vert in list(self.vert_list.values()):
            if vert.is_spline:
                spline_list.append(vert.idstr)
            if vert.dim == 2:
                vert_json.update({vert.idstr: [vert.xval, vert.yval]})
            if vert.dim == 3:
                vert_json.update({vert.idstr: [vert.xval, vert.yval, vert.zval]})

        for facekey, face in list(self.face_list.items()):
            tmplist = []
            for point in face.point_list:
                tmplist.append(point.idstr)
            face_json.update({facekey: tmplist})

        if self.dimension == 3:
            for volkey, vol in list(self.vols_list.items()):
                tmplist = []
                for face in vol.face_list:
                    tmplist.append(face.idstr)
                vols_json.update({volkey: tmplist})

        if self.dimension == 2:
            dataobj = {"vertices": vert_json, "faces": face_json}
            if len(spline_list) > 0:
                dataobj["splines"] = spline_list
        else:
            dataobj = {"vertices": vert_json,
                          "faces": face_json,
                          "volumes": vols_json}

        #Now we compute some stats to add to the raw data
        xmin = float("inf")
        xmax = -float("inf")
        ymin = float("inf")
        ymax = -float("inf")
        zmin = float("inf")
        zmax = -float("inf")

        for point in list(self.vert_list.values()):
            if point.xval < xmin:
                xmin = point.xval
            if point.xval > xmax:
                xmax = point.xval
            if point.yval < ymin:
                ymin = point.yval
            if point.yval > ymax:
                ymax = point.yval
            if self.dimension == 3:
                if point.zval < zmin:
                    zmin = point.zval
                if point.zval > zmax:
                    zmax = point.zval

        stats_list = {}
        stats_list["Xmin"] = xmin
        stats_list["Xmax"] = xmax
        stats_list["Ymin"] = ymin
        stats_list["Ymax"] = ymax
        if self.dimension == 3:
            stats_list["Zmin"] = zmin
            stats_list["Zmax"] = zmax

        fileobj = {"id": self.idstr, "data": dataobj,
                      "dimensions": self.dimension,
                      "stats": stats_list}

        outstr = json.dumps(fileobj, indent=1, separators=(',', ':'))
        outstr = outstr.replace(',\n    ', ',')
        outstr = outstr.replace('[\n    ', '[')
        outstr = outstr.replace('\n   ]', ']')
        #outstr = outstr.replace('],', '],\n')

        filehandle.write(outstr)

        filehandle.close()

    def perform_slice(self, slice_plane, slice_height, slice_angle):
        """Takes a 2d slice of a 3d geometry"""
        all_faces = list(self.face_list.values())
        all_points = list(self.vert_list.values())

        if slice_plane == "x":
            for apoint in all_points:
                tmp = apoint.xval
                apoint.point_list.xval = apoint.point_list.yval
                apoint.point_list.yval = apoint.point_list.zval
                apoint.point_list.zval = tmp
        elif slice_plane == "y":
            for apoint in all_points:
                tmp = apoint.point_list.yval
                apoint.point_list.yval = apoint.point_list.zval
                apoint.point_list.zval = tmp
        elif slice_plane == "z":
            pass
        else:
            print("ERROR: slide direction should be x,y or z:")
            sys.exit(4)

        for apoint in all_points:
            apoint.zval -= slice_height

        #Now we are cutting allong z = 0

        full_tmp_segments = []

        for aface in all_faces:
            less_point_found = False
            more_point_found = False
            near_points = []
            for apoint in aface.point_list:
                if apoint.zval < 0.0:
                    less_point_found = True
                if apoint.zval > 0.0:
                    more_point_found = True
                if (apoint.zval > -ParseObjs.MARGIN) and (apoint.zval < ParseObjs.MARGIN):
                    near_points.append(apoint)
            if len(near_points) == 2:
                tmp_seg = ParseObjs.SGFsegment(near_points)
                full_tmp_segments.append(near_points)
            elif len(near_points) == 1:
                pass
            elif (less_point_found is True) and (more_point_found is True):
                new_points = []
                self.segment_slice_check(new_points,
                                         aface.point_list[len(aface.point_list) - 1],
                                         aface.point_list[0])
                for i in range(len(aface.point_list) - 1):
                    self.segment_slice_check(new_points,
                                             aface.point_list[i],
                                             aface.point_list[i + 1])
                if len(new_points) == 2:
                    tmp_seg = ParseObjs.SGFsegment(new_points)
                    full_tmp_segments.append(tmp_seg)

        if len(full_tmp_segments) == 0:
            print("ERROR: This figure does not intersect the cutting plane")
            sys.exit(4)

        tmp_segments = []

        for aseg in full_tmp_segments:
            redundant = False
            for test_seg in tmp_segments:
                if self.seg_redundant(aseg, test_seg):
                    redundant = True
            if redundant is False:
                tmp_segments.append(aseg)

        #Now we remove the old data and replate it with new
        self.idstr = "%s sliced %s %f" % (self.idstr, slice_plane, slice_height)
        if (slice_angle > ParseObjs.MARGIN) or (slice_angle < -ParseObjs.MARGIN):
            self.idstr = "%s angle: %f" % (self.idstr, slice_angle)
        self.dimension = 2

        self.vert_list = {}
        self.face_list = {}
        self.vols_list = {}

        vert_itr = 2
        face_itr = 0
        temp_face = []
        temp_face.append(tmp_segments[0].points[0])
        temp_face.append(tmp_segments[0].points[1])

        self.vert_list["0"] = tmp_segments[0].points[0]
        self.vert_list["1"] = tmp_segments[0].points[1]

        del tmp_segments[0]

        active_point = self.vert_list["1"]
        head_point = self.vert_list["0"]

        while len(tmp_segments) > 0:
            next_seg = self.get_next_slice_segment(tmp_segments, active_point)

            if active_point.is_point(next_seg.points[0]):
                active_point = next_seg.points[1]
            else:
                active_point = next_seg.points[0]

            tmp_segments.remove(next_seg)

            if head_point.is_point(active_point):
                newface = ParseObjs.SGFface(temp_face, self.vert_list)
                self.face_list[str(face_itr)] = newface
                face_itr += 1
                temp_face = []
                if len(tmp_segments) > 0:
                    next_seg = tmp_segments[0]
                    self.vert_list[str(vert_itr)] = next_seg.points[0]
                    self.vert_list[str(vert_itr + 1)] = next_seg.points[1]
                    head_point = next_seg.points[0]
                    active_point = next_seg.points[1]
                    vert_itr += 2
                    temp_face.append(next_seg.points[0])
                    temp_face.append(next_seg.points[1])
                    del next_seg

            else:
                self.vert_list[str(vert_itr)] = active_point
                temp_face.append(active_point)
                vert_itr += 1

        #correct for re-centering
        first = True

        for the_point in list(self.vert_list.values()):
            if first is True:
                first = False
                minx = the_point.xval
                maxx = the_point.xval
                miny = the_point.yval
                maxy = the_point.yval
            else:
                if the_point.xval < minx:
                    minx = the_point.xval
                if the_point.xval > maxx:
                    maxx = the_point.xval
                if the_point.yval < miny:
                    miny = the_point.yval
                if the_point.yval > maxy:
                    maxy = the_point.yval

        midx = (maxx - minx) / 2.0 + minx
        midy = (maxy - miny) / 2.0 + miny

        if (slice_angle < ParseObjs.MARGIN) and (slice_angle > -ParseObjs.MARGIN):
            costh = 1.0
            sinth = 0.0
        else:
            rads = math.radians(slice_angle)
            costh = math.cos(rads)
            sinth = math.sin(rads)

        for the_point in list(self.vert_list.values()):
            the_point.xval -= midx
            the_point.yval -= midy
            inx = the_point.xval
            iny = the_point.yval
            the_point.xval = costh * inx - sinth * iny
            the_point.yval = sinth * inx + costh * iny

        for vert in list(self.vert_list.values()):
            vert.is_spline = False

        #Verifiy this algorithm
        #remove co-linear points
        for the_face in list(self.face_list.values()):
            pt_itr = 0
            while pt_itr < len(the_face.point_list):
                point_before = the_face.point_list[pt_itr - 1]
                point_mid = the_face.point_list[pt_itr]
                if pt_itr == len(the_face.point_list) - 1:
                    point_after = the_face.point_list[0]
                else:
                    point_after = the_face.point_list[pt_itr + 1]

                if point_mid.in_line_with(point_before, point_after):
                    the_face.point_list.remove(point_mid)
                else:
                    pt_itr += 1

        new_vert_list = {}

        for vert_key, vert in self.vert_list.items():
            if self.point_used(vert):
                new_vert_list[vert_key] = vert
        self.vert_list = new_vert_list

        itr = 0
        for vert in list(self.vert_list.values()):
            vert.idstr = str(itr)
            itr += 1

        itr = 0
        for face in list(self.face_list.values()):
            face.idstr = str(itr)
            itr += 1

    def get_next_slice_segment(self, seg_list, active_point):
        """Finds a segnemt in the seg_list with active_point in it"""
        for segment in seg_list:
            if segment.points[0].is_point(active_point):
                return segment
            if segment.points[1].is_point(active_point):
                return segment
        return None

    def segment_slice_check(self, new_point_list, seg_point1, seg_point2):
        """Adds a point to new_point_list if segment crosses z=0 plane"""
        if (seg_point1.zval > 0.0) and (seg_point2.zval < 0.0):
            tmp = seg_point1
            seg_point1 = seg_point2
            seg_point2 = tmp
        if (seg_point1.zval > 0.0) or (seg_point2.zval < 0.0):
            return

        xdist = seg_point2.xval - seg_point1.xval
        ydist = seg_point2.yval - seg_point1.yval
        zdist = seg_point2.zval - seg_point1.zval

        proportion = (-seg_point1.zval) / zdist
        newx = seg_point1.xval + proportion * xdist
        newy = seg_point1.yval + proportion * ydist

        new_point_list.append(ParseObjs.SGFvert([newx, newy]))

    def seg_redundant(self, seg1, seg2):
        """Returns true if the two segments are the same"""
        if seg1.points[0].is_point(seg2.points[0]):
            if seg1.points[1].is_point(seg2.points[1]):
                return True
        if seg1.points[0].is_point(seg2.points[1]):
            if seg1.points[1].is_point(seg2.points[0]):
                return True
        return False

    def point_used(self, query_point):
        """Returns True if point appears in a face, False otherwise"""
        for aface in list(self.face_list.values()):
            for apoint in aface.point_list:
                if apoint is query_point:
                    return True
        return False
