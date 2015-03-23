/********************************************************************
 文件名称 :	ParseFile.h 头文件
 作    者 :  liutian
 创建时间 :  2011-1-20  13:50:41
 文件描述 :	文本解析处理类头文件
 修改历史 :	2011-1-20	1.00	初始版本
 *********************************************************************/
#ifndef _PARSEFILE_H_
#define _PARSEFILE_H_

#include <string>
#include <vector>
#include "const_def.h"
#include "Node.h"


class CHTML;
class CParseFile
{
    enum
    {
        E_EMPTY_TAG,        // 空标签<>
        E_COMMENT_TAG,      // 注释标签<!--xxx-->
        E_START_TAG,        // 开始标签<xxx a="" b="">
        E_END_TAG,          // 结束标签</xxx>
        E_SELF_END_TAG,     // 自结束标签<xxx a="" b=""/>
        E_TEXT_TAG,         // 可绘制文本
        E_SCRIPT_TAG,     // 自结束标签<xxx a="" b=""/>
        E_STYLE_TAG,         // 可绘制文本
    };
	
    enum
    {
        E_TAG_NAME,        // 节点名称
        E_ATTR_NAME,       // 属性名称
        E_ATTR_VAUE,        // 属性值
        E_ATTR_DEF          //
    };
public:
	
    static CParseFile* GetInstance();
    static void ReleaseInstance();
	
    /********************************************************************
	 函数名称  :	Load
	 函数描述  : 从文件加载HTML文件，将HTML构建成树结构
	 输入参数  : strFileName --- 待加载的HTML文件名
	 输出参数  : N/A
	 返回值    : void
	 备注      :	N/A
	 *********************************************************************/
    BOOL Load(const char* pszValue, void* pParse, BOOL isPath);
	
    /********************************************************************
	 函数名称  :	CreateTree
	 函数描述  : 创建树，初始树只有一个根节点
	 输入参数  : N/A
	 输出参数  : N/A
	 返回值    : 成功返回TRUE, 否则返回FALSE
	 备注      :
	 *********************************************************************/
    BOOL CreateTree();
	
    /*********************************************************************
	 函数名称  :	AddNormalNode
	 函数描述  : 向html树中添加节点
	 输入参数  : szName       --- 节点名称
	 vecAttribute --- 节点的属性列表
	 输出参数  : void
	 返回值    : 成功返回TRUE，失败返回FALSE
	 备注      :	N/A
	 *********************************************************************/
    BOOL AddNode(const char* szName, ATTRIBUTE_LIST& vecAttribute, int iTagType);
	
    /********************************************************************
	 函数名称  :	GetTagTypeByTagName
	 函数描述  : 根据标签名称提取标签id
	 输入参数  : strTagName    标签名称
	 输出参数  : void
	 返回值    : void
	 备注      :	N/A
	 *********************************************************************/
    int GetTagTypeByTagName(string& strTagName);
	
    /********************************************************************
	 函数名称  :	ResetCurNode
	 函数描述  : 重新复位当前所指向的节点，使当前节点指向根节点
	 输入参数  : N/A
	 输出参数  : void
	 返回值    : void
	 备注      :	N/A
	 *********************************************************************/
    void ResetPos();
	
    /********************************************************************
	 函数名称  :	Parse
	 函数描述  : 解析文本
	 输入参数  : pszDoc --- 待解析的文本
	 输出参数  : N/A
	 返回值    : 成功返回TRUE, 否则返回FALSE
	 备注      :	N/A
	 *********************************************************************/
    BOOL Parse(const char* pszDoc);
	
    /********************************************************************
	 函数名称  :	Stop
	 函数描述  : 停止解析
	 输入参数  : N/A
	 输出参数  : N/A
	 返回值    : void
	 备注      :	N/A
	 *********************************************************************/
    void Stop();
	
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
	 备注      :	N/A
	 *********************************************************************/
    int AnalysisTag(const char** pszTag, string& strTagName, ATTRIBUTE_LIST& vecAttribute);
	
