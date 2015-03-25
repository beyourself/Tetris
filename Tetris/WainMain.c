#include <Windows.h>
#include <time.h>
#include "resource.h"
#include "Tetris.h"

HINSTANCE hInstance;
TCHAR szAppName[] = TEXT("����˹����");

HFONT FontInitialize(HWND hwnd);//��ʼ����������
void ShowBitmap(HWND hwnd, RECT rect, TCHAR *szBitmapName);	//��ʾ��ͣ/����λͼ
BOOL CALLBACK DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmd, int nShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASSEX wcls;

	wcls.cbSize = sizeof(wcls);
	wcls.style = CS_HREDRAW | CS_VREDRAW;
	wcls.lpfnWndProc = WndProc;
	wcls.hInstance = hInst;
	wcls.cbWndExtra = 0;
	wcls.cbClsExtra = 0;
	wcls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//(COLOR_BTNFACE + 1);
	wcls.hIcon = LoadIcon(hInst, szAppName);
	wcls.hIconSm = LoadIcon(hInst, szAppName);
	wcls.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcls.lpszClassName = szAppName;
	wcls.lpszMenuName = szAppName;

	if (!RegisterClassEx(&wcls)){
		MessageBox(NULL, TEXT("ע�������"), TEXT("����"), MB_ICONERROR);
		return 0;
	}

	hInstance = hInst;

	hwnd = CreateWindow(szAppName,
		szAppName,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		400,
		500,
		NULL, NULL,
		hInst, NULL);
	ShowWindow(hwnd, nShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static RECT rect;
	static HFONT hfont;
	static BOOL bGameBegin, bGameOver, bGamePause;
	static int cxChar, cyChar, iBlockSize, score, iTime;
	int iCleanRows;
	HDC hdc;
	HMENU hmenu;
	PAINTSTRUCT ps;
	TEXTMETRIC txm;

	switch (message)
	{
	case WM_CREATE:
		srand((UINT)time(NULL));
		hfont = FontInitialize(hwnd);

		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &txm);
		cyChar = txm.tmHeight + txm.tmExternalLeading;
		ReleaseDC(hwnd, hdc);

		return 0;
	case WM_SIZE:
		GetClientRect(hwnd, &rect);
		rect.left = cyChar;
		rect.top = cyChar;
		iBlockSize = (rect.bottom - cyChar - 1) / VERT_BLK_NUMS;//��ʼ������Ĵ�С
		rect.right = iBlockSize * HORZ_BLK_NUMS + rect.left;
		rect.bottom = iBlockSize * VERT_BLK_NUMS + rect.top;
		return 0;
	case WM_INITMENUPOPUP:
		hmenu = GetMenu(hwnd);
		switch (LOWORD(lParam))
		{
		case 0:
			EnableMenuItem(hmenu, IDM_PAUSE, bGameBegin && !bGamePause ? MF_ENABLED : MF_DISABLED);
			EnableMenuItem(hmenu, IDM_RESUME, bGameBegin && bGamePause ? MF_ENABLED : MF_DISABLED);
		}
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_START:
			ClientInit(&score);//��ʼ���ͻ�������
			NextBlockInit();//�����µķ���
			NewBlockEntry();//�·��������Ϸ��

			hdc = GetDC(hwnd);
			PaintClient(hwnd, rect, iBlockSize, score, hfont);//��ʾ�ͻ���
			ReleaseDC(hwnd, hdc);

			iTime = 1000;//����������ٶ�
			bGameBegin = TRUE;
			bGameOver = bGamePause = FALSE;
			SetTimer(hwnd, ID_TIMER, iTime, NULL);
			break;
		case IDM_PAUSE:
			bGamePause = TRUE;
			ShowBitmap(hwnd, rect, TEXT("��ͣ"));
			KillTimer(hwnd, ID_TIMER);
			break;
		case IDM_RESUME:
			bGamePause = FALSE;
			InvalidateRect(hwnd, NULL, TRUE);
			SetTimer(hwnd, ID_TIMER, iTime, NULL);
			break;
		case IDM_ABOUT:
			DialogBox(hInstance, szAppName, hwnd, DlgProc);
			break;
		case IDM_HELP:
			MessageBox(hwnd, TEXT("�������ʼ��Ϸ����ʼ\n����������ҡ��¡����Ʒ����ƶ�\n���ո�������Ʒ��鷭ת"),
				szAppName, MB_ICONINFORMATION);
			break;
		case IDM_EXIT:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		}
		return 0;
	case WM_KEYDOWN:
		if (!bGameBegin || bGamePause)
			break;
		switch (wParam)
		{
		case VK_LEFT:
		case VK_RIGHT:
		case VK_SPACE:
			PaintNowPos(hwnd, rect, iBlockSize, FALSE);//���ԭλ��ͼ��
			CanToNextPos(wParam);
			PaintNowPos(hwnd, rect, iBlockSize, TRUE);//������λ��ͼ��
			return 0;
		case VK_DOWN:
			SetTimer(hwnd, ID_TIMER, 1, NULL);//���¡��������������
			return 0;
		}
		break;
	case WM_TIMER:
		PaintNowPos(hwnd, rect, iBlockSize, FALSE);//���ԭλ��ͼ��
		if (!CanToNextPos(VK_DOWN))//�жϻ�����Ƿ����䵽��
		{
			iCleanRows = CleanRow();
			score += iCleanRows;
			iTime = 1000 - score * 10;//�ӿ������ٶ�
			SetTimer(hwnd, ID_TIMER, iTime, NULL);//�����������䣬���ĳ��µ��ٶ�
			if (iCleanRows)
				//�����������У���Ϸ�������»��ƣ�����ֻ���ƻ�����ѽ��ʹ�����˸��
				PaintClient(hwnd, rect, iBlockSize, score, hfont);
			else
				PaintNowPos(hwnd, rect, iBlockSize, TRUE);//����ԭλ��ͼ��
			if (!NewBlockEntry())//�ж���Ϸ�Ƿ����
			{//��Ϸ������
				bGameOver = GameOver(hwnd, &bGameBegin);
				ShowBitmap(hwnd, rect, TEXT("��Ϸ����"));
				return 0;
			}
			//��Ϸδ������
			PaintNextBlock(hwnd, rect, iBlockSize);//���������ɵġ��¸�������ͼ��
		}
		PaintNowPos(hwnd, rect, iBlockSize, TRUE);//������λ��ͼ��
		return 0;
	case WM_PAINT:
		BeginPaint(hwnd, &ps);

		PaintClient(hwnd, rect, iBlockSize, score, hfont);
		if (bGamePause)
			ShowBitmap(hwnd, rect, TEXT("��ͣ"));
		if (bGameOver)
			ShowBitmap(hwnd, rect, TEXT("��Ϸ����"));

		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		KillTimer(hwnd, ID_TIMER);
		DeleteObject(hfont);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
