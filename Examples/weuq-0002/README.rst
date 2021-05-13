.. _weuq-0002:

9-Story Frame - Sampling with Select EDPs
=========================================

+----------------+--------------------+
| Source files   | :weuq-0002:`/`     |
+----------------+--------------------+

In this example demonstrates the use of the user defined output option for the EDP panel. In the previous example, :ref:`weuq-0001`, we got the standard output, which can be both a lot and also limited (in sense you may not get the information you want). In this example we will present how to obtain results just for the roof displacement, the displacement of node **10** in both the **MDOF** and **OpenSees** model generator examples and shear force at the base of the structure. For the OpenSees model, it is also possible to obtain the overturning moment (something not possible in MDOF model due to fact it is modelled using spring elements). The examples could be extended to output for example the element end rotations, plastic rotations, ...

For this example you will need two additional files, :weuq-0002:`FrameRecorder.tcl <src/FrameRecorder.tcl>`. 

The recorder script as shown will record the envelope displacements and RMS accelerations in the first two degrees-of-freedom for the nodes in the modes. The script will also record the element forces. The file is as shown below:

.. literalinclude:: src/FrameRecorder.tcl
   :language: tcl

The ``FramePost.tcl`` script shown below will accept as input any of the 10 nodes in the domain and for each of the two DOF directions and element forces.

.. literalinclude:: src/FramePost.tcl
   :language: tcl

.. note::

   The user has the option when using the OpenSees SIM application to provide no post-process script (in which case the main script must create a ``results.out`` file containing a single line with as many space separated numbers as QoI) or the user may provide a Python script that also performs the postprocessing. An example of a postprocessing Python script is :weuq-0002:`FramePost.py <src/FramePost.py>`. The Python script at present only responds to nodal displacements.

   .. literalinclude:: src/FramePost.py
      :language: python

The steps are the same as the previous example, with exception of step 4 defining the **EDP**.

#. For the **EDP** panel, we will change the generator to **User Defined**. In the panel that presents itself the user must provide the paths to both the recorder commands and the postprocessing script. Next the user must provide information on the response parameters they are interested in. The user presses the **Add** button and the enters ``Disp_10_1``, ``RMSA_10_1``, and ``Force_1_1`` in the entry field as shown in figure below.

   .. figure:: figures/9story-EDP-USER.png
      :align: center
      :figclass: align-center


#. Next click on the **Run** button. This will cause the backend application to launch dakota. When done the **RES** panel will be selected and the results will be displayed. The results show the values the mean and standard deviation as before but now only for the one quantity of interest.

   .. figure:: figures/9story-RES-USER.png
      :align: center
      :figclass: align-center

