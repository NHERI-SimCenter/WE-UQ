.. _advancedCFD:

Community level wind simulation: WE-UQ coupled with BRAILS++
==========================================================

+----------------+-----------------------------------------------------------------------------------------------------------+
| Problem files  | `WE-UQ/Examples/weuq-0019 <https://github.com/NHERI-SimCenter/WE-UQ/tree/master/Examples//advancedCFD//>`_  |
+----------------+-----------------------------------------------------------------------------------------------------------+

Introduction
^^^^^^^^^^^^^

This module provides a workflow to simulate wind flow inside a community. The building footprints are generated using the  inventory. To install BRAILS++, the user needs to run the command: pip install brails. Additionally, the user needs to install the geopandas, shapely, pyproj, trimesh, rtree, and mapbox-earcut python libraries using the command: pip install geopandas shapely pyproj trimesh rtree mapbox-earcut. To generate the computational domain, the user needs to provide the longitude and latitude of the center point, the bounding radius, and the longitudes and latitudes of the bounding box encompassing the region of interest. The user needs to have  downloaded on their computer. The user needs to input the parameters for the CFD simulation following the workflow shown in Fig. 1, using the developed Python script for this module, then run the wind simulation in OpenFOAM, and finally view the results in Paraview.

.. _fig-advanced_cfd-1:

.. figure:: figures/Picture1_worflow.png
   :align: center
   :width: 75%

   The WE-UQ and BRAILS++ integration workflow.

Detailed Workflow
^^^^^^^^^^^^^^^^^^^^
Generating the GeoJSON files with building footprints and their heights:

Generating the GeoJSON files with building footprints and their heights
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""



The BRAILS++ inventory contains the longitudes and latitudes of building footprints with their respective heights and outputs them as a geojson file, which can be viewed in a GIS (Geographic Information System) software such as ArcGIS or QGIS. The user needs to input the center point and the bounding radius around the community of interest as shown in Fig. 2. Then, the user must input two coordinates (minimum and maximum extents) for the bounding box that encompasses the region of interest (ROI) as shown in Fig. 3. The ROI must contain buildings within the bounding radius that are of more importance than others. The latitude and longitude of a specific point can be obtained by clicking at a location on the google maps. For the ROI, the latitude and longitude of the two points marked with red circles in Fig. 3 are required as inputs. The user also has an option to choose the footprint scraper (USA, OSM, and Microsoft) to retrieve the building information. The default scraper is USA. These inputs generate two geojson files named “inventoryTotal.geojson” and “inventoryROI.geojson”. The former contains information about all buildings inside the bounding radius, and the latter contains information about buildings inside the bounding box.


.. _fig-advanced_cfd-2:

.. figure:: figures/Picture2.png
   :align: center
   :width: 35%

   Center coordinate and the bounding radius of the total region.


.. _fig-advanced_cfd-3:

.. figure:: figures/Picture3.png
   :align: center
   :width: 35%

   Bounding box for the region of interest.

Defining the computational domain and creating the mesh
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

The computational domain consists of 8 boundary faces: inlet, outlet, side1, side2, top, ground, ROI, and Surrounding. The inlet face is where the inflow of wind is defined. According to the COST 732 (Franke et al. 2007) recommendations, the inlet face must be at least a distance of five times the maximum height (5Hmax) of the buildings in the domain, from the outermost footprint coordinate. The orientation of the inlet face shall be perpendicular to the wind direction. The sides will be parallel to the wind direction. The COST recommendations suggest the lateral boundaries to be at least a distance of 5Hmax from the community. The outlet boundary is where the flow leaves the domain. It needs to be at least a distance of 15Hmax from the community. The top boundary of the domain must also be a distance of more than 5Hmax from the top of the building with maximum height. Since the horizontal extents of the domain are much larger than the vertical extent, the default value for the top boundary is 15Hmax from the ground. Ground, ROI, and surrounding boundary faces are wall boundaries where the flow can’t enter and represent the ground, the buildings in ROI, and the buildings surrounding the ROI, respectively. A depiction of computational domain extents is presented in Fig. 4. 

