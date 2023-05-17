"""
This script contains functions for reading and writing OpenFoam dictionaries. 

"""
def find_keyword_line(dict_lines, keyword):
    
    start_line = -1
    
    count = 0
    for line in dict_lines:
        l = line.lstrip(" ")
        
        if l.startswith(keyword):
            start_line = count
            break
        
        count += 1

    return start_line
    
