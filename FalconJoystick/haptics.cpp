#include "haptics.h"
#include <windows.h>
#include <math.h>

// Continuous servo callback function
HDLServoOpExitCode ContactCB(void* pUserData)
{
  // Get pointer to haptics object
  HapticsClass* haptics = static_cast< HapticsClass* >( pUserData );

  // Get current state of haptic device
  hdlToolPosition(haptics->m_positionServo);
  hdlToolButton(&(haptics->m_buttonServo));

  // Call the function that does the heavy duty calculations.
  haptics->calculate();

  // Send forces to device
  hdlSetToolForce(haptics->m_forceServo);

  // Make sure to continue processing
  return HDL_SERVOOP_CONTINUE;
}

// On-demand synchronization callback function
HDLServoOpExitCode GetStateCB(void* pUserData)
{
  // Get pointer to haptics object
  HapticsClass* haptics = static_cast< HapticsClass* >( pUserData );

  // Call the function that copies data between servo side 
  // and client side
  haptics->synch();

  // Only do this once.  The application will decide when it
  // wants to do it again, and call CreateServoOp with
  // bBlocking = true
  return HDL_SERVOOP_EXIT;
}

// Constructor--just make sure needed variables are initialized.
HapticsClass::HapticsClass()
  : m_deviceHandle(HDL_INVALID_HANDLE),
	m_servoOp(HDL_INVALID_HANDLE),
	m_inited(false),
	m_currentPosition(VECTOR_SIZE,0.0)
{
  for (int i = 0; i < ARRAY_SIZE; i++) {
	m_positionServo[i] = 0;
	m_forceServo[i] = 0;
	m_forceRef[i] = 0;
  }
}

// Destructor--make sure devices are uninited.
HapticsClass::~HapticsClass()
{
  cleanup();
}



//void HapticsClass::init(double springK, double dampingC)
void HapticsClass::init()
{
  HDLError err = HDL_NO_ERROR;

  // Passing "DEFAULT" or 0 initializes the default device based on the
  // [DEFAULT] section of HDAL.INI.   The names of other sections of HDAL.INI
  // could be passed instead, allowing run-time control of different devices
  // or the same device with different parameters.  See HDAL.INI for details.
  m_deviceHandle = hdlInitNamedDevice("DEFAULT");
  testHDLError("hdlInitDevice");

  if (m_deviceHandle == HDL_INVALID_HANDLE)
    {
	  std::cerr << "Device Failure: Could not open device" << std::endl;
	  exit(0);
    }

  // Now that the device is fully initialized, start the servo thread.
  // Failing to do this will result in a non-funtional haptics application.
  hdlStart();
  testHDLError("hdlStart");

  // Set up callback function
  m_servoOp = hdlCreateServoOp(ContactCB, this, bNonBlocking);
  if (m_servoOp == HDL_INVALID_HANDLE)
    {
	  std::cerr << "Device Failure: Invalid servo op handle" << std::endl;
    }
  testHDLError("hdlCreateServoOp");

  // Make the device current.  All subsequent calls will
  // be directed towards the current device.
  hdlMakeCurrent(m_deviceHandle);
  testHDLError("hdlMakeCurrent");

  arrayToVectorCoppy(m_positionServo, m_currentPosition);

  m_inited = true;

  Sleep(100);

  if (!isDeviceCalibrated()) {
	std::cerr << "Please home the device by extending" << std::endl;
	std::cerr << "then pushing the arms all the way in." << std::endl;
	std::cerr << "Not Homed" << std::endl;
  }
}

// uninit() undoes the setup in reverse order.  Note the setting of
// handles.  This prevents a problem if uninit() is called
// more than once.
void HapticsClass::cleanup()
{
  if (m_servoOp != HDL_INVALID_HANDLE)
    {
	  hdlDestroyServoOp(m_servoOp);
	  m_servoOp = HDL_INVALID_HANDLE;
    }
  hdlStop();
  if (m_deviceHandle != HDL_INVALID_HANDLE)
    {
	  hdlUninitDevice(m_deviceHandle);
	  m_deviceHandle = HDL_INVALID_HANDLE;
    }
  m_inited = false;
}

// This is a simple function for testing error returns.  A production
// application would need to be more sophisticated than this.
void HapticsClass::testHDLError(const char* str)
{
  HDLError err = hdlGetError();
  if (err != HDL_NO_ERROR)
    {
	  std::cerr << "HDAL ERROR: " << str << std::endl;
	  abort();
    }
}

// This is the entry point used by the application to synchronize
// data access to the device.  Using this function eliminates the 
// need for the application to worry about threads.
void HapticsClass::synchFromServo()
{
  hdlCreateServoOp(GetStateCB, this, bBlocking);
}

// GetStateCB calls this function to do the actual data movement.
void HapticsClass::synch()
{
  arrayToVectorCoppy(m_positionServo,m_currentPosition);
  m_buttonApp = m_buttonServo;
}


// Here is where the heavy calculations are done.  This function is
// called from ContactCB to calculate the forces based on current
// cursor position and cube dimensions.  A simple spring model is
// used.
void HapticsClass::calculate()
{

  // Skip the whole thing if not initialized
  if (!m_inited) return;

  arrayCoppy(m_forceRef,m_forceServo);
  //  std::cout << "force seervo : " << m_forceServo[0] << " " << m_forceServo[1] << std::endl;

}


// For this application, the only device status of interest is the
// calibration status.  A different application may want to test for
// HDAL_UNINITIALIZED and/or HDAL_SERVO_NOT_STARTED
bool HapticsClass::isDeviceCalibrated()
{
  unsigned int state = hdlGetState();

  return ((state & HDAL_NOT_CALIBRATED) == 0);
}