For this step, the user is expected to provide the wind direction in the community, the domain extents (optional, default values are according to the COST 732 recommendations), the nominal computational cell size for blockMesh, the mesh refinement regions, the level of mesh refinement for each region, and the levels of mesh refinement for the ROI and surrounding buildings. Level n mesh refinement would mean the cell size in that region would be . If the user doesn’t define the mesh refinement for the ROI buildings and the Surrounding buildings, the level of refinement for the surrounding would be the minimum level of refinement for the refinement boxes + 1, and the level of refinement for the ROI would be the level of refinement for the surrounding + 1. An example mesh refinement is shown in Fig. 5. The origin (0,0,0) of the computational domain is at the bottom right corner of the inlet plane. The domain extents are defined in terms of Hmax. For example, if the user enters a value of 10 for inlet distance multiplier, the inlet will be 10Hmax from the buildings. The user also has the option to name the OpenFOAM case folder (the default is “case”). The outputs generated from this step are saved in the case/system folder and case/constant/triSurface folder. The blockMeshDict and snappyHexMeshDict files are saved in the case/system folder, while the ROI.stl and Surrounding.stl files are saved in the case/constant/triSurface folder. 



.. _fig-advanced_cfd-4:

.. figure:: figures/Picture4.png
   :align: center
   :width: 50%

   Domain extents.


.. _fig-advanced_cfd-5:

.. figure:: figures/Picture5.png
   :align: center
   :width: 50%

   Levels of mesh refinement.



