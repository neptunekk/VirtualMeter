/**
 * @brief		
 * @details		
 * @date		2016-08-16
 **/

/* Includes ------------------------------------------------------------------*/
#include "lcd.h"

#if defined ( _WIN32 ) || defined ( _WIN64 ) || defined ( __linux )
#include "comm_socket.h"
#else
#include "stm32f0xx.h"
#endif

/* Private typedef -----------------------------------------------------------*/
#if defined ( _WIN32 ) || defined ( _WIN64 ) || defined ( __linux )
struct __win_lcd_message
{
    struct
    {
        enum __lcd_dot          dot;
        enum __lcd_unit         unit;
        enum __lcd_date_format  format;
        enum
        {
            LCD_WIN_SHOW_BIN = 0,
            LCD_WIN_SHOW_DEC,
            LCD_WIN_SHOW_HEX,
            LCD_WIN_SHOW_DATE,
            LCD_WIN_SHOW_MSG,
            LCD_WIN_SHOW_NONE,
            LCD_WIN_SHOW_ALL,
            
        }                       type;
        
        union
        {
            uint16_t            bin;
            int32_t             dec;
            uint32_t            hex;
            uint64_t            date;
            uint8_t             message[8];
            
        }                       value;
        
    }                           windows[LCD_MAX_WINDOWS];

    struct
    {
        enum
        {
            LCD_LAB_SHOW_OFF = 0,
            LCD_LAB_SHOW_ON,
            LCD_LAB_SHOW_FLASH,
            
        }                       status;
        
        uint8_t                 value;
        
    }                           label[LCD_MAX_LABELS];
    
    enum
    {
        LCD_GLO_NONE_OPTION = 0,
        LCD_GLO_SHOW_NONE,
        LCD_GLO_SHOW_ALL,
        
    }                           global;
    
    enum
    {
        LCD_BKL_NONE = 0,
        LCD_BKL_OPEN,
        
    }                           backlight;
};
#endif
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static enum __dev_status status = DEVICE_NOTINIT;

#if defined ( _WIN32 ) || defined ( _WIN64 ) || defined ( __linux )
static SOCKADDR_IN src;
static SOCKET sock = INVALID_SOCKET;
static struct __win_lcd_message lcd_message;
#endif

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  
  */
static enum __dev_status lcd_status(void)
{
    return(status);
}

/**
  * @brief  
  */
static void lcd_init(enum __dev_state state)
{
#if !defined ( _WIN32 ) && !defined ( _WIN64 ) && !defined ( __linux )
    status = DEVICE_INIT;
#else
    memset(&lcd_message, 0, sizeof(lcd_message));
    
    lcd_message.global = LCD_GLO_SHOW_ALL;
    lcd_message.backlight = LCD_BKL_NONE;

	sock = emitter.open(50001, &src);

    if(sock == INVALID_SOCKET)
    {
        status = DEVICE_ERROR;
    }
    else
    {
		emitter.write(sock, &src, (uint8_t *)&lcd_message, sizeof(lcd_message));
        status = DEVICE_INIT;
    }
#endif
}

/**
  * @brief  
  */
static void lcd_suspend(void)
{
#if !defined ( _WIN32 ) && !defined ( _WIN64 ) && !defined ( __linux )
    
#else
    memset(&lcd_message, 0, sizeof(lcd_message));
    lcd_message.global = LCD_GLO_SHOW_NONE;
    
    if(sock != INVALID_SOCKET)
    {
		emitter.write(sock, &src, (uint8_t *)&lcd_message, sizeof(lcd_message));
		emitter.close(sock);
		sock = INVALID_SOCKET;
    }
#endif
    
    status = DEVICE_NOTINIT;
}



/**
  * @brief  
  */
static void lcd_runner(uint16_t msecond)
{
#if !defined ( _WIN32 ) && !defined ( _WIN64 ) && !defined ( __linux )
    
#else
    if((sock != INVALID_SOCKET) && (status == DEVICE_INIT))
    {
        if(emitter.write(sock, &src, (uint8_t *)&lcd_message, sizeof(lcd_message)) != sizeof(lcd_message))
        {
			emitter.close(sock);
			sock = INVALID_SOCKET;
        }
    }
    else
    {
		sock = emitter.open(50001, &src);

		if (sock != INVALID_SOCKET)
		{
			status = DEVICE_INIT;
		}
    }
#endif
}

/**
  * @brief  
  */
