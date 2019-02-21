#include "CommonyChooser.h"
#include <cstdarg>

// Hex-Rays API pointer
hexdsp_t *hexdsp = NULL;


SMARTDEFINECHOOSER(Struct_Chooser_Item);
SMARTDEFINECHOOSER(Type_Library_Chooser_Item);

static void *  find_deep_members(QVector<Struct_Chooser_Item>* listAllPtr, tinfo_t* parent_tinfo, tinfo_t* target_tinfo, int idx)
{

	udt_type_data_t udt;
	udt_type_data_t::iterator it;
	udt_type_data_t::iterator itRef;

	int szVoid = 32;
	if (inf.is_64bit())
	{
		szVoid = 64;
	}
	bool hr = parent_tinfo->get_udt_details(&udt, GTD_NO_LAYOUT);
	if (hr)
	{
		for (it = udt.begin(); it != udt.end(); it++)
		{

			if (it->type.equals_to(*target_tinfo))
			{
				Struct_Chooser_Item item;
				parent_tinfo->get_final_type_name(&item.name);
				item.item = parent_tinfo;
				item.offset = it->offset;

				int q = 0;
				for (itRef = udt.begin(); itRef != udt.end(); itRef++)
				{
					if (itRef->offset >= it->offset + (idx*szVoid))
					{
						item.isbaseclass = itRef->is_baseclass();
						qstring parent_name;
						itRef->type.get_final_type_name(&parent_name);
						item.desc = itRef->is_baseclass() ? qstring().sprnt("BaseClass%d_", q) + parent_name : itRef->name;
						listAllPtr->push_back(item);
						break;
					}
					q++;
				}
				break;
			
			}
			else
			{
				if (it->type.is_udt())
				{
					find_deep_members(listAllPtr, &it->type, target_tinfo, idx);
				}

			}

		}
	}
	return 0;
}

static QVector<Struct_Chooser_Item> find_containing_structures(tinfo_t* target_tinfo, til_t* lib, int idx)
{
	QVector<Struct_Chooser_Item> listAll;


	const char *typeName = target_tinfo->dstr();
	int status = get_named_type(lib, typeName, NTF_TYPE);
	if (status)
	{
		size_t min_struct_size = target_tinfo->get_size();
		int typeqty = get_ordinal_qty(lib);
		for (int i = 0; i < typeqty; i++)
		{
			tinfo_t* parent_tinfo = new tinfo_t();
			parent_tinfo->create_typedef(lib, i);
			if (parent_tinfo->get_size() >= min_struct_size)
			{
				find_deep_members(&listAll, parent_tinfo, target_tinfo, idx);

			}


		}

	}


	return listAll;
}

