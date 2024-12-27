@echo off
rem Helper to run clang-format on files
rem run-clang-format.bat - clang-format all the .cpp/.h files
rem run-clang-format.bat LabXX - clang-format the files for LabXX
rem run-clang-format.bat LabXX test - Will check for clang-format issues and output warning if so

if [%1]==[] goto applyall
dir /b /s *.cpp | findstr %1 > clang-format-files.txt
dir /b /s *.h | findstr %1 >> clang-format-files.txt

if [%2]==[] goto applylab
clang-format --files=clang-format-files.txt -n --ferror-limit=1 --Werror 2> clang-format-temp.txt
goto :eof

:applylab
clang-format -i --files=clang-format-files.txt
goto :eof

:applyall
dir /b /s *.cpp | findstr Lab* > clang-format-files.txt
dir /b /s *.h | findstr Lab* >> clang-format-files.txt
clang-format -i --files=clang-format-files.txt
