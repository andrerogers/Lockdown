@echo off

mkdir ..\build
pushd ..\build

echo "cleaning old build..."
del /F /Q *


echo "building..."
set CommonCompilerFlags=/EHsc -FC -Zi /MD
set CommonLinkerFlags= /NODEFAULTLIB:MSVCRTD -incremental:no -opt:ref user32.lib kernel32.lib advapi32.lib gdi32.lib winmm.lib shell32.lib opengl32.lib glfw3.lib

rem cl /Fe"lockdown" -FC -Zi ..\src\win32_lockdown.cpp opengl32.lib user32.lib gdi32.lib 

cl /Fe"lockdown" -I ..\src\includes %CommonCompilerFlags% -D_CRT_SECURE_NO_WARNINGS ..\src\renderer\*.c ..\src\renderer\*.cpp %CommonDependencies% /link /LIBPATH:"..\\src\\libs" %CommonLinkerFlags%

lockdown

popd
