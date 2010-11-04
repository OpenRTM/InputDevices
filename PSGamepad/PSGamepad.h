// -*- C++ -*-
/*!
 * @file  PSGamepad.h
 * @brief Playstation Gamepad class
 * @date  $Date$
 *
 * $Id$
 */

#ifndef PSGAMEPAD_H
#define PSGAMEPAD_H

#include <iostream>

#define DIRECTINPUT_VERSION 0x0800
//#define _CRT_SECURE_NO_DEPRECATE 
#ifndef _WIN32_DCOM
#define _WIN32_DCOM 
#endif


#include <windows.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 ) 
// #include "resource.h"

// include for Direct X
#include <dinput.h>
#include <dinputd.h>
#include <vector>

using namespace std;

class PSGamepad
{
public:
  PSGamepad();
  ~PSGamepad();

  HRESULT InitDirectInput(int max, int min, int threshold);
  HRESULT UpdateInputState(std::vector<float>& data, const std::vector<float>& rate);
  VOID    FreeDirectInput();

  struct DI_ENUM_CONTEXT
  {
    DIJOYCONFIG* pPreferredJoyCfg;
    bool bPreferredJoyCfgValid;
  };
  
private:
  
};


extern "C"
{
  BOOL CALLBACK    EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext );
};

#endif // PSGAMEPAD_H
