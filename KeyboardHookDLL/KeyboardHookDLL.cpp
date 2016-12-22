#include <Windows.h>

#define WM_GLOBALKEYBOARD WM_USER+100

HHOOK g_hKeyboardHook = NULL;
DWORD g_dwThreadID = 0;


//LRESULT CALLBACK LowLevelKeyboardProc(
//	int nCode,      
//	WPARAM wParam,  
//	LPARAM lParam   
//	)
//{
//	// 按键释放为一个键值
//	if ( WM_KEYUP == wParam)
//	{
//		KBDLLHOOKSTRUCT* pKHook = (KBDLLHOOKSTRUCT*) lParam;
//
//		PostThreadMessage(g_dwThreadID, WM_GLOBALKEYBOARD, pKHook->vkCode, pKHook->scanCode);
//	}
//
//	return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
//}


//这是处理键盘消息的主要函数，在其中进行禁止操作
//LRESULT CALLBACK HookProc(int nCode,WPARAM wParam,LPARAM lParam)
//{
//	if (nCode < 0)
//	{
//		return CallNextHookEx(g_hKeyboardHook,nCode,wParam,lParam);
//	}
//	if (nCode != HC_ACTION)
//	{
//		return CallNextHookEx(g_hKeyboardHook,nCode,wParam,lParam);
//	}
//	 //给出提示：键盘已经被锁定，要进行判断，看是否已有提示窗口，否则会弹个没完
//    if (!::FindWindow(0, "KeyBoard Locked"))
//    {
//		::MessageBox(0,"键盘已经锁定！！！","KeyBoard Locked",MB_OK);
//    }
//    return 1;  //没有return CallNextHookEx(hhkHook,nCode,wParam,lParam)则不会把消息//传递下去，所以我们的键盘就不起作用了
//    
//}

//  低级键盘钩子处理函数  
LRESULT  CALLBACK  LowLevelKeyboardProc(int  nCode,  WPARAM  wParam,  LPARAM  lParam)  
{  
	BOOL  fEatKeystroke  =  FALSE;  
	PKBDLLHOOKSTRUCT  p  =  NULL;  
	if  (nCode  ==  HC_ACTION)    
	{  
		p  =  (PKBDLLHOOKSTRUCT)  lParam;  
		switch  (wParam)    
		{  
			case  WM_KEYDOWN:    
			case  WM_SYSKEYDOWN:  
			case  WM_KEYUP:          
			case  WM_SYSKEYUP:    
				fEatKeystroke  =  (p->vkCode  ==  VK_LWIN) || (p->vkCode  ==  VK_RWIN)||    //  屏蔽Win  
				//  屏蔽Alt+Tab  
				((p->vkCode  ==  VK_TAB)  &&  ((p->flags  &  LLKHF_ALTDOWN)  !=  0))|| 
				//  屏蔽Alt+Esc  
				((p->vkCode  ==  VK_ESCAPE)  &&  ((p->flags  &  LLKHF_ALTDOWN)  !=  0))||  
				
				//  屏蔽Ctrl+Esc  
				((p->vkCode  ==  VK_ESCAPE)  &&  ((GetKeyState(VK_CONTROL)  &  0x8000)  !=  0));  
				break;  
			default:  
				break;  
		}  
	}  
	return  (fEatKeystroke  ?  TRUE  :  CallNextHookEx(g_hKeyboardHook,nCode,wParam,lParam));  
}

extern "C" __declspec(dllexport) void SetHook(DWORD dwThreadID)
{
	HMODULE mod = GetModuleHandle("KeyboardHookDLL.dll");   //获取当前DLL模块句柄
	
	g_hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, mod, 0);

	g_dwThreadID = dwThreadID;

}

extern "C" __declspec(dllexport) void DeleteHook()
{

	UnhookWindowsHookEx(g_hKeyboardHook);

}