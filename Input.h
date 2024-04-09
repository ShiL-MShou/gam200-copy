/******************************************************************************/
/*
	File Name: Input.h
	Project Name: WhereGotLate
	Author(s): Lee Shi Lin [100%]

	Brief: Input system keeps track of the state of the keyboard

	Copyright Information:
	All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

#include "System.h"
#include "Vector2.h"

/////////////////////////////////////////////////////////////////////
//Defines

#define MVK_LBUTTON		 VK_LBUTTON	    //!< Left mouse button
#define MVK_RBUTTON		 VK_RBUTTON	    //!< Right mouse button
#define MVK_MBUTTON		 VK_MBUTTON	    //!< Middle mouse button

#define MVK_BACK         VK_BACK         //!< Backspace
#define MVK_TAB          VK_TAB      
#define MVK_RETURN	     VK_RETURN		
#define MVK_LSHIFT		 VK_LSHIFT		
#define MVK_RSHIFT		 VK_RSHIFT		
#define MVK_LCTRL		 VK_LCONTROL		
#define MVK_RCTRL		 VK_RCONTROL		
#define MVK_LALT		 VK_LMENU		
#define MVK_RALT		 VK_RMENU		
#define MVK_PRINTSCREEN  VK_SNAPSHOT
#define MVK_SCROLLLOCK	 VK_SCROLL
#define MVK_PAUSE		 VK_PAUSE		
#define MVK_CAPSLOCK	 VK_CAPITAL

#define MVK_ESCAPE       VK_ESCAPE   

#define MVK_SPACE	     VK_SPACE		
#define MVK_PAGEUP       VK_PRIOR		
#define MVK_PAGEDOWN     VK_NEXT
#define MVK_END		     VK_END		
#define MVK_HOME		 VK_HOME		
#define MVK_LEFT		 VK_LEFT		
#define MVK_UP			 VK_UP			
#define MVK_RIGHT        VK_RIGHT		
#define MVK_DOWN         VK_DOWN		
#define MVK_INSERT		 VK_INSERT		
#define MVK_DELETE		 VK_DELETE	

#define MVK_0              0x30        
#define MVK_1              0x31        
#define MVK_2              0x32        
#define MVK_3              0x33        
#define MVK_4              0x34        
#define MVK_5              0x35        
#define MVK_6              0x36        
#define MVK_7              0x37        
#define MVK_8              0x38        
#define MVK_9              0x39        

#define MVK_A              0x41        
#define MVK_B              0x42        
#define MVK_C              0x43        
#define MVK_D              0x44        
#define MVK_E              0x45        
#define MVK_F              0x46        
#define MVK_G              0x47        
#define MVK_H              0x48        
#define MVK_I              0x49        
#define MVK_J              0x4A        
#define MVK_K              0x4B        
#define MVK_L              0x4C        
#define MVK_M              0x4D        
#define MVK_N              0x4E        
#define MVK_O              0x4F        
#define MVK_P              0x50        
#define MVK_Q              0x51        
#define MVK_R              0x52        
#define MVK_S              0x53        
#define MVK_T              0x54        
#define MVK_U              0x55        
#define MVK_V              0x56        
#define MVK_W              0x57        
#define MVK_X              0x58        
#define MVK_Y              0x59        
#define MVK_Z              0x5A        

#define MVK_NUMPAD0	       VK_NUMPAD0	
#define MVK_NUMPAD1	       VK_NUMPAD1	
#define MVK_NUMPAD2	       VK_NUMPAD2	
#define MVK_NUMPAD3	       VK_NUMPAD3	
#define MVK_NUMPAD4	       VK_NUMPAD4	
#define MVK_NUMPAD5        VK_NUMPAD5	
#define MVK_NUMPAD6	       VK_NUMPAD6	
#define MVK_NUMPAD7	       VK_NUMPAD7	
#define MVK_NUMPAD8	       VK_NUMPAD8	
#define MVK_NUMPAD9	       VK_NUMPAD9	

#define MVK_NUM_MULTIPLY   VK_MULTIPLY
#define MVK_NUM_PLUS	   VK_ADD
#define MVK_NUM_MINUS	   VK_SUBTRACT
#define MVK_NUM_PERIOD	   VK_DECIMAL
#define MVK_NUM_DIVIDE	   VK_DIVIDE
#define MVK_NUMLOCK	       VK_NUMLOCK	

#define MVK_F1			    VK_F1			
#define MVK_F2			    VK_F2			
#define MVK_F3			    VK_F3			
#define MVK_F4			    VK_F4			
#define MVK_F5			    VK_F5			
#define MVK_F6			    VK_F6			
#define MVK_F7			    VK_F7			
#define MVK_F8			    VK_F8			
#define MVK_F9			    VK_F9			
#define MVK_F10		        VK_F10		
#define MVK_F11		        VK_F11		
#define MVK_F12		        VK_F12		

#define MVK_SEMICOLON	    VK_OEM_1        //!< ; :
#define MVK_SLASH		    VK_OEM_2		//!< / ?
#define MVK_BACKQUOTE		VK_OEM_3        //!< ` ~
#define MVK_LBRACKET	    VK_OEM_4        //!< [ {
#define MVK_BACKSLASH	    VK_OEM_5        //!< \ |
#define MVK_RBRACKET	    VK_OEM_6        //!< ] }
#define MVK_QUOTE		    VK_OEM_7	    //!< ' "

#define MVK_EQUAL		    VK_OEM_PLUS     //!< = +		
#define MVK_MINUS		    VK_OEM_MINUS    //!< - _		
#define MVK_PERIOD		    VK_OEM_PERIOD   //!< . >		
#define MVK_COMMA		    VK_OEM_COMMA    //!< , <	

/////////////////////////////////////////////////////////////////////

const int KEYS_BUFFER_LENGTH = 256;

class Input : public ISystem
{
	// 2 buffer to keep track of the states
	char prevstate[KEYS_BUFFER_LENGTH];
	char currstate[KEYS_BUFFER_LENGTH];

	POINT mousePos;

public:
	void Initialize();

	~Input() {};

	void Update(float _dt) override;

	void Clear() override;

	std::string GetName() override { return "Input"; }

	/* Helper functions to get Key State */
	void ReadKeyboard();

	bool GetCurrState(unsigned char key);

	bool GetPrevState(unsigned char key);

	/* Check if Key was Pressed, Hold or Released */
	bool GetKeyPressed(unsigned char key);

	bool GetKeyHold(unsigned char key);

	bool GetKeyRelease(unsigned char key);

	/* Get Mouse Position relative to screen */
	VEC2 GetMousePosition();

	/* true - Show cursor in exe window screen */
	void SetCursorView(bool value);
};

extern std::shared_ptr<Input> INPUTMGR;