// -*- C++ -*-
/*!
 * @file  PSGamepadComp.cpp
 * @brief Playstation Gamepad component
 * $Date$
 *
 * $Id$
 */

#include "PSGamepadRTC.h"
#include "VectorConvert.h"

#define AXES_AND_BUTTON_NUM 15

using namespace std;

// Module specification
static const char* psgamepadcomp_spec[] =
  {
    "implementation_id", "PSGamepadComp",
    "type_name",         "PSGamepadComp",
    "description",       "Playstation Gamepad component",
    "version",           "0.1",
    "vendor",            "S.Kurihara",
    "category",          "Generic",
    "activity_type",     "DataFlowComponent",
    "max_instance",      "10",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.ratio", "0.01,-0.01,0.01",
    "conf.default.threshold", "1000",
    "conf.default.max", "1000",
    "conf.default.min", "-1000",

    ""
  };

PSGamepadComp::PSGamepadComp(RTC::Manager* manager)
  : RTC::DataFlowComponentBase(manager),
    m_axes_and_buttonsOut("axes_and_buttons", m_axes_and_buttons),
    dummy(0)
{
  // Registration: InPort/OutPort/Service
  // Set OutPort buffer
  registerOutPort("axes_and_buttons", m_axes_and_buttonsOut);
  
}

PSGamepadComp::~PSGamepadComp()
{
}


RTC::ReturnCode_t PSGamepadComp::onInitialize()
{
  // Bind variables and configuration variable
  bindParameter("ratio", m_ratio, "0.01,-0.01,0.01");
  bindParameter("threshold", m_threshold, "1000");
  bindParameter("max", m_max, "1000");
  bindParameter("min", m_min, "-1000");
  m_axes_and_buttons.data.length(AXES_AND_BUTTON_NUM);
  return RTC::RTC_OK;
}


RTC::ReturnCode_t PSGamepadComp::onActivated(RTC::UniqueId ec_id)
{
  if (FAILED(m_psGamepad.InitDirectInput(m_max, m_min, m_threshold)))
    {
      cout << "Error: Called InitDirectInput()." << endl;
      return RTC::RTC_ERROR;
    }
  return RTC::RTC_OK;
}



RTC::ReturnCode_t PSGamepadComp::onDeactivated(RTC::UniqueId ec_id)
{
  m_psGamepad.FreeDirectInput();
  return RTC::RTC_OK;
}



RTC::ReturnCode_t PSGamepadComp::onExecute(RTC::UniqueId ec_id)
{
  std::vector<float> axes_and_buttons_data(AXES_AND_BUTTON_NUM, 0.0);
  if (FAILED(m_psGamepad.UpdateInputState(axes_and_buttons_data, m_ratio)))
    {
      return RTC::RTC_ERROR;
    }
  for (int i = 0; i < AXES_AND_BUTTON_NUM; i++) {
    m_axes_and_buttons.data[i] = axes_and_buttons_data[i];
  }
  m_axes_and_buttonsOut << m_axes_and_buttons;
  return RTC::RTC_OK;
}


extern "C"
{
 
  void PSGamepadCompInit(RTC::Manager* manager)
  {
    RTC::Properties profile(psgamepadcomp_spec);
    manager->registerFactory(profile,
                             RTC::Create<PSGamepadComp>,
                             RTC::Delete<PSGamepadComp>);
  }
  
};


