/* 
 * File:   lcd_screen.c
 * Author: Pranav Ahuja
 * DESC: 
 * Created on November 1, 2023, 9:53 PM
 */

#include "main.h"
extern unsigned char to_print[3];
extern unsigned char time[9];
extern unsigned char password_address;
extern unsigned char screens_flag;
extern unsigned char sys_pass[];
extern unsigned char menu_iter;
extern unsigned char on_hold;
extern unsigned char menu_event[16];
extern unsigned char select_pos;
extern unsigned char event_num, pos;
extern unsigned char change_password_status;
extern char first_password[5], second_password[5];
extern unsigned char overflow_data;

char *menu_items[] = {"VIEW LOGS", "DOWNLOAD LOGS", "CLEAR LOGS", "CHANGE PASSWORD", "SET TIME"};
unsigned char locked = 0;
unsigned char menu_subscreen = MENU_SCREEN;
int time_pos = 2;
unsigned char clock_reg[3];
unsigned char clcd_pos = 0xCB;


void default_screen(unsigned char event[3], unsigned short speed)
{    
    clcd_specific_instruction(CLCD_DISPLAY_ON_CURSOR_OFF);
    clcd_print("TIME     EVT  SP", LINE1(0)); 
    get_time();
    clcd_print(time, LINE2(0));
    
    clcd_print(event, LINE2(9));
    
    //adc to char
    if((!strcmp(event, "ON")) || (!strcmp(event, " C")))
        speed = 0;
    to_print[0] = (speed / 10) + '0';
    to_print[1] = (speed % 10) + '0';
    to_print[2] = '\0';
    clcd_print(to_print, LINE2(14));
}

void login_screen(unsigned char reset_flag, unsigned char key)
{    
    static char attempt = 3, i = 0;
    char user_pss[PASSWORD_LENGTH+1];
    
    if(locked == 0)
    {
        if(reset_flag == RESET_PASSWORD)
        {
            attempt = 3;
            user_pss[0] = '\0';
            user_pss[1] = '\0';
            user_pss[2] = '\0';
            user_pss[3] = '\0';
//            user_pss[4] = '\0';

            i = 0;
            key = 0xFF;  
        }
        else {
            if(key == DKS4 && i < 4)
                {
                    user_pss[i] = '1';            
                    clcd_putch('*', LINE2(6 + i));
                    i++;
                    reset_timer0();
                }
            else if(key == DKS5 && i < 4)
                {
                    user_pss[i] = '0';
                    clcd_putch('*', LINE2(6 + i));
                    i++;
                    reset_timer0();
                }

            if(i == 4)
            {               
                for(int k = 0; k < PASSWORD_LENGTH; k++)
                {
                    sys_pass[k] = random_read_at24c04(k);                
                }
                user_pss[4] = '\0';
                sys_pass[4] = '\0';
                if(attempt > 0)
                {
                    if(!strcmp(user_pss, sys_pass))
                    {
                        screens_flag = MENU_SCREEN;   
                        clcd_specific_instruction(CLCD_CLEAR_DISPLAY);
                        clcd_specific_instruction(CLCD_DISPLAY_ON_CURSOR_OFF);
                        TMR0IE = 0;
                    }
                    else{

                        attempt--;
                        i = 0;
                        clcd_print("WRONG Password", LINE2(0));
                        for(long int i = 100000; i--; );
                        clcd_print("                ", LINE2(0));                    
                        clcd_goto_pos(CLCD_POS_FOR_PASSWORD);
                        clcd_specific_instruction(CLCD_DISPLAY_ON_CURSOR_BLINKING); 
                    }
                }
                if(attempt == 0)
                {
                    reset_timer0();
                    clcd_print(" Wrong Password ", LINE1(0));
                    clcd_print("Locked for 5min", LINE2(0));
                    clcd_specific_instruction(CLCD_DISPLAY_ON_CURSOR_OFF);
                    locked = 1;
                }
            }
        }
    }    
}

unsigned char menu_screen(unsigned char key)
{   
    static int menu_iter = 0;
    
    if(key == DKS5 && menu_iter < 4)
    {
        clcd_specific_instruction(CLCD_CLEAR_DISPLAY);
        menu_iter++;
        select_pos++;
        reset_timer0();
    }
    else if(key == DKS4 && menu_iter > 0)
    {
        clcd_specific_instruction(CLCD_CLEAR_DISPLAY);
        menu_iter--;
        select_pos--;
        reset_timer0();
    }
    
    if(menu_iter < 4)
    {
        clcd_putch('*', LINE1(0));
        clcd_print(menu_items[menu_iter], LINE1(1));
        clcd_print(menu_items[menu_iter + 1], LINE2(1));
    }
    if(menu_iter == 4)
    {
        clcd_putch('*', LINE2(0));
        clcd_print(menu_items[menu_iter - 1], LINE1(1));
        clcd_print(menu_items[menu_iter], LINE2(1));
        select_pos = 4;
    }
}

