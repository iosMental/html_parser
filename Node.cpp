#include "EasyLog.h"
#include "Node.h"

/********************************************************************
 函数名称  :	CNode
 函数描述  : html树结构节点类的构造函数
 输入参数  : N/A
 输出参数  : void
 返回值    : void
 备注      :	N/A
 *********************************************************************/
CNode::CNode(void)
{
    FUN_TRACE();
    m_pParentNode = NULL;
    m_bIsNested = FALSE;
    m_bIsIgnore = FALSE;
}

/********************************************************************
 函数名称  :	CNode
 函数描述  : html树结构节点类的析构函数
 输入参数  : N/A
 输出参数  : void
 返回值    : void
 备注      :	N/A
 *********************************************************************/
CNode::~CNode(void)
{
    FUN_TRACE();
    //删除节点的属性
    vector<NODEATTRIBUTE*>::iterator iter_Attribute = m_vecAttribute.begin();
    vector<NODEATTRIBUTE*>::iterator iterEnd_Attribute = m_vecAttribute.end();
    for (; iter_Attribute != iterEnd_Attribute; ++iter_Attribute)
    {
        if (NULL != (*iter_Attribute))
        {
            DELETE_OBJ(*iter_Attribute);
        }
    }
    m_vecAttribute.clear();
    //删除节点的子节点
    vector<CNode*>::iterator iter_Node = m_vecChildNode.begin();
    vector<CNode*>::iterator iterEnd_Node = m_vecChildNode.end();
    for (; iter_Node != iterEnd_Node; ++iter_Node)
    {
        if (NULL != (*iter_Node))
        {
            DELETE_OBJ(*iter_Node);
        }
    }
    m_vecChildNode.clear();
}
