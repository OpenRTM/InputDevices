// -*- C++ -*-
/*!
 * @file  PSGamepadComp.h
 * @brief Playstation Gamepad component
 * @date  $Date$
 *
 * $Id$
 */

#ifndef PSGAMEPADCOMP_H
#define PSGAMEPADCOMP_H

#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/BasicDataTypeSkel.h>

#include <iostream>

#include "PSGamepad.h"

using namespace RTC;

class PSGamepadComp
  : public RTC::DataFlowComponentBase
{
public:
  PSGamepadComp(RTC::Manager* manager);
  ~PSGamepadComp();

  // The initialize action (on CREATED->ALIVE transition)
  // formaer rtc_init_entry() 
  virtual RTC::ReturnCode_t onInitialize();

  // The activated action (Active state entry action)
  // former rtc_active_entry()
  virtual RTC::ReturnCode_t onActivated(RTC::UniqueId ec_id);

  // The deactivated action (Active state exit action)
  // former rtc_active_exit()
  virtual RTC::ReturnCode_t onDeactivated(RTC::UniqueId ec_id);

  // The execution action that is invoked periodically
  // former rtc_active_do()
  virtual RTC::ReturnCode_t onExecute(RTC::UniqueId ec_id);


protected:
  // Configuration variable declaration
  std::vector<float> m_ratio;
  int m_threshold;
  int m_max;
  int m_min;
  
  // DataOutPort declaration
  TimedFloatSeq m_axes_and_buttons;
  OutPort<TimedFloatSeq> m_axes_and_buttonsOut;
  
private:
  int dummy;
  PSGamepad m_psGamepad;

};


extern "C"
{
  void PSGamepadCompInit(RTC::Manager* manager);
};

#endif // PSGAMEPADCOMP_H
