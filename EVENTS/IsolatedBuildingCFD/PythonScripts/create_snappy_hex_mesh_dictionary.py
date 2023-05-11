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

def write_decomposeParDict_file(input_json_path, template_dict_path, case_path):

    #Read JSON data for turbulence model
    snpy_json_file = open(input_json_path + "/IsolatedBuildingCFD.json")

    # Returns JSON object as a dictionary
    snpy_data = json.load(snpy_json_file)["snappyHexMeshParameters"]
      
    snpy_json_file.close()
    
    num_processors = snpy_data['numProcessors']

    
    #Open the template file (OpenFOAM file) for manipulation
    dict_file = open(template_dict_path + "/decomposeParDictTemplate", "r")

    dict_lines = dict_file.readlines()
    dict_file.close()
    
    #Write number of sub-domains
    start_index = foam.find_keyword_line(dict_lines, "numberOfSubdomains") 
    dict_lines[start_index] = "numberOfSubdomains\t{};\n".format(num_processors)
    
    #Write method of decomposition
    start_index = foam.find_keyword_line(dict_lines, "decomposer") 
    dict_lines[start_index] = "decomposer\t\t{};\n".format("hierarchical")
    

    #Add refinment box geometry
    start_index = foam.find_keyword_line(dict_lines, "hierarchicalCoeffs") + 2 

    added_part = "    n\t\t\t({} {} {});\n".format(2, 2, num_processors//4)
        
    dict_lines.insert(start_index, added_part)
    
    #Write edited dict to file
    write_file_name = case_path + "system/decomposeParDict"
    
    if os.path.exists(write_file_name):
        os.remove(write_file_name)
    
    output_file = open(write_file_name, "w+")
    for line in dict_lines:
        output_file.write(line)
    output_file.close()   
    
def create_snappy_hex_mesh_dict(input_json_path, template_dict_path, output_dict_path):

    #Read JSON data
    json_file = open(input_json_path + "/IsolatedBuildingCFD.json")
      
    # Returns JSON object as a dictionary
    domain_data = json.load(json_file)["snappyHexMeshParameters"]
      
    json_file.close()
    
    scale =  domain_data['geometricScale']
    H = domain_data['buildingHeight']/scale #convert to model-scale
    
    Lx = domain_data['domainLength']
    Ly = domain_data['domainWidth']
    Lz = domain_data['domainHeight']
    Lf = domain_data['fetchLength']
    normalization_type = domain_data['normalizationType']
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
    
    add_prism_layers = domain_data['addPrismLayers']
    number_of_prism_layers = domain_data['numberOfPrismLayers']
    prism_layer_expantion_ratio = domain_data['prismLayerExpantionRatio']
    final_prism_layer_thickness = domain_data['finalPrismLayerThickness']
    prism_layer_surface_name = domain_data['prismLayerSurfaceName']
    prism_layer_relative_size = "on"  

    if normalization_type == "Relative":
        Lx = Lx*H
        Ly = Ly*H
        Lz = Lz*H
        Lf = Lf*H
        origin = origin*H

        for i in range(len(refinement_boxes)):
            for j in range(2, 8, 1):
                refinement_boxes[i][j] = refinement_boxes[i][j]*H
                
        surface_refinement_distance = surface_refinement_distance*H
    
    x_min = -Lf - origin[0]
    y_min = -Ly/2.0 - origin[1]
    z_min =  0.0 - origin[2]

    x_max = x_min + Lx
    y_max = y_min + Ly
    z_max = z_min + Lz    
    
    inside_point  = [x_min + Lf/2.0, (y_min + y_max)/2.0, H]


    #Open the template blockMeshDict (OpenFOAM file) for manipulation
    dict_file = open(template_dict_path + "/snappyHexMeshDictTemplate", "r")

    #Export to OpenFOAM probe format
    dict_lines = dict_file.readlines()
    dict_file.close()
    
    
    #Write 'addLayers' switch    
    start_index = foam.find_keyword_line(dict_lines, "addLayers")
    dict_lines[start_index] = "addLayers\t{};\n".format("on" if add_prism_layers else "off")



    ###################### Edit Geometry Section ##############################
    
    #Add refinment box geometry
    start_index = foam.find_keyword_line(dict_lines, "geometry") + 2 
    added_part = ""
    n_boxes  = len(refinement_boxes)
    for i in range(n_boxes):
        added_part += "    {}\n".format(refinement_boxes[i][0])
        added_part += "    {\n"
        added_part += "         type searchableBox;\n"
        added_part += "         min ({:.4f} {:.4f} {:.4f});\n".format(refinement_boxes[i][2], refinement_boxes[i][3], refinement_boxes[i][4])
        added_part += "         max ({:.4f} {:.4f} {:.4f});\n".format(refinement_boxes[i][5], refinement_boxes[i][6], refinement_boxes[i][7])
        added_part += "    }\n"
        
    dict_lines.insert(start_index, added_part)
       
    #Add building stl geometry
    start_index = foam.find_keyword_line(dict_lines, "geometry") + 2 
    added_part = ""
    added_part += "    {}\n".format(building_stl_name)
    added_part += "    {\n"
    added_part += "         type triSurfaceMesh;\n"
    added_part += "         file \"{}.stl\";\n".format(building_stl_name)
    added_part += "    }\n"
    
    dict_lines.insert(start_index, added_part)
    
    
    ################# Edit castellatedMeshControls Section ####################

    #Write 'nCellsBetweenLevels'     
    start_index = foam.find_keyword_line(dict_lines, "nCellsBetweenLevels")
    dict_lines[start_index] = "    nCellsBetweenLevels {:d};\n".format(num_cells_between_levels)

    #Write 'resolveFeatureAngle'     
    start_index = foam.find_keyword_line(dict_lines, "resolveFeatureAngle")
    dict_lines[start_index] = "    resolveFeatureAngle {:d};\n".format(resolve_feature_angle)

    #Write 'insidePoint'     
    start_index = foam.find_keyword_line(dict_lines, "insidePoint")
    dict_lines[start_index] = "    insidePoint ({:.4f} {:.4f} {:.4f});\n".format(inside_point[0], inside_point[1], inside_point[2])

    #Add refinment edge 
    if add_edge_refinement: 
        start_index = foam.find_keyword_line(dict_lines, "features") + 2 
        added_part  = ""
        added_part += "         {\n"
        added_part += "             file \"{}.eMesh\";\n".format(refinement_edge_name)
        added_part += "             level {};\n".format(edge_refinement_level)
        added_part += "         }\n"
        
        dict_lines.insert(start_index, added_part)
        
    #Add refinment surface
    if add_surface_refinement:         
        start_index = foam.find_keyword_line(dict_lines, "refinementSurfaces") + 2 
        added_part = ""
        added_part += "         {}\n".format(refinement_surface_name)
        added_part += "         {\n"
        added_part += "             level ({} {});\n".format(surface_refinement_level, surface_refinement_level)
        added_part += "             patchInfo\n"
        added_part += "             {\n"
        added_part += "                 type wall;\n"
        added_part += "             }\n"
        added_part += "         }\n"
        
        dict_lines.insert(start_index, added_part)
        
    #Add surface refinment around the building as a refinment region
    # if surface_refinement_level > refinement_boxes[-1][1]:
    added_part = ""
    added_part += "         {}\n".format(refinement_surface_name)
    added_part += "         {\n"
    added_part += "             mode   distance;\n"
    added_part += "             levels  (({:.4f} {}));\n".format(surface_refinement_distance, refinement_boxes[-1][1] + 1)
    added_part += "         }\n"
                
    start_index = foam.find_keyword_line(dict_lines, "refinementRegions") + 2 
    dict_lines.insert(start_index, added_part)
    
    #Add box refinments 
    added_part = ""
    for i in range(n_boxes):
        added_part += "         {}\n".format(refinement_boxes[i][0])
        added_part += "         {\n"
        added_part += "             mode   inside;\n"
        added_part += "             level  {};\n".format(refinement_boxes[i][1])
        added_part += "         }\n"
                
    start_index = foam.find_keyword_line(dict_lines, "refinementRegions") + 2 
    dict_lines.insert(start_index, added_part)
    
    
    ####################### Edit PrismLayer Section ##########################
    
    #Add surface layers (prism layers)
    added_part = ""
    added_part += "         \"{}\"\n".format(prism_layer_surface_name)
    added_part += "         {\n"
    added_part += "             nSurfaceLayers {};\n".format(number_of_prism_layers)
    added_part += "         }\n"
    
    start_index = foam.find_keyword_line(dict_lines, "layers") + 2 
    dict_lines.insert(start_index, added_part)

    #Write 'relativeSizes'     
    start_index = foam.find_keyword_line(dict_lines, "relativeSizes")
    dict_lines[start_index] = "    relativeSizes {};\n".format(prism_layer_relative_size)

    #Write 'expansionRatio'     
    start_index = foam.find_keyword_line(dict_lines, "expansionRatio")
    dict_lines[start_index] = "    expansionRatio {:.4f};\n".format(prism_layer_expantion_ratio)
    
    #Write 'finalLayerThickness'     
    start_index = foam.find_keyword_line(dict_lines, "finalLayerThickness")
    dict_lines[start_index] = "    finalLayerThickness {:.4f};\n".format(final_prism_layer_thickness)    
    
    
    
    #Write edited dict to file
    write_file_name = output_dict_path + "system/snappyHexMeshDict"
    
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
    
    #Read JSON data for turbulence model
    snpy_json_file = open(input_json_path + "/snappyHexMeshParameters.json")

    # Returns JSON object as a dictionary
    snpy_data = json.load(snpy_json_file)
      
    snpy_json_file.close()
    
    run_in_parallel = snpy_data['runInParallel']
    
    # print(run_in_parallel)
    
    if run_in_parallel:
        write_decomposeParDict_file(input_json_path, template_dict_path, output_dict_path)

