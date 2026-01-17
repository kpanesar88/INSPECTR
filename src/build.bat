@echo off

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

cl /nologo /std:c++20 /EHsc /W4 ^
   main.cpp ^
   monitor\cpu.cpp ^
   monitor\memory.cpp ^
   monitor\systeminfo.cpp ^
   monitor\output.cpp ^
   monitor\storage.cpp ^
   monitor\utils.cpp ^
   monitor\gpu.cpp ^
   monitor\process.cpp ^
   /I . /I monitor ^
   Advapi32.lib ^
   /Fe:Inspectr.exe

echo.
echo Build finished.
