
// ParseCommandDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "ParseCommand.h"
#include "ParseCommandDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CParseCommandDlg 对话框



CParseCommandDlg::CParseCommandDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PARSECOMMAND_DIALOG, pParent)
	, m_strFilePath(_T(""))
	, m_strCommandPacket(_T(""))
	, m_strDebugInfo(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//strMessage = _T("");
}

void CParseCommandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ADC_FILE, m_strFilePath);
	DDX_Text(pDX, IDC_EDIT_COMMAND_PACKET, m_strCommandPacket);
	DDX_Text(pDX, IDC_EDIT_DEBUG_INFO, m_strDebugInfo);
	DDX_Control(pDX, IDC_EDIT_DEBUG_INFO, m_EditDebugInfo);
	DDX_Control(pDX, IDC_EDIT_COMMAND_PACKET, m_EditCommandParse);
}

BEGIN_MESSAGE_MAP(CParseCommandDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CParseCommandDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CParseCommandDlg::OnBnClickedOk)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CParseCommandDlg 消息处理程序

BOOL CParseCommandDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CParseCommandDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CParseCommandDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CParseCommandDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CParseCommandDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CParseCommandDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


 

void CParseCommandDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	DragQueryFile(hDropInfo, 0, m_strFilePath.GetBuffer(MAX_PATH), MAX_PATH);
	m_strFilePath.ReleaseBuffer();
	UpdateData(FALSE);

	DoWork();

	CDialogEx::OnDropFiles(hDropInfo);
}







////////////////// 数据结构定义 ////////////////////
#define ADC_DAT_NUM  10000 
#define ADC_DAT_BUF_LEN  ADC_DAT_NUM*4

typedef struct tagADC_DATA_BUF
{
	int i;							// 当前读取的数据索引 
	int num;						// 数据总数
	int ADC_Data[ADC_DAT_BUF_LEN];	// ADC 数据
}ADC_DATA_BUF;


typedef struct tagADC_DATA_PROCESS
{
	int adc_dat[8];
	int field_0x20;
	int index1_0x24;
	int dat[32];
	int field_0xA8;
	int field_0xAC;
	int index3_0xB0;
	int index2_0xB4;
	int counter_0xB8;
	int counter_0xBC;
	int counter_0xC0;
	int field_0xC4;
	int field_0xC8;
	int field_0xCC;
	int field_0xD0;
	int field_0xD4;
	int field_0xD8;
	int counter_0xDC;
	int field_0xE0;
	int field_0xE4;
	int field_0xE8;
	int field_0xEC;
	int field_0xF0;
	int field_0xF4;
	int counter_0xF8;
	int field_0xFC;
	int field_0x100;
	int field_0x104;
	int field_0x108;
	int field_0x10C;
	int counter_0x110;
	int field_0x114;
	int field_0x118;
	int field_0x11C;
	int field_0x120;
	int field_0x124;
	int field_0x128;
	int field_0x12C;
	int field_0x130;
	int pre_counter_0x134;
	int field_0x138;

	int process_id;
} ADC_DATA_PROCESS;


typedef struct tagBIT_DATA
{
	int j;
	int d8[8];
}BIT_DATA;


typedef struct tagBYTE_FORM
{
	//int control_0x14;
	int in_byte_0x10;
	int received_num_0x1A;
	int counter2_0xC;
	int byte_bit_len_0x1C;
}BYTE_FORM;

////////////////// 全局变量定义 ////////////////////
CString strMessage;
CParseCommandDlg* pMainDlg;
int rom_0x15964[] = {	0x0,		0xC80,		0x640,
						0x0,		0x3200,		0x1900,
						0x800,		0x2000000,	0x10,
						0x960010,	0x1A0000,	0x0,
						0x0,		0x0,		0xC		};

int base;
ADC_DATA_BUF AdcDataBuf;
ADC_DATA_PROCESS AdcDataProcess0, AdcDataProcess1;
BIT_DATA BitData;
BYTE_FORM ByteForm;
//命令包最大长度29字节
unsigned char CommandPackage[32];

// ======== debug ========
int __Debug_Counter = 0;
CFile DebugFile;
 
 




int __DebugWrite(int val)
{
	char str[12] = { 0 };
	sprintf_s(str, "%08X", val);
	strcat_s(str, "  ");
	//_itoa_s(val, str, 16);
	DebugFile.Write(str, (UINT)strlen(str));

	return 0;
}

