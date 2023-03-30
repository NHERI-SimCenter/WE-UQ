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


def create_block_mesh_dict(input_json_path, template_dict_path, output_dict_path):

    #Read JSON data
    json_file = open(input_json_path + "/backgroundMeshParameters.json")
      
    # Returns JSON object as a dictionary
    domain_data = json.load(json_file)
      
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
    
    relative_dimensions = domain_data['relativeDimensions']
    origin = np.array(domain_data['origin'])
    length_unit = domain_data['lengthUnit']

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
    dict_file = open(template_dict_path + "/blockMeshDictTemplate", "r")

    #Export to OpenFOAM probe format
    start_line = 27
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

        # dict_lines.insert(start_line, line)
        # start_line+=1


# scale = 400.0
# offset = 1.0e-4
# H = cwe.CAARC.H/scale + offset
# B = cwe.CAARC.B/scale + offset
# D = cwe.CAARC.D/scale + offset

# directions = np.array([0, 10, 20, 30, 40, 45, 50, 60, 70, 80, 90])
# n_directions = len(directions)
# directions_rad = np.deg2rad(directions)

# ##############################################################################
# # Create probes around the building at 2/3H of the building 
# ##############################################################################
# n_B = 45
# n_D = int(np.rint(n_B*(D/B)))

# probes  = np.zeros((2*(n_B + n_D), 3))

# corner_dist = B/n_B

# x_dim  = D - D/n_D
# y_dim  = B - B/n_B

# side_x = np.linspace(-x_dim/2.0, x_dim/2.0, n_D)
# front_y = np.linspace(-y_dim/2.0, y_dim/2.0, n_B)

# #z-coordinate, fixed to 2/3H
# probes[:,2] = (2.0/3.0)*H

# #North face of the bldg
# probes[:n_B,0] = -D/2.0
# probes[:n_B,1] = front_y

# #East face of the bldg
# probes[n_B:(n_D + n_B),0] = side_x
# probes[n_B:(n_D + n_B),1] = B/2.0

# len(probes[n_B:(n_D + n_B),0])

# #South face of the bldg
# probes[(n_D + n_B):(n_D + 2*n_B),0] = D/2.0
# probes[(n_D + n_B):(n_D + 2*n_B),1] = np.flip(front_y)

# #West face of the bldg
# probes[(n_D + 2*n_B):2*(n_D + n_B),0] = np.flip(side_x)
# probes[(n_D + 2*n_B):2*(n_D + n_B),1] = -B/2.0


# probes_final = np.zeros((np.shape(probes)[0], 3))

# for i in range(n_directions):
    
#     t_matrix  = np.array([[np.cos(directions_rad[i]), -np.sin(directions_rad[i])], 
#                           [np.sin(directions_rad[i]),  np.cos(directions_rad[i])]])
#     for j in range(np.shape(probes)[0]):
#         probes_final[j,0:2]  = np.matmul(t_matrix, probes[j,0:2])
#         probes_final[j, 2]  = probes[j,2]

#     file_name = 'probes/ringProbesA%02d'% (directions[i])
#     # cwe.write_open_foam_vector_field(probes_final, file_name)
#     np.savetxt(file_name + ".csv", probes_final, delimiter=",",  header="X, Y, Z", comments='')
    
#     #Export to OpenFOAM probe format
#     start_line = 27
#     OF_file = open("pProbesTemplate", "r")
#     OF_lines = OF_file.readlines()
#     OF_file.close()
    
#     for pi in range(np.shape(probes_final)[0]):
#         line = '(%f %f %f)\n' % (probes_final[pi,0], probes_final[pi,1], probes_final[pi,2])
#         OF_lines.insert(start_line, line)
#         start_line+=1
    
    
#     if os.path.exists(file_name):
#         os.remove(file_name)
    
#     f = open(file_name, "w+")
#     for linei in OF_lines:
#         f.write(linei)
#     f.close()

# ##############################################################################
# # Create probes corresponding to the experimental model 
# ##############################################################################

# probes_df  = pd.read_table('exp_tap_coordinates.txt')   

# probes  = probes_df.iloc[:,2:6].to_numpy() 

# probes_final = np.zeros((np.shape(probes)[0], 3))

# for i in range(n_directions):
    
#     t_matrix  = np.array([[np.cos(directions_rad[i]), -np.sin(directions_rad[i])], 
#                           [np.sin(directions_rad[i]),  np.cos(directions_rad[i])]])
#     for j in range(np.shape(probes)[0]):
#         probes_final[j,0:2]  = (1.0 + 1.0e-3)*np.matmul(t_matrix, probes[j,0:2])
#         probes_final[j, 2]  =  (1.0 + 1.0e-4)*probes[j,2]
        
#     file_name = 'probes/expProbesA%02d'% (directions[i])
#     # cwe.write_open_foam_vector_field(probes_final, file_name)
#     np.savetxt(file_name + ".csv", probes_final, delimiter=",",  header="X, Y, Z", comments='')

