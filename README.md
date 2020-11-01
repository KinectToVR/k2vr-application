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
   - Download latest Eigen release and unpack it at src level to<br> 'external\Eigen',<br> 
   creating file path like 'external\Eigen\Geometry' available
   - Download latest glm release and unpack it to 'external\glm',<br> 
   making file path similar to 'external\glm\glm.hpp' available
   - Download boost 1.74 and unpack it to 'external'.<br> 
   Change folder name from 'boost_1_74_0' to 'boost',<br> 
   making file path like 'external\boost\lib64-msvc-14.2\\*.lib' available
   - Download opencv 3.4.0 and unpack it to 'external\opencv',<br> 
   making file path like 'external\opencv\build\bin\\*.dll' available
   - Download latest openvr and unpack it to 'external\openvr',<br> 
   creating file path similar to 'external\openvr\headers\\*.h' available
   - You may also build static ZMQ library and update files in 'external\ZMQ\'
<br>

3. Setting up project files
   - Add your Qt installation to available installations list in Qt Visual Studio plugin:<br>
   ![](https://imgur.com/X1H8OSP.png)<br>
   ![](https://imgur.com/MghbPj8.png)<br>
   - In Qt project settings change used installation to one you've just added (For each project):<br>
   ![](https://imgur.com/0d7qpuP.png)
<br>

4. Building projects
   - Make sure project 'KinectToVR' is being built first,<br> 
   other projects will need its output library from output directory<br> 
   (Should be built to '$(Platform)\$(Configuration)')
<br>

5. Deploying project
   - Run [windeployqt](https://doc.qt.io/qt-5/windows-deployment.html) in output directory.<br> 
   You may ned to experiment with removing some plugins<br> 
   and checking with degugger to achieve better results.<br> 
   You may also download [zip](https://drive.google.com/file/d/15FX0Ubg9m7rEBj8G2OBDGCgHNLwfZ53W/view?usp=sharing) from my own workaround,<br> 
   assuming you haven't added any other plugins.
