import sys
import os
import subprocess
import numpy as np
from stl import mesh
import json
import stat
import shutil
import numpy as np
import scipy.io as sio
from pprint import pprint

def create_stl_file(input_path, output_path):
    
    #Read JSON data
    json_file = open(input_path + "/buildingDescription.json")
      
    # returns JSON object as 
    # a dictionary
    bldg_data = json.load(json_file)
      
    # # Iterating through the json
    # # list
    # for i in data['emp_details']:
    #     print(i)
      
    # Closing file
    # f.close()
    
    scale =  bldg_data['geometricScale']
    
    #Convert from full-scale to model-scale
    B = bldg_data['buildingWidth']/scale
    D = bldg_data['buildingDepth']/scale
    H = bldg_data['buildingHeight']/scale
    
    wind_dxn = bldg_data['windDirection']
    
    # print (scale)
    # print (H)
    # print (B)
    # print (D)
    # print (wind_dxn) 
    
    # B = 45.72 
    # D = 30.48 
    # H = 182.88 
    # # wind_dxn = 45.0
    # wind_dxn = 0.0
    
    wind_dxn_rad = np.deg2rad(wind_dxn)
    epsilon = 1.0e-5 
    
        
    # Define the 8 vertices of the building
    vertices = np.array([[-D/2.0, -B/2.0, -epsilon],
                         [+D/2.0, -B/2.0, -epsilon],
                         [+D/2.0, +B/2.0, -epsilon],
                         [-D/2.0, +B/2.0, -epsilon],
                         [-D/2.0, -B/2.0, +H],
                         [+D/2.0, -B/2.0, +H],
                         [+D/2.0, +B/2.0, +H],
                         [-D/2.0, +B/2.0, +H]])
        
    n_vertices = np.shape(vertices)[0]
    
    #Transform transform the vertices to account the wind direction. 
    trans_vertices = np.zeros((n_vertices, 3))
    trans_vertices[:,2] = vertices[:,2]
    
    t_matrix  = np.array([[np.cos(wind_dxn_rad), -np.sin(wind_dxn_rad)], 
                          [np.sin(wind_dxn_rad),  np.cos(wind_dxn_rad)]])
    
    for i in range(n_vertices):
        trans_vertices[i,0:2]  = np.matmul(t_matrix, vertices[i,0:2])
        
        
    # Define the 12 triangles composing the rectangular building
    faces = np.array([\
        [0,3,1],
        [1,3,2],
        [0,4,7],
        [0,7,3],
        [4,5,6],
        [4,6,7],
        [5,1,2],
        [5,2,6],
        [2,3,6],
        [3,7,6],
        [0,1,5],
        [0,5,4]])
    
    # Create the mesh
    bldg = mesh.Mesh(np.zeros(faces.shape[0], dtype=mesh.Mesh.dtype))
    for i, f in enumerate(faces):
        for j in range(3):
            bldg.vectors[i][j] = trans_vertices[f[j],:]
    
    # Write the mesh to file "building.stl"
    bldg.save(output_path + '/building.stl')


if __name__ == '__main__':    
    
    input_args = sys.argv

    # set filenames
    input_path = sys.argv[1]
    output_path = sys.argv[2]
    
    # data_path = os.getcwd()
    # script_path = os.path.dirname(os.path.realpath(__file__))
    
    create_stl_file(input_path, output_path)