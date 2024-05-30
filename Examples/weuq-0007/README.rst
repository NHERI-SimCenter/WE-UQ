.. _weuq-0007:

3 Story Building: Sensitivity Analysis
======================================

Consider the problem of uncertainty quantification in a three-story steel building. The building being modeled is the 3-story Boston building presented in the FEMA-355C report. From the description in Appendix B of the FEMA document the building is a rectangular building **120** ft by **180** ft with a height above ground of **39** ft. The building weight is approx. **6,500** kips. Eigenvalues are shown in Table 5.1. of the FEMA document to be between **1.6** sec and **1.9** sec depending on connection details. For this example (and for demonstrative purposes only), this building will be modeled as a shear building with **10** nodes and **9** elements, as shown in the following figure. For loading, the TPU Low Rise Wind Generation tool will be used with the mean wind speed being treated as a random variable with a normal distribution described by a mean :math:`\mu_{gustWS}=20 \mathrm{mph}` and standard deviation :math:`\sigma_{gustWS} =3 \mathrm{mph}` (COV = 15%). 

.. figure:: figures/3story.png
   :align: center
   :alt: A minimalistic image with a white rectangle featuring a diagonal line pattern, centered against a solid black background.
   :height: 600
   :figclass: align-center

   Three-Story Downtown Boston Building from FEMA-355C

The structure has uncertain properties that all follow normal distribution:

1. Weight of Building(bldgW): mean :math:`\mu_{bldgW}=6500 \mathrm{kip}` and standard deviation :math:`\sigma_{bldgW} =400 \mathrm{kip}` (COV = 6%)
2. Story Stiffness(storyK): mean :math:`\mu_{storyK}=1600 \mathrm{kip/in}` and standard deviation :math:`\sigma_{storyK} =160 \mathrm{kip/in}` (COV = 10%)

.. note::

   #. For the mean values provided the natural period of the structure is **1.7** sec.
   #. The choice of COV percentages is for demonstrative purposes only.

The exercise will use the OpenSees, :numref:`lblOpenSeesSIM`, structural generators. For the OpenSees generator the following model script, `3story.tcl <https://github.com/NHERI-SimCenter/WE-UQ/blob/master/examples/3Story/3story.tcl>`_ :

.. literalinclude:: 3Story.tcl
   :language: tcl

.. note::
   
   1. The first lines containing ``pset`` will be read by the application when the file is selected and the application will autopopulate the Random Variables ``w`` and ``k`` in the **RV** tab with this same variable name. It is of course possible to explicitly use Random Variables without the ``pset`` command by "RV.**variable name" in the input file. However, no random variables will be auto-populated if the user chooses this route.

.. warning::

   Do not place the file in your root, downloads, or desktop folder as when the application runs it will copy the contents on the directories and subdirectories containing this file multiple times (a copy will be made for each sample specified). If you are like us, your root, Downloads or Documents folders contain an awful lot of files and when the backend workflow runs you will slowly find you will run out of disk space!


Global Sensitivity Analysis
^^^^^^^^^^^^^^^^^^^^^^^^^^^

In a global sensitivity analysis, the user wishes to understand what is the influence of the individual random variables on the quantities of interest. This is typically done before the user launches large-scale forward uncertainty problems to limit the number of random variables used so as to limit the number of simulations performed.

For this problem, we will limit the response quantities of interest to the following six quantities. Peak Roof displacement in 1 and 2 directions, root mean square (RMS) accelerations in 1 and 2 directions, Peak BAse shear and moments in 1 and 2 directions. In order to do this we will introduce User defined EPD quantities, which necessitates the following two files: `recorder.tcl <https://github.com/NHERI-SimCenter/WE-UQ/blob/master/examples/3Story/recorder.tcl>`_ and `postprocess.tcl <https://github.com/NHERI-SimCenter/WE-UQ/blob/master/examples/3Story/postprocess.tcl>`_.  These files are the same as those presented in :ref:`lblWE-Example9`


To perform a Global Sensitivity  analysis the user would perform the following steps:


#. Start the application and the UQ Selection will be highlighted. In the panel for the UQ selection, keep the UQ engine as that selected, i.e. Dakota. From the UQ Method Category drop-down menu select Sensitivity Analysis, Keeping the method as LHS (Latin Hypercube). Change the #samples to 500 and the seed to 20 as shown in the figure.

    .. figure:: figures/weuq-0007-UQ.png
       :align: center
   :alt: Screenshot of a software interface for uncertainty quantification (UQ) using the Dakota toolkit. The interface shows options for the Dakota Method Category selected as "Sensitivity Analysis" with the method dropdown set to "LHS" (Latin Hypercube Sampling), "# Samples" inputted as 100, "Seed" number entered as 453, and an unchecked box for "Keep Samples." To the right, a dropdown for the UQ Engine with "Dakota" selected and an unchecked box for "Parallel Execution." The left sidebar includes menu items labeled UQ, GI, SIM, EVT, FEM, EDP, RV, and RES.
       :figclass: align-center