static void lcd_show_none(void)
{
#if !defined ( _WIN32 ) && !defined ( _WIN64 ) && !defined ( __linux )
    
#else
    lcd_message.global = LCD_GLO_SHOW_NONE;
#endif
}

/**
  * @brief  
  */
static void lcd_show_all(void)
{
#if !defined ( _WIN32 ) && !defined ( _WIN64 ) && !defined ( __linux )
    
#else
    lcd_message.global = LCD_GLO_SHOW_ALL;
#endif
}

/**
  * @brief  
  */
static enum __lcd_backlight lcd_backlight_open(void)
{
#if !defined ( _WIN32 ) && !defined ( _WIN64 ) && !defined ( __linux )
    
#else
    lcd_message.backlight = LCD_BKL_OPEN;
    return(lcd_message.backlight);
#endif
}

/**
  * @brief  
  */
static enum __lcd_backlight lcd_backlight_close(void)
{
#if !defined ( _WIN32 ) && !defined ( _WIN64 ) && !defined ( __linux )
    
#else
    lcd_message.backlight = LCD_BKL_NONE;
    return(lcd_message.backlight);
#endif
}

/**
  * @brief  
  */
static enum __lcd_backlight lcd_backlight_status(void)
{
#if !defined ( _WIN32 ) && !defined ( _WIN64 ) && !defined ( __linux )
    
#else
    return(lcd_message.backlight);
#endif
}



/**
  * @brief  
  */
static void window_show_bin(uint8_t channel, uint16_t val, enum __lcd_dot dot, enum __lcd_unit unit)
{
#if !defined ( _WIN32 ) && !defined ( _WIN64 ) && !defined ( __linux )
    
#else
	if(channel >= LCD_MAX_WINDOWS)
	{
		return;
	}
    lcd_message.global = LCD_GLO_NONE_OPTION;
	lcd_message.windows[channel].dot = dot;
	lcd_message.windows[channel].unit = unit;
	lcd_message.windows[channel].type = LCD_WIN_SHOW_BIN;
	lcd_message.windows[channel].value.bin = val;
#endif
}

/**
  * @brief  
  */
static void window_show_dec(uint8_t channel, int32_t val, enum __lcd_dot dot, enum __lcd_unit unit)
{
#if !defined ( _WIN32 ) && !defined ( _WIN64 ) && !defined ( __linux )
    
#else
	if(channel >= LCD_MAX_WINDOWS)
	{
		return;
	}
    lcd_message.global = LCD_GLO_NONE_OPTION;
	lcd_message.windows[channel].dot = dot;
	lcd_message.windows[channel].unit = unit;
	lcd_message.windows[channel].type = LCD_WIN_SHOW_DEC;
	lcd_message.windows[channel].value.dec = val;
#endif
}


static void window_show_hex(uint8_t channel, uint32_t val, enum __lcd_dot dot, enum __lcd_unit unit)
{
#if !defined ( _WIN32 ) && !defined ( _WIN64 ) && !defined ( __linux )
    
#else
	if(channel >= LCD_MAX_WINDOWS)
	{
		return;
	}
    lcd_message.global = LCD_GLO_NONE_OPTION;
	lcd_message.windows[channel].dot = dot;
	lcd_message.windows[channel].unit = unit;
	lcd_message.windows[channel].type = LCD_WIN_SHOW_HEX;
	lcd_message.windows[channel].value.hex = val;
#endif
}

/**
  * @brief  
  */
static void window_show_date(uint8_t channel, uint64_t val, enum __lcd_date_format theme)
{
#if !defined ( _WIN32 ) && !defined ( _WIN64 ) && !defined ( __linux )
    
#else
	if(channel >= LCD_MAX_WINDOWS)
	{
		return;
	}
    lcd_message.global = LCD_GLO_NONE_OPTION;
	lcd_message.windows[channel].format = theme;
	lcd_message.windows[channel].type = LCD_WIN_SHOW_DATE;
	lcd_message.windows[channel].value.date = val;
#endif
}

/**
  * @brief  
  */
static uint8_t window_show_msg(uint8_t channel, const char *msg)
{
#if !defined ( _WIN32 ) && !defined ( _WIN64 ) && !defined ( __linux )
    
#else
	if(channel >= LCD_MAX_WINDOWS)
	{
		return(0);
	}
    lcd_message.global = LCD_GLO_NONE_OPTION;
	lcd_message.windows[channel].type = LCD_WIN_SHOW_MSG;
	if(strlen(msg) > 8)
	{
		memcpy(lcd_message.windows[channel].value.message, msg, 8);
		return(8);
	}
	else
	{
		memset(lcd_message.windows[channel].value.message, 0, sizeof(lcd_message.windows[channel].value.message));
		memcpy(lcd_message.windows[channel].value.message, msg, strlen(msg));
		return((uint8_t)strlen(msg));
	}
#endif
}

