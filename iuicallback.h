/********************************************************************
 文件名称 :	IUICallBack.h 头文件
 作    者 :  liutian
 创建时间 :  2011-1-16  9:00:31
 文件描述 :	界面回调接口类
 修改历史 :	2011-1-16	1.00	初始版本
 *********************************************************************/
#ifndef _IUICALLBACK_H_
#define _IUICALLBACK_H_


#include <vector>
#include "HTML.h"

class IUICallBack
{
public:
	
	/********************************************************************
	 函数名称  :	HTMLCallBackFunc
	 函数描述  : 供html解析层调用的回调函数
	 输入参数  : iType  --- 要绘制的元素类型
	 pNode  --- 保存要绘制的元素结构指针
	 输出参数  : void
	 返回值    : void
	 备注      :	N/A
	 *********************************************************************/
	virtual void HtmlCallBackFunc(CContent* pElement) = 0;
	
};

#endif  //_IUICALLBACK_H_