int __DebugWriteNewLine()
{
	char str[4] = { 0 };
	str[0] = 0x0D;
	str[1] = 0x0A;
	DebugFile.Write(str, 2);

	return 0;
}


UINT workerthread(LPVOID pParam);
int CParseCommandDlg::DoWork()
{
	if (DebugFile.Open(_T("D:\\debug.txt"), CFile::modeCreate | CFile::modeWrite) == NULL)
	{
		AfxMessageBox(_T("调试文件创建失败！"));
		return -1;
	}
	//运行线程
	CWinThread* g_pThread = AfxBeginThread(workerthread, this);

	return 0;
}



int InitBitData()
{
	memset(&BitData, 0x00, sizeof(BitData));

	return 0;
}


int InitByteForm()
{
	memset(&ByteForm, 0x00, sizeof(ByteForm));
	ByteForm.byte_bit_len_0x1C = 11;

	return 0;
}



int sub_3D2C(int base, int adc_val)
{
	int diff = 0;
	diff = (adc_val >> 16) - base;
	if ((diff + 0x4000) < 0)
	{
		return diff + 0x8000;
	}
	else
	{
		if (diff > 0x4000)
		{
			return diff - 0x8000;
		}
		else
		{
			return diff;
		}
	}
}




int sub_4144(int index, int num, int val)
{
	//ROM:00004144 - OK
	if (num < 1)
	{
		return val;
	}

	//loc_4148 - OK
	int temp = (0x26 * num + 0x40) / 128;
	int iElement1 = rom_0x15964[index * 3 + 1];
	if (iElement1 < temp)
	{
		return iElement1;
	}

	//loc_4160  - ?
	int iElement2 = rom_0x15964[index * 3 + 2];
	if (temp < iElement2)
	{
		return iElement2;
	}
	else
	{
		return temp;
	}

}