static int idaapi callback(void *, hexrays_event_t event, va_list va)
{
	switch (event)
	{
	case hxe_populating_popup:
	{
		TWidget *widget = va_arg(va, TWidget *);
		TPopupMenu *popup = va_arg(va, TPopupMenu *);
		vdui_t &vu = *va_arg(va, vdui_t *);
		if (vu.item.citype == VDI_EXPR)
		{
			if (vu.item.e->op == cot_var)
			{


				attach_action_to_popup(widget, popup, ACTION_NAME);

			}


		}



		break;
	}


	case hxe_maturity:
	{

		break;
	}
	default:
		break;
	}
	return 0;
}
struct ida_local if_finder_t : public ctree_visitor_t
{
	ea_t ea;
	vdui_t *_vu;
	cexpr_t* arg_ret;
	QVector<Struct_Chooser_Item> _listStruct;
	tinfo_t*  _target_tinfo;
	til_t* _lib;
	Struct_Chooser_Item _structIt;
	if_finder_t(vdui_t *vu, tinfo_t* target_tinfo, til_t* lib) : ctree_visitor_t(CV_FAST), _target_tinfo(target_tinfo), _vu(vu), _lib(lib), ea(vu->tail.loc.ea)
	{

	}
	cexpr_t marshalExp(cexpr_t *ie)
	{

		int szVoid = 32;
		if (inf.is_64bit())
		{
			szVoid = 64;
		}
		tinfo_t ntp(BT_INT);
		int idx = ie->x->y->n->value(ntp);

		tinfo_t* parent_tinfo = (tinfo_t*)(_structIt.item);

		uint64 offsetVal = _structIt.offset;
		udt_type_data_t udt;
		udt_type_data_t::iterator it;
		tinfo_t* info_typeFake = nullptr;
		bool hr = parent_tinfo->get_udt_details(&udt, GTD_NO_LAYOUT);
		if (hr)
		{
			for (it = udt.begin(); it != udt.end(); it++)
			{
				if (it->offset >= offsetVal + (idx*szVoid))
				{
					info_typeFake = new tinfo_t(it->type);
					break;
				}
			}
		}

		carg_t*	arg_address = new  carg_t();
		cexpr_t*cpyexp = new cexpr_t(*ie->x->x);
		arg_address->consume_cexpr(cpyexp);

		tinfo_t* info_typeVoid = new tinfo_t(BT_VOID);
		info_typeVoid->create_ptr(*info_typeVoid);	

		qstring parent_name;
		parent_tinfo->get_final_type_name(&parent_name);
		carg_t*	arg_type = new carg_t();
		cexpr_t*	cexpr_arg = create_helper(true, *parent_tinfo, parent_name.c_str());		
		arg_type->consume_cexpr(cexpr_arg);
		carg_t*	arg_field = new  carg_t();
		
		if (_structIt.isbaseclass)
		{
			tinfo_t* parent_tinfo_Ref = new tinfo_t(*parent_tinfo);
			parent_tinfo_Ref->create_ptr(*parent_tinfo_Ref);
			qstring FullName = qstring("root->") + _structIt.desc;
			cexpr_t* expVar = create_helper(true, *parent_tinfo_Ref, FullName.c_str());		
			arg_field->consume_cexpr(expVar);

		} else
		{
			tinfo_t* parent_tinfo_Ref = new tinfo_t(*parent_tinfo);
			parent_tinfo_Ref->create_ptr(*parent_tinfo_Ref);
			cexpr_t* expVar = create_helper(true, *parent_tinfo_Ref, "root");			
			cexpr_t* expPtr = new cexpr_t(cot_memptr, expVar);		
			cexpr_t*expPtrHelp = new cexpr_t(*expPtr);
			expPtr->v.idx = (offsetVal + (idx*szVoid)) / szVoid;
			arg_field->consume_cexpr(expPtrHelp);
		}	
		//cexpr_t* cexpr_field = create_helper(true, *info_typeVoid, _structIt.desc.c_str());		
		carglist_t arglist;
		arglist.push_back(*arg_address);
		arglist.push_back(*arg_type);
		arglist.push_back(*arg_field);


		cexpr_t* arg_ret = call_helper(*info_typeFake, nullptr, "CONTAINING_RECORD");
		arg_ret->a->push_back(*arg_address);
		arg_ret->a->push_back(*arg_type);
		arg_ret->a->push_back(*arg_field);



		return *arg_ret;
	}

	cexpr_t * findChildExp(cexpr_t *i)
	{
		if (i->op == cot_memref)
		{
			if (i->x->op == cot_idx)
			{
				if (i->x->x->op == cot_var)
				{
					return  i;

				}
				else
				{
					return findChildExp(i->x->x);
				}
			}

		}

		return nullptr;
	}


	int  findChildIndex(cexpr_t *i)
	{
		if (i->op == cot_memref)
		{
			if (i->x->op == cot_idx)
			{
				if (i->x->x->op == cot_var)
				{
					tinfo_t ntp(BT_INT);
					int idx = i->x->y->n->value(ntp);
					return  idx;

				}
				else
				{
					return findChildIndex(i->x->x);
				}
			}

		}

		return 0;
	}

	void auto_fix_expr(cexpr_t *i, cexpr_t arg_ret)
	{
		insn_t ins;
		decode_insn(&ins, i->ea);
		/*qstring disline;
		generate_disasm_line(&disline, i->ea);
		msg(disline.c_str());*/
		uint offIdx = ins.ops[0].addr;
		tinfo_t* info_typeFake = new tinfo_t(arg_ret.type);
		udt_type_data_t udt;
		udt_type_data_t::iterator it;

		bool hr = info_typeFake->get_udt_details(&udt, GTD_NO_LAYOUT);
		if (hr)
		{
			it = udt.begin();
			//it->type.create_ptr(arg_ret.type);
			cexpr_t* expRef = new cexpr_t(cot_memref, &arg_ret);
			expRef->type = it->type;
			tinfo_t* info_typeRef = new tinfo_t(it->type);
			info_typeRef->remove_ptr_or_array();
			udt_type_data_t udtRef;
			udt_type_data_t::iterator itRef;
			hr = info_typeRef->get_udt_details(&udtRef, GTD_NO_LAYOUT);
			if (hr)
			{
				for (itRef = udtRef.begin(); itRef != udtRef.end(); itRef++)
				{
					if (itRef->offset / 8 >= offIdx)
					{
						cexpr_t* expPtr = new cexpr_t(cot_memptr, expRef);
						//itRef->type.create_ptr(it->type);
						expPtr->type = itRef->type;
						expPtr->v.idx = offIdx;
						i->assign(*expPtr);

						break;
					}
				}
			}


		}
		i->calc_type(true);

	}


