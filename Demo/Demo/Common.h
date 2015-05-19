#pragma once

#include <windows.h>

//安全释放某个类的实例
#ifndef SAFE_DELETE
#define SAFE_DELETE(p) {if(p){delete (p); p = NULL;}}
#endif

//安全释放某个接口
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) {if (p){ p -> Release(); p = NULL}}
#endif

//安全释放数组
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) {if(p) { delete[](p) ; p = NULL; }}
#endif


