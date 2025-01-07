int START_BTN = 19;
int STOP_BTN  = 18;
int SAFETY_RLY = 5;


#define MINS_THRESH   5 //in minutes = Auto Cutoff Threshold Time

#define DELTA_MINS(start_min , end_min) ((end_min >= start_min)?(end_min - start_min):(end_min + (60-start_min)))
int motor_status = 0;

extern int tm_min;
int start_tm_min; //Store the time (minutes) during Motor Start Operation

void btn_setup(void)
{
  pinMode (START_BTN, OUTPUT);
  pinMode (STOP_BTN, OUTPUT);
  pinMode (SAFETY_RLY, OUTPUT);

  digitalWrite(SAFETY_RLY,0);
  digitalWrite(START_BTN,0);
  digitalWrite(STOP_BTN,0);
  
}

void btn_activate_start(void)
{
  digitalWrite(SAFETY_RLY,1);
  digitalWrite(START_BTN,1);
  delay(750);
  digitalWrite(START_BTN,0);
  motor_status = 1;
  start_tm_min = tm_min;
}

void btn_activate_stop(void)
{
  digitalWrite(STOP_BTN,1);
  delay(750);
  digitalWrite(STOP_BTN,0);
  digitalWrite(SAFETY_RLY,0);
  motor_status = 0;
}

void btn_task(void)
{
  if(motor_status)
  {
    if(DELTA_MINS(tm_min, start_tm_min) > MINS_THRESH)
    {
      telegram_send_SafetyStop();
      btn_activate_stop();
    }
  }
}
