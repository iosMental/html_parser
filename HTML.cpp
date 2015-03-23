/******************************************************************************
 文件名称 :	HTML.cpp 实现文件
 作    者 :  ttc
 创建时间 :  2010-12-3  14:37:00
 文件描述 :
 修改历史 :	2010-12-3	1.00	初始版本
 
 ******************************************************************************/
#include "EasyLog.h"
#include "HTML.h"
#include <stdio.h>
#include "IUICallBack.h"
#include "ParseFile.h"
#include "Content.h"
#include "TextContent.h"
#include "HyperContent.h"
#include "ListContent.h"
#include "ListItemContent.h"
#include "TableContent.h"
#include "TableCellContent.h"
#include "ImageContent.h"
#include "RenderEngine.h"
#include "StrUtility.h"

#include <iostream>
//++++++++++++++++++++++++以下为CHTML类的成员方法定义+++++++++++++++++++++++++++++++++++++++
/********************************************************************
 函数名称  :	CHTML
 函数描述  : 构造函数
 输入参数  :
 输出参数  : void
 返回值    : void
 备注      :	N/A
 *********************************************************************/
CHTML::CHTML(CRenderEngine* pEngine)
: m_pRenderEngine(pEngine)
{
    FUN_TRACE();
    m_bIsBreakLine = FALSE;
}

/********************************************************************
 函数名称  :	~CHTML
 函数描述  : 析构函数
 输入参数  :
 输出参数  : void
 返回值    : void
 备注      :	N/A
 *********************************************************************/
CHTML::~CHTML()
{
    FUN_TRACE();
    CParseFile::ReleaseInstance();
}

/********************************************************************
 函数名称  :	Load
 函数描述  : 直接加载文件内容
 输入参数  : pszValue --- 需要解析的内容或文件路径
 isPath  传入的pszValue是否为文件路劲
 输出参数  : N/A
 返回值    : void
 备注      :	N/A
 *********************************************************************/
BOOL CHTML::Load(const char* pszValue, BOOL isPath)
{
    FUN_TRACE();
	
    BOOL bResult = FALSE;
	
    bResult = CParseFile::GetInstance()->Load(pszValue, (void*)this, isPath);
    
    return bResult;
}

/********************************************************************
 函数名称  :	ConstructRenderNode
 函数描述  : 构造渲染节点
 输入参数  : pNode    --- 当前需要绘制的节点
 iTagType --- 标签类型
 输出参数  : N/A
 返回值    : void
 备注      :	N/A
 *********************************************************************/
void CHTML::ConstructRenderNode(CNode* pNode, int iTagType)
{
    FUN_TRACE();
    CContent* pElement = NULL;  // 抛给绘制层的图元指针
    //遇到列表结束标签开始绘制列表
    if ((DL == pNode->m_iTagId || OL == pNode->m_iTagId || UL == pNode->m_iTagId)
        && E_END_TAG == iTagType)
    {
        // 构造 list
        pElement = ConstructListNode(pNode);
    }
    else if (TABLE == pNode->m_iTagId
			 && E_END_TAG == iTagType) //遇到表格结束标签开始绘制表格
    {
        // 构造 table
        pElement = ConstructTableNode(pNode);
    }
    else if (IMG == pNode->m_iTagId
			 && E_SELF_END_TAG == iTagType) //遇到图像节点
    {
        // 构造 image
        pElement = ConstructImageNode(pNode);
    }
    else if (A == pNode->m_iTagId
			 && E_END_TAG == iTagType)
    {
        // 构造超链接
        pElement = ConstructHyperLinkNode(pNode);
        if (((CHyperContent*)pElement)->m_pContent == NULL)
        {
            delete pElement;
            return;
        }
    }
    else if (E_TEXT_TAG == iTagType)  //其余为文本节点
    {
        //对文本节点的回溯绘制
        pElement = ConstructTextNode(pNode);
    }
	
    if (NULL != pElement)
    {
        //调用回调函数绘制节点
        if (m_pRenderEngine != NULL)
        {
            m_pRenderEngine->HtmlCallBackFunc((CContent*)pElement);
        }
    }
}

/********************************************************************
 函数名称  :	ConstructTextNode
 函数描述  : 构造文本节点
 输入参数  : pNode     --- 要绘制的节点指针
 iTagType  --- 标签类型
 输出参数  : void
 返回值    : CElement* --- 抛给绘制层的要绘制的结构指针
 备注      :	N/A
 *********************************************************************/
