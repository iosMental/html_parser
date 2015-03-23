#ifndef _CONST_DEF_H_
#define _CONST_DEF_H_


#include "WinPorting.h"

#ifndef DELETE_OBJ
#define DELETE_OBJ(s)       \
do                      \
{                       \
    if (NULL != (s))    \
    {                   \
        delete (s);     \
        (s) = NULL;     \
    }                   \
} while(0)
#endif //DELETE_OBJ


// html标签常量定义
enum TAG
{
    IGNORE,  // 定义可以忽略的节点类型
    DEFAULT,
    BODY,
    _HEAD,
    P,
    DIV,
    B,
    EM,
    I,
    SPAN,
    STRONG,
    BIG,
    SMALL,
    TT,
    CENTER,
    LABEL,
    H1,
    H2,
    H3,
    H4,
    H5,
    H6,
    U,
    SUP,
    SUB,
    STRIKE,
    FONT,
    BLINK,
    ADDRESS,
    CITE,
    DFN,
    VAR,
    BR,
    UL,
    OL,
    DL,
    LI,
    DT,
    DD,
    IMG,
    A,   // 40
    TABLE,
    CAPTION,
    TR,
    THEAD,
    TBODY,
    TFOOT,
    TD,
    TH,
    TEXT
};

// html标签名字符串常量定义 （定义的57个标签, 其中47个程序中没有支持）
#define    CONST_LABEL_HTML          "html"//NO
#define    CONST_LABEL_STYLE         "style"//NO
#define    CONST_LABEL_HEAD          "head" 
#define    CONST_LABEL_TITLE         "title"//NO
#define    CONST_LABEL_BODY          "body"
#define    CONST_LABEL_ADDRESS       "address"
#define    CONST_LABEL_BLOCKQUOTE    "blockquote"//NO
#define    CONST_LABEL_BR            "br"
#define    CONST_LABEL_CITE          "cite"
#define    CONST_LABEL_CODE          "code"//NO

#define    CONST_LABEL_DFN           "dfn"
#define    CONST_LABEL_DIV           "div"
#define    CONST_LABEL_EM            "em"
#define    CONST_LABEL_I             "i" 
#define    CONST_LABEL_H1            "h1"
#define    CONST_LABEL_H2            "h2"
#define    CONST_LABEL_H3            "h3"
#define    CONST_LABEL_H4            "h4"
#define    CONST_LABEL_H5            "h5"
#define    CONST_LABEL_H6            "h6"

#define    CONST_LABEL_U             "u"
#define    CONST_LABEL_SUP           "sup"
#define    CONST_LABEL_SUB           "sub"
#define    CONST_LABEL_STRIKE        "strike"
#define    CONST_LABEL_FONT          "font"
#define    CONST_LABEL_BLINK         "blink"
#define    CONST_LABEL_KBD           "kbd"//N
#define    CONST_LABEL_P             "p"
#define    CONST_LABEL_PRE           "pre"//N
#define    CONST_LABEL_Q             "q" //N

#define    CONST_LABEL_SAMP          "samp"//N
#define    CONST_LABEL_SPAN          "span"
#define    CONST_LABEL_STRONG        "strong"
#define    CONST_LABEL_BIG           "big"
#define    CONST_LABEL_SMALL         "small"
#define    CONST_LABEL_TT            "tt"
#define    CONST_LABEL_CENTER        "center"
#define    CONST_LABEL_LABEL         "label"
#define    CONST_LABEL_B             "b"
#define    CONST_LABEL_VAR           "var"

#define    CONST_LABEL_A             "a"
#define    CONST_LABEL_DL            "dl"
#define    CONST_LABEL_DT            "dt"
#define    CONST_LABEL_DD            "dd"
#define    CONST_LABEL_OL            "ol"
#define    CONST_LABEL_UL            "ul"
#define    CONST_LABEL_LI            "li"
#define    CONST_LABEL_CAPTION       "caption"//N
#define    CONST_LABEL_TABLE         "table"
#define    CONST_LABEL_TD            "td"

