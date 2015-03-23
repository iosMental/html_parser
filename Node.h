#ifndef _NODE_H_
#define _NODE_H_


#include <string>
#include <vector>
#include "const_def.h"


//样式表节点的属性结构
typedef struct _CSS_ATTRIBUTE
{
    string strName;
    string strValue;
} CSS_ATTRIBUTE;

//样式表结构
typedef struct _CSS_STYLE
{
    string                     strSelector;    //选择器
    vector<CSS_ATTRIBUTE*>     vecAttribute;   //属性列表
	
    _CSS_STYLE()
    {
    }
	
    ~_CSS_STYLE()
    {
        vector<CSS_ATTRIBUTE*>::iterator iter = vecAttribute.begin();
        for (; iter != vecAttribute.end(); ++iter)
        {
            DELETE_OBJ(*iter);
        }
        vecAttribute.clear();
    }
	
} CSS_STYLE;

//节点属性结构
typedef struct _NODEATTRIBUTE
{
    string  strName;    //节点属性名称
    string  strValue;   //节点属性值
} NODEATTRIBUTE;

//树结构中节点的属性列表类型定义
typedef  vector<NODEATTRIBUTE*> ATTRIBUTE_LIST;
typedef  vector<CSS_STYLE*> CSS_STYLE_LIST;

//节点类
class CNode
{
    friend class CParseFile;
    friend class CHTML;
private:
	
    /********************************************************************
	 函数名称  :	CNode
	 函数描述  : 树结构节点类的构造函数
	 输入参数  : N/A
	 输出参数  : void
	 返回值    : void
	 备注      :	N/A
	 *********************************************************************/
    CNode();
    /********************************************************************
	 函数名称  :	~CNode
	 函数描述  : 树结构节点类的析构函数
	 输入参数  : N/A
	 输出参数  : void
	 返回值    : void
	 备注      :	N/A
	 *********************************************************************/
    ~CNode(void);
	
public:
    CNode*                        m_pParentNode;     // 该节点的父节点
    string                        m_strName;         //节点名称
    int                           m_iTagId;          // 标签的id
    
    BOOL                          m_bIsNested;       // 是否为列表、表格或超链接内嵌的节点
    
    BOOL                          m_bIsIgnore;       // 是否为可忽略的节点（head标签内的节点均为可忽略的节点）
	
private:
    vector<CNode*>                m_vecChildNode;    //该节点的子节点列表
    ATTRIBUTE_LIST                m_vecAttribute;    //节点的属性列表
};

#endif // _NODE_H_