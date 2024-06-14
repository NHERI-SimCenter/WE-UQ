
.. _eeuq-0000:


A Test of Different Random Variable Types, Multiple Analysis and Multiple Model Options
=======================================================================================

This example is for demo purposes. It's intent is to show the options available in the use of random variables,
multiple models, and multiple analysis options. 

The building being modeled is the LA Post-Northridge 3 story North-South no cover plates, no column splice
building from the  FEMA-355C/Sept. 2000  report. (https://www.nehrp.gov/pdf/fema355c.pdf).


Two structural models are used: 1) a 2d nonlinear beam column model of the moment frame, 2) an elastic stick model of same frame. Two analysis models are used to explore effect of model and rayleigh damping on the results.

For the moment frame and stick model, the columns of each story are provided a unique youngs modululs. All beams have same modulus. For the columns each story has same yield and hardening ratios (OpenSees Steel01 material is used).

.. figure:: figures/LA_3Story.png
   :align: center
   :alt: "Architectural engineering drawings of a 3-story building showing plans and elevation. The plan view illustrates a structure with a grid of 6 bays by 4 bays, each 30 feet square, with one area shaded to represent a specific section. The elevation depicts a side view with multiple levels and supporting columns indicated. To the right, there is another drawing labeled '3-STORY MOMENT FRAMES' with a similar grid arrangement marked with the letters A to E on top. Below the drawings, there are tables detailing the different structural elements for each storey, such as exterior and interior columns (W14X257 for exterior and W14X311 for interior), doubler plates, girders, and beams, with specific sizes and designations like W30X116 and W16X26."
   :width: 400
   :figclass: align-center

	      
   LA 3-Story Post Northridge Building FEMA 355-c Building.

