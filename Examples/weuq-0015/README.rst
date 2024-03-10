.. _weuq-0015:

Digital Wind Tunnel: Characterizing ABL Flows using Large-eddy Simulation
=======================================================================

+----------------+----------------------------------------+
| Problem files  | :github:`Download <Examples/weuq-0015/>` |
+----------------+----------------------------------------+

This example uses the Digital Twin capability implemented in WE-UQ Version 3.2.0. It demonstrates the procedure for calibrating Atmospheric Boundary Layer (ABL) flows using large eddy simulation in empty domain configuration. For creating the CFD model, the inputs are taken from wind tunnel experiment conducted at boundary layer wind tunnel of Tokyo Polytechnic University (TPU). The cross-section dimensions  of the CFD model are set to match the wind tunnel dimensions at the test section. :numref:`fig-we15-dmn` shows extent of the computational domain with dimensions. Fetch distance measures the distance between the inlet anticipated location of the structure. At this location, the characteristics of the approaching wind will be examined. Detailed instruction to setup the CFD model can be found in the documentation of :ref:`Empty Domain CFD Simulation <lblEmptyDomainCFD>` 

.. _fig-we15-dmn:

.. figure:: figures/we15_domain_geometry.svg
   :align: center
   :width: 60%

   Configuration of the computational domain for empty domain analysis



The simulation for this example is run at 1:400 geometric scale similar to the wind tunnel study. Details about the geometry the CFD model and the target flow characteristics are given in :numref:`tbl-we15-1`. 

.. _tbl-we15-1:
.. table:: Parameters needed to define the CFD model 
   :align: center
    
   +---------------------+----------------------------------------------+------------------+---------------+
   |Parameter            |Description                                   |Value             | Unit          |
   +=====================+==============================================+==================+===============+
   |:math:`L_x`          |Domain length                                 | 5.2             | m             |
   +---------------------+----------------------------------------------+------------------+---------------+
   |:math:`L_y`          |Domain width                                  | 2.2              | m             | 
   +---------------------+----------------------------------------------+------------------+---------------+
   |:math:`L_z`          |Domain height                                 | 1.8              | m             | 
   +---------------------+----------------------------------------------+------------------+---------------+
   |:math:`L_f`          |Wind fetch distance                           | 2.1              | m             | 
   +---------------------+----------------------------------------------+------------------+---------------+
   |:math:`\lambda_L`    |Geometric scale                               | 1:400            |               | 
   +---------------------+----------------------------------------------+------------------+---------------+
   |:math:`\lambda_V`    |Velocity scale                                | 1:4              |               | 
   +---------------------+----------------------------------------------+------------------+---------------+
   |:math:`\lambda_T`    |Time scale                                    | 1:100            |               | 
   +---------------------+----------------------------------------------+------------------+---------------+
   |:math:`U_{ref}`      |Reference wind speed                          | 10.69            | m/s           | 
   +---------------------+----------------------------------------------+------------------+---------------+
   |:math:`T`            |Duration of the simulation                    | 37               | s             | 
   +---------------------+----------------------------------------------+------------------+---------------+
   |:math:`z_0`          |Aerodynamic roughness length in full scale    | 0.5              | m             | 
   +---------------------+----------------------------------------------+------------------+---------------+
   |:math:`\rho_{air}`   |Air density                                   | 1.225            | kg/m^3        | 
   +---------------------+----------------------------------------------+------------------+---------------+
   |:math:`\nu_{air}`    |Kinematic viscosity of air                    | :math:`1.5e^{-5}`| m^2/s         | 
   +---------------------+----------------------------------------------+------------------+---------------+
   |:math:`f_{s}`        |Sampling frequency (rate)                     | 200              | Hz            | 
   +---------------------+----------------------------------------------+------------------+---------------+


Start
^^^^^^^
First, open the workflow by selecting **Tools -> CFD - Empty Domain Simulation** from the menu bar. Then, load the JSON setup file using **LOAD File** button. The JSON file can be found :github:`here <Examples/weuq-0015/src/input.json>`. Once the JSON file is loaded, in the *Start* tab, change the **Path** to your preferred directory by clicking **Browse** button as shown in :numref:`fig-we15-start`. Leave the remaining settings with their default values. 

   .. note::
      If the **Path** field does not automatically change when the JSON path is loaded, you can manually copy paste your working directory path in the field. 

.. _fig-we15-start:

