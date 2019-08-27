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

Manages name-value key pairs for templater"""
from __future__ import print_function

import sys
import json
import os
import shutil

from CFDmods.TemplateMods.TemplateManage import TemplateData
from CFDmods.TemplateMods.ComputeVars import ComputeVars


class KeyValDataSet(object):
    """A set of template key-values"""
    def __init__(self, templateFolder, caseFolder, statFolder=''):
        try:
            var_file = open(caseFolder + '/.varStore', 'r')
        except IOError:
            print("Unable to open folder to read case files")
            sys.exit(2)
        self.case_folder = caseFolder
        raw_data = json.load(var_file)
        var_file.close()

        self.template_name = raw_data['type']
        self.template_data = TemplateData(templateFolder, self.template_name)

        self.var_list = raw_data['vars']

        self.append_stat_vars(statFolder)

        for avar in self.template_data.config.var_defaults:
            if avar not in self.var_list:
                self.var_list[avar] = self.template_data.config.var_defaults.get(avar)

        for avar in self.template_data.config.stat_defaults:
            if avar not in self.var_list:
                self.var_list[avar] = self.template_data.config.stat_defaults.get(avar)

        self.append_expr_vars()

    def task_data_print(self):
        """Prints task data of corresponding template"""
        return self.template_data.task_data_print()

    def roll_back_to_task(self, task_name):
        """Deletes files marked as result for a particular task and template"""
        raw_task_list = self.template_data.config.script_entries.keys
        task_list = []
        if task_name == 'all':
            for entry in list(raw_task_list):
                task_list.append(entry.scriptName)
        else:
            for entry in list(raw_task_list):
                current_entry = entry
                while ((current_entry is not None)
                      and (current_entry.script_name is not task_name)):
                    current_entry = current_entry.prereq
                if current_entry is not None:
                    task_list.append(entry.script_name)
        self.recursive_rollback(task_list,
                               self.case_folder,
                               self.template_data.config.file_tree_root)

    def recursive_rollback(self, task_list, root_folder, file_node):
        """Recursivly traverses file tree to delete rollback files"""
        for achild in file_node.children:
            self.recursive_rollback(task_list, root_folder, achild)
        if file_node.myType == 'result':
            try:
                task_list.index(file_node.my_task)  # excpetion if not found
                path_to_use = root_folder + file_node.parentPath()
                if os.path.isdir(path_to_use):
                    file_list = os.listdir(path_to_use)
                    for afile in file_list:
                        if (afile[0] != '.') and (file_node.hasChildWithName(afile) is False):
                            if os.path.isdir(path_to_use + '/' + afile):
                                try:
                                    shutil.rmtree(path_to_use + '/' + afile)
                                except os.error:
                                    pass
                            else:
                                try:
                                    os.unlink(path_to_use + '/' + afile)
                                except os.error:
                                    pass
                    file_list = os.listdir(path_to_use)
                    if len(file_list) == 0:
                        try:
                            shutil.rmtree(path_to_use)
                        except os.error:
                            pass
                else:
                    try:
                        os.unlink(path_to_use)
                    except os.error:
                        pass
            except IndexError:
                pass

    def print_keys(self):
        """Prints all keys of corresponding template"""
        self.template_data.config.print_all_keys()

    def print_keyvals(self, list_of_keys):
        """Prints all values for the given keys"""
        the_def_list = self.template_data.config.var_defaults
        if len(list_of_keys) == 1:
            anentry = list_of_keys[0]
            if the_def_list.get(anentry) is None:
                print("Error: variable not recognized: " + anentry)
                sys.exit()
            if self.var_list.get(anentry) is None:
                print(the_def_list.get(anentry))
            else:
                print(self.var_list[anentry])

            sys.exit()
        for anentry in list_of_keys:
            if the_def_list.get(anentry) is None:
                print("Error: variable not recognized: " + anentry)
                sys.exit()
            outstr = anentry + " : "
            if self.var_list.get(anentry) is None:
                outstr += the_def_list.get(anentry)
            else:
                outstr += self.var_list[anentry]
            print(outstr)

    def set_keyval(self, key, val):
        """Sets a template variable to a value"""
        default_val = self.template_data.config.var_defaults.get(key)
        if default_val is None:
            print ("Error: that variable does not exist for this template.")
            sys.exit(2)
        if not isinstance(val, str):
            print ("Error: variable expects a string.")
            sys.exit(2)
        self.var_list[key] = val

    def emit_new_files(self):
        """Emits files based on variable list"""
        self.template_data.emit_files_from_vars(self.var_list, self.case_folder)

    def pull_all_vals(self):
        """Prints all key vals for a case, including defaults"""
        for avar in self.template_data.config.var_defaults:
            if self.var_list.get(avar) is None:
                entry = self.template_data.config.var_defaults[avar]
            else:
                entry = self.var_list[avar]
            print((avar + ": " + entry))

    def dump_keyvals(self):
        """Prints out all stored keyvals for this case"""
        try:
            varfile = open(self.case_folder + '/.varStore', 'w')
        except IOError:
            print ("Unable to open folder to read case files")
            sys.exit(2)

        file_obj = {"type": self.template_name, "vars": self.var_list}

        outstr = json.dumps(file_obj, indent=1, separators=(',', ':'))
        varfile.write(outstr)
        varfile.close()

    def append_stat_vars(self, stat_folder):
        '''Adds variable from stat folders to var list, if applicable'''
        if stat_folder == '':
            return

        try:
            files_list = os.listdir(stat_folder)
        except BaseException:
            return

        for a_file in files_list:
            if not a_file.startswith("stats."):
                continue

            try:
                stat_file = open(stat_folder + '/' + a_file, 'r')
            except IOError:
                print("Unable to open stat file")
                sys.exit(60)

            raw_stats = json.load(stat_file)
            for entry in list(raw_stats):
                if entry not in self.template_data.config.stat_defaults:
                    continue
                if entry in self.var_list:
                    print("Stat keyword double-defined: " + entry)
                    sys.exit(62)
                self.var_list[entry] = raw_stats[entry]
            stat_file.close()

    def append_expr_vars(self):
        '''Adds computed variables to var list'''

        expr_manager = ComputeVars(self.template_data.config.expression_dict)

        found_one = True
        while found_one is True:
            found_one = False

            for entry_key in list(self.template_data.config.expression_dict):
                if entry_key in self.var_list:
                    continue

                new_val = expr_manager.compute_value(self.var_list, entry_key)
                if new_val is not None:
                    self.var_list[entry_key] = float(new_val)
                    found_one = True

        for entry_key in list(self.template_data.config.expression_dict):
            if entry_key not in self.var_list:
                self.var_list[entry_key] = 'INVALID'

        
