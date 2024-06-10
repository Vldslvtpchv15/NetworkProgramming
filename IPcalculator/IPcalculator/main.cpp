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

//BOOL CheckMask(DWORD mask)
//{
//	DWORD pattern = 1 << 32;
//	for (int i = 0; i < 32; i++)
//	{
//
//	}
//	return TRUE;
//}
//INT CountOnes(DWORD mask)
//{
//	INT zero_bits = 0;
//	DWORD power;
//	for (int i = 1; i; i <<= 1, zero_bits++)
//	{
//		if (mask & i)
//		{
//			power = 1;
//			break;
//		}
//	}
//
//	return 32-zero_bits;
//
//}

//CHAR* ParseAddress(CHAR sz_address[], CHAR sz_description, DWORD dw_address)
//{
//
//}

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
			HWND hIPAddress = GetDlgItem(hwnd, IDC_IPADDRESS);
			HWND hEditPrefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);
			HWND hIPmask = GetDlgItem(hwnd, IDC_IPMASK);
			HWND hStaticInfo = GetDlgItem(hwnd, IDC_STATIC_INFO);
			DWORD dw_mask =UINT_MAX;
			if (HIWORD(wParam) == EN_CHANGE)
			{
				CONST INT SIZE_PREFIX = 8;
				CHAR sz_buffer[SIZE_PREFIX];
				SendMessage(hEditPrefix, WM_GETTEXT, SIZE_PREFIX, (LPARAM)sz_buffer);
				INT shift = atoi(sz_buffer);
				/*dw_mask >> (32 - shift);
				dw_mask << (32 - shift);*/
				SendMessage(hIPmask, IPM_SETADDRESS, 0, dw_mask /*>> (32- shift)*/ <<= (32-shift));

				

				/////// Info ////////
				CONST INT SIZE = 256;
				CHAR sz_info[SIZE]{};
				CHAR sz_network_address[SIZE]{};
				CHAR sz_broadcast_address[SIZE]{};
				CHAR sz_number_of_hosts[SIZE]{};

				DWORD dw_address = 0;
				SendMessage(hIPAddress, IPM_GETADDRESS, 0, (LPARAM)&dw_address);
				DWORD dw_network_address = dw_address & dw_mask;

				
				sprintf
				(
					sz_network_address,
					"Info:\nАдрес сети:\t%i.%i.%i.%i",
					FIRST_IPADDRESS(dw_network_address),
					SECOND_IPADDRESS(dw_network_address),
					THIRD_IPADDRESS(dw_network_address),
					FOURTH_IPADDRESS(dw_network_address)
				);

				DWORD dw_broadcast_address = ~dw_mask | dw_network_address;
				sprintf
				(
					sz_broadcast_address,
					"Широковещательный адрес:\t%i.%i.%i.%i",
					FIRST_IPADDRESS(dw_broadcast_address),
					SECOND_IPADDRESS(dw_broadcast_address),
					THIRD_IPADDRESS(dw_broadcast_address),
					FOURTH_IPADDRESS(dw_broadcast_address)
				);

				DWORD dw_number_of_hosts = dw_broadcast_address - dw_network_address - 1;
				sprintf(sz_number_of_hosts, "Количество узлов:\t\t%i", dw_number_of_hosts);

				sprintf
				(
					sz_info,
					"Info:\n%s\n%s\n%s",
					sz_network_address,
					sz_broadcast_address,
					sz_number_of_hosts
				);

				SendMessage(hStaticInfo, WM_SETTEXT, 0, (LPARAM)sz_info);
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