# KinectToVR (Qt6 rewrite)

## Build instructions
0. Note: Repository may be chaotically updated, please check out<br>
repository before posting an issue with building or missing files.

1. Prerequesties
   To build this project you will need:<br>
   - Visual Studio 2019 with v142 build tools<br>and /std:c++latest support (Last checked with 16.9.0)
   - Qt plugin for Visual Studio installed
   - Qt 6.0.0 (Current) with all needed plugins
   - Kinect SDK 1.8 and 2.0 installed and visible in PATH

2. External dependiences
   - Restore NuGet packages for solution
   - Install vcpkg with Visual Studio integration<br>and install glog 
   (we'll need glog:x64-windows)<br>
   [Installation instructions are provided here.](https://github.com/google/glog#vcpkg)
   - Download opencv 3.4.0 and unpack it onto ```external/opencv/```,<br> 
   (model path: ```external/opencv/build/include/opencv2/opencv.hpp```)
   - Clone latest openvr into ```external/```
   - Clone latest Eigen into ```external/```
   - You may also update ZMQ library for C++ in ```external/ZMQ/```

3. Setting up project files
   - Add your Qt installation to available installations list in Qt Visual Studio plugin:<br>
   (Qt5 in screens, you shall select latest version you have. Current: Qt6.0.0)<br><br>
   ![](https://imgur.com/X1H8OSP.png)<br><br>
   ![](https://imgur.com/MghbPj8.png)<br><br>
   - In Qt project settings change used installation to one you've just added (For each project):<br><br>
   ![](https://imgur.com/0d7qpuP.png)

4. Building projects
   - Make sure project ```KinectToVR``` is being built first,<br> 
   other projects will need its output library from output directory<br> 
   (Should be built to ```$(Platform)/$(Configuration)/```)

5. Deploying project
   - Run [windeployqt](https://doc.qt.io/qt-5/windows-deployment.html) in output directory, specify ```KinectToVR/``` as QmlDir.<br> 
   You may need to experiment with removing some plugins<br> 
   and checking with degugger to achieve better results.<br> 

## Debugging OpenVR driver

1. Change run command in project properties, selecting your SteamVR installation path. <br>
(Must be the path to vrserver.exe file in current SteamVR installation directory) <br><br>
![](https://imgur.com/QAvogtW.png)

2. Remove driver dll from ```OutDir/driver/KinectToVR/bin/win64``` and create a hard link:
   - Open cmd and navigate to ```OutDir``` <br>
   (Where you've built your binaries, default is ```x64/$(Configuration)```)
   - Delete ```driver_KinectToVR.dll``` but keep rest
   - Create a hard link: ```mklink /h driver/KinectToVR/bin/win64/driver_KinectToVR.dll driver_KinectToVR.dll```

3. Click on ```driver_KinectToVR``` project and select ```Debug→Start New Instance```<br>
You may turn on profiling tools (mostly for CPU) and experiment with debugger right after.