#include <brushless.h>

// variables
int starting;
int startupState;
uint32_t startTime;
uint32_t TotStartupTime;
// int commandRead;
// Command latestCommand;
ramp rampPWMDuty;
ramp rampAutomaFrequencyA;
ramp rampAutomaFrequencyB;

// brushless::brushless() {
// debug("Entering constructor");
void
ramp_init ()
{
  rampPWMDuty.gain = 20;
  rampPWMDuty.offset = 1;
  rampPWMDuty.currentValue = 0;
  rampPWMDuty.end = 98;

  rampAutomaFrequencyA.gain = 50;
  rampAutomaFrequencyA.offset = DEFAULT_T1_INIT_FREQUENCY;
  rampAutomaFrequencyA.currentValue = 0;
  rampAutomaFrequencyA.end = 250;

  rampAutomaFrequencyB.gain = 150;
  rampAutomaFrequencyB.offset = DEFAULT_T1_INIT_FREQUENCY;
  rampAutomaFrequencyB.currentValue = 0;
  rampAutomaFrequencyB.end = 3800;
}

int
init (void)
{
  // int brushless::init(void) {
  pins_init ();
  registerISRCallback (pins_commuteDriveTable);
  pwmInit ();
  setStartupState (startupState_MotorOff);
  startupState = startupState_MotorOff;
  // allocate buffer for char array
  //  latestCommand = (Command)malloc(sizeof(_command));
}
int
getStartupOpenLoopValue (ramp ramp)
{
  // int brushless::getStartupOpenLoopValue(ramp ramp) {

  // proportional open loop controller
  // y = K * t + y0 [Hz]

  float ang
      = (float)ramp.gain * (float)(timeClock_getMillis () - startTime) * 0.001;
  int OpenLoopValue = ang + ramp.offset;

  // debug("rampTime: %u, gain: %d, offset: %d, value: %d", (unsigned int)
  // (avrClock() - startTime), ramp.gain, ramp.offset, OpenLoopValue); debug
  // ("%d",OpenLoopValue);
  return OpenLoopValue;
}

