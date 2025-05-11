mkdir build
cd  build
call "D:\Programmer\VS2019\VC\Auxiliary\Build\vcvarsamd64_x86.bat"
cmake  -G "Visual Studio 16 2019"  -A x64 ..
cmake --build . --config Debug --verbose
cd ../
pause