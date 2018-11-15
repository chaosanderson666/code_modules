/*
*********************************************************************************************************
*                                             Clock/Calendar
*
*                            (c) Copyright 1999, John All Rights Reserved
*
*********************************************************************************************************
*/
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "clock.h"
/*
*********************************************************************************************************
*/
#define  CLK_TS_BASE_YEAR    2000       /* Time stamps start year                                       */
/*
*********************************************************************************************************
*/
static uint8_t   clk_hr;
static uint8_t   clk_min;
static uint8_t   clk_sec;               /* Counters for local TIME                                      */
static uint8_t   clk_day;               /* Counters for local DATE                                      */
static uint8_t   clk_week;              /* Day of week (0 is Sunday)                                    */
static uint8_t   clk_month;
static uint16_t  clk_year;
static uint32_t  clk_ts;                /* Current TIME-STAMP                                           */
/*
*********************************************************************************************************
*/
typedef struct{                         /* MONTH RELATED VARIABLES                                      */
    uint8_t  month_days;                /* Number of days in each month                                 */
    char    *month_name;                /* Name of the month                                            */
    uint8_t  month_value;               /* Value used to compute day of the week                        */
}clk_month_t;
/*
*********************************************************************************************************
*/
static char *clk_week_table[] = {            /* NAME FOR EACH DAY OF THE WEEK                           */
    "Sunday ",
    "Monday ",
    "Tuesday ",
    "Wednesday ",
    "Thursday ",
    "Friday ",
    "Saturday "
};

