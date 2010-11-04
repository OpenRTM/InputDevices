// Copyright 2007 Novint Technologies, Inc. All rights reserved.
// Available only under license from Novint Technologies, Inc.

// data: 2008.04.18,  modified by Shinji Kurihara

// Make sure this header is included only once
#ifndef HAPTICS_H
#define HAPTICS_H

#include <iostream>
#include <hdl/hdl.h>
#include <hdlu/hdlu.h>
#include <vector>

#define ARRAY_SIZE  3
#define VECTOR_SIZE 3

// Blocking values
const bool bNonBlocking = false;
const bool bBlocking = true;

class HapticsClass 
{

  // Define callback functions as friends
  friend HDLServoOpExitCode ContactCB(void *data);
  friend HDLServoOpExitCode GetStateCB(void *data);
  
 public:
  // Constructor
  HapticsClass();
  
  // Destructor
  ~HapticsClass();
  
  // Initialize
  //  void init(double springK, double dampingC);
  void init();
  
  // Clean up
  void cleanup();
  
  // Get position
  std::vector<double> getPosition() {
	return m_currentPosition;
  }

  // Set Force  
  void setForce(const double (&force)[ARRAY_SIZE]) {
	for (int i = 0; i < ARRAY_SIZE; i++)
	  m_forceRef[i] = force[i];
  }

  // caliculate force and position.
  void calculate();
  //--------------------------------------------------------

  // synchFromServo() is called from the application thread when it wants to exchange
  // data with the HapticClass object.  HDAL manages the thread synchronization
  // on behalf of the application.
  void synchFromServo();
  
  // Get state of device button
  // Interface function to get button state.  Only one button is used
  // in this application.
  bool isButtonDown() {
	return m_buttonApp;
  }

 private:
  
  // Get ready state of device.
  bool isDeviceCalibrated();
  
  // Move data between servo and app variables
  void synch();
  
  // Check error result; display message and abort, if any
  void testHDLError(const char* str);
  
  void arraySubstract(double (&result)[ARRAY_SIZE], double a[ARRAY_SIZE], double b[ARRAY_SIZE]) {
	for (int i = 0; i < ARRAY_SIZE; i++)
	  result[i] = a[i] - b[i];
  }

  void arrayCoppy(double from[ARRAY_SIZE], double (&to)[ARRAY_SIZE]) {
	for (int i = 0; i < ARRAY_SIZE; i++)
	  to[i] = from[i];
  }

  void arrayToVectorCoppy(double from[ARRAY_SIZE], std::vector<double>& to) {
	for (int i = 0; i < ARRAY_SIZE; i++)
	  to[i] = from[i];
  }


  void vectorToArrayCoppy(std::vector<double> from, double (&to)[ARRAY_SIZE]) {
	for (int i = 0; i < ARRAY_SIZE; i++)
	  to[i] = from[i];
  }


  // Nothing happens until initialization is done
  bool m_inited;
  
  std::vector<double> m_currentPosition;

  // Variables used only by servo thread
  double m_positionServo[ARRAY_SIZE];
  bool   m_buttonServo;
  double m_forceServo[ARRAY_SIZE];
  double m_forceRef[ARRAY_SIZE];
  

  // Variables used only by application thread
  bool   m_buttonApp;

  // Handle to device
  HDLDeviceHandle m_deviceHandle;

  // Handle to Contact Callback 
  HDLServoOpExitCode m_servoOp;

};

#endif // HAPTICS_H