Defining the Boundary Conditions
""""""""""""""""""""""""""""""""""

The boundary conditions are one of the most important parts of a CFD simulation. Therefore, it is essential that they are defined properly. The user has the choice to use a steady logarithmic velocity profile or generate a time-varying velocity profile using the digital filter method described by the turbulent inflow tool (). For the logarithmic profile, the user needs to provide the reference wind speed (Uref), the reference height (Zref), and the roughness length (zo). On the other hand, if the user chooses turbulent inflow, they must provide a CSV file containing the following information: points in the vertical direction, mean wind speed at those points, the 6 Reynolds stress tensor entries, and the 9 length scales. All of these quantities must occupy a column in the CSV file. The side boundaries could be defined as either slip or cyclic. The slip condition mimics the symmetry boundary condition, i.e., there is no flow through the surface. The top boundary is very far away, and hence a slip condition is applied there. The outlet surface is in a zero-pressure condition. The ground, ROI, and surrounding surfaces are walls; therefore, the velocity is zero at these surfaces. The standard wall functions are applied to other variables. The user can choose between smooth and rough wall functions. 


Choosing the turbulence model and setting up the simulation
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

There are three ways to model turbulence: Reynolds averaging (RANS), large eddy simulations (LES), and direct numerical simulations (DNS). For atmospheric flow, using DNS is not feasible due to the very high Reynolds number and a large variation in the length scales. Hence, the user has an option to choose between RANS and LES turbulence models. The RANS model predicts the mean flow very well, but models turbulence using a  model (Launder and Spalding 1974), whereas LES models the largest turbulent eddies in the flow and models the subgrid scale (SGS) eddies using a Smagorinsky model (Smagorinsky 1963). 

If the user chooses the RANS model, the inflow velocity profile is automatically chosen to be logarithmic, given by the following equation:

where  is the friction velocity,  is the von Karman constant, z is the vertical coordinate, and  is the roughness length. The initial files with the variables (U, k, epsilon, p, and nut) are saved in the case/0 folder. The turbulence parameters are written in the “turbulenceProperties” file and saved in the case/constant folder. The user has the option to also prescribe the kinematic viscosity of air (default is  m2/s). This value is saved in the “transportProperties” file in the case/constant folder. The user also needs to provide the end time of the simulation and the time step. The number of iterations then becomes . The time step size doesn’t matter as this is a steady-state simulation. The user also needs to specify the interval for writing the files. The output files will be written after the number of iterations mentioned in the interval. The simulation stops either on convergence or if the simulation reaches the end time, whichever comes first. The equations are solved using the “SIMPLE” (Semi-Implicit Method for Pressure Linked Equations) algorithm. These details are outputted in the “controlDict” file saved in the case/system folder. Additional files such as “surfaceFeaturesDict”, “fvSolution”, and “fvSchemes” are also saved in the case/system folder containing details of the building features, solution algorithms to linear system of equations, the convergence criteria, and the discretization schemes for various terms. Convergence is reached when all of the residuals are under .

If the user opts to choose the LES model, the user has an option to choose between TINF and the mean velocity profile as described in the RANS section. If the user chooses TINF, they are expected to provide details on the mean wind profile, Reynolds stresses, and the integral length scales in the format described in section 2.3. An example screenshot of the CSV file is shown in Fig. 6. The TINF files are saved in the case/constant/boundaryData/inlet folder. Even though the user chooses TINF, they need to provide reference wind speed, reference height, and the roughness length for the atmospheric boundary layer (ABL) wall functions used. The user is required to provide the initial time step for the simulation. The size of the time step is very important in LES as it is a transient simulation. The user needs to define the end time of the simulation as well. It takes time for the flow to settle and become independent of the initial conditions; therefore, it is suggested that the user give more time than what is required. Additionally, the user can choose between “PISO” (Pressure-Implicit with Splitting of Operators) and “PIMPLE” (PISO + SIMPLE) solvers for the simulation. Moreover, if the user selects “PIMPLE”, there is an option to automatically adjust the time step according to the maximum Courant number (also prescribed by the user). If the user chooses the “PISO” algorithm, the initial time step will remain constant throughout the simulation (even though there is an option to select the adjusted time step option). As opposed to RANS, in LES mode, the write interval is based on run-time and not the number of iterations. For example, if the user chooses 1 as the write interval for LES, the outputs will be saved at each second rather than each iteration. The user has the option to prescribe several profiles and planes for recording velocity or pressure, or both, at every iteration. The profile contains a line of probes (number is user-defined), with the start and end points of the line defined by the user. For the plane, the user needs to define the point in the plane and the normal vector to the plane. The point must not be on the boundary. The user also needs to provide the number of processors to run the simulation in parallel. If the user doesn’t choose to run in parallel, they can put 1 in the number of processors.

.. _fig-advanced_cfd-5:

.. figure:: figures/Picture6.png
   :align: center
   :width: 50%

   An example of csv file for TINF.



Running in OpenFOAM
"""""""""""""""""""""
The user is required to have OpenFOAM v10 installed on their computer. Once the user has generated all the required files using the above workflow, they can run the simulation using the following procedure:

1. Open the Linux terminal in which OpenFOAM v10 is installed and go to the case folder.
2. Run the ``blockMesh`` command to generate the background mesh.
3. Run the ``surfaceFeatures`` command to create the building features.
4. Optionally run ``decomposePar`` to decompose the mesh.
5. Run ``snappyHexMesh -overwrite`` command either in serial or parallel mode.
6. If ``snappyHexMesh`` was run in parallel, run reconstructParMesh -constant command to reconstruct the mesh.
7. Optionally run ``decomposePar -force``, to decompose the mesh and run the simulation in parallel.
8. Run the ``simpleFoam`` or ``pimpleFoam``, or pisoFoam command (depending on the algorithm chosen by the user) either in serial or parallel mode.
9. If the user ran the simulation in parallel, then run the ``reconstructPar`` command.


