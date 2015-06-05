/*************************************************************************************
 [Include File]

 PURPOSE: 
    Include common header files and common macro.
*************************************************************************************/

#ifndef GAME_ENGINE_COMMON_H
#define GAME_ENGINE_COMMON_H

//#define DIRECTINPUT_VERSION 0x0800  // let compile shut up

#include <windows.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>

#include <d3d9.h>
#include <d3dx9.h>
//#include <dinput.h>
#include <dsound.h>

#pragma comment(lib, "d3d9.lib")  
#pragma comment(lib, "d3dx9.lib") 


// define window size
#define WINDOW_WIDTH    640
#define WINDOW_HEIGHT   480

// defines for small numbers
#define EPSILON_E3  (float)(1E-3)
#define EPSILON_E4  (float)(1E-4)
#define EPSILON_E5  (float)(1E-5)
#define EPSILON_E6  (float)(1E-6)

#define Safe_Release(object) if((object) != NULL) { (object)->Release(); (object)=NULL; }

#define FCMP(a, b) (fabs((a) - (b)) < EPSILON_E3 ? 1 : 0)

#endif

