#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <CommCtrl.h>
#include <cstdio>
#include "resource.h"

#pragma comment(lib, "Comctl32.lib")

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
CHAR* FormatAddress(CHAR szBuffer[], CONST CHAR szMessage[], DWORD dwAddress);
CHAR* FormatNumber(CHAR szBuffer[], CONST CHAR szMessage[], DWORD dwNumber);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
	
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		InitCommonControls();
		HWND hSpinPrefix = GetDlgItem(hwnd, IDC_SPIN_PREFIX);
		SendMessage(hSpinPrefix, UDM_SETRANGE, 0, 32);
	}
		break;
	case WM_COMMAND:
	{
		HWND hIPaddress = GetDlgItem(hwnd, IDC_IPADDRESS);
		HWND hIPmask = GetDlgItem(hwnd, IDC_IPMASK);
		HWND hEditPrefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);
		HWND hStaticInfo = GetDlgItem(hwnd, IDC_STATIC_INFO);

				DWORD dwIPAddress = 0;
				DWORD dwIPmask = UINT_MAX;
				DWORD dwIPprefix = 0;
				CHAR szIPprefix[3] = {};
		switch (LOWORD(wParam))
		{
		case IDC_IPADDRESS:
		{
			if (HIWORD(wParam) == EN_CHANGE)
			{
			/*	DWORD dwIPAddress = 0;
				DWORD dwIPmask = UINT_MAX;
				DWORD dwIPprefix = 0;
				CHAR szIPprefix[3] = {};*/
				SendMessage(hIPaddress, IPM_GETADDRESS, 0, (LPARAM)&dwIPAddress);
				DWORD dwFirst = FIRST_IPADDRESS(dwIPAddress);
				if (dwFirst < 128)dwIPprefix = 8;
				else if (dwFirst < 192)dwIPprefix = 16;
				else if (dwFirst < 224)dwIPprefix = 24;
			

				sprintf(szIPprefix, "%i", dwIPprefix);
				SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)szIPprefix);
				dwIPmask <<= (32 - dwIPprefix);
				SendMessage(hIPmask, IPM_SETADDRESS, 0, dwIPmask);
			}
		}
		break;
	/*	case IDC_IPMASK:
		{
			if (HIWORD(wParam) == EN_CHANGE)
			{
				DWORD dwIPAddress = 0;
				DWORD dwIPmask = UINT_MAX;
				DWORD dwIPprefix = 0;
				CHAR szIPprefix[3] = {};
				SendMessage(hIPmask, IPM_GETADDRESS, 0, (LPARAM)&dwIPmask);
				for (; dwIPmask; dwIPmask <<= 1)dwIPprefix++;
				sprintf(szIPprefix, "%i", dwIPprefix);
				SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)szIPprefix);
			}
		}
		break;*/
		case IDC_EDIT_PREFIX:
		{
			if (HIWORD(wParam) == EN_CHANGE)
			{
		/*		DWORD dwIPAddress = 0;
				DWORD dwIPmask = UINT_MAX;
				DWORD dwIPprefix = 0;
				CHAR szIPprefix[3] = {};*/

				SendMessage(hEditPrefix, WM_GETTEXT, 3, (LPARAM)szIPprefix);
				dwIPprefix = atoi(szIPprefix);
				if (dwIPprefix > 31)
				{
					dwIPprefix = 31;
					strcpy(szIPprefix, "31");
					SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)szIPprefix);
				}
				dwIPmask <<= (32 - dwIPprefix);
				SendMessage(hIPmask, IPM_SETADDRESS, 0, dwIPmask);
			}
		}
		break;
		case IDOK:
		{

			SendMessage(hIPaddress, IPM_GETADDRESS, 0, (LPARAM)&dwIPAddress);
			SendMessage(hIPmask, IPM_GETADDRESS, 0, (LPARAM)&dwIPmask);
			DWORD dwNetworkAddress = dwIPAddress & dwIPmask;
			DWORD dwBroadcastADdress = dwIPAddress | ~dwIPmask;
			DWORD dwCapacity = dwBroadcastADdress - dwNetworkAddress + 1;
			DWORD dwHosts = dwCapacity - 2;


			CONST INT SIZE = 256;
			CHAR szInfo[SIZE] = "";
			CHAR szNetwork[SIZE] = "";
			CHAR szBroadcast[SIZE] = "";
			CHAR szCapacity[SIZE] = "";
			CHAR szHost[SIZE] = "";

			sprintf
			(
				szInfo,
				"Info:\n%s\n%s\n%s\n%s\n",
				FormatAddress(szNetwork,"Адрес сети:\t\t\t ", dwNetworkAddress),
				FormatAddress(szBroadcast,"Широковещательный адрес:\t ", dwBroadcastADdress),
				FormatNumber(szCapacity,"Колличество IP-адрессов:\t ",dwCapacity),
				FormatNumber(szHost,"Колличество Узлов:\t\t ",dwHosts)
			);
			SendMessage(hStaticInfo, WM_SETTEXT, 0, (LPARAM)szInfo);
		}
			break;
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		}
	}
		break;
	case WM_NOTIFY:
	{
	case IDC_IPMASK:
	{
		//if (wParam == IDC_IPMASK) 
		if (((LPNMHDR)lParam)->code == IPN_FIELDCHANGED) 
		{
			
			HWND hIPmask = GetDlgItem(hwnd, IDC_IPMASK);
			HWND hEditPrefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);
			DWORD dwIPAddress = 0;
			DWORD dwIPmask = UINT_MAX;
			DWORD dwIPprefix = 0;
			CHAR szIPprefix[3] = {};
			SendMessage(hIPmask, IPM_GETADDRESS, 0, (LPARAM)&dwIPmask);
			for (; dwIPmask >> 31; dwIPmask <<= 1)dwIPprefix++;
			/*
			dwIPmask = UINT_MAX << (32- dwIPprefix);
			SendMessage(hIPmask, IPM_SETADDRESS, 0, (LPARAM)&dwIPmask);
			*/

			sprintf(szIPprefix, "%i", dwIPprefix);
			SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)szIPprefix);
	
		}
		break;
	}
	break;
	}
	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}

CHAR* FormatAddress(CHAR szBuffer[], CONST CHAR szMessage[], DWORD dwAddress)
{
	sprintf
	(
		szBuffer,
		"%s%i.%i.%i.%i;",
		szMessage,
		FIRST_IPADDRESS(dwAddress),
		SECOND_IPADDRESS(dwAddress),
		THIRD_IPADDRESS(dwAddress),
		FOURTH_IPADDRESS(dwAddress)
	);
	return szBuffer;
}

CHAR* FormatNumber(CHAR szBuffer[], CONST CHAR szMessage[], DWORD dwNumber)
{
	sprintf
	(
		szBuffer,
		"%s%i;",
		szMessage,
		dwNumber
		);
	return szBuffer;
}