call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86_amd64
SET CL=/MP
rmdir .build /s /q
mkdir .build
cd .build
set CL=/MP
cmake -DCMAKE_BUILD_TYPE=MinSizeRel -G "NMake Makefiles" ../
nmake
cd ../bin
call UnitTests.exe
pause
