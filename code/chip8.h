/*********************************************
* File - chip8.h
* Author - Miha
* Created - 07 Jul 2021
* Description - 
* *******************************************/

#if !defined(CHIP8_H)
#define CHIP8_H

#include <stdint.h>

#include "GLFW/glfw3.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef u32 b32;

struct glfw_window
{
    GLFWwindow *GLFWWindow;
    i32 Width;
    i32 Height;
    char *Title;

};

// TODO(miha): I dont know the size of void * registers
struct chip8_registers
{
    union
    {
        struct
        {
            u8 V0;
            u8 V1;
            u8 V2;
            u8 V3;
            u8 V4;
            u8 V5;
            u8 V6;
            u8 V7;
            u8 V8;
            u8 V9;
            u8 VA;
            u8 VB;
            u8 VC;
            u8 VD;
            u8 VE;
            u8 VF;

            u16 I;
            u16 PC;
            u8 SP;
        };

        // CARE(miha): With array we can only acces first 16 registers..
        u16 E[22];
    };

};

struct chip8_key
{
    i32 KeyCode;
    b32 IsPressed;
};

struct chip8_keyboard
{
    u32 NumberOfKeys;
    b32 GetKeyFunction;

    union
    {
        struct
        {
            chip8_key Key1;
            chip8_key Key2;
            chip8_key Key3;
            chip8_key Key4;

            chip8_key KeyQ;
            chip8_key KeyW;
            chip8_key KeyE;
            chip8_key KeyR;

            chip8_key KeyA;
            chip8_key KeyS;
            chip8_key KeyD;
            chip8_key KeyF;

            chip8_key KeyZ;
            chip8_key KeyX;
            chip8_key KeyC;
            chip8_key KeyV;
        };

        chip8_key Keys[16];
    };
};

struct chip8_timer
{
    i32 Value;
};

struct chip8
{
    u8 Memory[4096];
    u8 Display[64 * 32];
    chip8_registers Registers;
    chip8_keyboard Keyboard;
    chip8_timer SoundTimer;
    chip8_timer DelayTimer;
};

#endif // CHIP8_H
