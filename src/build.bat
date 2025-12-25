@echo off

cl /std:c++20 /EHsc ^
   main.cpp ^
   monitor\cpu.cpp ^
   /I . ^
   /Fe:SystemBuddy.exe

echo.
echo Build finished.
