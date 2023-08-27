/*
 * lcd_private.h
 *
 * Created: 18/4/2023 3:45:30 PM
 *  Author: Hossam
 */ 


#ifndef LCD_PRIVATE_H_
#define LCD_PRIVATE_H_

/* Helper Macro */
#define CONCAT4(w,x,y,z) CONCAT4_HELPER(w,x,y,z)
#define CONCAT4_HELPER(w,x,y,z) w##x##y##z

/* Options */
// Enable/Disable Display
#define LCD_OPT_DISP            1

// Show/Hide Cursor
#define LCD_OPT_CURSOR          0

// Enable/Disable Blinking
#define LCD_OPT_BLINK           0

/* Commands */
#define LCD_CMD_RETURN_HOME         0x02
#define LCD_CMD_MODE_4Bit           0x28
#define LCD_CMD_INC_CURSOR_RIGHT    0x06
#define LCD_CMD_DISP_SHIFT_RIGHT    0x1C
#define LCD_CMD_CLEAR               0x01
// Enables or disables Display, Cursor, Blinking
#define LCD_CMD_DCB  CONCAT4(0b00001,LCD_OPT_DISP, LCD_OPT_CURSOR, LCD_OPT_BLINK)

/* Delays */
#define LCD_US_DELAY_PULSE  1000  //   1 us
#define LCD_US_DELAY_HOLD   1000 // 200 us
#define LCD_MS_DELAY_CHAR   25 // 25 ms
#define LCD_MS_DELAY_STORE  2 // 2 ms
#define LCD_MS_DELAY_INIT   10 // 10 ms
#define LCD_MS_DELAY_SHIFT  20 // 20 ms

/* Private Addresses */
#define LCD_LINES_BASE_RAM_LOC 0x80
#define LCD_LINE0_ADDR  0x00
#define LCD_LINE1_ADDR  0x40
#define LCD_LINE2_ADDR  0x14
#define LCD_LINE3_ADDR  0x54
#define LCD_CGRAM_ADDR  0x40

/* Sizes */
#define LCD_CGRAM_LOC_SIZE  8 // bytes
#define LCD_CGRAM_LOC_COUNT  8 // 8 locations (8 bytes each)
#define LCD_LINE_COLS  20
#define LCD_LINES_COUNT  4

#endif /* LCD_PRIVATE_H_ */