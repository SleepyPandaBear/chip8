/*********************************************
* File - chip8.cpp
* Author - Miha
* Created - 07 Jul 2021
* Description - 
* *******************************************/

#include <stdio.h>
#include "GLFW/glfw3.h"
#include "chip8.h"
#include <stdlib.h>
#include <time.h>

void
GLFWInit()
{
    if(!glfwInit())
    {
        printf("GLFW init failed\n");
    }
}

void
GLFWCreateWindow(glfw_window *Window)
{
    Window->GLFWWindow = glfwCreateWindow(Window->Width, Window->Height,
                                          Window->Title, NULL, NULL);

    if (!Window->GLFWWindow)
    {
        printf("GLFW window or OpenGL context creation failed\n");
    }

    glfwMakeContextCurrent(Window->GLFWWindow);

    glfwGetFramebufferSize(Window->GLFWWindow, &Window->Width, &Window->Height);
    glViewport(0, 0, Window->Width, Window->Height);
}

void
GLFWTerminate()
{
    glfwTerminate();
}

void
GLFWUpdate(glfw_window *Window)
{
    glfwSwapBuffers(Window->GLFWWindow);
    glfwPollEvents();
}

void
GLFWClearScreen(glfw_window *Window)
{
    glfwGetFramebufferSize(Window->GLFWWindow, &Window->Width,
                           &Window->Height);

    glViewport(0, 0, Window->Width, Window->Height);
    glClear(GL_COLOR_BUFFER_BIT);
}

void
DrawDisplay(glfw_window *Window, chip8 *Chip8)
{
    for(i32 Y = 0; Y < 32; ++Y)
    {
        for(i32 X = 0; X < 64; ++X)
        {
            // NOTE(miha): If Display is 0, then color is black, otherwise
            // color is white.
            if(Chip8->Display[64 * Y + X] == 0)
            {
                glColor3f(0.0f, 0.0f, 0.0f);

                i32 RectX = X * 10;
                i32 RectY = Y * 10;
                i32 RectWidth = RectX + 10;
                i32 RectHeight = RectY + 10;

                glRecti(RectX, RectY, RectWidth, RectHeight);
            }
            else
            {
                glColor3f(1.0f, 1.0f, 1.0f);

                i32 RectX = X * 10;
                i32 RectY = Y * 10;
                i32 RectWidth = RectX + 10;
                i32 RectHeight = RectY + 10;

                glRecti(RectX, RectY, RectWidth, RectHeight);
            }
        }
    }
}

void
MakeRandomPattern(chip8 *Chip8)
{    
    srand((u32) time(NULL));
    for(i32 Y = 0; Y < 32; ++Y)
    {
        for(i32 X = 0; X < 64; ++X)
        {
            Chip8->Display[64 * Y + X] = rand() % 2;
        }
    }
}

void
PrintKeyboard(chip8 *Chip8)
{
    for(u32 KeyIndex = 0; KeyIndex < Chip8->Keyboard.NumberOfKeys; ++KeyIndex)
    {
        if(Chip8->Keyboard.Keys[KeyIndex].IsPressed)
        {
            printf("Key %d is pressed\n", Chip8->Keyboard.Keys[KeyIndex].KeyCode);
        }
    }

}

