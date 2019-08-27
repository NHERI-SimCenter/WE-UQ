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

Manages computed vars, both paring and getting computed values"""
from __future__ import print_function

import re
import sys

PAREN_FINDER = re.compile(r'(\(|\))')

class ComputeVars(object):
    '''
    This object will parse computed vars equations and
    compute the results.
    '''

    def __init__(self, newVarsDict):
        '''
        A compute vars database gets a dictionary of varnames to
        computed extression strings
        '''
        self.var_dict = newVarsDict

    def compute_value(self, known_vars, to_compute):
        '''Returns either the float value of a computed var or None if invalid'''
        try:
            target_expression = self.var_dict[to_compute]
        except KeyError:
            return None

        split_result = self.find_inner_paren_group(target_expression)
        while split_result is not None:
            new_result = self.compute_simple_expression(split_result[1], known_vars)
            if new_result is None:
                return None
            target_expression = split_result[0]+str(new_result)+split_result[2]
            split_result = self.find_inner_paren_group(target_expression)

        return self.compute_simple_expression(target_expression, known_vars)

    def compute_simple_expression(self, the_expr, var_list):
        '''This does simplified order of operations to evaluate an expression'''
        finder_list = ['-', '+', '*', '/', '^']

        for operation in finder_list:
            index = the_expr.rfind(operation)
            if index == -1:
                continue
            split_expr = the_expr.rsplit(operation,1)
            if len(split_expr) != 2:
                return None

            left_side = self.compute_simple_expression(split_expr[0], var_list)
            right_side = self.compute_simple_expression(split_expr[1], var_list)
            if left_side is None:
                return None
            if right_side is None:
                return None

            if operation == '-':
                return float(left_side) - float(right_side)
            if operation == '+':
                return float(left_side) + float(right_side)
            if operation == '/':
                return float(left_side) / float(right_side)
            if operation == '*':
                return float(left_side) * float(right_side)
            if operation == '^':
                return float(left_side) ** float(right_side)

        true_exp = the_expr.strip()

        try:
            return float(var_list[true_exp])
        except KeyError:
            pass

        try:
            return float(true_exp)
        except ValueError:
            return None

        return None

    def find_inner_paren_group(self, the_string):
        '''Locates a pair of matched parens. Return none of no parens'''
        broken_up_string = PAREN_FINDER.split(the_string)

        ret = []

        if len(broken_up_string) < 2:
            return None

        for i in range(len(broken_up_string) - 2):
            if (broken_up_string[i] == '(') and (broken_up_string[i + 2] == ')'):
                ret.append("".join(broken_up_string[0:i]))
                ret.append(broken_up_string[i + 1])
                if len(broken_up_string) > i + 3:
                    ret.append("".join(broken_up_string[i + 3:]))
                return ret

        print("ERROR: ( ) mismatch")
        sys.exit(50)
        return None