#. Next, select the **GI** panel. In this panel, the building properties and units are set. For this example enter **3** for the number of stories, **468** for building height, **1440** for building width, and **2160** for building depth.

    .. figure:: figures/weuq-0007-GI.png
       :align: center
   :alt: Screenshot of a user interface for inputting building information. The interface has sections for the name, properties such as number of stories (3), height (468), width (1440), depth (2160), plan area (129600), location with latitude (37.8715) and longitude (-122.273), and units for force (Kips), length (Inches), and temperature (Celsius). There are vertical navigation tabs on the left with acronyms UQ, GI, SIM, EVT, FEM, EDP, RV, and RES.
       :figclass: align-center

#. Next, select the **SIM** tab from the input panel. This will default in the MDOF model generator. Using the drop-down menu change the generator to the OpenSees option. Specify the path the the story.tcl file, and for the two line edits for centroid nodes and response node enter **1 2 3 4** (which identifies the 4 column nodes as having an id of **1**, **2**, **3**, and **4**. For the spatial dimension enter **3** (3d model) and for number of dof enter **6** as shown in figure:

   .. figure:: figures/weuq-0007-SIM.png
      :align: center
   :alt: Screenshot of a software interface labeled as "Building Model Generator" with a dropdown menu option set to OpenSees. The interface includes fields for Input Script, Centroid Nodes, Response Nodes, Spatial Dimension, and # DOF at Nodes with filled in values and a 'Choose' button next to the Input Script field. The sidebar lists various tabs such as UQ, GI, SIM, EVT, FEM, EDP, RV, and RES, with the SIM tab highlighted.
      :figclass: align-center

#. Next, select the **EVT** panel. From the Load Generator pull-down menu select the **Low Rise TPU** option. Leave the exposure condition as **B**. Set the drag coefficient as **1.3** and enter ``gustWS`` for the 3-sec gust wind speed at the 33 ft height.

   .. figure:: figures/weuq-0007-EVT.png
      :align: center
   :alt: Screenshot of software interface with a section titled "Wind Tunnel Building Geometry" showcasing input fields for Roof Type, Height/Width, Depth/Width, Roof Pitch, and Angle of Incidence. A small diagram illustrates wind direction and building dimensions. There is also a section for inputting Wind Speed, labeled "meanWS" in meters per second. The interface includes a sidebar with various menu options like UQ, GI, SIM, EVT, FEM, EDP, RV, and RES.
      :figclass: align-center

#. Next, choose the **FEM** panel. Here we will change the entries to use Rayleigh damping, with the Rayleigh factor chosen using **1** and **6** modes. For the **MDOF** model generator, because it generates a model with two translational and 1 rotational degree-of-freedom in each direction and because we have provided the same **k** values in each translational direction, i.e. we will have duplicate eigenvalues, we specify as shown in the figure modes **1** and **6**.

    .. figure:: figures/weuq-0007-FEM.png
       :align: center
   :alt: Screenshot of the 'FE Application' settings interface, likely from a software for engineering simulation or analysis. The selected application is OpenSees, with various input fields and dropdown menus for setting up analysis parameters such as 'Analysis,' 'Integration,' 'Algorithm,' 'ConvergenceTest,' 'Solver,' 'Damping Model,' along with fields to input values for 'Damping Ratio,' 'Selected Tangent Stiffness,' 'Mode 1,' and 'Mode 2.' There is also an 'Analysis Script' section with a 'Choose' button. The user interface appears functional and is designed for technical users to configure their finite element analysis settings.
       :figclass: align-center

#. We will skip the **EDP** panel leaving it in its default condition, that being to use the **Standard Wind** EDP generator.

#. For the **RV** panel, we will enter the distributions and values for our random variables. Because of the steps we have followed and entries we have made, the panel when we open it should contain the **3** random variables and they should all be set constant. For the w, k and wS random variables we change the distributions to normal and enter the values given for the problem, as shown in the figure below. 

   .. figure:: figures/9story-RV.png
      :align: center
   :alt: Screenshot of a software interface titled "Input Random Variables" with options to add or remove variables. It lists three variables with their corresponding distribution type set to "Normal," their means, and standard deviations. The variables 'w' and 'k' have means of 2200 and 1600 with standard deviations of 220 and 160 respectively. Variable 'gustWS' has a mean of 20 and a standard deviation of 2. Each variable entry has a button to "Show PDF." The left sidebar contains various abbreviated menu options such as UQ, GI, SIM, EVT, FEM, EDP, RV, and RES, with RV highlighted in blue.
      :figclass: align-center

