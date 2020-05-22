@echo off

mkdir ..\build
pushd ..\build

cl -FC -Zi ..\src\win32_lockdown.cpp opengl32.lib user32.lib gdi32.lib 

popd
