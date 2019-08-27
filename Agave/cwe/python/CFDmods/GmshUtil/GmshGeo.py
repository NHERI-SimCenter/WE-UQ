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

Classes and methods to convert from SGF to gmsh GEO file"""
from __future__ import print_function

import math
import sys
import json
import os

TOLERANCE = 0.000001


class GeoPoint(object):
    """Class for a 2d or 3d point in geo file"""
    def __init__(self, valList, pointDensity):
        self.idnum = -1
        self.xval = 0.0
        self.yval = 0.0
        self.zval = 0.0
        self.is_spline = False
        self.embeded_element = None
        self.mesh_density = float(pointDensity)
        if len(valList) > 0:
            self.xval = float(valList[0])
        if len(valList) > 1:
            self.yval = float(valList[1])
        if len(valList) > 2:
            self.zval = float(valList[2])

    def emit_point(self):
        """Returns string for point in GEO file"""
        ret = "Point(%d) = {%f, %f, %f, %f};\n" % \
            (self.idnum, self.xval, self.yval, self.zval, self.mesh_density)
        return ret

    def emit_embedding(self):
        """Returns string for embedded points in GEO file"""
        if self.embeded_element is None:
            return ""
        embed = "Line"
        if isinstance(self.embeded_element, GeoLine):
            embed = "Line"
            return ""
        if isinstance(self.embeded_element, GeoSurface):
            embed = "Surface"
        if isinstance(self.embeded_element, GeoVolume):
            embed = "Volume"

        ret = "Point{%d} In %s{%d};\n" % (self.idnum, embed, self.embeded_element.idnum)
        return ret

    def point_dist(self, other):
        """3returns 3D distance from this point to the other"""
        xdiff = other.xval - self.xval
        ydiff = other.yval - self.yval
        zdiff = other.zval - self.zval
        ret = xdiff * xdiff + ydiff * ydiff + zdiff * zdiff
        return math.sqrt(ret)

    def is_same_point(self, other):
        """Returns true if coordinates of other poinr are same as this one"""
        if self.xval - other.xval > TOLERANCE:
            return False
        if self.xval - other.xval < -TOLERANCE:
            return False
        if self.yval - other.yval > TOLERANCE:
            return False
        if self.yval - other.yval < -TOLERANCE:
            return False
        if self.zval - other.zval > TOLERANCE:
            return False
        if self.zval - other.zval < -TOLERANCE:
            return False
        return True


class GeoLine(object):
    """Class for a  line in geo file"""
    def __init__(self, endPoint1, endPoint2):
        self.idnum = -1
        self.pt1 = endPoint1
        self.pt2 = endPoint2
        self.btwn_points = []

    def emit_line(self):
        """Returns string for line in GEO file"""
        return "Line(%d) = {%d, %d};\n" % (self.idnum, self.pt1.idnum, self.pt2.idnum)

    def fill_in_points(self, grain_size, mesh_density):
        """Fills in GEO points along a line segment. Returns created points"""
        if len(self.btwn_points) > 0:
            return []
        interval_len = self.pt1.point_dist(self.pt2)
        new_points = int(interval_len / grain_size)
        if new_points <= 0:
            return []

        real_interval_x = (self.pt2.xval - self.pt1.xval) / (new_points + 1.0)
        real_interval_y = (self.pt2.yval - self.pt1.yval) / (new_points + 1.0)
        real_interval_z = (self.pt2.zval - self.pt1.zval) / (new_points + 1.0)

        last_point = self.pt1

        itr = 0
        while itr < new_points:
            newx = last_point.xval + real_interval_x
            newy = last_point.yval + real_interval_y
            newz = last_point.zval + real_interval_z
            new_point = GeoPoint([newx, newy, newz], mesh_density)
            new_point.embeded_element = self
            self.btwn_points.append(new_point)

            last_point = new_point
            itr += 1

        return self.btwn_points

    def is_forward_replica(self, other):
        """Returns true if line is same as other line
           Returns false if line is other line in reverse
           Returns None otherwise"""
        if self.pt1.is_same_point(other.pt1):
            if self.pt2.is_same_point(other.pt2):
                return True
        if self.pt1.is_same_point(other.pt2):
            if self.pt2.is_same_point(other.pt1):
                return False
        return None

    def get_other_point(self, a_point):
        """Given a point at one end of the line, gets the other point"""
        if a_point is self.pt1:
            return self.pt2
        if a_point is self.pt2:
            return self.pt1
        return None


class GeoLineList(object):
    """Class for Line Loop element in GEO file"""
    def __init__(self, lineList):
        self.idnum = -1
        self.my_lines = []
        for item in lineList:
            self.my_lines.append(item)
        self.reversed = [False] * len(self.my_lines)
        self.ref_face = None

    def emit_line_list(self):
        """Returns  string for line loop for GEO file"""
        ret = "Line Loop(%d) = {" % (self.idnum)
        for i in range(len(self.my_lines)):
            aline = self.my_lines[i]
            if i > 0:
                ret += ", "
            if (i < len(self.reversed)) and (self.reversed[i] is True):
                ret +="-%d" % (aline.idnum)
            else:
                ret +="%d" % (aline.idnum)
        ret += "};\n"
        return ret


class GeoBSpline(object):
    """Class for BSpline element in GEO file"""
    def __init__(self, point_list):
        self.idnum = -1
        self.my_points = [point_list]  # Be sure this is true copy

    def emit_bspline(self):
        """Returns  string for BSpline for GEO file"""
        ret = "BSpline(%d) = {" % (self.idnum)
        first = True
        for a_point in self.my_points:
            if first is True:
                first = False
            else:
                ret += ", "
            ret += "%d" % (a_point.idnum)
        ret += "};\n"


class GeoSurface(object):
    """Class for Surface element in GEO file"""
    def __init__(self, theLoop, removals = None):
        self.idnum = -1
        self.my_loop = theLoop
        self.removals = removals

    def emit_surface(self):
        """Returns  string for Surface for GEO file"""
        if self.removals is None:
            return "Plane Surface(%d) = {%d};\n" % (self.idnum, self.my_loop.idnum)
        ret = "Plane Surface(%d) = {%d" % (self.idnum, self.my_loop.idnum)
        for element in self.removals:
            ret += ", %d" % element.idnum
        ret += "};\n"
        return ret


class GeoSurfaceLoop(object):
    """Class for Surface Loop element in GEO file"""
    def __init__(self, surfaces):
        self.idnum = -1
        self.my_surfaces = surfaces[:]

    def emit_surface_loop(self):
        """Returns  string for Surface Loop for GEO file"""
        ret = "Surface Loop(%d) = {" % (self.idnum)
        first = True
        for a_surface in self.my_surfaces:
            if first is True:
                first = False
            else:
                ret += ", "
            ret += "%d" % (a_surface.idnum)
        ret += "};\n"

        return ret


class GeoVolume(object):
    """Class for Volume in GEO file"""
    def __init__(self, surface_loops):
        self.my_loops = surface_loops
        self.idnum = -1

    def emit_volume(self):
        """Returns string for Volume in GEO file"""
        ret = "Volume(%d) = {" % (self.idnum)
        first = True
        for entry in self.my_loops:
            if first is True:
                first = False
            else:
                ret += ", "
            ret +=  "%d" % (entry.idnum)
        ret += "};\n"
        return ret


class BoundaryInfo(object):
    """Stores information for boundary conditions of a GEO mesh"""
    def __init__(self, all_points, rawParams):
        self.my_params = rawParams

        xmin = float("inf")
        xmax = -float("inf")
        ymin = float("inf")
        ymax = -float("inf")
        zmin = float("inf")
        zmax = -float("inf")

        for point in all_points:
            if point.xval < xmin:
                xmin = point.xval
            if point.xval > xmax:
                xmax = point.xval
            if point.yval < ymin:
                ymin = point.yval
            if point.yval > ymax:
                ymax = point.yval
            if point.zval < zmin:
                zmin = point.zval
            if point.zval > zmax:
                zmax = point.zval
        xwide = (xmax - xmin)
        ywide = (ymax - ymin)
        zwide = (zmax - zmin)

        self.h_stat = xwide
        self.b_stat = ywide
        self.l_stat = zwide

        self.xmin = xmin - xwide * self.get_param('inPad')
        self.xmax = xmax + xwide * self.get_param('outPad')
        self.ymin = ymin - ywide * self.get_param('lowYPad')
        self.ymax = ymax + ywide * self.get_param('highYPad')
        self.zmin = zmin - zwide * self.get_param('lowZPad')
        self.zmax = zmax + zwide * self.get_param('highZPad')

        self.density_near = self.get_param('meshDensity')
        self.density_far = self.get_param('meshDensityFar')
        self.inner_domains = self.get_param('innerDomains')

        self.low_yplane = self.get_param('lowYPlane')
        self.high_yplane = self.get_param('highYPlane')
        self.low_zplane = self.get_param('lowZPlane')
        self.high_zplane = self.get_param('highZPlane')

        self.x_min_dom = [0, 0, 0, 0]
        self.x_max_dom = [0, 0, 0, 0]
        self.y_min_dom = [0, 0, 0, 0]
        self.y_max_dom = [0, 0, 0, 0]
        self.z_min_dom = [0, 0, 0, 0]
        self.z_max_dom = [0, 0, 0, 0]
        self.density_dom = [0, 0, 0, 0]

        if self.inner_domains >= 1:
            self.x_min_dom[1] = xmin - xwide * self.get_param('inPadDom1')
            self.x_max_dom[1] = xmax + xwide * self.get_param('outPadDom1')
            self.y_min_dom[1] = ymin - ywide * self.get_param('lowYDom1')
            self.y_max_dom[1] = ymax + ywide * self.get_param('highYDom1')
            self.z_min_dom[1] = zmin - zwide * self.get_param('lowZDom1')
            self.z_max_dom[1] = zmax + zwide * self.get_param('highZDom1')
            self.density_dom[1] = self.get_param('meshDensityDom1')

        if self.inner_domains >= 2:
            self.x_min_dom[2] = xmin - xwide * self.get_param('inPadDom2')
            self.x_max_dom[2] = xmax + xwide * self.get_param('outPadDom2')
            self.y_min_dom[2] = ymin - ywide * self.get_param('lowYDom2')
            self.y_max_dom[2] = ymax + ywide * self.get_param('highYDom2')
            self.z_min_dom[2] = zmin - zwide * self.get_param('lowZDom2')
            self.z_max_dom[2] = zmax + zwide * self.get_param('highZDom3')
            self.density_dom[2] = self.get_param('meshDensityDom2')

        if self.inner_domains >= 3:
            self.x_min_dom[3] = xmin - xwide * self.get_param('inPadDom3')
            self.x_max_dom[3] = xmax + xwide * self.get_param('outPadDom3')
            self.y_min_dom[3] = ymin - ywide * self.get_param('lowYDom3')
            self.y_max_dom[3] = ymax + ywide * self.get_param('highYDom3')
            self.z_min_dom[3] = zmin - zwide * self.get_param('lowZDom3')
            self.z_max_dom[3] = zmax + zwide * self.get_param('highZDom3')
            self.density_dom[3] = self.get_param('meshDensityDom3')

    def get_param(self, key):
        """Returns value of a boundary parameter"""
        if key in self.my_params:
            if (key == 'lowYPlane') or (key == 'highYPlane') or \
               (key == 'lowZPlane') or (key == 'highZPlane'):
                return self.my_params[key]

            return float(self.my_params[key])
        return self.get_default(key)

    def get_default(self, key):
        """Returns default value for a boundary parameter"""
        defaults = {}
        defaults['inPad'] = 1.0
        defaults['outPad'] = 5.0
        defaults['lowYPad'] = 1.0
        defaults['highYPad'] = 1.0
        defaults['lowZPad'] = 0.0
        defaults['highZPad'] = 1.0
        defaults['meshDensity'] = 5.0
        defaults['meshDensityFar'] = 25.0

        defaults['lowYPlane'] = 'SYM_PLANE'
        defaults['highYPlane'] = 'SYM_PLANE'
        defaults['lowZPlane'] = 'SYM_PLANE'
        defaults['highZPlane'] = 'SYM_PLANE'

        defaults['inner_domains'] = 0

        defaults['inPadDom1'] = 2.0
        defaults['outPadDom1'] = 4.0
        defaults['lowYDom1'] = 2.0
        defaults['highYDom1'] = 2.0
        defaults['lowZDom1'] = 2.0
        defaults['highZDom1'] = 2.0
        defaults['meshDensityDom1'] = 5.0
        defaults['bottomPadDom1'] = 25.0

        defaults['inPadDom2'] = 2.0
        defaults['outPadDom2'] = 4.0
        defaults['lowYDom2'] = 2.0
        defaults['highYDom2'] = 2.0
        defaults['lowZDom2'] = 2.0
        defaults['highZDom2'] = 2.0
        defaults['meshDensityDom2'] = 5.0
        defaults['bottomPadDom2'] = 25.0

        defaults['inPadDom3'] = 2.0
        defaults['outPadDom3'] = 4.0
        defaults['lowYDom3'] = 2.0
        defaults['highYDom3'] = 2.0
        defaults['lowZDom3'] = 2.0
        defaults['highZDom3'] = 2.0
        defaults['meshDensityDom3'] = 5.0
        defaults['bottomPadDom3'] = 25.0

        if key not in defaults:
            return None
        return defaults[key]


class GeoInstance(object):
    """Class containg all information for a GEO mesh"""
    def __init__(self):
        self.geo_info = None
        self.bound_info = None

        self.full_point_list = []
        self.full_line_list = []
        self.full_line_loop_list = []
        self.bspline_list = []
        self.surface_list = []
        self.surface_loop_list = []
        self.volume_list = []

        self.stats_object = dict()

    def set_boundary_info(self, file_name):
        """Retrives boundary info from a file and sets boundary paramters"""
        all_points = list(self.geo_info.vert_list.values())
        try:
            file_handle = open(file_name, "r")
        except IOError:
            print("ERROR: Unable to open setting file")
            sys.exit(6)
        raw_boundaries = json.load(file_handle)

        self.bound_info = BoundaryInfo(all_points, raw_boundaries)

        self.stats_object['H'] = self.bound_info.h_stat
        self.stats_object['B'] = self.bound_info.b_stat
        self.stats_object['L'] = self.bound_info.l_stat

    def set_boundary_default(self):
        """Sets boundary solely from default values"""
        all_points = list(self.geo_info.vert_list.values())
        self.bound_info = BoundaryInfo(all_points, {})

    def get_new_point(self, vals, density):
        """Creates new GEO point"""
        new_point = GeoPoint(vals, density)
        self.full_point_list.append(new_point)

        return new_point

    def find_line_loop(self, p_list):
        """Will search for a pre-existing loop of lines in p_list"""
        start_point = p_list[0]
        search_point = start_point
        lines_list = []
        reversal_list = []
        line_in = None
        while True:
            (next_line, is_forward) = self.find_line_span_in_point_list(search_point, line_in,
                                                                        p_list)
            if next_line is None:
                return None
            lines_list.append(next_line)
            reversal_list.append(not is_forward)
            line_in = next_line
            search_point = next_line.get_other_point(search_point)

            if search_point is start_point:
                ret = GeoLineList(lines_list)
                self.full_line_loop_list.append(ret)
                ret.reversed = reversal_list
                return ret
            if len(lines_list) > len(p_list):
                return None
        return None

    def find_line_span_in_point_list(self, search_point, line_in, p_list):
        """Given a search point, an excluded line and a list of points to compare with,
        finds the next span out of the search point"""
        for a_point in p_list:
            if a_point is search_point:
                continue
            a_span = GeoLine(search_point, a_point)
            (old_line, is_forward) = self.check_for_line(a_span)
            if old_line is False:
                continue
            if old_line is line_in:
                continue
            return (old_line, is_forward)
        return (None, False)

    def check_for_line(self, new_line):
        """Checks if line already exists. False if new/old line if not and non-reversal value"""
        for a_line in self.full_line_list:
            result = a_line.is_forward_replica(new_line)

            if result is True:
                return (a_line, result)
            if result is False:
                return (a_line, result)

        return (False, True)

    def string_line_list(self, p_list):
        """Creates new Line Loop with filled in points (fill points removed)"""
        new_points = []
        new_lines = []
        loop_lines = []
        reversal_list = []
        num_pts = len(p_list)
        for i in range(num_pts):
            a_span = GeoLine(p_list[i - 1], p_list[i])
            (old_line, is_forward) = self.check_for_line(a_span)
            if old_line is False:
                #new_points.extend(a_span.fill_in_points(grain, density))
                new_lines.append(a_span)
                loop_lines.append(a_span)
            else:
                loop_lines.append(old_line)
            reversal_list.append(not is_forward)

        ret = GeoLineList(loop_lines)
        self.full_line_loop_list.append(ret)
        self.full_line_list.extend(new_lines)
        self.full_point_list.extend(new_points)
        ret.reversed = reversal_list

        return ret

    def get_point_box(self, p_list, density, grain, embeded_element):
        """Creates new point box"""
        ret = []
        num_points = len(p_list)
        for i in range(num_points):
            line_span = GeoLine(p_list[i - 1], p_list[i])
            ret.append(p_list[i - 1])
            ret.extend(line_span.fill_in_points(grain, density))

        for point in ret:
            point.embeded_element = embeded_element

        self.full_point_list.extend(ret)

        return ret

    def get_cube_line(self, side_list, used_indexes, reverse):
        """Creates line list for face of cube"""
        lines = []
        if len(used_indexes) != 4:
            return None
        for itr in range(4):
            lines.append(side_list[used_indexes[itr]])
        ret = GeoLineList(lines)
        ret.reversed = [False, False, reverse, reverse]
        return ret

    def get_cube_of_lines(self, p_list):
        """Creates new cube of lines"""
        spans = []
        spans.append(GeoLine(p_list[0], p_list[1]))
        spans.append(GeoLine(p_list[1], p_list[2]))
        spans.append(GeoLine(p_list[2], p_list[3]))
        spans.append(GeoLine(p_list[3], p_list[0]))
        spans.append(GeoLine(p_list[0], p_list[4]))
        spans.append(GeoLine(p_list[1], p_list[5]))
        spans.append(GeoLine(p_list[2], p_list[6]))
        spans.append(GeoLine(p_list[3], p_list[7]))
        spans.append(GeoLine(p_list[4], p_list[5]))
        spans.append(GeoLine(p_list[5], p_list[6]))
        spans.append(GeoLine(p_list[6], p_list[7]))
        spans.append(GeoLine(p_list[7], p_list[4]))

        for line in spans:
            #self.full_point_list.extend(line.fill_in_points(grain, density))
            self.full_line_list.append(line)

        loops = []
        loops.append(self.get_cube_line(spans, [0, 1, 2, 3], False))
        loops.append(self.get_cube_line(spans, [8, 9, 10, 11], False))
        loops.append(self.get_cube_line(spans, [0, 5, 8, 4], True))
        loops.append(self.get_cube_line(spans, [1, 6, 9, 5], True))
        loops.append(self.get_cube_line(spans, [2, 7, 10, 6], True))
        loops.append(self.get_cube_line(spans, [3, 4, 11, 7], True))

        self.full_line_loop_list.extend(loops)

        surfaces = []
        for a_loop in loops:
            surfaces.append(GeoSurface(a_loop))
        self.surface_list.extend(surfaces)

        ret = GeoSurfaceLoop(surfaces)
        self.surface_loop_list.append(ret)

        return ret

    def get_cube_of_points(self, p_list, density, grain, embeded_element):
        """Creates a cube of points"""
        line_spans = []
        line_spans.append(GeoLine(p_list[0], p_list[1]))
        line_spans.append(GeoLine(p_list[1], p_list[2]))
        line_spans.append(GeoLine(p_list[2], p_list[3]))
        line_spans.append(GeoLine(p_list[3], p_list[0]))
        line_spans.append(GeoLine(p_list[0], p_list[4]))
        line_spans.append(GeoLine(p_list[1], p_list[5]))
        line_spans.append(GeoLine(p_list[2], p_list[6]))
        line_spans.append(GeoLine(p_list[3], p_list[7]))
        line_spans.append(GeoLine(p_list[4], p_list[5]))
        line_spans.append(GeoLine(p_list[5], p_list[6]))
        line_spans.append(GeoLine(p_list[6], p_list[7]))
        line_spans.append(GeoLine(p_list[7], p_list[4]))

        ret = []
        ret.extend(p_list)
        for span in line_spans:
            ret.extend(span.fill_in_points(grain, density))

        for point in ret:
            point.embeded_element = embeded_element
            self.full_point_list.append(point)

        return ret

    def get_new_bspline(self, p_list):
        """Creates new GEO BSpline"""
        ret = GeoBSpline(p_list)
        self.bspline_list.append(ret)

        return ret

    def emit_geo_file_2d(self, grain_size):
        """Creates data structures from this object data for 2D mesh"""
        self.get_new_point([self.bound_info.xmin, self.bound_info.ymin],
                         self.bound_info.density_far)
        self.get_new_point([self.bound_info.xmax, self.bound_info.ymin],
                         self.bound_info.density_far)
        self.get_new_point([self.bound_info.xmax, self.bound_info.ymax],
                         self.bound_info.density_far)
        self.get_new_point([self.bound_info.xmin, self.bound_info.ymax],
                         self.bound_info.density_far)

        self.string_line_list(self.full_point_list)

        for a_point in list(self.geo_info.vert_list.values()):
            new_point = self.get_new_point([a_point.xval, a_point.yval],
                                        self.bound_info.density_near)
            a_point.refPoint = new_point
            new_point.is_spline = a_point.is_spline

        for face_val in list(self.geo_info.face_list.values()):
            face_points = face_val.point_list
            ref_points = []
            for a_point in face_points:
                ref_points.append(a_point.refPoint)

            element_list = []
            i = -1
            while i < len(ref_points) - 1:
                j = i + 1
                # i is start, j is end of current element
                if ref_points[j].is_spline is True:
                    while ref_points[j].is_spline is True:
                        j += 1
                    element_list.append(self.get_new_bspline(ref_points[i:j + 1]))
                else:
                    new_line = GeoLine(ref_points[i], ref_points[j])
                    #new_points = new_line.fill_in_points(grain_size, self.bound_info.density_near)

                    #self.full_point_list.extend(new_points)
                    element_list.append(new_line)

                i = j

            self.full_line_list.extend(element_list)
            self.full_line_loop_list.append(GeoLineList(element_list))

        full_surface = GeoSurface(self.full_line_loop_list[0], self.full_line_loop_list[1:])

        self.surface_list.append(full_surface)

        if self.bound_info.inner_domains > 0:
            for layer in range(1, int(self.bound_info.inner_domains) + 1):
                short_list = []
                short_list.append(GeoPoint(
                                [self.bound_info.x_min_dom[layer],
                                 self.bound_info.y_min_dom[layer]],
                                 self.bound_info.density_dom[layer]))
                short_list.append(GeoPoint(
                                [self.bound_info.x_max_dom[layer],
                                 self.bound_info.y_min_dom[layer]],
                                 self.bound_info.density_dom[layer]))
                short_list.append(GeoPoint(
                                [self.bound_info.x_max_dom[layer],
                                 self.bound_info.y_max_dom[layer]],
                                 self.bound_info.density_dom[layer]))
                short_list.append(GeoPoint(
                                [self.bound_info.x_min_dom[layer],
                                 self.bound_info.y_max_dom[layer]],
                                 self.bound_info.density_dom[layer]))

                self.get_point_box(short_list, self.bound_info.density_dom[layer],
                                 grain_size, full_surface)

    def emit_geo_file_3d(self, grain_size):
        """Creates data structures from this object data for 3D mesh"""
        self.get_new_point([self.bound_info.xmin, self.bound_info.ymin,
                          self.bound_info.zmin],
                          self.bound_info.density_far)
        self.get_new_point([self.bound_info.xmax, self.bound_info.ymin,
                          self.bound_info.zmin],
                          self.bound_info.density_far)
        self.get_new_point([self.bound_info.xmax, self.bound_info.ymax,
                          self.bound_info.zmin],
                          self.bound_info.density_far)
        self.get_new_point([self.bound_info.xmin, self.bound_info.ymax,
                          self.bound_info.zmin],
                          self.bound_info.density_far)
        self.get_new_point([self.bound_info.xmin, self.bound_info.ymin,
                          self.bound_info.zmax],
                          self.bound_info.density_far)
        self.get_new_point([self.bound_info.xmax, self.bound_info.ymin,
                          self.bound_info.zmax],
                          self.bound_info.density_far)
        self.get_new_point([self.bound_info.xmax, self.bound_info.ymax,
                          self.bound_info.zmax],
                          self.bound_info.density_far)
        self.get_new_point([self.bound_info.xmin, self.bound_info.ymax,
                          self.bound_info.zmax],
                          self.bound_info.density_far)

        self.get_cube_of_lines(self.full_point_list)

        for a_point in list(self.geo_info.vert_list.values()):
            new_point = self.get_new_point([a_point.xval, a_point.yval, a_point.zval],
                                           self.bound_info.density_near)
            a_point.refPoint = new_point

        floor_points = []

        for face_val in list(self.geo_info.face_list.values()):
            face_points = face_val.point_list
            ref_points = []
            all_points_at_floor = True

            for a_point in face_points:
                ref_points.append(a_point.refPoint)
                if a_point.zval >= (self.bound_info.zmin + TOLERANCE):
                    all_points_at_floor = False

            if all_points_at_floor:
                floor_points.extend(ref_points)
                face_val.ref_face = None
                continue

            new_face = self.string_line_list(ref_points)

            new_surface = GeoSurface(new_face)
            self.surface_list.append(new_surface)
            face_val.ref_face = new_surface

        for a_volume in list(self.geo_info.vols_list.values()):
            faces = a_volume.face_list
            surfaces = []
            points_in_floor = False

            for a_face in faces:
                if a_face.ref_face is None:
                    points_in_floor = True
                else:
                    surfaces.append(a_face.ref_face)

            if points_in_floor:
                self.surface_loop_list[0].my_surfaces.extend(surfaces)
            else:
                self.surface_loop_list.append(GeoSurfaceLoop(surfaces))

        while len(floor_points) > 0:
            new_line_loop = self.find_line_loop(floor_points)
            if new_line_loop is None:
                print("Error floor line loop can not be found")
                sys.exit(30)
            for a_line in new_line_loop.my_lines:
                while floor_points.count(a_line.pt1) > 0:
                    floor_points.remove(a_line.pt1)
                while floor_points.count(a_line.pt2) > 0:
                    floor_points.remove(a_line.pt2)
            if self.surface_list[0].removals is None:
                self.surface_list[0].removals = []
            self.surface_list[0].removals.append(new_line_loop)

        flow_area = GeoVolume(self.surface_loop_list)

        self.volume_list.append(flow_area)

        if self.bound_info.inner_domains > 0:
            for layer in range(1, int(self.bound_info.inner_domains) + 1):
                short_list = []
                short_list.append(GeoPoint(
                                [self.bound_info.x_min_dom[layer],
                                 self.bound_info.y_min_dom[layer],
                                 self.bound_info.z_min_dom[layer]],
                                 self.bound_info.density_dom[layer]))
                short_list.append(GeoPoint(
                                [self.bound_info.x_max_dom[layer],
                                 self.bound_info.y_min_dom[layer],
                                 self.bound_info.z_min_dom[layer]],
                                 self.bound_info.density_dom[layer]))
                short_list.append(GeoPoint(
                                [self.bound_info.x_max_dom[layer],
                                 self.bound_info.y_max_dom[layer],
                                 self.bound_info.z_min_dom[layer]],
                                 self.bound_info.density_dom[layer]))
                short_list.append(GeoPoint(
                                [self.bound_info.x_min_dom[layer],
                                 self.bound_info.y_max_dom[layer],
                                 self.bound_info.z_min_dom[layer]],
                                 self.bound_info.density_dom[layer]))
                short_list.append(GeoPoint(
                                [self.bound_info.x_min_dom[layer],
                                 self.bound_info.y_min_dom[layer],
                                 self.bound_info.z_max_dom[layer]],
                                 self.bound_info.density_dom[layer]))
                short_list.append(GeoPoint(
                                [self.bound_info.x_max_dom[layer],
                                 self.bound_info.y_min_dom[layer],
                                 self.bound_info.z_max_dom[layer]],
                                 self.bound_info.density_dom[layer]))
                short_list.append(GeoPoint(
                                [self.bound_info.x_max_dom[layer],
                                 self.bound_info.y_max_dom[layer],
                                 self.bound_info.z_max_dom[layer]],
                                 self.bound_info.density_dom[layer]))
                short_list.append(GeoPoint(
                                [self.bound_info.x_min_dom[layer],
                                 self.bound_info.y_max_dom[layer],
                                 self.bound_info.z_max_dom[layer]],
                                 self.bound_info.density_dom[layer]))

                self.get_cube_of_points(short_list,
                                     self.bound_info.density_dom[layer],
                                     grain_size, flow_area)

    def emit_geo_file(self, out_file_name):
        """Outputs GEO file from this object data"""
        try:
            file_handle = open(out_file_name, "w")
        except IOError:
            print("ERROR: Unable to open output file")
            sys.exit(3)

        tolerance = (self.bound_info.xmax - self.bound_info.xmin) + \
                    (self.bound_info.ymax - self.bound_info.ymin) + \
                    (self.bound_info.zmax - self.bound_info.zmin)
        tolerance /= 10.0

        string_grain = lcf_float(self.bound_info.xmax - self.bound_info.xmin,
                               self.bound_info.ymax - self.bound_info.ymin,
                               tolerance)

        if self.geo_info.dimension == 3:
            string_grain = lcf_float(string_grain,
                                   self.bound_info.zmax - self.bound_info.zmin,
                                   tolerance)

        for face_val in list(self.geo_info.face_list.values()):
            face_points = face_val.point_list
            for i in range(len(face_points) - 1):
                point_dist = face_points[i].point_dist(face_points[i + 1])
                string_grain = lcf_float(string_grain, point_dist, tolerance)
            point_dist = face_points[len(face_points) - 1].point_dist(face_points[0])
            string_grain = lcf_float(string_grain, point_dist, tolerance)

        if self.geo_info.dimension == 2:
            self.emit_geo_file_2d(string_grain)
        elif self.geo_info.dimension == 3:
            self.emit_geo_file_3d(string_grain)
        else:
            print("ERROR: Should be either 2 or 3 dimensions")
            sys.exit(6)

        full_itr = 1
        for point in self.full_point_list:
            point.idnum = full_itr
            full_itr += 1
        for line in self.full_line_list:
            line.idnum = full_itr
            full_itr += 1
        for lineloop in self.full_line_loop_list:
            lineloop.idnum = full_itr
            full_itr += 1
        for spline in self.bspline_list:
            spline.idnum = full_itr
            full_itr += 1
        for surface in self.surface_list:
            surface.idnum = full_itr
            full_itr += 1
        for surface_loop in self.surface_loop_list:
            surface_loop.idnum = full_itr
            full_itr += 1
        for volume in self.volume_list:
            volume.idnum = full_itr
            full_itr += 1

        for point in self.full_point_list:
            file_handle.write(point.emit_point())
        for line in self.full_line_list:
            file_handle.write(line.emit_line())
        for lineloop in self.full_line_loop_list:
            file_handle.write(lineloop.emit_line_list())
        for spline in self.bspline_list:
            file_handle.write(spline.emit_bspline())
        for surface in self.surface_list:
            file_handle.write(surface.emit_surface())
        for surface_loop in self.surface_loop_list:
            file_handle.write(surface_loop.emit_surface_loop())
        for volume in self.volume_list:
            file_handle.write(volume.emit_volume())

        if self.geo_info.dimension == 2:
            file_handle.write("Physical Surface(\"Back Plane\") = {%d};\n" %
                             (self.surface_list[0].idnum))
            file_handle.write("MeshAlgorithm Surface {%d} = 6;\n" %
                             (self.surface_list[0].idnum))

        elif self.geo_info.dimension == 3:
            file_handle.write("Physical Volume(\"Flow Area\") = {%d};\n" %
                             (self.volume_list[0].idnum))
        else:
            print("ERROR: Should be either 2 or 3 dimensions")
            sys.exit(6)

        for point in self.full_point_list:
            file_handle.write(point.emit_embedding())

        file_handle.close()

        out_paths = out_file_name.rsplit('/',1)
        stat_path = os.path.realpath(out_paths[0])
        stat_path = '/'.join(stat_path.split('/')[0:-1])
        stat_path = stat_path + '/stats.mesh'

        try:
            file_handle = open(stat_path, "w")
        except IOError:
            print("ERROR: Unable to open stats file")
            sys.exit(3)

        raw_stat_json = json.dumps(self.stats_object,
                                   sort_keys=True, indent=4, separators=(',', ': '))

        try:
            file_handle.write(raw_stat_json)
        except IOError:
            print("ERROR: Unable to write to stats file")
            sys.exit(3)

        file_handle.close()


def lcf_float(val1, val2, tolerance):
    """Finds lowest common floating point factor between two floating point numbers"""
    i = 1.0
    while True:
        test = float(val1) / i
        check = float(val2) / test
        floor_check = math.floor(check)
        compare = floor_check * test
        if val2 - compare < (tolerance / 2.0):
            return test
        i += 1.0
        
