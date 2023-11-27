/* 
 * File:   main.c
 * Author: Pranav Ahuja
 * DESC: 
 * Created on October 30, 2023, 6:38 PM
 */

#include "main.h"

#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT disabled)

unsigned short speed;
unsigned char to_print[3];
unsigned char *gears[] = {"GN", "G1", "G2", "G3", "G4", "GR"};
unsigned char event[3] = "ON";
unsigned char screens_flag = DASHBOARD_SCREEN;
unsigned char first_enter = 0, pos = -1, event_num = -1;
unsigned char time[9];
unsigned char log[11];
unsigned char key;
unsigned char reset_flag = RESET_PASSWORD;
unsigned char password_address = 0x00;
unsigned char sys_pass[5];
unsigned char uart_ch;
unsigned char on_hold = 0;
unsigned char select_pos = 0;
unsigned char change_password_status = 0;
char first_time = 1;
unsigned char in_subscreen = 0;
unsigned char overflow_data = 0;

//Configuration function
static void init_config(void) 
{
    //Initialize the peripherals
    init_clcd();
    init_adc();
    init_digital_keypad();  
    
    i2c_init(100000);
    init_rtc();    
    init_uart(9600); 
    
    clcd_print(event, LINE2(9));
    GIE = 1;        
    PEIE = 1;
}

void main(void) {
    init_config();            
    unsigned short adc_val;
    static unsigned gear_sel = 0;
    write_at24c04_string(password_address, "1010");

    while (1) {
        //write the loop code                   
        
        key = read_digital_keypad(DK_STATE_MODE);
        
        for(int wait = 400; wait--;  );
        
        if(key == DKS4 || key == DKS5)
        {            
            if(screens_flag == DASHBOARD_SCREEN)
            { 
                init_timer0();
                reset_flag = RESET_PASSWORD;
                screens_flag = LOGIN_SCREEN; 
                
                clcd_specific_instruction(CLCD_CLEAR_DISPLAY);                       
                clcd_print(" ENTER PASSWORD ", LINE1(0));
                
                clcd_goto_pos(CLCD_POS_FOR_PASSWORD);
                clcd_specific_instruction(CLCD_DISPLAY_ON_CURSOR_BLINKING);   
                
                __delay_us(100);
            }              
        }        
        else if(key == DKS1)
        {
            strcpy(event, " C");
            eeprom_log_data(event, speed);
        }
        else if(key == DKS2)
        {
            if(screens_flag == DASHBOARD_SCREEN && gear_sel <= 5)
            {
                strcpy(event, gears[gear_sel]);
                gear_sel++;
                eeprom_log_data(event, speed);
            }
        }
        else if(key == DKS3)
        {
            if(screens_flag == DASHBOARD_SCREEN && gear_sel > 0)
            {
                gear_sel--;
                strcpy(event, gears[gear_sel]);      
                eeprom_log_data(event, speed);
            }
        }
        else if(key == LPSW5 && screens_flag == MENU_SCREEN)
        {            
            in_subscreen = 1;
            clcd_specific_instruction(CLCD_CLEAR_DISPLAY);
            switch(select_pos)
            {
                case 0:                    
                    screens_flag = VIEW_LOGS;
                    break;
                case 1:
                    screens_flag = DOWNLOAD_LOGS;                    
                    break;
                case 2:
                    screens_flag = CLEAR_LOGS;
                    break;
                case 3:
                    screens_flag = CHANGE_PASSWORD;     
                    first_time = 1; 
                    reset_timer0();
                    break;
                case 4:
                    screens_flag = SET_TIME;
                    reset_flag = RESET_TIME;
                    break;
            }
        }        
        else if(key == LPSW4)
        {                    
            if(screens_flag == DOWNLOAD_LOGS || (screens_flag == VIEW_LOGS || (screens_flag == CLEAR_LOGS || (screens_flag == CHANGE_PASSWORD || screens_flag == SET_TIME))))
            {
                in_subscreen = 0;
                clcd_specific_instruction(CLCD_CLEAR_DISPLAY);
                screens_flag = MENU_SCREEN;
            }
            else if(screens_flag == MENU_SCREEN)
            {
                clcd_specific_instruction(CLCD_CLEAR_DISPLAY);
                screens_flag = DASHBOARD_SCREEN;   
                __delay_ms(500);
            }            
        }
        
        switch(screens_flag)
        {
            case DASHBOARD_SCREEN:
                adc_val = read_adc();
                adc_to_speed(adc_val);                                                
                default_screen(event, speed);
                break;
            case LOGIN_SCREEN:                
                login_screen(reset_flag, key);                               
                break;
            case MENU_SCREEN: 
                clcd_specific_instruction(CLCD_DISPLAY_ON_CURSOR_OFF);
                menu_screen(key);
                break;
            case VIEW_LOGS:
                view_logs(key, reset_flag);
                break;
            case DOWNLOAD_LOGS:                
                download_logs(key);                
                
                break;
            case CLEAR_LOGS:
                clear_logs();
                break;
            case CHANGE_PASSWORD:   
                change_password_status = change_password(key);
                if(change_password_status == SUCCESSFUL_CHANGE)
                {                    
                    clcd_print("PASSWORD CHANGED", LINE1(0));
                    clcd_print("SUCCESSFULLY", LINE2(0));
                    
                    __delay_ms(1000);
                    clcd_specific_instruction(CLCD_CLEAR_DISPLAY);
                    screens_flag = MENU_SCREEN;
                }
                else if(change_password_status == CHANGE_FAILED)
                {
                    clcd_print(" CHANGE  FAILED ", LINE1(0));
                    clcd_print("  RETRY PLEASE  ", LINE2(0));
                    first_time = 1;
                    change_password_status = 0;
                    __delay_ms(1000);
                    clcd_specific_instruction(CLCD_CLEAR_DISPLAY);
                }          
                
                break;
            case SET_TIME:                
                set_time(key, reset_flag);                
                break;                        
        }            
        reset_flag = RESET_NOTHING;
    }
    return;
}

void adc_to_speed(unsigned short adc_val)
{
    speed = adc_val / 10.23;
    if(speed > 99)
        speed = 99;
}

void eeprom_log_data(unsigned char *event, unsigned char speed)
{
    get_time();
    
    log[0] = time[0];
    log[1] = time[1];
    log[2] = time[3];
    log[3] = time[4];
    log[4] = time[6];
    log[5] = time[7];
    
    strncpy(&log[6], event, 2);
    
    log[8] = (speed / 10) + '0';
    log[9] = (speed % 10) + '0';
    
    log[10] = '\0';
    
    eeprom_data_to_memory();
}

void eeprom_data_to_memory()
{
    char address = 0x05;
    pos++;
    if(pos == 10)
    {
        pos = 0;
        overflow_data = 1;
    }        
    
    address = (pos * 10) + 5;                
                
    if(event_num < 9)
    {
        event_num++;
    }
    write_at24c04_string(address, log);
}
