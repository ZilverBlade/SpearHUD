@echo off

glslc.exe -o ./shud.vert.spv -g ./shud.vert 
glslc.exe -o ./shud.frag.spv -g ./shud.frag 

pause
