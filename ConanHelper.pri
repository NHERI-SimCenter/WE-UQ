
#Checking if the current pass is the build pass (For multi configuration)
!build_pass{
    #Checking Conan version
    message(Detecting Conan: $$system(conan -v))

    #Detecting build type
    CONFIG(release, debug|release) {
      CONAN_BUILD_TYPE = Release
    }

    CONFIG(debug, debug|release) {
      CONAN_BUILD_TYPE = Debug
    }

    #Running Conan to install dependencies
    message(Running Conan)
    CONAN_INSTALL_COMMAND=cd $$OUT_PWD && conan install $$PWD -s build_type=$$CONAN_BUILD_TYPE --build missing
    message($$CONAN_INSTALL_COMMAND)
    system($$CONAN_INSTALL_COMMAND)
}


#Conan setup to include generated pri
CONFIG += conan_basic_setup
build_pass:include($$OUT_PWD/conanbuildinfo.pri)
