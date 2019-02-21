#include <hexrays.hpp>
#include <idp.hpp>
#include <loader.hpp>
#include <dbg.hpp>
#include <name.hpp>
#include <bytes.hpp>
#include <QtCore/qvector.h>





static bool inited = false;


// The node to keep inverted-if information.
static const char nodename[] = "$ hexrays inverted-if";
static netnode node;

// Cached copy of inverted if-statement addresses
static eavec_t inverted_ifs;

#ifndef ACTION_NAME
#define ACTION_NAME "Find Containing Structures"
#endif 


typedef struct  Type_Library_Chooser_Item{
	til_t* lib;
	qstring name;
	qstring desc;
} *Type_Library_Chooser_Item_Ptr;



typedef struct  Struct_Chooser_Item{
	void* item;
	qstring name;
	qstring desc;
	uint64 offset;
} *Struct_Chooser_Item_Ptr;

