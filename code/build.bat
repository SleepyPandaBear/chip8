@echo off

set CFLAGS= -MTd -nologo -GR- -EHa- -Od -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -FC -Z7
set LDFLAGS= -incremental:no -opt:ref user32.lib gdi32.lib shell32.lib opengl32.lib ..\code\GLFW\glfw3.lib

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build
    del *.pdb > NUL 2> NUL
    cl %CFLAGS% ..\code\chip8.cpp /link %LDFLAGS%
popd
