// -*- C++ -*-
/*!
 * @file SpaceNavigatorWin32COM.h
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

#pragma once

#import "progid:TDxInput.Device.1" no_namespace
#include "StdAfx.h"
#include <windows.h>
#include <vector>

namespace InputDevice
{

class SpaceNavigator
{
public:
  SpaceNavigator(double sensitivity = 1.0,
		 double scale_translation = 1.0,
		 double scale_rotation = 1.0,
		 const char* preference = "",
		 bool co_initialize = true,
		 bool message_pump = true);
  virtual ~SpaceNavigator(void);

  void setSensitivity(double sensitivity);
  double getSensitivity();
  void setTranslationScale(double scale);
  double getTranslationScale();
  void setRotationScale(double scale);
  double getRotationScale();
  std::vector<double> getData();
  std::vector<int> getKeys();
  void bumpMessage();

private:
  double m_sensitivity;
  double m_scaleTranslation;
  double m_scaleRotation;
  const char* m_preference;
  bool m_coInitialize;
  bool m_bumpMessage;
  CComPtr<ISensor> m_3DSensor;
  CComPtr<IKeyboard> m_3DKeyboard;
};
};