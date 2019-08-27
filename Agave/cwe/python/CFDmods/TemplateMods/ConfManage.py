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

ConfManage module manages reading configurations of CFD case templates"""
from __future__ import print_function

import os
import sys
import json
import re

from string import Template

CONDITION_FINDER = re.compile(r'"(.*)"\s*(==|!=)\s*"(.*)"')

class TemplateOpData(object):
    """TemplateOpData will parse the template operator and env vars"""
    STATE_TASK = 0
    STATE_TARGET = 1
    STATE_OTHER = 2
    STATE_ARG = 3
    STATE_FIRST = 4

    def __init__(self, args):
        self.template_dir = False
        self.output_dir = False
        self.stats_dir = False
        self.the_task = False
        self.the_target = False
        self.other_targets = []

        scan_state = TemplateOpData.STATE_FIRST
        save_state = TemplateOpData.STATE_FIRST
        the_arg = "FAIL"

        for entry in args:
            if scan_state == TemplateOpData.STATE_FIRST:
                scan_state = TemplateOpData.STATE_TASK
            elif scan_state == TemplateOpData.STATE_ARG:
                scan_state = save_state
                if (the_arg == '-t') or (the_arg == '-templates'):
                    self.template_dir = entry
                if (the_arg == '-o') or (the_arg == '-out'):
                    self.output_dir = entry
                if the_arg == '-s':
                    self.stats_dir = entry
            elif entry[0] == '-':
                save_state = scan_state
                scan_state = TemplateOpData.STATE_ARG
                the_arg = entry
                if (the_arg == '-t') or (the_arg == '-templates'):
                    if self.template_dir is not False:
                        print("You can only specify the output folder once")
                        sys.exit(1)
                elif (the_arg == '-o') or (the_arg == '-out'):
                    if self.output_dir is not False:
                        print("You can only specify the template folder once")
                        sys.exit(1)
                elif the_arg == '-s':
                    if self.output_dir is not False:
                        print("You can only specify the stats folder once")
                        sys.exit(1)
                else:
                    print(("Unrecognized option: " + the_arg))
                    sys.exit(1)
            else:
                if scan_state == TemplateOpData.STATE_TASK:
                    self.the_task = entry
                    scan_state = TemplateOpData.STATE_TARGET
                elif scan_state == TemplateOpData.STATE_TARGET:
                    self.the_target = entry
                    scan_state = TemplateOpData.STATE_OTHER
                elif scan_state == TemplateOpData.STATE_OTHER:
                    self.other_targets.append(entry)

        if self.template_dir is False:
            try:
                self.template_dir = os.environ['TEMPLATER_FOLDER']
            except KeyError:
                print("You must specify the template folder, either with:")
                print("\t-t ,  -templates or")
                print("\tthe environment variable TEMPLATER_FOLDER")
                sys.exit(1)

        if self.output_dir is False:
            self.output_dir = os.getcwd()

        if self.stats_dir is False:
            self.stats_dir = ''


class TemplateConf(object):
    """TemplateConf read and store data from template config file"""
    def __init__(self, templateRoot, templateName):
        self.template_name = templateName
        self.template_root = templateRoot
        config_file = templateRoot + '/' + templateName + '/config.json'
        try:
            the_file = open(config_file, 'r')
        except IOError:
            print(("Unable to open configuration file for: " + templateName))
            sys.exit(2)

        raw_config = json.load(the_file)
        the_file.close()

        raw_files = raw_config['files']
        raw_scripts = raw_config['scripts']
        raw_vars = raw_config['vars']

        if 'stats' in raw_config:
            self.stat_defaults = raw_config['stats']
        else:
            self.stat_defaults = dict()

        if 'expressions' in raw_config:
            self.expression_dict = raw_config['expressions']
        else:
            self.expression_dict = dict()

        self.script_entries = {}
        for key, value in list(raw_scripts.items()):
            self.script_entries[key] = TaskScript(key, value)

        for an_entry in list(self.script_entries.values()):
            an_entry.get_prereq(self.script_entries)

        self.var_defaults = raw_vars

        self.file_tree_root = self.get_tree_object_from_entry(raw_files, None)

        self.dfs_file_parser(raw_files, self.file_tree_root)

    def dfs_file_parser(self, current_json, current_node):
        """Parses template files DFS"""
        if 'files' not in current_json:
            return
        for a_file in current_json['files']:
            new_node = self.get_tree_object_from_entry(a_file, current_node)
            self.dfs_file_parser(a_file, new_node)

    def get_tree_object_from_entry(self, json_entry, parent_node):
        """Parses on entry in templater config file"""
        data_path = self.template_root + '/' + self.template_name + '/files/'
        if parent_node is not None:
            data_path += parent_node.parent_path() + '/' + json_entry['name']

        if json_entry['type'] == 'provided':
            data_path = None

        try:
            the_condition = json_entry['condition']
        except KeyError:
            the_condition = None

        if ((json_entry['type'] == 'static') or
            (json_entry['type'] == 'variable') or
            (json_entry['type'] == 'provided')):
            return FileTreeObject(json_entry['name'], parent_node,
                                  json_entry['type'], data_path, the_condition)

        if json_entry['type'] == 'result':
            if json_entry['task'] not in self.script_entries:
                print("Task referenced in file listing is not defined.")
                sys.exit(8)
            if 'task' not in json_entry:
                print("Result type must include vaild task name")
                sys.exit(9)
            return FileTreeObject(json_entry['name'], parent_node,
                                  'result', json_entry['task'])

    def __repr__(self):
        ret = "Task Listing:\n"
        for anentry in list(self.script_entries.values()):
            ret += ("%s: (%s %s)\n" % (anentry.scriptName,
                                     anentry.preReqName,
                                     anentry.realName))

        ret += "\n"
        ret += "File Listing:\n"
        ret += self.file_tree_root.recurse_list()

        ret += "\n"
        ret += self.print_all_keys()
        return ret

    def task_data_print(self):
        """Returns list of all tasks for the template"""
        ret = ""
        for anentry in list(self.script_entries.values()):
            ret += repr(anentry)
        return ret

    def print_all_keys(self):
        """Returns all variables an defaults"""
        ret = "Default Variable Listing:\n"
        for key in list(self.var_defaults):
            ret += (key + ": " + self.var_defaults[key] + "\n")
        ret = "Default Stat Listing:\n"
        for key in list(self.stat_defaults):
            ret += (key + ": " + self.stat_defaults[key] + "\n")
        ret = "Expression Listing:\n"
        for key in list(self.expression_dict):
            ret += (key + ": " + self.expression_dict[key] + "\n")
        return ret


class FileTreeObject(object):
    """Object stores a node in the file tree for a template config"""
    def __init__(self, newName, newParent, newType, dataEntry, conditional = None):
        self.children = []
        self.parent = newParent
        self.my_name = newName
        self.my_path = dataEntry
        self.my_task = dataEntry
        self.my_type = newType

        self.my_condition = conditional

        if newParent is not None:
            previous = newParent.get_child_with_name(self.my_name)
            if previous is not False:
                newParent.children.remove(previous)
            newParent.children.append(self)

    def recurse_list(self, level):
        """Recursive list of all files including and below this one"""
        itr = level
        while itr > 0:
            sys.stdout.write("\t")
            itr -= 1
        if self.my_type == 'result':
            print ((self.my_name + ": " + self.my_type + ": " + self.my_task))
        elif self.my_type == 'provided':
            print ((self.my_name + ": " + self.my_type))
        else:
            print ((self.my_name + ": " + self.my_type + ": " + self.my_path))
        for child in self.children:
            child.recurseList(level + 1)

    def is_root(self):
        """Returns true if node has no parent"""
        if self.parent is None:
            return True
        return False

    def parent_path(self):
        """Returns path of this node fron the parent"""
        if self.parent is None:
            return ""
        return self.parent.parent_path() + '/' + self.my_name

    def has_child_with_name(self, needle):
        """Checks if node has a child with name needle"""
        for child in self.children:
            if child.my_name == needle:
                return True
        return False

    def get_child_with_name(self, needle):
        """Gets child node with name needle"""
        for child in self.children:
            if child.my_name == needle:
                return child
        return False

    def condition_accepted(self, varlist):
        '''Verifies if the condition for this file's printing is met'''
        if self.my_condition is None:
            return True

        if isinstance(self.my_condition, list):
            for an_str in self.my_condition:
                if self.check_condition_string(varlist, an_str):
                    return True
            return False

        return self.check_condition_string(varlist, self.my_condition)

    def check_condition_string(self, varlist, string_to_check):
        '''Checks one condition  string for this file'''
        conditions = CONDITION_FINDER.search(string_to_check).groups()

        if len(conditions) != 3:
            print("ERROR: conditional clause parse error, for conditional file")
            sys.exit(12)
            return None

        test_equal = False

        if conditions[1] == "==":
            test_equal = True
        elif conditions[1] == "!=":
            test_equal = False
        else:
            print("ERROR: invalid comparison operator, for conditional file")
            sys.exit(13)
            return None

        side1 = Template(conditions[0]).substitute(varlist)
        side2 = Template(conditions[2]).substitute(varlist)

        return (side1 == side2) is test_equal


class TaskScript(object):
    """Data Object for a template task"""
    def __init__(self, name, rawJSON):
        self.script_name = name
        if 'prereq' in rawJSON:
            self.prereq_name = rawJSON['prereq']
        else:
            self.prereq_name = ""
        self.real_name = rawJSON['fullname']
        self.prereq = None
        self.script_file = rawJSON['file']

    def get_prereq(self, task_list):
        """Connects to prereq task object based on loaded name"""
        if self.prereq_name == "":
            return
        for anentry in list(task_list.values()):
            if self.prereq_name == anentry.script_name:
                self.prereq = anentry
        if self.prereq is None:
            print(("Task prereq not defined: " + self.prereq_name))
            sys.exit(1)

    def __repr__(self):
        ret = self.real_name + " ("
        if self.prereq is None:
            ret += self.script_name + ")\n"
        else:
            ret += self.prereq_name + "=>" + self.script_name + ")\n"
        return ret
            
