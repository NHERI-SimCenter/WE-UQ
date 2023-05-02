"""
This script writes BC and initial condition in OpenFoam case directory.

"""
import numpy as np
import pandas as pd
import sys
import os
import subprocess
import json
import stat
import shutil
import numpy as np
import foam_dict_reader as foam


def write_U_file(input_json_path, template_dict_path, output_dict_path):

    #Read JSON data
    bc_json_file = open(input_json_path + "/boundaryConditions.json")
    wc_json_file = open(input_json_path + "/windCharacteristics.json")
      
    # Returns JSON object as a dictionary
    boundary_data = json.load(bc_json_file)
    wind_data = json.load(wc_json_file)
      
    wc_json_file.close()
    bc_json_file.close()
    
    inlet_BC_type =  boundary_data['inletBoundaryCondition']
    top_BC_type = boundary_data['topBoundaryCondition']
    sides_BC_type = boundary_data['sidesBoundaryCondition']
    building_BC_type = boundary_data['buildingBoundaryCondition']
 
    wind_speed = wind_data['roofHeightWindSpeed']
    building_height = wind_data['buildingHeight']
    roughness_length = wind_data['aerodynamicRoughnessLength']


    #Open the template file (OpenFOAM file) for manipulation
    dict_file = open(template_dict_path + "/UFileTemplate", "r")

    dict_lines = dict_file.readlines()
    dict_file.close()
    
    
    #Write 'Uinlet' for later reference    
    start_index = foam.find_keyword_line(dict_lines, "Uinlet")
    dict_lines[start_index] = "Uinlet \t ({} 0 0);\n".format(wind_speed)


    ##################### Internal Field #########################
    
    start_index = foam.find_keyword_line(dict_lines, "internalField") 
    dict_lines[start_index] = "internalField   uniform ({:.4f} 0 0);\n".format(wind_speed)


    ###################### Inlet BC ##############################  
    #Write uniform
    start_index = foam.find_keyword_line(dict_lines, "inlet") + 2 

    if inlet_BC_type == "Uniform":    
        added_part = ""
        added_part += "         type fixedValue;\n"
        added_part += "         value ({:.4f} 0 0);\n".format(wind_speed)
        
    if inlet_BC_type == "MeanABL":    
        added_part = ""
        added_part += "         type atmBoundaryLayerInletVelocity;\n"
        added_part += "         Uref {:.4f};\n".format(wind_speed)
        added_part += "         Zref (0.0 0.0 {:.4f});\n".format(building_height)
        added_part += "         zDir (1.0 0.0 0.0);\n"
        added_part += "         z0 uniform {:.4f};\n".format(roughness_length)
        added_part += "         zGround uniform 0.0;\n"
        
    if inlet_BC_type == "Place holder for TInf":    
        added_part = ""
        
    dict_lines.insert(start_index, added_part)

    ###################### Outlet BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "outlet") + 2 
    added_part = ""
    added_part += "     type    pressureInletOutletVelocity;\n"
    added_part += "     value   uniform (0 0 0);\n"
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Ground BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "ground") + 2 
    added_part = ""
    added_part += "     type    uniformFixedValue;\n"
    added_part += "     value   uniform (0 0 0);\n"
    added_part += "     uniformValue   constant (0 0 0);\n"
    
    dict_lines.insert(start_index, added_part)
    
    
    ###################### Top BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "top") + 2 
    added_part = ""
    added_part += "     type    {};\n".format(top_BC_type)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Front BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "front") + 2 
    added_part = ""
    added_part += "     type    {};\n".format(sides_BC_type)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Back BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "back") + 2 
    added_part = ""
    added_part += "     type    {};\n".format(sides_BC_type)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Building BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "building") + 2 
    added_part = ""
    added_part += "     type    {};\n".format(building_BC_type)
    
    dict_lines.insert(start_index, added_part)
    

