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

Manages CFD case templates and emits corresponding files"""
from __future__ import print_function

import sys
import os
import shutil
import re

from string import Template

from CFDmods.TemplateMods.ConfManage import TemplateConf

IF_SPLITTER = re.compile(r'(\$if|\$fi)')
IF_BREAKER = re.compile(r'\$if')
THEN_BREAKER = re.compile(r'\$then')
CONDITION_FINDER = re.compile(r'"(.*)"\s*(==|!=)\s*"(.*)"')
ELSE_SPLITTER = re.compile(r'(\$else|\$elif)')
QUOTE_FINDER = re.compile(r'"')


class TemplateData(object):
    """Object for performing actions using a template"""
    def __init__(self, templateFolder, templateName):
        self.config = TemplateConf(templateFolder, templateName)
        self.template_name = templateName

    def __repr__(self):
        return repr(self.config)

    def task_data_print(self):
        """Prints task data for this template"""
        self.config.task_data_print()

    def emit_files_from_vars(self, var_list, case_folder):
        """Creates files using this template"""
        self.recursive_emit_files(var_list, self.config.file_tree_root, case_folder)

    def recursive_emit_files(self, var_list, file_node, root_folder):
        """Recursive helper function to emit files"""
        path_to_use = file_node.my_path
        path_to_make = root_folder + file_node.parent_path()

        if (file_node.my_type == 'static') or (file_node.my_type == 'variable'):
            if file_node.my_condition is not None:
                if not file_node.condition_accepted(var_list):
                    try:
                        if os.path.isdir(path_to_use):
                            os.unlink(path_to_make)
                        else:
                            os.remove(path_to_make)
                    except os.error:
                        pass
                    return

        if file_node.my_type == 'static':
            if os.path.isdir(path_to_use):
                try:
                    os.makedirs(path_to_make)
                except os.error:
                    pass
                file_list = os.listdir(path_to_use)
                for afile in file_list:
                    if ((afile[0] != '.') and
                            (file_node.has_child_with_name(afile) is False)):
                        if os.path.isdir(path_to_use + '/' + afile):
                            try:
                                shutil.rmtree(path_to_make + '/' + afile)
                            except os.error:
                                pass
                            shutil.copytree(path_to_use + '/' + afile,
                                            path_to_make + '/' + afile)
                        else:
                            try:
                                os.unlink(path_to_make + '/' + afile)
                            except os.error:
                                pass
                            shutil.copyfile(path_to_use + '/' + afile,
                                            path_to_make + '/' + afile)
            else:
                try:
                    os.unlink(path_to_make)
                except os.error:
                    pass
                shutil.copyfile(path_to_use, path_to_make)
        elif file_node.my_type == 'variable':
            try:
                os.unlink(path_to_make)
            except os.error:
                pass
            try:
                os.makedirs(path_to_make)
            except os.error:
                pass
            try:
                os.rmdir(path_to_make)
            except os.error:
                pass
            if os.path.isdir(path_to_use):
                try:
                    os.makedirs(path_to_make)
                except os.error:
                    pass
                file_list = os.listdir(path_to_use)
                for afile in file_list:
                    if file_node.has_child_with_name(afile) is False:
                        self.emit_by_template(var_list,
                                            path_to_use + '/' + afile,
                                            path_to_make + '/' + afile)
            else:
                self.emit_by_template(var_list, path_to_use, path_to_make)
        elif file_node.my_type == 'provided':
            path_to_make = root_folder + file_node.parent_path()
            if not os.path.exists(path_to_make):
                try:
                    os.makedirs(path_to_make)
                except os.error:
                    pass
                try:
                    os.rmdir(path_to_make)
                except os.error:
                    pass
        for achild in file_node.children:
            self.recursive_emit_files(var_list, achild, root_folder)

    def emit_by_template(self, var_list, from_file, to_file):
        """Emit a veriable file using the template"""
        try:
            file_handle = open(from_file, "r")
        except IOError:
            print("ERROR: Unable to open input file: %s" % (from_file))
            sys.exit(1)

        raw_file = file_handle.read()

        if_cleaned = self.resolve_all_ifs(raw_file, var_list)
        final_output = Template(if_cleaned).substitute(var_list)

        try:
            file_handle2 = open(to_file, "w")
        except IOError:
            print("ERROR: Unable to open output file")
            sys.exit(2)

        file_handle2.write(final_output)

    def find_if_block(self, in_str):
        """Locates in if block in a template"""
        broken_up_file = IF_SPLITTER.split(in_str)

        ret = []

        if len(broken_up_file) < 2:
            return None

        for i in range(len(broken_up_file) - 2):
            if (broken_up_file[i] == r'$if') and (broken_up_file[i + 2] == r'$fi'):
                ret.append("".join(broken_up_file[0:i]))
                ret.append("".join(broken_up_file[i:i + 3]))
                if len(broken_up_file) > i + 3:
                    ret.append("".join(broken_up_file[i + 3:]))
                return ret

        print("ERROR: if, fi mismatch")
        sys.exit(3)
        return None

    def resolve_if(self, if_str, var_dict):
        """Resolves if in configuration file"""
        if_parts = IF_BREAKER.split(if_str)

        if len(if_parts) != 2:
            print("ERROR: internal if parse error")
            sys.exit(4)
            return None

        then_parts = THEN_BREAKER.split(if_parts[1], 1)
        conditions = CONDITION_FINDER.search(then_parts[0]).groups()

        if len(conditions) != 3:
            print("ERROR: conditional clause parse error")
            sys.exit(5)
            return None

        test_equal = False

        if conditions[1] == "==":
            test_equal = True
        elif conditions[1] == "!=":
            test_equal = False
        else:
            print("ERROR: invalid comparison operator")
            sys.exit(6)
            return None

        side1 = Template(conditions[0]).substitute(var_dict)
        side2 = Template(conditions[2]).substitute(var_dict)

        test_correct = (side1 == side2) is test_equal

        if test_correct:
            elsed_statement = ELSE_SPLITTER.split(then_parts[1], 1)
            return self.str_from_quote(elsed_statement[0])
        else:
            elsed_statement = ELSE_SPLITTER.split(then_parts[1], 1)
            if len(elsed_statement) < 3:
                return ""
            elif elsed_statement[1] == "$else":
                return self.str_from_quote(elsed_statement[2])
            else:
                return self.resolve_if("$if " + elsed_statement[2], var_dict)

    def resolve_all_ifs(self, raw_str, var_dict):
        """Resolves all template if statements in raw_str"""
        working_str = raw_str

        while True:
            block_list = self.find_if_block(working_str)
            if block_list is None:
                return working_str

            if len(block_list) != 3:
                print("ERROR: Internal Parser Error")
                sys.exit(3)

            block_list[1] = self.resolve_if(block_list[1], var_dict)
            working_str = "".join(block_list)

    def str_from_quote(self, raw_str):
        """Gets widest quoted string within another string"""
        split_str = QUOTE_FINDER.split(raw_str)
        if len(split_str) < 3:
            return ""

        ret = ""

        del split_str[0]
        del split_str[len(split_str) - 1]
        first = True
        for astr in split_str:
            if first:
                first = False
            else:
                ret += "\""
            ret += astr
        return ret

    def get_script_file(self, script_name):
        """Gets real full path of requested script file"""
        raw_file = self.config.script_entries[script_name].script_file
        ret = self.config.template_root + '/' + self.config.template_name + '/scripts/' + raw_file

        return ret

    def get_data_folder(self):
        """Gets real full path of data folder for template"""
        return self.config.template_root + '/' + self.config.template_name + '/data/'

    def get_last_stage(self, script_name):
        """Gets last of previous stage"""
        return self.config.script_entries[script_name].prereq
