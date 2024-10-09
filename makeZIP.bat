
REM ## make the exe .. it assumes EE-UUQ_Windows_Download exists with vcredist and openssl there

cd build
conan install .. --build missing
qmake ..\WE-UQ.pro
nmake

REM ## copy application into folder and winddeployqt it

copy .\Release\WE_UQ.exe .\WE-UQ_Windows_Download
cd WE-UQ_Windows_Download
windeployqt WE_UQ.exe

REM ## copy examples

rmdir /s /q .\Examples
mkdir .\Examples
xcopy /s /e /Q ..\..\Examples .\Examples

REM ## delete applications folder and copy new stuff

rmdir /s /q .\applications
mkdir .\applications
mkdir .\applications\common
mkdir .\applications\createEDP
mkdir .\applications\createEVENT
mkdir .\applications\createSAM
mkdir .\applications\performSIMULATION
mkdir .\applications\performUQ
mkdir .\applications\Workflow
mkdir .\applications\opensees
mkdir .\applications\dakota
mkdir .\applications\python

xcopy /s /e ..\..\..\SimCenterBackendApplications\applications\common  .\applications\common
xcopy /s /e ..\..\..\SimCenterBackendApplications\applications\createEDP  .\applications\createEDP
xcopy /s /e ..\..\..\SimCenterBackendApplications\applications\createEVENT  .\applications\createEVENT
xcopy /s /e ..\..\..\SimCenterBackendApplications\applications\createSAM  .\applications\createSAM
xcopy /s /e ..\..\..\SimCenterBackendApplications\applications\performSIMULATION  .\applications\performSIMULATION
xcopy /s /e ..\..\..\SimCenterBackendApplications\applications\performUQ  .\applications\performUQ
xcopy /s /e ..\..\..\SimCenterBackendApplications\applications\Workflow  .\applications\Workflow
xcopy /s /e ..\..\..\SimCenterBackendApplications\applications\OpenSees  .\applications\opensees
xcopy /s /e ..\..\..\SimCenterBackendApplications\applications\dakota  .\applications\dakota
xcopy /s /e /Q ..\..\..\SimCenterBackendApplications\applications\python  .\applications\python

rmdir /s /q .\applications\createEVENT\GeoClawOpenFOAM
rmdir /s /q .\applications\createEVENT\M9
rmdir /s /q .\applications\createEVENT\multiplePEER
rmdir /s /q .\applications\createEVENT\siteResponse
rmdir /s /q .\applications\createEVENT\MPM
rmdir /s /q .\applications\createEVENT\pointWindSpeed
rmdir /s /q .\applications\createEVENT\stochasticGroundMotion
rmdir /s /q .\applications\createEVENT\uniformPEER


REM ## zip it up with 7zip

set sevenzip_path="C:\Program Files\7-Zip\7z.exe"
cd ..
if exist .\WE-UQ_Windows_Download.zip (
    del .\WE-UQ_Windows_Download.zip
    echo File deleted.
)
%sevenzip_path% a -tzip .\WE-UQ_WindowsDownload.zip  .\WE-UQ_Windows_Download



