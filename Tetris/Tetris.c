/*-------------------------
	����˹������Ϸģ��
	-------------------------*/
#include <Windows.h>
#include "Tetris.h"

static BLOCKTYPE NowBlock,//��ǰ������������Ϣ
NextBlock;//�¸������������Ϣ
static POINT ptNowPos[4],//�����ĵ�ǰλ�ã�����Ϸ���ṹiClient�еĵ㣩
ptNextPos[4];//�������¸�λ��
static int iClient[VERT_BLK_NUMS][HORZ_BLK_NUMS];//��Ϸ����¼�ṹ
//��Ϸ�������״����һά�������򣨵ڶ�ά��
//4*4����ķ���״̬����ͼ�Σ�0Ϊ��ɫ��1Ϊ��ɫ
static int Member[][4][4][4] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0,
0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0,
0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0,
0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0
};
//��Ϸ���������
void ClientInit(int *score)
{
	int i, j;

	*score = 0;//��������
	for (i = 0; i < VERT_BLK_NUMS; i++)
	for (j = 0; j < HORZ_BLK_NUMS; j++)
		iClient[i][j] = 0;//��Ϸ���ṹ����
	return;
}
//��ȡ��һ������
void NextBlockInit(void)
{
	int iType, iStyle, i, j;
	//����Ϸ��״���������ѡ��һ����Ϊ�¸��������״�ͷ���
	iStyle = rand() / (RAND_MAX / 7);
	iType = rand() / (RAND_MAX / 4);
	for (i = 0; i < 4; i++)
	for (j = 0; j < 4; j++)
		NextBlock.block[i][j] = Member[iStyle][iType][i][j];

	NextBlock.style = iStyle;
	NextBlock.type = iType;

	return;
}
// ����/���� �ͻ��������
void PaintNowPos(HWND hwnd, RECT rect, int iBlockSize, BOOL bIsBlack)
{
	int i;
	HDC hdc;

	hdc = GetDC(hwnd);

	for (i = 0; i < 4; i++)
	{
		SelectObject(hdc, GetStockObject(bIsBlack ? BLACK_PEN : WHITE_PEN));//bIsBlack �����ǲ������ǻ��ƣ���/�׻��ʣ�
		Rectangle(hdc,
			rect.left + iBlockSize * ptNowPos[i].x + 1,
			rect.top + iBlockSize * ptNowPos[i].y + 1,
			rect.left + iBlockSize * (ptNowPos[i].x + 1) - 1,
			rect.top + iBlockSize * (ptNowPos[i].y + 1) - 1);
	}
	ReleaseDC(hwnd, hdc);
	return;
}
//�����¸�����
void PaintNextBlock(HWND hwnd, RECT rect, int iBlockSize)
{
	int i, j;
	HDC hdc;

	hdc = GetDC(hwnd);

	for (i = 0; i < 4; i++)		//�����¸�������ʽ
	for (j = 0; j < 4; j++)
	{
		SelectObject(hdc, GetStockObject(NextBlock.block[i][j] ? BLACK_PEN : WHITE_PEN));
		Rectangle(hdc,
			rect.right + iBlockSize * (j + 2) + 1,
			rect.bottom / 4 + iBlockSize * i + 1,
			rect.right + iBlockSize * (j + 3) - 1,
			rect.bottom / 4 + iBlockSize * (i + 1) - 1);
	}

	ReleaseDC(hwnd, hdc);
	return;
}
//����������Ϸ����
void PaintClient(HWND hwnd, RECT rect, int iBlockSize, int score, HFONT hfont)
{
	int i, j;
	HDC hdc;
	TCHAR temp[20];

	hdc = GetDC(hwnd);
	//������Ϸ���߿�
	Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
	//������Ϸ������
	for (i = 0; i < VERT_BLK_NUMS; i++)
	for (j = 0; j < HORZ_BLK_NUMS; j++)
	{
		SelectObject(hdc, GetStockObject(iClient[i][j] ? BLACK_PEN : WHITE_PEN));
		Rectangle(hdc,
			rect.left + iBlockSize * j + 1,
			rect.top + iBlockSize * i + 1,
			rect.left + iBlockSize * (j + 1) - 1,
			rect.top + iBlockSize * (i + 1) - 1);

	}
	PaintNextBlock(hwnd, rect, iBlockSize);

	hfont = (HFONT)SelectObject(hdc, hfont);
	wsprintf(temp, TEXT("������%d"), score);//��ʾ����
	TextOut(hdc, rect.right + iBlockSize, rect.bottom / 2, temp, wcslen(temp));
	hfont = (HFONT)SelectObject(hdc, hfont);

	ReleaseDC(hwnd, hdc);
	return;
}
//�����ṩ������ʽ������������x��y��4*4����ͼ���½ǣ������㷽������Ϸ��λ������
void GetBlockPos(int x, int y, const BLOCKTYPE *Block, POINT *NextPos)
{
	int i, j, iFirstLine = -1, num = 0;

	for (i = 0; i < 4; i++)
	for (j = 0; j < 4; j++)
	if (Block->block[i][j])
	{
		if (iFirstLine < 0)
			iFirstLine = i;
		NextPos[num].x = j + x;
		NextPos[num++].y = i + (y == -4 ? -iFirstLine : y);
	}
}
//�·����볡���ɹ�����TRUE,��֮ΪFALSE��ͬʱ�����¸�������ʽ��
BOOL NewBlockEntry(void)
{
	int i;

	NowBlock = NextBlock;//��ԭ���¸��������Ϊ�����
	NextBlockInit();//����¡��¸�������
	GetBlockPos(8, -4, &NowBlock, ptNextPos);//��ʼ�������λ��
	for (i = 0; i < 4; i++)
	if (iClient[ptNextPos[i].y][ptNextPos[i].x] == 1)//�жϻ�������λ���Ƿ��ѱ�ռ��
		return FALSE;//��ռ�ã���Ϸ����������false
	for (i = 0; i < 4; i++)
	{
		ptNowPos[i] = ptNextPos[i];
		iClient[ptNowPos[i].y][ptNowPos[i].x] = 1;//�ɹ��볡������Ϸ�������б��λ��
	}
	return TRUE;
}
//��û�����������½ǵĵ�ǰ��Ϸ��λ�ã�����ȷ���������λ��
void TranslatePos(const BLOCKTYPE *Block)
{
	int i, j, x, y;
	BOOL jump = FALSE;

	x = ptNowPos[0].x;
	y = ptNowPos[0].y;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		if (NowBlock.block[i][j])
		{
			jump = TRUE;
			break;
		}
		if (jump)
			break;
	}
	x -= j;
	y -= i;
	GetBlockPos(x, y, Block, ptNextPos);
	return;
}
//���㷽���¸�λ��
BOOL CanToNextPos(int iStyle)
{
	int i, j;
	BOOL bCanChange = TRUE;
	BLOCKTYPE temp;
	//�ȴ���Ϸ������ƶ�����ĵ�ǰλ�ã���������¸�λ��
	for (i = 0; i < 4; i++)
	{
		ptNextPos[i] = ptNowPos[i];
		iClient[ptNowPos[i].y][ptNowPos[i].x] = 0;
	}
	switch (iStyle)
	{
	case VK_LEFT:		//�����ƶ�
		for (i = 0; i < 4; i++)
		{
			ptNextPos[i].x -= 1;
			if (ptNextPos[i].x < 0 || iClient[ptNextPos[i].y][ptNextPos[i].x] == 1)
			{
				bCanChange = FALSE;
				break;
			}
		}
		break;
	case VK_RIGHT:		//�����ƶ�
		for (i = 0; i<4; i++)
		{
			ptNextPos[i].x += 1;
			if (ptNextPos[i].x > HORZ_BLK_NUMS - 1 || iClient[ptNextPos[i].y][ptNextPos[i].x] == 1)
			{
				bCanChange = FALSE;
				break;
			}
		}
		break;
	case VK_DOWN://����
		for (i = 0; i<4; i++)
		{
			ptNextPos[i].y += 1;
			if (ptNextPos[i].y > VERT_BLK_NUMS - 1 || iClient[ptNextPos[i].y][ptNextPos[i].x] == 1)
			{
				bCanChange = FALSE;
				break;
			}
		}
		break;
	case VK_SPACE:		//���鷭ת
		//�ȼ��㷭ת�󷽿����ʽ
		temp = NowBlock;
		if (++(temp.type) > 3)
			temp.type = 0;
		for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			temp.block[i][j] = Member[temp.style][temp.type][i][j];
		//ȷ������ʽ������
		TranslatePos(&temp);
		//�ж��������Ƿ�Ϸ�
		for (i = 0; i<4; i++)
		{
			if (ptNextPos[i].x > HORZ_BLK_NUMS - 1 || ptNextPos[i].x < 0 || ptNextPos[i].y > VERT_BLK_NUMS - 1 ||
				ptNextPos[i].y < 0 || iClient[ptNextPos[i].y][ptNextPos[i].x] == 1)
			{
				bCanChange = FALSE;
				break;
			}

		}
		if (bCanChange)
			NowBlock = temp;
		break;
	}

	for (i = 0; i < 4; i++)
	{
		if (bCanChange)
			ptNowPos[i] = ptNextPos[i];		//��λ�úϷ��Ļ���Ӧ����λ��
		iClient[ptNowPos[i].y][ptNowPos[i].x] = 1;	//���»����ƶ������λ��
	}
	return bCanChange;
}
//�������еķ��飬��������������
int CleanRow(void)
{
	int i, j, n, num = 0, NeedClean[4];
	BOOL jump;

	for (i = 0; i < VERT_BLK_NUMS; i++)
	{
		jump = FALSE;
		for (j = 0; j < HORZ_BLK_NUMS; j++)
		if (!iClient[i][j])
		{
			jump = TRUE;
			break;
		}
		if (!jump)
			NeedClean[num++] = i;
	}
	for (i = 0; i<num; i++)
	{
		for (j = NeedClean[i]; j>0; j--)
		{
			for (n = 0; n < HORZ_BLK_NUMS; n++)
				iClient[j][n] = iClient[j - 1][n];
		}
		for (n = 0; n < HORZ_BLK_NUMS; n++)
			iClient[0][n] = 0;
	}
	return num;
}
