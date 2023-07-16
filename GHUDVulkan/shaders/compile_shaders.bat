@echo off

glslc.exe -o ./ghud.vert.spv -g ./ghud.vert 
glslc.exe -o ./ghud.frag.spv -g ./ghud.frag 

pause