//  产生1bit有效数据
int sub_7A60(ADC_DATA_PROCESS* pADP, short adc_val)
{
	int i = 0, j = 0, k = 0;
	int temp = 0;
	int ret = 0;

	//////////////////////////////////////////////////////
	__Debug_Counter++;
	__DebugWrite(__Debug_Counter);
	if (__Debug_Counter == 0x30A)
	{
		int stop = 1;
	}

	__DebugWrite(adc_val);
	__DebugWriteNewLine();
	//////////////////////////////////////////////////////

	i = pADP->index1_0x24;
	// 更新field_0x20
	pADP->field_0x20 = pADP->field_0x20 + adc_val - pADP->adc_dat[i];
	//////////////////////////////////////////////////////
	//__DebugWrite(pADP->field_0x20);
	//__DebugWriteNewLine();
	//////////////////////////////////////////////////////
	// 保存ADC的值
	pADP->adc_dat[i] = adc_val;

	// 更新index1
	pADP->index1_0x24 = (pADP->index1_0x24 + 1) % 8;

	j = pADP->index2_0xB4;
	k = pADP->index3_0xB0;
	// 更新field_0xA8
	pADP->field_0xA8 = pADP->field_0xA8 + pADP->field_0x20 + pADP->dat[k] - (pADP->dat[j] << 1 );
	/////////////////////////////////////////////////////
	//__DebugWrite(pADP->field_0xA8);
	//__DebugWriteNewLine();
	/////////////////////////////////////////////////////
	// 更新index3处的值
	pADP->dat[k] = pADP->field_0x20;

	// 更新index2 和 index3
	pADP->index2_0xB4 = (pADP->index2_0xB4 + 1) % 32;
	pADP->index3_0xB0 = (pADP->index3_0xB0 + 1) % 32;

	// loc_7AE0  前40个数据用于初始化, 之后的数据才用于产生位
	if (pADP->pre_counter_0x134 != 0)
	{
		pADP->pre_counter_0x134--;
		return -1;
	}
	//AfxEndThread(0);
	///////////////////////////////////////////////
	//__DebugWrite(pADP->counter_0xB8);
	//__DebugWriteNewLine();
	///////////////////////////////////////////////
	// loc_7AEA  检测counter_0xB8是否有效
	if (pADP->counter_0xB8 > 63)
	{
		return -2;
	}

	//loc_7AF4:
	// counter_0xB8 加1
	pADP->counter_0xB8++;
	// counter_0xBC 加1 
	pADP->counter_0xBC++;
	// counter_0xC0 加1 
	pADP->counter_0xC0++; //0x12F0
	
	// ==== Above is OK ====

	if (1 == pADP->field_0xD8)	//ROM:00007B1E - OK
	{
		//loc_7B20 - OK
		if (pADP->field_0xC8 < pADP->field_0xA8)
		{
			//loc_7BAA - OK
			pADP->field_0xC8 = pADP->field_0xA8;
			pADP->field_0xC4 = pADP->counter_0xC0;
			pADP->counter_0xBC = 0;
			pADP->field_0x114 = pADP->counter_0x110;
			goto loc_7BBE;
		}
		else
		{
			//loc_7B36 - OK
			temp = pADP->field_0xC8 - pADP->field_0xA8;
			if (temp <= pADP->field_0xD4)
			{
				goto loc_7BBE;
			}
			//loc_7B42 - OK
			pADP->field_0xD8 = 0;
			pADP->counter_0xB8 = 0;
			// - OK
			//temp = pADP->field_0xD0;
			int num = pADP->field_0xC8 - pADP->field_0xCC;
			//if (pADP->field_0xC8 > pADP->field_0xCC)
			if(num > 0)
			{
				pADP->field_0xD0 = num;
			}
			
			//if (num <= 0)
			//{
			//	num = temp;
			//}
			pADP->field_0xD4 = sub_4144(pADP->process_id, pADP->field_0xD0, pADP->field_0xD4);
			///////////////////////////////////////////////
			//__DebugWrite(pADP->field_0xD4);
			//__DebugWriteNewLine();
			//////////////////////////////////////////////
			pADP->field_0xCC = 0x7FFFFFFF;
			// unwalked - (1) ?
			if (pADP->field_0x138 == 3)
			{
				//loc_7B7A 
				pADP->counter_0xDC++;
				pADP->field_0xE0 += pADP->field_0xC8;
				///////////////////////////////////////////////
				//__DebugWrite(pADP->counter_0xDC);
				//__DebugWrite(pADP->field_0xE0);
				//__DebugWriteNewLine();
				//////////////////////////////////////////////
			}


		}
	}
	else
	{
		// loc_7B90 - OK
		if (pADP->field_0xCC <= pADP->field_0xA8)
		{
			//loc_7C98 - OK
			temp = pADP->field_0xA8 - pADP->field_0xCC;
			if (temp <= pADP->field_0xD4)
			{
				goto loc_7BBE;
			}

			//loc_7CA4  - OK
			pADP->field_0xD8 = 1;
			pADP->counter_0xB8 = 0;
			//temp = pADP->field_0xD0;
			int num = pADP->field_0xC8 - pADP->field_0xCC;
			// - OK
			//if (pADP->field_0xC8 > pADP->field_0xCC)
			if(num > 0)
			{
				pADP->field_0xD0 = num;
			}
			// - OK
			
			//if (num <= 0)
			//{
			//	num = temp;
			//}
			pADP->field_0xD4 = sub_4144(pADP->process_id, pADP->field_0xD0, pADP->field_0xD4);
			///////////////////////////////////////////////
			//__DebugWrite(pADP->field_0xD4);
			//__DebugWriteNewLine();
			//////////////////////////////////////////////
			pADP->field_0xC8 = 0x80000000;
		}
		else
		{
			//loc_7BAA => OK 
			pADP->field_0xCC = pADP->field_0xA8;		// 0x12F3
			pADP->field_0xC4 = pADP->counter_0xBC;
			pADP->counter_0xC0 = 0;
			pADP->field_0x114 = pADP->counter_0x110;
			goto loc_7BBE;
		}
	}


	//loc_7CD2 - OK
	temp = pADP->field_0xEC;
	pADP->field_0xEC = pADP->field_0xC4;
	if ((pADP->field_0x138 & 0x4) == 0)
	{
		//loc_7CEC - OK
		int dif1 = abs(pADP->field_0xC4 - pADP->field_0xF0);
		int dif2 = abs(pADP->field_0xC4 - temp);
		if (dif2 > 5)
		{
			//loc_7E0A - OK
			if (pADP->counter_0xF8 <= 7)
			{
				//loc_7E12 - OK
				pADP->counter_0xF8 = 0;
				pADP->field_0xF4 = 0;
			}
		}
		else
		{
			// loc_7D02 - OK
			if (dif1 > 5)
			{
				//loc_7E0A - OK
				if (pADP->counter_0xF8 <= 7)
				{
					//loc_7E12 - OK
					pADP->counter_0xF8 = 0;
					pADP->field_0xF4 = 0;
				}
			}
			else
			{
				//loc_7D0A - OK 
				pADP->field_0xF4 += pADP->field_0xC4;
				temp = pADP->counter_0xF8;
				pADP->counter_0xF8++;
				if (temp >= 7)  // - OK
				{
					//loc_7D22 - OK
					if ((pADP->field_0x138 & 0x2) == 0)
					{
						pADP->field_0x138 |= 0x2;
						///////////////////////////////////////////////
						//__DebugWrite(pADP->field_0xE0);
						//__DebugWriteNewLine();
						///////////////////////////////////////////////
					}
					//loc_7D2A - OK
					pADP->field_0xF0 = ((pADP->counter_0xF8 >> 1) + pADP->field_0xF4) / pADP->counter_0xF8;
				}
				//loc_7D36 - OK
				pADP->field_0xFC = pADP->field_0xF0;
				pADP->field_0x100 = (pADP->field_0xF0 + 1) / 2;
				pADP->field_0x104 = ((pADP->field_0xF0 * 0xB3) + 0x40) / 128;
			}
		}

	}
	// 2023-01-31
	//loc_7D52 - OK
	if ((pADP->field_0x138 & 0x2) != 0)
	{
		//loc_7D5E - OK
		if (pADP->counter_0x110 != -1)
		{
			//loc_7D68 - OK
			temp = abs(pADP->field_0xC4 - pADP->field_0xF0);
			i = 0; //R6
			j = 0; //R10

			if (temp < 8)
			{
				i = 1;
			}
			if ((pADP->field_0x138 & 0x4) == 0x4)
			{
				//loc_7E1E - OK
				k = abs(pADP->field_0xC4 - pADP->field_0xFC);
				if (k < 5)
				{
					j = 1;
				}
				if (k < temp)
				{
					i = j; //R6=R10
				}

			}
			//loc_7D88 - OK
			if (pADP->field_0x114 > pADP->field_0x100)
			{
				temp = pADP->field_0xFC;
			}
			else
			{
				temp = 0;
			}
			// loc_7D9A - OK
			if (i != 0)
			{
				// loc_7DA0 - OK
				temp -= pADP->field_0x114;
				if (abs(temp) <= 7)
				{
					// loc_7DAE - OK
					pADP->counter_0x110 += temp;
				}

			}

		}
		else
		{
			//loc_7DF8  - OK
			temp = pADP->counter_0xBC;
			if (1 == pADP->field_0xD8)
			{
				// ======== 在提供的数据内,没有走到此处 ========
				// - ==== unwalked ====
				temp = pADP->counter_0xC0;
			}
			// - OK
			pADP->counter_0x110 = temp;
		}
	}

loc_7BBE:
	// loc_7BBE - OK
	if ((pADP->field_0x138 & 0x2) == 0)
	{
		return -1;
	}
	//loc_7BC8 - OK
	pADP->counter_0x110++;
	///////////////////////////////////////////////
	//__DebugWrite(pADP->counter_0x110);
	//__DebugWriteNewLine();
	//////////////////////////////////////////////
	if (pADP->counter_0x110 < pADP->field_0xFC)
	{
		return -1;
	}
	//loc_7BFA - OK
	pADP->counter_0x110 -= pADP->field_0xFC;
	pADP->field_0x120 = pADP->field_0x11C;
	pADP->field_0x11C = pADP->field_0x118;
	pADP->field_0x118 = pADP->field_0xA8;
	pADP->field_0x128 = (pADP->field_0x118 - pADP->field_0x120) << 1;
	pADP->field_0x124 = (pADP->field_0x118 + pADP->field_0x120) - (pADP->field_0x11C << 1);
	///////////////////////////////////////////////
	//__DebugWrite(pADP->counter_0x110);
	//__DebugWriteNewLine();
	//////////////////////////////////////////////
	if ((pADP->field_0x138 & 0x2) == 0)
	{
		return -1;
	}
	//loc_7C32 - OK
	if ((pADP->field_0x138 & 0x4) == 0x4)
	{
		//loc_7DC2 - OK
		if (0 == pADP->field_0x12C)
		{
			//loc_7DD8 - OK
			pADP->field_0x12C = 1;
			i = abs(pADP->field_0x124);
			j = abs(pADP->field_0x128);
			if (i > j)
			{
				// - OK
				return 1;
			}
			else
			{
				// - OK
				return 0;
			} // end

		}
		else
		{
			// loc_7DC8 
			// loc_7DCC  - OK
			if (pADP->field_0x12C <= 0)
			{
				// ======== 在提供的数据内，没有走到此处 ========
				//loc_7DF0 - ==== unwalked  ====
				pADP->field_0x12C = 1;
				return -1;
			}
			else
			{
				//loc_7DD2 - OK
				pADP->field_0x12C--;
				return -1;
			}//end

		}
	}
	else
	{
		//loc_7C3E - OK
		if (pADP->field_0x12C >= 2)
		{
			//loc_7DB8 - OK
			pADP->field_0x12C--;
			return -1; // end 
		}
		else
		{
			//loc_7C46 - OK
			if (0 == pADP->field_0x12C)
			{
				// - OK
				ret = 2;
			}
			else
			{
				// - OK
				ret = -1;
			}
			// - OK
			i = abs(pADP->field_0x128);
			j = abs(pADP->field_0x124);
			///////////////////////////////////////////////
			//__DebugWrite(i);
			//__DebugWrite(j);
			//__DebugWriteNewLine();
			//////////////////////////////////////////////
			if (i <= j)
			{
				// loc_7DCC (2) - OK
				if (pADP->field_0x12C <= 0)
				{
					//loc_7DF0 - OK
					pADP->field_0x12C = 1;
					return ret;
				}
				else
				{
					// loc_7DD2 - OK
					pADP->field_0x12C--;
					return ret;
				}
			}
			else
			{
				//loc_7C68 - OK
				j = (abs(pADP->field_0x128 + pADP->field_0x124) * 3 + 1) >> 1;
				if (i >= j)
				{
					// ======== 在目前提供的数据内，此路径没有走到 ========
					// loc_7DCC - ==== unwalked ====
					if (pADP->field_0x12C <= 0)
					{
						//loc_7DF0 - ==== unwalked ====
						pADP->field_0x12C = 1;
						return ret;
					}
					else
					{
						// loc_7DD2 - ==== unwalked ====
						pADP->field_0x12C--;
						return ret;
					}
				}
				else
				{
					//loc_7C80 - OK
					pADP->field_0x138 |= 0x4;
					pADP->field_0xF0 <<= 1;
					//loc_7DF0 - OK
					pADP->field_0x12C = 1;
					return 0;
				}
			}



		}
	}

}