.. warning::   

   The user cannot leave any of the distributions for these values as constant for the Dakota UQ engine.

#. Next, click on the 'Run' button. This will cause the backend application to launch Dakota. When done the **RES** tab will be selected and the results will be displayed. The results show the values of the mean and standard deviation. The peak displacement of the roof is the quantity **1-PFD-9-1** (first event (tool to be extended to multiple events), 9th floor (in US ground floor considered 0), and 1 dof direction). the **PFA** quantity defines peak floor acceleration, the **RMSA** quantity is the root mean square of floor accelerations, and the **PID** quantity corresponds to peak inter-story drift.

   .. figure:: figures/9story-RES1.png
      :align: center
   :alt: Screenshot of a statistical summary table with multiple entries, each showing the Name, Mean, Standard Deviation, Skewness, and Kurtosis of various datasets. The datasets are identified by codes such as "1-RMSA-7-1", "1-PFD-7-1", and similar. The table is organized in a clean, tabular format with alternating shades of grey for better readability.
      :figclass: align-center

If the user selects the "Data" tab in the results panel, they will be presented with both a graphical plot and a tabular listing of the data. By left- and right-clicking with the mouse in the individual columns the axis changes (the left mouse click controls the vertical axis, right mouse clicks the horizontal axis).

.. figure:: figures/9story-RES2.png
   :align: center
   :alt: A screenshot depicting a scatter plot with a large number of blue dots that exhibit an upward trend from left to right. The x-axis is labeled "1-PFA-1-1" with values ranging from 0.7 to 10.1, and the y-axis is labeled "gustWS" with values ranging from 9.0 to 30.6. Below the chart, there's a partial view of a table with numerical data labeled "Run #", "w", "k", "gustWS", "1-PFA-1-1", "1-RMSA-1-1", "1-PFD-1-1", "1-PID-1-1", "1-PFA-2-1", and "1-RMSA". There's also a button that says "Save Data", and navigation tabs on the left side with acronyms such as "UQ", "GI", "SIM", and others.
   :figclass: align-center

Various views of the graphical display can be obtained by left and right-clicking in the columns of the tabular data. If a singular column of the tabular data is pressed with both right and left buttons a frequency and CDF will be displayed, as shown in the figure below.

.. figure:: figures/9story-RES6.png
   :align: center
   :alt: Two charts side by side within a software interface. The chart on the left is a histogram with vertical bars representing the distribution of data points over a range of values on the x-axis, labeled 'NPV>Prob %.' The chart on the right is a cumulative distribution curve that smoothly rises across the x-axis range, also labeled 'NPV>Prob %.' Below each chart is a table with numerical data corresponding to specific points on the horizontal axis of the respective charts. The interface has tabs for "Summary" and "Data Values," with the "Data Values" tab currently selected.
   :figclass: align-center

User Defined Output
^^^^^^^^^^^^^^^^^^^

In this section, we will demonstrate the use of the user-defined output option for the EDP panel. In the previous example, we got the standard output, which can be both a lot and also limited (in the sense you may not get the information you want). In this example we will present how to obtain results just for the roof displacement, the displacement of node **10** in both the **MDOF** and **OpenSees** model generator examples and the shear force at the base of the structure. For the OpenSees model, it is also possible to obtain the overturning moment (something not possible in the MDOF model due to the fact it is modeled using spring elements). The examples could be extended to output for example the element end rotations, plastic rotations, ...

For this example you will need two additional file `recorder.tcl <https://github.com/NHERI-SimCenter/WE-UQ/blob/master/Examples/9story/recorder.tcl>`_ and `postprocess.tcl <https://github.com/NHERI-SimCenter/WE-UQ/blob/master/examples/9story/postprocess.tcl>`_. 

The recorder script as shown will record the envelope displacements and RMS accelerations in the first two degrees-of-freedom for the nodes in the modes. The script will also record the element forces. The file is as shown below:

.. literalinclude:: recorder.tcl
   :language: tcl

The ``postprocess.tcl`` script shown below will accept as input any of the 10 nodes *in the domain and for each of the two dof directions and element forces.

.. literalinclude:: postprocess.tcl
   :language: tcl

