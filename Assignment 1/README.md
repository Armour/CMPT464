# ASSIGNMENT 1
#### Chong Guo armourg@sfu.ca

## Not Finished Features

- None
- **Bonus added**

## File Structure

- src -- the source file folder

    - main.cc -- Main function, render function
    - init_shader.cc -- Used to read shader
    - smf_parser.cc -- Used to import and export smf file
    - **subdivision.cc** -- Uesd to do the loop and butterfly subdivision

- include -- the header file folder

    - lib_consts.h -- Constant variables header
    - lib_gl.h -- Include GL related header
    - init_shader.h -- Used to read shader
    - smf_parser.h -- Used to import and export smf file
    - winged_edge.h -- Declare winged edge data structure
    - **subdivision.h** -- Uesd to do the loop and butterfly subdivision
    - glm -- OpenGL math library

- mesh -- the mesh data folder
    
    - import and export file wil be here

- cmake -- the cmake findmodule file folder

    - findGLUI.cmake -- find GLUI in different computers
 
    
## How to use

- Mac
    - Use `Clion` 
    - Manual compile
        - `cmake .` (Optional)
        - `make`
        - `./smf_view`
        
- Ubuntu
    - Use `Clion`
    - Manual compile
        - `cmake .` (Optional)
        - `make`
        - `./smf_view` 

- Put all your mesh data in `mesh` folder, and input file name only (with extension)

- If `make` in ubuntu not well, you should remove depend and try to run `make` twice.

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
* `?`: run subdivision
* `q`: exit
* Ps: sometime program has no response to key press, you should click GUI and then click display window, after that the key press should work now. (Seems a bug fot GLUI?)

## Others

- Cross platform (Clion + Cmake)
- Following Google C++ Style, well commented
- Self-learned OpenGL from scratch
- Time complexicity
    - $O(nlogn)$ 
    - Used C++ `map` 

##Screen Shots

- Mac

![](images/M1.jpg)

![](images/M2.jpg)

![](images/M3.jpg)

![](images/M4.jpg)

![](images/M5.jpg)

![](images/M6.jpg)

![](images/M7.jpg)

![](images/M8.jpg)

![](images/M9.jpg)

- Ubuntu

![](images/U1.png)

![](images/U2.png)

![](images/U3.png)

![](images/U4.png)

![](images/U5.png)

![](images/U6.png)

![](images/U7.png)

![](images/U8.png)

![](images/U9.png)

- Loop Subdivision (level 0 - 5)

![](images/L0.png)

![](images/L1.png)

![](images/L2.png)

![](images/L3.png)

![](images/L4.png)

![](images/L5.png)

- Butterfly Subdivision (level 0 - 5)

![](images/B0.png)

![](images/B1.png)

![](images/B2.png)

![](images/B3.png)

![](images/B4.png)

![](images/B5.png)

