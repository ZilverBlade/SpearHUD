@echo off

glslc.exe -o ./ghud.vert.spv ./ghud.vert 
glslc.exe -o ./ghud.frag.spv ./ghud.frag 

pause
