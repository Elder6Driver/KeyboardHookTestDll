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
//	// �����ͷ�Ϊһ����ֵ
//	if ( WM_KEYUP == wParam)
//	{
//		KBDLLHOOKSTRUCT* pKHook = (KBDLLHOOKSTRUCT*) lParam;
//
//		PostThreadMessage(g_dwThreadID, WM_GLOBALKEYBOARD, pKHook->vkCode, pKHook->scanCode);
//	}
//
//	return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
//}


//���Ǵ��������Ϣ����Ҫ�����������н��н�ֹ����
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
//	 //������ʾ�������Ѿ���������Ҫ�����жϣ����Ƿ�������ʾ���ڣ�����ᵯ��û��
//    if (!::FindWindow(0, "KeyBoard Locked"))
//    {
//		::MessageBox(0,"�����Ѿ�����������","KeyBoard Locked",MB_OK);
//    }
//    return 1;  //û��return CallNextHookEx(hhkHook,nCode,wParam,lParam)�򲻻����Ϣ//������ȥ���������ǵļ��̾Ͳ���������
//    
//}

//  �ͼ����̹��Ӵ�����  
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
				fEatKeystroke  =  (p->vkCode  ==  VK_LWIN) || (p->vkCode  ==  VK_RWIN)||    //  ����Win  
				//  ����Alt+Tab  
				((p->vkCode  ==  VK_TAB)  &&  ((p->flags  &  LLKHF_ALTDOWN)  !=  0))|| 
				//  ����Alt+Esc  
				((p->vkCode  ==  VK_ESCAPE)  &&  ((p->flags  &  LLKHF_ALTDOWN)  !=  0))||  
				
				//  ����Ctrl+Esc  
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
	HMODULE mod = GetModuleHandle("KeyboardHookDLL.dll");   //��ȡ��ǰDLLģ����
	
	g_hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, mod, 0);

	g_dwThreadID = dwThreadID;

}

extern "C" __declspec(dllexport) void DeleteHook()
{

	UnhookWindowsHookEx(g_hKeyboardHook);

}