#ifndef avradc_h
#define avradc_h

#include <avr/io.h>

//! ADC multiplexer selection for channel U sampling.
#define ADC_MUX_U           0x0

//! ADC multiplexer selection for channel V sampling.
#define ADC_MUX_V           0x1

//! ADC multiplexer selection for channel W sampling.
#define ADC_MUX_W           0x2

//! ADC multiplexer selection or speed reference sampling.
#define ADC_MUX_SPEED_REF   0x4

//! ADC multiplexer selection for current sampling.
#define ADC_MUX_CURRENT     0x3

//! ADC multiplexer selection for reference voltage sampling.
#define ADC_MUX_REF_VOLTAGE 0x5

//! ADC reference channel selection.
#define ADC_REF_CHANNEL                 ((0 << REFS1) | (0 << REFS0))

//! ADC result alignment for BEMF measurement.
#define ADC_RES_ALIGNMENT_BEMF          (1 << ADLAR)

//! ADC result alignment for speed reference measurement.
#define ADC_RES_ALIGNMENT_SPEED_REF     (1 << ADLAR)

//! ADC result alignment for CURRENT measurement.
#define ADC_RES_ALIGNMENT_CURRENT       (1 << ADLAR)

//! ADC result alignment for reference voltage measurement.
#define ADC_RES_ALIGNMENT_REF_VOLTAGE   (1 << ADLAR)

//! ADMUX register value for channel U sampling.
#define ADMUX_U             (ADC_REF_CHANNEL | ADC_RES_ALIGNMENT_BEMF | ADC_MUX_U)

//! ADMUX register value for channel V sampling.
#define ADMUX_V             (ADC_REF_CHANNEL | ADC_RES_ALIGNMENT_BEMF | ADC_MUX_V)

//! ADMUX register value for channel W sampling.
#define ADMUX_W             (ADC_REF_CHANNEL | ADC_RES_ALIGNMENT_BEMF | ADC_MUX_W)

//! ADMUX register value for speed reference sampling.
#define ADMUX_SPEED_REF     (ADC_REF_CHANNEL | ADC_RES_ALIGNMENT_SPEED_REF | ADC_MUX_SPEED_REF)

//! ADMUX register value for current sampling.
#define ADMUX_CURRENT       (ADC_REF_CHANNEL | ADC_RES_ALIGNMENT_CURRENT | ADC_MUX_CURRENT)

//! ADMUX register value for reference voltage sampling.
#define ADMUX_REF_VOLTAGE   (ADC_REF_CHANNEL | ADC_RES_ALIGNMENT_REF_VOLTAGE | ADC_MUX_REF_VOLTAGE)

//! ADC clock prescaled by 8 value.
#define ADC_PRESCALER_8     ((0 << ADPS2) | (1 << ADPS1) | (1 << ADPS0))

//! ADC clock prescaled by 8 value.
#define ADC_PRESCALER_16     ((1 << ADPS2) | (0 << ADPS1) | (0 << ADPS0))

//! ADC prescaler used.
#define ADC_PRESCALER       ADC_PRESCALER_8

//! ADC trigger source.
#define ADC_TRIGGER_SOURCE  ((1 << ADTS2) | (0 << ADTS1) | (0 << ADTS0))

//! Current measurement shunt value in milliOhm.
#define SHUNT_RESISTANCE          220

//! The ADC resolution used.
#define ADC_RESOLUTION   256

//! Zero-cross threshold.
#define ADC_ZC_THRESHOLD 0x98


//! Array of ADC channel selections for each commutation step.
unsigned char ADMUXTable[6];


#endif
