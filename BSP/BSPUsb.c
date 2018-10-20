#include "BSPUsb.h"



static void keybroad_value(int8_t value);

static void BSPPutArray(uint8_t *array,uint16_t  len);

__IO uint8_t prev_transfer_complete = 1;

uint8_t key_buffer[HID_IN_PACKET] = {0};

usbd_core_handle_struct  usb_device_dev = 
{
    .dev_desc = (uint8_t *)&device_descripter,
    .config_desc = (uint8_t *)&configuration_descriptor,
    .strings = usbd_strings,
    .class_init = hid_init,
    .class_deinit = hid_deinit,
    .class_req_handler = hid_req_handler,
    .class_data_handler = hid_data_handler
};


static void USB_rcu_config(void)
{
    /* enable USB pull-up pin clock */ 
    rcu_periph_clock_enable(RCC_AHBPeriph_GPIO_PULLUP);

    /* enable the power clock */
//    rcu_periph_clock_enable(RCU_PMU);

    /* configure USB model clock from PLL clock */
    rcu_usbd_clock_config(RCU_USBD_CKPLL_DIV1_5);

    /* enable USB APB1 clock */
    rcu_periph_clock_enable(RCU_USBD);
}


static void USB_gpio_config(void)
{
    /* configure usb pull-up pin */
    gpio_mode_set(USB_PULLUP, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, USB_PULLUP_PIN);
    gpio_output_options_set(USB_PULLUP, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, USB_PULLUP_PIN);

    /* USB wakeup EXTI line configuration */
    exti_interrupt_flag_clear(EXTI_18);
    exti_init(EXTI_18, EXTI_INTERRUPT, EXTI_TRIG_RISING);
}



static void USB_int_config(void)
{
    /* 1 bit for pre-emption priority, 3 bits for subpriority */
//    nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);

    /* enable the USB low priority interrupt */
    nvic_irq_enable(USBD_LP_IRQn, 2, 1);//20

    /* enable the USB Wake-up interrupt */
    nvic_irq_enable(USBDWakeUp_IRQChannel, 0, 0);
}

static void USB_connect(void)
{
	/* enabled USB pull-up */
	gpio_bit_set(USB_PULLUP, USB_PULLUP_PIN);
	usb_device_dev.status = USBD_CONNECTED;
//	while(usb_device_dev.status != USBD_CONFIGURED);
}

extern uint32_t g_interrupt_mask;
static void  Usbd_core_init (usbd_core_handle_struct *pudev)
{
    /* disable remote wakeup feature */
    pudev->remote_wakeup = 0U;

    /* just reset the CLOSE bit */
    USBD_REG_SET(USBD_CTL, CTL_SETRST);

    /* may be need wait some time(tSTARTUP) ... */

    /* clear SETRST bit in USBD_CTL register */
    USBD_REG_SET(USBD_CTL, 0U);

    /* clear all pending interrupts */
    USBD_REG_SET(USBD_INTF, 0U);

    /* set allocation buffer address */
    USBD_REG_SET(USBD_BADDR, BUFFER_ADDRESS & 0xFFF8U);

    g_interrupt_mask = (CTL_STIE | CTL_WKUPIE | CTL_SOFIE | CTL_ESOFIE | CTL_RSTIE);

    /* enable all interrupts mask bits */
    USBD_REG_SET(USBD_CTL, g_interrupt_mask);
}



void BSPUsbInit()
{

	USB_rcu_config();
	USB_gpio_config();
//	usbd_core_init(&usb_device_dev);		//库函数
	Usbd_core_init(&usb_device_dev);		//自定义库函数，修改其中参数
	USB_int_config();
	USB_connect();
}



void SendWire()
{
	//usb插入
	if(gpio_input_bit_get(GPIOA,GPIO_PIN_1))
	{
		if(usb_device_dev.status == USBD_CONFIGURED)
		{
			BSPPutArray(TxBuffer,tx_len);	//有线	
			BSPPlaySuccess();		//成功			
			return;
		}
	}

	BSPPlayFail();
}



/*改善 0)0)0) 只输出0的问题*/
static void BSPPutArray(uint8_t *array,uint16_t  len)
{
	uint16_t i;
	static int8_t next=0;
	//	GET_ARRAY_LEN(Rx_buffer,len); //tips:If you want to be free to get to length of array ,you can use some function here.
	for(i=0;i<len;i++)
	{
		keybroad_value(array[i]);	//获取ascii 对应键值
		
		if((next^key_buffer[2])!=0)
		{
			next = key_buffer[2];
			
			if(prev_transfer_complete==1&&key_buffer[2]!=0)                               //Determine whether USB is ready
			{
				hid_report_send(&usb_device_dev, key_buffer, 8);            //Send the key_buffer message 
				while(prev_transfer_complete==0);																						//Wait for USB to be sent to complete
				key_buffer[2] = 0 ;	//Clear the buffer
				key_buffer[0] = 0 ;	//Clear the buffer
			}					
		}
		else
		{		
			next = key_buffer[2];
			key_buffer[2] = 0 ;
			key_buffer[0] = 0 ;	//Clear the buffer
			hid_report_send(&usb_device_dev, key_buffer, 8);            //Send the key_buffer message 
			while(prev_transfer_complete==0);
			keybroad_value(array[i]);
			if(prev_transfer_complete==1&&key_buffer[2]!=0)                               //Determine whether USB is ready
			{
				hid_report_send(&usb_device_dev, key_buffer, 8);            //Send the key_buffer message 
				while(prev_transfer_complete==0);																						//Wait for USB to be sent to complete
				key_buffer[2] = 0 ;	//Clear the buffer
				key_buffer[0] = 0 ;	//Clear the buffer
			}	
		}
	}
}


