"""
This script contains functions for reading and writing OpenFoam dictionaries. 

"""
def find_keyword_line(dict_file, keyword):
    
    start_line = -1
    dict_lines = dict_file.readlines()
    
    count = 0
    for line in dict_lines:
        l = line.lstrip(" ")
        
        if l.startswith(keyword):
            start_line = count
            break
        
        count += 1

    return start_line
    
