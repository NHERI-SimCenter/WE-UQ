# remove & rebuild app and macdeploy it

appName="WE-UQ"
appFile="WE_UQ.app"
dmgFile="WE-UQ_Mac_Download.dmg"

QTDIR="/Users/simcenter/Qt/5.15.2/clang_64/"

pathToBackendApps="/Users/fmckenna/release/SimCenterBackendApplications"
pathToOpenSees="/Users/fmckenna/bin/OpenSees3.2.2"
pathToDakota="/Users/fmckenna/dakota-6.12.0"

#pathToPython="/Users/fmckenna/PythonEnvR2D"

#
# build it
#

./makeEXE.sh
cd build

#
# Check to see if the app built
#

if ! [ -x "$(command -v open $pathApp)" ]; then
	echo "$appFile did not build. Exiting."
	exit 
fi

#
# macdeployqt it
#

macdeployqt ./WE_UQ.app 

#
# add missing files from macdeployqt (a known bug)
#

mkdir -p ./$appFile/Contents/plugins/renderers/
cp -R $QTDIR/plugins/renderers/libopenglrenderer.dylib ./$appFile/Contents/plugins/renderers/

mkdir -p ./$appFile/Contents/plugins/renderplugins/
cp -R $QTDIR/plugins/renderplugins/libscene2d.dylib ./$appFile/Contents/plugins/renderplugins/

echo "cp -R $QTDIR/plugins/renderplugins/libscene2d.dylib ./$appFile/Contents/plugins/renderplugins/"


#
# copy test case
#
cp -fr ../tests/IsolateBuildingCFD  ./$appFile/Contents/MacOS


#
# copy files from VTK dir
#

cp ../../VTK/lib/libvtkChartsCore-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkCommonColor-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkCommonComputationalGeometry-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkCommonCore-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkCommonDataModel-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkCommonExecutionModel-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkCommonMath-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkCommonMisc-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkCommonSystem-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkCommonTransforms-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkDICOMParser-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkFiltersCore-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkFiltersExtraction-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkFiltersGeneral-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkFiltersGeometry-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkFiltersHybrid-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkFiltersHyperTree-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkFiltersImaging-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkFiltersModeling-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkFiltersSources-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkFiltersStatistics-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkFiltersTexture-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkGUISupportQt-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkIOCore-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkIOGeometry-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkIOImage-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkIOLegacy-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkIOXML-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkIOXMLParser-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkImagingColor-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkImagingCore-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkImagingGeneral-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkImagingHybrid-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkImagingSources-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkInfovisCore-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkInfovisLayout-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkInteractionStyle-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkInteractionWidgets-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkParallelCore-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkParallelDIY-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkRenderingAnnotation-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkRenderingContext2D-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkRenderingContextOpenGL2-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkRenderingContextOpenGL2-9.2.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkRenderingCore-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkRenderingFreeType-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkRenderingHyperTreeGrid-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkRenderingLabel-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkRenderingOpenGL2-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkRenderingUI-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkRenderingVolume-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkViewsCore-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkViewsInfovis-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkViewsQt-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkdoubleconversion-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkexpat-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkfmt-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkfreetype-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkglew-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkjpeg-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkkissfft-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkloguru-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtklz4-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtklzma-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkmetaio-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkpng-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkpugixml-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtksys-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtktiff-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks
cp ../../VTK/lib/libvtkzlib-9.2.1.dylib ./WE_UQ.app/Contents/Frameworks

# copy applications folderm opensees and dakota
echo "cp -fR $pathToBackendApps/applications ./$appFile/Contents/MacOS"
cp -fR $pathToBackendApps/applications ./$appFile/Contents/MacOS
mkdir  ./$appFile/Contents/MacOS/applications/opensees
mkdir  ./$appFile/Contents/MacOS/applications/dakota
echo "cp -fr $pathToOpenSees/* $pathApp/Contents/MacOS/applications/opensees"
cp -fr $pathToOpenSees/* ./$appFile/Contents/MacOS/applications/opensees
cp -fr $pathToDakota/*  ./$appFile/Contents/MacOS/applications/dakota

# clean up
declare -a notWantedApp=("createBIM" 
			 "performRegionalMapping"
			 "performRegionalEventSimulation"
			 "performDL"			
			 "createEDP/standardEarthquakeEDP_R"
			 "createEDP/userEDP_R"
			 "createEDP/gmtEDP"			 
			 "createEDP/simpleEDP"
			 "createEDP/standardEarthquakeEDP"
			 "createEVENT/GeoClawOpenFOAM"			 
			 "createEVENT/ASCE7_WindSpeed"
			 "createEVENT/stochasticGroundMotion"
			 "createEVENT/LLNL_SW4"
			 "createEVENT/NNGM"
			 "createEVENT/pointWindSpeed"
			 "createEVENT/hazardBasedEvent"
			 "createEVENT/siteResponse"
			 "createSAM/MDOF-LU"
			 "createSAM/siteResponse"
			 "createSAM/AutoSDA"			 
			 "performSIMULATION/IMasEDP"
			 "performSIMULATION/extractPGA"
			 "performSIMULATION/openSees_R"
			)

for app in "${notWantedApp[@]}"
do
   echo "removing $app"
   rm -fr ./$appFile/Contents/MacOS/applications/$app
done

find ./$appFile -name __pycache__ -exec rm -rf {} +;

#
# load my credential file
#

userID="../userID.sh"

if [ ! -f "$userID" ]; then

    echo "creating dmg $dmgFile"
    rm $dmgFile
    hdiutil create $dmgFile -fs HFS+ -srcfolder ./$appFile -format UDZO -volname $appName

    echo "No password & credential file to continue with codesign and App store verification"
    exit
fi

source $userID
echo $appleID    
    

#codesign
echo "codesign --deep --force --verbose --options=runtime  --sign "$appleCredential" $appFile"
codesign --deep --force --verbose --options=runtime  --sign "$appleCredential" $appFile

# create dmg
echo "hdiutil create $dmgFile -fs HFS+ -srcfolder ./$appFile -format UDZO -volname $appName"
hdiutil create $dmgFile -fs HFS+ -srcfolder ./$appFile -format UDZO -volname $appName

status=$?
if [[ $status != 0 ]]
then
    echo "DMG Creation FAILED cd build and try the following:"
    echo "hdiutil create $dmgFile -fs HFS+ -srcfolder ./$appFile -format UDZO -volname $appName"    
    echo "codesign --force --sign "$appleCredential" $dmgFile"
    echo "xcrun altool --notarize-app -u $appleID -p $appleAppPassword -f ./$dmgFile --primary-bundle-id altool"
    echo "xcrun altool --notarization-info ID  -u $appleID  -p $appleAppPassword"
    echo "xcrun stapler staple \"$appName\" $dmgFile"
    exit $status;
fi

#codesign dmg
echo "codesign --force --sign "$appleCredential" $dmgFile"
codesign --force --sign "$appleCredential" $dmgFile

echo "Issue the following: " 
echo "xcrun altool --notarize-app -u $appleID -p $appleAppPassword -f ./$dmgFile --primary-bundle-id altool"
echo ""
echo "returns id: ID .. wait for email indicating success"
echo "To check status"
echo "xcrun altool --notarization-info ID  -u $appleID  -p $appleAppPassword"
echo ""
echo "Finally staple the dmg"
echo "xcrun stapler staple \"$appName\" $dmgFile"
