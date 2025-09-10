.. _weuq-0001:

9 Story Building: Sampling Analysis
===================================

Consider the problem of uncertainty quantification in a nine-story steel building. The building being modeled is the 9-story LA building presented in the FEMA-355C report. From the description in Appendix B of the FEMA document the building is a **150** ft square building with a height above ground of **120** ft with a weight of approx. **19,800** kips. Eigenvalues are shown in Table 5.1. of the FEMA document to be between **2.3** sec and **2.2** sec depending on connection details. For this example (and for demonstrative purposes only), this building will be modeled as a shear building with **10** nodes and **9** elements, as shown in the following figure. For loading, the Stochastic Wind Generation tool will be used with the gust wind speed being treated as a random variable with a normal distribution described by a mean :math:`\mu_{gustWS}=20 \mathrm{mph}` and standard deviation :math:`\sigma_{gustWS} =3 \mathrm{mph}`. 

.. figure:: figures/9Story.png
   :align: center
   :alt: The image displays three architectural or structural drawings labeled PLAN, ELEVATION, and MODEL. The PLAN shows a top view with a 5x5 grid, each grid labeled '5 bays @ 30' and a shaded central area. The ELEVATION displays a side view with vertical and horizontal lines representing the framework of a building structure with dimensions marked '@ 13' horizontally and '12' vertically. The MODEL has a simple line diagram representing a vertical element with two dots indicating connection points. All three represent different aspects of the same structure in engineering or architectural design.
   :width: 600
   :figclass: align-center

   Nine Story Downtown LA Building from FEMA-355C

The structure has uncertain properties that all follow normal distribution:

1. Weight of Typical Floor(``w``): mean :math:`\mu_w=2200 \mathrm{kip}` and standard deviation :math:`\sigma_w =200 \mathrm{kip}` (COV = 10%)
2. Story Stiffness(``k``): mean :math:`\mu_k=1600 \mathrm{kip/in}` and standard deviation :math:`\sigma_k =160 \mathrm{kip/in}` (COV = 10%)

.. note::

   #. For the mean values provided the natural period of the structure is **2.27** sec.
   #. The choice of COV percentages is for demonstrative purposes only.

   #. The exercise will use both the MDOF, :numref:`lblMDOFSIM`,  and OpenSees, :numref:`lblOpenSeesSIM`, structural generators. For the OpenSees generator the following model script, :weuq-0001:`Frame9Model.tcl <src/Frame9Model.tcl>`.

   .. literalinclude:: src/Frame9Model.tcl
      :language: tcl

   #. The first lines containing ``pset`` will be read by the application when the file is selected and the application will auto-populate the random variables ``w`` and ``k`` in the **RV** panel with this same variable name. It is of course possible to explicitly use random variables without the ``pset`` command by "RV.**variable name" in the input file. However, no random variables will be auto-populated if user chooses this route.

.. warning::

   Do not place the file in your root, downloads, or desktop folder as when the application runs it will copy the contents on the directories and subdirectories containing this file multiple times (a copy will be made for each sample specified). If you are like us, your root, Downloads or Documents folders contain an awful lot of files and when the backend workflow runs you will slowly find you will run out of disk space!


Sampling Analysis
^^^^^^^^^^^^^^^^^

+----------------+-------------------------------------------------------------------------------------+
| Problem files  | Link : `https://github.com/NHERI-SimCenter/WE-UQ/tree/master/Examples//weuq-0001//` |
+----------------+-------------------------------------------------------------------------------------+


To perform a Sampling or Forward propagation uncertainty analysis the user would perform the following steps:

1. Start the application and the UQ Selection will be highlighted. In the panel for the UQ selection, keep the UQ engine as that selected, i.e. Dakota, and the UQ Method Category as Forward Propagation, and the Forward Propagation method as LHS (Latin Hypercube). Change the #samples to 500 and the seed to 20 as shown in the figure.