.. note::

   The user has the option when using the OpenSees SIM application to provide no post-process script (in which case the main script must create a ``results.out`` file containing a single line with as many space-separated numbers as QoI or the user may provide a Python script that also performs the postprocessing. An example of a postprocessing Python script is `postprocess.py <https://github.com/NHERI-SimCenter/WE-UQ/blob/master/examples/9story/postprocess.py>`_. The Python script at present only responds to nodal displacements.

   .. literalinclude:: postprocess.py
      :language: python

The steps are the same as the previous example, except step 4 defining the **EDP**.

#. For the **EDP** panel, we will change the generator to **User Defined**. In the panel that presents itself, the user must provide the paths to both the recorder commands and the postprocessing script. Next, the user must provide information on the response parameters they are interested in. The user presses the **Add** button and the entries ``Disp_10_1``, ``RMSA_10_1``, and ``Force_1_1`` in the entry field as shown in the figure below.

   .. figure:: figures/9story-EDP-USER.png
      :align: center
   :alt: Screenshot of a software interface showing "Engineering Demand Parameters" with an option for 'User Defined' selected from a dropdown menu. Below are fields for 'Additional Input' and 'Processing Script', each with file paths provided and a 'Choose' button alongside. A section titled 'Response Parameters' lists three parameters with the names 'Disp_10_1', 'RMSA_10_1', and 'Force_1_1', each with a 'Remove' button next to them. On the left side, a vertical navigation menu with acronyms like 'UQ', 'GI', 'SIM', 'EVT', 'FEM', 'EDP', 'RV', and 'RES' is partially visible.
      :figclass: align-center


#. Next, click on the 'Run' button. This will cause the backend application to launch Dakota. When done the **RES** tab will be selected and the results will be displayed. The results show the values of the mean and standard deviation as before but now only for the one quantity of interest.

   .. figure:: figures/9story-RES-USER.png
      :align: center
   :alt: A screenshot depicting a statistical summary table with columns for Name, Mean, StdDev (Standard Deviation), Skewness, and Kurtosis. The table contains three rows with statistical values for different variables: Disp_10_1, RMSA_10_1, and Force_1_1. The 'Summary' tab is currently selected, suggesting this image is possibly taken from a software application used for statistical analysis or simulation results.
      :figclass: align-center


Reliability Analysis
^^^^^^^^^^^^^^^^^^^^

If the user is interested in the probability that certain response measures will be exceeded an alternative strategy is to perform a reliability analysis. To perform a reliability analysis the steps above would be repeated with the exception that the user would select a reliability analysis method instead of a Forward Propagation method. To obtain reliability results using the Global Reliability method presented in Dakota choose the **Global Reliability** methods from the methods drop-down menu. In the response levels enter values of **0.5** and **0.8**, specifying that we are interested in the value of the CDF for a displacement of the roof of 0.5 in and 0.8 in, i.e. what is the probability that displacement will be less than 0.8 in.


.. figure:: figures/9story-UQ-Reliability.png
   :align: center
   :alt: Screenshot of a user interface for the 'UQ Engine' with a menu bar on the left side showing options like UQ, GI, SIM, EVT, FEM, EDP, RV, and RES. The main panel is titled 'UQ Engine' with a dropdown menu set to 'Dakota.' Below, there's a section titled 'Dakota Method Category' with 'Reliability Analysis' selected. 'Reliability Method' is set to 'Global Reliability,' 'GP Approximation' to 'x_gaussian_process,' 'Response Levels' has values '0.5 0.8', and 'Seed' has the number '377' entered. The interface features a clean, modern design with predominantly blue and white colors.
   :figclass: align-center

After the user fills in the rest of the tabs as per the previous section, the user would then press the **RUN** button. The application (after spinning for a while with the wheel of death) will present the user with the results, which as shown below, indicate that the probabilities as **52%** and **92%**.

.. figure:: figures/9story-RES-Reliability.png
   :align: center
   :alt: A line graph with a horizontal x-axis labeled "Disp_10_1:" ranging from 0.5 to 0.8 and a vertical y-axis labeled "Probability" ranging from 0.0 to 1.0. The graph displays a positively sloped line, starting at the lowest x-axis value with a probability slightly above 0.5 and ending at the highest x-axis value with a probability nearly 1.0. Two data points are marked with blue diamonds, one near the start and one near the end of the line. Below the graph, there's a table with two rows displaying numerical values in scientific notation. The first row has values "5.2634999965e-01" and "5.0000000000e-01," while the second row has values "9.1670259474e-01" and "8.0000000000e-01."
   :figclass: align-center

.. warning::

   Reliability analysis can only be performed when there is only one EDP.

   .. figure:: figures/9story-EDP-Reliability.png
      :align: center
   :alt: Screenshot of a user interface for software parameters configuration. The interface includes dropdown menus for 'Engineering Demand Parameters' set to 'User Defined', fields for 'Additional Input' and 'Processing Script' with file paths provided, and a section called 'Response Parameters' with one entry named 'Disp_10_1'. There are buttons labeled 'Choose' next to the input fields and 'Add' and 'Remove' next to the response parameter. The left sidebar shows tabs labeled UQ, GI, SIM, EVT, FEM, EDP, RV, RES in a vertical layout, with the EDP tab currently selected.
      :figclass: align-center

