"""
This code reads wind profile from TPU database 

@author: Abiy
"""
import matplotlib.pyplot as plt   
import matplotlib.gridspec as gridspec   
import numpy as np
from scipy import signal
from scipy.interpolate import interp1d
from scipy.interpolate import UnivariateSpline
from scipy import stats
from matplotlib.ticker import MaxNLocator

def setup_plot(plt, font_size=20, legend_font_size=20, axis_font_size=20):
    fig = plt.figure(facecolor='white')
    font = {'family' : 'Times New Roman','weight' : 'normal', 'size'   : font_size}
    plt.rcParams['xtick.major.pad'] = 10
    plt.rcParams['ytick.major.pad'] = 10    
    plt.rcParams['xtick.direction'] = 'inout'
    plt.rcParams['ytick.direction'] = 'inout'
    plt.rcParams['xtick.major.size'] = 8
    plt.rcParams['ytick.major.size'] = 8

    plt.rc('font', **font)    
    plt.rc('axes', labelsize=font_size)    # fontsize of the x and y labels
    plt.rc('axes', titlesize=font_size)  # fontsize of the axes title
    plt.rc('xtick', labelsize=axis_font_size)    # fontsize of the tick labels
    plt.rc('ytick', labelsize=axis_font_size)    # fontsize of the tick labels
    plt.rc('axes', linewidth=1.25)    
    plt.rc('legend', fontsize=legend_font_size)
    plt.rc('text', usetex=True)
    
    return plt, fig


geom_scale = 400.0
time_scale = 100.0
vel_scale = geom_scale/time_scale

H = 200.0
B = 40.0
D = 40.0
         
prof = np.loadtxt('profile.csv', delimiter=',')
Uh = 11.3737*vel_scale

z_exp = geom_scale*prof[:, 0]
Uav_exp = Uh*prof[:, 1]
               
kappa = 0.41
z0 = 0.03

u_star = kappa*Uh/np.log(H/z0)
Uav_log = (u_star/kappa)*np.log(z_exp/z0)


##############################################################################
# Plot mean velocity and turbulence intensity profiles
##############################################################################
plt, fig = setup_plot(plt, 30, 34, 28)
gs = gridspec.GridSpec(ncols=1, nrows=1, figure=fig)
markersize = 15
linewidth = 3.5
ax = fig.add_subplot(gs[0, 0])    

#Plot the mean velocity profile     
ax.tick_params(which='major', direction='in', size=10, width=1.25)
ax.grid(linestyle='--')  
ax.set_xlim([0, 1.5])
ax.set_ylim([0, 2.0])   
ax.set_xlabel(r'$\overline{U}/U_{H}$') 
ax.set_ylabel(r'$z/H$')    
# ax.xaxis.set_major_locator(MaxNLocator(4))
# ax.yaxis.set_major_locator(MaxNLocator(4))
ax.set_xticks(np.arange(0, 1.51, 0.5))
ax.set_yticks(np.arange(0, 2.01, 0.5))

ax.set_title('Mean Velocity', fontsize=40)

ax.plot(Uav_exp/Uh, z_exp/H, 'ks', markersize=markersize, markerfacecolor='none', markeredgewidth=2.0)
ax.plot(Uav_log/Uh, z_exp/H, 'r-', linewidth=linewidth)
ax.legend(['Experiment', 'Log-law fit \n($z_0=0.03m$)'], frameon=True, framealpha=1.0, labelspacing = 0.75, loc='center left', bbox_to_anchor=(1, 0.85))  

plt.grid(linestyle='--')     
fig.set_size_inches(32/2.54, 30/2.54)
plt.tight_layout()
plt.savefig('plots/we13_mean_velocity_profile_fitting.pdf')
plt.savefig('plots/we13_mean_velocity_profile_fitting.png')
plt.savefig('plots/we13_mean_velocity_profile_fitting.svg')
plt.show()