.. figure:: figures/9story-UQ.png
   :align: center
   :alt: Screenshot of a software interface with a sidebar menu on the left side including the options UQ, GI, SIM, EVT, FEM, EDP, RV, and RES. The main area on the right displays settings under the header 'UQ Engine' with dropdown menus and input fields, including 'UE Engine' with the option 'Dakota', 'Dakota Method Category' with 'Forward Propagation' selected, 'Method' with 'LHS' selected, a field for '# Samples' with 500 entered, and a field for 'Seed' with 90 entered. The overall theme is light with blue accents.
   :figclass: align-center

#. Next, select the **GI** panel. In this panel, the building properties and units are set. For this example enter **9** for the number of stories, **1400** for building height, and **1600** for building breadth and depth

.. figure:: figures/9story-GI.png
   :align: center
   :alt: Screenshot of a software interface displaying 'Building Information' with editable fields and dropdown menus. Fields shown include 'Name' (blank) and 'Properties' such as the number of stories (9), height (1440), width (1800), depth (1800), and plan area (129600). 'Location' information includes latitude (37.8715) and longitude (-122.273). 'Units' with dropdown selections for 'Force' (Kips), 'Length' (Inches), and 'Temperature' (Celsius) are also visible. The interface has a vertical menu on the left with various options, such as UQ, GI, SIM, EVT, and others, highlighted in a blue and grey color scheme.
   :figclass: align-center

#. Then, select the **SIM** panel from the input panel. This will default in the MDOF model generator. We will use this generator (the NOTE below contains instructions on how to use the OpenSees script instead). In the building information panel, the number of stories should show **9** and the story height **160**. In the building Information box specify **w** for the floor weights and **k** for story stiffness (in both x and y directions). 


.. figure:: figures/9story-SIM1.png
   :align: center
   :alt: Screenshot of a Building Model Generator software interface with a menu bar on the left side, including options like UQ, GI, SIM, EVT, FEM, EDP, RV, and RES. The central part of the interface shows a form under "Building Information" with fields for defining the characteristics of a building such as "Number Stories," "Floor Weights," "Story Stiffness X dirn," and parameters for X and Y directions including yield strength, hardening ratio, and eccentricity. There are also filled tables at the bottom with values for weight, height, and stiffness among others. To the right, a simple visual representation of a Multi Degree Of Freedom (MDOF) structure is displayed with blue squares representing the mass at each story level.
   :figclass: align-center

.. note::

   To specify instead to use the OpenSees script, from the Model Generator pull-down menu select ``OpenSees``. For the fields in the panel presented enter the path to the :weuq-0001:`Frame9Model.tcl <src/Frame9Model.tcl>` script. For both the Centroid  Nodes (those nodes where the floor loads will be applied) and the Response Nodes (those nodes from which the response quantities will be evaluated) as **1 2 3 4 5 6 7 8 9 10** in the panel. The Response nodes will tell the model generator which nodes correspond to nodes at the 4th-floor levels for which responses are to be obtained when using the standard earthquake EDP's.

   .. figure:: figures/9story-SIM2.png
      :align: center
   :alt: Screenshot of a user interface titled "Building Model Generator" for a software application, possibly a structural analysis or engineering software. The interface shows fields for 'Input Script,' 'Centroid Nodes,' 'Response Nodes,' 'Spatial Dimension,' and 'Number of Degrees of Freedom at Nodes,' with some fields pre-populated with example paths and numerical values.
      :figclass: align-center

#. Next, select the **EVT** panel. From the Load Generator pull-down menu select the **Stochastic Wind** option. Leave the exposure condition as **B**. Set the drag coefficient as **1.3** and enter ``gustWS`` for the 3-sec gust wind speed at the 33 ft height.

.. figure:: figures/9story-EVENT.png
   :align: center
   :alt: Screenshot of a "Load Generator" software interface with the Stochastic Wind tab selected, displaying settings for a stochastic loading model based on Wittig & Sinha (1975). The model generates wind speed time histories with options to adjust the Drag Coefficient, select ASCE 7 Exposure Condition, input Gust Wind Speed, and an option to provide a seed value for random number generation.
   :figclass: align-center