	int idaapi visit_expr(cexpr_t *i)
	{


		cexpr_t arg_ret;
		if (i->ea == ea)
		{


			if (i->op == cot_memref)
			{
				char * title_Struct = "Type_Library_Struct";
				int idx = findChildIndex(i);
				_listStruct = find_containing_structures(_target_tinfo, _lib, idx);
				if (!_listStruct.empty())
				{
					Struct_Chooser_Item_Chooser* chrStruct = new  Struct_Chooser_Item_Chooser(&_listStruct, title_Struct);
					ssize_t sctidx = chrStruct->choose();

					if (sctidx != -1)
					{

						_structIt = _listStruct[sctidx];
						cexpr_t * foundExp = findChildExp(i);
						if (foundExp != nullptr)
						{
							arg_ret = marshalExp(foundExp);

							auto_fix_expr(i, arg_ret);
						}
						return 0;
					}
				}
			}

		}
		return 0;
	}
};
//-------------------------------------------------------------------------
struct find_containing_ah_t : public action_handler_t
{
	virtual int idaapi activate(action_activation_ctx_t *ctx)
	{
		vdui_t &vu = *get_widget_vdui(ctx->widget);
		const til_t *ida_typelib = get_idati();
		QVector<Type_Library_Chooser_Item> list_type_library;
		Type_Library_Chooser_Item tplibFirst = { (til_t *)ida_typelib, qstring(ida_typelib->name), qstring(ida_typelib->desc) };
		list_type_library.push_back(tplibFirst);
		for (int i = 0; i < ida_typelib->nbases; i++)
		{
			Type_Library_Chooser_Item tplib = { ida_typelib->base[i], qstring(ida_typelib->base[i]->name), qstring(ida_typelib->base[i]->desc) };
			list_type_library.push_back(tplib);

		}

		char * title = "Type_Library_Chooser";
		Type_Library_Chooser_Item_Chooser* chrTypeLib = new  Type_Library_Chooser_Item_Chooser(&list_type_library, title);
		ssize_t chsidx = chrTypeLib->choose();

		if (chsidx != -1)
		{
			tinfo_t* target_tinfo = &vu.item.e->type;

			if (target_tinfo->is_ptr())
			{
				target_tinfo->remove_ptr_or_array();

			}




			if_finder_t iff(&vu, target_tinfo, list_type_library[chsidx].lib);
			iff.apply_to(&vu.cfunc->body, NULL);







		}
		vu.refresh_ctext();

		return 1;
	}

	virtual action_state_t idaapi update(action_update_ctx_t *ctx)
	{
		/*vdui_t *vu = get_widget_vdui(ctx->widget);
		if (vu == NULL)
		return AST_DISABLE_FOR_WIDGET;*/
		//return find_if_statement(*vu) == NULL ? AST_DISABLE : AST_ENABLE;

		return  AST_ENABLE_ALWAYS;
	}
};
static find_containing_ah_t find_containing_ah;
static const action_desc_t find_containing_action =
ACTION_DESC_LITERAL(
ACTION_NAME,
ACTION_NAME,
&find_containing_ah,
NULL,
NULL,
-1
);

//--------------------------------------------------------------------------
// Initialize the plugin.
int idaapi init(void)
{
	if (!inited)
	{


		if (!init_hexrays_plugin())
			return PLUGIN_SKIP; // no decompiler
		if (!node.create(nodename)) // create failed -> node existed
		{
			size_t n;
			void *blob = node.getblob(NULL, &n, 0, 'I');
			if (blob != NULL)
			{
				inverted_ifs.clear();
				inverted_ifs.inject((ea_t *)blob, n / sizeof(ea_t));
				for (int i = 0; i < inverted_ifs.size(); i++)
					inverted_ifs[i] = node2ea(inverted_ifs[i]);
			}
		}
		const char *hxver = get_hexrays_version();
		msg("Hex-rays version %s has been detected, %s ready to use\n", hxver, PLUGIN.wanted_name);

		install_hexrays_callback(callback, nullptr);
		register_action(find_containing_action);
		inited = true;
	}
	return PLUGIN_KEEP;
}

//--------------------------------------------------------------------------
void idaapi term(void)
{
	if (inited)
	{
		// clean up
		remove_hexrays_callback(callback, NULL);
		term_hexrays_plugin();
	}
}

//--------------------------------------------------------------------------
bool idaapi run(size_t)
{




	return true;
}

//--------------------------------------------------------------------------
static char comment[] = "Find Containing Structure plugin for Hex-Rays decompiler";

//--------------------------------------------------------------------------
//
//      PLUGIN DESCRIPTION BLOCK
//
//--------------------------------------------------------------------------
plugin_t PLUGIN =
{
	IDP_INTERFACE_VERSION,
	PLUGIN_HIDE,          // plugin flags
	init,                 // initialize
	term,                 // terminate. this pointer may be NULL.
	run,                  // invoke plugin
	comment,              // long comment about the plugin
	// it could appear in the status line
	// or as a hint
	"",                   // multiline help about the plugin
	"Hex-Rays Find Containing Structure", // the preferred short name of the plugin
	""                    // the preferred hotkey to run the plugin
};
