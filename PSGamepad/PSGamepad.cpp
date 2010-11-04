// -*- C++ -*-
/*!
 * @file  PSGamepad.cpp
 * @brief Playstation Gamepad class
 * $Date$
 *
 * $Id$
 */

#include "PSGamepad.h"

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }


LPDIRECTINPUT8       g_pDI              = NULL;         
LPDIRECTINPUTDEVICE8 g_pJoystick        = NULL;     


PSGamepad::PSGamepad()
{
}


PSGamepad::~PSGamepad()
{
}


//-----------------------------------------------------------------------------
// Name: InitDirectInput()
// Desc: Initialize the DirectInput variables.
//-----------------------------------------------------------------------------
HRESULT PSGamepad::InitDirectInput(int max, int min, int threshold)
{
  HRESULT hr;

  // Register with the DirectInput subsystem and get a pointer
  // to a IDirectInput interface we can use.
  // Create a DInput object
  if FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
				      IID_IDirectInput8, (VOID**)&g_pDI, NULL ) ) {
      cout << "Create a DInput object failed. return value: " << hr << endl;
      return hr;
    }


  DIJOYCONFIG PreferredJoyCfg = {0};
  DI_ENUM_CONTEXT enumContext;
  enumContext.pPreferredJoyCfg = &PreferredJoyCfg;
  enumContext.bPreferredJoyCfgValid = false;

  IDirectInputJoyConfig8* pJoyConfig = NULL;  
  if( FAILED( hr = g_pDI->QueryInterface( IID_IDirectInputJoyConfig8, 
					  (void **) &pJoyConfig ) ) ) {
    cout << "Error QueryInterface."  << endl;
    return hr;
  }

  PreferredJoyCfg.dwSize = sizeof(PreferredJoyCfg);
  if( SUCCEEDED( pJoyConfig->GetConfig(0, &PreferredJoyCfg, DIJC_GUIDINSTANCE ) ) ) // This function is expected to fail if no joystick is attached
    enumContext.bPreferredJoyCfgValid = true;
  SAFE_RELEASE( pJoyConfig );


  // Look for a simple joystick we can use for this sample program.
  if( FAILED( hr = g_pDI->EnumDevices( DI8DEVCLASS_GAMECTRL, 
				       EnumJoysticksCallback,
				       &enumContext, DIEDFL_ATTACHEDONLY ) ) ) {
    cout << "Look for a simple joystick failed." << endl;
    return hr;
  }

  // Set the data format to "simple joystick" - a predefined data format 
  //
  // A data format specifies which controls on a device we are interested in,
  // and how they should be reported. This tells DInput that we will be
  // passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
  if( FAILED( hr = g_pJoystick->SetDataFormat( &c_dfDIJoystick2 ) ) ) {
    cout << "SetDataFormat failed." << endl;
    return hr;
  }


  HRESULT	res;
  DIPROPDWORD	pw;
  DIPROPRANGE	pr;

  pw.diph.dwSize = sizeof(DIPROPDWORD);	/* ジョイスティック設定 */
  pr.diph.dwSize = sizeof(DIPROPRANGE);
  pw.diph.dwHeaderSize = pr.diph.dwHeaderSize = sizeof(DIPROPHEADER);
  pw.diph.dwHow = pr.diph.dwHow = DIPH_BYOFFSET;

  pw.dwData = threshold;		/* 入力しきい値 (0-10000: 入力値範囲の10%に相当) */
  pr.lMin = min;   			/* 入力値範囲 */
  pr.lMax = max;
  /* Ｘ軸の設定 */
  pw.diph.dwObj = pr.diph.dwObj = DIJOFS_X;
  res = g_pJoystick->SetProperty(DIPROP_RANGE, &pr.diph);
  res |= g_pJoystick->SetProperty(DIPROP_DEADZONE, &pw.diph);
  /* Ｙ軸の設定 */
  pw.diph.dwObj = pr.diph.dwObj = DIJOFS_Y;
  res |= g_pJoystick->SetProperty(DIPROP_RANGE, &pr.diph);
  res |= g_pJoystick->SetProperty(DIPROP_DEADZONE, &pw.diph);

  if (res != DI_OK)
    {
      cout << "Error: SetProperty." << endl;
      g_pJoystick->Release();
      g_pJoystick = NULL;
      return DIENUM_CONTINUE;
    }

  return S_OK;
}



