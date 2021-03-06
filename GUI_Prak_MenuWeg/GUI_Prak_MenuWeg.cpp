// GUI_Prak_MenuWeg.cpp : Definiert den Einstiegspunkt der Anwendung.
//

#include "stdafx.h"
#include "GUI_Prak_MenuWeg.h"

#define MAX_LOADSTRING 100
#define NEWTITLE 100000

// Globale Variablen:
HINSTANCE hInst;                                // Aktuelle Instanz
WCHAR szTitle[MAX_LOADSTRING];                  // Titelleistentext
WCHAR szWindowClass[MAX_LOADSTRING];            // Klassenname des Hauptfensters

// Vorwärtsdeklarationen der in diesem Codemodul enthaltenen Funktionen:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	DialTitle(HWND, UINT, WPARAM, LPARAM);
void				centerToParent(HWND hWnd);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					  _In_opt_ HINSTANCE hPrevInstance,
					  _In_ LPWSTR    lpCmdLine,
					  _In_ int       nCmdShow){
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Globale Zeichenfolgen initialisieren
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_GUIPRAKMENUWEG, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Anwendungsinitialisierung ausführen:
	if(!InitInstance(hInstance, nCmdShow)){
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GUIPRAKMENUWEG));

	MSG msg;

	// Hauptnachrichtenschleife:
	while(GetMessage(&msg, nullptr, 0, 0)){
		if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNKTION: MyRegisterClass()
//
//  ZWECK: Registriert die Fensterklasse.
//
ATOM MyRegisterClass(HINSTANCE hInstance){
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	// Doppelklick regestrieren
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GUIPRAKMENUWEG));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_GUIPRAKMENUWEG);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNKTION: InitInstance(HINSTANCE, int)
//
//   ZWECK: Speichert das Instanzenhandle und erstellt das Hauptfenster.
//
//   KOMMENTARE:
//
//        In dieser Funktion wird das Instanzenhandle in einer globalen Variablen gespeichert, und das
//        Hauptprogrammfenster wird erstellt und angezeigt.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow){
	hInst = hInstance; // Instanzenhandle in der globalen Variablen speichern

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
							  CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if(!hWnd){
		return FALSE;
	}

	centerToParent(hWnd);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}

//
//  FUNKTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ZWECK:  Verarbeitet Meldungen vom Hauptfenster.
//
//  WM_COMMAND  - Verarbeiten des Anwendungsmenüs
//  WM_PAINT    - Darstellen des Hauptfensters
//  WM_DESTROY  - Ausgeben einer Beendenmeldung und zurückkehren
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	// Anfangsmenü
	static HMENU menu = GetMenu(hWnd);
	// Rechteck des Textes
	static RECT rcText;
	switch(message){
		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// Menüauswahl bearbeiten:
			switch(wmId){
				case IDM_TITLE:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_TITLE), hWnd, DialTitle);
					break;
				case IDM_CLOSE:
					PostQuitMessage(0);
					break;
				case IDM_MENUWEG:
					SetMenu(hWnd, NULL);
					break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// größe des Inhaltrechtecks
			RECT rc;
			// größe des Textrechtecks (x, y)
			SIZE size;
			int x, y;

			GetClientRect(hWnd, &rc);
			GetTextExtentPoint(hdc, szTitle, wcslen(szTitle), &size);

			//   mitte des Fensterinhaltes             -  mitte des Textes
			x = (rc.left + ((rc.right - rc.left) / 2)) - (size.cx / 2);
			y = (rc.top + ((rc.bottom - rc.top) / 2)) - (size.cy / 2);

			// setzen der Werte des Textrechtecks
			rcText.top = y;
			rcText.bottom = y + size.cy;
			rcText.left = x;
			rcText.right = x + size.cx;

			TextOut(hdc, x, y, szTitle, wcslen(szTitle));
			EndPaint(hWnd, &ps);
		}
		break;
		case NEWTITLE:
		{
			WCHAR * newTitle = reinterpret_cast<WCHAR *>(lParam);
			wcscpy_s(szTitle, newTitle);
			SetWindowText(hWnd, szTitle);
			InvalidateRect(hWnd, nullptr, true);
		}
		break;
		case WM_RBUTTONDBLCLK:
		{
			int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
			if(x >= rcText.left && x <= rcText.right
			   && y >= rcText.top && y <= rcText.bottom){
				SetMenu(hWnd, menu);
			}
		}
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return FALSE;
}


INT_PTR CALLBACK DialTitle(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){
	switch(message){
		case WM_INITDIALOG:
			centerToParent(hDlg);
			return TRUE;
		case WM_COMMAND:
			if(LOWORD(wParam) == IDB_OK){
				// holen der Textlänge + Endezeichen
				int len = GetWindowTextLength(GetDlgItem(hDlg, IDC_TITLE)) + 1;
				WCHAR *newTitle = new wchar_t[len];

				// Holt den Text
				GetWindowText(GetDlgItem(hDlg, IDC_TITLE), newTitle, len);
				// Versendet den neuen Titel
				SendMessage(GetParent(hDlg), NEWTITLE, 0, reinterpret_cast<LPARAM>(newTitle));
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
		case WM_CLOSE:
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
	}
	return FALSE;
}

void centerToParent(HWND hWnd){

	HWND hOwner;
	RECT rc, rcDlg, rcOwner;

	if((hOwner = GetParent(hWnd)) == NULL){
		hOwner = GetDesktopWindow();
	}

	GetWindowRect(hOwner, &rcOwner);
	GetWindowRect(hWnd, &rcDlg);
	CopyRect(&rc, &rcOwner);

	// Schieben an position 0/0
	OffsetRect(&rcDlg, -rcDlg.left, -rcDlg.top);
	OffsetRect(&rc, -rc.left, -rc.top);
	// Größe des neuen Fesnters abziehen
	OffsetRect(&rc, -rcDlg.right, -rcDlg.bottom);

	SetWindowPos(hWnd, HWND_TOP, rcOwner.left + (rc.right / 2), rcOwner.top + (rc.bottom / 2), 0, 0, SWP_NOSIZE);
}