/********************************************************************
 文件名称 :	HTML.h 头文件
 作    者 :  liutian
 创建时间 :  2010-12-3  9:33:18
 文件描述 :	xhtml解析树节点类
 修改历史 :	2010-12-3	1.00	初始版本
 *********************************************************************/
#ifndef _HTML_H_
#define _HTML_H_


#include <string>
#include <vector>
#include "const_def.h"

#include "Content.h"

class IUICallBack;
class CNode;

class CContent;
class CListContent;
class CListItemContent;
class CTableContent;
class CTableCellContent;
class CRenderEngine;



//节点管理类
class CHTML
{
    enum
    {
        E_EMPTY_TAG,        // 空标签<>
        E_COMMENT_TAG,      // 注释标签<!--xxx-->
        E_START_TAG,        // 开始标签<xxx a="" b="">
        E_END_TAG,          // 结束标签</xxx>
        E_SELF_END_TAG,     // 自结束标签<xxx a="" b=""/>
        E_TEXT_TAG,         // 可绘制文本
    };
	
public:
    CHTML(CRenderEngine* pEngine);
    ~CHTML();
	
    /********************************************************************
	 函数名称  :	Load
	 函数描述  : 直接加载文件内容
	 输入参数  : pszSource --- 需要解析的内容
	 isPath  传入的pszValue是否为文件路劲
	 输出参数  : N/A
	 返回值    : void
	 备注      :	N/A
	 *********************************************************************/
    BOOL Load(const char* pszValue, BOOL isPath);
	
    /********************************************************************
	 函数名称  :	ConstructRenderNode
	 函数描述  : 构造渲染节点
	 输入参数  : pNode    --- 当前需要绘制的节点
	 iTagType --- 标签类型
	 输出参数  : N/A
	 返回值    : void
	 备注      :	N/A
	 *********************************************************************/
    void ConstructRenderNode(CNode* pNode, int iTagType);
	
private:
	
    /********************************************************************
	 函数名称  :	ConstructTextNode
	 函数描述  : 构造文本节点
	 输入参数  : pNode    --- 要绘制的节点指针
	 iTagType --- 标签类型
	 输出参数  : void
	 返回值    : CElement* --- 抛给绘制层的要绘制的结构指针
	 备注      :	N/A
	 *********************************************************************/
    CContent* ConstructTextNode(CNode* pNode);
	
    /********************************************************************
	 函数名称  :	ConstructImageNode
	 函数描述  : 构造图片节点
	 输入参数  : pNode     ---  解析树的节点指针
	 输出参数  : void
	 返回值    : CElement* --- 抛给绘制层的要绘制的结构指针
	 备注      :	N/A
	 *********************************************************************/
    CContent* ConstructImageNode(CNode* pNode);
	
    /********************************************************************
	 函数名称  :	ConstructHyperLinkNode
	 函数描述  : 构造超链接节点
	 输入参数  : pNode    ---  解析树的节点指针
	 输出参数  : void
	 返回值    : CElement* --- 抛给绘制层的要绘制的结构指针
	 备注      :	N/A
	 *********************************************************************/
    CContent* ConstructHyperLinkNode(CNode* pNode);
	
    /********************************************************************
	 函数名称  :	ConstructListNode
	 函数描述  : 构造list节点
	 输入参数  : pNode       --- list节点
	 输出参数  : void
	 返回值    : CElement* --- 构造好的list节点指针
	 备注      :	N/A
	 *********************************************************************/
    CContent* ConstructListNode(CNode* pNode);
	
    /********************************************************************
	 函数名称  :	ConstructListItemNode
	 函数描述  : 解析list中每个列表项节点（dt、dd、li）内的内容，遍历其
	 子节点，主要考虑列表嵌套的情形
	 输入参数  : pNode     --- dt/dd/li节点
	 返回值    : 构造好的列表条目渲染节点
	 备注      :	N/A
	 *********************************************************************/
    CListItemContent* ConstructListItemNode(const CNode* const pNode);
	
    /********************************************************************
	 函数名称  :	ConstructListItemChild
	 函数描述  : 构造列表条目节点中的子节点
	 输入参数  : pListItem 当前的列表条目节点
	 pNode 要处理的节点
	 输出参数  : void
	 返回值    : void
	 备注      :	N/A
	 *********************************************************************/
    void ConstructListItemChild(CListItemContent* pListItem, const CNode* pNode);
	
    /********************************************************************
	 函数名称  :	ConstructTableNode
	 函数描述  : 构造Table节点
	 输入参数  : pNode     --- 要解析的树节点
	 输出参数  : void
	 返回值    : CElement* --- 构造好的Table节点指针
	 备注      :	N/A
	 *********************************************************************/
    CContent* ConstructTableNode(CNode* pNode);
	
    /********************************************************************
	 函数名称  :	ParserTrRow
	 函数描述  : 解析table子节点tr节点，一个tr节点表示了表格的一行，该
	 函数提供了遍历一个tr标签内的子节点的方法，也就是遍历表
	 格中的行，然后再调用ParserTdThCol构造解析每个单元格
	 输入参数  : pTrNode    --- 要解析的tr节点
	 iRow       --- 当前的行号
	 iCol       --- 当前的列号
	 输出参数  : pTableNode ---  保存解析后的表格项
	 返回值    : void
	 备注      :	N/A
	 *********************************************************************/
    int ParserTableRow(const CNode* const pTrNode, CTableContent* pTableNode, int iRow);
	
    /********************************************************************
	 函数名称  :	ConstructTableCellNode
	 函数描述  : 解析td,th节点，也就是tr的子节点，代表一行中的某个表格
	 单元，该函数供ParserTrRow调用
	 输入参数  : pChildNode --- 要解析的节点
	 iRow       --- 当前的行号
	 iCol       --- 当前的列号
	 输出参数  : pTableNode --- 保存解析后的表格项
	 返回值    : void
	 备注      :	N/A
	 *********************************************************************/
    CTableCellContent* ConstructTableCellNode(const CNode* const pChildNode);
	
    /********************************************************************
	 函数名称  :	ConstructTableCellChild
	 函数描述  : 构造表格单元格节点中的子节点
	 输入参数  : pTableCell 当前的表格单元格
	 pNode 要处理的节点
	 输出参数  : void
	 返回值    : void
	 备注      :	N/A
	 *********************************************************************/
    void ConstructTableCellChild(CTableCellContent* pTableCell, const CNode* pNode);
	
public:
    BOOL                    m_bIsBreakLine;        // 当前html页面的全局换行标志
private:
    CRenderEngine*          m_pRenderEngine;
};

#endif // _HTML_H_

