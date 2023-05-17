import sys
from stl import mesh
import json
import numpy as np

def create_stl_file(input_path, output_path):
    
    #Read JSON data
    json_file = open(input_path + "/IsolatedBuildingCFD.json")
      
    # returns JSON object as 
    # a dictionary
    bldg_data = json.load(json_file)["buildingParameters"]
      
    json_file.close()
    
    scale =  bldg_data['geometricScale']
    length_unit =  bldg_data['lengthUnit']

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
    
    #Convert from full-scale to model-scale
    B = convert_to_meters*bldg_data['buildingWidth']/scale
    D = convert_to_meters*bldg_data['buildingDepth']/scale
    H = convert_to_meters*bldg_data['buildingHeight']/scale
    
    normalization_type = bldg_data['normalizationType']

    origin = np.array(bldg_data['origin'])
    wind_dxn = bldg_data['windDirection']

    if normalization_type == "Relative":
        origin = origin*H
    


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

    #The default coordinate system is building center. 
    #Transform the preferred origin
    vertices = vertices - origin
    
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
    fmt = mesh.stl.Mode.ASCII # binary or ASCII format 
    bldg.save(output_path + '/building.stl', mode=fmt)


if __name__ == '__main__':    
    
    input_args = sys.argv

    # set filenames
    input_path = sys.argv[1]
    output_path = sys.argv[2]
    
    # data_path = os.getcwd()
    # script_path = os.path.dirname(os.path.realpath(__file__))
    
    create_stl_file(input_path, output_path)
