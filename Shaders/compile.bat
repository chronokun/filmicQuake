@echo off
call "%VS140COMNTOOLS%\VsDevCmd.bat"
cl.exe /nologo bintoc.c 
%VULKAN_SDK%\bin\glslangValidator.exe -V basic.vert -o basic.vspv
%VULKAN_SDK%\bin\glslangValidator.exe -V basic.frag -o basic.fspv
bintoc.exe basic.vspv basic_vert_spv > basic_vert.c
bintoc.exe basic.fspv basic_frag_spv > basic_frag.c
pause