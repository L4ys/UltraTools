#include "CommonyChooser.h"




BEGINSTRUCTREGION()
//--------------------------------------------------------------------------
int common_single_chooser_t<Struct_Chooser_Item>::widths[] =
{
	CHCOL_HEX | 10,  // Address
	32,           // Name
	40           // Description
};
// column headers
char * common_single_chooser_t<Struct_Chooser_Item>::header[] =
{
	"Offset",      // 0
	"Name" , // 1
	"Description"  // 1
};

common_single_chooser_t<Struct_Chooser_Item>::common_single_chooser_t(QVector<Struct_Chooser_Item>* list_type_library, const char *title) :chooser_t(CH_MODAL, qnumber(widths), widths, header, title)
{
	_list_library = *(list_type_library);

}
size_t idaapi common_single_chooser_t<Struct_Chooser_Item>::get_count() const {
	return _list_library.size();
}
void idaapi common_single_chooser_t<Struct_Chooser_Item>::get_row(
	qstrvec_t *cols_,
	int *icon_,
	chooser_item_attrs_t *attrs,
	size_t n) const
{
	qstrvec_t &cols = *cols_;
	Struct_Chooser_Item item = (_list_library)[n];
	cols[0].sprnt("%08x",item.offset);
	cols[1]=(item.name);
	cols[2]=(item.desc);
}
ENDSTRUCTREGION()


BEGINSTRUCTREGION()
//--------------------------------------------------------------------------
int common_single_chooser_t<Type_Library_Chooser_Item>::widths[] =
{
	CHCOL_HEX | 8,  // Address
	32           // Instruction
};
// column headers
char * common_single_chooser_t<Type_Library_Chooser_Item>::header[] =
{
	"TypeLibrary",      // 0
	"Instruction"  // 1
};

common_single_chooser_t<Type_Library_Chooser_Item>::common_single_chooser_t(QVector<Type_Library_Chooser_Item>* list_type_library, const char *title) :chooser_t(CH_MODAL, qnumber(widths), widths, header, title)
{
	_list_library = *(list_type_library);

}
size_t idaapi common_single_chooser_t<Type_Library_Chooser_Item>::get_count() const {
	return _list_library.size();
}
void idaapi common_single_chooser_t<Type_Library_Chooser_Item>::get_row(
	qstrvec_t *cols_,
	int *icon_,
	chooser_item_attrs_t *attrs,
	size_t n) const
{
	qstrvec_t &cols = *cols_;
	Type_Library_Chooser_Item item = (_list_library)[n];
	cols[0]=(item.name);
	cols[1]=(item.desc);
}
ENDSTRUCTREGION()
