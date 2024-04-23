.. _weuq-0016:

Wind Tunnel-Informed Stochastic Wind Pressure Generation
=======================================================================

+----------------+----------------------------------------+
| Problem files  | :github:`Download <Examples/weuq-0016/>` |
+----------------+----------------------------------------+

This example estimates the probabilistic response of a building model excited by wind tunnel-informed stochastic wind **pressure time histories**. This example uses the experimental data obtained at the University of Florida (UF) NHERI Experimental Facility (EF), and applies the simulated wind loads to a 25-story rectangular shaped building model for response simulation. This example uses the data presented in [Duarte2023]_.

.. [Duarte2023] Duarte, T.G., Arunachalam, S., Subgranon, A. and Spence, S.M., (2023). Uncertainty Quantification and Simulation of Wind-Tunnel-Informed Stochastic Wind Loads. Wind, 3(3), pp.375-393.


Preparation of "Wind Pressure Time History File"
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
The experimental records should first be reformatted by the user such that **wind force time histories recorded at each floor in x-, y-, and z- directions (Fx, Fy, and Tz, respectively)** can be directly imported in WE-UQ in the EVT tab. The dataset can be imported as either a MATLAB binary file or a json file. Additionally, the model-scale dimensions and additional information on the experiment should be provided. Please refer to :ref:`the User Manual<lblExperimentalWindForce>` for the list of variables the file should contain. In this example, the variables in the table below are imported as a json file. Note that the x- and y- directional forces and z-directional moments at each of the 25 stories are recorded for 32 sec with dt=0.0016 sec (20,000 time points). The reference wind speed (Vref) was measured at top of the building model.