#. Next, choose the **FEM** panel. Here we will change the entries to use Rayleigh damping, with the Rayleigh factor chosen using **1** and **6** modes. For the **MDOF** model generator, because it generates a model with two translational and 1 rotational degree-of-freedom in each direction and because we have provided the same **k** values in each translational direction, i.e. we will have duplicate eigenvalues, we specify as shown in the figure modes **1** and **6**.

.. figure:: figures/9story-FEM.png
   :align: center
   :alt: Screenshot of a software interface for finite element analysis application named OpenSees. The interface includes fields and drop-down menus to input parameters such as Analysis, Integration, Algorithm, ConvergenceTest, Solver, and Damping Model. Options like Rayleigh Damping, Newton integration method, and Umfpack solver are selected, among other settings for damping ratio and mode numbers.
   :figclass: align-center

4. We will skip the **EDP** panel leaving it in its default condition, that is to use the **Standard Wind** EDP generator.

5. For the **RV** panel, we will enter the distributions and values for our random variables. Because of the steps we have followed and entries we have made, the panel when we open it should contain the **3** random variables and they should all be set constant. For the w, k and wS random variables we change the distributions to normal and enter the values given for the problem, as shown in the figure below. 

.. figure:: figures/9story-RV.png
   :align: center
   :alt: A software interface displaying a table titled "Input Random Variables" with three rows of data. Each row has fields for 'Variable Name,' 'Distribution,' 'Mean,' and 'Standard Dev.' The variable names are 'w,' 'k,' and 'gustWS,' all with 'Normal' distribution. Mean values are 2200 for 'w,' 1600 for 'k,' and 20 for 'gustWS' with corresponding standard deviations of 220, 160, and 2. There are 'Show PDF' buttons on the right of each row and 'Add' and 'Remove' buttons at the top of the table. On the left side of the screen, there are vertical tabs with abbreviations such as 'UQ,' 'GI,' 'SIM,' 'EVT,' 'FEM,' 'EDP,' 'RV,' and 'RES,' with 'RV' highlighted in blue.
   :figclass: align-center

.. warning::   

   The user cannot leave any of the distributions for these values as constant for the Dakota UQ engine.

5. Next click on the **Run** button. This will cause the backend application to launch Dakota. When done the **RES** panel will be selected and the results will be displayed. The results show the values of the mean and standard deviation. The peak displacement of the roof is the quantity **1-PFD-9-1** (first event (tool to be extended to multiple events), 9th floor (in US ground floor considered 0), and 1 dof direction). the **PFA** quantity defines peak floor acceleration, the **RMSA** quantity is the root mean square of floor accelerations, and the **PID** quantity corresponds to peak inter-story drift.

.. figure:: figures/9story-RES1.png
   :align: center
   :alt: Screenshot of a data summary table displaying statistical metrics for various items. Each row includes a unique identifier (e.g., 1-RMSA-7-1) followed by columns for the Mean, Standard Deviation (StdDev), Skewness, and Kurtosis of the data. There are several groups labeled with acronyms like UQ, GI, SIM, EVT, FEM, EDP, RV, and RES; each group has entries with their respective statistical values listed. The visual style suggests this is from a statistical analysis software or spreadsheet.
   :figclass: align-center

If the user selects the "Data" tab in the results panel, they will be presented with both a graphical plot and a tabular listing of the data. By left- and right-clicking with the mouse in the individual columns the axis changes (left mouse click controls the vertical axis, right mouse clicks the horizontal axis).

.. figure:: figures/9story-RES2.png
   :align: center
   :alt: A scatter plot with a clear trend of increasing y-values with increasing x-values, depicted by numerous blue dots that form a diagonal line from the bottom left to the top right of the graph. X-axis is labeled "1-PFA-1-1" with values ranging from 0.7 to 10.1, and the y-axis is labeled "gustWS" with values ranging from approximately 9.0 to 30.6. Below the plot is a tabular inset showing numerical values under headers: Run #, w, k, gustWS, 1-PFA-1-1, 1-RMSA-1-1, 1-PFD-1-1, 1-PID-1-1, 1-PFA-2-1, and 1-RMSA, with five rows of data visible. The interface has a "Save Data" button, and there are additional tabs and menu items on the left and top that are not active in the image.
   :figclass: align-center

