# CSC767M - Introduction to Computer Graphics
Made by: Alfonso Miguel Cruz, Gelson Sze

Repository Link: [Link to Github Repository](https://github.com/GelsonSze/CSC767M-Project/tree/main)
## Final Project: Underwater Fantasy Exploration
This project implements an underwater fantasy environment in OpenGL with C++. The user gets to see a variety of marine animals moving around the scenery, see marine plants that give a more underwater feel to the environment, and explore artifacts that reside in the environment. 

## Instructions
To run the environment in Visual Studio, click on the [Graphics_1.sln](https://github.com/GelsonSze/CSC767M-Project/blob/main/visual_studio/Graphics_1.sln) file to load the VS Code solution. Click on the `Local Windows Debugger` button to compile and run the environment. This also produces an `Graphics_1.exe` file that can be run anytime without needing to recompile the solution.

Once the program is running, you can use the following keys to move around the environment:
- `W` - go upwards in the environment
- `A` - go downwards in the environment
- `S` - go left in the environment
- `D` - go right in the environment

You may also move your mouse around to look at other areas of the environment within your view.

## Additional Features
This environment makes use of shadow mapping, which allows objects hit by light to cast shadows. Implementing shadows and light provide a more realistic view of the environment, which can be seen as you move around the environment.

A sample image of the environment which shows the effect of shadows is shown below:

![shadow_mapping_sample.png](https://github.com/GelsonSze/CSC767M-Project/blob/main/readme_images/shadow_mapping_sample.png)

In this image, you can see the terrain on the left hand side be partially covered by a shadow, whilst the terrain on the right side is heavily covered, which darkens the objects affected by the shadow.