.. figure:: figures/we15_start_tab.svg
   :align: center
   :width: 75%

   Setup the path to the working directory in the *Start* tab of the workflow.


Domain Dimensions
^^^^^^^^^^^^^^^^^^^
Since the CFD simulation is run at reduced scale, change the **Geometric Scale** (:math:`\lambda_L`) to 1:400 as given in :numref:`tbl-we15-1`. In the text, you need to put only 400. Set the lenght (:math:`L_x` ), width(:math:`L_y` ) and height (:math:`L_z` ) of the computational to 5.2 m, 2.2 m, 1.8 m, respectively, as shown in :numref:`fig-we15-geom`. For the **Fetch lenght (X-axis)** specify 2.1 m.

.. _fig-we15-geom:

.. figure:: figures/we15_geometry_tab.svg
   :align: center
   :width: 80%

   Set the dimensions of the computational domain in the *Geometry* tab.  


Mesh Generation
^^^^^^^^^^^^^^^^
The mesh is generated by first creating a background grid. Make a uniformly spaced background mesh by specifying **No. of Cells** in x, y and z direction to 52, 44 and 36. Then, generate the background mesh by clicking **Run Background Mesh** button. Next, in the *Regional Refinements* subtab define local refinements. For this example, we create three refinement boxes shown in :numref:`fig-we15-mesh`. The refinement regions are created by providing the min and max coordinates of the bounding boxes. Then, the final mesh is generated by clicking the **Run Final Mesh** button. This will automatically update the mesh visualization window on the right panel. 

.. _fig-we15-mesh:

.. figure:: figures/we15_mesh_tab.svg
   :align: center
   :width: 100%

   Define meshing controls and generate the grid in *Mesh* tab.  


Boundary Conditions
^^^^^^^^^^^^^^^^^^^^
Set the fields in *Wind characteristics* group based on the values provided in :numref:`tbl-we15-1`. Set the **Velocity Scale** (:math:`\lambda_V`) to 4, **Reference Wind Speed** (:math:`U_{ref}`) to 10.69 m/s, **Reference Height** (:math:`H_{ref}`) to 0.3 m and **Roughness Lenght** (:math:`z_0`) in full-scale to 0.5 m. For the boundary conditions, use *TInf* option at the inlet of the domain. For the ground surface, use *roughWallFunction* and setup the boundary conditions on the remaining surfaces as seen in :numref:`fig-we15-bc`.  

If *TInf* is used for the inlet, specify turbulence generation scheme in *Inflow Generation* group. For this example, use *DFM* which represents Digital Filtering Method. To specify the target wind profiles, click **Import(*.csv)** and locate the ``*.csv`` file you download from :github:`here <Examples/weuq-0015/src/target_wind_profiles.json>`.  

.. _fig-we15-bc:

.. figure:: figures/we15_bc_tab.svg
   :align: center
   :width: 75%

   Setup wind characteristics with the boundary conditions in *Boundary Conditions* tab.  


Numerical Setup
^^^^^^^^^^^^^^^^
Set the turbulence modeling, solver type, duration and time step options in *Numerical Setup* tab. For the **Simulation Type** use *LES* and select *dynamicKEqn* for the **Sub-grid Scale Model**. Set the **Solver Type** to *pimpleFoam* with 1 **Number of Non-Orthogonal Correctors** , 2 **Number of Corrector Loops** and 1 **Number of Outer Corrector Loops**.


For the **Duration** of the simulation, use :math:`37 s` based on what is specified in :numref:`tbl-we15-1`. This time step translates to :math:`3700s` in full scale with a time scale 1:100, which is a litter over the recommended 1 hour duration. The **Time Steep** can be calculated automatically from the smallest mesh size by clicking **Calculate** button. For this example, the calculated time step is :math:`0.000736624 s`, which is approximated to :math:`0.005 s` for convenience. Finally, check the **Run Simulation in Parallel** option and put 56 for the **Number of Processors**.  

.. _fig-we15-num-setup:

.. figure:: figures/we14_numerical_setup.svg
   :align: center
   :width: 75%

   Edit inputs in *Numerical Setup* tab