UQ Method
"""""""""""
Specify the details of uncertainty analysis in the **UQ** panel. This example uses forward uncertainty propagation. Select "Forward Propagation" for UQ Method and specify "Dakota" for the UQ Engine driver. For specific UQ algorithms, use Latin Hypercube ("LHC"). Change the number of samples to 500 and set the seed to 101.

.. figure:: figures/we19_UQ_panel.svg
   :align: center
   :alt: Image showing error in description
   :width: 80%
   :figclass: align-center

   Selection of the Uncertainty Quantification Technique

General Information
"""""""""""""""""""
Next, in the **GI** panel, specify the properties of the building and the unit system. For the **# Stories** use 2 assuming a floor height of approximately 4.0 m. Set the **Height**, **Width** and **Depth** to 8.8, 24.0 and 16.0 with a **Plan Area** of 384.0. Define the units for **Force** and **Length** as "Newtons" and "Meters", respectively. 


.. figure:: figures/we19_GI_panel.svg
   :align: center
   :alt: Image showing error in description
   :width: 75%

   Set the building properties in **GI** panel

Structural Properties
"""""""""""""""""""""
Please leave the **SIM** panel of the workflow as it is, this example does not involve any structural analysis. 


CFD Model
"""""""""""""""""""
To set up the CFD model, in the **EVT** panel, select "CFD - Wind Loads on Isolated Building" for **Load Generator**.  Detailed documentation on how to define the CFD model can be found in :ref:`the user manual<lblIsolatedBuildingCFD>`.   

1. Specify the path to the case directory in *Start* tab, by clicking **Browse** button. Use version 10 for **Version of OpenFOAM Distribution**. 

.. figure:: figures/we19_EVT_Start_tab.svg
   :align: center
   :alt: Image showing error in description
   :width: 75%

   Setting up the case directory and OpenFOAM version in the *Start* tab

2. In the *Geometry* tab, first set the **Input Dimension Normalization** to *Relative* to put the size of the domain relative to the building height. For **Geometric Scale** of the CFD model use 100.0 as the simulation is conducted at model scale. Set the **Shape Type** to *Complex* and import the building geometry by clicking **Import STL** from :github:`here <Examples/weuq-0019/src/buildingGeometry/tpu_building.stl>`. Set the **Wind Direction** to 90.0 to simulate wind incidence normal to the building width. See :numref:`fig-we19-geometry-tab` for the details.

.. _fig-we19-geometry-tab:
.. figure:: figures/we19_EVT_Geometry_tab.svg
   :align: center
   :width: 95%

   Defining the domain dimensions and the building geometry.  


2. Define the computational in *Mesh* tab with *Background Mesh*, *Regional Refinements*, *Surface Refinements*, *Edge Refinements* and *Edge Refinements* as shown bellow.
   
   .. figure:: figures/we19_EVT_Mesh_tab.svg
      :align: center
      :width: 75%

      Define the computational grid in the *Mesh* tab

   .. figure:: figures/we19_EVT_Mesh_RegionalRefinement_tab.svg
      :align: center
      :width: 75%

      Create regional refinements

   .. figure:: figures/we19_EVT_Mesh_SurfaceRefinement_tab.svg
      :align: center
      :width: 75%

      Create surface refinements
   
   .. figure:: figures/we19_EVT_Mesh_EdgeRefinement_tab.svg
      :align: center
      :width: 75%

      Apply further refinements along the building edges

   **Run Mesh**
   
   To generate the computational grid with all the refinements applied, click the **Run Final Mesh** button in the *Mesh* tab. Once meshing is done, in the side window, the model will be updated automatically displaying the generated grid. 


   .. figure:: figures/we19_EVT_Mesh_View.svg
      :align: center
      :width: 85%

      Breakout View of the Mesh
   
4. To define initial and boundary conditions, select *Boundary Conditions* tab. 

   * Based on the values given in :numref:`tbl-we19-1`, set the boundary conditions as shown in the following figure. Here the **Wind Speed Scaling Factor** is defined as a random variable and the uncertainties will be propagated in the wind load calculation. At the **Inlet** of the domain use *TInf* with the specified inflow generation method (DFM). Then, select *Table* for the **Wind Profile** and import the wind characteristics from :github:`here <Examples/weuq-0019/src/wind_profile.csv>`

   .. figure:: figures/we19_EVT_BoundaryConditions.svg
      :align: center
      :width: 75%

      Setup the *Boundary Conditions*  

