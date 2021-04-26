.. _weuq-0007:

3 Story Building: Sensitivity Analysis
======================================

Consider the problem of uncertainty quantification in a three story steel building. The building being modelled is the 3 story Boston building presented in the FEMA-355C report. From the description in Appendix B of the FEMA document the building is a rectangular building **120** ft by **180** ft with a height above ground of **39** ft. The building weight is approx. **6,500** kips. Eigenvalues are shown in Table 5.1. of the FEMA document to be between **1.6** sec and **1.9** sec depending on connection detals. For this example (and for demonstrative purposes only), this building will be modelled as a shear building with **10** nodes and **9** elements, as shown in following figure. For loading, the TPU Low Rise Wind Generation tool will be used with the mean wind speed being treated as a random variable with a normal distribtion described by a mean :math:`\mu_{gustWS}=20 \mathrm{mph}` and standard deviation :math:`\sigma_{gustWS} =3 \mathrm{mph}` (COV = 15%). 

.. figure:: figures/3story.png
   :align: center
   :height: 600
   :figclass: align-center

   Three Story Downtown Boston Building from FEMA-355C

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
   
   1. The first lines containing ``pset`` will be read by the application when the file is selected and the application will autopopulate the Random Variables ``w`` and ``k`` in the **RV** tab with this same variable name. It is of course possible to explicitly use Random Variables without the ``pset`` command by "RV.**variable name" in the input file. However, no random variables will be autopopulated if user chooses this route.

.. warning::

   Do not place the file in your root, downloads, or desktop folder as when the application runs it will copy the contents on the directories and subdirectories containing this file multiple times (a copy will be made for each sample specified). If you are like us, your root, Downloads or Documents folders contains and awful lot of files and when the backend workflow runs you will slowly find you will run out of disk space!


Global Sensitivity Analysis
^^^^^^^^^^^^^^^^^^^^^^^^^^^

In a global sensitivity analysis the user is wishing to understand what is the influence of the individual random variables on the quantities of interest. This is typically done before the user launches large scale forward uncertainty problems in order to limit the number of random variables used so as to limit the number of simulations performed.

For this problem we will limit the response qunataties of interest to the following six quantaties. Peak Roof displacement in 1 and 2 directions, root mean square (RMS) accelerations in 1 and 2 directions, Peak BAse shear and moments in 1 and 2 directions. In order to do this we will introduce User defined EPD quantaties, which necessitates the followng two files: `recorder.tcl <https://github.com/NHERI-SimCenter/WE-UQ/blob/master/examples/3Story/recorder.tcl>`_ and `postprocess.tcl <https://github.com/NHERI-SimCenter/WE-UQ/blob/master/examples/3Story/postprocess.tcl>`_.  These files are the same as those presented in :ref:`lblWE-Example9`


To perform a Global Sensitivity  analysis the user would perform the following steps:


#. Start the application and the UQ Selection will be highlighted. In the panel for the UQ selection, keep the UQ engine as that selected, i.e. Dakota. From the UQ Method Category drop down menu select Sensitivity Analysis, Keeping the method as LHS (Latin Hypercube). Change the #samples to 500 and the seed to 20 as shown in the figure.

    .. figure:: figures/weuq-0007-UQ.png
       :align: center
       :figclass: align-center

#. Next select the **GI** panel. In this panel the building properties and units are set. For this example enter **3** for the number of stories, **468** for building height, **1440** fow building width, and **2160** for building depth.

    .. figure:: figures/weuq-0007-GI.png
       :align: center
       :figclass: align-center

#. Next select the **SIM** tab from the input panel. This will default in the MDOF model generator. Using the drop down menu change the generator to the OpenSees option. Specify the path the the story.tcl file, and for the two line edits for centroid nodes and response node enter **1 2 3 4** (which identifies the 4 column nodes as having an id of **1**, **2**, **3**, and **4**. For the spatial dimension enter **3** (3d model) and for number of dof enter **6** as shown in figure:

   .. figure:: figures/weuq-0007-SIM.png
      :align: center
      :figclass: align-center

#. Next select the **EVT** panel. From the Load Generator pull down menu select the **Low Rise TPU** option. Leave the exposure condition as **B**. Set the drag coefficient as **1.3** and enter ``gustWS`` for the 3 sec gust wind speed at the 33 ft height.

   .. figure:: figures/weuq-0007-EVT.png
      :align: center
      :figclass: align-center

#. Next choose the **FEM** panel. Here we will change the entries to use Rayleigh damping, with rayleigh factor chosen using **1** and **6** modes. For the **MDOF** model generator, because it generates a model with two translational and 1 rotational degree-of-freedom in each direction and because we have provided the same **k** values in each translational direction, i.e. we will have duplicate eigenvalues, we specify as shown in the figure modes **1** and **6**.

    .. figure:: figures/weuq-0007-FEM.png
       :align: center
       :figclass: align-center

#. We will skip the **EDP** panel leaving it in it's default condition, that being to use the **Standard Wind** EDP generator.

#. For the **RV** panel, we will enter the distributions and values for our random variables. Because of the steps we have followed and entries we have made, the panel when we open it should contain the **3** random variables and they should all be set constant. For the w, k and wS random variables we change the distributions to normal and enter the values given for the problem, as shown in figure below. 

   .. figure:: figures/9story-RV.png
      :align: center
      :figclass: align-center

