// -*- C++ -*-
/*!
 * @file SpaceNavigatorWin32COM.cpp
 * @brief Space Navigator device access class for Win32
 * @date $Date$
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2007 Noriaki Ando
 *     All rights reserved.
 *
 * $Id$
 *
 */

#include "StdAfx.h"
#include "SpaceNavigatorWin32COM.h"

#include <math.h>
#include <iostream>

namespace InputDevice
{
SpaceNavigator::SpaceNavigator(double sensitivity,
			       double scale_translation,
			       double scale_rotation,
			       const char* preference,
			       bool co_initialize,
			       bool bump_message)
  : m_sensitivity(sensitivity),
    m_scaleTranslation(scale_translation),
    m_scaleRotation(scale_rotation),
    m_preference(preference),
    m_coInitialize(co_initialize),
    m_bumpMessage(bump_message)
{
	HRESULT hr;

	if (m_coInitialize)
	{
//		hr =::CoInitialize(NULL);
		hr =::CoInitializeEx(NULL, COINIT_MULTITHREADED);
		if (!SUCCEEDED(hr)) throw std::bad_alloc();
	}

	CComPtr<IUnknown> _3DxDevice;
	hr = _3DxDevice.CoCreateInstance(__uuidof(Device));

	if (SUCCEEDED(hr))
	{
		CComPtr<ISimpleDevice> _3DxSimpleDevice;

		hr = _3DxDevice.QueryInterface(&_3DxSimpleDevice);
		if (SUCCEEDED(hr))
		{
			// Get the interfaces to the sensor and the keyboard;
			m_3DSensor = _3DxSimpleDevice->Sensor;
			m_3DKeyboard = _3DxSimpleDevice->Keyboard;
			// Associate a configuration with this device
			_3DxSimpleDevice->LoadPreferences(m_preference);

			// Connect to the driver
			_3DxSimpleDevice->Connect();

		}
	}
}


SpaceNavigator::~SpaceNavigator(void)
{
  CComPtr<ISimpleDevice> _3DxDevice;
  if (m_3DSensor)
    {
      m_3DSensor->get_Device((IDispatch**)&_3DxDevice);
      m_3DSensor.Release();
    }
  
  if (m_3DKeyboard)
    m_3DKeyboard.Release();
  
  
  
  if (_3DxDevice)
    {
      // Disconnect it from the driver
      _3DxDevice->Disconnect();
      _3DxDevice.Release();
    }
  if (m_coInitialize) CoUninitialize();
}

void SpaceNavigator::setSensitivity(double sensitivity)
{
  m_sensitivity = sensitivity;
}

double SpaceNavigator::getSensitivity()
{
  return m_sensitivity;
}

void SpaceNavigator::setTranslationScale(double scale)
{
  m_scaleTranslation = scale;
}

double SpaceNavigator::getTranslationScale()
{
  return m_scaleTranslation;
}

void SpaceNavigator::setRotationScale(double scale)
{
  m_scaleRotation = scale;
}

double SpaceNavigator::getRotationScale()
{
  return m_scaleRotation;
}

std::vector<double> SpaceNavigator::getData()
{
  if (m_bumpMessage) bumpMessage();

  CComPtr<IAngleAxis> pRotation = m_3DSensor->Rotation;
  CComPtr<IVector3D> pTranslation = m_3DSensor->Translation;

  std::vector<double> pos;
  double Kt(m_sensitivity * m_scaleTranslation);
  pos.push_back(pTranslation->X * Kt);
  pos.push_back(pTranslation->Y * Kt);
  pos.push_back(pTranslation->Z * Kt);

  double Kr(pRotation->Angle * Kt);
  pos.push_back(pRotation->X * Kr);
  pos.push_back(pRotation->Y * Kr);
  pos.push_back(pRotation->Z * Kr);

  pRotation.Release();
  pTranslation.Release();
  return pos;
}

std::vector<int> SpaceNavigator::getKeys()
{
  if (m_bumpMessage) bumpMessage();

  if (!m_3DKeyboard) return std::vector<int>(0);

  long nKeys;
  nKeys = m_3DKeyboard->Keys;

  std::vector<int> keys;

  for (long i(0); i < nKeys; ++i)
  {
	  VARIANT_BOOL isPressed;
	  isPressed = m_3DKeyboard->IsKeyDown(i+1);
	  if (isPressed == VARIANT_TRUE)
	  {
		  keys.push_back(1);
	  }
	  else
	  {
		  keys.push_back(0);
	  }
  }

  return keys;
}

void SpaceNavigator::bumpMessage()
{
  MSG msg;
  if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
    {	
      TranslateMessage(&msg); 
      DispatchMessage(&msg); 
    } 
}
};