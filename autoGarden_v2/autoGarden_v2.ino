
void setup() 
{ 
  btn_setup();
  comm_setup();  
  flowsensor_setup();
  time_setup();  
  routine_setup();
}

void loop() 
{
    comm_task();      
    flowsensor_task();    
    time_task();
    btn_task(); 
    analog_task();    
    routine_loop(); 
}
