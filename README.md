# KinectToVRのメインアプリケーション

![](https://img.shields.io/badge/%E7%95%8C%E9%9D%A2%E3%83%86%E3%82%B9%E3%83%88-%E2%96%B3-yellowgreen) 
![](https://img.shields.io/badge/%E3%82%B9%E3%82%B1%E3%83%AB%E3%83%88%E3%83%B3%E7%B5%B5-CPU30%EF%BC%85-orange) 
![](https://img.shields.io/badge/%E6%AD%A3%E5%B8%B8%E5%8A%A0%E5%B7%A5-CPU5→12%25-yellowgreen)　
    
<p align="center">
  <img src="https://api-lunacy.icons8.com/api/assets/5142ad36-fc34-4281-80ea-7c7060d7bef0/group_4_objects.png" alt=""/>
</p>
    
## なにこれ？    
KinectToVRはVRへのフルボディトラッキングアプリやOpenVRオーバーレイ。    
提供できます：    
-トラッキングにのフィルタ    
-PSMoveでフルボディトラッキング（オプショナル）    
-かわいいSteamVRオーバーレイ    
-位置や回転トラッキングオプション    
-マニュアルとオートマチック較正    
-正確なオフセットアジャスト    
-必須セッティングを保存します    
-より多くの事で、書きたくなかった（自分で試して！）    
    
## コードビルド諭示    
Qt 5.15(__MSVC2019__)のインストールやセットアップ。    
Boost __1.72__ とOpenCV __3.4.0__、OpenVR 1.12.5(又は新しい)、    
ライブラリをダウンロード、__ルートディレクトリ__ へ解凍。    
    
### ストラクチャー：(”｜”のFが例えばフォルダサイン)    
-    boost（Boostのヘッダー）    
    　　｜（例ファイル）algortihm(F), lexical_cast.hpp      
-    Eigen（Eigenのヘッダー）    
    　　｜（例ファイル）src(F), Geometry, Dense, etc.    
-    glm（GLMのヘッダー）    
    　　｜（例ファイル）detai(F)l, common.hpp, glm.hpp    
-    KinectToVR    
-    KinectToVR-360    
-    KinectToVR-One     
-    lib64-msvc-14.2（Boostのバイナリ）    
-    opencv（OpenCVのヘッダーやバイナリ）    
    　　｜（例ファイル）build(F), LICENSE.txt    
-    openvr（OpenVRのヘッダーやバイナリ）    
    　　｜（例ファイル）bin(F), lib(F), openvr.h    
-    .gitattributes    
-    .gitignore    
-    IKinectHandler.h    
-    KinectHandlerBase.h    
-    LICENSE    
-    README.md    
    
### ビルドをします    
すべてのプロジェクトファイルをロード、コンパイル。    
グーグルドライブからその包み物をダウンロード、    
次に __ビルドディレクトリ__ へ解凍。    
(リンク：[deploy.7z](https://drive.google.com/file/d/1d61vxKg7o69cBXlxG8ZRbsMp0-9wxJhd/view?usp=sharing))    
    
### 展開します    
次にその他のプラグインを追加しないで、    
そうでなければwindeployqtのコマンドを実行します！    
(リンク：[windeployqt](https://doc.qt.io/qt-5/windows-deployment.html))    
**とにかく、deploy.7zからのresourcesフォルダを追加する必要があります**