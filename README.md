

# WE-UQ 
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.3464692.svg)](https://doi.org/10.5281/zenodo.3464692)

|Build OS|Windows|MacOS|Linux|
|---|---|---|---|
|Status|[![Build Status](https://el7addad.visualstudio.com/NHERI%20SimCenter/_apis/build/status/el7addad.WE-UQ?branchName=master&jobName=Job&configuration=Job%20windows)](https://el7addad.visualstudio.com/NHERI%20SimCenter/_build/latest?definitionId=4&branchName=master)|[![Build Status](https://el7addad.visualstudio.com/NHERI%20SimCenter/_apis/build/status/el7addad.WE-UQ?branchName=master&jobName=Job&configuration=Job%20macHighSierra)](https://el7addad.visualstudio.com/NHERI%20SimCenter/_build/latest?definitionId=4&branchName=master)|[![Build Status](https://el7addad.visualstudio.com/NHERI%20SimCenter/_apis/build/status/el7addad.WE-UQ?branchName=master&jobName=Job&configuration=Job%20linux)](https://el7addad.visualstudio.com/NHERI%20SimCenter/_build/latest?definitionId=4&branchName=master)|




### Wind Engineering with Uncertainty Quantification ###

This open-source research application provides an application that can be used to predict the response of a building subjected to wind events. The application is focused on quantifying the uncertainties in the predicted response, given the that the properties of the buildings and the wind events are not known exactly, and that both the simulation software and the user make simplifying assumptions in the numerical modeling of that structure. In this application, the user is required to characterize the uncertainties in the input. The application will, after utilizing the users selected sampling method, provide information that characterizes the uncertainties in the computed response measures. As the computations to make these determinations can be prohibitively expensive to perform on a user's local computer, the user has the option to perform the computations remotely on HPC resources located at the Texas Advanced Computing Center (TACC) and made available to the user through NHERI DesignSafe, the cyberinfrastructure provider for the distributed NSF funded Natural Hazards in Engineering Research Infrastructure (NHERI) facility.

Please visit the [WE-UQ Research Tool webpage](https://simcenter.designsafe-ci.org/research-tools/we-uq/)
for more resources related to this tool. Additionally, this page
provides more information on the NHERI SimCenter, including other SimCenter
applications, FAQ, and how to collaborate.

### How to build using Conan package manager
WE-UQ requires two dependencies to be able to build it and run it, SimCenterCommon and SimCenterBackendApplications. The provided Conan recipe ```conanfile.py```, will help to obtain these dependencies automatically while building WE-UQ. The following are the requirements to build WE-UQ:

1. **Qt Framework version 5.10 or higher**: Qt 5.12 is recommended because it is a long term support (LTS) release. You can download and install Qt for you platform and compiler from the official [website](https://www.qt.io/). You can optionally choose to obtain Qt using Conan as will be shown later.
2. **C++11 compliant compiler**: It is recommended to use Visual C++ 2015 or later on Windows, Clang 10.0 or later on MacOS and GCC 5 or later on Linux.
3. **Conan Package Manager**: Conan package manager is required to automatically handle the dependencies of WE-UQ. The easiest way to install Conan is using the command ```pip install conan```. For more information check Conan [website](https://conan.io/) and [online documentation](https://docs.conan.io/en/latest/introduction.html).

The following are the requirements to run WE-UQ:
1. **OpenSees**: version 3.0 or newer.
2. **DAKOTA**: version 6.5 or newer.
3. **Python**: version 3.6 or newer. Python 2 is supported, but the support might be dropped in the future as it is being deprecated in 2020.
4. **Perl**: ActivateState or Strawberry Perl on Windows, or the built-in perl on Mac or Linux.

#### Set up Conan remotes (One time setup)
This is necessary as a one time setup for Conan to make sure it is configured to use the right remotes. Before we start this process you can check the current remotes used by Conan using the following command:
```
conan remote list
```
This will print the remotes to the screen, you need to make sure Conan is set up to use both the Bincrafters and the NHERI SimCenter repositories. By default, these remotes are not configured, but they can be added using the following commands:

```
conan remote add simcenter https://api.bintray.com/conan/nheri-simcenter/simcenter --insert
```
This will add the NHERI SimCenter Conan repository as the first entry in the remotes list. Bincrafters repository can be added as follows:
```
$ conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
```

#### Steps to build WE-UQ from the terminal
1. Create an output folder for the build process. This is important to have an out-of-source build, also called shadow build. It is useful to do so to avoid mixing compiled code with the source code and this will keep the source directory clean. It is common to call this folder build and it can be created as a subfolder of the WE-UQ folder as follows:
```
mkdir build
cd build
```
2. Set up Conan remotes as shown above, this needs to be done only once (i.e. not every time you build WE-UQ).

3. Run QMake using the WE-UQ project, as follows:
```
qmake ../WE-UQ.pro
```
if you want to build  for debugging, you can use the following command:
```
qmake ../WE-UQ.pro CONFIG+=Debug
```

This step will first run Conan to install the dependencies and then will generate Makefiles to build WE-UQ. The first time to run this it might take a longer time, specially if this triggered some dependencies to be built form the source. Subsequent runs should be much faster.

4. Build using the generated Makefiles. This can be done on Linux and Mac by runnning ```make``` and on Windows by runnning ```nmake```. Alternatively, if you want to build in parallel to speed up the process, you can use ```make -j``` on Linux\Mac or ```jom``` on Windows.

#### Steps to build WE-UQ using Qt Creator IDE
1. Open WE-UQ.pro project in Qt Creator.
2. Choose and configure the build kit using the prompt. This allows you to choose which compiler version you use, the architecture and build type (e.g. Release).
3. Run QMake by right clicking on the project (or using the Build menu) and choosing to run QMake. This step may take a longer time the first time to run it while it is installing the dependencies. To see the output and progress of this step, check the compile output pane in Qt Creator.
4. Build WE-UQ using the build tool at the bottom left corner or using the build menu.
5. Run WE-UQ using the run tool at the bottom left corner or using the build menu.

#### Troubleshooting Conan dependencies
Although, the QMake project is set up to automatically call Conan, sometimes it is useful to install the Conan dependencies manually to troubleshoot the process if a problem happens. To do so, we can use the command from the terminal in the build directory:
```
conan install ..
```
This will install dependencies for the default Conan profile (e.g. default compiler, x64 and release build).

#### Automated build using Azure pipelines
This project is set up to automatically build using Azure pipelines. Changes in the master branch or release branches will trigger the build, in addition to pull requests. All the builds are acessible through this [link](https://el7addad.visualstudio.com/NHERI%20SimCenter/_build?definitionId=4&_a=summary). In addition, build artifacts are published with the release packages of WE-UQ for both Windows and Mac and are available through the build status page.

### How to Build using the legacy QMake project
This procedure is currently dated, but it is kept to allow some time to transition to building using Conan for dependency management.

##### 1. Download this repo.

##### 2. Download the SimCenterCommon repo: https://github.com/NHERI-SimCenter/SimCenterCommon

Place the SimCenterCommon Repo in the same directory that you placed the WE-UQ repo.

##### 3. Install Qt: https://www.qt.io/

Qt is free for open source developers. Download it and start the Qt Creator application. From Qt Creator open the WE-UQ-legacy.pro file located in the directory the WE-UQ repo was downloaded into and select build to build it. For detailed instructions on using Qt, browse their website.

#### 4. To run locally you will need to install and build the SimCenterBackendApplications Repo https://github.com/NHERI-SimCenter/SimCenterBackendApplications

SimCenterBackendApplications contains a number of applications written in C++, C and Python. Follow the build instructions on the SimCenterBackendApplications githib page to build them. Once built inside the WE-UQ applicationss preferences set the applications directory entry to point to the applications folder that the build process creates.

### Acknowledgement

This material is based upon work supported by the National Science Foundation under Grant No. 1612843.
