# DEPRECATED, WILL BE REPLACED WITH AMETHYST
## The K2QT projects and wikis were removed, stick back to K2EX for now<br>This repo is gonna hold KinectToVR's SteamVR driver & K2API until then
## Don't worry, removed files are still in the commit history
<br>

## KinectToVR API (w/ OpenVR driver)
Current KinectToVR API is also a bit deprecated and not version-interchangeable,<br>
due to boost serialization dependency. If you really really wanna use it, please try<br>
looking around in the wiki and find out how to use it for yourself.<br>
(Though I may help a bit when asked to... Don't guarantee how much is a bit tho, heh)<br>
The OpenVR/SteamVR driver protocol has also changed in amethyst (KinectToVR >1.0)

## Build instructions
0. Note: Repository may be chaotically updated, please check out<br>
repository before posting an issue with building or missing files.

1. Prerequisites
   - Visual Studio 2019 (Personally, I'm on VS2019 Preview, probably latest)

2. Set up the project
   - Take a deep breath and make yourself some coffee, <br>
   it's gonna take some time to set up all ```vcpkg``` packages.<br>
   (About 1-1.5 hour if you have 300-500mbps internet and a hdd)

   - Clone the latest OpenVR, GLM and Eigen3 into ```external/```:<br>
   ```git clone https://github.com/ValveSoftware/openvr external/openvr```<br>
   ```git clone https://gitlab.com/libeigen/eigen external/eigen```
   ```git clone https://github.com/g-truc/glm external/glm```

   - Fix min/max error in GLM (unresolved with NOMINMAX for now)
   ```sed -i '/#include <limits>/c\#include <limits>\n\n#undef min\n#undef max' external/glm/glm/gtx/component_wise.inl```

   - Install ```vcpkg``` and its Visual Studio integration<br>
   ```git clone https://github.com/Microsoft/vcpkg.git```<br>
   ```cd vcpkg```<br>
   ```./bootstrap-vcpkg.sh```<br>
   ```./vcpkg integrate install```

   - Install needed libraries (You should choose one linking method for all packages)<br>
   ```vcpkg install boost:x64-windows opencv3[world]:x64-windows cppzmq:x64-windows curlpp:x64-windows```<br>
   (Now you may rest a bit, also consider using a drive other than ```C:/```, it'll be about 6-7GB)

   - Setup GLog<br>
   ```git clone https://github.com/google/glog.git external/glog```<br>
   ```git reset --hard f8c8e99fdfb998c2ba96cfb470decccf418f0b30```<br>
   ```cd external/glog```<br>
   ```mkdir vcbuild; cd vcbuild```<br>
   ```cmake -DBUILD_SHARED_LIBS=ON ..```<br>
   ```msbuild glog.vcxproj "/p:Configuration=Release;Platform=x64```<br>
   ```WindowsTargetPlatformVersion=10.0"```<br>

   - Setup GFlags<br>
   ```git clone https://github.com/gflags/gflags.git external/gflags```<br>
   ```git reset --hard 827c769e5fc98e0f2a34c47cef953cc6328abced```<br>
   ```cd external/gflags```<br>
   ```mkdir vcbuild; cd vcbuild```<br>
   ```cmake -DBUILD_SHARED_LIBS=ON ..```<br>
   ```msbuild gflags.vcxproj "/p:Configuration=Release;Platform=x64;WindowsTargetPlatformVersion=10.0"```<br>

3. Build and deploy
   - Make sure you're building for x64 platform in release mode<br><br>
   ![](https://imgur.com/75ZXiqQ.png)

   - Click ```Build → build all``` and wait for the build to finish

   - All needed dlls from ```vcpkg``` should be automatically moved to deploy directory<br>
   (For general it's ```x64/Release/``` and for driver ```x64/Release/driver/```<br>
   You can find there KinectToVR driver fully set-up and ready to be registered.)

## Debugging OpenVR driver

1. Change run command in project properties, selecting your SteamVR installation path. <br>
(Must be the path to vrserver.exe file in current SteamVR installation directory) <br><br>
![](https://imgur.com/QAvogtW.png)

1. Remove driver dll from ```[K2Driver]/bin/win64``` and create a hard link:<br>
(```[K2Driver]``` stands for driver's deployment directory, it possibly is ```k2vr-application/x64/Release/driver/KinectToVR```)
   - Open cmd and navigate to ```OutDir``` <br>
   (Where you've built your binaries, possibly in ```x64/Release```)
   - Delete ```driver_KinectToVR.dll``` from ```[K2Driver]/bin/win64``` and keep the rest
   - Create a hard link: ```New-Item -ItemType HardLink -Name "[K2Driver]/bin/win64\driver_KinectToVR.dll" -Value driver_KinectToVR.dll``` where ```[K2Driver]``` possibly stands for ```./driver/KinectToVR```

1. Click on ```driver_KinectToVR``` project and select ```Debug→Start New Instance```<br>
You may turn on profiling tools (mostly for CPU) and experiment with the debugger right after.
