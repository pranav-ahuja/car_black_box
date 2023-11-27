/* 
 * File:   main.h
 * Author: Rakesh Ahuja
 *
 * Created on October 30, 2023, 4:52 PM
 */
#include<xc.h>

#ifndef MAIN_H
#define	MAIN_H

#include <xc.h>
#include <string.h>
#include "adc.h"
#include "clcd.h"
#include "digital_keypad.h"
#include "i2c.h"
#include "rtc_ds1307.h"
#include "AT24C04.h"
#include "uart.h"
#include "timer.h"


#define PASSWORD_LENGTH             4

#define DASHBOARD_SCREEN            1
#define LOGIN_SCREEN                2
#define MENU_SCREEN                 3

#define VIEW_LOGS                   4
#define DOWNLOAD_LOGS               5
#define CLEAR_LOGS                  6
#define CHANGE_PASSWORD             7
#define SET_TIME                    8

#define ADC_TO_CLCD_STR             1
#define CLCD_STR                    2
#define GEAR_TO_CLCD                3

#define RESET_PASSWORD              1
#define RESET_NOTHING               0

//View Logs
#define RESET_SHIFT                 1

//Change password macros
#define RE_ENTER_PASSWORD           1
#define SUCCESSFUL_CHANGE           2
#define CHANGE_FAILED               3

//Set Time
#define RESET_TIME                  1
void adc_to_speed(unsigned short adc_val);
void val_to_clcd(unsigned char peripheral);
void eeprom_log_data(unsigned char *event, unsigned char speed);
void eeprom_data_to_memory();
void get_time();

void display_menu_screen();
void default_screen(unsigned char event[3], unsigned short speed);
void login_screen(unsigned char reset_flag, unsigned char key);
unsigned char menu_screen(unsigned char key);

void view_logs(unsigned char key, unsigned char reset_shift);
void download_logs(unsigned char key);
void clear_logs();
int change_password(unsigned char key);
void set_time(unsigned char key, unsigned char reset_flag);


#endif	/* MAIN_H */

