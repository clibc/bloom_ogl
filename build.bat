@echo off

set cc=cl.exe
set flags=/nologo /W3 /EHsc /Zi /MP2
set libs=..\depend\glfw\lib\glfw3dll.lib  ..\depend\glew\lib\glew32.lib opengl32.lib shell32.lib 
set includes=/I ..\depend\glm\ /I ..\depend\glfw\include\ /I ..\depend\glew\include\ /I ..\depend\ /I ..\src\ 
set files=..\src\main.cpp

mkdir build\

pushd build\
	  %cc% %flags% %files% %includes% /link %libs% /SUBSYSTEM:CONSOLE
popd