CContent* CHTML::ConstructTextNode(CNode* pNode)
{
    FUN_TRACE();
	
    if (NULL == pNode)
    {
        return NULL;
    }
	
    CTextContent* pTextNode = new CTextContent(m_pRenderEngine);
    if (NULL == pTextNode)
    {
        //EPUB_TRACE(LOG_LEVEL_ERROR, "Alloc memory failure.\n");
        return NULL;
    }
    pTextNode->m_pNode = pNode;
    pTextNode->m_bIsBreakLine = m_bIsBreakLine;
    m_bIsBreakLine = FALSE;
	
    //节点回溯保存祖先节点列表
    CNode* pCursor = pNode->m_pParentNode;
    CNode* pRootNode = CParseFile::GetInstance()->m_pRootNode;
    while (pCursor != pRootNode)
    {
        //判断是否为斜体
        if (I == pCursor->m_iTagId
            || ADDRESS == pCursor->m_iTagId
            || CITE == pCursor->m_iTagId
            || DFN == pCursor->m_iTagId
            || EM == pCursor->m_iTagId
            || VAR == pCursor->m_iTagId)
        {
            pTextNode->m_bIsItalic = TRUE;
        }
        //判断是否为粗体
        else if (STRONG == pCursor->m_iTagId
				 || B == pCursor->m_iTagId)
        {
            pTextNode->m_bIsBold = TRUE;
        }
        // 判断是否有下划线
        else if (U == pCursor->m_iTagId
				 || A == pCursor->m_iTagId)
        {
            pTextNode->m_bIsUnderline = TRUE;
        }
		
        pCursor = pCursor->m_pParentNode;
    }
	
    return pTextNode;
}


BOOL ProcessDir(string &strDest, const string &strSource)
{
    int iArrBevelPos[MAX_PATH] = {0};
    int iArrLen=0;
    int iBackPos = 0;
    int len = strSource.length();
    string temp;
    int i = 0;
	
    // 计算源里面有多少层目录
    for (i = 0; i < len; ++i)
    {
        if (strSource.at(i) == '/')
        {
            iArrBevelPos[iArrLen] = i+1;
            iArrLen++;
        }
    }
	
    // 计算目标里面有多少层回退 "../"
    for ( i = 0; i < len; ++i)
    {
        if (strDest.substr(i, 3) == "../")
        {
            i += 2;
            iBackPos++;
        }
        else
        {
            break;
        }
    }
	
    // 如果回退的目录层次大于源的目录层次，则文件有错误，因为所有的资源文件都在epub包里面.
    if (iBackPos > iArrLen)
    {
        return FALSE;
    }
    // 回退层次小于源的目录层次
    else if (iBackPos < iArrLen)
    {
        string strSubDest = strDest.substr(iBackPos*3, strDest.length());
        strDest = strSource.substr(0, iArrBevelPos[iArrLen - iBackPos - 1])
		+ strSubDest;
    }
    // 回退层次等于源的目录层次，且层次不能为零，为零的话不需要处理，直接返回strDest即可
    else if(iBackPos > 0)
    {
        strDest = strDest.substr(iBackPos*3, strDest.length());
    }
	
    return TRUE;
}

/********************************************************************
 函数名称  :	ConstructImageNode
 函数描述  : 构造图片节点
 输入参数  : pNode     ---  解析树的节点指针
 输出参数  : void
 返回值    : CElement* --- 抛给绘制层的要绘制的结构指针
 备注      :	N/A
 *********************************************************************/
