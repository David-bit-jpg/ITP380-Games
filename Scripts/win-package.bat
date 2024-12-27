@echo off

cmake -G Ninja -B Releases -DCMAKE_BUILD_TYPE=Release
IF %ERRORLEVEL% NEQ 0 (echo Failed to generate CMake for project &exit /b 1)

cd Releases

cmake --build . --target %1
IF %ERRORLEVEL% NEQ 0 (echo Failed to build %1 & cd .. &exit /b 1)

if exist Packages\ (
  if exist Packages\%1 (
	rmdir /s /q Packages\%1
	del Packages\%1.zip
  )
) else (
  mkdir Packages
)

mkdir Packages\%1

copy %1\*.exe Packages\%1\
if %ERRORLEVEL% NEQ 0 (echo Failed to copy exe &exit /b 1)

copy %1\*.dll Packages\%1\
if %ERRORLEVEL% NEQ 0 (echo Failed to copy dlls &exit /b 1)

if exist ..\%1\Shaders\ (
	xcopy ..\%1\Shaders Packages\%1\Shaders\ /E
	if %ERRORLEVEL% NEQ 0 (echo Failed to copy shaders &exit /b 1)
)

if exist ..\%1\Assets\ (
	xcopy ..\%1\Assets Packages\%1\Assets\ /E
	if %ERRORLEVEL% NEQ 0 (echo Failed to copy assets &exit /b 1)
)

cd Packages\%1
tar.exe -a -c -f ..\%1.zip *
if %ERRORLEVEL% NEQ 0 (echo Failed to create %1.zip & cd ..\..\.. exit /b 1)

cd ..\..\..

echo Package Releases\Packages\%1.zip successfully created!
