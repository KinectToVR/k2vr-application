# KinectToVR (Qt5 rewrite)

## Build instructions
0. Repository may be chaotically updated, please check out<br>
repository before posting an issue with building or missing files.
<br>

1. Prerequesties
   To build this project you will need:<br>
   - Visual Studio 2019 with v142 build tools
   - Qt plugin for Visual Studio installed
   - Qt (currently used: 5.15.1) with all needed plugins
   - Kinect SDK 1.8 and 2.0 installed and visible in PATH
<br>

2. External dependiences
   - Restore NuGet packages for solution
   - Download opencv 3.4.0 and unpack it to ```external\opencv```,<br> 
   creating file path similar to ```external\opencv\build\bin\\*.dll```
   - Clone latest openvr to ```external\openvr```
   - You may also update ZMQ library for C++ in ```external\ZMQ\```
<br>

3. Setting up project files
   - Add your Qt installation to available installations list in Qt Visual Studio plugin:<br>
   ![](https://imgur.com/X1H8OSP.png)<br>
   ![](https://imgur.com/MghbPj8.png)<br>
   - In Qt project settings change used installation to one you've just added (For each project):<br>
   ![](https://imgur.com/0d7qpuP.png)
<br>

4. Building projects
   - Make sure project ```KinectToVR``` is being built first,<br> 
   other projects will need its output library from output directory<br> 
   (Should be built to ```$(Platform)\$(Configuration)```)
<br>

5. Deploying project
   - Run [windeployqt](https://doc.qt.io/qt-5/windows-deployment.html) in output directory.<br> 
   You may ned to experiment with removing some plugins<br> 
   and checking with degugger to achieve better results.<br> 

## Debugging OpenVR driver

1. Change run command in project properties, selecting your SteamVR installation path. <br>
(Path to vrserver.exe) <br>
![](https://imgur.com/QAvogtW.png)

2. Remove driver dll from ```OutDir/driver/KinectToVR/bin/win64``` and create a hard link:
   - Open cmd and navigate to ```OutDir``` <br>
   (Where you've built your binaries, default is ```x64/$(Configuration)```)
   - Delete ```driver_KinectToVR.dll``` but keep rest
   - Create a hard link: ```mklink /h driver\KinectToVR\bin\win64\driver_KinectToVR.dll driver_KinectToVR.dll```

3. Click on ```driver_KinectToVR``` project and select ```Debug→Start New Instance```