Various views of the graphical display can be obtained by left and right-clicking in the columns of the tabular data. If a singular column of the tabular data is pressed with both right and left buttons a frequency and CDF will be displayed, as shown in the figure below.

.. figure:: figures/9story-RES6.png
   :align: center
   :alt: The image shows two graphs side by side, framed within a user interface with multiple tabs such as 'Summary' and 'Draw Values' highlighted. On the left is a histogram with a range of bins along the x-axis representing some variable (likely a probability value) and the y-axis showing frequency. The bins show a roughly normal distribution. On the right is a cumulative distribution function (CDF) plot, with the same x-axis as the histogram and the y-axis representing cumulative probability, which gradually increases to 1. Below both graphs is a table of numerical data corresponding to the bins and values in the graphs. There are check boxes and other UI elements, indicating this is some form of interactive software for statistical analysis.
   :figclass: align-center

User Defined Output
^^^^^^^^^^^^^^^^^^^

+----------------+--------------------+
| Problem files  | :weuq-0002:`/`     |
+----------------+--------------------+

In this section, we will demonstrate the use of the user-defined output option for the EDP panel. In the previous example, we got the standard output, which can be both a lot and also limited (in the sense you may not get the information you want). In this example we will present how to obtain results just for the roof displacement, the displacement of node **10** in both the **MDOF** and **OpenSees** model generator examples and the shear force at the base of the structure. For the OpenSees model, it is also possible to obtain the overturning moment (something not possible in the MDOF model due to the fact it is modeled using spring elements). The examples could be extended to output for example the element end rotations, plastic rotations, ...

For this example, you will need two additional files, :weuq-0002:`FrameRecorder.tcl <src/FrameRecorder.tcl>`. 

The recorder script as shown will record the envelope displacements and RMS accelerations in the first two degrees of freedom for the nodes in the modes. The script will also record the element forces. The file is as shown below:

.. literalinclude:: ../weuq-0002/src/FrameRecorder.tcl
   :language: tcl

The ``FramePost.tcl`` script shown below will accept as input any of the 10 nodes in the domain and for each of the two DOF directions and element forces.

.. literalinclude:: ../weuq-0002/src/FramePost.tcl
   :language: tcl

.. note::

   The user has the option when using the OpenSees SIM application to provide no post-process script (in which case the main script must create a ``results.out`` file containing a single line with as many space-separated numbers as QoI) or the user may provide a Python script that also performs the postprocessing. An example of a postprocessing Python script is :weuq-0002:`FramePost.py <src/FramePost.py>`. The Python script at present only responds to nodal displacements.

   .. literalinclude:: ../weuq-0002/src/FramePost.py
      :language: python

The steps are the same as the previous example, with the exception of step 4 defining the **EDP**.
   
#. For the **EDP** panel, we will change the generator to **User Defined**. In the panel that presents itself, the user must provide the paths to both the recorder commands and the postprocessing script. Next, the user must provide information on the response parameters they are interested in. The user presses the **Add** button and the entries ``Disp_10_1``, ``RMSA_10_1``, and ``Force_1_1`` in the entry field as shown in the figure below.

   .. figure:: figures/9story-EDP-USER.png
      :align: center
   :alt: Screenshot of a software interface with options for "Engineering Demand Parameters" set to "User Defined." There are fields for 'Additional Input' and 'Processing Script,' both showing file paths, and each with a 'Choose' button. Below, under "Response Parameters," there are three listed items with names: "Disp_10_1," "RMSA_10_1," and "Force_1_1," each with an option to 'Remove.' On the left, there is a vertical navigation bar with various abbreviations like "UQ," "GI," "SIM," and others highlighted in blue. The overall theme suggests a settings or configuration screen within an engineering or scientific software application.
      :figclass: align-center