CContent* CHTML::ConstructImageNode(CNode* pNode)
{
    FUN_TRACE();
    CImageContent* pImageNode = new CImageContent(m_pRenderEngine);
    if (NULL == pImageNode)
    {
        EPUB_TRACE(LOG_LEVEL_ERROR, "Alloc CImageElement memory failure.\n");
        return NULL;
    }
	
    pImageNode->m_pNode = pNode;
    pImageNode->m_bIsBreakLine = m_bIsBreakLine;
    m_bIsBreakLine = FALSE;
	
    ATTRIBUTE_LIST::const_iterator iter = pNode->m_vecAttribute.begin();
    ATTRIBUTE_LIST::const_iterator iterEnd = pNode->m_vecAttribute.end();
    for (; iter != iterEnd; ++iter)
    {
		string strName = (*iter)->strName;
        cout<<"strName==="<<strName<<endl;
        if (0 == strName.compare(CONST_ATTRIBUTE_SRC)
            || 0 == strName.compare(CONST_ATTRIBUTE_XLINKHREF))
        {
            wstring wstrPath = Utf8ToUnicode((*iter)->strValue);
            // 获取Html所在目录，并存起来
            CRenderEngine* pRenderEngine = m_pRenderEngine;
			
#ifdef WIN32
            int pos = pRenderEngine->m_strHtmlPath.find_last_of('\\') + 1;
            string strHtmlDir = pRenderEngine->m_strHtmlPath.substr(0, pos);
            string strPath = UnicodeToAnsi(wstrPath);
            //string strPath = strPath1;
            ProcessDir(strPath, pRenderEngine->m_strFileDir);
            pImageNode->m_wstrPath = AnsiToUnicode(strHtmlDir + strPath);
#else
            int pos = pRenderEngine->m_strHtmlPath.find_last_of('/') + 1;
            string strHtmlDir = pRenderEngine->m_strHtmlPath.substr(0, pos);
            pImageNode->m_wstrPath = Utf8ToUnicode(strHtmlDir) + wstrPath;
#endif
        }
        
        else if (0 == strName.compare(CONST_ATTRIBUTE_WIDTH))
        {
               cout<<"strName==="<<strName<<endl;
            pImageNode->m_iPointedWidth = atoi((*iter)->strValue.c_str());
        }
        else if (0 == strName.compare(CONST_ATTRIBUTE_HEIGHT))
        {
               cout<<"strName==="<<strName<<endl;
            pImageNode->m_iPointedHeight = atoi((*iter)->strValue.c_str());
        }
    }
    return (CContent*)pImageNode;
}

/********************************************************************
 函数名称  :	ConstructHyperLinkNode
 函数描述  : 构造超链接节点
 输入参数  : pNode    ---  解析树的节点指针
 输出参数  : void
 返回值    : CElement* --- 抛给绘制层的要绘制的结构指针
 备注      :	N/A
 *********************************************************************/
CContent* CHTML::ConstructHyperLinkNode(CNode* pNode)
{
    FUN_TRACE();
    CHyperContent* pHyperNode = new CHyperContent(m_pRenderEngine);
    if (NULL == pHyperNode)
    {
        EPUB_TRACE(LOG_LEVEL_ERROR, "Alloc CHyperContent memory failure.\n");
        return NULL;
    }
	
    pHyperNode->m_pNode = pNode;
    pHyperNode->m_bIsBreakLine = m_bIsBreakLine;
    m_bIsBreakLine = FALSE;
	
    // 获取链接到的地址
    ATTRIBUTE_LIST::const_iterator iter = pNode->m_vecAttribute.begin();
    ATTRIBUTE_LIST::const_iterator iterEnd = pNode->m_vecAttribute.end();
    for (; iter != iterEnd; ++iter)
    {
        cout<<"1111111strName=="<<(*iter)->strName<<endl;
        string strName = (*iter)->strName;
        if (0 == strName.compare(CONST_ATTRIBUTE_HREF))
        {
            cout<<"strName=="<<strName<<endl;
            pHyperNode->m_strHref = (*iter)->strValue;
            cout<<"pHyperNode->m_strHref=="<<pHyperNode->m_strHref<<endl;
        }
    }
	
    // 获取超链接内容
    if (!pNode->m_vecChildNode.empty())
    {
        CContent* pElement = NULL;
        // 应该只有一个子节点
        if (IMG == pNode->m_vecChildNode[0]->m_iTagId)
        {     // 图片链接
            pElement = ConstructImageNode(pNode->m_vecChildNode[0]);
        }
        else if (TEXT == pNode->m_vecChildNode[0]->m_iTagId)  // 为文本链接
        {
            pElement = ConstructTextNode(pNode->m_vecChildNode[0]);
        }
		
        if (NULL != pElement)
        {
            pHyperNode->m_pContent = pElement;
        }
    }
	
    return (CContent*)pHyperNode;
}

/********************************************************************
 函数名称  :	ConstructListNode
 函数描述  : 构造list节点
 输入参数  : pNode       --- list节点
 输出参数  : void
 返回值    : CElement*   --- 构造好的list节点指针
 备注      :	N/A
 *********************************************************************/
