// -*- C++ -*-
/*!
* @file  SpaceNavigator.cpp
* @brief 3DConnexion SpaceNavigator component
* $Date$
*
* $Id$
*/

#include "SpaceNavigator.h"

// Module specification
// <rtc-template block="module_spec">
static const char* spacenavigator_spec[] =
{
	"implementation_id", "SpaceNavigator",
	"type_name",         "SpaceNavigator",
	"description",       "3DConnexion SpaceNavigator component",
	"version",           "0.1",
	"vendor",            "AIST",
	"category",          "input_device",
	"activity_type",     "DataFlowComponent",
	"max_instance",      "1",
	"language",          "C++",
	"lang_type",         "compile",
	// Configuration variables
	// default mode: general world coordination
	"conf.default.mode", "default",
	"conf.default.sensitivity", "1.0",
	"conf.default.translation_scale", "1.0",
	"conf.default.rotation_scale", "1.0",
	// SpaceNavigator's default screen coordination 
	"conf.screen.mode", "screen",
	"conf.screen.sensitivity", "1.0",
	"conf.screen.translation_scale", "1.0",
	"conf.screen.rotation_scale", "1.0",

	""
};
// </rtc-template>

SpaceNavigator::SpaceNavigator(RTC::Manager* manager)
: RTC::DataFlowComponentBase(manager),
// <rtc-template block="initializer">
m_posOut("pos", m_pos),
m_buttonOut("button", m_button),

// </rtc-template>
dummy(0)
{
	// Registration: InPort/OutPort/Service
	// <rtc-template block="registration">
	// Set InPort buffers

	// Set OutPort buffer
	registerOutPort("pos", m_posOut);
	registerOutPort("button", m_buttonOut);

	// Set service provider to Ports

	// Set service consumers to Ports

	// Set CORBA Service Ports

	// </rtc-template>

}

SpaceNavigator::~SpaceNavigator()
{
}


RTC::ReturnCode_t SpaceNavigator::onInitialize()
{
	// <rtc-template block="bind_config">
	// Bind variables and configuration variable
	bindParameter("sensitivity", m_sensitivity, "1.0");
	bindParameter("translation_scale", m_translation_scale, "1.0");
	bindParameter("rotation_scale", m_rotation_scale, "1.0");
	bindParameter("mode", m_mode, "default");

	// </rtc-template>
	if (m_sn.getSensitivity() != m_sensitivity)
		m_sn.setSensitivity(m_sensitivity);
	if (m_sn.getTranslationScale() != m_translation_scale)
		m_sn.setTranslationScale(m_translation_scale);
	if (m_sn.getRotationScale() != m_rotation_scale)
		m_sn.setRotationScale(m_rotation_scale);
//	m_button.data.length(m_sn.getKeys().size());

	return RTC::RTC_OK;
}



/*
RTC::ReturnCode_t SpaceNavigator::onFinalize()
{
return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SpaceNavigator::onStartup(RTC::UniqueId ec_id)
{
return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SpaceNavigator::onShutdown(RTC::UniqueId ec_id)
{
return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SpaceNavigator::onActivated(RTC::UniqueId ec_id)
{
return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SpaceNavigator::onDeactivated(RTC::UniqueId ec_id)
{
return RTC::RTC_OK;
}
*/

RTC::ReturnCode_t SpaceNavigator::onExecute(RTC::UniqueId ec_id)
{
	m_sn.setSensitivity(m_sensitivity);
	m_sn.setTranslationScale(m_translation_scale);
	m_sn.setRotationScale(m_rotation_scale);

	std::vector<double> pos(m_sn.getData());
	std::vector<int> keys(m_sn.getKeys());

	m_pos.data.length(pos.size());
	if (m_mode == "default")
	{
		this->defaultMapping(pos);
	}
	else if (m_mode == "screen")
	{
		this->screenMapping(pos);
	}
	else
	{
		this->defaultMapping(pos);
	}

	m_button.data.length(keys.size());
	for (CORBA::ULong i(0), len(keys.size());  i < len; ++i)
	{
		m_button.data[i] = keys[i];
	}

	m_posOut.write();
	m_buttonOut.write();
	return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t SpaceNavigator::onAborting(RTC::UniqueId ec_id)
{
return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SpaceNavigator::onError(RTC::UniqueId ec_id)
{
return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SpaceNavigator::onReset(RTC::UniqueId ec_id)
{
return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SpaceNavigator::onStateUpdate(RTC::UniqueId ec_id)
{
return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SpaceNavigator::onRateChanged(RTC::UniqueId ec_id)
{
return RTC::RTC_OK;
}
*/

void SpaceNavigator::defaultMapping(std::vector<double>& pos)
{
	m_pos.data[0] = (float)pos[0];
	m_pos.data[1] = (float)-pos[2];
	m_pos.data[2] = (float)pos[1];
	m_pos.data[3] = (float)pos[3];
	m_pos.data[4] = (float)-pos[5];
	m_pos.data[5] = (float)pos[4];
}

void SpaceNavigator::screenMapping(std::vector<double>& pos)
{
	m_pos.data[0] = (float)pos[0];
	m_pos.data[1] = (float)pos[1];
	m_pos.data[2] = (float)pos[2];
	m_pos.data[3] = (float)pos[3];
	m_pos.data[4] = (float)pos[4];
	m_pos.data[5] = (float)pos[5];
}



extern "C"
{

	void SpaceNavigatorInit(RTC::Manager* manager)
	{
		RTC::Properties profile(spacenavigator_spec);
		manager->registerFactory(profile,
			RTC::Create<SpaceNavigator>,
			RTC::Delete<SpaceNavigator>);
	}

};


