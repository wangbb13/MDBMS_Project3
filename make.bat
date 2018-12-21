@echo off

echo compile main.cpp ...
g++ -std=c++11 main.cpp -o main 
echo done

echo continue?
pause

echo.
echo run main.exe ...
main.exe