#define    CONST_LABEL_TH            "th"
#define    CONST_LABEL_TR            "tr"
#define    CONST_LABEL_THEAD         "thead"
#define    CONST_LABEL_TFOOT         "tfoot"
#define    CONST_LABEL_TBODY         "tbody"
#define    CONST_LABEL_IMG           "img"
#define    CONST_LABEL_SVGIMG        "svg:image"

// html标签属性定义 （8个属性标签）
#define    CONST_ATTRIBUTE_SRC       "src"
#define    CONST_ATTRIBUTE_XLINKHREF "xlink:href"
#define    CONST_ATTRIBUTE_WIDTH     "width"
#define    CONST_ATTRIBUTE_HEIGHT    "height"
#define    CONST_ATTRIBUTE_HREF      "href"
#define    CONST_ATTRIBUTE_BORDER     "border"
#define    CONST_ATTRIBUTE_CELLPADDING  "cellpadding"
#define    CONST_ATTRIBUTE_CELLSPACING  "cellspacing"


//++++++++++++++++++以下为OCF中的字符常量定义+++++++++++++++++++++++++++++++++++
#define CONST_OCF_LABEL_CONTAINER   "container"
#define CONST_OCF_LABEL_ROOTFILES   "rootfiles"
#define CONST_OCF_LABEL_ROOTFILE    "rootfile"
#define CONST_OCF_LABEL_PACKAGE     "package"
#define CONST_OCF_LABEL_METADATA    "metadata"
#define CONST_OCF_LABEL_MANIFEST    "manifest"
#define CONST_OCF_LABEL_SPINE       "spine"
#define CONST_OCF_LABEL_GUIDE       "guide"
#define CONST_OCF_LABEL_NCX         "ncx"
#define CONST_OCF_LABEL_DATABLOCK   "datablock"
#define CONST_OCF_LABEL_COVER       "cover"

#define CONST_OCF_LABEL_DC_TITLE    "dc:title"
#define CONST_OCF_LABEL_DC_ID       "dc:identifier"
#define CONST_OCF_LABEL_DC_LANGUAGE "dc:language"
#define CONST_OCF_LABEL_DC_CREATOR  "dc:creator"
#define CONST_OCF_LABEL_DC_PUBLISHER "dc:publisher"
#define CONST_OCF_LABEL_DC_DESCRIPTION "dc:description"
#define CONST_OCF_LABEL_DC_COVERAGE "dc:coverage"
#define CONST_OCF_LABEL_DC_SOURCE   "dc:source"
#define CONST_OCF_LABEL_DC_DATE     "dc:date"
#define CONST_OCF_LABEL_DC_RIGHTS   "dc:rights"

#define CONST_OCF_LABEL_ITEMREF     "itemref"
#define CONST_OCF_LABEL_ITEM        "item"

#define CONST_OCF_LABEL_DOC_TITLE   "docTitle"
#define CONST_OCF_LABEL_DOC_AUTHOR  "docAuthor"
#define CONST_OCF_LABEL_ROOTPATH    "rootpath"
#define CONST_OCF_LABEL_NAV_MAP     "navMap"
#define CONST_OCF_LABEL_NAV_POINT   "navPoint"
#define CONST_OCF_LABEL_NAV_LABEL   "navLabel"
#define CONST_OCF_LABEL_TEXT        "text"
#define CONST_OCF_LABEL_CONTENT     "content"

#define CONST_OCF_ATTRIB_FULL_PATH  "full-path"
#define CONST_OCF_ATTRIB_MEDIA_TYPE "media-type"
#define CONST_OCF_ATTRIB_UNIQUE_ID  "unique-identifier"
#define CONST_OCF_ATTRIB_IDREF      "idref"
#define CONST_OCF_ATTRIB_LINEAR     "linear"
#define CONST_OCF_ATTRIB_SRC        "src"
#define CONST_OCF_ATTRIB_ID         "id"
#define CONST_OCF_ATTRIB_PLAY_ORDER "playOrder"
#define CONST_OCF_ATTRIB_HREF       "href"
#define CONST_OCF_ATTRIB_BLOCK_ID   "blockid"
#define CONST_OCF_ATTRIB_WIDTH      "width"
#define CONST_OCF_ATTRIB_HEIGHT     "height"

#endif  // _CONST_DEF_H_

