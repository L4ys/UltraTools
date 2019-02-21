#include "UltraGlobal.h"

template <class T>
class common_single_chooser_t : public chooser_t
{
protected:
	static  int widths[];
	// column headers
	static char * header[];

public:
	QVector<T> _list_library;;
	common_single_chooser_t(QVector<T>* list_type_library, const char *title);

	virtual void idaapi get_row(
		qstrvec_t *cols,
		int *icon_,
		chooser_item_attrs_t *attrs,
		size_t n) const;

	virtual size_t idaapi get_count() const;

	virtual const void  * get_obj_id(size_t *len) const
	{
		*len = strlen(title);
		return title;
	}




	// function that is called when the user hits Enter
	virtual cbret_t idaapi enter(size_t n)
	{

		return cbret_t(); // nothing changed
	}
};

#ifndef SMARTDEFINECHOOSER(cls) 
#define SMARTDEFINECHOOSER(cls)  typedef common_single_chooser_t<##cls> cls ## _Chooser
#endif

#ifndef BEGINSTRUCTREGION()
#define BEGINSTRUCTREGION()
#endif

#ifndef ENDSTRUCTREGION()
#define ENDSTRUCTREGION()
#endif