CContent* CHTML::ConstructListNode(CNode* pNode)
{
    FUN_TRACE();
    CListContent* pListNode = new CListContent(m_pRenderEngine);
    if (NULL == pListNode)
    {
        EPUB_TRACE(LOG_LEVEL_ERROR, "Alloc CListElement memory failure.\n");
        return NULL;
    }
	
    pListNode->m_pNode = pNode;
    pListNode->m_bIsBreakLine = m_bIsBreakLine;
    m_bIsBreakLine = FALSE;
	
    if (DL == pNode->m_iTagId) //定义列表
    {
        pListNode->m_iType = E_LIST_TYPE_DEFINE;
    }
    else if (OL == pNode->m_iTagId) //有序列表
    {
        pListNode->m_iType = E_LIST_TYPE_ORDER;
    }
    else if (UL == pNode->m_iTagId) //无序列表
    {
        pListNode->m_iType = E_LIST_TYPE_UNORDER;
    }
	
    //遍历子节点
    CListItemContent* pListItemContent = NULL;
    vector<CNode*>::const_iterator iter = pNode->m_vecChildNode.begin();
    vector<CNode*>::const_iterator iterEnd = pNode->m_vecChildNode.end();
    int iOrder = 1;
    for (; iter != iterEnd; ++iter)
    {
        CNode* pChildNode = *iter;
		
        //首先判断是否为dt/dd/li节点
        if (DT == pChildNode->m_iTagId
            || DD == pChildNode->m_iTagId
            || LI == pChildNode->m_iTagId)
        {
            pListItemContent = ConstructListItemNode(pChildNode);
            if (pListItemContent != NULL)
            {
                pListItemContent->m_pListContent = pListNode;
                pListItemContent->m_iOrder = iOrder++;
                pListNode->m_vecItem.push_back(pListItemContent);
            }
        }
    }
    return (CContent*)pListNode;
}

/********************************************************************
 函数名称  :	ConstructListItemNode
 函数描述  : 解析list中每个列表项节点（dt、dd、li）内的内容，遍历其
 子节点，主要考虑列表嵌套的情形
 输入参数  : pNode     --- dt/dd/li节点
 返回值    : 构造好的列表条目渲染节点
 备注      :	N/A
 *********************************************************************/
CListItemContent* CHTML::ConstructListItemNode(const CNode* const pNode)
{
    FUN_TRACE();
    CListItemContent* pListItem = new CListItemContent(m_pRenderEngine);
    if (NULL == pListItem)
    {
        return NULL;
    }
	
    // 构造列表条目节点中的子节点
    ConstructListItemChild(pListItem, pNode);
	
    return pListItem;
}

/********************************************************************
 函数名称  :	ConstructListItemChild
 函数描述  : 构造列表条目节点中的子节点
 输入参数  : pListItem 当前的列表条目节点
 pNode 要处理的节点
 输出参数  : void
 返回值    : void
 备注      :	N/A
 *********************************************************************/
void CHTML::ConstructListItemChild(CListItemContent* pListItem, const CNode* pNode)
{
    //遍历dt/dd/li的子节点
    vector<CNode*>::const_iterator iter = pNode->m_vecChildNode.begin();
    vector<CNode*>::const_iterator iterEnd = pNode->m_vecChildNode.end();
    for (; iter != iterEnd; ++iter)
    {
        CNode* pChildNode = *iter;
        if ((DL == pChildNode->m_iTagId
			 || OL == pChildNode->m_iTagId
			 || UL == pChildNode->m_iTagId)
            && !pChildNode->m_vecChildNode.empty()) //节点内有一个嵌套的List
        {
            CContent* pNestList = NULL;
            pNestList = ConstructListNode(pChildNode);
            if (NULL != pNestList)
            {
                pListItem->m_vecUnit.push_back(pNestList);
            }
        }
        else if (TABLE == pChildNode->m_iTagId
				 && !pChildNode->m_vecChildNode.empty()) //节点内有一个嵌套的TABLE
        {
            CContent* pNestTable = NULL;
            pNestTable = ConstructTableNode(pChildNode);
            if (NULL != pNestTable)
            {
                pListItem->m_vecUnit.push_back(pNestTable);
            }
        }
        else if (IMG == pChildNode->m_iTagId) //嵌套图片
        {
            CContent* pNestImage = NULL;
            pNestImage = ConstructImageNode(pChildNode);
            if (NULL != pNestImage)
            {
                pListItem->m_vecUnit.push_back(pNestImage);
            }
        }
        else if (TEXT == pChildNode->m_iTagId) // 是一个普通的列表元素
        {
            CContent* pNestText = NULL;
            pNestText = ConstructTextNode(pChildNode);
            if (NULL != pNestText)
            {
                pListItem->m_vecUnit.push_back(pNestText);
            }
        }
        else
        {
            ConstructListItemChild(pListItem, pChildNode);
        }
    }
}

