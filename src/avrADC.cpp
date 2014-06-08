#include "avrADC.h"



/*! \brief Initializes the AD-converter.
*
*  This function initializes the AD-converter and makes a reading of the external
*  reference voltage.
*/


//! ADC reading of the known external reference voltage.
static volatile unsigned char referenceVoltageADC;


static void initADC(void)
{
  // Disable digital input buffers on ADC channels.
  DIDR0 = (1 << ADC4D) | (1 << ADC3D) | (1 << ADC2D) | (1 << ADC1D) | (1 << ADC0D);

  // First make a measurement of the external reference voltage.
  ADMUX = ADMUX_REF_VOLTAGE;
  ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADIF) | (ADC_PRESCALER_16);
  while (ADCSRA & (1 << ADSC))
  { }

  referenceVoltageADC = ADCH;

  // Initialize the ADC for autotriggered operation on PWM timer overflow.
  ADCSRA = (1 << ADEN) | (0 << ADSC) | (1 << ADATE) | (1 << ADIF) | (0 << ADIE) | ADC_PRESCALER_8;
  ADCSRB = ADC_TRIGGER_SOURCE;
}


/*! \brief Initializes the analog comparator.
 *
 *  This function initializes the analog comparator for overcurrent detection.
 */
static void initAnalogComparator(void)
{

#ifdef ANALOG_COMPARATOR_ENABLE
  // Enable analog comparator interrupt on rising edge.
  ACSR = (0 << ACBG) | (1 << ACI) | (1 << ACIE) | (1 << ACIS1) | (1 << ACIS0);
#endif

}

static void initADMUXTable(void)

{
  ADMUXTable[0] = ADMUX_U;
  ADMUXTable[1] = ADMUX_V;
  ADMUXTable[2] = ADMUX_W;
  ADMUXTable[3] = ADMUX_U;
  ADMUXTable[4] = ADMUX_V;
  ADMUXTable[5] = ADMUX_W;

}

