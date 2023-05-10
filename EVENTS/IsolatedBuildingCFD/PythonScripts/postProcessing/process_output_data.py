"""
This script create blockMeshDict for OpenFoam given vertices and boundary type. 


code creates pressure probes for the main simulation. Three types of 
probes are created. 

"""

import sys
import os
import subprocess
import json
import stat
import shutil
import numpy as np
import CWE as cwe
import matplotlib.pyplot as plt   
import matplotlib.gridspec as gridspec   
import numpy as np
import CWE as cwe
from scipy import signal
from scipy.interpolate import interp1d
from scipy.interpolate import UnivariateSpline
from scipy import stats


def write_wind_profiles(case_path):
    inf_path  = case_path + "/constant/boundaryData/windProfile/sampledData/verticalProfile/"
    
    
    inf = cwe.VelocityData('cfd', inf_path, start_time=None, end_time=None)
    
    
    #Read JSON data for turbulence model
    wc_json_file = open(case_path + "/constant/simCenter/windCharacteristics.json")
    
    # Returns JSON object as a dictionary
    wind_data = json.load(wc_json_file)
    wc_json_file.close()
      
    building_height = wind_data['buildingHeight']
    
    #Wind profile z, Uav, Iu, Lu
    prof = np.zeros((len(inf.z), 4))
    prof[:,0] = inf.z
    prof[:,1] = inf.Uav
    prof[:,2] = inf.I[:,0]
    prof[:,3] = inf.L[:,0]
    
    
    #Wind velocity at roof height 
    H_loc = np.argmin(np.abs(inf.z - building_height))
    
    # U, v, w in at roof height 
    Uh = inf.U[H_loc, :, :].T
    
    Suh = []
    
    
    for i in range(3):
        f, s = cwe.psd(Uh[:, i], 0.0025, 8)
        Suh.append(np.abs(s))
        
    Suh.insert(0, f)
    
    Suhout = np.asarray(Suh, dtype=np.float32).T  
    
    write_precision = 6
    fmt ='%.{}e'.format(write_precision)
    
    prof_path = case_path + "/constant/simCenter/output/windProfiles.txt"    
    Suh_path = case_path + "/constant/simCenter/output/Suh.txt"    
    
    np.savetxt(prof_path, prof, fmt=fmt)
    np.savetxt(Suh_path, Suhout, fmt=fmt)


def write_wind_loads(case_path):    
    #Write base forces 
    base_forces_path  = case_path + "/postProcessing/baseForces/0/forces.dat"
    base_o, base_t, base_f, base_m = cwe.read_forces_OF10(base_forces_path)
    
    base_forces = np.zeros((len(base_t), 3))
    
    base_forces[:,0:2] = base_f[:, 0:2]
    base_forces[:,2] = base_m[:, 2]
    
      
    #Write story forces 
    story_forces_path  = case_path + "/postProcessing/storyForces/0/forces_bins.dat"
    story_coord, story_t, story_f, story_m = cwe.read_bin_forces_OF10(story_forces_path)

    
    write_precision = 6
    fmt ='%.{}e'.format(write_precision)
    
    out_base_path = case_path + "/constant/simCenter/output/baseForces.txt"    
    
    out_story_path_Fx = case_path + "/constant/simCenter/output/storyForcesFx.txt"    
    out_story_path_Fy = case_path + "/constant/simCenter/output/storyForcesFy.txt"    
    out_story_path_Mz = case_path + "/constant/simCenter/output/storyForcesMz.txt"    
    
    np.savetxt(out_base_path, base_forces, fmt=fmt)
    
    np.savetxt(out_story_path_Fx, story_f[:,:,0], fmt=fmt)
    np.savetxt(out_story_path_Fy, story_f[:,:,1], fmt=fmt)
    np.savetxt(out_story_path_Mz, story_m[:,:,2], fmt=fmt)




if __name__ == '__main__':    
    
    input_args = sys.argv

    # Set filenames
    case_path = sys.argv[1]
    
    write_wind_profiles(case_path)
    write_wind_loads(case_path)
    
