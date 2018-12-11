@echo off

echo compile test.cpp ...
g++ -std=c++11 test.cpp -o test 
echo done

echo.
echo run test.exe ...
test.exe