void keybroad_value(int8_t value)
{
	uint8_t j,k;
	switch(DevConfig.country_keyboard)
	{
		case keyboard_AMERICAN:
			switch(value)
			{
/****************************************************/		
				case 0x08:	j=KV_Backspace;
										k=0;
										break;
										

				case '\t':	j=KV_Tab;	//0x09
										k=0;
										break;


				case '\n':	j=KV_Return;	//0x0A
										k=0;
										break;

				case '\r':	j=KV_Return;	//0x0D
										k=0;
										break;			
										
				case 0x1B:	j=KV_Escape;
										k=0;
										break;				
/****************************************************/				

				case ' ':		j=KV_Space;
										k=0;
										break;				
				
				case '!':		j=KV_1;
										k=KV_LeftShift;
										break;

				case '"':		j=KV_SingleQuote;
										k=KV_LeftShift;
										break;

				case '#':		j=KV_3;
										k=KV_LeftShift;
										break;

				case '$':   j=KV_4;
										k=KV_LeftShift;
										break;

				case '%':   j=KV_5;
										k=KV_LeftShift;
										break;

				case '&':   j=KV_7;
										k=KV_LeftShift;
										break;

				case '\'':  j=KV_SingleQuote;
										k=0;
										break;

				case '(':   j=KV_9;
										k=KV_LeftShift;
										break;

				case ')':   j=KV_0;
										k=KV_LeftShift;
										break;

				case '*':   j=KV_8;
										k=KV_LeftShift;
										break; 

				case '+':   j=KV_Equal;
										k=KV_LeftShift;
										break; 

				case ',':   j=KV_Comma;
										k=0;
										break; 

				case '-':   j=KV_Minus;
										k=0;
										break;

				case '.':   j=KV_Period;
										k=0;
										break; 

				case '/':   j=KV_Slash;
										k=0;
										break; 

				case '0':   j=KV_0;
										k=0;
										break; 

				case '1':   j=KV_1;
										k=0;
										break;

				case '2':   j=KV_2;
										k=0;
										break;

				case '3':   j=KV_3;
										k=0;
										break;

				case '4':   j=KV_4;
										k=0;
										break;

				case '5':   j=KV_5;
										k=0;
										break;   

				case '6':   j=KV_6;
										k=0;
										break; 

				case '7':   j=KV_7;
										k=0;
										break; 

				//--------------------

				case '8':   j=KV_8;
										k=0;
										break; 

				case '9':   j=KV_9;
										k=0;
										break;

				case ':':   j=KV_Semicolon;
										k=KV_LeftShift;
										break; 

				case ';':   j=KV_Semicolon;
										k=0;
										break; 

				case '<':   j=KV_Comma;
										k=KV_LeftShift;
										break; 

				case '=':   j=KV_Equal;
										k=0;
										break;

				case '>':   j=KV_Period;
										k=KV_LeftShift;
										break; 

				case '?':   j=KV_Slash;
										k=KV_LeftShift;
										break;

				case '@':   j=KV_2;
										k=KV_LeftShift;
										break;


				case 'A':   j=KV_a;
										k=KV_LeftShift;  
										break; 

				case 'B':   j=KV_b;
										k=KV_LeftShift;  
										break; 

				case 'C':   j=KV_c;
										k=KV_LeftShift;  
										break; 

				case 'D':   j=KV_d;
										k=KV_LeftShift;  
										break; 

				case 'E':		j=KV_e;
										k=KV_LeftShift;  
										break;

				case 'F':   j=KV_f;
										k=KV_LeftShift;  
										break; 

				case 'G':   j=KV_g;
										k=KV_LeftShift;  
										break;

				case 'H':   j=KV_h;
										k=KV_LeftShift;  
										break;

				case 'I':   j=KV_i;
										k=KV_LeftShift;
										break;

				case 'J':   j=KV_j;
										k=KV_LeftShift;
										break;

				case 'K':   j=KV_k;
										k=KV_LeftShift;
										break;

				case 'L':   j=KV_l;
										k=KV_LeftShift;
										break; 

				case 'M':   j=KV_m;
										k=KV_LeftShift;
										break; 

				case 'N':   j=KV_n;
										k=KV_LeftShift;
										break;

				case 'O':   j=KV_o;
										k=KV_LeftShift;
										break;

				case 'P':   j=KV_p;
										k=KV_LeftShift;  
										break;

				case 'Q':   j=KV_q;
										k=KV_LeftShift;  
										break; 

				case 'R':   j=KV_r;
										k=KV_LeftShift;
										break;

				case 'S':   j=KV_s;
										k=KV_LeftShift;  
										break;

				case 'T':   j=KV_t;
										k=KV_LeftShift;
										break;

				case 'U':   j=KV_u;
										k=KV_LeftShift;  
										break; 

				case 'V':   j=KV_v;
										k=KV_LeftShift;  
										break;

				case 'W':   j=KV_w;
										k=KV_LeftShift;
										break;

				case 'X':   j=KV_x;
										k=KV_LeftShift;
										break;

				case 'Y':   j=KV_y;
										k=KV_LeftShift;  
										break;

				case 'Z':   j=KV_z;
										k=KV_LeftShift;  
										break;

				case '[':   j=KV_LeftBracket;
										k=0;
										break;

				case '\\':  j=KV_BackSlash;
										k=0;
										break;   

				case ']':   j=KV_RightBracket;
										k=0;
										break; 

				case '^':   j=KV_6;
										k=KV_LeftShift;
										break; 

				case '_':   j=KV_Minus;
										k=KV_LeftShift;
										break;

				case '`':  j=KV_backQuote;
										k=0;
										break;

				case 'a':   j=KV_a;
										k=0;
										break;

				case 'b':   j=KV_b;
										k=0;
										break;  

				case 'c':   j=KV_c;
										k=0;
										break; 

				case 'd':   j=KV_d;
										k=0;
										break; 

				case 'e':   j=KV_e;
										k=0;
										break;

				case 'f':	  j=KV_f;
										k=0;
										break;

				case 'g':   j=KV_g;
										k=0;
										break;

				case 'h':   j=KV_h;
										k=0;
										break;  

				case 'i':   j=KV_i;
										k=0;
										break;  

				case 'j':   j=KV_j;
										k=0;
										break; 

				case 'k':   j=KV_k;
										k=0; 
										break; 

				case 'l':   j=KV_l;
										k=0;
										break;

				case 'm':   j=KV_m;
										k=0;
										break;

				case 'n':   j=KV_n;
										k=0;
										break;

				case 'o':   j=KV_o;
										k=0;
										break;

				case 'p':   j=KV_p;
										k=0;
										break; 

				case 'q':   j=KV_q;
										k=0;
										break; 

				case 'r':   j=KV_r;
										k=0;
										break;

				case 's':   j=KV_s;
										k=0;
										break;

				case 't':   j=KV_t;
										k=0;
										break;

				case 'u':   j=KV_u;
										k=0;
										break;

				case 'v':   j=KV_v;
										k=0;
										break; 

				case 'w':   j=KV_w;
										k=0;
										break; 

				case 'x':		j=KV_x;
										k=0; 
										break;

				case 'y':   j=KV_y;
										k=0;
										break;

				case 'z':		j=KV_z;
										k=0;
										break;

				case '{':   j=KV_LeftBracket;
										k=KV_LeftShift;
										break;

				case '|':   j=KV_BackSlash;
										k=KV_LeftShift;
										break; 

				case '}':   j=KV_RightBracket;
										k=KV_LeftShift;
										break;

				case '~':   j=KV_backQuote;
										k=KV_LeftShift;
										break;

				default:    
										j=0;
										k=0;		
										break;
																
				
			}
			key_buffer[0] = k;
			key_buffer[2] = j;			
		break;
		
		
		case keyboard_GERMANY:
			switch(value)
			{
/****************************************************/		
				case 0x08:	j=KV_Backspace;
										k=0;
										break;
										

				case '\t':	j=KV_Tab;	//0x09
										k=0;
										break;


				case '\n':	j=KV_Return;	//0x0A
										k=0;
										break;

				case '\r':	j=KV_Return;	//0x0D
										k=0;
										break;			
										
				case 0x1B:	j=KV_Escape;
										k=0;
										break;	

/****************************************************/						
				
				case ' ':		j=KV_Space;
										k=0;
										break;		 
				
				case '!':		j=KV_1;
										k=KV_LeftShift;
										break;

				case '"':		j=KV_2;
										k=KV_LeftShift;
										break;

				case '#':		j=KV_BackSlash;
										k=0;
										break;

				case '$':   j=KV_4;
										k=KV_LeftShift;
										break;

				case '%':   j=KV_5;
										k=KV_LeftShift;
										break;

				case '&':   j=KV_6;
										k=KV_LeftShift;
										break;

				case '\'':  j=KV_BackSlash;
										k=KV_LeftShift;
										break; 

				case '(':   j=KV_8;
										k=KV_LeftShift;
										break;

				case ')':   j=KV_9;
										k=KV_LeftShift;
										break;

				case '*':   j=KV_RightBracket;
										k=KV_LeftShift;
										break; 

				case '+':   j=KV_RightBracket;
										k=0;
										break; 

				case ',':   j=KV_Comma;
										k=0;
										break; 

				case '-':   j=KV_Slash;
										k=0;
										break;

				case '.':   j=KV_Period;
										k=0;
										break;

				case '/':   j=KV_7;
										k=KV_LeftShift;
										break; 

				case '0':   j=KV_0;
										k=0;
										break; 

				case '1':   j=KV_1;
										k=0;
										break;

				case '2':   j=KV_2;
										k=0;
										break;

				case '3':   j=KV_3;
										k=0;
										break;

				case '4':   j=KV_4;
										k=0;
										break;

				case '5':   j=KV_5;
										k=0;
										break;   

				case '6':   j=KV_6;
										k=0;
										break; 

				case '7':   j=KV_7;
										k=0;
										break; 

				//--------------------

				case '8':   j=KV_8;
										k=0;
										break; 

				case '9':   j=KV_9;
										k=0;
										break;

				case ':':   j=KV_Period;
										k=KV_LeftShift;
										break; 

				case ';':   j=KV_Comma;
										k=KV_LeftShift;
										break; 

				case '<':   j=KV_Europe2;
										k=0;
										break; 

				case '=':   j=KV_0;
										k=KV_LeftShift;
										break;

				case '>':   j=KV_Europe2;
										k=KV_LeftShift;
										break; 

				case '?':   j=KV_Minus;
										k=KV_LeftShift;
										break;

				case '@':   j=KV_q;
										k=KV_RightAlt;
										break;

				case 'A':   j=KV_a;
										k=KV_LeftShift;
										break;

				case 'B':   j=KV_b;
										k=KV_LeftShift;
										break;

				case 'C':   j=KV_c;
										k=KV_LeftShift;
										break;

				case 'D':   j=KV_d;
										k=KV_LeftShift;
										break; 

				case 'E':		j=KV_e;
										k=KV_LeftShift;
										break;

				case 'F':   j=KV_f;
										k=KV_LeftShift;
										break;

				case 'G':   j=KV_g;
										k=KV_LeftShift;
										break;

				case 'H':   j=KV_h;
										k=KV_LeftShift;
										break;

				case 'I':   j=KV_i;
										k=KV_LeftShift;
										break;

				case 'J':   j=KV_j;
										k=KV_LeftShift;
										break;

				case 'K':   j=KV_k;
										k=KV_LeftShift;
										break;

				case 'L':   j=KV_l;
										k=KV_LeftShift;
										break; 

				case 'M':   j=KV_m;
										k=KV_LeftShift;
										break; 

				case 'N':   j=KV_n;
										k=KV_LeftShift;
										break;

				case 'O':   j=KV_o;
										k=KV_LeftShift;
										break;

				case 'P':   j=KV_p;
										k=KV_LeftShift;  
										break;

				case 'Q':   j=KV_q;
										k=KV_LeftShift;  
										break; 

				case 'R':   j=KV_r;
										k=KV_LeftShift;
										break;

				case 'S':   j=KV_s;
										k=KV_LeftShift;  
										break;

				case 'T':   j=KV_t;
										k=KV_LeftShift;
										break;

				case 'U':   j=KV_u;
										k=KV_LeftShift;  
										break; 

				case 'V':   j=KV_v;
										k=KV_LeftShift;  
										break;

				case 'W':   j=KV_w;
										k=KV_LeftShift;
										break;

				case 'X':   j=KV_x;
										k=KV_LeftShift;
										break;

				case 'Y':   j=KV_z;
										k=KV_LeftShift;  
										break;

				case 'Z':   j=KV_y;
										k=KV_LeftShift;  
										break;

				case '[':   j=KV_8;
										k=KV_RightAlt;
										break;

				case '\\':  j=KV_Minus;
										k=KV_RightAlt;
										break;   

				case ']':   j=KV_9;
										k=KV_RightAlt;
										break; 

				case '^':   j=KV_backQuote;
										k=0;
										key_buffer[0] = k;
										key_buffer[2] = j;	
										hid_report_send(&usb_device_dev, key_buffer, 8);            //Send the key_buffer message 
										while(prev_transfer_complete==0);	
										delay_1ms(10);
										
										j=KV_Space;
										k=0;																								
										break; 

				case '_':   j=KV_Slash;
										k=KV_LeftShift;
										break;

				case '`': 	j=KV_Equal;
										k=KV_LeftShift;
										key_buffer[0] = k;
										key_buffer[2] = j;											
										hid_report_send(&usb_device_dev, key_buffer, 8);            //Send the key_buffer message 
										while(prev_transfer_complete==0);	
										delay_1ms(10);
										
										j=KV_Space;
										k=0;
										break;

				case 'a':   j=KV_a;
										k=0;
										break;

				case 'b':   j=KV_b;
										k=0;
										break;  

				case 'c':   j=KV_c;
										k=0;
										break; 

				case 'd':   j=KV_d;
										k=0;
										break; 

				case 'e':   j=KV_e;
										k=0;
										break;

				case 'f':	  j=KV_f;
										k=0;
										break;

				case 'g':   j=KV_g;
										k=0;
										break;

				case 'h':   j=KV_h;
										k=0;
										break;  

				case 'i':   j=KV_i;
										k=0;
										break;  

				case 'j':   j=KV_j;
										k=0;
										break; 

				case 'k':   j=KV_k;
										k=0; 
										break; 

				case 'l':   j=KV_l;
										k=0;
										break;

				case 'm':   j=KV_m;
										k=0;
										break;

				case 'n':   j=KV_n;
										k=0;
										break;

				case 'o':   j=KV_o;
										k=0;
										break;

				case 'p':   j=KV_p;
										k=0;
										break; 

				case 'q':   j=KV_q;
										k=0;
										break;

				case 'r':   j=KV_r;
										k=0;
										break;

				case 's':   j=KV_s;
										k=0;
										break;

				case 't':   j=KV_t;
										k=0;
										break;

				case 'u':   j=KV_u;
										k=0;
										break;

				case 'v':   j=KV_v;
										k=0;
										break; 

				case 'w':   j=KV_w;
										k=0;
										break; 

				case 'x':		j=KV_x;
										k=0; 
										break;

				case 'y':   j=KV_z;
										k=0;
										break;

				case 'z':		j=KV_y;
										k=0;
										break;

				case '{':   j=KV_7;
										k=KV_RightAlt;
										break;

				case '|':   j=KV_Europe2;
										k=KV_RightAlt;
										break; 

				case '}':   j=KV_0;
										k=KV_RightAlt;
										break;

				case '~':   j=KV_RightBracket;
										k=KV_RightAlt;
										break;

				default:   
										j=0;
										k=0;
					break;
			}
			key_buffer[0] = k;
			key_buffer[2] = j;	
		break;		
		
			
			
		case keyboard_FRANCE_CAN:
			switch(value)
			{
/****************************************************/	
				case 0x08:	j=KV_Backspace;
										k=0;
										break;
										

				case '\t':	j=KV_Tab;	//0x09
										k=0;
										break;


				case '\n':	j=KV_Return;	//0x0A
										k=0;
										break;

				case '\r':	j=KV_Return;	//0x0D
										k=0;
										break;			
										
				case 0x1B:	j=KV_Escape;
										k=0;
										break;				
				
/****************************************************/						
				
				case ' ':		j=KV_Space;
										k=0;
										break;		 

				case '!':		j=KV_1;
										k=KV_LeftShift;
										break;

				case '"':		j=KV_2;
										k=KV_LeftShift;
										break;

				case '#':		j=KV_backQuote;
										k=0;
										break;

				case '$':   j=KV_4;
										k=KV_LeftShift;
										break;

				case '%':   j=KV_5;
										k=KV_LeftShift;
										break;

				case '&':   j=KV_7;
										k=KV_LeftShift;
										break;

				case '\'':  j=KV_Comma;
										k=KV_LeftShift;
										break; 

				case '(':   j=KV_9;
										k=KV_LeftShift;
										break;

				case ')':   j=KV_0;
										k=KV_LeftShift;
										break;

				case '*':   j=KV_8;
										k=KV_LeftShift;
										break;

				case '+':   j=KV_Equal;
										k=KV_LeftShift;
										break; 

				case ',':   j=KV_Comma;
										k=0;
										break; 

				case '-':   j=KV_Minus;
										k=0;
										break;

				case '.':   j=KV_Period;
										k=0;
										break; 

				case '/':   j=KV_3;
										k=KV_LeftShift;
										break; 

				case '0':   j=KV_0;
										k=0;
										break; 

				case '1':   j=KV_1;
										k=0;
										break;

				case '2':   j=KV_2;
										k=0;
										break;

				case '3':   j=KV_3;
										k=0;
										break;

				case '4':   j=KV_4;
										k=0;
										break;

				case '5':   j=KV_5;
										k=0;
										break;   

				case '6':   j=KV_6;
										k=0;
										break; 

				case '7':   j=KV_7;
										k=0;
										break; 

				//--------------------

				case '8':   j=KV_8;
										k=0;
										break; 

				case '9':   j=KV_9;
										k=0;
										break;

				case ':':   j=KV_Semicolon;
										k=KV_LeftShift;
										break; 

				case ';':   j=KV_Semicolon;
										k=0;
										break; 

				case '<':   j=KV_BackSlash;
										k=0;
										break; 

				case '=':   j=KV_Equal;
										k=0;
										break;

				case '>':   j=KV_BackSlash;
										k=KV_LeftShift;
										break; 

				case '?':   j=KV_6;
										k=KV_LeftShift;
										break;

				case '@':   j=KV_2;
										k=KV_RightAlt;
										break;

				case 'A':   j=KV_a;
										k=KV_LeftShift;
										break;

				case 'B':   j=KV_b;
										k=KV_LeftShift;
										break;

				case 'C':   j=KV_c;
										k=KV_LeftShift;
										break;

				case 'D':   j=KV_d;
										k=KV_LeftShift;
										break; 

				case 'E':		j=KV_e;
										k=KV_LeftShift;
										break;

				case 'F':   j=KV_f;
										k=KV_LeftShift;
										break;

				case 'G':   j=KV_g;
										k=KV_LeftShift;
										break;

				case 'H':   j=KV_h;
										k=KV_LeftShift;
										break;

				case 'I':   j=KV_i;
										k=KV_LeftShift;
										break;

				case 'J':   j=KV_j;
										k=KV_LeftShift;
										break;

				case 'K':   j=KV_k;
										k=KV_LeftShift;
										break;

				case 'L':   j=KV_l;
										k=KV_LeftShift;
										break; 

				case 'M':   j=KV_m;
										k=KV_LeftShift;
										break; 

				case 'N':   j=KV_n;
										k=KV_LeftShift;
										break;

				case 'O':   j=KV_o;
										k=KV_LeftShift;
										break;

				case 'P':   j=KV_p;
										k=KV_LeftShift;  
										break;

				case 'Q':   j=KV_q;
										k=KV_LeftShift;  
										break; 

				case 'R':   j=KV_r;
										k=KV_LeftShift;
										break;

				case 'S':   j=KV_s;
										k=KV_LeftShift;  
										break;

				case 'T':   j=KV_t;
										k=KV_LeftShift;
										break;

				case 'U':   j=KV_u;
										k=KV_LeftShift;  
										break; 

				case 'V':   j=KV_v;
										k=KV_LeftShift;  
										break;

				case 'W':   j=KV_w;
										k=KV_LeftShift;
										break;

				case 'X':   j=KV_x;
										k=KV_LeftShift;
										break;

				case 'Y':   j=KV_y;
										k=KV_LeftShift;  
										break;

				case 'Z':   j=KV_z;
										k=KV_LeftShift;  
										break;

				case '[':   j=KV_LeftBracket;
										k=KV_RightAlt;
										break;

				case '\\':  j=KV_backQuote;
										k=KV_RightAlt;
										break;   

				case ']':   j=KV_RightBracket;
										k=KV_RightAlt;
										break; 

				case '^':   j=KV_LeftBracket;
										k=KV_LeftShift;
										
								
										key_buffer[0] = k;
										key_buffer[2] = j;	
										hid_report_send(&usb_device_dev, key_buffer, 8);            //Send the key_buffer message 
										while(prev_transfer_complete==0);	
										delay_1ms(10);									

										j=KV_Space;
										k=0;																								
										break; 

				case '_':   j=KV_Minus;
										k=KV_LeftShift;
										break;

				case '`':		j=KV_SingleQuote;
										k=KV_LeftShift;
										key_buffer[0] = k;
										key_buffer[2] = j;	
										hid_report_send(&usb_device_dev, key_buffer, 8);            //Send the key_buffer message 
										while(prev_transfer_complete==0);	
										delay_1ms(10);
										j=KV_Space;
										k=0;
										break;

				case 'a':   j=KV_a;
										k=0;
										break;

				case 'b':   j=KV_b;
										k=0;
										break;  

				case 'c':   j=KV_c;
										k=0;
										break; 

				case 'd':   j=KV_d;
										k=0;
										break; 

				case 'e':   j=KV_e;
										k=0;
										break;

				case 'f':	  j=KV_f;
										k=0;
										break;

				case 'g':   j=KV_g;
										k=0;
										break;

				case 'h':   j=KV_h;
										k=0;
										break;  

				case 'i':   j=KV_i;
										k=0;
										break;  

				case 'j':   j=KV_j;
										k=0;
										break; 

				case 'k':   j=KV_k;
										k=0; 
										break; 

				case 'l':   j=KV_l;
										k=0;
										break;

				case 'm':   j=KV_m;
										k=0;
										break;

				case 'n':   j=KV_n;
										k=0;
										break;

				case 'o':   j=KV_o;
										k=0;
										break;

				case 'p':   j=KV_p;
										k=0;
										break; 

				case 'q':   j=KV_q;
										k=0;
										break;

				case 'r':   j=KV_r;
										k=0;
										break;

				case 's':   j=KV_s;
										k=0;
										break;

				case 't':   j=KV_t;
										k=0;
										break;

				case 'u':   j=KV_u;
										k=0;
										break;

				case 'v':   j=KV_v;
										k=0;
										break; 

				case 'w':   j=KV_w;
										k=0;
										break; 

				case 'x':		j=KV_x;
										k=0; 
										break;

				case 'y':   j=KV_y;
										k=0;
										break;

				case 'z':		j=KV_z;
										k=0;
										break;

				case '{':   j=KV_SingleQuote;
										k=KV_RightAlt;
										break;

				case '|':   j=KV_backQuote;
										k=KV_LeftShift;
										break; 

				case '}':   j=KV_BackSlash;
										k=KV_RightAlt;
										break;

				case '~':   j=KV_Semicolon;
										k=KV_RightAlt;
										break;

				default:   
										j=0;
										k=0;
					break;			
			}
			
			key_buffer[0] = k;
			key_buffer[2] = j;				
		break;		


		case keyboard_FRANCE:
			switch(value)
			{
/****************************************************/		
				case 0x08:	j=KV_Backspace;
										k=0;
										break;
										

				case '\t':	j=KV_Tab;	//0x09
										k=0;
										break;


				case '\n':	j=KV_Return;	//0x0A
										k=0;
										break;

				case '\r':	j=KV_Return;	//0x0D
										k=0;
										break;			
										
				case 0x1B:	j=KV_Escape;
										k=0;
										break;				
				
/****************************************************/						
				
				case ' ':		j=KV_Space;
										k=0;
										break;		

				case '!':		j=KV_Slash;
										k=0;
										break;

				case '"':		j=KV_3;
										k=0;
										break;

				case '#':		j=KV_3;
										k=KV_RightAlt;
										break;

				case '$':   j=KV_RightBracket;
										k=0;
										break;

				case '%':   j=KV_SingleQuote;
										k=KV_LeftShift;
										break;

				case '&':   j=KV_1;
										k=0;
										break;

				case '\'':  j=KV_4;
										k=0;
										break; 

				case '(':   j=KV_5;
										k=0;
										break;

				case ')':   j=KV_Minus;
										k=0;
										break;

				case '*':   j=KV_BackSlash;
										k=0;
										break;

				case '+':   j=KV_Equal;
										k=KV_LeftShift;
										break; 

				case ',':   j=KV_m;
										k=0;
										break; 

				case '-':   j=KV_6;
										k=0;
										break;

				case '.':   j=KV_Comma;
										k=KV_LeftShift;
										break; 

				case '/':   j=KV_Period;
										k=KV_LeftShift;
										break; 

				case '0':   j=KV_0;
										k=KV_LeftShift;
										break; 

				case '1':   j=KV_1;
										k=KV_LeftShift;
										break;

				case '2':   j=KV_2;
										k=KV_LeftShift;
										break;

				case '3':   j=KV_3;
										k=KV_LeftShift;
										break;

				case '4':   j=KV_4;
										k=KV_LeftShift;
										break;

				case '5':   j=KV_5;
										k=KV_LeftShift;
										break;   

				case '6':   j=KV_6;
										k=KV_LeftShift;
										break; 

				case '7':   j=KV_7;
										k=KV_LeftShift;
										break; 

				//--------------------

				case '8':   j=KV_8;
										k=KV_LeftShift;
										break; 

				case '9':   j=KV_9;
										k=KV_LeftShift;
										break;

				case ':':   j=KV_Period;
										k=0;
										break; 

				case ';':   j=KV_Comma;
										k=0;
										break; 

				case '<':   j=KV_Europe2;
										k=0;
										break; 

				case '=':   j=KV_Equal;
										k=0;
										break;

				case '>':   j=KV_Europe2;
										k=KV_LeftShift;
										break; 

				case '?':   j=KV_m;
										k=KV_LeftShift;
										break;

				case '@':   j=KV_0;
										k=KV_RightAlt;
										break;

				case 'A':   j=KV_q;
										k=KV_LeftShift;
										break;

				case 'B':   j=KV_b;
										k=KV_LeftShift;
										break;

				case 'C':   j=KV_c;
										k=KV_LeftShift;
										break;

				case 'D':   j=KV_d;
										k=KV_LeftShift;
										break; 

				case 'E':		j=KV_e;
										k=KV_LeftShift;
										break;

				case 'F':   j=KV_f;
										k=KV_LeftShift;
										break;

				case 'G':   j=KV_g;
										k=KV_LeftShift;
										break;

				case 'H':   j=KV_h;
										k=KV_LeftShift;
										break;

				case 'I':   j=KV_i;
										k=KV_LeftShift;
										break;

				case 'J':   j=KV_j;
										k=KV_LeftShift;
										break;

				case 'K':   j=KV_k;
										k=KV_LeftShift;
										break;

				case 'L':   j=KV_l;
										k=KV_LeftShift;
										break; 

				case 'M':   j=KV_Semicolon;
										k=KV_LeftShift;
										break; 

				case 'N':   j=KV_n;
										k=KV_LeftShift;
										break;

				case 'O':   j=KV_o;
										k=KV_LeftShift;
										break;

				case 'P':   j=KV_p;
										k=KV_LeftShift;  
										break;

				case 'Q':   j=KV_a;
										k=KV_LeftShift;  
										break; 

				case 'R':   j=KV_r;
										k=KV_LeftShift;
										break;

				case 'S':   j=KV_s;
										k=KV_LeftShift;  
										break;

				case 'T':   j=KV_t;
										k=KV_LeftShift;
										break;

				case 'U':   j=KV_u;
										k=KV_LeftShift;  
										break; 

				case 'V':   j=KV_v;
										k=KV_LeftShift;  
										break;

				case 'W':   j=KV_z;
										k=KV_LeftShift;
										break;

				case 'X':   j=KV_x;
										k=KV_LeftShift;
										break;

				case 'Y':   j=KV_y;
										k=KV_LeftShift;  
										break;

				case 'Z':   j=KV_w;
										k=KV_LeftShift;  
										break;

				case '[':   j=KV_5;
										k=KV_RightAlt;
										break;

				case '\\':  j=KV_8;
										k=KV_RightAlt;
										break;   

				case ']':   j=KV_Minus;
										k=KV_RightAlt;
										break; 

				case '^':   j=KV_9;
										k=KV_RightAlt;
										break; 

				case '_':   j=KV_8;
										k=0;
										break;

				case '`':		j=KV_7;
										k=KV_RightAlt;
										key_buffer[0] = k;
										key_buffer[2] = j;	
										hid_report_send(&usb_device_dev, key_buffer, 8);            //Send the key_buffer message 
										while(prev_transfer_complete==0);	
										delay_1ms(10);

										j=KV_Space;
										k=0;
										break;

				case 'a':   j=KV_q;
										k=0;
										break;

				case 'b':   j=KV_b;
										k=0;
										break;  

				case 'c':   j=KV_c;
										k=0;
										break; 

				case 'd':   j=KV_d;
										k=0;
										break; 

				case 'e':   j=KV_e;
										k=0;
										break;

				case 'f':	  j=KV_f;
										k=0;
										break;

				case 'g':   j=KV_g;
										k=0;
										break;

				case 'h':   j=KV_h;
										k=0;
										break;  

				case 'i':   j=KV_i;
										k=0;
										break;  

				case 'j':   j=KV_j;
										k=0;
										break; 

				case 'k':   j=KV_k;
										k=0; 
										break; 

				case 'l':   j=KV_l;
										k=0;
										break;

				case 'm':   j=KV_Semicolon;
										k=0;
										break;

				case 'n':   j=KV_n;
										k=0;
										break;

				case 'o':   j=KV_o;
										k=0;
										break;

				case 'p':   j=KV_p;
										k=0;
										break; 

				case 'q':   j=KV_a;
										k=0;
										break;

				case 'r':   j=KV_r;
										k=0;
										break;

				case 's':   j=KV_s;
										k=0;
										break;

				case 't':   j=KV_t;
										k=0;
										break;

				case 'u':   j=KV_u;
										k=0;
										break;

				case 'v':   j=KV_v;
										k=0;
										break; 

				case 'w':   j=KV_z;
										k=0;
										break; 

				case 'x':		j=KV_x;
										k=0; 
										break;

				case 'y':   j=KV_y;
										k=0;
										break;

				case 'z':		j=KV_w;
										k=0;
										break;

				case '{':   j=KV_4;
										k=KV_RightAlt;
										break;

				case '|':   j=KV_6;
										k=KV_RightAlt;
										break; 

				case '}':   j=KV_Equal;
										k=KV_RightAlt;
										break;

				case '~':   j=KV_2;
										k=KV_RightAlt;
										key_buffer[0] = k;
										key_buffer[2] = j;	
										hid_report_send(&usb_device_dev, key_buffer, 8);            //Send the key_buffer message 
										while(prev_transfer_complete==0);	
										delay_1ms(10);

										j=KV_Space;
										k=0;
										break;

				default:   
										j=0;
										k=0;
				break;											
			}
			key_buffer[0] = k;
			key_buffer[2] = j;		
		break;			
			
		case keyboard_ITALIAN:
			switch(value)
			{
/****************************************************/		
				case 0x08:	j=KV_Backspace;
										k=0;
										break;
										

				case '\t':	j=KV_Tab;	//0x09
										k=0;
										break;


				case '\n':	j=KV_Return;	//0x0A
										k=0;
										break;

				case '\r':	j=KV_Return;	//0x0D
										k=0;
										break;			
										
				case 0x1B:	j=KV_Escape;
										k=0;
										break;				
				
/****************************************************/						
				
				case ' ':		j=KV_Space;
										k=0;
										break;		

				case '!':		j=KV_1;
										k=KV_LeftShift;
										break;

				case '"':		j=KV_2;
										k=KV_LeftShift;
										break;

				case '#':		j=KV_3;
										k=KV_RightAlt;
										break;

				case '$':	j=KV_4;
										k=KV_LeftShift;
										break;

				case '%':	j=KV_5;
										k=KV_LeftShift;
										break;

				case '&':	j=KV_6;
										k=KV_LeftShift;
										break;

				case '\'':	j=KV_Minus; //mark
										k=0;
										break; 

				case '(':	j=KV_8;
										k=KV_LeftShift;
										break;

				case ')':	j=KV_9;
										k=KV_LeftShift;
										break;

				case '*':	j=KV_RightBracket;
										k=KV_LeftShift;
										break; 

				case '+':	j=KV_RightBracket;
										k=0;
										break; 

				case ',':	j=KV_Comma;
										k=0;
										break; 

				case '-':	j=KV_Slash;
										k=0;
										break;

				case '.':	j=KV_Period;
										k=0;
										break;

				case '/':	j=KV_7;
										k=KV_LeftShift;
										break; 

				case '0':	j=KV_0;
										k=0;
										break; 

				case '1':	j=KV_1;
										k=0;
										break;

				case '2':	j=KV_2;
										k=0;
										break;

				case '3':	j=KV_3;
										k=0;
										break;

				case '4':	j=KV_4;
										k=0;
										break;

				case '5':	j=KV_5;
										k=0;
										break;	 

				case '6':	j=KV_6;
										k=0;
										break; 

				case '7':	j=KV_7;
										k=0;
										break; 

				//--------------------

				case '8':	j=KV_8;
										k=0;
										break; 

				case '9':	j=KV_9;
										k=0;
										break;

				case ':':	j=KV_Period;
										k=KV_LeftShift;
										break; 

				case ';':	j=KV_Comma;
										k=KV_LeftShift;
										break; 

				case '<':	j=KV_Europe2;
										k=0;
										break; 

				case '=':	j=KV_0;
										k=KV_LeftShift;
										break;

				case '>':	j=KV_Europe2;
										k=KV_LeftShift;
										break; 

				case '?':	j=KV_Minus;
										k=KV_LeftShift;
										break;

				case '@':	j=KV_q;
										k=KV_RightAlt;
										break;

				case 'A':	j=KV_a;
										k=KV_LeftShift;
										break;

				case 'B':	j=KV_b;
										k=KV_LeftShift;
										break;

				case 'C':	j=KV_c;
										k=KV_LeftShift;
										break;

				case 'D':	j=KV_d;
										k=KV_LeftShift;
										break; 

				case 'E':		j=KV_e;
										k=KV_LeftShift;
										break;

				case 'F':	j=KV_f;
										k=KV_LeftShift;
										break;

				case 'G':	j=KV_g;
										k=KV_LeftShift;
										break;

				case 'H':	j=KV_h;
										k=KV_LeftShift;
										break;

				case 'I':	j=KV_i;
										k=KV_LeftShift;
										break;

				case 'J':	j=KV_j;
										k=KV_LeftShift;
										break;

				case 'K':	j=KV_k;
										k=KV_LeftShift;
										break;

				case 'L':	j=KV_l;
										k=KV_LeftShift;
										break; 

				case 'M':	j=KV_m;
										k=KV_LeftShift;
										break; 

				case 'N':	j=KV_n;
										k=KV_LeftShift;
										break;

				case 'O':	j=KV_o;
										k=KV_LeftShift;
										break;

				case 'P':	j=KV_p;
										k=KV_LeftShift;  
										break;

				case 'Q':	j=KV_q;
										k=KV_LeftShift;  
										break; 

				case 'R':	j=KV_r;
										k=KV_LeftShift;
										break;

				case 'S':	j=KV_s;
										k=KV_LeftShift;  
										break;

				case 'T':	j=KV_t;
										k=KV_LeftShift;
										break;

				case 'U':	j=KV_u;
										k=KV_LeftShift;  
										break; 

				case 'V':	j=KV_v;
										k=KV_LeftShift;  
										break;

				case 'W':	j=KV_w;
										k=KV_LeftShift;
										break;

				case 'X':	j=KV_x;
										k=KV_LeftShift;
										break;

				case 'Y':	j=KV_y;
										k=KV_LeftShift;  
										break;

				case 'Z':	j=KV_z;
										k=KV_LeftShift;  
										break;

				case '[':	j=KV_8; 					//corrected
										k=KV_RightAlt;
										break;

				case '\\':	j=KV_backQuote;
										k=0;
										break;	 

				case ']':	j=KV_9; 					//corrected
										k=KV_RightAlt;
										break; 

				case '^':	j=KV_Equal;
										k=KV_LeftShift;
										break; 

				case '_':	j=KV_Slash;
										k=KV_LeftShift;
										break;

				case '`':	j=KV_BackSlash;
										k=KV_RightAlt;
										break;

				case 'a':	j=KV_a;
										k=0;
										break;

				case 'b':	j=KV_b;
										k=0;
										break;	

				case 'c':	j=KV_c;
										k=0;
										break; 

				case 'd':	j=KV_d;
										k=0;
										break; 

				case 'e':	j=KV_e;
										k=0;
										break;

				case 'f':	  j=KV_f;
										k=0;
										break;

				case 'g':	j=KV_g;
										k=0;
										break;

				case 'h':	j=KV_h;
										k=0;
										break;	

				case 'i':	j=KV_i;
										k=0;
										break;	

				case 'j':	j=KV_j;
										k=0;
										break; 

				case 'k':	j=KV_k;
										k=0; 
										break; 

				case 'l':	j=KV_l;
										k=0;
										break;

				case 'm':	j=KV_m;
										k=0;
										break;

				case 'n':	j=KV_n;
										k=0;
										break;

				case 'o':	j=KV_o;
										k=0;
										break;

				case 'p':	j=KV_p;
										k=0;
										break; 

				case 'q':	j=KV_q;
										k=0;
										break;

				case 'r':	j=KV_r;
										k=0;
										break;

				case 's':	j=KV_s;
										k=0;
										break;

				case 't':	j=KV_t;
										k=0;
										break;

				case 'u':	j=KV_u;
										k=0;
										break;

				case 'v':	j=KV_v;
										k=0;
										break; 

				case 'w':	j=KV_w;
										k=0;
										break; 

				case 'x':		j=KV_x;
										k=0; 
										break;

				case 'y':	j=KV_y;
										k=0;
										break;

				case 'z':		j=KV_z;
										k=0;
										break;

				case '{':	j=KV_7;
										k=KV_RightAlt;
										break;

				case '|':	j=KV_backQuote;
										k=KV_LeftShift;
										break; 

				case '}':	j=KV_0;
										k=KV_RightAlt;
										break;

				case '~':	j=KV_RightBracket;
										k=KV_RightAlt;
										break;

				default:	
							j=0;
							k=0;
				break;										
			}
			key_buffer[0] = k;
			key_buffer[2] = j;		
		break;

		case keyboard_PORTUGUESE:
			switch(value)
			{
/****************************************************/		
				case 0x08:	j=KV_Backspace;
										k=0;
										break;
										

				case '\t':	j=KV_Tab;	//0x09
										k=0;
										break;


				case '\n':	j=KV_Return;	//0x0A
										k=0;
										break;

				case '\r':	j=KV_Return;	//0x0D
										k=0;
										break;			
										
				case 0x1B:	j=KV_Escape;
										k=0;
										break;			
				
/****************************************************/						
				
				case ' ':		j=KV_Space;
										k=0;
										break;		

				case '!':		j=KV_1;
										k=KV_LeftShift;
										break;

				case '"':		j=KV_2;
										k=KV_LeftShift;
										break;

				case '#':		j=KV_3;						//corrected
										k=KV_LeftShift;		//corrected
										break;

				case '$':   j=KV_4;
										k=KV_LeftShift;
										break;

				case '%':   j=KV_5;
										k=KV_LeftShift;
										break;

				case '&':   j=KV_6;
										k=KV_LeftShift;
										break;

				case '\'':  j=KV_Minus;
										k=0;
										break; 

				case '(':   j=KV_8;
										k=KV_LeftShift;
										break;

				case ')':   j=KV_9;
										k=KV_LeftShift;
										break;

				case '*':   j=KV_LeftBracket;
										k=KV_LeftShift;
										break; 

				case '+':   j=KV_LeftBracket;
										k=0;
										break; 

				case ',':   j=KV_Comma;
										k=0;
										break; 

				case '-':   j=KV_Slash;
										k=0;
										break;

				case '.':   j=KV_Period;
										k=0;
										break;

				case '/':   j=KV_7;
										k=KV_LeftShift;
										break; 

				case '0':   j=KV_0;
										k=0;
										break; 

				case '1':   j=KV_1;
										k=0;
										break;

				case '2':   j=KV_2;
										k=0;
										break;

				case '3':   j=KV_3;
										k=0;
										break;

				case '4':   j=KV_4;
										k=0;
										break;

				case '5':   j=KV_5;
										k=0;
										break;   

				case '6':   j=KV_6;
										k=0;
										break; 

				case '7':   j=KV_7;
										k=0;
										break; 

				//--------------------

				case '8':   j=KV_8;
										k=0;
										break; 

				case '9':   j=KV_9;
										k=0;
										break;

				case ':':   j=KV_Period;
										k=KV_LeftShift;
										break; 

				case ';':   j=KV_Comma;
										k=KV_LeftShift;
										break; 

				case '<':   j=KV_Europe2;
										k=0;
										break; 

				case '=':   j=KV_0;
										k=KV_LeftShift;
										break;

				case '>':   j=KV_Europe2;
										k=KV_LeftShift;
										break; 

				case '?':   j=KV_Minus;
										k=KV_LeftShift;
										break;

				case '@':   j=KV_2;
										k=KV_RightAlt;
										break;

				case 'A':   j=KV_a;
										k=KV_LeftShift;
										break;

				case 'B':   j=KV_b;
										k=KV_LeftShift;
										break;

				case 'C':   j=KV_c;
										k=KV_LeftShift;
										break;

				case 'D':   j=KV_d;
										k=KV_LeftShift;
										break; 

				case 'E':		j=KV_e;
										k=KV_LeftShift;
										break;

				case 'F':   j=KV_f;
										k=KV_LeftShift;
										break;

				case 'G':   j=KV_g;
										k=KV_LeftShift;
										break;

				case 'H':   j=KV_h;
										k=KV_LeftShift;
										break;

				case 'I':   j=KV_i;
										k=KV_LeftShift;
										break;

				case 'J':   j=KV_j;
										k=KV_LeftShift;
										break;

				case 'K':   j=KV_k;
										k=KV_LeftShift;
										break;

				case 'L':   j=KV_l;
										k=KV_LeftShift;
										break; 

				case 'M':   j=KV_m;
										k=KV_LeftShift;
										break; 

				case 'N':   j=KV_n;
										k=KV_LeftShift;
										break;

				case 'O':   j=KV_o;
										k=KV_LeftShift;
										break;

				case 'P':   j=KV_p;
										k=KV_LeftShift;  
										break;

				case 'Q':   j=KV_q;
										k=KV_LeftShift;  
										break; 

				case 'R':   j=KV_r;
										k=KV_LeftShift;
										break;

				case 'S':   j=KV_s;
										k=KV_LeftShift;  
										break;

				case 'T':   j=KV_t;
										k=KV_LeftShift;
										break;

				case 'U':   j=KV_u;
										k=KV_LeftShift;  
										break; 

				case 'V':   j=KV_v;
										k=KV_LeftShift;  
										break;

				case 'W':   j=KV_w;
										k=KV_LeftShift;
										break;

				case 'X':   j=KV_x;
										k=KV_LeftShift;
										break;

				case 'Y':   j=KV_y;
										k=KV_LeftShift;  
										break;

				case 'Z':   j=KV_z;
										k=KV_LeftShift;  
										break;

				case '[':   j=KV_8;
										k=KV_RightAlt;
										break;

				case '\\':  j=KV_backQuote;
										k=0;
										break;   

				case ']':   j=KV_9;
										k=KV_RightAlt;
										break; 

				case '^':   j=KV_BackSlash;
										k=KV_LeftShift;
										break; 

				case '_':   j=KV_Slash;
										k=KV_LeftShift;																							
										break;

				case '`': 	j=KV_RightBracket;
										k=KV_LeftShift;
										key_buffer[0] = k;
										key_buffer[2] = j;	
										hid_report_send(&usb_device_dev, key_buffer, 8);            //Send the key_buffer message 
										while(prev_transfer_complete==0);	
										delay_1ms(10);

										j=KV_Space;
										k=0;
										break;

				case 'a':   j=KV_a;
										k=0;
										break;

				case 'b':   j=KV_b;
										k=0;
										break;  

				case 'c':   j=KV_c;
										k=0;
										break; 

				case 'd':   j=KV_d;
										k=0;
										break; 

				case 'e':   j=KV_e;
										k=0;
										break;

				case 'f':	  j=KV_f;
										k=0;
										break;

				case 'g':   j=KV_g;
										k=0;
										break;

				case 'h':   j=KV_h;
										k=0;
										break;  

				case 'i':   j=KV_i;
										k=0;
										break;  

				case 'j':   j=KV_j;
										k=0;
										break; 

				case 'k':   j=KV_k;
										k=0; 
										break; 

				case 'l':   j=KV_l;
										k=0;
										break;

				case 'm':   j=KV_m;
										k=0;
										break;

				case 'n':   j=KV_n;
										k=0;
										break;

				case 'o':   j=KV_o;
										k=0;
										break;

				case 'p':   j=KV_p;
										k=0;
										break; 

				case 'q':   j=KV_q;
										k=0;
										break;

				case 'r':   j=KV_r;
										k=0;
										break;

				case 's':   j=KV_s;
										k=0;
										break;

				case 't':   j=KV_t;
										k=0;
										break;

				case 'u':   j=KV_u;
										k=0;
										break;

				case 'v':   j=KV_v;
										k=0;
										break; 

				case 'w':   j=KV_w;
										k=0;
										break; 

				case 'x':		j=KV_x;
										k=0; 
										break;

				case 'y':   j=KV_y;
										k=0;
										break;

				case 'z':		j=KV_z;
										k=0;
										break;

				case '{':   j=KV_7;
										k=KV_RightAlt;
										break;

				case '|':   j=KV_backQuote;
										k=KV_LeftShift;
										break; 

				case '}':   j=KV_0;
										k=KV_RightAlt;
										break;

				case '~':   j=KV_BackSlash;
										k=0;
										key_buffer[0] = k;
										key_buffer[2] = j;	
										hid_report_send(&usb_device_dev, key_buffer, 8);            //Send the key_buffer message 
										while(prev_transfer_complete==0);	
										delay_1ms(10);

										j=KV_Space;
										k=0;																								
										break;

				default:    
					j=0;
					k=0;
				break;										
			}
			key_buffer[0] = k;
			key_buffer[2] = j;			
		break;


		case keyboard_PORTUGUESE_BR:
			switch(value)
			{
/****************************************************/		
				case 0x08:	j=KV_Backspace;
										k=0;
										break;
										

				case '\t':	j=KV_Tab;	//0x09
										k=0;
										break;


				case '\n':	j=KV_Return;	//0x0A
										k=0;
										break;

				case '\r':	j=KV_Return;	//0x0D
										k=0;
										break;			
										
				case 0x1B:	j=KV_Escape;
										k=0;
										break;			
				
/****************************************************/						
				
				case ' ':		j=KV_Space;
										k=0;
										break;		

				case '!':		j=KV_1;
										k=KV_LeftShift;
										break;

				case '"':		j=KV_backQuote;
										k=KV_LeftShift;
										break;

				case '#':		j=KV_3;
										k=KV_LeftShift;  //corrected
										break;

				case '$':   j=KV_4;
										k=KV_LeftShift;
										break;

				case '%':   j=KV_5;
										k=KV_LeftShift;
										break;

				case '&':   j=KV_7;
										k=KV_LeftShift;
										break;

				case '\'':  j=KV_backQuote;
										k=0;
										break; 

				case '(':   j=KV_9;
										k=KV_LeftShift;
										break;

				case ')':   j=KV_0;
										k=KV_LeftShift;
										break;

				case '*':   j=KV_8;
										k=KV_LeftShift;
										break; 

				case '+':   j=KV_Equal;
										k=KV_LeftShift;
										break; 

				case ',':   j=KV_Comma;
										k=0;
										break; 

				case '-':   j=KV_Minus;
										k=0;
										break;

				case '.':   j=KV_Period;
										k=0;
										break;

				case '/':   j=KV_q;
										k=KV_RightAlt;
										break; 

				case '0':   j=KV_0;
										k=0;
										break; 

				case '1':   j=KV_1;
										k=0;
										break;

				case '2':   j=KV_2;
										k=0;
										break;

				case '3':   j=KV_3;
										k=0;
										break;

				case '4':   j=KV_4;
										k=0;
										break;

				case '5':   j=KV_5;
										k=0;
										break;   

				case '6':   j=KV_6;
										k=0;
										break; 

				case '7':   j=KV_7;
										k=0;
										break; 

				//--------------------

				case '8':   j=KV_8;
										k=0;
										break; 

				case '9':   j=KV_9;
										k=0;
										break;

				case ':':   j=KV_Slash;
										k=KV_LeftShift;
										break; 

				case ';':   j=KV_Slash;
										k=0;
										break; 

				case '<':   j=KV_Comma;
										k=KV_LeftShift;
										break; 

				case '=':   j=KV_Equal;
										k=0;
										break;

				case '>':   j=KV_Period;
										k=KV_LeftShift;
										break; 

				case '?':   j=KV_w;
										k=KV_RightAlt;
										break;

				case '@':   j=KV_2;
										k=KV_LeftShift;
										break;

				case 'A':   j=KV_a;
										k=KV_LeftShift;
										break;

				case 'B':   j=KV_b;
										k=KV_LeftShift;
										break;

				case 'C':   j=KV_c;
										k=KV_LeftShift;
										break;

				case 'D':   j=KV_d;
										k=KV_LeftShift;
										break; 

				case 'E':		j=KV_e;
										k=KV_LeftShift;
										break;

				case 'F':   j=KV_f;
										k=KV_LeftShift;
										break;

				case 'G':   j=KV_g;
										k=KV_LeftShift;
										break;

				case 'H':   j=KV_h;
										k=KV_LeftShift;
										break;

				case 'I':   j=KV_i;
										k=KV_LeftShift;
										break;

				case 'J':   j=KV_j;
										k=KV_LeftShift;
										break;

				case 'K':   j=KV_k;
										k=KV_LeftShift;
										break;

				case 'L':   j=KV_l;
										k=KV_LeftShift;
										break; 

				case 'M':   j=KV_m;
										k=KV_LeftShift;
										break; 

				case 'N':   j=KV_n;
										k=KV_LeftShift;
										break;

				case 'O':   j=KV_o;
										k=KV_LeftShift;
										break;

				case 'P':   j=KV_p;
										k=KV_LeftShift;  
										break;

				case 'Q':   j=KV_q;
										k=KV_LeftShift;  
										break; 

				case 'R':   j=KV_r;
										k=KV_LeftShift;
										break;

				case 'S':   j=KV_s;
										k=KV_LeftShift;  
										break;

				case 'T':   j=KV_t;
										k=KV_LeftShift;
										break;

				case 'U':   j=KV_u;
										k=KV_LeftShift;  
										break; 

				case 'V':   j=KV_v;
										k=KV_LeftShift;  
										break;

				case 'W':   j=KV_w;
										k=KV_LeftShift;
										break;

				case 'X':   j=KV_x;
										k=KV_LeftShift;
										break;

				case 'Y':   j=KV_y;
										k=KV_LeftShift;  
										break;

				case 'Z':   j=KV_z;
										k=KV_LeftShift;  
										break;

				case '[':   j=KV_RightBracket;
										k=0;
										break;

				case '\\':  j=KV_Europe2;
										k=0;
										break;   

				case ']':   j=KV_BackSlash;
										k=0;
										break; 

				case '^':   j=KV_SingleQuote;
										k=KV_LeftShift;
										key_buffer[0] = k;
										key_buffer[2] = j;	
										hid_report_send(&usb_device_dev, key_buffer, 8);            //Send the key_buffer message 
										while(prev_transfer_complete==0);	
										delay_1ms(10);

										j=KV_Space;
										k=0;																								
										break; 

				case '_':   j=KV_Minus;
										k=KV_LeftShift;
										break;

				case '`': 	j=KV_LeftBracket;
										k=KV_LeftShift;
										key_buffer[0] = k;
										key_buffer[2] = j;	
										hid_report_send(&usb_device_dev, key_buffer, 8);            //Send the key_buffer message 
										while(prev_transfer_complete==0);	
										delay_1ms(10);

										j=KV_Space;
										k=0;
										break;

				case 'a':   j=KV_a;
										k=0;
										break;

				case 'b':   j=KV_b;
										k=0;
										break;  

				case 'c':   j=KV_c;
										k=0;
										break; 

				case 'd':   j=KV_d;
										k=0;
										break; 

				case 'e':   j=KV_e;
										k=0;
										break;

				case 'f':	  j=KV_f;
										k=0;
										break;

				case 'g':   j=KV_g;
										k=0;
										break;

				case 'h':   j=KV_h;
										k=0;
										break;  

				case 'i':   j=KV_i;
										k=0;
										break;  

				case 'j':   j=KV_j;
										k=0;
										break; 

				case 'k':   j=KV_k;
										k=0; 
										break; 

				case 'l':   j=KV_l;
										k=0;
										break;

				case 'm':   j=KV_m;
										k=0;
										break;

				case 'n':   j=KV_n;
										k=0;
										break;

				case 'o':   j=KV_o;
										k=0;
										break;

				case 'p':   j=KV_p;
										k=0;
										break; 

				case 'q':   j=KV_q;
										k=0;
										break;

				case 'r':   j=KV_r;
										k=0;
										break;

				case 's':   j=KV_s;
										k=0;
										break;

				case 't':   j=KV_t;
										k=0;
										break;

				case 'u':   j=KV_u;
										k=0;
										break;

				case 'v':   j=KV_v;
										k=0;
										break; 

				case 'w':   j=KV_w;
										k=0;
										break; 

				case 'x':		j=KV_x;
										k=0; 
										break;

				case 'y':   j=KV_y;
										k=0;
										break;

				case 'z':		j=KV_z;
										k=0;
										break;

				case '{':   j=KV_RightBracket;
										k=KV_LeftShift;
										break;

				case '|':   j=KV_Europe2;
										k=KV_LeftShift;
										break; 

				case '}':   j=KV_BackSlash;
										k=KV_LeftShift;
										break;

				case '~':   j=KV_SingleQuote;
										k=0;
										key_buffer[0] = k;
										key_buffer[2] = j;	
										hid_report_send(&usb_device_dev, key_buffer, 8);            //Send the key_buffer message 
										while(prev_transfer_complete==0);	
										delay_1ms(10);

										j=KV_Space;
										k=0;																								
										break;

				default:    
					j=0;
					k=0;
				break;										
			}			
			key_buffer[0] = k;
			key_buffer[2] = j;						
		break;


		case keyboard_SPANISH:
			switch(value)
			{
/****************************************************/	
				case 0x08:	j=KV_Backspace;
										k=0;
										break;
										

				case '\t':	j=KV_Tab;	//0x09
										k=0;
										break;


				case '\n':	j=KV_Return;	//0x0A
										k=0;
										break;

				case '\r':	j=KV_Return;	//0x0D
										k=0;
										break;			
										
				case 0x1B:	j=KV_Escape;
										k=0;
										break;				
				
/****************************************************/						
				
				case ' ':		j=KV_Space;
										k=0;
										break;		


				case '!':		j=KV_1;
										k=KV_LeftShift;
										break;

				case '"':		j=KV_2;
										k=KV_LeftShift;
										break;

				case '#':		j=KV_3;
										k=KV_RightAlt;
										break;

				case '$':   j=KV_4;
										k=KV_LeftShift;
										break;

				case '%':   j=KV_5;
										k=KV_LeftShift;
										break;

				case '&':   j=KV_6;
										k=KV_LeftShift;
										break;

				case '\'':  j=KV_Minus;
										k=0;
										break; 

				case '(':   j=KV_8;
										k=KV_LeftShift;
										break;

				case ')':   j=KV_9;
										k=KV_LeftShift;
										break;

				case '*':   j=KV_RightBracket;
										k=KV_LeftShift;
										break; 

				case '+':   j=KV_RightBracket;
										k=0;
										break; 

				case ',':   j=KV_Comma;
										k=0;
										break; 

				case '-':   j=KV_Slash;
										k=0;
										break;

				case '.':   j=KV_Period;
										k=0;
										break;

				case '/':   j=KV_7;
										k=KV_LeftShift;
										break; 

				case '0':   j=KV_0;
										k=0;
										break; 

				case '1':   j=KV_1;
										k=0;
										break;

				case '2':   j=KV_2;
										k=0;
										break;

				case '3':   j=KV_3;
										k=0;
										break;

				case '4':   j=KV_4;
										k=0;
										break;

				case '5':   j=KV_5;
										k=0;
										break;   

				case '6':   j=KV_6;
										k=0;
										break; 

				case '7':   j=KV_7;
										k=0;
										break; 

				//--------------------

				case '8':   j=KV_8;
										k=0;
										break; 

				case '9':   j=KV_9;
										k=0;
										break;

				case ':':   j=KV_Period;
										k=KV_LeftShift;
										break; 

				case ';':   j=KV_Comma;
										k=KV_LeftShift;
										break; 

				case '<':   j=KV_Europe2;
										k=0;
										break; 

				case '=':   j=KV_0;
										k=KV_LeftShift;
										break;

				case '>':   j=KV_Europe2;
										k=KV_LeftShift;
										break; 

				case '?':   j=KV_Minus;
										k=KV_LeftShift;
										break;

				case '@':   j=KV_2;
										k=KV_RightAlt;
										break;

				case 'A':   j=KV_a;
										k=KV_LeftShift;
										break;

				case 'B':   j=KV_b;
										k=KV_LeftShift;
										break;

				case 'C':   j=KV_c;
										k=KV_LeftShift;
										break;

				case 'D':   j=KV_d;
										k=KV_LeftShift;
										break; 

				case 'E':		j=KV_e;
										k=KV_LeftShift;
										break;

				case 'F':   j=KV_f;
										k=KV_LeftShift;
										break;

				case 'G':   j=KV_g;
										k=KV_LeftShift;
										break;

				case 'H':   j=KV_h;
										k=KV_LeftShift;
										break;

				case 'I':   j=KV_i;
										k=KV_LeftShift;
										break;

				case 'J':   j=KV_j;
										k=KV_LeftShift;
										break;

				case 'K':   j=KV_k;
										k=KV_LeftShift;
										break;

				case 'L':   j=KV_l;
										k=KV_LeftShift;
										break; 

				case 'M':   j=KV_m;
										k=KV_LeftShift;
										break; 

				case 'N':   j=KV_n;
										k=KV_LeftShift;
										break;

				case 'O':   j=KV_o;
										k=KV_LeftShift;
										break;

				case 'P':   j=KV_p;
										k=KV_LeftShift;  
										break;

				case 'Q':   j=KV_q;
										k=KV_LeftShift;  
										break; 

				case 'R':   j=KV_r;
										k=KV_LeftShift;
										break;

				case 'S':   j=KV_s;
										k=KV_LeftShift;  
										break;

				case 'T':   j=KV_t;
										k=KV_LeftShift;
										break;

				case 'U':   j=KV_u;
										k=KV_LeftShift;  
										break; 

				case 'V':   j=KV_v;
										k=KV_LeftShift;  
										break;

				case 'W':   j=KV_w;
										k=KV_LeftShift;
										break;

				case 'X':   j=KV_x;
										k=KV_LeftShift;
										break;

				case 'Y':   j=KV_y;
										k=KV_LeftShift;  
										break;

				case 'Z':   j=KV_z;
										k=KV_LeftShift;  
										break;

				case '[':   j=KV_LeftBracket;
										k=KV_RightAlt;
										break;

				case '\\':  j=KV_backQuote;
										k=KV_RightAlt;
										break;   

				case ']':   j=KV_RightBracket;
										k=KV_RightAlt;
										break; 

				case '^':   j=KV_LeftBracket;
										k=KV_LeftShift;
										key_buffer[0] = k;
										key_buffer[2] = j;	
										hid_report_send(&usb_device_dev, key_buffer, 8);            //Send the key_buffer message 
										while(prev_transfer_complete==0);	
										delay_1ms(10);

										j=KV_Space;
										k=0;																								
										break; 

				case '_':   j=KV_Slash;
										k=KV_LeftShift;
										break;

				case '`': 	j=KV_LeftBracket;
										k=0;
										key_buffer[0] = k;
										key_buffer[2] = j;	
										hid_report_send(&usb_device_dev, key_buffer, 8);            //Send the key_buffer message 
										while(prev_transfer_complete==0);	
										delay_1ms(10);

										j=KV_Space;
										k=0;
										break;

				case 'a':   j=KV_a;
										k=0;
										break;

				case 'b':   j=KV_b;
										k=0;
										break;  

				case 'c':   j=KV_c;
										k=0;
										break; 

				case 'd':   j=KV_d;
										k=0;
										break; 

				case 'e':   j=KV_e;
										k=0;
										break;

				case 'f':	  j=KV_f;
										k=0;
										break;

				case 'g':   j=KV_g;
										k=0;
										break;

				case 'h':   j=KV_h;
										k=0;
										break;  

				case 'i':   j=KV_i;
										k=0;
										break;  

				case 'j':   j=KV_j;
										k=0;
										break; 

				case 'k':   j=KV_k;
										k=0; 
										break; 

				case 'l':   j=KV_l;
										k=0;
										break;

				case 'm':   j=KV_m;
										k=0;
										break;

				case 'n':   j=KV_n;
										k=0;
										break;

				case 'o':   j=KV_o;
										k=0;
										break;

				case 'p':   j=KV_p;
										k=0;
										break; 

				case 'q':   j=KV_q;
										k=0;
										break;

				case 'r':   j=KV_r;
										k=0;
										break;

				case 's':   j=KV_s;
										k=0;
										break;

				case 't':   j=KV_t;
										k=0;
										break;

				case 'u':   j=KV_u;
										k=0;
										break;

				case 'v':   j=KV_v;
										k=0;
										break; 

				case 'w':   j=KV_w;
										k=0;
										break; 

				case 'x':		j=KV_x;
										k=0; 
										break;

				case 'y':   j=KV_y;
										k=0;
										break;

				case 'z':		j=KV_z;
										k=0;
										break;

				case '{':   j=KV_SingleQuote;
										k=KV_RightAlt;
										break;

				case '|':   j=KV_1;
										k=KV_RightAlt;
										break; 

				case '}':   j=KV_BackSlash;		//corrected
										k=KV_RightAlt;
										break;

				case '~':   j=KV_4;
										k=KV_RightAlt;
										key_buffer[0] = k;
										key_buffer[2] = j;	
										hid_report_send(&usb_device_dev, key_buffer, 8);            //Send the key_buffer message 
										while(prev_transfer_complete==0);	
										delay_1ms(10);

										j=KV_Space;
										k=0;																								
										break;

				default:    
					j=0;
					k=0;
				break;										
			}	
			key_buffer[0] = k;
			key_buffer[2] = j;	
		break;				
	}

}
