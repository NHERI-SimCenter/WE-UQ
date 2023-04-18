"""
This script create blockMeshDict for OpenFoam given vertices and boundary type. 


code creates pressure probes for the main simulation. Three types of 
probes are created. 

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


def create_snappy_hex_mesh_dict(input_json_path, template_dict_path, output_dict_path):

    #Read JSON data
    json_file = open(input_json_path + "/snappyHexMeshParameters.json")
      
    # Returns JSON object as a dictionary
    domain_data = json.load(json_file)
      
    json_file.close()
    
    scale =  domain_data['geometricScale']
    H = domain_data['buildingHeight']/scale #convert to model-scale
    
    Lx = domain_data['domainLength']
    Ly = domain_data['domainWidth']
    Lz = domain_data['domainHeight']
    Lf = domain_data['fetchLength']
    relative_dimensions = domain_data['relativeDimensions']
    origin = np.array(domain_data['origin'])
    length_unit = domain_data['lengthUnit']
    
    building_stl_name = domain_data['buildingSTLName']
    num_cells_between_levels = domain_data['numCellsBetweenLevels']
    resolve_feature_angle = domain_data['resolveFeatureAngle']
    num_processors = domain_data['numProcessors']
    
    refinement_boxes = domain_data['refinementBoxes']
    
    add_surface_refinement = domain_data['addSurfaceRefinement']
    surface_refinement_level = domain_data['surfaceRefinementLevel']
    surface_refinement_distance = domain_data['surfaceRefinementDistance']
    refinement_surface_name = domain_data['refinementSurfaceName']
    
    add_edge_refinement = domain_data['addEdgeRefinement']
    edge_refinement_level = domain_data['edgeRefinementLevel']
    refinement_edge_name = domain_data['refinementEdgeName']
    
    add_prismLayers = domain_data['addPrismLayers']
    prism_layer_expantion_ratio = domain_data['prismLayerExpantionRatio']
    final_prismLayer_thickness = domain_data['finalPrismLayerThickness']
    prism_layer_surface_name = domain_data['prismLayerSurfaceName']
    prism_layer_relative_size = "on"
    
    inside_point  = [1.0, 1.0, 1.0]

    if relative_dimensions:
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
    dict_file = open(template_dict_path + "/snappyHexMeshDictTemplate", "r")

    #Export to OpenFOAM probe format
    dict_lines = dict_file.readlines()
    dict_file.close()
    
    dict_lines[18] = "addLayers\t{};\n".format(add_prismLayers)

    dict_lines[23] = "    buildingSTLName\t\t\"{}.stl\";\n".format(building_stl_name)

    dict_lines[24] = "    nCellsBetweenLevels\t\t{:d};\n".format(num_cells_between_levels)
    dict_lines[25] = "    resolveFeatureAngle\t\t{:d};\n".format(resolve_feature_angle)    
    dict_lines[26] = "    insidePoint\t\t\t({:.4f} {:.4f} {:.4f});\n".format(inside_point[0], inside_point[1], inside_point[2])
    
    dict_lines[27] = "    refinementSurfaceName\t\"{}\";\n".format(refinement_surface_name)
    dict_lines[28] = "    surfaceRefinementDistance\t{:.4f};\n".format(surface_refinement_distance)
    dict_lines[29] = "    surfaceRefinementLevel\t{:d};\n".format(surface_refinement_level)
    
    dict_lines[30] = "    refinementEdgeName\t\t\"{}\";\n".format(refinement_edge_name)
    dict_lines[31] = "    edgeRefinementLevel\t\t{:d};\n".format(edge_refinement_level)

    dict_lines[32] = "    prismLayerSurfaceName\t\"{}\";\n".format(prism_layer_surface_name)
    dict_lines[33] = "    nSurfaceLayers\t\t{:d};\n".format(resolve_feature_angle)
    dict_lines[34] = "    relativeSizes\t\t{};\n".format(prism_layer_relative_size)
    dict_lines[35] = "    expansionRatio\t\t{:.4f};\n".format(prism_layer_expantion_ratio)
    dict_lines[36] = "    finalLayerThickness\t\t{:.4f};\n".format(final_prismLayer_thickness)

 
    # dict_lines[100] = "\t file  \"{}.eMesh\"; \n".format(refinement_edge_name)
    # dict_lines[101] = "\t level {:d};\n".format(edge_refinement_level)


    # convert_to_meters = 1.0

    # if length_unit=='m':
    #     convert_to_meters = 1.0
    # elif length_unit=='cm':
    #     convert_to_meters = 0.01
    # elif length_unit=='mm':
    #     convert_to_meters = 0.001
    # elif length_unit=='ft':
    #     convert_to_meters = 0.3048
    # elif length_unit=='in':
    #     convert_to_meters = 0.0254

    # dict_lines[31] = "convertToMeters {:.4f};\n".format(convert_to_meters)
    # dict_lines[61] = "        type {};\n".format(inlet_type)
    # dict_lines[70] = "        type {};\n".format(outlet_type)
    # dict_lines[79] = "        type {};\n".format(ground_type)
    # dict_lines[88] = "        type {};\n".format(top_type)
    # dict_lines[97] = "        type {};\n".format(front_type)
    # dict_lines[106] = "        type {};\n".format(back_type)

    
    write_file_name = output_dict_path + "/snappyHexMeshDict"
    
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
    
    create_snappy_hex_mesh_dict(input_json_path, template_dict_path, output_dict_path)
