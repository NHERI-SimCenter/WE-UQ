from conans import ConanFile

class WEUQ(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "qmake"
    requires = "SimCenterCommonQt/0.1.9@simcenter/stable",\
               "SimCenterBackendApplications/1.2.2@simcenter/stable",\
               "opensees/3.2.2@simcenter/testing",\
               "dakota/6.12.0@simcenter/testing"

    default_options = {"SimCenterCommonQt:MDOFwithQt3D": True}

    def configure(self):
        if self.settings.os == "Windows":
            self.options["libcurl"].with_winssl = True
            self.options["libcurl"].with_openssl = False

    def imports(self):
        appsDestination = "./applications"
        if self.settings.os == "Windows":
            appsDestination = './%s/applications' % self.settings.build_type
        elif self.settings.os == "Macos":
            appsDestination = './WE_UQ.app/Contents/MacOS/applications'


        self.copy("createEVENT/*wind*", dst=appsDestination, src="bin")
        self.copy("createEVENT/*Wind*", dst=appsDestination, src="bin")
        self.copy("createEVENT/*CFD*", dst=appsDestination, src="bin")
        self.copy("createEVENT/*TPU*", dst=appsDestination, src="bin")
        self.copy("createEVENT/*DEDM*", dst=appsDestination, src="bin")
        self.copy("createEVENT/multipleSimCenter/*", dst=appsDestination, src="bin")
        self.copy("createEVENT/SimCenterEvent/*", dst=appsDestination, src="bin")

        self.copy("createSAM/*", dst=appsDestination, src="bin")
        self.copy("createEDP/*", dst=appsDestination, src="bin")
        self.copy("performSIMULATION/*", dst=appsDestination, src="bin")
        self.copy("performUQ/*", dst=appsDestination, src="bin")
        self.copy("Workflow/*", dst=appsDestination, src="bin")
        self.copy("*", dst="{}/opensees".format(appsDestination), src="opensees")
        self.copy("*", dst="{}/dakota".format(appsDestination), src="dakota")