//-----------------------------------------------------------------------------
// Name: UpdateInputState(RTC::TimedFloatSeq& data)
// Desc: Get the input device's state and display it.
//-----------------------------------------------------------------------------
HRESULT PSGamepad::UpdateInputState(std::vector<float>& data, const std::vector<float>& rate)
{
  HRESULT     hr;
  TCHAR       strText[512] = {0}; // Device state text
  DIJOYSTATE2 js;           // DInput joystick state 

  if( NULL == g_pJoystick ) 
    return S_OK;

  // Poll the device to read the current state
  hr = g_pJoystick->Poll(); 
  if( FAILED(hr) )  
    {
      // DInput is telling us that the input stream has been
      // interrupted. We aren't tracking any state between polls, so
      // we don't have any special reset that needs to be done. We
      // just re-acquire and try again.
      hr = g_pJoystick->Acquire();
      while( hr == DIERR_INPUTLOST ) 
	hr = g_pJoystick->Acquire();

      // hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
      // may occur when the app is minimized or in the process of 
      // switching, so just try again later 
      return S_OK; 
    }

  // Get the input's device state
  if( FAILED( hr = g_pJoystick->GetDeviceState( sizeof(DIJOYSTATE2), &js ) ) ) {
    cout << "Error: GetDeviceState" << endl;
    return hr; // The device should have been acquired during the Poll()
  }

  // Points of view
  for (int i = 0; i < 4; i++ )
    cout << "js.rgdwPOV[" << i << "]: " << js.rgdwPOV[i] << endl;
 
   
  // Fill up text with which buttons are pressed
  for( int i = 0; i < 128; i++ )
    {
      if ( js.rgbButtons[i] & 0x80 )
        {
	  cout << "js.rgbButtons[" << i << "]: " << i << endl;
        }
    }

  data[0] = js.lX * rate[0];
  data[1] = js.lY * rate[1];
  data[2] = js.lZ * rate[2];

  for( int i = 3; i < 15; i++ )
    {
      if ( js.rgbButtons[i-3] & 0x80 )
	{
	  data[i] = 1.0;
	}
      else {
	data[i] = 0.0;
      }
    }

  return S_OK;
}


//-----------------------------------------------------------------------------
// Name: FreeDirectInput()
// Desc: Initialize the DirectInput variables.
//-----------------------------------------------------------------------------
VOID PSGamepad::FreeDirectInput()
{
  // Unacquire the device one last time just in case 
  // the app tried to exit while the device is still acquired.
  if( g_pJoystick ) 
    g_pJoystick->Unacquire();
    
  // Release any DirectInput objects.
  SAFE_RELEASE( g_pJoystick );
  SAFE_RELEASE( g_pDI );
}


extern "C"
{
 
  //-----------------------------------------------------------------------------
  // Name: EnumJoysticksCallback()
  // Desc: Called once for each enumerated joystick. If we find one, create a
  //       device interface on it so we can play with it.
  //-----------------------------------------------------------------------------
  BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance,
				       VOID* pContext )
  {
    HRESULT hr;

    // Obtain an interface to the enumerated joystick.
    hr = g_pDI->CreateDevice( pdidInstance->guidInstance, &g_pJoystick, NULL );

    // If it failed, then we can't use this joystick. (Maybe the user unplugged
    // it while we were in the middle of enumerating it.)
    if( FAILED(hr) ) 
      return DIENUM_CONTINUE;

    // Stop enumeration. Note: we're just taking the first joystick we get. You
    // could store all the enumerated joysticks and let the user pick.
    return DIENUM_STOP;
  }
  
};