Set Results to Monitor
^^^^^^^^^^^^^^^^^^^^^^^
In the *Monitoring* tab, specify simulation results to be record as seen in :numref:`fig-we15-mon`. For this example we will monitor both the velocity and pressure distribution at selected places in the computational domain. Firstly, check **Record Wind Profiles** checkbox. Then, add three line probes (profiles) by clicking **Add Profile** and edit details as shown in bellow figure. For example, *Profile1* is located at the inlet and measures the characteristics flow velocity. *Profile2* is located downstream and measures the incident flow (i.e., the wind the structure will experience) on a vertical line. Whereas, *Profile3* measures the downstream evolution of the pressure fluctuation in the stream-wise direction. Also, specify the **Field Write Interval** to 10 and **Write Start Time** to 1.      
 
To monitor the snapshot of the flow field, check **Sample Flow Field** and add two section planes for monitoring velocity field. Edit the table entries as seen in :numref:`fig-we15-mon`. *Plane1* is vertical plane normal to y-axis and monitors the snapshot of the velocity field from 1 s to 3 s.  *Plane2* is a horizontal plane normal to z-axis and monitors the snapshot of the velocity field from 1 s to 3 s. The files for both planes will be written every 50 time step (i.e :math:`50 \times 0.0005 = 0.025 s`) as specified in **Flow Write Interval**. 


Random Variables
"""""""""""""""""
The random variables are defined in **RV** tab. Here, the floor stiffness named as :math:`k` in **SIM** panel is automatically assigned as a random variable. Select *Normal* for its probability **Distribution**  with :math:`5 \times 10^{8}` for the **Mean** and :math:`5 \times 10^{7}` for **Standard Dev**. 

.. figure:: figures/we14_RV_panel.svg
   :align: center
   :width: 75%

   Define the Random Variable (RV)

Running the Simulation 
"""""""""""""""""""""""
 To run the CFD simulation, first login to *DesignSafe* with your credential. Then, run the job remotely by clicking **RUN at DesignSafe**. Give the simulation a **Job Name**.  Set **Num Nodes** to 1 and **# Processes Per Node** to 56. For the **Max Run Time**, specify *20:00:00*. Finally, click the **Submit** button to send the job to *DesignSafe*.  

.. figure:: figures/we14_RunJob.svg
   :align: center
   :width: 80%

   Submit the simulation to the remote server (DesignSafe-CI)

Results
"""""""""
The status of the remote job can be tracked by clicking **GET from DesignSafe**. Once the remote job finishes, the results can be reloaded by selecting **Retrieve Data** option right-click clicking on the job name. Then, the results will be displayed in **RES** tab. The responses qualitative reported for *Standard* EDP include statistics of floor displacement, acceleration and inter-story drift, e.g.,    

      * 1-PFA-0-1: represents **peak floor acceleration** at the **ground floor** for **component 1** (x-dir)
      * 1-PFD-1-2: represents **peak floor displacement** (relative to the ground) at the **1st floor** ceiling for **component 2** (y-dir)
      * 1-PID-3-1: represents  **peak inter-story drift ratio** of the **3rd floor** for **component 1** (x-dir) and
      * 1-RMSA-108-1: represents **root-mean-squared acceleration** of the **106th floor** for **component 1** (x-dir).   

The *Summary* tab of the panel, shows the four statistical moments of the EDPs which include *Mean*, *StdDev*, *Skewness* and *Kurtosis*. 

.. figure:: figures/we14_RES_Summary.svg
   :align: center
   :width: 75%

   Summary of the recorded EDPs in **RES** panel

By switching to *Data Values* tab, the user can also visualize all the realizations of the simulation. The figure below shows the variation of the top-floor acceleration with floor stiffness. 

.. figure:: figures/we14_RES_DataValues.svg
   :align: center
   :width: 75%
   :figclass: align-center

   (scatter-plot) Top-floor acceleration vs floor stiffness, (table) Report of EDPs for all realizations   



Flow visualization 
^^^^^^^^^^^^^^^^^^^^^^^^^^^
The full simulation data can be retrieved from *DesignSafe* and visualized remotely using Paraview. :numref:`fig-we14-CFD-result` shows the streamlines and velocity contour taken on a vertical stream-wise section. From the plots, it is clearly visible that important flow features such as vortex shading and turbulence at the wake are captured.  

.. _fig-we14-CFD-result:

.. figure:: figures/we14_CFD_Results.svg
   :align: center
   :width: 100%
   :figclass: align-center

   Instantaneous velocity field around the building.

.. [Franke2007] Franke, J., Hellsten, A., Schlünzen, K.H. and Carissimo, B., 2007. COST Action 732: Best practice guideline for the CFD simulation of flows in the urban environment.

