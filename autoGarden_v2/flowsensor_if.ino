
#define FLOW_IN_PIN 32
#define FLOW_THRESHOLD 15 

int flow_isr_count;
uint32_t flow_state,prev_flow_state; //0 = WATER_STAGNANT; 1 = WATER_FLOWING
void flowsensor_setup(void)
{
  pinMode(FLOW_IN_PIN, INPUT_PULLUP);
  attachInterrupt(FLOW_IN_PIN,flow_isr,CHANGE);
}


void flow_isr(void)
{
  flow_isr_count++;
}

void flowsensor_task(void)
{
  int flow_change_bit = 0;
  if(flow_isr_count > FLOW_THRESHOLD)
  {
    flow_state = 1; //WATER_FLOWING
    flow_isr_count = 0;
  }

  else
  {
    flow_state = 0; //WATER_STAGNANT
  }

  if(prev_flow_state != flow_state)
  {
    Serial.println("Flow Sensor state");
    prev_flow_state = flow_state;
    flow_change_bit = 0x80;

    comm_update_status();
  }   
}

int flowsensor_get_state(void)
{
  return prev_flow_state;
}
