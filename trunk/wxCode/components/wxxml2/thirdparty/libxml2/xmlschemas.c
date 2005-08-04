/*
 * schemas.c : implementation of the XML Schema handling and
 *             schema validity checking
 *
 * See Copyright for the status of this software.
 *
 * Daniel Veillard <veillard@redhat.com>
 */

/* 
 * TODO:
 *   - when types are redefined in includes, check that all
 *     types in the redef list are equal
 *     -> need a type equality operation.
 *   - if we don't intend to use the schema for schemas, we 
 *     need to validate all schema attributes (ref, type, name)
 *     against their types.
 *   - Eliminate item creation for: ??
 *
 * NOTES:
 *   - Elimated item creation for: <restriction>, <extension>, 
 *     <simpleContent>, <complexContent>, <list>, <union>
 *     
 */
#define IN_LIBXML
#include "libxml.h"

#ifdef LIBXML_SCHEMAS_ENABLED

#include <string.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include <libxml/hash.h>
#include <libxml/uri.h>

#include <libxml/xmlschemas.h>
#include <libxml/schemasInternals.h>
#include <libxml/xmlschemastypes.h>
#include <libxml/xmlautomata.h>
#include <libxml/xmlregexp.h>
#include <libxml/dict.h>
#ifdef LIBXML_PATTERN_ENABLED
#include <libxml/pattern.h>
#endif

/* #define DEBUG 1 */

/* #define DEBUG_CONTENT 1 */

/* #define DEBUG_TYPE 1 */

/* #define DEBUG_CONTENT_REGEXP 1 */

/* #define DEBUG_AUTOMATA 1 */

/* #define DEBUG_ATTR_VALIDATION 1 */

/* #define DEBUG_UNION_VALIDATION 1 */

/* #define DEBUG_IDC 1 */

/* #define DEBUG_INCLUDES 1 */

#define DUMP_CONTENT_MODEL


#define UNBOUNDED (1 << 30)
#define TODO 								\
    xmlGenericError(xmlGenericErrorContext,				\
	    "Unimplemented block at %s:%d\n",				\
            __FILE__, __LINE__);

#define XML_SCHEMAS_NO_NAMESPACE (const xmlChar *) "##"

/*
 * The XML Schemas namespaces
 */
static const xmlChar *xmlSchemaNs = (const xmlChar *)
    "http://www.w3.org/2001/XMLSchema";

static const xmlChar *xmlSchemaInstanceNs = (const xmlChar *)
    "http://www.w3.org/2001/XMLSchema-instance";

static const xmlChar *xmlSchemaElemDesElemDecl = (const xmlChar *)
    "Element decl.";
static const xmlChar *xmlSchemaElemDesElemRef = (const xmlChar *)
    "Element ref.";
static const xmlChar *xmlSchemaElemDesAttrDecl = (const xmlChar *)
    "Attribute decl.";
static const xmlChar *xmlSchemaElemDesAttrRef = (const xmlChar *)
    "Attribute ref.";
static const xmlChar *xmlSchemaElemDesST = (const xmlChar *)
    "simple type";
static const xmlChar *xmlSchemaElemDesCT = (const xmlChar *)
    "complex type";
static const xmlChar *xmlSchemaElemModelGrDef = (const xmlChar *)
    "Model group";
#if 0
static const xmlChar *xmlSchemaElemModelGrRef = (const xmlChar *)
    "Model group ref.";
#endif

#define IS_SCHEMA(node, type)						\
   ((node != NULL) && (node->ns != NULL) &&				\
    (xmlStrEqual(node->name, (const xmlChar *) type)) &&		\
    (xmlStrEqual(node->ns->href, xmlSchemaNs)))

#define FREE_AND_NULL(str)						\
    if (str != NULL) {							\
	xmlFree(str);							\
	str = NULL;							\
    }

#define IS_ANYTYPE(item)                           \
     ((item->type == XML_SCHEMA_TYPE_BASIC) &&     \
      (item->builtInType == XML_SCHEMAS_ANYTYPE))

#define IS_COMPLEX_TYPE(item)                      \
    ((item->type == XML_SCHEMA_TYPE_COMPLEX) ||    \
     (item->builtInType == XML_SCHEMAS_ANYTYPE))

#define IS_SIMPLE_TYPE(item)                       \
    ((item->type == XML_SCHEMA_TYPE_SIMPLE) ||     \
     ((item->type == XML_SCHEMA_TYPE_BASIC) &&     \
      (item->builtInType != XML_SCHEMAS_ANYTYPE))) 

#define IS_ANY_SIMPLE_TYPE(item)                   \
    ((item->type == XML_SCHEMA_TYPE_BASIC) &&      \
      (item->builtInType == XML_SCHEMAS_ANYSIMPLETYPE)) 

#define IS_NOT_TYPEFIXED(item)                      \
    ((item->type != XML_SCHEMA_TYPE_BASIC) &&       \
     ((item->flags & XML_SCHEMAS_TYPE_INTERNAL_RESOLVED) == 0))

#define HAS_COMPLEX_CONTENT(item)			 \
    ((item->contentType == XML_SCHEMA_CONTENT_MIXED) ||  \
     (item->contentType == XML_SCHEMA_CONTENT_EMPTY) ||  \
     (item->contentType == XML_SCHEMA_CONTENT_ELEMENTS))

#define GET_NODE(item) xmlSchemaGetComponentNode((xmlSchemaBasicItemPtr) item)

#define IS_MODEL_GROUP(item)                     \
    ((item->type == XML_SCHEMA_TYPE_SEQUENCE) || \
     (item->type == XML_SCHEMA_TYPE_CHOICE) ||   \
     (item->type == XML_SCHEMA_TYPE_ALL))

#if 0
#define WXS_GET_NEXT(item) xmlSchemaGetNextComponent((xmlSchemaBasicItemPtr) item)
#endif

/*
#define XML_SCHEMAS_VAL_WTSP_PRESERVE 0
#define XML_SCHEMAS_VAL_WTSP_REPLACE  1
#define XML_SCHEMAS_VAL_WTSP_COLLAPSE 2
*/

#define XML_SCHEMAS_PARSE_ERROR		1

#define SCHEMAS_PARSE_OPTIONS XML_PARSE_NOENT


/*
* XML_SCHEMA_VAL_LOCATE_BY_NSNAME = 1<<2
* locate schemata to be imported
* using the namespace name; otherwise
* the location URI will be used */

/*
* xmlSchemaParserOption:
*
* This is the set of XML Schema parser options.
*
typedef enum {
    XML_SCHEMA_PAR_LOCATE_BY_NSNAME	= 1<<0
	* locate schemata to be imported
	* using the namespace name; otherwise
	* the location URI will be used *
} xmlSchemaParserOption;
*/

typedef struct _xmlSchemaItemList xmlSchemaAssemble;
typedef xmlSchemaAssemble *xmlSchemaAssemblePtr;

typedef struct _xmlSchemaItemList xmlSchemaItemList;
typedef xmlSchemaItemList *xmlSchemaItemListPtr;

struct _xmlSchemaItemList {
    void **items;  /* used for dynamic addition of schemata */
    int nbItems; /* used for dynamic addition of schemata */
    int sizeItems; /* used for dynamic addition of schemata */
};

struct _xmlSchemaParserCtxt {
    void *userData;             /* user specific data block */
    xmlSchemaValidityErrorFunc error;   /* the callback in case of errors */
    xmlSchemaValidityWarningFunc warning;       /* the callback in case of warning */
    xmlSchemaValidError err;
    int nberrors;
    xmlStructuredErrorFunc serror;

    xmlSchemaPtr topschema;	/* The main schema */
    xmlHashTablePtr namespaces;	/* Hash table of namespaces to schemas */

    xmlSchemaPtr schema;        /* The schema in use */
    const xmlChar *container;   /* the current element, group, ... */
    int counter;

    const xmlChar *URL;
    xmlDocPtr doc;
    int preserve;		/* Whether the doc should be freed  */

    const char *buffer;
    int size;

    /*
     * Used to build complex element content models
     */
    xmlAutomataPtr am;
    xmlAutomataStatePtr start;
    xmlAutomataStatePtr end;
    xmlAutomataStatePtr state;

    xmlDictPtr dict;		/* dictionnary for interned string names */
    int        includes;	/* the inclusion level, 0 for root or imports */
    xmlSchemaTypePtr ctxtType; /* The current context simple/complex type */
    xmlSchemaTypePtr parentItem; /* The current parent schema item */
    xmlSchemaAssemblePtr assemble;
    int options;
    xmlSchemaValidCtxtPtr vctxt;
    const xmlChar **localImports; /* list of locally imported namespaces */
    int sizeLocalImports;
    int nbLocalImports;
};


#define XML_SCHEMAS_ATTR_UNKNOWN 1
#define XML_SCHEMAS_ATTR_CHECKED 2
#define XML_SCHEMAS_ATTR_PROHIBITED 3
#define XML_SCHEMAS_ATTR_MISSING 4
#define XML_SCHEMAS_ATTR_INVALID_VALUE 5
#define XML_SCHEMAS_ATTR_TYPE_NOT_RESOLVED 6
#define XML_SCHEMAS_ATTR_INVALID_FIXED_VALUE 7
#define XML_SCHEMAS_ATTR_DEFAULT 8
#define XML_SCHEMAS_ATTR_VALIDATE_VALUE 9
#define XML_SCHEMAS_ATTR_WILD_NO_DECL 10

typedef struct _xmlSchemaAttrState xmlSchemaAttrState;
typedef xmlSchemaAttrState *xmlSchemaAttrStatePtr;
struct _xmlSchemaAttrState {
    xmlSchemaAttrStatePtr next;
    xmlAttrPtr attr;
    int state;
    xmlSchemaAttributePtr decl;
    const xmlChar *value;
};

/**
 * xmlSchemaBasicItem:
 *
 * The abstract base type for schema components.
 */
typedef struct _xmlSchemaBasicItem xmlSchemaBasicItem;
typedef xmlSchemaBasicItem *xmlSchemaBasicItemPtr;
struct _xmlSchemaBasicItem {
    xmlSchemaTypeType type;
};

/**
 * xmlSchemaAnnotItem:
 *
 * The abstract base type for annotated schema components.
 * (Extends xmlSchemaBasicItem)
 */
typedef struct _xmlSchemaAnnotItem xmlSchemaAnnotItem;
typedef xmlSchemaAnnotItem *xmlSchemaAnnotItemPtr;
struct _xmlSchemaAnnotItem {
    xmlSchemaTypeType type;
    xmlSchemaAnnotPtr annot;
};

/**
 * xmlSchemaTreeItem:
 *
 * The abstract base type for tree-like structured schema components.
 * (Extends xmlSchemaAnnotItem)
 */
typedef struct _xmlSchemaTreeItem xmlSchemaTreeItem;
typedef xmlSchemaTreeItem *xmlSchemaTreeItemPtr;
struct _xmlSchemaTreeItem {
    xmlSchemaTypeType type;
    xmlSchemaAnnotPtr annot;
    xmlSchemaTreeItemPtr next;
    xmlSchemaTreeItemPtr children;
};
    
/**
 * xmlSchemaQNameRef:
 *
 * A component reference item (not a schema component)
 * (Extends xmlSchemaBasicItem)
 */
typedef struct _xmlSchemaQNameRef xmlSchemaQNameRef;
typedef xmlSchemaQNameRef *xmlSchemaQNameRefPtr;
struct _xmlSchemaQNameRef {
    xmlSchemaTypeType type;
    xmlSchemaBasicItemPtr item;
    xmlSchemaTypeType itemType;
    const xmlChar *name;
    const xmlChar *targetNamespace;
};

/**
 * xmlSchemaParticle:
 *
 * A particle component.
 * (Extends xmlSchemaTreeItem)
 */
typedef struct _xmlSchemaParticle xmlSchemaParticle;
typedef xmlSchemaParticle *xmlSchemaParticlePtr;
struct _xmlSchemaParticle {
    xmlSchemaTypeType type;
    xmlSchemaAnnotPtr annot;
    xmlSchemaTreeItemPtr next; /* next particle (OR "element decl" OR "wildcard") */
    xmlSchemaTreeItemPtr children; /* the "term" ("model group" OR "group definition") */
    int minOccurs;
    int maxOccurs;  
    xmlNodePtr node;
};

/**
 * xmlSchemaModelGroup:
 *
 * A model group component.
 * (Extends xmlSchemaTreeItem)
 */
typedef struct _xmlSchemaModelGroup xmlSchemaModelGroup;
typedef xmlSchemaModelGroup *xmlSchemaModelGroupPtr;
struct _xmlSchemaModelGroup {
    xmlSchemaTypeType type; /* XML_SCHEMA_TYPE_SEQUENCE, XML_SCHEMA_TYPE_CHOICE, XML_SCHEMA_TYPE_ALL */
    xmlSchemaAnnotPtr annot;
    xmlSchemaTreeItemPtr next; /* not used */
    xmlSchemaTreeItemPtr children; /* first particle (OR "element decl" OR "wildcard") */
    xmlNodePtr node;
};

/**
 * xmlSchemaModelGroupDef:
 *
 * A model group definition component.
 * (Extends xmlSchemaTreeItem)
 */
typedef struct _xmlSchemaModelGroupDef xmlSchemaModelGroupDef;
typedef xmlSchemaModelGroupDef *xmlSchemaModelGroupDefPtr;
struct _xmlSchemaModelGroupDef {
    xmlSchemaTypeType type; /* XML_SCHEMA_TYPE_GROUP */
    xmlSchemaAnnotPtr annot;
    xmlSchemaTreeItemPtr next; /* not used */
    xmlSchemaTreeItemPtr children; /* the "model group" */
    const xmlChar *name;
    const xmlChar *targetNamespace;
    xmlNodePtr node;
};

typedef struct _xmlSchemaIDC xmlSchemaIDC;
typedef xmlSchemaIDC *xmlSchemaIDCPtr;

/**
 * xmlSchemaIDCSelect:
 *
 * The identity-constraint "field" and "selector" item, holding the
 * XPath expression.
 */
typedef struct _xmlSchemaIDCSelect xmlSchemaIDCSelect;
typedef xmlSchemaIDCSelect *xmlSchemaIDCSelectPtr;
struct _xmlSchemaIDCSelect {    
    xmlSchemaIDCSelectPtr next;
    xmlSchemaIDCPtr idc;
    int index; /* an index position if significant for IDC key-sequences */
    const xmlChar *xpath; /* the XPath expression */
    void *xpathComp; /* the compiled XPath expression */
};

/**
 * xmlSchemaIDC:
 *
 * The identity-constraint definition component.
 * (Extends xmlSchemaAnnotItem)
 */

struct _xmlSchemaIDC {
    xmlSchemaTypeType type;
    xmlSchemaAnnotPtr annot;
    xmlSchemaIDCPtr next;
    xmlNodePtr node;
    const xmlChar *name;    
    const xmlChar *targetNamespace;
    xmlSchemaIDCSelectPtr selector;
    xmlSchemaIDCSelectPtr fields;
    int nbFields;
    xmlSchemaQNameRefPtr ref;
};

/**
 * xmlSchemaIDCAug:
 *
 * The augmented IDC information used for validation.
 */
typedef struct _xmlSchemaIDCAug xmlSchemaIDCAug;
typedef xmlSchemaIDCAug *xmlSchemaIDCAugPtr;
struct _xmlSchemaIDCAug {
    xmlSchemaIDCAugPtr next; /* next in a list */
    xmlSchemaIDCPtr def; /* the IDC definition */
    int bubbleDepth; /* the lowest tree level to which IDC 
                        tables need to be bubbled upwards */
};

/**
 * xmlSchemaPSVIIDCKeySequence:
 *
 * The key sequence of a node table item.
 */
typedef struct _xmlSchemaPSVIIDCKey xmlSchemaPSVIIDCKey;
typedef xmlSchemaPSVIIDCKey *xmlSchemaPSVIIDCKeyPtr;
struct _xmlSchemaPSVIIDCKey {
    xmlSchemaTypePtr type;
    xmlSchemaValPtr compValue;
};

/**
 * xmlSchemaPSVIIDCNode:
 *
 * The node table item of a node table.
 */
typedef struct _xmlSchemaPSVIIDCNode xmlSchemaPSVIIDCNode;
typedef xmlSchemaPSVIIDCNode *xmlSchemaPSVIIDCNodePtr;
struct _xmlSchemaPSVIIDCNode {
    xmlNodePtr node;
    xmlSchemaPSVIIDCKeyPtr *keys;
};

/**
 * xmlSchemaPSVIIDCBinding:
 *
 * The identity-constraint binding item of the [identity-constraint table].
 */
typedef struct _xmlSchemaPSVIIDCBinding xmlSchemaPSVIIDCBinding;
typedef xmlSchemaPSVIIDCBinding *xmlSchemaPSVIIDCBindingPtr;
struct _xmlSchemaPSVIIDCBinding {
    xmlSchemaPSVIIDCBindingPtr next; /* next binding of a specific node */
    xmlSchemaIDCPtr definition; /* the IDC definition */
    xmlSchemaPSVIIDCNodePtr *nodeTable; /* array of key-sequences */
    int nbNodes; /* number of entries in the node table */
    int sizeNodes; /* size of the node table */
    int nbDupls; /* number of already identified duplicates in the node 
                    table */
    /* int nbKeys; number of keys in each key-sequence */
};

#define XPATH_STATE_OBJ_TYPE_IDC_SELECTOR 1
#define XPATH_STATE_OBJ_TYPE_IDC_FIELD 2

#define XPATH_STATE_OBJ_MATCHES -2
#define XPATH_STATE_OBJ_BLOCKED -3

typedef struct _xmlSchemaIDCMatcher xmlSchemaIDCMatcher;
typedef xmlSchemaIDCMatcher *xmlSchemaIDCMatcherPtr;

/**
 * xmlSchemaIDCStateObj:
 *
 * The state object used to evaluate XPath expressions.
 */
typedef struct _xmlSchemaIDCStateObj xmlSchemaIDCStateObj;
typedef xmlSchemaIDCStateObj *xmlSchemaIDCStateObjPtr;
struct _xmlSchemaIDCStateObj {
    int type;    
    xmlSchemaIDCStateObjPtr next; /* next if in a list */
    int depth; /* depth of creation */
    int *history; /* list of (depth, state-id) tuples */
    int nbHistory;
    int sizeHistory;
    xmlSchemaIDCMatcherPtr matcher; /* the correspondent field/selector
                                       matcher */
    xmlSchemaIDCSelectPtr sel;
    void *xpathCtxt;
};

#define IDC_MATCHER 0

/**
 * xmlSchemaIDCMatcher:
 *
 * Used to  IDC selectors (and fields) successively.
 */
struct _xmlSchemaIDCMatcher {
    int type;
    int depth; /* the tree depth at creation time */
    xmlSchemaIDCMatcherPtr next; /* next in the list */
    xmlSchemaIDCAugPtr aidc; /* the augmented IDC item */
    xmlSchemaPSVIIDCKeyPtr **keySeqs; /* the key-sequences of the target
                                         elements */
    int sizeKeySeqs;
    int targetDepth;
};

/*
* Element info flags.
*/
#define XML_SCHEMA_ELEM_INFO_VALUE_NEEDED 1
/* #define XML_SCHEMA_ELEM_INFO_ATTR 2 */
/* #define XML_SCHEMA_ELEM_INFO_ELEM 4 */

/**
 * xmlSchemaNodeInfo:
 *
 * Holds information of an element node.
 */
typedef struct _xmlSchemaNodeInfo xmlSchemaNodeInfo;
typedef xmlSchemaNodeInfo *xmlSchemaNodeInfoPtr;
struct _xmlSchemaNodeInfo {
    int depth;
    int flags; /* combination of node info flags */
    xmlNodePtr node;
    const xmlChar *localName;
    const xmlChar *namespaceName;
    xmlSchemaTypePtr typeDef; /* the complex/simple type definition if any */
    xmlSchemaTypePtr decl; /* the element/attribute declaration */
    xmlSchemaValPtr value; /* the pre-computed value if any */
    xmlSchemaPSVIIDCBindingPtr idcTable; /* the table of PSVI IDC bindings
                                            for the scope element*/
    xmlSchemaIDCMatcherPtr idcMatchers; /* the IDC matchers for the scope
                                           element */
};

#define XML_SCHEMA_VALID_INVALID_NEG_WILDCARD 1<<0

/**
 * xmlSchemaValidCtxt:
 *
 * A Schemas validation context
 */
struct _xmlSchemaValidCtxt {
    void *userData;             /* user specific data block */
    xmlSchemaValidityErrorFunc error;   /* the callback in case of errors */
    xmlSchemaValidityWarningFunc warning; /* the callback in case of warning */
    xmlStructuredErrorFunc serror;

    xmlSchemaPtr schema;        /* The schema in use */
    xmlDocPtr doc;
    xmlParserInputBufferPtr input;
    xmlCharEncoding enc;
    xmlSAXHandlerPtr sax;
    void *user_data;

    xmlDocPtr myDoc;
    int err;
    int nberrors;

    xmlNodePtr node;
    xmlNodePtr cur;
    xmlSchemaTypePtr type;

    xmlRegExecCtxtPtr regexp;
    xmlSchemaValPtr value;

    xmlSchemaAttrStatePtr attrTop;
    xmlSchemaAttrStatePtr attr;
    /* xmlNodePtr scope; not used */
    int valueWS;
    int options;
    xmlNodePtr validationRoot;    
    xmlSchemaParserCtxtPtr pctxt;
    int xsiAssemble;

    int depth;
    xmlSchemaNodeInfoPtr *elemInfos; /* array of element informations */
    int sizeElemInfos;
    xmlSchemaNodeInfoPtr nodeInfo; /* the current element information */
    xmlSchemaNodeInfoPtr attrInfo; /* node infor for the current attribute */

    xmlSchemaIDCAugPtr aidcs; /* a list of augmented IDC informations */

    xmlSchemaIDCStateObjPtr xpathStates; /* first active state object. */
    xmlSchemaIDCStateObjPtr xpathStatePool; /* first stored state object. */
    
    xmlSchemaPSVIIDCNodePtr *idcNodes; /* list of all IDC node-table entries*/
    int nbIdcNodes;
    int sizeIdcNodes;

    xmlSchemaPSVIIDCKeyPtr *idcKeys; /* list of all IDC node-table entries */
    int nbIdcKeys;
    int sizeIdcKeys;

    int flags;
};

/*
 * These are the entries in the schemas importSchemas hash table
 */
typedef struct _xmlSchemaImport xmlSchemaImport;
typedef xmlSchemaImport *xmlSchemaImportPtr;
struct _xmlSchemaImport {
    const xmlChar *schemaLocation;
    xmlSchemaPtr schema; /* not used any more */
    xmlDocPtr doc;
    int isMain;
};

/*
 * These are the entries associated to includes in a schemas
 */
typedef struct _xmlSchemaInclude xmlSchemaInclude;
typedef xmlSchemaInclude *xmlSchemaIncludePtr;
struct _xmlSchemaInclude {
    xmlSchemaIncludePtr next;
    const xmlChar *schemaLocation;
    xmlDocPtr doc;
    const xmlChar *origTargetNamespace;
    const xmlChar *targetNamespace;
};

/************************************************************************
 * 									*
 * 			Some predeclarations				*
 * 									*
 ************************************************************************/

static int xmlSchemaParseInclude(xmlSchemaParserCtxtPtr ctxt,
                                 xmlSchemaPtr schema,
                                 xmlNodePtr node);
static void
xmlSchemaTypeFixup(xmlSchemaTypePtr typeDecl,
                   xmlSchemaParserCtxtPtr ctxt, const xmlChar * name);
static const xmlChar *
xmlSchemaFacetTypeToString(xmlSchemaTypeType type);
static int
xmlSchemaValidateSimpleTypeValue(xmlSchemaValidCtxtPtr ctxt, 
				 xmlSchemaTypePtr type,
				 const xmlChar *value,
				 int fireErrors,
				 int applyFacets,
				 int normalize,
				 int checkNodes);
static int
xmlSchemaValidateElementByDeclaration(xmlSchemaValidCtxtPtr ctxt,
				      xmlSchemaElementPtr elemDecl); 
static int
xmlSchemaValidateElementByWildcard(xmlSchemaValidCtxtPtr ctxt,
				   xmlSchemaWildcardPtr wild);
static int
xmlSchemaHasElemOrCharContent(xmlNodePtr node);
static int
xmlSchemaParseImport(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                     xmlNodePtr node);
static void
xmlSchemaCheckFacetValues(xmlSchemaTypePtr typeDecl,
                       xmlSchemaParserCtxtPtr ctxt);
static void
xmlSchemaClearValidCtxt(xmlSchemaValidCtxtPtr vctxt);
static int
xmlSchemaPostCreateVal(xmlSchemaValidCtxtPtr vctxt,
		       xmlSchemaTypePtr type,
		       const xmlChar *value,
		       xmlSchemaValPtr *val);
static xmlSchemaTypePtr
xmlSchemaGetSimpleContentType(xmlSchemaTypePtr complexType);
static int
xmlSchemaGetWhiteSpaceFacetValue(xmlSchemaTypePtr type);
static xmlSchemaTreeItemPtr
xmlSchemaParseModelGroup(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
			 xmlNodePtr node, xmlSchemaTypeType type,
			 int withParticle);
static const xmlChar *
xmlSchemaCompTypeToString(xmlSchemaTypeType type);
static xmlSchemaTypeLinkPtr
xmlSchemaGetUnionSimpleTypeMemberTypes(xmlSchemaTypePtr type);

/************************************************************************
 *									*
 * 			Datatype error handlers				*
 *									*
 ************************************************************************/

/**
 * xmlSchemaPErrMemory:
 * @node: a context node
 * @extra:  extra informations
 *
 * Handle an out of memory condition
 */
static void
xmlSchemaPErrMemory(xmlSchemaParserCtxtPtr ctxt,
                    const char *extra, xmlNodePtr node)
{
    if (ctxt != NULL)
        ctxt->nberrors++;
    __xmlSimpleError(XML_FROM_SCHEMASP, XML_ERR_NO_MEMORY, node, NULL,
                     extra);
}

/**
 * xmlSchemaPErr:
 * @ctxt: the parsing context
 * @node: the context node
 * @error: the error code
 * @msg: the error message
 * @str1: extra data
 * @str2: extra data
 * 
 * Handle a parser error
 */
static void
xmlSchemaPErr(xmlSchemaParserCtxtPtr ctxt, xmlNodePtr node, int error,
              const char *msg, const xmlChar * str1, const xmlChar * str2)
{
    xmlGenericErrorFunc channel = NULL;
    xmlStructuredErrorFunc schannel = NULL;
    void *data = NULL;

    if (ctxt != NULL) {
        ctxt->nberrors++;
        channel = ctxt->error;
        data = ctxt->userData;
	schannel = ctxt->serror;
    }
    __xmlRaiseError(schannel, channel, data, ctxt, node, XML_FROM_SCHEMASP,
                    error, XML_ERR_ERROR, NULL, 0,
                    (const char *) str1, (const char *) str2, NULL, 0, 0,
                    msg, str1, str2);
}

/**
 * xmlSchemaPErr2:
 * @ctxt: the parsing context
 * @node: the context node
 * @node: the current child
 * @error: the error code
 * @msg: the error message
 * @str1: extra data
 * @str2: extra data
 * 
 * Handle a parser error
 */
static void
xmlSchemaPErr2(xmlSchemaParserCtxtPtr ctxt, xmlNodePtr node,
               xmlNodePtr child, int error,
               const char *msg, const xmlChar * str1, const xmlChar * str2)
{
    if (child != NULL)
        xmlSchemaPErr(ctxt, child, error, msg, str1, str2);
    else
        xmlSchemaPErr(ctxt, node, error, msg, str1, str2);
}


/**
 * xmlSchemaPErrExt:
 * @ctxt: the parsing context
 * @node: the context node
 * @error: the error code 
 * @strData1: extra data
 * @strData2: extra data
 * @strData3: extra data
 * @msg: the message
 * @str1:  extra parameter for the message display
 * @str2:  extra parameter for the message display
 * @str3:  extra parameter for the message display
 * @str4:  extra parameter for the message display
 * @str5:  extra parameter for the message display
 * 
 * Handle a parser error
 */
static void
xmlSchemaPErrExt(xmlSchemaParserCtxtPtr ctxt, xmlNodePtr node, int error,
		const xmlChar * strData1, const xmlChar * strData2, 
		const xmlChar * strData3, const char *msg, const xmlChar * str1, 
		const xmlChar * str2, const xmlChar * str3, const xmlChar * str4,
		const xmlChar * str5)
{

    xmlGenericErrorFunc channel = NULL;
    xmlStructuredErrorFunc schannel = NULL;
    void *data = NULL;

    if (ctxt != NULL) {
        ctxt->nberrors++;
        channel = ctxt->error;
        data = ctxt->userData;
	schannel = ctxt->serror;
    }
    __xmlRaiseError(schannel, channel, data, ctxt, node, XML_FROM_SCHEMASP,
                    error, XML_ERR_ERROR, NULL, 0,
                    (const char *) strData1, (const char *) strData2, 
		    (const char *) strData3, 0, 0, msg, str1, str2, 
		    str3, str4, str5);
}


/**
 * xmlSchemaVTypeErrMemory:
 * @node: a context node
 * @extra:  extra informations
 *
 * Handle an out of memory condition
 */
static void
xmlSchemaVErrMemory(xmlSchemaValidCtxtPtr ctxt,
                    const char *extra, xmlNodePtr node)
{
    if (ctxt != NULL) {
        ctxt->nberrors++;
        ctxt->err = XML_SCHEMAV_INTERNAL;
    }
    __xmlSimpleError(XML_FROM_SCHEMASV, XML_ERR_NO_MEMORY, node, NULL,
                     extra);
}

/**
 * xmlSchemaVErr3:
 * @ctxt: the validation context
 * @node: the context node
 * @error: the error code
 * @msg: the error message
 * @str1: extra data
 * @str2: extra data
 * @str3: extra data
 * 
 * Handle a validation error
 */
static void
xmlSchemaVErr3(xmlSchemaValidCtxtPtr ctxt, xmlNodePtr node, int error,
               const char *msg, const xmlChar *str1, const xmlChar *str2,
	       const xmlChar *str3)
{
    xmlStructuredErrorFunc schannel = NULL;
    xmlGenericErrorFunc channel = NULL;
    void *data = NULL;

    if (ctxt != NULL) {
        ctxt->nberrors++;
	ctxt->err = error;
        channel = ctxt->error;
        schannel = ctxt->serror;
        data = ctxt->userData;
    }
    /* reajust to global error numbers */
    /* Removed, since the old schema error codes have been 
    * substituted for the global error codes.
    *
    * error += XML_SCHEMAV_NOROOT - XML_SCHEMAS_ERR_NOROOT; 
    */
    __xmlRaiseError(schannel, channel, data, ctxt, node, XML_FROM_SCHEMASV,
                    error, XML_ERR_ERROR, NULL, 0,
                    (const char *) str1, (const char *) str2,
		    (const char *) str3, 0, 0,
                    msg, str1, str2, str3);
}

/**
 * xmlSchemaVErrExt:
 * @ctxt: the validation context
 * @node: the context node
 * @error: the error code 
 * @msg: the message
 * @str1:  extra parameter for the message display
 * @str2:  extra parameter for the message display
 * @str3:  extra parameter for the message display
 * @str4:  extra parameter for the message display
 * @str5:  extra parameter for the message display
 * 
 * Handle a validation error
 */
static void
xmlSchemaVErrExt(xmlSchemaValidCtxtPtr ctxt, xmlNodePtr node, int error,
		 const char *msg, const xmlChar * str1, 
		 const xmlChar * str2, const xmlChar * str3, 
		 const xmlChar * str4, const xmlChar * str5)
{
    xmlStructuredErrorFunc schannel = NULL;
    xmlGenericErrorFunc channel = NULL;
    void *data = NULL;

    if (ctxt != NULL) {
        ctxt->nberrors++;
	ctxt->err = error;
        channel = ctxt->error;
        schannel = ctxt->serror;
        data = ctxt->userData;
    }
    /* reajust to global error numbers */
     /* Removed, since the old schema error codes have been 
    * substituted for the global error codes.
    *
    * error += XML_SCHEMAV_NOROOT - XML_SCHEMAS_ERR_NOROOT;
    */
    __xmlRaiseError(schannel, channel, data, ctxt, node, XML_FROM_SCHEMASP,
                    error, XML_ERR_ERROR, NULL, 0, NULL, NULL, NULL, 0, 0, 
		    msg, str1, str2, str3, str4, str5);
}
/**
 * xmlSchemaVErr:
 * @ctxt: the validation context
 * @node: the context node
 * @error: the error code
 * @msg: the error message
 * @str1: extra data
 * @str2: extra data
 * 
 * Handle a validation error
 */
static void
xmlSchemaVErr(xmlSchemaValidCtxtPtr ctxt, xmlNodePtr node, int error,
              const char *msg, const xmlChar * str1, const xmlChar * str2)
{
    xmlStructuredErrorFunc schannel = NULL;
    xmlGenericErrorFunc channel = NULL;
    void *data = NULL;

    if (ctxt != NULL) {
        ctxt->nberrors++;
	ctxt->err = error;
        channel = ctxt->error;
        data = ctxt->userData;
        schannel = ctxt->serror;
    }
    /* reajust to global error numbers */
    /* Removed, since the old schema error codes have been 
    * substituted for the global error codes.
    *
    * error += XML_SCHEMAV_NOROOT - XML_SCHEMAS_ERR_NOROOT;
    */
    __xmlRaiseError(schannel, channel, data, ctxt, node, XML_FROM_SCHEMASV,
                    error, XML_ERR_ERROR, NULL, 0,
                    (const char *) str1, (const char *) str2, NULL, 0, 0,
                    msg, str1, str2);
}

/**
 * xmlSchemaGetComponentNode:
 * @item: a schema component
 *
 * Returns node associated with the schema component.
 * NOTE that such a node need not be available; plus, a component's
 * node need not to reflect the component directly, since there is no
 * one-to-one relationship between the XML Schema representation and
 * the component representation.
 */
static xmlNodePtr
xmlSchemaGetComponentNode(xmlSchemaBasicItemPtr item)
{
    switch (item->type) {
	case XML_SCHEMA_TYPE_ELEMENT:
	    return (((xmlSchemaElementPtr) item)->node);
	case XML_SCHEMA_TYPE_ATTRIBUTE:
	    return (((xmlSchemaAttributePtr) item)->node);
	case XML_SCHEMA_TYPE_COMPLEX:
	case XML_SCHEMA_TYPE_SIMPLE:
	    return (((xmlSchemaTypePtr) item)->node);
	case XML_SCHEMA_TYPE_ANY:
	case XML_SCHEMA_TYPE_ANY_ATTRIBUTE:
	    return (((xmlSchemaWildcardPtr) item)->node);
	case XML_SCHEMA_TYPE_PARTICLE:
	    return (((xmlSchemaParticlePtr) item)->node);
	case XML_SCHEMA_TYPE_SEQUENCE:
	case XML_SCHEMA_TYPE_CHOICE:
	case XML_SCHEMA_TYPE_ALL:
	    return (((xmlSchemaModelGroupPtr) item)->node);
	case XML_SCHEMA_TYPE_GROUP:
	    return (((xmlSchemaModelGroupDefPtr) item)->node);
	case XML_SCHEMA_TYPE_ATTRIBUTEGROUP:
	    return (((xmlSchemaAttributeGroupPtr) item)->node);
	case XML_SCHEMA_TYPE_IDC_UNIQUE:
	case XML_SCHEMA_TYPE_IDC_KEY:
	case XML_SCHEMA_TYPE_IDC_KEYREF:
	    return (((xmlSchemaIDCPtr) item)->node);
	default:
	    return (NULL);
    }     
}

#if 0
/**
 * xmlSchemaGetNextComponent:
 * @item: a schema component
 *
 * Returns the next sibling of the schema component.
 */
static xmlSchemaBasicItemPtr
xmlSchemaGetNextComponent(xmlSchemaBasicItemPtr item)
{
    switch (item->type) {
	case XML_SCHEMA_TYPE_ELEMENT:
	    return ((xmlSchemaBasicItemPtr) ((xmlSchemaElementPtr) item)->next);
	case XML_SCHEMA_TYPE_ATTRIBUTE:
	    return ((xmlSchemaBasicItemPtr) ((xmlSchemaAttributePtr) item)->next);
	case XML_SCHEMA_TYPE_COMPLEX:
	case XML_SCHEMA_TYPE_SIMPLE:
	    return ((xmlSchemaBasicItemPtr) ((xmlSchemaTypePtr) item)->next);
	case XML_SCHEMA_TYPE_ANY:
	case XML_SCHEMA_TYPE_ANY_ATTRIBUTE:
	    return (NULL);
	case XML_SCHEMA_TYPE_PARTICLE:
	    return ((xmlSchemaBasicItemPtr) ((xmlSchemaParticlePtr) item)->next);
	case XML_SCHEMA_TYPE_SEQUENCE:
	case XML_SCHEMA_TYPE_CHOICE:
	case XML_SCHEMA_TYPE_ALL:
	    return (NULL);
	case XML_SCHEMA_TYPE_GROUP:
	    return (NULL);
	case XML_SCHEMA_TYPE_ATTRIBUTEGROUP:
	    return ((xmlSchemaBasicItemPtr) ((xmlSchemaAttributeGroupPtr) item)->next);
	case XML_SCHEMA_TYPE_IDC_UNIQUE:
	case XML_SCHEMA_TYPE_IDC_KEY:
	case XML_SCHEMA_TYPE_IDC_KEYREF:
	    return ((xmlSchemaBasicItemPtr) ((xmlSchemaIDCPtr) item)->next);
	default:
	    return (NULL);
    }     
}
#endif

/**
 * xmlSchemaGetAttrName:
 * @attr:  the attribute declaration/use
 *
 * Returns the name of the attribute; if the attribute
 * is a reference, the name of the referenced global type will be returned.
 */
static const xmlChar *
xmlSchemaGetAttrName(xmlSchemaAttributePtr attr) 
{
    if (attr->ref != NULL) 
	return(attr->ref);
    else
	return(attr->name);	
}

/**
 * xmlSchemaGetAttrTargetNsURI:
 * @type:  the type (element or attribute)
 *
 * Returns the target namespace URI of the type; if the type is a reference,
 * the target namespace of the referenced type will be returned.
 */
static const xmlChar *
xmlSchemaGetAttrTargetNsURI(xmlSchemaAttributePtr attr)
{  
    if (attr->ref != NULL)
	return (attr->refNs);
    else
	return(attr->targetNamespace);  
}

/**
 * xmlSchemaFormatNsUriLocal:
 * @buf: the string buffer
 * @uri:  the namespace URI
 * @local: the local name
 *
 * Returns a representation of the given URI used
 * for error reports.
 *
 * Returns an empty string, if @ns is NULL, a formatted
 * string otherwise.
 */  
static const xmlChar*   
xmlSchemaFormatNsUriLocal(xmlChar **buf,
			   const xmlChar *uri, const xmlChar *local)
{
    if (*buf != NULL)
	xmlFree(*buf);
    if (uri == NULL) {
	*buf = xmlStrdup(BAD_CAST "{'");
	*buf = xmlStrcat(*buf, local);
    } else {
	*buf = xmlStrdup(BAD_CAST "{'");
	*buf = xmlStrcat(*buf, uri);
	*buf = xmlStrcat(*buf, BAD_CAST "', '");
	*buf = xmlStrcat(*buf, local);	
    }
    *buf = xmlStrcat(*buf, BAD_CAST "'}");
    return ((const xmlChar *) *buf);
}

/**
 * xmlSchemaFormatNsPrefixLocal:
 * @buf: the string buffer
 * @ns:  the namespace
 * @local: the local name
 *
 * Returns a representation of the given URI used
 * for error reports.
 *
 * Returns an empty string, if @ns is NULL, a formatted
 * string otherwise.
 */  
static const xmlChar*   
xmlSchemaFormatNsPrefixLocal(xmlChar **buf,
			      xmlNsPtr ns, const xmlChar *local)
{
    if (*buf != NULL) {
	xmlFree(*buf);
	*buf = NULL;
    }
    if ((ns == NULL) || (ns->prefix == NULL))
	return(local);
    else {
	*buf = xmlStrdup(ns->prefix);
	*buf = xmlStrcat(*buf, BAD_CAST ":");
	*buf = xmlStrcat(*buf, local);
    }
    return ((const xmlChar *) *buf);
}

/**
 * xmlSchemaFormatQName:
 * @buf: the string buffer
 * @namespaceName:  the namespace name
 * @localName: the local name
 *
 * Returns the given QName in the format "{namespaceName}localName" or
 * just "localName" if @namespaceName is NULL.
 *
 * Returns the localName if @namespaceName is NULL, a formatted
 * string otherwise.
 */  
static const xmlChar*   
xmlSchemaFormatQName(xmlChar **buf,
		     const xmlChar *namespaceName,
		     const xmlChar *localName)
{
    FREE_AND_NULL(*buf)
    if (namespaceName == NULL)
	return(localName);
    
    *buf = xmlStrdup(BAD_CAST "{");
    *buf = xmlStrcat(*buf, namespaceName);
    *buf = xmlStrcat(*buf, BAD_CAST "}");
    *buf = xmlStrcat(*buf, localName);
    
    return ((const xmlChar *) *buf);
}

static const xmlChar *
xmlSchemaGetComponentName(xmlSchemaBasicItemPtr item)
{
    switch (item->type) {
	case XML_SCHEMA_TYPE_ELEMENT:
	    return (((xmlSchemaElementPtr) item)->name);
	case XML_SCHEMA_TYPE_ATTRIBUTE:
	    return (((xmlSchemaAttributePtr) item)->name);
	case XML_SCHEMA_TYPE_ATTRIBUTEGROUP:
	    return (((xmlSchemaAttributeGroupPtr) item)->name);
	case XML_SCHEMA_TYPE_SIMPLE:
	case XML_SCHEMA_TYPE_COMPLEX:	
	    return (((xmlSchemaTypePtr) item)->name);
	case XML_SCHEMA_TYPE_GROUP:
	    return (((xmlSchemaModelGroupDefPtr) item)->name);
	case XML_SCHEMA_TYPE_IDC_KEY:
	case XML_SCHEMA_TYPE_IDC_UNIQUE:
	case XML_SCHEMA_TYPE_IDC_KEYREF:
	    return (((xmlSchemaIDCPtr) item)->name);
	default:
	    /*
	    * Other components cannot have names.
	    */
	    break;
    }
    return (NULL);
}

static const xmlChar *
xmlSchemaGetComponentTargetNs(xmlSchemaBasicItemPtr item)
{
    switch (item->type) {
	case XML_SCHEMA_TYPE_ELEMENT:
	    return (((xmlSchemaElementPtr) item)->targetNamespace);
	case XML_SCHEMA_TYPE_ATTRIBUTE:
	    return (((xmlSchemaAttributePtr) item)->targetNamespace);
	case XML_SCHEMA_TYPE_ATTRIBUTEGROUP:
	    return (((xmlSchemaAttributeGroupPtr) item)->targetNamespace);
	case XML_SCHEMA_TYPE_SIMPLE:
	case XML_SCHEMA_TYPE_COMPLEX:	
	    return (((xmlSchemaTypePtr) item)->targetNamespace);
	case XML_SCHEMA_TYPE_GROUP:
	    return (((xmlSchemaModelGroupDefPtr) item)->targetNamespace);
	case XML_SCHEMA_TYPE_IDC_KEY:
	case XML_SCHEMA_TYPE_IDC_UNIQUE:
	case XML_SCHEMA_TYPE_IDC_KEYREF:
	    return (((xmlSchemaIDCPtr) item)->targetNamespace);
	default:
	    /*
	    * Other components cannot have names.
	    */
	    break;
    }
    return (NULL);
}

static const xmlChar*
xmlSchemaGetComponentQName(xmlChar **buf, 
			   void *item)
{
    return (xmlSchemaFormatQName(buf,
	xmlSchemaGetComponentTargetNs((xmlSchemaBasicItemPtr) item),
	xmlSchemaGetComponentName((xmlSchemaBasicItemPtr) item)));
}

/**
 * xmlSchemaWildcardPCToString:
 * @pc: the type of processContents
 *
 * Returns a string representation of the type of 
 * processContents.
 */
static const xmlChar *
xmlSchemaWildcardPCToString(int pc)
{
    switch (pc) {
	case XML_SCHEMAS_ANY_SKIP:
	    return (BAD_CAST "skip");
	case XML_SCHEMAS_ANY_LAX:
	    return (BAD_CAST "lax");
	case XML_SCHEMAS_ANY_STRICT:
	    return (BAD_CAST "strict");
	default:
	    return (BAD_CAST "invalid process contents");
    }
}

/**
 * xmlSchemaFormatItemForReport:
 * @buf: the string buffer
 * @itemDes: the designation of the item
 * @itemName: the name of the item
 * @item: the item as an object 
 * @itemNode: the node of the item
 * @local: the local name
 * @parsing: if the function is used during the parse
 *
 * Returns a representation of the given item used
 * for error reports. 
 *
 * The following order is used to build the resulting 
 * designation if the arguments are not NULL:
 * 1a. If itemDes not NULL -> itemDes
 * 1b. If (itemDes not NULL) and (itemName not NULL)
 *     -> itemDes + itemName
 * 2. If the preceding was NULL and (item not NULL) -> item
 * 3. If the preceding was NULL and (itemNode not NULL) -> itemNode
 * 
 * If the itemNode is an attribute node, the name of the attribute
 * will be appended to the result.
 *
 * Returns the formatted string and sets @buf to the resulting value.
 */  
static xmlChar*   
xmlSchemaFormatItemForReport(xmlChar **buf,		     
		     const xmlChar *itemDes,
		     xmlSchemaTypePtr item,
		     xmlNodePtr itemNode,
		     int parsing)
{
    xmlChar *str = NULL;
    int named = 1;

    if (*buf != NULL) {
	xmlFree(*buf);
	*buf = NULL;
    }
            
    if (itemDes != NULL) {
	*buf = xmlStrdup(itemDes);	
    } else if (item != NULL) {
	switch (item->type) {
	case XML_SCHEMA_TYPE_BASIC:
	    if (item->builtInType == XML_SCHEMAS_ANYTYPE)
		*buf = xmlStrdup(BAD_CAST "'anyType'");
	    else if (item->builtInType == XML_SCHEMAS_ANYSIMPLETYPE)
		*buf = xmlStrdup(BAD_CAST "'anySimpleType'");
	    else {
		/* *buf = xmlStrdup(BAD_CAST "bi "); */
		/* *buf = xmlStrcat(*buf, xmlSchemaElemDesST); */
		*buf = xmlStrdup(BAD_CAST "'");
		*buf = xmlStrcat(*buf, item->name);
		*buf = xmlStrcat(*buf, BAD_CAST "'");
	    }
	    break;
	case XML_SCHEMA_TYPE_SIMPLE:
	    if (item->flags & XML_SCHEMAS_TYPE_GLOBAL) {
		*buf = xmlStrdup(xmlSchemaElemDesST);
		*buf = xmlStrcat(*buf, BAD_CAST " '");
		*buf = xmlStrcat(*buf, item->name);
		*buf = xmlStrcat(*buf, BAD_CAST "'");
	    } else {
		*buf = xmlStrdup(xmlSchemaElemDesST);
	    }
	    break;
	case XML_SCHEMA_TYPE_COMPLEX:
	    if (item->flags & XML_SCHEMAS_TYPE_GLOBAL) {
		*buf = xmlStrdup(xmlSchemaElemDesCT);
		*buf = xmlStrcat(*buf, BAD_CAST " '");
		*buf = xmlStrcat(*buf, item->name);
		*buf = xmlStrcat(*buf, BAD_CAST "'");
	    } else {
		*buf = xmlStrdup(xmlSchemaElemDesCT);
	    }
	    break;
	case XML_SCHEMA_TYPE_ATTRIBUTE: {
		xmlSchemaAttributePtr attr;
	    
		attr = (xmlSchemaAttributePtr) item;	    
		if ((attr->flags & XML_SCHEMAS_ATTR_GLOBAL) ||
		    (attr->ref == NULL)) {
		    *buf = xmlStrdup(xmlSchemaElemDesAttrDecl);
		    *buf = xmlStrcat(*buf, BAD_CAST " '");
		    *buf = xmlStrcat(*buf, attr->name);
		    *buf = xmlStrcat(*buf, BAD_CAST "'");
		} else {
		    *buf = xmlStrdup(xmlSchemaElemDesAttrRef);
		    *buf = xmlStrcat(*buf, BAD_CAST " '");
		    *buf = xmlStrcat(*buf, attr->refPrefix);
		    *buf = xmlStrcat(*buf, BAD_CAST ":");
		    *buf = xmlStrcat(*buf, attr->ref);
		    *buf = xmlStrcat(*buf, BAD_CAST "'");
		}	
	    }
	    break;
	case XML_SCHEMA_TYPE_ELEMENT: {
		xmlSchemaElementPtr elem;

		elem = (xmlSchemaElementPtr) item;	    
		if ((elem->flags & XML_SCHEMAS_ELEM_GLOBAL) || 
		    (elem->ref == NULL)) {
		    *buf = xmlStrdup(xmlSchemaElemDesElemDecl);
		    *buf = xmlStrcat(*buf, BAD_CAST " '");
		    *buf = xmlStrcat(*buf, elem->name);
		    *buf = xmlStrcat(*buf, BAD_CAST "'");
		} else {
		    *buf = xmlStrdup(xmlSchemaElemDesElemRef);
		    *buf = xmlStrcat(*buf, BAD_CAST " '");
		    *buf = xmlStrcat(*buf, elem->refPrefix);
		    *buf = xmlStrcat(*buf, BAD_CAST ":");
		    *buf = xmlStrcat(*buf, elem->ref);
		    *buf = xmlStrcat(*buf, BAD_CAST "'");
		}
	    }
	    break;
	case XML_SCHEMA_TYPE_IDC_UNIQUE:
	case XML_SCHEMA_TYPE_IDC_KEY:
	case XML_SCHEMA_TYPE_IDC_KEYREF:		
	    if (item->type == XML_SCHEMA_TYPE_IDC_UNIQUE)
		*buf = xmlStrdup(BAD_CAST "unique '");
	    else if (item->type == XML_SCHEMA_TYPE_IDC_KEY)
		*buf = xmlStrdup(BAD_CAST "key '");
	    else
		*buf = xmlStrdup(BAD_CAST "keyRef '");
	    *buf = xmlStrcat(*buf, ((xmlSchemaIDCPtr) item)->name);
	    *buf = xmlStrcat(*buf, BAD_CAST "'");
	    break;
	case XML_SCHEMA_TYPE_ANY:
	case XML_SCHEMA_TYPE_ANY_ATTRIBUTE:
	    *buf = xmlStrdup(xmlSchemaWildcardPCToString(
		    ((xmlSchemaWildcardPtr) item)->processContents));
	    *buf = xmlStrcat(*buf, BAD_CAST " wildcard");
	    break;
	case XML_SCHEMA_FACET_MININCLUSIVE:
	case XML_SCHEMA_FACET_MINEXCLUSIVE:
	case XML_SCHEMA_FACET_MAXINCLUSIVE:
	case XML_SCHEMA_FACET_MAXEXCLUSIVE:
	case XML_SCHEMA_FACET_TOTALDIGITS:
	case XML_SCHEMA_FACET_FRACTIONDIGITS:
	case XML_SCHEMA_FACET_PATTERN:
	case XML_SCHEMA_FACET_ENUMERATION:
	case XML_SCHEMA_FACET_WHITESPACE:
	case XML_SCHEMA_FACET_LENGTH:
	case XML_SCHEMA_FACET_MAXLENGTH:
	case XML_SCHEMA_FACET_MINLENGTH:
	    *buf = xmlStrdup(BAD_CAST "facet '");
	    *buf = xmlStrcat(*buf, xmlSchemaFacetTypeToString(item->type));
	    *buf = xmlStrcat(*buf, BAD_CAST "'");
	    break;
	case XML_SCHEMA_TYPE_NOTATION:
	    *buf = xmlStrdup(BAD_CAST "notation");
	    break;
	case XML_SCHEMA_TYPE_GROUP: {
		xmlChar *s = NULL;

		*buf = xmlStrdup(xmlSchemaElemModelGrDef);
		*buf = xmlStrcat(*buf, BAD_CAST " '");
		*buf = xmlStrcat(*buf, xmlSchemaFormatQName(&s,
		    ((xmlSchemaModelGroupDefPtr) item)->targetNamespace,
		    ((xmlSchemaModelGroupDefPtr) item)->name));
		*buf = xmlStrcat(*buf, BAD_CAST "'");
		FREE_AND_NULL(s)
	    }
	    break;
	case XML_SCHEMA_TYPE_SEQUENCE:
	case XML_SCHEMA_TYPE_CHOICE:
	case XML_SCHEMA_TYPE_ALL:
	case XML_SCHEMA_TYPE_PARTICLE:
	    *buf = xmlStrdup(xmlSchemaCompTypeToString(item->type));
	    break;	
	default:
	    named = 0;
	}
    } else 
	named = 0;

    if ((named == 0) && (itemNode != NULL)) {
	xmlNodePtr elem;

	if (itemNode->type == XML_ATTRIBUTE_NODE)
	    elem = itemNode->parent;
	else 
	    elem = itemNode;
	*buf = xmlStrdup(BAD_CAST "Element '");
	if (parsing)
	    *buf = xmlStrcat(*buf, elem->name);
	else
	    *buf = xmlStrcat(*buf, 
		xmlSchemaFormatNsPrefixLocal(&str, elem->ns, elem->name));
	*buf = xmlStrcat(*buf, BAD_CAST "'");
    }
    if ((itemNode != NULL) && (itemNode->type == XML_ATTRIBUTE_NODE)) {
	*buf = xmlStrcat(*buf, BAD_CAST ", attribute '");
	*buf = xmlStrcat(*buf, xmlSchemaFormatNsPrefixLocal(&str, 
	    itemNode->ns, itemNode->name));
	*buf = xmlStrcat(*buf, BAD_CAST "'");
    }
    FREE_AND_NULL(str);
    
    return (*buf);
}

/**
 * xmlSchemaPFormatItemDes:
 * @buf: the string buffer
 * @item: the item as a schema object
 * @itemNode: the item as a node
 *
 * If the pointer to @buf is not NULL and @but holds no value,
 * the value is set to a item designation using 
 * xmlSchemaFormatItemForReport. This one avoids adding
 * an attribute designation postfix.
 *
 * Returns a string of all enumeration elements.
 */
static void
xmlSchemaPRequestItemDes(xmlChar **buf,
		       xmlSchemaTypePtr item,
		       xmlNodePtr itemNode)
{
    if ((buf == 0) || (*buf != NULL)) 
	return;
    if (itemNode->type == XML_ATTRIBUTE_NODE)
	itemNode = itemNode->parent;
    xmlSchemaFormatItemForReport(buf, NULL, item, itemNode, 1);	
}

/**
 * xmlSchemaGetCanonValueWhtsp:
 * @val: the precomputed value
 * @retValue: the returned value
 * @ws: the whitespace type of the value
 *
 * Get a the cononical representation of the value.
 * The caller has to free the returned retValue.
 *
 * Returns 0 if the value could be built and -1 in case of
 *         API errors or if the value type is not supported yet.
 */
static int
xmlSchemaGetCanonValueWhtsp(const xmlChar *value,
			    xmlSchemaValPtr val,
			    xmlSchemaWhitespaceValueType ws,
			    const xmlChar **retValue)
{
    xmlSchemaValType valType;

    if ((retValue == NULL) || (value == NULL) || (val == NULL))
	return (-1);
    *retValue = NULL;
    valType = xmlSchemaGetValType(val);    
    switch (valType) {
	case XML_SCHEMAS_STRING:
	    if (value == NULL)
		*retValue = BAD_CAST xmlStrdup(BAD_CAST "");
	    else if (ws == XML_SCHEMA_WHITESPACE_COLLAPSE)
		*retValue = xmlSchemaCollapseString(value);
	    else if (ws == XML_SCHEMA_WHITESPACE_REPLACE)
		*retValue = xmlSchemaWhiteSpaceReplace(value);
	    if ((*retValue) == NULL)
		*retValue = BAD_CAST xmlStrdup(value);
	    break;
	case XML_SCHEMAS_NORMSTRING:
	    if (value == NULL)
		*retValue = BAD_CAST xmlStrdup(BAD_CAST "");
	    else {
		if (ws == XML_SCHEMA_WHITESPACE_COLLAPSE)
		    *retValue = xmlSchemaCollapseString(value);
		else
		    *retValue = xmlSchemaWhiteSpaceReplace(value);
		if ((*retValue) == NULL)
		    *retValue = BAD_CAST xmlStrdup(value);
	    }
	    break;
	default:
	    return (xmlSchemaGetCanonValue(val, retValue));
    }    
    return (0);
}

/**
 * xmlSchemaFormatFacetEnumSet:
 * @buf: the string buffer
 * @type: the type holding the enumeration facets
 *
 * Builds a string consisting of all enumeration elements.
 *
 * Returns a string of all enumeration elements.
 */
static const xmlChar *
xmlSchemaFormatFacetEnumSet(xmlChar **buf, xmlSchemaTypePtr type)
{
    xmlSchemaFacetPtr facet;
    xmlSchemaWhitespaceValueType ws;
    const xmlChar *value = NULL;
    int res, found = 0;

    if (*buf != NULL)
	xmlFree(*buf);    
    *buf = NULL;

    do {
	/*
	* Use the whitespace type of the base type.
	*/	
	ws = (xmlSchemaWhitespaceValueType)
	    xmlSchemaGetWhiteSpaceFacetValue(type->baseType);
	for (facet = type->facets; facet != NULL; facet = facet->next) {
	    if (facet->type != XML_SCHEMA_FACET_ENUMERATION)
		continue;
	    found = 1;
	    res = xmlSchemaGetCanonValueWhtsp(facet->value, facet->val,
		ws, &value);
	    if (res == -1) {
		xmlSchemaVErr(NULL, NULL,
		    XML_SCHEMAV_INTERNAL,
		    "Internal error: xmlSchemaFormatFacetEnumSet, failed to "
		    "compute the canonical lexical representation.\n",
		    NULL, NULL);
		if (*buf != NULL)
		    xmlFree(*buf);
		*buf = NULL;
		return (NULL);
	    }
	    if (*buf == NULL) {
		*buf = xmlStrdup(BAD_CAST "'");
		*buf = xmlStrcat(*buf, value);
		*buf = xmlStrcat(*buf, BAD_CAST "'");
	    } else {
		*buf = xmlStrcat(*buf, BAD_CAST ", '");
		*buf = xmlStrcat(*buf, value);
		*buf = xmlStrcat(*buf, BAD_CAST "'");
	    }
	    if (value != NULL) {
		xmlFree((xmlChar *)value);
		value = NULL;
	    }
	}
	type = type->baseType;
    } while ((! found) && (type != NULL) &&
	(type->type != XML_SCHEMA_TYPE_BASIC));

    return ((const xmlChar *) *buf);
}

/**
 * xmlSchemaVFacetErr:
 * @ctxt:  the schema validation context
 * @error: the error code
 * @node: the node to be validated  
 * @value: the value of the node
 * @type: the type holding the facet
 * @facet: the facet
 * @message: the error message of NULL
 * @str1: extra data
 * @str2: extra data
 * @str3: extra data
 *
 * Reports a facet validation error.
 * TODO: Should this report the value of an element as well?
 */
static void
xmlSchemaVFacetErr(xmlSchemaValidCtxtPtr ctxt, 
		   xmlParserErrors error,
		   xmlNodePtr node,		   
		   const xmlChar *value,
		   unsigned long length,
		   xmlSchemaTypePtr type,
		   xmlSchemaFacetPtr facet,		   
		   const char *message,
		   const xmlChar *str1,
		   const xmlChar *str2,
		   const xmlChar *str3)
{
    xmlChar *str = NULL, *msg = NULL;
    xmlSchemaTypeType facetType;

    xmlSchemaFormatItemForReport(&msg, NULL, NULL, node, 0);
    msg = xmlStrcat(msg, BAD_CAST " [");
    msg = xmlStrcat(msg, xmlSchemaFormatItemForReport(&str, NULL, type, NULL, 0));
    msg = xmlStrcat(msg, BAD_CAST ", facet '");
    if (error == XML_SCHEMAV_CVC_ENUMERATION_VALID) {
	facetType = XML_SCHEMA_FACET_ENUMERATION;
	/*
	* If enumerations are validated, one must not expect the
	* facet to be given.
	*/	
    } else	
	facetType = facet->type;
    msg = xmlStrcat(msg, xmlSchemaFacetTypeToString(facetType));
    msg = xmlStrcat(msg, BAD_CAST "']: ");
    if (message == NULL) {
	/*
	* Use a default message.
	*/
	if ((facetType == XML_SCHEMA_FACET_LENGTH) ||
	    (facetType == XML_SCHEMA_FACET_MINLENGTH) ||
	    (facetType == XML_SCHEMA_FACET_MAXLENGTH)) {

	    char len[25], actLen[25];

	    /* FIXME, TODO: What is the max expected string length of the
	    * this value?
	    */
	    if (node->type == XML_ATTRIBUTE_NODE)
		msg = xmlStrcat(msg, BAD_CAST "The value '%s' has a length of '%s'; ");
	    else
		msg = xmlStrcat(msg, BAD_CAST "The value has a length of '%s'; ");

	    snprintf(len, 24, "%lu", xmlSchemaGetFacetValueAsULong(facet));
	    snprintf(actLen, 24, "%lu", length);

	    if (facetType == XML_SCHEMA_FACET_LENGTH)
		msg = xmlStrcat(msg, 
		BAD_CAST "this differs from the allowed length of '%s'.\n");     
	    else if (facetType == XML_SCHEMA_FACET_MAXLENGTH)
		msg = xmlStrcat(msg, 
		BAD_CAST "this exceeds the allowed maximum length of '%s'.\n");
	    else if (facetType == XML_SCHEMA_FACET_MINLENGTH)
		msg = xmlStrcat(msg, 
		BAD_CAST "this underruns the allowed minimum length of '%s'.\n");
	    
	    if (node->type == XML_ATTRIBUTE_NODE)
		xmlSchemaVErrExt(ctxt, node, error,
		    (const char *) msg,
		    value, (const xmlChar *) actLen, (const xmlChar *) len,
		    NULL, NULL);
	    else 
		xmlSchemaVErr(ctxt, node, error,  
		    (const char *) msg,
		    (const xmlChar *) actLen, (const xmlChar *) len);
	
	} else if (facetType == XML_SCHEMA_FACET_ENUMERATION) {
	    msg = xmlStrcat(msg, BAD_CAST "The value '%s' is not an element "
		"of the set {%s}.\n");
	    xmlSchemaVErr(ctxt, node, error, (const char *) msg, value, 
		xmlSchemaFormatFacetEnumSet(&str, type));
	} else if (facetType == XML_SCHEMA_FACET_PATTERN) {
	    msg = xmlStrcat(msg, BAD_CAST "The value '%s' is not accepted "
		"by the pattern '%s'.\n");
	    xmlSchemaVErr(ctxt, node, error, (const char *) msg, value, 
		facet->value);
	} else if (facetType == XML_SCHEMA_FACET_MININCLUSIVE) {
	    msg = xmlStrcat(msg, BAD_CAST "The value '%s' is less than the "
		"minimum value allowed ('%s').\n");
	    xmlSchemaVErr(ctxt, node, error, (const char *) msg, value,
		facet->value);
	} else if (facetType == XML_SCHEMA_FACET_MAXINCLUSIVE) {
	    msg = xmlStrcat(msg, BAD_CAST "The value '%s' is greater than the "
		"maximum value allowed ('%s').\n");
	    xmlSchemaVErr(ctxt, node, error, (const char *) msg, value,
		facet->value);
#if 0
	} else if (facetType == XML_SCHEMA_FACET_MINEXCLUSIVE) {
	    msg = xmlStrcat(msg, BAD_CAST "The value '%s' is less than the "
		"minimum exclusive value allowed ('%s').\n");
	    xmlSchemaVErr(ctxt, node, error, (const char *) msg, value,
		facet->value);
	} else if (facetType == XML_SCHEMA_FACET_MAXEXCLUSIVE) {
	    msg = xmlStrcat(msg, BAD_CAST "The value '%s' is greater than the "
		"maximum exclusive value allowed ('%s').\n");
	    xmlSchemaVErr(ctxt, node, error, (const char *) msg, value,
		facet->value);
#endif
	} else if (node->type == XML_ATTRIBUTE_NODE) {		
	    msg = xmlStrcat(msg, BAD_CAST "The value '%s' is not facet-valid.\n");
	    xmlSchemaVErr(ctxt, node, error, (const char *) msg, value, NULL);
	} else {	    
	    msg = xmlStrcat(msg, BAD_CAST "The value is not facet-valid.\n");
	    xmlSchemaVErr(ctxt, node, error, (const char *) msg, NULL, NULL);
	}
    } else {
	msg = xmlStrcat(msg, (const xmlChar *) message);
	msg = xmlStrcat(msg, BAD_CAST ".\n");
	xmlSchemaVErr3(ctxt, node, error, (const char *) msg, str1, str2, str3);
    }        
    FREE_AND_NULL(str)
    xmlFree(msg);
}

/**
 * xmlSchemaVSimpleTypeErr:
 * @ctxt:  the schema validation context
 * @error: the error code
 * @type: the type used for validation
 * @node: the node containing the validated value
 * @value: the validated value
 *
 * Reports a simple type validation error.
 * TODO: Should this report the value of an element as well?
 */
static void
xmlSchemaVSimpleTypeErr(xmlSchemaValidCtxtPtr ctxt, 
			xmlParserErrors error,			
			xmlNodePtr node,
			const xmlChar *value,
			xmlSchemaTypePtr type)
{
    xmlChar *str = NULL, *msg = NULL;
    
    xmlSchemaFormatItemForReport(&msg, NULL,  NULL, node, 0);    
    msg = xmlStrcat(msg, BAD_CAST " [");
    msg = xmlStrcat(msg, xmlSchemaFormatItemForReport(&str, NULL, type, NULL, 0));
    if (node->type == XML_ATTRIBUTE_NODE) {
	msg = xmlStrcat(msg, BAD_CAST "]: The value '%s' is not valid.\n");
	xmlSchemaVErr(ctxt, node, error, (const char *) msg, value, NULL);
    } else {
	msg = xmlStrcat(msg, BAD_CAST "]: The character content is not valid.\n");
	xmlSchemaVErr(ctxt, node, error, (const char *) msg, NULL, NULL);
    }
    FREE_AND_NULL(str)	
    xmlFree(msg);
}

/**
 * xmlSchemaVComplexTypeErr:
 * @ctxt:  the schema validation context
 * @error: the error code
 * @node: the node containing the validated value
 * @type: the complex type used for validation
 * @message: the error message
 *
 * Reports a complex type validation error.
 */
static void
xmlSchemaVComplexTypeErr(xmlSchemaValidCtxtPtr ctxt, 
			xmlParserErrors error,
			xmlNodePtr node,
			xmlSchemaTypePtr type,			
			const char *message)
{
    xmlChar *str = NULL, *msg = NULL;
    
    xmlSchemaFormatItemForReport(&msg, NULL,  NULL, node, 0);
    /* Specify the complex type only if it is global. */
    if ((type != NULL) && (type->flags & XML_SCHEMAS_TYPE_GLOBAL)) {
	msg = xmlStrcat(msg, BAD_CAST " [");
	msg = xmlStrcat(msg, xmlSchemaFormatItemForReport(&str, NULL, type, NULL, 0));
	msg = xmlStrcat(msg, BAD_CAST "]");
    }
    msg = xmlStrcat(msg, BAD_CAST ": %s.\n");   
    xmlSchemaVErr(ctxt, node, error, (const char *) msg,
	(const xmlChar *) message, NULL);
    FREE_AND_NULL(str)	
    xmlFree(msg);
}

/**
 * xmlSchemaVComplexTypeElemErr:
 * @ctxt:  the schema validation context
 * @error: the error code
 * @node: the node containing the validated value
 * @type: the complex type used for validation
 * @message: the error message
 *
 * Reports a complex type validation error.
 */
static void
xmlSchemaVComplexTypeElemErr(xmlSchemaValidCtxtPtr ctxt, 
			xmlParserErrors error,
			xmlNodePtr node,
			xmlSchemaTypePtr type,			
			const char *message,
			int nbval,
			int nbneg,
			xmlChar **values)
{
    xmlChar *str = NULL, *msg = NULL;
    xmlChar *localName, *nsName;
    const xmlChar *cur, *end;
    int i;
    
    xmlSchemaFormatItemForReport(&msg, NULL,  NULL, node, 0);
    /* Specify the complex type only if it is global. */
    if ((type != NULL) && (type->flags & XML_SCHEMAS_TYPE_GLOBAL)) {
	msg = xmlStrcat(msg, BAD_CAST " [");
	msg = xmlStrcat(msg, xmlSchemaFormatItemForReport(&str, NULL, type, NULL, 0));
	msg = xmlStrcat(msg, BAD_CAST "]");
	FREE_AND_NULL(str)
    }
    msg = xmlStrcat(msg, BAD_CAST ": ");
    msg = xmlStrcat(msg, (const xmlChar *) message);
    /*
    * Note that is does not make sense to report that we have a
    * wildcard here, since the wildcard might be unfolded into
    * multiple transitions.
    */
    if (nbval + nbneg > 0) {
	if (nbval + nbneg > 1) {
	    str = xmlStrdup(BAD_CAST ". Expected is one of ( ");
	} else
	    str = xmlStrdup(BAD_CAST ". Expected is ( ");
	nsName = NULL;
    	    
	for (i = 0; i < nbval + nbneg; i++) {
	    cur = values[i];
	    /*
	    * Get the local name.
	    */
	    localName = NULL;
	    
	    end = cur;
	    if (*end == '*') {
		localName = xmlStrdup(BAD_CAST "*");
		*end++;
	    } else {
		while ((*end != 0) && (*end != '|'))
		    end++;
		localName = xmlStrncat(localName, BAD_CAST cur, end - cur);
	    }		
	    if (*end != 0) {		    
		*end++;
		/*
		* Skip "*|*" if they come with negated expressions, since
		* they represent the same negated wildcard.
		*/
		if ((nbneg == 0) || (*end != '*') || (*localName != '*')) {
		    /*
		    * Get the namespace name.
		    */
		    cur = end;
		    if (*end == '*') {
			nsName = xmlStrdup(BAD_CAST "{*}");
		    } else {
			while (*end != 0)
			    end++;
			
			if (i >= nbval)
			    nsName = xmlStrdup(BAD_CAST "{##other:");
			else
			    nsName = xmlStrdup(BAD_CAST "{");
			
			nsName = xmlStrncat(nsName, BAD_CAST cur, end - cur);
			nsName = xmlStrcat(nsName, BAD_CAST "}");
		    }
		    str = xmlStrcat(str, BAD_CAST nsName);
		    FREE_AND_NULL(nsName)
		} else {
		    FREE_AND_NULL(localName);
		    continue;
		}
	    }	        
	    str = xmlStrcat(str, BAD_CAST localName);
	    FREE_AND_NULL(localName);
		
	    if (i < nbval + nbneg -1)
		str = xmlStrcat(str, BAD_CAST ", ");
	}	
	str = xmlStrcat(str, BAD_CAST " )");
	msg = xmlStrcat(msg, BAD_CAST str);
	FREE_AND_NULL(str)
    }    
    msg = xmlStrcat(msg, BAD_CAST ".\n");
    xmlSchemaVErr(ctxt, node, error, (const char *) msg, NULL, NULL);    	
    xmlFree(msg);
}

/**
 * xmlSchemaPMissingAttrErr:
 * @ctxt: the schema validation context
 * @ownerDes: the designation of  the owner
 * @ownerName: the name of the owner
 * @ownerItem: the owner as a schema object
 * @ownerElem: the owner as an element node
 * @node: the parent element node of the missing attribute node
 * @type: the corresponding type of the attribute node
 *
 * Reports an illegal attribute.
 */
static void
xmlSchemaPMissingAttrErr(xmlSchemaParserCtxtPtr ctxt,
			 xmlParserErrors error,			 
			 xmlChar **ownerDes,
			 xmlSchemaTypePtr ownerItem,
			 xmlNodePtr ownerElem,
			 const char *name,
			 const char *message)
{
    xmlChar *des = NULL;

    if (ownerDes == NULL)
	xmlSchemaFormatItemForReport(&des, NULL, ownerItem, ownerElem, 1);
    else if (*ownerDes == NULL) {
	xmlSchemaFormatItemForReport(ownerDes, NULL, ownerItem, ownerElem, 1);
	des = *ownerDes;
    } else 
	des = *ownerDes;      
    if (message != NULL)
	xmlSchemaPErr(ctxt, ownerElem, error, "%s: %s.\n", BAD_CAST des, BAD_CAST message);
    else	
	xmlSchemaPErr(ctxt, ownerElem, error, 
	    "%s: The attribute '%s' is required but missing.\n", 
	    BAD_CAST des, BAD_CAST name);
    if (ownerDes == NULL)
	FREE_AND_NULL(des);
}

/**
 * xmlSchemaCompTypeToString:
 * @type: the type of the schema item
 *
 * Returns the component name of a schema item.
 */
static const xmlChar *
xmlSchemaCompTypeToString(xmlSchemaTypeType type)
{
    switch (type) {
	case XML_SCHEMA_TYPE_SIMPLE:
	    return(BAD_CAST "simple type definition");
	case XML_SCHEMA_TYPE_COMPLEX:
	    return(BAD_CAST "complex type definition");
	case XML_SCHEMA_TYPE_ELEMENT:
	    return(BAD_CAST "element declaration");
	case XML_SCHEMA_TYPE_ATTRIBUTE:
	    return(BAD_CAST "attribute declaration");
	case XML_SCHEMA_TYPE_GROUP:
	    return(BAD_CAST "model group definition");
	case XML_SCHEMA_TYPE_ATTRIBUTEGROUP:
	    return(BAD_CAST "attribute group definition");
	case XML_SCHEMA_TYPE_NOTATION:
	    return(BAD_CAST "notation declaration");
	case XML_SCHEMA_TYPE_SEQUENCE:
	    return(BAD_CAST "model group (sequence)");
	case XML_SCHEMA_TYPE_CHOICE:
	    return(BAD_CAST "model group (choice)");
	case XML_SCHEMA_TYPE_ALL:
	    return(BAD_CAST "model group (all)");
	case XML_SCHEMA_TYPE_PARTICLE:
	    return(BAD_CAST "particle");
	default:
	    return(BAD_CAST "Not a schema component");
    }
}
/**
 * xmlSchemaPResCompAttrErr:
 * @ctxt: the schema validation context
 * @error: the error code
 * @ownerDes: the designation of  the owner
 * @ownerItem: the owner as a schema object
 * @ownerElem: the owner as an element node
 * @name: the name of the attribute holding the QName 
 * @refName: the referenced local name
 * @refURI: the referenced namespace URI
 * @message: optional message
 *
 * Used to report QName attribute values that failed to resolve
 * to schema components.
 */
static void
xmlSchemaPResCompAttrErr(xmlSchemaParserCtxtPtr ctxt,
			 xmlParserErrors error,			 
			 xmlChar **ownerDes,
			 xmlSchemaTypePtr ownerItem,
			 xmlNodePtr ownerElem,
			 const char *name,
			 const xmlChar *refName,
			 const xmlChar *refURI,
			 xmlSchemaTypeType refType,
			 const char *refTypeStr)
{
    xmlChar *des = NULL, *strA = NULL;

    if (ownerDes == NULL)
	xmlSchemaFormatItemForReport(&des, NULL, ownerItem, ownerElem, 1);
    else if (*ownerDes == NULL) {
	xmlSchemaFormatItemForReport(ownerDes, NULL, ownerItem, ownerElem, 1);
	des = *ownerDes;
    } else
	des = *ownerDes;
    if (refTypeStr == NULL)
	refTypeStr = (const char *) xmlSchemaCompTypeToString(refType);    
	xmlSchemaPErrExt(ctxt, ownerElem, error, 
	    NULL, NULL, NULL,
	    "%s, attribute '%s': The QName value %s does not resolve to a(n) "
	    "%s.\n", BAD_CAST des, BAD_CAST name, 
	    xmlSchemaFormatNsUriLocal(&strA, refURI, refName), 
	    BAD_CAST refTypeStr, NULL);
    if (ownerDes == NULL)
	FREE_AND_NULL(des)
    FREE_AND_NULL(strA)
}

/**
 * xmlSchemaPCustomAttrErr:
 * @ctxt: the schema parser context
 * @error: the error code
 * @ownerDes: the designation of the owner
 * @ownerItem: the owner as a schema object
 * @attr: the illegal attribute node 
 *
 * Reports an illegal attribute during the parse.
 */
static void
xmlSchemaPCustomAttrErr(xmlSchemaParserCtxtPtr ctxt,
			xmlParserErrors error,	
			xmlChar **ownerDes,
			xmlSchemaTypePtr ownerItem,
			xmlAttrPtr attr,
			const char *msg)
{
    xmlChar *des = NULL;

    if (ownerDes == NULL)
	xmlSchemaFormatItemForReport(&des, NULL, ownerItem, attr->parent, 1);
    else if (*ownerDes == NULL) {
	xmlSchemaFormatItemForReport(ownerDes, NULL, ownerItem, attr->parent, 1);
	des = *ownerDes;
    } else 
	des = *ownerDes;    
    xmlSchemaPErrExt(ctxt, (xmlNodePtr) attr, error, NULL, NULL, NULL,
	"%s, attribute '%s': %s.\n", 
	BAD_CAST des, attr->name, (const xmlChar *) msg, NULL, NULL);
    if (ownerDes == NULL)
	FREE_AND_NULL(des);
}

/**
 * xmlSchemaPIllegalAttrErr:
 * @ctxt: the schema parser context
 * @error: the error code
 * @ownerDes: the designation of the attribute's owner
 * @ownerItem: the attribute's owner item
 * @attr: the illegal attribute node 
 *
 * Reports an illegal attribute during the parse.
 */
static void
xmlSchemaPIllegalAttrErr(xmlSchemaParserCtxtPtr ctxt,
			 xmlParserErrors error,	
			 xmlChar **ownerDes,
			 xmlSchemaTypePtr ownerItem,
			 xmlAttrPtr attr)
{
    xmlChar *des = NULL, *strA = NULL;

    if (ownerDes == NULL)
	xmlSchemaFormatItemForReport(&des, NULL, ownerItem, attr->parent, 1);
    else if (*ownerDes == NULL) {
	xmlSchemaFormatItemForReport(ownerDes, NULL, ownerItem, attr->parent, 1);
	des = *ownerDes;
    } else 
	des = *ownerDes;    
    xmlSchemaPErr(ctxt, (xmlNodePtr) attr, error, 
	"%s: The attribute '%s' is not allowed.\n", BAD_CAST des, 
	xmlSchemaFormatNsPrefixLocal(&strA, attr->ns, attr->name));
    if (ownerDes == NULL)
	FREE_AND_NULL(des);
    FREE_AND_NULL(strA);
}

/**
 * xmlSchemaPAquireDes:
 * @des: the first designation 
 * @itemDes: the second designation
 * @item: the schema item 
 * @itemElem: the node of the schema item
 *
 * Creates a designation for an item.
 */
static void
xmlSchemaPAquireDes(xmlChar **des,
		    xmlChar **itemDes, 
		    xmlSchemaTypePtr item,
		    xmlNodePtr itemElem)
{
    if (itemDes == NULL)
	xmlSchemaFormatItemForReport(des, NULL, item, itemElem, 1);
    else if (*itemDes == NULL) {
	xmlSchemaFormatItemForReport(itemDes, NULL, item, itemElem, 1);
	*des = *itemDes;
    } else 
	*des = *itemDes;  
}

/**
 * xmlSchemaPCustomErr:
 * @ctxt: the schema parser context
 * @error: the error code
 * @itemDes: the designation of the schema item
 * @item: the schema item
 * @itemElem: the node of the schema item
 * @message: the error message
 * @str1: an optional param for the error message
 * @str2: an optional param for the error message
 * @str3: an optional param for the error message
 *
 * Reports an error during parsing.
 */
static void
xmlSchemaPCustomErrExt(xmlSchemaParserCtxtPtr ctxt,
		    xmlParserErrors error,	
		    xmlChar **itemDes,
		    xmlSchemaTypePtr item,
		    xmlNodePtr itemElem,
		    const char *message,
		    const xmlChar *str1,
		    const xmlChar *str2,
		    const xmlChar *str3)
{
    xmlChar *des = NULL, *msg = NULL;

    xmlSchemaPAquireDes(&des, itemDes, item, itemElem);   
    msg = xmlStrdup(BAD_CAST "%s: ");
    msg = xmlStrcat(msg, (const xmlChar *) message);
    msg = xmlStrcat(msg, BAD_CAST ".\n");
    if ((itemElem == NULL) && (item != NULL))
	itemElem = item->node;
    xmlSchemaPErrExt(ctxt, itemElem, error, NULL, NULL, NULL, 
	(const char *) msg, BAD_CAST des, str1, str2, str3, NULL);
    if (itemDes == NULL)
	FREE_AND_NULL(des);
    FREE_AND_NULL(msg);
}

/**
 * xmlSchemaPCustomErr:
 * @ctxt: the schema parser context
 * @error: the error code
 * @itemDes: the designation of the schema item
 * @item: the schema item
 * @itemElem: the node of the schema item
 * @message: the error message
 * @str1: the optional param for the error message
 *
 * Reports an error during parsing.
 */
static void
xmlSchemaPCustomErr(xmlSchemaParserCtxtPtr ctxt,
		    xmlParserErrors error,	
		    xmlChar **itemDes,
		    xmlSchemaTypePtr item,
		    xmlNodePtr itemElem,
		    const char *message,
		    const xmlChar *str1)
{
    xmlSchemaPCustomErrExt(ctxt, error, itemDes, item, itemElem, message,
	str1, NULL, NULL);
}

/**
 * xmlSchemaPAttrUseErr:
 * @ctxt: the schema parser context
 * @error: the error code
 * @itemDes: the designation of the schema type
 * @item: the schema type
 * @itemElem: the node of the schema type
 * @attr: the invalid schema attribute
 * @message: the error message
 * @str1: the optional param for the error message
 *
 * Reports an attribute use error during parsing.
 */
static void
xmlSchemaPAttrUseErr(xmlSchemaParserCtxtPtr ctxt,
		    xmlParserErrors error,	
		    xmlChar **itemDes,
		    xmlSchemaTypePtr item,
		    xmlNodePtr itemElem,
		    const xmlSchemaAttributePtr attr,
		    const char *message,
		    const xmlChar *str1)
{
    xmlChar *des = NULL, *strA = NULL, *msg = NULL;

    xmlSchemaPAquireDes(&des, itemDes, item, itemElem);
    xmlSchemaFormatNsUriLocal(&strA, xmlSchemaGetAttrTargetNsURI(attr), 
	xmlSchemaGetAttrName(attr));
    msg = xmlStrdup(BAD_CAST "%s, attr. use %s: ");
    msg = xmlStrcat(msg, (const xmlChar *) message);
    msg = xmlStrcat(msg, BAD_CAST ".\n");
    if ((itemElem == NULL) && (item != NULL))
	itemElem = item->node;
    xmlSchemaPErrExt(ctxt, itemElem, error, NULL, NULL, NULL, 
	(const char *) msg, BAD_CAST des, BAD_CAST strA, str1, NULL, NULL);
    if (itemDes == NULL)
	FREE_AND_NULL(des);
    FREE_AND_NULL(strA);
    xmlFree(msg);
}

/**
 * xmlSchemaPIllegalFacetAtomicErr:
 * @ctxt: the schema parser context
 * @error: the error code
 * @itemDes: the designation of the type
 * @item: the schema type
 * @baseItem: the base type of type
 * @facet: the illegal facet
 *
 * Reports an illegal facet for atomic simple types.
 */
static void
xmlSchemaPIllegalFacetAtomicErr(xmlSchemaParserCtxtPtr ctxt,
			  xmlParserErrors error,	
			  xmlChar **itemDes,
			  xmlSchemaTypePtr item,
			  xmlSchemaTypePtr baseItem,
			  xmlSchemaFacetPtr facet)
{
    xmlChar *des = NULL, *strT = NULL;

    xmlSchemaPAquireDes(&des, itemDes, item, item->node);
    xmlSchemaPErrExt(ctxt, item->node, error, NULL, NULL, NULL,
	"%s: The facet '%s' is not allowed on types derived from the "
	"type %s.\n",
	BAD_CAST des, xmlSchemaFacetTypeToString(facet->type),
	xmlSchemaFormatItemForReport(&strT, NULL, baseItem, NULL, 1),
	NULL, NULL);
    if (itemDes == NULL)
	FREE_AND_NULL(des);
    FREE_AND_NULL(strT);
}

/**
 * xmlSchemaPIllegalFacetListUnionErr:
 * @ctxt: the schema parser context
 * @error: the error code
 * @itemDes: the designation of the schema item involved
 * @item: the schema item involved
 * @facet: the illegal facet
 *
 * Reports an illegal facet for <list> and <union>.
 */
static void
xmlSchemaPIllegalFacetListUnionErr(xmlSchemaParserCtxtPtr ctxt,
			  xmlParserErrors error,	
			  xmlChar **itemDes,
			  xmlSchemaTypePtr item,
			  xmlSchemaFacetPtr facet)
{
    xmlChar *des = NULL, *strT = NULL;

    xmlSchemaPAquireDes(&des, itemDes, item, item->node);
    xmlSchemaPErr(ctxt, item->node, error, 
	"%s: The facet '%s' is not allowed.\n", 
	BAD_CAST des, xmlSchemaFacetTypeToString(facet->type));
    if (itemDes == NULL)
	FREE_AND_NULL(des);
    FREE_AND_NULL(strT);
}

/**
 * xmlSchemaPMutualExclAttrErr:
 * @ctxt: the schema validation context
 * @error: the error code
 * @elemDes: the designation of the parent element node
 * @attr: the bad attribute node
 * @type: the corresponding type of the attribute node
 *
 * Reports an illegal attribute.
 */
static void
xmlSchemaPMutualExclAttrErr(xmlSchemaParserCtxtPtr ctxt,
			 xmlParserErrors error,
			 xmlChar **ownerDes,
			 xmlSchemaTypePtr ownerItem,
			 xmlAttrPtr attr,			 
			 const char *name1,
			 const char *name2)
{
    xmlChar *des = NULL;

    if (ownerDes == NULL)
	xmlSchemaFormatItemForReport(&des, NULL, ownerItem, attr->parent, 1);	
    else if (*ownerDes == NULL) {
	xmlSchemaFormatItemForReport(ownerDes, NULL, ownerItem, attr->parent, 1);
	des = *ownerDes;
    } else 
	des = *ownerDes;  
    xmlSchemaPErrExt(ctxt, (xmlNodePtr) attr, error, NULL, NULL, NULL,
	"%s: The attributes '%s' and '%s' are mutually exclusive.\n", 
	BAD_CAST des, BAD_CAST name1, BAD_CAST name2, NULL, NULL);
    if (ownerDes == NULL)
	FREE_AND_NULL(des)
}

/**
 * xmlSchemaPSimpleTypeErr:
 * @ctxt:  the schema validation context
 * @error: the error code
 * @type: the type specifier
 * @ownerDes: the designation of the owner
 * @ownerItem: the schema object if existent 
 * @node: the validated node
 * @value: the validated value
 *
 * Reports a simple type validation error.
 * TODO: Should this report the value of an element as well?
 */
static void
xmlSchemaPSimpleTypeErr(xmlSchemaParserCtxtPtr ctxt, 
			xmlParserErrors error,
			xmlChar **ownerDes,
			xmlSchemaTypePtr ownerItem,
			xmlNodePtr node,
			xmlSchemaTypePtr type,
			const char *typeDes,
			const xmlChar *value,
			const char *message,
			const xmlChar *str1,
			const xmlChar *str2)
{
    xmlChar *des = NULL, *strA = NULL, *strT = NULL;    
    
    if (ownerDes == NULL)
	xmlSchemaPRequestItemDes(&des, ownerItem, node);
    else if (*ownerDes == NULL) {
	xmlSchemaPRequestItemDes(ownerDes, ownerItem, node);
	des = *ownerDes;
    } else 
	des = *ownerDes;   
    if (type != NULL)
	typeDes = (const char *) xmlSchemaFormatItemForReport(&strT, NULL, type, NULL, 1);
    if (message == NULL) {
	/*
	* Use default messages.
	*/
	if (node->type == XML_ATTRIBUTE_NODE) {
	    xmlSchemaPErrExt(ctxt, node, error, NULL, NULL, NULL,
		"%s, attribute '%s' [%s]: The value '%s' is not "
		"valid.\n", 
		BAD_CAST des, xmlSchemaFormatNsPrefixLocal(&strA, node->ns, 
		node->name), BAD_CAST typeDes, value, NULL);
	} else {
	    xmlSchemaPErr(ctxt, node, error, 
		"%s [%s]: The character content is not valid.\n",
		BAD_CAST des, BAD_CAST typeDes);
	}
    } else {
	xmlChar *msg;

	msg = xmlStrdup(BAD_CAST "%s");
	if (node->type == XML_ATTRIBUTE_NODE)
	    msg = xmlStrcat(msg, BAD_CAST ", attribute '%s'");
	msg = xmlStrcat(msg, BAD_CAST " [%s]: ");
	msg = xmlStrcat(msg, (const xmlChar *) message);
	msg = xmlStrcat(msg, BAD_CAST ".\n");
	if (node->type == XML_ATTRIBUTE_NODE) {
	    xmlSchemaPErrExt(ctxt, node, error, NULL, NULL, NULL,
		(const char *) msg, 
		BAD_CAST des, xmlSchemaFormatNsPrefixLocal(&strA, 
		node->ns, node->name), BAD_CAST typeDes, str1, str2);
	} else {
	    xmlSchemaPErrExt(ctxt, node, error, NULL, NULL, NULL,
		(const char *) msg, 
		BAD_CAST des, BAD_CAST typeDes, str1, str2, NULL);
	}
	xmlFree(msg);
    }
    /* Cleanup. */
    FREE_AND_NULL(strA)
    FREE_AND_NULL(strT)
    if (ownerDes == NULL)
	FREE_AND_NULL(des)
}

/**
 * xmlSchemaPContentErr:
 * @ctxt: the schema parser context
 * @error: the error code
 * @onwerDes: the designation of the holder of the content
 * @ownerItem: the owner item of the holder of the content
 * @ownerElem: the node of the holder of the content
 * @child: the invalid child node
 * @message: the optional error message
 * @content: the optional string describing the correct content
 *
 * Reports an error concerning the content of a schema element.
 */
static void
xmlSchemaPContentErr(xmlSchemaParserCtxtPtr ctxt, 
		     xmlParserErrors error,
		     xmlChar **ownerDes,
		     xmlSchemaTypePtr ownerItem,
		     xmlNodePtr ownerElem,		     
		     xmlNodePtr child,
		     const char *message,
		     const char *content)
{
    xmlChar *des = NULL;
    
    if (ownerDes == NULL)
	xmlSchemaFormatItemForReport(&des, NULL, ownerItem, ownerElem, 1);
    else if (*ownerDes == NULL) {
	xmlSchemaFormatItemForReport(ownerDes, NULL, ownerItem, ownerElem, 1);
	des = *ownerDes;
    } else 
	des = *ownerDes;   
    if (message != NULL)
	xmlSchemaPErr2(ctxt, ownerElem, child, error, 
	    "%s: %s.\n", 
	    BAD_CAST des, BAD_CAST message);
    else {
	if (content != NULL) {
	    xmlSchemaPErr2(ctxt, ownerElem, child, error, 
		"%s: The content is not valid. Expected is %s.\n", 
		BAD_CAST des, BAD_CAST content);
	} else {
	    xmlSchemaPErr2(ctxt, ownerElem, child, error, 
		"%s: The content is not valid.\n", 
		BAD_CAST des, NULL);
	}
    }
    if (ownerDes == NULL)
	FREE_AND_NULL(des)
}   

/**
 * xmlSchemaVIllegalAttrErr:
 * @ctxt: the schema validation context
 * @error: the error code
 * @attr: the illegal attribute node
 *
 * Reports an illegal attribute.
 */
static void
xmlSchemaVIllegalAttrErr(xmlSchemaValidCtxtPtr ctxt,
			 xmlParserErrors error,
			 xmlAttrPtr attr)
{
    xmlChar *strE = NULL, *strA = NULL;
    
    xmlSchemaVErr(ctxt, (xmlNodePtr) attr, 	
	error,
	/* XML_SCHEMAS_ERR_ATTRUNKNOWN, */
	"%s: The attribute '%s' is not allowed.\n",
	xmlSchemaFormatItemForReport(&strE, NULL, NULL, attr->parent, 0),
	xmlSchemaFormatNsPrefixLocal(&strA, attr->ns, attr->name));
    FREE_AND_NULL(strE)
    FREE_AND_NULL(strA)
}


static int
xmlSchemaIsGlobalItem(xmlSchemaTypePtr item)
{
    switch (item->type) {
	case XML_SCHEMA_TYPE_COMPLEX:
	case XML_SCHEMA_TYPE_SIMPLE:	
	    if (item->flags & XML_SCHEMAS_TYPE_GLOBAL)
		return(1);
	    break;
	case XML_SCHEMA_TYPE_GROUP:
	    return (1);
	case XML_SCHEMA_TYPE_ELEMENT:
	    if ( ((xmlSchemaElementPtr) item)->flags & 
		XML_SCHEMAS_ELEM_GLOBAL)
		return(1);
	    break;
	case XML_SCHEMA_TYPE_ATTRIBUTE:
	    if ( ((xmlSchemaAttributePtr) item)->flags & 
		XML_SCHEMAS_ATTR_GLOBAL)
		return(1);
	    break;
	/* Note that attribute groups are always global. */
	default:
	    return(1);
    }
    return (0);
}


static void
xmlSchemaStreamVCustomErr(xmlSchemaValidCtxtPtr vctxt,
			  xmlParserErrors error,
			  xmlSchemaNodeInfoPtr nodeInfo,
			  xmlSchemaTypePtr type,
			  const char *message,
			  const xmlChar *str1,
			  const xmlChar *str2)
{
    xmlChar *msg = NULL, *str = NULL; 

    msg = xmlStrdup(BAD_CAST "Element '");

    if (vctxt->elemInfos[vctxt->depth] != nodeInfo) {
	xmlSchemaNodeInfoPtr elemInfo;
	/*
	* The node info is an attribute info.
	*/
	elemInfo = vctxt->elemInfos[vctxt->depth];
	msg = xmlStrcat(msg, xmlSchemaFormatQName(&str,
	    elemInfo->namespaceName, elemInfo->localName));
	msg = xmlStrcat(msg, BAD_CAST "', ");
	msg = xmlStrcat(msg, BAD_CAST "attribute '");
    }
    msg = xmlStrcat(msg, xmlSchemaFormatQName(&str,
	nodeInfo->namespaceName, nodeInfo->localName));
    msg = xmlStrcat(msg, BAD_CAST "'");
    
    if ((type != NULL) && (xmlSchemaIsGlobalItem(type))) {
	msg = xmlStrcat(msg, BAD_CAST " [");
	msg = xmlStrcat(msg, xmlSchemaFormatItemForReport(&str,
	    NULL, type, NULL, 0));
	msg = xmlStrcat(msg, BAD_CAST "]");
    }
    msg = xmlStrcat(msg, BAD_CAST ": ");
    
    msg = xmlStrcat(msg, (const xmlChar *) message);
    msg = xmlStrcat(msg, BAD_CAST ".\n");   
    xmlSchemaVErr(vctxt, nodeInfo->node, error, (const char *) msg,
	str1, str2);
    FREE_AND_NULL(msg)
    FREE_AND_NULL(str)    
}

/**
 * xmlSchemaVCustomErr:
 * @ctxt: the schema validation context
 * @error: the error code
 * @node: the validated node
 * @type: the schema type of the validated node
 * @message: the error message
 * @str1: the optional param for the message
 *
 * Reports a validation error.
 */
static void
xmlSchemaVCustomErr(xmlSchemaValidCtxtPtr ctxt,
		    xmlParserErrors error,			    
		    xmlNodePtr node,
		    xmlSchemaTypePtr type,
		    const char *message,
		    const xmlChar *str1)
{
    xmlChar *msg = NULL, *str = NULL;
    
    if (node == NULL) {
	xmlSchemaVErr(ctxt, NULL,
	    XML_SCHEMAV_INTERNAL,
	    "Internal error: xmlSchemaVCustomErr, no node "
	    "given.\n", NULL, NULL);
	return;
    }
    /* TODO: Are the HTML and DOCB doc nodes expected here? */
    if (node->type != XML_DOCUMENT_NODE) {
	xmlSchemaFormatItemForReport(&msg, NULL, NULL, node, 0);
	if ((type != NULL) && (xmlSchemaIsGlobalItem(type))) {
	    msg = xmlStrcat(msg, BAD_CAST " [");
	    msg = xmlStrcat(msg, xmlSchemaFormatItemForReport(&str, NULL, type, NULL, 0));
	    msg = xmlStrcat(msg, BAD_CAST "]");
	}
	msg = xmlStrcat(msg, BAD_CAST ": ");
    } else
	msg = xmlStrdup((const xmlChar *) "");
    msg = xmlStrcat(msg, (const xmlChar *) message);
    msg = xmlStrcat(msg, BAD_CAST ".\n");   
    xmlSchemaVErr(ctxt, node, error, (const char *) msg, str1, NULL);
    FREE_AND_NULL(msg)
    FREE_AND_NULL(str)
}

/**
 * xmlSchemaVWildcardErr:
 * @ctxt: the schema validation context
 * @error: the error code
 * @node: the validated node
 * @wild: the wildcard used
 * @message: the error message
 *
 * Reports an validation-by-wildcard error.
 */
static void
xmlSchemaVWildcardErr(xmlSchemaValidCtxtPtr ctxt,
		    xmlParserErrors error,			    
		    xmlNodePtr node,
		    xmlSchemaWildcardPtr wild,
		    const char *message)
{
    xmlChar *des = NULL, *msg = NULL;

    xmlSchemaFormatItemForReport(&des, NULL, NULL, node, 0);
    msg = xmlStrdup(BAD_CAST "%s [");
    msg = xmlStrcat(msg, xmlSchemaWildcardPCToString(wild->processContents));
    msg = xmlStrcat(msg, BAD_CAST " wildcard]: ");
    msg = xmlStrcat(msg, (const xmlChar *) message);
    msg = xmlStrcat(msg, BAD_CAST ".\n");
    xmlSchemaVErr(ctxt, node, error, (const char *) msg, BAD_CAST des, NULL);
    FREE_AND_NULL(des);
    FREE_AND_NULL(msg);
}

/**
 * xmlSchemaVMissingAttrErr:
 * @ctxt: the schema validation context
 * @node: the parent element node of the missing attribute node
 * @type: the corresponding type of the attribute node
 *
 * Reports an illegal attribute.
 */
static void
xmlSchemaVMissingAttrErr(xmlSchemaValidCtxtPtr ctxt,
			 xmlNodePtr elem,
			 xmlSchemaAttributePtr type)
{
    const xmlChar *name, *uri;
    xmlChar *strE = NULL, *strA = NULL;

    if (type->ref != NULL) {				
	name = type->ref;
	uri = type->refNs;
    } else {
	name = type->name;
	uri = type->targetNamespace;
    }			    
    xmlSchemaVErr(ctxt, elem, 
	XML_SCHEMAV_CVC_COMPLEX_TYPE_4,
	/* XML_SCHEMAS_ERR_MISSING, */
	"%s: The attribute %s is required but missing.\n",
	xmlSchemaFormatItemForReport(&strE, NULL, NULL, elem, 0),
	xmlSchemaFormatNsUriLocal(&strA, uri, name));
    FREE_AND_NULL(strE)
    FREE_AND_NULL(strA)
}

/************************************************************************
 * 									*
 * 			Allocation functions				*
 * 									*
 ************************************************************************/

/**
 * xmlSchemaNewSchemaForParserCtxt:
 * @ctxt:  a schema validation context
 *
 * Allocate a new Schema structure.
 *
 * Returns the newly allocated structure or NULL in case or error
 */
static xmlSchemaPtr
xmlSchemaNewSchema(xmlSchemaParserCtxtPtr ctxt)
{
    xmlSchemaPtr ret;

    ret = (xmlSchemaPtr) xmlMalloc(sizeof(xmlSchema));
    if (ret == NULL) {
        xmlSchemaPErrMemory(ctxt, "allocating schema", NULL);
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchema));
    ret->dict = ctxt->dict;
    xmlDictReference(ret->dict);

    return (ret);
}

/**
 * xmlSchemaNewSchema:
 * @ctxt:  a schema validation context
 *
 * Allocate a new Schema structure.
 *
 * Returns the newly allocated structure or NULL in case or error
 */
static xmlSchemaAssemblePtr
xmlSchemaNewAssemble(void)
{
    xmlSchemaAssemblePtr ret;

    ret = (xmlSchemaAssemblePtr) xmlMalloc(sizeof(xmlSchemaAssemble));
    if (ret == NULL) {
        /* xmlSchemaPErrMemory(ctxt, "allocating assemble info", NULL); */
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaAssemble));
    ret->items = NULL;
    return (ret);
}

/**
 * xmlSchemaNewFacet:
 *
 * Allocate a new Facet structure.
 *
 * Returns the newly allocated structure or NULL in case or error
 */
xmlSchemaFacetPtr
xmlSchemaNewFacet(void)
{
    xmlSchemaFacetPtr ret;

    ret = (xmlSchemaFacetPtr) xmlMalloc(sizeof(xmlSchemaFacet));
    if (ret == NULL) {
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaFacet));

    return (ret);
}

/**
 * xmlSchemaNewAnnot:
 * @ctxt:  a schema validation context
 * @node:  a node
 *
 * Allocate a new annotation structure.
 *
 * Returns the newly allocated structure or NULL in case or error
 */
static xmlSchemaAnnotPtr
xmlSchemaNewAnnot(xmlSchemaParserCtxtPtr ctxt, xmlNodePtr node)
{
    xmlSchemaAnnotPtr ret;

    ret = (xmlSchemaAnnotPtr) xmlMalloc(sizeof(xmlSchemaAnnot));
    if (ret == NULL) {
        xmlSchemaPErrMemory(ctxt, "allocating annotation", node);
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaAnnot));
    ret->content = node;
    return (ret);
}

/**
 * xmlSchemaFreeAnnot:
 * @annot:  a schema type structure
 *
 * Deallocate a annotation structure
 */
static void
xmlSchemaFreeAnnot(xmlSchemaAnnotPtr annot)
{
    if (annot == NULL)
        return;
    xmlFree(annot);
}

/**
 * xmlSchemaFreeImport:
 * @import:  a schema import structure
 *
 * Deallocate an import structure
 */
static void
xmlSchemaFreeImport(xmlSchemaImportPtr import)
{
    if (import == NULL)
        return;

    xmlSchemaFree(import->schema);
    xmlFreeDoc(import->doc);
    xmlFree(import);
}

/**
 * xmlSchemaFreeInclude:
 * @include:  a schema include structure
 *
 * Deallocate an include structure
 */
static void
xmlSchemaFreeInclude(xmlSchemaIncludePtr include)
{
    if (include == NULL)
        return;

    xmlFreeDoc(include->doc);
    xmlFree(include);
}

/**
 * xmlSchemaFreeIncludeList:
 * @includes:  a schema include list
 *
 * Deallocate an include structure
 */
static void
xmlSchemaFreeIncludeList(xmlSchemaIncludePtr includes)
{
    xmlSchemaIncludePtr next;

    while (includes != NULL) {
        next = includes->next;
	xmlSchemaFreeInclude(includes);
	includes = next;
    }
}

/**
 * xmlSchemaFreeNotation:
 * @schema:  a schema notation structure
 *
 * Deallocate a Schema Notation structure.
 */
static void
xmlSchemaFreeNotation(xmlSchemaNotationPtr nota)
{
    if (nota == NULL)
        return;
    xmlFree(nota);
}

/**
 * xmlSchemaFreeAttribute:
 * @schema:  a schema attribute structure
 *
 * Deallocate a Schema Attribute structure.
 */
static void
xmlSchemaFreeAttribute(xmlSchemaAttributePtr attr)
{
    if (attr == NULL)
        return;
    if (attr->annot != NULL) 
	xmlSchemaFreeAnnot(attr->annot);
    if (attr->defVal != NULL)
	xmlSchemaFreeValue(attr->defVal);
    xmlFree(attr);
}

/**
 * xmlSchemaFreeWildcardNsSet:
 * set:  a schema wildcard namespace
 *
 * Deallocates a list of wildcard constraint structures.
 */
static void
xmlSchemaFreeWildcardNsSet(xmlSchemaWildcardNsPtr set)
{
    xmlSchemaWildcardNsPtr next;
    
    while (set != NULL) {
	next = set->next;
	xmlFree(set);
	set = next;
    }
}

/**
 * xmlSchemaFreeWildcard:
 * @wildcard:  a wildcard structure
 *
 * Deallocates a wildcard structure.
 */
void
xmlSchemaFreeWildcard(xmlSchemaWildcardPtr wildcard)
{
    if (wildcard == NULL)
        return;
    if (wildcard->annot != NULL)
        xmlSchemaFreeAnnot(wildcard->annot);
    if (wildcard->nsSet != NULL) 
	xmlSchemaFreeWildcardNsSet(wildcard->nsSet);    
    if (wildcard->negNsSet != NULL) 
	xmlFree(wildcard->negNsSet);    
    xmlFree(wildcard);
}

/**
 * xmlSchemaFreeAttributeGroup:
 * @schema:  a schema attribute group structure
 *
 * Deallocate a Schema Attribute Group structure.
 */
static void
xmlSchemaFreeAttributeGroup(xmlSchemaAttributeGroupPtr attr)
{
    if (attr == NULL)
        return;
    if (attr->annot != NULL)
        xmlSchemaFreeAnnot(attr->annot);
    xmlFree(attr);
}

/**
 * xmlSchemaFreeAttributeUseList:
 * @attrUse:  an attribute link
 *
 * Deallocate a list of schema attribute uses.
 */
static void
xmlSchemaFreeAttributeUseList(xmlSchemaAttributeLinkPtr attrUse)
{
    xmlSchemaAttributeLinkPtr next;

    while (attrUse != NULL) {
	next = attrUse->next;
	xmlFree(attrUse);
	attrUse = next;
    }    
}

/**
 * xmlSchemaFreeQNameRef:
 * @item: a QName reference structure
 *
 * Deallocatea a QName reference structure.
 */
static void
xmlSchemaFreeQNameRef(xmlSchemaQNameRefPtr item)
{
    xmlFree(item);
}

static int
xmlSchemaAddVolatile(xmlSchemaPtr schema,
		     xmlSchemaBasicItemPtr item)
{
    xmlSchemaItemListPtr list;
    
    if (schema->volatiles == NULL) {
	schema->volatiles = (void *) xmlMalloc(sizeof(xmlSchemaItemList));
	if (schema->volatiles == NULL) {
	    xmlSchemaPErrMemory(NULL,
		"allocating list of volatiles", NULL);
	    return (-1);
	}
	memset(schema->volatiles, 0, sizeof(xmlSchemaItemList));
	
    }
    list = (xmlSchemaItemListPtr) schema->volatiles;
    if (list->items == NULL) {
	list->items = (void **) xmlMalloc(
	    20 * sizeof(xmlSchemaBasicItemPtr));
	if (list->items == NULL) {
	    xmlSchemaPErrMemory(NULL,
		"allocating new volatile item buffer", NULL);
	    return (-1);
	}	
	list->sizeItems = 20;
    } else if (list->sizeItems <= list->nbItems) {
	list->sizeItems *= 2;
	list->items = (void **) xmlRealloc(list->items, 
	    list->sizeItems * sizeof(xmlSchemaTypePtr));
	if (list->items == NULL) {
	    xmlSchemaPErrMemory(NULL,
		"growing volatile item buffer", NULL);
	    list->sizeItems = 0;
	    return (-1);
	}	
    }
    ((xmlSchemaBasicItemPtr *) list->items)[list->nbItems++] = (void *) item;
    return (0);
}

/**
 * xmlSchemaFreeTypeLinkList:
 * @alink: a type link
 *
 * Deallocate a list of types.
 */
static void
xmlSchemaFreeTypeLinkList(xmlSchemaTypeLinkPtr link)
{
    xmlSchemaTypeLinkPtr next;

    while (link != NULL) {
	next = link->next;
	xmlFree(link);
	link = next;
    }    
}

static void
xmlSchemaFreeIDCStateObjList(xmlSchemaIDCStateObjPtr sto)
{
    xmlSchemaIDCStateObjPtr next;
    while (sto != NULL) {
	next = sto->next;
	if (sto->history != NULL)
	    xmlFree(sto->history);
	if (sto->xpathCtxt != NULL)
	    xmlFreeStreamCtxt((xmlStreamCtxtPtr) sto->xpathCtxt);
	xmlFree(sto);
	sto = next;
    }
}

/**
 * xmlSchemaFreeIDC:
 * @idc: a identity-constraint definition
 *
 * Deallocates an identity-constraint definition.
 */
static void
xmlSchemaFreeIDC(xmlSchemaIDCPtr idcDef)
{
    xmlSchemaIDCSelectPtr cur, prev;

    if (idcDef == NULL)
	return;
    if (idcDef->annot != NULL)
        xmlSchemaFreeAnnot(idcDef->annot);    
    /* Selector */
    if (idcDef->selector != NULL) {
	if (idcDef->selector->xpathComp != NULL)
	    xmlFreePattern((xmlPatternPtr) idcDef->selector->xpathComp);
	xmlFree(idcDef->selector);
    }
    /* Fields */
    if (idcDef->fields != NULL) {
	cur = idcDef->fields;
	do {
	    prev = cur;
	    cur = cur->next;
	    if (prev->xpathComp != NULL)
		xmlFreePattern((xmlPatternPtr) prev->xpathComp);
	    xmlFree(prev);	    
	} while (cur != NULL);
    }
    xmlFree(idcDef);
}

/**
 * xmlSchemaFreeElement:
 * @schema:  a schema element structure
 *
 * Deallocate a Schema Element structure.
 */
static void
xmlSchemaFreeElement(xmlSchemaElementPtr elem)
{
    if (elem == NULL)
        return;
    if (elem->annot != NULL)
        xmlSchemaFreeAnnot(elem->annot);
    if (elem->contModel != NULL)
        xmlRegFreeRegexp(elem->contModel);
    if (elem->defVal != NULL)
	xmlSchemaFreeValue(elem->defVal);
    xmlFree(elem);
}

/**
 * xmlSchemaFreeFacet:
 * @facet:  a schema facet structure
 *
 * Deallocate a Schema Facet structure.
 */
void
xmlSchemaFreeFacet(xmlSchemaFacetPtr facet)
{
    if (facet == NULL)
        return;
    if (facet->val != NULL)
        xmlSchemaFreeValue(facet->val);
    if (facet->regexp != NULL)
        xmlRegFreeRegexp(facet->regexp);
    if (facet->annot != NULL)
        xmlSchemaFreeAnnot(facet->annot);
    xmlFree(facet);
}

/**
 * xmlSchemaFreeType:
 * @type:  a schema type structure
 *
 * Deallocate a Schema Type structure.
 */
void
xmlSchemaFreeType(xmlSchemaTypePtr type)
{
    if (type == NULL)
        return;
    if (type->annot != NULL)
        xmlSchemaFreeAnnot(type->annot);
    if (type->facets != NULL) {
        xmlSchemaFacetPtr facet, next;

        facet = type->facets;
        while (facet != NULL) {
            next = facet->next;
            xmlSchemaFreeFacet(facet);
            facet = next;
        }
    }
    if (type->type != XML_SCHEMA_TYPE_BASIC) {
	if (type->attributeUses != NULL)
	    xmlSchemaFreeAttributeUseList(type->attributeUses);	
    }
    if (type->memberTypes != NULL)
	xmlSchemaFreeTypeLinkList(type->memberTypes);
    if (type->facetSet != NULL) {
	xmlSchemaFacetLinkPtr next, link;

	link = type->facetSet;
	do {
	    next = link->next;
	    xmlFree(link);
	    link = next;
	} while (link != NULL);
    }  
    if (type->contModel != NULL)
        xmlRegFreeRegexp(type->contModel);
    xmlFree(type);
}

/**
 * xmlSchemaFreeModelGroupDef:
 * @item:  a schema model group definition
 *
 * Deallocates a schema model group definition.
 */
static void
xmlSchemaFreeModelGroupDef(xmlSchemaModelGroupDefPtr item)
{
    if (item->annot != NULL)
	xmlSchemaFreeAnnot(item->annot);
    xmlFree(item);
}

/**
 * xmlSchemaFreeModelGroup:
 * @item:  a schema model group
 *
 * Deallocates a schema model group structure.
 */
static void
xmlSchemaFreeModelGroup(xmlSchemaModelGroupPtr item)
{
    if (item->annot != NULL)
	xmlSchemaFreeAnnot(item->annot);
    xmlFree(item);
}

/**
 * xmlSchemaFreeParticle:
 * @type:  a schema type structure
 *
 * Deallocate a Schema Type structure.
 */
static void
xmlSchemaFreeParticle(xmlSchemaParticlePtr item)
{
    if (item->annot != NULL)
	xmlSchemaFreeAnnot(item->annot);
    xmlFree(item);
}

/**
 * xmlSchemaFreeMiscComponents:
 * @item:  a schema component
 *
 * Deallocates misc. schema component structures.
 */
static void
xmlSchemaFreeMiscComponents(xmlSchemaTreeItemPtr item)
{
    if (item == NULL)
        return;
    switch (item->type) {
	case XML_SCHEMA_TYPE_PARTICLE:
	    xmlSchemaFreeParticle((xmlSchemaParticlePtr) item);
	    return;
	case XML_SCHEMA_TYPE_SEQUENCE:
	case XML_SCHEMA_TYPE_CHOICE:
	case XML_SCHEMA_TYPE_ALL:
	    xmlSchemaFreeModelGroup((xmlSchemaModelGroupPtr) item);
	    return;
	case XML_SCHEMA_TYPE_ANY:
	case XML_SCHEMA_TYPE_ANY_ATTRIBUTE:
	    xmlSchemaFreeWildcard((xmlSchemaWildcardPtr) item);
	    break;
	default:
	    /* TODO: This should never be hit. */
	    TODO
	    return;
    }	
}

static void
xmlSchemaFreeVolatiles(xmlSchemaPtr schema)
{
    if (schema->volatiles == NULL)
	return;
    {
	xmlSchemaItemListPtr list = (xmlSchemaItemListPtr) schema->volatiles;
	xmlSchemaTreeItemPtr item;
	int i;
	
	for (i = 0; i < list->nbItems; i++) {
	    if (list->items[i] != NULL) {
		item = (xmlSchemaTreeItemPtr) list->items[i];
		switch (item->type) {
		    case XML_SCHEMA_EXTRA_QNAMEREF:
			xmlSchemaFreeQNameRef((xmlSchemaQNameRefPtr) item);
			break;
		    default:
			xmlSchemaFreeMiscComponents(item);
		}
	    }
	}
	if (list->items != NULL)
	    xmlFree(list->items);
	xmlFree(list);
    }
}
/**
 * xmlSchemaFreeTypeList:
 * @type:  a schema type structure
 *
 * Deallocate a Schema Type structure.
 */
static void
xmlSchemaFreeTypeList(xmlSchemaTypePtr type)
{
    xmlSchemaTypePtr next;

    while (type != NULL) {
        next = type->redef;
	xmlSchemaFreeType(type);
	type = next;
    }
}

/**
 * xmlSchemaFree:
 * @schema:  a schema structure
 *
 * Deallocate a Schema structure.
 */
void
xmlSchemaFree(xmlSchemaPtr schema)
{
    if (schema == NULL)
        return;

    if (schema->volatiles != NULL)
	xmlSchemaFreeVolatiles(schema);
    if (schema->notaDecl != NULL)
        xmlHashFree(schema->notaDecl,
                    (xmlHashDeallocator) xmlSchemaFreeNotation);
    if (schema->attrDecl != NULL)
        xmlHashFree(schema->attrDecl,
                    (xmlHashDeallocator) xmlSchemaFreeAttribute);
    if (schema->attrgrpDecl != NULL)
        xmlHashFree(schema->attrgrpDecl,
                    (xmlHashDeallocator) xmlSchemaFreeAttributeGroup);
    if (schema->elemDecl != NULL)
        xmlHashFree(schema->elemDecl,
                    (xmlHashDeallocator) xmlSchemaFreeElement);
    if (schema->typeDecl != NULL)
        xmlHashFree(schema->typeDecl,
                    (xmlHashDeallocator) xmlSchemaFreeTypeList);
    if (schema->groupDecl != NULL)
        xmlHashFree(schema->groupDecl,
                    (xmlHashDeallocator) xmlSchemaFreeModelGroupDef);
    if (schema->idcDef != NULL)
        xmlHashFree(schema->idcDef,
                    (xmlHashDeallocator) xmlSchemaFreeIDC);
    if (schema->schemasImports != NULL)
	xmlHashFree(schema->schemasImports,
		    (xmlHashDeallocator) xmlSchemaFreeImport);
    if (schema->includes != NULL) {
        xmlSchemaFreeIncludeList((xmlSchemaIncludePtr) schema->includes);
    }
    if (schema->annot != NULL)
        xmlSchemaFreeAnnot(schema->annot);
    if (schema->doc != NULL && !schema->preserve)
        xmlFreeDoc(schema->doc);
    xmlDictFree(schema->dict);    
    xmlFree(schema);
}

/************************************************************************
 * 									*
 * 			Debug functions					*
 * 									*
 ************************************************************************/

#ifdef LIBXML_OUTPUT_ENABLED

/**
 * xmlSchemaElementDump:
 * @elem:  an element
 * @output:  the file output
 *
 * Dump the element
 */
static void
xmlSchemaElementDump(xmlSchemaElementPtr elem, FILE * output,
                     const xmlChar * name ATTRIBUTE_UNUSED,
		     const xmlChar * namespace ATTRIBUTE_UNUSED,
                     const xmlChar * context ATTRIBUTE_UNUSED)
{
    if (elem == NULL)
        return;

    if (elem->flags & XML_SCHEMAS_ELEM_REF) {
	fprintf(output, "Particle: %s", name);
	fprintf(output, ", term element: %s", elem->ref);
	if (elem->refNs != NULL)
	    fprintf(output, " ns %s", elem->refNs);
    } else { 
	fprintf(output, "Element");
	if (elem->flags & XML_SCHEMAS_ELEM_GLOBAL)
	    fprintf(output, " (global)");
	fprintf(output, ": %s ", elem->name);
	if (namespace != NULL)
	    fprintf(output, "ns %s", namespace);
    }
    fprintf(output, "\n");
    if ((elem->minOccurs != 1) || (elem->maxOccurs != 1)) {
	fprintf(output, "  min %d ", elem->minOccurs);
        if (elem->maxOccurs >= UNBOUNDED)
            fprintf(output, "max: unbounded\n");
        else if (elem->maxOccurs != 1)
            fprintf(output, "max: %d\n", elem->maxOccurs);
        else
            fprintf(output, "\n");
    }
    /*
    * Misc other properties.
    */
    if ((elem->flags & XML_SCHEMAS_ELEM_NILLABLE) ||
	(elem->flags & XML_SCHEMAS_ELEM_ABSTRACT) ||
	(elem->flags & XML_SCHEMAS_ELEM_FIXED) ||
	(elem->flags & XML_SCHEMAS_ELEM_DEFAULT) ||
	(elem->id != NULL)) {
	fprintf(output, "  props: ");
	if (elem->flags & XML_SCHEMAS_ELEM_FIXED)
	    fprintf(output, "[fixed] ");
	if (elem->flags & XML_SCHEMAS_ELEM_DEFAULT)
	    fprintf(output, "[default] ");
	if (elem->flags & XML_SCHEMAS_ELEM_ABSTRACT)
	    fprintf(output, "[abstract] ");
	if (elem->flags & XML_SCHEMAS_ELEM_NILLABLE)
	    fprintf(output, "[nillable] ");
	if (elem->id != NULL)
	    fprintf(output, "[id: '%s'] ", elem->id);
	fprintf(output, "\n");
    }
    /*
    * Default/fixed value.
    */
    if (elem->value != NULL)
	fprintf(output, "  value: '%s'\n", elem->value);
    /*
    * Type.
    */
    if (elem->namedType != NULL) {
	fprintf(output, "  type: %s ", elem->namedType);
	if (elem->namedTypeNs != NULL)
	    fprintf(output, "ns %s\n", elem->namedTypeNs);
	else
	    fprintf(output, "\n");
    }
    /*
    * Substitution group.
    */
    if (elem->substGroup != NULL) {
	fprintf(output, "  substitutionGroup: %s ", elem->substGroup);
	if (elem->substGroupNs != NULL)
	    fprintf(output, "ns %s\n", elem->substGroupNs);
	else
	    fprintf(output, "\n");
    }
}

/**
 * xmlSchemaAnnotDump:
 * @output:  the file output
 * @annot:  a annotation
 *
 * Dump the annotation
 */
static void
xmlSchemaAnnotDump(FILE * output, xmlSchemaAnnotPtr annot)
{
    xmlChar *content;

    if (annot == NULL)
        return;

    content = xmlNodeGetContent(annot->content);
    if (content != NULL) {
        fprintf(output, "  Annot: %s\n", content);
        xmlFree(content);
    } else
        fprintf(output, "  Annot: empty\n");
}

/**
 * xmlSchemaTypeDump:
 * @output:  the file output
 * @type:  a type structure
 *
 * Dump a SchemaType structure
 */
static void
xmlSchemaContentModelDump(xmlSchemaParticlePtr particle, FILE * output, int depth)
{
    xmlChar *str = NULL;
    xmlSchemaTreeItemPtr term;
    char shift[100];
    int i;
    
    if (particle == NULL)
	return;
    for (i = 0;((i < depth) && (i < 25));i++)
        shift[2 * i] = shift[2 * i + 1] = ' ';
    shift[2 * i] = shift[2 * i + 1] = 0;
    fprintf(output, shift);
    if (particle->children == NULL) {
	fprintf(output, "MISSING particle term\n");
	return;
    }
    term = particle->children;
    switch (term->type) {
	case XML_SCHEMA_TYPE_ELEMENT:
	    fprintf(output, "ELEM '%s'", xmlSchemaFormatQName(&str, 
		((xmlSchemaElementPtr)term)->targetNamespace,
		((xmlSchemaElementPtr)term)->name));
	    break;
	case XML_SCHEMA_TYPE_SEQUENCE:
	    fprintf(output, "SEQUENCE");
	    break;
	case XML_SCHEMA_TYPE_CHOICE:
	    fprintf(output, "CHOICE");
	    break;
	case XML_SCHEMA_TYPE_ALL:
	    fprintf(output, "ALL");
	    break;
	case XML_SCHEMA_TYPE_ANY:
	    fprintf(output, "ANY");
	    break;
	default:
	    fprintf(output, "UNKNOWN\n");
	    return;
    }
    if (particle->minOccurs != 1)
	fprintf(output, " min: %d", particle->minOccurs);
    if (particle->maxOccurs >= UNBOUNDED)
	fprintf(output, " max: unbounded");
    else if (particle->maxOccurs != 1)
	fprintf(output, " max: %d", particle->maxOccurs);
    fprintf(output, "\n");
    if (((term->type == XML_SCHEMA_TYPE_SEQUENCE) ||
	(term->type == XML_SCHEMA_TYPE_CHOICE) ||
	(term->type == XML_SCHEMA_TYPE_ALL)) &&
	(term->children != NULL)) {
	xmlSchemaContentModelDump((xmlSchemaParticlePtr) term->children,
	    output, depth +1);
    }
    if (particle->next != NULL)
	xmlSchemaContentModelDump((xmlSchemaParticlePtr) particle->next,
		output, depth);
}
/**
 * xmlSchemaTypeDump:
 * @output:  the file output
 * @type:  a type structure
 *
 * Dump a SchemaType structure
 */
static void
xmlSchemaTypeDump(xmlSchemaTypePtr type, FILE * output)
{
    if (type == NULL) {
        fprintf(output, "Type: NULL\n");
        return;
    }
    fprintf(output, "Type: ");
    if (type->name != NULL)
        fprintf(output, "%s ", type->name);
    else
        fprintf(output, "no name ");
    if (type->targetNamespace != NULL)
	fprintf(output, "ns %s ", type->targetNamespace);
    switch (type->type) {
        case XML_SCHEMA_TYPE_BASIC:
            fprintf(output, "[basic] ");
            break;
        case XML_SCHEMA_TYPE_SIMPLE:
            fprintf(output, "[simple] ");
            break;
        case XML_SCHEMA_TYPE_COMPLEX:
            fprintf(output, "[complex] ");
            break;
        case XML_SCHEMA_TYPE_SEQUENCE:
            fprintf(output, "[sequence] ");
            break;
        case XML_SCHEMA_TYPE_CHOICE:
            fprintf(output, "[choice] ");
            break;
        case XML_SCHEMA_TYPE_ALL:
            fprintf(output, "[all] ");
            break;
        case XML_SCHEMA_TYPE_UR:
            fprintf(output, "[ur] ");
            break;
        case XML_SCHEMA_TYPE_RESTRICTION:
            fprintf(output, "[restriction] ");
            break;
        case XML_SCHEMA_TYPE_EXTENSION:
            fprintf(output, "[extension] ");
            break;
        default:
            fprintf(output, "[unknown type %d] ", type->type);
            break;
    }    
    fprintf(output, "content: ");
    switch (type->contentType) {
        case XML_SCHEMA_CONTENT_UNKNOWN:
            fprintf(output, "[unknown] ");
            break;
        case XML_SCHEMA_CONTENT_EMPTY:
            fprintf(output, "[empty] ");
            break;
        case XML_SCHEMA_CONTENT_ELEMENTS:
            fprintf(output, "[element] ");
            break;
        case XML_SCHEMA_CONTENT_MIXED:
            fprintf(output, "[mixed] ");
            break;
        case XML_SCHEMA_CONTENT_MIXED_OR_ELEMENTS:
	/* not used. */
            break;
        case XML_SCHEMA_CONTENT_BASIC:
            fprintf(output, "[basic] ");
            break;
        case XML_SCHEMA_CONTENT_SIMPLE:
            fprintf(output, "[simple] ");
            break;
        case XML_SCHEMA_CONTENT_ANY:
            fprintf(output, "[any] ");
            break;
    }
    fprintf(output, "\n");
    if (type->base != NULL) {
        fprintf(output, "  base type: %s", type->base);
	if (type->baseNs != NULL)
	    fprintf(output, " ns %s\n", type->baseNs);
	else
	    fprintf(output, "\n");
    }
    if (type->annot != NULL)
        xmlSchemaAnnotDump(output, type->annot);
#ifdef DUMP_CONTENT_MODEL
    if ((type->type == XML_SCHEMA_TYPE_COMPLEX) &&
	(type->subtypes != NULL)) {
	xmlSchemaContentModelDump((xmlSchemaParticlePtr) type->subtypes,
	    output, 1);
    }
#endif
}

/**
 * xmlSchemaDump:
 * @output:  the file output
 * @schema:  a schema structure
 *
 * Dump a Schema structure.
 */
void
xmlSchemaDump(FILE * output, xmlSchemaPtr schema)
{
    if (output == NULL)
        return;
    if (schema == NULL) {
        fprintf(output, "Schemas: NULL\n");
        return;
    }
    fprintf(output, "Schemas: ");
    if (schema->name != NULL)
        fprintf(output, "%s, ", schema->name);
    else
        fprintf(output, "no name, ");
    if (schema->targetNamespace != NULL)
        fprintf(output, "%s", (const char *) schema->targetNamespace);
    else
        fprintf(output, "no target namespace");
    fprintf(output, "\n");
    if (schema->annot != NULL)
        xmlSchemaAnnotDump(output, schema->annot);

    xmlHashScan(schema->typeDecl, (xmlHashScanner) xmlSchemaTypeDump,
                output);
    xmlHashScanFull(schema->elemDecl,
                    (xmlHashScannerFull) xmlSchemaElementDump, output);
}

#ifdef DEBUG_IDC
/**
 * xmlSchemaDebugDumpIDCTable: 
 * @vctxt: the WXS validation context
 *
 * Displays the current IDC table for debug purposes.
 */
static void
xmlSchemaDebugDumpIDCTable(FILE * output,
			   const xmlChar *namespaceName,
			   const xmlChar *localName,
			   xmlSchemaPSVIIDCBindingPtr bind)
{
    xmlChar *str = NULL, *value;    
    xmlSchemaPSVIIDCNodePtr tab;
    xmlSchemaPSVIIDCKeyPtr key;
    int i, j, res;
    
    fprintf(output, "IDC: TABLES on %s\n", 
	xmlSchemaFormatNsUriLocal(&str, namespaceName, localName));
    FREE_AND_NULL(str)

    if (bind == NULL)
	return;
    do {
	fprintf(output, "IDC:   BINDING %s\n", 
	    xmlSchemaFormatNsUriLocal(&str, bind->definition->targetNamespace,
	    bind->definition->name));
	FREE_AND_NULL(str)	
	for (i = 0; i < bind->nbNodes; i++) {
	    tab = bind->nodeTable[i];
	    fprintf(output, "         ( ");
	    for (j = 0; j < bind->definition->nbFields; j++) {
		key = tab->keys[j];		
		if ((key != NULL) && (key->compValue != NULL)) {
		    res = xmlSchemaGetCanonValue(key->compValue, &value);
		    if (res >= 0)
			fprintf(output, "\"%s\" ", value);
		    else
			fprintf(output, "CANON-VALUE-FAILED ");
		    if (res == 0) 
			FREE_AND_NULL(value)
		} else if (key != NULL)
		    fprintf(output, "(no val), ");
		else
		    fprintf(output, "(key missing), ");
	    }
	    fprintf(output, ")\n");
	}
	bind = bind->next;
    } while (bind != NULL);
}
#endif /* DEBUG_IDC */
#endif /* LIBXML_OUTPUT_ENABLED */

/************************************************************************
 *									*
 * 			Utilities					*
 *									*
 ************************************************************************/

/**
 * xmlSchemaGetPropNode:
 * @node: the element node 
 * @name: the name of the attribute
 *
 * Seeks an attribute with a name of @name in
 * no namespace.
 *
 * Returns the attribute or NULL if not present. 
 */
static xmlAttrPtr
xmlSchemaGetPropNode(xmlNodePtr node, const char *name) 
{
    xmlAttrPtr prop;

    if ((node == NULL) || (name == NULL)) 
	return(NULL);
    prop = node->properties;
    while (prop != NULL) {
        if ((prop->ns == NULL) && xmlStrEqual(prop->name, BAD_CAST name))	    
	    return(prop);
	prop = prop->next;
    }
    return (NULL);
}

/**
 * xmlSchemaGetPropNodeNs:
 * @node: the element node 
 * @uri: the uri
 * @name: the name of the attribute
 *
 * Seeks an attribute with a local name of @name and
 * a namespace URI of @uri.
 *
 * Returns the attribute or NULL if not present. 
 */
static xmlAttrPtr
xmlSchemaGetPropNodeNs(xmlNodePtr node, const char *uri, const char *name) 
{
    xmlAttrPtr prop;

    if ((node == NULL) || (name == NULL)) 
	return(NULL);
    prop = node->properties;
    while (prop != NULL) {
	if ((prop->ns != NULL) &&
	    xmlStrEqual(prop->name, BAD_CAST name) &&
	    xmlStrEqual(prop->ns->href, BAD_CAST uri))
	    return(prop);
	prop = prop->next;
    }
    return (NULL);
}

static const xmlChar *
xmlSchemaGetNodeContent(xmlSchemaParserCtxtPtr ctxt, xmlNodePtr node)
{
    xmlChar *val;
    const xmlChar *ret;

    val = xmlNodeGetContent(node);
    if (val == NULL)
        return(NULL);
    ret = xmlDictLookup(ctxt->dict, val, -1);
    xmlFree(val);
    return(ret);    
}

/**
 * xmlSchemaGetProp:
 * @ctxt: the parser context
 * @node: the node
 * @name: the property name
 * 
 * Read a attribute value and internalize the string
 *
 * Returns the string or NULL if not present.
 */
static const xmlChar *
xmlSchemaGetProp(xmlSchemaParserCtxtPtr ctxt, xmlNodePtr node,
                 const char *name)
{
    xmlChar *val;
    const xmlChar *ret;

    val = xmlGetProp(node, BAD_CAST name);
    if (val == NULL)
        return(NULL);
    ret = xmlDictLookup(ctxt->dict, val, -1);
    xmlFree(val);
    return(ret);
}

/************************************************************************
 * 									*
 * 			Parsing functions				*
 * 									*
 ************************************************************************/

/**
 * xmlSchemaGetElem:
 * @schema:  the schema context
 * @name:  the element name
 * @ns:  the element namespace
 *
 * Lookup a global element declaration in the schema.
 *
 * Returns the element declaration or NULL if not found.
 */
static xmlSchemaElementPtr
xmlSchemaGetElem(xmlSchemaPtr schema, const xmlChar * name,
                 const xmlChar * namespace)
{
    xmlSchemaElementPtr ret;

    if ((name == NULL) || (schema == NULL))
        return (NULL);
        
        ret = xmlHashLookup2(schema->elemDecl, name, namespace);
        if ((ret != NULL) &&
	    (ret->flags & XML_SCHEMAS_ELEM_GLOBAL)) {
            return (ret);
    } else
	ret = NULL;
    /*
     * This one was removed, since top level element declarations have
     * the target namespace specified in targetNamespace of the <schema>
     * information element, even if elementFormDefault is "unqualified".
     */
    
    /* else if ((schema->flags & XML_SCHEMAS_QUALIF_ELEM) == 0) {
        if (xmlStrEqual(namespace, schema->targetNamespace))
	    ret = xmlHashLookup2(schema->elemDecl, name, NULL);
	else
	    ret = xmlHashLookup2(schema->elemDecl, name, namespace);
        if ((ret != NULL) &&
	    ((level == 0) || (ret->flags & XML_SCHEMAS_ELEM_TOPLEVEL))) {
            return (ret);
	}
    */
    
    /*
    * Removed since imported components will be hold by the main schema only.
    *
    if (namespace == NULL)
	import = xmlHashLookup(schema->schemasImports, XML_SCHEMAS_NO_NAMESPACE);
    else
    import = xmlHashLookup(schema->schemasImports, namespace);
    if (import != NULL) {
	ret = xmlSchemaGetElem(import->schema, name, namespace, level + 1);
	if ((ret != NULL) && (ret->flags & XML_SCHEMAS_ELEM_GLOBAL)) {
	    return (ret);
	} else
	    ret = NULL;
    }
    */
#ifdef DEBUG
    if (ret == NULL) {
        if (namespace == NULL)
            fprintf(stderr, "Unable to lookup element decl. %s", name);
        else
            fprintf(stderr, "Unable to lookup element decl. %s:%s", name,
                    namespace);
    }
#endif
    return (ret);
}

/**
 * xmlSchemaGetType:
 * @schema:  the schemas context
 * @name:  the type name
 * @ns:  the type namespace
 *
 * Lookup a type in the schemas or the predefined types
 *
 * Returns the group definition or NULL if not found.
 */
static xmlSchemaTypePtr
xmlSchemaGetType(xmlSchemaPtr schema, const xmlChar * name,
                 const xmlChar * namespace)
{
    xmlSchemaTypePtr ret;

    if (name == NULL)
        return (NULL);
    if (schema != NULL) {
        ret = xmlHashLookup2(schema->typeDecl, name, namespace);
        if ((ret != NULL) && (ret->flags & XML_SCHEMAS_TYPE_GLOBAL))
            return (ret);
    }
    ret = xmlSchemaGetPredefinedType(name, namespace);
    if (ret != NULL)
	return (ret);
    /*
    * Removed, since the imported components will be grafted on the
    * main schema only.    
    if (namespace == NULL)
	import = xmlHashLookup(schema->schemasImports, XML_SCHEMAS_NO_NAMESPACE);
    else
    import = xmlHashLookup(schema->schemasImports, namespace);
    if (import != NULL) {
	ret = xmlSchemaGetType(import->schema, name, namespace);
	if ((ret != NULL) && (ret->flags & XML_SCHEMAS_TYPE_GLOBAL)) {
	    return (ret);
	} else
	    ret = NULL;
    }
    */
#ifdef DEBUG
    if (ret == NULL) {
        if (namespace == NULL)
            fprintf(stderr, "Unable to lookup type %s", name);
        else
            fprintf(stderr, "Unable to lookup type %s:%s", name,
                    namespace);
    }
#endif
    return (ret);
}

/**
 * xmlSchemaGetAttribute:
 * @schema:  the context of the schema 
 * @name:  the name of the attribute
 * @ns:  the target namespace of the attribute 
 *
 * Lookup a an attribute in the schema or imported schemas
 *
 * Returns the attribute declaration or NULL if not found.
 */
static xmlSchemaAttributePtr
xmlSchemaGetAttribute(xmlSchemaPtr schema, const xmlChar * name,
                 const xmlChar * namespace)
{
    xmlSchemaAttributePtr ret;

    if ((name == NULL) || (schema == NULL))
        return (NULL);
    
    
    ret = xmlHashLookup2(schema->attrDecl, name, namespace);
    if ((ret != NULL) && (ret->flags & XML_SCHEMAS_ATTR_GLOBAL))
	return (ret); 
    else
	ret = NULL;
    /*
    * Removed, since imported components will be hold by the main schema only.
    *
    if (namespace == NULL)
	import = xmlHashLookup(schema->schemasImports, XML_SCHEMAS_NO_NAMESPACE);
    else
	import = xmlHashLookup(schema->schemasImports, namespace);	
    if (import != NULL) {
	ret = xmlSchemaGetAttribute(import->schema, name, namespace);
	if ((ret != NULL) && (ret->flags & XML_SCHEMAS_ATTR_GLOBAL)) {
	    return (ret);
	} else
	    ret = NULL;
    }
    */
#ifdef DEBUG
    if (ret == NULL) {
        if (namespace == NULL)
            fprintf(stderr, "Unable to lookup attribute %s", name);
        else
            fprintf(stderr, "Unable to lookup attribute %s:%s", name,
                    namespace);
    }
#endif
    return (ret);
}

/**
 * xmlSchemaGetAttributeGroup:
 * @schema:  the context of the schema 
 * @name:  the name of the attribute group
 * @ns:  the target namespace of the attribute group 
 *
 * Lookup a an attribute group in the schema or imported schemas
 *
 * Returns the attribute group definition or NULL if not found.
 */
static xmlSchemaAttributeGroupPtr
xmlSchemaGetAttributeGroup(xmlSchemaPtr schema, const xmlChar * name,
                 const xmlChar * namespace)
{
    xmlSchemaAttributeGroupPtr ret;

    if ((name == NULL) || (schema == NULL))
        return (NULL);
    
    
    ret = xmlHashLookup2(schema->attrgrpDecl, name, namespace);
    if ((ret != NULL) && (ret->flags & XML_SCHEMAS_ATTRGROUP_GLOBAL))
	return (ret);  
    else
	ret = NULL;
    /*
    * Removed since imported components will be hold by the main schema only.
    *
    if (namespace == NULL)
	import = xmlHashLookup(schema->schemasImports, XML_SCHEMAS_NO_NAMESPACE);
    else
	import = xmlHashLookup(schema->schemasImports, namespace);	
    if (import != NULL) {
	ret = xmlSchemaGetAttributeGroup(import->schema, name, namespace);
	if ((ret != NULL) && (ret->flags & XML_SCHEMAS_ATTRGROUP_GLOBAL))
	    return (ret);
	else
	    ret = NULL;
    }
    */
#ifdef DEBUG
    if (ret == NULL) {
        if (namespace == NULL)
            fprintf(stderr, "Unable to lookup attribute group %s", name);
        else
            fprintf(stderr, "Unable to lookup attribute group %s:%s", name,
                    namespace);
    }
#endif
    return (ret);
}

/**
 * xmlSchemaGetGroup:
 * @schema:  the context of the schema 
 * @name:  the name of the group
 * @ns:  the target namespace of the group 
 *
 * Lookup a group in the schema or imported schemas
 *
 * Returns the group definition or NULL if not found.
 */
static xmlSchemaTypePtr
xmlSchemaGetGroup(xmlSchemaPtr schema, const xmlChar * name,
                 const xmlChar * namespace)
{
    xmlSchemaTypePtr ret;

    if ((name == NULL) || (schema == NULL))
        return (NULL);
    
    ret = xmlHashLookup2(schema->groupDecl, name, namespace);
    /*
    * Removed since imported components will be hold by the main schema only.
    *
    if (namespace == NULL)
	import = xmlHashLookup(schema->schemasImports, XML_SCHEMAS_NO_NAMESPACE);
    else
	import = xmlHashLookup(schema->schemasImports, namespace);	
    if (import != NULL) {
	ret = xmlSchemaGetGroup(import->schema, name, namespace);
	if ((ret != NULL) && (ret->flags & XML_SCHEMAS_TYPE_GLOBAL))
	    return (ret);
	else
	    ret = NULL;
    }
    */
#ifdef DEBUG
    if (ret == NULL) {
        if (namespace == NULL)
            fprintf(stderr, "Unable to lookup group %s", name);
        else
            fprintf(stderr, "Unable to lookup group %s:%s", name,
                    namespace);
    }
#endif
    return (ret);
}

/**
 * xmlSchemaGetNamedComponent:
 * @schema:  the schema 
 * @name:  the name of the group
 * @ns:  the target namespace of the group 
 *
 * Lookup a group in the schema or imported schemas
 *
 * Returns the group definition or NULL if not found.
 */
static xmlSchemaTreeItemPtr
xmlSchemaGetNamedComponent(xmlSchemaPtr schema,
			   xmlSchemaTypeType itemType,
			   const xmlChar *name,
			   const xmlChar *targetNs)
{
    switch (itemType) {
	case XML_SCHEMA_TYPE_GROUP:
	    return ((xmlSchemaTreeItemPtr) xmlSchemaGetGroup(schema,
		name, targetNs));
	case XML_SCHEMA_TYPE_ELEMENT:
	    return ((xmlSchemaTreeItemPtr) xmlSchemaGetElem(schema,
		name, targetNs));
	default:
	    return (NULL);
    }
}

/************************************************************************
 * 									*
 * 			Parsing functions				*
 * 									*
 ************************************************************************/

#define IS_BLANK_NODE(n)						\
    (((n)->type == XML_TEXT_NODE) && (xmlSchemaIsBlank((n)->content)))

/**
 * xmlSchemaIsBlank:
 * @str:  a string
 *
 * Check if a string is ignorable
 *
 * Returns 1 if the string is NULL or made of blanks chars, 0 otherwise
 */
static int
xmlSchemaIsBlank(xmlChar * str)
{
    if (str == NULL)
        return (1);
    while (*str != 0) {
        if (!(IS_BLANK_CH(*str)))
            return (0);
        str++;
    }
    return (1);
}

/**
 * xmlSchemaAddAssembledItem:
 * @ctxt:  a schema parser context
 * @schema:  the schema being built
 * @item:  the item
 *
 * Add a item to the schema's list of current items.
 * This is used if the schema was already constructed and
 * new schemata need to be added to it.
 * *WARNING* this interface is highly subject to change.
 *
 * Returns 0 if suceeds and -1 if an internal error occurs.
 */
static int
xmlSchemaAddAssembledItem(xmlSchemaParserCtxtPtr ctxt,
			   xmlSchemaTypePtr item)
{
    static int growSize = 100;
    xmlSchemaAssemblePtr ass;

    ass = ctxt->assemble;
    if (ass->sizeItems < 0) {
	/* If disabled. */
	return (0);
    }
    if (ass->sizeItems <= 0) {
	ass->items = (void **) xmlMalloc(growSize * sizeof(xmlSchemaTypePtr));
	if (ass->items == NULL) {
	    xmlSchemaPErrMemory(ctxt,
		"allocating new item buffer", NULL);
	    return (-1);
	}	
	ass->sizeItems = growSize;
    } else if (ass->sizeItems <= ass->nbItems) {
	ass->sizeItems *= 2;
	ass->items = (void **) xmlRealloc(ass->items, 
	    ass->sizeItems * sizeof(xmlSchemaTypePtr));
	if (ass->items == NULL) {
	    xmlSchemaPErrMemory(ctxt,
		"growing item buffer", NULL);
	    ass->sizeItems = 0;
	    return (-1);
	}	
    }
    /* ass->items[ass->nbItems++] = (void *) item; */
    ((xmlSchemaTypePtr *) ass->items)[ass->nbItems++] = (void *) item;
    return (0);
}

/**
 * xmlSchemaAddNotation:
 * @ctxt:  a schema parser context
 * @schema:  the schema being built
 * @name:  the item name
 *
 * Add an XML schema annotation declaration
 * *WARNING* this interface is highly subject to change
 *
 * Returns the new struture or NULL in case of error
 */
static xmlSchemaNotationPtr
xmlSchemaAddNotation(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                     const xmlChar *name)
{
    xmlSchemaNotationPtr ret = NULL;
    int val;

    if ((ctxt == NULL) || (schema == NULL) || (name == NULL))
        return (NULL);

    if (schema->notaDecl == NULL)
        schema->notaDecl = xmlHashCreateDict(10, ctxt->dict);
    if (schema->notaDecl == NULL)
        return (NULL);

    ret = (xmlSchemaNotationPtr) xmlMalloc(sizeof(xmlSchemaNotation));
    if (ret == NULL) {
        xmlSchemaPErrMemory(ctxt, "add annotation", NULL);
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaNotation));
    ret->name = xmlDictLookup(ctxt->dict, name, -1);
    val = xmlHashAddEntry2(schema->notaDecl, name, schema->targetNamespace,
                           ret);
    if (val != 0) {
	/*
	* TODO: This should never happen, since a unique name will be computed.
	* If it fails, then an other internal error must have occured.
	*/
	xmlSchemaPErr(ctxt, (xmlNodePtr) ctxt->doc,
		      XML_SCHEMAP_REDEFINED_NOTATION,
                      "Annotation declaration '%s' is already declared.\n",
                      name, NULL);
        xmlFree(ret);
        return (NULL);
    }
    return (ret);
}


/**
 * xmlSchemaAddAttribute:
 * @ctxt:  a schema parser context
 * @schema:  the schema being built
 * @name:  the item name
 * @namespace:  the namespace
 *
 * Add an XML schema Attrribute declaration
 * *WARNING* this interface is highly subject to change
 *
 * Returns the new struture or NULL in case of error
 */
static xmlSchemaAttributePtr
xmlSchemaAddAttribute(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                      const xmlChar * name, const xmlChar * namespace,
		      xmlNodePtr node, int topLevel)
{
    xmlSchemaAttributePtr ret = NULL;
    int val;

    if ((ctxt == NULL) || (schema == NULL) || (name == NULL))
        return (NULL);

#ifdef DEBUG
    fprintf(stderr, "Adding attribute %s\n", name);
    if (namespace != NULL)
	fprintf(stderr, "  target namespace %s\n", namespace);
#endif

    if (schema->attrDecl == NULL)
        schema->attrDecl = xmlHashCreateDict(10, ctxt->dict);
    if (schema->attrDecl == NULL)
        return (NULL);

    ret = (xmlSchemaAttributePtr) xmlMalloc(sizeof(xmlSchemaAttribute));
    if (ret == NULL) {
        xmlSchemaPErrMemory(ctxt, "allocating attribute", NULL);
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaAttribute));
    ret->name = xmlDictLookup(ctxt->dict, name, -1);
    ret->targetNamespace = namespace;
    val = xmlHashAddEntry3(schema->attrDecl, name,
                           namespace, ctxt->container, ret);
    if (val != 0) {
	if (topLevel) {
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_REDEFINED_ATTR,
		NULL, NULL, node,
		"A global attribute declaration with the name '%s' does "
		"already exist", name);
	    xmlFree(ret);	    
	    return (NULL);
	} else {
	    char buf[30];
	    /*
	    * Using the ctxt->container for xmlHashAddEntry3 is ambigious
	    * in the scenario:
	    * 1. multiple top-level complex types have different target
	    *    namespaces but have the SAME NAME; this can happen if
	    *	 schemata are  imported
	    * 2. those complex types contain attributes with an equal name
	    * 3. those attributes are in no namespace 
	    * We will compute a new context string.
	    */	    
	    snprintf(buf, 29, "#aCont%d", ctxt->counter++ + 1);
	    val = xmlHashAddEntry3(schema->attrDecl, name,
		namespace, xmlDictLookup(ctxt->dict, BAD_CAST buf, -1), ret);

	    if (val != 0) {
		xmlSchemaPCustomErr(ctxt,
		    XML_SCHEMAP_INTERNAL,
		    NULL, NULL, node,
		    "Internal error: xmlSchemaAddAttribute, "
		    "a dublicate attribute declaration with the name '%s' "
		    "could not be added to the hash.", name);
		xmlFree(ret);
		return (NULL);
	    }	    
	}
    }
    if (ctxt->assemble != NULL)
	xmlSchemaAddAssembledItem(ctxt, (xmlSchemaTypePtr) ret); 
    return (ret);
}

/**
 * xmlSchemaAddAttributeGroup:
 * @ctxt:  a schema parser context
 * @schema:  the schema being built
 * @name:  the item name
 *
 * Add an XML schema Attrribute Group declaration
 *
 * Returns the new struture or NULL in case of error
 */
static xmlSchemaAttributeGroupPtr
xmlSchemaAddAttributeGroup(xmlSchemaParserCtxtPtr ctxt,
                           xmlSchemaPtr schema, const xmlChar * name,
			   xmlNodePtr node)
{
    xmlSchemaAttributeGroupPtr ret = NULL;
    int val;

    if ((ctxt == NULL) || (schema == NULL) || (name == NULL))
        return (NULL);

    if (schema->attrgrpDecl == NULL)
        schema->attrgrpDecl = xmlHashCreateDict(10, ctxt->dict);
    if (schema->attrgrpDecl == NULL)
        return (NULL);

    ret =
        (xmlSchemaAttributeGroupPtr)
        xmlMalloc(sizeof(xmlSchemaAttributeGroup));
    if (ret == NULL) {
        xmlSchemaPErrMemory(ctxt, "allocating attribute group", NULL);
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaAttributeGroup));
    ret->name = xmlDictLookup(ctxt->dict, name, -1);
    val = xmlHashAddEntry3(schema->attrgrpDecl, name,
                           schema->targetNamespace, ctxt->container, ret);
    if (val != 0) {
	xmlSchemaPCustomErr(ctxt,
	    XML_SCHEMAP_REDEFINED_ATTRGROUP,
	    NULL, NULL, node,
	    "A global attribute group definition with the name '%s' does already exist", name);
        xmlFree(ret);
        return (NULL);
    }
    if (ctxt->assemble != NULL)	
	xmlSchemaAddAssembledItem(ctxt, (xmlSchemaTypePtr) ret);
    return (ret);
}

/**
 * xmlSchemaAddElement:
 * @ctxt:  a schema parser context
 * @schema:  the schema being built
 * @name:  the type name
 * @namespace:  the type namespace
 *
 * Add an XML schema Element declaration
 * *WARNING* this interface is highly subject to change
 *
 * Returns the new struture or NULL in case of error
 */
static xmlSchemaElementPtr
xmlSchemaAddElement(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                    const xmlChar * name, const xmlChar * namespace,
		    xmlNodePtr node, int topLevel)
{
    xmlSchemaElementPtr ret = NULL;
    int val;

    if ((ctxt == NULL) || (schema == NULL) || (name == NULL))
        return (NULL);

#ifdef DEBUG
    fprintf(stderr, "Adding element %s\n", name);
    if (namespace != NULL)
	fprintf(stderr, "  target namespace %s\n", namespace);
#endif

    if (schema->elemDecl == NULL)
        schema->elemDecl = xmlHashCreateDict(10, ctxt->dict);
    if (schema->elemDecl == NULL)
        return (NULL);

    ret = (xmlSchemaElementPtr) xmlMalloc(sizeof(xmlSchemaElement));
    if (ret == NULL) {
        xmlSchemaPErrMemory(ctxt, "allocating element", NULL);
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaElement));
    ret->name = xmlDictLookup(ctxt->dict, name, -1);
    val = xmlHashAddEntry3(schema->elemDecl, name,
                           namespace, ctxt->container, ret);
    if (val != 0) {
	if (topLevel) {
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_REDEFINED_ELEMENT,
		NULL, NULL, node,
		"A global element declaration with the name '%s' does "
		"already exist", name);
            xmlFree(ret);
            return (NULL);
	} else {
	    char buf[30]; 

	    snprintf(buf, 29, "#eCont%d", ctxt->counter++ + 1);
	    val = xmlHashAddEntry3(schema->elemDecl, name, 
		namespace, (xmlChar *) buf, ret);
	    if (val != 0) {
		xmlSchemaPCustomErr(ctxt,
		    XML_SCHEMAP_INTERNAL,
		    NULL, NULL, node,
		    "Internal error: xmlSchemaAddElement, "
		    "a dublicate element declaration with the name '%s' "
		    "could not be added to the hash.", name);
		xmlFree(ret);
		return (NULL);
	    }
	}
        
    }
    if (ctxt->assemble != NULL)	
	xmlSchemaAddAssembledItem(ctxt, (xmlSchemaTypePtr) ret);
    return (ret);
}

/**
 * xmlSchemaAddType:
 * @ctxt:  a schema parser context
 * @schema:  the schema being built
 * @name:  the item name
 * @namespace:  the namespace
 *
 * Add an XML schema item
 * *WARNING* this interface is highly subject to change
 *
 * Returns the new struture or NULL in case of error
 */
static xmlSchemaTypePtr
xmlSchemaAddType(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                 const xmlChar * name, const xmlChar * namespace,
		 xmlNodePtr node)
{
    xmlSchemaTypePtr ret = NULL;
    int val;

    if ((ctxt == NULL) || (schema == NULL) || (name == NULL))
        return (NULL);

#ifdef DEBUG
    fprintf(stderr, "Adding type %s\n", name);
    if (namespace != NULL)
	fprintf(stderr, "  target namespace %s\n", namespace);
#endif

    if (schema->typeDecl == NULL)
        schema->typeDecl = xmlHashCreateDict(10, ctxt->dict);
    if (schema->typeDecl == NULL)
        return (NULL);

    ret = (xmlSchemaTypePtr) xmlMalloc(sizeof(xmlSchemaType));
    if (ret == NULL) {
        xmlSchemaPErrMemory(ctxt, "allocating type", NULL);
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaType));
    ret->name = xmlDictLookup(ctxt->dict, name, -1);
    ret->redef = NULL;
    val = xmlHashAddEntry2(schema->typeDecl, name, namespace, ret);
    if (val != 0) {	
        if (ctxt->includes == 0) {	    
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_REDEFINED_TYPE,
		NULL, NULL, node, 
		"A global type definition with the name '%s' does already exist", name);            	    
	    xmlFree(ret);
	    return (NULL);
	} else {
	    xmlSchemaTypePtr prev;

	    prev = xmlHashLookup2(schema->typeDecl, name, namespace);
	    if (prev == NULL) {
		xmlSchemaPErr(ctxt, (xmlNodePtr) ctxt->doc,
		    XML_ERR_INTERNAL_ERROR,
		    "Internal error: xmlSchemaAddType, on type "
		    "'%s'.\n",
		    name, NULL);
		xmlFree(ret);
		return (NULL);
	    }
	    ret->redef = prev->redef;
	    prev->redef = ret;
	}
    }
    ret->node = node;
    ret->minOccurs = 1;
    ret->maxOccurs = 1;
    ret->attributeUses = NULL;
    ret->attributeWildcard = NULL;
    if (ctxt->assemble != NULL)	
	xmlSchemaAddAssembledItem(ctxt,ret);
    return (ret);
}

static xmlSchemaQNameRefPtr
xmlSchemaNewQNameRef(xmlSchemaPtr schema,
		     xmlSchemaTypeType refType,
		     const xmlChar *refName, 
		     const xmlChar *refNs)
{
    xmlSchemaQNameRefPtr ret;

    ret = (xmlSchemaQNameRefPtr)
	xmlMalloc(sizeof(xmlSchemaQNameRef));
    if (ret == NULL) {
	xmlSchemaPErrMemory(NULL, "allocating QName reference item",
	    NULL);
	return (NULL);
    }
    ret->type = XML_SCHEMA_EXTRA_QNAMEREF;
    ret->name = refName;
    ret->targetNamespace = refNs;
    ret->item = NULL;
    ret->itemType = refType;
    /*
    * Store the reference item in the schema.
    */
    xmlSchemaAddVolatile(schema, (xmlSchemaBasicItemPtr) ret);
    return (ret);
}

/**
 * xmlSchemaAddModelGroup:
 * @ctxt:  a schema parser context
 * @schema:  the schema being built
 * @type: the "compositor" type of the model group
 * @container:  the internal component name
 * @node: the node in the schema doc    
 *
 * Adds a schema model group
 * *WARNING* this interface is highly subject to change
 *
 * Returns the new struture or NULL in case of error
 */
static xmlSchemaModelGroupPtr
xmlSchemaAddModelGroup(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
		 xmlSchemaTypeType type, const xmlChar **container,
		 xmlNodePtr node)
{
    xmlSchemaModelGroupPtr ret = NULL;
    xmlChar buf[30];

    if ((ctxt == NULL) || (schema == NULL))
        return (NULL);

#ifdef DEBUG
    fprintf(stderr, "Adding model group component\n");
#endif    
    ret = (xmlSchemaModelGroupPtr)
	xmlMalloc(sizeof(xmlSchemaModelGroup));
    if (ret == NULL) {
	xmlSchemaPErrMemory(ctxt, "allocating model group component",
	    NULL);
	return (NULL);
    }
    ret->type = type;
    ret->annot = NULL;
    ret->node = node;
    ret->children = NULL;
    ret->next = NULL;
    if (type == XML_SCHEMA_TYPE_SEQUENCE) {
	if (container != NULL)
	    snprintf((char *) buf, 29, "#seq%d", ctxt->counter++ + 1);
    } else if (type == XML_SCHEMA_TYPE_CHOICE) {
	if (container != NULL)	    
	    snprintf((char *) buf, 29, "#cho%d", ctxt->counter++ + 1);
    } else {
	if (container != NULL)
	    snprintf((char *) buf, 29, "#all%d", ctxt->counter++ + 1);
    }
    if (container != NULL)
	*container = xmlDictLookup(ctxt->dict, BAD_CAST buf, -1);
    /*
    * Add to volatile items.
    * TODO: this should be changed someday.
    */
    if (xmlSchemaAddVolatile(schema, (xmlSchemaBasicItemPtr) ret) != 0) {
	xmlFree(ret);
	return (NULL);
    }
    return (ret);
}


/**
 * xmlSchemaAddParticle:
 * @ctxt:  a schema parser context
 * @schema:  the schema being built
 * @node: the corresponding node in the schema doc
 * @min: the minOccurs
 * @max: the maxOccurs
 *
 * Adds an XML schema particle component.
 * *WARNING* this interface is highly subject to change
 *
 * Returns the new struture or NULL in case of error
 */
static xmlSchemaParticlePtr
xmlSchemaAddParticle(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
		     xmlNodePtr node, int min, int max)
{
    xmlSchemaParticlePtr ret = NULL;
    if ((ctxt == NULL) || (schema == NULL))
        return (NULL);

#ifdef DEBUG
    fprintf(stderr, "Adding particle component\n");
#endif
    ret = (xmlSchemaParticlePtr)
	xmlMalloc(sizeof(xmlSchemaParticle));
    if (ret == NULL) {
	xmlSchemaPErrMemory(ctxt, "allocating particle component",
	    NULL);
	return (NULL);
    }
    ret->type = XML_SCHEMA_TYPE_PARTICLE;
    ret->annot = NULL;
    ret->node = node;
    ret->minOccurs = min;
    ret->maxOccurs = max;
    ret->next = NULL;
    ret->children = NULL;

    if (xmlSchemaAddVolatile(schema, (xmlSchemaBasicItemPtr) ret) != 0) {
	xmlFree(ret);
	return (NULL);
    }
    return (ret);
}

/**
 * xmlSchemaAddGroup:
 * @ctxt:  a schema validation context
 * @schema:  the schema being built
 * @name:  the group name
 *
 * Add an XML schema Group definition
 *
 * Returns the new struture or NULL in case of error
 */
static xmlSchemaModelGroupDefPtr
xmlSchemaAddGroup(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                  const xmlChar *name, const xmlChar *namespaceName,
		  xmlNodePtr node)
{
    xmlSchemaModelGroupDefPtr ret = NULL;
    int val;

    if ((ctxt == NULL) || (schema == NULL) || (name == NULL))
        return (NULL);

    if (schema->groupDecl == NULL)
        schema->groupDecl = xmlHashCreateDict(10, ctxt->dict);
    if (schema->groupDecl == NULL)
        return (NULL);

    ret = (xmlSchemaModelGroupDefPtr) xmlMalloc(sizeof(xmlSchemaModelGroupDef));
    if (ret == NULL) {
        xmlSchemaPErrMemory(ctxt, "adding group", NULL);
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaModelGroupDef));
    ret->name = xmlDictLookup(ctxt->dict, name, -1);
    ret->type = XML_SCHEMA_TYPE_GROUP;    
    ret->node = node;
    ret->targetNamespace = namespaceName;
    val = xmlHashAddEntry2(schema->groupDecl, ret->name, namespaceName, ret);
    if (val != 0) {
	xmlSchemaPCustomErr(ctxt,
	    XML_SCHEMAP_REDEFINED_GROUP,
	    NULL, NULL, node,
	    "A global model group definition with the name '%s' does already "
	    "exist", name);
        xmlFree(ret);
        return (NULL);
    }    
    if (ctxt->assemble != NULL)	
	xmlSchemaAddAssembledItem(ctxt, (xmlSchemaTypePtr) ret);
    return (ret);
}

/**
 * xmlSchemaNewWildcardNs:
 * @ctxt:  a schema validation context
 *
 * Creates a new wildcard namespace constraint.
 *
 * Returns the new struture or NULL in case of error
 */
static xmlSchemaWildcardNsPtr
xmlSchemaNewWildcardNsConstraint(xmlSchemaParserCtxtPtr ctxt)
{
    xmlSchemaWildcardNsPtr ret;

    ret = (xmlSchemaWildcardNsPtr) 
	xmlMalloc(sizeof(xmlSchemaWildcardNs));
    if (ret == NULL) {
	xmlSchemaPErrMemory(ctxt, "creating wildcard namespace constraint", NULL);
	return (NULL);    
    }
    ret->value = NULL;
    ret->next = NULL;
    return (ret);
}

/**
 * xmlSchemaAddWildcard:
 * @ctxt:  a schema validation context
 * @schema: a schema
 *
 * Adds a wildcard. 
 * It corresponds to a xsd:anyAttribute and xsd:any.
 *
 * Returns the new struture or NULL in case of error
 */
static xmlSchemaWildcardPtr
xmlSchemaAddWildcard(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
		     xmlSchemaTypeType type, xmlNodePtr node)
{
    xmlSchemaWildcardPtr ret = NULL;

    if ((ctxt == NULL) || (schema == NULL))
        return (NULL);

#ifdef DEBUG
    fprintf(stderr, "Adding wildcard component\n");
#endif

    ret = (xmlSchemaWildcardPtr) xmlMalloc(sizeof(xmlSchemaWildcard));
    if (ret == NULL) {
        xmlSchemaPErrMemory(ctxt, "adding wildcard", NULL);
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaWildcard));
    ret->type = type;
    ret->minOccurs = 1;
    ret->maxOccurs = 1;

    if (xmlSchemaAddVolatile(schema, (xmlSchemaBasicItemPtr) ret) != 0) {
	xmlSchemaPCustomErr(ctxt, XML_SCHEMAP_INTERNAL, NULL, NULL, node,
	    "Failed to add a wildcard component to the list", NULL);
	xmlFree(ret);
	return (NULL);
    }
    return (ret);
}

/************************************************************************
 * 									*
 *		Utilities for parsing					*
 * 									*
 ************************************************************************/

#if 0
/**
 * xmlGetQNameProp:
 * @ctxt:  a schema validation context
 * @node:  a subtree containing XML Schema informations
 * @name:  the attribute name
 * @namespace:  the result namespace if any
 *
 * Extract a QName Attribute value
 *
 * Returns the NCName or NULL if not found, and also update @namespace
 *    with the namespace URI
 */
static const xmlChar *
xmlGetQNameProp(xmlSchemaParserCtxtPtr ctxt, xmlNodePtr node,
                const char *name, const xmlChar ** namespace)
{
    const xmlChar *val;
    xmlNsPtr ns;
    const xmlChar *ret, *prefix;
    int len;
    xmlAttrPtr attr;

    *namespace = NULL;
    attr = xmlSchemaGetPropNode(node, name);
    if (attr == NULL)
	return (NULL);
    val = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);

    if (val == NULL)
        return (NULL);

    if (!strchr((char *) val, ':')) {
	ns = xmlSearchNs(node->doc, node, 0);
	if (ns) {
	    *namespace = xmlDictLookup(ctxt->dict, ns->href, -1);
	    return (val);
	}
    }
    ret = xmlSplitQName3(val, &len);
    if (ret == NULL) {
        return (val);
    }
    ret = xmlDictLookup(ctxt->dict, ret, -1);
    prefix = xmlDictLookup(ctxt->dict, val, len);

    ns = xmlSearchNs(node->doc, node, prefix);
    if (ns == NULL) {
        xmlSchemaPSimpleTypeErr(ctxt, XML_SCHEMAP_PREFIX_UNDEFINED, 
	    NULL, NULL, (xmlNodePtr) attr, 
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_QNAME), NULL, val,
	    "The QName value '%s' has no corresponding namespace "
	    "declaration in scope", val, NULL);
    } else {
        *namespace = xmlDictLookup(ctxt->dict, ns->href, -1);
    }
    return (ret);
}
#endif

/**
 * xmlSchemaPValAttrNodeQNameValue:
 * @ctxt:  a schema parser context
 * @schema: the schema context
 * @ownerDes: the designation of the parent element
 * @ownerItem: the parent as a schema object
 * @value:  the QName value 
 * @local: the resulting local part if found, the attribute value otherwise
 * @uri:  the resulting namespace URI if found
 *
 * Extracts the local name and the URI of a QName value and validates it.
 * This one is intended to be used on attribute values that
 * should resolve to schema components.
 *
 * Returns 0, in case the QName is valid, a positive error code
 * if not valid and -1 if an internal error occurs.
 */
static int
xmlSchemaPValAttrNodeQNameValue(xmlSchemaParserCtxtPtr ctxt, 
				       xmlSchemaPtr schema,
				       xmlChar **ownerDes,
				       xmlSchemaTypePtr ownerItem,
				       xmlAttrPtr attr,
				       const xmlChar *value,
				       const xmlChar **uri,
				       const xmlChar **prefix,
				       const xmlChar **local)
{
    const xmlChar *pref;
    xmlNsPtr ns;
    int len, ret;
    
    *uri = NULL;
    *local = NULL;
    if (prefix != 0)
	*prefix = NULL;
    ret = xmlValidateQName(value, 1);
    if (ret > 0) {		
	xmlSchemaPSimpleTypeErr(ctxt, 
	    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE, 
	    ownerDes, ownerItem, (xmlNodePtr) attr, 
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_QNAME), 
	    "QName", value, 
	    NULL, NULL, NULL);	
	*local = value;
	return (ctxt->err); 
    } else if (ret < 0)
	return (-1);
   
    if (!strchr((char *) value, ':')) {	
	ns = xmlSearchNs(attr->doc, attr->parent, 0);
	if (ns)
	    *uri = xmlDictLookup(ctxt->dict, ns->href, -1);
	else if (schema->flags & XML_SCHEMAS_INCLUDING_CONVERT_NS) {
	    /*
	    * This one takes care of included schemas with no
	    * target namespace.
	    */
	    *uri = schema->targetNamespace;
	}	
	*local = xmlDictLookup(ctxt->dict, value, -1);
	return (0);
    }
    /*
    * At this point xmlSplitQName3 has to return a local name.
    */
    *local = xmlSplitQName3(value, &len);
    *local = xmlDictLookup(ctxt->dict, *local, -1);
    pref = xmlDictLookup(ctxt->dict, value, len);
    if (prefix != 0)	
	*prefix = pref;
    ns = xmlSearchNs(attr->doc, attr->parent, pref);
    if (ns == NULL) {
	xmlSchemaPSimpleTypeErr(ctxt, 
	    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
	    ownerDes, ownerItem, (xmlNodePtr) attr, 
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_QNAME), "QName", value,
	    "The QName value '%s' has no corresponding namespace "
	    "declaration in scope", value, NULL);
	return (ctxt->err);
    } else {
        *uri = xmlDictLookup(ctxt->dict, ns->href, -1);
    }    
    return (0);
}

/**
 * xmlSchemaPValAttrNodeQName:
 * @ctxt:  a schema parser context
 * @schema: the schema context
 * @ownerDes: the designation of the owner element
 * @ownerItem: the owner as a schema object
 * @attr:  the attribute node
 * @local: the resulting local part if found, the attribute value otherwise
 * @uri:  the resulting namespace URI if found
 *
 * Extracts and validates the QName of an attribute value.
 * This one is intended to be used on attribute values that
 * should resolve to schema components.
 *
 * Returns 0, in case the QName is valid, a positive error code
 * if not valid and -1 if an internal error occurs.
 */
static int
xmlSchemaPValAttrNodeQName(xmlSchemaParserCtxtPtr ctxt, 
				       xmlSchemaPtr schema,
				       xmlChar **ownerDes,
				       xmlSchemaTypePtr ownerItem,
				       xmlAttrPtr attr,
				       const xmlChar **uri,
				       const xmlChar **prefix,
				       const xmlChar **local)
{
    const xmlChar *value;

    value = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);
    return (xmlSchemaPValAttrNodeQNameValue(ctxt, schema, 
	ownerDes, ownerItem, attr, value, uri, prefix, local));
}

/**
 * xmlSchemaPValAttrQName:
 * @ctxt:  a schema parser context
 * @schema: the schema context
 * @ownerDes: the designation of the parent element
 * @ownerItem: the owner as a schema object
 * @ownerElem:  the parent node of the attribute
 * @name:  the name of the attribute
 * @local: the resulting local part if found, the attribute value otherwise
 * @uri:  the resulting namespace URI if found
 *
 * Extracts and validates the QName of an attribute value.
 *
 * Returns 0, in case the QName is valid, a positive error code
 * if not valid and -1 if an internal error occurs.
 */
static int
xmlSchemaPValAttrQName(xmlSchemaParserCtxtPtr ctxt, 
				   xmlSchemaPtr schema, 
				   xmlChar **ownerDes,
				   xmlSchemaTypePtr ownerItem,
				   xmlNodePtr ownerElem,
				   const char *name,
				   const xmlChar **uri,
				   const xmlChar **prefix,
				   const xmlChar **local)
{
    xmlAttrPtr attr;

    attr = xmlSchemaGetPropNode(ownerElem, name);
    if (attr == NULL) {
	*local = NULL;
	if (prefix != NULL)
	    *prefix = NULL;
	*uri = NULL;
	return (0);    
    }
    return (xmlSchemaPValAttrNodeQName(ctxt, schema, 
	ownerDes, ownerItem, attr, uri, prefix, local));
}

/**
 * xmlSchemaPValAttrID:
 * @ctxt:  a schema parser context
 * @schema: the schema context
 * @ownerDes: the designation of the parent element
 * @ownerItem: the owner as a schema object
 * @ownerElem:  the parent node of the attribute
 * @name:  the name of the attribute
 *
 * Extracts and validates the ID of an attribute value.
 *
 * Returns 0, in case the ID is valid, a positive error code
 * if not valid and -1 if an internal error occurs.
 */
static int
xmlSchemaPValAttrID(xmlSchemaParserCtxtPtr ctxt,
		    xmlChar **ownerDes,
		    xmlSchemaTypePtr ownerItem,
		    xmlNodePtr ownerElem,
		    const xmlChar *name)
{
    int ret;
    xmlChar *value;
    xmlAttrPtr attr; 

    value = xmlGetNoNsProp(ownerElem, name);
    if (value == NULL)
	return (0);

    attr = xmlSchemaGetPropNode(ownerElem, (const char *) name);
    if (attr == NULL)
	return (-1);

    ret = xmlValidateNCName(BAD_CAST value, 1);
    if (ret == 0) {	
	/*
	* NOTE: the IDness might have already be declared in the DTD
	*/
	if (attr->atype != XML_ATTRIBUTE_ID) {
	    xmlIDPtr res;
	    xmlChar *strip;
	    
	    /* 
	    * TODO: Use xmlSchemaStrip here; it's not exported at this
	    * moment.
	    */
	    strip = xmlSchemaCollapseString(BAD_CAST value);	    
	    if (strip != NULL)
		value = strip;		
    	    res = xmlAddID(NULL, ownerElem->doc, BAD_CAST value, attr);
	    if (res == NULL) {
		ret = XML_SCHEMAP_S4S_ATTR_INVALID_VALUE;
		xmlSchemaPSimpleTypeErr(ctxt, 
		    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE, 
		    ownerDes, ownerItem, (xmlNodePtr) attr, 
		    xmlSchemaGetBuiltInType(XML_SCHEMAS_ID), 
		    NULL, NULL, "The ID '%s' is already defined",
		    BAD_CAST value, NULL);
	    } else
		attr->atype = XML_ATTRIBUTE_ID;
	    if (strip != NULL)
		xmlFree(strip);
	}
    } else if (ret > 0) {
	ret = XML_SCHEMAP_S4S_ATTR_INVALID_VALUE;
	xmlSchemaPSimpleTypeErr(ctxt, 
	    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE, 
	    ownerDes, ownerItem, (xmlNodePtr) attr, 
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_ID), 
	    NULL, BAD_CAST value, NULL, NULL, NULL);	
    }
    xmlFree(value);

    return (ret);
}

/**
 * xmlGetMaxOccurs:
 * @ctxt:  a schema validation context
 * @node:  a subtree containing XML Schema informations
 *
 * Get the maxOccurs property
 *
 * Returns the default if not found, or the value
 */
static int
xmlGetMaxOccurs(xmlSchemaParserCtxtPtr ctxt, xmlNodePtr node,
		int min, int max, int def, const char *expected)
{
    const xmlChar *val, *cur;
    int ret = 0;
    xmlAttrPtr attr;

    attr = xmlSchemaGetPropNode(node, "maxOccurs");
    if (attr == NULL)
	return (def);
    val = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);

    if (xmlStrEqual(val, (const xmlChar *) "unbounded")) {
	if (max != UNBOUNDED) {
	    xmlSchemaPSimpleTypeErr(ctxt, 
		XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
		/* XML_SCHEMAP_INVALID_MINOCCURS, */
		NULL, NULL, (xmlNodePtr) attr, NULL, expected,
		val, NULL, NULL, NULL);
	    return (def);
	} else 
	    return (UNBOUNDED);  /* encoding it with -1 might be another option */
    }

    cur = val;
    while (IS_BLANK_CH(*cur))
        cur++;
    if (*cur == 0) {
        xmlSchemaPSimpleTypeErr(ctxt, 
	    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
	    /* XML_SCHEMAP_INVALID_MINOCCURS, */
	    NULL, NULL, (xmlNodePtr) attr, NULL, expected,
	    val, NULL, NULL, NULL);
	return (def);
    }
    while ((*cur >= '0') && (*cur <= '9')) {
        ret = ret * 10 + (*cur - '0');
        cur++;
    }
    while (IS_BLANK_CH(*cur))
        cur++;
    /*
    * TODO: Restrict the maximal value to Integer.
    */
    if ((*cur != 0) || (ret < min) || ((max != -1) && (ret > max))) {
	xmlSchemaPSimpleTypeErr(ctxt, 
	    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
	    /* XML_SCHEMAP_INVALID_MINOCCURS, */
	    NULL, NULL, (xmlNodePtr) attr, NULL, expected,
	    val, NULL, NULL, NULL);
        return (def);
    }
    return (ret);
}

/**
 * xmlGetMinOccurs:
 * @ctxt:  a schema validation context
 * @node:  a subtree containing XML Schema informations
 *
 * Get the minOccurs property
 *
 * Returns the default if not found, or the value
 */
static int
xmlGetMinOccurs(xmlSchemaParserCtxtPtr ctxt, xmlNodePtr node, 
		int min, int max, int def, const char *expected)
{
    const xmlChar *val, *cur;
    int ret = 0;
    xmlAttrPtr attr;

    attr = xmlSchemaGetPropNode(node, "minOccurs");
    if (attr == NULL)
	return (def);
    val = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);
    cur = val;
    while (IS_BLANK_CH(*cur))
        cur++;
    if (*cur == 0) {
        xmlSchemaPSimpleTypeErr(ctxt, 
	    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
	    /* XML_SCHEMAP_INVALID_MINOCCURS, */
	    NULL, NULL, (xmlNodePtr) attr, NULL, expected,
	    val, NULL, NULL, NULL);
        return (def);
    }
    while ((*cur >= '0') && (*cur <= '9')) {
        ret = ret * 10 + (*cur - '0');
        cur++;
    }
    while (IS_BLANK_CH(*cur))
        cur++;
    /*
    * TODO: Restrict the maximal value to Integer.
    */
    if ((*cur != 0) || (ret < min) || ((max != -1) && (ret > max))) {
	xmlSchemaPSimpleTypeErr(ctxt, 
	    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
	    /* XML_SCHEMAP_INVALID_MINOCCURS, */
	    NULL, NULL, (xmlNodePtr) attr, NULL, expected,
	    val, NULL, NULL, NULL);
        return (def);
    }
    return (ret);
}

/**
 * xmlSchemaPGetBoolNodeValue:
 * @ctxt:  a schema validation context
 * @ownerDes:  owner designation
 * @ownerItem:  the owner as a schema item
 * @node: the node holding the value
 *
 * Converts a boolean string value into 1 or 0.
 *
 * Returns 0 or 1.
 */
static int
xmlSchemaPGetBoolNodeValue(xmlSchemaParserCtxtPtr ctxt,			   
			   xmlChar **ownerDes,
			   xmlSchemaTypePtr ownerItem,
			   xmlNodePtr node)
{
    xmlChar *value = NULL;
    int res = 0;
   
    value = xmlNodeGetContent(node);
    /* 
    * 3.2.2.1 Lexical representation
    * An instance of a datatype that is defined as �boolean� 
    * can have the following legal literals {true, false, 1, 0}.
    */
    if (xmlStrEqual(BAD_CAST value, BAD_CAST "true"))
        res = 1;
    else if (xmlStrEqual(BAD_CAST value, BAD_CAST "false"))
        res = 0;
    else if (xmlStrEqual(BAD_CAST value, BAD_CAST "1"))
	res = 1;
    else if (xmlStrEqual(BAD_CAST value, BAD_CAST "0"))
        res = 0;    
    else {
        xmlSchemaPSimpleTypeErr(ctxt, 
	    XML_SCHEMAP_INVALID_BOOLEAN,
	    ownerDes, ownerItem, node, 
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_BOOLEAN), 
	    "(1 | 0 | true | false)", BAD_CAST value, 
	    NULL, NULL, NULL);
    }
    if (value != NULL)
	xmlFree(value);
    return (res);
}

/**
 * xmlGetBooleanProp:
 * @ctxt:  a schema validation context
 * @node:  a subtree containing XML Schema informations
 * @name:  the attribute name
 * @def:  the default value
 *
 * Evaluate if a boolean property is set
 *
 * Returns the default if not found, 0 if found to be false,
 * 1 if found to be true
 */
static int
xmlGetBooleanProp(xmlSchemaParserCtxtPtr ctxt, 
		  xmlChar **ownerDes,
		  xmlSchemaTypePtr ownerItem,
		  xmlNodePtr node,
                  const char *name, int def)
{
    const xmlChar *val;

    val = xmlSchemaGetProp(ctxt, node, name);
    if (val == NULL)
        return (def);
    /* 
    * 3.2.2.1 Lexical representation
    * An instance of a datatype that is defined as �boolean� 
    * can have the following legal literals {true, false, 1, 0}.
    */
    if (xmlStrEqual(val, BAD_CAST "true"))
        def = 1;
    else if (xmlStrEqual(val, BAD_CAST "false"))
        def = 0;
    else if (xmlStrEqual(val, BAD_CAST "1"))
	def = 1;
    else if (xmlStrEqual(val, BAD_CAST "0"))
        def = 0;    
    else {    
        xmlSchemaPSimpleTypeErr(ctxt, 
	    XML_SCHEMAP_INVALID_BOOLEAN,
	    ownerDes, ownerItem, 
	    (xmlNodePtr) xmlSchemaGetPropNode(node, name),
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_BOOLEAN), 
	    "(1 | 0 | true | false)", val, NULL, NULL, NULL);
    }
    return (def);
}

/************************************************************************
 * 									*
 *		Shema extraction from an Infoset			*
 * 									*
 ************************************************************************/
static xmlSchemaTypePtr xmlSchemaParseSimpleType(xmlSchemaParserCtxtPtr
                                                 ctxt, xmlSchemaPtr schema,
                                                 xmlNodePtr node,
						 int topLevel);
static xmlSchemaTypePtr xmlSchemaParseComplexType(xmlSchemaParserCtxtPtr
                                                  ctxt,
                                                  xmlSchemaPtr schema,
                                                  xmlNodePtr node, 
						  int topLevel);
static xmlSchemaTypePtr xmlSchemaParseRestriction(xmlSchemaParserCtxtPtr
                                                  ctxt,
                                                  xmlSchemaPtr schema,
                                                  xmlNodePtr node,
						  xmlSchemaTypeType parentType);
static xmlSchemaAttributePtr xmlSchemaParseAttribute(xmlSchemaParserCtxtPtr
                                                     ctxt,
                                                     xmlSchemaPtr schema,
                                                     xmlNodePtr node,
						     int topLevel);
static xmlSchemaAttributeGroupPtr
xmlSchemaParseAttributeGroup(xmlSchemaParserCtxtPtr ctxt,
                             xmlSchemaPtr schema, xmlNodePtr node,
			     int topLevel);
static xmlSchemaTypePtr xmlSchemaParseList(xmlSchemaParserCtxtPtr ctxt,
                                           xmlSchemaPtr schema,
                                           xmlNodePtr node);
static xmlSchemaWildcardPtr
xmlSchemaParseAnyAttribute(xmlSchemaParserCtxtPtr ctxt,
                           xmlSchemaPtr schema, xmlNodePtr node);

/**
 * xmlSchemaPValAttrNodeValue:
 * 
 * @ctxt:  a schema parser context
 * @ownerDes: the designation of the parent element
 * @ownerItem: the schema object owner if existent
 * @attr:  the schema attribute node being validated
 * @value: the value
 * @type: the built-in type to be validated against 
 *
 * Validates a value against the given built-in type.
 * This one is intended to be used internally for validation
 * of schema attribute values during parsing of the schema.
 *
 * Returns 0 if the value is valid, a positive error code
 * number otherwise and -1 in case of an internal or API error.
 */
static int
xmlSchemaPValAttrNodeValue(xmlSchemaParserCtxtPtr ctxt,
			   xmlChar **ownerDes,
			   xmlSchemaTypePtr ownerItem,			   
			   xmlAttrPtr attr,
			   const xmlChar *value,
			   xmlSchemaTypePtr type)
{
    
    int ret = 0; 

    /*
    * NOTE: Should we move this to xmlschematypes.c? Hmm, but this
    * one is really meant to be used internally, so better not.
    */    
    if ((ctxt == NULL) || (type == NULL) || (attr == NULL))
	return (-1);   
    if (type->type != XML_SCHEMA_TYPE_BASIC) {
	xmlSchemaPErr(ctxt, (xmlNodePtr) attr, 
	    XML_SCHEMAP_INTERNAL,
	    "Internal error: xmlSchemaPValAttrNodeValue, the given "
	    "type '%s' is not a built-in type.\n",
	    type->name, NULL);
	return (-1);
    }    
    switch (type->builtInType) {
	case XML_SCHEMAS_NCNAME:
	case XML_SCHEMAS_QNAME:
	case XML_SCHEMAS_ANYURI:
	case XML_SCHEMAS_TOKEN:
	case XML_SCHEMAS_LANGUAGE:
	    ret = xmlSchemaValPredefTypeNode(type, value, NULL, (xmlNodePtr) attr);
	    break;

	/*
	case XML_SCHEMAS_NCNAME:
	    ret = xmlValidateNCName(value, 1);
	    break;
	case XML_SCHEMAS_QNAME:
	    xmlSchemaPErr(ctxt, (xmlNodePtr) attr, 
		XML_SCHEMAP_INTERNAL,
		"Internal error: xmlSchemaPvalueAttrNode, use "
		"the function xmlSchemaExtractSchemaQNamePropvalueidated "
		"for extracting QName valueues instead.\n",
		NULL, NULL);
	    return (-1);
	case XML_SCHEMAS_ANYURI:
	    if (value != NULL) {
		xmlURIPtr uri = xmlParseURI((const char *) value);
		if (uri == NULL)
		    ret = 1;
		else
		    xmlFreeURI(uri);
	    }
	    break;
	case XML_SCHEMAS_TOKEN: {
	    const xmlChar *cur = value;

		if (IS_BLANK_CH(*cur)) {
                    ret = 1;		       
		} else while (*cur != 0) {
                    if ((*cur == 0xd) || (*cur == 0xa) || (*cur == 0x9)) {
                        ret = 1;
			break;
                    } else if (*cur == ' ') {
                        cur++;
                        if ((*cur == 0) || (*cur == ' ')) {
			    ret = 1;
			    break;
			}
                    } else {
                        cur++;
                    }
                }
	    }
	    break;
	case XML_SCHEMAS_LANGUAGE:
	    if (xmlCheckLanguageID(value) != 1) 
		ret = 1;
	    break;
	*/
	default: {
	    xmlSchemaPErr(ctxt, (xmlNodePtr) attr, 
		    XML_SCHEMAP_INTERNAL,
		    "Internal error: xmlSchemaPValAttrNodeValue, "
		    "valueidation using the type '%s' is not implemented "
		    "yet.\n",
		    type->name, NULL);
	    return (-1);
	}
    }              
    /*
    * TODO: Should we use the S4S error codes instead?
    */
    if (ret < 0) {
	xmlSchemaPErr(ctxt, (xmlNodePtr) attr, 
	    XML_SCHEMAP_INTERNAL,
	    "Internal error: xmlSchemaPValAttrNodeValue, "
	    "failed to validate a schema attribute value.\n",
	    NULL, NULL);
	return (-1);
    } else if (ret > 0) { 	
	if (type->flags & XML_SCHEMAS_TYPE_VARIETY_LIST) {	   
	    xmlSchemaPSimpleTypeErr(ctxt, 
		XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_2, 
		ownerDes, ownerItem, (xmlNodePtr) attr, 
		type, NULL, value, 
		NULL, NULL, NULL);
	    return(XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_2);
	} else {	    
	    xmlSchemaPSimpleTypeErr(ctxt, 
		XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_1, 
		ownerDes, ownerItem, (xmlNodePtr) attr, 
		type, NULL, value, 
		NULL, NULL, NULL);
	    return(XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_1);
	}	
    }    
    return (ret);
}

/**
 * xmlSchemaPValAttrNode:
 * 
 * @ctxt:  a schema parser context
 * @ownerDes: the designation of the parent element
 * @ownerItem: the schema object owner if existent
 * @attr:  the schema attribute node being validated
 * @type: the built-in type to be validated against
 * @value: the resulting value if any
 *
 * Extracts and validates a value against the given built-in type.
 * This one is intended to be used internally for validation
 * of schema attribute values during parsing of the schema.
 *
 * Returns 0 if the value is valid, a positive error code
 * number otherwise and -1 in case of an internal or API error.
 */
static int
xmlSchemaPValAttrNode(xmlSchemaParserCtxtPtr ctxt,
			   xmlChar **ownerDes,
			   xmlSchemaTypePtr ownerItem,			   
			   xmlAttrPtr attr,			   
			   xmlSchemaTypePtr type,
			   const xmlChar **value)
{    
    const xmlChar *val;

    if ((ctxt == NULL) || (type == NULL) || (attr == NULL))
	return (-1);   
       
    val = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);
    if (value != NULL)
	*value = val;

    return (xmlSchemaPValAttrNodeValue(ctxt, ownerDes, ownerItem, attr,
	val, type));    
}

/**
 * xmlSchemaPValAttr:
 * 
 * @ctxt:  a schema parser context
 * @node: the element node of the attribute
 * @ownerDes: the designation of the parent element
 * @ownerItem: the schema object owner if existent
 * @ownerElem: the owner element node
 * @name:  the name of the schema attribute node
 * @type: the built-in type to be validated against
 * @value: the resulting value if any
 *
 * Extracts and validates a value against the given built-in type.
 * This one is intended to be used internally for validation
 * of schema attribute values during parsing of the schema.
 *
 * Returns 0 if the value is valid, a positive error code
 * number otherwise and -1 in case of an internal or API error.
 */
static int
xmlSchemaPValAttr(xmlSchemaParserCtxtPtr ctxt,		       
		       xmlChar **ownerDes,
		       xmlSchemaTypePtr ownerItem,
		       xmlNodePtr ownerElem,
		       const char *name,
		       xmlSchemaTypePtr type,
		       const xmlChar **value)
{
    xmlAttrPtr attr;

    if ((ctxt == NULL) || (type == NULL)) {
	if (value != NULL)
	    *value = NULL;
	return (-1);   
    }
    if (type->type != XML_SCHEMA_TYPE_BASIC) {
	if (value != NULL)
	    *value = NULL;
	xmlSchemaPErr(ctxt, ownerElem, 
	    XML_SCHEMAP_INTERNAL,
	    "Internal error: xmlSchemaPValAttr, the given "
	    "type '%s' is not a built-in type.\n",
	    type->name, NULL);
	return (-1);
    }
    attr = xmlSchemaGetPropNode(ownerElem, name);
    if (attr == NULL) {
	if (value != NULL)
	    *value = NULL;
	return (0);
    }    
    return (xmlSchemaPValAttrNode(ctxt, ownerDes, ownerItem, attr, 
	type, value));
}

static int
xmlSchemaCheckReference(xmlSchemaParserCtxtPtr pctxt,
		  xmlSchemaPtr schema,
		  xmlNodePtr node,
		  xmlSchemaBasicItemPtr item,
		  const xmlChar *namespaceName)
{
    if (xmlStrEqual(schema->targetNamespace, namespaceName))
	return (1); 
    if (pctxt->localImports != NULL) {
	int i;
	for (i = 0; i < pctxt->nbLocalImports; i++)
	    if (xmlStrEqual(namespaceName, pctxt->localImports[i]))
		return (1);
    }
    if (namespaceName == NULL)
	xmlSchemaPCustomErr(pctxt, XML_SCHEMAP_SRC_RESOLVE,
	    NULL, (xmlSchemaTypePtr) item, node,
	    "References from this schema to components in no "
	    "namespace are not valid, since not indicated by an import "
	    "statement", NULL);
    else
	xmlSchemaPCustomErr(pctxt, XML_SCHEMAP_SRC_RESOLVE,
	    NULL, (xmlSchemaTypePtr) item, node,
	    "References from this schema to components in the "
	    "namespace '%s' are not valid, since not indicated by an import "
	    "statement", namespaceName);
    return (0);
}

/**
 * xmlSchemaParseAttrDecls:
 * @ctxt:  a schema validation context
 * @schema:  the schema being built
 * @node:  a subtree containing XML Schema informations
 * @type:  the hosting type where the attributes will be anchored
 *
 * parse a XML schema attrDecls declaration corresponding to
 * <!ENTITY % attrDecls  
 *       '((%attribute;| %attributeGroup;)*,(%anyAttribute;)?)'>
 */
static xmlNodePtr
xmlSchemaParseAttrDecls(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                        xmlNodePtr child, xmlSchemaTypePtr type)
{
    xmlSchemaAttributePtr lastattr = NULL, attr;

    while ((IS_SCHEMA(child, "attribute")) ||
           (IS_SCHEMA(child, "attributeGroup"))) {
        attr = NULL;
        if (IS_SCHEMA(child, "attribute")) {
            attr = xmlSchemaParseAttribute(ctxt, schema, child, 0);
        } else if (IS_SCHEMA(child, "attributeGroup")) {
            attr = (xmlSchemaAttributePtr)
                xmlSchemaParseAttributeGroup(ctxt, schema, child, 0);
        }
        if (attr != NULL) {
            if (lastattr == NULL) {
		if (type->type == XML_SCHEMA_TYPE_ATTRIBUTEGROUP)
		    ((xmlSchemaAttributeGroupPtr) type)->attributes = attr;
		else
		    type->attributes = attr;
                lastattr = attr;
            } else {
                lastattr->next = attr;
                lastattr = attr;
            }
        }
        child = child->next;
    }    
    return (child);
}

/**
 * xmlSchemaParseAnnotation:
 * @ctxt:  a schema validation context
 * @schema:  the schema being built
 * @node:  a subtree containing XML Schema informations
 *
 * parse a XML schema Attrribute declaration
 * *WARNING* this interface is highly subject to change
 *
 * Returns -1 in case of error, 0 if the declaration is improper and
 *         1 in case of success.
 */
static xmlSchemaAnnotPtr
xmlSchemaParseAnnotation(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                         xmlNodePtr node)
{
    xmlSchemaAnnotPtr ret;
    xmlNodePtr child = NULL;
    xmlAttrPtr attr;
    int barked = 0;

    /*
    * INFO: S4S completed.
    */
    /*
    * id = ID
    * {any attributes with non-schema namespace . . .}>
    * Content: (appinfo | documentation)*
    */
    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);
    ret = xmlSchemaNewAnnot(ctxt, node);
    attr = node->properties;
    while (attr != NULL) {
	if (((attr->ns == NULL) && 
	    (!xmlStrEqual(attr->name, BAD_CAST "id"))) ||
	    ((attr->ns != NULL) && 
	    xmlStrEqual(attr->ns->href, xmlSchemaNs))) {
	    
	    xmlSchemaPIllegalAttrErr(ctxt, 
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED,
		NULL, NULL, attr);
	}
	attr = attr->next;
    }
    xmlSchemaPValAttrID(ctxt, NULL, NULL, node, BAD_CAST "id");
    /*
    * And now for the children...
    */
    child = node->children;
    while (child != NULL) {
	if (IS_SCHEMA(child, "appinfo")) {
	    /* TODO: make available the content of "appinfo". */
	    /* 
	    * source = anyURI
	    * {any attributes with non-schema namespace . . .}>
	    * Content: ({any})*
	    */
	    attr = child->properties;
	    while (attr != NULL) {
		if (((attr->ns == NULL) && 
		     (!xmlStrEqual(attr->name, BAD_CAST "source"))) ||
		     ((attr->ns != NULL) && 
		      xmlStrEqual(attr->ns->href, xmlSchemaNs))) {

		    xmlSchemaPIllegalAttrErr(ctxt, 
			XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED,
			NULL, NULL, attr);
		}
		attr = attr->next;
	    }
	    xmlSchemaPValAttr(ctxt, NULL, NULL, child, "source", 
		xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYURI), NULL);	    
	    child = child->next;
	} else if (IS_SCHEMA(child, "documentation")) {
	    /* TODO: make available the content of "documentation". */
	    /*
	    * source = anyURI
	    * {any attributes with non-schema namespace . . .}>
	    * Content: ({any})*
	    */
	    attr = child->properties;
	    while (attr != NULL) {
		if (attr->ns == NULL) {
		    if (!xmlStrEqual(attr->name, BAD_CAST "source")) {
			xmlSchemaPIllegalAttrErr(ctxt, 
			    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED,
			    NULL, NULL, attr);
		    }
		} else {
		    if (xmlStrEqual(attr->ns->href, xmlSchemaNs) ||
			(xmlStrEqual(attr->name, BAD_CAST "lang") &&
			(!xmlStrEqual(attr->ns->href, XML_XML_NAMESPACE)))) {
			
			xmlSchemaPIllegalAttrErr(ctxt, 
			    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED,
			    NULL, NULL, attr);
		    }
		}
		attr = attr->next;
	    }
	    /*
	    * Attribute "xml:lang".
	    */
	    attr = xmlSchemaGetPropNodeNs(child, (const char *) XML_XML_NAMESPACE, "lang");
	    if (attr != NULL)
		xmlSchemaPValAttrNode(ctxt, NULL, NULL, attr,
		xmlSchemaGetBuiltInType(XML_SCHEMAS_LANGUAGE), NULL);	    
	    child = child->next;
	} else {
	    if (!barked)
		xmlSchemaPContentErr(ctxt, 
		    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED, 
		    NULL, NULL, node, child, NULL, "(appinfo | documentation)*");
	    barked = 1;
	    child = child->next;
	}
    }
    
    return (ret);
}

/**
 * xmlSchemaParseFacet:
 * @ctxt:  a schema validation context
 * @schema:  the schema being built
 * @node:  a subtree containing XML Schema informations
 *
 * parse a XML schema Facet declaration
 * *WARNING* this interface is highly subject to change
 *
 * Returns the new type structure or NULL in case of error
 */
static xmlSchemaFacetPtr
xmlSchemaParseFacet(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                    xmlNodePtr node)
{
    xmlSchemaFacetPtr facet;
    xmlNodePtr child = NULL;
    const xmlChar *value;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);

    facet = xmlSchemaNewFacet();
    if (facet == NULL) {
        xmlSchemaPErrMemory(ctxt, "allocating facet", node);
        return (NULL);
    }
    facet->node = node;
    value = xmlSchemaGetProp(ctxt, node, "value");
    if (value == NULL) {
        xmlSchemaPErr2(ctxt, node, child, XML_SCHEMAP_FACET_NO_VALUE,
                       "Facet %s has no value\n", node->name, NULL);
        xmlSchemaFreeFacet(facet);
        return (NULL);
    }
    if (IS_SCHEMA(node, "minInclusive")) {
        facet->type = XML_SCHEMA_FACET_MININCLUSIVE;
    } else if (IS_SCHEMA(node, "minExclusive")) {
        facet->type = XML_SCHEMA_FACET_MINEXCLUSIVE;
    } else if (IS_SCHEMA(node, "maxInclusive")) {
        facet->type = XML_SCHEMA_FACET_MAXINCLUSIVE;
    } else if (IS_SCHEMA(node, "maxExclusive")) {
        facet->type = XML_SCHEMA_FACET_MAXEXCLUSIVE;
    } else if (IS_SCHEMA(node, "totalDigits")) {
        facet->type = XML_SCHEMA_FACET_TOTALDIGITS;
    } else if (IS_SCHEMA(node, "fractionDigits")) {
        facet->type = XML_SCHEMA_FACET_FRACTIONDIGITS;
    } else if (IS_SCHEMA(node, "pattern")) {
        facet->type = XML_SCHEMA_FACET_PATTERN;
    } else if (IS_SCHEMA(node, "enumeration")) {
        facet->type = XML_SCHEMA_FACET_ENUMERATION;
    } else if (IS_SCHEMA(node, "whiteSpace")) {
        facet->type = XML_SCHEMA_FACET_WHITESPACE;
    } else if (IS_SCHEMA(node, "length")) {
        facet->type = XML_SCHEMA_FACET_LENGTH;
    } else if (IS_SCHEMA(node, "maxLength")) {
        facet->type = XML_SCHEMA_FACET_MAXLENGTH;
    } else if (IS_SCHEMA(node, "minLength")) {
        facet->type = XML_SCHEMA_FACET_MINLENGTH;
    } else {
        xmlSchemaPErr2(ctxt, node, child, XML_SCHEMAP_UNKNOWN_FACET_TYPE,
                       "Unknown facet type %s\n", node->name, NULL);
        xmlSchemaFreeFacet(facet);
        return (NULL);
    }
    xmlSchemaPValAttrID(ctxt, NULL,
	(xmlSchemaTypePtr) facet, node, BAD_CAST "id");
    facet->value = value;
    if ((facet->type != XML_SCHEMA_FACET_PATTERN) &&
	(facet->type != XML_SCHEMA_FACET_ENUMERATION)) {
	const xmlChar *fixed;

	fixed = xmlSchemaGetProp(ctxt, node, "fixed");
	if (fixed != NULL) {
	    if (xmlStrEqual(fixed, BAD_CAST "true"))
		facet->fixed = 1;
	}
    }    
    child = node->children;

    if (IS_SCHEMA(child, "annotation")) {
        facet->annot = xmlSchemaParseAnnotation(ctxt, schema, child);
        child = child->next;
    }
    if (child != NULL) {
        xmlSchemaPErr2(ctxt, node, child, XML_SCHEMAP_UNKNOWN_FACET_CHILD,
                       "Facet %s has unexpected child content\n",
                       node->name, NULL);
    }
    return (facet);
}

/**
 * xmlSchemaParseWildcardNs:
 * @ctxt:  a schema parser context
 * @wildc:  the wildcard, already created
 * @node:  a subtree containing XML Schema informations
 *
 * Parses the attribute "processContents" and "namespace"
 * of a xsd:anyAttribute and xsd:any.
 * *WARNING* this interface is highly subject to change
 *
 * Returns 0 if everything goes fine, a positive error code
 * if something is not valid and -1 if an internal error occurs.
 */
static int
xmlSchemaParseWildcardNs(xmlSchemaParserCtxtPtr ctxt,
			 xmlSchemaPtr schema,
			 xmlSchemaWildcardPtr wildc,
			 xmlNodePtr node)
{
    const xmlChar *pc, *ns, *dictnsItem;
    int ret = 0;
    xmlChar *nsItem;
    xmlSchemaWildcardNsPtr tmp, lastNs = NULL;
    xmlAttrPtr attr;
    
    pc = xmlSchemaGetProp(ctxt, node, "processContents");
    if ((pc == NULL)
        || (xmlStrEqual(pc, (const xmlChar *) "strict"))) {
        wildc->processContents = XML_SCHEMAS_ANY_STRICT;
    } else if (xmlStrEqual(pc, (const xmlChar *) "skip")) {
        wildc->processContents = XML_SCHEMAS_ANY_SKIP;
    } else if (xmlStrEqual(pc, (const xmlChar *) "lax")) {
        wildc->processContents = XML_SCHEMAS_ANY_LAX;
    } else {
        xmlSchemaPSimpleTypeErr(ctxt, 
	    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
	    NULL, NULL, node,
	    NULL, "(strict | skip | lax)", pc, 
	    NULL, NULL, NULL);
        wildc->processContents = XML_SCHEMAS_ANY_STRICT;
	ret = XML_SCHEMAP_S4S_ATTR_INVALID_VALUE;
    }
    /*
     * Build the namespace constraints.
     */
    attr = xmlSchemaGetPropNode(node, "namespace");
    ns = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);
    if ((ns == NULL) || (xmlStrEqual(ns, BAD_CAST "##any")))
	wildc->any = 1;
    else if (xmlStrEqual(ns, BAD_CAST "##other")) {
	wildc->negNsSet = xmlSchemaNewWildcardNsConstraint(ctxt);
	if (wildc->negNsSet == NULL) {	    	    
	    return (-1);
	}
	wildc->negNsSet->value = schema->targetNamespace; 
    } else {    
	const xmlChar *end, *cur;

	cur = ns;
	do {
	    while (IS_BLANK_CH(*cur))
		cur++;
	    end = cur;
	    while ((*end != 0) && (!(IS_BLANK_CH(*end))))
		end++;
	    if (end == cur)
		break;
	    nsItem = xmlStrndup(cur, end - cur);    	    
	    if ((xmlStrEqual(nsItem, BAD_CAST "##other")) ||
		    (xmlStrEqual(nsItem, BAD_CAST "##any"))) {
		xmlSchemaPSimpleTypeErr(ctxt, 
		    XML_SCHEMAP_WILDCARD_INVALID_NS_MEMBER,
		    NULL, NULL, (xmlNodePtr) attr,
		    NULL, 
		    "((##any | ##other) | List of (anyURI | "
		    "(##targetNamespace | ##local)))", 
		    nsItem, NULL, NULL, NULL);
		ret = XML_SCHEMAP_WILDCARD_INVALID_NS_MEMBER;
	    } else {
		if (xmlStrEqual(nsItem, BAD_CAST "##targetNamespace")) {
		    dictnsItem = schema->targetNamespace;
		} else if (xmlStrEqual(nsItem, BAD_CAST "##local")) {
		    dictnsItem = NULL;
		} else {
		    /*
		    * Validate the item (anyURI).
		    */
		    xmlSchemaPValAttrNodeValue(ctxt, NULL, NULL, attr, 
			nsItem, xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYURI));
		    dictnsItem = xmlDictLookup(ctxt->dict, nsItem, -1);
		}
		/*
		* Avoid dublicate namespaces.
		*/
		tmp = wildc->nsSet;
		while (tmp != NULL) {
		    if (dictnsItem == tmp->value)
			break;
		    tmp = tmp->next;
		}
		if (tmp == NULL) {
		    tmp = xmlSchemaNewWildcardNsConstraint(ctxt);
		    if (tmp == NULL) {
			xmlFree(nsItem);			
			return (-1);
		    }
		    tmp->value = dictnsItem;
		    tmp->next = NULL;
		    if (wildc->nsSet == NULL) 
			wildc->nsSet = tmp;
		    else
			lastNs->next = tmp;
		    lastNs = tmp;
		}

	    }	
	    xmlFree(nsItem);
	    cur = end;
	} while (*cur != 0);    
    }
    return (ret);
}

static int
xmlSchemaPCheckParticleCorrect_2(xmlSchemaParserCtxtPtr ctxt, 
				 xmlSchemaParticlePtr item ATTRIBUTE_UNUSED, 
				 xmlNodePtr node,
				 int minOccurs,
				 int maxOccurs) {

    if ((maxOccurs == 0) && ( minOccurs == 0))
	return (0);
    if (maxOccurs != UNBOUNDED) {
	/*
	* TODO: Maby we should better not create the particle, 
	* if min/max is invalid, since it could confuse the build of the 
	* content model.
	*/
	/* 
	* 3.9.6 Schema Component Constraint: Particle Correct
	*
	*/
	if (maxOccurs < 1) { 
	    /* 
	    * 2.2 {max occurs} must be greater than or equal to 1.
	    */
	    xmlSchemaPCustomAttrErr(ctxt,
		XML_SCHEMAP_P_PROPS_CORRECT_2_2,
		NULL, NULL,
		xmlSchemaGetPropNode(node, "maxOccurs"),
		"The value must be greater than or equal to 1");
	    return (XML_SCHEMAP_P_PROPS_CORRECT_2_2);
	} else if (minOccurs > maxOccurs) {
	    /*
	    * 2.1 {min occurs} must not be greater than {max occurs}.
	    */
	    xmlSchemaPCustomAttrErr(ctxt,
		XML_SCHEMAP_P_PROPS_CORRECT_2_1, 
		NULL, NULL,
		xmlSchemaGetPropNode(node, "minOccurs"),
		"The value must not be greater than the value of 'maxOccurs'");
	    return (XML_SCHEMAP_P_PROPS_CORRECT_2_1);
	}
    }	
    return (0);
}

/**
 * xmlSchemaParseAny:
 * @ctxt:  a schema validation context
 * @schema:  the schema being built
 * @node:  a subtree containing XML Schema informations
 *
 * Parsea a XML schema <any> element. A particle and wildcard
 * will be created (except if minOccurs==maxOccurs==0, in this case
 * nothing will be created).
 * *WARNING* this interface is highly subject to change
 *
 * Returns the particle or NULL in case of error or if minOccurs==maxOccurs==0
 */
static xmlSchemaParticlePtr
xmlSchemaParseAny(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                  xmlNodePtr node)
{
    xmlSchemaParticlePtr particle;
    xmlNodePtr child = NULL;
    xmlSchemaWildcardPtr wild;
    int min, max;
    xmlAttrPtr attr;
    xmlSchemaAnnotPtr annot = NULL;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);
    /*
    * Check for illegal attributes.
    */
    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "minOccurs")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "maxOccurs")) &&
	        (!xmlStrEqual(attr->name, BAD_CAST "namespace")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "processContents"))) {
		xmlSchemaPIllegalAttrErr(ctxt, 
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		    NULL, NULL, attr);		    
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt, 
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		NULL, NULL, attr);	
	}
	attr = attr->next;
    }
    xmlSchemaPValAttrID(ctxt, NULL, NULL, node, BAD_CAST "id");
    /*
    * minOccurs/maxOccurs.
    */  
    max = xmlGetMaxOccurs(ctxt, node, 0, UNBOUNDED, 1, 
	"(nonNegativeInteger | unbounded)");
    min = xmlGetMinOccurs(ctxt, node, 0, -1, 1, 
	"nonNegativeInteger");      
    xmlSchemaPCheckParticleCorrect_2(ctxt, NULL, node, min, max);
    /*
    * Create & parse the wildcard.
    */
    wild = xmlSchemaAddWildcard(ctxt, schema, XML_SCHEMA_TYPE_ANY, node);
    if (wild == NULL)
	return (NULL);
    xmlSchemaParseWildcardNs(ctxt, schema, wild, node);
    /*
    * And now for the children...
    */
    child = node->children;    
    if (IS_SCHEMA(child, "annotation")) {
        annot = xmlSchemaParseAnnotation(ctxt, schema, child);
        child = child->next;
    }
    if (child != NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
	    NULL, NULL, node, child, 
	    NULL, "(annotation?)");
    }
    /*
    * No component if minOccurs==maxOccurs==0.
    */
    if ((min == 0) && (max == 0)) {
	/* Don't free the wildcard, since it's already on the list. */
	return (NULL);
    }
    /*
    * Create the particle.
    */
    particle = xmlSchemaAddParticle(ctxt, schema, node, min, max);
    if (particle == NULL)
        return (NULL);
    particle->annot = annot;
    particle->children = (xmlSchemaTreeItemPtr) wild;    

    return (particle);
}

/**
 * xmlSchemaParseNotation:
 * @ctxt:  a schema validation context
 * @schema:  the schema being built
 * @node:  a subtree containing XML Schema informations
 *
 * parse a XML schema Notation declaration
 *
 * Returns the new structure or NULL in case of error
 */
static xmlSchemaNotationPtr
xmlSchemaParseNotation(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                       xmlNodePtr node)
{
    const xmlChar *name;
    xmlSchemaNotationPtr ret;
    xmlNodePtr child = NULL;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);
    name = xmlSchemaGetProp(ctxt, node, "name");
    if (name == NULL) {
        xmlSchemaPErr2(ctxt, node, child, XML_SCHEMAP_NOTATION_NO_NAME,
                       "Notation has no name\n", NULL, NULL);
        return (NULL);
    }
    ret = xmlSchemaAddNotation(ctxt, schema, name);
    if (ret == NULL) {
        return (NULL);
    }
    ret->targetNamespace = schema->targetNamespace;

    xmlSchemaPValAttrID(ctxt, NULL, (xmlSchemaTypePtr) ret,
	node, BAD_CAST "id");

     if (IS_SCHEMA(child, "annotation")) {
        ret->annot = xmlSchemaParseAnnotation(ctxt, schema, child);
        child = child->next;
    }

    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
        ret->annot = xmlSchemaParseAnnotation(ctxt, schema, child);
        child = child->next;
    }
    if (child != NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
	    NULL, NULL, node, child, 
	    NULL, "(annotation?)");
    }

    return (ret);
}

/**
 * xmlSchemaParseAnyAttribute:
 * @ctxt:  a schema validation context
 * @schema:  the schema being built
 * @node:  a subtree containing XML Schema informations
 *
 * parse a XML schema AnyAttrribute declaration
 * *WARNING* this interface is highly subject to change
 *
 * Returns a wildcard or NULL.
 */
static xmlSchemaWildcardPtr
xmlSchemaParseAnyAttribute(xmlSchemaParserCtxtPtr ctxt,
                           xmlSchemaPtr schema, xmlNodePtr node)
{
    xmlSchemaWildcardPtr ret;
    xmlNodePtr child = NULL;
    xmlAttrPtr attr;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);

    ret = xmlSchemaAddWildcard(ctxt, schema, XML_SCHEMA_TYPE_ANY_ATTRIBUTE,
	node);
    if (ret == NULL) {
        return (NULL);
    }
    /*
    * Check for illegal attributes.
    */
    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "id")) &&
	        (!xmlStrEqual(attr->name, BAD_CAST "namespace")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "processContents"))) {
		xmlSchemaPIllegalAttrErr(ctxt, 
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		    NULL, NULL, attr);		    
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt, 
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		NULL, NULL, attr);	
	}
	attr = attr->next;
    }
    xmlSchemaPValAttrID(ctxt, NULL, (xmlSchemaTypePtr) ret,
	node, BAD_CAST "id");
    /*
    * Parse the namespace list.
    */
    if (xmlSchemaParseWildcardNs(ctxt, schema, ret, node) != 0) 	
	return (NULL);
    /*
    * And now for the children...
    */
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
        ret->annot = xmlSchemaParseAnnotation(ctxt, schema, child);
        child = child->next;
    }
    if (child != NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
	    NULL, NULL, node, child, 
	    NULL, "(annotation?)");
    }

    return (ret);
}


/**
 * xmlSchemaParseAttribute:
 * @ctxt:  a schema validation context
 * @schema:  the schema being built
 * @node:  a subtree containing XML Schema informations
 *
 * parse a XML schema Attrribute declaration
 * *WARNING* this interface is highly subject to change
 *
 * Returns the attribute declaration.
 */
static xmlSchemaAttributePtr
xmlSchemaParseAttribute(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                        xmlNodePtr node, int topLevel)
{
    const xmlChar *name, *attrValue;
    xmlChar *repName = NULL; /* The reported designation. */
    xmlSchemaAttributePtr ret;
    xmlNodePtr child = NULL;    
    xmlAttrPtr attr, nameAttr;
    int isRef = 0;

    /*
     * Note that the w3c spec assumes the schema to be validated with schema
     * for schemas beforehand.
     *
     * 3.2.3 Constraints on XML Representations of Attribute Declarations
     */

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);
    attr = xmlSchemaGetPropNode(node, "ref");
    nameAttr = xmlSchemaGetPropNode(node, "name");

    if ((attr == NULL) && (nameAttr == NULL)) {
	/* 
	* 3.2.3 : 3.1
	* One of ref or name must be present, but not both 
	*/
	xmlSchemaPMissingAttrErr(ctxt, XML_SCHEMAP_SRC_ATTRIBUTE_3_1, 
	    (xmlChar **) &xmlSchemaElemDesAttrDecl, NULL, node, NULL, 
	    "One of the attributes 'ref' or 'name' must be present");
	return (NULL);
    }
    if ((topLevel) || (attr == NULL)) {
	if (nameAttr == NULL) {
	    xmlSchemaPMissingAttrErr(ctxt, XML_SCHEMAP_S4S_ATTR_MISSING, 
		(xmlChar **) &xmlSchemaElemDesAttrDecl, NULL, node, 
		"name", NULL);
	    return (NULL);
	}	
    } else
	isRef = 1;	
    
    if (isRef) {
	char buf[50]; 
	const xmlChar *refNs = NULL, *ref = NULL, *refPrefix = NULL; 

	/*
	* Parse as attribute reference.
	*/		
	if (xmlSchemaPValAttrNodeQName(ctxt, schema, 
	    (xmlChar **) &xmlSchemaElemDesAttrRef, NULL, attr, &refNs, 
	    &refPrefix, &ref) != 0) {
	    return (NULL);
	}	
        snprintf(buf, 49, "#aRef%d", ctxt->counter++ + 1);
        name = (const xmlChar *) buf;	
	ret = xmlSchemaAddAttribute(ctxt, schema, name, NULL, node, 0);
	if (ret == NULL) {
	    if (repName != NULL)
		xmlFree(repName);
	    return (NULL);
	}
	ret->type = XML_SCHEMA_TYPE_ATTRIBUTE;
	ret->node = node;
	ret->refNs = refNs;	
	ret->refPrefix = refPrefix;
	ret->ref = ref;
	xmlSchemaCheckReference(ctxt, schema, node, (xmlSchemaBasicItemPtr) ret,
	    refNs);
	/*
	xmlSchemaFormatTypeRep(&repName, (xmlSchemaTypePtr) ret, NULL, NULL);
	*/
	if (nameAttr != NULL)
	    xmlSchemaPMutualExclAttrErr(ctxt, XML_SCHEMAP_SRC_ATTRIBUTE_3_1, 
		&repName, (xmlSchemaTypePtr) ret, nameAttr, 
		"ref", "name");
	/*
	* Check for illegal attributes.
	*/
	attr = node->properties;
	while (attr != NULL) {
	    if (attr->ns == NULL) {
		if (xmlStrEqual(attr->name, BAD_CAST "type") ||
		    xmlStrEqual(attr->name, BAD_CAST "form")) {
		    /* 
		    * 3.2.3 : 3.2
		    * If ref is present, then all of <simpleType>,
		    * form and type must be absent. 
		    */
		    xmlSchemaPIllegalAttrErr(ctxt, 
			XML_SCHEMAP_SRC_ATTRIBUTE_3_2, &repName, 
			(xmlSchemaTypePtr) ret, attr);
		} else if ((!xmlStrEqual(attr->name, BAD_CAST "ref")) &&
		    (!xmlStrEqual(attr->name, BAD_CAST "use")) &&
		    (!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		    (!xmlStrEqual(attr->name, BAD_CAST "name")) && 
		    (!xmlStrEqual(attr->name, BAD_CAST "fixed")) && 
		    (!xmlStrEqual(attr->name, BAD_CAST "default"))) {
		    xmlSchemaPIllegalAttrErr(ctxt, 
			XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
			&repName, (xmlSchemaTypePtr) ret, attr);		    
		}
	    } else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
		xmlSchemaPIllegalAttrErr(ctxt, 
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		    &repName, (xmlSchemaTypePtr) ret, attr);		
	    }
	    attr = attr->next;
	}	
    } else {
        const xmlChar *ns = NULL;
	
	/*
	* Parse as attribute declaration.
	*/			
	if (xmlSchemaPValAttrNode(ctxt, 
	    (xmlChar **) &xmlSchemaElemDesAttrDecl, NULL, nameAttr, 
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_NCNAME), &name) != 0) {
	    return (NULL);
	}
	/*
	xmlSchemaFormatTypeRep(&repName, NULL, xmlSchemaElemDesAttrDecl, name);
	*/
	/* 
	* 3.2.6 Schema Component Constraint: xmlns Not Allowed 
	*/
	if (xmlStrEqual(name, BAD_CAST "xmlns")) {
	    xmlSchemaPSimpleTypeErr(ctxt, 
		XML_SCHEMAP_NO_XMLNS, 
		&repName, NULL, (xmlNodePtr) nameAttr, 
		xmlSchemaGetBuiltInType(XML_SCHEMAS_NCNAME), "NCName", NULL,
		"The value must not match 'xmlns'", 
		NULL, NULL);	    
	    if (repName != NULL)
		xmlFree(repName);
	    return (NULL);
	}	    
	/* 
	* Evaluate the target namespace 
	*/	
	if (topLevel) {
	    ns = schema->targetNamespace;
	} else {
	    attr = xmlSchemaGetPropNode(node, "form");
	    if (attr != NULL) {
		attrValue = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);
		if (xmlStrEqual(attrValue, BAD_CAST "qualified")) {
		    ns = schema->targetNamespace;
		} else if (!xmlStrEqual(attrValue, BAD_CAST "unqualified")) {
		    xmlSchemaPSimpleTypeErr(ctxt, 
			XML_SCHEMAP_S4S_ATTR_INVALID_VALUE, 
			&repName, NULL, (xmlNodePtr) attr, 
			NULL, "(qualified | unqualified)", 
			attrValue, NULL, NULL, NULL);			
		}
	    } else if (schema->flags & XML_SCHEMAS_QUALIF_ATTR)
		ns = schema->targetNamespace;		
	}	
        ret = xmlSchemaAddAttribute(ctxt, schema, name, ns, node, topLevel);
	if (ret == NULL) {
	    if (repName != NULL)
		xmlFree(repName);
	    return (NULL);
	}
	ret->type = XML_SCHEMA_TYPE_ATTRIBUTE;
	ret->node = node;	
	if (topLevel)
	    ret->flags |= XML_SCHEMAS_ATTR_GLOBAL;
	/* 
	* 3.2.6 Schema Component Constraint: xsi: Not Allowed 
	*/	
	if (xmlStrEqual(ret->targetNamespace, xmlSchemaInstanceNs)) {
	    xmlSchemaPCustomErr(ctxt, 
		XML_SCHEMAP_NO_XSI,
		&repName, (xmlSchemaTypePtr) ret, node,
		"The target namespace must not match '%s'", 
		xmlSchemaInstanceNs);	        
	}
	/*
	* Check for illegal attributes. 
	*/	
	attr = node->properties;
	while (attr != NULL) {
	    if (attr->ns == NULL) {		
		if ((!xmlStrEqual(attr->name, BAD_CAST "id")) && 
		    (!xmlStrEqual(attr->name, BAD_CAST "default")) && 				
		    (!xmlStrEqual(attr->name, BAD_CAST "fixed")) &&		    
		    (!xmlStrEqual(attr->name, BAD_CAST "name")) &&
		    (!xmlStrEqual(attr->name, BAD_CAST "type"))) {
		    if ((topLevel) ||						    		
		        ((!xmlStrEqual(attr->name, BAD_CAST "form")) &&
			 (!xmlStrEqual(attr->name, BAD_CAST "use")))) {
			xmlSchemaPIllegalAttrErr(ctxt, 
			    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
			    &repName, (xmlSchemaTypePtr) ret, attr);	
		    }
		}
	    } else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
		xmlSchemaPIllegalAttrErr(ctxt, XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		    &repName, (xmlSchemaTypePtr) ret, attr);	
	    }
	    attr = attr->next;
	}
	xmlSchemaPValAttrQName(ctxt, schema, &repName, (xmlSchemaTypePtr) ret,
	    node, "type", &ret->typeNs, NULL, &ret->typeName);
    }        
    xmlSchemaPValAttrID(ctxt, NULL, (xmlSchemaTypePtr) ret,
	node, BAD_CAST "id");
    /*
    * Attribute "fixed".
    */
    ret->defValue = xmlSchemaGetProp(ctxt, node, "fixed");
    if (ret->defValue != NULL)
	ret->flags |= XML_SCHEMAS_ATTR_FIXED;
    /* 
    * Attribute "default".
    */
    attr = xmlSchemaGetPropNode(node, "default");
    if (attr != NULL) {
	/* 
	* 3.2.3 : 1
	* default and fixed must not both be present. 
	*/
	if (ret->flags & XML_SCHEMAS_ATTR_FIXED) {
	    xmlSchemaPMutualExclAttrErr(ctxt, XML_SCHEMAP_SRC_ATTRIBUTE_1,
		&repName, (xmlSchemaTypePtr) ret, attr, "default", "fixed");
	} else
	    ret->defValue = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);		
    }    
    if (topLevel == 0) {
	/* 
	* Attribute "use". 
	*/
	attr = xmlSchemaGetPropNode(node, "use");
	if (attr != NULL) {
	    attrValue = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);
	    if (xmlStrEqual(attrValue, BAD_CAST "optional"))
		ret->occurs = XML_SCHEMAS_ATTR_USE_OPTIONAL;
	    else if (xmlStrEqual(attrValue, BAD_CAST "prohibited"))
		ret->occurs = XML_SCHEMAS_ATTR_USE_PROHIBITED;
	    else if (xmlStrEqual(attrValue, BAD_CAST "required"))
		ret->occurs = XML_SCHEMAS_ATTR_USE_REQUIRED;
	    else
		xmlSchemaPSimpleTypeErr(ctxt, 
		    XML_SCHEMAP_INVALID_ATTR_USE, 
		    &repName, (xmlSchemaTypePtr) ret, (xmlNodePtr) attr, 
		    NULL, "(optional | prohibited | required)", 
		    attrValue, NULL, NULL, NULL);				
	} else
	    ret->occurs = XML_SCHEMAS_ATTR_USE_OPTIONAL;
	/* 
	* 3.2.3 : 2
	* If default and use are both present, use must have
	* the actual value optional.
	*/
	if ((ret->occurs != XML_SCHEMAS_ATTR_USE_OPTIONAL) && 
	    (ret->defValue != NULL) && 
	    ((ret->flags & XML_SCHEMAS_ATTR_FIXED) == 0)) {
	    xmlSchemaPSimpleTypeErr(ctxt, 
		XML_SCHEMAP_SRC_ATTRIBUTE_2, 
		&repName, (xmlSchemaTypePtr) ret, (xmlNodePtr) attr, 
		NULL, "(optional | prohibited | required)", NULL, 
		"The value must be 'optional' if the attribute "
		"'default' is present as well", NULL, NULL);	    
	}
    }                          
    /*
    * And now for the children...
    */
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
        ret->annot = xmlSchemaParseAnnotation(ctxt, schema, child);
        child = child->next;
    }    
    if (isRef) {
	if (child != NULL) {	    
	    if (IS_SCHEMA(child, "simpleType"))
		/* 
		* 3.2.3 : 3.2
		* If ref is present, then all of <simpleType>,
		* form and type must be absent. 
		*/
		xmlSchemaPContentErr(ctxt, XML_SCHEMAP_SRC_ATTRIBUTE_3_2,
		    &repName, (xmlSchemaTypePtr) ret, node, child, NULL,
		    "(annotation?)");
	    else 
		xmlSchemaPContentErr(ctxt, XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
		    &repName, (xmlSchemaTypePtr) ret, node, child, NULL,
		    "(annotation?)");  
	}
    } else {
	if (IS_SCHEMA(child, "simpleType")) {
	    if (ret->typeName != NULL) {
		/* 
		* 3.2.3 : 4
		* type and <simpleType> must not both be present. 
		*/
		xmlSchemaPContentErr(ctxt, XML_SCHEMAP_SRC_ATTRIBUTE_4,
		    &repName,  (xmlSchemaTypePtr) ret, node, child,
		    "The attribute 'type' and the <simpleType> child "
		    "are mutually exclusive", NULL);
	    } else
		ret->subtypes = xmlSchemaParseSimpleType(ctxt, schema, child, 0);
	    child = child->next;
	}
	if (child != NULL)
	    xmlSchemaPContentErr(ctxt, XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
		&repName, (xmlSchemaTypePtr) ret, node, child, NULL,
		"(annotation?, simpleType?)");
    }
    /*
    * Cleanup.
    */
    if (repName != NULL)
	xmlFree(repName);
    return (ret);
}

/**
 * xmlSchemaParseAttributeGroup:
 * @ctxt:  a schema validation context
 * @schema:  the schema being built
 * @node:  a subtree containing XML Schema informations
 *
 * parse a XML schema Attribute Group declaration
 * *WARNING* this interface is highly subject to change
 *
 * Returns the attribute group or NULL in case of error.
 */
static xmlSchemaAttributeGroupPtr
xmlSchemaParseAttributeGroup(xmlSchemaParserCtxtPtr ctxt,
                             xmlSchemaPtr schema, xmlNodePtr node,
			     int topLevel)
{
    const xmlChar *name;
    xmlSchemaAttributeGroupPtr ret;
    xmlNodePtr child = NULL;
    const xmlChar *oldcontainer;    
    xmlAttrPtr attr, nameAttr;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);

    nameAttr = xmlSchemaGetPropNode(node, "name");
    attr = xmlSchemaGetPropNode(node, "ref");   
    if ((topLevel) || (attr == NULL)) {
	/*
	* Parse as an attribute group definition.
	* Note that those are allowed at top level only.
	*/
	if (nameAttr == NULL) {
	    xmlSchemaPMissingAttrErr(ctxt,
		XML_SCHEMAP_S4S_ATTR_MISSING,
		NULL, NULL, node, "name", NULL);	    
	    return (NULL);
	}
	name = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) nameAttr);
	/*
	* The name is crucial, exit if invalid. 
	*/
	if (xmlSchemaPValAttrNode(ctxt,
	    NULL, NULL, nameAttr, 
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_NCNAME), &name) != 0) {
	    return (NULL);
	}
	ret = xmlSchemaAddAttributeGroup(ctxt, schema, name, node);
	if (ret == NULL)
	    return (NULL);
	ret->type = XML_SCHEMA_TYPE_ATTRIBUTEGROUP;
	ret->flags |= XML_SCHEMAS_ATTRGROUP_GLOBAL;
	ret->node = node;
	ret->targetNamespace = schema->targetNamespace;
    } else {    
	char buf[50];
	const xmlChar *refNs = NULL, *ref = NULL, *refPrefix;

	/*
	* Parse as an attribute group definition reference.
	*/
	if (attr == NULL) {
	    xmlSchemaPMissingAttrErr(ctxt, 
		XML_SCHEMAP_S4S_ATTR_MISSING, 
		NULL, NULL, node, "ref", NULL);
	}	
	xmlSchemaPValAttrNodeQName(ctxt, schema,
	    NULL, NULL, attr, &refNs, &refPrefix, &ref);
	 
        snprintf(buf, 49, "#agRef%d", ctxt->counter++ + 1);
	name = (const xmlChar *) buf;
	if (name == NULL) {
	    xmlSchemaPErrMemory(ctxt, "creating internal name for an "
		"attribute group definition reference", node);
            return (NULL);
        }
	ret = xmlSchemaAddAttributeGroup(ctxt, schema, name, node);
	if (ret == NULL)
	    return (NULL);
	ret->type = XML_SCHEMA_TYPE_ATTRIBUTEGROUP;
	ret->ref = ref;
	ret->refNs = refNs;
	/* TODO: Is @refPrefix currently used? */
	ret->refPrefix = refPrefix;	
	ret->node = node;
	xmlSchemaCheckReference(ctxt, schema, node,
	    (xmlSchemaBasicItemPtr) ret, refNs);
    }    
    /*
    * Check for illegal attributes.
    */
    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((((topLevel == 0) && (!xmlStrEqual(attr->name, BAD_CAST "ref"))) ||
		 (topLevel && (!xmlStrEqual(attr->name, BAD_CAST "name")))) &&
		(!xmlStrEqual(attr->name, BAD_CAST "id"))) 
	    {
		xmlSchemaPIllegalAttrErr(ctxt,
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED,
		    NULL, NULL, attr);
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt,
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED,
		NULL, NULL, attr);
	}
	attr = attr->next;
    }	
    /* TODO: Validate "id" ? */  
    /*
    * And now for the children...
    */
    oldcontainer = ctxt->container;
    ctxt->container = name;
    child = node->children;    
    if (IS_SCHEMA(child, "annotation")) {
        ret->annot = xmlSchemaParseAnnotation(ctxt, schema, child);
        child = child->next;
    }
    if (topLevel) {
	child = xmlSchemaParseAttrDecls(ctxt, schema, child, (xmlSchemaTypePtr) ret); 
	if (IS_SCHEMA(child, "anyAttribute")) {
	    ret->attributeWildcard = xmlSchemaParseAnyAttribute(ctxt, schema, child);
	    child = child->next;
	}
    }
    if (child != NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED, 
	    NULL, NULL, node, child, NULL, 
	    "(annotation?)");
    }
    ctxt->container = oldcontainer;
    return (ret);
}

/**
 * xmlSchemaPValAttrFormDefault:
 * @value:  the value
 * @flags: the flags to be modified
 * @flagQualified: the specific flag for "qualified"
 *
 * Returns 0 if the value is valid, 1 otherwise.
 */
static int
xmlSchemaPValAttrFormDefault(const xmlChar *value,
			     int *flags,
			     int flagQualified)
{
    if (xmlStrEqual(value, BAD_CAST "qualified")) {
	if  ((*flags & flagQualified) == 0)
	    *flags |= flagQualified;
    } else if (!xmlStrEqual(value, BAD_CAST "unqualified"))
	return (1);    
	
    return (0);
}

/**
 * xmlSchemaPValAttrBlockFinal:
 * @value:  the value
 * @flags: the flags to be modified
 * @flagAll: the specific flag for "#all"
 * @flagExtension: the specific flag for "extension"
 * @flagRestriction: the specific flag for "restriction"
 * @flagSubstitution: the specific flag for "substitution"
 * @flagList: the specific flag for "list"
 * @flagUnion: the specific flag for "union"
 *
 * Validates the value of the attribute "final" and "block". The value
 * is converted into the specified flag values and returned in @flags.
 *
 * Returns 0 if the value is valid, 1 otherwise.
 */

static int
xmlSchemaPValAttrBlockFinal(const xmlChar *value,
			    int *flags,			
			    int flagAll,
			    int flagExtension,
			    int flagRestriction,
			    int flagSubstitution,
			    int flagList,
			    int flagUnion)			
{
    int ret = 0;

    /*
    * TODO: This does not check for dublicate entries.
    */
    if (value == NULL)
	return (1);
    if (xmlStrEqual(value, BAD_CAST "#all")) {
	if (flagAll != -1)
	    *flags |= flagAll;
	else {
	    if (flagExtension != -1) 
		*flags |= flagExtension; 
	    if (flagRestriction != -1) 
		*flags |= flagRestriction;
	    if (flagSubstitution != -1) 
		*flags |= flagSubstitution;
	    if (flagList != -1) 
		*flags |= flagList;
	    if (flagUnion != -1) 
		*flags |= flagUnion;
	}
    } else {
	const xmlChar *end, *cur = value;
	xmlChar *item;
	
	do {
	    while (IS_BLANK_CH(*cur))
		cur++;
	    end = cur;
	    while ((*end != 0) && (!(IS_BLANK_CH(*end))))
		end++;
	    if (end == cur)
		break;
	    item = xmlStrndup(cur, end - cur);    	    
	    if (xmlStrEqual(item, BAD_CAST "extension")) {
		if (flagExtension != -1) {
		    if ((*flags & flagExtension) == 0)
			*flags |= flagExtension;
		} else 
		    ret = 1;
	    } else if (xmlStrEqual(item, BAD_CAST "restriction")) {
		if (flagRestriction != -1) {
		    if ((*flags & flagRestriction) == 0)
			*flags |= flagRestriction;
		} else 
		    ret = 1;
	    } else if (xmlStrEqual(item, BAD_CAST "substitution")) {
		if (flagSubstitution != -1) {
		    if ((*flags & flagSubstitution) == 0)
			*flags |= flagSubstitution;
		} else 
		    ret = 1;
	    } else if (xmlStrEqual(item, BAD_CAST "list")) {
		if (flagList != -1) {
		    if ((*flags & flagList) == 0)
			*flags |= flagList;
		} else 
		    ret = 1;
	    } else if (xmlStrEqual(item, BAD_CAST "union")) {
		if (flagUnion != -1) {
		    if ((*flags & flagUnion) == 0)
			*flags |= flagUnion;
		} else 
		    ret = 1;
	    } else 
		ret = 1;
	    if (item != NULL)
		xmlFree(item);
	    cur = end;
	} while ((ret == 0) && (*cur != 0)); 
    }    
    
    return (ret);
}

static int
xmlSchemaCheckCSelectorXPath(xmlSchemaParserCtxtPtr ctxt, 
			     xmlSchemaIDCPtr idc,
			     xmlSchemaIDCSelectPtr selector,
			     xmlAttrPtr attr,
			     int isField)
{
    xmlNodePtr node;

    /*
    * c-selector-xpath: 
    * Schema Component Constraint: Selector Value OK
    *
    * TODO: 1 The {selector} must be a valid XPath expression, as defined 
    * in [XPath].
    */
    if (selector == NULL) {
	xmlSchemaPErr(ctxt, idc->node, 
	    XML_SCHEMAP_INTERNAL,
	    "Internal error: xmlSchemaCheckCSelectorXPath, "
	    "the selector is not specified.\n", NULL, NULL);
	return (-1);
    }
    if (attr == NULL)
	node = idc->node;
    else
	node = (xmlNodePtr) attr;
    if (selector->xpath == NULL) {
	xmlSchemaPCustomErr(ctxt,
	    /* TODO: Adjust error code. */
	    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE, 
	    NULL, NULL, node, 
	    "The XPath expression of the selector is not valid", NULL);
	return (XML_SCHEMAP_S4S_ATTR_INVALID_VALUE);
    } else {
	const xmlChar **nsArray = NULL;
	xmlNsPtr *nsList = NULL;
	/*
	* Compile the XPath expression.
	*/
	/*
	* TODO: We need the array of in-scope namespaces for compilation.
	* TODO: Call xmlPatterncompile with different options for selector/
	* field.
	*/
	nsList = xmlGetNsList(attr->doc, attr->parent);
	/*
	* Build an array of prefixes and namespaces.
	*/
	if (nsList != NULL) {
	    int i, count = 0;
	    xmlNsPtr ns;

	    for (i = 0; nsList[i] != NULL; i++)
		count++;

	    nsArray = (const xmlChar **) xmlMalloc(
		(count * 2 + 1) * sizeof(const xmlChar *));
	    if (nsArray == NULL) {
		xmlSchemaPErrMemory(ctxt, "allocating a namespace array",
		    NULL);
		return (-1);
	    }
	    for (i = 0; i < count; i++) {
		ns = nsList[i];
		nsArray[2 * i] = nsList[i]->href;
		nsArray[2 * i + 1] = nsList[i]->prefix;
	    }
	    nsArray[count * 2] = NULL;
	    xmlFree(nsList);
	}
	/*
	* TODO: Differentiate between "selector" and "field".
	*/
	if (isField)
	    selector->xpathComp = (void *) xmlPatterncompile(selector->xpath,
		NULL, 1, nsArray);
	else
	    selector->xpathComp = (void *) xmlPatterncompile(selector->xpath,
		NULL, 1, nsArray);
	if (nsArray != NULL)
	    xmlFree((xmlChar **) nsArray);
	
	if (selector->xpathComp == NULL) {
	    xmlSchemaPCustomErr(ctxt,
		/* TODO: Adjust error code? */
		XML_SCHEMAP_S4S_ATTR_INVALID_VALUE, 
		NULL, NULL, node, 
		"The XPath expression '%s' could not be "
		"compiled", selector->xpath);
	    return (XML_SCHEMAP_S4S_ATTR_INVALID_VALUE);
	}
    }
    return (0);
}

#define ADD_ANNOTATION(annot)   \
    xmlSchemaAnnotPtr cur = item->annot; \
    if (item->annot == NULL) {  \
	item->annot = annot;    \
	return (annot);         \
    }                           \
    cur = item->annot;          \
    if (cur->next != NULL) {    \
	cur = cur->next;	\
    }                           \
    cur->next = annot;

/**
 * xmlSchemaAssignAnnotation:
 * @item: the schema component
 * @annot: the annotation
 *
 * Adds the annotation to the given schema component.
 *
 * Returns the given annotaion.
 */
static xmlSchemaAnnotPtr
xmlSchemaAddAnnotation(xmlSchemaAnnotItemPtr annItem,
		       xmlSchemaAnnotPtr annot)
{   
    if ((annItem == NULL) || (annot == NULL))
	return (NULL);
    switch (annItem->type) {
	case XML_SCHEMA_TYPE_ELEMENT: {
		xmlSchemaElementPtr item = (xmlSchemaElementPtr) annItem;
		ADD_ANNOTATION(annot)
	    }
	    break;
	case XML_SCHEMA_TYPE_ATTRIBUTE: {
		xmlSchemaAttributePtr item = (xmlSchemaAttributePtr) annItem;
		ADD_ANNOTATION(annot)
	    }
	    break;
	case XML_SCHEMA_TYPE_ANY_ATTRIBUTE:
	case XML_SCHEMA_TYPE_ANY: {
		xmlSchemaWildcardPtr item = (xmlSchemaWildcardPtr) annItem;
		ADD_ANNOTATION(annot)
	    }
	    break;
	case XML_SCHEMA_TYPE_PARTICLE:
	case XML_SCHEMA_TYPE_IDC_KEY:
	case XML_SCHEMA_TYPE_IDC_KEYREF:
	case XML_SCHEMA_TYPE_IDC_UNIQUE: { 
		xmlSchemaAnnotItemPtr item = (xmlSchemaAnnotItemPtr) annItem;
		ADD_ANNOTATION(annot)
	    }
	    break;
	case XML_SCHEMA_TYPE_ATTRIBUTEGROUP: {
		xmlSchemaAttributeGroupPtr item =
		    (xmlSchemaAttributeGroupPtr) annItem;
		ADD_ANNOTATION(annot)
	    }
	    break;
	case XML_SCHEMA_TYPE_NOTATION: {
		xmlSchemaNotationPtr item = (xmlSchemaNotationPtr) annItem;
		ADD_ANNOTATION(annot)
	    }
	    break;
	case XML_SCHEMA_FACET_MININCLUSIVE:
	case XML_SCHEMA_FACET_MINEXCLUSIVE:
	case XML_SCHEMA_FACET_MAXINCLUSIVE:
	case XML_SCHEMA_FACET_MAXEXCLUSIVE:
	case XML_SCHEMA_FACET_TOTALDIGITS:
	case XML_SCHEMA_FACET_FRACTIONDIGITS:
	case XML_SCHEMA_FACET_PATTERN:
	case XML_SCHEMA_FACET_ENUMERATION:
	case XML_SCHEMA_FACET_WHITESPACE:
	case XML_SCHEMA_FACET_LENGTH:
	case XML_SCHEMA_FACET_MAXLENGTH:
	case XML_SCHEMA_FACET_MINLENGTH: {
		xmlSchemaFacetPtr item = (xmlSchemaFacetPtr) annItem;
		ADD_ANNOTATION(annot)
	    }
	    break;	
	case XML_SCHEMA_TYPE_SIMPLE:
	case XML_SCHEMA_TYPE_COMPLEX: {
		xmlSchemaTypePtr item = (xmlSchemaTypePtr) annItem;
		ADD_ANNOTATION(annot)
	    }
	    break;
	case XML_SCHEMA_TYPE_GROUP: {
		xmlSchemaModelGroupDefPtr item = (xmlSchemaModelGroupDefPtr) annItem;
		ADD_ANNOTATION(annot)
	    }
	    break;
	case XML_SCHEMA_TYPE_SEQUENCE:
	case XML_SCHEMA_TYPE_CHOICE:
	case XML_SCHEMA_TYPE_ALL: {
		xmlSchemaModelGroupPtr item = (xmlSchemaModelGroupPtr) annItem;
		ADD_ANNOTATION(annot)
	    }
	    break;
	default:
	     xmlSchemaPCustomErr(NULL,
		XML_SCHEMAP_INTERNAL, 
		NULL, NULL, NULL,
		"Internal error: xmlSchemaAddAnnotation, "
		"The item is not a annotated schema component", NULL);
	     break;
    }
    return (annot);
}

/**
 * xmlSchemaParseIDCSelectorAndField:
 * @ctxt:  a schema validation context
 * @schema:  the schema being built
 * @node:  a subtree containing XML Schema informations
 *
 * Parses a XML Schema identity-contraint definition's
 * <selector> and <field> elements.
 *
 * Returns the parsed identity-constraint definition.
 */
static xmlSchemaIDCSelectPtr
xmlSchemaParseIDCSelectorAndField(xmlSchemaParserCtxtPtr ctxt, 
			  xmlSchemaPtr schema,
			  xmlSchemaIDCPtr idc,
			  xmlNodePtr node,
			  int isField)
{
    xmlSchemaIDCSelectPtr item;
    xmlNodePtr child = NULL;
    xmlAttrPtr attr;
    
    /*
    * Check for illegal attributes.
    */
    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "xpath"))) {
		xmlSchemaPIllegalAttrErr(ctxt, 
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		    NULL, NULL, attr);		    
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt, 
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		NULL, NULL, attr);	
	}
	attr = attr->next;
    }      
    /*
    * Create the item.
    */       
    item = (xmlSchemaIDCSelectPtr) xmlMalloc(sizeof(xmlSchemaIDCSelect));
    if (item == NULL) {
        xmlSchemaPErrMemory(ctxt, 
	    "allocating a 'selector' of an identity-constraint definition", 
	    NULL);
        return (NULL);
    }
    memset(item, 0, sizeof(xmlSchemaIDCSelect));   
    /*
    * Attribute "xpath" (mandatory).
    */
    attr = xmlSchemaGetPropNode(node, "xpath");
    if (attr == NULL) {
    	xmlSchemaPMissingAttrErr(ctxt, 
	    XML_SCHEMAP_S4S_ATTR_MISSING, 
	    NULL, NULL, node,
	    "name", NULL);
    } else {
	item->xpath = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);
	/*
	* URGENT TODO: "field"s have an other syntax than "selector"s.
	*/

	if (xmlSchemaCheckCSelectorXPath(ctxt, idc, item, attr,
	    isField) == -1) {
	    xmlSchemaPErr(ctxt, 
		(xmlNodePtr) attr, 
		XML_SCHEMAP_INTERNAL,		
		"Internal error: xmlSchemaParseIDCSelectorAndField, "
		"validating the XPath expression of a IDC selector.\n", 
		NULL, NULL);
	}

    }    
    xmlSchemaPValAttrID(ctxt, NULL, NULL, node, BAD_CAST "id");
    /*
    * And now for the children...
    */
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
	/*
	* Add the annotation to the parent IDC.
	*/
	xmlSchemaAddAnnotation((xmlSchemaAnnotItemPtr) idc, 
	    xmlSchemaParseAnnotation(ctxt, schema, child));
	child = child->next;
    }  		    
    if (child != NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
	    NULL, NULL, node, child, 
	    NULL, "(annotation?)");
    }
    
    return (item);
}

/**
 * xmlSchemaParseIDC:
 * @ctxt:  a schema validation context
 * @schema:  the schema being built
 * @node:  a subtree containing XML Schema informations
 *
 * Parses a XML Schema identity-contraint definition.
 *
 * Returns the parsed identity-constraint definition.
 */
static xmlSchemaIDCPtr
xmlSchemaParseIDC(xmlSchemaParserCtxtPtr ctxt, 
		  xmlSchemaPtr schema,
		  xmlNodePtr node,
		  xmlSchemaTypeType idcCategory,
		  const xmlChar *targetNamespace)
{
    xmlSchemaIDCPtr item = NULL;
    xmlNodePtr child = NULL;
    xmlAttrPtr attr;
    const xmlChar *name = NULL;
    xmlSchemaIDCSelectPtr field = NULL, lastField = NULL;
    int resAdd;
        
    /*
    * Check for illegal attributes.
    */
    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "name")) &&
		((idcCategory != XML_SCHEMA_TYPE_IDC_KEYREF) ||
		 (!xmlStrEqual(attr->name, BAD_CAST "refer")))) {
		xmlSchemaPIllegalAttrErr(ctxt, 
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		    NULL, NULL, attr);		    
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt, 
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		NULL, NULL, attr);	
	}
	attr = attr->next;
    }  
    /*
    * Attribute "name" (mandatory).
    */
    attr = xmlSchemaGetPropNode(node, "name");
    if (attr == NULL) {
	xmlSchemaPMissingAttrErr(ctxt, 
	    XML_SCHEMAP_S4S_ATTR_MISSING, 
	    NULL, NULL, node,
	    "name", NULL);
	return (NULL);
    } else if (xmlSchemaPValAttrNode(ctxt, 
	NULL, NULL, attr, 
	xmlSchemaGetBuiltInType(XML_SCHEMAS_NCNAME), &name) != 0) {
	return (NULL);
    }    
    /*
    * Create the component.
    */
    if (schema->idcDef == NULL)
        schema->idcDef = xmlHashCreateDict(10, ctxt->dict);
    if (schema->idcDef == NULL) 
        return (NULL);

    item = (xmlSchemaIDCPtr) xmlMalloc(sizeof(xmlSchemaIDC));
    if (item == NULL) {
        xmlSchemaPErrMemory(ctxt, 
	    "allocating an identity-constraint definition", NULL);
        return (NULL);
    }    
    /*
    * Add the IDC to the list of IDCs on the schema component.
    */
    resAdd = xmlHashAddEntry2(schema->idcDef, name, targetNamespace, item);
    if (resAdd != 0) {	           
	xmlSchemaPCustomErrExt(ctxt,
	    XML_SCHEMAP_REDEFINED_TYPE,
	    NULL, NULL, node, 
	    "An identity-constraint definition with the name '%s' "
	    "and targetNamespace '%s' does already exist", 
	    name, targetNamespace, NULL);
	xmlFree(item);
	return (NULL);
    }
    memset(item, 0, sizeof(xmlSchemaIDC));
    item->name = name;
    item->type = idcCategory;  
    item->node = node;
    /*
    * The target namespace of the parent element declaration.
    */
    item->targetNamespace = targetNamespace;   
    xmlSchemaPValAttrID(ctxt, NULL, (xmlSchemaTypePtr) item,
	node, BAD_CAST "id");
    if (idcCategory == XML_SCHEMA_TYPE_IDC_KEYREF) {
	/*
	* Attribute "refer" (mandatory).
	*/
	attr = xmlSchemaGetPropNode(node, "refer");
	if (attr == NULL) {
	    xmlSchemaPMissingAttrErr(ctxt, 
		XML_SCHEMAP_S4S_ATTR_MISSING, 
		NULL, NULL, node,
		"refer", NULL);
	} else {
	    /*
	    * Create a reference item.
	    */
	    item->ref = xmlSchemaNewQNameRef(schema, XML_SCHEMA_TYPE_IDC_KEY,
		NULL, NULL);		
	    if (item->ref == NULL)	
		return (NULL);
	    xmlSchemaPValAttrNodeQName(ctxt, schema,
		NULL, NULL, attr, 
		&(item->ref->targetNamespace), 0, 
		&(item->ref->name));
	    xmlSchemaCheckReference(ctxt, schema, node,
		(xmlSchemaBasicItemPtr) item,
		item->ref->targetNamespace);
	}
    }
    /*
    * And now for the children...
    */
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
	item->annot = xmlSchemaParseAnnotation(ctxt, schema, child);
	child = child->next;
    }
    if (child == NULL) {
	xmlSchemaPContentErr(ctxt,
		XML_SCHEMAP_S4S_ELEM_MISSING,
		NULL, NULL, node, child, 
		"A child element is missing",
		"(annotation?, (selector, field+))");
    }
    /*
    * Child element <selector>.
    */
    if (IS_SCHEMA(child, "selector")) {    
	item->selector = xmlSchemaParseIDCSelectorAndField(ctxt, schema, 
	    item, child, 0);
	child = child->next;
	/*
	* Child elements <field>.
	*/
	if (IS_SCHEMA(child, "field")) {
	    do {
		field = xmlSchemaParseIDCSelectorAndField(ctxt, schema, 
		    item, child, 1);
		if (field != NULL) {
		    field->index = item->nbFields;
		    item->nbFields++;
		    if (lastField != NULL)
			lastField->next = field;		    
		    else
			item->fields = field;
		    lastField = field;
		}
		child = child->next;
	    } while (IS_SCHEMA(child, "field"));
	} else {
	    xmlSchemaPContentErr(ctxt,
		XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
		NULL, NULL, node, child, 
		NULL, "(annotation?, (selector, field+))");
	}
    }    
    if (child != NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
	    NULL, NULL, node, child, 
	    NULL, "(annotation?, (selector, field+))");
    }		

    return (item);
}

/**
 * xmlSchemaParseElement:
 * @ctxt:  a schema validation context
 * @schema:  the schema being built
 * @node:  a subtree containing XML Schema informations
 * @topLevel: indicates if this is global declaration
 *
 * Parses a XML schema element declaration.
 * *WARNING* this interface is highly subject to change
 *
 * Returns the element declaration or a particle; NULL in case
 * of an error or if the particle has minOccurs==maxOccurs==0.
 */
static xmlSchemaBasicItemPtr
xmlSchemaParseElement(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                      xmlNodePtr node, int topLevel)
{    
    xmlSchemaElementPtr decl = NULL;
    xmlSchemaParticlePtr particle = NULL;    
    xmlSchemaAnnotPtr annot = NULL;
    xmlNodePtr child = NULL;    
    xmlAttrPtr attr, nameAttr;    
    int min, max, isRef = 0;
    xmlChar *des = NULL;

    /* 3.3.3 Constraints on XML Representations of Element Declarations */
    /* TODO: Complete implementation of 3.3.6 */
   
    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);    
    /*
    * If we get a "ref" attribute on a local <element> we will assume it's
    * a reference - even if there's a "name" attribute; this seems to be more 
    * robust.
    */
    nameAttr = xmlSchemaGetPropNode(node, "name");
    attr = xmlSchemaGetPropNode(node, "ref");   
    if ((topLevel) || (attr == NULL)) {
	if (nameAttr == NULL) {
	    xmlSchemaPMissingAttrErr(ctxt,
		XML_SCHEMAP_S4S_ATTR_MISSING,
		NULL, NULL, node, "name", NULL);
	    return (NULL);
	}		
    } else
	isRef = 1;

    xmlSchemaPValAttrID(ctxt, NULL, NULL, node, BAD_CAST "id");    
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
	annot = xmlSchemaParseAnnotation(ctxt, schema, child);
	child = child->next;
    }
    /*
    * Skip particle part if a global declaration.
    */
    if (topLevel)
	goto declaration_part;
    /*
    * The particle part ==================================================
    */    
    min = xmlGetMinOccurs(ctxt, node, 0, -1, 1, "nonNegativeInteger");
    max = xmlGetMaxOccurs(ctxt, node, 0, UNBOUNDED, 1, "(nonNegativeInteger | unbounded)");
    xmlSchemaPCheckParticleCorrect_2(ctxt, NULL, node, min, max);        
    particle = xmlSchemaAddParticle(ctxt, schema, node, min, max);
    if (particle == NULL)
	goto return_null;
	
    /* ret->flags |= XML_SCHEMAS_ELEM_REF; */

    if (isRef) {	
	const xmlChar *refNs = NULL, *ref = NULL;
	xmlSchemaQNameRefPtr refer = NULL;
	/*
	* The reference part =============================================
	*/
	xmlSchemaPValAttrNodeQName(ctxt, schema,
	    NULL, NULL, attr, &refNs, NULL, &ref);
	xmlSchemaCheckReference(ctxt, schema, node, NULL, refNs);
	/* 
	* SPEC (3.3.3 : 2.1) "One of ref or name must be present, but not both" 
	*/
	if (nameAttr != NULL) {
	    xmlSchemaPMutualExclAttrErr(ctxt, 
		XML_SCHEMAP_SRC_ELEMENT_2_1,
		NULL, NULL, nameAttr, "ref", "name");
	}
	/* 
	* Check for illegal attributes.
	*/	
	attr = node->properties;
	while (attr != NULL) {
	    if (attr->ns == NULL) {
		if (xmlStrEqual(attr->name, BAD_CAST "ref") ||
		    xmlStrEqual(attr->name, BAD_CAST "name") ||
		    xmlStrEqual(attr->name, BAD_CAST "id") ||
		    xmlStrEqual(attr->name, BAD_CAST "maxOccurs") ||
		    xmlStrEqual(attr->name, BAD_CAST "minOccurs"))
		{
		    attr = attr->next;
		    continue;
		} else {
		    /* SPEC (3.3.3 : 2.2) */
		    xmlSchemaPCustomAttrErr(ctxt, 
			XML_SCHEMAP_SRC_ELEMENT_2_2,
			NULL, NULL, attr,
			"Only the attributes 'minOccurs', 'maxOccurs' and "
			"'id' are allowed in addition to 'ref'");
		    break;
		}
	    } else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
		xmlSchemaPIllegalAttrErr(ctxt,
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED,
		    NULL, NULL, attr);
	    }
	    attr = attr->next;
	}
	/*
	* No children except <annotation> expected.
	*/
	if (child != NULL) {
	    xmlSchemaPContentErr(ctxt, XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
		NULL, NULL, node, child, NULL, "(annotation?)");
	}
	if ((min == 0) && (max == 0))
	    goto return_null;
	/*
	* Create the reference item.
	*/
	refer = xmlSchemaNewQNameRef(schema, XML_SCHEMA_TYPE_ELEMENT,
	    ref, refNs);
	if (refer == NULL)
	    goto return_null;
	particle->children = (xmlSchemaTreeItemPtr) refer;
	particle->annot = annot;
	/*
	* Add to assembled items; the reference need to be resolved.
	*/
	if (ctxt->assemble != NULL)	
	    xmlSchemaAddAssembledItem(ctxt, (xmlSchemaTypePtr) particle);

	return ((xmlSchemaBasicItemPtr) particle);
    }
    /*
    * The declaration part ===============================================
    */
declaration_part:
    {
	const xmlChar *ns = NULL, *fixed, *name, *oldcontainer, *attrValue;
	xmlSchemaIDCPtr curIDC = NULL, lastIDC = NULL;

	if (xmlSchemaPValAttrNode(ctxt, NULL, NULL, nameAttr,
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_NCNAME), &name) != 0)
	    goto return_null;
	/* 
	* Evaluate the target namespace.
	*/
	if (topLevel) {
	    ns = schema->targetNamespace;
	} else {
	    attr = xmlSchemaGetPropNode(node, "form");
	    if (attr != NULL) {
		attrValue = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);
		if (xmlStrEqual(attrValue, BAD_CAST "qualified")) {
		    ns = schema->targetNamespace;
		} else if (!xmlStrEqual(attrValue, BAD_CAST "unqualified")) {
		    xmlSchemaPSimpleTypeErr(ctxt, 
			XML_SCHEMAP_S4S_ATTR_INVALID_VALUE, 
			NULL, NULL, (xmlNodePtr) attr,
			NULL, "(qualified | unqualified)", 
			attrValue, NULL, NULL, NULL);			
		}
	    } else if (schema->flags & XML_SCHEMAS_QUALIF_ELEM)
		ns = schema->targetNamespace;		
	}	
	decl = xmlSchemaAddElement(ctxt, schema, name, ns, node, topLevel);
	if (decl == NULL) {	
	    goto return_null;
	}
	decl->type = XML_SCHEMA_TYPE_ELEMENT;
	decl->node = node;
	decl->targetNamespace = ns;
	/* 
	* Check for illegal attributes.
	*/
	attr = node->properties;
	while (attr != NULL) {
	    if (attr->ns == NULL) {
		if ((!xmlStrEqual(attr->name, BAD_CAST "name")) &&
		    (!xmlStrEqual(attr->name, BAD_CAST "type")) &&
		    (!xmlStrEqual(attr->name, BAD_CAST "id")) &&		
		    (!xmlStrEqual(attr->name, BAD_CAST "default")) &&
		    (!xmlStrEqual(attr->name, BAD_CAST "fixed")) &&		
		    (!xmlStrEqual(attr->name, BAD_CAST "block")) &&
		    (!xmlStrEqual(attr->name, BAD_CAST "nillable"))) 
		{	
		    if (topLevel == 0) { 						
			if ((!xmlStrEqual(attr->name, BAD_CAST "maxOccurs")) &&
			    (!xmlStrEqual(attr->name, BAD_CAST "minOccurs")) &&
			    (!xmlStrEqual(attr->name, BAD_CAST "form"))) 
			{
			    if (xmlStrEqual(attr->name, BAD_CAST "substitutionGroup")) {
				/*
				* 3.3.6 : 3 If there is a non-�absent� {substitution 
				* group affiliation}, then {scope} must be global.
				* TODO: This one is redundant, since the S4S does 
				* prohibit this attribute on local declarations already; 
				* so why an explicit error code? Weird spec.
				* TODO: Think about hanling this equal to the other attributes.
				*/
				xmlSchemaPIllegalAttrErr(ctxt,
				    XML_SCHEMAP_E_PROPS_CORRECT_3,
				    NULL, (xmlSchemaTypePtr) decl, attr);
			    } else {
				xmlSchemaPIllegalAttrErr(ctxt,
				    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED,
				    NULL, (xmlSchemaTypePtr) decl, attr);
			    }
			}
		    } else if ((!xmlStrEqual(attr->name, BAD_CAST "final")) && 
			(!xmlStrEqual(attr->name, BAD_CAST "abstract")) && 
			(!xmlStrEqual(attr->name, BAD_CAST "substitutionGroup"))) {

			xmlSchemaPIllegalAttrErr(ctxt,
			    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED,
			    NULL, (xmlSchemaTypePtr) decl, attr);		    
		    }
		}
	    } else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
		
		xmlSchemaPIllegalAttrErr(ctxt,
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED,
		    NULL, (xmlSchemaTypePtr) decl, attr);
	    }
	    attr = attr->next;
	}		
	/*
	* Extract/validate attributes.
	*/
	if (topLevel) {
	    /* 
	    * Process top attributes of global element declarations here.
	    */
	    decl->flags |= XML_SCHEMAS_ELEM_GLOBAL;
	    decl->flags |= XML_SCHEMAS_ELEM_TOPLEVEL;
	    xmlSchemaPValAttrQName(ctxt, schema, NULL, 
		(xmlSchemaTypePtr) decl, node, "substitutionGroup", 
		&(decl->substGroupNs), NULL, &(decl->substGroup));
	    if (xmlGetBooleanProp(ctxt, NULL, (xmlSchemaTypePtr) decl,  
		node, "abstract", 0))
		decl->flags |= XML_SCHEMAS_ELEM_ABSTRACT; 
	    /*
	    * Attribute "final".
	    */
	    attr = xmlSchemaGetPropNode(node, "final");	    
	    if (attr == NULL) {
		decl->flags |= XML_SCHEMAS_ELEM_FINAL_ABSENT;
	    } else {
		attrValue = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);	    
		if (xmlSchemaPValAttrBlockFinal(attrValue, &(decl->flags), 
		    -1,
		    XML_SCHEMAS_ELEM_FINAL_EXTENSION,
		    XML_SCHEMAS_ELEM_FINAL_RESTRICTION, -1, -1, -1) != 0) {
		    xmlSchemaPSimpleTypeErr(ctxt, 
			XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
			NULL, (xmlSchemaTypePtr) decl, (xmlNodePtr) attr, 
			NULL, "(#all | List of (extension | restriction))", 
			attrValue, NULL, NULL, NULL);
		}
	    }
	}    
	/*
	* Attribute "block".
	*/
	attr = xmlSchemaGetPropNode(node, "block");	
	if (attr == NULL) {
	    decl->flags |= XML_SCHEMAS_ELEM_BLOCK_ABSENT;
	} else {
	    attrValue = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);	    
	    if (xmlSchemaPValAttrBlockFinal(attrValue, &(decl->flags), 
		-1,
		XML_SCHEMAS_ELEM_BLOCK_EXTENSION,
		XML_SCHEMAS_ELEM_BLOCK_RESTRICTION, 
		XML_SCHEMAS_ELEM_BLOCK_SUBSTITUTION, -1, -1) != 0) {
		xmlSchemaPSimpleTypeErr(ctxt,
		    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
		    NULL, (xmlSchemaTypePtr) decl, (xmlNodePtr) attr,
		    NULL, "(#all | List of (extension | "
		    "restriction | substitution))", attrValue, 
		    NULL, NULL, NULL);		
	    }
	}
	if (xmlGetBooleanProp(ctxt, NULL, (xmlSchemaTypePtr) decl, 
	    node, "nillable", 0))
	    decl->flags |= XML_SCHEMAS_ELEM_NILLABLE;	

	xmlSchemaPValAttrQName(ctxt, schema, 
	    NULL, (xmlSchemaTypePtr) decl, node, 
	    "type", &(decl->namedTypeNs), NULL, &(decl->namedType));

	decl->value = xmlSchemaGetProp(ctxt, node, "default");    
	attr = xmlSchemaGetPropNode(node, "fixed");	
	if (attr != NULL) {
	    fixed = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);
	    if (decl->value != NULL) {
		/* 
		* 3.3.3 : 1 
		* default and fixed must not both be present. 
		*/
		xmlSchemaPMutualExclAttrErr(ctxt,
		    XML_SCHEMAP_SRC_ELEMENT_1,
		    NULL, (xmlSchemaTypePtr) decl, attr,
		    "default", "fixed");
	    } else {
		decl->flags |= XML_SCHEMAS_ELEM_FIXED;
		decl->value = fixed;
	    }
	}
	/*
	* And now for the children...
	*/
	oldcontainer = ctxt->container;
	ctxt->container = decl->name;
	if (IS_SCHEMA(child, "complexType")) {
	    /* 
	    * 3.3.3 : 3 
	    * "type" and either <simpleType> or <complexType> are mutually
	    * exclusive 
	    */
	    if (decl->namedType != NULL) {
		xmlSchemaPContentErr(ctxt,
		    XML_SCHEMAP_SRC_ELEMENT_3,
		    NULL, (xmlSchemaTypePtr) decl, node, child, 
		    "The attribute 'type' and the <complexType> child are "
		    "mutually exclusive", NULL);		
	    } else
		decl->subtypes = xmlSchemaParseComplexType(ctxt, schema, child, 0);
	    child = child->next;
	} else if (IS_SCHEMA(child, "simpleType")) {
	    /* 
	    * 3.3.3 : 3 
	    * "type" and either <simpleType> or <complexType> are
	    * mutually exclusive 
	    */
	    if (decl->namedType != NULL) {
		xmlSchemaPContentErr(ctxt,
		    XML_SCHEMAP_SRC_ELEMENT_3,
		    NULL, (xmlSchemaTypePtr) decl, node, child, 
		    "The attribute 'type' and the <simpleType> child are "
		    "mutually exclusive", NULL);				
	    } else
		decl->subtypes = xmlSchemaParseSimpleType(ctxt, schema, child, 0);
	    child = child->next;
	}	
	while ((IS_SCHEMA(child, "unique")) ||
	    (IS_SCHEMA(child, "key")) || (IS_SCHEMA(child, "keyref"))) {
	    if (IS_SCHEMA(child, "unique")) {
		curIDC = xmlSchemaParseIDC(ctxt, schema, child, 
		    XML_SCHEMA_TYPE_IDC_UNIQUE, decl->targetNamespace);
	    } else if (IS_SCHEMA(child, "key")) {
		curIDC = xmlSchemaParseIDC(ctxt, schema, child,
		    XML_SCHEMA_TYPE_IDC_KEY, decl->targetNamespace);
	    } else if (IS_SCHEMA(child, "keyref")) {
		curIDC = xmlSchemaParseIDC(ctxt, schema, child,
		    XML_SCHEMA_TYPE_IDC_KEYREF, decl->targetNamespace);
	    }
	    if (lastIDC != NULL)
		lastIDC->next = curIDC;
	    else
		decl->idcs = (void *) curIDC;
	    lastIDC = curIDC;
	    child = child->next;
	}
	if (child != NULL) {
	    xmlSchemaPContentErr(ctxt,
		XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
		NULL, (xmlSchemaTypePtr) decl, node, child, 
		NULL, "(annotation?, ((simpleType | complexType)?, "
		"(unique | key | keyref)*))");
	}   
	ctxt->container = oldcontainer;
	decl->annot = annot;
    }
    /*
    * NOTE: Element Declaration Representation OK 4. will be checked at a 
    * different layer.
    */
    FREE_AND_NULL(des)
    if (topLevel)
	return ((xmlSchemaBasicItemPtr) decl);
    else {
	particle->children = (xmlSchemaTreeItemPtr) decl;
	return ((xmlSchemaBasicItemPtr) particle);
    }

return_null:
    FREE_AND_NULL(des)
    if (annot != NULL)
	xmlSchemaFreeAnnot(annot);
    if (particle != NULL)
	xmlSchemaFreeParticle(particle);
    if (decl != NULL)
	xmlSchemaFreeElement(decl);
    return (NULL);
}

/**
 * xmlSchemaParseUnion:
 * @ctxt:  a schema validation context
 * @schema:  the schema being built
 * @node:  a subtree containing XML Schema informations
 *
 * parse a XML schema Union definition
 * *WARNING* this interface is highly subject to change
 *
 * Returns -1 in case of internal error, 0 in case of success and a positive
 * error code otherwise.
 */
static int
xmlSchemaParseUnion(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                    xmlNodePtr node)
{
    xmlSchemaTypePtr type;
    xmlNodePtr child = NULL;
    xmlAttrPtr attr;
    const xmlChar *cur = NULL;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (-1);
    /* Not a component, don't create it. */
    type = ctxt->ctxtType;
    /*
    * Mark the simple type as being of variety "union".
    */
    type->flags |= XML_SCHEMAS_TYPE_VARIETY_UNION;
    /*
    * Check for illegal attributes.
    */
    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "memberTypes"))) {
		xmlSchemaPIllegalAttrErr(ctxt, 
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		    NULL, NULL, attr);		    
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt, 
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		NULL, NULL, attr);		
	}
	attr = attr->next;
    }	
    xmlSchemaPValAttrID(ctxt, NULL, NULL, node, BAD_CAST "id");
    /*
    * Attribute "memberTypes". This is a list of QNames.
    * TODO: Check the value to contain anything.
    */
    attr = xmlSchemaGetPropNode(node, "memberTypes");    
    if (attr != NULL) {
	const xmlChar *end;
	xmlChar *tmp;
	const xmlChar *localName, *nsName;
	xmlSchemaTypeLinkPtr link, lastLink = NULL;
	xmlSchemaQNameRefPtr ref;

	cur = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);
	type->ref = cur;	
	do {
	    while (IS_BLANK_CH(*cur))
		cur++;
	    end = cur;
	    while ((*end != 0) && (!(IS_BLANK_CH(*end))))
		end++;
	    if (end == cur)
		break;
	    tmp = xmlStrndup(cur, end - cur);
	    if (xmlSchemaPValAttrNodeQNameValue(ctxt, schema, NULL, 
		NULL, attr, BAD_CAST tmp, &nsName, NULL, &localName) == 0) { 
		/*
		* Create the member type link.
		*/
		link = (xmlSchemaTypeLinkPtr)
		    xmlMalloc(sizeof(xmlSchemaTypeLink));
		if (link == NULL) {
		    xmlSchemaPErrMemory(ctxt, "xmlSchemaParseUnion, "
			"allocating a type link", NULL);
		    return (-1);
		}
		link->type = NULL;
		link->next = NULL;
		if (lastLink == NULL)
		    type->memberTypes = link;
		else 
		    lastLink->next = link;
		lastLink = link;
		/*
		* Create a reference item.
		*/
		ref = xmlSchemaNewQNameRef(schema, XML_SCHEMA_TYPE_SIMPLE,
		    localName, nsName);
		if (ref == NULL) {
		    FREE_AND_NULL(tmp)
		    return (-1);
		}
		/*
		* Assign the reference to the link, it will be resolved
		* later during fixup of the union simple type.
		*/
		link->type = (xmlSchemaTypePtr) ref;
	    }
	    FREE_AND_NULL(tmp)
	    cur = end;
	} while (*cur != 0); 

    }    
    /*
    * And now for the children...
    */
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
	/*
	* Add the annotation to the simple type ancestor.
	*/
	xmlSchemaAddAnnotation((xmlSchemaAnnotItemPtr) type,
	    xmlSchemaParseAnnotation(ctxt, schema, child));
        child = child->next;
    }
    if (IS_SCHEMA(child, "simpleType")) {
	xmlSchemaTypePtr subtype, last = NULL;

	/*
	* Anchor the member types in the "subtypes" field of the
	* simple type.
	*/
	while (IS_SCHEMA(child, "simpleType")) {	
	    subtype = (xmlSchemaTypePtr)
		xmlSchemaParseSimpleType(ctxt, schema, child, 0);
	    if (subtype != NULL) {
		if (last == NULL) {
		    type->subtypes = subtype;
		    last = subtype;
		} else {
		    last->next = subtype;
		    last = subtype;
		}
		last->next = NULL;
	    }
	    child = child->next;
	}
    }
    if (child != NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED, 
	    NULL, NULL, node, child, NULL, "(annotation?, simpleType*)");
    }   
    if ((attr == NULL) && (type->subtypes == NULL)) {
	 /*
	* src-union-memberTypes-or-simpleTypes
	* Either the memberTypes [attribute] of the <union> element must 
	* be non-empty or there must be at least one simpleType [child]. 
	*/
	xmlSchemaPCustomErr(ctxt, 
	    XML_SCHEMAP_SRC_UNION_MEMBERTYPES_OR_SIMPLETYPES,
	    NULL, NULL, node,
	    "Either the attribute 'memberTypes' or "
	    "at least one <simpleType> child must be present", NULL);
    } 
    return (0);
}

/**
 * xmlSchemaParseList:
 * @ctxt:  a schema validation context
 * @schema:  the schema being built
 * @node:  a subtree containing XML Schema informations
 *
 * parse a XML schema List definition
 * *WARNING* this interface is highly subject to change
 *
 * Returns -1 in case of error, 0 if the declaration is improper and
 *         1 in case of success.
 */
static xmlSchemaTypePtr
xmlSchemaParseList(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                   xmlNodePtr node)
{
    xmlSchemaTypePtr type;
    xmlNodePtr child = NULL;
    xmlAttrPtr attr;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);
    /* Not a component, don't create it. */
    type = ctxt->ctxtType;
    /*
    * Mark the type as being of variety "list".
    */
    type->flags |= XML_SCHEMAS_TYPE_VARIETY_LIST;
    /*
    * Check for illegal attributes.
    */
    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "itemType"))) {
		xmlSchemaPIllegalAttrErr(ctxt, 
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		    NULL, NULL, attr);		    
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt, 
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		NULL, NULL, attr);		
	}
	attr = attr->next;
    }	

    xmlSchemaPValAttrID(ctxt, NULL, NULL, node, BAD_CAST "id");

    /*
    * Attribute "itemType". NOTE that we will use the "ref" and "refNs"
    * fields for holding the reference to the itemType.
    */
    xmlSchemaPValAttrQName(ctxt, schema, NULL, NULL,
	node, "itemType", &(type->refNs), NULL, &(type->ref));
    /*
    * And now for the children...
    */
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
	xmlSchemaAddAnnotation((xmlSchemaAnnotItemPtr) type,
	    xmlSchemaParseAnnotation(ctxt, schema, child));
        child = child->next;
    }    	
    if (IS_SCHEMA(child, "simpleType")) {
	/*
	* src-list-itemType-or-simpleType
	* Either the itemType [attribute] or the <simpleType> [child] of 
	* the <list> element must be present, but not both. 
	*/
	if (type->ref != NULL) {
	    xmlSchemaPCustomErr(ctxt, 
		XML_SCHEMAP_SRC_SIMPLE_TYPE_1,
		NULL, NULL, node, 
		"The attribute 'itemType' and the <simpleType> child "
		"are mutually exclusive", NULL);
	} else {	
	    type->subtypes = xmlSchemaParseSimpleType(ctxt, schema, child, 0);
	}
        child = child->next;        
    } else if (type->ref == NULL) {
	xmlSchemaPCustomErr(ctxt, 
	    XML_SCHEMAP_SRC_SIMPLE_TYPE_1,
	    NULL, NULL, node, 
	    "Either the attribute 'itemType' or the <simpleType> child "
	    "must be present", NULL);
    }
    if (child != NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED, 
	    NULL, NULL, node, child, NULL, "(annotation?, simpleType?)");
    }
    if ((type->ref == NULL) &&
	(type->subtypes == NULL) &&
	(xmlSchemaGetPropNode(node, "itemType") == NULL)) {
	xmlSchemaPCustomErr(ctxt, 
	    XML_SCHEMAP_SRC_SIMPLE_TYPE_1,
	    NULL, NULL, node, 
	    "Either the attribute 'itemType' or the <simpleType> child "
	    "must be present", NULL);
    }
    return (NULL);
}

/**
 * xmlSchemaParseSimpleType:
 * @ctxt:  a schema validation context
 * @schema:  the schema being built
 * @node:  a subtree containing XML Schema informations
 *
 * parse a XML schema Simple Type definition
 * *WARNING* this interface is highly subject to change
 *
 * Returns -1 in case of error, 0 if the declaration is improper and
 * 1 in case of success.
 */
static xmlSchemaTypePtr
xmlSchemaParseSimpleType(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                         xmlNodePtr node, int topLevel)
{
    xmlSchemaTypePtr type, oldCtxtType, oldParentItem;
    xmlNodePtr child = NULL;
    const xmlChar *attrValue = NULL;
    xmlAttrPtr attr;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);
   
    if (topLevel) {
	attr = xmlSchemaGetPropNode(node, "name");
	if (attr == NULL) {
	    xmlSchemaPMissingAttrErr(ctxt, 
		XML_SCHEMAP_S4S_ATTR_MISSING, 
		NULL, NULL, node,
		"name", NULL);
	    return (NULL);
	} else if (xmlSchemaPValAttrNode(ctxt, 
	    NULL, NULL, attr, 
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_NCNAME), &attrValue) != 0) {
	    return (NULL);
	}
    }
            
    if (topLevel == 0) {
        char buf[40];

	/*
	* Parse as local simple type definition.
	*/
        snprintf(buf, 39, "#ST%d", ctxt->counter++ + 1);
	type = xmlSchemaAddType(ctxt, schema, (const xmlChar *)buf, NULL, node);
	if (type == NULL)
	    return (NULL);
	type->node = node;
	type->type = XML_SCHEMA_TYPE_SIMPLE;
	/*
	* Check for illegal attributes.
	*/
	attr = node->properties;
	while (attr != NULL) {
	    if (attr->ns == NULL) {
		if (!xmlStrEqual(attr->name, BAD_CAST "id")) {
		    xmlSchemaPIllegalAttrErr(ctxt, 
			XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
			NULL, type, attr);		    
		}
	    } else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
		    xmlSchemaPIllegalAttrErr(ctxt, 
			XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
			NULL, type, attr);	
	    }
	    attr = attr->next;
	}
    } else {		
	/*
	* Parse as global simple type definition.
	*
	* Note that attrValue is the value of the attribute "name" here.
	*/	
	type = xmlSchemaAddType(ctxt, schema, attrValue, schema->targetNamespace, node);
	if (type == NULL)
	    return (NULL);
	type->node = node;
	type->type = XML_SCHEMA_TYPE_SIMPLE;
	type->flags |= XML_SCHEMAS_TYPE_GLOBAL;
	/*
	* Check for illegal attributes.
	*/
	attr = node->properties;
	while (attr != NULL) {
	    if (attr->ns == NULL) {
		if ((!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		    (!xmlStrEqual(attr->name, BAD_CAST "name")) &&
		    (!xmlStrEqual(attr->name, BAD_CAST "final"))) {
		    xmlSchemaPIllegalAttrErr(ctxt, 
			XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
			NULL, type, attr);	
		}
	    } else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
		xmlSchemaPIllegalAttrErr(ctxt, 
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		    NULL, type, attr);	
	    }
	    attr = attr->next;
	}
	/*
	* Attribute "final".
	*/
	attr = xmlSchemaGetPropNode(node, "final");	
	if (attr == NULL) {
	    type->flags |= XML_SCHEMAS_TYPE_FINAL_DEFAULT;
	} else {
	    attrValue = xmlSchemaGetProp(ctxt, node, "final");
	    if (xmlSchemaPValAttrBlockFinal(attrValue, &(type->flags), 
		-1, -1, XML_SCHEMAS_TYPE_FINAL_RESTRICTION, -1,	    
		XML_SCHEMAS_TYPE_FINAL_LIST,
		XML_SCHEMAS_TYPE_FINAL_UNION) != 0) {

		xmlSchemaPSimpleTypeErr(ctxt, 
		    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
		    NULL, type, (xmlNodePtr) attr, 
		    NULL, "(#all | List of (list | union | restriction)", 
		    attrValue, NULL, NULL, NULL);
	    }
	}
    }   
    type->targetNamespace = schema->targetNamespace;
    xmlSchemaPValAttrID(ctxt, NULL, type, node, BAD_CAST "id");
    /*
    * And now for the children...
    */
    oldCtxtType = ctxt->ctxtType;
    oldParentItem = ctxt->parentItem;
    ctxt->ctxtType = type;
    ctxt->parentItem = type;
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
        type->annot = xmlSchemaParseAnnotation(ctxt, schema, child);
        child = child->next;
    }
    if (child == NULL) {
	xmlSchemaPContentErr(ctxt, XML_SCHEMAP_S4S_ELEM_MISSING, 
	    NULL, type, node, child, NULL, 
	    "(annotation?, (restriction | list | union))");
    } else if (IS_SCHEMA(child, "restriction")) {
        xmlSchemaParseRestriction(ctxt, schema, child,
		XML_SCHEMA_TYPE_SIMPLE);
        child = child->next;
    } else if (IS_SCHEMA(child, "list")) {        
        xmlSchemaParseList(ctxt, schema, child);
        child = child->next;
    } else if (IS_SCHEMA(child, "union")) {        
        xmlSchemaParseUnion(ctxt, schema, child);
        child = child->next;
    }
    if (child != NULL) {
	xmlSchemaPContentErr(ctxt, XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED, 
	    NULL, type, node, child, NULL, 
	    "(annotation?, (restriction | list | union))");
    }
    ctxt->parentItem = oldParentItem;
    ctxt->ctxtType = oldCtxtType;

    return (type);
}

/**
 * xmlSchemaParseModelGroupDefRef:
 * @ctxt:  a schema validation context
 * @schema:  the schema being built
 * @node:  a subtree containing XML Schema informations
 *
 * Parses a XML schema particle (reference to a model group definition).
 * *WARNING* this interface is highly subject to change
 *
 * Returns -1 in case of error, 0 if the declaration is improper and
 *         1 in case of success.
 */
static xmlSchemaTreeItemPtr
xmlSchemaParseModelGroupDefRef(xmlSchemaParserCtxtPtr ctxt,
			       xmlSchemaPtr schema,
			       xmlNodePtr node)
{
    xmlSchemaParticlePtr item;
    xmlNodePtr child = NULL;    
    xmlAttrPtr attr;
    const xmlChar *ref = NULL, *refNs = NULL;
    int min, max;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);        

    attr = xmlSchemaGetPropNode(node, "ref");
    if (attr == NULL) {
	xmlSchemaPMissingAttrErr(ctxt, 
	    XML_SCHEMAP_S4S_ATTR_MISSING, 
	    NULL, NULL, node,
	    "ref", NULL);
	return (NULL);
    } else if (xmlSchemaPValAttrNodeQName(ctxt, schema, NULL, NULL,
	attr, &refNs, NULL, &ref) != 0) {
	return (NULL);
    }    
    min = xmlGetMinOccurs(ctxt, node, 0, -1, 1, "nonNegativeInteger");
    max = xmlGetMaxOccurs(ctxt, node, 0, UNBOUNDED, 1,
	"(nonNegativeInteger | unbounded)");
    /*
    * Check for illegal attributes.
    */
    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "ref")) && 
		(!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "minOccurs")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "maxOccurs"))) {
		xmlSchemaPIllegalAttrErr(ctxt, 
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		    NULL, NULL, attr);		    
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt, 
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		NULL, NULL, attr);	
	}
	attr = attr->next;
    }
    xmlSchemaPValAttrID(ctxt, NULL, NULL, node, BAD_CAST "id");    
    item = xmlSchemaAddParticle(ctxt, schema, node, min, max);
    if (item == NULL)
	return (NULL);    
    /*
    * Create a reference item as the term; it will be substituted for
    * the model group after the reference has been resolved.
    */    
    item->children = (xmlSchemaTreeItemPtr) 
	xmlSchemaNewQNameRef(schema, XML_SCHEMA_TYPE_GROUP, ref, refNs);
    xmlSchemaCheckReference(ctxt, schema, node, (xmlSchemaBasicItemPtr) item, refNs);
    xmlSchemaPCheckParticleCorrect_2(ctxt, item, node, min, max);
    /*
    * And now for the children...
    */
    child = node->children;
    /* TODO: Is annotation even allowed for a model group reference? */
    if (IS_SCHEMA(child, "annotation")) {
	/*
	* TODO: What to do exactly with the annotation? 
	*/
	item->annot = xmlSchemaParseAnnotation(ctxt, schema, child);
	child = child->next;
    }
    if (child != NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED, 
	    NULL, NULL, node, child, NULL,
	    "(annotation?)");
    }
    /*
    * Corresponds to no component at all if minOccurs==maxOccurs==0.
    */
    if ((min == 0) && (max == 0))
	return (NULL);
    if (ctxt->assemble != NULL)	
	xmlSchemaAddAssembledItem(ctxt, (xmlSchemaTypePtr) item);
    return ((xmlSchemaTreeItemPtr) item);
}

/**
 * xmlSchemaParseModelGroupDefinition:
 * @ctxt:  a schema validation context
 * @schema:  the schema being built
 * @node:  a subtree containing XML Schema informations
 *
 * Parses a XML schema model group definition.
 * *WARNING* this interface is highly subject to change
 *
 * Returns -1 in case of error, 0 if the declaration is improper and
 *         1 in case of success.
 */
static xmlSchemaModelGroupDefPtr
xmlSchemaParseModelGroupDefinition(xmlSchemaParserCtxtPtr ctxt,
				   xmlSchemaPtr schema,
				   xmlNodePtr node)
{
    xmlSchemaModelGroupDefPtr item;
    xmlNodePtr child = NULL;    
    xmlAttrPtr attr;
    const xmlChar *name;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);    
 
    attr = xmlSchemaGetPropNode(node, "name");
    if (attr == NULL) {
	xmlSchemaPMissingAttrErr(ctxt, 
	    XML_SCHEMAP_S4S_ATTR_MISSING, 
	    NULL, NULL, node,
	    "name", NULL);
	return (NULL);
    } else if (xmlSchemaPValAttrNode(ctxt, 
	NULL, NULL, attr, 
	xmlSchemaGetBuiltInType(XML_SCHEMAS_NCNAME), &name) != 0) {
	return (NULL);
    }
    item = xmlSchemaAddGroup(ctxt, schema, name, schema->targetNamespace, node);
    if (item == NULL)
	return (NULL);    
    /*
    * Check for illegal attributes.
    */
    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "name")) && 
		(!xmlStrEqual(attr->name, BAD_CAST "id"))) {
		xmlSchemaPIllegalAttrErr(ctxt, 
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		    NULL, NULL, attr);		    
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt, 
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		NULL, NULL, attr);	
	}
	attr = attr->next;
    }
    xmlSchemaPValAttrID(ctxt, NULL, NULL, node, BAD_CAST "id");
    /*
    * And now for the children...
    */    
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
	item->annot = xmlSchemaParseAnnotation(ctxt, schema, child);
	child = child->next;
    }
    if (IS_SCHEMA(child, "all")) {
	item->children = xmlSchemaParseModelGroup(ctxt, schema, child,
	    XML_SCHEMA_TYPE_ALL, 0);
	child = child->next;
    } else if (IS_SCHEMA(child, "choice")) {
	item->children = xmlSchemaParseModelGroup(ctxt, schema, child,
	    XML_SCHEMA_TYPE_CHOICE, 0);
	child = child->next;
    } else if (IS_SCHEMA(child, "sequence")) {
	item->children = xmlSchemaParseModelGroup(ctxt, schema, child,
	    XML_SCHEMA_TYPE_SEQUENCE, 0);
	child = child->next;
    }
    if (child != NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED, 
	    NULL, NULL, node, child, NULL, 
	    "(annotation?, (all | choice | sequence)?)");
    }
    
    return (item);
}

/**
 * xmlSchemaCleanupDoc:
 * @ctxt:  a schema validation context
 * @node:  the root of the document.
 *
 * removes unwanted nodes in a schemas document tree
 */
static void
xmlSchemaCleanupDoc(xmlSchemaParserCtxtPtr ctxt, xmlNodePtr root)
{
    xmlNodePtr delete, cur;

    if ((ctxt == NULL) || (root == NULL)) return;

    /*
     * Remove all the blank text nodes
     */
    delete = NULL;
    cur = root;
    while (cur != NULL) {
        if (delete != NULL) {
            xmlUnlinkNode(delete);
            xmlFreeNode(delete);
            delete = NULL;
        }
        if (cur->type == XML_TEXT_NODE) {
            if (IS_BLANK_NODE(cur)) {
                if (xmlNodeGetSpacePreserve(cur) != 1) {
                    delete = cur;
                }
            }
        } else if ((cur->type != XML_ELEMENT_NODE) &&
                   (cur->type != XML_CDATA_SECTION_NODE)) {
            delete = cur;
            goto skip_children;
        }

        /*
         * Skip to next node
         */
        if (cur->children != NULL) {
            if ((cur->children->type != XML_ENTITY_DECL) &&
                (cur->children->type != XML_ENTITY_REF_NODE) &&
                (cur->children->type != XML_ENTITY_NODE)) {
                cur = cur->children;
                continue;
            }
        }
      skip_children:
        if (cur->next != NULL) {
            cur = cur->next;
            continue;
        }

        do {
            cur = cur->parent;
            if (cur == NULL)
                break;
            if (cur == root) {
                cur = NULL;
                break;
            }
            if (cur->next != NULL) {
                cur = cur->next;
                break;
            }
        } while (cur != NULL);
    }
    if (delete != NULL) {
        xmlUnlinkNode(delete);
        xmlFreeNode(delete);
        delete = NULL;
    }
}


/**
 * xmlSchemaImportSchema
 * 
 * @ctxt:  a schema validation context
 * @schemaLocation:  an URI defining where to find the imported schema
 *
 * import a XML schema
 * *WARNING* this interface is highly subject to change
 *
 * Returns -1 in case of error and 1 in case of success.
 */
#if 0
static xmlSchemaImportPtr
xmlSchemaImportSchema(xmlSchemaParserCtxtPtr ctxt,
                      const xmlChar *schemaLocation)
{
    xmlSchemaImportPtr import;
    xmlSchemaParserCtxtPtr newctxt;

    newctxt = (xmlSchemaParserCtxtPtr) xmlMalloc(sizeof(xmlSchemaParserCtxt));
    if (newctxt == NULL) {
        xmlSchemaPErrMemory(ctxt, "allocating schema parser context",
                            NULL);
        return (NULL);
    }
    memset(newctxt, 0, sizeof(xmlSchemaParserCtxt));
    /* Keep the same dictionnary for parsing, really */
    xmlDictReference(ctxt->dict);
    newctxt->dict = ctxt->dict;
    newctxt->includes = 0;
    newctxt->URL = xmlDictLookup(newctxt->dict, schemaLocation, -1);

    xmlSchemaSetParserErrors(newctxt, ctxt->error, ctxt->warning,
	                     ctxt->userData);

    import = (xmlSchemaImport*) xmlMalloc(sizeof(xmlSchemaImport));
    if (import == NULL) {
        xmlSchemaPErrMemory(NULL, "allocating imported schema",
                            NULL);
	xmlSchemaFreeParserCtxt(newctxt);
        return (NULL);
    }

    memset(import, 0, sizeof(xmlSchemaImport));
    import->schemaLocation = xmlDictLookup(ctxt->dict, schemaLocation, -1);
    import->schema = xmlSchemaParse(newctxt);

    if (import->schema == NULL) {
        /* FIXME use another error enum here ? */
        xmlSchemaPErr(ctxt, NULL, XML_SCHEMAP_INTERNAL,
	              "Failed to import schema from location \"%s\".\n",
		      schemaLocation, NULL);

	xmlSchemaFreeParserCtxt(newctxt);
	/* The schemaLocation is held by the dictionary.
	if (import->schemaLocation != NULL)
	    xmlFree((xmlChar *)import->schemaLocation);
	*/
	xmlFree(import);
	return NULL;
    }

    xmlSchemaFreeParserCtxt(newctxt);
    return import;
}
#endif

static void
xmlSchemaClearSchemaDefaults(xmlSchemaPtr schema)
{
    if (schema->flags & XML_SCHEMAS_QUALIF_ELEM)
	schema->flags ^= XML_SCHEMAS_QUALIF_ELEM;

    if (schema->flags & XML_SCHEMAS_QUALIF_ATTR)
	schema->flags ^= XML_SCHEMAS_QUALIF_ATTR;

    if (schema->flags & XML_SCHEMAS_FINAL_DEFAULT_EXTENSION)
	schema->flags ^= XML_SCHEMAS_FINAL_DEFAULT_EXTENSION;
    if (schema->flags & XML_SCHEMAS_FINAL_DEFAULT_RESTRICTION)
	schema->flags ^= XML_SCHEMAS_FINAL_DEFAULT_RESTRICTION;
    if (schema->flags & XML_SCHEMAS_FINAL_DEFAULT_LIST)
	schema->flags ^= XML_SCHEMAS_FINAL_DEFAULT_LIST;
    if (schema->flags & XML_SCHEMAS_FINAL_DEFAULT_UNION)
	schema->flags ^= XML_SCHEMAS_FINAL_DEFAULT_UNION;

    if (schema->flags & XML_SCHEMAS_BLOCK_DEFAULT_EXTENSION)
	schema->flags ^= XML_SCHEMAS_BLOCK_DEFAULT_EXTENSION;
    if (schema->flags & XML_SCHEMAS_BLOCK_DEFAULT_RESTRICTION)
	schema->flags ^= XML_SCHEMAS_BLOCK_DEFAULT_RESTRICTION;
    if (schema->flags & XML_SCHEMAS_BLOCK_DEFAULT_SUBSTITUTION)
	schema->flags ^= XML_SCHEMAS_BLOCK_DEFAULT_SUBSTITUTION;
}

static void
xmlSchemaParseSchemaDefaults(xmlSchemaParserCtxtPtr ctxt, 
			     xmlSchemaPtr schema,
			     xmlNodePtr node)
{
    xmlAttrPtr attr;
    const xmlChar *val;

    xmlSchemaPValAttrID(ctxt, NULL, NULL, node, BAD_CAST "id");
    if (schema->version == NULL)
	xmlSchemaPValAttr(ctxt, NULL, NULL, node, "version", 
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_TOKEN), &(schema->version));
    else
	xmlSchemaPValAttr(ctxt, NULL, NULL, node, "version", 
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_TOKEN), NULL);

    attr = xmlSchemaGetPropNode(node, "elementFormDefault");     
    if (attr != NULL) {
	val = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);
	if (xmlSchemaPValAttrFormDefault(val, &schema->flags, 
	    XML_SCHEMAS_QUALIF_ELEM) != 0) {
	    xmlSchemaPSimpleTypeErr(ctxt, 
		XML_SCHEMAP_ELEMFORMDEFAULT_VALUE,
		NULL, NULL, (xmlNodePtr) attr, NULL, 
		"(qualified | unqualified)", val, NULL, NULL, NULL);
	}
    }
    
    attr = xmlSchemaGetPropNode(node, "attributeFormDefault");     
    if (attr != NULL) {
	val = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);
	if (xmlSchemaPValAttrFormDefault(val, &schema->flags, 
	    XML_SCHEMAS_QUALIF_ATTR) != 0) {
	    xmlSchemaPSimpleTypeErr(ctxt, 
		XML_SCHEMAP_ATTRFORMDEFAULT_VALUE,
		NULL, NULL, (xmlNodePtr) attr, NULL, 
		"(qualified | unqualified)", val, NULL, NULL, NULL);
	}
    }
    
    attr = xmlSchemaGetPropNode(node, "finalDefault");    
    if (attr != NULL) {
	val = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);
	if (xmlSchemaPValAttrBlockFinal(val, &(schema->flags), -1,
	    XML_SCHEMAS_FINAL_DEFAULT_EXTENSION,
	    XML_SCHEMAS_FINAL_DEFAULT_RESTRICTION,
	    -1,
	    XML_SCHEMAS_FINAL_DEFAULT_LIST,
	    XML_SCHEMAS_FINAL_DEFAULT_UNION) != 0) {
	    xmlSchemaPSimpleTypeErr(ctxt,
		XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
		NULL, NULL, (xmlNodePtr) attr, NULL,
		"(#all | List of (extension | restriction | list | union))",
		val, NULL, NULL, NULL);
	}	    
    }
    
    attr = xmlSchemaGetPropNode(node, "blockDefault");     
    if (attr != NULL) {
	val = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);
	if (xmlSchemaPValAttrBlockFinal(val, &(schema->flags), -1,
	    XML_SCHEMAS_BLOCK_DEFAULT_EXTENSION,
	    XML_SCHEMAS_BLOCK_DEFAULT_RESTRICTION,
	    XML_SCHEMAS_BLOCK_DEFAULT_SUBSTITUTION, -1, -1) != 0) {
	     xmlSchemaPSimpleTypeErr(ctxt,
		XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
		NULL, NULL, (xmlNodePtr) attr, NULL,
		"(#all | List of (extension | restriction | substitution))",
		val, NULL, NULL, NULL);
	}	    
    }
}

/**
 * xmlSchemaParseSchemaTopLevel:
 * @ctxt:  a schema validation context
 * @schema:  the schemas
 * @nodes:  the list of top level nodes
 *
 * Returns the internal XML Schema structure built from the resource or
 *         NULL in case of error
 */
static void
xmlSchemaParseSchemaTopLevel(xmlSchemaParserCtxtPtr ctxt,
                             xmlSchemaPtr schema, xmlNodePtr nodes)
{
    xmlNodePtr child;
    xmlSchemaAnnotPtr annot;

    if ((ctxt == NULL) || (schema == NULL) || (nodes == NULL))
        return;

    child = nodes;
    while ((IS_SCHEMA(child, "include")) ||
	   (IS_SCHEMA(child, "import")) ||
	   (IS_SCHEMA(child, "redefine")) ||
	   (IS_SCHEMA(child, "annotation"))) {
	if (IS_SCHEMA(child, "annotation")) {
	    annot = xmlSchemaParseAnnotation(ctxt, schema, child);
	    if (schema->annot == NULL)
		schema->annot = annot;
	    else
		xmlSchemaFreeAnnot(annot);
	} else if (IS_SCHEMA(child, "import")) {
	    xmlSchemaParseImport(ctxt, schema, child);
	} else if (IS_SCHEMA(child, "include")) {
	    ctxt->includes++;
	    xmlSchemaParseInclude(ctxt, schema, child);
	    ctxt->includes--;
	} else if (IS_SCHEMA(child, "redefine")) {
	    TODO
	}
	child = child->next;
    }
    while (child != NULL) {
	if (IS_SCHEMA(child, "complexType")) {
	    xmlSchemaParseComplexType(ctxt, schema, child, 1);
	    child = child->next;
	} else if (IS_SCHEMA(child, "simpleType")) {
	    xmlSchemaParseSimpleType(ctxt, schema, child, 1);
	    child = child->next;
	} else if (IS_SCHEMA(child, "element")) {
	    xmlSchemaParseElement(ctxt, schema, child, 1);
	    child = child->next;
	} else if (IS_SCHEMA(child, "attribute")) {
	    xmlSchemaParseAttribute(ctxt, schema, child, 1);
	    child = child->next;
	} else if (IS_SCHEMA(child, "attributeGroup")) {
	    xmlSchemaParseAttributeGroup(ctxt, schema, child, 1);
	    child = child->next;
	} else if (IS_SCHEMA(child, "group")) {
	    xmlSchemaParseModelGroupDefinition(ctxt, schema, child);
	    child = child->next;
	} else if (IS_SCHEMA(child, "notation")) {
	    xmlSchemaParseNotation(ctxt, schema, child);
	    child = child->next;
	} else {
	    xmlSchemaPErr2(ctxt, NULL, child,
			   XML_SCHEMAP_UNKNOWN_SCHEMAS_CHILD,
			   "Unexpected element \"%s\" as child of <schema>.\n",
			   child->name, NULL);
	    child = child->next;
	}
	while (IS_SCHEMA(child, "annotation")) {
	    annot = xmlSchemaParseAnnotation(ctxt, schema, child);
	    if (schema->annot == NULL)
		schema->annot = annot;
	    else
		xmlSchemaFreeAnnot(annot);
	    child = child->next;
	}
    }
    ctxt->parentItem = NULL;
    ctxt->ctxtType = NULL;
}

static xmlSchemaImportPtr
xmlSchemaAddImport(xmlSchemaParserCtxtPtr ctxt, 
		   xmlHashTablePtr *imports,
		   const xmlChar *nsName)
{
    xmlSchemaImportPtr ret;

    if (*imports == NULL) {
	*imports = xmlHashCreateDict(10, ctxt->dict);
	if (*imports == NULL) {
	    xmlSchemaPCustomErr(ctxt, 
		XML_SCHEMAP_FAILED_BUILD_IMPORT,
		NULL, NULL, (xmlNodePtr) ctxt->doc,
		"Internal error: failed to build the import table",
		NULL);
	    return (NULL);
	}
    }
    ret = (xmlSchemaImport*) xmlMalloc(sizeof(xmlSchemaImport));
    if (ret == NULL) {
	xmlSchemaPErrMemory(NULL, "allocating import struct", NULL);
	return (NULL);
    }   
    memset(ret, 0, sizeof(xmlSchemaImport));
    if (nsName == NULL)
	nsName = XML_SCHEMAS_NO_NAMESPACE;
    xmlHashAddEntry(*imports, nsName, ret);  

    return (ret);
}

static int
xmlSchemaAcquireSchemaDoc(xmlSchemaParserCtxtPtr ctxt,
			  xmlSchemaPtr schema,
			  xmlNodePtr node,
			  const xmlChar *nsName,
			  const xmlChar *location,
			  xmlDocPtr *doc,
			  const xmlChar **targetNamespace,
			  int absolute)
{
    xmlParserCtxtPtr parserCtxt;
    xmlSchemaImportPtr import;
    const xmlChar *ns;
    xmlNodePtr root;

    /*
    * NOTE: This will be used for <import>, <xsi:schemaLocation> and
    * <xsi:noNamespaceSchemaLocation>.
    */
    *doc = NULL;
    /*
    * Given that the schemaLocation [attribute] is only a hint, it is open 
    * to applications to ignore all but the first <import> for a given 
    * namespace, regardless of the �actual value� of schemaLocation, but
    * such a strategy risks missing useful information when new
    * schemaLocations are offered.
    *
    * XSV (ver 2.5-2) does use the first <import> which resolves to a valid schema.
    * Xerces-J (ver 2.5.1) ignores all but the first given <import> - regardless if
    * valid or not.
    * We will follow XSV here. 
    */
    if (location == NULL) {
	/*
	* Schema Document Location Strategy:
	*
	* 3 Based on the namespace name, identify an existing schema document,
	* either as a resource which is an XML document or a <schema> element 
	* information item, in some local schema repository; 
	*
	* 5 Attempt to resolve the namespace name to locate such a resource. 
	*
	* NOTE: Those stategies are not supported, so we will skip.
	*/
	return (0);
    }
    if (nsName == NULL) 
	ns = XML_SCHEMAS_NO_NAMESPACE;
    else
	ns = nsName;
    
    import = xmlHashLookup(schema->schemasImports, ns);
    if (import != NULL) {	
	/*
	* There was a valid resource for the specified namespace already
	* defined, so skip.
	* TODO: This might be changed someday to allow import of
	* components from multiple documents for a single target namespace.
	*/
	return (0);
    } 
   
    /*
    * Schema Document Location Strategy: 
    *
    * 2 Based on the location URI, identify an existing schema document, 
    * either as a resource which is an XML document or a <schema> element 
    * information item, in some local schema repository;   
    *
    * 4 Attempt to resolve the location URI, to locate a resource on the 
    * web which is or contains or references a <schema> element;
    * TODO: Hmm, I don't know if the reference stuff in 4. will work.
    *
    */
    if ((absolute == 0) && (node != NULL)) {
	xmlChar *base, *URI;

	base = xmlNodeGetBase(node->doc, node);
	if (base == NULL) {
	    URI = xmlBuildURI(location, node->doc->URL);
	} else {
	    URI = xmlBuildURI(location, base);
	    xmlFree(base);
	}
	if (URI != NULL) {
	    location = xmlDictLookup(ctxt->dict, URI, -1);
	    xmlFree(URI);
	}
    }
    parserCtxt = xmlNewParserCtxt();
    if (parserCtxt == NULL) {
	xmlSchemaPErrMemory(NULL, "xmlSchemaParseImport: "
	    "allocating a parser context", NULL);
	return(-1);
    }	   
    
    if ((ctxt->dict != NULL) && (parserCtxt->dict != NULL)) {
	xmlDictFree(parserCtxt->dict);
	parserCtxt->dict = ctxt->dict;
	xmlDictReference(parserCtxt->dict);
    }
    
    *doc = xmlCtxtReadFile(parserCtxt, (const char *) location, 
	    NULL, SCHEMAS_PARSE_OPTIONS);

    /*
    * 2.1 The referent is (a fragment of) a resource which is an 
    * XML document (see clause 1.1), which in turn corresponds to 
    * a <schema> element information item in a well-formed information 
    * set, which in turn corresponds to a valid schema.
    * TODO: What to do with the "fragment" stuff?
    *
    * 2.2 The referent is a <schema> element information item in 
    * a well-formed information set, which in turn corresponds 
    * to a valid schema.
    * NOTE: 2.2 won't apply, since only XML documents will be processed 
    * here.
    */       
    if (*doc == NULL) {	
	xmlErrorPtr lerr;
	/*
	* It is *not* an error for the application schema reference 
	* strategy to fail.
	* 
	* If the doc is NULL and the parser error is an IO error we
	* will assume that the resource could not be located or accessed.
	*
	* TODO: Try to find specific error codes to react only on
	* localisation failures.
	*
	* TODO, FIXME: Check the spec: is a namespace added to the imported
	* namespaces, even if the schemaLocation did not provide
	* a resource? I guess so, since omitting the "schemaLocation"
	* attribute, imports a namespace as well.
	*/
	lerr = xmlGetLastError();
	if ((lerr != NULL) && (lerr->domain == XML_FROM_IO)) {	
	    xmlFreeParserCtxt(parserCtxt);
	    return(0);
	}

	xmlSchemaPCustomErr(ctxt,
	    XML_SCHEMAP_SRC_IMPORT_2_1,
	    NULL, NULL, node,
	    "Failed to parse the resource '%s' for import",
	    location);
	xmlFreeParserCtxt(parserCtxt);
	return(XML_SCHEMAP_SRC_IMPORT_2_1);
    }
    xmlFreeParserCtxt(parserCtxt);
    
    root = xmlDocGetRootElement(*doc);
    if (root == NULL) {
	xmlSchemaPCustomErr(ctxt,
	    XML_SCHEMAP_SRC_IMPORT_2_1,
	    NULL, NULL, node,
	    "The XML document '%s' to be imported has no document "
	    "element", location);	
	xmlFreeDoc(*doc);
	*doc = NULL;
	return (XML_SCHEMAP_SRC_IMPORT_2_1);
    }	
    
    xmlSchemaCleanupDoc(ctxt, root);	
    
    if (!IS_SCHEMA(root, "schema")) {
	xmlSchemaPCustomErr(ctxt,
	    XML_SCHEMAP_SRC_IMPORT_2_1,
	    NULL, NULL, node,
	    "The XML document '%s' to be imported is not a XML schema document",
	    location);	
	xmlFreeDoc(*doc);
	*doc = NULL;
	return (XML_SCHEMAP_SRC_IMPORT_2_1);
    }	
    *targetNamespace = xmlSchemaGetProp(ctxt, root, "targetNamespace");
    /*
    * Schema Representation Constraint: Import Constraints and Semantics
    */    
    if (nsName == NULL) {
	if (*targetNamespace != NULL) {
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_SRC_IMPORT_3_2,
		NULL, NULL, node,
		"The XML schema to be imported is not expected "
		"to have a target namespace; this differs from "
		"its target namespace of '%s'", *targetNamespace);
	    xmlFreeDoc(*doc);
	    *doc = NULL;
	    return (XML_SCHEMAP_SRC_IMPORT_3_2);
	}
    } else {
	if (*targetNamespace == NULL) {
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_SRC_IMPORT_3_1,
		NULL, NULL, node,
		"The XML schema to be imported is expected to have a target "
		"namespace of '%s'", nsName);
	    xmlFreeDoc(*doc);
	    *doc = NULL;
	    return (XML_SCHEMAP_SRC_IMPORT_3_1);
	} else if (!xmlStrEqual(*targetNamespace, nsName)) {
	    xmlSchemaPCustomErrExt(ctxt,
		XML_SCHEMAP_SRC_IMPORT_3_1,
		NULL, NULL, node,
		"The XML schema to be imported is expected to have a "
		"target namespace of '%s'; this differs from "
		"its target namespace of '%s'", 
		nsName, *targetNamespace, NULL);
	    xmlFreeDoc(*doc);
	    *doc = NULL;
	    return (XML_SCHEMAP_SRC_IMPORT_3_1);
	}
    }

    import = xmlSchemaAddImport(ctxt, &(schema->schemasImports), nsName);
    if (import == NULL) {
	xmlSchemaPCustomErr(ctxt, XML_SCHEMAP_FAILED_BUILD_IMPORT,
	    NULL, NULL, NULL,	    
	    "Internal error: xmlSchemaAcquireSchemaDoc, "
	    "failed to build import table", NULL);
	xmlFreeDoc(*doc);
	*doc = NULL;
	return (-1);
    }
    import->schemaLocation = location;
    import->doc = *doc;
    return (0);
}

static void
xmlSchemaParseForImpInc(xmlSchemaParserCtxtPtr pctxt,
			xmlSchemaPtr schema,
			const xmlChar *targetNamespace,
			xmlNodePtr node)
{	
    const xmlChar *oldURL, **oldLocImps, *oldTNS;
    int oldFlags, oldNumLocImps, oldSizeLocImps;
    
    /*
    * Save and reset the context & schema.
    */
    oldURL = pctxt->URL;
    /* TODO: Is using the doc->URL here correct? */
    pctxt->URL = node->doc->URL;	
    oldLocImps = pctxt->localImports;
    pctxt->localImports = NULL;
    oldNumLocImps = pctxt->nbLocalImports;
    pctxt->nbLocalImports = 0;
    oldSizeLocImps = pctxt->sizeLocalImports;
    pctxt->sizeLocalImports = 0;
    oldFlags = schema->flags;
    xmlSchemaClearSchemaDefaults(schema);
    oldTNS = schema->targetNamespace;
    schema->targetNamespace = targetNamespace;
    /*
    * Parse the schema.
    */	
    xmlSchemaParseSchemaDefaults(pctxt, schema, node);
    xmlSchemaParseSchemaTopLevel(pctxt, schema, node->children);
    /*
    * Restore the context & schema.
    */
    schema->flags = oldFlags;
    schema->targetNamespace = oldTNS;
    if (pctxt->localImports != NULL)
	xmlFree((xmlChar *) pctxt->localImports);
    pctxt->localImports = oldLocImps;
    pctxt->nbLocalImports = oldNumLocImps;
    pctxt->sizeLocalImports = oldSizeLocImps;
    pctxt->URL = oldURL;
}

/**
 * xmlSchemaParseImport:
 * @ctxt:  a schema validation context
 * @schema:  the schema being built
 * @node:  a subtree containing XML Schema informations
 *
 * parse a XML schema Import definition
 * *WARNING* this interface is highly subject to change
 *
 * Returns 0 in case of success, a positive error code if 
 * not valid and -1 in case of an internal error. 
 */
static int
xmlSchemaParseImport(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                     xmlNodePtr node)
{    
    xmlNodePtr child;
    const xmlChar *namespaceName = NULL;
    const xmlChar *schemaLocation = NULL;
    const xmlChar *targetNamespace;
    xmlAttrPtr attr;
    xmlDocPtr doc;
    int ret = 0;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (-1);

    /*
    * Check for illegal attributes.
    */
    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "namespace")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "schemaLocation"))) {
		xmlSchemaPIllegalAttrErr(ctxt, 
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		    NULL, NULL, attr);		    
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt, 
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		NULL, NULL, attr);		
	}
	attr = attr->next;
    }	
    /*
    * Extract and validate attributes.
    */
    if (xmlSchemaPValAttr(ctxt, NULL, NULL, node, 
	"namespace", xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYURI), 
	&namespaceName) != 0) {
	xmlSchemaPSimpleTypeErr(ctxt,	    
	    XML_SCHEMAP_IMPORT_NAMESPACE_NOT_URI, 
	    NULL, NULL, node, 
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYURI), 
	    NULL, namespaceName, NULL, NULL, NULL);
	return (XML_SCHEMAP_IMPORT_NAMESPACE_NOT_URI);
    }

    if (xmlSchemaPValAttr(ctxt, NULL, NULL, node, 
	"schemaLocation", xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYURI), 
	&schemaLocation) != 0) {
	xmlSchemaPSimpleTypeErr(ctxt,	    
	    XML_SCHEMAP_IMPORT_SCHEMA_NOT_URI, 
	    NULL, NULL, node, 
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYURI), 
	    NULL, namespaceName, NULL, NULL, NULL);
	return (XML_SCHEMAP_IMPORT_SCHEMA_NOT_URI);
    }    
    /*
    * And now for the children...
    */
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
        /*
         * the annotation here is simply discarded ...
	 * TODO: really?
         */
        child = child->next;
    }
    if (child != NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_UNKNOWN_IMPORT_CHILD,
	    NULL, NULL, node, child, NULL,
	    "(annotation?)");
    }
    /*
    * Apply additional constraints.
    */
    if (namespaceName != NULL) {
	/*
	* 1.1 If the namespace [attribute] is present, then its �actual value� 
	* must not match the �actual value� of the enclosing <schema>'s 
	* targetNamespace [attribute].
	*/
	if (xmlStrEqual(schema->targetNamespace, namespaceName)) {
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_SRC_IMPORT_1_1,
		NULL, NULL, node,
		"The value of the attribute 'namespace' must not match "
		"the target namespace '%s' of the importing schema",
		schema->targetNamespace);
	    return (XML_SCHEMAP_SRC_IMPORT_1_1);
	}
    } else {
	/*
	* 1.2 If the namespace [attribute] is not present, then the enclosing 
	* <schema> must have a targetNamespace [attribute].
	*/
	if (schema->targetNamespace == NULL) {
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_SRC_IMPORT_1_2,
		NULL, NULL, node,
		"The attribute 'namespace' must be existent if "
		"the importing schema has no target namespace",
		NULL);
	    return (XML_SCHEMAP_SRC_IMPORT_1_2);
	}
    }
    /*
    * Add the namespace to the list of locally imported namespace.
    */
    if (ctxt->localImports == NULL) {
	ctxt->localImports = (const xmlChar **) xmlMalloc(10 * 
	    sizeof(const xmlChar*));
	ctxt->sizeLocalImports = 10;
	ctxt->nbLocalImports = 0;
    } else if (ctxt->sizeLocalImports <= ctxt->nbLocalImports) {
	ctxt->sizeLocalImports *= 2;
	ctxt->localImports = (const xmlChar **) xmlRealloc(
	    (xmlChar **) ctxt->localImports,
	    ctxt->sizeLocalImports * sizeof(const xmlChar*));
    }
    ctxt->localImports[ctxt->nbLocalImports++] = namespaceName;
    /*
    * Locate and aquire the schema document.
    */
    ret = xmlSchemaAcquireSchemaDoc(ctxt, schema, node, namespaceName, 
	schemaLocation, &doc, &targetNamespace, 0);
    if (ret != 0) {
	if (doc != NULL)
	    xmlFreeDoc(doc);
	return (ret);
    } else if (doc != NULL) {
       	xmlSchemaParseForImpInc(ctxt, schema, targetNamespace,
	    xmlDocGetRootElement(doc));
    }
    
    return (0);
}

/**
 * xmlSchemaParseInclude:
 * @ctxt:  a schema validation context
 * @schema:  the schema being built
 * @node:  a subtree containing XML Schema informations
 *
 * parse a XML schema Include definition
 *
 * Returns -1 in case of error, 0 if the declaration is improper and
 *         1 in case of success.
 */
static int
xmlSchemaParseInclude(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                      xmlNodePtr node)
{
    xmlNodePtr child = NULL;
    const xmlChar *schemaLocation, *targetNamespace;
    xmlDocPtr doc = NULL;
    xmlNodePtr root = NULL;
    xmlSchemaIncludePtr include = NULL;
    int wasConvertingNs = 0;
    xmlAttrPtr attr;
    xmlParserCtxtPtr parserCtxt;


    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (-1);

    /*
    * Check for illegal attributes.
    */
    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "schemaLocation"))) {
		xmlSchemaPIllegalAttrErr(ctxt, 
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		    NULL, NULL, attr);		    
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt, 
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		NULL, NULL, attr);		
	}
	attr = attr->next;
    }
    /*
    * Extract and validate attributes.
    */
    xmlSchemaPValAttrID(ctxt, NULL, NULL, node, BAD_CAST "id");
    /*
    * Preliminary step, extract the URI-Reference for the include and
    * make an URI from the base.
    */
    attr = xmlSchemaGetPropNode(node, "schemaLocation");
    if (attr != NULL) {
        xmlChar *base = NULL;
        xmlChar *uri = NULL;

	if (xmlSchemaPValAttrNode(ctxt, NULL, NULL, attr,
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYURI), &schemaLocation) != 0)
	    goto exit_invalid;
	base = xmlNodeGetBase(node->doc, node);
	if (base == NULL) {
	    uri = xmlBuildURI(schemaLocation, node->doc->URL);
	} else {
	    uri = xmlBuildURI(schemaLocation, base);
	    xmlFree(base);
	}
	if (uri == NULL) {
	    xmlSchemaPErr(ctxt,
		node,
		XML_SCHEMAP_INTERNAL,
		"Internal error: xmlSchemaParseInclude, "
		"could not build an URI from the schemaLocation.\n",
		NULL, NULL);
	    goto exit_failure;
	}
	schemaLocation = xmlDictLookup(ctxt->dict, uri, -1);
	xmlFree(uri);
    } else {
	xmlSchemaPMissingAttrErr(ctxt, 
	    XML_SCHEMAP_INCLUDE_SCHEMA_NO_URI, 
	    NULL, NULL, node, "schemaLocation", NULL);
	goto exit_invalid;
    }     
    /*
    * And now for the children...
    */
    child = node->children;
    while (IS_SCHEMA(child, "annotation")) {
        /*
         * the annotations here are simply discarded ...
	 * TODO: really?
         */
        child = child->next;
    }
    if (child != NULL) {
	xmlSchemaPContentErr(ctxt, 
	    XML_SCHEMAP_UNKNOWN_INCLUDE_CHILD,
	    NULL, NULL, node, child, NULL,
	    "(annotation?)");
    }
    /*
    * Report self-inclusion.
    */
    if (xmlStrEqual(schemaLocation, ctxt->URL)) {
	xmlSchemaPCustomErr(ctxt,
	    XML_SCHEMAP_SRC_INCLUDE,
	    NULL, NULL, node,
	    "The schema document '%s' cannot include itself.",
	    schemaLocation);
	return (XML_SCHEMAP_SRC_INCLUDE);
    }
    /*
    * Check if this one was already processed to avoid incorrect
    * duplicate component errors and infinite circular inclusion.
    */
    include = schema->includes;
    while (include != NULL) {
	if (xmlStrEqual(include->schemaLocation, schemaLocation)) {
	    targetNamespace = include->origTargetNamespace;
	    if (targetNamespace == NULL) {
		/*
		* Chameleon include: skip only if it was build for
		* the targetNamespace of the including schema.
		*/
		if (xmlStrEqual(schema->targetNamespace,
		    include->targetNamespace)) {
		    goto check_targetNamespace;
		}
	    } else {
		goto check_targetNamespace;
	    }
	}
	include = include->next;
    }
    /*
    * First step is to parse the input document into an DOM/Infoset
    * TODO: Use xmlCtxtReadFile to share the dictionary?
    */
    parserCtxt = xmlNewParserCtxt();
    if (parserCtxt == NULL) {
	xmlSchemaPErrMemory(NULL, "xmlSchemaParseInclude: "
	    "allocating a parser context", NULL);
	goto exit_failure;
    }	   
    
    if ((ctxt->dict != NULL) && (parserCtxt->dict != NULL)) {
	xmlDictFree(parserCtxt->dict);
	parserCtxt->dict = ctxt->dict;
	xmlDictReference(parserCtxt->dict);
    }
    
    doc = xmlCtxtReadFile(parserCtxt, (const char *) schemaLocation, 
	    NULL, SCHEMAS_PARSE_OPTIONS);
    xmlFreeParserCtxt(parserCtxt);
    if (doc == NULL) {
	/*
	* TODO: It is not an error for the �actual value� of the 
	* schemaLocation [attribute] to fail to resolve it all, in which 
	* case no corresponding inclusion is performed. 
	* So do we need a warning report here?
	*/
	xmlSchemaPCustomErr(ctxt,
	    XML_SCHEMAP_FAILED_LOAD,
	    NULL, NULL, node, 
	    "Failed to load the document '%s' for inclusion", schemaLocation);
	goto exit_invalid;
    }

    /*
     * Then extract the root of the schema
     */
    root = xmlDocGetRootElement(doc);
    if (root == NULL) {
	xmlSchemaPCustomErr(ctxt,
	    XML_SCHEMAP_NOROOT,
	    NULL, NULL, node,
	    "The included document '%s' has no document "
	    "element", schemaLocation);		
	goto exit_invalid;
    }

    /*
     * Remove all the blank text nodes
     */
    xmlSchemaCleanupDoc(ctxt, root);

    /*
     * Check the schemas top level element
     */
    if (!IS_SCHEMA(root, "schema")) {
	xmlSchemaPCustomErr(ctxt,
	    XML_SCHEMAP_NOT_SCHEMA,
	    NULL, NULL, node,
	    "The document '%s' to be included is not a schema document", 
	    schemaLocation);
	goto exit_invalid;
    }
    
    targetNamespace = xmlSchemaGetProp(ctxt, root, "targetNamespace");
    /*
    * 2.1 SII has a targetNamespace [attribute], and its �actual 
    * value� is identical to the �actual value� of the targetNamespace 
    * [attribute] of SII� (which must have such an [attribute]).
    */
check_targetNamespace:
    if (targetNamespace != NULL) {
	if (schema->targetNamespace == NULL) {
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_SRC_INCLUDE,
		NULL, NULL, node,
		"The target namespace of the included schema "
		"'%s' has to be absent, since the including schema "
		"has no target namespace", 
		schemaLocation);	    
	    goto exit_invalid;
	} else if (!xmlStrEqual(targetNamespace, schema->targetNamespace)) {
	    xmlSchemaPCustomErrExt(ctxt,
		XML_SCHEMAP_SRC_INCLUDE,
		NULL, NULL, node,
		"The target namespace '%s' of the included schema '%s' "
		"differs from '%s' of the including schema", 
		targetNamespace, schemaLocation, schema->targetNamespace);
	    goto exit_invalid;
	}
    } else if (schema->targetNamespace != NULL) {     	
	if ((schema->flags & XML_SCHEMAS_INCLUDING_CONVERT_NS) == 0) {
	    schema->flags |= XML_SCHEMAS_INCLUDING_CONVERT_NS;	    
	} else
	    wasConvertingNs = 1;
    }

    if (include != NULL)
	goto exit;

    /*
    * URGENT TODO: If the schema is a chameleon-include then copy the
    * components into the including schema and modify the targetNamespace
    * of those components, do nothing otherwise. 
    * NOTE: This is currently worked-around by compiling the chameleon 
    * for every destinct including targetNamespace; thus not performant at
    * the moment.
    * TODO: Check when the namespace in wildcards for chameleons needs
    * to be converted: before we built wildcard intersections or after.
    */
    /*
    * Register the include.
    */
    include = (xmlSchemaIncludePtr) xmlMalloc(sizeof(xmlSchemaInclude));
    if (include == NULL) {
        xmlSchemaPErrMemory(ctxt, "allocating include entry", NULL);
	goto exit_failure;
    }
    memset(include, 0, sizeof(xmlSchemaInclude));
    include->next = schema->includes;
    schema->includes = include;
    /*
    * TODO: Use the resolved URI for the this location, since it might
    * differ if using filenames/URIs simultaneosly.
    */
    include->schemaLocation = schemaLocation;
    include->doc = doc;
    /*
    * In case of chameleons, the original target namespace will differ
    * from the resulting namespace.
    */
    include->origTargetNamespace = targetNamespace;
    include->targetNamespace = schema->targetNamespace;
#ifdef DEBUG_INCLUDES
    if (targetNamespace != schema->targetNamespace) 
	xmlGenericError(xmlGenericErrorContext,
	    "INCLUDING CHAMELEON '%s'\n  orig TNS '%s'\n"
	    "  into TNS '%s'\n", schemaLocation,
	    targetNamespace, schema->targetNamespace);
    else
	xmlGenericError(xmlGenericErrorContext,
	    "INCLUDING '%s'\n  orig-TNS '%s'\n", schemaLocation,
	    targetNamespace);
#endif
    /*
    * Compile the included schema.
    */
    xmlSchemaParseForImpInc(ctxt, schema, schema->targetNamespace, root);

exit:
    /*
    * Remove the converting flag.
    */
    if ((wasConvertingNs == 0) && 
	(schema->flags & XML_SCHEMAS_INCLUDING_CONVERT_NS))
	schema->flags ^= XML_SCHEMAS_INCLUDING_CONVERT_NS;
    return (1);

exit_invalid:
    if (doc != NULL) {
	if (include != NULL)
	    include->doc = NULL;
	xmlFreeDoc(doc);
    }
    return (ctxt->err);

exit_failure:
    if (doc != NULL) {
	if (include != NULL)
	    include->doc = NULL;
	xmlFreeDoc(doc);
    }
    return (-1);
}

/**
 * xmlSchemaParseModelGroup:
 * @ctxt:  a schema validation context
 * @schema:  the schema being built
 * @node:  a subtree containing XML Schema informations
 * @type: the "compositor" type
 * @particleNeeded: if a a model group with a particle
 *
 * parse a XML schema Sequence definition
 * *WARNING* this interface is highly subject to change
 *
 * Returns -1 in case of error, 0 if the declaration is improper and
 *         1 in case of success.
 */
static xmlSchemaTreeItemPtr
xmlSchemaParseModelGroup(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
			 xmlNodePtr node, xmlSchemaTypeType type,
			 int withParticle)
{
    xmlSchemaModelGroupPtr item;
    xmlSchemaParticlePtr particle = NULL;
    xmlNodePtr child = NULL;
    xmlAttrPtr attr;
    const xmlChar *oldcontainer, *container;
    int min, max;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);    
    /* 
    * Create a model group with the given compositor.
    */
    item = xmlSchemaAddModelGroup(ctxt, schema, type, &container, node);
    if (item == NULL)
	return (NULL);

    if (withParticle) {
	if (type == XML_SCHEMA_TYPE_ALL) {
	    min = xmlGetMinOccurs(ctxt, node, 0, 1, 1, "(0 | 1)");
	    max = xmlGetMaxOccurs(ctxt, node, 1, 1, 1, "1"); 
	} else {
	    /* choice + sequence */
	    min = xmlGetMinOccurs(ctxt, node, 0, -1, 1, "nonNegativeInteger");
	    max = xmlGetMaxOccurs(ctxt, node, 0, UNBOUNDED, 1, 
		"(nonNegativeInteger | unbounded)");
	}    
	/* 
	* Create a particle 
	*/
	particle = xmlSchemaAddParticle(ctxt, schema, node, min, max);
	if (particle == NULL)
	    return (NULL);    
	particle->children = (xmlSchemaTreeItemPtr) item;
	/*
	* Check for illegal attributes.
	*/
	attr = node->properties;
	while (attr != NULL) {
	    if (attr->ns == NULL) {
		if ((!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		    (!xmlStrEqual(attr->name, BAD_CAST "maxOccurs")) &&
		    (!xmlStrEqual(attr->name, BAD_CAST "minOccurs"))) {
		    xmlSchemaPIllegalAttrErr(ctxt, 
			XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
			NULL, NULL, attr);		    
		}
	    } else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
		xmlSchemaPIllegalAttrErr(ctxt, 
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		    NULL, NULL, attr);		
	    }
	    attr = attr->next;
	}
    } else {
	/*
	* Check for illegal attributes.
	*/
	attr = node->properties;
	while (attr != NULL) {
	    if (attr->ns == NULL) {
		if (!xmlStrEqual(attr->name, BAD_CAST "id")) {
		    xmlSchemaPIllegalAttrErr(ctxt, 
			XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
			NULL, NULL, attr);		    
		}
	    } else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
		xmlSchemaPIllegalAttrErr(ctxt, 
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		    NULL, NULL, attr);		
	    }
	    attr = attr->next;
	}

    }
    
    /*
    * Extract and validate attributes.
    */
    xmlSchemaPValAttrID(ctxt, NULL, NULL, node, BAD_CAST "id");    
    /*
    * And now for the children...
    */
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
        item->annot = xmlSchemaParseAnnotation(ctxt, schema, child);
        child = child->next;
    }
    oldcontainer = ctxt->container;
    ctxt->container = container;
    if (type == XML_SCHEMA_TYPE_ALL) {
	xmlSchemaParticlePtr part, last = NULL;

	while (IS_SCHEMA(child, "element")) {	    
	    part = (xmlSchemaParticlePtr) xmlSchemaParseElement(ctxt,
		schema, child, 0);
	    if (part != NULL) {
		if (part->minOccurs > 1)
		    xmlSchemaPCustomErr(ctxt, XML_SCHEMAP_INVALID_MINOCCURS,
			NULL, NULL, child, 
			"Invalid value for minOccurs (must be 0 or 1)", NULL);
		if (part->maxOccurs > 1)
		    xmlSchemaPCustomErr(ctxt, XML_SCHEMAP_INVALID_MAXOCCURS,
			NULL, NULL, child, 
			"Invalid value for maxOccurs (must be 0 or 1)",
			NULL);
		if (last == NULL)
		    item->children = (xmlSchemaTreeItemPtr) part;
		else
		    last->next = (xmlSchemaTreeItemPtr) part;
		last = part;
	    }
	    child = child->next;
	}
	if (child != NULL) {
	    xmlSchemaPContentErr(ctxt,
		XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
		NULL, NULL, node, child, NULL,
		"(annotation?, (annotation?, element*)");
	}
    } else {
	/* choice + sequence */
	xmlSchemaTreeItemPtr part = NULL, last = NULL;

	while ((IS_SCHEMA(child, "element")) ||
	    (IS_SCHEMA(child, "group")) ||
	    (IS_SCHEMA(child, "any")) ||
	    (IS_SCHEMA(child, "choice")) ||
	    (IS_SCHEMA(child, "sequence"))) {

	    if (IS_SCHEMA(child, "element")) {
		part = (xmlSchemaTreeItemPtr)
		    xmlSchemaParseElement(ctxt, schema, child, 0);
	    } else if (IS_SCHEMA(child, "group")) {
		part = 
		    xmlSchemaParseModelGroupDefRef(ctxt, schema, child);
	    } else if (IS_SCHEMA(child, "any")) {
		part = (xmlSchemaTreeItemPtr) 
		    xmlSchemaParseAny(ctxt, schema, child);
	    } else if (IS_SCHEMA(child, "choice")) {
		part = xmlSchemaParseModelGroup(ctxt, schema, child,
		    XML_SCHEMA_TYPE_CHOICE, 1);
	    } else if (IS_SCHEMA(child, "sequence")) {
		part = xmlSchemaParseModelGroup(ctxt, schema, child,
		    XML_SCHEMA_TYPE_SEQUENCE, 1);
	    }
	    if (part != NULL) {
		if (last == NULL)
		    item->children = part;                
		else
		    last->next = part;
		last = part;
	    }
	    child = child->next;
	}    
	if (child != NULL) {
	    xmlSchemaPContentErr(ctxt,
		XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
		NULL, NULL, node, child, NULL,
		"(annotation?, (element | group | choice | sequence | any)*)");
	}
    }
    ctxt->container = oldcontainer;
    if (withParticle)
	return ((xmlSchemaTreeItemPtr) particle);
    else
	return ((xmlSchemaTreeItemPtr) item);
}

/**
 * xmlSchemaParseRestriction:
 * @ctxt:  a schema validation context
 * @schema:  the schema being built
 * @node:  a subtree containing XML Schema informations
 *
 * parse a XML schema Restriction definition
 * *WARNING* this interface is highly subject to change
 *
 * Returns the type definition or NULL in case of error
 */
static xmlSchemaTypePtr
xmlSchemaParseRestriction(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                          xmlNodePtr node, xmlSchemaTypeType parentType)
{
    xmlSchemaTypePtr type;
    xmlNodePtr child = NULL;
    char buf[30];
    const xmlChar *oldcontainer, *container;
    xmlAttrPtr attr;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);
    /* Not a component, don't create it. */  
    type = ctxt->ctxtType;
    type->flags |= XML_SCHEMAS_TYPE_DERIVATION_METHOD_RESTRICTION;
    
    /* 
    * TODO: Is the container needed at all? the anonymous
    * items inside should generate unique names already.
    */
    snprintf(buf, 29, "#restr%d", ctxt->counter++ + 1);
    container = xmlDictLookup(ctxt->dict, BAD_CAST buf, -1);    
    /*
    * Check for illegal attributes.
    */
    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "base"))) {
		xmlSchemaPIllegalAttrErr(ctxt, 
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		    NULL, NULL, attr);		    
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt, 
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		NULL, NULL, attr);		
	}
	attr = attr->next;
    }	
    /*
    * Extract and validate attributes.
    */
    xmlSchemaPValAttrID(ctxt, NULL, NULL, node, BAD_CAST "id");
    /*
    * Attribute "base" - mandatory if inside a complex type.
    */
    if ((xmlSchemaPValAttrQName(ctxt, schema,
	NULL, NULL, node, "base",
	&(type->baseNs), NULL,
	&(type->base)) == 0) &&
	(type->base == NULL) &&
	(type->type == XML_SCHEMA_TYPE_COMPLEX)) {
	xmlSchemaPMissingAttrErr(ctxt,
	    XML_SCHEMAP_S4S_ATTR_MISSING, 
	    NULL, type, node, "base", NULL);
    }    
    /*
    * And now for the children...
    */    
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
	/*
	* Add the annotation to the simple type ancestor.
	*/
	xmlSchemaAddAnnotation((xmlSchemaAnnotItemPtr) type,
	    xmlSchemaParseAnnotation(ctxt, schema, child));
        child = child->next;
    }
    oldcontainer = ctxt->container;
    ctxt->container = container;
    if (parentType == XML_SCHEMA_TYPE_SIMPLE) {
	/*
	* Corresponds to <simpleType><restriction><simpleType>.
	*/
	if (IS_SCHEMA(child, "simpleType")) {
	    if (type->base != NULL) {
		/* 
		* src-restriction-base-or-simpleType
		* Either the base [attribute] or the simpleType [child] of the 
		* <restriction> element must be present, but not both. 
		*/
		xmlSchemaPContentErr(ctxt, 
		    XML_SCHEMAP_SRC_RESTRICTION_BASE_OR_SIMPLETYPE,
		    NULL, NULL, node, child,
		    "The attribute 'base' and the <simpleType> child are "
		    "mutually exclusive", NULL);
	    } else {
		type->baseType = (xmlSchemaTypePtr)
		    xmlSchemaParseSimpleType(ctxt, schema, child, 0);
	    }	        
	    child = child->next;
	} else if (type->base == NULL) {
	    xmlSchemaPContentErr(ctxt, 
		XML_SCHEMAP_SRC_RESTRICTION_BASE_OR_SIMPLETYPE,
		NULL, NULL, node, child,
		"Either the attribute 'base' or a <simpleType> child "
		"must be present", NULL);
	}
    } else if (parentType == XML_SCHEMA_TYPE_COMPLEX_CONTENT) {
	/*
	* Corresponds to <complexType><complexContent><restriction>...
	* followed by:
	* 
	* Model groups <all>, <choice> and <sequence>.
	*/
	if (IS_SCHEMA(child, "all")) {
	    type->subtypes = (xmlSchemaTypePtr)
		xmlSchemaParseModelGroup(ctxt, schema, child,
		    XML_SCHEMA_TYPE_ALL, 1);
	    child = child->next;
	} else if (IS_SCHEMA(child, "choice")) {
	    type->subtypes = (xmlSchemaTypePtr)
		xmlSchemaParseModelGroup(ctxt,
		    schema, child, XML_SCHEMA_TYPE_CHOICE, 1);
	    child = child->next;
	} else if (IS_SCHEMA(child, "sequence")) {
	    type->subtypes = (xmlSchemaTypePtr)
		xmlSchemaParseModelGroup(ctxt, schema, child,
		    XML_SCHEMA_TYPE_SEQUENCE, 1);
	    child = child->next;
	/*
	* Model group reference <group>.
	*/
	} else if (IS_SCHEMA(child, "group")) {
	    type->subtypes = (xmlSchemaTypePtr)
		xmlSchemaParseModelGroupDefRef(ctxt, schema, child);
	    child = child->next;
	}
    } else if (parentType == XML_SCHEMA_TYPE_SIMPLE_CONTENT) {
	xmlSchemaTypePtr contType, baseType = NULL;
	/*
	* Corresponds to <complexType><simpleContent><restriction>...
	*
	* SPEC (content type):
	* "1 If the type definition �resolved� to by the �actual value� of
	* the base [attribute] is a complex type definition whose own
	* {content type} is a simple type definition and the <restriction>
	* alternative is chosen, then starting from either" ...
	*
	* "1.1 the simple type definition corresponding to the <simpleType>
	* among the [children] of <restriction> if there is one;"
	*/
	if (IS_SCHEMA(child, "simpleType")) {
	    
	    baseType = (xmlSchemaTypePtr)
		xmlSchemaParseSimpleType(ctxt, schema, child, 0);
	    if (baseType == NULL)
		return (NULL);	    	    
	    child = child->next;
	}
	/*
	* SPEC
	* "... a simple type definition which restricts the simple type
	* definition identified in clause 1.1 or clause 1.2 with a set
	* of facet components"
	*
	* Create the anonymous simple type, which will be the content type
	* of the complex type.
	* Note that we will use the same node as for the <restriction> to
	* have it somehow anchored in the schema doc.
	*/
	snprintf(buf, 29, "#scST%d", ctxt->counter++ + 1);
	contType = xmlSchemaAddType(ctxt, schema, (const xmlChar *)buf,
	    container, node);
	if (contType == NULL)
	    return (NULL);
	contType->node = node;
	contType->type = XML_SCHEMA_TYPE_SIMPLE;
	contType->baseType = baseType;
	type->contentTypeDef = contType;			
    }

    if ((parentType == XML_SCHEMA_TYPE_SIMPLE) ||
	(parentType == XML_SCHEMA_TYPE_SIMPLE_CONTENT)) {	
	xmlSchemaFacetPtr facet, lastfacet = NULL;
	xmlSchemaTypePtr facetHolder;
	
	if (parentType == XML_SCHEMA_TYPE_SIMPLE)
	    facetHolder = type;
	else
	    facetHolder = type->contentTypeDef;
	/*
	* Corresponds to <complexType><simpleContent><restriction>...
	* <simpleType><restriction>... 
	*/
		
	/*
	* Add the facets to the simple type ancestor.
	*/
	/*
	* TODO: Datatypes: 4.1.3 Constraints on XML Representation of 
	* Simple Type Definition Schema Representation Constraint: 
	* *Single Facet Value*
	*/
	while ((IS_SCHEMA(child, "minInclusive")) ||
	    (IS_SCHEMA(child, "minExclusive")) ||
	    (IS_SCHEMA(child, "maxInclusive")) ||
	    (IS_SCHEMA(child, "maxExclusive")) ||
	    (IS_SCHEMA(child, "totalDigits")) ||
	    (IS_SCHEMA(child, "fractionDigits")) ||
	    (IS_SCHEMA(child, "pattern")) ||
	    (IS_SCHEMA(child, "enumeration")) ||
	    (IS_SCHEMA(child, "whiteSpace")) ||
	    (IS_SCHEMA(child, "length")) ||
	    (IS_SCHEMA(child, "maxLength")) ||
	    (IS_SCHEMA(child, "minLength"))) {
	    facet = xmlSchemaParseFacet(ctxt, schema, child);
	    if (facet != NULL) {
		if (lastfacet == NULL)
		    facetHolder->facets = facet;			
		else
		    lastfacet->next = facet;
		lastfacet = facet;
		lastfacet->next = NULL;
	    }
	    child = child->next;
	}
	/*
	* Create links for derivation and validation.
	*/	    
	if (facetHolder->facets != NULL) {
	    xmlSchemaFacetLinkPtr facetLink, lastFacetLink = NULL;

	    facet = facetHolder->facets;
	    do {		    
		facetLink = (xmlSchemaFacetLinkPtr)
		    xmlMalloc(sizeof(xmlSchemaFacetLink));
		if (facetLink == NULL) {
		    xmlSchemaPErrMemory(ctxt, "allocating a facet link", NULL);
		    xmlFree(facetLink);
		    return (NULL);
		}	
		facetLink->facet = facet;
		facetLink->next = NULL;
		if (lastFacetLink == NULL) 
		    facetHolder->facetSet = facetLink;			                                         
		else
		    lastFacetLink->next = facetLink;
		lastFacetLink = facetLink;
		facet = facet->next;
	    } while (facet != NULL);
	}
    }    
    if (type->type == XML_SCHEMA_TYPE_COMPLEX) {
	/*
	* Attribute uses/declarations.
	*/
	child = xmlSchemaParseAttrDecls(ctxt, schema, child, type);
	/*
	* Attribute wildcard.
	*/
	if (IS_SCHEMA(child, "anyAttribute")) {
	    type->attributeWildcard =
		xmlSchemaParseAnyAttribute(ctxt, schema, child);
	    child = child->next;
	}
    }
    if (child != NULL) {
	if (parentType == XML_SCHEMA_TYPE_COMPLEX_CONTENT) {	    
	    xmlSchemaPContentErr(ctxt, 
		XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED, 
		NULL, NULL, node, child, NULL,
		"annotation?, (group | all | choice | sequence)?, "
		"((attribute | attributeGroup)*, anyAttribute?))");
	} else if (parentType == XML_SCHEMA_TYPE_SIMPLE_CONTENT) {
	     xmlSchemaPContentErr(ctxt, 
		XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED, 
		NULL, NULL, node, child, NULL,
		"(annotation?, (simpleType?, (minExclusive | minInclusive | "
		"maxExclusive | maxInclusive | totalDigits | fractionDigits | "
		"length | minLength | maxLength | enumeration | whiteSpace | "
		"pattern)*)?, ((attribute | attributeGroup)*, anyAttribute?))");
	} else {
	    /* Simple type */
	    xmlSchemaPContentErr(ctxt, 
		XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED, 
		NULL, NULL, node, child, NULL,
		"(annotation?, (simpleType?, (minExclusive | minInclusive | "
		"maxExclusive | maxInclusive | totalDigits | fractionDigits | "
		"length | minLength | maxLength | enumeration | whiteSpace | "
		"pattern)*))");
	}
    }       
    ctxt->container = oldcontainer;
    return (NULL);
}

/**
 * xmlSchemaParseExtension:
 * @ctxt:  a schema validation context
 * @schema:  the schema being built
 * @node:  a subtree containing XML Schema informations
 *
 * Parses an <extension>, which is found inside a
 * <simpleContent> or <complexContent>.
 * *WARNING* this interface is highly subject to change. 
 *
 * TODO: Returns the type definition or NULL in case of error
 */
static xmlSchemaTypePtr
xmlSchemaParseExtension(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                        xmlNodePtr node, xmlSchemaTypeType parentType)
{
    xmlSchemaTypePtr type;
    xmlNodePtr child = NULL;
    char buf[30];
    const xmlChar *oldcontainer, *container;
    xmlAttrPtr attr;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);
    /* Not a component, don't create it. */    
    type = ctxt->ctxtType;
    type->flags |= XML_SCHEMAS_TYPE_DERIVATION_METHOD_EXTENSION;

    snprintf(buf, 29, "#ext%d", ctxt->counter++ + 1);
    container = xmlDictLookup(ctxt->dict, BAD_CAST buf, -1);
    /*
    * Check for illegal attributes.
    */
    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "base"))) {
		xmlSchemaPIllegalAttrErr(ctxt, 
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		    NULL, NULL, attr);		    
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt, 
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		NULL, NULL, attr);		
	}
	attr = attr->next;
    }	

    xmlSchemaPValAttrID(ctxt, NULL, NULL, node, BAD_CAST "id");
    
    /*
    * Attribute "base" - mandatory.
    */
    if ((xmlSchemaPValAttrQName(ctxt, schema,
	NULL, NULL, node, "base", &(type->baseNs), NULL,
	&(type->base)) == 0) && (type->base == NULL)) {
	xmlSchemaPMissingAttrErr(ctxt,
	    XML_SCHEMAP_S4S_ATTR_MISSING, 
	    NULL, NULL, node, "base", NULL);
    }  
    /*
    * And now for the children...
    */
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
	/*
	* Add the annotation to the type ancestor.
	*/
	xmlSchemaAddAnnotation((xmlSchemaAnnotItemPtr) type,
	    xmlSchemaParseAnnotation(ctxt, schema, child));
        child = child->next;
    }
    oldcontainer = ctxt->container;
    ctxt->container = container;
    if (parentType == XML_SCHEMA_TYPE_COMPLEX_CONTENT) {
	/*
	* Corresponds to <complexType><complexContent><extension>... and:
	*
	* Model groups <all>, <choice>, <sequence> and <group>.
	*/
	if (IS_SCHEMA(child, "all")) {
	    type->subtypes = (xmlSchemaTypePtr)
		xmlSchemaParseModelGroup(ctxt, schema,
		    child, XML_SCHEMA_TYPE_ALL, 1);
	    child = child->next;
	} else if (IS_SCHEMA(child, "choice")) {
	    type->subtypes = (xmlSchemaTypePtr)
		xmlSchemaParseModelGroup(ctxt, schema,
		    child, XML_SCHEMA_TYPE_CHOICE, 1);
	    child = child->next;
	} else if (IS_SCHEMA(child, "sequence")) {
	    type->subtypes = (xmlSchemaTypePtr)
		xmlSchemaParseModelGroup(ctxt, schema,
		child, XML_SCHEMA_TYPE_SEQUENCE, 1);
	    child = child->next;
	} else if (IS_SCHEMA(child, "group")) {
	    type->subtypes = (xmlSchemaTypePtr)
		xmlSchemaParseModelGroupDefRef(ctxt, schema, child);
	    child = child->next;
	}
    }
    if (child != NULL) {
	/*
	* Attribute uses/declarations.
	*/
	child = xmlSchemaParseAttrDecls(ctxt, schema, child, type);
	/*
	* Attribute wildcard.
	*/
	if (IS_SCHEMA(child, "anyAttribute")) {	    
	    ctxt->ctxtType->attributeWildcard = 
		xmlSchemaParseAnyAttribute(ctxt, schema, child);
	    child = child->next;
	}
    }
    if (child != NULL) {
	if (parentType == XML_SCHEMA_TYPE_COMPLEX_CONTENT) {
	    /* Complex content extension. */
	    xmlSchemaPContentErr(ctxt, 
		XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED, 
		NULL, NULL, node, child, NULL,
		"(annotation?, ((group | all | choice | sequence)?, "
		"((attribute | attributeGroup)*, anyAttribute?)))");	    	
	} else {
	    /* Simple content extension. */
	    xmlSchemaPContentErr(ctxt, 
		XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED, 
		NULL, NULL, node, child, NULL,
		"(annotation?, ((attribute | attributeGroup)*, "
		"anyAttribute?))");
	}
    }
    ctxt->container = oldcontainer;
    return (NULL);
}

/**
 * xmlSchemaParseSimpleContent:
 * @ctxt:  a schema validation context
 * @schema:  the schema being built
 * @node:  a subtree containing XML Schema informations
 *
 * parse a XML schema SimpleContent definition
 * *WARNING* this interface is highly subject to change
 *
 * Returns the type definition or NULL in case of error
 */
static int
xmlSchemaParseSimpleContent(xmlSchemaParserCtxtPtr ctxt,
                            xmlSchemaPtr schema, xmlNodePtr node)
{
    xmlSchemaTypePtr type;
    xmlNodePtr child = NULL;
    xmlAttrPtr attr;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (-1);
    /* Not a component, don't create it. */
    type = ctxt->ctxtType;
    type->contentType = XML_SCHEMA_CONTENT_SIMPLE;
    /*
    * Check for illegal attributes.
    */
    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "id"))) {
		xmlSchemaPIllegalAttrErr(ctxt, 
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		    NULL, NULL, attr);		    
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt, 
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		NULL, NULL, attr);		
	}
	attr = attr->next;
    }	

    xmlSchemaPValAttrID(ctxt, NULL, NULL, node, BAD_CAST "id");

    /*
    * And now for the children...
    */
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
	/*
	* Add the annotation to the complex type ancestor.
	*/
	xmlSchemaAddAnnotation((xmlSchemaAnnotItemPtr) type,
	    xmlSchemaParseAnnotation(ctxt, schema, child));
        child = child->next;
    }
    if (IS_SCHEMA(child, "restriction")) {
        xmlSchemaParseRestriction(ctxt, schema, child,
	    XML_SCHEMA_TYPE_SIMPLE_CONTENT);
        child = child->next;
    } else if (IS_SCHEMA(child, "extension")) {
        xmlSchemaParseExtension(ctxt, schema, child,
	    XML_SCHEMA_TYPE_SIMPLE_CONTENT);
        child = child->next;
    }
    if (child != NULL) {	
	xmlSchemaPContentErr(ctxt, 
	    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED, 
	    NULL, NULL, node, child, NULL,
	    "(annotation?, (restriction | extension))");
    }
    return (0);
}

/**
 * xmlSchemaParseComplexContent:
 * @ctxt:  a schema validation context
 * @schema:  the schema being built
 * @node:  a subtree containing XML Schema informations
 *
 * parse a XML schema ComplexContent definition
 * *WARNING* this interface is highly subject to change
 *
 * Returns the type definition or NULL in case of error
 */
static int
xmlSchemaParseComplexContent(xmlSchemaParserCtxtPtr ctxt,
                             xmlSchemaPtr schema, xmlNodePtr node)
{
    xmlSchemaTypePtr type;
    xmlNodePtr child = NULL;
    xmlAttrPtr attr;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (-1);
    /* Not a component, don't create it. */
    type = ctxt->ctxtType;
    /*
    * Check for illegal attributes.
    */
    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "mixed"))) 
	    {
		xmlSchemaPIllegalAttrErr(ctxt,
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED,
		    NULL, NULL, attr);
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt,
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED,
		NULL, NULL, attr);
	}
	attr = attr->next;
    }	

    xmlSchemaPValAttrID(ctxt, NULL, NULL, node, BAD_CAST "id");

    /*
    * Set the 'mixed' on the complex type ancestor.
    */
    if (xmlGetBooleanProp(ctxt, NULL, NULL, node, "mixed", 0))  {
	if ((type->flags & XML_SCHEMAS_TYPE_MIXED) == 0)
	    type->flags |= XML_SCHEMAS_TYPE_MIXED;
    }
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
	/*
	* Add the annotation to the complex type ancestor.
	*/
	xmlSchemaAddAnnotation((xmlSchemaAnnotItemPtr) type,
	    xmlSchemaParseAnnotation(ctxt, schema, child));
        child = child->next;
    }
    if (IS_SCHEMA(child, "restriction")) {        
        xmlSchemaParseRestriction(ctxt, schema, child,
	    XML_SCHEMA_TYPE_COMPLEX_CONTENT);
        child = child->next;
    } else if (IS_SCHEMA(child, "extension")) {
        xmlSchemaParseExtension(ctxt, schema, child,
	    XML_SCHEMA_TYPE_COMPLEX_CONTENT);
        child = child->next;
    }
    if (child != NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
	    NULL, NULL, node, child,
	    NULL, "(annotation?, (restriction | extension))");
    }
    return (0);
}

/**
 * xmlSchemaParseComplexType:
 * @ctxt:  a schema validation context
 * @schema:  the schema being built
 * @node:  a subtree containing XML Schema informations
 *
 * parse a XML schema Complex Type definition
 * *WARNING* this interface is highly subject to change
 *
 * Returns the type definition or NULL in case of error
 */
static xmlSchemaTypePtr
xmlSchemaParseComplexType(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                          xmlNodePtr node, int topLevel)
{
    xmlSchemaTypePtr type, ctxtType;
    xmlNodePtr child = NULL;
    const xmlChar *oldcontainer, *name = NULL;
    xmlAttrPtr attr;
    const xmlChar *attrValue;
    xmlChar *des = NULL; /* The reported designation. */
    char buf[40];


    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);

    ctxtType = ctxt->ctxtType;

    if (topLevel) {
	attr = xmlSchemaGetPropNode(node, "name");
	if (attr == NULL) {
	    xmlSchemaPMissingAttrErr(ctxt, 
		XML_SCHEMAP_S4S_ATTR_MISSING, 
		(xmlChar **) &xmlSchemaElemDesCT, NULL, node,
		"name", NULL);
	    return (NULL);
	} else if (xmlSchemaPValAttrNode(ctxt, 
	    (xmlChar **) &xmlSchemaElemDesCT, NULL, attr, 
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_NCNAME), &name) != 0) {
	    return (NULL);
	}
    }
            
    if (topLevel == 0) {
	/*
	* Parse as local complex type definition.
	*/
        snprintf(buf, 39, "#CT%d", ctxt->counter++ + 1);
	type = xmlSchemaAddType(ctxt, schema, (const xmlChar *)buf, NULL, node);
	if (type == NULL)
	    return (NULL);
	name = type->name;
	type->node = node;
	type->type = XML_SCHEMA_TYPE_COMPLEX;
	/*
	* TODO: We need the target namespace.
	*/	
    } else {	
	/*
	* Parse as global complex type definition.
	*/	
	type = xmlSchemaAddType(ctxt, schema, name, schema->targetNamespace, node);
	if (type == NULL)
	    return (NULL);
	type->node = node;
	type->type = XML_SCHEMA_TYPE_COMPLEX;
	type->flags |= XML_SCHEMAS_TYPE_GLOBAL;	
	/* 
	* Set defaults.
	*/
	type->flags |= XML_SCHEMAS_TYPE_FINAL_DEFAULT;
	type->flags |= XML_SCHEMAS_TYPE_BLOCK_DEFAULT;
    }
    type->targetNamespace = schema->targetNamespace;
    /*
    * Handle attributes.
    */
    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if (xmlStrEqual(attr->name, BAD_CAST "id")) {
		/*
		* Attribute "id".
		*/
		xmlSchemaPValAttrID(ctxt, NULL, type, node,
		    BAD_CAST "id");
	    } else if (xmlStrEqual(attr->name, BAD_CAST "mixed")) {
		/*
		* Attribute "mixed".
		*/
		if (xmlSchemaPGetBoolNodeValue(ctxt, &des, type, 
		    (xmlNodePtr) attr))
		    type->flags |= XML_SCHEMAS_TYPE_MIXED; 		
	    } else if (topLevel) {		
		/*
		* Attributes of global complex type definitions.
		*/
		if (xmlStrEqual(attr->name, BAD_CAST "name")) {
		    /* Pass. */
		} else if (xmlStrEqual(attr->name, BAD_CAST "abstract")) {
		    /*
		    * Attribute "abstract".
		    */
		    if (xmlSchemaPGetBoolNodeValue(ctxt, &des, type, 
			(xmlNodePtr) attr))		    
			type->flags |= XML_SCHEMAS_TYPE_ABSTRACT;
		} else if (xmlStrEqual(attr->name, BAD_CAST "final")) {
		    /*
		    * Attribute "final".
		    */
		    attrValue = xmlSchemaGetNodeContent(ctxt, 
			(xmlNodePtr) attr);
		    if (xmlSchemaPValAttrBlockFinal(attrValue, 
			&(type->flags), 
			-1, 
			XML_SCHEMAS_TYPE_FINAL_EXTENSION, 
			XML_SCHEMAS_TYPE_FINAL_RESTRICTION, 
			-1, -1, -1) != 0) 
		    {
			xmlSchemaPSimpleTypeErr(ctxt, 
			    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
			    &des, type, (xmlNodePtr) attr, 
			    NULL, 
			    "(#all | List of (extension | restriction))", 
			    attrValue, NULL, NULL, NULL);
		    }
		} else if (xmlStrEqual(attr->name, BAD_CAST "block")) {
		    /*
		    * Attribute "block".
		    */			
		    attrValue = xmlSchemaGetNodeContent(ctxt, 
			(xmlNodePtr) attr);	    
		    if (xmlSchemaPValAttrBlockFinal(attrValue, &(type->flags), 
			-1,
			XML_SCHEMAS_TYPE_BLOCK_EXTENSION,
			XML_SCHEMAS_TYPE_BLOCK_RESTRICTION, 
			-1, -1, -1) != 0) {
			xmlSchemaPSimpleTypeErr(ctxt,
			    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
			    &des, type, (xmlNodePtr) attr,
			    NULL, 
			    "(#all | List of (extension | restriction)) ", 
			    attrValue, NULL, NULL, NULL);
		    }
		} else {
			xmlSchemaPIllegalAttrErr(ctxt, 
			    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
			    &des, type, attr);
		}
	    } else {	    
		xmlSchemaPIllegalAttrErr(ctxt, 
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		    &des, type, attr);
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt, 
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, 
		&des, type, attr);	
	}
	attr = attr->next;
    }
    /*
    * And now for the children...
    */
    oldcontainer = ctxt->container;
    ctxt->container = name;    
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
        type->annot = xmlSchemaParseAnnotation(ctxt, schema, child);
        child = child->next;
    }
    ctxt->ctxtType = type;
    if (IS_SCHEMA(child, "simpleContent")) {
	/* 
	* 3.4.3 : 2.2  
	* Specifying mixed='true' when the <simpleContent>
	* alternative is chosen has no effect
	*/
	if (type->flags & XML_SCHEMAS_TYPE_MIXED)
	    type->flags ^= XML_SCHEMAS_TYPE_MIXED;
        xmlSchemaParseSimpleContent(ctxt, schema, child);
        child = child->next;
    } else if (IS_SCHEMA(child, "complexContent")) {
	type->contentType = XML_SCHEMA_CONTENT_EMPTY;
        xmlSchemaParseComplexContent(ctxt, schema, child);
        child = child->next;
    } else {
	/*
	* SPEC
	* "...the third alternative (neither <simpleContent> nor
	* <complexContent>) is chosen. This case is understood as shorthand
	* for complex content restricting the �ur-type definition�, and the
	* details of the mappings should be modified as necessary.
	*/
	type->baseType = xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYTYPE);
	type->flags |= XML_SCHEMAS_TYPE_DERIVATION_METHOD_RESTRICTION;
	/*
	* Parse model groups.
	*/
        if (IS_SCHEMA(child, "all")) {
            type->subtypes = (xmlSchemaTypePtr)
		xmlSchemaParseModelGroup(ctxt, schema, child,
		    XML_SCHEMA_TYPE_ALL, 1);
            child = child->next;
        } else if (IS_SCHEMA(child, "choice")) {
            type->subtypes = (xmlSchemaTypePtr)
		xmlSchemaParseModelGroup(ctxt, schema, child,
		    XML_SCHEMA_TYPE_CHOICE, 1);
            child = child->next;
        } else if (IS_SCHEMA(child, "sequence")) {
            type->subtypes = (xmlSchemaTypePtr)
		xmlSchemaParseModelGroup(ctxt, schema, child,
		    XML_SCHEMA_TYPE_SEQUENCE, 1);
            child = child->next;
        } else if (IS_SCHEMA(child, "group")) {
            type->subtypes = (xmlSchemaTypePtr)
		xmlSchemaParseModelGroupDefRef(ctxt, schema, child);
            child = child->next;
        }
	/*
	* Parse attribute decls/refs.
	*/
        child = xmlSchemaParseAttrDecls(ctxt, schema, child, type);
	/*
	* Parse attribute wildcard.
	*/
	if (IS_SCHEMA(child, "anyAttribute")) {	    
	    type->attributeWildcard = xmlSchemaParseAnyAttribute(ctxt, schema, child);
	    child = child->next;
	}
    }
    if (child != NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED, 
	    &des, type, node, child,
	    NULL, "(annotation?, (simpleContent | complexContent | "
	    "((group | all | choice | sequence)?, ((attribute | "
	    "attributeGroup)*, anyAttribute?))))");
    }
    FREE_AND_NULL(des);
    ctxt->container = oldcontainer;
    ctxt->ctxtType = ctxtType;
    return (type);
}

/**
 * xmlSchemaParseSchema:
 * @ctxt:  a schema validation context
 * @node:  a subtree containing XML Schema informations
 *
 * parse a XML schema definition from a node set
 * *WARNING* this interface is highly subject to change
 *
 * Returns the internal XML Schema structure built from the resource or
 *         NULL in case of error
 */
static xmlSchemaPtr
xmlSchemaParseSchema(xmlSchemaParserCtxtPtr ctxt, xmlNodePtr node)
{
    xmlSchemaPtr schema = NULL;
    const xmlChar *val;
    int nberrors;
    xmlAttrPtr attr;

    /*
    * This one is called by xmlSchemaParse only and is used if
    * the schema to be parsed was specified via the API; i.e. not
    * automatically by the validated instance document.
    */
    if ((ctxt == NULL) || (node == NULL))
        return (NULL);
    nberrors = ctxt->nberrors;
    ctxt->nberrors = 0;
    if (IS_SCHEMA(node, "schema")) {
	xmlSchemaImportPtr import;

        schema = xmlSchemaNewSchema(ctxt);
        if (schema == NULL)
            return (NULL);
	/*
	* Disable build of list of items.
	*/
	attr = xmlSchemaGetPropNode(node, "targetNamespace"); 		
	if (attr != NULL) {
	    xmlSchemaPValAttrNode(ctxt, NULL, NULL, attr, 
		xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYURI), &val);
	    /*
	    * TODO: Should we proceed with an invalid target namespace?
	    */
	    schema->targetNamespace = xmlDictLookup(ctxt->dict, val, -1);
	} else {
	    schema->targetNamespace = NULL;
	}
	/*
	* Add the current ns name and location to the import table;
	* this is needed to have a consistent mechanism, regardless
	* if all schemata are constructed dynamically fired by the
	* instance or if the schema to be used was specified via
	* the API.
	*/
	import = xmlSchemaAddImport(ctxt, &(schema->schemasImports),
	    schema->targetNamespace);
	if (import == NULL) {
	    xmlSchemaPCustomErr(ctxt, XML_SCHEMAP_FAILED_BUILD_IMPORT,
		NULL, NULL, (xmlNodePtr) ctxt->doc,
		"Internal error: xmlSchemaParseSchema, "
		"failed to add an import entry", NULL);
	    xmlSchemaFree(schema);
	    schema = NULL;
	    return (NULL);
	}
	import->schemaLocation = ctxt->URL;
	/*
	* NOTE: We won't set the doc here, otherwise it will be freed
	* if the import struct is freed.
	* import->doc = ctxt->doc;
	*/
	xmlSchemaParseSchemaDefaults(ctxt, schema, node);	
        xmlSchemaParseSchemaTopLevel(ctxt, schema, node->children);
    } else {
        xmlDocPtr doc;

	doc = node->doc;

        if ((doc != NULL) && (doc->URL != NULL)) {
	    xmlSchemaPErr(ctxt, (xmlNodePtr) doc,
		      XML_SCHEMAP_NOT_SCHEMA,
		      "The file \"%s\" is not a XML schema.\n", doc->URL, NULL);
	} else {
	    xmlSchemaPErr(ctxt, (xmlNodePtr) doc,
		      XML_SCHEMAP_NOT_SCHEMA,
		      "The file is not a XML schema.\n", NULL, NULL);
	}
	return(NULL);
    }
    if (ctxt->nberrors != 0) {
        if (schema != NULL) {
            xmlSchemaFree(schema);
            schema = NULL;
        }
    }
    if (schema != NULL)
	schema->counter = ctxt->counter;
    ctxt->nberrors = nberrors;
#ifdef DEBUG
    if (schema == NULL)
        xmlGenericError(xmlGenericErrorContext,
                        "xmlSchemaParse() failed\n");
#endif
    return (schema);
}

/************************************************************************
 * 									*
 * 			Validating using Schemas			*
 * 									*
 ************************************************************************/

/************************************************************************
 * 									*
 * 			Reading/Writing Schemas				*
 * 									*
 ************************************************************************/

#if 0 /* Will be enabled if it is clear what options are needed. */
/**
 * xmlSchemaParserCtxtSetOptions:
 * @ctxt:	a schema parser context
 * @options: a combination of xmlSchemaParserOption
 *
 * Sets the options to be used during the parse.
 *
 * Returns 0 in case of success, -1 in case of an
 * API error.
 */
static int
xmlSchemaParserCtxtSetOptions(xmlSchemaParserCtxtPtr ctxt,
			      int options)
					
{
    int i;

    if (ctxt == NULL)
	return (-1);
    /*
    * WARNING: Change the start value if adding to the
    * xmlSchemaParseOption.
    */
    for (i = 1; i < (int) sizeof(int) * 8; i++) {
        if (options & 1<<i) {
	    return (-1);   
        }	
    }
    ctxt->options = options;
    return (0);      
}

/**
 * xmlSchemaValidCtxtGetOptions:
 * @ctxt: a schema parser context 
 *
 * Returns the option combination of the parser context.
 */
static int
xmlSchemaParserCtxtGetOptions(xmlSchemaParserCtxtPtr ctxt)
					
{    
    if (ctxt == NULL)
	return (-1);
    else 
	return (ctxt->options);    
}

 void *curItems;  /* used for dynamic addition of schemata */
    int nbCurItems; /* used for dynamic addition of schemata */
    int sizeCurItems; /* used for dynamic addition of schemata */

#endif

/**
 * xmlSchemaNewParserCtxt:
 * @URL:  the location of the schema
 *
 * Create an XML Schemas parse context for that file/resource expected
 * to contain an XML Schemas file.
 *
 * Returns the parser context or NULL in case of error
 */
xmlSchemaParserCtxtPtr
xmlSchemaNewParserCtxt(const char *URL)
{
    xmlSchemaParserCtxtPtr ret;

    if (URL == NULL)
        return (NULL);

    ret = (xmlSchemaParserCtxtPtr) xmlMalloc(sizeof(xmlSchemaParserCtxt));
    if (ret == NULL) {
        xmlSchemaPErrMemory(NULL, "allocating schema parser context",
                            NULL);
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaParserCtxt));
    ret->dict = xmlDictCreate();
    ret->URL = xmlDictLookup(ret->dict, (const xmlChar *) URL, -1);
    ret->includes = 0;
    return (ret);
}

/**
 * xmlSchemaNewParserCtxtUseDict:
 * @URL:  the location of the schema
 * @dict: the dictionary to be used
 *
 * Create an XML Schemas parse context for that file/resource expected
 * to contain an XML Schemas file.
 *
 * Returns the parser context or NULL in case of error
 */
static xmlSchemaParserCtxtPtr
xmlSchemaNewParserCtxtUseDict(const char *URL, xmlDictPtr dict)
{
    xmlSchemaParserCtxtPtr ret;
    /*
    if (URL == NULL)
        return (NULL);
	*/

    ret = (xmlSchemaParserCtxtPtr) xmlMalloc(sizeof(xmlSchemaParserCtxt));
    if (ret == NULL) {
        xmlSchemaPErrMemory(NULL, "allocating schema parser context",
                            NULL);
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaParserCtxt));
    ret->dict = dict;
    xmlDictReference(dict);    
    if (URL != NULL)
	ret->URL = xmlDictLookup(dict, (const xmlChar *) URL, -1);
    ret->includes = 0;
    return (ret);
}


/**
 * xmlSchemaNewMemParserCtxt:
 * @buffer:  a pointer to a char array containing the schemas
 * @size:  the size of the array
 *
 * Create an XML Schemas parse context for that memory buffer expected
 * to contain an XML Schemas file.
 *
 * Returns the parser context or NULL in case of error
 */
xmlSchemaParserCtxtPtr
xmlSchemaNewMemParserCtxt(const char *buffer, int size)
{
    xmlSchemaParserCtxtPtr ret;

    if ((buffer == NULL) || (size <= 0))
        return (NULL);

    ret = (xmlSchemaParserCtxtPtr) xmlMalloc(sizeof(xmlSchemaParserCtxt));
    if (ret == NULL) {
        xmlSchemaPErrMemory(NULL, "allocating schema parser context",
                            NULL);
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaParserCtxt));
    ret->buffer = buffer;
    ret->size = size;
    ret->dict = xmlDictCreate();
    return (ret);
}

/**
 * xmlSchemaNewDocParserCtxt:
 * @doc:  a preparsed document tree
 *
 * Create an XML Schemas parse context for that document.
 * NB. The document may be modified during the parsing process.
 *
 * Returns the parser context or NULL in case of error
 */
xmlSchemaParserCtxtPtr
xmlSchemaNewDocParserCtxt(xmlDocPtr doc)
{
    xmlSchemaParserCtxtPtr ret;

    if (doc == NULL)
      return (NULL);

    ret = (xmlSchemaParserCtxtPtr) xmlMalloc(sizeof(xmlSchemaParserCtxt));
    if (ret == NULL) {
      xmlSchemaPErrMemory(NULL, "allocating schema parser context",
			  NULL);
      return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaParserCtxt));
    ret->doc = doc;
    ret->dict = xmlDictCreate();
    /* The application has responsibility for the document */
    ret->preserve = 1;

    return (ret);
}

/**
 * xmlSchemaFreeParserCtxt:
 * @ctxt:  the schema parser context
 *
 * Free the resources associated to the schema parser context
 */
void
xmlSchemaFreeParserCtxt(xmlSchemaParserCtxtPtr ctxt)
{
    if (ctxt == NULL)
        return;
    if (ctxt->doc != NULL && !ctxt->preserve)
        xmlFreeDoc(ctxt->doc);
    if (ctxt->assemble != NULL) {
	xmlFree((xmlSchemaTypePtr *) ctxt->assemble->items);
	xmlFree(ctxt->assemble);
    }
    if (ctxt->vctxt != NULL) {
	xmlSchemaFreeValidCtxt(ctxt->vctxt);
    }
    if (ctxt->localImports != NULL)
	xmlFree((xmlChar *) ctxt->localImports);
    xmlDictFree(ctxt->dict);
    xmlFree(ctxt);
}

/************************************************************************
 *									*
 *			Building the content models			*
 *									*
 ************************************************************************/

/**
 * xmlSchemaBuildAContentModel:
 * @ctxt:  the schema parser context
 * @particle:  the particle component
 * @name:  the complex type's name whose content is being built
 *
 * Generate the automata sequence needed for that type
 */
static void
xmlSchemaBuildAContentModel(xmlSchemaParserCtxtPtr ctxt,
			    xmlSchemaParticlePtr particle,                            
                            const xmlChar * name)
{
    if (particle == NULL) {
	xmlSchemaPErr(ctxt, NULL,
	    XML_SCHEMAP_INTERNAL,
	    "Internal error: xmlSchemaBuildAContentModel, "
	    "particle is NULL.\n", NULL, NULL);
	return;
    }
    if (particle->children == NULL) {
	xmlSchemaPErr(ctxt, GET_NODE(particle),
	    XML_SCHEMAP_INTERNAL,
	    "Internal error: xmlSchemaBuildAContentModel, "
	    "no term on particle.\n", NULL, NULL);
	return;
    }

    switch (particle->children->type) {
	case XML_SCHEMA_TYPE_ANY: {
	    xmlAutomataStatePtr start, end;
	    xmlSchemaWildcardPtr wild;	    
	    xmlSchemaWildcardNsPtr ns;

	    wild = (xmlSchemaWildcardPtr) particle->children;
	    
	    start = ctxt->state;
	    end = xmlAutomataNewState(ctxt->am);
	    
	    if (particle->maxOccurs == 1) {		
		if (wild->any == 1) {
		    /*
		    * We need to add both transitions:
		    *
		    * 1. the {"*", "*"} for elements in a namespace.
		    */		    
		    ctxt->state = 
			xmlAutomataNewTransition2(ctxt->am,
			start, NULL, BAD_CAST "*", BAD_CAST "*", wild);
		    xmlAutomataNewEpsilon(ctxt->am, ctxt->state, end);
		    /*
		    * 2. the {"*"} for elements in no namespace.
		    */
		    ctxt->state = 
			xmlAutomataNewTransition2(ctxt->am,
			start, NULL, BAD_CAST "*", NULL, wild);
		    xmlAutomataNewEpsilon(ctxt->am, ctxt->state, end);

		} else if (wild->nsSet != NULL) {
		    ns = wild->nsSet;
		    do {
			ctxt->state = start;
			ctxt->state = xmlAutomataNewTransition2(ctxt->am,
			    ctxt->state, NULL, BAD_CAST "*", ns->value, wild);
			xmlAutomataNewEpsilon(ctxt->am, ctxt->state, end);
			ns = ns->next;
		    } while (ns != NULL);

		} else if (wild->negNsSet != NULL) {
		    xmlAutomataStatePtr deadEnd;

		    deadEnd = xmlAutomataNewState(ctxt->am);
		    ctxt->state = xmlAutomataNewTransition2(ctxt->am,
			start, deadEnd, BAD_CAST "*", wild->negNsSet->value, wild);
		    ctxt->state = xmlAutomataNewTransition2(ctxt->am,
			start, NULL, BAD_CAST "*", BAD_CAST "*", wild);
		    xmlAutomataNewEpsilon(ctxt->am, ctxt->state, end);
		}		
	    } else {
		int counter;
		xmlAutomataStatePtr hop;
		int maxOccurs = 
		    particle->maxOccurs == UNBOUNDED ? UNBOUNDED : particle->maxOccurs - 1;
		int minOccurs =
		    particle->minOccurs < 1 ? 0 : particle->minOccurs - 1;
		
		counter = xmlAutomataNewCounter(ctxt->am, minOccurs, maxOccurs);
		hop = xmlAutomataNewState(ctxt->am);		
		if (wild->any == 1) {		    
		    ctxt->state =
			xmlAutomataNewTransition2(ctxt->am,
			start, NULL, BAD_CAST "*", BAD_CAST "*", wild);
		    xmlAutomataNewEpsilon(ctxt->am, ctxt->state, hop);
		    ctxt->state = 
			xmlAutomataNewTransition2(ctxt->am,
			start, NULL, BAD_CAST "*", NULL, wild);
		    xmlAutomataNewEpsilon(ctxt->am, ctxt->state, hop);
		} else if (wild->nsSet != NULL) {		    
		    ns = wild->nsSet;
		    do {
			ctxt->state = 
			    xmlAutomataNewTransition2(ctxt->am,
				start, NULL, BAD_CAST "*", ns->value, wild);
			xmlAutomataNewEpsilon(ctxt->am, ctxt->state, hop);
			ns = ns->next;
		    } while (ns != NULL);

		} else if (wild->negNsSet != NULL) {
		    xmlAutomataStatePtr deadEnd;

		    deadEnd = xmlAutomataNewState(ctxt->am);
		    ctxt->state = xmlAutomataNewTransition2(ctxt->am,
			start, deadEnd, BAD_CAST "*", wild->negNsSet->value, wild);
		    ctxt->state = xmlAutomataNewTransition2(ctxt->am,
			start, NULL, BAD_CAST "*", BAD_CAST "*", wild);
		    xmlAutomataNewEpsilon(ctxt->am, ctxt->state, hop);
		}	
		xmlAutomataNewCountedTrans(ctxt->am, hop, start, counter);
		xmlAutomataNewCounterTrans(ctxt->am, hop, end, counter);
	    }
	    if (particle->minOccurs == 0) {
		xmlAutomataNewEpsilon(ctxt->am, start, end);
	    }	    	    				            
	    ctxt->state = end;
            break;
	}
        case XML_SCHEMA_TYPE_ELEMENT:{
		xmlAutomataStatePtr oldstate;
                xmlSchemaElementPtr elemDecl;

		elemDecl = (xmlSchemaElementPtr) particle->children;		

                oldstate = ctxt->state;

                if (particle->maxOccurs >= UNBOUNDED) {
                    if (particle->minOccurs > 1) {
                        xmlAutomataStatePtr tmp;
                        int counter;

                        ctxt->state = xmlAutomataNewEpsilon(ctxt->am,
			    oldstate, NULL);
                        oldstate = ctxt->state;
                        counter = xmlAutomataNewCounter(ctxt->am,
			    particle->minOccurs - 1, UNBOUNDED);                      
                        ctxt->state =
			    xmlAutomataNewTransition2(ctxt->am,
				ctxt->state, NULL, 
				elemDecl->name, 
				elemDecl->targetNamespace,
				(xmlSchemaTypePtr) elemDecl);
                        tmp = ctxt->state;
                        xmlAutomataNewCountedTrans(ctxt->am, tmp, oldstate,
			    counter);
                        ctxt->state =
                            xmlAutomataNewCounterTrans(ctxt->am, tmp, NULL,
				counter);

                    } else {                        
			ctxt->state =
			    xmlAutomataNewTransition2(ctxt->am,
			    ctxt->state, NULL,
			    elemDecl->name, 
			    elemDecl->targetNamespace,
			    (xmlSchemaTypePtr) elemDecl);
                        xmlAutomataNewEpsilon(ctxt->am, ctxt->state,
                                              oldstate);
                        if (particle->minOccurs == 0) {
                            /* basically an elem* */
                            xmlAutomataNewEpsilon(ctxt->am, oldstate,
                                                  ctxt->state);
                        }
                    }
                } else if ((particle->maxOccurs > 1) || (particle->minOccurs > 1)) {
                    xmlAutomataStatePtr tmp;
                    int counter;

                    ctxt->state = xmlAutomataNewEpsilon(ctxt->am,
                                                        oldstate, NULL);
                    oldstate = ctxt->state;
                    counter = xmlAutomataNewCounter(ctxt->am,
			particle->minOccurs - 1,
			particle->maxOccurs - 1);
                    ctxt->state = xmlAutomataNewTransition2(ctxt->am,
			ctxt->state,
			NULL,
			elemDecl->name,
			elemDecl->targetNamespace,
			(xmlSchemaTypePtr) elemDecl);
                    tmp = ctxt->state;
                    xmlAutomataNewCountedTrans(ctxt->am, tmp, oldstate,
			counter);
                    ctxt->state = xmlAutomataNewCounterTrans(ctxt->am, tmp,
			NULL, counter);
                    if (particle->minOccurs == 0) {
                        /* basically an elem? */
                        xmlAutomataNewEpsilon(ctxt->am, oldstate,
			    ctxt->state);
                    }

                } else {                    
		    ctxt->state = xmlAutomataNewTransition2(ctxt->am,
			ctxt->state,
			NULL,
			elemDecl->name,
			elemDecl->targetNamespace,
			(xmlSchemaTypePtr) elemDecl);
                    if (particle->minOccurs == 0) {
                        /* basically an elem? */
                        xmlAutomataNewEpsilon(ctxt->am, oldstate,
			    ctxt->state);
                    }
                }
                break;
            }
        case XML_SCHEMA_TYPE_SEQUENCE:{
                xmlSchemaTreeItemPtr sub;

                /*
                 * If max and min occurances are default (1) then
                 * simply iterate over the particles of the <sequence>.
                 */
                if ((particle->minOccurs == 1) && (particle->maxOccurs == 1)) {
                    sub = particle->children->children;
                    while (sub != NULL) {
                        xmlSchemaBuildAContentModel(ctxt,
			    (xmlSchemaParticlePtr) sub, name);
                        sub = sub->next;
                    }
                } else {
                    xmlAutomataStatePtr oldstate = ctxt->state;

                    if (particle->maxOccurs >= UNBOUNDED) {
                        if (particle->minOccurs > 1) {
                            xmlAutomataStatePtr tmp;
                            int counter;

                            ctxt->state = xmlAutomataNewEpsilon(ctxt->am,
				oldstate, NULL);
                            oldstate = ctxt->state;

                            counter = xmlAutomataNewCounter(ctxt->am,
				particle->minOccurs - 1, UNBOUNDED);

                            sub = particle->children->children;
                            while (sub != NULL) {
                                xmlSchemaBuildAContentModel(ctxt,
				    (xmlSchemaParticlePtr) sub, name);
                                sub = sub->next;
                            }
                            tmp = ctxt->state;
                            xmlAutomataNewCountedTrans(ctxt->am, tmp,
                                                       oldstate, counter);
                            ctxt->state =
                                xmlAutomataNewCounterTrans(ctxt->am, tmp,
                                                           NULL, counter);

                        } else {
			    sub = particle->children->children;
                            while (sub != NULL) {
                                xmlSchemaBuildAContentModel(ctxt,
				    (xmlSchemaParticlePtr) sub, name);
                                sub = sub->next;
                            }
                            xmlAutomataNewEpsilon(ctxt->am, ctxt->state,
                                                  oldstate);
                            if (particle->minOccurs == 0) {
                                xmlAutomataNewEpsilon(ctxt->am,
				    oldstate, ctxt->state);
                            }
                        }
                    } else if ((particle->maxOccurs > 1)
                               || (particle->minOccurs > 1)) {
                        xmlAutomataStatePtr tmp;
                        int counter;

                        ctxt->state = xmlAutomataNewEpsilon(ctxt->am,
			    oldstate, NULL);
                        oldstate = ctxt->state;

                        counter = xmlAutomataNewCounter(ctxt->am,
			    particle->minOccurs - 1,
			    particle->maxOccurs - 1);

                        sub = particle->children->children;
                        while (sub != NULL) {
                            xmlSchemaBuildAContentModel(ctxt,
				(xmlSchemaParticlePtr) sub, name);
                            sub = sub->next;
                        }
                        tmp = ctxt->state;
                        xmlAutomataNewCountedTrans(ctxt->am,
			    tmp, oldstate, counter);
                        ctxt->state =
                            xmlAutomataNewCounterTrans(ctxt->am, tmp, NULL,
                                                       counter);
                        if (particle->minOccurs == 0) {
                            xmlAutomataNewEpsilon(ctxt->am,
				oldstate, ctxt->state);
                        }
                    } else {
                        sub = particle->children->children;
                        while (sub != NULL) {
                            xmlSchemaBuildAContentModel(ctxt,
				(xmlSchemaParticlePtr) sub, name);
                            sub = sub->next;
                        }
                        if (particle->minOccurs == 0) {
                            xmlAutomataNewEpsilon(ctxt->am, oldstate,
                                                  ctxt->state);
                        }
                    }
                }
                break;
            }
        case XML_SCHEMA_TYPE_CHOICE:{
                xmlSchemaTreeItemPtr sub;
                xmlAutomataStatePtr start, end;

                start = ctxt->state;
                end = xmlAutomataNewState(ctxt->am);

                /*
                 * iterate over the subtypes and remerge the end with an
                 * epsilon transition
                 */
                if (particle->maxOccurs == 1) {
		    sub = particle->children->children;
                    while (sub != NULL) {
                        ctxt->state = start;
                        xmlSchemaBuildAContentModel(ctxt,
			    (xmlSchemaParticlePtr) sub, name);
                        xmlAutomataNewEpsilon(ctxt->am, ctxt->state, end);
                        sub = sub->next;
                    }
                } else {
                    int counter;
                    xmlAutomataStatePtr hop;
                    int maxOccurs = particle->maxOccurs == UNBOUNDED ?
                        UNBOUNDED : particle->maxOccurs - 1;
                    int minOccurs =
                        particle->minOccurs < 1 ? 0 : particle->minOccurs - 1;

                    /*
                     * use a counter to keep track of the number of transtions
                     * which went through the choice.
                     */
                    counter =
                        xmlAutomataNewCounter(ctxt->am, minOccurs,
                                              maxOccurs);
                    hop = xmlAutomataNewState(ctxt->am);

		    sub = particle->children->children;
                    while (sub != NULL) {
                        ctxt->state = start;
                        xmlSchemaBuildAContentModel(ctxt,
			    (xmlSchemaParticlePtr) sub, name);
                        xmlAutomataNewEpsilon(ctxt->am, ctxt->state, hop);
                        sub = sub->next;
                    }
                    xmlAutomataNewCountedTrans(ctxt->am, hop, start,
                                               counter);
                    xmlAutomataNewCounterTrans(ctxt->am, hop, end,
                                               counter);
                }
                if (particle->minOccurs == 0) {
                    xmlAutomataNewEpsilon(ctxt->am, start, end);
                }
                ctxt->state = end;
                break;
            }
        case XML_SCHEMA_TYPE_ALL:{
                xmlAutomataStatePtr start;
		xmlSchemaParticlePtr sub;
		xmlSchemaElementPtr elemDecl;
                int lax;
                
		sub = (xmlSchemaParticlePtr) particle->children->children;
                if (sub == NULL)
                    break;
                start = ctxt->state;
                while (sub != NULL) {
                    ctxt->state = start;

		    elemDecl = (xmlSchemaElementPtr) sub->children;
		    if (elemDecl == NULL) {
			xmlSchemaPErr(ctxt, NULL,
			    XML_SCHEMAP_INTERNAL,
			    "Internal error: xmlSchemaBuildAContentModel, "
			    "<element> particle a NULL term.\n", NULL, NULL);
			return;
		    };                    		  
		    /*
		    * NOTE: The {max occurs} of all the particles in the 
		    * {particles} of the group must be 0 or 1; this is
		    * already ensured during the parse of the content of
		    * <all>.
		    */                    
                    if ((sub->minOccurs == 1) &&
			(sub->maxOccurs == 1)) {
                        xmlAutomataNewOnceTrans2(ctxt->am, ctxt->state,
                                                ctxt->state, 
						elemDecl->name, 
						elemDecl->targetNamespace,
						1, 1, elemDecl);
                    } else if ((sub->minOccurs == 0) &&
			(sub->maxOccurs == 1)) {
			
                        xmlAutomataNewCountTrans2(ctxt->am, ctxt->state,
                                                 ctxt->state, 
						 elemDecl->name,
						 elemDecl->targetNamespace,
                                                 0,
                                                 1,
                                                 elemDecl);
                    }
                    sub = (xmlSchemaParticlePtr) sub->next;
                }
                lax = particle->minOccurs == 0;
                ctxt->state =
                    xmlAutomataNewAllTrans(ctxt->am, ctxt->state, NULL, lax);
                break;
            }
        default:
            xmlGenericError(xmlGenericErrorContext,
		"Internal error: xmlSchemaBuildAContentModel, found "
		"unexpected term of type %d in content model of complex "
		"type '%s'.\n",
		particle->children->type, name);
            return;
    }
}

/**
 * xmlSchemaBuildContentModel:
 * @ctxt:  the schema parser context
 * @type:  the complex type definition 
 * @name:  the element name
 *
 * Builds the content model of the complex type.
 */
static void
xmlSchemaBuildContentModel(xmlSchemaTypePtr type,
			   xmlSchemaParserCtxtPtr ctxt,
                           const xmlChar * name)
{
    xmlAutomataStatePtr start;

    if ((type->type != XML_SCHEMA_TYPE_COMPLEX) ||
	(type->contModel != NULL) ||
	((type->contentType != XML_SCHEMA_CONTENT_ELEMENTS) &&
	(type->contentType != XML_SCHEMA_CONTENT_MIXED)))
	return;

#ifdef DEBUG_CONTENT
    xmlGenericError(xmlGenericErrorContext,
                    "Building content model for %s\n", name);
#endif

    ctxt->am = xmlNewAutomata();
    if (ctxt->am == NULL) {
        xmlGenericError(xmlGenericErrorContext,
	    "Cannot create automata for complex type %s\n", name);
        return;
    }
    start = ctxt->state = xmlAutomataGetInitState(ctxt->am);
    xmlSchemaBuildAContentModel(ctxt, (xmlSchemaParticlePtr) type->subtypes, name);    
    xmlAutomataSetFinalState(ctxt->am, ctxt->state);
    type->contModel = xmlAutomataCompile(ctxt->am);
    if (type->contModel == NULL) {
        xmlSchemaPCustomErr(ctxt, 
	    XML_SCHEMAP_INTERNAL, 
	    NULL, type, type->node,	    
	    "Failed to compile the content model", NULL);
    } else if (xmlRegexpIsDeterminist(type->contModel) != 1) {
        xmlSchemaPCustomErr(ctxt, 
	    XML_SCHEMAP_NOT_DETERMINISTIC,
	    /* XML_SCHEMAS_ERR_NOTDETERMINIST, */
	    NULL, type, type->node,
	    "The content model is not determinist", NULL);
    } else {
#ifdef DEBUG_CONTENT_REGEXP
        xmlGenericError(xmlGenericErrorContext,
                        "Content model of %s:\n", type->name);
        xmlRegexpPrint(stderr, type->contModel);
#endif
    }
    ctxt->state = NULL;
    xmlFreeAutomata(ctxt->am);
    ctxt->am = NULL;
}

/**
 * xmlSchemaElementFixup:
 * @elem:  the schema element context
 * @ctxt:  the schema parser context
 *
 * Resolves the references of an element declaration
 * or particle, which has an element declaration as it's
 * term. 
 */
static void
xmlSchemaElementFixup(xmlSchemaElementPtr elem,
                          xmlSchemaParserCtxtPtr ctxt,
                          const xmlChar * name ATTRIBUTE_UNUSED,
                          const xmlChar * context ATTRIBUTE_UNUSED,
                          const xmlChar * namespace ATTRIBUTE_UNUSED)
{
    if ((ctxt == NULL) || (elem == NULL) || 
	((elem != NULL) && (elem->flags & XML_SCHEMAS_ELEM_INTERNAL_RESOLVED)))
        return;
    elem->flags |= XML_SCHEMAS_ELEM_INTERNAL_RESOLVED;
    
    if ((elem->subtypes == NULL) && (elem->namedType != NULL)) {
	xmlSchemaTypePtr type;
	
	/* (type definition) ... otherwise the type definition �resolved� 
	* to by the �actual value� of the type [attribute] ...
	*/	    	    
	type = xmlSchemaGetType(ctxt->schema, elem->namedType,
	    elem->namedTypeNs);	    
	if (type == NULL) {	
	    xmlSchemaPResCompAttrErr(ctxt,
		XML_SCHEMAP_SRC_RESOLVE,
		NULL, (xmlSchemaTypePtr) elem, elem->node,
		"type", elem->namedType, elem->namedTypeNs,
		XML_SCHEMA_TYPE_BASIC, "type definition");
	} else
	    elem->subtypes = type;
    }
    if (elem->substGroup != NULL) {
	xmlSchemaElementPtr substHead;
	
	/*
	* FIXME TODO: Do we need a new field in _xmlSchemaElement for 
	* substitutionGroup?
	*/
	substHead = xmlSchemaGetElem(ctxt->schema, elem->substGroup, 
	    elem->substGroupNs);	    
	if (substHead == NULL) {
	    xmlSchemaPResCompAttrErr(ctxt,
		XML_SCHEMAP_SRC_RESOLVE,
		NULL, (xmlSchemaTypePtr) elem, NULL,
		"substitutionGroup", elem->substGroup, elem->substGroupNs,
		XML_SCHEMA_TYPE_ELEMENT, NULL);
	} else {
	    xmlSchemaElementFixup(substHead, ctxt, NULL, NULL, NULL);
	    /*
	    * (type definition)...otherwise the {type definition} of the 
	    * element declaration �resolved� to by the �actual value� of 
	    * the substitutionGroup [attribute], if present
	    */
	    if (elem->subtypes == NULL) 
		elem->subtypes = substHead->subtypes;
	}
    }
    if ((elem->subtypes == NULL) && (elem->namedType == NULL) &&
	(elem->substGroup == NULL))
	elem->subtypes = xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYTYPE);
}

/**
 * xmlSchemaResolveUnionMemberTypes:
 * @ctxt:  the schema parser context
 * @type:  the schema simple type definition
 *
 * Checks and builds the memberTypes of the union simple type.
 * Returns -1 in case of an internal error, 0 otherwise.
 */
static int
xmlSchemaResolveUnionMemberTypes(xmlSchemaParserCtxtPtr ctxt,
				 xmlSchemaTypePtr type)
{
    
    xmlSchemaTypeLinkPtr link, lastLink, prevLink, subLink, newLink;
    xmlSchemaTypePtr memberType;

    /* 1 If the <union> alternative is chosen, then [Definition:]  
    * define the explicit members as the type definitions �resolved� 
    * to by the items in the �actual value� of the memberTypes [attribute], 
    * if any, followed by the type definitions corresponding to the 
    * <simpleType>s among the [children] of <union>, if any. 
    */
    /*
    * Resolve references.
    */
    link = type->memberTypes; 
    lastLink = NULL;
    while (link != NULL) {
	const xmlChar *name, *nsName;

	name = ((xmlSchemaQNameRefPtr) link->type)->name;
	nsName = ((xmlSchemaQNameRefPtr) link->type)->targetNamespace;

	memberType = xmlSchemaGetType(ctxt->schema, name, nsName);
	if ((memberType == NULL) || (! IS_SIMPLE_TYPE(memberType))) {
	    xmlSchemaPResCompAttrErr(ctxt, XML_SCHEMAP_SRC_RESOLVE,
		NULL, type, type->node, "memberTypes",
		name, nsName, XML_SCHEMA_TYPE_SIMPLE, NULL);
	    /*
	    * Remove the member type link.
	    */
	    if (lastLink == NULL)
		type->memberTypes = link->next;
	    else
		lastLink->next = link->next;
	    newLink = link;
	    link = link->next;
	    xmlFree(newLink);
	} else {
	    link->type = memberType;
	    if (IS_NOT_TYPEFIXED(memberType))
		xmlSchemaTypeFixup(memberType, ctxt, NULL);
	    
	    lastLink = link;
	    link = link->next;
	}		
    }  
    /*
    * Add local simple types,
    */    
    memberType = type->subtypes;
    while (memberType != NULL) {	
	link = (xmlSchemaTypeLinkPtr) xmlMalloc(sizeof(xmlSchemaTypeLink));
	if (link == NULL) {
	    xmlSchemaPErrMemory(ctxt, "allocating a type link", NULL);
	    return (-1);
	}
	if (IS_NOT_TYPEFIXED(memberType))
	    xmlSchemaTypeFixup(memberType, ctxt, NULL);
	link->type = memberType;
	link->next = NULL;
	if (lastLink == NULL)
	    type->memberTypes = link;		    
	else 
	    lastLink->next = link;
	lastLink = link;
	memberType = memberType->next;
    }    
    /*
    * The actual value is then formed by replacing any union type 
    * definition in the �explicit members� with the members of their 
    * {member type definitions}, in order.
    */
    link = type->memberTypes;
    while (link != NULL) {
	/* TODO: type-fixup it. */
	if (link->type->flags & XML_SCHEMAS_TYPE_VARIETY_UNION) {
	    subLink = xmlSchemaGetUnionSimpleTypeMemberTypes(link->type);
	    if (subLink != NULL) {		
		link->type = subLink->type;
		if (subLink->next != NULL) {
		    lastLink = link->next;
		    subLink = subLink->next;		
		    prevLink = link;
		    while (subLink != NULL) {		    
			newLink = (xmlSchemaTypeLinkPtr) 
			    xmlMalloc(sizeof(xmlSchemaTypeLink));
			if (newLink == NULL) {
			    xmlSchemaPErrMemory(ctxt, "allocating a type link", 
				NULL);
			    return (-1);
			}
			newLink->type = memberType;	    
			prevLink->next = newLink;
			prevLink = newLink;
			newLink->next = lastLink;
			
			subLink = subLink->next;
		    }
		}
	    }
	}
	link = link->next;
    }    

    return (0);
}

/**
 * xmlSchemaIsDerivedFromBuiltInType:
 * @ctxt:  the schema parser context
 * @type:  the type definition
 * @valType: the value type
 * 
 *
 * Returns 1 if the type has the given value type, or
 * is derived from such a type.
 */
static int
xmlSchemaIsDerivedFromBuiltInType(xmlSchemaParserCtxtPtr ctxt, 
				  xmlSchemaTypePtr type, int valType)
{
    /* TODO: Check if this works in every case. */
    if ((type->type == XML_SCHEMA_TYPE_BASIC) &&
		(type->contentType == XML_SCHEMA_CONTENT_BASIC)) {
		if (type->builtInType == valType)
			return(1);
    } else if (type->type == XML_SCHEMA_TYPE_ATTRIBUTE) {
	if (((xmlSchemaAttributePtr) type)->subtypes != NULL) 
	    return(xmlSchemaIsDerivedFromBuiltInType(ctxt, 
		((xmlSchemaAttributePtr) type)->subtypes, valType));
    } else if ((type->type == XML_SCHEMA_TYPE_RESTRICTION) ||
	(type->type == XML_SCHEMA_TYPE_EXTENSION)) {
	if (type->baseType != NULL) 
	    return(xmlSchemaIsDerivedFromBuiltInType(ctxt, type->baseType, 
		valType));
    } else if ((type->subtypes != NULL) &&
	((type->subtypes->type == XML_SCHEMA_TYPE_COMPLEX) ||
	 (type->subtypes->type == XML_SCHEMA_TYPE_COMPLEX_CONTENT) ||
	 (type->subtypes->type == XML_SCHEMA_TYPE_SIMPLE) ||
	 (type->subtypes->type == XML_SCHEMA_TYPE_SIMPLE_CONTENT))) {
	return(xmlSchemaIsDerivedFromBuiltInType(ctxt, type->subtypes, 
	    valType));
    }

    return (0);
}

/**
 * xmlSchemaGetPrimitiveType:
 * @type:  the simpleType definition
 *
 * Returns the primitive type of the given type or
 * NULL in case of error.
 */
static xmlSchemaTypePtr
xmlSchemaGetPrimitiveType(xmlSchemaTypePtr type)
{

    while (type != NULL) {
	/*
	* Note that anySimpleType is actually not a primitive type
	* but we need that here.
	*/
	if ((type->builtInType == XML_SCHEMAS_ANYSIMPLETYPE) ||
	   (type->flags & XML_SCHEMAS_TYPE_BUILTIN_PRIMITIVE))
	    return (type);
	type = type->baseType;
    }

    return (NULL);
}

#if 0
/**
 * xmlSchemaGetBuiltInTypeAncestor:
 * @type:  the simpleType definition
 *
 * Returns the primitive type of the given type or
 * NULL in case of error.
 */
static xmlSchemaTypePtr
xmlSchemaGetBuiltInTypeAncestor(xmlSchemaTypePtr type)
{
    if ((type->flags & XML_SCHEMAS_TYPE_VARIETY_LIST) ||
	(type->flags & XML_SCHEMAS_TYPE_VARIETY_UNION))
	return (0);
    while (type != NULL) {
	if (type->type == XML_SCHEMA_TYPE_BASIC)
	    return (type);
	type = type->baseType;
    }

    return (NULL);
}
#endif

/**
 * xmlSchemaBuildAttributeUsesOwned:
 * @ctxt:  the schema parser context
 * @type:  the complex type definition
 * @cur: the attribute declaration list
 * @lastUse: the top of the attribute use list
 *
 * Builds the attribute uses list on the given complex type.
 * This one is supposed to be called by 
 * xmlSchemaBuildAttributeValidation only.
 */
static int
xmlSchemaBuildAttributeUsesOwned(xmlSchemaParserCtxtPtr ctxt, 
				 xmlSchemaAttributePtr cur,
				 xmlSchemaAttributeLinkPtr *uses,
				 xmlSchemaAttributeLinkPtr *lastUse)
{
    xmlSchemaAttributeLinkPtr tmp;
    while (cur != NULL) {
	if (cur->type == XML_SCHEMA_TYPE_ATTRIBUTEGROUP) {
	    /* 
	     * W3C: "2 The {attribute uses} of the attribute groups �resolved� 
	     * to by the �actual value�s of the ref [attribute] of the 
	     * <attributeGroup> [children], if any."
	     */
	    if (xmlSchemaBuildAttributeUsesOwned(ctxt, 
		((xmlSchemaAttributeGroupPtr) cur)->attributes, uses, 
		lastUse) == -1) {
		return (-1);	    
	    }
	} else {
	    /* W3C: "1 The set of attribute uses corresponding to the 
	     * <attribute> [children], if any."
	     */	    	    
	    tmp = (xmlSchemaAttributeLinkPtr) 
		xmlMalloc(sizeof(xmlSchemaAttributeLink));
	    if (tmp == NULL) {
		xmlSchemaPErrMemory(ctxt, "building attribute uses", NULL);
		return (-1);
	    }
	    tmp->attr = cur;
	    tmp->next = NULL;
	    if (*uses == NULL)
		*uses = tmp;		    
	    else 
		(*lastUse)->next = tmp;
	    *lastUse = tmp;	    
	}	
	cur = cur->next;
    }	
    return (0);
}

/**
 * xmlSchemaCloneWildcardNsConstraints:
 * @ctxt:  the schema parser context
 * @dest:  the destination wildcard
 * @source: the source wildcard
 *
 * Clones the namespace constraints of source
 * and assignes them to dest.
 * Returns -1 on internal error, 0 otherwise.
 */
static int
xmlSchemaCloneWildcardNsConstraints(xmlSchemaParserCtxtPtr ctxt,
				    xmlSchemaWildcardPtr *dest,
				    xmlSchemaWildcardPtr source)				    
{
    xmlSchemaWildcardNsPtr cur, tmp, last;

    if ((source == NULL) || (*dest == NULL))
	return(-1);    
    (*dest)->any = source->any;
    cur = source->nsSet;
    last = NULL;
    while (cur != NULL) {
	tmp = xmlSchemaNewWildcardNsConstraint(ctxt);
	if (tmp == NULL)
	    return(-1);
	tmp->value = cur->value;
	if (last == NULL)
	    (*dest)->nsSet = tmp;
	else 
	    last->next = tmp;
	last = tmp;
	cur = cur->next;
    }    
    if ((*dest)->negNsSet != NULL)
	xmlSchemaFreeWildcardNsSet((*dest)->negNsSet);	   
    if (source->negNsSet != NULL) {
	(*dest)->negNsSet = xmlSchemaNewWildcardNsConstraint(ctxt);
	if ((*dest)->negNsSet == NULL)
	    return(-1);
	(*dest)->negNsSet->value = source->negNsSet->value;	    
    } else
	(*dest)->negNsSet = NULL;
    return(0);
}

/**
 * xmlSchemaUnionWildcards:
 * @ctxt:  the schema parser context
 * @completeWild:  the first wildcard
 * @curWild: the second wildcard 
 *
 * Unions the namespace constraints of the given wildcards.
 * @completeWild will hold the resulting union.
 * Returns a positive error code on failure, -1 in case of an
 * internal error, 0 otherwise.
 */
static int
xmlSchemaUnionWildcards(xmlSchemaParserCtxtPtr ctxt, 			    
			    xmlSchemaWildcardPtr completeWild,
			    xmlSchemaWildcardPtr curWild)
{
    xmlSchemaWildcardNsPtr cur, curB, tmp;

    /*
    * 1 If O1 and O2 are the same value, then that value must be the 
    * value.
    */
    if ((completeWild->any == curWild->any) &&
	((completeWild->nsSet == NULL) == (curWild->nsSet == NULL)) &&
	((completeWild->negNsSet == NULL) == (curWild->negNsSet == NULL))) {
	
	if ((completeWild->negNsSet == NULL) ||
	    (completeWild->negNsSet->value == curWild->negNsSet->value)) {
	    
	    if (completeWild->nsSet != NULL) {
		int found = 0;
		
		/* 
		* Check equality of sets. 
		*/
		cur = completeWild->nsSet;
		while (cur != NULL) {
		    found = 0;
		    curB = curWild->nsSet;
		    while (curB != NULL) {
			if (cur->value == curB->value) {
			    found = 1;
			    break;
			}
			curB = curB->next;
		    }
		    if (!found)
			break;
		    cur = cur->next;
		}
		if (found)
		    return(0);
	    } else
		return(0);
	}
    }	        
    /*
    * 2 If either O1 or O2 is any, then any must be the value
    */
    if (completeWild->any != curWild->any) {	
	if (completeWild->any == 0) {
	    completeWild->any = 1;
	    if (completeWild->nsSet != NULL) {
		xmlSchemaFreeWildcardNsSet(completeWild->nsSet);
		completeWild->nsSet = NULL;
	    }
	    if (completeWild->negNsSet != NULL) {
		xmlFree(completeWild->negNsSet);
		completeWild->negNsSet = NULL;
	    }
	}
	return (0);
    }
    /*
    * 3 If both O1 and O2 are sets of (namespace names or �absent�), 
    * then the union of those sets must be the value.
    */
    if ((completeWild->nsSet != NULL) && (curWild->nsSet != NULL)) {		
	int found;
	xmlSchemaWildcardNsPtr start;
	
	cur = curWild->nsSet;
	start = completeWild->nsSet;
	while (cur != NULL) {
	    found = 0;
	    curB = start;
	    while (curB != NULL) {
		if (cur->value == curB->value) {
		    found = 1;
		    break;
		}
		curB = curB->next;
	    }
	    if (!found) {
		tmp = xmlSchemaNewWildcardNsConstraint(ctxt);
		if (tmp == NULL) 
		    return (-1);
		tmp->value = cur->value;
		tmp->next = completeWild->nsSet;		    		    
		completeWild->nsSet = tmp;
	    }
	    cur = cur->next;
	}	
		    		
	return(0);
    }    
    /*
    * 4 If the two are negations of different values (namespace names 
    * or �absent�), then a pair of not and �absent� must be the value.
    */
    if ((completeWild->negNsSet != NULL) && 
	(curWild->negNsSet != NULL) &&
	(completeWild->negNsSet->value != curWild->negNsSet->value)) {
	completeWild->negNsSet->value = NULL;

	return(0);
    }
    /* 
     * 5.
     */
    if (((completeWild->negNsSet != NULL) && 
	(completeWild->negNsSet->value != NULL) &&
	(curWild->nsSet != NULL)) ||
	((curWild->negNsSet != NULL) && 
	(curWild->negNsSet->value != NULL) &&
	(completeWild->nsSet != NULL))) {

	int nsFound, absentFound = 0;
	
	if (completeWild->nsSet != NULL) {
	    cur = completeWild->nsSet;
	    curB = curWild->negNsSet;
	} else {
	    cur = curWild->nsSet;
	    curB = completeWild->negNsSet;
	}
	nsFound = 0;
	while (cur != NULL) {
	    if (cur->value == NULL) 
		absentFound = 1;
	    else if (cur->value == curB->value)
		nsFound = 1;
	    if (nsFound && absentFound)
		break;
	    cur = cur->next;
	}	

	if (nsFound && absentFound) {
	    /*
	    * 5.1 If the set S includes both the negated namespace 
	    * name and �absent�, then any must be the value.
	    */    
	    completeWild->any = 1;
	    if (completeWild->nsSet != NULL) {
		xmlSchemaFreeWildcardNsSet(completeWild->nsSet);
		completeWild->nsSet = NULL;
	    }
	    if (completeWild->negNsSet != NULL) {
		xmlFree(completeWild->negNsSet);
		completeWild->negNsSet = NULL;
	    }
	} else if (nsFound && (!absentFound)) {
	    /* 
	    * 5.2 If the set S includes the negated namespace name 
	    * but not �absent�, then a pair of not and �absent� must 
	    * be the value.
	    */
	    if (completeWild->nsSet != NULL) {
		xmlSchemaFreeWildcardNsSet(completeWild->nsSet);
		completeWild->nsSet = NULL;
	    }
	    if (completeWild->negNsSet == NULL) {
		completeWild->negNsSet = xmlSchemaNewWildcardNsConstraint(ctxt);
		if (completeWild->negNsSet == NULL)
		    return (-1);
	    }
	    completeWild->negNsSet->value = NULL;
	} else if ((!nsFound) && absentFound) {
	    /*
	    * 5.3 If the set S includes �absent� but not the negated 
	    * namespace name, then the union is not expressible.
	    */
	    xmlSchemaPErr(ctxt, completeWild->node, 
		XML_SCHEMAP_UNION_NOT_EXPRESSIBLE,
		"The union of the wilcard is not expressible.\n",
		NULL, NULL);	
	    return(XML_SCHEMAP_UNION_NOT_EXPRESSIBLE);
	} else if ((!nsFound) && (!absentFound)) {
	    /* 
	    * 5.4 If the set S does not include either the negated namespace 
	    * name or �absent�, then whichever of O1 or O2 is a pair of not 
	    * and a namespace name must be the value.
	    */
	    if (completeWild->negNsSet == NULL) {
		if (completeWild->nsSet != NULL) {
		    xmlSchemaFreeWildcardNsSet(completeWild->nsSet);
		    completeWild->nsSet = NULL;
		}
		completeWild->negNsSet = xmlSchemaNewWildcardNsConstraint(ctxt);
		if (completeWild->negNsSet == NULL)
		    return (-1);
		completeWild->negNsSet->value = curWild->negNsSet->value;
	    }
	}
	return (0);
    }
    /* 
     * 6.
     */
    if (((completeWild->negNsSet != NULL) && 
	(completeWild->negNsSet->value == NULL) &&
	(curWild->nsSet != NULL)) ||
	((curWild->negNsSet != NULL) && 
	(curWild->negNsSet->value == NULL) &&
	(completeWild->nsSet != NULL))) {

	if (completeWild->nsSet != NULL) {
	    cur = completeWild->nsSet;
	} else {
	    cur = curWild->nsSet;
	}	
	while (cur != NULL) {
	    if (cur->value == NULL) {
		/*
		* 6.1 If the set S includes �absent�, then any must be the 
		* value.
		*/
		completeWild->any = 1;
		if (completeWild->nsSet != NULL) {
		    xmlSchemaFreeWildcardNsSet(completeWild->nsSet);
		    completeWild->nsSet = NULL;
		}
		if (completeWild->negNsSet != NULL) {
		    xmlFree(completeWild->negNsSet);
		    completeWild->negNsSet = NULL;
		}
		return (0);
	    }
	    cur = cur->next;
	}			
	if (completeWild->negNsSet == NULL) {
	    /*
	    * 6.2 If the set S does not include �absent�, then a pair of not 
	    * and �absent� must be the value.
	    */
	    if (completeWild->nsSet != NULL) {
		xmlSchemaFreeWildcardNsSet(completeWild->nsSet);
		completeWild->nsSet = NULL;
	    }
	    completeWild->negNsSet = xmlSchemaNewWildcardNsConstraint(ctxt);
	    if (completeWild->negNsSet == NULL)
		return (-1);
	    completeWild->negNsSet->value = NULL;
	}
	return (0);
    }
    return (0);

}

/**
 * xmlSchemaIntersectWildcards:
 * @ctxt:  the schema parser context
 * @completeWild:  the first wildcard
 * @curWild: the second wildcard 
 *
 * Intersects the namespace constraints of the given wildcards.
 * @completeWild will hold the resulting intersection.
 * Returns a positive error code on failure, -1 in case of an
 * internal error, 0 otherwise.
 */
static int
xmlSchemaIntersectWildcards(xmlSchemaParserCtxtPtr ctxt, 			    
			    xmlSchemaWildcardPtr completeWild,
			    xmlSchemaWildcardPtr curWild)
{
    xmlSchemaWildcardNsPtr cur, curB, prev,  tmp;

    /*
    * 1 If O1 and O2 are the same value, then that value must be the 
    * value.
    */
    if ((completeWild->any == curWild->any) &&
	((completeWild->nsSet == NULL) == (curWild->nsSet == NULL)) &&
	((completeWild->negNsSet == NULL) == (curWild->negNsSet == NULL))) {
	
	if ((completeWild->negNsSet == NULL) ||
	    (completeWild->negNsSet->value == curWild->negNsSet->value)) {
	    
	    if (completeWild->nsSet != NULL) {
		int found = 0;
		
		/* 
		* Check equality of sets. 
		*/
		cur = completeWild->nsSet;
		while (cur != NULL) {
		    found = 0;
		    curB = curWild->nsSet;
		    while (curB != NULL) {
			if (cur->value == curB->value) {
			    found = 1;
			    break;
			}
			curB = curB->next;
		    }
		    if (!found)
			break;
		    cur = cur->next;
		}
		if (found)
		    return(0);
	    } else
		return(0);
	}
    }	        
    /*
    * 2 If either O1 or O2 is any, then the other must be the value.
    */
    if ((completeWild->any != curWild->any) && (completeWild->any)) {		    
	if (xmlSchemaCloneWildcardNsConstraints(ctxt, &completeWild, curWild) == -1)
	    return(-1);	    
	return(0);
    }	            
    /*
    * 3 If either O1 or O2 is a pair of not and a value (a namespace 
    * name or �absent�) and the other is a set of (namespace names or 
    * �absent�), then that set, minus the negated value if it was in 
    * the set, minus �absent� if it was in the set, must be the value.
    */
    if (((completeWild->negNsSet != NULL) && (curWild->nsSet != NULL)) ||
	((curWild->negNsSet != NULL) && (completeWild->nsSet != NULL))) {
	const xmlChar *neg;
	
	if (completeWild->nsSet == NULL) {
	    neg = completeWild->negNsSet->value;
	    if (xmlSchemaCloneWildcardNsConstraints(ctxt, &completeWild, curWild) == -1)
		return(-1);
	} else
	    neg = curWild->negNsSet->value;
	/*
	* Remove absent and negated.
	*/
	prev = NULL;
	cur = completeWild->nsSet;
	while (cur != NULL) {
	    if (cur->value == NULL) {
		if (prev == NULL) 
		    completeWild->nsSet = cur->next;
		else 
		    prev->next = cur->next;
		xmlFree(cur);
		break;
	    }
	    prev = cur;
	    cur = cur->next;
	}
	if (neg != NULL) {
	    prev = NULL;
	    cur = completeWild->nsSet;
	    while (cur != NULL) {
		if (cur->value == neg) {
		    if (prev == NULL) 
			completeWild->nsSet = cur->next;
		    else 
			prev->next = cur->next;
		    xmlFree(cur);
		    break;
		}
		prev = cur;
		cur = cur->next;
	    }
	}

	return(0);
    }	        
    /*
    * 4 If both O1 and O2 are sets of (namespace names or �absent�), 
    * then the intersection of those sets must be the value.
    */
    if ((completeWild->nsSet != NULL) && (curWild->nsSet != NULL)) {		
	int found;
	
	cur = completeWild->nsSet;
	prev = NULL;
	while (cur != NULL) {
	    found = 0;
	    curB = curWild->nsSet;
	    while (curB != NULL) {
		if (cur->value == curB->value) {
		    found = 1;
		    break;
		}
		curB = curB->next;
	    }
	    if (!found) {
		if (prev == NULL)
		    completeWild->nsSet = cur->next;
		else 
		    prev->next = cur->next;
		tmp = cur->next;
		xmlFree(cur);
		cur = tmp;		
		continue;
	    }
	    prev = cur;
	    cur = cur->next;
	}	
		    		
	return(0);
    }    
    /* 5 If the two are negations of different namespace names, 
    * then the intersection is not expressible
    */	    
    if ((completeWild->negNsSet != NULL) && 
	(curWild->negNsSet != NULL) &&
	(completeWild->negNsSet->value != curWild->negNsSet->value) &&
	(completeWild->negNsSet->value != NULL) && 
	(curWild->negNsSet->value != NULL)) {

	xmlSchemaPErr(ctxt, completeWild->node, XML_SCHEMAP_INTERSECTION_NOT_EXPRESSIBLE,
	    "The intersection of the wilcard is not expressible.\n",
	    NULL, NULL);	
	return(XML_SCHEMAP_INTERSECTION_NOT_EXPRESSIBLE);
    }		    
    /* 
    * 6 If the one is a negation of a namespace name and the other 
    * is a negation of �absent�, then the one which is the negation 
    * of a namespace name must be the value.
    */
    if ((completeWild->negNsSet != NULL) && (curWild->negNsSet != NULL) &&
	(completeWild->negNsSet->value != curWild->negNsSet->value) &&
	(completeWild->negNsSet->value == NULL)) {	
	completeWild->negNsSet->value =  curWild->negNsSet->value; 
    }
    return(0);
}

/**
 * xmlSchemaIsWildcardNsConstraintSubset:
 * @ctxt:  the schema parser context
 * @wildA:  the first wildcard
 * @wildB: the second wildcard 
 *
 * Returns 1 if the namespace constraint of @wildA is an intensional 
 * subset of @wildB, 0 otherwise.
 */
static int
xmlSchemaIsWildcardNsConstraintSubset(xmlSchemaWildcardPtr wildA,
				      xmlSchemaWildcardPtr wildB)
{    

    /*
    * Schema Component Constraint: Wildcard Subset 
    */
    /*
    * 1 super must be any. 
    */
    if (wildB->any)
	return (1);
    /*
    * 2.1 sub must be a pair of not and a namespace name or �absent�.
    * 2.2 super must be a pair of not and the same value.
    */
    if ((wildA->negNsSet != NULL) &&
	(wildB->negNsSet != NULL) &&
	(wildA->negNsSet->value == wildA->negNsSet->value))
	return (1);    
    /* 
    * 3.1 sub must be a set whose members are either namespace names or �absent�. 
    */
    if (wildA->nsSet != NULL) {
	/*
	* 3.2.1 super must be the same set or a superset thereof. 
	*/
	if (wildB->nsSet != NULL) {
	    xmlSchemaWildcardNsPtr cur, curB;
	    int found = 0;
	    
	    cur = wildA->nsSet;
	    while (cur != NULL) {
		found = 0;
		curB = wildB->nsSet;
		while (curB != NULL) {
		    if (cur->value == curB->value) {
			found = 1;
			break;
		    }
		    curB = curB->next;
		}
		if (!found)
		    return (0);
		cur = cur->next;
	    }
	    if (found)
		return (1); 
	} else if (wildB->negNsSet != NULL) {
	    xmlSchemaWildcardNsPtr cur;
	    /*
	    * 3.2.2 super must be a pair of not and a namespace name or 
	    * �absent� and that value must not be in sub's set. 
	    */
	    cur = wildA->nsSet;
	    while (cur != NULL) {		
		if (cur->value == wildB->negNsSet->value)
		    return (0);
		cur = cur->next;
	    }  
	    return (1);
	}
    }
    return (0);
}

/**
 * xmlSchemaBuildCompleteAttributeWildcard:
 * @ctxt:  the schema parser context
 * @attrs: the attribute list 
 * @completeWild: the resulting complete wildcard
 *
 * Returns -1 in case of an internal error, 0 otherwise.
 */
static int
xmlSchemaBuildCompleteAttributeWildcard(xmlSchemaParserCtxtPtr ctxt, 				    
				   xmlSchemaAttributePtr attrs,
				   xmlSchemaWildcardPtr *completeWild)				
{        
    while (attrs != NULL) {
	if (attrs->type == XML_SCHEMA_TYPE_ATTRIBUTEGROUP) {
	    xmlSchemaAttributeGroupPtr group;

	    group = (xmlSchemaAttributeGroupPtr) attrs;
	    /*
	    * Handle attribute group references.
	    */
	    if (group->ref != NULL) {
		if (group->refItem == NULL) {
		    /*
		    * TODO: Should we raise a warning here?
		    */
		    /*
		    * The referenced attribute group definition could not
		    * be resolved beforehand, so skip.
		    */
		    attrs = attrs->next;
		    continue;
		} else
		    group = group->refItem;
	    }	    
	    /*
	    * For every attribute group definition, an intersected wildcard 
	    * will be created (assumed that a wildcard exists on the 
	    * particular attr. gr. def. or on any contained attr. gr. def 
	    * at all).
	    * The flag XML_SCHEMAS_ATTRGROUP_WILDCARD_BUILDED ensures
	    * that the intersection will be performed only once.
	    */
	    if ((group->flags & XML_SCHEMAS_ATTRGROUP_WILDCARD_BUILDED) == 0) {
		if (group->attributes != NULL) {
		    if (xmlSchemaBuildCompleteAttributeWildcard(ctxt, 
			group->attributes, &group->attributeWildcard) == -1)
			return (-1);
		}
		group->flags |= XML_SCHEMAS_ATTRGROUP_WILDCARD_BUILDED;
	    }		
	    if (group->attributeWildcard != NULL) {		
		if (*completeWild == NULL) {
		    /*
		    * Copy the first encountered wildcard as context, except for the annotation.
		    *
		    * Although the complete wildcard might not correspond to any
		    * node in the schema, we will save this context node.
		    */
		    *completeWild = xmlSchemaAddWildcard(ctxt, ctxt->schema,
			XML_SCHEMA_TYPE_ANY_ATTRIBUTE,
			group->attributeWildcard->node);
		    if (xmlSchemaCloneWildcardNsConstraints(ctxt, 
			completeWild, group->attributeWildcard) == -1)
			return (-1);
		    (*completeWild)->processContents = group->attributeWildcard->processContents;
		    (*completeWild)->node = group->attributeWildcard->node;
		} else if (xmlSchemaIntersectWildcards(ctxt, *completeWild, group->attributeWildcard) == -1)
		    return (-1);
	    }
	}
	attrs = attrs->next;
    }
   		                 
    return (0);   
}

static int
xmlSchemaGetEffectiveValueConstraint(xmlSchemaAttributePtr item,
				     int *fixed,
				     const xmlChar **value,
				     xmlSchemaValPtr *val)
{
    *fixed = 0;
    *value = NULL;
    if (val != 0) 
	*val = NULL;

    if (item->defValue == NULL)
	item = item->refDecl;

    if (item == NULL)
	return (0);

    if (item->defValue != NULL) {
	*value = item->defValue;
	if (val != 0)
	    *val = item->defVal;
	if (item->flags & XML_SCHEMAS_ATTR_FIXED)
	    *fixed = 1;
	return (1);
    }
    return (0);
}
/**
 * xmlSchemaMatchesWildcardNs:
 * @wild:  the wildcard
 * @ns:  the namespace
 * 
 *
 * Returns 1 if the given namespace matches the wildcard,
 * 0 otherwise.
 */
static int
xmlSchemaMatchesWildcardNs(xmlSchemaWildcardPtr wild, const xmlChar* ns)
{
    if (wild == NULL)
	return(0);

    if (wild->any)
	return(1);
    else if (wild->nsSet != NULL) {
	xmlSchemaWildcardNsPtr cur;

	cur = wild->nsSet;
	while (cur != NULL) {
	    if (xmlStrEqual(cur->value, ns))
		return(1);
	    cur = cur->next;
	}
    } else if ((wild->negNsSet != NULL) && (ns != NULL) && 
	(!xmlStrEqual(wild->negNsSet->value, ns)))
	return(1);	
	
    return(0);
}

/**
 * xmlSchemaBuildAttributeValidation:
 * @ctxt:  the schema parser context
 * @type:  the complex type definition
 * 
 *
 * Builds the wildcard and the attribute uses on the given complex type.
 * Returns -1 if an internal error occurs, 0 otherwise.
 */
static int
xmlSchemaBuildAttributeValidation(xmlSchemaParserCtxtPtr ctxt, xmlSchemaTypePtr type)
{
    xmlSchemaTypePtr baseType = NULL;
    xmlSchemaAttributeLinkPtr cur, base, tmp, id = NULL, prev = NULL, uses = NULL, 
	lastUse = NULL, lastBaseUse = NULL;
    xmlSchemaAttributePtr attrs;
    xmlSchemaTypePtr anyType;
    int baseIsAnyType = 0;
    xmlChar *str = NULL;
    int err = 0;

    anyType = xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYTYPE);
    /* 
     * Complex Type Definition with complex content Schema Component.
     *
     * Attribute uses.
     * TODO: Add checks for absent referenced attribute declarations and
     * simple types.
     */
    if (type->attributeUses != NULL) {
        xmlSchemaPErr(ctxt, type->node, XML_SCHEMAP_INTERNAL,
		      "Internal error: xmlSchemaBuildAttributeValidation: "
		      "attribute uses already builded.\n",
		      NULL, NULL);
        return (-1);
    }
    if (type->baseType == NULL) {
        xmlSchemaPErr(ctxt, type->node, XML_SCHEMAP_INTERNAL,
		      "Internal error: xmlSchemaBuildAttributeValidation: "
		      "complex type '%s' has no base type.\n",
		      type->name, NULL);
        return (-1);
    }
    baseType = type->baseType;
    if (baseType == anyType)
	baseIsAnyType = 1;
    /*
     * Inherit the attribute uses of the base type.
     */
    /*
     * NOTE: It is allowed to "extend" the anyType complex type.
     */
    if (!baseIsAnyType) {
	if (baseType != NULL) {
	    for (cur = baseType->attributeUses; cur != NULL; cur = cur->next) {
		tmp = (xmlSchemaAttributeLinkPtr) 
		    xmlMalloc(sizeof(xmlSchemaAttributeLink));
		if (tmp == NULL) {
		    xmlSchemaPErrMemory(ctxt, 
			"building attribute uses of complexType", NULL);
		    return (-1);
		}
		tmp->attr = cur->attr;
		tmp->next = NULL;
		if (type->attributeUses == NULL) {
		    type->attributeUses = tmp;
		} else 
		    lastBaseUse->next = tmp;
		lastBaseUse = tmp; 
	    }
	}
    }
    if ((type->subtypes != NULL) && 
	((type->subtypes->type == XML_SCHEMA_TYPE_COMPLEX_CONTENT) || 
	 (type->subtypes->type == XML_SCHEMA_TYPE_SIMPLE_CONTENT))) {
	/* 
	* type --> (<simpleContent>|<complexContent>) 
	*        --> (<restriction>|<extension>) --> attributes
	*/ 
	attrs = type->subtypes->subtypes->attributes;
    } else {
	/* Short hand form of the complexType. */
	attrs = type->attributes;
    }
    /*
    * Handle attribute wildcards.
    */	
    err = xmlSchemaBuildCompleteAttributeWildcard(ctxt, 
	attrs, &type->attributeWildcard);    
    /*
    * NOTE: During the parse time, the wildcard is created on the complexType
    * directly, if encountered in a <restriction> or <extension> element.
    */
    if (err == -1) {
	xmlSchemaPErr(ctxt, type->node, XML_SCHEMAP_INTERNAL,
	    "Internal error: xmlSchemaBuildAttributeValidation: "
	    "failed to build an intersected attribute wildcard.\n",
	    NULL, NULL);
	return (-1);
    }

    if ((type->flags & XML_SCHEMAS_TYPE_DERIVATION_METHOD_EXTENSION) && 
	((baseIsAnyType) ||
	 ((baseType != NULL) && 	    
	  (baseType->type == XML_SCHEMA_TYPE_COMPLEX) &&	      
	  (baseType->attributeWildcard != NULL)))) {	    
	if (type->attributeWildcard != NULL) {
	    /*
	    * Union the complete wildcard with the base wildcard.
	    */
	    if (xmlSchemaUnionWildcards(ctxt, type->attributeWildcard, 
		baseType->attributeWildcard) == -1)
		return (-1);
	} else {
	    /*
	    * Just inherit the wildcard.
	    */
	    /*
	    * NOTE: This is the only case where an attribute 
            * wildcard is shared.
            */
	    type->attributeWildcard = baseType->attributeWildcard;
	}
    }
    
    if (type->flags & XML_SCHEMAS_TYPE_DERIVATION_METHOD_RESTRICTION) {
	if (type->attributeWildcard != NULL) {
	    /* 
	    * Derivation Valid (Restriction, Complex) 	    
	    * 4.1 The {base type definition} must also have one. 
	    */
	    if (baseType->attributeWildcard == NULL) {	  
		xmlSchemaPCustomErr(ctxt,
		    XML_SCHEMAP_DERIVATION_OK_RESTRICTION_4_1,
		    NULL, type, NULL, 
		    "The type has an attribute wildcard, "
		    "but the base type %s does not have one",
		    xmlSchemaFormatItemForReport(&str, NULL, baseType, NULL, 1));
		FREE_AND_NULL(str)
		return (1);
	    } else if (xmlSchemaIsWildcardNsConstraintSubset(
		type->attributeWildcard, baseType->attributeWildcard) == 0) {
		/* 4.2 */
		xmlSchemaPCustomErr(ctxt,
		    XML_SCHEMAP_DERIVATION_OK_RESTRICTION_4_2,
		    NULL, type, NULL, 		
		    "The attribute wildcard is not a valid " 
		    "subset of the wildcard in the base type %s",
		    xmlSchemaFormatItemForReport(&str, NULL, baseType, NULL, 1));
		FREE_AND_NULL(str)	    
		return (1);
	    }
	    /* 4.3 Unless the {base type definition} is the �ur-type 
	    * definition�, the complex type definition's {attribute 
	    * wildcard}'s {process contents} must be identical to or 
	    * stronger than the {base type definition}'s {attribute 
	    * wildcard}'s {process contents}, where strict is stronger 
	    * than lax is stronger than skip.
	    */
	    if ((type->baseType != anyType) && 
		(type->attributeWildcard->processContents < 
		baseType->attributeWildcard->processContents)) {
		xmlSchemaPCustomErr(ctxt,
		    XML_SCHEMAP_DERIVATION_OK_RESTRICTION_4_3,
		    NULL, type, NULL, 		
		    "The 'process contents' of the attribute wildcard is weaker than "
		    "the one in the base type %s",
		    xmlSchemaFormatItemForReport(&str, NULL, baseType, NULL, 1));
		FREE_AND_NULL(str)
		return (1);
	    }
	}
    } else if (type->flags & XML_SCHEMAS_TYPE_DERIVATION_METHOD_EXTENSION) {
	/*
	* Derivation Valid (Extension)
	* At this point the type and the base have both, either
	* no wildcard or a wildcard.
	*/
	if ((baseType->attributeWildcard != NULL) &&
	    (baseType->attributeWildcard != type->attributeWildcard)) {
	    /* 1.3 */
	    if (xmlSchemaIsWildcardNsConstraintSubset(
		baseType->attributeWildcard, type->attributeWildcard) == 0) {
		xmlSchemaPCustomErr(ctxt,
		    XML_SCHEMAP_COS_CT_EXTENDS_1_3,
		    NULL, type, NULL, 		
		    "The attribute wildcard is not a valid " 
		    "superset of the one in the base type %s",
		    xmlSchemaFormatItemForReport(&str, NULL, baseType, NULL, 1));
		FREE_AND_NULL(str)		
		return (1);		
	    }
	}		
    }	

    /*
     * Gather attribute uses defined by this type.
     */
    if (attrs != NULL) {
	if (xmlSchemaBuildAttributeUsesOwned(ctxt, attrs, 
	    &uses, &lastUse) == -1) {
	    return (-1);
	}
    }
    /* 3.4.6 -> Complex Type Definition Properties Correct 4.
     * "Two distinct attribute declarations in the {attribute uses} must 
     * not have identical {name}s and {target namespace}s."
     *
     * For "extension" this is done further down.
     */
    if ((uses != NULL) && ((type->flags & XML_SCHEMAS_TYPE_DERIVATION_METHOD_EXTENSION) == 0)) {
	cur = uses;
	while (cur != NULL) {
	    tmp = cur->next;
	    while (tmp != NULL) {	    
		if ((xmlStrEqual(xmlSchemaGetAttrName(cur->attr), 
		    xmlSchemaGetAttrName(tmp->attr))) &&
		    (xmlStrEqual(xmlSchemaGetAttrTargetNsURI(cur->attr ), 
		    xmlSchemaGetAttrTargetNsURI(tmp->attr)))) {

		    xmlSchemaPAttrUseErr(ctxt,
			XML_SCHEMAP_CT_PROPS_CORRECT_4, 
			NULL, type, NULL, cur->attr,			
			"Duplicate attribute use %s specified",
			xmlSchemaFormatNsUriLocal(&str, 
			    xmlSchemaGetAttrTargetNsURI(tmp->attr), 
			    xmlSchemaGetAttrName(tmp->attr))
		    );
		    FREE_AND_NULL(str)		    		    
		    break;
		}
		tmp = tmp->next;
	    }
	    cur = cur->next;
	}
    }	
    if (type->flags & XML_SCHEMAS_TYPE_DERIVATION_METHOD_RESTRICTION) {	
	/*
	 * Derive by restriction.
	 */
	if (baseIsAnyType) {
	    type->attributeUses = uses;
	} else {
	    int found;
	    const xmlChar *bEffValue;
	    int effFixed;

	    cur = uses;
	    while (cur != NULL) {
		found = 0;
		base = type->attributeUses;
		while (base != NULL) {
		    if (xmlStrEqual(xmlSchemaGetAttrName(cur->attr), 
			xmlSchemaGetAttrName(base->attr)) &&
			xmlStrEqual(xmlSchemaGetAttrTargetNsURI(cur->attr), 
			xmlSchemaGetAttrTargetNsURI(base->attr))) {
			
			found = 1;			
			
			if ((cur->attr->occurs == XML_SCHEMAS_ATTR_USE_OPTIONAL) &&
			    (base->attr->occurs == XML_SCHEMAS_ATTR_USE_REQUIRED)) {
			    /*
			    * derivation-ok-restriction 2.1.1
			    */	
			    xmlSchemaPAttrUseErr(ctxt,
				XML_SCHEMAP_DERIVATION_OK_RESTRICTION_2_1_1,
				NULL, type, NULL, cur->attr,
				"The 'optional' use is inconsistent with a matching "
				"'required' use of the base type", NULL);				
			} else if ((cur->attr->occurs == XML_SCHEMAS_ATTR_USE_PROHIBITED) &&
			    (base->attr->occurs == XML_SCHEMAS_ATTR_USE_REQUIRED)) {
			    /*
			    * derivation-ok-restriction 3 
			    */
			    xmlSchemaPCustomErr(ctxt,
				XML_SCHEMAP_DERIVATION_OK_RESTRICTION_3, 
				NULL, type, NULL, 		
				"A matching attribute use for the 'required' "
				"attribute use %s of the base type is missing",
				xmlSchemaFormatNsUriLocal(&str, 
				xmlSchemaGetAttrTargetNsURI(base->attr), 
				xmlSchemaGetAttrName(base->attr)));	
			    FREE_AND_NULL(str)
			} else {
			    /*
			    * 2.1.3 [Definition:]  Let the effective value 
			    * constraint of an attribute use be its {value 
			    * constraint}, if present, otherwise its {attribute 
			    * declaration}'s {value constraint} . 
			    */
			    xmlSchemaGetEffectiveValueConstraint(base->attr, &effFixed, 
				&bEffValue, 0);			   							    
			    /*
			    * 2.1.3 ... one of the following must be true
			    *
			    * 2.1.3.1 B's �effective value constraint� is 
			    * �absent� or default.
			    */
			    if ((bEffValue != NULL) &&
				(effFixed == 1)) {
				const xmlChar *rEffValue = NULL;

				xmlSchemaGetEffectiveValueConstraint(base->attr, &effFixed, 
				    &rEffValue, 0);	
				/*
				* 2.1.3.2 R's �effective value constraint� is 
				* fixed with the same string as B's.
				*/
				if ((effFixed == 0) ||
				    (! xmlStrEqual(rEffValue, bEffValue))) {
				    xmlSchemaPAttrUseErr(ctxt,
					XML_SCHEMAP_DERIVATION_OK_RESTRICTION_2_1_3, 
					NULL, type, NULL, cur->attr,		
					"The effective value constraint of the "
					"attribute use is inconsistent with "
					"its correspondent of the base type",
					NULL);					    
				}
			    }
			    /*
			    * TODO: derivation-ok-restriction  2.1.2 ({type definition} must be validly derived)
			    */
			    /*
			    * Override the attribute use.
			    */
			    base->attr = cur->attr;
			}
								
			break;
		    }				
		    base = base->next;
		}
		
		if (!found) {
		    if (cur->attr->occurs != XML_SCHEMAS_ATTR_USE_PROHIBITED) {
			/*
			* derivation-ok-restriction  2.2
			*/
			if ((type->attributeWildcard != NULL) &&
			    xmlSchemaMatchesWildcardNs(type->attributeWildcard,
				cur->attr->targetNamespace))
			    found = 1;

			if (!found) {
			    xmlSchemaPAttrUseErr(ctxt,
				XML_SCHEMAP_DERIVATION_OK_RESTRICTION_2_2, 
				NULL, type, NULL, cur->attr,		
				"Neither a matching attribute use, "
				"nor a matching wildcard in the base type does exist",
				NULL);
			} else {
			    /* 
			    * Add the attribute use.
			    *
			    * Note that this may lead to funny derivation error reports, if
			    * multiple equal attribute uses exist; but this is not
			    * allowed anyway, and it will be reported beforehand.
			    */
			    tmp = cur;
			    if (prev != NULL)
				prev->next = cur->next;
			    else 
				uses = cur->next;
			    cur = cur->next;    			
			    if (type->attributeUses == NULL) {
				type->attributeUses = tmp;
			    } else 
				lastBaseUse->next = tmp;
			    lastBaseUse = tmp;		
			    
			    continue;
			}
		    }
		}		    	    
		prev = cur;	
		cur = cur->next;
	    }
	    if (uses != NULL)
		xmlSchemaFreeAttributeUseList(uses);
	}
    } else if (type->flags & XML_SCHEMAS_TYPE_DERIVATION_METHOD_EXTENSION) { 
	/*
	 * The spec allows only appending, and not other kinds of extensions.
	 *
	 * This ensures: Schema Component Constraint: Derivation Valid (Extension) : 1.2 
	 */
	if (uses != NULL) {
	    if (type->attributeUses == NULL) {
		type->attributeUses = uses;
	    } else 
		lastBaseUse->next = uses;
	}
    } else {
	/* 
	* Derive implicitely from the ur-type.
	*/
	type->attributeUses = uses;
    }
    /*
     * 3.4.6 -> Complex Type Definition Properties Correct
     */
    if (type->attributeUses != NULL) {
	cur = type->attributeUses;
	prev = NULL;
	while (cur != NULL) {
	    /*
	    * 4. Two distinct attribute declarations in the {attribute uses} must 
	    * not have identical {name}s and {target namespace}s.
	    *
	    * Note that this was already done for "restriction" and types derived from
	    * the ur-type.
	    */
	    if (type->flags & XML_SCHEMAS_TYPE_DERIVATION_METHOD_EXTENSION) {
		tmp = cur->next;
		while (tmp != NULL) {	    
		    if ((xmlStrEqual(xmlSchemaGetAttrName(cur->attr), 
			xmlSchemaGetAttrName(tmp->attr))) &&
			(xmlStrEqual(xmlSchemaGetAttrTargetNsURI(cur->attr ), 
			xmlSchemaGetAttrTargetNsURI(tmp->attr)))) {

			xmlSchemaPAttrUseErr(ctxt,
			    XML_SCHEMAP_CT_PROPS_CORRECT_4, 
			    NULL, type, NULL, tmp->attr,		
			    "Duplicate attribute use specified", NULL);
			break;
		    }
		    tmp = tmp->next;
		}
	    }
	    /*
	    * 5. Two distinct attribute declarations in the {attribute uses} must 
	    * not have {type definition}s which are or are derived from ID.
	    */
	    if ((cur->attr->subtypes != NULL) && 
		(xmlSchemaIsDerivedFromBuiltInType(ctxt, (xmlSchemaTypePtr) cur->attr, XML_SCHEMAS_ID))) {
		if (id != NULL) {
		    xmlSchemaPAttrUseErr(ctxt,
			XML_SCHEMAP_CT_PROPS_CORRECT_5, 
			NULL, type, NULL, cur->attr,
			"There must not exist more than one attribute use, "
			"declared of type 'ID' or derived from it", 
			NULL);
		    FREE_AND_NULL(str)
		} 
		id = cur;
	    }
	    /*
	    * Remove "prohibited" attribute uses. The reason this is done at this late 
	    * stage is to be able to catch dublicate attribute uses. So we had to keep
	    * prohibited uses in the list as well.
	    */
	    if (cur->attr->occurs == XML_SCHEMAS_ATTR_USE_PROHIBITED) {
		tmp = cur;
		if (prev == NULL)
		    type->attributeUses = cur->next;
		else
		    prev->next = cur->next;
		cur = cur->next;
		xmlFree(tmp);
	    } else {
		prev = cur;
		cur = cur->next;
	    }
	}    
    }
    /*	
     * TODO: This check should be removed if we are 100% sure of
     * the base type attribute uses already being built.
     */
    if ((baseType != NULL) && (!baseIsAnyType) &&
	(baseType->type == XML_SCHEMA_TYPE_COMPLEX) &&
	(IS_NOT_TYPEFIXED(baseType))) {
	xmlSchemaPErr(ctxt, baseType->node, XML_SCHEMAP_INTERNAL,
	    "Internal error: xmlSchemaBuildAttributeValidation: "
	    "attribute uses not builded on base type '%s'.\n",
	    baseType->name, NULL);
    }    
    return (0);
}

/**
 * xmlSchemaTypeFinalContains:
 * @schema:  the schema
 * @type:  the type definition
 * @final: the final
 *
 * Evaluates if a type definition contains the given "final".
 * This does take "finalDefault" into account as well.
 *
 * Returns 1 if the type does containt the given "final",
 * 0 otherwise.
 */
static int
xmlSchemaTypeFinalContains(xmlSchemaPtr schema, xmlSchemaTypePtr type, int final)
{
    int tfinal = final, tflags = type->flags;

    if (type == NULL)
	return (0);    
    if (type->flags & XML_SCHEMAS_TYPE_FINAL_DEFAULT) {
	switch (final) {
	    case XML_SCHEMAS_TYPE_FINAL_RESTRICTION:
		tfinal = XML_SCHEMAS_FINAL_DEFAULT_RESTRICTION;
		break;
	    case XML_SCHEMAS_TYPE_FINAL_EXTENSION:
		tfinal = XML_SCHEMAS_FINAL_DEFAULT_EXTENSION;
		break;
	    case XML_SCHEMAS_TYPE_FINAL_LIST:
		tfinal = XML_SCHEMAS_FINAL_DEFAULT_LIST;
		break;
	    case XML_SCHEMAS_TYPE_FINAL_UNION:
		tfinal = XML_SCHEMAS_FINAL_DEFAULT_UNION;
		break;
	}
	tflags = schema->flags;
    }
    if (tflags & tfinal) 
	return (1);
    else
	return (0);
    
}

/**
 * xmlSchemaGetUnionSimpleTypeMemberTypes:
 * @type:  the Union Simple Type
 *
 * Returns a list of member types of @type if existing, 
 * returns NULL otherwise.
 */
static xmlSchemaTypeLinkPtr
xmlSchemaGetUnionSimpleTypeMemberTypes(xmlSchemaTypePtr type)
{
    while ((type != NULL) && (type->type == XML_SCHEMA_TYPE_SIMPLE)) {
	if (type->memberTypes != NULL)
	    return (type->memberTypes);
	else
	    type = type->baseType;
    }
    return (NULL);
}

/**
 * xmlSchemaGetListSimpleTypeItemType:
 * @type:  the simple type definition
 *
 * Returns the item type definition of the list simple type.
 */ 
static xmlSchemaTypePtr
xmlSchemaGetListSimpleTypeItemType(xmlSchemaTypePtr type)
{    
    if ((type->flags & XML_SCHEMAS_TYPE_VARIETY_LIST) == 0)
	return (NULL);
    /*
    * Note: In libxml2, the built-in types do not reflect 
    * the datatype hierarchy (yet?) - we have to treat them
    * in a special way.
    */
    if (type->type == XML_SCHEMA_TYPE_BASIC) 
	return (xmlSchemaGetBuiltInListSimpleTypeItemType(type));
    return (type->subtypes);
}


static int
xmlSchemaGetParticleTotalRangeMin(xmlSchemaTreeItemPtr particle)
{
    if ((particle->children == NULL) ||
	( ((xmlSchemaParticlePtr) particle)->minOccurs == 0))
	return (0);
    if (particle->children->type == XML_SCHEMA_TYPE_CHOICE) {
	int min = 0, cur;
	xmlSchemaTreeItemPtr part = particle->children->children;

	if (part == NULL)
	    return (0);	
	if ((part->children->type == XML_SCHEMA_TYPE_ELEMENT) ||
	    (part->children->type == XML_SCHEMA_TYPE_ANY))
	    min = ((xmlSchemaParticlePtr) part)->minOccurs;
	else
	    min = xmlSchemaGetParticleTotalRangeMin(part);
	if (min == 0)
	    return (0);
	part = part->next;
	while (part != NULL) {
	    if ((part->children->type == XML_SCHEMA_TYPE_ELEMENT) ||
		(part->children->type == XML_SCHEMA_TYPE_ANY))
		cur = ((xmlSchemaParticlePtr) part)->minOccurs;
	    else
		cur = xmlSchemaGetParticleTotalRangeMin(part);	    
	    if (cur == 0)
		return (0);
	    if (min > cur)
		min = cur;
	    part = part->next;
	}
	return (((xmlSchemaParticlePtr) particle)->minOccurs * min);
    } else {
	/* <all> and <sequence> */
	int sum = 0;
	xmlSchemaTreeItemPtr part = particle->children->children;

	if (part == NULL)
	    return (0);
	do {
	    if ((part->children->type == XML_SCHEMA_TYPE_ELEMENT) ||
		(part->children->type == XML_SCHEMA_TYPE_ANY))
		sum += ((xmlSchemaParticlePtr) part)->minOccurs;
	    else
		sum += xmlSchemaGetParticleTotalRangeMin(part);
	    part = part->next;
	} while (part != NULL);
	return (((xmlSchemaParticlePtr) particle)->minOccurs * sum);
    }
}


static int
xmlSchemaIsParticleEmptiable(xmlSchemaParticlePtr particle)
{
    if ((particle == NULL) || (particle->minOccurs == 0) ||
	(particle->children == NULL))
	return (1);

    if (IS_MODEL_GROUP(particle->children)) {
	if (xmlSchemaGetParticleTotalRangeMin(
	    (xmlSchemaTreeItemPtr) particle) == 0)
	    return (1);
    }
    return (0);
}

/**
 * xmlSchemaCheckCOSSTDerivedOK:
 * @type:  the derived simple type definition
 * @baseType:  the base type definition
 *
 * Checks wheter @type can be validly 
 * derived from @baseType.
 *
 * Returns 0 on success, an positive error code otherwise.
 */ 
static int
xmlSchemaCheckCOSSTDerivedOK(xmlSchemaPtr schema,
				     xmlSchemaTypePtr type,
				     xmlSchemaTypePtr baseType,
				     int subset)
{   
    /*
    * Schema Component Constraint: Type Derivation OK (Simple)
    *
    *
    * 1 They are the same type definition.
    * TODO: The identy check might have to be more complex than this.
    */
    if (type == baseType)
	return (0);    
    /* 
    * 2.1 restriction is not in the subset, or in the {final}
    * of its own {base type definition};
    */
    if ((subset & XML_SCHEMAS_TYPE_FINAL_RESTRICTION) ||
	(xmlSchemaTypeFinalContains(schema, 
	    type->baseType, XML_SCHEMAS_TYPE_FINAL_RESTRICTION))) {
	return (XML_SCHEMAP_COS_ST_DERIVED_OK_2_1); 
    }
    /* 2.2 */
    if (type->baseType == baseType) {
	/*
	* 2.2.1 D's �base type definition� is B.
	*/
	return (0);
    }   
    /* 
    * 2.2.2 D's �base type definition� is not the �ur-type definition� 
    * and is validly derived from B given the subset, as defined by this 
    * constraint.    
    */
    if ((type->baseType != xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYTYPE)) &&
	(xmlSchemaCheckCOSSTDerivedOK(schema, type->baseType, baseType, subset) == 0)) {
	return (0);		
    } 
    /* 
    * 2.2.3 D's {variety} is list or union and B is the �simple ur-type 
    * definition�.
    */
    if (((type->flags & XML_SCHEMAS_TYPE_VARIETY_LIST) ||
	(type->flags & XML_SCHEMAS_TYPE_VARIETY_UNION)) &&
	(baseType == xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYSIMPLETYPE))) {
	return (0);
    }    
    /* 
    * 2.2.4 B's {variety} is union and D is validly derived from a type 
    * definition in B's {member type definitions} given the subset, as 
    * defined by this constraint.
    *
    * NOTE: This seems not to involve built-in types, since there is no
    * built-in Union Simple Type.
    */
    if (baseType->flags & XML_SCHEMAS_TYPE_VARIETY_UNION) {
	xmlSchemaTypeLinkPtr cur;

	cur = baseType->memberTypes;
	while (cur != NULL) {
	    if (xmlSchemaCheckCOSSTDerivedOK(schema, type, 
		cur->type, subset) == 0)
		return (0);
	    cur = cur->next;
	}	
    }
    
    return (XML_SCHEMAP_COS_ST_DERIVED_OK_2_2);
}

/**
 * xmlSchemaCheckTypeDefCircularInternal:
 * @pctxt:  the schema parser context
 * @ctxtType:  the type definition
 * @ancestor: an ancestor of @ctxtType
 *
 * Checks st-props-correct (2) + ct-props-correct (3).
 * Circular type definitions are not allowed. 
 *
 * Returns XML_SCHEMAP_ST_PROPS_CORRECT_2 if the given type is
 * circular, 0 otherwise.
 */
static int
xmlSchemaCheckTypeDefCircularInternal(xmlSchemaParserCtxtPtr pctxt,
			   xmlSchemaTypePtr ctxtType,
			   xmlSchemaTypePtr ancestor)
{
    int ret;

    if ((ancestor == NULL) || (ancestor->type == XML_SCHEMA_TYPE_BASIC))
	return (0);

    if (IS_NOT_TYPEFIXED(ancestor))
	xmlSchemaTypeFixup(ancestor, pctxt,  NULL);
    if (ancestor->flags & XML_SCHEMAS_TYPE_MARKED) {
	/*
	* Avoid inifinite recursion on circular types not yet checked.
	*/
	return (0);
    }
    if (ctxtType == ancestor) {
	xmlSchemaPCustomErr(pctxt,
	    XML_SCHEMAP_ST_PROPS_CORRECT_2,
	    NULL, ctxtType, GET_NODE(ctxtType),
	    "The definition is circular", NULL);
	return (XML_SCHEMAP_ST_PROPS_CORRECT_2);
    }
    ancestor->flags |= XML_SCHEMAS_TYPE_MARKED;
    ret = xmlSchemaCheckTypeDefCircularInternal(pctxt, ctxtType,
	ancestor->baseType);
    ancestor->flags ^= XML_SCHEMAS_TYPE_MARKED;
    return (ret);
}

/**
 * xmlSchemaCheckGroupDefCircular:
 * @item:  the complex/simple type definition
 * @ctxt:  the parser context
 * @name:  the name
 *
 * Checks for circular type definitions.
 */
static void
xmlSchemaCheckTypeDefCircular(xmlSchemaTypePtr item,
			      xmlSchemaParserCtxtPtr ctxt, 
			      const xmlChar * name ATTRIBUTE_UNUSED)
{
    if ((item == NULL) ||
	((item->type != XML_SCHEMA_TYPE_COMPLEX) &&
	(item->type != XML_SCHEMA_TYPE_SIMPLE)))
	return;
    xmlSchemaCheckTypeDefCircularInternal(ctxt, item, item->baseType);

}

/**
 * xmlSchemaCheckSTPropsCorrect:
 * @ctxt:  the schema parser context
 * @type:  the simple type definition
 *
 * Checks st-props-correct.
 *
 * Returns 0 if the properties are correct,
 * if not, a positive error code and -1 on internal
 * errors.
 */
static int
xmlSchemaCheckSTPropsCorrect(xmlSchemaParserCtxtPtr ctxt, 
			     xmlSchemaTypePtr type)
{
    xmlSchemaTypePtr baseType = type->baseType, anySimpleType,
	anyType;
    xmlChar *str = NULL;

    /* STATE: error funcs converted. */
    /*
    * Schema Component Constraint: Simple Type Definition Properties Correct
    *
    * NOTE: This is somehow redundant, since we actually built a simple type
    * to have all the needed information; this acts as an self test.
    */
    anySimpleType = xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYSIMPLETYPE);
    anyType = xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYTYPE);
    /* Base type: If the datatype has been �derived� by �restriction� 
    * then the Simple Type Definition component from which it is �derived�, 
    * otherwise the Simple Type Definition for anySimpleType (�4.1.6). 
    */
    if (baseType == NULL) {
	/*
	* TODO: Think about: "modulo the impact of Missing
	* Sub-components (�5.3)."
	*/
	xmlSchemaPCustomErr(ctxt,
	    XML_SCHEMAP_ST_PROPS_CORRECT_1,
	    NULL, type, NULL,	
	    "No base type existent", NULL);
	return (XML_SCHEMAP_ST_PROPS_CORRECT_1);

    }
    if (! IS_SIMPLE_TYPE(baseType)) {
	xmlSchemaPCustomErr(ctxt,
	    XML_SCHEMAP_ST_PROPS_CORRECT_1,
	    NULL, type, NULL,	
	    "The base type '%s' is not a simple type", 
	    xmlSchemaGetComponentQName(&str, baseType));
	FREE_AND_NULL(str)	
	return (XML_SCHEMAP_ST_PROPS_CORRECT_1);
    }
    if ((baseType->builtInType != XML_SCHEMAS_ANYSIMPLETYPE) &&
	((type->flags &
	    XML_SCHEMAS_TYPE_DERIVATION_METHOD_RESTRICTION) == 0) &&
	((type->flags & XML_SCHEMAS_TYPE_VARIETY_LIST) ||
	 (type->flags & XML_SCHEMAS_TYPE_VARIETY_UNION))) {
	xmlSchemaPCustomErr(ctxt,
	    XML_SCHEMAP_ST_PROPS_CORRECT_1,
	    NULL, type, NULL,	
	    "A type, derived by list or union, must have"
	    "the simple ur-type definition as base type, not '%s'",
	    xmlSchemaGetComponentQName(&str, baseType));
	FREE_AND_NULL(str)
	return (XML_SCHEMAP_ST_PROPS_CORRECT_1);
    }
    /* 
    * Variety: One of {atomic, list, union}. 
    */
    if (((type->flags & XML_SCHEMAS_TYPE_VARIETY_ATOMIC) == 0) &&
	((type->flags & XML_SCHEMAS_TYPE_VARIETY_UNION) == 0) &&
	((type->flags & XML_SCHEMAS_TYPE_VARIETY_LIST) == 0)) {
	xmlSchemaPCustomErr(ctxt,
	    XML_SCHEMAP_ST_PROPS_CORRECT_1,
	    NULL, type, NULL,	
	    "The variety is absent", NULL);
	return (XML_SCHEMAP_ST_PROPS_CORRECT_1);
    }
    /* TODO: Finish this. Hmm, is this finished? */
    
    /*
    * 3 The {final} of the {base type definition} must not contain restriction.
    */
    if (xmlSchemaTypeFinalContains(ctxt->schema, baseType,
	XML_SCHEMAS_TYPE_FINAL_RESTRICTION)) {
	xmlSchemaPCustomErr(ctxt,
	    XML_SCHEMAP_ST_PROPS_CORRECT_3,
	    NULL, type, NULL,	
	    "The 'final' of its base type '%s' must not contain "
	    "'restriction'",
	    xmlSchemaGetComponentQName(&str, baseType));
	FREE_AND_NULL(str)	
	return (XML_SCHEMAP_ST_PROPS_CORRECT_3);
    }    

    /*
    * 2 All simple type definitions must be derived ultimately from the �simple 
    * ur-type definition (so� circular definitions are disallowed). That is, it 
    * must be possible to reach a built-in primitive datatype or the �simple 
    * ur-type definition� by repeatedly following the {base type definition}.    
    *
    * NOTE: this is done in xmlSchemaCheckTypeDefCircular().
    */
    return (0);
}

/**
 * xmlSchemaCheckCOSSTRestricts:
 * @ctxt:  the schema parser context
 * @type:  the simple type definition
 *
 * Checks if the given @type (simpleType) is derived 
 * validly by restriction.
 * STATUS:
 *   missing:
 *     (3.3.2.5) facet derivation
 *     (1.3.2)   facet derivation
 *     (2.3.2.5)
 *     (3.3.2.5) facet derivation
 *
 *
 * Returns -1 on internal errors, 0 if the type is validly derived, 
 * a positive error code otherwise.
 */
static int
xmlSchemaCheckCOSSTRestricts(xmlSchemaParserCtxtPtr ctxt, 
			     xmlSchemaTypePtr type)
{    
    xmlChar *str = NULL;

    if (type->type != XML_SCHEMA_TYPE_SIMPLE) {
	xmlSchemaPErr(ctxt, type->node,
	    XML_ERR_INTERNAL_ERROR,
	    "xmlSchemaCheckCOSSTRestricts: The given "
	    "type '%s' is not a user-derived simpleType.\n",
	    type->name, NULL);
	return (-1);
    }

    if (type->flags & XML_SCHEMAS_TYPE_VARIETY_ATOMIC) {
	xmlSchemaTypePtr primitive;
	/* 
	* 1.1 The {base type definition} must be an atomic simple 
	* type definition or a built-in primitive datatype.
	*/	
	if ((type->baseType->flags & XML_SCHEMAS_TYPE_VARIETY_ATOMIC) == 0) {
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_COS_ST_RESTRICTS_1_1,
		NULL, type, NULL,	
		"The base type '%s' is not an atomic simple type",
		xmlSchemaGetComponentQName(&str, type->baseType));
	    FREE_AND_NULL(str)
	    return (XML_SCHEMAP_COS_ST_RESTRICTS_1_1);
	}
	/* 1.2 The {final} of the {base type definition} must not contain 
	* restriction.
	*/
	/* OPTIMIZE TODO : This is already done in xmlSchemaCheckStPropsCorrect */
	if (xmlSchemaTypeFinalContains(ctxt->schema, type->baseType, 
	    XML_SCHEMAS_TYPE_FINAL_RESTRICTION)) {
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_COS_ST_RESTRICTS_1_2,
		NULL, type, NULL,	
		"The final of its base type '%s' must not contain 'restriction'",
		xmlSchemaGetComponentQName(&str, type->baseType));
	    FREE_AND_NULL(str)
	    return (XML_SCHEMAP_COS_ST_RESTRICTS_1_2);
	}
	
	/* 
	* 1.3.1 DF must be an allowed constraining facet for the {primitive
	* type definition}, as specified in the appropriate subsection of 3.2 
	* Primitive datatypes.
	*/
	if (type->facets != NULL) {
	    xmlSchemaFacetPtr facet;
	    int ok = 1;
	    
	    primitive = xmlSchemaGetPrimitiveType(type);
	    if (primitive == NULL) {
		xmlSchemaPErr(ctxt, type->node,
		    XML_ERR_INTERNAL_ERROR,
		    "xmlSchemaCheckCOSSTRestricts: failed "
		    "to get primitive type of type '%s'.\n",
		    type->name, NULL);
		return (-1);
	    }	    
	    facet = type->facets;
	    do {
		if (xmlSchemaIsBuiltInTypeFacet(primitive, facet->type) == 0) {
		    ok = 0;
		    xmlSchemaPIllegalFacetAtomicErr(ctxt,
			XML_SCHEMAP_COS_ST_RESTRICTS_1_3_1,
			NULL, type, primitive, facet);		    		    		    
		}
		facet = facet->next;
	    } while (facet != NULL);	    
	    if (ok == 0)
		return (XML_SCHEMAP_COS_ST_RESTRICTS_1_3_1);	    
	}
	/*
	* TODO: 1.3.2 (facet derivation)
	*/
    } else if (type->flags & XML_SCHEMAS_TYPE_VARIETY_LIST) {
	xmlSchemaTypePtr itemType = NULL;

	itemType = type->subtypes;
	if ((itemType == NULL) || (! IS_SIMPLE_TYPE(itemType))) {
	    xmlSchemaPErr(ctxt, type->node,
		XML_ERR_INTERNAL_ERROR,
		"Internal error: xmlSchemaCheckCOSSTRestricts: "
		"failed to evaluate the item type of type '%s'.\n",
		type->name, NULL);
	    return (-1);
	}
	if (IS_NOT_TYPEFIXED(itemType))
	    xmlSchemaTypeFixup(itemType, ctxt, NULL);
	/*
	* 2.1 The {item type definition} must have a {variety} of atomic or 
	* union (in which case all the {member type definitions} 
	* must be atomic).
	*/
	if (((itemType->flags & XML_SCHEMAS_TYPE_VARIETY_ATOMIC) == 0) &&  
	    ((itemType->flags & XML_SCHEMAS_TYPE_VARIETY_UNION) == 0)) {	    
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_COS_ST_RESTRICTS_2_1,
		NULL, type, NULL,	
		"The item type '%s' does not have a variety of atomic or union",
		xmlSchemaGetComponentQName(&str, itemType));
	    FREE_AND_NULL(str)	    
	    return (XML_SCHEMAP_COS_ST_RESTRICTS_2_1);
	} else if (itemType->flags & XML_SCHEMAS_TYPE_VARIETY_UNION) {
	    xmlSchemaTypeLinkPtr member;

	    member = itemType->memberTypes;
	    while (member != NULL) {
		if ((member->type->flags & 
		    XML_SCHEMAS_TYPE_VARIETY_ATOMIC) == 0) {
		    xmlSchemaPCustomErr(ctxt,
			XML_SCHEMAP_COS_ST_RESTRICTS_2_1,
			NULL, type, NULL,	
			"The item type is a union type, but the "
			"member type '%s' of this item type is not atomic",
			xmlSchemaGetComponentQName(&str, member->type));
		    FREE_AND_NULL(str)		    
		    return (XML_SCHEMAP_COS_ST_RESTRICTS_2_1);
		}
		member = member->next;
	    }
	}
	
	if (type->baseType->builtInType == XML_SCHEMAS_ANYSIMPLETYPE) {
	    xmlSchemaFacetPtr facet;
	    /*
	    * This is the case if we have: <simpleType><list ..
	    */
	    /*
	    * 2.3.1 
	    * 2.3.1.1 The {final} of the {item type definition} must not 
	    * contain list.
	    */
	    if (xmlSchemaTypeFinalContains(ctxt->schema, 
		itemType, XML_SCHEMAS_TYPE_FINAL_LIST)) {
		xmlSchemaPCustomErr(ctxt,
		    XML_SCHEMAP_COS_ST_RESTRICTS_2_3_1_1,
		    NULL, type, NULL,	
		    "The final of its item type '%s' must not contain 'list'",
		    xmlSchemaGetComponentQName(&str, itemType));
		FREE_AND_NULL(str)			
		return (XML_SCHEMAP_COS_ST_RESTRICTS_2_3_1_1);
	    }
	    /*
	    * 2.3.1.2 The {facets} must only contain the whiteSpace
	    * facet component.
	    */
	    if (type->facets != NULL) {
		facet = type->facets;
		do {
		    if (facet->type != XML_SCHEMA_FACET_WHITESPACE) {
			xmlSchemaPIllegalFacetListUnionErr(ctxt,
			    XML_SCHEMAP_COS_ST_RESTRICTS_2_3_1_2,
			    NULL, type, facet);
			return (XML_SCHEMAP_COS_ST_RESTRICTS_2_3_1_2);
		    }
		    facet = facet->next;
		} while (facet != NULL);
	    }
	    /*
	    * MABY TODO: (Hmm, not really) Datatypes states: 
	    * A �list� datatype can be �derived� from an �atomic� datatype 
	    * whose �lexical space� allows space (such as string or anyURI)or 
	    * a �union� datatype any of whose {member type definitions}'s 
	    * �lexical space� allows space.
	    */
	} else {
	    /*
	    * This is the case if we have: <simpleType><restriction ...
	    * I.e. the variety of "list" is inherited.
	    */
	    /*
	    * 2.3.2 
	    * 2.3.2.1 The {base type definition} must have a {variety} of list.
	    */
	    if ((type->baseType->flags & XML_SCHEMAS_TYPE_VARIETY_LIST) == 0) {
		xmlSchemaPCustomErr(ctxt,
		    XML_SCHEMAP_COS_ST_RESTRICTS_2_3_2_1,
		    NULL, type, NULL,	
		    "The base type '%s' must be a list type",
		    xmlSchemaGetComponentQName(&str, type->baseType));
		FREE_AND_NULL(str)					
		return (XML_SCHEMAP_COS_ST_RESTRICTS_2_3_2_1);
	    }
	    /*
	    * 2.3.2.2 The {final} of the {base type definition} must not
	    * contain restriction.
	    */
	    if (xmlSchemaTypeFinalContains(ctxt->schema, type->baseType,
		XML_SCHEMAS_TYPE_FINAL_RESTRICTION)) {
		xmlSchemaPCustomErr(ctxt,
		    XML_SCHEMAP_COS_ST_RESTRICTS_2_3_2_2,
		    NULL, type, NULL,	
		    "The 'final' of the base type '%s' must not contain 'restriction'",
		    xmlSchemaGetComponentQName(&str, type->baseType));
		FREE_AND_NULL(str)				
		return (XML_SCHEMAP_COS_ST_RESTRICTS_2_3_2_2);
	    }
	    /*
	    * 2.3.2.3 The {item type definition} must be validly derived 
	    * from the {base type definition}'s {item type definition} given
	    * the empty set, as defined in Type Derivation OK (Simple) (�3.14.6).
	    */
	    {
		xmlSchemaTypePtr baseItemType;

		baseItemType = type->baseType->subtypes;
		if ((baseItemType == NULL) || (! IS_SIMPLE_TYPE(baseItemType))) {
		    xmlSchemaPErr(ctxt, type->node,
			XML_ERR_INTERNAL_ERROR,
			"Internal error: xmlSchemaCheckCOSSTRestricts, "
			"List simple type '%s': Failed to "
			"evaluate the item type of its base type '%s'.\n",
			type->name, type->baseType->name);
		    return (-1);
		}
		if ((itemType != baseItemType) &&
		    (xmlSchemaCheckCOSSTDerivedOK(ctxt->schema, itemType,
		    baseItemType, 0) != 0)) {
		    xmlChar *strBIT = NULL, *strBT = NULL;
		    xmlSchemaPCustomErrExt(ctxt,
			XML_SCHEMAP_COS_ST_RESTRICTS_2_3_2_3,
			NULL, type, NULL,	
			"The item type '%s' is not validly derived from the "
			"item type '%s' of the base type '%s'",
			xmlSchemaGetComponentQName(&str, itemType),
			xmlSchemaGetComponentQName(&strBIT, baseItemType),
			xmlSchemaGetComponentQName(&strBT, type->baseType));

		    FREE_AND_NULL(str)
		    FREE_AND_NULL(strBIT)
		    FREE_AND_NULL(strBT)		    
		    return (XML_SCHEMAP_COS_ST_RESTRICTS_2_3_2_3);
		}
	    }
	    
	    if (type->facets != NULL) {
		xmlSchemaFacetPtr facet;
		int ok = 1;
		/* 
		* 2.3.2.4 Only length, minLength, maxLength, whiteSpace, pattern 
		* and enumeration facet components are allowed among the {facets}.
		*/
		facet = type->facets;
		do {
		    switch (facet->type) {
			case XML_SCHEMA_FACET_LENGTH:
			case XML_SCHEMA_FACET_MINLENGTH:
			case XML_SCHEMA_FACET_MAXLENGTH:
			case XML_SCHEMA_FACET_WHITESPACE:
			    /*
			    * TODO: 2.5.1.2 List datatypes
			    * The value of �whiteSpace� is fixed to the value collapse. 
			    */
			case XML_SCHEMA_FACET_PATTERN:
			case XML_SCHEMA_FACET_ENUMERATION:
			    break;
			default: {
			    xmlSchemaPIllegalFacetListUnionErr(ctxt,
				XML_SCHEMAP_COS_ST_RESTRICTS_2_3_2_4,
				NULL, type, facet);
			    /*
			    * We could return, but it's nicer to report all 
			    * invalid facets.
			    */
			    ok = 0;			    
			}
		    }		    
		    facet = facet->next;
		} while (facet != NULL);
		if (ok == 0)
		    return (XML_SCHEMAP_COS_ST_RESTRICTS_2_3_2_4);
		/*
		* TODO: 2.3.2.5 For each facet in the {facets} (call this DF), if there
		* is a facet of the same kind in the {facets} of the {base type 
		* definition} (call this BF),then the DF's {value} must be a valid 
		* restriction of BF's {value} as defined in [XML Schemas: Datatypes].
		*/
	    }	    
	    

	}
    } else if (type->flags & XML_SCHEMAS_TYPE_VARIETY_UNION) {
	/*
	* 3.1 The {member type definitions} must all have {variety} of 
	* atomic or list.
	*/
	xmlSchemaTypeLinkPtr member;

	member = type->memberTypes;
	while (member != NULL) {
	    if (IS_NOT_TYPEFIXED(member->type))
		xmlSchemaTypeFixup(member->type, ctxt, NULL);

	    if (((member->type->flags & 
		XML_SCHEMAS_TYPE_VARIETY_ATOMIC) == 0) && 
		((member->type->flags & 
		XML_SCHEMAS_TYPE_VARIETY_LIST) == 0)) {
		xmlSchemaPCustomErr(ctxt,
		    XML_SCHEMAP_COS_ST_RESTRICTS_3_1,
		    NULL, type, NULL,
		    "The member type '%s' is neither an atomic, nor a list type",
		    xmlSchemaGetComponentQName(&str, member->type));
		FREE_AND_NULL(str)
		return (XML_SCHEMAP_COS_ST_RESTRICTS_3_1);
	    }
	    member = member->next;
	}
	/*
	* 3.3.1 If the {base type definition} is the �simple ur-type 
	* definition� 
	*/
	if (type->baseType->builtInType == XML_SCHEMAS_ANYSIMPLETYPE) {
	    /*
	    * 3.3.1.1 All of the {member type definitions} must have a 
	    * {final} which does not contain union.
	    */
	    member = type->memberTypes;
	    while (member != NULL) {
		if (xmlSchemaTypeFinalContains(ctxt->schema, member->type, 
		    XML_SCHEMAS_TYPE_FINAL_UNION)) {
		    xmlSchemaPCustomErr(ctxt,
			XML_SCHEMAP_COS_ST_RESTRICTS_3_3_1,
			NULL, type, NULL,
			"The 'final' of member type '%s' contains 'union'",
			xmlSchemaGetComponentQName(&str, member->type));
		    FREE_AND_NULL(str)		   
		    return (XML_SCHEMAP_COS_ST_RESTRICTS_3_3_1);
		}
		member = member->next;
	    }
	    /*
	    * 3.3.1.2 The {facets} must be empty.
	    */
	    if (type->facetSet != NULL) {
		xmlSchemaPCustomErr(ctxt,
		    XML_SCHEMAP_COS_ST_RESTRICTS_3_3_1_2,
		    NULL, type, NULL, 
		    "No facets allowed", NULL);
		return (XML_SCHEMAP_COS_ST_RESTRICTS_3_3_1_2);
	    }
	} else {
	    /*
	    * 3.3.2.1 The {base type definition} must have a {variety} of union.
	    * I.e. the variety of "list" is inherited.
	    */
	    if ((type->baseType->flags & XML_SCHEMAS_TYPE_VARIETY_UNION) == 0) {
		xmlSchemaPCustomErr(ctxt,
		    XML_SCHEMAP_COS_ST_RESTRICTS_3_3_2_1,
		    NULL, type, NULL,
		    "The base type '%s' is not a union type",
		    xmlSchemaGetComponentQName(&str, type->baseType));
		FREE_AND_NULL(str)			
		return (XML_SCHEMAP_COS_ST_RESTRICTS_3_3_2_1);
	    }
	    /*
	    * 3.3.2.2 The {final} of the {base type definition} must not contain restriction.
	    */
	    if (xmlSchemaTypeFinalContains(ctxt->schema, type->baseType, 
		XML_SCHEMAS_TYPE_FINAL_RESTRICTION)) {
		xmlSchemaPCustomErr(ctxt,
		    XML_SCHEMAP_COS_ST_RESTRICTS_3_3_2_2,
		    NULL, type, NULL,
		    "The 'final' of its base type '%s' must not contain 'restriction'",
		    xmlSchemaGetComponentQName(&str, type->baseType));
		FREE_AND_NULL(str)		
		return (XML_SCHEMAP_COS_ST_RESTRICTS_3_3_2_2);
	    }
	    /*
	    * 3.3.2.3 The {member type definitions}, in order, must be validly 
	    * derived from the corresponding type definitions in the {base 
	    * type definition}'s {member type definitions} given the empty set, 
	    * as defined in Type Derivation OK (Simple) (�3.14.6).
	    */
	    {
		xmlSchemaTypeLinkPtr baseMember;

		/*
		* OPTIMIZE: if the type is restricting, it has no local defined 
		* member types and inherits the member types of the base type; 
		* thus a check for equality can be skipped.
		*/
		/*
		* Even worse: I cannot see a scenario where a restricting
		* union simple type can have other member types as the member 
		* types of it's base type. This check seems not necessary with
		* respect to the derivation process in libxml2.
		* But necessary if constructing types with an API.
		*/
		if (type->memberTypes != NULL) {
		    member = type->memberTypes;
		    baseMember = xmlSchemaGetUnionSimpleTypeMemberTypes(type->baseType);
		    if ((member == NULL) && (baseMember != NULL)) {		   
			xmlSchemaPErr(ctxt, type->node,
			    XML_SCHEMAP_INTERNAL,
			    "Internal error: xmlSchemaCheckCOSSTRestricts, "
			    "(3.3.2.3), union simple type '%s', unequal number "
			    "of member types in the base type\n",
			    type->name, NULL);
		    }		
		    while (member != NULL) {
			if (baseMember == NULL) {
			    xmlSchemaPErr(ctxt, type->node,
				XML_SCHEMAP_INTERNAL,
				"Internal error: "
				"xmlSchemaCheckCOSSTRestricts "
				"(3.3.2.3), union simple type '%s', unequal number "
				"of member types in the base type.\n",
				type->name, NULL);
			}
			if ((member->type != baseMember->type) &&
			    (xmlSchemaCheckCOSSTDerivedOK(ctxt->schema, 
			    member->type, baseMember->type, 0) != 0)) {
			    xmlChar *strBMT = NULL, *strBT = NULL;

			    xmlSchemaPCustomErrExt(ctxt,
				XML_SCHEMAP_COS_ST_RESTRICTS_3_3_2_3,
				NULL, type, NULL,
				"The member type %s is not validly derived from its "
				"corresponding member type %s of the base type %s",
				xmlSchemaGetComponentQName(&str, member->type),
				xmlSchemaGetComponentQName(&strBMT, baseMember->type),
				xmlSchemaGetComponentQName(&strBT, type->baseType));
			    FREE_AND_NULL(str)
			    FREE_AND_NULL(strBMT)
			    FREE_AND_NULL(strBT)
			    return (XML_SCHEMAP_COS_ST_RESTRICTS_3_3_2_3);
			}		
			member = member->next;
			baseMember = baseMember->next;
		    }
		}
	    }
	    /*
	    * 3.3.2.4 Only pattern and enumeration facet components are 
	    * allowed among the {facets}.
	    */	    
	    if (type->facets != NULL) {
		xmlSchemaFacetPtr facet;
		int ok = 1;

		facet = type->facets;
		do {
		    if ((facet->type != XML_SCHEMA_FACET_PATTERN) &&
			(facet->type != XML_SCHEMA_FACET_ENUMERATION)) {
			xmlSchemaPIllegalFacetListUnionErr(ctxt,
				XML_SCHEMAP_COS_ST_RESTRICTS_3_3_2_4,
				NULL, type, facet);			
			ok = 0;			    
		    }		    
		    facet = facet->next;
		} while (facet != NULL);
		if (ok == 0)
		    return (XML_SCHEMAP_COS_ST_RESTRICTS_3_3_2_4);
		    
	    }
	    /*
	    * TODO: 3.3.2.5 (facet derivation)
	    */
	}
    }

    return (0);
}

/**
 * xmlSchemaCheckSRCSimpleType:
 * @ctxt:  the schema parser context
 * @type:  the simple type definition
 *
 * Checks crc-simple-type constraints.
 *
 * Returns 0 if the constraints are satisfied,
 * if not a positive error code and -1 on internal
 * errors.
 */
static int
xmlSchemaCheckSRCSimpleType(xmlSchemaParserCtxtPtr ctxt,
			    xmlSchemaTypePtr type)
{   
    /* 
    * src-simple-type.1 The corresponding simple type definition, if any,
    * must satisfy the conditions set out in Constraints on Simple Type 
    * Definition Schema Components (�3.14.6).    
    */
    if ((xmlSchemaCheckSTPropsCorrect(ctxt, type) != 0) ||
	(xmlSchemaCheckCOSSTRestricts(ctxt, type) != 0)) {
	/*
	* TODO: Removed this, since it got annoying to get an
	* extra error report, if anything failed until now.
	* Enable this if needed.
	*/
	/*
	xmlSchemaPErr(ctxt, type->node,
	    XML_SCHEMAP_SRC_SIMPLE_TYPE_1,
	    "Simple type '%s' does not satisfy the constraints "
	    "on simple type definitions.\n",
	    type->name, NULL);
	*/
	return (XML_SCHEMAP_SRC_SIMPLE_TYPE_1);
    }

    if (type->flags & XML_SCHEMAS_TYPE_DERIVATION_METHOD_RESTRICTION) {
	/*
	* src-simple-type.2 If the <restriction> alternative is chosen, 
	* either it must have a base [attribute] or a <simpleType> among its 
	* [children], but not both.
	*/	
	/*
	* XML_SCHEMAP_SRC_SIMPLE_TYPE_2
	* NOTE: This is checked in the parse function of <restriction>.
	*/	
    } else if (type->flags & XML_SCHEMAS_TYPE_VARIETY_LIST) {
	/* src-simple-type.3 If the <list> alternative is chosen, either it must have 
	* an itemType [attribute] or a <simpleType> among its [children], 
	* but not both.
	*
	* REMOVED: This is checked in the parse function of <list>.
	*/   
    } else if (type->flags & XML_SCHEMAS_TYPE_VARIETY_UNION) {
	xmlSchemaTypeLinkPtr member;
	xmlSchemaTypePtr ancestor, anySimpleType;

	anySimpleType = xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYSIMPLETYPE);

	/* src-simple-type.4 Circular union type definition is disallowed. That is, if 
	* the <union> alternative is chosen, there must not be any entries 
	* in the memberTypes [attribute] at any depth which resolve to the 
	* component corresponding to the <simpleType>.
	*/	
	member = type->memberTypes;
	while (member != NULL) {
	    ancestor = member->type;
	    while ((ancestor != NULL) && (ancestor->type != XML_SCHEMA_TYPE_BASIC)) {		
		if (ancestor == type) {
		    xmlSchemaPCustomErr(ctxt, 
			XML_SCHEMAP_SRC_SIMPLE_TYPE_4,
			NULL, type, NULL,
			"The definition is circular", NULL);
		    return (XML_SCHEMAP_SRC_SIMPLE_TYPE_4);
		}
		if (IS_NOT_TYPEFIXED(ancestor))
		    xmlSchemaTypeFixup(ancestor, ctxt,  NULL);
		if (ancestor->flags & XML_SCHEMAS_TYPE_VARIETY_LIST) {
		    /*
		    * TODO, FIXME: Although a list simple type must not have a union ST
		    * type as item type, which in turn has a list ST as member 
		    * type, we will assume this here as well, since this check 
		    * was not yet performed.
		    */
		}
		
		ancestor = ancestor->baseType;
	    }   
	    member = member->next;
	}
    }

    return (0);
}

#if 0 /* Not yet used code for ST schema validation */
static int
xmlSchemaCheckCVCSimpleType(xmlSchemaValidCtxtPtr ctxt,
			    const xmlChar * value,
			    xmlSchemaTypePtr type,
			    int fireErrors)
{
    int ret;
    /*
    * 3.14.4 Simple Type Definition Validation Rules
    * Validation Rule: String Valid
    */
    /*
    * 1 It is schema-valid with respect to that definition as defined 
    * by Datatype Valid in [XML Schemas: Datatypes].
    */
    ret = xmlSchemaValidateSimpleTypeValue(ctxt, type, value, 
	fireErrors, 1, 1, 1);
    return (ret);
    /*
    * 2.1 If The definition is ENTITY or is validly derived from ENTITY given 
    * the empty set, as defined in Type Derivation OK (Simple) (�3.14.6), then
    * the string must be a �declared entity name�.
    */
    /*
    * 2.2 If The definition is ENTITIES or is validly derived from ENTITIES 
    * given the empty set, as defined in Type Derivation OK (Simple) (�3.14.6), 
    * then every whitespace-delimited substring of the string must be a �declared 
    * entity name�.
    */
    /*
    * 2.3 otherwise no further condition applies.
    */

    return (0);
}
#endif


static int
xmlSchemaCreatePCtxtOnVCtxt(xmlSchemaValidCtxtPtr vctxt)
{
    if (vctxt->pctxt == NULL) {
        if (vctxt->schema != NULL)
	    vctxt->pctxt = xmlSchemaNewParserCtxtUseDict("*", vctxt->schema->dict);
	else
	    vctxt->pctxt = xmlSchemaNewParserCtxt("*");
	if (vctxt->pctxt == NULL) {
	    xmlSchemaVErr(vctxt, NULL,
		XML_SCHEMAV_INTERNAL,
		"Internal error: xmlSchemaCreatePCtxtOnVCtxt, "
		"failed to create a temp. parser context.\n",
		NULL, NULL);
	    return (-1);
	}
	/* TODO: Pass user data. */
	xmlSchemaSetParserErrors(vctxt->pctxt, vctxt->error, vctxt->warning, NULL);	
    }
    return (0);
}

static int
xmlSchemaCreateVCtxtOnPCtxt(xmlSchemaParserCtxtPtr ctxt)
{
   if (ctxt->vctxt == NULL) {
	ctxt->vctxt = xmlSchemaNewValidCtxt(NULL);
	if (ctxt->vctxt == NULL) {
	    xmlSchemaPErr(ctxt, NULL,
		XML_SCHEMAP_INTERNAL,
		"Internal error: xmlSchemaCreatePCtxtOnVCtxt, "
		"failed to create a temp. validation context.\n",
		NULL, NULL);
	    return (-1);
	}
	/* TODO: Pass user data. */
	xmlSchemaSetValidErrors(ctxt->vctxt, ctxt->error, ctxt->warning, NULL);	
    }
    return (0);
}

/**
 * xmlSchemaCheckCOSValidDefault:
 * @pctxt:  the schema parser context
 * @vctxt:  the temporary schema validation context
 * @type:  the simple type definition
 * @value: the default value
 * @node: an optional node (the holder of the value)
 *
 * Checks the "cos-valid-default" constraints.
 *
 * Returns 0 if the constraints are satisfied,
 * if not, a positive error code and -1 on internal
 * errors.
 */
static int
xmlSchemaCheckCOSValidDefault(xmlSchemaParserCtxtPtr pctxt,
			      xmlSchemaValidCtxtPtr vctxt,
			      xmlSchemaTypePtr type,
			      const xmlChar *value,
			      xmlNodePtr node)
{   
    int ret = 0;

    /*
    * cos-valid-default:
    * Schema Component Constraint: Element Default Valid (Immediate)
    * For a string to be a valid default with respect to a type 
    * definition the appropriate case among the following must be true:
    */
    /*
    * NOTE: This has to work without a given node (the holder of the
    * value), since it should work on the component, i.e. an underlying
    * DOM must not be mandatory.
    */     
    if ((pctxt == NULL) || (vctxt == NULL)) {
	xmlSchemaPErr(pctxt, node,
	    XML_SCHEMAP_INTERNAL,
	    "Internal error: xmlSchemaCheckCOSValidDefault, "
	    "bad arguments: the parser and/or validation context is "
	    "missing.\n",
	    NULL, NULL);
	return (-1);	
    }       
    if IS_COMPLEX_TYPE(type) {
	/*
	* Complex type.
	*
	* 2.1 its {content type} must be a simple type definition or mixed.
	*/
	/* 
	* TODO: Adjust this when the content type will be computed 
	* correctly. 
	*/
	if ((type->contentType != XML_SCHEMA_CONTENT_SIMPLE) &&
	    (type->contentType != XML_SCHEMA_CONTENT_BASIC) &&
	    (type->contentType != XML_SCHEMA_CONTENT_MIXED)) {
	    xmlSchemaPSimpleTypeErr(pctxt, 
		XML_SCHEMAP_COS_VALID_DEFAULT_2_1,
		NULL, NULL, node,
		type, NULL, NULL,
		"If the type of a constraint value is complex, its content "
		"type must be mixed or a simple type",
		NULL, NULL);
	    return(XML_SCHEMAP_COS_VALID_DEFAULT_2_1);
	}
	if (type->contentType == XML_SCHEMA_CONTENT_MIXED) {
	    /*
	    * 2.2.2 If the {content type} is mixed, then the {content type}'s 
	    * particle must be �emptiable� as defined by Particle Emptiable 
	    * (�3.9.6).
	    */
	    
	    /*
	    * URGENT TODO: Implement this.
	    */
	    return (0);
	}
    }	
    /*
    * 1 If the type definition is a simple type definition, then the string 
    * must be �valid� with respect to that definition as defined by String 
    * Valid (�3.14.4).
    *
    * AND
    *
    * 2.2.1 If the {content type} is a simple type definition, then the 
    * string must be �valid� with respect to that simple type definition 
    * as defined by String Valid (�3.14.4).
    */    
    vctxt->node = node;
    vctxt->cur = NULL;
    ret = xmlSchemaValidateSimpleTypeValue(vctxt, type, value, 1, 1, 1, 0);
    /* ret = xmlSchemaCheckCVCSimpleType(vctxt, elemDecl->value, typeDef, 0); */   
    if (ret < 0) {
	xmlSchemaPErr(pctxt, node,
	/* NOTNICE: error code: This function will be used during
	* schema construction and xsi:type validation.
	*/
	XML_SCHEMAP_INTERNAL,
	"Internal error: xmlSchemaCheckCOSValidDefault, "
	"while validating a value constaint value.\n",
	NULL, NULL);

    }     	    
    return (ret);
}

/**
 * xmlSchemaCheckCTPropsCorrect:
 * @ctxt:  the schema parser context
 * @type:  the complex type definition
 *
 *.(4.6) Constraints on Complex Type Definition Schema Components
 * Schema Component Constraint:
 * Complex Type Definition Properties Correct (ct-props-correct)
 * STATUS: (seems) complete
 *
 * Returns 0 if the constraints are satisfied, a positive
 * error code if not and -1 if an internal error occured.
 */
static int
xmlSchemaCheckCTPropsCorrect(xmlSchemaParserCtxtPtr pctxt,
			     xmlSchemaTypePtr type)
{
    /*
    * TODO: Correct the error code; XML_SCHEMAP_SRC_CT_1 is used temporarily.
    *
    * SPEC (1) "The values of the properties of a complex type definition must
    * be as described in the property tableau in The Complex Type Definition
    * Schema Component (�3.4.1), modulo the impact of Missing 
    * Sub-components (�5.3)."
    */
    if ((type->baseType != NULL) &&
	(IS_SIMPLE_TYPE(type->baseType)) &&
	((type->flags & XML_SCHEMAS_TYPE_DERIVATION_METHOD_EXTENSION) == 0)) {
	/*
	* SPEC (2) "If the {base type definition} is a simple type definition,
	* the {derivation method} must be extension."
	*/
	xmlSchemaPCustomErr(pctxt,
	    XML_SCHEMAP_SRC_CT_1,
	    NULL, type, NULL,
	    "If the base type is a simple type, the derivation method must be "
	    "'extension'", NULL);
	return (XML_SCHEMAP_SRC_CT_1);
    }	
    /*
    * SPEC (3) "Circular definitions are disallowed, except for the �ur-type
    * definition�. That is, it must be possible to reach the �ur-type
    * definition by repeatedly following the {base type definition}."
    *
    * NOTE (3) is done in xmlSchemaCheckTypeDefCircular().
    *
    * SPEC (4) "Two distinct attribute declarations in the {attribute uses}
    * must not have identical {name}s and {target namespace}s."    
    * SPEC (5) "Two distinct attribute declarations in the {attribute uses}
    * must not have {type definition}s which are or are derived from ID."
    *
    * NOTE (4) and (5) are done in xmlSchemaBuildAttributeValidation().
    */
    return (0);
}

/**
 * xmlSchemaCheckCOSCTExtends:
 * @ctxt:  the schema parser context
 * @type:  the complex type definition
 *
 * (3.4.6) Constraints on Complex Type Definition Schema Components
 * Schema Component Constraint: 
 * Derivation Valid (Extension) (cos-ct-extends)
 *
 * STATUS:
 *   missing:  
 *     (1.5)
 *     (1.4.3.2.2.2) "Particle Valid (Extension)", which is not really needed.
 *
 * Returns 0 if the constraints are satisfied, a positive
 * error code if not and -1 if an internal error occured.
 */
static int
xmlSchemaCheckCOSCTExtends(xmlSchemaParserCtxtPtr ctxt,
			   xmlSchemaTypePtr type)
{
    xmlSchemaTypePtr base = type->baseType;
    /*
    * TODO: Correct the error code; XML_SCHEMAP_COS_CT_EXTENDS_1_1 is used
    * temporarily only.
    */
    /* 
    * SPEC (1) "If the {base type definition} is a complex type definition, 
    * then all of the following must be true:"
    */
    if (base->type == XML_SCHEMA_TYPE_COMPLEX) {
	/*
	* SPEC (1.1) "The {final} of the {base type definition} must not 
	* contain extension."
	*/
	if (base->flags & XML_SCHEMAS_TYPE_FINAL_EXTENSION) {
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_COS_CT_EXTENDS_1_1,
		NULL, type, NULL,
		"The 'final' of the base type definition "
		"contains 'extension'", NULL);
	    return (XML_SCHEMAP_COS_CT_EXTENDS_1_1);
	}
	/*
	* SPEC (1.2) "Its {attribute uses} must be a subset of the {attribute
	* uses} 
	* of the complex type definition itself, that is, for every attribute 
	* use in the {attribute uses} of the {base type definition}, there 
	* must be an attribute use in the {attribute uses} of the complex 
	* type definition itself whose {attribute declaration} has the same 
	* {name}, {target namespace} and {type definition} as its attribute 
	* declaration"
	*
	* NOTE (1.2): This will be already satisfied by the way the attribute
	* uses are extended in xmlSchemaBuildAttributeValidation(); thus this
	* check is not needed.
	*/

	/*
	* SPEC (1.3) "If it has an {attribute wildcard}, the complex type
	* definition must also have one, and the base type definition's
	* {attribute  wildcard}'s {namespace constraint} must be a subset
	* of the complex  type definition's {attribute wildcard}'s {namespace
	* constraint}, as defined by Wildcard Subset (�3.10.6)."
	*
	* NOTE (1.3) This is already checked in 
	* xmlSchemaBuildAttributeValidation; thus this check is not needed.
	*
	* SPEC (1.4) "One of the following must be true:"
	*/
	if ((type->contentTypeDef != NULL) &&
	    (type->contentTypeDef == base->contentTypeDef)) {
	    /*
	    * SPEC (1.4.1) "The {content type} of the {base type definition}
	    * and the {content type} of the complex type definition itself
	    * must be the same simple type definition"
	    * PASS
	    */
	} else if ((type->contentType == XML_SCHEMA_CONTENT_EMPTY) &&
	    (base->contentType == XML_SCHEMA_CONTENT_EMPTY) ) {
	    /*
	    * SPEC (1.4.2) "The {content type} of both the {base type
	    * definition} and the complex type definition itself must
	    * be empty."
	    * PASS
	    */
	} else {
	    /*
	    * SPEC (1.4.3) "All of the following must be true:"
	    */
	    if (type->subtypes == NULL) {
		/*
		* SPEC 1.4.3.1 The {content type} of the complex type 
		* definition itself must specify a particle.
		*/
		xmlSchemaPCustomErr(ctxt,
		    XML_SCHEMAP_COS_CT_EXTENDS_1_1,
		    NULL, type, NULL,
		    "The content type must specify a particle", NULL);
		return (XML_SCHEMAP_COS_CT_EXTENDS_1_1);
	    }
	    /*
	    * SPEC (1.4.3.2) "One of the following must be true:"
	    */
	    if (base->contentType == XML_SCHEMA_CONTENT_EMPTY) {
		/*
		* SPEC (1.4.3.2.1) "The {content type} of the {base type
		* definition} must be empty.
		* PASS
		*/
	    } else {
		/*
		* SPEC (1.4.3.2.2) "All of the following must be true:"
		*/
		if ((type->contentType != base->contentType) ||
		    ((type->contentType != XML_SCHEMA_CONTENT_MIXED) &&
		    (type->contentType != XML_SCHEMA_CONTENT_ELEMENTS))) {
		    /*
		    * SPEC (1.4.3.2.2.1) "Both {content type}s must be mixed
		    * or both must be element-only."
		    */
		    xmlSchemaPCustomErr(ctxt,
			XML_SCHEMAP_COS_CT_EXTENDS_1_1,
			NULL, type, NULL,
			"The content type of both, the type and its base "
			"type, must either 'mixed' or 'element-only'", NULL);
		    return (XML_SCHEMAP_COS_CT_EXTENDS_1_1);
		}		
		/*
		* FUTURE TODO SPEC (1.4.3.2.2.2) "The particle of the
		* complex type definition must be a �valid extension�
		* of the {base type definition}'s particle, as defined
		* in Particle Valid (Extension) (�3.9.6)."
		*
		* NOTE that we won't check "Particle Valid (Extension)",
		* since it is ensured by the derivation process in
		* xmlSchemaTypeFixup(). We need to implement this when heading
		* for a construction API
		*/		    		
	    }
	    /*
	    * TODO (1.5)
	    */
	}	
    } else {
	/*
	* SPEC (2) "If the {base type definition} is a simple type definition, 
	* then all of the following must be true:"
	*/
	if (type->contentTypeDef != base) {
	    /*
	    * SPEC (2.1) "The {content type} must be the same simple type
	    * definition."
	    */
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_COS_CT_EXTENDS_1_1,
		NULL, type, NULL,
		"The content type must be the simple base type", NULL);
	    return (XML_SCHEMAP_COS_CT_EXTENDS_1_1);
	}
	if (base->flags & XML_SCHEMAS_TYPE_FINAL_EXTENSION) {
	    /*
	    * SPEC (2.2) "The {final} of the {base type definition} must not
	    * contain extension"
	    * NOTE that this is the same as (1.1).
	    */
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_COS_CT_EXTENDS_1_1,
		NULL, type, NULL,
		"The 'final' of the base type definition "
		"contains 'extension'", NULL);
	    return (XML_SCHEMAP_COS_CT_EXTENDS_1_1);
	}	
    }
    return (0);
}

/**
 * xmlSchemaCheckDerivationOKRestriction:
 * @ctxt:  the schema parser context
 * @type:  the complex type definition
 *
 * (3.4.6) Constraints on Complex Type Definition Schema Components
 * Schema Component Constraint: 
 * Derivation Valid (Restriction, Complex) (derivation-ok-restriction)
 *
 * STATUS:
 *   missing: 
 *     (5.4.2), (5.2.2.1)
 *
 * Returns 0 if the constraints are satisfied, a positive
 * error code if not and -1 if an internal error occured.
 */
static int
xmlSchemaCheckDerivationOKRestriction(xmlSchemaParserCtxtPtr ctxt,
				      xmlSchemaTypePtr type)
{
    xmlSchemaTypePtr base;
    
    /*
    * TODO: Correct the error code; XML_SCHEMAP_COS_CT_EXTENDS_1_1 is used
    * temporarily only.
    */    
    base = type->baseType;
    if (base->flags & XML_SCHEMAS_TYPE_FINAL_RESTRICTION) {
	/*
	* SPEC (1) "The {base type definition} must be a complex type
	* definition whose {final} does not contain restriction."
	*/
	xmlSchemaPCustomErr(ctxt,
	    XML_SCHEMAP_COS_CT_EXTENDS_1_1,
	    NULL, type, NULL,
	    "The 'final' of the base type definition "
	    "contains 'restriction'", NULL);
	return (XML_SCHEMAP_COS_CT_EXTENDS_1_1);
    }
    /*
    * NOTE (3) and (4) are done in xmlSchemaBuildAttributeValidation().
    *
    * SPEC (5) "One of the following must be true:"
    */
    if (base->builtInType == XML_SCHEMAS_ANYTYPE) {
	/*
	* SPEC (5.1) "The {base type definition} must be the
	* �ur-type definition�."
	* PASS
	*/
    } else if ((type->contentType == XML_SCHEMA_CONTENT_SIMPLE) ||
	    (type->contentType == XML_SCHEMA_CONTENT_BASIC)) {
	/*
	* SPEC (5.2.1) "The {content type} of the complex type definition
	* must be a simple type definition"
	*
	* SPEC (5.2.2) "One of the following must be true:"
	*/
	if ((base->contentType == XML_SCHEMA_CONTENT_SIMPLE) ||
	    (base->contentType == XML_SCHEMA_CONTENT_BASIC)) {
	    /*
	    * SPEC (5.2.2.1) "The {content type} of the {base type
	    * definition} must be a simple type definition from which
	    * the {content type} is validly derived given the empty
	    * set as defined in Type Derivation OK (Simple) (�3.14.6)."
	    * URGENT TODO
	    */	    
	} else if ((base->contentType == XML_SCHEMA_CONTENT_MIXED) &&
	    (xmlSchemaIsParticleEmptiable(
		(xmlSchemaParticlePtr) base->subtypes))) {		
	    /*
	    * SPEC (5.2.2.2) "The {base type definition} must be mixed
	    * and have a particle which is �emptiable� as defined in
	    * Particle Emptiable (�3.9.6)."
	    * PASS
	    */
	} else {
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_COS_CT_EXTENDS_1_1,
		NULL, type, NULL,
		"The content type of the base type must be either "
		"a simple type or 'mixed' and an emptiable particle", NULL);
	    return (XML_SCHEMAP_COS_CT_EXTENDS_1_1);
	}
    } else if (type->contentType == XML_SCHEMA_CONTENT_EMPTY) {
	/*
	* SPEC (5.3.1) "The {content type} of the complex type itself must
	* be empty"
	*/
	if (base->contentType == XML_SCHEMA_CONTENT_EMPTY) {
	    /*
	    * SPEC (5.3.2.1) "The {content type} of the {base type 
	    * definition} must also be empty."
	    * PASS
	    */
	} else if (((base->contentType == XML_SCHEMA_CONTENT_ELEMENTS) ||
	    (base->contentType == XML_SCHEMA_CONTENT_MIXED)) &&
	    xmlSchemaIsParticleEmptiable(
		(xmlSchemaParticlePtr) base->subtypes)) {
	    /*
	    * SPEC (5.3.2.2) "The {content type} of the {base type
	    * definition} must be elementOnly or mixed and have a particle
	    * which is �emptiable� as defined in Particle Emptiable (�3.9.6)."
	    * PASS
	    */
	} else {
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_COS_CT_EXTENDS_1_1,
		NULL, type, NULL,
		"The content type of the base type must be either "
		"empty or 'mixed' (or 'elements-only') and an emptiable "
		"particle", NULL);
	    return (XML_SCHEMAP_COS_CT_EXTENDS_1_1);
	}
    } else if ((type->contentType == XML_SCHEMA_CONTENT_ELEMENTS) ||
	((type->contentType == XML_SCHEMA_CONTENT_MIXED) &&
	 (base->contentType == XML_SCHEMA_CONTENT_MIXED))) {
	/*
	* SPEC (5.4.1.1) "The {content type} of the complex type definition
	* itself must be element-only"
	* SPEC (5.4.1.2) "The {content type} of the complex type definition
	* itself and of the {base type definition} must be mixed"
	*/
	/*
	* SPEC (5.4.2) "The particle of the complex type definition itself
	* must be a �valid restriction� of the particle of the {content
	* type} of the {base type definition} as defined in Particle Valid
	* (Restriction) (�3.9.6).
	* URGENT TODO
	*/	
    } else {
	xmlSchemaPCustomErr(ctxt,
	    XML_SCHEMAP_COS_CT_EXTENDS_1_1,
	    NULL, type, NULL,
	    "The type is not a valid restriction of its base type", NULL);
	return (XML_SCHEMAP_COS_CT_EXTENDS_1_1);
    }
    return (0);
}

/**
 * xmlSchemaCheckCTPropertiesCorrect:
 * @ctxt:  the schema parser context
 * @type:  the complex type definition
 *
 * (3.4.6) Constraints on Complex Type Definition Schema Components
 *
 * Returns 0 if the constraints are satisfied, a positive
 * error code if not and -1 if an internal error occured.
 */
static int
xmlSchemaCheckCTComponent(xmlSchemaParserCtxtPtr ctxt,
			  xmlSchemaTypePtr type)
{
    int ret;
    /*
    * Complex Type Definition Properties Correct
    */    
    ret = xmlSchemaCheckCTPropsCorrect(ctxt, type);
    if (ret != 0)
	return (ret);
    if (type->flags & XML_SCHEMAS_TYPE_DERIVATION_METHOD_EXTENSION)
	ret = xmlSchemaCheckCOSCTExtends(ctxt, type);
    else
	ret = xmlSchemaCheckDerivationOKRestriction(ctxt, type);
    return (ret);
}

/**
 * xmlSchemaCheckSRCCT:
 * @ctxt:  the schema parser context
 * @type:  the complex type definition
 *
 * (3.4.3) Constraints on XML Representations of Complex Type Definitions:
 * Schema Representation Constraint: 
 * Complex Type Definition Representation OK (src-ct)
 *
 * Returns 0 if the constraints are satisfied, a positive
 * error code if not and -1 if an internal error occured.
 */
static int
xmlSchemaCheckSRCCT(xmlSchemaParserCtxtPtr ctxt, 
		    xmlSchemaTypePtr type)
{
    xmlSchemaTypePtr base;
    int ret = 0;

    /*
    * TODO: Adjust the error codes here, as I used
    * XML_SCHEMAP_SRC_CT_1 only yet.
    */
    base = type->baseType;
    if (type->contentType != XML_SCHEMA_CONTENT_SIMPLE) {
	/*
	* 1 If the <complexContent> alternative is chosen, the type definition
	* �resolved� to by the �actual value� of the base [attribute] 
	* must be a complex type definition;
	*/
	if (! IS_COMPLEX_TYPE(base)) {
	    xmlChar *str = NULL;
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_SRC_CT_1,
		NULL, type, type->node,
		"If using <complexContent>, the base type is expected to be "
		"a complex type. The base type '%s' is a simple type",
		xmlSchemaFormatQName(&str, base->targetNamespace,
		base->name));
	    FREE_AND_NULL(str)
	    return (XML_SCHEMAP_SRC_CT_1);
	}
    } else {
	/*
	* SPEC
	* 2 If the <simpleContent> alternative is chosen, all of the
	* following must be true:
	* 2.1 The type definition �resolved� to by the �actual value� of the
	* base [attribute] must be one of the following:
	*/
	if (IS_SIMPLE_TYPE(base)) {
	    if ((type->flags & 
		XML_SCHEMAS_TYPE_DERIVATION_METHOD_EXTENSION) == 0) {
		xmlChar *str = NULL;
		/* 
		* 2.1.3 only if the <extension> alternative is also 
		* chosen, a simple type definition.
		*/
		/* TODO: Change error code to ..._SRC_CT_2_1_3. */
		xmlSchemaPCustomErr(ctxt,
		    XML_SCHEMAP_SRC_CT_1,
		    NULL, type, NULL,
		    "If using <simpleContent> and <restriction>, the base "
		    "type must be a complex type. The base type '%s' is "
		    "a simple type",
		    xmlSchemaFormatQName(&str, base->targetNamespace,
			base->name));
		FREE_AND_NULL(str)
		return (XML_SCHEMAP_SRC_CT_1);
	    }	   
	} else {
	    /* Base type is a complex type. */
	    if ((base->contentType == XML_SCHEMA_CONTENT_SIMPLE) ||
		(base->contentType == XML_SCHEMA_CONTENT_BASIC)) {
		/*
		* 2.1.1 a complex type definition whose {content type} is a 
		* simple type definition;
		* PASS
		*/
		if (base->contentTypeDef == NULL) {
		    xmlSchemaPCustomErr(ctxt, XML_SCHEMAP_INTERNAL, 
			NULL, type, NULL,
			"Internal error: xmlSchemaCheckSRCCT, "
			"'%s', base type has no content type", 
			type->name);
		    return (-1);
		}
	    } else if ((base->contentType == XML_SCHEMA_CONTENT_MIXED) &&
		(type->flags &
		    XML_SCHEMAS_TYPE_DERIVATION_METHOD_RESTRICTION)) {

		/*
		* 2.1.2 only if the <restriction> alternative is also 
		* chosen, a complex type definition whose {content type} 
		* is mixed and a particle emptiable.
		*/
		if (! xmlSchemaIsParticleEmptiable(
		    (xmlSchemaParticlePtr) base->subtypes)) {
		    ret = XML_SCHEMAP_SRC_CT_1;
		} else if ((type->contentTypeDef == NULL) ||
		    (type->contentTypeDef->baseType == NULL)) {
		    xmlChar *str = NULL;
		    /*
		    * 2.2 If clause 2.1.2 above is satisfied, then there 
		    * must be a <simpleType> among the [children] of 
		    * <restriction>.
		    */							
		    /* TODO: Change error code to ..._SRC_CT_2_2. */
		    xmlSchemaPCustomErr(ctxt,
			XML_SCHEMAP_SRC_CT_1,
			NULL, type, NULL,
			"A <simpleType> is expected among the children "
			"of <restriction>, if <simpleContent> is used and "
			"the base type '%s' is a complex type",
			xmlSchemaFormatQName(&str, base->targetNamespace,
			base->name));
		    FREE_AND_NULL(str)
		    return (XML_SCHEMAP_SRC_CT_1);
		}
	    } else {
		ret = XML_SCHEMAP_SRC_CT_1;		
	    }	    
	}
	if (ret > 0) {
	    xmlChar *str = NULL;
	    if (type->flags & XML_SCHEMAS_TYPE_DERIVATION_METHOD_RESTRICTION) {		
		xmlSchemaPCustomErr(ctxt,
		    XML_SCHEMAP_SRC_CT_1,
		    NULL, type, NULL,
		    "If <simpleContent> and <restriction> is used, the "
		    "base type must be a simple type or a complex type with "
		    "mixed content and particle emptiable. The base type "
		    "'%s' is none of those",
		    xmlSchemaFormatQName(&str, base->targetNamespace, 
		    base->name));
	    } else {
		xmlSchemaPCustomErr(ctxt,
		    XML_SCHEMAP_SRC_CT_1,
		    NULL, type, NULL,
		    "If <simpleContent> and <extension> is used, the "
		    "base type must be a simple type. The base type '%s' "
		    "is a complex type",
		    xmlSchemaFormatQName(&str, base->targetNamespace, 
		    base->name));
	    }
	    FREE_AND_NULL(str)
	}
    }
    /*
    * SPEC (3) "The corresponding complex type definition component must 
    * satisfy the conditions set out in Constraints on Complex Type 
    * Definition Schema Components (�3.4.6);"
    * NOTE (3) will be done in xmlSchemaTypeFixup().
    */   
    /*
    * SPEC (4) If clause 2.2.1 or clause 2.2.2 in the correspondence specification
    * above for {attribute wildcard} is satisfied, the intensional 
    * intersection must be expressible, as defined in Attribute Wildcard 
    * Intersection (�3.10.6).
    * NOTE (4) is done in xmlSchemaBuildAttributeValidation().
    */
    return (ret);
}

#define FACET_RESTR_MUTUAL_ERR(fac1, fac2) \
    xmlSchemaPCustomErrExt(pctxt,      \
	XML_SCHEMAP_INVALID_FACET_VALUE, \
	NULL, (xmlSchemaTypePtr) fac1, fac1->node, \
	"It is an error for both '%s' and '%s' to be specified on the "\
	"same type definition", \
	BAD_CAST xmlSchemaFacetTypeToString(fac1->type), \
	BAD_CAST xmlSchemaFacetTypeToString(fac2->type), NULL);

#define FACET_RESTR_ERR(fac1, msg) \
    xmlSchemaPCustomErr(pctxt,      \
	XML_SCHEMAP_INVALID_FACET_VALUE, \
	NULL, (xmlSchemaTypePtr) fac1, fac1->node, \
	msg, NULL); 

#define FACET_RESTR_FIXED_ERR(fac) \
    xmlSchemaPCustomErr(pctxt, \
	XML_SCHEMAP_INVALID_FACET_VALUE, \
	NULL, (xmlSchemaTypePtr) fac, fac->node, \
	"The base type's facet is 'fixed', thus the value must not " \
	"differ", NULL); 

static void
xmlSchemaDeriveFacetErr(xmlSchemaParserCtxtPtr pctxt,
			xmlSchemaFacetPtr facet1,
			xmlSchemaFacetPtr facet2,
			int lessGreater,
			int orEqual,
			int ofBase)
{
    xmlChar *msg = NULL;

    msg = xmlStrdup(BAD_CAST "'");
    msg = xmlStrcat(msg, xmlSchemaFacetTypeToString(facet1->type));
    msg = xmlStrcat(msg, BAD_CAST "' has to be");
    if (lessGreater == 0)
	msg = xmlStrcat(msg, BAD_CAST " equal to");
    if (lessGreater == 1)
	msg = xmlStrcat(msg, BAD_CAST " greater than");
    else
	msg = xmlStrcat(msg, BAD_CAST " less than");

    if (orEqual)
	msg = xmlStrcat(msg, BAD_CAST " or equal to");
    msg = xmlStrcat(msg, BAD_CAST " '");
    msg = xmlStrcat(msg, xmlSchemaFacetTypeToString(facet2->type));
    if (ofBase)
	msg = xmlStrcat(msg, BAD_CAST "' of the base type");
    else
	msg = xmlStrcat(msg, BAD_CAST "'");

    xmlSchemaPCustomErr(pctxt,
	XML_SCHEMAP_INVALID_FACET_VALUE,
	NULL, (xmlSchemaTypePtr) facet1, facet1->node,	
	(const char *) msg, NULL);

    if (msg != NULL)
	xmlFree(msg);
}

static int
xmlSchemaDeriveAndValidateFacets(xmlSchemaParserCtxtPtr pctxt,
				 xmlSchemaTypePtr type)
{
    xmlSchemaTypePtr base = type->baseType;
    xmlSchemaFacetLinkPtr link, cur, last = NULL;
    xmlSchemaFacetPtr facet, bfacet, 
	flength = NULL, ftotdig = NULL, ffracdig = NULL,
	fmaxlen = NULL, fminlen = NULL, /* facets of the current type */
	fmininc = NULL, fmaxinc = NULL,
	fminexc = NULL, fmaxexc = NULL,
	bflength = NULL, bftotdig = NULL, bffracdig = NULL,
	bfmaxlen = NULL, bfminlen = NULL, /* facets of the base type */
	bfmininc = NULL, bfmaxinc = NULL,
	bfminexc = NULL, bfmaxexc = NULL;
    int res, err = 0, fixedErr;
    /*
    * 3 The {facets} of R are the union of S and the {facets} 
    * of B, eliminating duplicates. To eliminate duplicates, 
    * when a facet of the same kind occurs in both S and the 
    * {facets} of B, the one in the {facets} of B is not 
    * included, with the exception of enumeration and pattern 
    * facets, for which multiple occurrences with distinct values 
    * are allowed.
    */
    if ((type->facetSet == NULL) && (base->facetSet == NULL))
	return (0);

    last = type->facetSet;
    if (last != NULL)
	while (last->next != NULL)
	    last = last->next;

    for (cur = type->facetSet; cur != NULL; cur = cur->next) {
	facet = cur->facet;
	switch (facet->type) {
	    case XML_SCHEMA_FACET_LENGTH:
		flength = facet; break;
	    case XML_SCHEMA_FACET_MINLENGTH:
		fminlen = facet; break;
	    case XML_SCHEMA_FACET_MININCLUSIVE:
		fmininc = facet; break;
	    case XML_SCHEMA_FACET_MINEXCLUSIVE:
		fminexc = facet; break;
	    case XML_SCHEMA_FACET_MAXLENGTH:
		fmaxlen = facet; break;
	    case XML_SCHEMA_FACET_MAXINCLUSIVE:
		fmaxinc = facet; break;
	    case XML_SCHEMA_FACET_MAXEXCLUSIVE:
		fmaxexc = facet; break;
	    case XML_SCHEMA_FACET_TOTALDIGITS:
		ftotdig = facet; break;
	    case XML_SCHEMA_FACET_FRACTIONDIGITS:
		ffracdig = facet; break;
	    default:
		break;
	}	
    }
    for (cur = base->facetSet; cur != NULL; cur = cur->next) {
	facet = cur->facet;
	switch (facet->type) {
	    case XML_SCHEMA_FACET_LENGTH:
		bflength = facet; break;
	    case XML_SCHEMA_FACET_MINLENGTH:
		bfminlen = facet; break;
	    case XML_SCHEMA_FACET_MININCLUSIVE:
		bfmininc = facet; break;
	    case XML_SCHEMA_FACET_MINEXCLUSIVE:
		bfminexc = facet; break;
	    case XML_SCHEMA_FACET_MAXLENGTH:
		bfmaxlen = facet; break;
	    case XML_SCHEMA_FACET_MAXINCLUSIVE:
		bfmaxinc = facet; break;
	    case XML_SCHEMA_FACET_MAXEXCLUSIVE:
		bfmaxexc = facet; break;
	    case XML_SCHEMA_FACET_TOTALDIGITS:
		bftotdig = facet; break;
	    case XML_SCHEMA_FACET_FRACTIONDIGITS:
		bffracdig = facet; break;
	    default:
		break;
	}	
    }
    err = 0;
    /*
    * length and minLength or maxLength (2.2) + (3.2)
    */
    if (flength && (fminlen || fmaxlen)) {
	FACET_RESTR_ERR(flength, "It is an error for both 'length' and "
	    "either of 'minLength' or 'maxLength' to be specified on "
	    "the same type definition")
    }
    /*
    * Mutual exclusions in the same derivation step.
    */
    if ((fmaxinc) && (fmaxexc)) {
	/* 
	* SCC "maxInclusive and maxExclusive" 
	*/
	FACET_RESTR_MUTUAL_ERR(fmaxinc, fmaxexc)
    }
    if ((fmininc) && (fminexc)) {
	/* 
	* SCC "minInclusive and minExclusive"
	*/
	FACET_RESTR_MUTUAL_ERR(fmininc, fminexc)
    }
    
    if (flength && bflength) {
	/*
	* SCC "length valid restriction" 
	* The values have to be equal.
	*/
	res = xmlSchemaCompareValues(flength->val, bflength->val);
	if (res == -2)
	    goto internal_error;
	/*
	* TODO: Maby a bug in the spec.
	*/
	if (res != 0)
	    xmlSchemaDeriveFacetErr(pctxt, flength, bflength, 0, 0, 1);
	if ((res != 0) && (bflength->fixed)) {
	    FACET_RESTR_FIXED_ERR(flength)
	}
	
    }
    if (fminlen && bfminlen) {
	/*
	* SCC "minLength valid restriction"
	* minLength >= BASE minLength
	*/
	res = xmlSchemaCompareValues(fminlen->val, bfminlen->val);
	if (res == -2)
	    goto internal_error;
	if (res == -1)
	    xmlSchemaDeriveFacetErr(pctxt, fminlen, bfminlen, 1, 1, 1);
	if ((res != 0) && (bfminlen->fixed)) {
	    FACET_RESTR_FIXED_ERR(fminlen)
	}	    
    }
    if (fmaxlen && bfmaxlen) {
	/*
	* SCC "maxLength valid restriction"
	* maxLength <= BASE minLength
	*/
	res = xmlSchemaCompareValues(fmaxlen->val, bfmaxlen->val);
	if (res == -2)
	    goto internal_error;
	if (res == 1)
	    xmlSchemaDeriveFacetErr(pctxt, fmaxlen, bfmaxlen, -1, 1, 1);
	if ((res != 0) && (bfmaxlen->fixed)) {
	    FACET_RESTR_FIXED_ERR(fmaxlen)
	}	    
    }
    /*
    * SCC "length and minLength or maxLength"
    */
    if (! flength)
	flength = bflength;    
    if (flength) {
	if (! fminlen)
	    flength = bflength;
	if (fminlen) {
	    /* (1.1) length >= minLength */
	    res = xmlSchemaCompareValues(flength->val, fminlen->val);
	    if (res == -2)
		goto internal_error;
	    if (res == -1)
		xmlSchemaDeriveFacetErr(pctxt, flength, fminlen, 1, 1, 0);
	}
	if (! fmaxlen)
	    fmaxlen = bfmaxlen;
	if (fmaxlen) {
	    /* (2.1) length <= maxLength */
	    res = xmlSchemaCompareValues(flength->val, fmaxlen->val);
	    if (res == -2)
		goto internal_error;
	    if (res == 1)
		xmlSchemaDeriveFacetErr(pctxt, flength, fmaxlen, -1, 1, 0);
	}
    }
    if (fmaxinc) {
	/*
	* "maxInclusive"
	*/
	if (fmininc) {
	    /* SCC "maxInclusive >= minInclusive" */
	    res = xmlSchemaCompareValues(fmaxinc->val, fmininc->val);
	    if (res == -2)
		goto internal_error;
	    if (res == -1) {
		xmlSchemaDeriveFacetErr(pctxt, fmaxinc, fmininc, 1, 1, 0);
	    }
	}
	/*
	* SCC "maxInclusive valid restriction"
	*/
	if (bfmaxinc) {
	    /* maxInclusive <= BASE maxInclusive */
	    res = xmlSchemaCompareValues(fmaxinc->val, bfmaxinc->val);
	    if (res == -2)
		goto internal_error;
	    if (res == 1)
		xmlSchemaDeriveFacetErr(pctxt, fmaxinc, bfmaxinc, -1, 1, 1);
	    if ((res != 0) && (bfmaxinc->fixed)) {
		FACET_RESTR_FIXED_ERR(fmaxinc)
	    }
	}
	if (bfmaxexc) {
	    /* maxInclusive < BASE maxExclusive */
	    res = xmlSchemaCompareValues(fmaxinc->val, bfmaxexc->val);
	    if (res == -2)
		goto internal_error;
	    if (res != -1) {
		xmlSchemaDeriveFacetErr(pctxt, fmaxinc, bfmaxexc, -1, 0, 1);
	    }
	}
	if (bfmininc) {
	    /* maxInclusive >= BASE minInclusive */
	    res = xmlSchemaCompareValues(fmaxinc->val, bfmininc->val);
	    if (res == -2)
		goto internal_error;
	    if (res == -1) {
		xmlSchemaDeriveFacetErr(pctxt, fmaxinc, bfmininc, 1, 1, 1);
	    }
	}
	if (bfminexc) {
	    /* maxInclusive > BASE minExclusive */
	    res = xmlSchemaCompareValues(fmaxinc->val, bfminexc->val);
	    if (res == -2)
		goto internal_error;
	    if (res != 1) {
		xmlSchemaDeriveFacetErr(pctxt, fmaxinc, bfminexc, 1, 0, 1);
	    }
	}	
    }
    if (fmaxexc) {
	/*
	* "maxExclusive >= minExclusive"
	*/
	if (fminexc) {
	    res = xmlSchemaCompareValues(fmaxexc->val, fminexc->val);
	    if (res == -2)
		goto internal_error;
	    if (res == -1) {
		xmlSchemaDeriveFacetErr(pctxt, fmaxexc, fminexc, 1, 1, 0);
	    }
	}
	/*
	* "maxExclusive valid restriction"
	*/
	if (bfmaxexc) {
	    /* maxExclusive <= BASE maxExclusive */
	    res = xmlSchemaCompareValues(fmaxexc->val, bfmaxexc->val);
	    if (res == -2)
		goto internal_error;
	    if (res == 1) {
		xmlSchemaDeriveFacetErr(pctxt, fmaxexc, bfmaxexc, -1, 1, 1);
	    }
	    if ((res != 0) && (bfmaxexc->fixed)) {
		FACET_RESTR_FIXED_ERR(fmaxexc)	
	    }
	}
	if (bfmaxinc) {
	    /* maxExclusive <= BASE maxInclusive */
	    res = xmlSchemaCompareValues(fmaxexc->val, bfmaxinc->val);
	    if (res == -2)
		goto internal_error;
	    if (res == 1) {
		xmlSchemaDeriveFacetErr(pctxt, fmaxexc, bfmaxinc, -1, 1, 1);
	    }
	}
	if (bfmininc) {
	    /* maxExclusive > BASE minInclusive */
	    res = xmlSchemaCompareValues(fmaxexc->val, bfmininc->val);
	    if (res == -2)
		goto internal_error;
	    if (res != 1) {
		xmlSchemaDeriveFacetErr(pctxt, fmaxexc, bfmininc, 1, 0, 1);
	    }
	}
	if (bfminexc) {
	    /* maxExclusive > BASE minExclusive */
	    res = xmlSchemaCompareValues(fmaxexc->val, bfminexc->val);
	    if (res == -2)
		goto internal_error;
	    if (res != 1) {
		xmlSchemaDeriveFacetErr(pctxt, fmaxexc, bfminexc, 1, 0, 1);
	    }
	}
    }
    if (fminexc) {
	/*
	* "minExclusive < maxInclusive"
	*/
	if (fmaxinc) {
	    res = xmlSchemaCompareValues(fminexc->val, fmaxinc->val);
	    if (res == -2)
		goto internal_error;
	    if (res != -1) {
		xmlSchemaDeriveFacetErr(pctxt, fminexc, fmaxinc, -1, 0, 0);
	    }
	}
	/*
	* "minExclusive valid restriction"
	*/
	if (bfminexc) {
	    /* minExclusive >= BASE minExclusive */
	    res = xmlSchemaCompareValues(fminexc->val, bfminexc->val);
	    if (res == -2)
		goto internal_error;
	    if (res == -1) {
		xmlSchemaDeriveFacetErr(pctxt, fminexc, bfminexc, 1, 1, 1);
	    }
	    if ((res != 0) && (bfminexc->fixed)) {
		FACET_RESTR_FIXED_ERR(fminexc)
	    }
	}
	if (bfmaxinc) {
	    /* minExclusive <= BASE maxInclusive */
	    res = xmlSchemaCompareValues(fminexc->val, bfmaxinc->val);
	    if (res == -2)
		goto internal_error;
	    if (res == 1) {
		xmlSchemaDeriveFacetErr(pctxt, fminexc, bfmaxinc, -1, 1, 1);
	    }
	}
	if (bfmininc) {
	    /* minExclusive >= BASE minInclusive */
	    res = xmlSchemaCompareValues(fminexc->val, bfmininc->val);
	    if (res == -2)
		goto internal_error;
	    if (res == -1) {
		xmlSchemaDeriveFacetErr(pctxt, fminexc, bfmininc, 1, 1, 1);
	    }
	}
	if (bfmaxexc) {
	    /* minExclusive < BASE maxExclusive */
	    res = xmlSchemaCompareValues(fminexc->val, bfmaxexc->val);
	    if (res == -2)
		goto internal_error;
	    if (res != -1) {
		xmlSchemaDeriveFacetErr(pctxt, fminexc, bfmaxexc, -1, 0, 1);
	    }
	}	
    }
    if (fmininc) {
	/*
	* "minInclusive < maxExclusive"
	*/
	if (fmaxexc) {
	    res = xmlSchemaCompareValues(fmininc->val, fmaxexc->val);
	    if (res == -2)
		goto internal_error;
	    if (res != -1) {
		xmlSchemaDeriveFacetErr(pctxt, fmininc, fmaxexc, -1, 0, 0);
	    }
	}
	/*
	* "minExclusive valid restriction"
	*/
	if (bfmininc) {
	    /* minInclusive >= BASE minInclusive */
	    res = xmlSchemaCompareValues(fmininc->val, bfmininc->val);
	    if (res == -2)
		goto internal_error;
	    if (res == -1) {
		xmlSchemaDeriveFacetErr(pctxt, fmininc, bfmininc, 1, 1, 1);
	    }
	    if ((res != 0) && (bfmininc->fixed)) {
		FACET_RESTR_FIXED_ERR(fmininc)	
	    }
	}
	if (bfmaxinc) {
	    /* minInclusive <= BASE maxInclusive */
	    res = xmlSchemaCompareValues(fmininc->val, bfmaxinc->val);
	    if (res == -2)
		goto internal_error;
	    if (res == -1) {
		xmlSchemaDeriveFacetErr(pctxt, fmininc, bfmaxinc, -1, 1, 1);
	    }
	}
	if (bfminexc) {
	    /* minInclusive > BASE minExclusive */
	    res = xmlSchemaCompareValues(fmininc->val, bfminexc->val);
	    if (res == -2)
		goto internal_error;
	    if (res != 1)
		xmlSchemaDeriveFacetErr(pctxt, fmininc, bfminexc, 1, 0, 1);
	}
	if (bfmaxexc) {
	    /* minInclusive < BASE maxExclusive */
	    res = xmlSchemaCompareValues(fmininc->val, bfmaxexc->val);
	    if (res == -2)
		goto internal_error;
	    if (res != -1)
		xmlSchemaDeriveFacetErr(pctxt, fmininc, bfmaxexc, -1, 0, 1);
	}
    }
    if (ftotdig && bftotdig) {
	/*
	* SCC " totalDigits valid restriction"
	* totalDigits <= BASE totalDigits
	*/
	res = xmlSchemaCompareValues(ftotdig->val, bftotdig->val);
	if (res == -2)
	    goto internal_error;
	if (res == 1)
	    xmlSchemaDeriveFacetErr(pctxt, ftotdig, bftotdig,
	    -1, 1, 1);
	if ((res != 0) && (bftotdig->fixed)) {
	    FACET_RESTR_FIXED_ERR(ftotdig)
	}
    }
    if (ffracdig && bffracdig) {	
	/*
	* SCC  "fractionDigits valid restriction"
	* fractionDigits <= BASE fractionDigits
	*/
	res = xmlSchemaCompareValues(ffracdig->val, bffracdig->val);
	if (res == -2)
	    goto internal_error;
	if (res == 1)
	    xmlSchemaDeriveFacetErr(pctxt, ffracdig, bffracdig,
	    -1, 1, 1);
	if ((res != 0) && (bffracdig->fixed)) {
	    FACET_RESTR_FIXED_ERR(ffracdig)
	}
    }
    /*
    * SCC "fractionDigits less than or equal to totalDigits"
    */
    if (! ftotdig)
	ftotdig = bftotdig;
    if (! ffracdig)
	ffracdig = bffracdig;
    if (ftotdig && ffracdig) {
	res = xmlSchemaCompareValues(ffracdig->val, ftotdig->val);
	if (res == -2)
	    goto internal_error;
	if (res == 1)
	    xmlSchemaDeriveFacetErr(pctxt, ffracdig, ftotdig,
		-1, 1, 0);
    }
    /* 
    * *Enumerations* won' be added here, since only the first set
    * of enumerations in the ancestor-or-self axis is used
    * for validation, plus we need to use the base type of those
    * enumerations for whitespace.
    *
    * *Patterns*: won't be add here, since they are ORed at
    * type level and ANDed at ancestor level. This will
    * happed during validation by walking the base axis
    * of the type.
    */		
    for (cur = base->facetSet; cur != NULL; cur = cur->next) {
	bfacet = cur->facet;
	/* 
	* Special handling of enumerations and patterns.
	* TODO: hmm, they should not appear in the set, so remove this.
	*/
	if ((bfacet->type == XML_SCHEMA_FACET_PATTERN) ||	
	    (bfacet->type == XML_SCHEMA_FACET_ENUMERATION))
	    continue;	
	/*
	* Search for a duplicate facet in the current type.
	*/
	link = type->facetSet;
	err = 0;
	fixedErr = 0;
	while (link != NULL) {
	    facet = link->facet;
	    if (facet->type == bfacet->type) {
		switch (facet->type) {		
		    case XML_SCHEMA_FACET_WHITESPACE:
			/*
			* The whitespace must be stronger.
			*/
			if (facet->whitespace < bfacet->whitespace) {
			    FACET_RESTR_ERR(flength,
				"The 'whitespace' value has to be equal to "
				"or stronger than the 'whitespace' value of "
				"the base type")
			}
			if ((bfacet->fixed) && 
			    (facet->whitespace != bfacet->whitespace)) {
			    FACET_RESTR_FIXED_ERR(facet)
			}
			break;
		    default:
			break;
		}		
		/* Duplicate found. */
		break;
	    }
	    link = link->next;
	}
	/*
	* If no duplicate was found: add the base types's facet
	* to the set.
	*/
	if (link == NULL) {
	    link = (xmlSchemaFacetLinkPtr) 
		xmlMalloc(sizeof(xmlSchemaFacetLink));
	    if (link == NULL) {
		xmlSchemaPErrMemory(pctxt,
		    "deriving facets, creating a facet link", NULL);
		return (-1);
	    }
	    link->facet = cur->facet;
	    link->next = NULL;
	    if (last == NULL)
		type->facetSet = link;		    
	    else 
		last->next = link;
	    last = link;
	}
	  
    }

    return (0);
internal_error:
    xmlSchemaPCustomErr(pctxt,
	XML_SCHEMAP_INVALID_FACET_VALUE,
	NULL, type, NULL,
	"Internal error: xmlSchemaDeriveAndValidateFacets", NULL);
    return (-1);
}

/**
 * xmlSchemaTypeFixup:
 * @typeDecl:  the schema type definition
 * @ctxt:  the schema parser context
 *
 * Fixes the content model of the type.
 */
static void
xmlSchemaTypeFixup(xmlSchemaTypePtr type,
                   xmlSchemaParserCtxtPtr ctxt, const xmlChar * name)
{
    if (type == NULL)
        return;
    if ((type->type != XML_SCHEMA_TYPE_COMPLEX) &&
	(type->type != XML_SCHEMA_TYPE_SIMPLE))
	return;
    if (! IS_NOT_TYPEFIXED(type))
	return;
    type->flags |= XML_SCHEMAS_TYPE_INTERNAL_RESOLVED;
    if (name == NULL)
        name = type->name;
    
    if (type->type == XML_SCHEMA_TYPE_COMPLEX) {
	xmlSchemaTypePtr baseType;

	/*
	* Resolve & type-fix the base type.
	*/
	baseType = type->baseType;
	if (baseType == NULL) {
	    baseType = xmlSchemaGetType(ctxt->schema,
		type->base, type->baseNs);	    
	    if (baseType == NULL) {
		xmlSchemaPResCompAttrErr(ctxt,
		    XML_SCHEMAP_SRC_RESOLVE,
		    NULL, type, type->node,
		    "base", type->base, type->baseNs,
		    XML_SCHEMA_TYPE_SIMPLE, NULL);
		return;
	    }
	    type->baseType = baseType;
	}
	if (IS_NOT_TYPEFIXED(baseType))
	    xmlSchemaTypeFixup(baseType, ctxt, NULL);
	if (baseType->flags & XML_SCHEMAS_TYPE_INTERNAL_INVALID) {
	    /*
	    * Skip fixup if the base type is invalid.
	    * TODO: Generate a warning!
	    */
	    return;
	}
	/*
	* This basically checks if the base type can be derived.
	*/
	if (xmlSchemaCheckSRCCT(ctxt, type) != 0) {
	    type->flags |= XML_SCHEMAS_TYPE_INTERNAL_INVALID;
	    return;
	}
	/*
	* Fixup the content type.
	*/
	if (type->contentType == XML_SCHEMA_CONTENT_SIMPLE) {
	    /*
	    * Corresponds to <complexType><simpleContent>...
	    */	    	   
	    if ((IS_COMPLEX_TYPE(baseType)) &&
		(baseType->contentTypeDef != NULL) &&
		(type->flags & XML_SCHEMAS_TYPE_DERIVATION_METHOD_RESTRICTION)) {
		 /*
		* SPEC (1) If <restriction> + base type is <complexType>, 
		* "whose own {content type} is a simple type..."
		*
		* NOTE (1.1) is handled during parsing of <restriction>.
		*
		* (1.2) "...otherwise (<restriction> has no <simpleType> among
		* its [children]), the simple type definition which is the
		* {content type} of the ... base type."
		*/
		if (type->contentTypeDef->baseType == NULL) {
		    /*
		    * <simpleContent><restriction> has *no* <simpleType>
		    * child.
		    */
		    type->contentTypeDef->baseType = 
			baseType->contentTypeDef;
		}
	    } else if ((IS_COMPLEX_TYPE(baseType)) &&
		(baseType->contentType == XML_SCHEMA_CONTENT_MIXED) &&
		(type->flags & XML_SCHEMAS_TYPE_DERIVATION_METHOD_RESTRICTION)) {
		/*
		* SPEC (2) If <restriction> + base is a mixed <complexType> with
		* an emptiable particle, then a simple type definition which
		* restricts the <restriction>'s <simpleType> child.
		*/
		if ((type->contentTypeDef == NULL) || 
		    (type->contentTypeDef->baseType == NULL)) {
		    /*
		    * TODO: Check if this ever happens.
		    */
		    xmlSchemaPCustomErr(ctxt,
			XML_SCHEMAP_INTERNAL,
			NULL, type, NULL,
			"Internal error: xmlSchemaTypeFixup, "
			"complex type '%s': the <simpleContent><restriction> "
			"is missing a <simpleType> child, but was not catched "
			"by xmlSchemaCheckSRCCT()", type->name);
		}		
	    } else if ((IS_COMPLEX_TYPE(baseType)) &&
		(type->flags & XML_SCHEMAS_TYPE_DERIVATION_METHOD_EXTENSION)) {
		/*
		* SPEC (3) If <extension> + base is <complexType> with
		* <simpleType> content, "...then the {content type} of that
		* complex type definition"
		*/		
		if (baseType->contentTypeDef == NULL) {
		    /*
		    * TODO: Check if this ever happens. xmlSchemaCheckSRCCT
		    * should have catched this already.
		    */
		    xmlSchemaPCustomErr(ctxt,
			XML_SCHEMAP_INTERNAL,
			NULL, type, NULL,
			"Internal error: xmlSchemaTypeFixup, "
			"complex type '%s': the <extension>ed base type is "
			"a complex type with no simple content type",
			type->name);
		}
		type->contentTypeDef = baseType->contentTypeDef;
	    } else if ((IS_SIMPLE_TYPE(baseType)) &&
		(type->flags & XML_SCHEMAS_TYPE_DERIVATION_METHOD_EXTENSION)) {
		/*
		* SPEC (4) <extension> + base is <simpleType> 
		* "... then that simple type definition"
		*/
		type->contentTypeDef = baseType;
	    } else {
		/*
		* TODO: Check if this ever happens.
		*/
		xmlSchemaPCustomErr(ctxt,
		    XML_SCHEMAP_INTERNAL,
		    NULL, type, NULL,
		    "Internal error: xmlSchemaTypeFixup, "
		    "complex type '%s' with <simpleContent>: unhandled "
		    "derivation case", type->name);
	    }
	} else {
	    int dummySequence = 0;
	    xmlSchemaParticlePtr particle = 
		(xmlSchemaParticlePtr) type->subtypes;
	    /*
	    * Corresponds to <complexType><complexContent>...
	    *
	    * NOTE that the effective mixed was already set during parsing of
	    * <complexType> and <complexContent>; its flag value is	    
	    * XML_SCHEMAS_TYPE_MIXED.
	    *	   
	    * Compute the "effective content":
	    * (2.1.1) + (2.1.2) + (2.1.3)
	    */
	    if ((particle == NULL) ||
		((particle->type == XML_SCHEMA_TYPE_PARTICLE) &&
		 ((particle->children->type == XML_SCHEMA_TYPE_ALL) ||
		  (particle->children->type == XML_SCHEMA_TYPE_SEQUENCE) ||
		  ((particle->children->type == XML_SCHEMA_TYPE_CHOICE) &&
		   (particle->minOccurs == 0))) &&
		   ( ((xmlSchemaTreeItemPtr) particle->children)->children == NULL))) {
		if (type->flags & XML_SCHEMAS_TYPE_MIXED) {		    
		    /*
		    * SPEC (2.1.4) "If the �effective mixed� is true, then
		    * a particle whose properties are as follows:..."
		    *
		    * Empty sequence model group with 
		    * minOccurs/maxOccurs = 1 (i.e. a "particle emptiable").
		    * NOTE that we sill assign it the <complexType> node to
		    * somehow anchor it in the doc.
		    */
		    if ((particle == NULL) ||
			(particle->children->type != XML_SCHEMA_TYPE_SEQUENCE)) {			
			/*
			* Create the particle.
			*/
			particle = xmlSchemaAddParticle(ctxt, ctxt->schema,
			    type->node, 1, 1);
			if (particle == NULL)
			    return;
			/*
			* Create the model group.
			*/
			particle->children = (xmlSchemaTreeItemPtr)
			    xmlSchemaAddModelGroup(ctxt, ctxt->schema,
				XML_SCHEMA_TYPE_SEQUENCE, NULL, type->node);
			if (particle->children == NULL)
			    return;
			
			type->subtypes = (xmlSchemaTypePtr) particle;
		    }
		    dummySequence = 1;
		    type->contentType = XML_SCHEMA_CONTENT_ELEMENTS;
		} else {
		    /*
		    * SPEC (2.1.5) "otherwise empty"
		    */
		    type->contentType = XML_SCHEMA_CONTENT_EMPTY;
		}				
	    } else {
		/*
	 	* SPEC (2.2) "otherwise the particle corresponding to the
		* <all>, <choice>, <group> or <sequence> among the
		* [children]."
		*/
		type->contentType = XML_SCHEMA_CONTENT_ELEMENTS;
	    }
	    /*
	    * Compute the "content type".
	    */
	    if (type->flags & XML_SCHEMAS_TYPE_DERIVATION_METHOD_RESTRICTION) {
		/*
		* SPEC (3.1) "If <restriction>..."
		* (3.1.1) + (3.1.2) */
		if (type->contentType != XML_SCHEMA_CONTENT_EMPTY) {
		    if (type->flags & XML_SCHEMAS_TYPE_MIXED)
			type->contentType = XML_SCHEMA_CONTENT_MIXED;
		}
	    } else {
		/*
		* SPEC (3.2) "If <extension>..."		
		*/
		if (type->contentType == XML_SCHEMA_CONTENT_EMPTY) {
		    /*
		    * SPEC (3.2.1)
		    */
		    type->contentType = baseType->contentType;
		    type->subtypes = baseType->subtypes;
		    /*
		    * NOTE that the effective mixed is ignored here.
		    */
		} else if (baseType->contentType == XML_SCHEMA_CONTENT_EMPTY) {
		    /*
		    * SPEC (3.2.2)
		    */
		    if (type->flags & XML_SCHEMAS_TYPE_MIXED)
			type->contentType = XML_SCHEMA_CONTENT_MIXED;
		} else {
		    /*
		    * SPEC (3.2.3)
		    */
		    if (type->flags & XML_SCHEMAS_TYPE_MIXED)
			type->contentType = XML_SCHEMA_CONTENT_MIXED;
		    /*
		    * "A model group whose {compositor} is sequence and whose 
		    * {particles} are..."
		    */
		    if (! dummySequence) {
			xmlSchemaTreeItemPtr effectiveContent =
			    (xmlSchemaTreeItemPtr) type->subtypes;
			/*
			* Create the particle.
			*/
			particle = xmlSchemaAddParticle(ctxt, ctxt->schema,
			    type->node, 1, 1);
			if (particle == NULL)
			    return;
			/*
			* Create the "sequence" model group.
			*/
			particle->children = (xmlSchemaTreeItemPtr)
			    xmlSchemaAddModelGroup(ctxt, ctxt->schema,
				XML_SCHEMA_TYPE_SEQUENCE, NULL, type->node);
			if (particle->children == NULL)
			    return;
			type->subtypes = (xmlSchemaTypePtr) particle;
			/*
			* SPEC "the particle of the {content type} of
			* the ... base ..."
			* Create a duplicate of the base type's particle
			* and assign its "term" to it.
			*/
			particle->children->children = 
			    (xmlSchemaTreeItemPtr) xmlSchemaAddParticle(ctxt,
				ctxt->schema, type->node,
				((xmlSchemaParticlePtr) type->subtypes)->minOccurs,
				((xmlSchemaParticlePtr) type->subtypes)->maxOccurs);				
			if (particle->children->children == NULL)
			    return;
			particle = (xmlSchemaParticlePtr)
			    particle->children->children;
			particle->children =			    
				((xmlSchemaParticlePtr) baseType->subtypes)->children;
			/*
			* SPEC "followed by the �effective content�."
			*/
			particle->next = effectiveContent;			
		    } else {
			/*
			* This is the case when there is already an empty
			* <sequence> with minOccurs==maxOccurs==1.
			* Just add the base types's content type.
			* NOTE that, although we miss to add an intermediate
			* <sequence>, this should produce no difference to
			* neither the regex compilation of the content model,
			* nor to the complex type contraints.
			*/
			particle->children->children =
			    (xmlSchemaTreeItemPtr) baseType->subtypes;
		    }
		}
	    }
	}	
	/*
	* Apply the complex type component constraints; this will not
	* check attributes, since this is done in
	* xmlSchemaBuildAttributeValidation().
	*/
	if (xmlSchemaCheckCTComponent(ctxt, type) != 0)
	    return;	
	/*
	* Inherit & check constraints for attributes.
	*/
	xmlSchemaBuildAttributeValidation(ctxt, type);
    } else if (type->type == XML_SCHEMA_TYPE_SIMPLE) {	
	/*
	* Simple Type Definition Schema Component
	*/
	type->contentType = XML_SCHEMA_CONTENT_SIMPLE;
	if (type->flags & XML_SCHEMAS_TYPE_VARIETY_LIST) {
	    /* 
	    * Corresponds to <simpleType><list>...
	    * Resolve the itemType.
	    */
	    if ((type->subtypes == NULL) && (type->ref != NULL)) {
		type->subtypes = xmlSchemaGetType(ctxt->schema,
		    type->ref, type->refNs);
		if ((type->subtypes == NULL) ||
		    (! IS_SIMPLE_TYPE(type->subtypes))) {
		    type->subtypes = NULL;
		    xmlSchemaPResCompAttrErr(ctxt,	    
			XML_SCHEMAP_SRC_RESOLVE,
			NULL, type, type->node,
			"itemType", type->ref, type->refNs,
			XML_SCHEMA_TYPE_SIMPLE, NULL);
		}		
	    }
	    if (type->subtypes == NULL) {
		/*
		* This one is really needed, so get out.
		*/
		return;
	    }
	    if (IS_NOT_TYPEFIXED(type->subtypes))
		xmlSchemaTypeFixup(type->subtypes, ctxt, NULL);
	    /* Base type:
	    * 2 If the <list> or <union> alternative is chosen, 
	    * then the �simple ur-type definition�.
	    */
	    type->baseType = xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYSIMPLETYPE);	    
	} else if (type->flags & XML_SCHEMAS_TYPE_VARIETY_UNION) {
	    /*
	    * Corresponds to <simpleType><union>...
	    * Resolve the member types.
	    */
	    xmlSchemaResolveUnionMemberTypes(ctxt, type);
	    if (type->memberTypes == NULL) {
		/*
		* This one is really needed, so get out.
		*/
		return;
	    }
	    type->baseType = xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYSIMPLETYPE);
	} else {
	    xmlSchemaTypePtr baseType;
	    /* 
	    * Corresponds to <simpleType><restriction>...
	    *
	    * Resolve the base type.
	    */
	    if ((type->baseType == NULL) && (type->base != NULL)) {
		baseType = xmlSchemaGetType(ctxt->schema,
		    type->base, type->baseNs);
		
		if (baseType == NULL) {
		    xmlSchemaPResCompAttrErr(ctxt,	    
			XML_SCHEMAP_SRC_RESOLVE,
			NULL, type, type->node,
			"base", type->base, type->baseNs,
			XML_SCHEMA_TYPE_SIMPLE, NULL);
		}
		type->baseType = baseType;
	    } else
		baseType = type->baseType;

	    if (baseType == NULL)
		return;			

	    if (IS_NOT_TYPEFIXED(baseType))
		xmlSchemaTypeFixup(baseType, ctxt, NULL);
	    /* 
	    * Variety
	    * If the <restriction> alternative is chosen, then the 
	    * {variety} of the {base type definition}.
	    */			    
	    if (baseType->flags & XML_SCHEMAS_TYPE_VARIETY_ATOMIC)
		type->flags |= XML_SCHEMAS_TYPE_VARIETY_ATOMIC;
	    else if (baseType->flags & XML_SCHEMAS_TYPE_VARIETY_LIST) {
		type->flags |= XML_SCHEMAS_TYPE_VARIETY_LIST;
		/*
		* Inherit the itemType.
		*/
		type->subtypes = baseType->subtypes;
	    } else if (baseType->flags & XML_SCHEMAS_TYPE_VARIETY_UNION) {
		type->flags |= XML_SCHEMAS_TYPE_VARIETY_UNION;
		/*
		* NOTE that we won't assign the memberTypes of the base,
		* since this will make trouble when freeing them; we will
		* use a lookup function to access them instead.
		*/		    
	    }
	    	    
	    /*
	    * Some optimization for validation:
	    * If there are no facets beside the "whitespace" facet,
	    * then a value needs not to checked against against a
	    * facet, thus no computed value is needed.
	    */
#if 0
	    if (baseType->flags & XML_SCHEMAS_TYPE_FACETSNEEDVALUE)
		type->flags |= XML_SCHEMAS_TYPE_FACETSNEEDVALUE;
	    else {
		for (cur = type->facetSet; cur != NULL;
		cur = cur->next) {
		    if (cur->facet->type != XML_SCHEMA_FACET_WHITESPACE) {
			type->flags |= XML_SCHEMAS_TYPE_FACETSNEEDVALUE;
			break;
		    }
		}
	    }
#endif
	}	
	/*
	* Check constraints.
	*
	* TODO: Split this somehow, we need to know first if we can derive
	* from the base type at all!
	*/
	if (type->baseType != NULL) {
	    /*
	    * Schema Component Constraint: Simple Type Restriction 
	    * (Facets)
	    * NOTE: Satisfaction of 1 and 2 arise from the fixup 
	    * applied beforehand.
	    */
	    xmlSchemaCheckSRCSimpleType(ctxt, type);
	    xmlSchemaCheckFacetValues(type, ctxt);
	    xmlSchemaDeriveAndValidateFacets(ctxt, type);	    
	}		
    }
    
#ifdef DEBUG_TYPE
    if (type->node != NULL) {
        xmlGenericError(xmlGenericErrorContext,
                        "Type of %s : %s:%d :", name,
                        type->node->doc->URL,
                        xmlGetLineNo(type->node));
    } else {
        xmlGenericError(xmlGenericErrorContext, "Type of %s :", name);
    }
    if ((IS_SIMPLE_TYPE(type)) || (IS_COMPLEX_TYPE(type))) {
	switch (type->contentType) {
	    case XML_SCHEMA_CONTENT_SIMPLE:
		xmlGenericError(xmlGenericErrorContext, "simple\n");
		break;
	    case XML_SCHEMA_CONTENT_ELEMENTS:
		xmlGenericError(xmlGenericErrorContext, "elements\n");
		break;
	    case XML_SCHEMA_CONTENT_UNKNOWN:
		xmlGenericError(xmlGenericErrorContext, "unknown !!!\n");
		break;
	    case XML_SCHEMA_CONTENT_EMPTY:
		xmlGenericError(xmlGenericErrorContext, "empty\n");
		break;
	    case XML_SCHEMA_CONTENT_MIXED:
		if (xmlSchemaIsParticleEmptiable((xmlSchemaParticlePtr)
		    type->subtypes)) 
		    xmlGenericError(xmlGenericErrorContext,
			"mixed as emptiable particle\n");
		else
		    xmlGenericError(xmlGenericErrorContext, "mixed\n");
		break;
		/* Removed, since not used. */
		/*
		case XML_SCHEMA_CONTENT_MIXED_OR_ELEMENTS:
		xmlGenericError(xmlGenericErrorContext, "mixed or elems\n");
		break;
		*/
	    case XML_SCHEMA_CONTENT_BASIC:
		xmlGenericError(xmlGenericErrorContext, "basic\n");
		break;
	    default:
		xmlGenericError(xmlGenericErrorContext,
		    "not registered !!!\n");
		break;
	}
    }
#endif
}

/**
 * xmlSchemaCheckFacet:
 * @facet:  the facet
 * @typeDecl:  the schema type definition
 * @ctxt:  the schema parser context or NULL
 * @name: name of the type
 *
 * Checks the default values types, especially for facets 
 *
 * Returns 0 if okay or -1 in cae of error
 */
int
xmlSchemaCheckFacet(xmlSchemaFacetPtr facet,
                    xmlSchemaTypePtr typeDecl,
                    xmlSchemaParserCtxtPtr ctxt, const xmlChar * name)
{
    xmlSchemaTypePtr nonNegativeIntegerType = NULL;
    int ret = 0, reuseValCtxt = 0;

    if ((facet == NULL) || (typeDecl == NULL))
        return(-1);
    /* 
    * TODO: will the parser context be given if used from
    * the relaxNG module?
    */

    if (nonNegativeIntegerType == NULL) {
        nonNegativeIntegerType =
            xmlSchemaGetBuiltInType(XML_SCHEMAS_NNINTEGER);
    }
    switch (facet->type) {
        case XML_SCHEMA_FACET_MININCLUSIVE:
        case XML_SCHEMA_FACET_MINEXCLUSIVE:
        case XML_SCHEMA_FACET_MAXINCLUSIVE:
        case XML_SCHEMA_FACET_MAXEXCLUSIVE:
	case XML_SCHEMA_FACET_ENUMERATION: {
                /*
                 * Okay we need to validate the value
                 * at that point.
                 */
                xmlSchemaValidCtxtPtr vctxt;
		xmlSchemaTypePtr base;

		/* 4.3.5.5 Constraints on enumeration Schema Components
		* Schema Component Constraint: enumeration valid restriction
		* It is an �error� if any member of {value} is not in the 
		* �value space� of {base type definition}. 
		*
		* minInclusive, maxInclusive, minExclusive, maxExclusive:
		* The value �must� be in the 
		* �value space� of the �base type�. 
		*/
		/*
		* This function is intended to deliver a compiled value
		* on the facet. In this implementation of XML Schemata the
		* type holding a facet, won't be a built-in type. 
		* Thus to ensure that other API
		* calls (relaxng) do work, if the given type is a built-in 
		* type, we will assume that the given built-in type *is
		* already* the base type.		
		*/
		if (typeDecl->type != XML_SCHEMA_TYPE_BASIC) {
		    base = typeDecl->baseType;
		    if (base == NULL) {
			xmlSchemaPErr(ctxt, typeDecl->node,
			    XML_SCHEMAP_INTERNAL,
			    "Internal error: xmlSchemaCheckFacet, "
			    "the type '%s' has no base type.\n",
			    typeDecl->name, NULL);
			return (-1);
		    }		
		} else
		    base = typeDecl;
		/*
		* This avoids perseverative creation of the 
		* validation context if a parser context is
		* used.
		*/
		if (ctxt != NULL) {
		    reuseValCtxt = 1;
		    if (ctxt->vctxt == NULL) {
			if (xmlSchemaCreateVCtxtOnPCtxt(ctxt) == -1)
			    return (-1);
		    }
		    vctxt = ctxt->vctxt;
		} else {
		    vctxt = xmlSchemaNewValidCtxt(NULL);
		    if (vctxt == NULL) {
			xmlSchemaPErr(ctxt, typeDecl->node,
			    XML_SCHEMAP_INTERNAL,
			    "Internal error: xmlSchemaCheckFacet, "
			    "creating a new validation context.\n",
			    NULL, NULL);
			return (-1);	
		    }
		}
	                
		vctxt->node = facet->node;
		vctxt->cur = NULL;
		/*
		* NOTE: This call does not check the content nodes, 
		* since they are not available:
		* facet->node is just the node holding the facet 
		* definition, *not* the attribute holding the *value* 
		* of the facet.
		*/
		ret = xmlSchemaValidateSimpleTypeValue(vctxt, base, 
		    facet->value, 0, 1, 1, 0);				
                if (ret > 0) {
                    /* error code */
                    if (ctxt != NULL) {
			xmlChar *str = NULL;

                        xmlSchemaPCustomErrExt(ctxt,
			    XML_SCHEMAP_INVALID_FACET_VALUE,
			    NULL, (xmlSchemaTypePtr) facet, facet->node,
			    "The value '%s' of the facet does not validate "
			    "against the base type '%s'",
			    facet->value, 
			    xmlSchemaFormatQName(&str, 
				base->targetNamespace, base->name), NULL);
			FREE_AND_NULL(str)
                    }
		    /* xmlSchemaFacetTypeToString(facet->type), */
                    ret = -1;
                } else if (ret < 0) {
		    xmlSchemaPErrExt(ctxt, facet->node,
			XML_SCHEMAP_INTERNAL,
			NULL, NULL, NULL,
			"Internal error: xmlSchemaCheckFacet, "
			"failed to validate the value '%s' name of the "
			"facet '%s' against the base type '%s'.\n",
			facet->value, 
			xmlSchemaFacetTypeToString(facet->type),
			base->name, NULL, NULL); 
		    ret = -1;
		} else {
		    if (vctxt->value != NULL) {
			facet->val = vctxt->value;
			vctxt->value = NULL;
		    } else {			
			xmlChar *str;
			/*
			* Ensure computed values even for type string.
			* TODO OPTIMIZE MEMORY: The value will be hold twice,
			* by the facet->value and by the computed value.
			*/
			str = xmlStrdup(facet->value);
			if (xmlSchemaPostCreateVal(vctxt, typeDecl,
			    BAD_CAST str, &(facet->val)) == -1) {
			    FREE_AND_NULL(str)
			    xmlSchemaPErr(ctxt, typeDecl->node,
				XML_SCHEMAP_INTERNAL,
				"Internal error: xmlSchemaCheckFacet, "
				"post-creating a computed value.\n",
				NULL, NULL);
			    /* Note that we don't return a failure yet.*/
			}
		    }
		}		
		if (reuseValCtxt == 0)
		    xmlSchemaFreeValidCtxt(vctxt);
                break;
            }
        case XML_SCHEMA_FACET_PATTERN:
            facet->regexp = xmlRegexpCompile(facet->value);
            if (facet->regexp == NULL) {
		xmlSchemaPErr(ctxt, typeDecl->node,
		    XML_SCHEMAP_REGEXP_INVALID,
		    "Type definition '%s': The value '%s' of the "
		    "facet 'pattern' is not valid.\n",
		    name, facet->value);
                ret = -1;
            }
            break;
        case XML_SCHEMA_FACET_TOTALDIGITS:
        case XML_SCHEMA_FACET_FRACTIONDIGITS:
        case XML_SCHEMA_FACET_LENGTH:
        case XML_SCHEMA_FACET_MAXLENGTH:
        case XML_SCHEMA_FACET_MINLENGTH:{
                int tmp;

                tmp =
                    xmlSchemaValidatePredefinedType(nonNegativeIntegerType,
                                                    facet->value,
                                                    &(facet->val));
                if (tmp != 0) {
                    /* error code */
                    if (ctxt != NULL) {
                        xmlSchemaPCustomErr(ctxt,
			    XML_SCHEMAP_INVALID_FACET_VALUE,
			    NULL, (xmlSchemaTypePtr)  facet, facet->node,
			    "The value '%s' of the facet is not a valid "
			    "nonNegativeInteger", facet->value);
                    }
                    ret = -1;
                }
                break;
            }
        case XML_SCHEMA_FACET_WHITESPACE:{
                if (xmlStrEqual(facet->value, BAD_CAST "preserve")) {
                    facet->whitespace = XML_SCHEMAS_FACET_PRESERVE;
                } else if (xmlStrEqual(facet->value, BAD_CAST "replace")) {
                    facet->whitespace = XML_SCHEMAS_FACET_REPLACE;
                } else if (xmlStrEqual(facet->value, BAD_CAST "collapse")) {
                    facet->whitespace = XML_SCHEMAS_FACET_COLLAPSE;
                } else {
                    if (ctxt != NULL) {
			/* error was previously: XML_SCHEMAP_INVALID_WHITE_SPACE */
			xmlSchemaPCustomErr(ctxt,
			    XML_SCHEMAP_INVALID_FACET_VALUE,
			    NULL, (xmlSchemaTypePtr) facet, facet->node,
			    "The value '%s' of the facet is not a valid",
			    facet->value);
                    }
                    ret = -1;
                }
            }
        default:
            break;
    }
    return (ret);
}

/**
 * xmlSchemaCheckFacetValues:
 * @typeDecl:  the schema type definition
 * @ctxt:  the schema parser context
 *
 * Checks the default values types, especially for facets 
 */
static void
xmlSchemaCheckFacetValues(xmlSchemaTypePtr typeDecl,
			  xmlSchemaParserCtxtPtr ctxt)
{
    const xmlChar *name = typeDecl->name; 
    /*
    * NOTE: It is intended to use the facets list, instead
    * of facetSet.
    */
    if (typeDecl->facets != NULL) {
	xmlSchemaFacetPtr facet = typeDecl->facets;
	
	/*
	* Temporarily assign the "schema" to the validation context
	* of the parser context. This is needed for NOTATION validation.
	*/
	if (ctxt->vctxt == NULL) {
	    if (xmlSchemaCreateVCtxtOnPCtxt(ctxt) == -1)
		return;
	}
	ctxt->vctxt->schema = ctxt->schema;

	while (facet != NULL) {
	    xmlSchemaCheckFacet(facet, typeDecl, ctxt, name);
	    facet = facet->next;
	}

	ctxt->vctxt->schema = NULL;
    }    
}

/**
 * xmlSchemaGetCircModelGrDefRef:
 * @ctxtMGroup: the searched model group
 * @selfMGroup: the second searched model group
 * @particle: the first particle 
 *
 * This one is intended to be used by
 * xmlSchemaCheckGroupDefCircular only.
 *
 * Returns the particle with the circular model group definition reference,
 * otherwise NULL.
 */
static xmlSchemaTreeItemPtr
xmlSchemaGetCircModelGrDefRef(xmlSchemaTreeItemPtr ctxtMGroup,
			      xmlSchemaTreeItemPtr selfMGroup,
			      xmlSchemaTreeItemPtr particle)
{
    xmlSchemaTreeItemPtr circ = NULL;
    xmlSchemaTreeItemPtr term;

    while (particle != NULL) {
	term = particle->children;
	if ((term != NULL) &&
	    ((term->type == XML_SCHEMA_TYPE_SEQUENCE) ||
	    (term->type == XML_SCHEMA_TYPE_CHOICE) ||
	    (term->type == XML_SCHEMA_TYPE_ALL))) {
	    if (term == ctxtMGroup)
		return (particle);
	    /*
	    * Avoid infinite recursion on circular references not yet
	    * examined.
	    */
	    if (term == selfMGroup)
		return (NULL);
	    circ = xmlSchemaGetCircModelGrDefRef(ctxtMGroup, term,
		term->children);
	    if (circ != NULL)
		return (circ);
	}
	particle = particle->next;
    }
    return (NULL);
}

/**
 * xmlSchemaCheckGroupDefCircular:
 * @item:  the model group definition
 * @ctxt:  the parser context
 * @name:  the name
 *
 * Checks for circular references to model group definitions.
 */
static void
xmlSchemaCheckGroupDefCircular(xmlSchemaModelGroupDefPtr item,
			    xmlSchemaParserCtxtPtr ctxt, 
			    const xmlChar * name ATTRIBUTE_UNUSED)
{    
    /*
    * Schema Component Constraint: Model Group Correct
    * 2 Circular groups are disallowed. That is, within the {particles} 
    * of a group there must not be at any depth a particle whose {term} 
    * is the group itself.
    */
    if ((item == NULL) ||
	(item->type != XML_SCHEMA_TYPE_GROUP) ||
	(item->children == NULL))
	return;
    {
	xmlSchemaTreeItemPtr circ;

	circ = xmlSchemaGetCircModelGrDefRef(item->children, NULL,
	    item->children->children);
	if (circ != NULL) {
	    xmlChar *str = NULL;
	    /*
	    * TODO: The error report is not adequate: this constraint
	    * is defined for model groups but not definitions, but since
	    * there cannot be any circular model groups without a model group
	    * definition (if not using a construction API), we check those 
	    * defintions only.
	    */
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_MG_PROPS_CORRECT_2,
		NULL, NULL, GET_NODE(circ),
		"Circular reference to the model group definition '%s' "
		"defined", xmlSchemaFormatQName(&str,
		    item->targetNamespace, item->name));
	    FREE_AND_NULL(str)
	    /*
	    * NOTE: We will cut the reference to avoid further
	    * confusion of the processor. This is a fatal error.
	    */
	    circ->children = NULL;
	}
    }
}


/**
 * xmlSchemaGetCircAttrGrRef:
 * @ctxtGr: the searched attribute group
 * @attr: the current attribute list to be processed
 *
 * This one is intended to be used by
 * xmlSchemaCheckSRCAttributeGroupCircular only.
 *
 * Returns the circular attribute grou reference, otherwise NULL.
 */
static xmlSchemaAttributeGroupPtr
xmlSchemaGetCircAttrGrRef(xmlSchemaAttributeGroupPtr ctxtGr,
			  xmlSchemaAttributePtr attr)
{    
    xmlSchemaAttributeGroupPtr circ = NULL, gr;
    int marked;
    /*
    * We will search for an attribute group reference which
    * references the context attribute group.
    */    	
    while (attr != NULL) {
	marked = 0;
	if (attr->type == XML_SCHEMA_TYPE_ATTRIBUTEGROUP) {
	    gr = (xmlSchemaAttributeGroupPtr) attr;
	    if (gr->refItem != NULL)  {
		if (gr->refItem == ctxtGr)
		    return (gr);
		else if (gr->refItem->flags & 
		    XML_SCHEMAS_ATTRGROUP_MARKED) {
		    attr = attr->next;
		    continue;
		} else {
		    /*
		    * Mark as visited to avoid infinite recursion on
		    * circular references not yet examined.
		    */
		    gr->refItem->flags |= XML_SCHEMAS_ATTRGROUP_MARKED;
		    marked = 1;
		}
	    }
	    if (gr->attributes != NULL)
		circ = xmlSchemaGetCircAttrGrRef(ctxtGr, gr->attributes);
	    /*
	    * Unmark the visited group's attributes.
	    */
	    if (marked)
		gr->refItem->flags ^= XML_SCHEMAS_ATTRGROUP_MARKED;
	    if (circ != NULL)
		return (circ);
	}
	attr = attr->next;
    }
    return (NULL);
}
				
/**
 * xmlSchemaCheckSRCAttributeGroupCircular:
 * attrGr:  the attribute group definition
 * @ctxt:  the parser context
 * @name:  the name
 *
 * Checks for circular references of attribute groups.
 */
static void
xmlSchemaCheckAttributeGroupCircular(xmlSchemaAttributeGroupPtr attrGr,
					xmlSchemaParserCtxtPtr ctxt, 
					const xmlChar * name ATTRIBUTE_UNUSED)
{    
    /*
    * Schema Representation Constraint: 
    * Attribute Group Definition Representation OK
    * 3 Circular group reference is disallowed outside <redefine>. 
    * That is, unless this element information item's parent is 
    * <redefine>, then among the [children], if any, there must 
    * not be an <attributeGroup> with ref [attribute] which resolves 
    * to the component corresponding to this <attributeGroup>. Indirect 
    * circularity is also ruled out. That is, when QName resolution 
    * (Schema Document) (�3.15.3) is applied to a �QName� arising from 
    * any <attributeGroup>s with a ref [attribute] among the [children], 
    * it must not be the case that a �QName� is encountered at any depth 
    * which resolves to the component corresponding to this <attributeGroup>.
    */
    /*
    * Only global components can be referenced.
    */
    if (((attrGr->flags & XML_SCHEMAS_ATTRGROUP_GLOBAL) == 0) || 
	(attrGr->attributes == NULL))
	return;
    else {
	xmlSchemaAttributeGroupPtr circ;

	circ = xmlSchemaGetCircAttrGrRef(attrGr, attrGr->attributes);
	if (circ != NULL) {
	    /*
	    * TODO: Report the referenced attr group as QName.
	    */
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_SRC_ATTRIBUTE_GROUP_3,
		NULL, NULL, circ->node,
		"Circular reference to the attribute group '%s' "
		"defined", attrGr->name);
	    /*
	    * NOTE: We will cut the reference to avoid further
	    * confusion of the processor.
	    * BADSPEC: The spec should define how to process in this case.
	    */
	    circ->attributes = NULL;
	    circ->refItem = NULL;
	}
    }
}

/**
 * xmlSchemaAttrGrpFixup:
 * @attrgrpDecl:  the schema attribute definition
 * @ctxt:  the schema parser context
 * @name:  the attribute name
 *
 * Fixes finish doing the computations on the attributes definitions
 */
static void
xmlSchemaAttrGrpFixup(xmlSchemaAttributeGroupPtr attrgrp,
                      xmlSchemaParserCtxtPtr ctxt, const xmlChar * name)
{
    if (name == NULL)
        name = attrgrp->name;
    if (attrgrp->attributes != NULL)
        return;
    if (attrgrp->ref != NULL) {
        xmlSchemaAttributeGroupPtr ref;

        ref = xmlSchemaGetAttributeGroup(ctxt->schema, attrgrp->ref, 
	    attrgrp->refNs);
        if (ref == NULL) {
	    xmlSchemaPResCompAttrErr(ctxt, 
		XML_SCHEMAP_SRC_RESOLVE,
		NULL, (xmlSchemaTypePtr) attrgrp, attrgrp->node,
		"ref", attrgrp->ref, attrgrp->refNs, 
		XML_SCHEMA_TYPE_ATTRIBUTEGROUP, NULL);
            return;
        }
	attrgrp->refItem = ref;
	/*
	* Check for self reference!
	*/
        xmlSchemaAttrGrpFixup(ref, ctxt, NULL);
        attrgrp->attributes = ref->attributes;
	attrgrp->attributeWildcard = ref->attributeWildcard;
    }
}

/**
 * xmlSchemaAttrCheckValConstr:
 * @item:  an schema attribute declaration/use
 * @ctxt:  a schema parser context
 * @name:  the name of the attribute
 * 
 * Validates the value constraints of an attribute declaration/use.
 *
 * Fixes finish doing the computations on the attributes definitions
 */
static void
xmlSchemaCheckAttrValConstr(xmlSchemaAttributePtr item,
			    xmlSchemaParserCtxtPtr ctxt, 
			    const xmlChar * name ATTRIBUTE_UNUSED)
{

    /*
    * a-props-correct
    * Schema Component Constraint: Attribute Declaration Properties Correct
    *
    * 2 if there is a {value constraint}, the canonical lexical 
    * representation of its value must be �valid� with respect 
    * to the {type definition} as defined in String Valid (�3.14.4). 
    */

    if (item->defValue != NULL) {
	int ret;
	xmlNodePtr node;
	xmlSchemaTypePtr type;

	if (item->subtypes == NULL) {
	    xmlSchemaPErr(ctxt, item->node,
		XML_SCHEMAP_INTERNAL,
		"Internal error: xmlSchemaCheckAttrValConstr, "
		"type is missing... skipping validation of "
		"value constraint", NULL, NULL);
	    return;
	}

	/*
	* TODO: Try to avoid creating a new context.
	* TODO: This all is not very performant.
	*/
	type = item->subtypes;
	/*
	* Ensure there's validation context.
	*/
	if (ctxt->vctxt == NULL) {
	    if (xmlSchemaCreateVCtxtOnPCtxt(ctxt) == -1) {
		xmlSchemaPErr(ctxt, item->node,
		    XML_SCHEMAP_INTERNAL,
		    "Internal error: xmlSchemaCheckAttrValConstr, "
		    "creating a new validation context.\n",
		    NULL, NULL);
		return;
	    }
	}

	if (item->flags & XML_SCHEMAS_ATTR_FIXED)
	    node = (xmlNodePtr) xmlHasProp(item->node, BAD_CAST "fixed");
	else
	    node = (xmlNodePtr) xmlHasProp(item->node, BAD_CAST "default");
	ctxt->vctxt->node = node;
	ctxt->vctxt->cur = NULL;
	/*
	* NOTE: This call does not check the content nodes, 
	* since they are not available:
	* facet->node is just the node holding the facet 
	* definition, *not* the attribute holding the *value* 
	* of the facet.
	*/
	ret = xmlSchemaValidateSimpleTypeValue(ctxt->vctxt, type, 
	    item->defValue, 0, 1, 1, 0);
	if (ret == 0) {
	    /*
	    * Store the computed value.
	    */
    	    item->defVal = ctxt->vctxt->value;
	    ctxt->vctxt->value = NULL;	
	} else if (ret > 0) {
	    if (ctxt != NULL) {
		xmlSchemaPSimpleTypeErr(ctxt, 
		    XML_SCHEMAP_A_PROPS_CORRECT_2, 
		    NULL, NULL, node, 
		    type, NULL, item->defValue,
		    NULL, NULL, NULL);
	    }
	} else if (ret < 0) {
	    xmlSchemaPCustomErr(ctxt, XML_SCHEMAP_INTERNAL,
		NULL, NULL, node,
		"Internal error: xmlSchemaAttrCheckValConstr, "
		"failed to validate the value constraint of the "
		"attribute decl/use against the type '%s'",
		type->name); 	    
	}	   	
    }    
}

#if 0 /* Not used yet. */
static int
xmlSchemaCheckElemPropsCorrect(xmlSchemaParserCtxtPtr ctxt,
			       xmlSchemaElementPtr edecl)
{
    /*
    * TODO: 1 The values of the properties of an element declaration must be as 
    * described in the property tableau in The Element Declaration Schema 
    * Component (�3.3.1), modulo the impact of Missing Sub-components (�5.3).
    */
    /*
    * 2 If there is a {value constraint}, the canonical lexical 
    * representation of its value must be �valid� with respect to the {type 
    * definition} as defined in Element Default Valid (Immediate) (�3.3.6).
    *
    * NOTE: This is done in xmlSchemaCheckElemValConstr.
    */
    /*
    * 3 If there is a non-�absent� {substitution group affiliation}, 
    * then {scope} must be global.
    *
    * NOTE: This is done in xmlSchemaParseElement.
    * TODO: Move it to this layer here.
    */
    /*
    * TODO: 4 If there is a {substitution group affiliation}, the {type definition} 
    * of the element declaration must be validly derived from the {type 
    * definition} of the {substitution group affiliation}, given the value 
    * of the {substitution group exclusions} of the {substitution group 
    * affiliation}, as defined in Type Derivation OK (Complex) (�3.4.6) 
    * (if the {type definition} is complex) or as defined in 
    * Type Derivation OK (Simple) (�3.14.6) (if the {type definition} is 
    * simple). 
    */
    /*
    * TODO: 5 If the {type definition} or {type definition}'s {content type} 
    * is or is derived from ID then there must not be a {value constraint}.
    * Note: The use of ID as a type definition for elements goes beyond 
    * XML 1.0, and should be avoided if backwards compatibility is desired
    */
    /*
    * TODO: 6 Circular substitution groups are disallowed. That is, it must not 
    * be possible to return to an element declaration by repeatedly following 
    * the {substitution group affiliation} property.
    */
}
#endif

/**
 * xmlSchemaCheckElemValConstr:
 * @item:  an schema element declaration/particle
 * @ctxt:  a schema parser context
 * @name:  the name of the attribute
 * 
 * Validates the value constraints of an element declaration.
 *
 * Fixes finish doing the computations on the element declarations.
 */
static void
xmlSchemaCheckElemValConstr(xmlSchemaElementPtr decl,
			    xmlSchemaParserCtxtPtr ctxt, 
			    const xmlChar * name ATTRIBUTE_UNUSED)
{   
    if (decl->value != NULL) {
	int ret;
	xmlNodePtr node = NULL;
	xmlSchemaTypePtr type;

	/*
	* 2 If there is a {value constraint}, the canonical lexical 
	* representation of its value must be �valid� with respect to the {type 
	* definition} as defined in Element Default Valid (Immediate) (�3.3.6).
	*/    
	if (decl->subtypes == NULL) {
	    xmlSchemaPErr(ctxt, decl->node,
		XML_SCHEMAP_INTERNAL,
		"Internal error: xmlSchemaCheckElemValConstr, "
		"type is missing... skipping validation of "
		"the value constraint", NULL, NULL);
	    return;
	}
	/*
	* Ensure there's a validation context.
	*/
	if (xmlSchemaCreateVCtxtOnPCtxt(ctxt) == -1)
	    return;

	type = decl->subtypes;

	if (decl->node != NULL) {
	    if (decl->flags & XML_SCHEMAS_ELEM_FIXED)
		node = (xmlNodePtr) xmlHasProp(decl->node, BAD_CAST "fixed");
	    else
		node = (xmlNodePtr) xmlHasProp(decl->node, BAD_CAST "default");
	}
	ctxt->vctxt->node = node;
	ctxt->vctxt->cur = NULL;
	ret = xmlSchemaCheckCOSValidDefault(ctxt, ctxt->vctxt, type, decl->value, 
	    node);
	if (ret == 0) {
	    /*
	    * Consume the computed value.
	    */
    	    decl->defVal = ctxt->vctxt->value;
  	    ctxt->vctxt->value = NULL;
	} else if (ret < 0) {
	    xmlSchemaPCustomErr(ctxt, XML_SCHEMAP_INTERNAL,
		NULL, NULL, node,
		"Internal error: xmlSchemaElemCheckValConstr, "
		"failed to validate the value constraint of the "
		"element declaration '%s'",
		decl->name); 	    
	}
    }    
}


/**
 * xmlSchemaMiscRefFixup:
 * @item:  an schema component
 * @ctxt:  a schema parser context
 * @name:  the internal name of the component
 *
 * Resolves references of misc. schema components.
 */
static void
xmlSchemaMiscRefFixup(xmlSchemaTreeItemPtr item,
                   xmlSchemaParserCtxtPtr ctxt, 
		   const xmlChar * name ATTRIBUTE_UNUSED)
{
    if (item->type == XML_SCHEMA_TYPE_PARTICLE) {
	if ((item->children != NULL) && 
	    (item->children->type == XML_SCHEMA_EXTRA_QNAMEREF)) {
	    xmlSchemaQNameRefPtr ref = (xmlSchemaQNameRefPtr) item->children;
	    xmlSchemaTreeItemPtr refItem;
	    /*
	    * Resolve the reference.
	    */
	    item->children = NULL;
	    refItem = xmlSchemaGetNamedComponent(ctxt->schema,
		ref->itemType, ref->name, ref->targetNamespace);
	    if (refItem == NULL) {
		xmlSchemaPResCompAttrErr(ctxt, XML_SCHEMAP_SRC_RESOLVE,
		    NULL, NULL, GET_NODE(item), "ref", ref->name,
		    ref->targetNamespace, ref->itemType, NULL);		
	    } else {
		if (refItem->type == XML_SCHEMA_TYPE_GROUP) {
		    /*
		    * Assign the model group of the model group definition
		    * to the particle's "term".
		    */
		    item->children = refItem->children;
		} else
		    item->children = refItem;
	    }
	}
    }
}


/**
 * xmlSchemaAttrFixup:
 * @item:  an schema attribute declaration/use.
 * @ctxt:  a schema parser context
 * @name:  the name of the attribute 
 *
 * Fixes finish doing the computations on attribute declarations/uses.
 */
static void
xmlSchemaAttrFixup(xmlSchemaAttributePtr item,
                   xmlSchemaParserCtxtPtr ctxt, 
		   const xmlChar * name ATTRIBUTE_UNUSED)
{
    /* 
    * TODO: If including this is done twice (!) for every attribute.
    *       -> Hmm, check if this is still done.
    */
    /*
    * The simple type definition corresponding to the <simpleType> element 
    * information item in the [children], if present, otherwise the simple 
    * type definition �resolved� to by the �actual value� of the type 
    * [attribute], if present, otherwise the �simple ur-type definition�.
    */
    if (item->flags & XML_SCHEMAS_ATTR_INTERNAL_RESOLVED)
	return;
    item->flags |= XML_SCHEMAS_ATTR_INTERNAL_RESOLVED;
    if (item->subtypes != NULL)
        return;
    if (item->typeName != NULL) {
        xmlSchemaTypePtr type;

	type = xmlSchemaGetType(ctxt->schema, item->typeName,
	    item->typeNs);
	if ((type == NULL) || (! IS_SIMPLE_TYPE(type))) {
	    xmlSchemaPResCompAttrErr(ctxt,
		XML_SCHEMAP_SRC_RESOLVE,
		NULL, (xmlSchemaTypePtr) item, item->node,
		"type", item->typeName, item->typeNs, 
		XML_SCHEMA_TYPE_SIMPLE, NULL);
	} else
	    item->subtypes = type;
	
    } else if (item->ref != NULL) {
        xmlSchemaAttributePtr decl;

	/*
	* We have an attribute use here; assign the referenced 
	* attribute declaration.
	*/
	/*
	* TODO: Evaluate, what errors could occur if the declaration is not
	* found. It might be possible that the "typefixup" might crash if
	* no ref declaration was found.
	*/
	decl = xmlSchemaGetAttribute(ctxt->schema, item->ref, item->refNs);
        if (decl == NULL) {
	    xmlSchemaPResCompAttrErr(ctxt,
	    	XML_SCHEMAP_SRC_RESOLVE,
		NULL, (xmlSchemaTypePtr) item, item->node,
		"ref", item->ref, item->refNs, 
		XML_SCHEMA_TYPE_ATTRIBUTE, NULL);
            return;
        }
	item->refDecl = decl;
        xmlSchemaAttrFixup(decl, ctxt, NULL);
	
        item->subtypes = decl->subtypes;
	/*
	* Attribute Use Correct
	* au-props-correct.2: If the {attribute declaration} has a fixed 
	* {value constraint}, then if the attribute use itself has a 
	* {value constraint}, it must also be fixed and its value must match 
	* that of the {attribute declaration}'s {value constraint}.
	*/
	if ((decl->flags & XML_SCHEMAS_ATTR_FIXED) && 
	    (item->defValue != NULL)) {
	    if (((item->flags & XML_SCHEMAS_ATTR_FIXED) == 0) ||
		(!xmlStrEqual(item->defValue, decl->defValue))) {
		xmlSchemaPCustomErr(ctxt,
		    XML_SCHEMAP_AU_PROPS_CORRECT_2, 
		    NULL, NULL, item->node, 
		    "The value constraint must be fixed "
		    "and match the referenced attribute "
		    "declarations's value constraint '%s'",
		    decl->defValue);
	    }
	    /*
	    * FUTURE: One should change the values of the attr. use
	    * if ever validation should be attempted even if the
	    * schema itself was not fully valid.
	    */
	}
    } else {
	item->subtypes = xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYSIMPLETYPE);        
    }	
}

/**
 * xmlSchemaResolveIDCKeyRef:
 * @idc:  the identity-constraint definition
 * @ctxt:  the schema parser context
 * @name:  the attribute name
 *
 * Resolve keyRef references to key/unique IDCs.
 */
static void
xmlSchemaResolveIDCKeyRef(xmlSchemaIDCPtr idc,
			  xmlSchemaParserCtxtPtr ctxt, 
			  const xmlChar * name ATTRIBUTE_UNUSED)
{  
    if (idc->type != XML_SCHEMA_TYPE_IDC_KEYREF)
        return;
    if (idc->ref->name != NULL) { 	
	idc->ref->item = (xmlSchemaBasicItemPtr) xmlHashLookup2(
	    ctxt->schema->idcDef, 
	    idc->ref->name, 
	    idc->ref->targetNamespace);
        if (idc->ref->item == NULL) {
	    /* 
	    * TODO: It is actually not an error to fail to resolve.
	    */
	    xmlSchemaPResCompAttrErr(ctxt, 
		XML_SCHEMAP_SRC_RESOLVE,
		NULL, (xmlSchemaTypePtr) idc, idc->node,
		"refer", idc->ref->name, 
		idc->ref->targetNamespace, 
		XML_SCHEMA_TYPE_IDC_KEYREF, NULL);
            return;
	}        
    }
}

/**
 * xmlSchemaParse:
 * @ctxt:  a schema validation context
 *
 * parse a schema definition resource and build an internal
 * XML Shema struture which can be used to validate instances.
 * *WARNING* this interface is highly subject to change
 *
 * Returns the internal XML Schema structure built from the resource or
 *         NULL in case of error
 */
xmlSchemaPtr
xmlSchemaParse(xmlSchemaParserCtxtPtr ctxt)
{
    xmlSchemaPtr ret = NULL;
    xmlDocPtr doc;
    xmlNodePtr root;
    int preserve = 0;

    /*
    * This one is used if the schema to be parsed was specified via 
    * the API; i.e. not automatically by the validated instance document.
    */

    xmlSchemaInitTypes();

    if (ctxt == NULL)
        return (NULL);

    ctxt->nberrors = 0;
    ctxt->counter = 0;
    ctxt->container = NULL;

    /*
     * First step is to parse the input document into an DOM/Infoset
     */
    if (ctxt->URL != NULL) {
        doc = xmlReadFile((const char *) ctxt->URL, NULL, 
	                  SCHEMAS_PARSE_OPTIONS);
        if (doc == NULL) {
	    xmlSchemaPErr(ctxt, NULL,
			  XML_SCHEMAP_FAILED_LOAD,
                          "xmlSchemaParse: could not load '%s'.\n",
                          ctxt->URL, NULL);
            return (NULL);
        }
    } else if (ctxt->buffer != NULL) {
        doc = xmlReadMemory(ctxt->buffer, ctxt->size, NULL, NULL,
	                    SCHEMAS_PARSE_OPTIONS);
        if (doc == NULL) {
	    xmlSchemaPErr(ctxt, NULL,
			  XML_SCHEMAP_FAILED_PARSE,
                          "xmlSchemaParse: could not parse.\n",
                          NULL, NULL);
            return (NULL);
        }
        doc->URL = xmlStrdup(BAD_CAST "in_memory_buffer");
        ctxt->URL = xmlDictLookup(ctxt->dict, BAD_CAST "in_memory_buffer", -1);
    } else if (ctxt->doc != NULL) {
        doc = ctxt->doc;
	preserve = 1;
    } else {
	xmlSchemaPErr(ctxt, NULL,
		      XML_SCHEMAP_NOTHING_TO_PARSE,
		      "xmlSchemaParse: could not parse.\n",
		      NULL, NULL);
        return (NULL);
    }

    /*
     * Then extract the root and Schema parse it
     */
    root = xmlDocGetRootElement(doc);
    if (root == NULL) {
	xmlSchemaPErr(ctxt, (xmlNodePtr) doc,
		      XML_SCHEMAP_NOROOT,
		      "The schema has no document element.\n", NULL, NULL);
	if (!preserve) {
	    xmlFreeDoc(doc);
	}
        return (NULL);
    }

    /*
     * Remove all the blank text nodes
     */
    xmlSchemaCleanupDoc(ctxt, root);

    /*
     * Then do the parsing for good
     */
    ret = xmlSchemaParseSchema(ctxt, root);
    if (ret == NULL) {
        if (!preserve) {
	    xmlFreeDoc(doc);
	}
        return (NULL);
    }
    ret->doc = doc;
    ret->preserve = preserve;
    ctxt->schema = ret;
    ctxt->ctxtType = NULL;
    ctxt->parentItem = NULL;


    if (ret->volatiles != NULL) {
	xmlSchemaItemListPtr list = (xmlSchemaItemListPtr) ret->volatiles;
	int i;
	xmlSchemaTreeItemPtr item;

	for (i = 0; i < list->nbItems; i++) {
	    item = (xmlSchemaTreeItemPtr) list->items[i];
	    if (item->type == XML_SCHEMA_TYPE_PARTICLE)
		xmlSchemaMiscRefFixup(item, ctxt, NULL);
	    /* xmlHashScan(ret->miscComps,
	    (xmlHashScanner) xmlSchemaMiscRefFixup, ctxt); */
	}
    }
    /*
     * Then fixup all attributes declarations
     */
    xmlHashScan(ret->attrDecl, (xmlHashScanner) xmlSchemaAttrFixup, ctxt);

    /*
     * Then fixup all attributes group declarations
     */
    xmlHashScan(ret->attrgrpDecl, (xmlHashScanner) xmlSchemaAttrGrpFixup,
                ctxt);

    /*
    * Resolve identity-constraint keyRefs.
    */
    xmlHashScan(ret->idcDef, (xmlHashScanner) xmlSchemaResolveIDCKeyRef, ctxt);
    /*
    * Check type defnitions for circular references.
    */
    xmlHashScan(ret->typeDecl, (xmlHashScanner) 
	xmlSchemaCheckTypeDefCircular, ctxt);
    /*
    * Check model groups defnitions for circular references.
    */
    xmlHashScan(ret->groupDecl, (xmlHashScanner) 
	xmlSchemaCheckGroupDefCircular, ctxt);

    /*
    * Check attribute groups for circular references.
    */
    xmlHashScan(ret->attrgrpDecl, (xmlHashScanner) 
	xmlSchemaCheckAttributeGroupCircular, ctxt);
    
    /*
     * Then fix references of element declaration; apply constraints.
     */    
    xmlHashScanFull(ret->elemDecl,
                    (xmlHashScannerFull) xmlSchemaElementFixup, ctxt);

    /*
    * We will stop here if the schema was not valid to avoid internal errors
    * on missing sub-components. This is not conforming to the spec, since it
    * allows missing components, but it might make further processing crash.
    * So see it as a very strict handling, which might be made more lax in the
    * future.
    */
    if (ctxt->nberrors != 0)
	goto exit;
    /*
     * Then fixup all types properties
     */    
    xmlHashScan(ret->typeDecl, (xmlHashScanner) xmlSchemaTypeFixup, ctxt);

    /*
     * Then build the content model for all complex types
     */
    xmlHashScan(ret->typeDecl,
                (xmlHashScanner) xmlSchemaBuildContentModel, ctxt);
    
    /*
    * Validate the value constraint of attribute declarations/uses.
    */
    xmlHashScan(ret->attrDecl, (xmlHashScanner) xmlSchemaCheckAttrValConstr, ctxt);

    /*
    * Validate the value constraint of element declarations.
    */
    xmlHashScan(ret->elemDecl, (xmlHashScanner) xmlSchemaCheckElemValConstr, ctxt);

exit:
    if (ctxt->nberrors != 0) {
        xmlSchemaFree(ret);
        ret = NULL;
    }
    return (ret);
}

/**
 * xmlSchemaSetParserErrors:
 * @ctxt:  a schema validation context
 * @err:  the error callback
 * @warn:  the warning callback
 * @ctx:  contextual data for the callbacks
 *
 * Set the callback functions used to handle errors for a validation context
 */
void
xmlSchemaSetParserErrors(xmlSchemaParserCtxtPtr ctxt,
                         xmlSchemaValidityErrorFunc err,
                         xmlSchemaValidityWarningFunc warn, void *ctx)
{
    if (ctxt == NULL)
        return;
    ctxt->error = err;
    ctxt->warning = warn;
    ctxt->userData = ctx;
}

/**
 * xmlSchemaGetParserErrors:
 * @ctxt:  a XMl-Schema parser context
 * @err: the error callback result
 * @warn: the warning callback result
 * @ctx: contextual data for the callbacks result
 *
 * Get the callback information used to handle errors for a parser context
 *
 * Returns -1 in case of failure, 0 otherwise
 */
int 
xmlSchemaGetParserErrors(xmlSchemaParserCtxtPtr ctxt,
							 xmlSchemaValidityErrorFunc * err,
							 xmlSchemaValidityWarningFunc * warn, void **ctx)
{
	if (ctxt == NULL)
		return(-1);
	if (err != NULL)
		*err = ctxt->error;
	if (warn != NULL)
		*warn = ctxt->warning;
	if (ctx != NULL)
		*ctx = ctxt->userData;
	return(0);
}

/**
 * xmlSchemaFacetTypeToString:
 * @type:  the facet type
 *
 * Convert the xmlSchemaTypeType to a char string.
 *
 * Returns the char string representation of the facet type if the
 *     type is a facet and an "Internal Error" string otherwise.
 */
static const xmlChar *
xmlSchemaFacetTypeToString(xmlSchemaTypeType type)
{
    switch (type) {
        case XML_SCHEMA_FACET_PATTERN:
            return (BAD_CAST "pattern");
        case XML_SCHEMA_FACET_MAXEXCLUSIVE:
            return (BAD_CAST "maxExclusive");
        case XML_SCHEMA_FACET_MAXINCLUSIVE:
            return (BAD_CAST "maxInclusive");
        case XML_SCHEMA_FACET_MINEXCLUSIVE:
            return (BAD_CAST "minExclusive");
        case XML_SCHEMA_FACET_MININCLUSIVE:
            return (BAD_CAST "minInclusive");
        case XML_SCHEMA_FACET_WHITESPACE:
            return (BAD_CAST "whiteSpace");
        case XML_SCHEMA_FACET_ENUMERATION:
            return (BAD_CAST "enumeration");
        case XML_SCHEMA_FACET_LENGTH:
            return (BAD_CAST "length");
        case XML_SCHEMA_FACET_MAXLENGTH:
            return (BAD_CAST "maxLength");
        case XML_SCHEMA_FACET_MINLENGTH:
            return (BAD_CAST "minLength");
        case XML_SCHEMA_FACET_TOTALDIGITS:
            return (BAD_CAST "totalDigits");
        case XML_SCHEMA_FACET_FRACTIONDIGITS:
            return (BAD_CAST "fractionDigits");
        default:
            break;
    }
    return (BAD_CAST "Internal Error");
}

static int
xmlSchemaGetWhiteSpaceFacetValue(xmlSchemaTypePtr type)
{
    xmlSchemaTypePtr anc;

    /* 
    * The normalization type can be changed only for types which are derived 
    * from xsd:string.
    */
    if (type->type == XML_SCHEMA_TYPE_BASIC) {
	if (type->builtInType == XML_SCHEMAS_STRING)
	    return(XML_SCHEMAS_FACET_PRESERVE);
	else if (type->builtInType == XML_SCHEMAS_NORMSTRING)
	    return(XML_SCHEMAS_FACET_REPLACE);
	else if (type->builtInType == XML_SCHEMAS_ANYSIMPLETYPE)
	    /*
	    * Note that we assume a whitespace of preserve for anySimpleType.
	    */
	    return(XML_SCHEMAS_FACET_PRESERVE);
	else {
	    /*
	    * For all �atomic� datatypes other than string (and types �derived� 
	    * by �restriction� from it) the value of whiteSpace is fixed to 
	    * collapse
	    */
	    return(XML_SCHEMAS_FACET_COLLAPSE);
	}		   	    
    } else if (type->flags & XML_SCHEMAS_TYPE_VARIETY_LIST) {
	/*
	* For list types the facet "whiteSpace" is fixed to "collapse". 
	*/
	return (XML_SCHEMAS_FACET_COLLAPSE);
    } else if (type->flags & XML_SCHEMAS_TYPE_VARIETY_UNION) {
	return (XML_SCHEMAS_FACET_UNKNOWN);
    } else if (type->facetSet != NULL) {
	xmlSchemaTypePtr anyST;
	xmlSchemaFacetLinkPtr lin;

	/*
	* Atomic types.
	*/
	anyST = xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYSIMPLETYPE);
	anc = type->baseType;
	do {
	    /*
	    * For all �atomic� datatypes other than string (and types �derived� 
	    * by �restriction� from it) the value of whiteSpace is fixed to 
	    * collapse
	    */
	    if ((anc->builtInType == XML_SCHEMAS_STRING) || 
		(anc->builtInType == XML_SCHEMAS_NORMSTRING)) {
		
		lin = type->facetSet;
		do {
		    if (lin->facet->type == XML_SCHEMA_FACET_WHITESPACE) {
			return(lin->facet->whitespace);
		    }
		    lin = lin->next;
		} while (lin != NULL);
		if (anc->builtInType == XML_SCHEMAS_NORMSTRING)
		    return (XML_SCHEMAS_FACET_REPLACE);
		else
		    return (XML_SCHEMAS_FACET_PRESERVE);
	    }
	    anc = anc->baseType;
	} while (anc != anyST);
	return (XML_SCHEMAS_FACET_COLLAPSE);
    }  
    return (-1);
}

/**
 * xmlSchemaValidateFacetsInternal:
 * @ctxt:  a schema validation context
 * @type:  the type holding the facets
 * @facets:  the list of facets to check
 * @value:  the lexical repr of the value to validate
 * @val:  the precomputed value
 * @fireErrors:  if 0, only internal errors will be fired;
 *		 otherwise all errors will be fired.
 *
 * Check a value against all facet conditions
 *
 * Returns 0 if the element is schemas valid, a positive error code
 *     number otherwise and -1 in case of internal or API error.
 */
static int
xmlSchemaValidateFacetsInternal(xmlSchemaValidCtxtPtr ctxt,
				xmlSchemaTypePtr type,
				const xmlChar * value,
				unsigned long length,
				int fireErrors)
{
    int ret = 0;
    xmlNodePtr node;
    xmlSchemaTypePtr biType; /* The build-in type. */
    xmlSchemaTypePtr tmpType;
    xmlSchemaFacetLinkPtr facetLink;
    int retFacet;
    xmlSchemaFacetPtr facet;
    unsigned long len = 0;
    xmlSchemaWhitespaceValueType ws;
        
    ws = (xmlSchemaWhitespaceValueType) xmlSchemaGetWhiteSpaceFacetValue(type);

#ifdef DEBUG_UNION_VALIDATION
    printf("Facets of type: '%s'\n", (const char *) type->name);
    printf("  fireErrors: %d\n", fireErrors);
#endif
        
    node = ctxt->node;
    /*
    * NOTE: Do not jump away, if the facetSet of the given type is
    * empty: until now, "pattern" facets of the *base types* need to
    * be checked as well.
    */
    biType = type->baseType;
    while ((biType != NULL) && (biType->type != XML_SCHEMA_TYPE_BASIC))
	biType = biType->baseType;
    if (biType == NULL) {
	xmlSchemaVErr(ctxt, node, XML_SCHEMAV_INTERNAL,		    
	    "Internal error: xmlSchemaValidateFacetsInternal, "
	    "the base type axis of the given type '%s' does not resolve to "
	    "a built-in type.\n",
	    type->name, NULL);	
	return (-1);
    }    
    
    if (type->facetSet != NULL) {
	facetLink = type->facetSet;
	while (facetLink != NULL) {
	    facet = facetLink->facet;
	    /*
	    * Skip the pattern "whiteSpace": it is used to 
	    * format the character content beforehand.
	    */	    
	    switch (facet->type) {
		case XML_SCHEMA_FACET_WHITESPACE:
		case XML_SCHEMA_FACET_PATTERN:
		case XML_SCHEMA_FACET_ENUMERATION:
		    break;
		case XML_SCHEMA_FACET_LENGTH:
		case XML_SCHEMA_FACET_MINLENGTH:
		case XML_SCHEMA_FACET_MAXLENGTH: 
		    if (type->flags & XML_SCHEMAS_TYPE_VARIETY_LIST) {
			ret = xmlSchemaValidateListSimpleTypeFacet(facet,
			    value, length, NULL);
			len = length;
		    } else
			ret = xmlSchemaValidateLengthFacetWhtsp(facet,
			    (xmlSchemaValType) biType->builtInType,
			    value, ctxt->value, &len, ws);
		    break;
		default:
		    ret = xmlSchemaValidateFacetWhtsp(facet, ws,
			biType->builtInType, value, ctxt->value, ws);
		    /*
		    * ret = xmlSchemaValidateFacet(biType, facet, value, 
		    *	ctxt->value);
		    */
	    }
	    if (ret < 0) {
		xmlSchemaVErr(ctxt, node, XML_SCHEMAV_INTERNAL,
		    "Internal error: xmlSchemaValidateFacetsInternal, "
		    "validating facet of type '%s'.\n",
		    type->name, NULL);
		break;
	    } else if ((ret > 0) && (fireErrors)) {
		xmlSchemaVFacetErr(ctxt, ret, node, value, len,
		    type, facet, NULL, NULL, NULL, NULL);
	    }

	    facetLink = facetLink->next;
	}
	
    }    
		
    if (ret >= 0) {
	xmlSchemaWhitespaceValueType fws;
	int found = 0;
	/*
	* Process enumerations. Facet values are in the value space
	* of the defining type's base type. This seems to be a bug in the
	* XML Schema 1.0 spec. Use the whitespace type of the base type.
	* Only the first set of enumerations in the ancestor-or-self axis
	* is used for validation.
	*/
	tmpType = type;	
	do {
	    /*
	    * Use the whitespace type of the base type.
	    */
	    fws = (xmlSchemaWhitespaceValueType)
		xmlSchemaGetWhiteSpaceFacetValue(tmpType->baseType);
	    retFacet = 0;
	    for (facet = tmpType->facets; facet != NULL; facet = facet->next) {
		if (facet->type != XML_SCHEMA_FACET_ENUMERATION)
		    continue;
		found = 1;
		retFacet = xmlSchemaValidateFacetWhtsp(facet, fws,
		    biType->builtInType, value, ctxt->value, ws);
		if (retFacet == 0) 
		    break;
		else if (retFacet < 0) {
		    xmlSchemaVErr(ctxt, node, XML_SCHEMAV_INTERNAL,
			"Internal error: xmlSchemaValidateFacetsInternal, "
			"validating enumeration facet '%s' of type '%s'.\n",
			facet->value, tmpType->name);
		    ret = -1;
		    break;
		}
	    }
	    if (retFacet <= 0)
		break;
	    tmpType = tmpType->baseType;
	} while ((! found) && (tmpType != NULL) &&
	    (tmpType->type != XML_SCHEMA_TYPE_BASIC));
	if (retFacet > 0) {
	    ret = XML_SCHEMAV_CVC_ENUMERATION_VALID;
	    if (fireErrors) {
		xmlSchemaVFacetErr(ctxt, ret, node, value, 0, type, NULL, 
		    NULL, NULL, NULL, NULL);
	    }
	}		
    }

    if (ret >= 0) {
	/*
	* Process patters. Pattern facets are ORed at type level 
	* and ANDed if derived. Walk the base type axis.
	*/
	tmpType = type;
	facet = NULL;
	do {
	    retFacet = 0;
	    for (facetLink = tmpType->facetSet; facetLink != NULL; 
		facetLink = facetLink->next) {
		if (facetLink->facet->type != XML_SCHEMA_FACET_PATTERN)
		    continue;
		retFacet = xmlSchemaValidateFacet(biType, facetLink->facet, 
		    value, ctxt->value);
		if (retFacet == 0) 
		    break;
		else if (retFacet < 0) {
		    xmlSchemaVErr(ctxt, node, XML_SCHEMAV_INTERNAL,
			"Internal error: xmlSchemaValidateFacetsInternal, "
			"validating 'pattern' facet '%s' of type '%s'.\n",
			facetLink->facet->value, tmpType->name);
		    ret = -1;
		    break;
		} else
		    /* Save the last non-validating facet. */
		    facet = facetLink->facet;
	    }
	    if (retFacet != 0)
		break;
	    tmpType = tmpType->baseType;
	} while ((tmpType != NULL) && (tmpType->type != XML_SCHEMA_TYPE_BASIC));
	if (retFacet > 0) {
	    ret = XML_SCHEMAV_CVC_PATTERN_VALID;
	    if (fireErrors) {
		xmlSchemaVFacetErr(ctxt, ret, node, value, 0, type, facet, 
		    NULL, NULL, NULL, NULL);
	    }
	}
    }	    
   
    return (ret);
}

/************************************************************************
 * 									*
 * 			Simple type validation				*
 * 									*
 ************************************************************************/


/************************************************************************
 * 									*
 * 			DOM Validation code				*
 * 									*
 ************************************************************************/

static int xmlSchemaValidateAttributes(xmlSchemaValidCtxtPtr ctxt,
                                       xmlNodePtr elem,
                                       xmlSchemaTypePtr type);
static int xmlSchemaValidateElementByType(xmlSchemaValidCtxtPtr ctxt,                                
					  xmlSchemaTypePtr type,
					  int isNil,
					  int valSimpleContent);

static void xmlSchemaBeginElement(xmlSchemaValidCtxtPtr vctxt);
static int xmlSchemaEndElement(xmlSchemaValidCtxtPtr vctxt);

/**
 * xmlSchemaGetFreshElemInfo:
 * @vctxt: the schema validation context
 *
 * Creates/reuses and initializes the element info item for 
 * the currect tree depth.
 *
 * Returns the element info item or NULL on API or internal errors.
 */
static xmlSchemaNodeInfoPtr
xmlSchemaGetFreshElemInfo(xmlSchemaValidCtxtPtr vctxt,
			  int depth)
{
    xmlSchemaNodeInfoPtr info = NULL;
    
    if (depth > vctxt->sizeElemInfos) {
	xmlSchemaVErr(vctxt, NULL, XML_SCHEMAV_INTERNAL,
	    "Internal error: xmlSchemaGetFreshElemInfo, "
	    "an inconsistent depth encountered.\n",
	    NULL, NULL);
	return (NULL);
    }
    if (vctxt->elemInfos == NULL) {	
	vctxt->elemInfos = (xmlSchemaNodeInfoPtr *) 
	    xmlMalloc(10 * sizeof(xmlSchemaNodeInfoPtr));
	if (vctxt->elemInfos == NULL) {
	    xmlSchemaVErrMemory(vctxt, 
		"allocating the element info array", NULL);
	    return (NULL);
	}
	memset(vctxt->elemInfos, 0, 10 * sizeof(xmlSchemaNodeInfoPtr));
	vctxt->sizeElemInfos = 10;
    } else if (vctxt->sizeElemInfos == vctxt->depth) {
	int i = vctxt->sizeElemInfos;

	vctxt->sizeElemInfos *= 2;
	vctxt->elemInfos = (xmlSchemaNodeInfoPtr *) 
	    xmlRealloc(vctxt->elemInfos, vctxt->sizeElemInfos * 
	    sizeof(xmlSchemaNodeInfoPtr));
	if (vctxt->elemInfos == NULL) {
	    xmlSchemaVErrMemory(vctxt, 
		"re-allocating the element info array", NULL);
	    return (NULL);
	}
	/*
	* We need the new memory to be NULLed.
	* TODO: Use memset instead?
	*/
	for (; i < vctxt->sizeElemInfos; i++)
	    vctxt->elemInfos[i] = NULL;
    } else
	info = vctxt->elemInfos[depth];

    if (info == NULL) {
	info = (xmlSchemaNodeInfoPtr) 
	    xmlMalloc(sizeof(xmlSchemaNodeInfo));
	if (info == NULL) {
	    xmlSchemaVErrMemory(vctxt, 
		"allocating an element info", NULL);
	    return (NULL);
	}	
	vctxt->elemInfos[depth] = info;
    }
    memset(info, 0, sizeof(xmlSchemaNodeInfo));
    info->depth = depth;
  
    return (info);
}

/**
 * xmlSchemaFreeAttrStates:
 * @state:  a list of attribute states
 *
 * Free the given list of attribute states
 *
 */
static void
xmlSchemaFreeAttributeStates(xmlSchemaAttrStatePtr state)
{
    xmlSchemaAttrStatePtr tmp;
    while (state != NULL) {
	tmp = state;
	state = state->next;	
	xmlFree(tmp);
    }
}

/**
 * xmlSchemaRegisterAttributes:
 * @ctxt:  a schema validation context
 * @attrs:  a list of attributes
 *
 * Register the list of attributes as the set to be validated on that element
 *
 * Returns -1 in case of error, 0 otherwise
 */
static int
xmlSchemaRegisterAttributes(xmlSchemaValidCtxtPtr ctxt, xmlAttrPtr attrs)
{
    xmlSchemaAttrStatePtr tmp;

    ctxt->attr = NULL;
    ctxt->attrTop = NULL;
    while (attrs != NULL) {
        if ((attrs->ns != NULL) &&
            (xmlStrEqual(attrs->ns->href, xmlSchemaInstanceNs))) {
            attrs = attrs->next;
            continue;
        }
	tmp = (xmlSchemaAttrStatePtr)
	    xmlMalloc(sizeof(xmlSchemaAttrState));
	if (tmp == NULL) {
	    xmlSchemaVErrMemory(ctxt, "registering attributes", NULL);
	    return (-1);
	}
	tmp->attr = attrs;
	tmp->state = XML_SCHEMAS_ATTR_UNKNOWN;
	tmp->next = NULL;
	tmp->decl = NULL;
	if (ctxt->attr == NULL) 
            ctxt->attr = tmp;
	else
	    ctxt->attrTop->next = tmp;
	ctxt->attrTop = tmp;
        attrs = attrs->next;
    }
    return (0);
}

#if 0 /* Currently not used */
/**
 * xmlSchemaValidateCheckNodeList
 * @nodelist: the list of nodes
 *
 * Check the node list is only made of text nodes and entities pointing
 * to text nodes
 *
 * Returns 1 if true, 0 if false and -1 in case of error
 */
static int
xmlSchemaValidateCheckNodeList(xmlNodePtr nodelist)
{
    while (nodelist != NULL) {
        if (nodelist->type == XML_ENTITY_REF_NODE) {
            TODO                /* implement recursion in the entity content */
        }
        if ((nodelist->type != XML_TEXT_NODE) &&
            (nodelist->type != XML_COMMENT_NODE) &&
            (nodelist->type != XML_PI_NODE) &&
            (nodelist->type != XML_CDATA_SECTION_NODE)) {
            return (0);
        }
        nodelist = nodelist->next;
    }
    return (1);
}
#endif

static void
xmlSchemaPostSchemaAssembleFixup(xmlSchemaParserCtxtPtr ctxt)
{
    int i, nbItems;
    xmlSchemaTypePtr item, *items;


    /*
    * During the Assemble of the schema ctxt->curItems has
    * been filled with the relevant new items. Fix those up.
    */
    nbItems = ctxt->assemble->nbItems;
    items = (xmlSchemaTypePtr *) ctxt->assemble->items;
    
    for (i = 0; i < nbItems; i++) {
	item = items[i];
	switch (item->type) {
	    case XML_SCHEMA_TYPE_ATTRIBUTE:
		xmlSchemaAttrFixup((xmlSchemaAttributePtr) item, ctxt, NULL);
		break;
	    case XML_SCHEMA_TYPE_ELEMENT:
		xmlSchemaElementFixup((xmlSchemaElementPtr) item, ctxt, 
		    NULL, NULL, NULL);
		break;
	    case XML_SCHEMA_TYPE_ATTRIBUTEGROUP:
		xmlSchemaAttrGrpFixup((xmlSchemaAttributeGroupPtr) item, 
		    ctxt, NULL);
		break;
	    case XML_SCHEMA_TYPE_PARTICLE:
		xmlSchemaMiscRefFixup((xmlSchemaTreeItemPtr) item, ctxt, NULL);
	    default:
		break;
	}
    }
    /*
    * Circularity checks.
    */
    for (i = 0; i < nbItems; i++) {
	item = items[i];
	switch (item->type) {
	    case XML_SCHEMA_TYPE_COMPLEX:
	    case XML_SCHEMA_TYPE_SIMPLE:
		xmlSchemaCheckTypeDefCircular(
		    (xmlSchemaTypePtr) item, ctxt, NULL);
		break;
	    case XML_SCHEMA_TYPE_GROUP:
		xmlSchemaCheckGroupDefCircular(
		    (xmlSchemaModelGroupDefPtr) item, ctxt, NULL);
		break;
	    case XML_SCHEMA_TYPE_ATTRIBUTEGROUP:
		xmlSchemaCheckAttributeGroupCircular(
		    (xmlSchemaAttributeGroupPtr) item, ctxt, NULL);
		break;
	    default:
		break;
	}
    }
    /*
    * Fixup for simple/complex types.
    */
    for (i = 0; i < nbItems; i++) {
	item = items[i];
	switch (item->type) {	    
            case XML_SCHEMA_TYPE_SIMPLE:
	    case XML_SCHEMA_TYPE_COMPLEX:
		xmlSchemaTypeFixup(item, ctxt, NULL);
		break;
	    default:
		break;
	}
    }
    /*
    * Build the content model for complex types.
    */
    for (i = 0; i < nbItems; i++) {
	item = items[i];
	switch (item->type) {	    
	    case XML_SCHEMA_TYPE_COMPLEX:
		xmlSchemaBuildContentModel(item, ctxt, NULL);
		break;
	    default:
		break;
	}
    } 
    /*
    * Validate value contraint values.
    */
    for (i = 0; i < nbItems; i++) {
	item = items[i];
	switch (item->type) {
	    case XML_SCHEMA_TYPE_ATTRIBUTE:
		xmlSchemaCheckAttrValConstr((xmlSchemaAttributePtr) item,
		    ctxt, NULL);
		break;
	    case XML_SCHEMA_TYPE_ELEMENT:
		xmlSchemaCheckElemValConstr((xmlSchemaElementPtr) item,
		    ctxt, NULL);
		break;
	    default:
		break;
	}
    }
}

/**
 * xmlSchemaAssembleByLocation:
 * @pctxt:  a schema parser context
 * @vctxt:  a schema validation context
 * @schema: the existing schema
 * @node: the node that fired the assembling
 * @nsName: the namespace name of the new schema
 * @location: the location of the schema
 *
 * Expands an existing schema by an additional schema.
 *
 * Returns 0 if the new schema is correct, a positive error code
 * number otherwise and -1 in case of an internal or API error.
 */
static int
xmlSchemaAssembleByLocation(xmlSchemaValidCtxtPtr vctxt,			    
			    xmlSchemaPtr schema,
			    xmlNodePtr node,
			    const xmlChar *nsName,
			    const xmlChar *location)
{
    const xmlChar *targetNs, *oldtns; 
    xmlDocPtr doc, olddoc;
    int oldflags, ret = 0;
    xmlNodePtr docElem;
    xmlSchemaParserCtxtPtr pctxt;

    /*
    * This should be used:
    * 1. on <import>(s)
    * 2. if requested by the validated instance 
    * 3. if requested via the API
    */
    if ((vctxt == NULL) || (schema == NULL))
	return (-1);
    /*
    * Create a temporary parser context.
    */
    if ((vctxt->pctxt == NULL) &&
	(xmlSchemaCreatePCtxtOnVCtxt(vctxt) == -1)) {
	xmlSchemaVErr(vctxt, node,
	    XML_SCHEMAV_INTERNAL,
	    "Internal error: xmlSchemaAssembleByLocation, "
	    "failed to create a temp. parser context.\n", 
	    NULL, NULL);
	return (-1);		
    }            
    pctxt = vctxt->pctxt;
    /*
    * Set the counter to produce unique names for anonymous items.
    */
    pctxt->counter = schema->counter;    
    /*
    * Acquire the schema document.
    */
    ret = xmlSchemaAcquireSchemaDoc(pctxt, schema, node,
	nsName, location, &doc, &targetNs, 0);
    if (ret != 0) {
	if (doc != NULL)
	    xmlFreeDoc(doc);
    } else if (doc != NULL) {
	docElem = xmlDocGetRootElement(doc);
	/*
	* Create new assemble info.
	*/
	if (pctxt->assemble == NULL) {
	    pctxt->assemble = xmlSchemaNewAssemble();
	    if (pctxt->assemble == NULL) {
		xmlSchemaVErrMemory(vctxt, 
		    "Memory error: xmlSchemaAssembleByLocation, "
		    "allocating assemble info", NULL);
		xmlFreeDoc(doc);
		return (-1);
	    }
	}
	/*
	* Save and reset the context & schema.
	*/
	oldflags = schema->flags;
	oldtns = schema->targetNamespace;
	olddoc = schema->doc;
	
	xmlSchemaClearSchemaDefaults(schema);
	schema->targetNamespace = targetNs;
	/* schema->nbCurItems = 0; */
	pctxt->schema = schema;
	pctxt->ctxtType = NULL;
	pctxt->parentItem = NULL;
	
	xmlSchemaParseSchemaDefaults(pctxt, schema, docElem);		
	xmlSchemaParseSchemaTopLevel(pctxt, schema, docElem->children);
	xmlSchemaPostSchemaAssembleFixup(pctxt);
	/*
	* Set the counter of items.
	*/
	schema->counter = pctxt->counter;
	/*
	* Free the list of assembled components.
	*/
	pctxt->assemble->nbItems = 0;
	/*
	* Restore the context & schema.
	*/
	schema->flags = oldflags;
	schema->targetNamespace = oldtns;
	schema->doc = olddoc;
	ret = pctxt->err;
    }        
    return (ret);
}

/**
 * xmlSchemaAssembleByXSIAttr:
 * @vctxt:  a schema validation context
 * @xsiAttr: an xsi attribute
 * @noNamespace: whether a schema with no target namespace is exptected
 *
 * Expands an existing schema by an additional schema using
 * the xsi:schemaLocation or xsi:noNamespaceSchemaLocation attribute
 * of an instance. If xsi:noNamespaceSchemaLocation is used, @noNamespace
 * must be set to 1.
 *
 * Returns 0 if the new schema is correct, a positive error code
 * number otherwise and -1 in case of an internal or API error.
 */
static int
xmlSchemaAssembleByXSIAttr(xmlSchemaValidCtxtPtr vctxt,
			 xmlAttrPtr xsiAttr,
			 int noNamespace)
{
    xmlChar *value;
    const xmlChar *cur, *end;
    const xmlChar *nsname = NULL, *location;
    int count = 0;
    int ret = 0;
    
    if (xsiAttr == NULL) {
	xmlSchemaVCustomErr(vctxt, XML_SCHEMAV_INTERNAL, 
	    NULL, NULL,
	    "Internal error: xmlSchemaAssembleByXSIAttr, "
	    "bad arguments", NULL);
	return (-1);
    }
    /*
    * Parse the value; we will assume an even number of values
    * to be given (this is how Xerces and XSV work).
    */
    value = xmlNodeGetContent((xmlNodePtr) xsiAttr);    
    cur = value;
    do {	
	if (noNamespace != 1) {
	    /*
	    * Get the namespace name.
	    */
	    while (IS_BLANK_CH(*cur))
		cur++;
	    end = cur;
	    while ((*end != 0) && (!(IS_BLANK_CH(*end))))
		end++;
	    if (end == cur)
		break;
	    count++;
	    nsname = xmlDictLookup(vctxt->schema->dict, cur, end - cur);		
	    cur = end;
	}
	/*
	* Get the URI.
	*/
	while (IS_BLANK_CH(*cur))
	    cur++;
	end = cur;
	while ((*end != 0) && (!(IS_BLANK_CH(*end))))
	    end++;
	if (end == cur)
	    break;
	count++;
	location = xmlDictLookup(vctxt->schema->dict, cur, end - cur);
	cur = end;	
	ret = xmlSchemaAssembleByLocation(vctxt, vctxt->schema, 
	    xsiAttr->parent, nsname, location);
	if (ret == -1) {
	    xmlSchemaVCustomErr(vctxt, 
		XML_SCHEMAV_INTERNAL,
		(xmlNodePtr) xsiAttr, NULL,
		"Internal error: xmlSchemaAssembleByXSIAttr, "
		"assembling schemata", NULL);
	    if (value != NULL)
		xmlFree(value);
	    return (-1);
	}
    } while (*cur != 0);
    if (value != NULL)
	xmlFree(value);
    return (ret);
}

/**
 * xmlSchemaAssembleByXSIElem:
 * @vctxt:  a schema validation context
 * @elem: an element node possibly holding xsi attributes
 * @noNamespace: whether a schema with no target namespace is exptected
 *
 * Assembles an existing schema by an additional schema using
 * the xsi:schemaLocation or xsi:noNamespaceSchemaLocation attributes
 * of the given @elem.
 *
 * Returns 0 if the new schema is correct, a positive error code
 * number otherwise and -1 in case of an internal or API error.
 */
static int
xmlSchemaAssembleByXSIElem(xmlSchemaValidCtxtPtr vctxt,  
			 xmlNodePtr elem)
{    
    int ret = 0, retNs = 0;
    xmlAttrPtr attr;

    attr = xmlHasNsProp(elem, BAD_CAST "schemaLocation", xmlSchemaInstanceNs);
    if (attr != NULL) {
	retNs = xmlSchemaAssembleByXSIAttr(vctxt, attr, 0);
	if (retNs == -1)
	    return (-1);
    }
    attr = xmlHasNsProp(elem, BAD_CAST "noNamespaceSchemaLocation", xmlSchemaInstanceNs);
    if (attr != NULL) {
	ret = xmlSchemaAssembleByXSIAttr(vctxt, attr, 1);
	if (ret == -1)
	    return (-1);
    }
    if (retNs != 0)
	return (retNs);
    else
	return (ret);
}

/**
 * xmlSchemaValidateCallback:
 * @ctxt:  a schema validation context
 * @name:  the name of the element detected (might be NULL)
 * @type:  the type
 *
 * A transition has been made in the automata associated to an element
 * content model
 */
static void
xmlSchemaValidateCallback(xmlSchemaValidCtxtPtr ctxt,
                          const xmlChar * name ATTRIBUTE_UNUSED,
                          xmlSchemaBasicItemPtr item, xmlNodePtr node)
{
    xmlNodePtr oldnode = ctxt->node;

    /*
    * @type->type will be XML_SCHEMA_TYPE_ANY or XML_SCHEMA_TYPE_ELEMENT.
    */
    ctxt->node = node;    
    ctxt->cur = node->children;

    xmlSchemaBeginElement(ctxt);

    /*
    * Assemble new schemata using xsi.
    */
    if (ctxt->xsiAssemble) {
	int ret;

	ret = xmlSchemaAssembleByXSIElem(ctxt, ctxt->node);
	if (ret == -1) {
	    xmlSchemaVCustomErr(ctxt, 
		XML_SCHEMAV_INTERNAL,
		ctxt->node, NULL, 	
		"Internal error: xmlSchemaValidateElement, "
		"assembling schema by xsi", NULL);
	    goto leave;
	}
	/*
	* NOTE: We won't react on schema parser errors here.
	* TODO: But a warning would be nice.
	*/
    }    
    switch (item->type) {
	case XML_SCHEMA_TYPE_ELEMENT: {
#ifdef DEBUG_CONTENT
	    xmlGenericError(xmlGenericErrorContext,
		"xmlSchemaValidateCallback: %s, %s, %s\n",
		name, ((xmlSchemaElementPtr) item)->name, node->name);
#endif
	    xmlSchemaValidateElementByDeclaration(ctxt, 
		(xmlSchemaElementPtr) item);
	    break;
	}
        case XML_SCHEMA_TYPE_ANY:
#ifdef DEBUG_CONTENT
	    xmlGenericError(xmlGenericErrorContext,
		"xmlSchemaValidateCallback: %s, <any>, %s\n",
		name, node->name);
#endif	    
	    xmlSchemaValidateElementByWildcard(ctxt,
		(xmlSchemaWildcardPtr) item);
            break;
	default: 
	    break;
    }
leave:

    xmlSchemaEndElement(ctxt);
    ctxt->node = oldnode;
}  

static int
xmlSchemaValidateNotation(xmlSchemaValidCtxtPtr vctxt, 
			  const xmlChar *value,
			  xmlSchemaValPtr *val,
			  xmlNodePtr node)
{
    int ret;
        
    ret = xmlValidateQName(value, 1);
    if (ret != 0)
	return (ret);

    {
	xmlChar *uri = NULL;
	xmlChar *local = NULL;
	xmlChar *prefix;
	
	local = xmlSplitQName2(value, &prefix);
	if (prefix != NULL) {
	    xmlNsPtr ns;

	    /*
	    * TODO: Make this streamable.
	    */
	    if ((node == NULL) || (node->doc == NULL)) {
    		xmlFree(prefix);
		xmlFree(local);
		return (3);
	    }
	    
	    ns = xmlSearchNs(node->doc, node, prefix);
	    if (ns == NULL) {
		xmlFree(prefix);
		xmlFree(local);
		return (1);
	    }
	}	
	if (prefix != NULL) {
	    if (xmlHashLookup2(vctxt->schema->notaDecl, local, uri) == NULL)
		ret = 1;
	} else if (xmlHashLookup2(vctxt->schema->notaDecl, value,
	    NULL) == NULL)
	    ret = 1;

	if ((ret == 0) && (val != NULL)) {
	    if (prefix != NULL) {
		(*val) = xmlSchemaNewNOTATIONValue(BAD_CAST local,
		    BAD_CAST xmlStrdup(uri));
		local = NULL;
	    } else 
		(*val) = xmlSchemaNewNOTATIONValue(BAD_CAST xmlStrdup(value),
		NULL);
	    if (*val == NULL)
		ret = -1;
	}
	if (local != NULL)
	    xmlFree(local);
    }

    return (ret);
}

static xmlSchemaTypePtr
xmlSchemaGetSimpleContentType(xmlSchemaTypePtr complexType)
{
    xmlSchemaTypePtr ret;

    if (complexType->type != XML_SCHEMA_TYPE_COMPLEX)
	return (NULL);
    if (complexType->contentTypeDef != NULL)
	return (complexType->contentTypeDef);
    /*
    * TODO: This is only a workaround until the simple content
    * type is computed for complex types with simple content.
    */
    ret = complexType->baseType;
    while (ret != NULL) {
	if (IS_SIMPLE_TYPE(ret))
	    return (ret);
	if (ret->builtInType == XML_SCHEMAS_ANYTYPE)
	    return (NULL);
	if ((ret->type == XML_SCHEMA_TYPE_COMPLEX) &&
	    (ret->contentTypeDef != NULL))
	    ret = ret->contentTypeDef;
	else
	    ret = ret->baseType;
    }
    return (ret);
}

/**
 * xmlSchemaValidateSimpleTypeValue:
 * @ctxt:  a schema validation context
 * @value: the value to be validated
 * @fireErrors: shall errors be reported?
 * @applyFacets: shall facets be applied?
 * @normalize: shall the value be normalized?
 * @checkNodes: shall the content nodes be checked?
 *
 * Validates a value by the given type (user derived or built-in).
 *
 * Returns 0 if the value is valid, a positive error code
 * number otherwise and -1 in case of an internal or API error.
 */
static int
xmlSchemaValidateSimpleTypeValue(xmlSchemaValidCtxtPtr ctxt, 
				 xmlSchemaTypePtr type,
				 const xmlChar *value,				 
				 int fireErrors,				 
				 int applyFacets,
				 int normalize,
				 int checkNodes)
{
    xmlNodePtr node;
    int ret = 0;  
    xmlChar *normValue = NULL;
    int wtsp;       
 
    node = ctxt->node;
    /* Save the current whitespace normalization type. */
    wtsp = ctxt->valueWS;
    /*
    * Normalize the value.
    */
    if (normalize && 
	(ctxt->valueWS != XML_SCHEMAS_FACET_COLLAPSE)) {
	int norm = xmlSchemaGetWhiteSpaceFacetValue(type);
	
	if ((norm != -1) && (norm > ctxt->valueWS)) {
	    if (norm == XML_SCHEMAS_FACET_COLLAPSE)
		normValue = xmlSchemaCollapseString(value);
	    else
		normValue = xmlSchemaWhiteSpaceReplace(value);
	    ctxt->valueWS = norm;
	    if (normValue != NULL)
		value = (const xmlChar *) normValue;
	}		
    }    
    /*
    * The nodes of a content must be checked only once,
    * this is not working since list types will fire this
    * multiple times.
    */
    if ((checkNodes == 1) && (ctxt->cur != NULL)) {
	xmlNodePtr cur = ctxt->cur;

	do {
	    switch (cur->type) {
	    case XML_TEXT_NODE:
	    case XML_CDATA_SECTION_NODE:
	    case XML_PI_NODE:
	    case XML_COMMENT_NODE:
	    case XML_XINCLUDE_START:
	    case XML_XINCLUDE_END:
		break;
	    case XML_ENTITY_REF_NODE:
	    case XML_ENTITY_NODE:
		/* TODO: Scour the entities for illegal nodes. */
		TODO break;
	    case XML_ELEMENT_NODE: {
	    /* NOTE: Changed to an internal error, since the 
	    * existence of an element node will be already checked in
	    * xmlSchemaValidateElementBySimpleType and in
	    * xmlSchemaValidateElementByComplexType.
		*/
		xmlSchemaVCustomErr(ctxt, 
		    XML_SCHEMAV_INTERNAL,
		    /* XML_SCHEMAS_ERR_INVALIDELEM, */
		    node, type,
		    "Element '%s' found in simple type content", 
		    cur->name);
		return (XML_SCHEMAV_INTERNAL);
				   }
	    case XML_ATTRIBUTE_NODE:
	    case XML_DOCUMENT_NODE:
	    case XML_DOCUMENT_TYPE_NODE:
	    case XML_DOCUMENT_FRAG_NODE:
	    case XML_NOTATION_NODE:
	    case XML_HTML_DOCUMENT_NODE:
	    case XML_DTD_NODE:
	    case XML_ELEMENT_DECL:
	    case XML_ATTRIBUTE_DECL:
	    case XML_ENTITY_DECL:
	    case XML_NAMESPACE_DECL:
#ifdef LIBXML_DOCB_ENABLED
	    case XML_DOCB_DOCUMENT_NODE: 
#endif		    		    		    
		xmlSchemaVCustomErr(ctxt, 
		    XML_SCHEMAV_INTERNAL,
		    /* XML_SCHEMAS_ERR_INVALIDELEM, */
		    node, NULL,
		    "Node of unexpected type found in simple type content",
		    NULL);
		return (XML_SCHEMAV_INTERNAL);
	    }
	    cur = cur->next;
	} while (cur != NULL);
    }

    if (type->type == XML_SCHEMA_TYPE_COMPLEX) {
	xmlSchemaTypePtr simpType, anyType;

	anyType = xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYTYPE);

	simpType = xmlSchemaGetSimpleContentType(type);
	if (simpType == NULL) {
	    xmlSchemaVErr(ctxt, node, XML_SCHEMAV_INTERNAL,
		"Internal error: xmlSchemaValidateSimpleTypeValue, "
		"failed to obtain the simple content type of the complex "
		"type '%s'\n",
		type->name, NULL);
	    return (-1);
	}
	ret = xmlSchemaValidateSimpleTypeValue(ctxt, simpType, value, 1, 0, 1, 0);
	if (ret < 0) {
	    xmlSchemaVErr(ctxt, node, XML_SCHEMAV_INTERNAL,
		"Internal error: xmlSchemaValidateSimpleTypeValue, "
		"validating complex type '%s'\n",
		type->name, NULL);
	} else if ((ret == 0) && (applyFacets) && (type->facetSet != NULL)) {
	    /* 
	    * Check facets.
	    *
	    * TODO: This is somehow not nice, since if an error occurs
	    * the reported type will be the complex type; the spec
	    * wants a simple type to be created on the complex type
	    * if it has a simple content. For now we have to live with
	    * it.
	    */	    
	    ret = xmlSchemaValidateFacetsInternal(ctxt, type,		
		value, 0, fireErrors);
	    if (ret < 0) {
		xmlSchemaVErr(ctxt, node, XML_SCHEMAV_INTERNAL,
		    "Internal error: xmlSchemaValidateSimpleTypeValue, "
		    "validating facets of complex type '%s'\n",
		    type->name, NULL);
	    } else if (ret > 0) {
		ret = XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_1;
		if (fireErrors) 
		    xmlSchemaVSimpleTypeErr(ctxt, ret, node, value, type);
	    }	
	}	
    } else if (type->type == XML_SCHEMA_TYPE_BASIC) {

	if (ctxt->value != NULL) {
	    xmlSchemaFreeValue(ctxt->value);
	    ctxt->value = NULL;
	}
	/*
	* STREAM-READ-CHILDREN.
	*/	    		
	if ((type->builtInType == XML_SCHEMAS_NOTATION) &&
	    (ctxt->schema != NULL)) {
	    /*
	    * NOTATIONs need to be processed here, since they need
	    * to lookup in the hashtable of NOTATION declarations.
	    */
	    ret = xmlSchemaValidateNotation(ctxt, value, &(ctxt->value), node); 
	} else
	    ret = xmlSchemaValPredefTypeNodeNoNorm(type, value, &(ctxt->value), node);
	if (ret > 0) {	    
	    if (type->flags & XML_SCHEMAS_TYPE_VARIETY_LIST) 
		ret = XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_2;
	    else
		ret = XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_1;	    
	    if (fireErrors)
		xmlSchemaVSimpleTypeErr(ctxt, ret, node, value, type);
	} else if (ret < 0) {
	    xmlSchemaVErr(ctxt, node, XML_SCHEMAV_INTERNAL,
		"Internal error: xmlSchemaValidateSimpleTypeValue, "
		"validating built-in type '%s'\n", type->name, NULL);
	} else if ((ctxt->value == NULL) && 
	    (type->builtInType == XML_SCHEMAS_STRING) &&
	    (ctxt->nodeInfo != NULL) &&
	    (ctxt->nodeInfo->flags & XML_SCHEMA_ELEM_INFO_VALUE_NEEDED)) {
	    xmlChar *valdup;
	    /*
	    * Create a precomputed string value for "string" as well if
	    * requested.
	    */
	    valdup = xmlStrdup(value);
	    ctxt->value = xmlSchemaNewStringValue(XML_SCHEMAS_STRING,
		BAD_CAST valdup);
	    if ((valdup != NULL) && (ctxt->value == NULL))
		xmlFree(valdup);
	}
    } else if (type->flags & XML_SCHEMAS_TYPE_VARIETY_ATOMIC) {        
	/* 1.2.1 if {variety} is �atomic� then the string must �match� 
	* a literal in the �lexical space� of {base type definition} 
	*/	
	ret = xmlSchemaValidateSimpleTypeValue(ctxt, type->baseType, value, 0, 0, 0, 0);
	if (ret < 0) {
	    xmlSchemaVErr(ctxt, node, XML_SCHEMAV_INTERNAL,
		"Internal error: xmlSchemaValidateSimpleTypeValue, "
		"validating atomic simple type '%s'\n",
		type->name, NULL);
	} else if (ret > 0) {	    
	    ret = XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_1;
	    if (fireErrors)
		xmlSchemaVSimpleTypeErr(ctxt, ret, node, value, type);	
	} else if ((applyFacets) && (type->facetSet != NULL)) {
	    /* 
	    * Check facets.
	    */	    	   	    
	    ret = xmlSchemaValidateFacetsInternal(ctxt, type,
		value, 0, fireErrors);
	    if (ret < 0) {
		xmlSchemaVErr(ctxt, node, XML_SCHEMAV_INTERNAL,
		    "Internal error: xmlSchemaValidateSimpleTypeValue, "
		    "validating facets of atomic simple type '%s'\n",
		    type->name, NULL);
	    } else if (ret > 0) {
		ret = XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_1;
		/*
		 Disabled, since the facet validation already reports errors.
		if (fireErrors) 
		    xmlSchemaVSimpleTypeErr(ctxt, ret, ctxt->cur, value, type);
		*/
	    }	
	}
    } else if (type->flags & XML_SCHEMAS_TYPE_VARIETY_LIST) {
        
	xmlSchemaTypePtr tmpType;
	const xmlChar *cur, *end;
	xmlChar *tmp;
	unsigned long len = 0;

	/* 1.2.2 if {variety} is �list� then the string must be a sequence 
	* of white space separated tokens, each of which �match�es a literal 
	* in the �lexical space� of {item type definition} 
	*/
	
	if (value == NULL)
	    value = BAD_CAST "";
	tmpType = xmlSchemaGetListSimpleTypeItemType(type);	
	cur = value;
	do {
	    while (IS_BLANK_CH(*cur))
		cur++;
	    end = cur;
	    while ((*end != 0) && (!(IS_BLANK_CH(*end))))
		end++;
	    if (end == cur)
		break;
	    tmp = xmlStrndup(cur, end - cur);
	    len++;
	    ret = xmlSchemaValidateSimpleTypeValue(ctxt, tmpType, tmp, 0, 1, 0, 0);
	    xmlFree(tmp);
	    if (ret < 0) {
		xmlSchemaVErr(ctxt, node, XML_SCHEMAV_INTERNAL,
		    "Internal error: xmlSchemaValidateSimpleTypeValue, "
		    "validating an item of list simple type '%s'\n",
		    type->name, NULL);	
		break;
	    } else if (ret > 0) {
		ret = XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_2;
		if (fireErrors)
		    xmlSchemaVSimpleTypeErr(ctxt, ret, node, value, type);
		break;
	    }	
	    cur = end;
	} while (*cur != 0);
	/* 
	* Check facets.
	*/
	if (ret < 0) {
	    xmlSchemaVErr(ctxt, node, XML_SCHEMAV_INTERNAL,
		"Internal error: xmlSchemaValidateSimpleTypeValue, "
		"validating list simple type '%s'\n",
		type->name, NULL);
	} else if ((ret == 0) && (applyFacets)) {
	    ret = xmlSchemaValidateFacetsInternal(ctxt, type,
		value, len, fireErrors);
	    if (ret < 0) {
		xmlSchemaVErr(ctxt, node, XML_SCHEMAV_INTERNAL,
		    "Internal error: xmlSchemaValidateSimpleTypeValue, "
		    "validating facets of list simple type '%s'\n",
		    type->name, NULL);
	    } else if (ret > 0) {
		ret = XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_2;
		/*
		 Disabled, since the facet validation already reports errors.
		if (fireErrors) 
		    xmlSchemaVSimpleTypeErr(ctxt, ret, ctxt->cur, value, type);
		*/
	    }	 	   
	   
	}
    } else if (type->flags & XML_SCHEMAS_TYPE_VARIETY_UNION) {
	xmlSchemaTypeLinkPtr memberLink;

	/*
	* TODO: For all datatypes �derived� by �union�  whiteSpace does 
	* not apply directly; however, the normalization behavior of �union� 
	* types is controlled by the value of whiteSpace on that one of the 
	* �memberTypes� against which the �union� is successfully validated. 
	*
	* This means that the value is normalized by the first validating
	* member type, then the facets of the union type are applied. This
	* needs changing of the value!
	*/	
	
	/*
	* 1.2.3 if {variety} is �union� then the string must �match� a 
	* literal in the �lexical space� of at least one member of 
	* {member type definitions} 
	*/
#ifdef DEBUG_UNION_VALIDATION
	printf("Union ST     : '%s'\n", (const char *) type->name);
	printf("  fireErrors : %d\n", fireErrors);
	printf("  applyFacets: %d\n", applyFacets);
#endif
	memberLink = xmlSchemaGetUnionSimpleTypeMemberTypes(type);
	if (memberLink == NULL) {
	    xmlSchemaVErr(ctxt, node, XML_SCHEMAV_INTERNAL,
		"Internal error: xmlSchemaValidateSimpleTypeValue, "
		"union simple type '%s' has no member types\n",
		type->name, NULL);
	    ret = -1;
	} 
	if (ret == 0) {
	    while (memberLink != NULL) {
		ret = xmlSchemaValidateSimpleTypeValue(ctxt, memberLink->type, 
		    value, 0, 1, 1, 0);
		if ((ret <= 0) || (ret == 0))
		    break;	    
		memberLink = memberLink->next;
	    }     
	    if (ret < 0) {
		xmlSchemaVErr(ctxt, node, XML_SCHEMAV_INTERNAL,
		    "Internal error: xmlSchemaValidateSimpleTypeValue, "
		    "validating members of union simple type '%s'\n",
		    type->name, NULL);
	    } else if (ret > 0) {
		ret = XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_3;
		if (fireErrors)
		    xmlSchemaVSimpleTypeErr(ctxt, ret, node, value, type);
	    }
	}
	/*
	* Apply facets (pattern, enumeration).	
	*/
	if ((ret == 0) && (applyFacets) && (type->facetSet != NULL)) {
	    int mws;
	    /*
	    * The normalization behavior of �union� types is controlled by 
	    * the value of whiteSpace on that one of the �memberTypes� 
	    * against which the �union� is successfully validated. 
	    */		    
	    if (normValue != NULL) {
		xmlSchemaVErr(ctxt, node, XML_SCHEMAV_INTERNAL,
		    "Internal error: xmlSchemaValidateSimpleTypeValue, "
		    "the value was already normalized for the union simple "
		    "type '%s'.\n", type->name, NULL);
	    }
	    mws = xmlSchemaGetWhiteSpaceFacetValue(memberLink->type);
	    if (mws > ctxt->valueWS) {
		if (mws == XML_SCHEMAS_FACET_COLLAPSE)
		    normValue = xmlSchemaCollapseString(value);
		else
		    normValue = xmlSchemaWhiteSpaceReplace(value);
		if (normValue != NULL)
		    value = (const xmlChar *) normValue;
	    }

	    ret = xmlSchemaValidateFacetsInternal(ctxt, type,
		value, 0, fireErrors);
	    if (ret < 0) {
		xmlSchemaVErr(ctxt, node, XML_SCHEMAV_INTERNAL,
		    "Internal error: xmlSchemaValidateSimpleTypeValue, "
		    "validating facets of union simple type '%s'\n",
		    type->name, NULL);
	    } else if (ret > 0) {
		ret = XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_3;
		/*
		if (fireErrors)
		    xmlSchemaVSimpleTypeErr(ctxt, ret, ctxt->cur, value, type);
		*/
	    }	
	}
    }           
    ctxt->valueWS = wtsp;
    if (normValue != NULL)
	xmlFree(normValue);
    return (ret);
}

/**
 * xmlSchemaValidateSimpleTypeElement:
 * @ctxt:  a schema validation context
 * @node:  the element node to be validated.
 *
 * Validate the element against a simple type.
 *
 * Returns 0 if the element is valid, a positive error code
 * number otherwise and -1 in case of an internal or API error.
 */
static int
xmlSchemaValidateElementBySimpleType(xmlSchemaValidCtxtPtr ctxt, 
				     xmlSchemaTypePtr type,
				     int isNil,
				     int valSimpleContent)
{
    xmlSchemaTypePtr oldtype;
    xmlNodePtr node;
    xmlAttrPtr attr;
    xmlNodePtr cur;
    int ret = 0, retval = 0;
        
    if ((ctxt == NULL) || (type == NULL)) {
        xmlSchemaVCustomErr(ctxt, XML_SCHEMAV_INTERNAL, NULL, NULL,
	    "Internal error: xmlSchemaValidateElementBySimpleType, "
	    "bad arguments", NULL);
        return (-1);    
    }

    oldtype = ctxt->type;
    node = ctxt->node;
    /* 
    * cvc-type: 3.1.2 The element information item must have no element 
    * information item [children].
    */   
    /*
    * STREAM: Child nodes are processed.
    */
    cur = node->children;
    while (cur != NULL) {
	/*
	* TODO: Entities, will they produce elements as well?
	*/
	if (cur->type == XML_ELEMENT_NODE) {
	    xmlSchemaVCustomErr(ctxt,
		XML_SCHEMAV_CVC_TYPE_3_1_2,
		node, type,		
		"No element content allowed", NULL);
	    ret = XML_SCHEMAV_CVC_TYPE_3_1_2;
	}
	cur = cur->next;
    }
    
    /*
    * cvc-type 3.1.1:
    *
    * The attributes of must be empty, excepting those whose namespace name 
    * is identical to http://www.w3.org/2001/XMLSchema-instance and whose local 
    * name is one of type, nil, schemaLocation or noNamespaceSchemaLocation.
    */   
    /*
    * STREAM: Attribute nodes are processed.
    */
    attr = node->properties;
    while (attr != NULL) {
        if ((attr->ns == NULL) ||
            (!xmlStrEqual(attr->ns->href, xmlSchemaInstanceNs)) ||
            ((!xmlStrEqual(attr->name, BAD_CAST "type")) &&
             (!xmlStrEqual(attr->name, BAD_CAST "nil")) &&
             (!xmlStrEqual(attr->name, BAD_CAST "schemaLocation")) &&
             (!xmlStrEqual
              (attr->name, BAD_CAST "noNamespaceSchemaLocation")))) {
	    xmlSchemaVIllegalAttrErr(ctxt, 
		XML_SCHEMAV_CVC_TYPE_3_1_1, attr);
	    ret = XML_SCHEMAV_CVC_TYPE_3_1_1;
        }
	attr = attr->next;
    }
    /*
    * This will skip validation if the type is 'anySimpleType' and
    * if the value was already validated (e.g. default values).
    */
    if ((! isNil) && 
	(valSimpleContent == 1) &&
	((type->type != XML_SCHEMA_TYPE_BASIC) ||
	 (type->builtInType != XML_SCHEMAS_ANYSIMPLETYPE))) {
	xmlChar *value;

	value = xmlNodeGetContent(node);
	/*
	* NOTE: This call will not check the content nodes, since
	* this should be checked here already.
	*/
	retval = xmlSchemaValidateSimpleTypeValue(ctxt, type, value, 
	    1, 1, 1, 0);
	if (value != NULL)
	    xmlFree(value);
	if (retval != 0)
	    ret = retval;
    }
    ctxt->type = oldtype;
    return (ret);
}

/**
 * xmlSchemaValQNameAcquire:
 * @value: the lexical represantation of the QName value
 * @node: the node to search for the corresponding namespace declaration
 * @nsName: the resulting namespace name if found
 *
 * Checks that a value conforms to the lexical space of the type QName;
 * if valid, the corresponding namespace name is searched and retured 
 * as a copy in @nsName. The local name is returned in @localName as
 * a copy.
 *
 * Returns 0 if valid, 1 if not valid by type, 2 if no corresponding 
 * namespace declaration was found in scope; -1 in case of an internal or 
 * API error.
 */
static int
xmlSchemaValQNameAcquire(const xmlChar *value, xmlNodePtr node,
			xmlChar **nsName, xmlChar **localName)
{
    int ret;
    xmlChar *local = NULL;

    if ((nsName == NULL) || (localName == NULL) || (node == NULL))
	return (-1);  
    *nsName = NULL;   
    *localName = NULL;
    ret = xmlValidateQName(value, 1);
    if (ret == 0) {
	xmlChar *prefix;
	xmlNsPtr ns;
	
	/*
	* NOTE: xmlSplitQName2 will return a duplicated
	* string.
	*/
	local = xmlSplitQName2(value, &prefix);
	if (local == NULL)
	    local = xmlStrdup(value);
	ns = xmlSearchNs(node->doc, node, prefix);
	/*
        * A namespace need not to be found if the prefix is NULL.
	*/
	if (ns != NULL) {
	    /*
	    * TODO: Is it necessary to duplicate the URI here?
	    */
	    *nsName = xmlStrdup(ns->href);
	} else if (prefix != NULL) {
	    xmlFree(prefix); 
	    if (local != NULL)
		xmlFree(local);
	    return (2);
	}		
	*localName = local;
	if (prefix != NULL)
	    xmlFree(prefix);    
    } else
	return (1);
    return (ret);
}

/**
 * xmlSchemaHasElemContent: 
 * @node:  the node
 *
 * Scours the content of the given node for element
 * nodes.
 *
 * Returns 1 if an element node is found,
 * 0 otherwise.
 */
static int
xmlSchemaHasElemContent(xmlNodePtr node)
{
    if (node == NULL)
	return (0);
    node = node->children;
    while (node != NULL) {
	if (node->type == XML_ELEMENT_NODE)
	    return (1);
	node = node->next;
    }
    return (0);
}
/**
 * xmlSchemaHasElemOrCharContent: 
 * @node:  the node
 *
 * Scours the content of the given node for element
 * and character nodes.
 *
 * Returns 1 if an element or character node is found,
 * 0 otherwise.
 */
static int
xmlSchemaHasElemOrCharContent(xmlNodePtr node)
{
    if (node == NULL)
	return (0);
    node = node->children;
    while (node != NULL) {
	switch (node->type) {
	    case XML_ELEMENT_NODE:	
	    /* 
	    * TODO: Ask Daniel if these are all character nodes.
	    */
	    case XML_TEXT_NODE:
	    case XML_CDATA_SECTION_NODE:
	    /*
	    * TODO: How XML_ENTITY_NODEs evaluated?
	    */
	    case XML_ENTITY_REF_NODE:
	    case XML_ENTITY_NODE:
		return (1);
		break;
	    default:
		break;
	}
	node = node->next;
    }
    return (0);
}

/************************************************************************
 * 									*
 *  Identity-constraints (IDC)                                          *
 * 									*
 ************************************************************************/

/**
 * xmlSchemaAugmentIDC:
 * @idcDef: the IDC definition
 *
 * Creates an augmented IDC definition item.
 *
 * Returns the item, or NULL on internal errors.
 */
static void
xmlSchemaAugmentIDC(xmlSchemaIDCPtr idcDef,
		    xmlSchemaValidCtxtPtr vctxt)
{
    xmlSchemaIDCAugPtr aidc;

    aidc = (xmlSchemaIDCAugPtr) xmlMalloc(sizeof(xmlSchemaIDCAug));
    if (aidc == NULL) {
	xmlSchemaVErrMemory(vctxt, 
	    "xmlSchemaAugmentIDC: allocating an augmented IDC definition",
	    NULL);
	return;
    }
    aidc->bubbleDepth = -1;
    aidc->def = idcDef;
    aidc->next = NULL;
    if (vctxt->aidcs == NULL)
	vctxt->aidcs = aidc;
    else {
	aidc->next = vctxt->aidcs;
	vctxt->aidcs = aidc;
    }
}

/**
 * xmlSchemaIDCNewBinding:
 * @idcDef: the IDC definition of this binding
 *
 * Creates a new IDC binding.
 *
 * Returns the new binding in case of succeeded, NULL on internal errors.
 */
static xmlSchemaPSVIIDCBindingPtr
xmlSchemaIDCNewBinding(xmlSchemaIDCPtr idcDef)
{
    xmlSchemaPSVIIDCBindingPtr ret;

    ret = (xmlSchemaPSVIIDCBindingPtr) xmlMalloc(
	    sizeof(xmlSchemaPSVIIDCBinding));
    if (ret == NULL) {
	xmlSchemaVErrMemory(NULL, 
	    "allocating a PSVI IDC binding item", NULL);
	return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaPSVIIDCBinding));
    ret->definition = idcDef;
    return (ret);
}

/**
 * xmlSchemaIDCStoreNodeTableItem:
 * @vctxt: the WXS validation context
 * @item: the IDC node table item
 *
 * The validation context is used to store an IDC node table items.
 * They are stored to avoid copying them if IDC node-tables are merged
 * with corresponding parent IDC node-tables (bubbling).
 *
 * Returns 0 if succeeded, -1 on internal errors.
 */
static int
xmlSchemaIDCStoreNodeTableItem(xmlSchemaValidCtxtPtr vctxt, 
			       xmlSchemaPSVIIDCNodePtr item)
{
    /*
    * Add to gobal list.
    */    
    if (vctxt->idcNodes == NULL) {			
	vctxt->idcNodes = (xmlSchemaPSVIIDCNodePtr *) 
	    xmlMalloc(20 * sizeof(xmlSchemaPSVIIDCNodePtr));
	if (vctxt->idcNodes == NULL) {
	    xmlSchemaVErrMemory(vctxt, 
		"allocating the IDC node table item list", NULL);
	    return (-1);
	}
	vctxt->sizeIdcNodes = 20;
    } else if (vctxt->sizeIdcNodes <= vctxt->nbIdcNodes) {
	vctxt->sizeIdcNodes *= 2;
	vctxt->idcNodes = (xmlSchemaPSVIIDCNodePtr *) 
	    xmlRealloc(vctxt->idcNodes, vctxt->sizeIdcNodes * 
	    sizeof(xmlSchemaPSVIIDCNodePtr));
	if (vctxt->idcNodes == NULL) {
	    xmlSchemaVErrMemory(vctxt, 
		"re-allocating the IDC node table item list", NULL);
	    return (-1);
	}
    }
    vctxt->idcNodes[vctxt->nbIdcNodes++] = item;
   
    return (0);
}

/**
 * xmlSchemaIDCStoreKey:
 * @vctxt: the WXS validation context
 * @item: the IDC key
 *
 * The validation context is used to store an IDC key.
 *
 * Returns 0 if succeeded, -1 on internal errors.
 */
static int
xmlSchemaIDCStoreKey(xmlSchemaValidCtxtPtr vctxt, 
		     xmlSchemaPSVIIDCKeyPtr key)
{
    /*
    * Add to gobal list.
    */    
    if (vctxt->idcKeys == NULL) {
	vctxt->idcKeys = (xmlSchemaPSVIIDCKeyPtr *) 
	    xmlMalloc(40 * sizeof(xmlSchemaPSVIIDCKeyPtr));
	if (vctxt->idcKeys == NULL) {
	    xmlSchemaVErrMemory(vctxt, 
		"allocating the IDC key storage list", NULL);
	    return (-1);
	}
	vctxt->sizeIdcKeys = 40;
    } else if (vctxt->sizeIdcKeys <= vctxt->nbIdcKeys) {
	vctxt->sizeIdcKeys *= 2;
	vctxt->idcKeys = (xmlSchemaPSVIIDCKeyPtr *) 
	    xmlRealloc(vctxt->idcKeys, vctxt->sizeIdcKeys * 
	    sizeof(xmlSchemaPSVIIDCKeyPtr));
	if (vctxt->idcKeys == NULL) {
	    xmlSchemaVErrMemory(vctxt, 
		"re-allocating the IDC key storage list", NULL);
	    return (-1);
	}
    }
    vctxt->idcKeys[vctxt->nbIdcKeys++] = key;
   
    return (0);
}

/**
 * xmlSchemaIDCAppendNodeTableItem:
 * @bind: the IDC binding
 * @ntItem: the node-table item
 *
 * Appends the IDC node-table item to the binding.
 *
 * Returns 0 on success and -1 on internal errors.
 */
static int 
xmlSchemaIDCAppendNodeTableItem(xmlSchemaPSVIIDCBindingPtr bind,
				xmlSchemaPSVIIDCNodePtr ntItem)
{
    if (bind->nodeTable == NULL) {
	bind->sizeNodes = 10;
	bind->nodeTable = (xmlSchemaPSVIIDCNodePtr *) 
	    xmlMalloc(10 * sizeof(xmlSchemaPSVIIDCNodePtr));
	if (bind->nodeTable == NULL) {
	    xmlSchemaVErrMemory(NULL, 
		"allocating an array of IDC node-table items", NULL);
	    return(-1);
	}	
    } else if (bind->sizeNodes <= bind->nbNodes) {
	bind->sizeNodes *= 2;
	bind->nodeTable = (xmlSchemaPSVIIDCNodePtr *) 
	    xmlRealloc(bind->nodeTable, bind->sizeNodes * 
		sizeof(xmlSchemaPSVIIDCNodePtr));
	if (bind->nodeTable == NULL) {
	    xmlSchemaVErrMemory(NULL, 
		"re-allocating an array of IDC node-table items", NULL);
	    return(-1);
	}
    }
    bind->nodeTable[bind->nbNodes++] = ntItem;
    return(0);   
}

/**
 * xmlSchemaIDCAquireBinding: 
 * @vctxt: the WXS validation context
 * @matcher: the IDC matcher
 *
 * Looks up an PSVI IDC binding, for the IDC definition and 
 * of the given matcher. If none found, a new one is created
 * and added to the IDC table.
 *
 * Returns an IDC binding or NULL on internal errors.
 */
static xmlSchemaPSVIIDCBindingPtr
xmlSchemaIDCAquireBinding(xmlSchemaValidCtxtPtr vctxt,
			  xmlSchemaIDCMatcherPtr matcher)
{
    xmlSchemaNodeInfoPtr info;

    info = vctxt->elemInfos[matcher->depth];

    if (info->idcTable == NULL) {
	info->idcTable = xmlSchemaIDCNewBinding(matcher->aidc->def);
	if (info->idcTable == NULL)
	    return (NULL);
	return(info->idcTable);
    } else {
	xmlSchemaPSVIIDCBindingPtr bind = NULL;
	
	bind = info->idcTable;
	do {
	    if (bind->definition == matcher->aidc->def)
		return(bind);
	    if (bind->next == NULL) {
		bind->next = xmlSchemaIDCNewBinding(matcher->aidc->def);
		if (bind->next == NULL)
		    return (NULL);
		return(bind->next);
	    }
	    bind = bind->next;
	} while (bind != NULL);	
    }
    return (NULL);
}

/**
 * xmlSchemaIDCFreeKey: 
 * @key: the IDC key
 *
 * Frees an IDC key together with its compiled value.
 */
static void 
xmlSchemaIDCFreeKey(xmlSchemaPSVIIDCKeyPtr key)
{
    if (key->compValue != NULL)
	xmlSchemaFreeValue(key->compValue);
    xmlFree(key);
}

/**
 * xmlSchemaIDCFreeBinding:
 *
 * Frees an IDC binding. Note that the node table-items
 * are not freed.
 */
static void
xmlSchemaIDCFreeBinding(xmlSchemaPSVIIDCBindingPtr bind)
{
    if (bind->nodeTable != NULL) {
	if (bind->definition->type == XML_SCHEMA_TYPE_IDC_KEYREF) {
	    int i;
	    /*
	    * Node-table items for keyrefs are not stored globally
	    * to the validation context, since they are not bubbled.
	    * We need to free them here.
	    */
	    for (i = 0; i < bind->nbNodes; i++) {
		xmlFree(bind->nodeTable[i]->keys);
		xmlFree(bind->nodeTable[i]);
	    }
	}
	xmlFree(bind->nodeTable);
    }
    xmlFree(bind);
}

/**
 * xmlSchemaIDCFreeIDCTable:
 * @bind: the first IDC binding in the list
 *
 * Frees an IDC table, i.e. all the IDC bindings in the list.
 */
static void
xmlSchemaIDCFreeIDCTable(xmlSchemaPSVIIDCBindingPtr bind)
{
    xmlSchemaPSVIIDCBindingPtr prev;

    while (bind != NULL) {
	prev = bind;		    
	bind = bind->next;
	xmlSchemaIDCFreeBinding(prev);
    }
}

/**
 * xmlSchemaIDCFreeMatcherList:
 * @matcher: the first IDC matcher in the list
 *
 * Frees a list of IDC matchers.
 */
static void
xmlSchemaIDCFreeMatcherList(xmlSchemaIDCMatcherPtr matcher)
{
    xmlSchemaIDCMatcherPtr next;

    while (matcher != NULL) {
	next = matcher->next;
	if (matcher->keySeqs != NULL) {
	    int i;
	    for (i = 0; i < matcher->sizeKeySeqs; i++)
		if (matcher->keySeqs[i] != NULL)
		    xmlFree(matcher->keySeqs[i]);
	    xmlFree(matcher->keySeqs);
	}
	xmlFree(matcher);
	matcher = next;
    }
}

/**
 * xmlSchemaAreValuesEqual:
 * @ta: the first type
 * @a: the first value
 * @tb: the second type
 * @b: the second value
 *
 * Compares two values.
 *
 * Returns 1 if they are equal, 0 if not and -1 on internal errors.
 */
static int
xmlSchemaAreValuesEqual(xmlSchemaValidCtxtPtr vctxt,
			xmlSchemaTypePtr ta,
			xmlSchemaValPtr a,
			xmlSchemaTypePtr tb,
			xmlSchemaValPtr b) 
{       
    /* Same user derived/built-in derived/built-in primitive types. */
    if (ta == tb)
	goto compareValue;
    
    /*
    * Note that comparison with anySimpleTypes with be supported for
    * string based types as well.
    */
#if 0    
    if ((ta->builtInType == XML_SCHEMAS_ANYSIMPLETYPE) ||
	(tb->builtInType == XML_SCHEMAS_ANYSIMPLETYPE))
	return(0);
#endif
    
    /*
    * 4.2.1 equal (data-types)
    *
    * the �value space�s of all �primitive� datatypes are disjoint 
    * (they do not share any values) 
    */
    if ((ta->builtInType != 0) && (tb->builtInType != 0) &&
	(ta->flags & XML_SCHEMAS_TYPE_BUILTIN_PRIMITIVE) && 
	(tb->flags & XML_SCHEMAS_TYPE_BUILTIN_PRIMITIVE))
	return(0);

    if ((ta->flags & XML_SCHEMAS_TYPE_VARIETY_LIST) ||
	(ta->flags & XML_SCHEMAS_TYPE_VARIETY_UNION) ||
	(tb->flags & XML_SCHEMAS_TYPE_VARIETY_LIST) ||
	(tb->flags & XML_SCHEMAS_TYPE_VARIETY_UNION)) {
	TODO
	return(0);
    }
    /*
    * (1) if a datatype T' is �derived� by �restriction� from an atomic datatype
    * T then the �value space� of T' is a subset of the �value space� of T. 
    */
    /*
    * (2) if datatypes T' and T'' are �derived� by �restriction� from a common 
    * atomic ancestor T then the �value space�s of T' and T'' may overlap. 
    */
    
    {
	xmlSchemaTypePtr pta = ta, ptb = tb;

	/* Note that we will compare the primitives here. */
	while ((pta->builtInType == 0) ||
	       ((pta->flags & XML_SCHEMAS_TYPE_BUILTIN_PRIMITIVE) == 0))
	    pta = pta->baseType;	
	while ((ptb->builtInType == 0) ||
	       ((ptb->flags & XML_SCHEMAS_TYPE_BUILTIN_PRIMITIVE) == 0))
	    ptb = ptb->baseType;
	if (pta == ptb)
	    goto compareValue;
	return(0);
    }
compareValue:
    {	
	int ret;
	int aws, bws;

	aws = xmlSchemaGetWhiteSpaceFacetValue(ta);
	bws = xmlSchemaGetWhiteSpaceFacetValue(tb);

	ret = xmlSchemaCompareValuesWhtsp(
	    a, (xmlSchemaWhitespaceValueType) aws,
	    b, (xmlSchemaWhitespaceValueType) bws);
	if (ret == 0) 
	    return(1);
	else if (ret == -2) {
	    xmlSchemaVErr(vctxt, vctxt->node,
		XML_SCHEMAV_INTERNAL,
		"Internal error: xmlSchemaAreValuesEqual, "
		"failed to compare the values.\n",
		NULL, NULL);
	    return(-1);
	} else
	    return(0);
    }
}

/**
 * xmlSchemaIDCAddStateObject:
 * @vctxt: the WXS validation context
 * @matcher: the IDC matcher
 * @sel: the XPath information
 * @parent: the parent "selector" state object if any
 * @type: "selector" or "field"
 *
 * Creates/reuses and activates state objects for the given
 * XPath information; if the XPath expression consists of unions,
 * multiple state objects are created for every unioned expression.
 *
 * Returns 0 on success and -1 on internal errors.
 */
static int
xmlSchemaIDCAddStateObject(xmlSchemaValidCtxtPtr vctxt,
			xmlSchemaIDCMatcherPtr matcher,
			xmlSchemaIDCSelectPtr sel,
			int type)
{
    xmlSchemaIDCStateObjPtr sto;

    /*
    * Reuse the state objects from the pool.
    */
    if (vctxt->xpathStatePool != NULL) {
	sto = vctxt->xpathStatePool;
	vctxt->xpathStatePool = sto->next;
	sto->next = NULL;
    } else {	
	/*
	* Create a new state object.
	*/
	sto = (xmlSchemaIDCStateObjPtr) xmlMalloc(sizeof(xmlSchemaIDCStateObj));
	if (sto == NULL) {
	    xmlSchemaVErrMemory(NULL,
		"allocating an IDC state object", NULL);
	    return (-1);
	}
	memset(sto, 0, sizeof(xmlSchemaIDCStateObj));
    }	
    /*
    * Add to global list. 
    */	
    if (vctxt->xpathStates != NULL)
	sto->next = vctxt->xpathStates;
    vctxt->xpathStates = sto;

    /*
    * Free the old xpath validation context.
    */
    if (sto->xpathCtxt != NULL)
	xmlFreeStreamCtxt((xmlStreamCtxtPtr) sto->xpathCtxt);

    /*
    * Create a new XPath (pattern) validation context.
    */
    sto->xpathCtxt = (void *) xmlPatternGetStreamCtxt(
	(xmlPatternPtr) sel->xpathComp);
    if (sto->xpathCtxt == NULL) {
	xmlSchemaVErr(vctxt, vctxt->node,
	    XML_SCHEMAV_INTERNAL,
	    "Internal error: xmlSchemaIDCAddStateObject, "
	    "failed to create the XPath validation context.\n",
	    NULL, NULL);
	return (-1);
    }    
    sto->type = type;
    sto->depth = vctxt->depth;
    sto->matcher = matcher;
    sto->sel = sel;
    sto->nbHistory = 0;
    
#if DEBUG_IDC
    xmlGenericError(xmlGenericErrorContext, "IDC:   STO push '%s'\n",
	sto->sel->xpath);
#endif
    return (0);
}

/**
 * xmlSchemaXPathEvaluate:
 * @vctxt: the WXS validation context
 * @nodeType: the nodeType of the current node
 *
 * Evaluates all active XPath state objects.
 *
 * Returns the number of IC "field" state objects which resolved to
 * this node, 0 if none resolved and -1 on internal errors.
 */
static int
xmlSchemaXPathEvaluate(xmlSchemaValidCtxtPtr vctxt,
		       xmlElementType nodeType)
{
    xmlSchemaIDCStateObjPtr sto, head = NULL, first;
    int res, resolved = 0, depth = vctxt->depth;
        
    if (vctxt->xpathStates == NULL)
	return (0);

    if (nodeType == XML_ATTRIBUTE_NODE)
	depth++;
#if DEBUG_IDC
    {
	xmlChar *str = NULL;
	xmlGenericError(xmlGenericErrorContext, 
	    "IDC: EVAL on %s, depth %d, type %d\n",	    
	    xmlSchemaFormatNsUriLocal(&str, vctxt->nodeInfo->namespaceName,
		vctxt->nodeInfo->localName), depth, nodeType);
	FREE_AND_NULL(str)
    }
#endif
    /*
    * Process all active XPath state objects.
    */
    first = vctxt->xpathStates;
    sto = first;
    while (sto != head) {
#if DEBUG_IDC
	if (sto->type == XPATH_STATE_OBJ_TYPE_IDC_SELECTOR)
	    xmlGenericError(xmlGenericErrorContext, "IDC:   ['%s'] selector '%s'\n", 
		sto->matcher->aidc->def->name, sto->sel->xpath);
	else
	    xmlGenericError(xmlGenericErrorContext, "IDC:   ['%s'] field '%s'\n", 
		sto->matcher->aidc->def->name, sto->sel->xpath);
#endif
	if (nodeType == XML_ELEMENT_NODE)
	    res = xmlStreamPush((xmlStreamCtxtPtr) sto->xpathCtxt,
		vctxt->nodeInfo->localName, vctxt->nodeInfo->namespaceName);
	else
	    res = xmlStreamPushAttr((xmlStreamCtxtPtr) sto->xpathCtxt,
		vctxt->nodeInfo->localName, vctxt->nodeInfo->namespaceName);

	if (res == -1) {
	    xmlSchemaVErr(vctxt, vctxt->node,
		XML_SCHEMAV_INTERNAL,
		"Internal error: xmlSchemaXPathEvaluate, "
		"failed to evaluate a node.\n",
		NULL, NULL);
	    return (-1);
	}
	if (res == 0)
	    goto next_sto;
	/*
	* Full match.
	*/
#if DEBUG_IDC
	xmlGenericError(xmlGenericErrorContext, "IDC:     "
	    "MATCH\n");
#endif
	/*
	* Register a match in the state object history.
	*/
	if (sto->history == NULL) {
	    sto->history = (int *) xmlMalloc(5 * sizeof(int));
	    if (sto->history == NULL) {
		xmlSchemaVErrMemory(NULL, 
		    "allocating the state object history", NULL);
		return(-1);
	    }
	    sto->sizeHistory = 10;
	} else if (sto->sizeHistory <= sto->nbHistory) {
	    sto->sizeHistory *= 2;
	    sto->history = (int *) xmlRealloc(sto->history,
		sto->sizeHistory * sizeof(int));
	    if (sto->history == NULL) {
		xmlSchemaVErrMemory(NULL, 
		    "re-allocating the state object history", NULL);
		return(-1);
	    }
	}		
	sto->history[sto->nbHistory++] = depth;

#ifdef DEBUG_IDC
	xmlGenericError(xmlGenericErrorContext, "IDC:       push match '%d'\n",
	    vctxt->depth);
#endif

	if (sto->type == XPATH_STATE_OBJ_TYPE_IDC_SELECTOR) {
	    xmlSchemaIDCSelectPtr sel;
	    /*
	    * Activate state objects for the IDC fields of
	    * the IDC selector.
	    */
#if DEBUG_IDC
	    xmlGenericError(xmlGenericErrorContext, "IDC:     "
		"activating field states\n");
#endif
	    sel = sto->matcher->aidc->def->fields;
	    while (sel != NULL) {
		if (xmlSchemaIDCAddStateObject(vctxt, sto->matcher,
		    sel, XPATH_STATE_OBJ_TYPE_IDC_FIELD) == -1)
		    return (-1);
		sel = sel->next;
	    }
	} else if (sto->type == XPATH_STATE_OBJ_TYPE_IDC_FIELD) {
	    /*
	    * An IDC key node was found.
	    */
#if DEBUG_IDC
	    xmlGenericError(xmlGenericErrorContext,
		"IDC:     key found\n");
#endif
	    /*
	    * Notify that the character value of this node is
	    * needed.
	    */
	    if (resolved == 0)
		vctxt->nodeInfo->flags |= XML_SCHEMA_ELEM_INFO_VALUE_NEEDED;
	    resolved++;
	}
next_sto:
	if (sto->next == NULL) {
	    /*
	    * Evaluate field state objects created on this node as well.
	    */
	    head = first;
	    sto = vctxt->xpathStates;
	} else
	    sto = sto->next;
    }
    return (resolved);
}

/**
 * xmlSchemaXPathProcessHistory:
 * @vctxt: the WXS validation context
 * @type: the simple/complex type of the current node if any at all
 * @compValue: the precompiled value
 *
 * Processes and pops the history items of the IDC state objects.
 * IDC key-sequences are validated/created on IDC bindings.
 * 
 * Returns 0 on success and -1 on internal errors.
 */
static int
xmlSchemaXPathProcessHistory(xmlSchemaValidCtxtPtr vctxt,
			     int depth)
{
    xmlSchemaIDCStateObjPtr sto, nextsto;
    int res, matchDepth;
    xmlSchemaPSVIIDCKeyPtr key = NULL;
    xmlSchemaTypePtr type = vctxt->nodeInfo->typeDef;

    if (vctxt->xpathStates == NULL)
	return (0);
    sto = vctxt->xpathStates;

#if DEBUG_IDC
    {
	xmlChar *str = NULL;
	xmlGenericError(xmlGenericErrorContext, 
	    "IDC: BACK on %s, depth %d\n",
	    xmlSchemaFormatNsUriLocal(&str, vctxt->nodeInfo->namespaceName,
		vctxt->nodeInfo->localName), vctxt->depth);
	FREE_AND_NULL(str)
    }
#endif    
    /*
    * Evaluate the state objects.
    */
    while (sto != NULL) {
	xmlStreamPop((xmlStreamCtxtPtr) sto->xpathCtxt);
#if DEBUG_IDC
	xmlGenericError(xmlGenericErrorContext, "IDC:   stream pop '%s'\n",
	    sto->sel->xpath);
#endif
	if (sto->nbHistory == 0)
	    goto deregister_check;

	matchDepth = sto->history[sto->nbHistory -1];

	/*
	* Only matches at the current depth are of interest.
	*/
	if (matchDepth != depth) {
	    sto = sto->next;
	    continue;
	}	
	if (sto->type == XPATH_STATE_OBJ_TYPE_IDC_FIELD) {
	    if (! IS_SIMPLE_TYPE(type)) {
		/*
		* Not qualified if the field resolves to a node of non
		* simple type.
		*/	
		xmlSchemaStreamVCustomErr(vctxt,
		    XML_SCHEMAV_CVC_IDC,
		    vctxt->nodeInfo, 
		    (xmlSchemaTypePtr) sto->matcher->aidc->def,
		    "The field '%s' does evaluate to a node of "
		    "non-simple type", sto->sel->xpath, NULL);
		
		sto->nbHistory--;
		goto deregister_check;
	    }
	    if ((key == NULL) && (vctxt->nodeInfo->value == NULL)) {
		/*
		* Failed to provide the normalized value; maby
		* the value was invalid.
		*/
		xmlSchemaStreamVCustomErr(vctxt, 
		    XML_SCHEMAV_CVC_IDC,
		    vctxt->nodeInfo,
		    (xmlSchemaTypePtr) sto->matcher->aidc->def,
		    "Warning: No precomputed value available, the value "
		    "was either invalid or something strange happend",
		    NULL, NULL);
		/*
		xmlSchemaVErr(vctxt, vctxt->nodeInfo->node, 
		    XML_SCHEMAV_INTERNAL,
		    "Internal error: xmlSchemaXPathProcessHistory, "
		    "computed value not available.\n",
		    NULL, NULL);
		*/
		sto->nbHistory--;
		goto deregister_check;
	    } else {
		xmlSchemaIDCMatcherPtr matcher = sto->matcher;
		xmlSchemaPSVIIDCKeyPtr *keySeq;
		int pos, idx;
		
		/*
		* The key will be anchored on the matcher's list of
		* key-sequences. The position in this list is determined
		* by the target node's depth relative to the matcher's
		* depth of creation (i.e. the depth of the scope element).
		*/		    
		pos = sto->depth - matcher->depth;
		idx = sto->sel->index;
		
		/*
		* Create/grow the array of key-sequences.
		*/
		if (matcher->keySeqs == NULL) {
		    if (pos > 9) 
			matcher->sizeKeySeqs = pos * 2;
		    else
			matcher->sizeKeySeqs = 10;
		    matcher->keySeqs = (xmlSchemaPSVIIDCKeyPtr **) 
			xmlMalloc(matcher->sizeKeySeqs *
			sizeof(xmlSchemaPSVIIDCKeyPtr *));			
		    if (matcher->keySeqs == NULL) {		
			xmlSchemaVErrMemory(NULL,
			    "allocating an array of key-sequences",
			    NULL);
			return(-1);
		    }
		    memset(matcher->keySeqs, 0,
			matcher->sizeKeySeqs *
			sizeof(xmlSchemaPSVIIDCKeyPtr *));
		} else if (pos >= matcher->sizeKeySeqs) {	
		    int i = matcher->sizeKeySeqs;
		    
		    matcher->sizeKeySeqs *= 2;
		    matcher->keySeqs = (xmlSchemaPSVIIDCKeyPtr **)
			xmlRealloc(matcher->keySeqs,
			matcher->sizeKeySeqs *
			sizeof(xmlSchemaPSVIIDCKeyPtr *));
		    if (matcher->keySeqs == NULL) {
			xmlSchemaVErrMemory(NULL,
			    "reallocating an array of key-sequences",
			    NULL);
			return (-1);
		    }
		    /*
		    * The array needs to be NULLed.
		    * TODO: Use memset?
		    */
		    for (; i < matcher->sizeKeySeqs; i++) 
			matcher->keySeqs[i] = NULL;			
		}
		
		/*
		* Get/create the key-sequence.
		*/
		keySeq = matcher->keySeqs[pos];		    
		if (keySeq == NULL) {	
		    goto create_sequence;
		} else {
		    if (keySeq[idx] != NULL) {
			/*
			* cvc-identity-constraint:
			* 3 For each node in the �target node set� all
			* of the {fields}, with that node as the context
			* node, evaluate to either an empty node-set or
			* a node-set with exactly one member, which must
			* have a simple type.
			* 
			* The key was already set; report an error.
			*/
			xmlSchemaStreamVCustomErr(vctxt, 
			    XML_SCHEMAV_CVC_IDC,
			    vctxt->nodeInfo,
			    (xmlSchemaTypePtr) matcher->aidc->def,
			    "The field '%s' evaluates to a node-set "
			    "with more than one member",
			    sto->sel->xpath, NULL);
			sto->nbHistory--;
			goto deregister_check;
		    } else {
			goto create_key;
		    }
		}
		
create_sequence:
		/*
		* Create a key-sequence.
		*/
		keySeq = (xmlSchemaPSVIIDCKeyPtr *) xmlMalloc(
		    matcher->aidc->def->nbFields * 
		    sizeof(xmlSchemaPSVIIDCKeyPtr));
		if (keySeq == NULL) {
		    xmlSchemaVErrMemory(NULL, 
			"allocating an IDC key-sequence", NULL);
		    return(-1);			
		}	
		memset(keySeq, 0, matcher->aidc->def->nbFields * 
		    sizeof(xmlSchemaPSVIIDCKeyPtr));
		matcher->keySeqs[pos] = keySeq;
create_key:
		/*
		* Created a key once per node only.
		*/  
		if (key == NULL) {
		    key = (xmlSchemaPSVIIDCKeyPtr) xmlMalloc(
			sizeof(xmlSchemaPSVIIDCKey));
		    if (key == NULL) {
			xmlSchemaVErrMemory(NULL,
			    "allocating a IDC key", NULL);
			xmlFree(keySeq);
			matcher->keySeqs[pos] = NULL;
			return(-1);			
		    }
		    /*
		    * Consume the compiled value.
		    */
		    key->type = type;
		    key->compValue = vctxt->nodeInfo->value;
		    vctxt->nodeInfo->value = NULL;
		    /*
		    * Store the key in a global list.
		    */
		    if (xmlSchemaIDCStoreKey(vctxt, key) == -1) {
			xmlSchemaIDCFreeKey(key);
			return (-1);
		    }
		}
		keySeq[idx] = key;		    
	    }
	} else if (sto->type == XPATH_STATE_OBJ_TYPE_IDC_SELECTOR) {
		
	    xmlSchemaPSVIIDCKeyPtr **keySeq = NULL;
	    xmlSchemaPSVIIDCBindingPtr bind;
	    xmlSchemaPSVIIDCNodePtr ntItem;
	    xmlSchemaIDCMatcherPtr matcher;
	    xmlSchemaIDCPtr idc;
	    int pos, i, j, nbKeys;
	    /*
	    * Here we have the following scenario:
	    * An IDC 'selector' state object resolved to a target node,
	    * during the time this target node was in the 
	    * ancestor-or-self axis, the 'field' state object(s) looked 
	    * out for matching nodes to create a key-sequence for this 
	    * target node. Now we are back to this target node and need
	    * to put the key-sequence, together with the target node 
	    * itself, into the node-table of the corresponding IDC 
	    * binding.
	    */
	    matcher = sto->matcher;
	    idc = matcher->aidc->def;
	    nbKeys = idc->nbFields;
	    pos = depth - matcher->depth;		
	    /*
	    * Check if the matcher has any key-sequences at all, plus
	    * if it has a key-sequence for the current target node.
	    */		
	    if ((matcher->keySeqs == NULL) ||
		(matcher->sizeKeySeqs <= pos)) {
		if (idc->type == XML_SCHEMA_TYPE_IDC_KEY)
		    goto selector_key_error;
		else
		    goto selector_leave;
	    }
	    
	    keySeq = &(matcher->keySeqs[pos]);		
	    if (*keySeq == NULL) {
		if (idc->type == XML_SCHEMA_TYPE_IDC_KEY)
		    goto selector_key_error;
		else
		    goto selector_leave;
	    }
	    
	    for (i = 0; i < nbKeys; i++) {
		if ((*keySeq)[i] == NULL) {
		    /*
		    * Not qualified, if not all fields did resolve.
		    */
		    if (idc->type == XML_SCHEMA_TYPE_IDC_KEY) {
			/*
			* All fields of a "key" IDC must resolve.
			*/
			goto selector_key_error;
		    }		    
		    goto selector_leave;
		}
	    }
	    /*
	    * All fields did resolve.
	    */
	    
	    /*
	    * 4.1 If the {identity-constraint category} is unique(/key),
	    * then no two members of the �qualified node set� have
	    * �key-sequences� whose members are pairwise equal, as
	    * defined by Equal in [XML Schemas: Datatypes].
	    *
	    * Get the IDC binding from the matcher and check for
	    * duplicate key-sequences.
	    */
	    bind = xmlSchemaIDCAquireBinding(vctxt, matcher);
	    if ((idc->type != XML_SCHEMA_TYPE_IDC_KEYREF) && 
		(bind->nbNodes != 0)) {
		xmlSchemaPSVIIDCKeyPtr ckey, bkey, *bkeySeq;
		
		i = 0;
		res = 0;
		/*
		* Compare the key-sequences, key by key.
		*/
		do {
		    bkeySeq = bind->nodeTable[i]->keys;		    
		    for (j = 0; j < nbKeys; j++) {
			ckey = (*keySeq)[j];
			bkey = bkeySeq[j];							
			res = xmlSchemaAreValuesEqual(vctxt, ckey->type,
			    ckey->compValue, bkey->type, bkey->compValue);
			if (res == -1) {
			    return (-1);
			} else if (res == 0)
			    break;
		    }
		    if (res == 1) {
			/*
			* Duplicate found.
			*/
			break;
		    }
		    i++;
		} while (i < bind->nbNodes);
		if (i != bind->nbNodes) {
		    /*   
		    * TODO: Try to report the key-sequence.
		    */
		    xmlSchemaStreamVCustomErr(vctxt, 
			XML_SCHEMAV_CVC_IDC,
			vctxt->nodeInfo,
			(xmlSchemaTypePtr) idc,
			"Duplicate key-sequence found", NULL, NULL);
		    
		    goto selector_leave;
		}
	    }
	    /*
	    * Add a node-table item to the IDC binding.
	    */
	    ntItem = (xmlSchemaPSVIIDCNodePtr) xmlMalloc(
		sizeof(xmlSchemaPSVIIDCNode));
	    if (ntItem == NULL) {
		xmlSchemaVErrMemory(NULL, 
		    "allocating an IDC node-table item", NULL);
		xmlFree(*keySeq);
		*keySeq = NULL;
		return(-1);
	    }	
	    memset(ntItem, 0, sizeof(xmlSchemaPSVIIDCNode));		
	    
	    /* 
	    * Store the node-table item on global list.
	    */
	    if (idc->type != XML_SCHEMA_TYPE_IDC_KEYREF) {
		if (xmlSchemaIDCStoreNodeTableItem(vctxt, ntItem) == -1) {
		    xmlFree(ntItem);
		    xmlFree(*keySeq);
		    *keySeq = NULL;
		    return (-1);
		}
	    }
	    /*
	    * Init the node-table item. Consume the key-sequence.
	    */
	    ntItem->node = vctxt->node;
	    ntItem->keys = *keySeq;
	    *keySeq = NULL;
	    if (xmlSchemaIDCAppendNodeTableItem(bind, ntItem) == -1) {		    
		if (idc->type == XML_SCHEMA_TYPE_IDC_KEYREF) {
		    /* 
		    * Free the item, since keyref items won't be
		    * put on a global list.
		    */
		    xmlFree(ntItem->keys);
		    xmlFree(ntItem);
		}
		return (-1);
	    }
	    
	    goto selector_leave;
selector_key_error:
	    /*
	    * 4.2.1 (KEY) The �target node set� and the 
	    * �qualified node set� are equal, that is, every 
	    * member of the �target node set� is also a member
	    * of the �qualified node set� and vice versa.
	    */
	    xmlSchemaStreamVCustomErr(vctxt, 
		XML_SCHEMAV_CVC_IDC,
		vctxt->nodeInfo, 
		(xmlSchemaTypePtr) idc,
		"All 'key' fields must evaluate to a node",
		NULL, NULL);
selector_leave:
	    /*
	    * Free the key-sequence if not added to the IDC table.
	    */
	    if ((keySeq != NULL) && (*keySeq != NULL)) {
		xmlFree(*keySeq);
		*keySeq = NULL;
	    }
	} /* if selector */
	
	sto->nbHistory--;

deregister_check:
	/*
	* Deregister state objects if they reach the depth of creation.
	*/
	if ((sto->nbHistory == 0) && (sto->depth == depth)) {
#if DEBUG_IDC
	    xmlGenericError(xmlGenericErrorContext, "IDC:   STO pop '%s'\n",
		sto->sel->xpath);
#endif
	    if (vctxt->xpathStates != sto) {
		xmlSchemaVErr(vctxt, vctxt->node,
		    XML_SCHEMAV_INTERNAL,
		    "Internal error: xmlSchemaXPathProcessHistory, "
		    "The state object to be removed is not the first "
		    "in the list.\n",
		    NULL, NULL);
	    }
	    nextsto = sto->next;
	    /*
	    * Unlink from the list of active XPath state objects.
	    */
	    vctxt->xpathStates = sto->next;
	    sto->next = vctxt->xpathStatePool;
	    /*
	    * Link it to the pool of reusable state objects.
	    */
	    vctxt->xpathStatePool = sto;	    
	    sto = nextsto;
	} else
	    sto = sto->next;
    } /* while (sto != NULL) */
    return (0);
}

/**
 * xmlSchemaIDCRegisterMatchers:
 * @vctxt: the WXS validation context
 * @elemDecl: the element declaration
 *
 * Creates helper objects to evaluate IDC selectors/fields
 * successively.
 *
 * Returns 0 if OK and -1 on internal errors.
 */
static int
xmlSchemaIDCRegisterMatchers(xmlSchemaValidCtxtPtr vctxt,
			     xmlSchemaElementPtr elemDecl)
{
    xmlSchemaIDCMatcherPtr matcher, last = NULL;
    xmlSchemaIDCPtr idc, refIdc;
    xmlSchemaIDCAugPtr aidc;
        
    idc = (xmlSchemaIDCPtr) elemDecl->idcs;
    if (idc == NULL)
	return (0);
    
#if DEBUG_IDC
    {
	xmlChar *str = NULL;
	xmlGenericError(xmlGenericErrorContext, 
	    "IDC: REGISTER on %s, depth %d\n",
	    (char *) xmlSchemaFormatNsUriLocal(&str, vctxt->nodeInfo->namespaceName,
		vctxt->nodeInfo->localName), vctxt->depth);
	FREE_AND_NULL(str)
    }
#endif
    if (vctxt->nodeInfo->idcMatchers != NULL) {
	xmlSchemaVErr(vctxt, vctxt->node,
	    XML_SCHEMAV_INTERNAL,
	    "Internal error: xmlSchemaIDCRegisterMatchers: "
	    "The chain of IDC matchers is expected to be empty.\n",
	    NULL, NULL);
	return (-1);
    }
    do {
	if (idc->type == XML_SCHEMA_TYPE_IDC_KEYREF) {
	    /*
	    * Since IDCs bubbles are expensive we need to know the
	    * depth at which the bubbles should stop; this will be
	    * the depth of the top-most keyref IDC. If no keyref
	    * references a key/unique IDC, the bubbleDepth will
	    * be -1, indicating that no bubbles are needed.
	    */
	    refIdc = (xmlSchemaIDCPtr) idc->ref->item;
	    if (refIdc != NULL) {
		/*
		* Lookup the augmented IDC.
		*/
		aidc = vctxt->aidcs;
		while (aidc != NULL) {
		    if (aidc->def == refIdc)
			break;
		    aidc = aidc->next;
		}
		if (aidc == NULL) {
		    xmlSchemaVErr(vctxt, vctxt->node,
			XML_SCHEMAV_INTERNAL,
			"Internal error: xmlSchemaIDCRegisterMatchers: "
			"Could not find an augmented IDC item for an IDC "
			"definition.\n",
			NULL, NULL);
		    return (-1);
		}		
		if ((aidc->bubbleDepth == -1) ||
		    (vctxt->depth < aidc->bubbleDepth))
		    aidc->bubbleDepth = vctxt->depth;
	    }
	}
	/*
	* Lookup the augmented IDC item for the IDC definition.
	*/
	aidc = vctxt->aidcs;
	while (aidc != NULL) {
	    if (aidc->def == idc)
		break;
	    aidc = aidc->next;
	}
	if (aidc == NULL) {
	    xmlSchemaVErr(vctxt, vctxt->node,
		XML_SCHEMAV_INTERNAL,
		"Internal error: xmlSchemaIDCRegisterMatchers: "
		"Could not find an augmented IDC item for an IDC definition.\n",
		NULL, NULL);
	    return (-1);
	}
	/*
	* Create an IDC matcher for every IDC definition.
	*/
	matcher = (xmlSchemaIDCMatcherPtr) 
	    xmlMalloc(sizeof(xmlSchemaIDCMatcher));
	if (matcher == NULL) {
	    xmlSchemaVErrMemory(vctxt, 
		"allocating an IDC matcher", NULL);
	    return (-1);
	}
	memset(matcher, 0, sizeof(xmlSchemaIDCMatcher));
	if (last == NULL)
	    vctxt->nodeInfo->idcMatchers = matcher;
	else
	    last->next = matcher;
	last = matcher;

	matcher->type = IDC_MATCHER;
	matcher->depth = vctxt->depth;	
	matcher->aidc = aidc;
#if DEBUG_IDC	
	xmlGenericError(xmlGenericErrorContext, "IDC:   register matcher\n");
#endif 
	/*
	* Init the automaton state object. 
	*/
	if (xmlSchemaIDCAddStateObject(vctxt, matcher, 
	    idc->selector, XPATH_STATE_OBJ_TYPE_IDC_SELECTOR) == -1)
	    return (-1);

	idc = idc->next;
    } while (idc != NULL);
    return (0);
}

/**
 * xmlSchemaBubbleIDCNodeTables: 
 * @depth: the current tree depth
 *
 * Merges IDC bindings of an element at @depth into the corresponding IDC 
 * bindings of its parent element. If a duplicate note-table entry is found, 
 * both, the parent node-table entry and child entry are discarded from the 
 * node-table of the parent.
 *
 * Returns 0 if OK and -1 on internal errors.
 */
static int
xmlSchemaBubbleIDCNodeTables(xmlSchemaValidCtxtPtr vctxt)
{
    xmlSchemaPSVIIDCBindingPtr bind; /* IDC bindings of the current node. */
    xmlSchemaPSVIIDCBindingPtr *parTable, parBind = NULL, lastParBind = NULL; /* parent IDC bindings. */
    xmlSchemaPSVIIDCNodePtr node, parNode = NULL; /* node-table entries. */
    xmlSchemaPSVIIDCKeyPtr key, parKey; /* keys of in a key-sequence. */
    xmlSchemaIDCAugPtr aidc;
    int i, j, k, ret = 0, oldNum, newDupls;
    int duplTop;

    /*
    * The node table has the following sections:
    *
    *  O --> old node-table entries (first)
    *  O 
    *  + --> new node-table entries
    *  + 
    *  % --> new duplicate node-table entries    
    *  % 
    *  # --> old duplicate node-table entries    
    *  # (last)
    *
    */
    bind = vctxt->nodeInfo->idcTable;        
    if (bind == NULL) {
	/* Fine, no table, no bubbles. */
	return (0);
    }
    
    parTable = &(vctxt->elemInfos[vctxt->depth -1]->idcTable);
    /*
    * Walk all bindings; create new or add to existing bindings.
    * Remove duplicate key-sequences.
    */
start_binding:
    while (bind != NULL) {
	/*
	* Skip keyref IDCs.
	*/
	if (bind->definition->type == XML_SCHEMA_TYPE_IDC_KEYREF) {
	    bind = bind->next;
	    continue;
	}
	/*
	* Check if the key/unique IDC table needs to be bubbled.
	*/
	aidc = vctxt->aidcs;
	do {
	    if (aidc->def == bind->definition) {
		if ((aidc->bubbleDepth == -1) || 
		    (aidc->bubbleDepth >= vctxt->depth)) {
		    bind = bind->next;
		    goto start_binding;
		}
		break;
	    }
	    aidc = aidc->next;
	} while (aidc != NULL);

	if (parTable != NULL)
	    parBind = *parTable;
	while (parBind != NULL) {
	    /*
	    * Search a matching parent binding for the
	    * IDC definition.
	    */
	    if (parBind->definition == bind->definition) {

		/*
		* Compare every node-table entry of the child node, 
		* i.e. the key-sequence within, ...
		*/
		oldNum = parBind->nbNodes; /* Skip newly added items. */
		duplTop = oldNum + parBind->nbDupls;
		newDupls = 0;

		for (i = 0; i < bind->nbNodes; i++) {
		    node = bind->nodeTable[i];
		    if (node == NULL)
			continue;
		    /*
		    * ...with every key-sequence of the parent node, already
		    * evaluated to be a duplicate key-sequence.
		    */
		    if (parBind->nbDupls != 0) {
			j = bind->nbNodes + newDupls; 
			while (j < duplTop) {
			    parNode = parBind->nodeTable[j];
			    for (k = 0; k < bind->definition->nbFields; k++) {
				key = node->keys[k];
				parKey = parNode->keys[k];
				ret = xmlSchemaAreValuesEqual(vctxt, key->type, 
				    key->compValue,
				    parKey->type, parKey->compValue);
				if (ret == -1) {
				    /* TODO: Internal error */
				    return(-1);
				} else if (ret == 0)
				    break;

			    }
			    if (ret == 1)
				/* Duplicate found. */
				break;
			    j++;
			}
			if (j != duplTop) {
			    /* Duplicate found. */
			    continue;
			}
		    }		    
		    /*
		    * ... and with every key-sequence of the parent node.
		    */		    		    
		    j = 0;
		    while (j < oldNum) {
			parNode = parBind->nodeTable[j];
			/*
			* Compare key by key. 
			*/
			for (k = 0; k < parBind->definition->nbFields; k++) {
			    key = node->keys[k];
			    parKey = parNode->keys[k];			

			    ret = xmlSchemaAreValuesEqual(vctxt, key->type, 
				key->compValue,
				parKey->type, parKey->compValue);
			    if (ret == -1) {
				/* TODO: Internal error */
			    } else if (ret == 0)
				break;

			}
			if (ret == 1)
			    /*
			    * The key-sequences are equal.
			    */
			    break;
			j++;
		    }
		    if (j != oldNum) {
			/*
			* Handle duplicates.
			*/
			newDupls++;
			oldNum--;
			parBind->nbNodes--;
			/*
			* Move last old item to pos of duplicate.
			*/
			parBind->nodeTable[j] = 
			    parBind->nodeTable[oldNum];
			
			if (parBind->nbNodes != oldNum) {
			    /*
			    * If new items exist, move last new item to 
			    * last of old items.
			    */
			    parBind->nodeTable[oldNum] = 
				parBind->nodeTable[parBind->nbNodes];
			}
			/*
			* Move duplicate to last pos of new/old items.
			*/
			parBind->nodeTable[parBind->nbNodes] = parNode;			
			
		    } else {
			/*
			* Add the node-table entry (node and key-sequence) of 
			* the child node to the node table of the parent node.
			*/
			if (parBind->nodeTable == NULL) {			
			    parBind->nodeTable = (xmlSchemaPSVIIDCNodePtr *) 
				xmlMalloc(10 * sizeof(xmlSchemaPSVIIDCNodePtr));
			    if (parBind->nodeTable == NULL) {
				xmlSchemaVErrMemory(NULL, 
				    "allocating IDC list of node-table items", NULL);
				return(-1);
			    }
			    parBind->sizeNodes = 1;
			} else if (duplTop >= parBind->sizeNodes) {
			    parBind->sizeNodes *= 2;
			    parBind->nodeTable = (xmlSchemaPSVIIDCNodePtr *) 
				xmlRealloc(parBind->nodeTable, parBind->sizeNodes * 
				sizeof(xmlSchemaPSVIIDCNodePtr));
			    if (parBind->nodeTable == NULL) {
				xmlSchemaVErrMemory(NULL, 
				    "re-allocating IDC list of node-table items", NULL);
				return(-1);
			    }
			}
			
			/*
			* Move first old duplicate to last position
			* of old duplicates +1.
			*/
			if (parBind->nbDupls != 0) {
			    parBind->nodeTable[duplTop] =
				parBind->nodeTable[parBind->nbNodes + newDupls];
			}
			/*
			* Move first new duplicate to last position of
			* new duplicates +1.
			*/
			if (newDupls != 0) {
			    parBind->nodeTable[parBind->nbNodes + newDupls] =
				parBind->nodeTable[parBind->nbNodes];
			}
			/*
			* Append the new node-table entry to the 'new node-table
			* entries' section.
			*/
			parBind->nodeTable[parBind->nbNodes] = node;
			parBind->nbNodes++;
			duplTop++;
		    }
		}
		parBind->nbDupls += newDupls;
		break;
	    }
	    if (parBind->next == NULL)
		lastParBind = parBind;
	    parBind = parBind->next;
	}
	if ((parBind == NULL) && (bind->nbNodes != 0)) {
	    /*
	    * No binding for the IDC was found: create a new one and
	    * copy all node-tables.
	    */
	    parBind = xmlSchemaIDCNewBinding(bind->definition);
	    if (parBind == NULL)
		return(-1);

	    parBind->nodeTable = (xmlSchemaPSVIIDCNodePtr *) 
		xmlMalloc(bind->nbNodes * sizeof(xmlSchemaPSVIIDCNodePtr));
	    if (parBind->nodeTable == NULL) {
		xmlSchemaVErrMemory(NULL, 
		    "allocating an array of IDC node-table items", NULL);
		xmlSchemaIDCFreeBinding(parBind);
		return(-1);
	    }
	    parBind->sizeNodes = bind->nbNodes;
	    parBind->nbNodes = bind->nbNodes;
	    memcpy(parBind->nodeTable, bind->nodeTable,
		bind->nbNodes * sizeof(xmlSchemaPSVIIDCNodePtr));
	    if (*parTable == NULL)
		*parTable = parBind;
	    else
		lastParBind->next = parBind;
	}
	bind = bind->next;
    }  
    return (0);
}

/**
 * xmlSchemaCheckCVCIDCKeyRef:
 * @vctxt: the WXS validation context
 * @elemDecl: the element declaration
 *
 * Check the cvc-idc-keyref constraints.
 */
static int
xmlSchemaCheckCVCIDCKeyRef(xmlSchemaValidCtxtPtr vctxt)
{
    xmlSchemaPSVIIDCBindingPtr refbind, bind;

    refbind = vctxt->nodeInfo->idcTable;
    /*
    * Find a keyref.
    */
    while (refbind != NULL) {
	if (refbind->definition->type == XML_SCHEMA_TYPE_IDC_KEYREF) {
	    int i, j, k, res;
	    xmlSchemaPSVIIDCKeyPtr *refKeys, *keys;
	    xmlSchemaPSVIIDCKeyPtr refKey, key;

	    /*
	    * Find the referred key/unique.
	    */
	    bind = vctxt->nodeInfo->idcTable;
	    do {
		if ((xmlSchemaIDCPtr) refbind->definition->ref->item == 
		    bind->definition)
		    break;
		bind = bind->next;
	    } while (bind != NULL);

	    /*
	    * Search for a matching key-sequences.
	    */
	    for (i = 0; i < refbind->nbNodes; i++) {
		res = 0;
		if (bind != NULL) {		    
		    refKeys = refbind->nodeTable[i]->keys;
		    for (j = 0; j < bind->nbNodes; j++) {
			keys = bind->nodeTable[j]->keys;
			for (k = 0; k < bind->definition->nbFields; k++) {
			    refKey = refKeys[k];
			    key = keys[k];
			    res = xmlSchemaAreValuesEqual(vctxt,
				key->type, key->compValue,
				refKey->type, refKey->compValue);
			    if (res == 0)
				break;
			    else if (res == -1) {
				return (-1);
			    }
			}
			if (res == 1) {
			    /*
			    * Match found.
			    */
			    break;
			}
		    }
		}
		if (res == 0) {
		    /* TODO: Report the key-sequence. */
		    xmlSchemaVCustomErr(vctxt,
			XML_SCHEMAV_CVC_IDC,
			refbind->nodeTable[i]->node, 
			(xmlSchemaTypePtr) refbind->definition,
			"No matching key-sequence found", NULL);
		}
	    }
	}
	refbind = refbind->next;
    }
    return (0);
}

/**
 * xmlSchemaBeginElement:
 * @vctxt: the WXS validation context
 *
 * Just a temporary workaround to simulate streaming validation
 * a bit.
 */
static void
xmlSchemaBeginElement(xmlSchemaValidCtxtPtr vctxt)
{
    vctxt->depth++;
    vctxt->nodeInfo = xmlSchemaGetFreshElemInfo(vctxt, vctxt->depth);
    vctxt->nodeInfo->node = vctxt->node;
    vctxt->nodeInfo->localName = vctxt->node->name;
    if (vctxt->node->ns != NULL)
	vctxt->nodeInfo->namespaceName = vctxt->node->ns->href;
    else 
	vctxt->nodeInfo->namespaceName = NULL;    
}

/**
 * xmlSchemaEndElement:
 * @vctxt: the WXS validation context
 *
 * Just a temporary workaround to simulate streaming validation
 * a bit.
 */
static int
xmlSchemaEndElement(xmlSchemaValidCtxtPtr vctxt)
{
    if (vctxt->depth < 0) {
	/* TODO: raise error? */
	vctxt->depth--;
	return (0);
    }
    /*
    * Evaluate the history of changes of active state objects.
    */
    if (xmlSchemaXPathProcessHistory(vctxt, vctxt->depth) == -1)
	return (-1);

    if (vctxt->nodeInfo->value != NULL) {
	xmlSchemaFreeValue(vctxt->nodeInfo->value);
	vctxt->nodeInfo->value = NULL;
    }
    /*
    * TODO: 6 The element information item must be �valid� with respect to each of 
    * the {identity-constraint definitions} as per Identity-constraint 
    * Satisfied (�3.11.4).
    */
    /*
    * Validate IDC keyrefs.
    */
    xmlSchemaCheckCVCIDCKeyRef(vctxt);
    
    /*
    * Merge/free the IDC table.
    */
    if (vctxt->nodeInfo->idcTable != NULL) {
#ifdef DEBUG_IDC
	xmlSchemaDebugDumpIDCTable(stdout,
	    vctxt->nodeInfo->namespaceName,
	    vctxt->nodeInfo->localName,
	    vctxt->nodeInfo->idcTable);
#endif
	if (vctxt->depth > 0) {
	    /*
	    * Merge the IDC node table with the table of the parent node.
	    */
	    xmlSchemaBubbleIDCNodeTables(vctxt);	    
	}
	/*
	* TODO: Don't free the PSVI IDC tables if they are
	* requested for the PSVI.
	*/
	xmlSchemaIDCFreeIDCTable(vctxt->nodeInfo->idcTable);
	vctxt->nodeInfo->idcTable = NULL;
    }

    /*
    * Cleanup IDC matchers.
    */
    if (vctxt->nodeInfo->idcMatchers != NULL) {	
	xmlSchemaIDCFreeMatcherList(vctxt->nodeInfo->idcMatchers);
	vctxt->nodeInfo->idcMatchers = NULL;
    }

    /*
    * Skip further processing if we are on the validation root.
    */
    if (vctxt->depth == 0) {
	vctxt->depth--;
	return (0);
    }

    /*
    * Reset the bubbleDepth if needed.
    */
    if (vctxt->aidcs != NULL) {
	xmlSchemaIDCAugPtr aidc = vctxt->aidcs;
	do {
	    if (aidc->bubbleDepth == vctxt->depth) {
		/*
		* A bubbleDepth of a key/unique IDC matches the current
		* depth, this means that we are leaving the scope of the
		* top-most keyref IDC.
		*/
		aidc->bubbleDepth = -1;
	    }
	    aidc = aidc->next;
	} while (aidc != NULL);
    }
    vctxt->depth--;
    /*
    * Clear the current elemInfo.
    */
    if (vctxt->nodeInfo->value != NULL) {
	xmlSchemaFreeValue(vctxt->nodeInfo->value);
	vctxt->nodeInfo->value = NULL;
    }
    vctxt->nodeInfo = vctxt->elemInfos[vctxt->depth];
    vctxt->node = vctxt->nodeInfo->node;

    return (0);
}

/**
 * xmlSchemaValidateElementByDeclaration:
 * @ctxt:  a schema validation context
 * @node:  the top node.
 *
 * Validate the content of an element type.
 * Validation Rule: Element Locally Valid (Element)
 *
 * Returns 0 if the element is schemas valid, a positive error code
 *     number otherwise and -1 in case of internal or API error.
 */
static int
xmlSchemaValidateElementByDeclaration(xmlSchemaValidCtxtPtr ctxt,
				      xmlSchemaElementPtr elemDecl)
{
    xmlNodePtr elem;
    int ret = 0;
    xmlSchemaTypePtr actualType = NULL;
    xmlAttrPtr attr;
    xmlChar *attrValue; 
    int nilled = 0, elemHasContent = -1;

    /* 
    * This one is called by xmlSchemaValidateElementByWildcardInternal, 
    * xmlSchemaValidateElementByAnyType and xmlSchemaValidateElement.
    * Note that @elemDecl will be the declaration and never the
    * reference to a declaration.
    */

    if (ctxt == NULL) {
        xmlSchemaVErr(ctxt, NULL, XML_SCHEMAV_INTERNAL,
	    "Internal error: xmlSchemaValidateElementByDeclaration, "
	    "bad arguments.\n",
	    NULL, NULL);
        return (-1);
    }

    elem = ctxt->node;   

    /*
    * cvc-elt (3.3.4) : 1
    */
    if (elemDecl == NULL) {
	xmlSchemaVCustomErr(ctxt,
	    XML_SCHEMAV_CVC_ELT_1, 
	    elem, NULL,
	    "No matching declaration available", NULL);
	/* 
	* Evaluate IDCs even if an error occured.
	*/
	if (xmlSchemaXPathEvaluate(ctxt, XML_ELEMENT_NODE) == -1)
	    return (-1);
        return (ctxt->err);
    }
    /*
    * cvc-elt (3.3.4) : 2
    */
    if (elemDecl->flags & XML_SCHEMAS_ELEM_ABSTRACT) {
	xmlSchemaVCustomErr(ctxt,
	    XML_SCHEMAV_CVC_ELT_2,
	    elem, NULL, 
	    "The element declaration is abstract", NULL);
	/* 
	* Evaluate IDCs even if an error occured.
	*/
	if (xmlSchemaXPathEvaluate(ctxt, XML_ELEMENT_NODE) == -1)
	    return (-1);
        return (ctxt->err);
    }
     
    /*
    * cvc-elt (3.3.4) : 3
    * Handle 'xsi:nil'.
    */
    
    attr = xmlHasNsProp(elem, BAD_CAST "nil", xmlSchemaInstanceNs);
    if (attr != NULL) {
	attrValue = xmlNodeGetContent((xmlNodePtr) attr);	
	ctxt->node = (xmlNodePtr) attr;
	ctxt->cur = attr->children;	
	ret = xmlSchemaValidateSimpleTypeValue(ctxt, 
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_BOOLEAN),
	    BAD_CAST attrValue, 1, 1, 1, 1);
	ctxt->node = elem;
	ctxt->type = (xmlSchemaTypePtr) elemDecl;
	if (ret < 0) {
	    xmlSchemaVCustomErr(ctxt,
		XML_SCHEMAV_INTERNAL, 
		(xmlNodePtr) attr, (xmlSchemaTypePtr) elemDecl,
		"Internal error: xmlSchemaValidateElementByDeclaration, "
		"validating the attribute 'xsi:nil'", NULL);
	    if (attrValue != NULL)
		xmlFree(attrValue);
	    return (-1);
	} 
	if ((elemDecl->flags & XML_SCHEMAS_ELEM_NILLABLE) == 0) {
	    /* 
	    * cvc-elt (3.3.4) : 3.1 
	    */
	    xmlSchemaVCustomErr(ctxt, 
		XML_SCHEMAV_CVC_ELT_3_1, 
		elem, NULL,
		"The element is not 'nillable'", NULL);	
	} else {		    
	    if (xmlStrEqual(BAD_CAST attrValue, BAD_CAST "true") ||
		xmlStrEqual(BAD_CAST attrValue, BAD_CAST "1")) {		
		ret = 0;
		/* 
		* cvc-elt (3.3.4) : 3.2.1 
		*/
		elemHasContent = xmlSchemaHasElemOrCharContent(elem);
		if (elemHasContent == 1) {
		    xmlSchemaVCustomErr(ctxt, 
			XML_SCHEMAV_CVC_ELT_3_2_1, 
			/* XML_SCHEMAS_ERR_NOTEMPTY, */
			elem, (xmlSchemaTypePtr) elemDecl,
			"The 'nilled' element must have no character or "
			"element content", NULL);
		    ret = XML_SCHEMAV_CVC_ELT_3_2_1;
		}
		/* 
		* cvc-elt (3.3.4) : 3.2.2 
		*/
		if ((elemDecl->flags & XML_SCHEMAS_ELEM_FIXED) &&
		    (elemDecl->value != NULL)) {
		    xmlSchemaVCustomErr(ctxt, XML_SCHEMAV_CVC_ELT_3_2_2, 
			/* XML_SCHEMAS_ERR_HAVEDEFAULT, */
			elem, (xmlSchemaTypePtr) elemDecl,
			"There is a fixed value constraint defined for "
			"the 'nilled' element", NULL);		    
		    ret = XML_SCHEMAV_CVC_ELT_3_2_2;
		}
		if (ret == 0)
		    nilled = 1;		
	    }
	}
	if (attrValue != NULL)
	    xmlFree(attrValue);
    }
    

    actualType = elemDecl->subtypes;
    /* 
    * cvc-elt (3.3.4) : 4 
    * Handle 'xsi:type'.
    */
    
    attr = xmlHasNsProp(elem, BAD_CAST "type",  xmlSchemaInstanceNs);
    if (attr != NULL) {	
	xmlChar *nsName = NULL, *local = NULL;
	
	/*
	* TODO: We should report a *warning* that the type was overriden
	* by the instance.
	*/
	
	/* 
	* cvc-elt (3.3.4) : 4.1 
	*/
	attrValue = xmlNodeGetContent((xmlNodePtr) attr);
	ret = xmlSchemaValQNameAcquire(attrValue, attr->parent,	
	    &nsName, &local);
	if (ret < 0) {
	    xmlSchemaVCustomErr(ctxt,
		XML_SCHEMAV_INTERNAL, 
		(xmlNodePtr) attr, (xmlSchemaTypePtr) elemDecl,
		"Internal error: xmlSchemaValidateElementByDeclaration, "
		"validating the attribute 'xsi:type'", NULL);;
	    FREE_AND_NULL(attrValue)
		FREE_AND_NULL(nsName)
		FREE_AND_NULL(local)
		return (-1);
	} else if (ret == 1) {
	    xmlSchemaVSimpleTypeErr(ctxt,
		XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_1,
		(xmlNodePtr) attr, attrValue, 
		xmlSchemaGetBuiltInType(XML_SCHEMAS_QNAME));
	} else if (ret == 2) {
	    xmlSchemaVCustomErr(ctxt,
		XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_1,
		(xmlNodePtr) attr, 
		xmlSchemaGetBuiltInType(XML_SCHEMAS_QNAME),
		"The QName value '%s' has no "
		"corresponding namespace declaration in scope", 
		attrValue);	    	    
	} else {
	    /*
	    * cvc-elt (3.3.4) : 4.2 
	    */
	    actualType = xmlSchemaGetType(ctxt->schema, local, nsName);
	    if (actualType == NULL) {	  
		xmlChar *strA = NULL;
		
		xmlSchemaVCustomErr(ctxt,
		    XML_SCHEMAV_CVC_ELT_4_2,
		    (xmlNodePtr) attr, 
		    xmlSchemaGetBuiltInType(XML_SCHEMAS_QNAME),
		    "The value %s does not resolve to a type "
		    "definition", 
		    xmlSchemaFormatNsUriLocal(&strA, nsName, local));
		FREE_AND_NULL(strA);    
	    } else {		
		/*
		* URGENT TODO: cvc-elt (3.3.4) : 4.3 (Type Derivation OK)
		*/		
	    }
	}
	FREE_AND_NULL(attrValue)
	FREE_AND_NULL(nsName)
	FREE_AND_NULL(local)
    }		
    /* TODO: Change the handling of missing types according to
    * the spec.
    */
    if (actualType == NULL) {
    	xmlSchemaVCustomErr(ctxt, 
    	    XML_SCHEMAV_CVC_TYPE_1,
    	    elem, (xmlSchemaTypePtr) elemDecl, 
    	    "The type definition is absent", NULL);
	/* 
	* Evaluate IDCs even if an error occured.
	*/
	if (xmlSchemaXPathEvaluate(ctxt, XML_ELEMENT_NODE) == -1)
	    return (-1);
    	return (XML_SCHEMAV_CVC_TYPE_1);
    }
    
    /*
    * Remember the actual-type definition.
    */
    ctxt->nodeInfo->typeDef = actualType;
    
    /*
    * TODO: Since this should be already checked by the content model automaton,
    * and we want to get rid of the XML_SCHEMAS_ERR... types, the error code
    * has been changed to XML_SCHEMAV_INTERNAL.
    */
    /*
    if (child == NULL) {
        if (decl->minOccurs > 0) {
            xmlSchemaVErr(ctxt, node, XML_SCHEMAV_INTERNAL,
		 XML_SCHEMAS_ERR_MISSING, 
		"Element %s: missing child %s\n",
		node->name, decl->name);
        }
        return (ctxt->err);
    } 
    */
    /*
     * Verify the element matches
     * TODO, FIXME: Can this still happen here? Isn't this already checked
     * by the content model automaton?         
    if (!xmlStrEqual(child->name, decl->name)) {
        xmlSchemaVErr3(ctxt, node, XML_SCHEMAV_INTERNAL,
	     XML_SCHEMAS_ERR_WRONGELEM, 
	    "Element %s: missing child %s found %s\n",
	    node->name, decl->name, child->name);
        return (ctxt->err);
    }
    */    
    if (elemHasContent == -1)
	elemHasContent = xmlSchemaHasElemOrCharContent(elem);

    /*
    * IDC: Register identity-constraint XPath matchers.
    */
    if (elemDecl->idcs != NULL)
	xmlSchemaIDCRegisterMatchers(ctxt, elemDecl);
    /* 
    * Evaluate IDCs.
    */
    if (xmlSchemaXPathEvaluate(ctxt, XML_ELEMENT_NODE) == -1)
	return (-1);
    /*
    * cvc-elt (3.3.4) : 5 
    * The appropriate case among the following must be true:
    */
    /*
    * cvc-elt (3.3.4) : 5.1 
    * If the declaration has a {value constraint}, 
    * the item has neither element nor character [children] and 
    * clause 3.2 has not applied, then all of the following must be true:
    */
    if ((elemHasContent == 0) && (nilled == 0) && (elemDecl->value != NULL)) {
	/*
	* cvc-elt (3.3.4) : 5.1.1 
	* If the �actual type definition� is a �local type definition�
	* then the canonical lexical representation of the {value constraint}
	* value must be a valid default for the �actual type definition� as 
	* defined in Element Default Valid (Immediate) (�3.3.6). 
	*/
	/* 
	* NOTE: 'local' above means types aquired by xsi:type.
	*/
	ret = 0;
	if (actualType != elemDecl->subtypes) {
	    xmlSchemaCreatePCtxtOnVCtxt(ctxt);
	    ret = xmlSchemaCheckCOSValidDefault(ctxt->pctxt, ctxt, actualType, 
		elemDecl->value, NULL);	    
	    if (ret < 0) {
		xmlSchemaVCustomErr(ctxt, 
		    XML_SCHEMAV_INTERNAL, 
		    elem, actualType,
		    "Internal error: xmlSchemaValidateElementByDeclaration, "
		    "validating a default value", NULL);
		return (-1);
	    }
	}
	/*
	* cvc-elt (3.3.4) : 5.1.2 
	* The element information item with the canonical lexical 
	* representation of the {value constraint} value used as its 
	* �normalized value� must be �valid� with respect to the 
	* �actual type definition� as defined by Element Locally Valid (Type)
	* (�3.3.4).
	*/
	/*
        * Disable validation of the simple content, if it was already
	* done above.
	*/
	if (ret == 0) {
	    if (actualType != elemDecl->subtypes)
		ret = xmlSchemaValidateElementByType(ctxt, actualType, 0, 0);
	    else
		ret = xmlSchemaValidateElementByType(ctxt, actualType, 0, 1);
	    ctxt->node = elem;
	    if (ret < 0) {
		xmlSchemaVCustomErr(ctxt, 
		    XML_SCHEMAV_INTERNAL, 
		    elem, actualType,
		    "Internal error: xmlSchemaValidateElementByDeclaration, "
		    "validating against the type", NULL);
		return (-1);
	    }
	    /*
	    * PSVI: Create a text node on the instance element.
	    */
	    if (ctxt->options & XML_SCHEMA_VAL_VC_I_CREATE) {
		xmlNodePtr textChild;
		
		textChild = xmlNewText(elemDecl->value);
		if (textChild == NULL) {
		    xmlSchemaVCustomErr(ctxt, 
			XML_SCHEMAV_INTERNAL, 
			elem, actualType,
			"Internal error: xmlSchemaValidateElementByDeclaration, "
			"could not create a default text node for the instance", 
			NULL);
		} else
		    xmlAddChild(elem, textChild);	    
	    }
	}

    } else {	
	/*
	* 5.2.1 The element information item must be �valid� with respect 
	* to the �actual type definition� as defined by Element Locally 
	* Valid (Type) (�3.3.4).
	*/
	ret = xmlSchemaValidateElementByType(ctxt, actualType, nilled, 1);
	/*
	* Consume the computed value for IDCs, ect. Note that default
	* values are not supported yet.
	*/
	if (ctxt->value != NULL) {
	    ctxt->nodeInfo->value = ctxt->value;
	    ctxt->value = NULL;
	}
	ctxt->node = elem;
	if (ret < 0) {
	    xmlSchemaVCustomErr(ctxt, 
		XML_SCHEMAV_INTERNAL, 
		elem, actualType,
		"Internal error: xmlSchemaValidateElementByDeclaration, "
		"calling validation by type", NULL);
	    return (-1);
	}
	/*
	* 5.2.2 If there is a fixed {value constraint} and clause 3.2 has 
	* not applied, all of the following must be true:
	*/
	if ((elemDecl->flags & XML_SCHEMAS_ELEM_FIXED) && (nilled == 0)) {
	    /*
	    * 5.2.2.1 The element information item must have no element 
	    * information item [children].
	    *
	    * TODO REDUNDANT: If the actual type exists, the above call  to 
	    * xmlSchemaValidateElementByType will already check for element 
	    * nodes.
	    */
	    if (xmlSchemaHasElemContent(elem)) {
		xmlSchemaVCustomErr(ctxt, 
		    XML_SCHEMAV_CVC_ELT_5_2_2_1, 
		    elem, (xmlSchemaTypePtr) elemDecl,
		    "Elements in the content are not allowed if it is "
		    "constrained by a fixed value", NULL);
	    } else {
		/*
		* 5.2.2.2 The appropriate case among the following must 
		* be true:
		*/
		
		if (actualType->contentType == XML_SCHEMA_CONTENT_MIXED) {
		    xmlChar *value;
		    /*
		    * 5.2.2.2.1 If the {content type} of the �actual type 
		    * definition� is mixed, then the *initial value* of the 
		    * item must match the canonical lexical representation 
		    * of the {value constraint} value.
		    *
		    * ... the *initial value* of an element information 
		    * item is the string composed of, in order, the 
		    * [character code] of each character information item in 
		    * the [children] of that element information item.
		    */
		    value = xmlNodeListGetString(elem->doc, elem->children, 1);
		    if (! xmlStrEqual(BAD_CAST value, elemDecl->value)) {
			/* 
			* TODO: Report invalid & expected values as well.
			* TODO: Implement the canonical stuff.
			*/
			xmlSchemaVCustomErr(ctxt, 
			    XML_SCHEMAV_CVC_ELT_5_2_2_2_1, 
			    elem, (xmlSchemaTypePtr) elemDecl,
			    "The value does not match the cononical "
			    "lexical representation of the fixed constraint", 
			    NULL);
		    }
		    if (value != NULL)
			xmlFree(value);
		} else if ((actualType->contentType == 
		    XML_SCHEMA_CONTENT_SIMPLE) || 
		    (actualType->contentType == XML_SCHEMA_CONTENT_BASIC)) {
		    xmlChar *value;

		    /*
		    * 5.2.2.2.2 If the {content type} of the �actual type 
		    * definition� is a simple type definition, then the 
		    * *actual value* of the item must match the canonical 
		    * lexical representation of the {value constraint} value.
		    */
		    /*
		    * TODO: *actual value* is the normalized value, impl. this.
		    * TODO: Report invalid & expected values as well.
		    * TODO: Implement the canonical stuff.
		    * 
		    */
		    value = xmlNodeListGetString(elem->doc, elem->children, 1);
		    if (! xmlStrEqual(BAD_CAST value, elemDecl->value)) {
			xmlSchemaVCustomErr(ctxt, 
			    XML_SCHEMAV_CVC_ELT_5_2_2_2_2, 
			    elem, (xmlSchemaTypePtr) elemDecl,
			    "The normalized value does not match the canonical "
			    "lexical representation of the fixed constraint", 
			    NULL);
		    }
		    if (value != NULL)
			xmlFree(value);
		    
		}
		/*
		* TODO: What if the content type is not 'mixed' or simple?
		*/

	    }
	    
	}
    }
    /*
    * TODO: 7 If the element information item is the �validation root�, it must be 
    * �valid� per Validation Root Valid (ID/IDREF) (�3.3.4).
    */
               
    return (ctxt->err);
}

/**
 * xmlSchemaValidateElementByWildcardInternal:
 * @ctxt:  a schema validation context
 * @node:  the top node.
 *
 * Represents the recursive portion of xmlSchemaValidateElementByWildcard. 
 * Not intended to be used by other functions.
 *
 * Returns 0 if the element is valid, a positive error code
 * number otherwise and -1 in case of an internal error.
 */
static int
xmlSchemaValidateElementByWildcardInternal(xmlSchemaValidCtxtPtr ctxt,
					   xmlSchemaWildcardPtr wild, 
					   xmlNodePtr node)
{        
    const xmlChar *uri;
    int ret = 0;
    xmlNodePtr child;

    if (ctxt->xsiAssemble) {	
	ret = xmlSchemaAssembleByXSIElem(ctxt, ctxt->node);
	if (ret == -1) {
	    xmlSchemaVCustomErr(ctxt, 
		XML_SCHEMAV_INTERNAL,
		ctxt->node, NULL, 	
		"Internal error: xmlSchemaValidateElement, "
		"assembling schema by xsi", NULL);
	    return (-1);
	}
	/*
	* NOTE: We won't react on schema parser errors here.
	* TODO: But a warning would be nice.
	*/
    }    
    if (wild->processContents != XML_SCHEMAS_ANY_SKIP) {
	xmlSchemaElementPtr decl = NULL;

	if (node->ns != NULL)
	    decl = xmlHashLookup3(ctxt->schema->elemDecl,
	    node->name, node->ns->href, NULL);
	else 
	    decl = xmlHashLookup3(ctxt->schema->elemDecl, node->name,
	    NULL, NULL);
	if (decl != NULL) {		    
	    ctxt->node = node;	
	    ret = xmlSchemaValidateElementByDeclaration(ctxt, decl);
	    if (ret < 0) {		
		xmlSchemaVErr(ctxt, node, XML_SCHEMAV_INTERNAL,
		    "Internal error: xmlSchemaValidateAnyInternal, "
		    "validating an element in the context of a wildcard.",
		    NULL, NULL);
	    }
	    return (ret);
	} else if (wild->processContents == XML_SCHEMAS_ANY_STRICT) {
	    /* TODO: Change to proper error code. */
	    xmlSchemaVWildcardErr(ctxt, XML_SCHEMAV_CVC_ELT_1,
		node, wild, "No matching global declaration available");
	    /* 
	    * Evaluate IDCs even if a validation error occured.
	    */
	    if (xmlSchemaXPathEvaluate(ctxt,XML_ELEMENT_NODE) == -1)
		return(-1);

	    return (ctxt->err);
	}
	/* 
	* Evaluate IDCs; we need to know if an IDC field resolves to
	* such a node. This node has no type definition and will
	* definitely result in an IDC validation error if an IDC field
	* resolves.
	*/
	if (xmlSchemaXPathEvaluate(ctxt, XML_ELEMENT_NODE) == -1)
	    return(-1);
    }
    if (node->children != NULL) {	   
	child = node->children;
	do {
	    if (child->type == XML_ELEMENT_NODE) {
		if (child->ns != NULL)
		    uri = child->ns->href;
		else
		    uri = NULL;
		if (xmlSchemaMatchesWildcardNs(wild, uri) == 0) {
		    /* TODO: error code. */
		    xmlSchemaVWildcardErr(ctxt, XML_SCHEMAV_ELEMENT_CONTENT,
			child, wild, 
			"The namespace of the element is not allowed");
		    return (ctxt->err);  
		}

		ctxt->node = child;
		xmlSchemaBeginElement(ctxt);

		/*
		* Recurse over the children.
		*/
		ret = xmlSchemaValidateElementByWildcardInternal(ctxt, 
		    wild, child);
		if (ret == -1)
		    return (-1);

		if (xmlSchemaEndElement(ctxt) == -1)
		    return (-1);

		if (ret != 0)
		    return (ret);		
	    }
	    child = child->next;
	} while  (child != NULL);
    }
    return (0);
}

/**
 * xmlSchemaValidateElementContByWildcard:
 * @ctxt:  a schema validation context
 *
 * Returns 0 if the element is valid, a positive error code
 * number otherwise and -1 in case of an internal or API error.
 */
static int
xmlSchemaValidateElementByWildcard(xmlSchemaValidCtxtPtr ctxt, 
				   xmlSchemaWildcardPtr wild)
{       
    
    if ((wild == NULL) || (wild->type != XML_SCHEMA_TYPE_ANY) ||
	(ctxt->node == NULL)) {
	xmlSchemaVCustomErr(ctxt,
	    XML_SCHEMAV_INTERNAL, ctxt->node, NULL,
	    "Internal error: xmlSchemaValidateElementByWildcard, "
	    "bad arguments", NULL);
	return (-1);
    }
    if (wild->negNsSet != NULL) {
	/*
	* Workaround for negated namespaces.
	*/
	if (ctxt->node->ns != NULL) {
	    if (xmlSchemaMatchesWildcardNs(wild, ctxt->node->ns->href) == 0) {
		ctxt->flags |= XML_SCHEMA_VALID_INVALID_NEG_WILDCARD;
		return (XML_SCHEMAV_ELEMENT_CONTENT);
	    }
	} else if (xmlSchemaMatchesWildcardNs(wild, NULL) == 0) {
	    ctxt->flags |= XML_SCHEMA_VALID_INVALID_NEG_WILDCARD;
	    return (XML_SCHEMAV_ELEMENT_CONTENT);
	}
    }
    return(xmlSchemaValidateElementByWildcardInternal(ctxt, 
	    wild, ctxt->node));
}

/**
 * xmlSchemaValidateElementByAnyType:
 * @ctxt:  a schema validation context
 * @node: the current element
 *
 * This one validates the content of an element of the type
 * 'anyType'. The process contents of the wildcard of 'anyType' is "lax", 
 * thus elements in the subtree will be validated, if a corresponding
 * declaration in the schema exists.
 *
 * Returns 0 if the element and its subtree is valid, a positive error code
 * otherwise and -1 in case of an internal or API error.
 */
static int
xmlSchemaValidateElementByAnyType(xmlSchemaValidCtxtPtr ctxt, 
				  xmlSchemaTypePtr type)
{
    xmlSchemaTypePtr oldtype;
    xmlNodePtr top, cur;
    xmlSchemaElementPtr decl;
    int skipContent, ret, insub = 0;

    if ((type == NULL) || (ctxt->node == NULL))
	return (-1);

    if (ctxt->node->children == NULL) 
	return (0);

    oldtype = ctxt->type;
    top = ctxt->node;        
    /*
    * STREAM: Child nodes are processed.
    */
    cur = ctxt->node->children;
    while (cur != NULL) {
	skipContent = 0;
	if (cur->type == XML_ELEMENT_NODE) {
	    /*
	    * The process contents of the wildcard is "lax", thus
	    * we need to validate the element if a declaration
	    * exists.
	    */		
	    if (cur->ns != NULL)
		decl = xmlHashLookup3(ctxt->schema->elemDecl,
		    cur->name, cur->ns->href, NULL);
	    else 
		decl = xmlHashLookup3(ctxt->schema->elemDecl, cur->name, NULL, NULL);
	    ctxt->node = cur;

	    if (insub) { 
		/*
		* BEGIN element.
		*/
		xmlSchemaBeginElement(ctxt);
	    } else
		insub = 1;

	    if (decl != NULL) {		
		ret = xmlSchemaValidateElementByDeclaration(ctxt, decl);
		ctxt->node = top;
		if (ret < 0) {		
		    xmlSchemaVErr(ctxt, cur, XML_SCHEMAV_INTERNAL,
			"Internal error: xmlSchemaValidateAnyTypeContent, "
			"validating an element in the context of a wildcard.",
			NULL, NULL);
		    return (ret);
		} else if (ret > 0)
		    return (ret);
		skipContent = 1;
	    } else {
		/*
		* IDCs.
		*/
		if (xmlSchemaXPathEvaluate(ctxt,XML_ELEMENT_NODE) == -1)
		    return(-1);
	    }
	}   
	/*
	* Browse the full subtree, deep first.
	*/
        if ((skipContent == 0) && (cur->children != NULL)) {
	    /* deep first */
	    cur = cur->children;
	} else if ((cur != top) && (cur->next != NULL)) {
	    /* then siblings */
	    cur = cur->next;
	} else if (cur != top) {	    
	    /* go up to parents->next if needed */
	    while (cur != top) {
		if (cur->type == XML_ELEMENT_NODE) {
		    /*
		    * END element.
		    */
		    xmlSchemaEndElement(ctxt);
		}
		if (cur->parent != NULL)
		    cur = cur->parent;
		if ((cur != top) && (cur->next != NULL)) {
		    cur = cur->next;
		    break;
		}
		if (cur->parent == NULL) {
		    cur = NULL;
		    break;
		}
	    }
	    /* exit condition */
	    if (cur == top) 
	        cur = NULL;
	} else
	    break;
    }
    ctxt->type = oldtype;
    return (0);
}

/**
 * xmlSchemaValidateElementByComplexType:
 * @ctxt:  a schema validation context
 * @node:  the top node.
 *
 * Validate the content of an element expected to be a complex type type
 * xmlschema-1.html#cvc-complex-type
 * Validation Rule: Element Locally Valid (Complex Type)
 *
 * Returns 0 if the element is schemas valid, a positive error code
 * number otherwise and -1 in case of internal or API error.
 * Note on reported errors: Although it might be nice to report
 * the name of the simple/complex type, used to validate the content
 * of a node, it is quite unnecessary: for global defined types
 * the local name of the element is equal to the NCName of the type,
 * for local defined types it makes no sense to output the internal
 * computed name of the type. TODO: Instead, one should attach the 
 * struct of the type involved to the error handler - this allows
 * the report of any additional information by the user.
 */
static int
xmlSchemaValidateElementByComplexType(xmlSchemaValidCtxtPtr ctxt, 
				      xmlSchemaTypePtr type,
				      int valSimpleContent)
{
    xmlSchemaTypePtr oldtype;    
    xmlNodePtr elem, child;
    int ret = 0;
    const xmlChar *nsUri;    
    xmlSchemaAttrStatePtr attrs = NULL, attrTop = NULL;

    if ((ctxt == NULL) || (type->type != XML_SCHEMA_TYPE_COMPLEX))
	return (-1);

    oldtype = ctxt->type;
    ctxt->type = type;
    elem = ctxt->node;

    /*
    * Verify the attributes
    */
    /*
    * TODO: This "attrTop" thing is not needed any more.
    */  
    /* NOTE: removed, since a check for abstract is
    * done in the cvc-type constraint.
    *
    *
    * if (type->flags & XML_SCHEMAS_TYPE_ABSTRACT) {
    *	xmlSchemaVComplexTypeErr(ctxt, 
    *	    XML_SCHEMAV_CVC_COMPLEX_TYPE_1,
    *	    elem, type, 
    *	    "The type definition is abstract");
    *	return (XML_SCHEMAV_CVC_COMPLEX_TYPE_1);
    *}
    */
    
    attrs = ctxt->attr;    
    attrTop = ctxt->attrTop;   
    /*
    * STREAM: Attribute nodes are processed.
    */
    xmlSchemaRegisterAttributes(ctxt, elem->properties);     
    xmlSchemaValidateAttributes(ctxt, elem, type);
    if (ctxt->attr != NULL)
	xmlSchemaFreeAttributeStates(ctxt->attr);
    ctxt->attr = attrs;    
    ctxt->attrTop = attrTop;    

    /*
    * TODO: This one creates a regexp even if no content
    * model was defined. Somehow ->contModel is always not NULL
    * for complex types, even if they are empty.
    * TODO: Check if the obove still occurs.
    */              
    switch (type->contentType) {
	case XML_SCHEMA_CONTENT_EMPTY: {
		/*
		* 1 If the {content type} is empty, then the element information 
		* item has no character or element information item [children].
		*/
		/*
		* TODO: Is the entity stuff correct?
		*/
		if (xmlSchemaHasElemOrCharContent(elem) == 1) {	    	    
		    xmlSchemaVComplexTypeErr(ctxt, 
			XML_SCHEMAV_CVC_COMPLEX_TYPE_2_1,
			elem, type, 
			"Character or element content is not allowed, "
			"because the content type is empty");
		}	 
		break;
	    }	    
	case XML_SCHEMA_CONTENT_MIXED:
	    /*
	    * Some speedups for anyType or types derived directly from it.
	    */
	    if (IS_ANYTYPE(type)) {
		/*
		* Corresponds to <element name="foo" [type="xsd:anyType"]/>.
		*/
		ret = xmlSchemaValidateElementByAnyType(ctxt, type);
		/* TODO: Handle -1. */
		break;
	    }
	    if (IS_ANYTYPE(type->baseType) &&
		(type->flags & XML_SCHEMAS_TYPE_DERIVATION_METHOD_EXTENSION) &&
		(type->subtypes == type->baseType->subtypes)) {
		/*
		* Corresponds to an <extension> of anyType.
		*/
		ret = xmlSchemaValidateElementByAnyType(ctxt, type->baseType);
		/* TODO: Handle -1. */
		break;
	    }
	    /* No break on purpose. */
        case XML_SCHEMA_CONTENT_ELEMENTS: {
	    xmlRegExecCtxtPtr oldregexp = NULL;
	    xmlChar *values[10];
	    int terminal, nbval = 10, nbneg;
	    /*
	    * SPEC (2.4) If the {content type} is element-only or mixed,
	    * then the sequence of the element information item's
	    * element information item [children], if any, taken in 
	    * order, is �valid� with respect to the {content type}'s 
	    * particle, as defined in Element Sequence Locally Valid 
	    * (Particle) (�3.9.4).
	    */
	    /*
	    * Content model check initialization.
	    */
	    if (type->contModel != NULL) {					
		oldregexp = ctxt->regexp;
		ctxt->regexp = xmlRegNewExecCtxt(type->contModel,
		    (xmlRegExecCallbacks)
		    xmlSchemaValidateCallback, ctxt);
#ifdef DEBUG_AUTOMATA
		xmlGenericError(xmlGenericErrorContext, "====> %s\n", elem->name);
#endif
	    }
	    /*
	    * STREAM: Children are processed.
	    */
	    child = elem->children;
	    while (child != NULL) {		
		if (child->type == XML_ELEMENT_NODE) {
		    if (child->ns != NULL)
			nsUri = child->ns->href;
		    else
			nsUri = NULL;
		    ret = xmlRegExecPushString2(ctxt->regexp,
			child->name, nsUri, child);		    
		    if (ctxt->err == XML_SCHEMAV_INTERNAL)
			return (-1);
		    if (ctxt->flags & XML_SCHEMA_VALID_INVALID_NEG_WILDCARD) {
			ctxt->flags ^= XML_SCHEMA_VALID_INVALID_NEG_WILDCARD;
			ret = -1;
		    }
			/*
			* URGENT TODO: Could we anchor an error report
			* here to notify of invalid elements?
			* TODO: Perhaps it would be better to report 
			* only the first erroneous element and then break.
		    */
#ifdef DEBUG_AUTOMATA
		    if (ret < 0)
			xmlGenericError(xmlGenericErrorContext,
			"  --> %s Error\n", child->name);
		    else
			xmlGenericError(xmlGenericErrorContext,
			"  --> %s\n", child->name);
#endif
		    if (ret < 0) {
			xmlRegExecErrInfo(ctxt->regexp, NULL, &nbval, &nbneg,
			    &values[0], &terminal);
			xmlSchemaVComplexTypeElemErr(ctxt, 
			    XML_SCHEMAV_ELEMENT_CONTENT,
			    child, NULL/* type */, 
			    "This element is not expected",
			    nbval, nbneg, values);
			ret = 1;
			/*
			* Note that this will skip further validation of the
			* content.
			*/
			break;
		    }
		} else if ((type->contentType == XML_SCHEMA_CONTENT_ELEMENTS) && 
		    /* 
		    * TODO: Ask Daniel if this are all character nodes.
		    */
		    (((child->type == XML_TEXT_NODE) && (!IS_BLANK_NODE(child))) ||
		     (child->type == XML_ENTITY_NODE) ||		    		    
		     (child->type == XML_ENTITY_REF_NODE) ||		    
		     (child->type == XML_CDATA_SECTION_NODE))) {		    
		    /* 
		    * 2.3 If the {content type} is element-only, then the 
		    * element information item has no character information 
		    * item [children] other than those whose [character 
		    * code] is defined as a white space in [XML 1.0 (Second 
		    * Edition)].
		    */			
		    xmlSchemaVComplexTypeErr(ctxt, 
			XML_SCHEMAV_CVC_COMPLEX_TYPE_2_3,
			elem, type, 
			"Character content is not allowed, "
			"because the content type is element-only");
		    ret = 1;
		    break;
		}
		child = child->next;		    
	    }    
	    /*
	    * Content model check finalization.
	    */
       	    if (type->contModel != NULL) {
		if (ret == 0) {
		    xmlRegExecNextValues(ctxt->regexp, &nbval, &nbneg,
			&values[0], &terminal);		    
		    /*
		    * If a next value still exists, It does not have to
		    * mean that there's an element missing, since it
		    * might be an optional element. So double check it.
		    */
		    ret = xmlRegExecPushString(ctxt->regexp,
			NULL, NULL);
		    if (ret <= 0) {
			ret = 1;
			xmlSchemaVComplexTypeElemErr(ctxt,
			    XML_SCHEMAV_ELEMENT_CONTENT,
			    elem, type, "Missing child element(s)",
			    nbval, nbneg, values);			    
		    } else
			ret = 0;			
#ifdef DEBUG_AUTOMATA
		    xmlGenericError(xmlGenericErrorContext,
			"====> %s : %d\n", elem->name, ret);
#endif
#ifdef DEBUG_CONTENT
		    if (ret == 0)
			xmlGenericError(xmlGenericErrorContext,
			"Element %s content check succeeded\n",
			elem->name);
#endif
		}
		xmlRegFreeExecCtxt(ctxt->regexp);
		ctxt->regexp = oldregexp;
	    }
	}
            break;
	case XML_SCHEMA_CONTENT_SIMPLE:
        case XML_SCHEMA_CONTENT_BASIC:
	    /*
	    * If the simple content was already validated 
	    * (e.g. a default value), the content need not
	    * to be validated again.
	    */
	if (valSimpleContent == 1) {
	    xmlChar *value = NULL;
	    /*
	    * We hit a complexType with a simpleContent resolving
	    * to a user derived or built-in simple type.
	    */
	    /* 
	    * 2.2 If the {content type} is a simple type definition, 
	    * then the element information item has no element 
	    * information item [children], and the �normalized value� 
	    * of the element information item is �valid� with respect 
	    * to that simple type definition as defined by String 
	    * Valid (�3.14.4).
	    */	  
	    /*
	    * STREAM: Children are processed.
	    */
	    child = elem->children;
	    while (child != NULL) {
		/*
		* TODO: Could the entity stuff produce elements
		* as well?
		*/
                if (child->type == XML_ELEMENT_NODE) {
		    xmlSchemaVComplexTypeErr(ctxt,
			XML_SCHEMAV_CVC_COMPLEX_TYPE_2_2,
			elem, type, 
			"Element content is not allowed, because "
			"the content type is a simple type");
		    ret = XML_SCHEMAV_CVC_COMPLEX_TYPE_2_2;
		    break;
		}
		child = child->next;		    
	    }	
	    ctxt->node = elem;
	    ctxt->cur = elem->children;
	    if (ret != 0) {
		FREE_AND_NULL(value)
		break;
	    }
	    /*
	    * Validate the character content against a simple type.
	    */		
	    if (elem->children == NULL)
		value = NULL;
	    else
		value = xmlNodeGetContent(elem); 
		/*
		* NOTE: This call won't check the correct types of the
		* content nodes, since this should be done here.
		*/
	    ret = xmlSchemaValidateSimpleTypeValue(ctxt,
		type->contentTypeDef, value, 1, 1, 1, 0);
	    if (ret > 0) {	
		/*
		* NOTE: Although an error will be reported by 
		* xmlSchemaValidateSimpleTypeValue, the spec wants
		* a specific complex type error to be reported 
		* additionally.
		*/
		xmlSchemaVComplexTypeErr(ctxt, 
		    XML_SCHEMAV_CVC_COMPLEX_TYPE_2_2,
		    elem, type,  
		    "The character content is not valid");
		ret = XML_SCHEMAV_CVC_COMPLEX_TYPE_2_2;
	    } else if (ret < 0) {
		xmlSchemaVErr(ctxt, elem, XML_SCHEMAV_INTERNAL,
		    "Internal error: xmlSchemaValidateComplexType, "
		    "Element '%s': Error while validating character "
		    "content against complex type '%s'.\n",
		    elem->name, type->name);
		if (value != NULL)
		    xmlFree(value); 
		ctxt->type = oldtype;
		return (-1);
	    }
	    if (value != NULL)
		xmlFree(value);

	}
	    break;
        default:
            TODO xmlGenericError(xmlGenericErrorContext,
                                 "unimplemented content type %d\n",
                                 type->contentType);
    }
    ctxt->type = oldtype;
    return (ctxt->err);
}

/**
 * xmlSchemaValidateElementByType:
 * @ctxt:  a schema validation context
 * @elem:  an element
 * @type:  the list of type declarations
 *
 * Validation Rule: Element Locally Valid (Type).
 *
 * Returns 0 if the element is schemas valid, a positive error code
 *     number otherwise and -1 in case of internal or API error.
 */
static int
xmlSchemaValidateElementByType(xmlSchemaValidCtxtPtr ctxt,
			       xmlSchemaTypePtr type,
			       int isNil,
			       int valSimpleContent)
{
    int ret;

   
    if ((ctxt == NULL) || (type == NULL)) {
        xmlSchemaVCustomErr(ctxt, XML_SCHEMAV_INTERNAL, NULL, NULL,
	    "Internal error: xmlSchemaValidateElementByType, "
	    "bad arguments", NULL);
        return (-1);    
    }    	
    /* 
    * This one is called by "xmlSchemaValidateElementByDeclaration".
    * It will forward to the proper validation 
    * procedures for the given type.
    */        
    if (type == NULL) {
    	xmlSchemaVCustomErr(ctxt, 
    	    XML_SCHEMAV_CVC_TYPE_1,
    	    ctxt->node, NULL, 
    	    "The type definition is absent", NULL);
    	return (XML_SCHEMAV_CVC_TYPE_1);
    }
    
    if (type->flags & XML_SCHEMAS_TYPE_ABSTRACT) {
    	xmlSchemaVCustomErr(ctxt, 
    	    XML_SCHEMAV_CVC_TYPE_2,
    	    ctxt->node, type, 
    	    "The type definition is abstract", NULL);
    	return (XML_SCHEMAV_CVC_TYPE_2);
    }

    switch (type->type) {
	case XML_SCHEMA_TYPE_COMPLEX:
            ret = xmlSchemaValidateElementByComplexType(ctxt, type,
		valSimpleContent);
            break;
	case XML_SCHEMA_TYPE_SIMPLE:
            ret = xmlSchemaValidateElementBySimpleType(ctxt, type,
		isNil, valSimpleContent);
            break;
	case XML_SCHEMA_TYPE_BASIC:
	    if (type->builtInType == XML_SCHEMAS_ANYTYPE)
		ret = xmlSchemaValidateElementByAnyType(ctxt, type);
	    else
		ret = xmlSchemaValidateElementBySimpleType(ctxt, type,
		    isNil, valSimpleContent);
	    break;
	default:
	    ret = -1;
	    break;
    }	
    if (ret == -1)
	return (-1);
    else
	return (ret);
}

static int
xmlSchemaPostCreateVal(xmlSchemaValidCtxtPtr vctxt,
		       xmlSchemaTypePtr type,
		       const xmlChar *value,
		       xmlSchemaValPtr *val)
{
    xmlSchemaTypePtr prim;

    if (val == NULL) {
	xmlSchemaVErr(vctxt, NULL, 
	    XML_SCHEMAV_INTERNAL,
	    "Internal error: xmlSchemaPostCreateVal, "
	    "bad arguments", NULL, NULL);
	return (-1);
    }
    /*
    * Only string or anySimpleType values are expected to be post-created.
    */
    prim = xmlSchemaGetPrimitiveType(type);
    if ((prim->builtInType == XML_SCHEMAS_STRING) || 
	(prim->builtInType == XML_SCHEMAS_ANYSIMPLETYPE))
    {
	if (value == NULL)
	    /* TODO: Can this happen at all? */
	    *val = xmlSchemaNewStringValue(XML_SCHEMAS_STRING,
		xmlStrdup(BAD_CAST ""));
	else
	    *val = xmlSchemaNewStringValue(XML_SCHEMAS_STRING, value);
	if ((*val) == NULL) {
	    xmlSchemaVErr(vctxt, NULL, 
		XML_SCHEMAV_INTERNAL,
		"Internal error: xmlSchemaPostCreateVal, "
		"failed to create the value", NULL, NULL);
	    return (-1); 
	}
	return (0);
    }
    xmlSchemaVErr(vctxt, NULL, 
	XML_SCHEMAV_INTERNAL,
	"Internal error: xmlSchemaPostCreateVal, "
	"the given type is not supported", NULL, NULL);
    return (-1);
}    

static int
xmlSchemaCheckAttrLocallyValid(xmlSchemaValidCtxtPtr vctxt,
			       xmlSchemaAttrStatePtr state)
{
    xmlChar *value;
    const xmlChar *defValue;
    xmlSchemaValPtr defVal;
    int fixed;
    int ret;

    if (vctxt->attrInfo->typeDef == NULL) {
	state->state = XML_SCHEMAS_ATTR_TYPE_NOT_RESOLVED;
	return (XML_SCHEMAS_ATTR_TYPE_NOT_RESOLVED);
    }
    vctxt->node = vctxt->attrInfo->node;
    vctxt->cur = vctxt->node->children;
    /* STREAM */
    value = xmlNodeListGetString(vctxt->node->doc, vctxt->cur, 1);
    
    /*
    * NOTE: This call also checks the content nodes for correct type.
    */
    ret = xmlSchemaValidateSimpleTypeValue(vctxt, vctxt->attrInfo->typeDef,
	value, 1, 1, 1, 1);
    	    
    /*
    * Handle 'fixed' attributes.
    */
    if (ret > 0) {
	state->state = XML_SCHEMAS_ATTR_INVALID_VALUE;
	/*
	* NOTE: Fixed value constraints will be not
	* applied if the value was invalid, because: 
	* 1. The validation process does not return a precomputed 
	*    value.
	* 2. An invalid value implies a violation of a fixed 
	*    value constraint.
	*/
    } else if (ret == 0) {
	state->state = XML_SCHEMAS_ATTR_CHECKED;
	if (xmlSchemaGetEffectiveValueConstraint(
	    (xmlSchemaAttributePtr) vctxt->attrInfo->decl, 
	    &fixed, &defValue, &defVal) && (fixed == 1)) {

	    int ws = xmlSchemaGetWhiteSpaceFacetValue(
		vctxt->nodeInfo->typeDef);	    
	    /*
	    * cvc-au : Attribute Locally Valid (Use)
	    * For an attribute information item to be�valid� 
	    * with respect to an attribute use its �normalized 
	    * value� must match the canonical lexical representation
	    * of the attribute use's {value constraint} value, if it
	    * is present and fixed.
	    *
	    * TODO: Use somehow the *normalized* value and the *canonical*
	    * fixed value. This here compares the canonical values of both.
	    * The normalized value of, for example, a float type can differ
	    * from its canonical representation. This all means that a fixed
	    * value can only be OK, if it's present in the canonical form in
	    * the instance.
	    * NOTE: Since the value for string and anySimpleType is not always
	    * precomputed during validation, we need to do it now.
	    */
	    if (vctxt->value == NULL) {
		/*
		* Post-create the value.
		*/
		if (xmlSchemaPostCreateVal(vctxt, vctxt->attrInfo->typeDef,
		    value, &(vctxt->value)) == -1) {
		    ret = -1;
		    goto exit;
		}
		value = NULL;
	    }
	    if (defVal == NULL) {
		xmlChar *str;
				
		/*
		* Post-create the default/fixed value.
		*/
		if (defValue == NULL)
		    str = xmlStrdup(BAD_CAST "");
		else
		    str = xmlStrdup(defValue);
		if (xmlSchemaPostCreateVal(vctxt, vctxt->attrInfo->typeDef,
		    str, &defVal) == -1) {
		    ret = -1;
		    FREE_AND_NULL(str)
		    goto exit;
		}
		((xmlSchemaAttributePtr) vctxt->attrInfo->decl)->defVal = defVal;
	    }	    
	    if (xmlSchemaCompareValuesWhtsp(vctxt->value,
		(xmlSchemaWhitespaceValueType) ws,
		defVal,
		(xmlSchemaWhitespaceValueType) ws) != 0)
	    {
		state->state = 	XML_SCHEMAS_ATTR_INVALID_FIXED_VALUE;
	    }
	}
    }
exit:  
    if (value != NULL) {
	xmlFree(value);
    }
    return (ret);
}

/**
 * xmlSchemaValidateAttributes:
 * @ctxt:  a schema validation context
 * @elem:  an element
 * @type:  the complexType holding the attribute uses
 *
 * Validate the attributes of an element.
 *
 * 1. Existent, invalid attributes are reported in the form 
 *    "prefix:localName". 
 *    Reason: readability - it is easier to find the actual XML 
 *    representation of the attributes QName.
 * 2. Missing attributes are reported in the form 
 *    {"URI", "localName"}.
 *    This is necessary, since the the prefix need not to be declared
 *    at all, and thus is not computable.
 *
 * Returns 0 if the element is schemas valid, a positive error code
 *     number otherwise and -1 in case of internal or API error.
 */
static int
xmlSchemaValidateAttributes(xmlSchemaValidCtxtPtr ctxt, xmlNodePtr elem, xmlSchemaTypePtr type)
{
    const xmlChar *nsURI;
    int ret;
    xmlAttrPtr attr; /* An attribute on the element. */
    const xmlChar *defValue;
    xmlSchemaValPtr defVal;
    int fixed;
    xmlSchemaAttributeLinkPtr attrUse = NULL;
    xmlSchemaAttributePtr attrDecl;
    int found;
    xmlSchemaAttrStatePtr curState, reqAttrStates = NULL, reqAttrStatesTop = NULL;
    xmlSchemaAttrStatePtr defAttrStates = NULL, defAttrStatesTop = NULL;
    xmlNodePtr oldnode;
#ifdef DEBUG_ATTR_VALIDATION
    int redundant = 0;
#endif

      
    /*
    * Allow all attributes if the type is anyType.
    */
    if (type == xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYTYPE))
	return (0);

    oldnode = ctxt->node;
    if (type != NULL)
	attrUse = type->attributeUses;
    while (attrUse != NULL) {
        found = 0;    
	attrDecl = attrUse->attr;
#ifdef DEBUG_ATTR_VALIDATION
	printf("attr use - name: %s\n", xmlSchemaGetAttrName(attrDecl));
	printf("attr use - use: %d\n", attrDecl->occurs);
#endif
        for (curState = ctxt->attr; curState != NULL; curState = curState->next) {

	    if (curState->decl == attrUse->attr) {
#ifdef DEBUG_ATTR_VALIDATION
		redundant = 1;
#endif
	    }
	    attr = curState->attr;
#ifdef DEBUG_ATTR_VALIDATION
	    printf("attr - name: %s\n", attr->name);
	    if (attr->ns != NULL)
		printf("attr - ns: %s\n", attr->ns->href);
	    else
		printf("attr - ns: none\n");
#endif
	    /* TODO: Can this ever happen? */
            if (attr == NULL)
                continue;
            if (attrDecl->ref != NULL) {
                if (!xmlStrEqual(attr->name, attrDecl->ref))
                    continue;
                if (attr->ns != NULL) {
                    if ((attrDecl->refNs == NULL) ||
                        (!xmlStrEqual(attr->ns->href, attrDecl->refNs)))
                        continue;
                } else if (attrDecl->refNs != NULL) {
                    continue;
                }
            } else {
                if (!xmlStrEqual(attr->name, attrDecl->name))
                    continue;
                /*
                 * handle the namespaces checks here
                 */
                if (attr->ns == NULL) {
		    /*
		     * accept an unqualified attribute only if the target
		     * namespace of the declaration is absent.
		     */
		    if (attrDecl->targetNamespace != NULL)
			/* 
			 * This check was removed, since the target namespace
			 * was evaluated during parsing and already took
			 * "attributeFormDefault" into account.
			 */
		        /* ((attributes->flags & XML_SCHEMAS_ATTR_NSDEFAULT) == 0)) */
		        continue;
		} else {
		    if (attrDecl->targetNamespace == NULL)
		        continue;
		    if (!xmlStrEqual(attrDecl->targetNamespace,
		                     attr->ns->href))
			continue;
		}
            }
#ifdef DEBUG_ATTR_VALIDATION
	    printf("found\n");
#endif
            found = 1;	    
	    curState->decl = attrDecl;
	    curState->state = XML_SCHEMAS_ATTR_VALIDATE_VALUE;
	    /*
	    ret = xmlSchemaCheckAttrLocallyValid(ctxt, attrDecl, curState, attr);
	    */
        }
        if (!found) {
	    if (attrDecl->occurs == XML_SCHEMAS_ATTR_USE_REQUIRED) {
		xmlSchemaAttrStatePtr tmp;
		
#ifdef DEBUG_ATTR_VALIDATION
		printf("required attr not found\n");
#endif
		/*
		* Add a new dummy attribute state.
		*/	
		tmp = (xmlSchemaAttrStatePtr) xmlMalloc(sizeof(xmlSchemaAttrState));
		if (tmp == NULL) {
		    xmlSchemaVErrMemory(ctxt, "registering required attributes", NULL);
		    ctxt->node = oldnode;
		    goto fatal_exit;
		}            
		tmp->attr = NULL;
		tmp->state = XML_SCHEMAS_ATTR_MISSING;
		tmp->decl = attrDecl;
		tmp->next = NULL;
		
		if (reqAttrStates == NULL) {
		    reqAttrStates = tmp;
		    reqAttrStatesTop = tmp;
		} else {
		    reqAttrStatesTop->next = tmp;
		    reqAttrStatesTop = tmp;
		}
	    } else if ((attrDecl->occurs == XML_SCHEMAS_ATTR_USE_OPTIONAL) &&
		    (xmlSchemaGetEffectiveValueConstraint(attrDecl, 
			&fixed, &defValue, &defVal))) {
		xmlSchemaAttrStatePtr tmp;
		/*
		* Handle non existent default/fixed attributes.
		*/	
		tmp = (xmlSchemaAttrStatePtr) 
		    xmlMalloc(sizeof(xmlSchemaAttrState));
		if (tmp == NULL) {
		    xmlSchemaVErrMemory(ctxt, 
			"registering schema specified attributes", NULL);
		    ctxt->node = oldnode;
		    goto fatal_exit;
		}            
		tmp->attr = NULL;
		tmp->state = XML_SCHEMAS_ATTR_DEFAULT;
		tmp->decl = attrDecl;
		tmp->value = defValue;
		tmp->next = NULL;
		if (defAttrStatesTop == NULL)
		    defAttrStates = tmp;
		else
		    defAttrStatesTop->next = tmp;
		defAttrStatesTop = tmp;
	    }
	}
        attrUse = attrUse->next;
    }
    /*
     * Add required attributes to the attribute states of the context.
     */
    if (reqAttrStates != NULL) {
	if (ctxt->attr == NULL) {
	    ctxt->attr = reqAttrStates;
	} else {		
	    ctxt->attrTop->next = reqAttrStates;
	}
	ctxt->attrTop = reqAttrStatesTop;
    }
    /*
    * Process wildcards.
    */
    
    if ((type != NULL) && (type->attributeWildcard != NULL)) {	
#ifdef DEBUG_ATTR_VALIDATION
	xmlSchemaWildcardNsPtr ns;	
	printf("matching wildcard: [%d] of complexType: %s\n", type->attributeWildcard, type->name);
	if (type->attributeWildcard->processContents == 
	    XML_SCHEMAS_ANY_LAX)
	    printf("processContents: lax\n");
	else if (type->attributeWildcard->processContents == 
	    XML_SCHEMAS_ANY_STRICT)
	    printf("processContents: strict\n");
	else
	    printf("processContents: skip\n");
	if (type->attributeWildcard->any)
	    printf("type: any\n");
	else if (type->attributeWildcard->negNsSet != NULL) {
	    printf("type: negated\n");
	    if (type->attributeWildcard->negNsSet->value == NULL)
		printf("ns: (absent)\n");
	    else
		printf("ns: %s\n", type->attributeWildcard->negNsSet->value);
	} else if (type->attributeWildcard->nsSet != NULL) {
	    printf("type: set\n");
	    ns = type->attributeWildcard->nsSet;
	    while (ns != NULL) {
		if (ns->value == NULL)
		    printf("ns: (absent)\n");
		else
		    printf("ns: %s\n", ns->value);
		ns = ns->next;
	    }	    
	} else
	    printf("empty\n");


#endif	
	curState = ctxt->attr;
	while (curState != NULL) {
	    if (curState->state == XML_SCHEMAS_ATTR_UNKNOWN) {		
		if (curState->attr->ns != NULL) 
		    nsURI = curState->attr->ns->href;
		else
		    nsURI = NULL;		
		if (xmlSchemaMatchesWildcardNs(type->attributeWildcard, 
		    nsURI)) {
		    /*
		    * Handle processContents.
		    */
		    if ((type->attributeWildcard->processContents == 
			XML_SCHEMAS_ANY_LAX) ||
			(type->attributeWildcard->processContents == 
			XML_SCHEMAS_ANY_STRICT)) {
			
			attr = curState->attr;						
			attrDecl = xmlSchemaGetAttribute(ctxt->schema, 
			    attr->name, nsURI);
			curState->decl = attrDecl;
			if (attrDecl != NULL) {
			    curState->decl = attrDecl;
			    curState->state = XML_SCHEMAS_ATTR_VALIDATE_VALUE;
			    /* TODO
			    ret = xmlSchemaCheckAttrLocallyValid(ctxt, attrDecl, curState, attr);
			    */
			} else if (type->attributeWildcard->processContents == 
			    XML_SCHEMAS_ANY_LAX) {
			    curState->state = XML_SCHEMAS_ATTR_CHECKED;
			} else
			    curState->state = XML_SCHEMAS_ATTR_WILD_NO_DECL;
		    } else
			curState->state = XML_SCHEMAS_ATTR_CHECKED;
		}
	    }
	    curState = curState->next;
        }
    }    
    
    if (ctxt->attr != NULL) {
	int valueNeeded;

	/*
	* Validate the value of the attribute.
	*/
	if (ctxt->value != NULL) {
	    xmlSchemaFreeValue(ctxt->value);
	    ctxt->value = NULL;
	}
	curState = ctxt->attr;
	while ((curState != NULL) && (curState != ctxt->attrTop->next)) {
	    valueNeeded = 0;
	    switch (curState->state) {
		case XML_SCHEMAS_ATTR_VALIDATE_VALUE:

		    /*
		    * Create an attribute info if needed.
		    */
		    if (ctxt->attrInfo == NULL) {
			ctxt->attrInfo = (xmlSchemaNodeInfoPtr) 
			    xmlMalloc(sizeof(xmlSchemaNodeInfo));
			if (ctxt->attrInfo == NULL) {
			    xmlSchemaVErrMemory(ctxt, 
				"allocating an attribute info", NULL);
			    goto fatal_exit;
			}
		    }
		    /*
		    * Init the attribute info.
		    */
		    ctxt->attrInfo->flags = 0;
		    ctxt->attrInfo->node = (xmlNodePtr) curState->attr;
		    ctxt->attrInfo->decl = (xmlSchemaTypePtr) curState->decl;
		    ctxt->attrInfo->value = NULL;
		    if (curState->decl != NULL)
			ctxt->attrInfo->typeDef = curState->decl->subtypes;
		    else
			ctxt->attrInfo->typeDef = NULL;
		    if (curState->attr->ns != NULL)
			ctxt->attrInfo->namespaceName = 
			curState->attr->ns->href;
		    else
			ctxt->attrInfo->namespaceName = NULL;
		    ctxt->attrInfo->localName = curState->attr->name;
		    
		    ctxt->nodeInfo = ctxt->attrInfo;
		    /*
		    * Evaluate IDCs.
		    */
		    if (ctxt->xpathStates != NULL) {
			ret = xmlSchemaXPathEvaluate(ctxt,
			    XML_ATTRIBUTE_NODE);
			if (ret == -1)
			    goto fatal_exit;
		    }

		    ret = xmlSchemaCheckAttrLocallyValid(ctxt, curState);
		    if (ret == -1)
			goto fatal_exit;
		    if ((ret != 0) && (ctxt->value != NULL)) {
			xmlSchemaFreeValue(ctxt->value);
			ctxt->value = NULL;
		    }
		    /* No break on purpose. */
		case XML_SCHEMAS_ATTR_CHECKED:
		    if (ctxt->xpathStates != NULL) {
			/*
			* Evaluate IDCs.
			*/
			if (ctxt->value != NULL) {
			    ctxt->attrInfo->value = ctxt->value;
			    ctxt->value = NULL;
			}		
			if (xmlSchemaXPathProcessHistory(ctxt, ctxt->depth +1) == -1)
			    goto fatal_exit;
		    }
		    break;
		default:
		    break;
	    }
	    curState = curState->next;
	}

	/*
	* Report missing and illegal attributes.
	*/
	curState = ctxt->attr;
	while ((curState != NULL) && (curState != ctxt->attrTop->next)) {
	    if (curState->state != XML_SCHEMAS_ATTR_CHECKED) {
		attr = curState->attr;
		if (curState->decl != NULL) {
		    if (curState->decl->ref != NULL)
			attrDecl = curState->decl->refDecl;
		    else 
			attrDecl = curState->decl;
		} else
		    attrDecl = NULL;
		if (curState->state == XML_SCHEMAS_ATTR_MISSING) {
		    xmlSchemaVMissingAttrErr(ctxt, elem, attrDecl);
		} else if (curState->state == 
		    XML_SCHEMAS_ATTR_TYPE_NOT_RESOLVED) {
		    xmlSchemaVCustomErr(ctxt,
			XML_SCHEMAV_CVC_ATTRIBUTE_2,
			(xmlNodePtr) attr,
			(xmlSchemaTypePtr) attrDecl,
			"The type definition is absent",
			NULL);
		} else if (curState->state == 
		    XML_SCHEMAS_ATTR_INVALID_FIXED_VALUE) {			
			xmlSchemaVCustomErr(ctxt,
			    XML_SCHEMAV_CVC_AU, 
			    (xmlNodePtr) attr, (xmlSchemaTypePtr) attrDecl,
			    "The value does not match the fixed value "
			    "constraint", NULL);
		} else if (curState->state == XML_SCHEMAS_ATTR_WILD_NO_DECL) {
		    xmlSchemaVWildcardErr(ctxt, 
			XML_SCHEMAV_CVC_WILDCARD,
			(xmlNodePtr) attr,
			type->attributeWildcard,
			"No global attribute declaration found, but "
			"stipulated by the strict processContents of "
			"the wildcard");			
		} else if (curState->state == XML_SCHEMAS_ATTR_UNKNOWN) {
		    /* TODO: "prohibited" won't ever be touched here!. 
		      (curState->state == XML_SCHEMAS_ATTR_PROHIBITED))
		    */
		    /*
		    * TODO: One might report different error messages 
		    * for the following errors.
		    */
		    if ((type == NULL) || (type->attributeWildcard == NULL)) {
			xmlSchemaVIllegalAttrErr(ctxt, 
			    XML_SCHEMAV_CVC_COMPLEX_TYPE_3_2_1, attr);
		    } else {
			xmlSchemaVIllegalAttrErr(ctxt, 
			    XML_SCHEMAV_CVC_COMPLEX_TYPE_3_2_2, attr);
		    }
		}
	    }	
	    curState = curState->next;
	}  
    }    
    
    /*
    * Add missing default/fixed attributes.
    */
    if (defAttrStates != NULL) {    
	curState = defAttrStates;
	
	while (curState != NULL) { 
	    attrDecl = curState->decl;
	    if (attrDecl->ref != NULL)
		attrDecl = attrDecl->refDecl;
	    /*
	    * Evaluate IDCs on default attributes.
	    */
	    if (ctxt->xpathStates != NULL) {
		/*
		* Create an attribute info if needed.
		*/
		if (ctxt->attrInfo == NULL) {
		    ctxt->attrInfo = (xmlSchemaNodeInfoPtr) 
			xmlMalloc(sizeof(xmlSchemaNodeInfo));
		    if (ctxt->attrInfo == NULL) {
			xmlSchemaVErrMemory(ctxt, 
			    "allocating an attribute info", NULL);
			goto fatal_exit;
		    }
		    ctxt->attrInfo->value = NULL;
		}
		/*
		* Init the attribute info.
		* TODO: Hmm, maby a bit oversized this all.
		*/
		ctxt->attrInfo->flags = 0;		
		ctxt->attrInfo->decl = (xmlSchemaTypePtr) attrDecl;
		ctxt->attrInfo->node = NULL;				
		ctxt->attrInfo->typeDef = attrDecl->subtypes;
		ctxt->attrInfo->namespaceName = attrDecl->targetNamespace;
		ctxt->attrInfo->localName = attrDecl->name;

		ctxt->nodeInfo = ctxt->attrInfo;
									    
		ret = xmlSchemaXPathEvaluate(ctxt,
		    XML_ATTRIBUTE_NODE);
		if (ret == -1)
		    goto fatal_exit;
		if (ctxt->attrInfo->value != NULL) {
		    xmlSchemaFreeValue(ctxt->attrInfo->value);
		    ctxt->attrInfo->value = NULL;
		}
		if (ret > 0) {
		    /*
		    * IDCs will consume the precomputed default value,
		    * so we need to clone it somehow.
		    */
		    /*
		    * string or anySimpleType does not create a precomputed value
		    * by default, so it will be created here on demand.
		    * TODO: default/fixed attributes are a bit unoptimized:
		    * the string value will be hold by ->defValue and inside
		    * the precomputed value.
		    */
		    if (attrDecl->defVal == NULL) {
			xmlChar *str = xmlStrdup(attrDecl->defValue);

			if (xmlSchemaPostCreateVal(ctxt,
			    ctxt->attrInfo->typeDef,
			    str,
			    &(attrDecl->defVal)) == -1) {
			    FREE_AND_NULL(str)
			    goto fatal_exit;
			}			
		    }
		    ctxt->attrInfo->value = xmlSchemaCopyValue(attrDecl->defVal);
		    /* TODO: error on NULL return. */
		}
		
		if (xmlSchemaXPathProcessHistory(ctxt, ctxt->depth +1) == -1)
		    goto fatal_exit;
	    }

	    if (ctxt->options & XML_SCHEMA_VAL_VC_I_CREATE) {
		/*
		* PSVI: Add a new attribute node to the current element.
		*/
		if (attrDecl->targetNamespace == NULL) {
		    xmlNewProp(elem, attrDecl->name, curState->value);
		} else {
		    xmlNsPtr ns;
		    
		    ns = xmlSearchNsByHref(elem->doc, elem, 
			attrDecl->targetNamespace);
		    if (ns == NULL) {
			xmlChar prefix[12];
			int counter = 1;
			
			attr = curState->attr;
			/*
			* Create a namespace declaration on the validation 
			* root node if no namespace declaration is in scope.
			*/		    
			snprintf((char *) prefix, sizeof(prefix), "p");
			/*
			* This is somehow not performant, since the ancestor 
			* axis beyond @elem will be searched as well.
			*/
			ns = xmlSearchNs(elem->doc, elem, BAD_CAST prefix);
			while (ns != NULL) {
			    if (counter > 1000) {
				xmlSchemaVErr(ctxt, (xmlNodePtr) attr, 
				    XML_SCHEMAV_INTERNAL,
				    "Internal error: xmlSchemaValidateAttributes, "
				    "could not compute a ns prefix for "
				    "default/fixed attribute '%s'.\n",
				    attrDecl->name, NULL);
				
				break;
			    }
			    snprintf((char *) prefix, 
				sizeof(prefix), "p%d", counter++);
			    ns = xmlSearchNs(elem->doc, elem, 
				BAD_CAST prefix);
			}
			if (ns == NULL) {
			    ns = xmlNewNs(ctxt->validationRoot, 
				attrDecl->targetNamespace, BAD_CAST prefix);
			    xmlNewNsProp(elem, ns, attrDecl->name, 
				curState->value);
			}
		    } else {
			xmlNewNsProp(elem, ns, attrDecl->name, 
			    curState->value);
		    }
		}
	    }
	    curState = curState->next;
	}
    }
    ret = ctxt->err;
    goto exit;

fatal_exit:
    ret = -1;

exit:    

    if (defAttrStates != NULL) 
	xmlSchemaFreeAttributeStates(defAttrStates);
		
#ifdef DEBUG_ATTR_VALIDATION
    if (redundant)
	xmlGenericError(xmlGenericErrorContext,
	    "xmlSchemaValidateAttributes: redundant call by "
	    "type: %s\n", type->name);
#endif
    ctxt->nodeInfo = ctxt->elemInfos[ctxt->depth];
    ctxt->node = oldnode;
    return (ret);
}

/**
 * xmlSchemaStartValidation:
 * @ctxt:  a schema validation context
 *
 * The starting point of the validation, called by 
 * xmlSchemaValidateDocument and xmlSchemaValidateOneElement.
 *
 * Returns 0 if the element is schemas valid, a positive error code
 *     number otherwise and -1 in case of internal or API error.
 */
static int
xmlSchemaStartValidation(xmlSchemaValidCtxtPtr ctxt)
{
    xmlSchemaElementPtr elemDecl;    
    int ret = 0;

    ctxt->err = 0;
    ctxt->nberrors = 0;     
    if (ctxt->schema == NULL) {
	/*
	* No schema was specified at time of creation of the validation
	* context. Use xsi:schemaLocation and xsi:noNamespaceSchemaLocation
	* of the instance to build a schema.
	*/
	if (ctxt->pctxt == NULL)
	    if (xmlSchemaCreatePCtxtOnVCtxt(ctxt) == -1)
		return (-1);
	ctxt->schema = xmlSchemaNewSchema(ctxt->pctxt);
	if (ctxt->schema == NULL)
	    return (-1);
	ctxt->xsiAssemble = 1;
    } else
	ctxt->xsiAssemble = 0;
    /*
    * Assemble new schemata using xsi.
    */
    if (ctxt->xsiAssemble) {	
	ret = xmlSchemaAssembleByXSIElem(ctxt, ctxt->node);
	if (ret == -1) {
	    xmlSchemaVCustomErr(ctxt, 
		XML_SCHEMAV_INTERNAL,
		ctxt->node, NULL, 	
		"Internal error: xmlSchemaValidateElement, "
		"assembling schema by xsi", NULL);	    
	}
	/*
	* NOTE: We won't react on schema parser errors here.
	* TODO: But a warning would be nice.
	*/
    }
    if (ret != -1) {	    
	if (ctxt->node->ns != NULL)
	    elemDecl = xmlSchemaGetElem(ctxt->schema, ctxt->node->name, 
		ctxt->node->ns->href);
	else
	    elemDecl = xmlSchemaGetElem(ctxt->schema, ctxt->node->name, NULL);
	
	if (elemDecl == NULL) {
	    xmlSchemaVCustomErr(ctxt, 
		XML_SCHEMAV_CVC_ELT_1,
		ctxt->node, NULL, 	  
		"No matching global declaration available", NULL);
	    ret = XML_SCHEMAV_CVC_ELT_1;
	} else { 
	    /*
	    * Augment the IDC definitions.
	    */
	    if (ctxt->schema->idcDef != NULL) {
		xmlHashScan(ctxt->schema->idcDef, 
		    (xmlHashScanner) xmlSchemaAugmentIDC, ctxt);
	    }
	    ctxt->depth = -1;
	    xmlSchemaBeginElement(ctxt);
	    ret = xmlSchemaValidateElementByDeclaration(ctxt, elemDecl);    
	    xmlSchemaEndElement(ctxt);
	    if (ret < 0) {
		xmlSchemaVCustomErr(ctxt,
		    XML_SCHEMAV_INTERNAL, ctxt->node, NULL,
		    "Internal error: xmlSchemaValidateElement, "
		    "calling validation by declaration", NULL);
	    }
	}
    }

    if (ctxt->xsiAssemble) {
	if (ctxt->schema != NULL) {
	    xmlSchemaFree(ctxt->schema);
	    ctxt->schema = NULL;
	}
    }
    xmlSchemaClearValidCtxt(ctxt);
    return (ret);   
}


/**
 * xmlSchemaValidateOneElement:
 * @ctxt:  a schema validation context
 * @elem:  an element node
 *
 * Validate a branch of a tree, starting with the given @elem.
 *
 * Returns 0 if the element and its subtree is valid, a positive error 
 * code number otherwise and -1 in case of an internal or API error.
 */
int
xmlSchemaValidateOneElement(xmlSchemaValidCtxtPtr ctxt, xmlNodePtr elem)
{
    if ((ctxt == NULL) || (elem == NULL) || (elem->type != XML_ELEMENT_NODE))
	return (-1);

    if (ctxt->schema == NULL) {
	xmlSchemaVErr(ctxt, NULL,
	    XML_SCHEMAV_INTERNAL,
	    "API error: xmlSchemaValidateOneElement, "
	    "no schema specified.\n", NULL, NULL);
	return (-1);
    }

    ctxt->doc = elem->doc;
    ctxt->node = elem;
    ctxt->validationRoot = elem;
    return (xmlSchemaStartValidation(ctxt));
}

/************************************************************************
 * 									*
 * 			SAX Validation code				*
 * 									*
 ************************************************************************/

/************************************************************************
 * 									*
 * 			Validation interfaces				*
 * 									*
 ************************************************************************/

/**
 * xmlSchemaNewValidCtxt:
 * @schema:  a precompiled XML Schemas
 *
 * Create an XML Schemas validation context based on the given schema.
 *
 * Returns the validation context or NULL in case of error
 */
xmlSchemaValidCtxtPtr
xmlSchemaNewValidCtxt(xmlSchemaPtr schema)
{
    xmlSchemaValidCtxtPtr ret;

    ret = (xmlSchemaValidCtxtPtr) xmlMalloc(sizeof(xmlSchemaValidCtxt));
    if (ret == NULL) {
        xmlSchemaVErrMemory(NULL, "allocating validation context", NULL);
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaValidCtxt));
    ret->schema = schema;    
    return (ret);
}

/**
 * xmlSchemaClearValidCtxt:
 * @ctxt: the schema validation context
 *
 * Free the resources associated to the schema validation context;
 * leaves some fields alive intended for reuse of the context.
 */
static void
xmlSchemaClearValidCtxt(xmlSchemaValidCtxtPtr vctxt)
{
    if (vctxt == NULL)
        return;

    vctxt->flags = 0;

    vctxt->validationRoot = NULL;
    if (vctxt->attr != NULL) {
        xmlSchemaFreeAttributeStates(vctxt->attr);
	vctxt->attr = NULL;
    }
    if (vctxt->value != NULL) {
        xmlSchemaFreeValue(vctxt->value);
	vctxt->value = NULL;
    }
    /*
    * Augmented IDC information.
    */
    if (vctxt->aidcs != NULL) {
	xmlSchemaIDCAugPtr cur = vctxt->aidcs, next;
	do {
	    next = cur->next;
	    xmlFree(cur);
	    cur = next;
	} while (cur != NULL);
	vctxt->aidcs = NULL;
    }
    if (vctxt->idcNodes != NULL) {
	int i;
	xmlSchemaPSVIIDCNodePtr item;

	for (i = 0; i < vctxt->nbIdcNodes; i++) {
	    item = vctxt->idcNodes[i];	    
	    xmlFree(item->keys);
	    xmlFree(item);
	}
	xmlFree(vctxt->idcNodes);
	vctxt->idcNodes = NULL;
    }
    /* 
    * Note that we won't delete the XPath state pool here.
    */
    if (vctxt->xpathStates != NULL) {
	xmlSchemaFreeIDCStateObjList(vctxt->xpathStates);
	vctxt->xpathStates = NULL;
    }
    if (vctxt->attrInfo != NULL) {
	if (vctxt->attrInfo->value != NULL) {
	    xmlSchemaFreeValue(vctxt->attrInfo->value);	    
	}
	memset(vctxt->attrInfo, 0, sizeof(xmlSchemaNodeInfo));
    }
    if (vctxt->elemInfos != NULL) {
	int i;
	xmlSchemaNodeInfoPtr info;
	
	for (i = 0; i < vctxt->sizeElemInfos; i++) {
	    info = vctxt->elemInfos[i];
	    if (info == NULL)
		break;
	    if (info->value != NULL) {
		xmlSchemaFreeValue(info->value);
		info->value = NULL;
	    }
	    if (info->idcMatchers != NULL) {
		xmlSchemaIDCFreeMatcherList(info->idcMatchers);
		info->idcMatchers = NULL;
	    }
	    if (info->idcTable != NULL) {
		xmlSchemaIDCFreeIDCTable(info->idcTable);
		info->idcTable = NULL;
	    }
	}
    }
}

/**
 * xmlSchemaFreeValidCtxt:
 * @ctxt:  the schema validation context
 *
 * Free the resources associated to the schema validation context
 */
void
xmlSchemaFreeValidCtxt(xmlSchemaValidCtxtPtr ctxt)
{
    if (ctxt == NULL)
        return;
    if (ctxt->attr != NULL)
        xmlSchemaFreeAttributeStates(ctxt->attr);
    if (ctxt->value != NULL)
        xmlSchemaFreeValue(ctxt->value);
    if (ctxt->pctxt != NULL)
	xmlSchemaFreeParserCtxt(ctxt->pctxt);
    if (ctxt->idcNodes != NULL) {
	int i;
	xmlSchemaPSVIIDCNodePtr item;

	for (i = 0; i < ctxt->nbIdcNodes; i++) {
	    item = ctxt->idcNodes[i];	    
	    xmlFree(item->keys);
	    xmlFree(item);
	}
	xmlFree(ctxt->idcNodes);
    }
    if (ctxt->idcKeys != NULL) {
	int i;
	for (i = 0; i < ctxt->nbIdcKeys; i++)
	    xmlSchemaIDCFreeKey(ctxt->idcKeys[i]);
	xmlFree(ctxt->idcKeys);
    }

    if (ctxt->xpathStates != NULL)
	xmlSchemaFreeIDCStateObjList(ctxt->xpathStates);
    if (ctxt->xpathStatePool != NULL)
	xmlSchemaFreeIDCStateObjList(ctxt->xpathStatePool);

    /*
    * Augmented IDC information.
    */
    if (ctxt->aidcs != NULL) {
	xmlSchemaIDCAugPtr cur = ctxt->aidcs, next;
	do {
	    next = cur->next;
	    xmlFree(cur);
	    cur = next;
	} while (cur != NULL);
    }
    if (ctxt->attrInfo != NULL) {
	if (ctxt->attrInfo->value != NULL)
	    xmlSchemaFreeValue(ctxt->attrInfo->value);
	xmlFree(ctxt->attrInfo);
    }
    if (ctxt->elemInfos != NULL) {
	int i;
	xmlSchemaNodeInfoPtr info;
	
	for (i = 0; i < ctxt->sizeElemInfos; i++) {
	    info = ctxt->elemInfos[i];
	    if (info == NULL)
		break;
	    if (info->value != NULL)
		xmlSchemaFreeValue(info->value);
	    if (info->idcMatchers != NULL)
		xmlSchemaIDCFreeMatcherList(info->idcMatchers);
	    if (info->idcTable != NULL)
		xmlSchemaIDCFreeIDCTable(info->idcTable);
	    /*
	    * TODO: Don't know if those will have to be freed if in streaming
	    * mode.
	    *
	    * xmlFree(info->localName);
	    * if (info->namespaceName != NULL)
	    *	xmlFree(info->namespaceName);
	    */
	    xmlFree(info);
	}
	xmlFree(ctxt->elemInfos);
    }
    xmlFree(ctxt);
}

/**
 * xmlSchemaSetValidErrors:
 * @ctxt:  a schema validation context
 * @err:  the error function
 * @warn: the warning function
 * @ctx: the functions context
 *
 * Set the error and warning callback informations
 */
void
xmlSchemaSetValidErrors(xmlSchemaValidCtxtPtr ctxt,
                        xmlSchemaValidityErrorFunc err,
                        xmlSchemaValidityWarningFunc warn, void *ctx)
{
    if (ctxt == NULL)
        return;
    ctxt->error = err;
    ctxt->warning = warn;
    ctxt->userData = ctx;
    if (ctxt->pctxt != NULL)
	xmlSchemaSetParserErrors(ctxt->pctxt, err, warn, ctx);
}

/**
 * xmlSchemaGetValidErrors:
 * @ctxt:	a XML-Schema validation context
 * @err: the error function result
 * @warn: the warning function result
 * @ctx: the functions context result
 *
 * Get the error and warning callback informations
 *
 * Returns -1 in case of error and 0 otherwise
 */
int
xmlSchemaGetValidErrors(xmlSchemaValidCtxtPtr ctxt,
						xmlSchemaValidityErrorFunc * err,
						xmlSchemaValidityWarningFunc * warn, void **ctx)
{
	if (ctxt == NULL)
		return (-1);
	if (err != NULL)
		*err = ctxt->error;
	if (warn != NULL)
		*warn = ctxt->warning;
	if (ctx != NULL)
		*ctx = ctxt->userData;
	return (0);
}


/**
 * xmlSchemaSetValidOptions:
 * @ctxt:	a schema validation context
 * @options: a combination of xmlSchemaValidOption
 *
 * Sets the options to be used during the validation.
 *
 * Returns 0 in case of success, -1 in case of an
 * API error.
 */
int
xmlSchemaSetValidOptions(xmlSchemaValidCtxtPtr ctxt,
			 int options)
					
{
    int i;

    if (ctxt == NULL)
	return (-1);
    /*
    * WARNING: Change the start value if adding to the
    * xmlSchemaValidOption.
    * TODO: Is there an other, more easy to maintain,
    * way?
    */
    for (i = 1; i < (int) sizeof(int) * 8; i++) {
        if (options & 1<<i) {
	    xmlSchemaVErr(ctxt, NULL,
		XML_SCHEMAV_INTERNAL,
		"Internal error: xmlSchemaSetValidOptions, "
		"invalid option argument.\n", NULL, NULL);
	    return (-1);   
        }	
    }
    ctxt->options = options;
    return (0);      
}

/**
 * xmlSchemaValidCtxtGetOptions:
 * @ctxt:	a schema validation context 
 *
 * Get the validation context options.
 * 
 * Returns the option combination or -1 on error.
 */
int
xmlSchemaValidCtxtGetOptions(xmlSchemaValidCtxtPtr ctxt)
					
{    
    if (ctxt == NULL)
	return (-1);
    else 
	return (ctxt->options);    
}


/**
 * xmlSchemaValidateDoc:
 * @ctxt:  a schema validation context
 * @doc:  a parsed document tree
 *
 * Validate a document tree in memory.
 *
 * Returns 0 if the document is schemas valid, a positive error code
 *     number otherwise and -1 in case of internal or API error.
 */
int
xmlSchemaValidateDoc(xmlSchemaValidCtxtPtr ctxt, xmlDocPtr doc)
{
    if ((ctxt == NULL) || (doc == NULL))
        return (-1);

    ctxt->doc = doc;      
    ctxt->node = xmlDocGetRootElement(doc);
    if (ctxt->node == NULL) {
        xmlSchemaVCustomErr(ctxt, 
	    XML_SCHEMAV_DOCUMENT_ELEMENT_MISSING,
	    (xmlNodePtr) doc, NULL,
	    "The document has no document element", NULL);
        return (ctxt->err);
    }    
    ctxt->validationRoot = ctxt->node;
    xmlSchemaStartValidation(ctxt);

    return (ctxt->err);
}

/**
 * xmlSchemaValidateStream:
 * @ctxt:  a schema validation context
 * @input:  the input to use for reading the data
 * @enc:  an optional encoding information
 * @sax:  a SAX handler for the resulting events
 * @user_data:  the context to provide to the SAX handler.
 *
 * Validate a document tree in memory.
 *
 * Returns 0 if the document is schemas valid, a positive error code
 *     number otherwise and -1 in case of internal or API error.
 */
int
xmlSchemaValidateStream(xmlSchemaValidCtxtPtr ctxt,
                        xmlParserInputBufferPtr input, xmlCharEncoding enc,
                        xmlSAXHandlerPtr sax, void *user_data)
{
    if ((ctxt == NULL) || (input == NULL))
        return (-1);
    ctxt->input = input;
    ctxt->enc = enc;
    ctxt->sax = sax;
    ctxt->user_data = user_data;
    TODO return (0);
}

#define bottom_xmlschemas
#include "elfgcchack.h"
#endif /* LIBXML_SCHEMAS_ENABLED */
