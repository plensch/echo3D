# echo3D
Generates pseudo-normals from multibeam-echosounder point clouds and the echosounder location over time.
Only useful for further surface reconstruction.

It can also color the point cloud by height (Z) and center the point cloud around (0|0|0)

#### Usage
##### GUI
INPUT PC: is an ascii file with one point per line and comma-separated x,y,z values. (MB-echosounder point cloud)<br/>
Echosounder Path: is an ascii file with one point per line and comma-separated x,y,z values. (location of the MB-echosounder over time)<br/>
Output PC: is a polygon file with the generated normals and color information.<br/>

##### CLI
Run `./echo3d_cli -h`

#### Build
Dependencies:
- Make
- IUP (for GUI, not required when building CLI)

Download the precompiled binaries from https://www.tecgraf.puc-rio.br/iup/en/download.html,
place them in a folder named "iup" in this directory and run `make`.

##### Make Options
`make cli` -> `./echo3d_cli`
`make gui` -> `./echo3d_gui`