static clk_month_t clk_month_table[] = {     /* MONTHS TABLE  */
    {0,  "",           0},                   /* Invalid month */
    {31, "January ",   6},                   /* January       */
    {28, "February ",  2},                   /* February, note leap years are handled by code           */
    {31, "March ",     2},                   /* March         */
    {30, "April ",     5},                   /* April         */
    {31, "May ",       0},                   /* May           */
    {30, "June ",      3},                   /* June          */
    {31, "July ",      5},                   /* July          */
    {31, "August ",    1},                   /* August        */
    {30, "September ", 4},                   /* September     */
    {31, "October ",   6},                   /* October       */
    {30, "November ",  2},                   /* November      */
    {31, "December ",  4}                    /* December      */
};
/*
*********************************************************************************************************
* Description : Formats the current date into an ASCII string.
* Arguments   : n      is the format type:
*                      1   will format the time as "MM-DD-YY"           (needs at least  9 characters)
*                      2   will format the time as "Day Month DD, YYYY" (needs at least 30 characters)
*                      3   will format the time as "YYYY-MM-DD"         (needs at least 11 characters)
*               s      is a pointer to the destination string.  The destination string must be large
*                      enough to hold the formatted date.
*                      contain
* Returns     : None.
* Notes       : - A 'switch' statement has been used to allow you to add your own date formats.  For
*                 example, you could display the date in French, Spanish, German etc. by assigning
*                 numbers for those types of conversions.
*               - This function assumes that strcpy(), strcat() and itoa() are reentrant.
*********************************************************************************************************
*/
void clk_format_date(uint8_t n, char *s)
{
    uint16_t  year;
    char str[5];

    switch (n) {
        case  1:
              strcpy(s, "MM-DD-YY");             /* Create the template for the selected format        */
              s[0] = clk_month / 10 + '0';       /* Convert DATE to ASCII                              */
              s[1] = clk_month % 10 + '0';
              s[3] = clk_day   / 10 + '0';
              s[4] = clk_day   % 10 + '0';
              year = clk_year % 100;
              s[6] = year / 10 + '0';
              s[7] = year % 10 + '0';
              break;

        case  2:
              strcpy(s, clk_week_table[clk_week]);              /* Get the day of the week             */
              strcat(s, clk_month_table[clk_month].month_name); /* Get name of month                   */
              if (clk_day < 10) {
                 str[0] = clk_day + '0';
                 str[1] = 0;
              } else {
                 str[0] = clk_day / 10 + '0';
                 str[1] = clk_day % 10 + '0';
                 str[2] = 0;
              }
              strcat(s, str);
              strcat(s, ", ");
			  sprintf(str, "%d", clk_year);
              strcat(s, str);
              break;

        case  3:
              strcpy(s, "YYYY-MM-DD");           /* Create the template for the selected format        */
              s[0] = clk_year / 1000 + '0';
              year = clk_year % 1000;
              s[1] = year /  100 + '0';
              year = year %  100;
              s[2] = year /   10 + '0';
              s[3] = year %   10 + '0';
              s[5] = clk_month / 10 + '0';       /* Convert DATE to ASCII                              */
              s[6] = clk_month % 10 + '0';
              s[8] = clk_day   / 10 + '0';
              s[9] = clk_day   % 10 + '0';
              break;

        default:
              strcpy(s, "?");
              break;
    }
}
/*
*********************************************************************************************************
* Description : Formats the current time into an ASCII string.
* Arguments   : n      is the format type:
*                      1   will format the time as "HH:MM:SS"     (24 Hour format)
*                                                                 (needs at least  9 characters)
*                      2   will format the time as "HH:MM:SS AM"  (With AM/PM indication)
*                                                                 (needs at least 13 characters)
*               s      is a pointer to the destination string.  The destination string must be large
*                      enough to hold the formatted time.
*                      contain
* Returns     : None.
* Notes       : - A 'switch' statement has been used to allow you to add your own time formats.
*               - This function assumes that strcpy() is reentrant.
*********************************************************************************************************
*/
void clk_format_time (uint8_t n, char *s)
{
    uint8_t hr;

    switch (n) {
        case  1:
              strcpy(s, "HH:MM:SS");                  /* Create the template for the selected format   */
              s[0] = clk_hr  / 10 + '0';              /* Convert TIME to ASCII                         */
              s[1] = clk_hr  % 10 + '0';
              s[3] = clk_min / 10 + '0';
              s[4] = clk_min % 10 + '0';
              s[6] = clk_sec / 10 + '0';
              s[7] = clk_sec % 10 + '0';
              break;

        case  2:
              strcpy(s, "HH:MM:SS AM");               /* Create the template for the selected format   */
              s[9] = (clk_hr >= 12) ? 'P' : 'A';      /* Set AM or PM indicator                        */
              if (clk_hr > 12) {                      /* Adjust time to be displayed                   */
                  hr   = clk_hr - 12;
              } else {
                  hr = clk_hr;
              }
              s[0] = hr      / 10 + '0';              /* Convert TIME to ASCII                         */
              s[1] = hr      % 10 + '0';
              s[3] = clk_min / 10 + '0';
              s[4] = clk_min % 10 + '0';
              s[6] = clk_sec / 10 + '0';
              s[7] = clk_sec % 10 + '0';
              break;

        default:
              strcpy(s, "?");
              break;
    }
}
/*
*********************************************************************************************************
* Description : This function converts a time-stamp to an ASCII string.
* Arguments   : n         is the desired format number:
*                            1 : "MM-DD-YY HH:MM:SS"         (needs at least 18 characters)
*                            2 : "YYYY-MM-DD HH:MM:SS"       (needs at least 20 characters)
*               ts        is the time-stamp value to format
*               s         is the destination ASCII string
* Returns     : none
* Notes       : - The time stamp is a 32 bit unsigned integer as follows:
*
*        Field: -------Year------ ---Month--- ------Day----- ----Hours----- ---Minutes--- --Seconds--
*        Bit# : 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
*
*               - The year is based from CLK_TS_BASE_YEAR.  That is, if bits 31..26 contain 0 it really 
*                 means that the year is really CLK_TS_BASE_YEAR.  If bits 31..26 contain 13, the year 
*                 is CLK_TS_BASE_YEAR + 13.
*********************************************************************************************************
*/
void clk_format_ts(uint8_t n, uint32_t ts, char *s)
{
    uint16_t yr;
    uint8_t month;
    uint8_t day;
    uint8_t hr;
    uint8_t min;
    uint8_t sec;

    yr    = CLK_TS_BASE_YEAR + (ts >> 26);       /* Unpack time-stamp                                  */
    month = (ts >> 22) & 0x0F;
    day   = (ts >> 17) & 0x1F;
    hr    = (ts >> 12) & 0x1F;
    min   = (ts >>  6) & 0x3F;
    sec   = (ts & 0x3F);
    switch(n){
        case  1:
              strcpy(s, "MM-DD-YY HH:MM:SS");    /* Create the template for the selected format        */
              yr    = yr    % 100;
              s[ 0] = month / 10 + '0';          /* Convert DATE to ASCII                              */
              s[ 1] = month % 10 + '0';
              s[ 3] = day   / 10 + '0';
              s[ 4] = day   % 10 + '0';
              s[ 6] = yr    / 10 + '0';
              s[ 7] = yr    % 10 + '0';
              s[ 9] = hr    / 10 + '0';          /* Convert TIME to ASCII                              */
              s[10] = hr    % 10 + '0';
              s[12] = min   / 10 + '0';
              s[13] = min   % 10 + '0';
              s[15] = sec   / 10 + '0';
              s[16] = sec   % 10 + '0';
              break;

        case  2:
              strcpy(s, "YYYY-MM-DD HH:MM:SS");  /* Create the template for the selected format        */
              s[ 0] = yr    / 1000 + '0';        /* Convert DATE to ASCII                              */
              yr    = yr    % 1000;
              s[ 1] = yr    / 100 + '0';
              yr    = yr    % 100;
              s[ 2] = yr    / 10 + '0';
              s[ 3] = yr    % 10 + '0';
              s[ 5] = month / 10 + '0';          
              s[ 6] = month % 10 + '0';
              s[ 8] = day   / 10 + '0';
              s[ 9] = day   % 10 + '0';
              s[11] = hr    / 10 + '0';          /* Convert TIME to ASCII                              */
              s[12] = hr    % 10 + '0';
              s[14] = min   / 10 + '0';
              s[15] = min   % 10 + '0';
              s[17] = sec   / 10 + '0';
              s[18] = sec   % 10 + '0';
              break;

        default:
              strcpy(s, "?");
              break;
    }
}
/*
*********************************************************************************************************
* Description : This function is used to return a time-stamp to your application.  The format of the
*               time-stamp is shown below:
*
* Field: -------Year------   ---Month---   ------Day-----   ----Hours-----   ---Minutes---   --Seconds--
* Bit# : 31 30 29 28 27 26 | 25 24 23 22 | 21 20 19 18 17 | 16 15 14 13 12 | 11 10 9 8 7 6 | 5 4 3 2 1 0
*
* Arguments   : None.
* Returns     : None.
* Notes       : The year is based from CLK_TS_BASE_YEAR.  That is, if bits 31..26 contain 0 it really 
*               means that the year is CLK_TS_BASE_YEAR.  If bits 31..26 contain 13, the year is 
*               CLK_TS_BASE_YEAR + 13.
*********************************************************************************************************
*/
uint32_t clk_get_ts(void)
{
    uint32_t ts;

    ts = clk_ts;
    return (ts);
}
/*
*********************************************************************************************************
* Description : This function initializes the time module.  The time of day clock task will be created
*               by this function.
* Arguments   : None
* Returns     : None.
*********************************************************************************************************
*/
void clk_init(void)
{
    clk_sec   = 0;
    clk_min   = 30;
    clk_hr    = 16;
    clk_day   = 25;
    clk_month = 12;
    clk_year  = 1990;
    clk_ts    = clk_make_ts(clk_month, clk_day, clk_year, clk_hr, clk_min, clk_sec);
}
/*
*********************************************************************************************************
* Description : This function determines whether the 'year' passed as an argument is a leap year.
* Arguments   : year    is the year to check for leap year.
* Returns     : true    if 'year' is a leap year.
*               false   if 'year' is NOT a leap year.
*********************************************************************************************************
*/
static char Clk_IsLeapYear(uint16_t year)
{
    if (!(year % 4) && (year % 100) || !(year % 400)){
        return 1;
    } else {
        return 0;
    }
}
/*
*********************************************************************************************************
* Description : This function maps a user specified date and time into a 32 bit variable called a
*               time-stamp.
* Arguments   : month     is the desired month   (1..12)
*               day       is the desired day     (1..31)
*               year      is the desired year    (CLK_TS_BASE_YEAR .. CLK_TS_BASE_YEAR+63)
*               hr        is the desired hour    (0..23)
*               min       is the desired minutes (0..59)
*               sec       is the desired seconds (0..59)
* Returns     : A time-stamp based on the arguments passed to the function.
* Notes       : - The time stamp is formatted as follows using a 32 bit unsigned integer:
*
*        Field: -------Year------ ---Month--- ------Day----- ----Hours----- ---Minutes--- --Seconds--
*        Bit# : 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
*
*               - The year is based from CLK_TS_BASE_YEAR.  That is, if bits 31..26 contain 0 it really 
*                 means that the year is really CLK_TS_BASE_YEAR.  If bits 31..26 contain 13, the year is 
*                 CLK_TS_BASE_YEAR + 13.
*********************************************************************************************************
*/
uint32_t  clk_make_ts(uint8_t month, uint8_t day, uint16_t yr, uint8_t hr, uint8_t min, uint8_t sec)
{
    uint32_t ts;
    yr -= CLK_TS_BASE_YEAR;
    ts  = ((uint32_t)yr << 26) | ((uint32_t)month << 22) | ((uint32_t)day << 17);
    ts |= ((uint32_t)hr << 12) | ((uint32_t)min   <<  6) |  (uint32_t)sec;
    return (ts);
}
/*
*********************************************************************************************************
* Description : This function computes the day of the week (0 == Sunday) based on the current month,
*               day and year.
* Arguments   : None.
* Returns     : None.
* Notes       : - This function updates clk_week.
*               - This function is called by ClkUpdateDate().
*********************************************************************************************************
*/
static void clk_update_week(void)
{
    uint16_t dow;

    dow = clk_day + clk_month_table[clk_month].month_value;
    if (clk_month < 3) {
        if (Clk_IsLeapYear(clk_year)) {
            dow--;
        }
    }
    dow    += clk_year + (clk_year / 4);
    dow    += (clk_year / 400) - (clk_year / 100);
    dow    %= 7;
    clk_week  = dow;
}
/*
*********************************************************************************************************
* Description : Set the date of the time-of-day clock
* Arguments   : month     is the desired month (1..12)
*               day       is the desired day   (1..31)
*               year      is the desired year  (CLK_TS_BASE_YEAR .. CLK_TS_BASE_YEAR+63)
* Returns     : None.
* Notes       : It is assumed that you are specifying a correct date (i.e. there is no range checking
*               done by this function).
*********************************************************************************************************
*/
void  clk_set_date(uint8_t month, uint8_t day, uint16_t year)
{
    clk_month = month;
    clk_day   = day;
    clk_year  = year;
	
    clk_update_week(); /* Compute the day of the week (i.e. Sunday ...)      */
}
/*
*********************************************************************************************************
* Description : Set the date and time of the time-of-day clock
* Arguments   : month     is the desired month   (1..12)
*               day       is the desired day     (1..31)
*               year      is the desired year    (2xxx)
*               hr        is the desired hour    (0..23)
*               min       is the desired minutes (0..59)
*               sec       is the desired seconds (0..59)
* Returns     : None.
* Notes       : It is assumed that you are specifying a correct date and time (i.e. there is no range
*               checking done by this function).
*********************************************************************************************************
*/
void clk_set_datetime(uint16_t year, uint8_t month, uint8_t day, uint8_t hr, uint8_t min, uint8_t sec)
{
    clk_month = month;
    clk_day   = day;
    clk_year  = year;
    clk_hr    = hr;
    clk_min   = min;
    clk_sec   = sec;
	
    clk_update_week(); /* Compute the day of the week (i.e. Sunday ...)      */
}
/*
*********************************************************************************************************
* Description : Set the time-of-day clock
* Arguments   : hr        is the desired hour    (0..23)
*               min       is the desired minutes (0..59)
*               sec       is the desired seconds (0..59)
* Returns     : None.
* Notes       : It is assumed that you are specifying a correct time (i.e. there is no range checking
*               done by this function).
*********************************************************************************************************
*/
void clk_set_time(uint8_t hr, uint8_t min, uint8_t sec)
{
    clk_hr  = hr;
    clk_min = min;
    clk_sec = sec;
}
/*
*********************************************************************************************************
* Description : Get the date and time of the time-of-day clock
* Arguments   : month     is the desired month   (1..12)
*               day       is the desired day     (1..31)
*               year      is the desired year    (2xxx)
*               hr        is the desired hour    (0..23)
*               min       is the desired minutes (0..59)
*               sec       is the desired seconds (0..59)
* Returns     : None.
* Notes       : It is assumed that you are specifying a correct date and time (i.e. there is no range
*               checking done by this function).
*********************************************************************************************************
*/
void  clk_get_datetime(uint8_t *month, uint8_t *day, uint16_t *year, uint8_t *hr, uint8_t *min, uint8_t *sec)
{
	if(NULL != month)
		*month = clk_month;
	if(NULL != day)
		*day = clk_day;
	if(NULL != year)
		*year = clk_year;
	if(NULL != hr)
		*hr = clk_hr;
	if(NULL != min)
		*min = clk_min;
	if(NULL != sec)
		*sec = clk_sec;
}
/*
*********************************************************************************************************
* Description : This function is called to update the date (i.e. month, day and year)
* Arguments   : None.
* Returns     : None.
* Notes       : This function updates clk_day, clk_month, clk_year and clk_week.
*********************************************************************************************************
*/
static void clk_update_date(void)
{
    char newmonth;

    newmonth = 1;
    if (clk_day >= clk_month_table[clk_month].month_days){  /* Last day of the month?       */
        if (clk_month == 2) {                               /* Is this February?            */
            if (Clk_IsLeapYear(clk_year) == 1) {            /* Yes, Is this a leap year?    */
                if (clk_day >= 29) {                        /* Yes, Last day in february?   */
                    clk_day = 1;                            /* Yes, Set to 1st day in March */
                } else {
                    clk_day++;
                    newmonth = 0;
                }
            } else {
                clk_day = 1;
            }
        } else {
            clk_day = 1;
        }
    } else {
        clk_day++;
        newmonth = 0;
    }
    if (newmonth == 1) {                         /* See if we have completed a month                   */
        if (clk_month >= 12) {                   /* Yes, Is this december ?                            */
            clk_month = 1;                       /* Yes, set month to january...                       */
            clk_year++;                          /*      ...we have a new year!                        */
        } else {
            clk_month++;                         /* No,  increment the month                           */
        }
    }
    clk_update_week();                            /* Compute the day of the week (i.e. Sunday ...)      */
}
/*
*********************************************************************************************************
* Description : This function is called to update the time (i.e. hours, minutes and seconds)
* Arguments   : None.
* Returns     : true     if we have completed one day.
*               false    otherwise
* Notes       : This function updates clk_sec, clk_min and clk_hr.
*********************************************************************************************************
*/
static char clk_update_time(void)
{
    char newday;

    newday = 0;                              /* Assume that we haven't completed one whole day yet      */
    if (clk_sec >= 59) {                     /* See if we have completed one minute yet                 */
        clk_sec = 0;                         /* Yes, clear seconds                                      */
        if (clk_min >= 59) {                 /*    See if we have completed one hour yet                */
            clk_min = 0;                     /*    Yes, clear minutes                                   */
            if (clk_hr >= 23) {              /*        See if we have completed one day yet             */
                clk_hr = 0;                  /*        Yes, clear hours ...                             */
                newday = 1;                  /*        ... change flag to indicate we have a new day    */
            } else {
                clk_hr++;                    /*        No,  increment hours                             */
            }
        } else {
            clk_min++;                       /*    No,  increment minutes                               */
        }
    } else {
        clk_sec++;                           /* No,  increment seconds                                  */
    }
    return (newday);
}
/*
*********************************************************************************************************
* Description : This function must be called every second to drive the clock.
* Arguments   : None.
*********************************************************************************************************
*/
void clk_update(void)
{
	if(clk_update_time() == 1) {         /* Update the clock TIME         */
		clk_update_date();               /* And date if a new day         */
	}
}
/*
*********************************************************************************************************
*/