#     #Export to OpenFOAM probe format
#     start_line = 27
#     OF_file = open("pProbesTemplate", "r")
#     OF_lines = OF_file.readlines()
#     OF_file.close()
    
#     for pi in range(np.shape(probes_final)[0]):
#         line = '(%f %f %f)\n' % (probes_final[pi,0], probes_final[pi,1], probes_final[pi,2])
#         OF_lines.insert(start_line, line)
#         start_line+=1
    
    
#     if os.path.exists(file_name):
#         os.remove(file_name)
    
#     f = open(file_name, "w+")
#     for linei in OF_lines:
#         f.write(linei)
#     f.close()

# ##############################################################################
# # Create probes for high resolution pressure measurements 
# ##############################################################################
# n_B = 30
# n_D = int(np.rint(n_B*(D/B)))
# n_H = int(np.rint(n_B*(H/B)))

# probes  = np.zeros((2*(n_B + n_D)*n_H + n_D*n_B, 3))

# corner_dist = B/n_B

# x_dim  = D - D/n_D
# y_dim  = B - B/n_B

# x = np.linspace(-x_dim/2.0, x_dim/2.0, n_D)
# y = np.linspace(-y_dim/2.0, y_dim/2.0, n_B)
# z = np.linspace(0.5*H/n_H, H - 0.5*H/n_H, n_H)

# faces = ['North', 'East', 'South', 'West', 'Top']

# n_faces  = len(faces)

# indx = 0

# #North face of the bldg 
# yy, zz = np.meshgrid(y, z)
# probes[:n_B*n_H,0] = -D/2.0
# for i in range(n_B):
#     for j in range(n_H):
#         probes[indx, 1] = yy[j,i]
#         probes[indx, 2] = zz[j,i]
#         indx += 1
        
# #East face of the bldg
# xx, zz = np.meshgrid(x, z)
# probes[indx:indx + n_D*n_H,1] = B/2.0
# for i in range(n_D):
#     for j in range(n_H):
#         probes[indx, 0] = xx[j,i]
#         probes[indx, 2] = zz[j,i]
#         indx += 1

# #South face of the bldg
# yy, zz = np.meshgrid(np.flip(y), z)
# probes[indx:indx + n_B*n_H, 0] = D/2.0
# for i in range(n_B):
#     for j in range(n_H):
#         probes[indx, 1] = yy[j,i]
#         probes[indx, 2] = zz[j,i]
#         indx += 1
        
# #West face of the bldg
# xx, zz = np.meshgrid(np.flip(x), z)
# probes[indx:indx + n_D*n_H, 1] = -B/2.0
# for i in range(n_D):
#     for j in range(n_H):
#         probes[indx, 0] = xx[j,i]
#         probes[indx, 2] = zz[j,i]
#         indx += 1

# #Top face of the bldg
# xx, yy = np.meshgrid(x, y)
# probes[indx:indx + n_D*n_B, 2] = H
# for i in range(n_D):
#     for j in range(n_B):
#         probes[indx, 0] = xx[j,i]
#         probes[indx, 1] = yy[j,i]
#         indx += 1

# probes_final = np.zeros((np.shape(probes)[0], 3))

# for i in range(n_directions):
    
#     t_matrix  = np.array([[np.cos(directions_rad[i]), -np.sin(directions_rad[i])], 
#                           [np.sin(directions_rad[i]),  np.cos(directions_rad[i])]])
#     for j in range(np.shape(probes)[0]):
#         probes_final[j,0:2]  = np.matmul(t_matrix, probes[j,0:2])
#         probes_final[j, 2]  = probes[j,2]

#     file_name = 'probes/surfaceProbesA%02d'% (directions[i])
#     # cwe.write_open_foam_vector_field(probes_final, file_name)
#     np.savetxt(file_name + ".csv", probes_final, delimiter=",",  header="X, Y, Z", comments='')
    
#     #Export to OpenFOAM probe format
#     start_line = 27
#     OF_file = open("pProbesTemplate", "r")
#     OF_lines = OF_file.readlines()
#     OF_file.close()
    
#     for pi in range(np.shape(probes_final)[0]):
#         line = '(%f %f %f)\n' % (probes_final[pi,0], probes_final[pi,1], probes_final[pi,2])
#         OF_lines.insert(start_line, line)
#         start_line+=1
    
    
#     if os.path.exists(file_name):
#         os.remove(file_name)
    
#     f = open(file_name, "w+")
#     for linei in OF_lines:
#         f.write(linei)
#     f.close()

if __name__ == '__main__':    
    
    input_args = sys.argv

    # Set filenames
    input_json_path = sys.argv[1]
    template_dict_path = sys.argv[2]
    output_dict_path = sys.argv[3]
    
    # data_path = os.getcwd()
    # script_path = os.path.dirname(os.path.realpath(__file__))
    
    create_block_mesh_dict(input_json_path, template_dict_path, output_dict_path)