int DisplayDebugInfo(CString str)
{
	pMainDlg->m_strDebugInfo = pMainDlg->m_strDebugInfo + str;
	pMainDlg->m_EditDebugInfo.SetWindowText(pMainDlg->m_strDebugInfo);
	return 0;
}

int DisplayParseResult(CString str)
{
	//CString strNewLine = _T("\r\n");
	pMainDlg->m_strCommandPacket = pMainDlg->m_strCommandPacket + str;
	pMainDlg->m_EditCommandParse.SetWindowText(pMainDlg->m_strCommandPacket);
	return 0;
}

//从文件中读取并转换ADC数据
int InitAdcData()
{
	//缓冲区清零
	memset(&AdcDataBuf, 0x00, sizeof(AdcDataBuf));
	//打开ADC文件
	HANDLE hFileInput = CreateFile(pMainDlg->m_strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFileInput)
	{
		strMessage.Format(_T("Fail to open file: %s \r\n"), pMainDlg->m_strFilePath);
		DisplayDebugInfo(strMessage);
		return -1;
	}
	else
	{
		strMessage.Format(_T("Success to open file: %s \r\n"), pMainDlg->m_strFilePath);
		DisplayDebugInfo(strMessage);
	}
	DWORD dwFileSize = GetFileSize(hFileInput, NULL);
	if (INVALID_FILE_SIZE == dwFileSize)
	{
		strMessage.Format(_T("Fail to get file size: %s \r\n"), pMainDlg->m_strFilePath);
		DisplayDebugInfo(strMessage);
		return -2;
	}
	else
	{
		strMessage.Format(_T("File size: %d Bytes\r\n"), dwFileSize);
		DisplayDebugInfo(strMessage);
	}
	// 创建文件映射对象
	HANDLE hFileMapInput = CreateFileMapping(hFileInput, NULL, PAGE_READONLY, 0, 0, NULL);
	if (hFileMapInput == NULL)
	{
		strMessage.Format(_T("Fail to Create File Mapping\r\n"), dwFileSize);
		DisplayDebugInfo(strMessage);
		return -3;
	}
	// 映射全部文件内容到进程虚拟地址空间
	char* pchSrc = (char*)MapViewOfFile(hFileMapInput, FILE_MAP_READ, 0, 0, 0);
	if (NULL == pchSrc)
	{
		strMessage.Format(_T("Fail to map file: %s \r\n"), pMainDlg->m_strFilePath);
		DisplayDebugInfo(strMessage);
		return -4;
	}
	char* pchEnd = pchSrc + dwFileSize - 1;
	char* pIndex = pchSrc;
	char szStr[16];
	int i = 0;
	do
	{
		//读取8个字符
		memset(szStr, 0x00, sizeof(szStr));
		memcpy(szStr, pIndex, 8);
		// 把字符转换为16进制DWORD值
		AdcDataBuf.ADC_Data[i++] = strtoul(szStr, NULL, 16);
		// 指向下一个字符串
		pIndex += 0xA;

	} while (pIndex < pchEnd);
	AdcDataBuf.num = i;
	strMessage.Format(_T("Read number: %d \r\n"), i);
	DisplayDebugInfo(strMessage);

	// ummap view
	UnmapViewOfFile(pchSrc);
	// close handle
	CloseHandle(hFileMapInput);
	CloseHandle(hFileInput);

	return 0;
}



