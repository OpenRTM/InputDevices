// -*- C++ -*-
/*!
 * @file  RTWiimote.h
 * @brief WiiリモコンのRTコンポーネント
 * @date  $Date$
 *
 * $Id$
 */

#ifndef RTWIIMOTE_H
#define RTWIIMOTE_H

#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/BasicDataTypeSkel.h>

#include "wiimote.h" 

// Service implementation headers
// <rtc-template block="service_impl_h">

// </rtc-template>

// Service Consumer stub headers
// <rtc-template block="consumer_stub_h">

// </rtc-template>

using namespace RTC;

/*!
 * @class RTWiimote
 * @brief WiiリモコンのRTコンポーネント
 *
 */
class RTWiimote
  : public RTC::DataFlowComponentBase
{
 public:
  /*!
   * @brief constructor
   * @param manager Maneger Object
   */
  RTWiimote(RTC::Manager* manager);

  /*!
   * @brief destructor
   */
  ~RTWiimote();

  // <rtc-template block="public_attribute">
  
  // </rtc-template>

  // <rtc-template block="public_operation">
  
  // </rtc-template>

  /*!
   *
   * The initialize action (on CREATED->ALIVE transition)
   * formaer rtc_init_entry() 
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onInitialize();

  /***
   *
   * The finalize action (on ALIVE->END transition)
   * formaer rtc_exiting_entry()
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onFinalize();

  /***
   *
   * The startup action when ExecutionContext startup
   * former rtc_starting_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onStartup(RTC::UniqueId ec_id);

  /***
   *
   * The shutdown action when ExecutionContext stop
   * former rtc_stopping_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onShutdown(RTC::UniqueId ec_id);

  /***
   *
   * The activated action (Active state entry action)
   * former rtc_active_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onActivated(RTC::UniqueId ec_id);

  /***
   *
   * The deactivated action (Active state exit action)
   * former rtc_active_exit()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onDeactivated(RTC::UniqueId ec_id);

  /***
   *
   * The execution action that is invoked periodically
   * former rtc_active_do()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onExecute(RTC::UniqueId ec_id);

  /***
   *
   * The aborting action when main logic error occurred.
   * former rtc_aborting_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onAborting(RTC::UniqueId ec_id);

  /***
   *
   * The error action in ERROR state
   * former rtc_error_do()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onError(RTC::UniqueId ec_id);

  /***
   *
   * The reset action that is invoked resetting
   * This is same but different the former rtc_init_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onReset(RTC::UniqueId ec_id);
  
  /***
   *
   * The state update action that is invoked after onExecute() action
   * no corresponding operation exists in OpenRTm-aist-0.2.0
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onStateUpdate(RTC::UniqueId ec_id);

  /***
   *
   * The action that is invoked when execution context's rate is changed
   * no corresponding operation exists in OpenRTm-aist-0.2.0
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onRateChanged(RTC::UniqueId ec_id);


 protected:
  // <rtc-template block="protected_attribute">
  
  // </rtc-template>

  // <rtc-template block="protected_operation">
  
  // </rtc-template>

  // Configuration variable declaration
  // <rtc-template block="config_declare">
  /*!
   * スピーカをONにしたときに出したいWavファイルを選択します．
   * - Name:  conf_varname
   * - DefaultValue: beep.wav
   */
  std::string m_conf_varname;
  // </rtc-template>

  // DataInPort declaration
  // <rtc-template block="inport_declare">
  TimedOctet m_led;
  /*!
   * Wiiリモコンについている4つのLEDを点灯させます．
   * ビット操作でON/OFFします．
   */
  InPort<TimedOctet> m_ledIn;
  TimedBoolean m_rumble;
  /*!
   * 振動のON/OFFをします．
   */
  InPort<TimedBoolean> m_rumbleIn;
  TimedBoolean m_speaker;
  /*!
   * スピーカの音をON/OFFします．
   * コンフィギュレーションで音を選択します．
   */
  InPort<TimedBoolean> m_speakerIn;
  
  // </rtc-template>


  // DataOutPort declaration
  // <rtc-template block="outport_declare">
  TimedUShort m_button;
  /*!
   * 11個のボタンの情報を出力します．
   * LEFT	= 0x0001,
   * RIGHT	= 0x0002,
   * DOWN	= 0x0004,
   * UP		= 0x0008,
   * PLUS	= 0x0010,
   * TWO		= 0x0100,
   * ONE		= 0x0200,
   * _B		= 0x0400,	// ie. trigger
   * _A		= 0x0800,
   * MINUS	= 0x1000,
   * HOME	= 0x8000,
   */
  OutPort<TimedUShort> m_buttonOut;
  TimedFloatSeq m_acceleration;
  /*!
   * 3軸加速度センサの情報をX，Y，Zで出力します．
   */
  OutPort<TimedFloatSeq> m_accelerationOut;
  TimedULong m_update_age;
  /*!
   * 加速度センサが更新されている間，カウントアップします．
   */
  OutPort<TimedULong> m_update_ageOut;
  TimedFloat m_pitch;
  /*!
   * Wiiリモコンを縦に回転させたときの現在の角度が出力されます．
   */
  OutPort<TimedFloat> m_pitchOut;
  TimedFloat m_roll;
  /*!
   * Wiiリモコンを軸方向に回転させたときの角度を出力します．
   */
  OutPort<TimedFloat> m_rollOut;
  
  // </rtc-template>

  // CORBA Port declaration
  // <rtc-template block="corbaport_declare">
  
  // </rtc-template>

  // Service declaration
  // <rtc-template block="service_declare">
  
  // </rtc-template>

  // Consumer declaration
  // <rtc-template block="consumer_declare">
  
  // </rtc-template>

 private:
	wiimote remote;
  // <rtc-template block="private_attribute">
  
  // </rtc-template>

  // <rtc-template block="private_operation">
  
  // </rtc-template>

};


extern "C"
{
  void RTWiimoteInit(RTC::Manager* manager);
};

#endif // RTWIIMOTE_H
