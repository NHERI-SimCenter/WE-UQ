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

Executable for CFD templater program"""
from __future__ import print_function

import sys
import os
import json
import shutil

from CFDmods.TemplateMods.ConfManage import TemplateOpData
from CFDmods.TemplateMods.KeyValFuncs import KeyValDataSet
from CFDmods.TemplateMods.TemplateManage import TemplateData

STATE_EXPECT_KEY = 0
STATE_EXPECT_VAL = 1
STATE_EXPECT_ARRAY = 2

def print_help_text():
    """Prints help text and exits"""
    print ('''\
Usage:")
templater [command] [arguments]")
          [-o alternate output folder]")
          [-t alternate template folder]
          [-s optional stats folder]")

If -o is not used, output goes to current working directory")

Options:
Managing Cases:
create template
    -create case with default values, in target directory.
    -only works if target directory is non-existant
duplicate case
    -duplicate case by creating a new one with same values as base,
    -in current target directory (must be non-existant)
getTasks
    -Printout a listing of all tasks for the template
rollback task_name
    -deletes nessesary files to rollback case to
    -a particular task name (recursive) or
    -(if ALL specified) everything
setval key value key2 val2 key3 val3
    -sets the value defined by key
    -Will update corresponding files
getval key . . .
    -get listing of selected key/value pairs
getkeys
    -get listing of all keys for a case
pullAllVals
    -dumps keyvals for reading by front-end:
getname
    -outputs template name for case

Managing Templates:
confirm template
    -Check template for consistancy
    ''')
    sys.exit(0)

def main(args):
    """Main function of Templater"""
    the_config = TemplateOpData(args)

    if (the_config.the_task is False) or (the_config.the_task == 'help'):
        print_help_text()

    if the_config.the_task == 'getkeys':
        case_keyvals = KeyValDataSet(the_config.template_dir, the_config.output_dir)
        case_keyvals.print_keys()
        sys.exit(0)
    if the_config.the_task == 'getTasks':
        case_keyvals = KeyValDataSet(the_config.template_dir, the_config.output_dir)
        case_keyvals.task_data_print()()
        sys.exit(0)
    if the_config.the_task == 'pullAllVals':
        case_keyvals = KeyValDataSet(the_config.template_dir, the_config.output_dir,
                                     the_config.stats_dir)
        case_keyvals.pull_all_vals()
        sys.exit(0)
    if the_config.the_task == 'getname':
        the_data = KeyValDataSet(the_config.template_dir, the_config.output_dir)
        print(the_data.template_name)
        sys.exit(0)
    if the_config.the_task == 'paramset':
        case_keyvals = KeyValDataSet(the_config.template_dir, the_config.output_dir,
                                     the_config.stats_dir)
        case_keyvals.emit_new_files()
        print("Parameters dumped to files")
        sys.exit(0)

    if the_config.the_task == 'getdatafolder':
        case_vals = KeyValDataSet(the_config.template_dir, the_config.output_dir)
        print(case_vals.template_data.get_data_folder())
        sys.exit(0)

    if the_config.the_target is False:
        print_help_text()

    if the_config.the_task == 'create':
        try:
            cwd_ls = os.listdir(the_config.output_dir)
        except os.error:
            print("Target folder for case creation must be exist")
            sys.exit(1)
        cwd_ls = os.listdir(the_config.output_dir)
        if len(cwd_ls) != 0:
            print("Target folder for new case must be empty")
            sys.exit(1)
        try:
            var_file = open(the_config.output_dir + '/.varStore', 'w')
        except IOError:
            print ("Unable to open folder to write files")
            sys.exit(2)

        out_object = {}
        out_object['type'] = the_config.the_target
        out_object['vars'] = {}
        outstr = json.dumps(out_object, indent=1, separators=(',', ':'))
        var_file.write(outstr)
        var_file.close()

        case_keyvals = KeyValDataSet(the_config.template_dir, the_config.output_dir)
        case_keyvals.emit_new_files()
        print(("New case created in " + the_config.output_dir))
        sys.exit(0)

    if the_config.the_task == 'duplicate':
        try:
            cwd_ls = os.listdir(the_config.output_dir)
        except os.error:
            print("Target folder for case creation must be exist")
            sys.exit(1)
        if len(cwd_ls) != 0:
            print("Target folder for new case must be empty")
            sys.exit(1)
        try:
            in_file = open(the_config.the_target + '/.varStore', 'r')
        except IOError:
            print ("Unable to open folder to read case files")
            sys.exit(2)
        try:
            out_file = open(the_config.output_dir + '/.varStore', 'w')
        except IOError:
            print ("Unable to open folder to write case files")
            sys.exit(2)

        for aline in in_file:
            out_file.write(aline)
        in_file.close()
        out_file.close()

        case_keyvals = KeyValDataSet(the_config.template_dir, the_config.output_dir)

        files_list = os.listdir(the_config.the_target)

        for afile in files_list:
            if afile == ".varStore":
                continue

            from_file = the_config.the_target + "/" + afile
            to_file = the_config.output_dir + "/" + afile
            if os.path.isdir(from_file):
                shutil.copytree(from_file, to_file)
            else:
                shutil.copy(from_file, to_file)

        case_keyvals.emit_new_files()
        print(("New, duplicate case created in " + the_config.output_dir))
        sys.exit(0)

    if the_config.the_task == 'rollback':
        case_keyvals = KeyValDataSet(the_config.template_dir, the_config.output_dir)
        case_keyvals.roll_back_to_task(the_config.the_target)
        if the_config.the_target == 'all':
            print("Case files rolled back to before all tasks")
        else:
            print(("Case files rolled back to before task: " + the_config.the_target))
        sys.exit(0)

    if the_config.the_task == 'setval':
        case_keyvals = KeyValDataSet(the_config.template_dir, the_config.output_dir,
                                     the_config.stats_dir)
        in_list = [the_config.the_target]
        in_list.extend(the_config.other_targets)

        scan_state = STATE_EXPECT_KEY
        key_entry = "FAIL"

        for entry in in_list:
            if scan_state == STATE_EXPECT_KEY:
                scan_state = STATE_EXPECT_VAL
                key_entry = entry
            elif scan_state == STATE_EXPECT_VAL:
                scan_state = STATE_EXPECT_KEY
                case_keyvals.set_keyval(key_entry, entry)

        if scan_state != STATE_EXPECT_KEY:
            print("Parse error on input list. Alternate key/vals")
            sys.exit(0)

        case_keyvals.emit_new_files()
        case_keyvals.dump_keyvals()
        print("New variables set for case.")
        sys.exit(0)

    if the_config.the_task == 'getval':
        case_keyvals = KeyValDataSet(the_config.template_dir, the_config.output_dir)
        key_list = [the_config.the_target]
        key_list.extend(the_config.other_targets)
        case_keyvals.print_keyvals(key_list)
        sys.exit(0)

    if the_config.the_task == 'confirm':
        the_data = TemplateData(the_config.template_dir, the_config.the_target)
        print(the_data)
        print("Global and template configurations read sucessfully.")
        sys.exit(0)

    if the_config.the_task == 'getscript':
        case_vals = KeyValDataSet(the_config.template_dir, the_config.output_dir)
        print(case_vals.template_data.get_script_file(the_config.the_target))
        sys.exit(0)

    if the_config.the_task == 'getlaststage':
        case_vals = KeyValDataSet(the_config.template_dir, the_config.output_dir)
        last_stage = case_vals.template_data.get_last_stage(the_config.the_target)
        if last_stage is None:
            print("None")
            sys.exit(0)
        print(last_stage.script_name)
        sys.exit(0)

    print_help_text()

if __name__ == '__main__':
    main(sys.argv)
