# KinectToVRのメインアプリケーション

## コードのビルドの諭示
Qtの5.15(MSVC2019)のインストールやセットアップ。    
Boostの1.72とOpenCVの3.4.0、OpenVRの1.12.5、    
ライブラリをダウンロード、ルートディレクトリへ解凍。    
ストラクチャー：(”｜”のFがフォルダサイン)    
	boost（Boostのヘッダー）    
		｜algortihm(F), lexical_cast.hpp    
    build-KinectToVR-360-Desktop_Qt_5_15_0_MSVC2019_64bit-Release    
    build-KinectToVR-Desktop_Qt_5_15_0_MSVC2019_64bit-Release    
    build-KinectToVR-One-Desktop_Qt_5_15_0_MSVC2019_64bit-Release    
    Eigen（Eigenのヘッダー）    
		｜src(F), Geometry, Dense, etc.    
    glm（GLMのヘッダー）    
		｜detai(F)l, common.hpp, glm.hpp    
    KinectToVR    
    KinectToVR-360    
    KinectToVR-One    
    lib64-msvc-14.2（Boostのバイナリ）    
    opencv（OpenCVのヘッダーやバイナリ）    
		｜build(F), LICENSE.txt    
    openvr（OpenVRのヘッダーやバイナリ）    
	｜bin(F), lib(F), openvr.h    
    .gitattributes    
    .gitignore    
    IKinectHandler.h    
    KinectHandlerBase.h    
    LICENSE    
    README.md    
    
すべてのプロジェクトファイルをロード、    
コンパイル、WinDeployQtのコマンドを実行するか、    
アプリはデバッグモードで実行するください。    
（今に解放する：Qtアプリの実行モード）    
