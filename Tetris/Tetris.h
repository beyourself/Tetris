//�ͻ����ķ���ĺ�������
#define HORZ_BLK_NUMS		20	//ˮƽ���򷽿������
#define VERT_BLK_NUMS		32	//��ֱ���򷽿������
#define ID_TIMER	1	//��ʱ��ID

//������Ϣ�ṹ��
typedef struct {
	int style;		//�������״
	int type;		//����ķ���
	int block[4][4];//����4*4�����ͼ��
} BLOCKTYPE;


int CleanRow(void);//�������У��������������
void NextBlockInit(void);//��ʼ���¸�����
BOOL NewBlockEntry(void);//�·����볡
void ClientInit(int *score);//������Ϸ��
BOOL CanToNextPos(int iStyle);//�ܷ��ƶ����¸�λ��
BOOL GameOver(HWND hwnd, BOOL *bGameBegin);
void PaintNowPos(HWND hwnd, RECT rect, int iBlockSize, BOOL bIsBlack);//����/�����ǰλ��ͼ��
void PaintNextBlock(HWND hwnd, RECT rect, int iBlockSize);//�����¸�����
void PaintClient(HWND hwnd, RECT rect, int iBlockSize, int score, HFONT hfont);//����������Ϸ��










#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif
