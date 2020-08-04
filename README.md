# KinectToVRのメインアプリケーション

![](https://img.shields.io/badge/%E7%95%8C%E9%9D%A2%E3%83%86%E3%82%B9%E3%83%88-%E2%96%B3-yellowgreen) 
![](https://img.shields.io/badge/%E3%82%B9%E3%82%B1%E3%83%AB%E3%83%88%E3%83%B3%E7%B5%B5-CPU30%EF%BC%85-orange) 
![](https://img.shields.io/badge/%E6%AD%A3%E5%B8%B8%E5%8A%A0%E5%B7%A5-CPU7%25-yellowgreen)　

## コードのビルドの諭示
Qtの5.15(__MSVC2019__)のインストールやセットアップ。    
Boostの __1.72__ とOpenCVの __3.4.0__、OpenVRの1.12.5、    
ライブラリをダウンロード、__ルートディレクトリ__ へ解凍。    
    
### ストラクチャー：(”｜”のFがフォルダサイン)    
-    boost（Boostのヘッダー）    
    ｜algortihm(F), lexical_cast.hpp    
    **IF (QT) (**     
         build-KinectToVR-360-Desktop_Qt_5_15_0_MSVC2019_64bit-Release    
         build-KinectToVR-Desktop_Qt_5_15_0_MSVC2019_64bit-Release    
         build-KinectToVR-One-Desktop_Qt_5_15_0_MSVC2019_64bit-Release   
    **)**     
-    Eigen（Eigenのヘッダー）    
    ｜src(F), Geometry, Dense, etc.    
-    glm（GLMのヘッダー）    
    ｜detai(F)l, common.hpp, glm.hpp    
-    KinectToVR    
     **｜IF (VS) release**    
-    KinectToVR-360    
     **｜IF (VS) release**    
-    KinectToVR-One    
     **｜IF (VS) release**    
-    lib64-msvc-14.2（Boostのバイナリ）    
-    opencv（OpenCVのヘッダーやバイナリ）    
    ｜build(F), LICENSE.txt    
-    openvr（OpenVRのヘッダーやバイナリ）    
    ｜bin(F), lib(F), openvr.h    
-    .gitattributes    
-    .gitignore    
-    IKinectHandler.h    
-    KinectHandlerBase.h    
-    LICENSE    
-    README.md    
    
### それから
**IF QT:**     
    すべてのプロジェクトファイルをロード、    
    コンパイル、WinDeployQtのコマンドを実行するか、    
    アプリはデバッグモードで実行するください。    
    （今に解放する：Qtアプリの実行モード）    
      
**IF VS:**      
    すべてのプロジェクトファイルをロード、     
    コンパイル、WinDeployQtのコマンドを実行する、   
    （**実験！！！**）        