5. Specify turbulence modeling, solver type, duration and time step options in the *Numerical Setup* tab as shown bellow. 

.. _fig-we19-CFD-num-setup:

.. figure:: figures/we19_EVT_NumericalSetup.svg
   :align: center
   :alt: Image showing error in description
   :width: 75%

   Edit inputs in the *Numerical Setup* tab


6. Monitor wind loads from the CFD simulation in the *Monitoring* tab. Leave this tab options as shown bellow. 

   .. figure:: figures/we19_EVT_Monitoring.svg
      :align: center
      :width: 75%

      Select the outputs from CFD in the *Monitoring* tab

Finite Element Analysis
"""""""""""""""""""""""""
Please leave this panel to the default values, since no structural analysis is needed. We are mainly interested in evaluating wind loads on components and cladding. 



Engineering Demand Parameter
"""""""""""""""""""""""""""""
Next, specify Engineering Demand Parameters(EDPs) in the **EDP** panel. Select *Component and Cladding EDP* option which allows the user to define the geometry of components. In the current workflow this is done using JSON file, which is provided in :github:`here <Examples/weuq-0019/src/ComponentDefinition.json>`.

.. figure:: figures/we19_EDP_panel.svg
   :align: center
   :width: 75%

   Select the EDPs to measure


Once specifying the path to this file in **Component Geometry JSON Path**, click **Map Component Geometry onto Building Surface**. This will map the comonent geometries on to the building surface as shown in the following figure.  

.. figure:: figures/we19_EDP_panel_components.svg
   :align: center
   :width: 75%

   Map components to the building geometry.  


Random Variables
"""""""""""""""""
Since the wind speed scaling factor is defined as a random variable, it will show up this panel. Now for the radom variable **wsF** set *Normal* for its probability **Distribution**  with :math:`1.0` **Mean** value and :math:`0.2` **Standard Dev**. 

.. figure:: figures/we19_RV_panel.svg
   :align: center
   :alt: Image showing error in description
   :width: 75%

   Define the Random Variable (RV)

Running the Simulation 
"""""""""""""""""""""""
The CFD simulation for this example is already run, and results are collected. The users can run the remain part of the workflow locally by clicking **RUN** button.


Results
"""""""""
Once the example is run, the results will aromatically show up . Then, the results will be displayed in the **RES** tab. The responses qualitative reported for *Standard* EDP include statistics of floor displacement, acceleration and inter-story drift, e.g.,    

      * 1-MP-zone1: represents **mean pressure** on a cladding/component element named **zone1** 
      * 1-RP-zone2: represents **root-mean-square pressure** on a cladding/component element named **zone2** 
      * 1-PP-zone1: represents **peak pressure** on a cladding/component element named **zone1** 
      * 1-MF-dr1: represents **mean force** on a component element named **dr1** 
      * 1-RP-wd1: represents **root-mean-square force** on a component element named **wd1** 
      * 1-PF-wd1: represents **peak force** on a component element named **wd1** 

The *Summary* tab of the panel shows the four statistical moments of the EDPs which include *Mean*, *StdDev*, *Skewness* and *Kurtosis*. 

.. figure:: figures/we19_RES_Summary.svg
   :align: center
   :width: 75%

   Summary of the recorded EDPs in **RES** panel

By switching to the *Data Values* tab, the user can also visualize all the realizations of the simulation. The figure below shows the variation of the peak pressure variation with the wind speed used in the simulation. 

.. figure:: figures/we19_RES_DataValues.svg
   :align: center
   :width: 75%
   :figclass: align-center

   (scatter-plot) Peak pressure vs wind speed, (table) Report of EDPs for all realizations   


.. [Franke2007] Franke, J., Hellsten, A., Schlünzen, K.H. and Carissimo, B., 2007. COST Action 732: Best practice guideline for the CFD simulation of flows in the urban environment.

