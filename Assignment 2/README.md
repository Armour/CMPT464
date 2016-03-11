# ASSIGNMENT 2

#### Chong Guo armourg@sfu.ca

## File Structure

- src -- the source file folder

    - main.cc -- Main function, render function
    - init_shader.cc -- Used to read shader
    - smf_parser.cc -- Used to import and export smf file
    - subdivision.cc -- Uesd to do the loop and butterfly subdivision
    - **decimation.cc** -- Uesd to do the mesh decimation

- include -- the header file folder

    - winged_edge.h -- Declare winged edge data structure
    - smf_parser.h -- Used to import and export smf file
    - subdivision.h -- Uesd to do the loop and butterfly subdivision
    - **decimation.h** -- Uesd to do the mesh decimation
    - init_shader.h -- Used to read shader
    - lib_consts.h -- Constant variables header
    - lib_gl.h -- Include GL related header
    - glm -- OpenGL math library

- mesh -- the mesh data folder
    
    - the import and export files wil be here

- cmake -- the cmake findmodule file folder

    - findGLUI.cmake -- find GLUI in different platforms
 
    
## How to use

- Mac
    - Use `Clion` 
    - Manual compile
        - `cmake .` (Optional)
        - `make`
        - `./mcaq`
        
- Ubuntu
    - Use `Clion`
    - Manual compile
        - `cmake .` (Optional)
        - `make`
        - `./mcaq` 

- Put all your mesh data in `mesh` folder, and input file name only (with extension)

- If `make` in ubuntu failed, you should remove depend and try to run `make` twice.

## Keyboard Mapping

* `i`: import mesh data from file
* `e`: export mesh data to file
* `1`: flat shaded
* `2`: smooth shaded
* `3`: wireframe only
* `4`: smooth shaded + wireframe
* `t`: change subdivision type
* `<`: decrease subdivision level
* `>`: increase subdivision level
* `s`: **run subdivision**
* `d`: **run decimation**
* `q`: exit
* Ps: sometime program has no response to key press, you should click GUI and then click display window, after that the key press should works well. (Seems to be a bug of GLUI?)

## Others

- Cross platform (Clion + Cmake)
- Following Google C++ Style, well commented

## Limitation

- Not check fold-over
- Subdivision and decimation can not work together now (easy to fix)
