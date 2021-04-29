@echo off

mkdir ..\build
pushd ..\build

del /F /Q *

rem cl -FC -Zi ..\src\win32_lockdown.cpp opengl32.lib user32.lib gdi32.lib 
set CommonCompilerFlags=/EHsc -FC -Zi /MD
set CommonLinkerFlags= /NODEFAULTLIB:MSVCRTD -incremental:no -opt:ref user32.lib kernel32.lib advapi32.lib gdi32.lib winmm.lib shell32.lib opengl32.lib glfw3.lib

rem cl -FC -Zi ..\src\renderer\renderer.cpp opengl32.lib user32.lib gdi32.lib glfw3.lib 

cl /Fe"renderer" -I ..\src\includes %CommonCompilerFlags% -D_CRT_SECURE_NO_WARNINGS ..\src\renderer\*.c ..\src\renderer\*.cpp %CommonDependencies% /link /LIBPATH:"..\\src\\libs" %CommonLinkerFlags%

renderer

popd