.. warning::   

   The user cannot leave any of the distributions for these values as constant for the Dakota UQ engine.

#. Next click on the 'Run' button. This will cause the backend application to launch dakota. When done the **RES** tab will be selected and the results will be displayed. The results show the values the mean and standard deviation. The peak displacement of the roof, is the quantity **1-PFD-9-1** (first event (tool to be extended to multiple events), 9th floor (in US ground floor considered 0), and 1 dof direction). the **PFA** quantity defines peak floor acceleration, the **RMSA** quantity the root mean square of floor accelerations, and the **PID** quantity corresponds to peak interstory drift.

   .. figure:: figures/9story-RES1.png
      :align: center
      :figclass: align-center

If the user selects the "Data" tab in the results panel, they will be presented with both a graphical plot and a tabular listing of the data. By left- and right-clicking with the mouse in the individual columns the axis change (left mouse click controls vertical axis, right mouse click the horizontal axis).

.. figure:: figures/9story-RES2.png
   :align: center
   :figclass: align-center

Various views of the graphical display can be obtained by left and right clicking in the columns of the tabular data. If a singular column of the tabular data is pressed with both right and left buttons a frequency and CDF will be displayed, as shown in figure below.

.. figure:: figures/9story-RES6.png
   :align: center
   :figclass: align-center

User Defined Output
^^^^^^^^^^^^^^^^^^^

In this section we will demonstrate the use of the user defined output option for the EDP panel. In the previous example we got the standard output, which can be both a lot and also limited (in sense you may not get the information you want). In this example we will present how to obtain results just for the roof displacement, the displacement of node **10** in both the **MDOF** and **OpenSees** model generator examples and shear force at the base of the structure. For the OpenSees model, it is also possible to obtain the overturning moment (something not possible in MDOF model due to fact it is modelled using spring elements). The examples could be extended to output for example the element end rotations, plastic rotations, ...

For this example you will need two additional file `recorder.tcl <https://github.com/NHERI-SimCenter/WE-UQ/blob/master/Examples/9story/recorder.tcl>`_ and `postprocess.tcl <https://github.com/NHERI-SimCenter/WE-UQ/blob/master/examples/9story/postprocess.tcl>`_. 

The recorder script as shown will record the envelope displacements and RMS acceleratiuons in the first two degrees-of-freedom for the nodes in the modes. The script will also record the element forces. The file is as shown below:

.. literalinclude:: recorder.tcl
   :language: tcl

The ``postprocess.tcl`` script shown below will accept as input any of the 10 nodes *in the domain and for each of the two dof directions and element forces.

.. literalinclude:: postprocess.tcl
   :language: tcl

.. note::

   The user has the option when using the OpenSees SIM application to provide no postprocess script (in which case the main script must create a ``results.out`` file containing a single line with as many space separated numbers as QoI or the user may provide a Python script that also performs the postprocessing. An example of a postprocessing Python script is `postprocess.py <https://github.com/NHERI-SimCenter/WE-UQ/blob/master/examples/9story/postprocess.py>`_. The python script at present only respods to nodal displacements.

   .. literalinclude:: postprocess.py
      :language: python

The steps are the same as the previous example, with exception of step 4 defining the **EDP**.

#. For the **EDP** panel, we will change the generator to **User Defined**. In the panel that presents itself the user must provide the paths to both the recorder commands and the postprocessing script. Next the user must provide information on the response parameters they are interested in. The user presses the **Add** button and the enters ``Disp_10_1``, ``RMSA_10_1``, and ``Force_1_1`` in the entry field as shown in figure below.

   .. figure:: figures/9story-EDP-USER.png
      :align: center
      :figclass: align-center


#. Next click on the 'Run' button. This will cause the backend application to launch dakota. When done the **RES** tab will be selected and the results will be displayed. The results show the values the mean and standard deviation as before but now only for the one quantity of interest.

   .. figure:: figures/9story-RES-USER.png
      :align: center
      :figclass: align-center


Reliability Analysis
^^^^^^^^^^^^^^^^^^^^

If the user is interested in the probability that certain response measure will be exceeded an alternative strategy is to perform a reliability analysis. To perform a reliability analysis the steps above would be repeated with the exception that the user would select a reliability analysis method instead of a Forward Propagation method. To obtain reliability results using the Global Reliability methose presented in Dakota choose the **Global Reliability** methods from the methods drop down menu. In the response levels enter a values of **0.5** and **0.8**, specifying that we are interested in the value of the CDF for a displacement of the roof of 0.5in and 0.8in, i.e. what is probability that displacement will be less than 0.8in.


.. figure:: figures/9story-UQ-Reliability.png
   :align: center
   :figclass: align-center

After the user fills in the rest of the tabs as per the previous section, the user would then press the **RUN** button. The application (after spinning for a while with the wheel of death) will present the user with the results, which as shown below, indicate that the probabilities as **52%** and **92%**.

.. figure:: figures/9story-RES-Reliability.png
   :align: center
   :figclass: align-center

.. warning::

   Reliability analysis can only be performed when their is only one EDP.

   .. figure:: figures/9story-EDP-Reliability.png
      :align: center
      :figclass: align-center