    /********************************************************************
	 函数名称  :	TrimTwoSideSpace
	 函数描述  : 去掉两边多余的不可见字符，实际的处理函数
	 输入参数  : szOriginal --- 要处理的字符串
	 iLen       --- 要处理的字符串长度
	 输出参数  : strDest    --- 返回处理后的字符串
	 返回值    : void
	 备注      :	N/A
	 *********************************************************************/
    void TrimTwoSideSpace(const char* szOriginal, int iLen, string& strDest);
	
    /********************************************************************
	 函数名称  :	GetPureHtmlText
	 函数描述  : 获取纯Html文本
	 输入参数  : pszRawText，原始读入的文本
	 输入参数  : len，文本长度
	 输出参数  : N/A
	 返回值    : 返回去除多余空格和转义后的Utf8字符串
	 备注      :	N/A
	 *********************************************************************/
    string GetPureHtmlText(const char* pszRawText, int len);
	
    /********************************************************************
	 函数名称  :	ParseCSS
	 函数描述  : 解析CSS样式表
	 输入参数  : strDoc --- HTML文本
	 输出参数  : strDoc --- 去掉样式表后的HTML文本
	 返回值    : void
	 备注      :	N/A
	 *********************************************************************/
    void ParseCSS(string& strDoc);
	
    /********************************************************************
	 函数名称  :	ParseCSSList
	 函数描述  : 将<style>标签内的样式字符串解析成样式表结构
	 输入参数  : strDoc --- <style>与</style>标签内的字符串
	 输出参数  : void
	 返回值    : void
	 备注      :	N/A
	 *********************************************************************/
    void ParseCSSList(const string& strDoc);
	
    /********************************************************************
	 函数名称  :	ParseCSSAttributeList
	 函数描述  : 解析选择器的属性列表
	 输入参数  : strDoc --- 属性字符串
	 pStyle --- 样式表项指针
	 输出参数  : void
	 返回值    : void
	 备注      :	N/A
	 *********************************************************************/
    void ParseCSSAttributeList(const string& strDoc, CSS_STYLE* pStyle);
	
    /********************************************************************
	 函数名称  :	ClearCSSList
	 函数描述  : 清空样式表列表
	 输入参数  : N/A
	 输出参数  : void
	 返回值    : void
	 备注      :	N/A
	 *********************************************************************/
    void ClearCSSList();
	
    /********************************************************************
	 函数名称  :	IsBreakLine
	 函数描述  : 判断节点是否为block节点
	 输入参数  : 要判断的节点
	 输出参数  : void
	 返回值    : void
	 备注      :	N/A
	 *********************************************************************/
    BOOL IsBreakLine(CNode* pNode);
	
    /********************************************************************
	 函数名称  :	IsDocUtf8
	 函数描述  : 判读文档是否UTF-8格式
	 输入参数  : pszText，文档文本
	 输入参数  : iLen，文档文本长度，如果为-1，则本函数计算长度
	 输出参数  : hasBom，返回是否有BOM头
	 返回值    : 如果是Utf8编码，返回true，否则返回false
	 备注      :	N/A
	 *********************************************************************/
    bool IsDocUtf8(const char* pszText, int iLen, bool &hasBom);
	
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
    int Strnicmp(const char * first, const char * last, int count);
	
private:
    CParseFile(void);
    ~CParseFile(void);
	
    /********************************************************************
	 函数名称  :	LoadHtmlSource
	 函数描述  : 直接加载文件内容
	 输入参数  : pszSource --- 需要解析的内容
	 输出参数  : N/A
	 返回值    : void
	 备注      :	N/A
	 *********************************************************************/
    BOOL LoadHtmlSource(const char* pszSource, void* pParse);
	
public:
    BOOL m_bStopLoad;
	
    CNode*              m_pRootNode;        // 存放html树的根节点
    CNode*              m_pCurNode;         // 当前子节点
	
private:
    static CParseFile*  _instance;          // 静态单实例对象
    CSS_STYLE_LIST      m_vecCssStyle;      // 存储样式表
    CHTML*              m_pHtml;
};

#endif // _PARSEFILE_H_

