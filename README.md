길벗 출판사 "모던 C++ 디자인 패턴" (원서: 'Design Patterns in Modern C++' by Dmitri Nesteruk)의 역자 수정 예제 코드 입니다. 

설치 방법:

#0: Microsoft Visual Studio 2017 버전을 설치 합니다.
    아래의 페이지에서 다운로드 받을 수 있습니다.
    https://visualstudio.microsoft.com/ko/vs/community/

#1: 본 github를 zip 으로 다운로드 받고 c:\ 에 풉니다.
    반드시 c:\에 풀어야 하고 아래와 같은 폴더 구조가 되어야 합니다.

    C:\design-patterns-in-modern-cpp\Behavioral, Creational, Structural, ...
    
#2: C:\design-patterns-in-modern-cpp\gtest.zip을 c:\ 에 풉니다.
    반드시 c:\에 풀어야 하고 아래와 같은 폴더 구조가 되어야 합니다.
    
    C:\gtest\cmake, codegrear, docs, gtest, include, ...
    
#3: https://www.boost.org/ 에서 1.69.0 버전을 다운 받아 c:\ 에 풉니다.
    반드시 c:\에 풀어야 하고 아래와 같은 폴더 구조가 되어야 합니다.
    
    C:\boost\bin.v2, boost, doc, libs, more, stage, status, ...
    
#4: 아래의 파일을 더블 클릭하여 MSVS를 띄우고 코드를 살펴보고, 빌드, 시험해 봅니다.

    C:\design-patterns-in-modern-cpp\CppDesignPatterns.sln 

    몇몇 예제들은 여러개의 main 함수가 있고 이름만 main_xxx 식으로 바뀌어 있습니다.
    시험할 파일에서 main_xxx를 main 으로 바꾸고 빌드/시험 합니다.


MSVS 샘플 코드 빌드 환경은 초심자 분들을 위한 것입니다.
리눅스 또는 다른 컴파일러 환경을 사용하시는 분은 샘플 코드만으로 충분 하시리라 생각됩니다.

===========================================================================================
아래는 원서 github의 readme 입니다.

# Apress Source Code

This repository accompanies [*Design Patterns in Modern C++*](http://www.apress.com/9781484236024) by Dmitri Nesteruk (Apress, 2018).

[comment]: #cover

Download the files as a zip using the green button, or clone the repository to your machine using Git.

## Releases

Release v1.0 corresponds to the code in the published book, without corrections or updates.

## Contributions

See the file Contributing.md for more information on how you can contribute to this repository.