/********************************************************************
 函数名称  :	ConstructTableNode
 函数描述  : 构造Table节点
 输入参数  : pNode     --- 要解析的树节点
 输出参数  : void
 返回值    : CElement* --- 构造好的Table节点指针
 备注      :	N/A
 *********************************************************************/
CContent* CHTML::ConstructTableNode(CNode* pNode)
{
	FUN_TRACE();
    CTableContent* pTableNode = new CTableContent(m_pRenderEngine);
    if (NULL == pTableNode)
    {
        EPUB_TRACE(LOG_LEVEL_ERROR, "Alloc CTableElement memory failure.\n");
        return NULL;
    }
	
    pTableNode->m_pNode = pNode;
    pTableNode->m_bIsBreakLine = m_bIsBreakLine;
    m_bIsBreakLine = FALSE;
	
    int iRow = 0;   //表格的行数
    int iCol = 0;   //表格的列数
    vector<CNode*>::const_iterator iter = pNode->m_vecChildNode.begin();
    for (; iter != pNode->m_vecChildNode.end(); ++iter)
    {
        CNode* pChildNode = *iter;
        if (CAPTION == pChildNode->m_iTagId) //找到caption节点
        {
            if (1 == pChildNode->m_vecChildNode.size())  //有一个子节点,取出caption标题
            {
                pTableNode->m_strCaption = pChildNode->m_vecChildNode[0]->m_strName;
            }
        }
        else if (TR == pChildNode->m_iTagId) //找到tr节点
        {
            //解析表格的一行
            int iTempCol = ParserTableRow(pChildNode, pTableNode, iRow);
            if (iTempCol > iCol)
            {
                iCol = iTempCol;
            }
            iRow++;
        }
        else if (THEAD == pChildNode->m_iTagId
				 || TBODY == pChildNode->m_iTagId
				 || TFOOT == pChildNode->m_iTagId)
        {
            vector<CNode*>::const_iterator iter = pChildNode->m_vecChildNode.begin();
            for (; iter != pChildNode->m_vecChildNode.end(); ++iter)
            {
                CNode* pBodyChildNode = *iter;
                if (TR == pBodyChildNode->m_iTagId) //找到tr节点
                {
                    //解析表格的一行
                    int iTempCol = ParserTableRow(pBodyChildNode, pTableNode, iRow);
                    if (iTempCol > iCol)
                    {
                        iCol = iTempCol;
                    }
                    iRow++;
                }
            }
        }
    }
	
    // 保存表格的总行数和总列数
    pTableNode->m_iSumRow = iRow;
    pTableNode->m_iSumCol = iCol;
	
    //解析表格的属性 border cellpadding cellspacing
    ATTRIBUTE_LIST::const_iterator iterAttr = pNode->m_vecAttribute.begin();
    NODEATTRIBUTE* pNodeAttribute = NULL;
    for (; iterAttr != pNode->m_vecAttribute.end(); ++iterAttr)
    {
        pNodeAttribute = *iterAttr;
         cout<<"strName==="<<pNodeAttribute->strName<<endl;
        if (0 == pNodeAttribute->strName.compare(CONST_ATTRIBUTE_BORDER))
        {
            pTableNode->m_iBorder = atoi(pNodeAttribute->strValue.c_str());
        }
        else if (0 == pNodeAttribute->strName.compare(CONST_ATTRIBUTE_CELLPADDING))
        {
            pTableNode->m_iCellpadding = atoi(pNodeAttribute->strValue.c_str());
        }
        else if (0 == pNodeAttribute->strName.compare(CONST_ATTRIBUTE_CELLSPACING))
        {
            pTableNode->m_iCellspacing = atoi(pNodeAttribute->strValue.c_str());
        }
    }
    return (CContent*)pTableNode;
}

/********************************************************************
 函数名称  :	ParserTrRow
 函数描述  : 解析table子节点tr节点内的表格单元
 输入参数  : pTrNode    --- 要解析的tr节点
 iRow       --- 当前的行号
 输出参数  : pTableNode ---  保存解析后的表格项
 返回值    : int  当前行中的单元格个数
 备注      :	N/A
 *********************************************************************/