def write_p_file(input_json_path, template_dict_path, output_dict_path):

    #Read JSON data
    bc_json_file = open(input_json_path + "/boundaryConditions.json")
      
    # Returns JSON object as a dictionary
    boundary_data = json.load(bc_json_file)
      
    bc_json_file.close()
    
    sides_BC_type = boundary_data['sidesBoundaryCondition']
    top_BC_type = boundary_data['topBoundaryCondition']


    #Open the template file (OpenFOAM file) for manipulation
    dict_file = open(template_dict_path + "/pFileTemplate", "r")

    dict_lines = dict_file.readlines()
    dict_file.close()
    
    
    #BC and initial condition
    p0 = 0.0; 


    ##################### Internal Field #########################
    
    start_index = foam.find_keyword_line(dict_lines, "internalField") 
    dict_lines[start_index] = "internalField   uniform {:.4f};\n".format(p0)


    ###################### Inlet BC ##############################  
    #Write uniform
    start_index = foam.find_keyword_line(dict_lines, "inlet") + 2 
    added_part = ""
    added_part += "     type    zeroGradient;\n"
    
    dict_lines.insert(start_index, added_part)

    ###################### Outlet BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "outlet") + 2 
    added_part = ""
    added_part += "     type            uniformFixedValue;\n"
    added_part += "     uniformValue    constant {:.4f};\n".format(p0)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Ground BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "ground") + 2 
    added_part = ""
    added_part += "     type    zeroGradient;\n"
    
    dict_lines.insert(start_index, added_part)
    
    
    ###################### Top BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "top") + 2 
    added_part = ""
    added_part += "     type    {};\n".format(top_BC_type)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Front BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "front") + 2 
    added_part = ""
    added_part += "     type    {};\n".format(sides_BC_type)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Back BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "back") + 2 
    added_part = ""
    added_part += "     type    {};\n".format(sides_BC_type)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Building BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "building") + 2 
    added_part = ""
    added_part += "     type    zeroGradient;\n"
    
    dict_lines.insert(start_index, added_part)
    
def write_nut_file(input_json_path, template_dict_path, output_dict_path):

    #Read JSON data
    bc_json_file = open(input_json_path + "/boundaryConditions.json")
    wc_json_file = open(input_json_path + "/windCharacteristics.json")

    # Returns JSON object as a dictionary
    boundary_data = json.load(bc_json_file)
    wind_data = json.load(wc_json_file)
      
    bc_json_file.close()
    wc_json_file.close()
    
    sides_BC_type = boundary_data['sidesBoundaryCondition']
    top_BC_type = boundary_data['topBoundaryCondition']
    ground_BC_type = boundary_data['groundBoundaryCondition']

    wind_speed = wind_data['roofHeightWindSpeed']
    building_height = wind_data['buildingHeight']
    roughness_length = wind_data['aerodynamicRoughnessLength']
    
    #Open the template file (OpenFOAM file) for manipulation
    dict_file = open(template_dict_path + "/pFileTemplate", "r")

    dict_lines = dict_file.readlines()
    dict_file.close()
    
    
    #BC and initial condition
    p0 = 0.0 

    ##################### Internal Field #########################
    
    start_index = foam.find_keyword_line(dict_lines, "internalField") 
    dict_lines[start_index] = "internalField   uniform {:.4f};\n".format(p0)


    ###################### Inlet BC ##############################  
    #Write uniform
    start_index = foam.find_keyword_line(dict_lines, "inlet") + 2 
    added_part  = ""
    added_part += "     type    zeroGradient;\n"
    
    dict_lines.insert(start_index, added_part)

    ###################### Outlet BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "outlet") + 2 
    added_part = ""
    added_part += "     type            uniformFixedValue;\n"
    added_part += "     uniformValue    constant {:.4f};\n".format(p0)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Ground BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "ground") + 2 
    
    if ground_BC_type == "noSlip": 
        added_part = ""
        added_part += "     type    zeroGradient;\n"
    
    if ground_BC_type == "wallFunction": 
        added_part = ""
        added_part += "     type    nutkAtmRoughWallFunction;\n"
        added_part += "     z0      {:.4f};\n".format(roughness_length)
        added_part += "     value   uniform 0.0;\n"
    
    dict_lines.insert(start_index, added_part)
    
    
    ###################### Top BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "top") + 2 
    added_part = ""
    added_part += "     type    {};\n".format(top_BC_type)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Front BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "front") + 2 
    added_part = ""
    added_part += "     type    {};\n".format(sides_BC_type)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Back BC ################################  
    
    start_index = foam.find_keyword_line(dict_lines, "back") + 2 
    added_part = ""
    added_part += "     type    {};\n".format(sides_BC_type)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Building BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "building") + 2 
    added_part = ""
    added_part += "     type    zeroGradient;\n"
    
    dict_lines.insert(start_index, added_part)

if __name__ == '__main__':    
    
    input_args = sys.argv

    # Set filenames
    input_json_path = sys.argv[1]
    template_dict_path = sys.argv[2]
    output_dict_path = sys.argv[3]
    
    # data_path = os.getcwd()
    # script_path = os.path.dirname(os.path.realpath(__file__))
    
    create_snappy_hex_mesh_dict(input_json_path, template_dict_path, output_dict_path)
