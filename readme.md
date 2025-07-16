# Image Viewer for PPM P3 Format

## Description

This project is an image viewer for PPM image types. It currently only supports PPM Ascii format which is referred to as P3 format. The project makes use of some dependencies such as OpenGL for the graphics rendering, and IMGUI for some simple GUI to tell the program to render the image set in code. Part of the reason I made this project was to practice file processing and apply some of the graphics programming I've been learning of late. 

To provide further details on the project, the processing of the PPM file required a few steps. As the project only supports P3 format, I only needed to focus on parsing the header data, which contained if its either P3 or P6, the dimension of the image, and the max color value. After storing the header data, we parse the ascii data and store it as a buffer for OpenGL to use.