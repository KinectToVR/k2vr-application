# KinectToVR (Qt6 rewrite)

## Build instructions
0. Note: Repository may be chaotically updated, please check out<br>
repository before posting an issue with building or missing files.

1. Prerequesties
   To build this project you will need:<br>
   - Visual Studio 2019 (Personally, I'm on VS2019 Preview, probably latest)
   - Kinect SDK 1.8 and 2.0 installed and visible in PATH<br>

   And if you're planning on building overlay projects:
   - Qt plugin for Visual Studio installed (Will be described later)
   - Qt 6.0.0 (Current) with all needed plugins

2. Set up the project
   - Take a deep breath and make yourself some coffee, <br>
   it's gonna take some time to set up all ```vcpkg``` packages, really.<br>
   (Although, you'll have a bunch of free time, ~~you may start learning japanese~~)

   - Clone the latest OpenVR and Eigen3 into ```external/```:<br>
   ```git clone https://github.com/ValveSoftware/openvr external/openvr```<br>
   ```git clone https://gitlab.com/libeigen/eigen external/eigen```

   - Install ```vcpkg``` and its Visual Studio integration<br>
   ```git clone https://github.com/Microsoft/vcpkg.git```<br>
   ```cd vcpkg```<br>
   ```./bootstrap-vcpkg.sh```<br>
   ```./vcpkg integrate install```

   - Install needed libraries (You should choose one linking method for all packages)<br>
   ```vcpkg install glm:x64-windows boost:x64-windows opencv3[world]:x64-windows cppzmq:x64-windows glog:x64-windows```<br>
   (Now you may rest a bit, also please use drive other than ```C:/```, it's gonna be a bit huge)

   - Install and setup Qt Visual Studio Tools (If you're planning on building overlay projects)
      + Download and install [for VS2019](https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools2019)

      + Add your Qt installation to available installations list in Qt Visual Studio plugin<br>
      You have to search for msvc2019_64 folder<br><br>
      ![](https://imgur.com/0T3xb8x.png)<br><br>
      ![](https://imgur.com/nK9dXHE.png)
      + Select Qt installation you've just added in each project properties (except driver and API projects)<br><br>
      ![](https://imgur.com/4Aloxdb.png)

3. Build and deploy
   - Make sure you're building for x64 platform in release mode<br><br>
   ![](https://imgur.com/75ZXiqQ.png)

   - Click ```Build → build all``` and wait for the build to finish

   - All needed dlls from ```vcpkg``` should be automatically moved to deploy directory<br>
   (For general it's ```x64/Release/``` and for driver ```x64/Release/driver/```<br>
   You can find there KinectToVR driver fully set-up and ready to be registered.)<br>
   Although, for the overlay app we'll need to copy additional Qt modules.<br>
   Important! If you didn't build overlay projects, skip the next step.

   - When everything is built, you may deploy the project by running [windeployqt](https://doc.qt.io/qt-5/windows-deployment.html)
      + Before doing anything, make sure you're in cloned ```k2vr-application``` folder!
      + ```cd x64/Release```
      + ```[your qt6 install path]\msvc2019_64\bin\windeployqt.exe --release --qmldir ../../KinectToVR ./```
      + This should copy all project dependiences from your Qt6 installation path to deploy directory <br>
      (If you have Anaconda-Qt you may encounter fatal error ```not found```, <br>
      I'd reccomend removing it or using virtual env next time)

## Debugging OpenVR driver

1. Change run command in project properties, selecting your SteamVR installation path. <br>
(Must be the path to vrserver.exe file in current SteamVR installation directory) <br><br>
![](https://imgur.com/QAvogtW.png)

2. Remove driver dll from ```[K2Driver]/driver/KinectToVR/bin/win64``` and create a hard link:<br>
(```K2Driver``` stands for driver's deployment directory, possibly in ```k2vr-application/x64/Release/driver```)
   - Open cmd and navigate to ```OutDir``` <br>
   (Where you've built your binaries, possibly in ```x64/Release```)
   - Delete ```driver_KinectToVR.dll``` and keep the rest
   - Create a hard link: ```mklink /h driver/KinectToVR/bin/win64/driver_KinectToVR.dll driver_KinectToVR.dll```

3. Click on ```driver_KinectToVR``` project and select ```Debug→Start New Instance```<br>
You may turn on profiling tools (mostly for CPU) and experiment with debugger right after.