void view_logs(unsigned char key, unsigned char reset_shift)
{   
    static unsigned char shift;
    unsigned char log_data_view[11];
    int log_pos;
    unsigned char address_to_fetch_data;
    clcd_print("View Logs", LINE1(0));
    
    if(event_num == -1)
    {
        clcd_print("No Log Available", LINE2(0));
    }
    else{
        if(reset_shift == RESET_SHIFT)
        {
            shift = 0;
        }
        log_pos = (overflow_data) ? (pos + 1) % 10 : 0;
        
        if(key == DKS5 && shift < 9)
        {
            shift++;
            clcd_print("                ", LINE2(0));
        }
        else if(key == DKS4 && shift > 0)
        {
            shift--;
            clcd_print("                ", LINE2(0));
        }
        
        log_pos = (log_pos + shift) % 10;
        
        for(int i = 0; i< 10; i++)
        {
            address_to_fetch_data = (log_pos * 10) + 5 + i;
            log_data_view[i] = random_read_at24c04(address_to_fetch_data);
        }
                
        clcd_putch(shift + '0', LINE2(0));
        clcd_putch(' ', LINE2(1));
        clcd_putch(log_data_view[0], LINE2(2));
        clcd_putch(log_data_view[1], LINE2(3));
        clcd_putch(':', LINE2(4));
        clcd_putch(log_data_view[2], LINE2(5));
        clcd_putch(log_data_view[3], LINE2(6));
        clcd_putch(':', LINE2(7));
        clcd_putch(log_data_view[4], LINE2(8));
        clcd_putch(log_data_view[5], LINE2(9));
        clcd_putch(log_data_view[6], LINE2(11));
        clcd_putch(log_data_view[7], LINE2(12));
        clcd_putch(log_data_view[8], LINE2(14));
        clcd_putch(log_data_view[9], LINE2(15));

    }
    
}
void download_logs(unsigned char key)
{
    static unsigned char shift;
    unsigned char log_data_view[11];
    int log_pos;
    int next = -1;
    unsigned char address_to_fetch_data;
    
    clcd_print("Download Logs", LINE1(0));
    
    if(event_num == -1)
    {
        putstr("No logs Available");
    }
    else
    {
        putstr("Logs-");
        putchar("\n");
        putchar("\n");
        putchar("\r");
        
        while(next < event_num)
        {
            next++;
            log_pos = (overflow_data) ? (pos + 1) % 10 : 0;
            log_pos = (log_pos + next) % 10;
            
            for(int i = 0; i< 10; i++)
            {
                address_to_fetch_data = (log_pos * 10) + 5 + i;
                log_data_view[i] = random_read_at24c04(address_to_fetch_data);
            }
            
            putchar(next + '0');
            putchar(' ');
            putchar(log_data_view[0]);
            putchar(log_data_view[1]);
            putchar(':');
            putchar(log_data_view[2]);
            putchar(log_data_view[3]);
            putchar(':');
            putchar(log_data_view[4]);
            putchar(log_data_view[5]);
            putchar(' ');
            putchar(log_data_view[6]);
            putchar(log_data_view[7]);
            putchar(' ');
            putchar(log_data_view[8]);
            putchar(log_data_view[9]);
            putchar('\n');
            putchar('\r');            
        }     
    }
}
void clear_logs()
{
    clcd_print("Logs Cleared", LINE1(0));
    clcd_print("Successfully", LINE2(0));
    
    reset_shift = RESET_SHIFT
    pos = -1;
    event_num = -1;        
}
int change_password(unsigned char key)
{   
    static char r_pwd[9];
    static int r_pwd_iter = 0;
    extern char first_time;
    
    if(first_time)
    {
        key = 0x3f;
        first_time = 0;
        __delay_ms(500);
        r_pwd_iter = 0;
    }
    
    if(r_pwd_iter < 4)
    {
        clcd_print("Enter new pwd   ", LINE1(0));
    }
    else if(r_pwd_iter > 3 && r_pwd_iter < 8)
    {
        clcd_print("Re enter new pwd", LINE1(0));
    }
    
    switch(key)
    {
        case DKS5: 
            reset_timer0();
            r_pwd[r_pwd_iter] = '0';
            clcd_putch('*', LINE2(r_pwd_iter % 4));
            r_pwd_iter++;
            
            if(r_pwd_iter == 4 || r_pwd_iter == 8)
            {
                clcd_print("                ", LINE2(0));
            }
            break;
            
        case DKS4:         
            reset_timer0();
            r_pwd[r_pwd_iter] = '1';
            clcd_putch('*', LINE2(r_pwd_iter % 4));
            r_pwd_iter++;
            
            if(r_pwd_iter == 4 || r_pwd_iter == 8)
            {
                clcd_print("                ", LINE2(0));
            }
            break;
    }
    
    if(r_pwd_iter == 8)
    {
        if(strncmp(r_pwd, &r_pwd[4], 4) == 0)
        {
            write_at24c04_string(0x00, &r_pwd[4]);
            change_password_status = SUCCESSFUL_CHANGE;
        }
        else{
            change_password_status = CHANGE_FAILED;
        }
    }
    
    return change_password_status;
}

