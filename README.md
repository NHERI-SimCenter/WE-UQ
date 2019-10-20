

# WE-UQ

[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.3464692.svg)](https://doi.org/10.5281/zenodo.3464692)

### Wind Engineering with Uncertainty Quantification ###

This open-source research application provides an application that can be used to predict the response of a building subjected to wind events. The application is focused on quantifying the uncertainties in the predicted response, given the that the properties of the buildings and the wind events are not known exactly, and that both the simulation software and the user make simplifying assumptions in the numerical modeling of that structure. In this application, the user is required to characterize the uncertainties in the input. The application will, after utilizing the users selected sampling method, provide information that characterizes the uncertainties in the computed response measures. As the computations to make these determinations can be prohibitively expensive to perform on a user's local computer, the user has the option to perform the computations remotely on HPC resources located at the Texas Advanced Computing Center (TACC) and made available to the user through NHERI DesignSafe, the cyberinfrastructure provider for the distributed NSF funded Natural Hazards in Engineering Research Infrastructure (NHERI) facility.

Please visit the [WE-UQ Research Tool webpage](https://simcenter.designsafe-ci.org/research-tools/we-uq/)
for more resources related to this tool. Additionally, this page
provides more information on the NHERI SimCenter, including other SimCenter
applications, FAQ, and how to collaborate.


### How to Build

##### 1. Download this repo.

##### 2. Download the SimCenterCommon repo: https://github.com/NHERI-SimCenter/SimCenterCommon

Place the SimCenterCommon Repo in the same directory that you placed the WE-UQ repo.

##### 3. Install Qt: https://www.qt.io/

Qt is free for open source developers. Download it and start the Qt Creator application. From Qt Creator open the WE-UQ.pro file located in the directory the WE-UQ repo was downloaded into and select build to build it. For detailed instructions on using Qt, browse their website.

#### 4. To run locally you will need to install and build the SimCenterBackendApplications Repo https://github.com/NHERI-SimCenter/SimCenterBackendApplications

SimCenterBackendApplications contains a number of applications written in C++, C and Python. Follow the build instructions on SimCenterBackendApplications githib page to build them. Once built inside the WE-UQ applicationss preferences set the applications directory entry to point to the applications folder that the build process creates.

### Acknowledgement

This material is based upon work supported by the National Science Foundation under Grant No. 1612843.