void
ProcessInput(glfw_window *Window, chip8 *Chip8)
{
    if (glfwGetKey(Window->GLFWWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(Window->GLFWWindow, GLFW_TRUE);

    for(u32 KeyIndex = 0; KeyIndex < Chip8->Keyboard.NumberOfKeys; ++KeyIndex)
    {
        if(glfwGetKey(Window->GLFWWindow, 
           Chip8->Keyboard.Keys[KeyIndex].KeyCode) == GLFW_PRESS)
        {
            Chip8->Keyboard.Keys[KeyIndex].IsPressed = 1;
        }

        if(glfwGetKey(Window->GLFWWindow,
           Chip8->Keyboard.Keys[KeyIndex].KeyCode) == GLFW_RELEASE)
        {
            Chip8->Keyboard.Keys[KeyIndex].IsPressed = 0;
        }
    }
}

void
MapKey(chip8 *Chip8, u32 KeyIndex, u32 KeyCode)
{
    Chip8->Keyboard.Keys[KeyIndex].KeyCode = KeyCode;

}

void
KeyboardInit(glfw_window *Window, chip8 *Chip8)
{
    MapKey(Chip8, 0, GLFW_KEY_1);
    MapKey(Chip8, 1, GLFW_KEY_2);
    MapKey(Chip8, 2, GLFW_KEY_3);
    MapKey(Chip8, 3, GLFW_KEY_4);

    MapKey(Chip8, 4, GLFW_KEY_Q);
    MapKey(Chip8, 5, GLFW_KEY_W);
    MapKey(Chip8, 6, GLFW_KEY_E);
    MapKey(Chip8, 7, GLFW_KEY_R);

    MapKey(Chip8, 8, GLFW_KEY_A);
    MapKey(Chip8, 9, GLFW_KEY_S);
    MapKey(Chip8, 10, GLFW_KEY_D);
    MapKey(Chip8, 11, GLFW_KEY_F);

    MapKey(Chip8, 12, GLFW_KEY_Z);
    MapKey(Chip8, 13, GLFW_KEY_X);
    MapKey(Chip8, 14, GLFW_KEY_C);
    MapKey(Chip8, 15, GLFW_KEY_V);

    Chip8->Keyboard.NumberOfKeys = 16;
}

// NOTE(miha): Each characters ocupies 5 bytes, we need to copy 16 characters.
// Values for font are found on:
// https://github.com/mattmikolay/chip-8/wiki/CHIP%E2%80%908-Technical-Reference.
// NOTE(miha): We place this characters in memory just before user program on
// the location 0x1B0.
void
InitBuildInCharacters(chip8 *Chip8)
{
    u32 *FontMemory = (u32 *) &Chip8->Memory[0x1B0];

    *FontMemory++ = 0xF0909090;
    *FontMemory++ = 0xF0206020;
    *FontMemory++ = 0x2070F010;
    *FontMemory++ = 0xF080F0F0;
    *FontMemory++ = 0x10F010F0;
    *FontMemory++ = 0x9090F010;
    *FontMemory++ = 0x10F080F0;
    *FontMemory++ = 0x10F0F080;
    *FontMemory++ = 0xF090F0F0;
    *FontMemory++ = 0x10204040;
    *FontMemory++ = 0xF090F090;
    *FontMemory++ = 0xF0F090F0;
    *FontMemory++ = 0x10F0F090;
    *FontMemory++ = 0xF09090E0;
    *FontMemory++ = 0x90E090E0;
    *FontMemory++ = 0xF0808080;
    *FontMemory++ = 0xF0E09090;
    *FontMemory++ = 0x90E0F080;
    *FontMemory++ = 0xF080F0F0;
    *FontMemory++ = 0x80F08080;
}

void
FetchDecodeExecute(chip8 *Chip8)
{
    // NOTE(miha): Fetch instruction.
    u16 *PC = &Chip8->Registers.PC;
    u8 InstuctionFirstHalf = Chip8->Memory[*PC];
    u8 InstuctionSecondHalf = Chip8->Memory[*PC + 1];
    u16 Instruction = InstuctionFirstHalf << 8 | InstuctionSecondHalf;
    // TODO(miha): Are we sure we want to increment PC here? What about
    // branching instructions?...
    *PC += 2;

    // TODO: Decode instuction

    u16 InstructionMSB = ((Instruction & 0xF000) >> 12);

    // NOTE(miha): Some instruction 
    switch(InstructionMSB)
    {
        case 0:
        {
            i16 InstructionSecondDecode = Instruction & 0x00F0;

            switch(InstructionSecondDecode)
            {
                case 0xE:
                {
                    i16 InstructionThridDecode = Instruction & 0x000F;

                    if(InstructionThridDecode == 0x0)
                    {
                        // NOTE(miha): Opcode 00E0 - clears the screen.
                        // TODO(miha): Function to clear the screen - set all
                        // bits in buffer to 0.
                    }
                    else if (InstructionThridDecode == 0xE)
                    {
                        // NOTE(miha): Opcode 00EE - returns from subroutine.
                        // TODO(miha): Do we need to pop address from stack or
                        // someting?

                    }
                } break;

                default:
                {
                    // NOTE(miha): 0NNN opcode.
                    u16 Number = Instruction & 0x0FFF;
                    // TODO(miha): Implement this opcode

                } break;
            }

        } break;

        case 1:
        {
            // NOTE(miha): Jump to address NNN.
            u16 Address = Instruction & 0x0FFF;
            Chip8->Registers.PC = Address;
        } break;

        case 2:
        {
            // NOTE(miha): Call subroutine at NNN.
            // TODO(miha): Do we just set PC to this address or do we have to
            // put someting on the stack?
            u16 Subroutine = Instruction & 0x0FFF;
        } break;

        case 3:
        {
            // NOTE(miha): Skips the next instruction if VX equals NN.
            u16 Register = (Instruction & 0x0F00) >> 8;
            u16 Number = Instruction & 0x00FF;

            if(Chip8->Registers.E[Register] == Number)
            {
                *PC += 2;
            }
        } break;

        case 4:
        {
            // NOTE(miha): Skips the next instruction if VX does not equal NN.
            u16 Register = (Instruction & 0x0F00) >> 8;
            u16 Number = Instruction & 0x00FF;

            if(Chip8->Registers.E[Register] != Number)
            {
                *PC += 2;
            }
        } break;

        case 5:
        {
            // NOTE(miha): Skips the next instruction if VX equals VY.
            u16 RegisterV = (Instruction & 0x0F00) >> 8;
            u16 RegisterY = (Instruction & 0x00F0) >> 4;

            if(Chip8->Registers.E[RegisterV] == Chip8->Registers.E[RegisterY])
            {
                *PC += 2;
            }
        } break;

        case 6:
        {
            // NOTE(miha): Set VX to NN.
            u16 Register = (Instruction & 0x0F00) >> 8;
            u16 Number = Instruction & 0x00FF;
            Chip8->Registers.E[Register] = Number;
        } break;

        case 7:
        {
            // NOTE(miha): Adds NN to VX.
            u16 Register = (Instruction & 0x0F00) >> 8;
            u16 Number = Instruction & 0x00FF;
            Chip8->Registers.E[Register] += Number;
        } break;

        case 8:
        {
            u16 RegisterX = (Instruction & 0x0F00) >> 8;
            u16 RegisterY = (Instruction & 0x00F0) >> 4;
            u16 InstructionSecondDecode = (Instruction & 0x000F);

            switch(InstructionSecondDecode)
            {
                case 0:
                {

                } break;

                case 1:
                {

                } break;

                case 2:
                {

                } break;

                case 3:
                {

                } break;

                case 4:
                {

                } break;

                case 5:
                {

                } break;

                case 6:
                {

                } break;

                case 7:
                {

                } break;

                case 0xE:
                {

                } break;
            }
        } break;

        case 9:
        {
            // NOTE(miha): Skips the next instruction if VX does not equal VY.
            u16 RegisterV = (Instruction & 0x0F00) >> 8;
            u16 RegisterY = (Instruction & 0x00F0) >> 4;

            if(Chip8->Registers.E[RegisterV] != Chip8->Registers.E[RegisterY])
            {
                *PC += 2;
            }

        } break;

        case 0xA:
        {
            // NOTE(miha): Sets I to the address NNN.
            u16 Address = (Instruction & 0x0FFF);
            Chip8->Registers.I = Address;
        } break;

        case 0xB:
        {
            // NOTE(miha): Jumps to the address NNN plus V0.
            u16 Number = (Instruction & 0x0FFF);
            *PC = Chip8->Registers.V0 + Number;
        } break;

        case 0xC:
        {
            u16 Register = ((Instruction & 0x0F00) >> 8);
            u16 Number = (Instruction & 0x00FF);
            u16 RandomNumber = rand() & Number;
            Chip8->Registers.E[Register] = RandomNumber;
        } break;

        case 0xD:
        {
            // NOTE(miha): Draws a sprite at coordinate (VX, VY) that has a
            // width of 8 pixels and a height of N+1 pixels. Each row of 8
            // pixels is read as bit-coded starting from memory location I; I
            // value does not change after the execution of this instruction.
            // As described above, VF is set to 1 if any screen pixels are
            // flipped from set to unset when the sprite is drawn, and to 0 if
            // that does not happen.
            
            u16 RegisterX = ((Instruction & 0x0F00) >> 8);
            u16 RegisterY = ((Instruction & 0x00F0) >> 4);
            u16 Number = (Instruction & 0x000F);
            u16 BitmapAddress = Chip8->Registers.I;
            // TODO(miha): Function for drawin bitmap on the screen (buffer).

        } break;

        case 0xE:
        {
            u16 RegisterX = ((Instruction & 0x0F00) >> 8);
            u16 InstructionSecondDecode = ((Instruction & 0x00F0) >> 4);
            
            if(InstructionSecondDecode == 0x9)
            {
                // NOTE(miha): Skips the next instruction if the key stored in
                // VX is pressed.

            }
            else if(InstructionSecondDecode == 0xA)
            {

            }
        } break;

        case 0xF:
        {
            u16 RegisterX = ((Instruction & 0x0F00) >> 8);
            u16 InstructionSecondDecode = ((Instruction & 0x00F0) >> 4);
            u16 InstructionSecondDecode = (Instruction & 0x000F);

        } break;

        default:
        {
            // TODO(miha): Error handling.

        } break;
    }

}

u32
GetFileSize(FILE **File)
{
    u32 Result = 0;

    fseek(*File, 0, SEEK_END);
    Result = ftell(*File);
    rewind(*File);

    return Result;
}

void
ReadGameIntoRAM(chip8 *Chip8, FILE **GameFile, char *GameName)
{
    u32 Error;

    Error = fopen_s(GameFile, GameName, "rb");

    if(Error)
    {
        printf("Error occured while opening file\n");
        exit(1);
    }

    u32 GameFileSize = GetFileSize(GameFile);

    // NOTE(miha): Memory for game starts from the location 0x200. Size of the
    // dedicated area is 0xFFF - 0x200.
    u8 *StartOfMemoryArea = &Chip8->Memory[0x200];
    u32 GameMemoryArea = (0xFFF -0x200);

    Error = (u32) fread_s((void *) StartOfMemoryArea, GameMemoryArea, 1, GameFileSize,
                          *GameFile);

    if(Error != GameFileSize)
    {
        printf("Error occured while reading the file in to buffer\n");
        exit(1);
    }

}

int
main(int ArgumentNumber, char *Arguments[])
{
    // TODO(miha): Create console from where we will be able to choose game
    // file.
    if(ArgumentNumber < 2)
    {
        printf("Please enter name of the game (.ch8 file).\n");
        exit(1);
    }

    char *GameName = Arguments[1];

    glfw_window Window = {};
    Window.Width = 640;
    Window.Height = 320;
    Window.Title = "Chip 8 emulator";

    FILE *GameFile;

    chip8 Chip8 = {};

    // NOTE(miha): First instruction is fetched from memory location 0x200.
    Chip8.Registers.PC = 0x200;

    MakeRandomPattern(&Chip8);

    GLFWInit();
    GLFWCreateWindow(&Window);
    KeyboardInit(&Window, &Chip8);
    InitBuildInCharacters(&Chip8);
    ReadGameIntoRAM(&Chip8, &GameFile, GameName);

    // TODO(miha): We need to init sound - a simple buzzer.

    glMatrixMode(GL_PROJECTION);
    glOrtho(0, Window.Width, 0, Window.Height, -1, 1);

    // NOTE(miha): Time target is 60 times per second (60 Hz).
    double TimeTarget = 1.0f / 60.0f; 
    // NOTE(miha): Main loop. All the magic happens here.
    while(!glfwWindowShouldClose(Window.GLFWWindow))
    {
        double FrameStartTime = glfwGetTime();
        GLFWClearScreen(&Window);

        // TODO: Move this to the new function

        ProcessInput(&Window, &Chip8);
        DrawDisplay(&Window, &Chip8);

        // NOTE(miha): Instruction is long 2 bytes and is stored in big endian
        // fashion.

        // Fetch
        // Decode
        // Execute
        FetchDecodeExecute(&Chip8);
        
        // TODO(miha): We need to implement GetKey, stalls the instuction until
        // key is pressed.
        PrintKeyboard(&Chip8);
        GLFWUpdate(&Window);

        // NOTE(miha): Decrease timers.
        if(Chip8.SoundTimer.Value)
            Chip8.SoundTimer.Value--;

        if(Chip8.DelayTimer.Value)
            Chip8.DelayTimer.Value--;

        double FrameEndTime = glfwGetTime();
        while(FrameEndTime < (FrameStartTime + TimeTarget))
        {
            FrameEndTime = glfwGetTime();
        }

        double MSPerFrame = (FrameEndTime - FrameStartTime) * 1000;
        printf("%f\n", MSPerFrame);
    }

    printf("hello world!\n");

    GLFWTerminate();
    return 0;
}