void set_time(unsigned char key, unsigned char reset_flag)
{                
    char time_write_to_rtc;
    static int new_time[3];        
    if(reset_flag == RESET_TIME)
    {
        clcd_print("HH:MM:SS", LINE1(4));    
       
        get_time();
        new_time[0] = ((time[0] & 0x0F) * 10) + ((time[1] & 0x0F) % 10);
        new_time[1] = ((time[3] & 0x0F) * 10) + ((time[4] & 0x0F) % 10);
        new_time[2] = ((time[6] & 0x0F) * 10) + ((time[7] & 0x0F) % 10);                
    }    
    
    switch(key)
    {
        case DKS4:
            new_time[time_pos]++;
            reset_timer0();
            break;
            
        case DKS5:
            new_time[time_pos]--;
            reset_timer0();
            break;
            
        case DKS6:
            time_pos--;
            reset_timer0();
            
            time_write_to_rtc = ((new_time[0] / 10) << 4) | (new_time[0] % 10);
            clock_reg[0] = (clock_reg[0] & 0xc0) | time_write_to_rtc;
            i2c_rtc_write(HR_REG, clock_reg[0]);
            
            time_write_to_rtc = ((new_time[1] / 10) << 4) | (new_time[1] % 10);
            clock_reg[1] = (clock_reg[1] & 0x80) | time_write_to_rtc;
            i2c_rtc_write(MIN_REG, clock_reg[1]);
            
            time_write_to_rtc = ((new_time[2] / 10) << 4) | (new_time[2] % 10);
            clock_reg[2] = (clock_reg[2] & 0x80) | time_write_to_rtc;
            i2c_rtc_write(SEC_REG , clock_reg[2]);
            
            if(time_pos == -1)
            {                
                clcd_specific_instruction(CLCD_CLEAR_DISPLAY);
                
                clcd_print("Time Changed", LINE1(0));
                clcd_print("Successfully", LINE2(0));
                __delay_ms(2000);

                clcd_specific_instruction(CLCD_DISPLAY_ON_CURSOR_OFF);
                screens_flag = MENU_SCREEN;
            }
            
            break;
    }    
    
    if(new_time[0] > 23)
        new_time[0] = 0;
    else if(new_time[0] < 0)
        new_time[0] = 23;    
    else if(new_time[1] > 59)
        new_time[1] = 0;
    else if(new_time[1] < 0)
        new_time[1] = 59;
    else if(new_time[2] > 59)
        new_time[2] = 0;
    else if(new_time[2] < 0)
        new_time[2] = 59;
                
    clcd_putch((new_time[0] / 10) + '0', LINE2(4));
    clcd_putch((new_time[0] % 10) + '0', LINE2(5));
    clcd_putch(':', LINE2(6));
    clcd_putch((new_time[1] / 10) + '0', LINE2(7));
    clcd_putch((new_time[1] % 10) + '0', LINE2(8));
    clcd_putch(':', LINE2(9));
    clcd_putch((new_time[2] / 10) + '0', LINE2(10));
    clcd_putch((new_time[2] % 10) + '0', LINE2(11));
    
    
    if(screens_flag == MENU_SCREEN)
        clcd_specific_instruction(CLCD_CLEAR_DISPLAY);
}

void get_time()
{            
    clock_reg[0] = i2c_rtc_read(HR_REG);
    clock_reg[1] = i2c_rtc_read(MIN_REG);
    clock_reg[2] = i2c_rtc_read(SEC_REG);
    
    time[0] = ((clock_reg[0] >> 4) & 0x03) + '0';
    time[1] = (clock_reg[0] & 0x0F) + '0';
    time[2] = ':';
    
    time[3] = ((clock_reg[1] >> 4) & 0x07) + '0';
    time[4] = (clock_reg[1] & 0x0F) + '0';
    time[5] = ':';
    
    time[6] = ((clock_reg[2] >> 4) & 0x07) + '0';
    time[7] = (clock_reg[2] & 0x0F) + '0';
    time[8] = '\0';        
}