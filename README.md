GrimApostles CS2 Radar
---
This is a simple DMA Radar for Counter-Strike 2 using IMGUI and DirectX11. This is built using pcileech to read memory using FPGA hardware (DMA Device). This is a read-only radar - no writes.
You will need the following three .DLL files in the same directory as the binary: **vmm.dll, leechcore.dll, FTD3XX.dll** 
These files can be found in the release section of ulf frisk's pcileech project, linked here https://github.com/ufrisk/pcileech. They will also be included in the release section of this project.

Building
---
To build this project, clone this repository and open the SLN file using Visual Studio. I am using the Visual Studio 2022 (v143) version. Make sure to set the output configuration to Release(x64) and build the project. The build and binary directories will be created. You will find your .exe in the "bin/release" directory.

As stated above, download the pcileech dependencies and place them along side your exe. Additonally, you will need a folder to hold the map images. You can find them in the release section, or use your own. Create a folder named "maps" and place your map images there **(.png files)**. The radar is hard coded to use the classic 1024x1024p CS2 map images for simplicity.

If you want to build using something else you will have to handle linking the dependencies.
