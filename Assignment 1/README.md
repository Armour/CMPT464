#ASSIGNMENT 1
####Chong Guo armourg@sfu.ca

##Not Finished Features

- None

##File Structure

- src -- the source file folder

    - main.cc -- Main function, render function
    - init_shader.cc -- Used to read shader
    - smf_parser.cc -- Used to import and export smf file

- include -- the header file folder

    - lib_consts.h -- Constant variables header
    - lib_gl.h -- Include GL related header
    - init_shader.h -- Used to read shader
    - smf_parser.h -- Used to import and export smf file
    - winged_edge.h -- Declare winged edge data structure
    - glm -- OpenGL math library

- mesh -- the mesh data folder
    
    - import and export file wil be here

- cmake -- the cmake findmodule file folder

    - findGLUI.cmake -- find GLUI in different computers
 
    
##How to use

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

##Others

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