#. Next, click on the **Run** button. This will cause the backend application to launch Dakota. When done the **RES** panel will be selected and the results will be displayed. The results show the values of the mean and standard deviation as before but now only for the one quantity of interest.

   .. figure:: figures/9story-RES-USER.png
      :align: center
   :alt: A screenshot of a statistical analysis software interface displaying a summary table with statistical metrics for three different data sets. The left-hand column lists categories such as UQ, GI, SIM, EVT, FEM, EDP, RV, and RES, with SIM and EVT expanded to show items named "RMSA_10_1" and "Force_1_1" respectively. Each item is associated with a table showing the mean, standard deviation (StdDev), skewness, and kurtosis values. "RMSA_10_1" has a mean of approximately 1.1725, and "Force_1_1" has a mean of approximately 128.146. Above the table, tabs labeled "Summary" and "Data Values" are visible, with "Summary" being the active selection. The interface has a simple design with a white background and table elements.
      :figclass: align-center


Reliability Analysis
^^^^^^^^^^^^^^^^^^^^

+----------------+-----------------------+
| Problem files  | :weuq-0003:`/`        |
+----------------+-----------------------+

If the user is interested in the probability that certain response measures will be exceeded an alternative strategy is to perform a reliability analysis. To perform a reliability analysis the steps above would be repeated with the exception that the user would select a reliability analysis method instead of a Forward Propagation method. To obtain reliability results using the Global Reliability methods presented in Dakota choose the **Global Reliability** methods from the methods drop-down menu. In the response levels enter a value of **0.5** and **0.8**, specifying that we are interested in the value of the CDF for a displacement of the roof of 0.5in and 0.8in, i.e. what is the probability that displacement will be less than 0.8in.

.. figure:: figures/9story-UQ-Reliability.png
   :align: center
   :alt: Screenshot of a user interface for an UQ (uncertainty quantification) engine with settings for a reliability analysis. The interface includes dropdown menus and fields labeled: UQ Engine (set to 'Dakota'), Dakota Method Category (set to 'Reliability Analysis'), Reliability Method (set to 'Global Reliability'), GP Approximation (set to 'x_gaussian_process'), Response Levels (with values '0.5 0.8'), and Seed (value '377'). On the left, a vertical menu shows categories like UQ, GI, SIM, EVT, FEM, EDP, RV, and RES.
   :figclass: align-center

After the user fills in the rest of the tabs as per the previous section, the user would then press the **RUN** button. The application (after spinning for a while with the wheel of death) will present the user with the results, which as shown below, indicate that the probabilities as **52%** and **92%**.

.. figure:: figures/9story-RES-Reliability.png
   :align: center
   :alt: The image displays a simple line graph with a horizontal axis labeled "Disp_10_1" ranging from 0.50 to 0.80 and a vertical axis labeled "Probability" ranging from 0.00 to 1.00. The graph includes a single upward-sloping green line, starting at a probability of approximately 0.50 at the lowest horizontal axis value and increasing to a probability of 1.00 at the highest horizontal axis value. Two data points are indicated by blue markers on the line. Below the graph, there's a table with two rows displaying numerical values corresponding to the points on the graph, with values approximately 0.526 for the first point and 0.917 for the second point.
   :figclass: align-center

.. warning::

   Reliability analysis can only be performed when there is only one EDP.

.. figure:: figures/9story-EDP-Reliability.png
   :align: center
   :alt: A screenshot of a software interface with various input fields under the heading "Engineering Demand Parameters" set to "User Defined". There are fields for "Additional Input" and "Processing Script" with file paths provided. A section titled "Response Parameters" shows a field with the name "Disp_10_1" entered, and buttons labeled "Add" and "Remove". On the left side, a vertical navigation bar shows items like UQ, GI, SIM, EVT, FEM, EDP, RV, and RES highlighted, indicating different sections of the software.
   :figclass: align-center

