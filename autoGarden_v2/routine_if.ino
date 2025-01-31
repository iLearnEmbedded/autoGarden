#include <EEPROM.h>

#define EEPROM_SIZE 20  // Define EEPROM size in bytes

#define MINS_THRESH_ADDR 0
#define ROUTINE_HH_ADDR  1

int mins_thresh = 5;
int routine_hh = 7;  //watering daily at 7 A.M in the morning
int routine_setup_done;


extern int tm_hour;
extern int tm_date;

void routine_setup(void)
{
  int eep_readval;
  if (!EEPROM.begin(EEPROM_SIZE)) 
    {
        Serial.println("Failed to initialize EEPROM");   
        return;     
    }  

  eep_readval = EEPROM.read(MINS_THRESH_ADDR);

  if(eep_readval > 30) 
  {
    EEPROM.write(MINS_THRESH_ADDR, mins_thresh);  // Store at address 0
    EEPROM.commit();         // Save changes to Flash
  }

  mins_thresh = eep_readval;

  eep_readval = EEPROM.read(ROUTINE_HH_ADDR);

  if(eep_readval > 12)
  {
    EEPROM.write(ROUTINE_HH_ADDR, routine_hh);  // Store at address 1
    EEPROM.commit();         // Save changes to Flash
  }

  routine_hh = eep_readval;

  routine_setup_done = 1;
}

void routine_write(int value)
{
  int eep_readval;
  if(routine_setup_done)
  {
    eep_readval = EEPROM.read(ROUTINE_HH_ADDR);
    if(value != eep_readval)
    {
      EEPROM.write(ROUTINE_HH_ADDR, value);  // Store at address 
      EEPROM.commit();         // Save changes to Flash
    }  

    routine_hh = value;
  }  
}

int routine_read(void)
{
  return routine_hh;
}

void thresh_write(int value)
{
  int eep_readval;
  if(routine_setup_done)
  {
    eep_readval = EEPROM.read(MINS_THRESH_ADDR);
    if(value != eep_readval)
    {
      EEPROM.write(MINS_THRESH_ADDR, value);  // Store at address 
      EEPROM.commit();         // Save changes to Flash
    }
    mins_thresh = value;
  }  
}

int thresh_read(void)
{
  return mins_thresh;
}


void routine_loop(void)
{
  static int routine_pending;
  static int prev_date;
  
  if((!routine_setup_done) || (!time_available))
    return;

  if(prev_date != tm_date)
  {
    prev_date = tm_date;
    routine_pending = 1;
  }

  if((routine_pending) && (tm_hour >= routine_hh))
  {
    routine_pending = 0;
    btn_activate_start();    
    comm_send_message("Routine Watering Start");
    delay(5000);
    comm_update_status();
  }
}
