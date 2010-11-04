// -*- C++ -*-
/*!
 * @file  RTWiimote.cpp
 * @brief WiiリモコンのRTコンポーネント
 * @date $Date$
 *
 * $Id$
 */

#include "RTWiimote.h"

// デバッグ情報の表示
//#define DEBUG_SHOW_INPUT_DATA
//#define DEBUG_SHOW_OUTPUT_DATA

// Module specification
// <rtc-template block="module_spec">
static const char* rtwiimote_spec[] =
  {
    "implementation_id", "RTWiimote",
    "type_name",         "RTWiimote",
    "description",       "Wiirimote RTComponent",
    "version",           "1.0.0",
    "vendor",            "Keisuke SUZUKI, AIST",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    "exec_cxt.periodic.rate", "1.0",
    // Configuration variables
    "conf.default.beep", "beep.wav",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
RTWiimote::RTWiimote(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_ledIn("led", m_led),
    m_rumbleIn("rumble", m_rumble),
    m_speakerIn("speaker", m_speaker),
    m_buttonOut("button", m_button),
    m_accelerationOut("acceleration", m_acceleration),
    m_update_ageOut("update_age", m_update_age),
    m_pitchOut("pitch", m_pitch),
    m_rollOut("roll", m_roll)

    // </rtc-template>
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  registerInPort("led", m_ledIn);
  registerInPort("rumble", m_rumbleIn);
  registerInPort("speaker", m_speakerIn);
  
  // Set OutPort buffer
  registerOutPort("button", m_buttonOut);
  registerOutPort("acceleration", m_accelerationOut);
  registerOutPort("update_age", m_update_ageOut);
  registerOutPort("pitch", m_pitchOut);
  registerOutPort("roll", m_rollOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

}

/*!
 * @brief destructor
 */
RTWiimote::~RTWiimote()
{
}



RTC::ReturnCode_t RTWiimote::onInitialize()
{
  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("beep", m_conf_varname, "beep.wav");
  
	_tprintf(_T("Start RTwiimote!\n"));

	//最初につながったリモコンを探す
	while(!remote.Connect(wiimote::FIRST_AVAILABLE)){
		_tprintf(_T("Connecting to a WiiRemote.\n"));
		Sleep(1000);
	}
	_tprintf(_T("connected.\n"));
	_tprintf(_T("Battery: %3u%%"), remote.BatteryPercent);

	//ボタンと加速度のイベント更新を伝える
	remote.SetReportType(wiimote::IN_BUTTONS_ACCEL);

  // </rtc-template>
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RTWiimote::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RTWiimote::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RTWiimote::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RTWiimote::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RTWiimote::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t RTWiimote::onExecute(RTC::UniqueId ec_id)
{
	unsigned char ledRead;
	bool rumbleRead;
	bool speakerRead;

	//バッテリーの容量を更新する
	_tprintf(_T("\b\b\b\b\b %3u%%"), remote.BatteryPercent);

	// 入力の更新がない場合はSleepする
	while(remote.RefreshState() == NO_CHANGE) {
		Sleep(1);
	}
	

	// LEDの入力ポートに新しい入力があった場合，LEDを点灯
	if(m_ledIn.isNew()) {
		m_ledIn.read();
		ledRead = m_led.data;
#ifdef DEBUG_SHOW_INPUT_DATA
		printf("led     : %x\n",ledRead);
#endif
		remote.SetLEDs(ledRead);
	}


	// バイブレータの入力ポートに新しい入力があった場合，振動させる
	if(m_rumbleIn.isNew()){
		m_rumbleIn.read();
		rumbleRead = m_rumble.data;
#ifdef DEBUG_SHOW_INPUT_DATA
		printf("rumble  : %x\n",rumbleRead);
#endif
		remote.SetRumble(rumbleRead);
	}

	// スピーカの入力ポートに新しい入力があった場合，スピーカから音を出す
	if(m_speakerIn.isNew()){
		m_speakerIn.read();
		speakerRead = m_speaker.data;
#ifdef DEBUG_SHOW_INPUT_DATA
		printf("speaker : %x\n",speakerRead);
#endif
		// ここにスピーカON/OFFの処理を書く（未対応）
	}
	
	static unsigned short old_button;

	// ボタン入力に変化があった場合のみボタン情報を出力
	if(old_button != (remote.Button.ALL & remote.Button.Bits)) {

		m_button.data			= remote.Button.Bits;

		m_buttonOut.write();	

#ifdef DEBUG_SHOW_OUTPUT_DATA
		printf("button : %x\n",(remote.Button.ALL & remote.Button.Bits));
#endif
	}

	old_button = remote.Button.ALL & remote.Button.Bits;



	// 加速度情報を出力
	m_acceleration.data.length(3);
	m_acceleration.data[0]	= remote.Acceleration.X;
	m_acceleration.data[1]	= remote.Acceleration.Y;
	m_acceleration.data[2]	= remote.Acceleration.Z;

	m_accelerationOut.write();
		
#ifdef DEBUG_SHOW_OUTPUT_DATA
	printf("AcceX : %f\n",remote.Acceleration.X);
	printf("AcceY : %f\n",remote.Acceleration.Y);
	printf("AcceZ : %f\n",remote.Acceleration.Z);
#endif


	// その他情報を出力
	m_update_age.data		= remote.Acceleration.Orientation.UpdateAge;
	m_pitch.data			= remote.Acceleration.Orientation.Pitch;
	m_roll.data				= remote.Acceleration.Orientation.Roll;

	m_update_ageOut.write();
	m_pitchOut.write();
	m_rollOut.write();
		
#ifdef DEBUG_SHOW_OUTPUT_DATA
	printf("Update Age : %d\n",remote.Acceleration.Orientation.UpdateAge);
	printf("Pitch      : %f\n",remote.Acceleration.Orientation.Pitch);
	printf("Roll       : %f\n",remote.Acceleration.Orientation.Roll);
#endif


  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RTWiimote::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RTWiimote::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RTWiimote::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RTWiimote::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RTWiimote::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void RTWiimoteInit(RTC::Manager* manager)
  {
    coil::Properties profile(rtwiimote_spec);
    manager->registerFactory(profile,
                             RTC::Create<RTWiimote>,
                             RTC::Delete<RTWiimote>);
  }
  
};