void
setStartupState (uint8_t state)
{
  // void brushless::setStartupState(int state) {

  switch (state)
    {

      // start pwm signal
    case startupState_MotorOff:
      stopISR ();
      pwmSetDuty (DEFAULT_T0_INIT_DUTY);
      // probabily missing pins_setDriveOpenInverter()
      startupState = startupState_MotorInit;
      // return  0;

    case startupState_MotorInit:
      pwmStart ();

      startupState = startupState_PWMStarted;
      //  return  0;

      // Stop motor for aligning rotor
    case startupState_PWMStarted:

      pins_setDriveState (DEFAULT_INITIAL_STATE);
      // debug("PWM Started - Commencing rotor alignment");
      startTime = timeClock_getMillis ();
      TotStartupTime = timeClock_getMillis ();
      startupState = startupState_RotorAligned;
      //  return  0;

      // start increasing pwm duty without changing automa state
    case startupState_RotorAligned:

      pwmSetDuty (getStartupOpenLoopValue (rampPWMDuty));
      // keep rotor fixed, until pwm is 50% of end duty

      if (pwmGetDuty () >= 60)
        {

          startupState = startupState_SetupAutomaRampA;
        }
      //  return 0;

      // start automa
    case startupState_SetupAutomaRampA:

      // start drive sequence
      startISR (DEFAULT_T1_INIT_FREQUENCY);

      // set ramp duty offset and reset clock
      rampPWMDuty.offset = pwmGetDuty ();
      startTime = timeClock_getMillis ();

      // debug("Starting Automa Ramp A");
      startupState = startupState_AutomaRampA;
      //  return  0;

      // increase frequency of automa and pwm duty until max duty value is
      // reached
    case startupState_AutomaRampA:
      // raise duty until end duty
      if (pwmGetDuty () < rampPWMDuty.end)
        {
          pwmSetDuty (getStartupOpenLoopValue (rampPWMDuty));
        }
      // raise automa frequency until end frequency
      if (getISRFrequency () < rampAutomaFrequencyA.end)
        {
          setISRFrequency (getStartupOpenLoopValue (rampAutomaFrequencyA));
        }
      // set next state once pwm and duty reach end value
      if (pwmGetDuty () >= rampPWMDuty.end
          && getISRFrequency () >= rampAutomaFrequencyA.end)
        {
          startupState = startupState_SetupAutomaRampB;
        }
      //  return  0;

      /////////////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////

      // increase automa frequency until max automa frequency of ramp A
    case startupState_SetupAutomaRampB:

      // set pwm offset and reset clock
      rampAutomaFrequencyB.offset = getISRFrequency ();
      setISRFrequency (rampAutomaFrequencyB.offset + 1);
      startTime = timeClock_getMillis ();

      // debug("Starting Automa Ramp B");
      startupState = startupState_AutomaRampB;
      //   return  0;

      /////////////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////

      // continue increasing automa frequency until max automa frequency of
      // ramp B
    case startupState_AutomaRampB:
      setISRFrequency (getStartupOpenLoopValue (rampAutomaFrequencyB));
      if (getISRFrequency () >= rampAutomaFrequencyB.end)
        {

          startupState = startupState_StartupFinished;
        }
      //   return  0;

      // finish
    case startupState_StartupFinished:
      // reduce duty for steady speed
      // pwmSetDuty(90);
      // debug("Startup Finished. Time is[ms]: %u", (int)(avrClock() -
      // TotStartupTime));
      startupState = startupState_MotorOff;
      //  return  1;

      // default:
      //  log_err("invalid state:%d", state);
      //  return -1;
    }
}
int
startupCallback ()
{
  // int brushless::startupCallback() {

  // debug("Startup state is: %d",startupState);

  //  if (setStartupState(startupState) == 1)
  //    starting = 0;

  return 0;
}

int
iterate ()
{
  // int brushless::iterate() {

  // parse latestcommand
  /*  if (commandRead == 0) {
      parseCommand(latestCommand);
      commandRead = 1;
    }
  */
  if (starting == 1)
    {
      setStartupState (startupState);
      starting = 0;
    }
  else
    {

      // future cltf goes here
    }
  return 0;
}

void
angSpeed (uint8_t val)
{
  // void brushless::angSpeed(uint8_t val) {
  unsigned int RPM_e = floor ((getISRFrequency () / NUM_DRIVE_STATES) * 60);
  unsigned int RPM_m = floor (RPM_e / (NUM_POLES / 2));
  int rads_e = floor (RPM_e / 60 * 2 * M_PI);
  int rads_m = floor (RPM_m / 60 * 2 * M_PI);
  // log_info("RPM elettrici:%u, RPM meccanici:%u, RAD/s elettrici:%d, RAD/s
  // meccanici:%d", RPM_e, RPM_m, rads_e, rads_m); return 0;
}
int
setStartupFreqEnd (int val)
{
  // int brushless::setStartupFreqEnd (int val) {
  if (val < 0 || val > 30000)
    {
      // log_err("invalid start freq:%d", val);
    }
  return -1;

  rampAutomaFrequencyA.end = val; // end value in Hz
  return 0;
}

int
setStartupFreqGain (int val)
{
  // int brushless::setStartupFreqGain (int val) {
  if (val < 0 || val > 2500)
    {
      // log_err("invalid start gain:%d", val);
    }
  return -1;

  rampAutomaFrequencyA.gain = val; // end value in Hz
  return 0;
}
void
manualMode (uint8_t val)
{
  // void brushless::manualMode(uint8_t val) {
  pwmStart ();
  startISR (300);
  pins_setDriveState (DEFAULT_INITIAL_STATE);
  pwmSetDuty (90);
  // return 0;
}

// brushless::~brushless() {
// }