//  初始化结构体的部分字段
int sub_408C(ADC_DATA_PROCESS* pADP)
{
	pADP->field_0x138 = 1;
	pADP->field_0xCC = 0x7FFFFFFF;
	pADP->field_0xC8 = 0x80000000;

	pADP->field_0xD0 = 0;
	pADP->field_0xC4 = 0;
	pADP->counter_0xC0 = 0;
	pADP->counter_0xBC = 0;
	pADP->counter_0xB8 = 0;

	pADP->field_0xE4 = NULL;

	pADP->field_0xD8 = 0;
	pADP->counter_0xDC = 0;
	pADP->field_0xE0 = 0;

	pADP->field_0xEC = 0xFFFFFFFA;

	pADP->field_0xF4 = 0;
	pADP->counter_0xF8 = 0;

	pADP->field_0xE8 = NULL;

	pADP->field_0xFC = 0;
	pADP->field_0x100 = 0;
	pADP->field_0x104 = 0;
	pADP->field_0x108 = 0;

	pADP->field_0x118 = 0;
	pADP->field_0x11C = 0;

	pADP->field_0x120 = 0;
	pADP->field_0x124 = 0;
	pADP->field_0x128 = 0;

	pADP->field_0x130 = NULL;
	pADP->field_0x10C = NULL;

	if (0 == pADP->process_id)
	{
		pADP->field_0xD4 = 0xC80;
	}
	else
	{
		pADP->field_0xD4 = 0x3200;
	}

	pADP->field_0xF0 = 0x10;
	pADP->counter_0x110 = 0xFFFFFFFF;
	pADP->field_0x114 = 0xFFFFFFFF;

	pADP->field_0x12C = 3;

	return 0;
}