int CHTML::ParserTableRow(const CNode* pTrNode, CTableContent* pTableNode, int iRow)
{
    FUN_TRACE();
    int iCol = 0;
    CNode* pNode = (CNode*)pTrNode;
    if (!pNode->m_vecChildNode.empty())  //有子节点
    {
        CTableCellContent* pTableCell = NULL;
        vector<CNode*>::iterator iterTd = pNode->m_vecChildNode.begin();
        vector<CNode*>::iterator iterTdEnd = pNode->m_vecChildNode.end();
        for (; iterTd != iterTdEnd; ++iterTd)
        {
            CNode* pAncestor = *iterTd;
            if (NULL == pAncestor)
            {
                EPUB_TRACE(LOG_LEVEL_ERROR, "In ParserTrRow the pAncestor pointer is NULL.\n");
                continue;
            }
			
            // 构造TD、TH节点
            pTableCell = ConstructTableCellNode(pAncestor);
            if (NULL != pTableCell)
            {
                pTableCell->m_iRow = iRow;
                pTableCell->m_iCol = iCol;
                pTableCell->m_pTableContent = pTableNode;
                pTableNode->m_vecUnit.push_back(pTableCell);
            }
            iCol++;
        }
    }
    return iCol;
    
}

/********************************************************************
 函数名称  :	ConstructTableCellNode
 函数描述  : 构造表格单元节点（td、th）
 输入参数  : pChildNode --- 要解析的节点
 　　　　　　iRow       --- 当前的行号
 　　　　　　iCol       --- 当前的列号
 输出参数  : pTableNode --- 保存解析后的表格项
 返回值    : void
 备注      :	N/A
 *********************************************************************/
CTableCellContent* CHTML::ConstructTableCellNode(const CNode* const pNode)
{
    FUN_TRACE();
    CTableCellContent* pTableCell = new CTableCellContent(m_pRenderEngine);
    if (NULL == pTableCell)
    {
        return NULL;
    }
	
    // 构造表格单元格节点中的子节点
    ConstructTableCellChild(pTableCell, pNode);
	
    return pTableCell;
}

/********************************************************************
 函数名称  :	ConstructTableCellChild
 函数描述  : 构造表格单元格节点中的子节点
 输入参数  : pTableCell 当前的表格单元格
 pNode 要处理的节点
 输出参数  : void
 返回值    : void
 备注      :	N/A
 *********************************************************************/
void CHTML::ConstructTableCellChild(CTableCellContent* pTableCell, const CNode* pNode)
{
    vector<CNode*>::const_iterator iter = pNode->m_vecChildNode.begin();
    vector<CNode*>::const_iterator iterEnd = pNode->m_vecChildNode.end();
    for (; iter != iterEnd; ++iter)
    {
        CNode* pChildNode = *iter;
        if (NULL == pChildNode)
        {
            continue;
        }
		
        if (TABLE == pChildNode->m_iTagId
            && !pChildNode->m_vecChildNode.empty()) //表明是一个嵌套的table
        {
            CContent* pNestTable = NULL;
            pNestTable = ConstructTableNode(pChildNode);
            if (NULL != pNestTable)
            {
                pTableCell->m_vecUnit.push_back(pNestTable);
            }
        }
        else if ((DL == pChildNode->m_iTagId
				  || OL == pChildNode->m_iTagId
				  || UL == pChildNode->m_iTagId)
				 && !pChildNode->m_vecChildNode.empty()) //表明是一个嵌套的list
        {
            CContent* pNestList = NULL;
            pNestList = ConstructListNode(pChildNode);
            if (NULL != pNestList)
            {
                pTableCell->m_vecUnit.push_back(pNestList);
            }
        }
        else if (IMG == pChildNode->m_iTagId) //嵌套图片
        {
            CContent* pNestImage = NULL;
            pNestImage = ConstructImageNode(pChildNode);
            if (NULL != pNestImage)
            {
                pTableCell->m_vecUnit.push_back(pNestImage);
            }
        }
        else if (TEXT == pChildNode->m_iTagId)
        {
            CContent* pNestText = NULL;
            pNestText = ConstructTextNode(pChildNode);
            if (NULL != pNestText)
            {
                pTableCell->m_vecUnit.push_back(pNestText);
            }
        }
        else
        {
            ConstructTableCellChild(pTableCell, pChildNode);
        }
    }
}

