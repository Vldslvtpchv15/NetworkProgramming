#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <CommCtrl.h>
#include "resource.h"
#include <cstdio>

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR IpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
	
	return 0;
}

BOOL CheckMask(DWORD mask)
{
	DWORD pattern = 1 << 32;
	for (int i = 0; i < 32; i++)
	{

	}
	return TRUE;
}
INT CountOnes(DWORD mask)
{
	INT zero_bits = 0;
	DWORD power;
	for (int i = 1; i; i <<= 1, zero_bits++)
	{
		if (mask & i)
		{
			power = 1;
			break;
		}
	}

	return 32-zero_bits;

}



BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HWND hUpDown = GetDlgItem(hwnd, IDC_SPIN_PREFIX);
		SendMessage(hUpDown, UDM_SETRANGE32, 0, 32);

		HWND hIPadress = GetDlgItem(hwnd, IDC_IPADDRESS);
		SetFocus(hIPadress);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_IPADDRESS:
		{
			CONST INT SIZE = 256;
			CHAR sz_info[SIZE]{};

			HWND hIPadress = GetDlgItem(hwnd, IDC_IPADDRESS);
			HWND hStaticInfo = GetDlgItem(hwnd, IDC_STATIC_INFO);
			HWND hEditPrefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);
			if (HIWORD(wParam) == EN_CHANGE)
			{
				DWORD dw_adress;
				SendMessage(hIPadress, IPM_GETADDRESS, 0, (LPARAM)&dw_adress);
				INT first = FIRST_IPADDRESS(dw_adress);
				INT second = SECOND_IPADDRESS(dw_adress);
				INT third = THIRD_IPADDRESS(dw_adress);
				INT fourth = FOURTH_IPADDRESS(dw_adress);

				sprintf(sz_info, "Info:\nFirst: %i, Second %i, Third %i, Forth: %i", first, second, third, fourth);
				SendMessage(hStaticInfo, WM_SETTEXT, 0, (LPARAM)sz_info);


				if (first < 128)SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)"8");
				else if (first < 192)SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)"16");
				else if (first < 224)SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)"24");
				
			}
		}
		break;



		case WM_NOTIFY:
		{
			HWND hIPmask = GetDlgItem(hwnd, IDC_IPMASK);
			HWND hEditPrefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);
			switch (wParam)
			{
				case IDC_IPMASK:
				{
					DWORD dw_mask = 0;
					SendMessage(hIPmask, IPM_GETADDRESS, 0, (LPARAM)&dw_mask);
					int i = 32;
					for (; dw_mask & 1 ^ 1; i--)dw_mask >>= 1;
					CHAR sz_prefix[5]{};
					sprintf(sz_prefix, "%i", i);
					SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)sz_prefix);

				}
				break;
			}
			break;
		}		
		break;



		case IDC_EDIT_PREFIX:
		{
			HWND hEditPrefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);
			HWND hIPmask = GetDlgItem(hwnd, IDC_IPMASK);
			DWORD dw_mask =UINT_MAX;
			if (HIWORD(wParam) == EN_CHANGE)
			{
				CONST INT SIZE = 8;
				CHAR sz_buffer[SIZE];
				SendMessage(hEditPrefix, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
				INT shift = atoi(sz_buffer);
				dw_mask >> (32 - shift);
				dw_mask << (32 - shift);
				SendMessage(hIPmask, IPM_SETADDRESS, 0, dw_mask>> (32- shift) << (32-shift));
			}

		}
			break;



		case IDOK: MessageBox(hwnd, "Была нажата кнопка ОК", "Info", MB_OK | MB_ICONINFORMATION); break;



		case IDCANCEL: EndDialog(hwnd, 0); break; 
		}	
		break;



	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}