int sub_3F60(ADC_DATA_PROCESS* pADP)
{
	sub_408C(pADP);
	//ByteForm.control_0x14 = 1;
	ByteForm.in_byte_0x10 = 0;
	ByteForm.received_num_0x1A = 0;
	//ByteForm.byte_bit_len_0x1C = 11;
	//ByteForm.packet_len_0x1E = 29 * 11;

	return 0;
}



char ReadByte()
{
	int i;
	int ret;
	int count = 0;
	unsigned char recv_byte = 0;
	ADC_DATA_PROCESS* pAdcDataProcess;


	do {
		if (0 == BitData.j)
		{
			////
			//Debug_Counter++;
			//if (Debug_Counter == 11)
			//{
			//	int stop = 0xFF;
			//}
			//从缓冲区读取第0、2、4、6 个数据。
			i = AdcDataBuf.i;
			memset(BitData.d8,0x00,sizeof(BitData.d8));
			BitData.d8[0] = AdcDataBuf.ADC_Data[i];
			BitData.d8[1] = AdcDataBuf.ADC_Data[i + 2];
			BitData.d8[2] = AdcDataBuf.ADC_Data[i + 4];
			BitData.d8[3] = AdcDataBuf.ADC_Data[i + 6];
			// 下一次读取的数据的起始地址
			AdcDataBuf.i = (i + 8) % (ADC_DAT_BUF_LEN);
			// 使用读取的4个数的高16位计算调整值
			BitData.d8[4] = sub_3D2C(base, BitData.d8[0]);
			BitData.d8[5] = sub_3D2C(base, BitData.d8[1]);
			BitData.d8[6] = sub_3D2C(base, BitData.d8[2]);
			BitData.d8[7] = sub_3D2C(base, BitData.d8[3]);
			BitData.j = 0;  //从第0个数据开始处理
		}
		for (int k = BitData.j; k <= 7; k++)
		{
			if ((k >= 0) && (k <= 3))
			{
				pAdcDataProcess = &AdcDataProcess0;
			}
			else
			{
				pAdcDataProcess = &AdcDataProcess1;
			}
			// 当前待处理的数据索引
			i = BitData.j;
			//指向下一个待处理的数据
			BitData.j = (i + 1) % 8; 
			//处理当前数据
			ret = sub_7A60(pAdcDataProcess, BitData.d8[i]);

			//处理返回值 -2 
			if (-2 == ret)
			{
				sub_3F60(pAdcDataProcess);
				continue;
			}
			//处理返回值-1
			if (-1 == ret)
			{
				continue;
			}
			//处理返回值2
			if (2 == ret)
			{
				ByteForm.counter2_0xC++;
				continue;
			}
			// ======== 此时返回值只有0或1 ========
			// 检测是否出错
			//if (ByteForm.received_num_0x1A >= ByteForm.packet_len_0x1E)
			//{
			//    printf("error occurred.");
			//    continue;
			//}
			//检测返回值2的次数
			if (ByteForm.counter2_0xC < 5)
			{
				sub_3F60(pAdcDataProcess);
				continue;
			}
			//处理返回值0和1
			//ByteForm.control_0x14 |= 0x02;
			ByteForm.received_num_0x1A++;
			if (1 == ret)
			{
				//当前位的值是1
				ByteForm.in_byte_0x10 >>= 1;
				ByteForm.in_byte_0x10 |= 0x400;
			}
			else
			{
				//当前位的值是0
				ByteForm.in_byte_0x10 >>= 1;
			}
			//检测是否接收完1个完整字节
			if (ByteForm.received_num_0x1A != ByteForm.byte_bit_len_0x1C)
			{
				continue;
			}
			//此处已接收1个完整字节,检测接收到的字节的有效性
			int x1, x2, x3, x4;
			unsigned temp;
			x1 = ByteForm.in_byte_0x10 & 0x7FFFFBFE;
			temp = x1;
			x2 = (temp >> 1) & 0x15555555;
			x1 -= x2;
			temp = x1;
			x2 = (temp >> 2) & 0x33333333;
			x1 &= 0x33333333; // ROM:00003DE0
			x1 += x2;
			temp = x1;
			x1 += temp >> 4;
			x1 &= 0x010F0F0F;
			x1 *= 0x01010101;
			temp = x1;
			x3 = (temp >> 23) & 0x2;
			x1 = ByteForm.in_byte_0x10 & 0x01;
			x4 = (ByteForm.in_byte_0x10 >> 8) & 0x04;
			x1 += x4;
			x1 += x3;
			x1 ^= 0x6;
			if (0 != x1)
			{
				sub_3F60(pAdcDataProcess);
				continue;
			}
			//有效,返回字节值
			recv_byte = (ByteForm.in_byte_0x10 >> 1) & 0xFF;
			ByteForm.in_byte_0x10 = 0;
			ByteForm.received_num_0x1A = 0;
			return recv_byte;
		}
	} while (AdcDataBuf.i < AdcDataBuf.num);

	//数据处理完毕,打印提示信息,退出线程
	strMessage.Format(_T("数据处理完毕！\r\n"), i);
	DisplayDebugInfo(strMessage);
	AfxEndThread(0);

	return 0x0;
}


