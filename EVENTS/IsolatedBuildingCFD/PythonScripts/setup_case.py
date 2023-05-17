"""
This script writes BC and initial condition, and setups the OpenFoam case 
directory.

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


def write_U_file(input_json_path, template_dict_path, case_path):

    #Read JSON data
    json_file = open(input_json_path + "/IsolatedBuildingCFD.json")

    # Returns JSON object as a dictionary
    boundary_data = json.load(json_file)["boundaryConditions"]
    wind_data = json.load(json_file)["windCharacteristics"]
      
    json_file.close()
    
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
    
    ##################### Internal Field #########################
    
    start_index = foam.find_keyword_line(dict_lines, "internalField") 
    dict_lines[start_index] = "internalField   uniform ({:.4f} 0 0);\n".format(wind_speed)


    ###################### Inlet BC ##############################  
    #Write uniform
    start_index = foam.find_keyword_line(dict_lines, "inlet") + 2 

    if inlet_BC_type == "Uniform":    
        added_part = ""
        added_part += "\t type \t fixedValue;\n"
        added_part += "\t value \t uniform ({:.4f} 0 0);\n".format(wind_speed)
        
    if inlet_BC_type == "MeanABL":    
        added_part = ""
        added_part += "\t ype \t atmBoundaryLayerInletVelocity;\n"
        added_part += "\t Uref \t {:.4f};\n".format(wind_speed)
        added_part += "\t Zref \t {:.4f};\n".format(building_height)
        added_part += "\t zDir \t (0.0 0.0 1.0);\n"
        added_part += "\t z0 uniform \t {:.4f};\n".format(roughness_length)
        added_part += "\t zGround \t uniform 0.0;\n"
        
    if inlet_BC_type == "Place holder for TInf":    
        added_part = ""
        
    dict_lines.insert(start_index, added_part)

    ###################### Outlet BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "outlet") + 2 
    added_part = ""
    added_part += "\t type \t pressureInletOutletVelocity;\n"
    added_part += "\t value \t uniform ({:.4f} 0 0);\n".format(wind_speed)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Ground BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "ground") + 2 
    added_part = ""
    added_part += "\t type \t uniformFixedValue;\n"
    added_part += "\t value \t uniform (0 0 0);\n"
    added_part += "\t uniformValue \t constant (0 0 0);\n"
    
    dict_lines.insert(start_index, added_part)
    
    
    ###################### Top BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "top") + 2 
    added_part = ""
    added_part += "\t type    {};\n".format(top_BC_type)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Front BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "front") + 2 
    added_part = ""
    added_part += "\t type \t {};\n".format(sides_BC_type)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Back BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "back") + 2 
    added_part = ""
    added_part += "\t type    {};\n".format(sides_BC_type)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Building BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "building") + 2 
    added_part = ""
    added_part += "\t type \t {};\n".format("noSlip")
    
    dict_lines.insert(start_index, added_part)
    
    
    #Write edited dict to file
    write_file_name = case_path + "0/U"
    
    if os.path.exists(write_file_name):
        os.remove(write_file_name)
    
    output_file = open(write_file_name, "w+")
    for line in dict_lines:
        output_file.write(line)
    output_file.close()
    

def write_p_file(input_json_path, template_dict_path, case_path):

    #Read JSON data
    json_file = open(input_json_path + "/IsolatedBuildingCFD.json")

    # Returns JSON object as a dictionary
    boundary_data = json.load(json_file)["boundaryConditions"]
      
    json_file.close()
    
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
    added_part += "\t type \t zeroGradient;\n"
    
    dict_lines.insert(start_index, added_part)

    ###################### Outlet BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "outlet") + 2 
    added_part = ""
    added_part += "\t type \t  uniformFixedValue;\n"
    added_part += "\t uniformValue \t constant {:.4f};\n".format(p0)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Ground BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "ground") + 2 
    added_part = ""
    added_part += "\t type \t zeroGradient;\n"
    
    dict_lines.insert(start_index, added_part)
    
    
    ###################### Top BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "top") + 2 
    added_part = ""
    added_part += "\t type \t {};\n".format(top_BC_type)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Front BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "front") + 2 
    added_part = ""
    added_part += "\t type \t {};\n".format(sides_BC_type)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Back BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "back") + 2 
    added_part = ""
    added_part += "\t type \t {};\n".format(sides_BC_type)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Building BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "building") + 2 
    added_part = ""
    added_part += "\t type  \t zeroGradient;\n"
    
    dict_lines.insert(start_index, added_part)
    
    
    #Write edited dict to file
    write_file_name = case_path + "0/p"
    
    if os.path.exists(write_file_name):
        os.remove(write_file_name)
    
    output_file = open(write_file_name, "w+")
    for line in dict_lines:
        output_file.write(line)
    output_file.close()
    
def write_nut_file(input_json_path, template_dict_path, case_path):

    #Read JSON data
    json_file = open(input_json_path + "/IsolatedBuildingCFD.json")

    # Returns JSON object as a dictionary
    boundary_data = json.load(json_file)["boundaryConditions"]
    wind_data = json.load(json_file)["windCharacteristics"]
      
    json_file.close()
    
    sides_BC_type = boundary_data['sidesBoundaryCondition']
    top_BC_type = boundary_data['topBoundaryCondition']
    ground_BC_type = boundary_data['groundBoundaryCondition']
    building_BC_type = boundary_data['buildingBoundaryCondition']

    # wind_speed = wind_data['roofHeightWindSpeed']
    # building_height = wind_data['buildingHeight']
    roughness_length = wind_data['aerodynamicRoughnessLength']
    
    #Open the template file (OpenFOAM file) for manipulation
    dict_file = open(template_dict_path + "/nutFileTemplate", "r")

    dict_lines = dict_file.readlines()
    dict_file.close()
    
    
    #BC and initial condition
    nut0 = 0.0 

    ##################### Internal Field #########################
    
    start_index = foam.find_keyword_line(dict_lines, "internalField") 
    dict_lines[start_index] = "internalField   uniform {:.4f};\n".format(nut0)


    ###################### Inlet BC ##############################  
    #Write uniform
    start_index = foam.find_keyword_line(dict_lines, "inlet") + 2 
    added_part  = ""
    added_part += "\t type \t zeroGradient;\n"
    
    dict_lines.insert(start_index, added_part)

    ###################### Outlet BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "outlet") + 2 
    added_part = ""
    added_part += "\t type \t uniformFixedValue;\n"
    added_part += "\t uniformValue \t constant {:.4f};\n".format(nut0)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Ground BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "ground") + 2 
    
    if ground_BC_type == "noSlip": 
        added_part = ""
        added_part += "\t type    zeroGradient;\n"
    
    if ground_BC_type == "wallFunction": 
        added_part = ""
        added_part += "\t type \t nutkAtmRoughWallFunction;\n"
        added_part += "\t z0 \t {:.4f};\n".format(roughness_length)
        added_part += "\t value \t uniform 0.0;\n"
    
    dict_lines.insert(start_index, added_part)
    
    
    ###################### Top BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "top") + 2 
    added_part = ""
    added_part += "\t type \t {};\n".format(top_BC_type)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Front BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "front") + 2 
    added_part = ""
    added_part += "\t type \t {};\n".format(sides_BC_type)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Back BC ################################  
    
    start_index = foam.find_keyword_line(dict_lines, "back") + 2 
    added_part = ""
    added_part += "\t type \t {};\n".format(sides_BC_type)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Building BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "building") + 2 
    
    if building_BC_type == "noSlip": 
        added_part = ""
        added_part += "\t type \t fixedValue;\n"
        added_part += "\t value \t uniform 0;\n"
    
    if building_BC_type == "wallFunction": 
        added_part = ""
        added_part += "\t type \t nutUSpaldingWallFunction;\n"
        added_part += "\t value \t uniform 0;\n"
    
    
    dict_lines.insert(start_index, added_part)
    
    #Write edited dict to file
    write_file_name = case_path + "0/nut"
    
    if os.path.exists(write_file_name):
        os.remove(write_file_name)
    
    output_file = open(write_file_name, "w+")
    for line in dict_lines:
        output_file.write(line)
    output_file.close()

def write_epsilon_file(input_json_path, template_dict_path, case_path):

    #Read JSON data
    json_file = open(input_json_path + "/IsolatedBuildingCFD.json")

    # Returns JSON object as a dictionary
    boundary_data = json.load(json_file)["boundaryConditions"]
    wind_data = json.load(json_file)["windCharacteristics"]
      
    json_file.close()
    
    sides_BC_type = boundary_data['sidesBoundaryCondition']
    top_BC_type = boundary_data['topBoundaryCondition']
    ground_BC_type = boundary_data['groundBoundaryCondition']
    building_BC_type = boundary_data['buildingBoundaryCondition']

    wind_speed = wind_data['roofHeightWindSpeed']
    building_height = wind_data['buildingHeight']
    roughness_length = wind_data['aerodynamicRoughnessLength']
    
    #Open the template file (OpenFOAM file) for manipulation
    dict_file = open(template_dict_path + "/epsilonFileTemplate", "r")

    dict_lines = dict_file.readlines()
    dict_file.close()
    
    
    #BC and initial condition
    epsilon0 = 0.01 

    ##################### Internal Field #########################
    
    start_index = foam.find_keyword_line(dict_lines, "internalField") 
    dict_lines[start_index] = "internalField   uniform {:.4f};\n".format(epsilon0)


    ###################### Inlet BC ##############################  
    #Write uniform
    start_index = foam.find_keyword_line(dict_lines, "inlet") + 2 
    added_part  = ""
    added_part += "\t type \t atmBoundaryLayerInletEpsilon;\n"
    added_part += "\t Uref \t {:.4f};\n".format(wind_speed)
    added_part += "\t Zref \t {:.4f};\n".format(building_height)
    added_part += "\t zDir \t (0.0 0.0 1.0);\n"
    added_part += "\t flowDir \t (1.0 0.0 0.0);\n"
    added_part += "\t z0 \t  uniform {:.4f};\n".format(roughness_length)
    added_part += "\t zGround \t uniform 0.0;\n"
    
    dict_lines.insert(start_index, added_part)

    ###################### Outlet BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "outlet") + 2 
    added_part = ""
    added_part += "\t type \t inletOutlet;\n"
    added_part += "\t inletValue \t uniform {:.4f};\n".format(epsilon0)
    added_part += "\t value \t uniform {:.4f};\n".format(epsilon0)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Ground BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "ground") + 2 
    
    if ground_BC_type == "noSlip": 
        added_part = ""
        added_part += "\t type \t zeroGradient;\n"
    
    if ground_BC_type == "wallFunction": 
        added_part = ""
        added_part += "\t type \t epsilonWallFunction;\n"
        added_part += "\t Cmu \t {:.4f};\n".format(0.09)
        added_part += "\t kappa \t {:.4f};\n".format(0.4)
        added_part += "\t E \t {:.4f};\n".format(9.8)
        added_part += "\t value \t uniform {:.4f};\n".format(epsilon0)
    
    dict_lines.insert(start_index, added_part)
    
    
    ###################### Top BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "top") + 2 
    added_part = ""
    added_part += "\t type  \t  {};\n".format(top_BC_type)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Front BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "front") + 2 
    added_part = ""
    added_part += "\t type  \t {};\n".format(sides_BC_type)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Back BC ################################  
    
    start_index = foam.find_keyword_line(dict_lines, "back") + 2 
    added_part = ""
    added_part += "\t type \t {};\n".format(sides_BC_type)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Building BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "building") + 2 
    
    if building_BC_type == "noSlip": 
        added_part = ""
        added_part += "\t type \t zeroGradient;\n"
    
    if building_BC_type == "wallFunction": 
        added_part = ""
        added_part += "\t type \t epsilonWallFunction;\n"
        added_part += "\t Cmu \t {:.4f};\n".format(0.09)
        added_part += "\t kappa \t {:.4f};\n".format(0.4)
        added_part += "\t E \t {:.4f};\n".format(9.8)
        added_part += "\t value \t uniform {:.4f};\n".format(epsilon0)
    
    dict_lines.insert(start_index, added_part)
    
    #Write edited dict to file
    write_file_name = case_path + "0/epsilon"
    
    if os.path.exists(write_file_name):
        os.remove(write_file_name)
    
    output_file = open(write_file_name, "w+")
    for line in dict_lines:
        output_file.write(line)
    output_file.close()

def write_k_file(input_json_path, template_dict_path, case_path):

    #Read JSON data
    json_file = open(input_json_path + "/IsolatedBuildingCFD.json")

    # Returns JSON object as a dictionary
    boundary_data = json.load(json_file)["boundaryConditions"]
    wind_data = json.load(json_file)["windCharacteristics"]
      
    json_file.close()
    
    sides_BC_type = boundary_data['sidesBoundaryCondition']
    top_BC_type = boundary_data['topBoundaryCondition']
    ground_BC_type = boundary_data['groundBoundaryCondition']
    building_BC_type = boundary_data['buildingBoundaryCondition']

    wind_speed = wind_data['roofHeightWindSpeed']
    building_height = wind_data['buildingHeight']
    roughness_length = wind_data['aerodynamicRoughnessLength']
    
    #Open the template file (OpenFOAM file) for manipulation
    dict_file = open(template_dict_path + "/kFileTemplate", "r")

    dict_lines = dict_file.readlines()
    dict_file.close()
    
    
    #BC and initial condition (you may need to scale to model scale)
    k0 = 1.3 #not in model scale

    ##################### Internal Field #########################
    
    start_index = foam.find_keyword_line(dict_lines, "internalField") 
    dict_lines[start_index] = "internalField \t uniform {:.4f};\n".format(k0)


    ###################### Inlet BC ##############################  
    #Write uniform
    start_index = foam.find_keyword_line(dict_lines, "inlet") + 2 
    added_part  = ""
    added_part += "\t type \t atmBoundaryLayerInletK;\n"
    added_part += "\t Uref \t {:.4f};\n".format(wind_speed)
    added_part += "\t Zref \t {:.4f};\n".format(building_height)
    added_part += "\t zDir \t (0.0 0.0 1.0);\n"
    added_part += "\t flowDir \t (1.0 0.0 0.0);\n"
    added_part += "\t z0 \t uniform {:.4f};\n".format(roughness_length)
    added_part += "\t zGround \t uniform 0.0;\n"
    
    dict_lines.insert(start_index, added_part)

    ###################### Outlet BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "outlet") + 2 
    added_part = ""
    added_part += "\t type \t inletOutlet;\n"
    added_part += "\t inletValue \t uniform {:.4f};\n".format(k0)
    added_part += "\t value \t uniform {:.4f};\n".format(k0)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Ground BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "ground") + 2 
    
    if ground_BC_type == "noSlip": 
        added_part = ""
        added_part += "\t type \t zeroGradient;\n"
    
    if ground_BC_type == "wallFunction": 
        added_part = ""
        added_part += "\t type \t kqRWallFunction;\n"
        added_part += "\t value \t uniform {:.4f};\n".format(0.0)
    
    dict_lines.insert(start_index, added_part)
    
    
    ###################### Top BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "top") + 2 
    added_part = ""
    added_part += "\t type  \t {};\n".format(top_BC_type)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Front BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "front") + 2 
    added_part  = ""
    added_part += "\t type \t {};\n".format(sides_BC_type)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Back BC ################################  
    
    start_index = foam.find_keyword_line(dict_lines, "back") + 2 
    added_part  = ""
    added_part += "\t type \t {};\n".format(sides_BC_type)
    
    dict_lines.insert(start_index, added_part)
    
    ###################### Building BC ##############################  
    
    start_index = foam.find_keyword_line(dict_lines, "building") + 2 
    
    if building_BC_type == "noSlip": 
        added_part = ""
        added_part += "\t type \t zeroGradient;\n"
    
    if building_BC_type == "wallFunction": 
        added_part = ""
        added_part += "\t type \t atmBoundaryLayerInletK;\n"
        added_part += "\t Cmu  \t {:.4f};\n".format(0.09)
        added_part += "\t kappa \t {:.4f};\n".format(0.4)
        added_part += "\t E \t {:.4f};\n".format(9.8)
        added_part += "\t value \t uniform {:.4f};\n".format(k0)
    
    dict_lines.insert(start_index, added_part)
    
    #Write edited dict to file
    write_file_name = case_path + "0/k"
    
    if os.path.exists(write_file_name):
        os.remove(write_file_name)
    
    output_file = open(write_file_name, "w+")
    for line in dict_lines:
        output_file.write(line)
    output_file.close()
    
    
def write_controlDict_file(input_json_path, template_dict_path, case_path):

    #Read JSON data
    json_file = open(input_json_path + "/IsolatedBuildingCFD.json")

    # Returns JSON object as a dictionary
    ns_data = json.load(json_file)["numericalSetup"]
    rm_data = json.load(json_file)["resultMonitoring"]
      
    json_file.close()
    
    solver_type = ns_data['solverType']
    duration = ns_data['duration']
    time_step = ns_data['timeStep']
    max_courant_number = ns_data['maxCourantNumber']
    adjust_time_step = ns_data['adjustTimeStep']
    
    
    num_stories = rm_data['numStories']
    floor_height = rm_data['floorHeight']
    center_of_rotation = rm_data['centerOfRotation']
    story_load_write_interval = rm_data['storyLoadWriteInterval']
    monitor_base_load = rm_data['monitorBaseLoad']
    monitor_surface_pressure = rm_data['monitorSurfacePressure']
    pressure_sampling_points = rm_data['pressureSamplingPoints']
    pressure_write_interval = rm_data['pressureWriteInterval']
    
    # Need to change this for      
    max_delta_t = 10*time_step
    
    write_interval = 1000
    purge_write =  3
    
    #Open the template file (OpenFOAM file) for manipulation
    dict_file = open(template_dict_path + "/controlDictTemplate", "r")

    dict_lines = dict_file.readlines()
    dict_file.close()
    
    #Write application type 
    start_index = foam.find_keyword_line(dict_lines, "application") 
    dict_lines[start_index] = "application \t{};\n".format(solver_type)
    
    #Write end time 
    start_index = foam.find_keyword_line(dict_lines, "endTime") 
    dict_lines[start_index] = "endTime \t{:.4f};\n".format(duration)
    
    #Write time step time 
    start_index = foam.find_keyword_line(dict_lines, "deltaT") 
    dict_lines[start_index] = "deltaT \t{:.6f};\n".format(time_step)
 
    #Write adjustable time step or not  
    start_index = foam.find_keyword_line(dict_lines, "adjustTimeStep") 
    dict_lines[start_index] = "adjustTimeStep \t\t{};\n".format("yes" if adjust_time_step else "no")
 
    #Write writeInterval  
    start_index = foam.find_keyword_line(dict_lines, "writeInterval") 
    dict_lines[start_index] = "writeInterval \t{};\n".format(write_interval)
    
    #Write maxCo  
    start_index = foam.find_keyword_line(dict_lines, "maxCo") 
    dict_lines[start_index] = "maxCo \t{:.2f};\n".format(max_courant_number)
    
    #Write maximum time step  
    start_index = foam.find_keyword_line(dict_lines, "maxDeltaT") 
    dict_lines[start_index] = "maxDeltaT \t{:.6f};\n".format(max_delta_t)
       

    #Write purge write interval  
    start_index = foam.find_keyword_line(dict_lines, "purgeWrite") 
    dict_lines[start_index] = "purgeWrite \t{};\n".format(purge_write)

    ########################### Function Objects ##############################
     
    #Find function object location  
    start_index = foam.find_keyword_line(dict_lines, "functions") + 2
    
    #Write pressure sampling points 
    if monitor_surface_pressure:
        added_part = "    #includeFunc  pressureSamplingPoints \n"
        dict_lines.insert(start_index, added_part)
    
    
    #Write story loads functionObjects  
    added_part = "    #includeFunc  storyForces \n"
    dict_lines.insert(start_index + 1, added_part)

    #Write base loads functionObjects
    if monitor_base_load:
        added_part = "    #includeFunc  baseForces \n"
        dict_lines.insert(start_index + 1, added_part)
    
    #Write edited dict to file
    write_file_name = case_path + "system/controlDict"
    
    if os.path.exists(write_file_name):
        os.remove(write_file_name)
    
    output_file = open(write_file_name, "w+")
    for line in dict_lines:
        output_file.write(line)
    output_file.close()
    
def write_fvSolution_file(input_json_path, template_dict_path, case_path):

    #Read JSON data
    json_file = open(input_json_path + "/IsolatedBuildingCFD.json")

    # Returns JSON object as a dictionary
    ns_data = json.load(json_file)["numericalSetup"]
      
    json_file.close()
    
    num_non_orthogonal_correctors = ns_data['numNonOrthogonalCorrectors']
    num_correctors = ns_data['numCorrectors']
    num_outer_correctors = ns_data['numOuterCorrectors']
        
    #Open the template file (OpenFOAM file) for manipulation
    dict_file = open(template_dict_path + "/fvSolutionTemplate", "r")

    dict_lines = dict_file.readlines()
    dict_file.close()

    
    #Write simpleFoam options  
    start_index = foam.find_keyword_line(dict_lines, "SIMPLE") + 2   
    added_part = ""
    added_part += "    nNonOrthogonalCorrectors \t{};\n".format(num_non_orthogonal_correctors)
    dict_lines.insert(start_index, added_part)

    
    #Write pimpleFoam options  
    start_index = foam.find_keyword_line(dict_lines, "PIMPLE") + 2   
    added_part = ""
    added_part += "    nOuterCorrectors \t{};\n".format(num_outer_correctors)
    added_part += "    nCorrectors \t{};\n".format(num_correctors)
    added_part += "    nNonOrthogonalCorrectors \t{};\n".format(num_non_orthogonal_correctors)
    dict_lines.insert(start_index, added_part)


    #Write pisoFoam options  
    start_index = foam.find_keyword_line(dict_lines, "PISO") + 2   
    added_part = ""
    added_part += "    nCorrectors \t{};\n".format(num_correctors)
    added_part += "    nNonOrthogonalCorrectors \t{};\n".format(num_non_orthogonal_correctors)
    dict_lines.insert(start_index, added_part)
   
   
    #Write edited dict to file
    write_file_name = case_path + "system/fvSolution"
    
    if os.path.exists(write_file_name):
        os.remove(write_file_name)
    
    output_file = open(write_file_name, "w+")
    for line in dict_lines:
        output_file.write(line)
    output_file.close()    


def write_pressure_probes_file(input_json_path, template_dict_path, case_path):

    #Read JSON data
    json_file = open(input_json_path + "/IsolatedBuildingCFD.json")

    # Returns JSON object as a dictionary
    rm_data = json.load(json_file)["resultMonitoring"]
      
    json_file.close()

    pressure_sampling_points = rm_data['pressureSamplingPoints']
    pressure_write_interval = rm_data['pressureWriteInterval']

    
    #Open the template file (OpenFOAM file) for manipulation
    dict_file = open(template_dict_path + "/probeTemplate", "r")

    dict_lines = dict_file.readlines()
    dict_file.close()
    

    #Write writeInterval 
    start_index = foam.find_keyword_line(dict_lines, "writeInterval") 
    dict_lines[start_index] = "writeInterval \t{};\n".format(pressure_write_interval)
    
    
    #Write fields to be montored 
    start_index = foam.find_keyword_line(dict_lines, "fields") 
    dict_lines[start_index] = "fields \t\t(p);\n"
    
    start_index = foam.find_keyword_line(dict_lines, "probeLocations") + 2

    added_part = ""
    
    for i in range(len(pressure_sampling_points)):
        added_part += " ({:.6f} {:.6f} {:.6f})\n".format(pressure_sampling_points[i][0], pressure_sampling_points[i][1], pressure_sampling_points[i][2])
    
    dict_lines.insert(start_index, added_part)

    #Write edited dict to file
    write_file_name = case_path + "system/pressureSamplingPoints"
    
    if os.path.exists(write_file_name):
        os.remove(write_file_name)
    
    output_file = open(write_file_name, "w+")
    for line in dict_lines:
        output_file.write(line)
    output_file.close()
    
    
  
def write_base_forces_file(input_json_path, template_dict_path, case_path):

    #Read JSON data
    json_file = open(input_json_path + "/IsolatedBuildingCFD.json")

    air_density = 1.0

    # Returns JSON object as a dictionary
    rm_data = json.load(json_file)["resultMonitoring"]
      
    json_file.close()
    

    num_stories = rm_data['numStories']
    floor_height = rm_data['floorHeight']
    center_of_rotation = rm_data['centerOfRotation']
    story_load_write_interval = rm_data['storyLoadWriteInterval']
    monitor_base_load = rm_data['monitorBaseLoad']

    
    #Open the template file (OpenFOAM file) for manipulation
    dict_file = open(template_dict_path + "/baseForcesTemplate", "r")

    dict_lines = dict_file.readlines()
    dict_file.close()
    

    #Write writeInterval 
    start_index = foam.find_keyword_line(dict_lines, "writeInterval") 
    dict_lines[start_index] = "writeInterval \t{};\n".format(story_load_write_interval)    
    
    #Write patch name to intergrate forces on 
    start_index = foam.find_keyword_line(dict_lines, "patches") 
    dict_lines[start_index] = "patches \t({});\n".format("building")
    
    #Write air density to rhoInf 
    start_index = foam.find_keyword_line(dict_lines, "rhoInf") 
    dict_lines[start_index] = "rhoInf \t\t{:.4f};\n".format(air_density)
    
    #Write center of rotation
    start_index = foam.find_keyword_line(dict_lines, "CofR") 
    dict_lines[start_index] = "CofR \t\t({:.4f} {:.4f} {:.4f});\n".format(center_of_rotation[0], center_of_rotation[1], center_of_rotation[2])
    

    #Write edited dict to file
    write_file_name = case_path + "system/baseForces"
    
    if os.path.exists(write_file_name):
        os.remove(write_file_name)
    
    output_file = open(write_file_name, "w+")
    for line in dict_lines:
        output_file.write(line)
    output_file.close()
    
def write_story_forces_file(input_json_path, template_dict_path, case_path):

    #Read JSON data
    json_file = open(input_json_path + "/IsolatedBuildingCFD.json")

    air_density = 1.0

    # Returns JSON object as a dictionary
    rm_data = json.load(json_file)["resultMonitoring"]
      
    json_file.close()
    

    num_stories = rm_data['numStories']
    floor_height = rm_data['floorHeight']
    center_of_rotation = rm_data['centerOfRotation']
    story_load_write_interval = rm_data['storyLoadWriteInterval']
    monitor_base_load = rm_data['monitorBaseLoad']

    
    #Open the template file (OpenFOAM file) for manipulation
    dict_file = open(template_dict_path + "/storyForcesTemplate", "r")

    dict_lines = dict_file.readlines()
    dict_file.close()
    

    #Write writeInterval 
    start_index = foam.find_keyword_line(dict_lines, "writeInterval") 
    dict_lines[start_index] = "writeInterval \t{};\n".format(story_load_write_interval)    
    
    #Write patch name to intergrate forces on 
    start_index = foam.find_keyword_line(dict_lines, "patches") 
    dict_lines[start_index] = "patches \t({});\n".format("building")
    
    #Write air density to rhoInf 
    start_index = foam.find_keyword_line(dict_lines, "rhoInf") 
    dict_lines[start_index] = "rhoInf \t\t{:.4f};\n".format(air_density)
    
    #Write center of rotation
    start_index = foam.find_keyword_line(dict_lines, "CofR") 
    dict_lines[start_index] = "CofR \t\t({:.4f} {:.4f} {:.4f});\n".format(center_of_rotation[0], center_of_rotation[1], center_of_rotation[2])
    
    #Number of stories  as nBins
    start_index = foam.find_keyword_line(dict_lines, "nBin") 
    dict_lines[start_index] = "    nBin \t{};\n".format(num_stories)
    
    #Write story direction
    start_index = foam.find_keyword_line(dict_lines, "direction") 
    dict_lines[start_index] = "    direction \t({:.4f} {:.4f} {:.4f});\n".format(0, 0, 1.0)

    #Write edited dict to file
    write_file_name = case_path + "system/storyForces"
    
    if os.path.exists(write_file_name):
        os.remove(write_file_name)
    
    output_file = open(write_file_name, "w+")
    for line in dict_lines:
        output_file.write(line)
    output_file.close()
    
    
def write_momentumTransport_file(input_json_path, template_dict_path, case_path):

    #Read JSON data for turbulence model
    json_file = open(input_json_path + "/IsolatedBuildingCFD.json")

    # Returns JSON object as a dictionary
    turb_data = json.load(json_file)["turbulenceModeling"]
      
    json_file.close()
    
    simulation_type = turb_data['simulationType']
    RANS_type = turb_data['RANSModelType']
    LES_type = turb_data['LESModelType']
    DES_type = turb_data['DESModelType']

    
    #Open the template file (OpenFOAM file) for manipulation
    dict_file = open(template_dict_path + "/momentumTransportTemplate", "r")

    dict_lines = dict_file.readlines()
    dict_file.close()
    

    #Write type of the simulation 
    start_index = foam.find_keyword_line(dict_lines, "simulationType") 
    dict_lines[start_index] = "simulationType \t{};\n".format("RAS" if simulation_type=="RANS" else simulation_type)
    
    if simulation_type=="RANS":
        #Write RANS model type 
        start_index = foam.find_keyword_line(dict_lines, "RAS") + 2
        added_part = "    model \t{};\n".format(RANS_type)
        dict_lines.insert(start_index, added_part)
        
    elif simulation_type=="LES":
        #Write LES SGS model type 
        start_index = foam.find_keyword_line(dict_lines, "LES") + 2
        added_part = "    model \t{};\n".format(LES_type)
        dict_lines.insert(start_index, added_part)
    
    elif simulation_type=="DES":
        #Write DES model type 
        start_index = foam.find_keyword_line(dict_lines, "LES") + 2
        added_part = "    model \t{};\n".format(DES_type)
        dict_lines.insert(start_index, added_part)

    #Write edited dict to file
    write_file_name = case_path + "constant/momentumTransport"
    
    if os.path.exists(write_file_name):
        os.remove(write_file_name)
    
    output_file = open(write_file_name, "w+")
    for line in dict_lines:
        output_file.write(line)
    output_file.close()
    
def write_physicalProperties_file(input_json_path, template_dict_path, case_path):

    #Read JSON data for turbulence model
    json_file = open(input_json_path + "/IsolatedBuildingCFD.json")

    # Returns JSON object as a dictionary
    wc_data = json.load(json_file)["windCharacteristics"]
      
    json_file.close()
    
    kinematic_viscosity = wc_data['kinematicViscosity']


    
    #Open the template file (OpenFOAM file) for manipulation
    dict_file = open(template_dict_path + "/physicalPropertiesTemplate", "r")

    dict_lines = dict_file.readlines()
    dict_file.close()
    

    #Write type of the simulation 
    start_index = foam.find_keyword_line(dict_lines, "nu") 
    dict_lines[start_index] = "nu\t\t[0 2 -1 0 0 0 0] {:.3e};\n".format(kinematic_viscosity)


    #Write edited dict to file
    write_file_name = case_path + "constant/physicalProperties"
    
    if os.path.exists(write_file_name):
        os.remove(write_file_name)
    
    output_file = open(write_file_name, "w+")
    for line in dict_lines:
        output_file.write(line)
    output_file.close()
    

def write_fvSchemes_file(input_json_path, template_dict_path, case_path):

    #Read JSON data for turbulence model
    json_file = open(input_json_path + "/IsolatedBuildingCFD.json")

    # Returns JSON object as a dictionary
    turb_data = json.load(json_file)["turbulenceModeling"]
      
    json_file.close()
    
    simulation_type = turb_data['simulationType']

    
    #Open the template file (OpenFOAM file) for manipulation
    dict_file = open(template_dict_path + "/fvSchemesTemplate{}".format(simulation_type), "r")

    dict_lines = dict_file.readlines()
    dict_file.close()
    

    #Write edited dict to file
    write_file_name = case_path + "system/fvSchemes"
    
    if os.path.exists(write_file_name):
        os.remove(write_file_name)
    
    output_file = open(write_file_name, "w+")
    for line in dict_lines:
        output_file.write(line)
    output_file.close()    
    
def write_decomposeParDict_file(input_json_path, template_dict_path, case_path):

    #Read JSON data for turbulence model
    json_file = open(input_json_path + "/IsolatedBuildingCFD.json")

    # Returns JSON object as a dictionary
    ns_data = json.load(json_file)["numericalSetup"]
      
    json_file.close()
    
    num_processors = ns_data['numProcessors']

    
    #Open the template file (OpenFOAM file) for manipulation
    dict_file = open(template_dict_path + "/decomposeParDictTemplate", "r")

    dict_lines = dict_file.readlines()
    dict_file.close()
    
    #Write number of sub-domains
    start_index = foam.find_keyword_line(dict_lines, "numberOfSubdomains") 
    dict_lines[start_index] = "numberOfSubdomains\t{};\n".format(num_processors)
    
    #Write method of decomposition
    start_index = foam.find_keyword_line(dict_lines, "decomposer") 
    dict_lines[start_index] = "decomposer\t\t{};\n".format("scotch")

    

    #Write edited dict to file
    write_file_name = case_path + "system/decomposeParDict"
    
    if os.path.exists(write_file_name):
        os.remove(write_file_name)
    
    output_file = open(write_file_name, "w+")
    for line in dict_lines:
        output_file.write(line)
    output_file.close()    
    
def write_DFSRTurbDict_file(input_json_path, template_dict_path, case_path):

    #Read JSON data for turbulence model
    json_file = open(input_json_path + "/IsolatedBuildingCFD.json")
    
    fmax = 200.0
    

    # Returns JSON object as a dictionary
    wc_data = json.load(json_file)["windCharacteristics"]
    ns_data = json.load(json_file)["numericalSetup"]
      
    json_file.close()

    wind_speed = wc_data['roofHeightWindSpeed']
    duration = ns_data['duration']
    
    #Generate a little longer duration to be safe
    duration = duration*1.010

    #Open the template file (OpenFOAM file) for manipulation
    dict_file = open(template_dict_path + "/DFSRTurbDictTemplate", "r")

    dict_lines = dict_file.readlines()
    dict_file.close()
    
    #Write the end time
    start_index = foam.find_keyword_line(dict_lines, "endTime") 
    dict_lines[start_index] = "endTime\t\t\t{:.4f};\n".format(duration)
    
    #Write patch name
    start_index = foam.find_keyword_line(dict_lines, "patchName") 
    dict_lines[start_index] = "patchName\t\t\"{}\";\n".format("inlet")

    #Write cohUav 
    start_index = foam.find_keyword_line(dict_lines, "cohUav") 
    dict_lines[start_index] = "cohUav\t\t\t{:.4f};\n".format(wind_speed)
    
    #Write fmax 
    start_index = foam.find_keyword_line(dict_lines, "fMax") 
    dict_lines[start_index] = "fMax\t\t\t{:.4f};\n".format(fmax)  
    
    #Write time step 
    start_index = foam.find_keyword_line(dict_lines, "timeStep") 
    dict_lines[start_index] = "timeStep\t\t{:.4f};\n".format(1.0/fmax)

    #Write edited dict to file
    write_file_name = case_path + "constant/DFSRTurbDict"
    
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
    case_path = sys.argv[3]
    
    # data_path = os.getcwd()
    # script_path = os.path.dirname(os.path.realpath(__file__))
    
    
    #Create case director
    # set up goes here 

    
    
    #Read JSON data for turbulence model
    json_file = open(input_json_path + "/IsolatedBuildingCFD.json")

    # Returns JSON object as a dictionary
    turb_data = json.load(json_file)["turbulenceModeling"]
      
    json_file.close()
    
    simulation_type = turb_data['simulationType']
    RANS_type = turb_data['RANSModelType']
    LES_type = turb_data['LESModelType']
    

    write_U_file(input_json_path, template_dict_path, case_path)
    write_p_file(input_json_path, template_dict_path, case_path)
    write_nut_file(input_json_path, template_dict_path, case_path)
    write_k_file(input_json_path, template_dict_path, case_path)
    
    if simulation_type == "RANS" and RANS_type=="kEpsilon":
        write_epsilon_file(input_json_path, template_dict_path, case_path)

    #Write controle dict
    write_controlDict_file(input_json_path, template_dict_path, case_path)
    
    #Write results to be monitored
    write_base_forces_file(input_json_path, template_dict_path, case_path)
    write_story_forces_file(input_json_path, template_dict_path, case_path)
    write_pressure_probes_file(input_json_path, template_dict_path, case_path)
    
    #Write fvSolution dict
    write_fvSolution_file(input_json_path, template_dict_path, case_path)

    #Write fvSchemes dict
    write_fvSchemes_file(input_json_path, template_dict_path, case_path)

    #Write momentumTransport dict
    write_momentumTransport_file(input_json_path, template_dict_path, case_path)
    
    #Write physicalProperties dict
    write_physicalProperties_file(input_json_path, template_dict_path, case_path)
    
    #Write decomposeParDict
    write_decomposeParDict_file(input_json_path, template_dict_path, case_path)
    
    #Write DFSRTurb dict
    write_DFSRTurbDict_file(input_json_path, template_dict_path, case_path)
    