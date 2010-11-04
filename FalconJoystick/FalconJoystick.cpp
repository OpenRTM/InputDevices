// -*- C++ -*-
/*!
 * @file  FalconJoystick.cpp
 * @brief Falcon Joystick component
 * @date $Date$
 *
 * $Id$
 */

#include "FalconJoystick.h"

// Module specification
// <rtc-template block="module_spec">
static const char* falconjoystick_spec[] =
  {
    "implementation_id", "FalconJoystick",
    "type_name",         "FalconJoystick",
    "description",       "Falcon Joystick component",
    "version",           "1.0.0",
    "vendor",            "S.K",
    "category",          "example",
    "activity_type",     "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.springK", "0.1",
    "conf.default.dampingC", "1.0",
    "conf.default.position_gain", "1.0,1.0,1.0",
    "conf.default.force_gain", "1.0,1.0,1.0",

    ""
  };
// </rtc-template>

FalconJoystick::FalconJoystick(RTC::Manager* manager)
  : RTC::DataFlowComponentBase(manager),
    m_forceIn("force", m_force),
    m_positionOut("position", m_position),
	dummy(0),
	m_basePosition(VECTOR_SIZE, 0.0),
	m_curPosition(VECTOR_SIZE, 0.0),
    m_inited(false)
{
  // Set InPort buffers
  registerInPort("force", m_forceIn);
  
  // Set OutPort buffer
  registerOutPort("position", m_positionOut);
  
  for (int i = 0; i < ARRAY_SIZE; i++) {
	m_refForce[i] = 0.0;
  }
  m_position.data.length(VECTOR_SIZE+VECTOR_SIZE);
}

FalconJoystick::~FalconJoystick()
{
}



RTC::ReturnCode_t FalconJoystick::onInitialize()
{
  // Bind variables and configuration variable
  bindParameter("springK", m_springK, "0.1");
  bindParameter("dampingC", m_dampingC, "1.0");
  bindParameter("position_gain", m_position_gain, "1.0,1.0,1.0");
  bindParameter("force_gain", m_force_gain, "1.0,1.0,1.0");
  
  return RTC::RTC_OK;
}


RTC::ReturnCode_t FalconJoystick::onActivated(RTC::UniqueId ec_id)
{
  m_haptics.init();
  m_haptics.synchFromServo();
  return RTC::RTC_OK;
}


RTC::ReturnCode_t FalconJoystick::onDeactivated(RTC::UniqueId ec_id)
{
  m_haptics.cleanup();
  return RTC::RTC_OK;
}


RTC::ReturnCode_t FalconJoystick::onExecute(RTC::UniqueId ec_id)
{
  if (m_forceIn.isNew()) {
	m_forceIn.read();
	/*
	for (int i = 0; i < VECTOR_SIZE; i++) {
	  m_refForce[i] = m_force.data[i] * m_force_gain[i];
	}
	*/
	m_refForce[0] = (-m_force.data[2]) * m_force_gain[0];
	m_refForce[1] = m_force.data[1] * m_force_gain[1];
	m_refForce[2] = m_force.data[0] * m_force_gain[2];
	m_haptics.setForce(m_refForce);
  }

  m_haptics.synchFromServo();


  if (m_haptics.isButtonDown()) {
	m_basePosition = m_haptics.getPosition();
  }
  m_curPosition = m_haptics.getPosition();

  for (int i = 0; i < VECTOR_SIZE; i ++) {
	m_position.data[i] = (CORBA::Float)((m_curPosition[i] - m_basePosition[i]) * m_position_gain[i]);
  }

  m_position.data[3]=0.0;
  m_position.data[4]=0.0;
  m_position.data[5]=0.0;
  
  m_positionOut.write();

  return RTC::RTC_OK;
}


extern "C"
{
 
  void FalconJoystickInit(RTC::Manager* manager)
  {
    RTC::Properties profile(falconjoystick_spec);
    manager->registerFactory(profile,
                             RTC::Create<FalconJoystick>,
                             RTC::Delete<FalconJoystick>);
  }
  
};