char ReadCommandPackageHeader()
{
	return ReadByte();
}



char ReadCommandPackageData()
{
	return ReadByte();
}



// 计算校验和
char sub_72E4(unsigned char* buf, int len)
{
	char ret = 0;

	for (int i = 0; i < len; i++)
	{
		ret ^= buf[i];
	}
	return ret;
}



// ret: 1 - sucess, 0 - failure
int ReadCommandPackage()
{
	unsigned char result;
	int ComPkgDatLen = 0;
	memset(CommandPackage, 0x00, sizeof(CommandPackage));
	ByteForm.byte_bit_len_0x1C = 11;
	//读取包的标头字节
	CommandPackage[0] = ReadCommandPackageHeader();
	strMessage.Format(_T("Command Package Header: %02X \r\n"), CommandPackage[0]);
	DisplayParseResult(strMessage);


	//计算命令包的总字节数
	if (CommandPackage[0] < 0x20)
	{
		ComPkgDatLen = 2; //命令数据+校验字节
	}
	else if ((CommandPackage[0] >= 0x20) && (CommandPackage[0] < 0x80))
	{
		ComPkgDatLen = ((CommandPackage[0] - 0x20) / 16) + 3;
	}
	else if ((CommandPackage[0] >= 0x80) && (CommandPackage[0] <= 0xDF))
	{
		ComPkgDatLen = ((CommandPackage[0] - 0x80) / 8) + 9;
	}
	else if ((CommandPackage[0] > 0xDF) && (CommandPackage[0] < 0xFF))
	{
		ComPkgDatLen = ((CommandPackage[0] - 0xE0) / 4) + 0x15;
	}
	else
	{
		ComPkgDatLen = 0;
	}
	strMessage.Format(_T("Command Package Length: %d Bytes. \r\n"), ComPkgDatLen);
	DisplayParseResult(strMessage);


	//接收剩余数据
	strMessage.Format(_T("Command Package Data: "));
	DisplayParseResult(strMessage);
	for (int i = 1; i <= ComPkgDatLen; i++)
	{
		CommandPackage[i] = ReadCommandPackageData();
		strMessage.Format(_T("%02X "), CommandPackage[i]);
		DisplayParseResult(strMessage);
	}
	strMessage.Format(_T("\r\n 命令包接收完毕,"));
	DisplayParseResult(strMessage);

	//校验包
	result = sub_72E4(CommandPackage, ComPkgDatLen);
	if (result == CommandPackage[ComPkgDatLen])
	{
		strMessage.Format(_T("校验OK.\r\n"));
		DisplayParseResult(strMessage);
		return 1;
	}
	else
	{
		strMessage.Format(_T("校验Failure.\r\n"));
		DisplayParseResult(strMessage);
		return 0;
	}

}




