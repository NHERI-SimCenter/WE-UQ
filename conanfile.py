from conans import ConanFile

class WEUQ(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "qmake"
    requires = "SimCenterCommonQt/0.0.2@simcenter/testing", "SimCenterBackendApplications/1.1.0@simcenter/stable"

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


        self.copy("createEVENT/*", dst=appsDestination, src="bin")
        self.copy("createSAM/*", dst=appsDestination, src="bin")
        self.copy("createEDP/*", dst=appsDestination, src="bin")
        self.copy("performSIMULATION/*", dst=appsDestination, src="bin")
        self.copy("Workflow/*", dst=appsDestination, src="bin")
