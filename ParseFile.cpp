/*******************************************************************************
 文件名称 :	ParseFile.cpp 实现文件
 作    者 :  liutian
 创建时间 :  2011-1-20  13:50:55
 文件描述 :	文本解析处理类实现文件
  修改历史 :	2011-1-20	1.00	初始版本
 *******************************************************************************/

#include "EasyLog.h"
#include "ParseFile.h"
#include "HTML.h"
#include "RenderEngine.h"
#include "StrUtility.h"

#include <iostream>

//+++++++++++++++++++++++++HTML解析树节点定义+++++++++++++++++++++++++++++++++++++++++++++++

/********************************************************************
 函数名称  :	GetEntityChar
 函数描述  : 根据Html转义字符获取实体字符
 输入参数  : key，html转义字符
 输入参数  : keyLen，html转义字符长度
 输出参数  : N/A
 返回值    : 返回实体字符的Unicode码
 备注      :	N/A
 *********************************************************************/
unsigned short GetEntityChar(const char* key, int keyLen)
{
    // 2个一组，分别为偏移量，字符串个数，以关键字字符串长度排序，长度从0开始
    const static int keyIndex[] =
    {
        0, 0,           // 长度为0的关键字的偏移量，个数
        0, 0,           // 长度为1的关键字的偏移量，个数
        0, 11,          // 长度为2的关键字的偏移量，个数
        33, 28,         // 长度为3的关键字的偏移量，个数
        145, 44,        // 长度为4的关键字的偏移量，个数
        365, 52,        // 长度为5的关键字的偏移量，个数
        677, 43,        // 长度为6的关键字的偏移量，个数
        978, 3,         // 长度为7的关键字的偏移量，个数
        1002, 1         // 长度为8的关键字的偏移量，个数
    };
	
    // HTML实体转义字符
    const static int keyCount = sizeof(keyIndex) / sizeof(int) >> 1;
	
    // HTML实体转义字符定义表
    const static unsigned short keyCharList[] = { 'g', 'e', 8805, 'g',
        't', 62, 'l', 'e', 8804, 'l', 't', 60, 'm', 'u', 956, 'n', 'e',
        8800, 'n', 'i', 8715, 'n', 'u', 925, 'o', 'r', 8744, 'p', 'i', 960,
        'x', 'i', 958, 'a', 'm', 'p', 38, 'a', 'n', 'd', 8743, 'a', 'n',
        'g', 8736, 'c', 'a', 'p', 8745, 'c', 'h', 'i', 967, 'c', 'u', 'p',
        8746, 'd', 'e', 'g', 176, 'e', 't', 'a', 951, 'e', 't', 'h', 240,
        'i', 'n', 't', 8747, 'l', 'o', 'z', 9674, 'l', 'r', 'm', 8206, 'n',
        'o', 't', 172, 'p', 'h', 'i', 966, 'p', 'i', 'v', 982, 'p', 's',
        'i', 968, 'r', 'e', 'g', 174, 'r', 'h', 'o', 961, 'r', 'l', 'm',
        8207, 's', 'h', 'y', 173, 's', 'i', 'm', 8764, 's', 'u', 'b', 8834,
        's', 'u', 'm', 8721, 's', 'u', 'p', 8835, 't', 'a', 'u', 964, 'u',
        'm', 'l', 168, 'y', 'e', 'n', 165, 'z', 'w', 'j', 8205, 'a', 'p',
        'o', 's', 39, 'a', 'u', 'm', 'l', 228, 'b', 'e', 't', 'a', 946,
        'b', 'u', 'l', 'l', 8226, 'c', 'e', 'n', 't', 162, 'c', 'i', 'r',
        'c', 710, 'c', 'o', 'n', 'g', 8773, 'c', 'o', 'p', 'y', 169, 'd',
        'a', 'r', 'r', 8595, 'e', 'm', 's', 'p', 8195, 'e', 'n', 's', 'p',
        8194, 'e', 'u', 'm', 'l', 235, 'e', 'u', 'r', 'o', 8364, 'f', 'n',
        'o', 'f', 402, 'h', 'a', 'r', 'r', 8596, 'i', 'o', 't', 'a', 953,
        'i', 's', 'i', 'n', 8712, 'i', 'u', 'm', 'l', 239, 'l', 'a', 'r',
        'r', 8592, 'm', 'a', 'c', 'r', 175, 'n', 'b', 's', 'p', 160, 'n',
        's', 'u', 'b', 8836, 'o', 'r', 'd', 'f', 170, 'o', 'r', 'd', 'm',
        186, 'o', 'u', 'm', 'l', 246, 'p', 'a', 'r', 'a', 182, 'p', 'a',
        'r', 't', 8706, 'p', 'e', 'r', 'p', 8869, 'p', 'r', 'o', 'd', 8719,
        'p', 'r', 'o', 'p', 8733, 'q', 'u', 'o', 't', 34, 'r', 'a', 'r',
        'r', 8594, 's', 'd', 'o', 't', 8901, 's', 'e', 'c', 't', 167, 's',
        'u', 'b', 'e', 8838, 's', 'u', 'p', '1', 185, 's', 'u', 'p', '2',
        178, 's', 'u', 'p', '3', 179, 's', 'u', 'p', 'e', 8839, 'u', 'a',
        'r', 'r', 8593, 'u', 'u', 'm', 'l', 252, 'y', 'u', 'm', 'l', 376,
        'z', 'e', 't', 'a', 950, 'z', 'w', 'n', 'j', 8204, 'a', 'c', 'i',
        'r', 'c', 226, 'a', 'c', 'u', 't', 'e', 180, 'a', 'e', 'l', 'i',
        'g', 230, 'a', 'l', 'p', 'h', 'a', 945, 'a', 'r', 'i', 'n', 'g',
        229, 'a', 's', 'y', 'm', 'p', 8776, 'b', 'd', 'q', 'u', 'o', 8222,
        'c', 'e', 'd', 'i', 'l', 184, 'c', 'l', 'u', 'b', 's', 9827, 'c',
        'r', 'a', 'r', 'r', 8629, 'd', 'e', 'l', 't', 'a', 948, 'd', 'i',
        'a', 'm', 's', 9830, 'e', 'c', 'i', 'r', 'c', 234, 'e', 'm', 'p',
        't', 'y', 8709, 'e', 'q', 'u', 'i', 'v', 8801, 'g', 'a', 'm', 'm',
        'a', 947, 'i', 'c', 'i', 'r', 'c', 238, 'i', 'e', 'x', 'c', 'l',
        161, 'i', 'n', 'f', 'i', 'n', 8734, 'k', 'a', 'p', 'p', 'a', 954,
        'l', 'a', 'q', 'u', 'o', 171, 'l', 'c', 'e', 'i', 'l', 8968, 'l',
        'd', 'q', 'u', 'o', 8220, 'l', 's', 'q', 'u', 'o', 8216, 'm', 'd',
        'a', 's', 'h', 8212, 'm', 'i', 'c', 'r', 'o', 181, 'm', 'i', 'n',
        'u', 's', 8722, 'n', 'a', 'b', 'l', 'a', 8711, 'n', 'd', 'a', 's',
        'h', 8211, 'n', 'o', 't', 'i', 'n', 8713, 'o', 'c', 'i', 'r', 'c',
        244, 'o', 'e', 'l', 'i', 'g', 339, 'o', 'l', 'i', 'n', 'e', 8254,
        'o', 'm', 'e', 'g', 'a', 969, 'o', 'p', 'l', 'u', 's', 8853, 'p',
        'o', 'u', 'n', 'd', 163, 'p', 'r', 'i', 'm', 'e', 8243, 'r', 'a',
        'd', 'i', 'c', 8730, 'r', 'a', 'q', 'u', 'o', 187, 'r', 'c', 'e',
        'i', 'l', 8969, 'r', 'd', 'q', 'u', 'o', 8221, 'r', 's', 'q', 'u',
        'o', 8217, 's', 'b', 'q', 'u', 'o', 8218, 's', 'i', 'g', 'm', 'a',
        963, 's', 'z', 'l', 'i', 'g', 223, 't', 'h', 'e', 't', 'a', 952,
        't', 'h', 'o', 'r', 'n', 254, 't', 'i', 'l', 'd', 'e', 732, 't',
        'i', 'm', 'e', 's', 215, 't', 'r', 'a', 'd', 'e', 8482, 'u', 'c',
        'i', 'r', 'c', 251, 'u', 'p', 's', 'i', 'h', 978, 'a', 'a', 'c',
        'u', 't', 'e', 225, 'a', 'g', 'r', 'a', 'v', 'e', 224, 'a', 't',
        'i', 'l', 'd', 'e', 227, 'b', 'r', 'v', 'b', 'a', 'r', 166, 'c',
        'c', 'e', 'd', 'i', 'l', 231, 'c', 'u', 'r', 'r', 'e', 'n', 164,
        'd', 'a', 'g', 'g', 'e', 'r', 8225, 'd', 'i', 'v', 'i', 'd', 'e',
        247, 'e', 'a', 'c', 'u', 't', 'e', 233, 'e', 'g', 'r', 'a', 'v',
        'e', 232, 'e', 'x', 'i', 's', 't', 's', 8707, 'f', 'o', 'r', 'a',
        'l', 'l', 8704, 'f', 'r', 'a', 'c', '1', '2', 189, 'f', 'r', 'a',
        'c', '1', '4', 188, 'f', 'r', 'a', 'c', '3', '4', 190, 'h', 'e',
        'a', 'r', 't', 's', 9829, 'h', 'e', 'l', 'l', 'i', 'p', 8230, 'i',
        'a', 'c', 'u', 't', 'e', 237, 'i', 'g', 'r', 'a', 'v', 'e', 236,
        'i', 'q', 'u', 'e', 's', 't', 191, 'l', 'a', 'm', 'b', 'd', 'a',
        923, 'l', 'f', 'l', 'o', 'o', 'r', 8970, 'l', 'o', 'w', 'a', 's',
        't', 8727, 'l', 's', 'a', 'q', 'u', 'o', 8249, 'm', 'i', 'd', 'd',
        'o', 't', 183, 'n', 't', 'i', 'l', 'd', 'e', 241, 'o', 'a', 'c',
        'u', 't', 'e', 243, 'o', 'g', 'r', 'a', 'v', 'e', 242, 'o', 's',
        'l', 'a', 's', 'h', 248, 'o', 't', 'i', 'l', 'd', 'e', 245, 'o',
        't', 'i', 'm', 'e', 's', 8855, 'p', 'e', 'r', 'm', 'i', 'l', 8240,
        'p', 'l', 'u', 's', 'm', 'n', 177, 'r', 'f', 'l', 'o', 'o', 'r',
        8971, 'r', 's', 'a', 'q', 'u', 'o', 8250, 's', 'c', 'a', 'r', 'o',
        'n', 353, 's', 'i', 'g', 'm', 'a', 'f', 962, 's', 'p', 'a', 'd',
        'e', 's', 9824, 't', 'h', 'e', 'r', 'e', '4', 8756, 't', 'h', 'i',
        'n', 's', 'p', 8201, 'u', 'a', 'c', 'u', 't', 'e', 250, 'u', 'g',
        'r', 'a', 'v', 'e', 249, 'y', 'a', 'c', 'u', 't', 'e', 253, 'e',
        'p', 's', 'i', 'l', 'o', 'n', 949, 'o', 'm', 'i', 'c', 'r', 'o',
        'n', 959, 'u', 'p', 's', 'i', 'l', 'o', 'n', 965, 't', 'h', 'e',
        't', 'a', 's', 'y', 'm', 977
    };
	
	
    const char* buff = key;
    int wordLen = keyLen;
    int p = 0;
	
    // 先根据字长分发，然后再使用二分法查找
    if (wordLen < keyCount) {
        // 获得指定长度的关键字的偏移和个数
        int keyPos = wordLen << 1;
        int offset = keyIndex[keyPos++];
        int count = keyIndex[keyPos++];
		
        // 计算停止位置和值位置
        int keyLast = p + wordLen;
        wordLen++;
		
        // 使用二分法进行查找
        int low = 0;
        int high = count - 1;
        while (low <= high) {
            int mid = (low + high) >> 1;
            int cp = offset + mid * wordLen;
            int p1 = p;
			
            // 判断字符串是否相等
            while (buff[p1] == keyCharList[cp]) {
                p1++;
                cp++;
				
                // 比较成功，返回映射的值
                if (p1 >= keyLast) {
                    return keyCharList[cp];
                }
            }
			
            // 按照二分法调整查找区间
            if (buff[p1] > keyCharList[cp]) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
    }
    return -1;
}

CParseFile* CParseFile::_instance = NULL;
CParseFile::CParseFile(void)
: m_pRootNode(NULL)
, m_pCurNode(NULL)
, m_pHtml(NULL)
, m_bStopLoad(FALSE)
{
}

CParseFile::~CParseFile(void)
{
	if (m_pRootNode)
	{
		DELETE_OBJ(m_pRootNode);
	}
    ClearCSSList();
}

CParseFile* CParseFile::GetInstance()
{
    if (NULL == _instance)
    {
        _instance = new CParseFile;
    }
    return _instance;
}

void CParseFile::ReleaseInstance()
{
    DELETE_OBJ(_instance);
}

/********************************************************************
 函数名称  :	Load
 函数描述  : 从文件加载HTML文件，将HTML构建成树结构
 输入参数  : pszValue --- 待加载的HTML文件名
 输出参数  : N/A
 返回值    : void
 备注      :	N/A
 *********************************************************************/
BOOL CParseFile::Load(const char* pszValue, void* pParse, BOOL isPath)
{
   
    FUN_TRACE();
	//如果是是文件路径，则需要读取文件，反之传入的是 文本内容，则需要去解析 文本
    if (isPath)
    {
        FILE* fp = fopen(pszValue, "rb");
        if (NULL == fp)
        {
            EPUB_TRACE(LOG_LEVEL_ERROR, "Open %s failed.", pszValue);
            return FALSE;
        }
		
        fseek(fp, 0L, SEEK_END);
        long lLength = ftell(fp);
        fseek(fp, 0L, SEEK_SET);
        char* pszText = (char*)calloc(lLength + 1, 1);
        if (NULL == pszText)
        {
            EPUB_TRACE(LOG_LEVEL_ERROR, "new char array failed.");
            fclose(fp);
            return FALSE;
        }
        int iRead = fread(pszText, sizeof(char), lLength, fp);
        if (iRead != lLength)
        {
            free(pszText);
            fclose(fp);
            EPUB_TRACE(LOG_LEVEL_ERROR, "Read file data failed.");
            return FALSE;
        }
        fclose(fp);
		
        BOOL bResult = LoadHtmlSource(pszText, pParse);
        free(pszText);
		
        return bResult;
    }
    else
    {
        return LoadHtmlSource(pszValue, pParse);
    }
}

/********************************************************************
 函数名称  :	LoadHtmlSource
 函数描述  : 直接加载文件内容
 输入参数  : pszSource --- 需要解析的内容
 输出参数  : N/A
 返回值    : void
 备注      :	N/A
 *********************************************************************/
BOOL CParseFile::LoadHtmlSource(const char* pszSource, void* pParse)
{
    FUN_TRACE();
    //先清空整个树
    DELETE_OBJ(m_pRootNode);
	
    BOOL bResult = FALSE;
    m_pHtml = (CHTML*)pParse;
	
    if (!CreateTree())
    {
        EPUB_TRACE(LOG_LEVEL_ERROR, "CreateTree failed.");
        return bResult;
    }
	
    int len = strlen(pszSource);
    bool hasBom = false;
    bool isUtf8 = IsDocUtf8(pszSource, len, hasBom);
	
    char * pszUtf8 = NULL;
    const char * pszTemp = NULL;
    if (!isUtf8)
    {
        pszUtf8 = (char *)calloc(len*4 + 1, 1);
        AnsiToUtf8_c(pszUtf8, pszSource);
        pszTemp = pszUtf8;
    }
    else
    {
        if (hasBom)
        {
            pszTemp = pszSource + 3;
        }
        else
        {
            pszTemp = pszSource;
        }
    }
	
    bResult = Parse(pszTemp);
	
    if (pszUtf8 != NULL)
    {
        free(pszUtf8);
    }
	
    return bResult;
}

/********************************************************************
 函数名称  :	AnalysisTag
 函数描述  : 分析标签
 输入参数  : pszTag         --- 待分析的标签，已经去除了'<'和'>'
 iLen           --- 待分析文本的长度
 输出参数  : strTagName     --- 返回标签名
 strAttribute   --- 返回标签属性字符串
 返回值    : E_EMPTY_TAG    --- 空标签<>
 E_COMMENT_TAG  --- 注释标签<!--xxx-->
 E_START_TAG    --- 开始标签<xxx a="" b="">
 E_END_TAG      --- 结束标签</xxx>
 E_SELF_END_TAG --- 自结束标签<xxx a="" b=""/>
 备注      :	次函数返回后，pszTag指向节点最后的'>'
 *********************************************************************/
int CParseFile::AnalysisTag(const char** pszTag, string& strTagName, ATTRIBUTE_LIST& vecAttribute)
{
    FUN_TRACE();
    const char *pszCursor = *pszTag;
    string strAttribute;
    int iResult = E_START_TAG;
#define TAG_NAME_LEN 20
#define ATTR_NAME_LEN 50
#define ATTR_VALUE_LEN 100
    char szName[TAG_NAME_LEN] = {0};
    char szAttrName[ATTR_NAME_LEN] = {0};
    char szAttrValue[ATTR_VALUE_LEN] = {0};
    int iIndex = 0;
    int iType = E_TAG_NAME;//当前解析的元素类型（分为标签名、标签属性名、标签属性值）
    int i1 = 0;//表示在节点的开始标签中是否有上双引号出现，1表示开始出现，当闭环时置为0
    int i2 = 0;//表示在节点的开始标签中是否有上单引号出现，1表示开始出现，当闭环时置为0
    int iAttrEnd = 0;
    NODEATTRIBUTE* pAttribute = 0;
    if (*pszCursor == '/')
    {
        iResult = E_END_TAG;
        ++pszCursor;//跳过'/'
    }
    else if (*pszCursor == '!' && *(pszCursor + 1) == '-' && *(pszCursor + 2) == '-')//注释
    {
        iResult = E_COMMENT_TAG;
        while (*pszCursor != 0)
        {
            if (*pszCursor == '-' && *(pszCursor + 1) == '-' && *(pszCursor + 2) == '>')
            {
                pszCursor += 2;//指向'>'
                break;
            }
            ++pszCursor;
        }
        *pszTag = pszCursor;
        return iResult;
    }
	
    while (*pszCursor != 0)
    {
        //如果是''或者""之间的部分，特殊字符失效
        if(*pszCursor=='\"')
        {
            ++pszCursor;
            i1 = !i1;
            iAttrEnd = !i1;
			continue;
        }
        else if(*pszCursor == '\'')
        {
            ++pszCursor;
            i2 = !i2;
            iAttrEnd = !i2;
			continue;
        }
		
        if (iAttrEnd)
        {
            switch (iType)
            {
				case E_ATTR_VAUE:
					if (iIndex != 0)
					{
						pAttribute->strName = szAttrName;
						pAttribute->strValue = szAttrValue;
						//vecAttribute.push_back(pAttribute);
						::memset(szAttrName,0,ATTR_NAME_LEN);
						::memset(szAttrValue,0,ATTR_VALUE_LEN);
						pAttribute = 0;
						iIndex = 0;
					}
					break;
				default:
				{
					break;
				}
            }
            iType = E_ATTR_DEF;
            iAttrEnd = 0;
        }
		
        if ((*pszCursor == '>' || (*pszCursor == '/' && *(pszCursor + 1) == '>') || *pszCursor <= ' ') && !i1 && !i2)
        {
            switch (iType)
            {
				case E_TAG_NAME:
					if (iIndex != 0)
					{
						strTagName = szName;
						::memset(szName,0,TAG_NAME_LEN);
						iIndex = 0;
					}
					break;
				default:
					break;
            }
            if (*pszCursor <= ' ')
            {
                iType = E_ATTR_NAME;
            }
            else if (*pszCursor != '>')//自结束的情况
            {
                iResult = E_SELF_END_TAG;
                ++pszCursor;
                break;
            }
            else
            {
                break;
            }
        }
        else
        {
            if (*pszCursor != '=')
            {
                switch (iType)
                {
					case E_TAG_NAME:
						if (*pszCursor > ' ')
						{
							szName[(iIndex = ((TAG_NAME_LEN == iIndex)?0:iIndex))++] = *pszCursor;
						}
						break;
					case E_ATTR_NAME:
						if (*pszCursor > ' ')
						{
							if (0 == iIndex)
							{
								pAttribute = new NODEATTRIBUTE;
								vecAttribute.push_back(pAttribute);
							}
							
							szAttrName[(iIndex = ((ATTR_NAME_LEN == iIndex)?0:iIndex))++] = *pszCursor;
						}
						break;
					case E_ATTR_VAUE:
						szAttrValue[(iIndex = ((ATTR_VALUE_LEN == iIndex)?0:iIndex))++] = *pszCursor;
						break;
					default:
						break;
                }
				
            }
            else if ( !i1 && !i2)
            {
                iIndex = 0;
                iType = E_ATTR_VAUE;
            }
        }
        ++pszCursor;
    }
    *pszTag = pszCursor;
    if (0 == strTagName.compare("script"))
    {
        iResult = E_SCRIPT_TAG;
    }
    else if (0 == strTagName.compare("style"))
    {
        iResult = E_STYLE_TAG;
    }
    return iResult;
}

/********************************************************************
 函数名称  :	ParseFile
 函数描述  : 解析文本
 输入参数  : pszDoc --- 待解析的文本
 输出参数  : N/A
 返回值    : 成功返回TRUE, 否则返回FALSE
 备注      :	N/A
 *********************************************************************/
BOOL CParseFile::Parse(const char* pszDoc)
{
    FUN_TRACE();
    EPUB_TRACE(LOG_LEVEL_CRITICAL, "Enter Parse ");
    m_bStopLoad = FALSE;
    BOOL bRet = FALSE;
    const char *pszText = pszDoc;
    const char *pszTagEnd = 0;//记录节点的结束位置
    BOOL bResult = TRUE;
    int iBegin = 0;//表示是否有节点开始
    int iPureText = 0;//是否出现scprit、style标签
    int i1 = 0;//表示在节点的开始标签中是否有上双引号出现，1表示开始出现，当闭环时置为0
    int i2 = 0;//表示在节点的开始标签中是否有上单引号出现，1表示开始出现，当闭环时置为0
	
    if (pszText == NULL)
    {
        return FALSE;
    }
	
    while (*pszText && !m_bStopLoad)
    {
        if(1 == iBegin)//已经遇到"<"或者"<!"
        {
            ATTRIBUTE_LIST vecAttribute;
            vecAttribute.clear();
            if(*pszText=='\"')
            {
                i1 = !i1;
            }
            else if(*pszText == '\'')
            {
                i2 = !i2;
            }
            // 处理两个标签之间的部分
            if (pszTagEnd + 1 != pszText && pszTagEnd != 0)
            {
                string strBetween = GetPureHtmlText(pszTagEnd , pszText - (pszTagEnd + 1));
                if (!(strBetween.empty() || 0 == strBetween.compare("")))
                {
                    // 以上一个结束字符和当前开始字符之间的内容作为一个node
                    bResult = AddNode(strBetween.c_str(), vecAttribute, E_TEXT_TAG);
                    if (bResult)
                    {
                        // 如果当前文本节点不是可忽略的节点，并且不是表格、列表或超链接中的节点，直接构造渲染节点节点
                        if (!m_pCurNode->m_bIsIgnore && !m_pCurNode->m_bIsNested)
                        {
                            m_pHtml->ConstructRenderNode(m_pCurNode, E_TEXT_TAG);
                        }
						
                        m_pCurNode = m_pCurNode->m_pParentNode;
                    }
                }
            }
			
            // 保存解析到的标签名称
            string strTagName;
            int iResult = AnalysisTag(&pszText, strTagName,vecAttribute);
            switch (iResult)
            {
				case E_COMMENT_TAG:// 注释,把两段文本分成两个节点
                {
                    break;
                }
				case E_EMPTY_TAG: // 认为是一个<>文本, 什么也不做，
                {
                    break;
                }
				case E_START_TAG: // 开始标签
                {
                    bResult = AddNode(strTagName.c_str(), vecAttribute, E_START_TAG);
                    if (bResult && !m_pCurNode->m_bIsIgnore)
                    {
                        if (IsBreakLine(m_pCurNode))
                        {
                            m_pHtml->m_bIsBreakLine = TRUE;
                        }
                    }
                    break;
                }
				case E_END_TAG: // 结束标签
                {
                    iPureText = 0;
                    if (NULL == m_pCurNode)
                    {
                        break;
                    }
					
                    // 如果当前节点不是可忽略的节点
                    if (!m_pCurNode->m_bIsIgnore)
                    {
                        // 如果遇到的不是list或table或超链接的结束标签则
                        // 开始构造列表、表格或超链接
                        if (!m_pCurNode->m_bIsNested
                            && (DL == m_pCurNode->m_iTagId
								|| OL == m_pCurNode->m_iTagId
								|| UL == m_pCurNode->m_iTagId
								|| TABLE == m_pCurNode->m_iTagId
								|| A == m_pCurNode->m_iTagId))
                        {
                            m_pHtml->ConstructRenderNode(m_pCurNode, E_END_TAG);
                        }
						
                        if (IsBreakLine(m_pCurNode))
                        {
                            m_pHtml->m_bIsBreakLine = TRUE;
                        }
                    }
					
                    // 将当前Node返回上面一层
                    m_pCurNode = m_pCurNode->m_pParentNode;
                    break;
                }
				case E_SELF_END_TAG: // 自结束的标签
                {
                    bResult = AddNode(strTagName.c_str(), vecAttribute, E_SELF_END_TAG);
                    if (bResult)
                    {
                        // 如果当前节点不是可忽略的节点
                        if (!m_pCurNode->m_bIsIgnore)
                        {
                            // 如果是图片节点并且不是在表格、列表和超链接中的图片，直接构造图片节点
                            if (!m_pCurNode->m_bIsNested && IMG == m_pCurNode->m_iTagId)
                            {
                                //绘制节点
                                m_pHtml->ConstructRenderNode(m_pCurNode, E_SELF_END_TAG);
                            }
							
                            if (IsBreakLine(m_pCurNode))
                            {
                                m_pHtml->m_bIsBreakLine = TRUE;
                            }
                        }
                        
                        m_pCurNode = m_pCurNode->m_pParentNode;
                    }
                    break;
                }
				case E_SCRIPT_TAG://出现script style之后必须遇到"</"才认为节点开始
				case E_STYLE_TAG:
                {
                    iPureText = !iPureText;
                    break;
                }
				default: // 未知情况
                {
                    bResult = FALSE;
                    break;
                }
            }
			
            if (!bResult)
            {
                EPUB_TRACE(LOG_LEVEL_ERROR, "Error occured while parsing HTML file\n");
                break;
            }
        }
		
        if(*pszText == '<' && *(pszText + 1) == '!' && !i1 && !i2)//Doctype开始
        {
            iBegin = !iPureText;
        }
        else if(*pszText == '<' && *(pszText + 1) != '/' && *(pszText + 1) != '!' && !i1 && !i2)
        {
            iBegin = !iPureText;
        }
        else if(*pszText == '>' && !i1 && !i2)
        {
            iBegin = 0;
            pszTagEnd = ((1 == iPureText)?0:(pszText + 1));
        }
        else if(*pszText == '/' && *(pszText + 1)=='>' && !i1 && !i2)
        {
            iBegin = 0;
            pszTagEnd = ((1 == iPureText)?0:(pszText + 1));
        }
        else if(*pszText == '<' && *(pszText + 1)=='/' && !i1 && !i2)
        {
            iBegin = 1;
        }
        ++pszText;
    }
	
    if (*pszText == '\0')
    {
        bRet = TRUE;
    }
    EPUB_TRACE(LOG_LEVEL_CRITICAL, "Leave Parse ");
    return bRet;
}

/********************************************************************
 函数名称  :	IsBreakLine
 函数描述  : 判断是否需要换行
 输入参数  : 要判断的节点
 输出参数  : void
 返回值    : void
 备注      :	N/A
 *********************************************************************/
BOOL CParseFile::IsBreakLine(CNode* pNode)
{
    if (P == pNode->m_iTagId
        || BODY == pNode->m_iTagId
        || DIV == pNode->m_iTagId
        || TABLE == pNode->m_iTagId
        || UL == pNode->m_iTagId
        || OL == pNode->m_iTagId
        || H1 == pNode->m_iTagId
        || H2 == pNode->m_iTagId
        || H3 == pNode->m_iTagId
        || H4 == pNode->m_iTagId
        || H5 == pNode->m_iTagId
        || H6 == pNode->m_iTagId
        || BR == pNode->m_iTagId
        || IMG == pNode->m_iTagId)
    {
        return TRUE;
    }
	
    return FALSE;
}

/********************************************************************
 函数名称  :	ParseCSS
 函数描述  : 解析CSS样式表
 输入参数  : strDoc --- HTML文本
 输出参数  : strDoc --- 去掉样式表后的HTML文本
 返回值    : void
 备注      :	N/A
 *********************************************************************/
void CParseFile::ParseCSS(string& strDoc)
{
    FUN_TRACE();
    string::size_type startPos = strDoc.find("<style");
    string::size_type endPos = strDoc.find("</style>");
	
    if (string::npos != startPos && string::npos != endPos)
    {
        string strStyle = strDoc.substr(startPos+6);
        string::size_type beginPos = strStyle.find_first_of('>');  //style标签的结束
        if (string::npos == beginPos)
        {
            return;
        }
        strStyle = strStyle.substr(beginPos+1);
        string::size_type iPos = strStyle.find("</style>");
        if (string::npos == iPos)
        {
            return;
        }
        strStyle = strStyle.substr(0, iPos);
        TrimTwoSideSpace(strStyle.c_str(),strStyle.length(),strStyle);
        ParseCSSList(strStyle); //将样式字符串解析成样式表结构
		
        //获取去掉样式表后的HTML源码
        string strHead = strDoc.substr(0, startPos);
        TrimTwoSideSpace(strHead.c_str(),strHead.length(),strHead);
        string strTail = strDoc.substr(endPos+8);
        TrimTwoSideSpace(strTail.c_str(),strTail.length(),strTail);
        strDoc = strHead + strTail;
    }
}

/********************************************************************
 函数名称  :	ParseCSSList
 函数描述  : 将<style>标签内的样式字符串解析成样式表结构
 输入参数  : strDoc --- <style>与</style>标签内的字符串
 输出参数  : void
 返回值    : void
 备注      :	N/A
 *********************************************************************/
void CParseFile::ParseCSSList(const string& strDoc)
{
    FUN_TRACE();
    //清空样式表
    ClearCSSList();
    string strStyle = strDoc;
    //string::size_type iOpenPos = strStyle.find_first_of('{');
    while (string::npos != strStyle.find_first_of('{'))
    {
        CSS_STYLE* pCssStyle = new CSS_STYLE;
        if (NULL != pCssStyle)
        {
            string::size_type iOpenPos = strStyle.find_first_of('{');
            string strSelectorName = strStyle.substr(0, iOpenPos);
            TrimTwoSideSpace(strSelectorName.c_str(),strSelectorName.length(),strSelectorName);
            pCssStyle->strSelector = strSelectorName;
            //printf("The SelectorName is %s\n", strSelectorName.c_str());
            strStyle = strStyle.substr(iOpenPos+1);
            string::size_type iClosePos = strStyle.find_first_of('}');
            string strAttribute = strStyle.substr(0, iClosePos);
            //解析样式属性表
            ParseCSSAttributeList(strAttribute, pCssStyle);
            m_vecCssStyle.push_back(pCssStyle);
			
            //获取还没有解析的字符串
            strStyle = strStyle.substr(iClosePos+1);
        }
    }
}

/********************************************************************
 函数名称  :	ParseCSSAttributeList
 函数描述  : 解析选择器的属性列表
 输入参数  : strDoc --- 属性字符串
 pStyle --- 样式表项指针
 输出参数  : void
 返回值    : void
 备注      :	N/A
 *********************************************************************/
void CParseFile::ParseCSSAttributeList(const string& strDoc, CSS_STYLE* pStyle)
{
    FUN_TRACE();
    if (NULL == pStyle)
    {
        return;
    }
	
    string strAttribute = strDoc;
    while (string::npos != strAttribute.find_first_of(':'))
    {
        CSS_ATTRIBUTE* pAttribute = new CSS_ATTRIBUTE;
        if (NULL != pAttribute)
        {
            string::size_type iColonPos = strAttribute.find_first_of(':');
            string strAttributeName = strAttribute.substr(0, iColonPos);
            TrimTwoSideSpace(strAttributeName.c_str(),strAttributeName.length(),strAttributeName);
            pAttribute->strName = strAttributeName;
            // printf("the CSS Attribute name is %s\n", strAttributeName.c_str());
            strAttribute = strAttribute.substr(iColonPos+1);
            string strAttributeValue;
            string::size_type iSemiPos = strAttribute.find_first_of(';');
            if (string::npos == iSemiPos)
            {
                strAttributeValue = strAttribute;
            }
            else
            {
                strAttributeValue = strAttribute.substr(0, iSemiPos);
                //获取还没有解析的属性串
                strAttribute = strAttribute.substr(iSemiPos+1);
            }
            TrimTwoSideSpace(strAttributeValue.c_str(),strAttributeValue.length(),strAttributeValue);
            pAttribute->strValue = strAttributeValue;
            //printf("the CSS Attribute value is %s\n", strAttributeValue.c_str());
            pStyle->vecAttribute.push_back(pAttribute);
        }
    }
}

/********************************************************************
 函数名称  :	CreateTree
 函数描述  : 创建树，初始树只有一个根节点
 输入参数  : N/A
 输出参数  : N/A
 返回值    : 成功返回TRUE, 否则返回FALSE
 备注      :
 *********************************************************************/
BOOL CParseFile::CreateTree()
{
    FUN_TRACE();
    BOOL bResult = TRUE;
    if (NULL == m_pRootNode)
    {
        m_pRootNode = new CNode;
        if (NULL != m_pRootNode)
        {
            m_pRootNode->m_strName = "root";
            m_pRootNode->m_pParentNode = NULL;
            m_pCurNode = m_pRootNode;
        }
        else
        {
            bResult = FALSE;
        }
    }
	
    return bResult;
}

/*********************************************************************
 函数名称  :	AddNormalNode
 函数描述  : 向html树中添加节点
 输入参数  : szName       --- 节点名称
 vecAttribute --- 节点的属性列表
 输出参数  : void
 返回值    : 成功返回TRUE，失败返回FALSE
 备注      :	N/A
 *********************************************************************/
BOOL CParseFile::AddNode(const char* szName, ATTRIBUTE_LIST& vecAttribute, int iTagType)
{
    FUN_TRACE();
    CNode* pNewNode = new CNode;
    if (NULL != pNewNode)
    {
        if (NULL == m_pCurNode)
        {
            return FALSE;
        }
        pNewNode->m_strName = szName;
        pNewNode->m_pParentNode = m_pCurNode;
        if (E_TEXT_TAG == iTagType)
        {
            pNewNode->m_iTagId = TEXT;
        }
        else if (E_START_TAG == iTagType || E_SELF_END_TAG == iTagType)
        {
            string strName = szName;
            pNewNode->m_iTagId = GetTagTypeByTagName(strName);
        }
		
        // 标志是否为可忽略的节点（head内的节点均为可忽略的节点）
        if (_HEAD == pNewNode->m_iTagId || m_pCurNode->m_bIsIgnore)
        {
            pNewNode->m_bIsIgnore = TRUE;
        }
        // 标志是否为表格、列表或超链接内的节点
        else if (LI == pNewNode->m_iTagId
				 || DT == pNewNode->m_iTagId
				 || DD == pNewNode->m_iTagId
				 || TR == pNewNode->m_iTagId
				 || CAPTION == pNewNode->m_iTagId
				 || TD == pNewNode->m_iTagId
				 || TH == pNewNode->m_iTagId
				 || A == m_pCurNode->m_iTagId
				 || m_pCurNode->m_bIsNested)
        {
            pNewNode->m_bIsNested = TRUE;
        }
        
        // 添加到当前节点的子节点列表中
        m_pCurNode->m_vecChildNode.push_back(pNewNode);
        if (!vecAttribute.empty())
        {
            pNewNode->m_vecAttribute = vecAttribute;
        }
		
        // 更新当前节点
        m_pCurNode = pNewNode;
    }
	
    return TRUE;
}

/********************************************************************
 函数名称  :	GetTagTypeByTagName  (支持47个标签，定义的68个标签 支持47个,11个不支持)
 函数描述  : 根据标签名称提取标签id
 输入参数  : strTagName    标签名称
 输出参数  : void
 返回值    : void
 备注      :	N/A
 *********************************************************************/
int CParseFile::GetTagTypeByTagName(string& strTagName)
{
    
   cout<< "GetTagTypeByTagName--strTagName==="<<strTagName <<endl;
    
    
	int iType = DEFAULT;
    if (0 == strTagName.compare(CONST_LABEL_P))
    {
       
        iType = P;
    }
    else if (0 == strTagName.compare(CONST_LABEL_DIV))//div
    {
        iType = DIV;
    }
    else if (0 == strTagName.compare(CONST_LABEL_BODY))
    {
        iType = BODY;
    }
    else if (0 == strTagName.compare(CONST_LABEL_HEAD))
    {
        iType = _HEAD;
    }
    else if (0 == strTagName.compare(CONST_LABEL_B))
    {
        iType = B;
    }
    else if (0 == strTagName.compare(CONST_LABEL_EM))//em
    {
        iType = EM;
    }
    else if (0 == strTagName.compare(CONST_LABEL_I))//i
    {
        iType = I;
    }
    else if (0 == strTagName.compare(CONST_LABEL_SPAN))
    {
        iType = SPAN;
    }
    else if (0 == strTagName.compare(CONST_LABEL_STRONG))
    {
        iType = STRONG;
    }
    else if (0 == strTagName.compare(CONST_LABEL_BIG))
    {
        iType = BIG;
    }
    else if (0 == strTagName.compare(CONST_LABEL_SMALL))
    {
        iType = SMALL;
    }
    else if (0 == strTagName.compare(CONST_LABEL_TT))
    {
        iType = TT;
    }
    else if (0 == strTagName.compare(CONST_LABEL_CENTER))
    {
        iType = CENTER;
    }
    else if (0 == strTagName.compare(CONST_LABEL_LABEL))
    {
        iType = LABEL;
    }
    else if (0 == strTagName.compare(CONST_LABEL_H1))//yes
    {
        iType = H1;
    }
    else if (0 == strTagName.compare(CONST_LABEL_H2))
    {
        iType = H2;
    }
    else if (0 == strTagName.compare(CONST_LABEL_H3))
    {
        iType = H3;
    }
    else if (0 == strTagName.compare(CONST_LABEL_H4))
    {
        iType = H4;
    }
    else if (0 == strTagName.compare(CONST_LABEL_H5))//h5
    {
        iType = H5;
    }
    else if (0 == strTagName.compare(CONST_LABEL_H6))
    {
        iType = H6;
    }
    else if (0 == strTagName.compare(CONST_LABEL_U))//u
    {
        iType = U;
    }
    else if (0 == strTagName.compare(CONST_LABEL_SUP))//yes
    {
        iType = SUP;
    }
    else if (0 == strTagName.compare(CONST_LABEL_SUB))
    {
        iType = SUB;
    }
    else if (0 == strTagName.compare(CONST_LABEL_STRIKE))
    {
        iType = STRIKE;
    }
    else if (0 == strTagName.compare(CONST_LABEL_FONT))
    {
        iType = FONT;
    }
    else if (0 == strTagName.compare(CONST_LABEL_BLINK))
    {
        iType = BLINK;
    }
    else if (0 == strTagName.compare(CONST_LABEL_ADDRESS))
    {
        iType = ADDRESS;
    }
    else if (0 == strTagName.compare(CONST_LABEL_CITE))//cite
    {
        iType = CITE;
    }
    else if (0 == strTagName.compare(CONST_LABEL_DFN))//dfn
    {
        iType = DFN;
    }
    else if (0 == strTagName.compare(CONST_LABEL_VAR))
    {
        iType = VAR;
    }
    else if (0 == strTagName.compare(CONST_LABEL_BR))//br
    {
        iType = BR;
    }
    else if (0 == strTagName.compare(CONST_LABEL_UL))//ul
    {
        iType = UL;
    }
    else if (0 == strTagName.compare(CONST_LABEL_OL))
    {
        iType = OL;
    }
    else if (0 == strTagName.compare(CONST_LABEL_DL))
    {
        iType = DL;
    }
    else if (0 == strTagName.compare(CONST_LABEL_LI))
    {
        iType = LI;
    }
    else if (0 == strTagName.compare(CONST_LABEL_DT))
    {
        iType = DT;
    }
    else if (0 == strTagName.compare(CONST_LABEL_DD))
    {
        iType = DD;
    }
    else if (0 == strTagName.compare(CONST_LABEL_IMG)
			 || 0 == strTagName.compare(CONST_LABEL_SVGIMG))
    {
        iType = IMG;
    }
    else if (0 == strTagName.compare(CONST_LABEL_A))
    {
        iType = A;
    }
    else if (0 == strTagName.compare(CONST_LABEL_TABLE))
    {
        iType = TABLE;
    }
    else if (0 == strTagName.compare(CONST_LABEL_TR))
    {
        iType = TR;
    }
    else if (0 == strTagName.compare(CONST_LABEL_THEAD))
    {
        iType = THEAD;
    }
    else if (0 == strTagName.compare(CONST_LABEL_TBODY))
    {
        iType = TBODY;
    }
    else if (0 == strTagName.compare(CONST_LABEL_TFOOT))
    {
        iType = TFOOT;
    }
    else if (0 == strTagName.compare(CONST_LABEL_TD))
    {
        iType = TD;
    }
    else if (0 == strTagName.compare(CONST_LABEL_TH))
    {
        iType = TH;
    }
    else if(0 == strTagName.compare("link"))
    {
        //add liutian debug <link>
        cout<< "strTagName===link，没有被支持" <<endl;
    }
	
    return iType;
}

/********************************************************************
 函数名称  :	ResetCurNode
 函数描述  : 重新复位当前所指向的节点，使当前节点指向根节点
 输入参数  : N/A
 输出参数  : void
 返回值    : void
 备注      :	N/A
 *********************************************************************/
void CParseFile::ResetPos()
{
    FUN_TRACE();
    if (NULL == m_pRootNode)
    {
        return;
    }
    m_pCurNode = m_pRootNode;
}

/********************************************************************
 函数名称  :	TrimTwoSideSpace
 函数描述  : 去掉两边多余的不可见字符，实际的处理函数
 输入参数  : szOriginal --- 要处理的字符串
 iLen       --- 要处理的字符串长度
 输出参数  : strDest    --- 返回处理后的字符串
 返回值    : void
 备注      :	N/A
 *********************************************************************/
void CParseFile::TrimTwoSideSpace(const char* szOriginal, int iLen, string& strDest)
{
    FUN_TRACE();
	
    int right = iLen - 1;
    while (right >= 0 && (unsigned char)(szOriginal[right]) <= ' ')
    {
        right--;
    }
	
    int left = 0;
    while (left < right && (unsigned char)(szOriginal[left]) <= ' ')
    {
        left++;
    }
	
    strDest.assign(szOriginal + left, right - left + 1);
}

/********************************************************************
 函数名称  :	GetPureHtmlText
 函数描述  : 获取纯Html文本
 输入参数  : pszRawText，原始读入的文本
 输入参数  : len，文本长度
 输出参数  : N/A
 返回值    : 返回去除多余空格和转义后的Utf8字符串
 备注      :	N/A
 *********************************************************************/
string CParseFile::GetPureHtmlText(const char* pszRawText, int len)
{
    string strRet;
    TrimTwoSideSpace(pszRawText, len, strRet);
	
    int iTrimLen = strRet.length();
    if (iTrimLen > 0)
    {
        const char* pSrc = strRet.c_str();
        char* pszDst = (char*)calloc(iTrimLen + 1, 1);
        if (pszDst != NULL)
        {
            int iDstLen = 0;
            for (int i = 0; i < iTrimLen; ++i)
            {
                if (pSrc[i] == '&') // 转义字符
                {
                    // 获取Unicode转义字符值
                    unsigned short wChar = 0;
					
                    const char* start = pSrc + i + 1;
                    const char* end = strchr(start, ';');
                    if (end != NULL)
                    {
                        int keyLen = end - start;
                        if (*start == '#')      // Unicode转义字符
                        {
                            // 最长为#8988五位
                            if (keyLen > 1 && keyLen <= 5)
                            {
                                // 转换成10进制数
                                for (++start; *start != ';'; ++start)
                                {
                                    wChar = wChar * 10 + (*start - '0');
                                }
								
                                // 移动到;上
                                i += keyLen + 1;
                            }
                        }
                        else                    // 实体转义字符
                        {
                            wChar = GetEntityChar(start, keyLen);
                            i += keyLen + 1;
                        }
						
                        // 将Unicode字符转换为Utf8字符
                        if (wChar < 0x0080)
                        {
                            pszDst[iDstLen++] = (char)wChar;
                        }
                        else
                        {
                            char low_six = wChar & 0x3F;
                            if (wChar < 0x7FF)
                            {
                                pszDst[iDstLen++] = 0xC0 | (wChar >> 6);
                                pszDst[iDstLen++] = 0x80 | low_six;
                            }
                            else
                            {
                                pszDst[iDstLen++] = 0xE0 | ((wChar >> 12) & 0x0F);
                                pszDst[iDstLen++] = 0x80 | ((wChar >> 6) & 0x3F);
                                pszDst[iDstLen++] = 0x80 | low_six;
                            }
                        }
                    }
                }
                else if ((unsigned char)pSrc[i] <= ' ') // 空白字符
                {
                    // 空白转换成空格
                    pszDst[iDstLen++] = ' ';
					
                    // 去除后面的空白字符
                    while ((unsigned char)pSrc[++i] <= ' ');
					
                    // 抵消for循环的i++
                    --i;
                }
                else
                {
                    // 一般字符，直接加入目标
                    pszDst[iDstLen++] = pSrc[i];
                }
            }
            strRet.assign(pszDst, iDstLen);
			
            free(pszDst);
        }
    }
	//cout<<"strRet==="<<strRet<<endl;
    return strRet;
}

/********************************************************************
 函数名称  :	ClearCSSList
 函数描述  : 清空样式表列表
 输入参数  : N/A
 输出参数  : void
 返回值    : void
 备注      :	N/A
 *********************************************************************/
void CParseFile::ClearCSSList()
{
    FUN_TRACE();
    vector<CSS_STYLE*>::iterator iter = m_vecCssStyle.begin();
    for (; iter != m_vecCssStyle.end(); ++iter)
    {
        DELETE_OBJ(*iter);
    }
    m_vecCssStyle.clear();
}

/********************************************************************
 函数名称  :	IsDocUtf8
 函数描述  : 判读文档是否UTF-8格式
 输入参数  : pszText，文档文本
 输入参数  : iLen，文档文本长度，如果为-1，则本函数计算长度
 输出参数  : hasBom，返回是否有BOM头
 返回值    : 如果是Utf8编码，返回true，否则返回false
 备注      :	N/A
 *********************************************************************/
bool CParseFile::IsDocUtf8(const char* pszText, int iLen, bool &hasBom)
{
    bool isUtf8 = false;
    hasBom = false;
	
    int iTextLen = iLen;
    if (iTextLen < 0)
    {
        iTextLen = strlen(pszText);
    }
	
    // 如果带有UTF-8 BOM头，则跳过
    if (iTextLen > 3
        && (unsigned char)pszText[0] == 0xef && (unsigned char)pszText[1] == 0xbb && (unsigned char)pszText[2] == 0xbf)
    {
        hasBom = true;
        isUtf8 = true;
    }
    else
    {
        int iOffset = 0;
        const char* p1 = strstr(pszText, "charset=");
        if (p1 == NULL)
        {
            p1 = strstr(pszText, "Charset=");
        }
		
        // 找到charset/Charset
        if (p1 != NULL)
        {
            iOffset = 8;
        }
        else
        {
            // 找encoding
            p1 = strstr(pszText, "encoding=");
            if (p1 != NULL)
            {
                iOffset = 9;
            }
        }
		
        if (p1 != NULL)
        {
            // 往后移动charset= 长度
            const char* p2 = p1 + iOffset;
            if (*p2 == '\"' || *p2 == '\'')
            {
                // 往后移动一个字符(引号)
                p2++;
            }
            if (Strnicmp(p2, "utf-8", 5) == 0)
            {
                isUtf8 = true;
            }
        }
    }
    return isUtf8;
}

/*******************************************************************************
 函数名称  : Strnicmp
 函数描述  : 忽略大小写的字符串比较
 输入参数  : first，第一个输入字符串
 输入参数  : last，第二个输入字符串
 输入参数  : count，比较的个数
 返回值    : 如果first>second,返回正数，如果first<second返回负数
 如果first==second返回0
 备注      : 该函数从微软的VC2005 crt移植
 *******************************************************************************/
int CParseFile::Strnicmp(const char * first, const char * last, int count)
{
    if(count)
    {
        int f=0;
        int l=0;
		
        do
        {
			
            if ( ((f = (unsigned char)(*(first++))) >= 'A') &&
                (f <= 'Z') )
                f -= 'A' - 'a';
			
            if ( ((l = (unsigned char)(*(last++))) >= 'A') &&
                (l <= 'Z') )
                l -= 'A' - 'a';
			
        }
        while ( --count && f && (f == l) );
		
        return ( f - l );
    }
    else
    {
        return 0;
    }
}

/********************************************************************
 函数名称  :	Stop
 函数描述  : 停止解析
 输入参数  : N/A
 输出参数  : N/A
 返回值    : void
 备注      :	N/A
 *********************************************************************/
void CParseFile::Stop()
{
    m_bStopLoad = true;
}