//   初始化结构体ADC_DATA_PROCESS的全部字段
void sub_79F4()
{

	memset(&AdcDataProcess0, 0, sizeof(ADC_DATA_PROCESS));
	AdcDataProcess0.process_id = 0;
	AdcDataProcess0.index1_0x24 = 0;
	AdcDataProcess0.field_0x20 = 0;
	AdcDataProcess0.index3_0xB0 = 0;
	AdcDataProcess0.field_0xAC = 0;
	AdcDataProcess0.field_0xA8 = 0;
	AdcDataProcess0.index2_0xB4 = 0x10;
	sub_408C(&AdcDataProcess0);
	AdcDataProcess0.pre_counter_0x134 = 0x28;


	memset(&AdcDataProcess1, 0, sizeof(ADC_DATA_PROCESS));
	AdcDataProcess1.process_id = 1;
	sub_408C(&AdcDataProcess1);
	AdcDataProcess1.index1_0x24 = 0;
	AdcDataProcess1.field_0x20 = 0;
	AdcDataProcess1.index3_0xB0 = 0;
	AdcDataProcess1.field_0xAC = 0;
	AdcDataProcess1.field_0xA8 = 0;
	AdcDataProcess1.index2_0xB4 = 0x10;
	AdcDataProcess1.pre_counter_0x134 = 0x28;

}





UINT workerthread(LPVOID pParam)
{
	////////////////////////////////////////////
	//int num = 0x686e;
	//int temp = (0x26 * num + 0x40);
	//int r1 = temp / 128;
	//int r2 = temp >> 7;
	////////////////////////////////////////////

	pMainDlg = (CParseCommandDlg*)pParam;


	//从文件读取ADC数据
	InitAdcData();
	//初始化位处理结构体
	sub_79F4();
	//初始化BitData
	InitBitData();
	//初始化ByteForm
	InitByteForm();
	//计算base
	base = sub_3D2C(0, AdcDataBuf.ADC_Data[6]);
	while (AdcDataBuf.i < AdcDataBuf.num)
	{
		if (ReadCommandPackage())
		{
			
		}

	}
	return 0;
}
