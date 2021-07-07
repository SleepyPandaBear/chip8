/*********************************************
* File - chip8.cpp
* Author - Miha
* Created - 07 Jul 2021
* Description - 
* *******************************************/

#include <stdio.h>

#include "GLFW/glfw3.h"

void
GLFWInit()
{
    if(!glfwInit())
    {
        printf("GLFW init failed\n");

    }
}

void
GLFWTerminate()
{
    glfwTerminate();
}

int
main()
{
    GLFWInit();

    printf("hello world!\n");

    GLFWTerminate();
    return 0;
}
