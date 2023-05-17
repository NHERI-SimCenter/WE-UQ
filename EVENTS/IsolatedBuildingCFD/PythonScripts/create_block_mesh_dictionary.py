"""
This script create blockMeshDict for OpenFoam given vertices and boundary type. 


code creates pressure probes for the main simulation. Three types of 
probes are created. 

"""
import numpy as np
import sys
import os
import json


def create_block_mesh_dict(input_json_path, template_dict_path, output_dict_path):

    #Read JSON data
    json_file = open(input_json_path + "/IsolatedBuildingCFD.json")
      
    # Returns JSON object as a dictionary
    domain_data = json.load(json_file)["blockMeshParameters"]
      
    json_file.close()
    
    scale =  domain_data['geometricScale']
    H = domain_data['buildingHeight']/scale #convert to model-scale
    
    Lx = domain_data['domainLength']
    Ly = domain_data['domainWidth']
    Lz = domain_data['domainHeight']
    Lf = domain_data['fetchLength']
    
    x_cells = domain_data['xNumCells']
    y_cells = domain_data['yNumCells']
    z_cells = domain_data['zNumCells']
    
    x_grading = domain_data['xGrading']
    y_grading = domain_data['yGrading']
    z_grading = domain_data['zGrading']

    inlet_type = domain_data['inletBoundaryType']
    outlet_type = domain_data['outletBoundaryType']
    ground_type = domain_data['groundBoundaryType']
    top_type = domain_data['topBoundaryType']
    front_type = domain_data['frontBoundaryType']
    back_type = domain_data['backBoundaryType']
    
    normalization_type = domain_data['normalizationType']
    origin = np.array(domain_data['origin'])
    length_unit = domain_data['lengthUnit']

    if normalization_type == "Relative":
        Lx = Lx*H
        Ly = Ly*H
        Lz = Lz*H
        Lf = Lf*H
        origin = origin*H
    
    x_min = -Lf - origin[0]
    y_min = -Ly/2.0 - origin[1]
    z_min =  0.0 - origin[2]

    x_max = x_min + Lx
    y_max = y_min + Ly
    z_max = z_min + Lz

    #Open the template blockMeshDict (OpenFOAM file) for manipulation
    dict_file = open(template_dict_path + "/blockMeshDictTemplate", "r")

    #Export to OpenFOAM probe format
    dict_lines = dict_file.readlines()
    dict_file.close()
    

    dict_lines[17] = "\txMin\t\t{:.4f};\n".format(x_min)
    dict_lines[18] = "\tyMin\t\t{:.4f};\n".format(y_min)
    dict_lines[19] = "\tzMin\t\t{:.4f};\n".format(z_min)

    dict_lines[20] = "\txMax\t\t{:.4f};\n".format(x_max)
    dict_lines[21] = "\tyMax\t\t{:.4f};\n".format(y_max)
    dict_lines[22] = "\tzMax\t\t{:.4f};\n".format(z_max)


    dict_lines[23] = "\txCells\t\t{:d};\n".format(x_cells)
    dict_lines[24] = "\tyCells\t\t{:d};\n".format(y_cells)
    dict_lines[25] = "\tzCells\t\t{:d};\n".format(z_cells)

    dict_lines[26] = "\txGrading\t{:.4f};\n".format(x_grading)
    dict_lines[27] = "\tyGrading\t{:.4f};\n".format(y_grading)
    dict_lines[28] = "\tzGrading\t{:.4f};\n".format(z_grading)

    convert_to_meters = 1.0

    if length_unit=='m':
        convert_to_meters = 1.0
    elif length_unit=='cm':
        convert_to_meters = 0.01
    elif length_unit=='mm':
        convert_to_meters = 0.001
    elif length_unit=='ft':
        convert_to_meters = 0.3048
    elif length_unit=='in':
        convert_to_meters = 0.0254

    dict_lines[31] = "convertToMeters {:.4f};\n".format(convert_to_meters)
    dict_lines[61] = "        type {};\n".format(inlet_type)
    dict_lines[70] = "        type {};\n".format(outlet_type)
    dict_lines[79] = "        type {};\n".format(ground_type)
    dict_lines[88] = "        type {};\n".format(top_type)
    dict_lines[97] = "        type {};\n".format(front_type)
    dict_lines[106] = "        type {};\n".format(back_type)

    
    write_file_name = output_dict_path + "/blockMeshDict"
    
    if os.path.exists(write_file_name):
        os.remove(write_file_name)
    
    output_file = open(write_file_name, "w+")
    for line in dict_lines:
        output_file.write(line)
    output_file.close()

if __name__ == '__main__':    
    
    input_args = sys.argv

    # Set filenames
    input_json_path = sys.argv[1]
    template_dict_path = sys.argv[2]
    output_dict_path = sys.argv[3]
    
    # data_path = os.getcwd()
    # script_path = os.path.dirname(os.path.realpath(__file__))
    
    create_block_mesh_dict(input_json_path, template_dict_path, output_dict_path)