//��ʼ����������
HFONT FontInitialize(HWND hwnd)
{
	static LOGFONT logfont;

	wcscpy_s(logfont.lfFaceName, sizeof(logfont.lfFaceName),TEXT("����"));
	logfont.lfHeight = 14;

	return CreateFontIndirect(&logfont);
}
//������Ӧλͼ
void ShowBitmap(HWND hwnd, RECT rect, TCHAR *szBitmapName)
{
	HDC hdc, hdcMem, hdcMask;
	BITMAP bitmap;
	HBITMAP hBitmap, hBitmapMask;

	hdc = GetDC(hwnd);
	//����λͼ������λͼ��С
	hBitmap = LoadBitmap(hInstance, szBitmapName);
	GetObject(hBitmap, sizeof(BITMAP), &bitmap);
	//�����ڴ��豸������ѡ��λͼ
	hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, hBitmap);
	//������ɫλͼ����ѡ���µ��ڴ滷��
	hBitmapMask = CreateBitmap(bitmap.bmWidth, bitmap.bmHeight, 1, 1, NULL);
	hdcMask = CreateCompatibleDC(hdc);
	SelectObject(hdcMask, hBitmapMask);
	//�ڵ�ɫ�ڴ��豸�����д�������λͼ������ɫΪ1������ɫΪ0
	SetBkColor(hdcMem, RGB(255, 255, 255));
	BitBlt(hdcMask, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
	//��Ŀ��λͼ����ɫ��Ϊ0
	BitBlt(hdcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMask, 0, 0, 0x220326);
	//����Ŀ��λ����ɫ��Ϊ��ɫ��0��
	BitBlt(hdc,
		rect.left + (rect.right - rect.left - bitmap.bmWidth) / 2,
		rect.top + (rect.bottom - rect.top - bitmap.bmHeight) / 2,
		bitmap.bmWidth, bitmap.bmHeight, hdcMask, 0, 0, SRCAND);
	//��Դλͼ�ʹ���Ŀ�������ࡰ��
	BitBlt(hdc,
		rect.left + (rect.right - rect.left - bitmap.bmWidth) / 2,
		rect.top + (rect.bottom - rect.top - bitmap.bmHeight) / 2,
		bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCPAINT);

	DeleteDC(hdcMem);
	DeleteDC(hdcMask);
	DeleteObject(hBitmap);
	DeleteObject(hBitmapMask);
	ReleaseDC(hwnd, hdc);
	return;
}

//��Ϸ���������Ӧ����
BOOL GameOver(HWND hwnd, BOOL *bGameBegin)
{
	*bGameBegin = FALSE;
	KillTimer(hwnd, ID_TIMER);
	MessageBeep(MB_ICONWARNING);
	return TRUE;
}
//�����ڡ� �Ի�����Ϣ������
BOOL CALLBACK DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			EndDialog(hwnd, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}