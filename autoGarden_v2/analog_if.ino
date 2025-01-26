int ANALOG_PIN = 34;
#define CAL_FACTOR 0.0012 //Derived out of experiments with current meter
uint32_t adc_value;

void analog_task(void)
{
  adc_value = analogRead(ANALOG_PIN);
}