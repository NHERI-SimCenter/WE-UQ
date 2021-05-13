.. _weuq-0003:

Reliability Analysis
====================

If the user is interested in the probability that certain response measure will be exceeded an alternative strategy is to perform a reliability analysis. To perform a reliability analysis the steps above would be repeated with the exception that the user would select a reliability analysis method instead of a Forward Propagation method. To obtain reliability results using the Global Reliability methods presented in Dakota choose the **Global Reliability** methods from the methods drop down menu. In the response levels enter a values of **0.5** and **0.8**, specifying that we are interested in the value of the CDF for a displacement of the roof of 0.5in and 0.8in, i.e. what is probability that displacement will be less than 0.8in.


.. figure:: figures/9story-UQ-Reliability.png
   :align: center
   :figclass: align-center

After the user fills in the rest of the tabs as per the previous section, the user would then press the **RUN** button. The application (after spinning for a while with the wheel of death) will present the user with the results, which as shown below, indicate that the probabilities as **52%** and **92%**.

.. figure:: figures/9story-RES-Reliability.png
   :align: center
   :figclass: align-center

.. warning::

   Reliability analysis can only be performed when there is only one EDP.

   .. figure:: figures/9story-EDP-Reliability.png
      :align: center
      :figclass: align-center