/**
  * @brief  
  */
static void window_show_none(uint8_t channel)
{
#if !defined ( _WIN32 ) && !defined ( _WIN64 ) && !defined ( __linux )
    
#else
	if(channel >= LCD_MAX_WINDOWS)
	{
		return;
	}
    lcd_message.global = LCD_GLO_NONE_OPTION;
	lcd_message.windows[channel].dot = LCD_DOT_NONE;
	lcd_message.windows[channel].unit = LCD_UNIT_NONE;
	lcd_message.windows[channel].type = LCD_WIN_SHOW_DEC;
	lcd_message.windows[channel].value.dec = 0;
#endif
}

/**
  * @brief  
  */
static void window_show_all(uint8_t channel)
{
#if !defined ( _WIN32 ) && !defined ( _WIN64 ) && !defined ( __linux )
    
#else
	if(channel >= LCD_MAX_WINDOWS)
	{
		return;
	}
    lcd_message.global = LCD_GLO_NONE_OPTION;
	lcd_message.windows[channel].dot = LCD_DOT_NONE;
	lcd_message.windows[channel].unit = LCD_UNIT_NONE;
	lcd_message.windows[channel].type = LCD_WIN_SHOW_DEC;
	lcd_message.windows[channel].value.dec = 88888888;
#endif
}

/**
  * @brief  
  */
static void window_read(uint8_t channel, void *dat)
{
#if !defined ( _WIN32 ) && !defined ( _WIN64 ) && !defined ( __linux )
    
#else
	if(channel >= LCD_MAX_WINDOWS)
	{
		return;
	}
#endif
}





/**
  * @brief  
  */
static void lcd_label_on(uint8_t channel, uint8_t state)
{
#if !defined ( _WIN32 ) && !defined ( _WIN64 ) && !defined ( __linux )
    
#else
	if(channel >= LCD_MAX_LABELS)
	{
		return;
	}
    lcd_message.global = LCD_GLO_NONE_OPTION;
	lcd_message.label[channel].status = LCD_LAB_SHOW_ON;
	lcd_message.label[channel].value = state;
#endif
}

/**
  * @brief  
  */
static void lcd_label_off(uint8_t channel)
{
#if !defined ( _WIN32 ) && !defined ( _WIN64 ) && !defined ( __linux )
    
#else
	if(channel >= LCD_MAX_LABELS)
	{
		return;
	}
    lcd_message.global = LCD_GLO_NONE_OPTION;
	lcd_message.label[channel].status = LCD_LAB_SHOW_OFF;
#endif
}

/**
  * @brief  
  */
static void lcd_label_flash(uint8_t channel)
{
#if !defined ( _WIN32 ) && !defined ( _WIN64 ) && !defined ( __linux )
    
#else
	if(channel >= LCD_MAX_LABELS)
	{
		return;
	}
    lcd_message.global = LCD_GLO_NONE_OPTION;
	lcd_message.label[channel].status = LCD_LAB_SHOW_FLASH;
#endif
}



















/**
  * @brief  
  */
const struct __lcd lcd = 
{
    .control                = 
    {
        .name               = "lcd driver",
        .status             = lcd_status,
        .init               = lcd_init,
        .suspend            = lcd_suspend,
    },
    
    .runner                 = lcd_runner,
    
    .show                   = 
    {
        .none               = lcd_show_none,
        .all                = lcd_show_all,
    },

    .backlight              = 
    {
        .open               = lcd_backlight_open,
        .close              = lcd_backlight_close,
        .status             = lcd_backlight_status,
    },
    
    .window                 = 
    {
        .show               = 
        {
            .bin            = window_show_bin,
            .dec            = window_show_dec,
            .hex            = window_show_hex,
            .date           = window_show_date,
            .msg            = window_show_msg,
            .none           = window_show_none,
            .all            = window_show_all,
        },
        
        .read               = window_read,
    },
    
    .label                  = 
    {
        .on                 = lcd_label_on,
        .off                = lcd_label_off,
        .flash              = lcd_label_flash,
    },
};

