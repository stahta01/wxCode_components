/////////////////////////////////////////////////////////////////////////////
// Name:        sclua.h
// Purpose:     wxScriptFunctionLua, wxScriptFileLua, wxLua
// Author:      Francesco Montorsi
// Modified by: 
// Created:     2004/6/5
// RCS-ID:      
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////



#ifdef wxSCRIPT_USE_LUA

// includes
extern "C" {			// this wrapping is very important: Lua is a C library !!!
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
};
#include "wx/script.h"



//! A Lua interpreted function.
class wxScriptFunctionLua : public wxScriptFunction
{
public:

	//! Creates the object; no info about return value and
	//! arguments are required since Lua does not store them.
	wxScriptFunctionLua(const wxString &name = wxEmptyString) { 
		Set(name, "", NULL, 0);
	}


	virtual wxScriptFunction *Clone() const {
		wxScriptFunction *newf = new wxScriptFunctionLua();
		newf->DeepCopy(this);
		return newf;
	}

	virtual bool Exec(wxScriptVar &ret, wxScriptVar *arg) const;
};




//! A Lua file script.
class wxScriptFileLua : public wxScriptFile
{
public:
	wxScriptFileLua(const wxString &toload = wxEmptyString) { 
		m_tScriptFile = wxLUA_SCRIPTFILE;
		if (!toload.IsEmpty())
			Load(toload);
	}

	virtual bool Load(const wxString &filename);
};




//! The Lua interpreter.
class wxLua : public wxScriptInterpreter
{
public:

	//! The LUA state of the interpreter. This variable is public because
	//! is must be accessible also by extern functions/classes.
	lua_State *m_state;

protected:
	
	//! TRUE if the script interpreter is ready to work.
	bool m_bInit;

	//! The Lua standard libraries loaded by wxLua.
	static luaL_reg luaLibs[16];

	//! The array of the standard functions of UnderC.
	wxScriptFunctionArray m_arrStd;

	//! Returns the array containing ALL currently interpreted functions.
	//! This functions is different from #GetFunctionList() because
	//! it doesn't exclude functions loaded by default by UnderC.
	wxScriptFunctionArray GetFunctionListComplete() const;

public:
	wxLua() : m_bInit(FALSE) {}
	virtual ~wxLua() { lua_close(m_state); }

	//! Returns the global instance of this class.
	static wxLua *Get() { return m_pLua; }

	//! Inits UnderC.
	virtual bool Init();

	//! Returns TRUE if UnderC is ready.
	virtual bool isReady() const { return m_bInit; }

	//! Returns the list of the functions currently recognized by the interpreter.
	virtual wxScriptFunctionArray GetFunctionList() const;
};



#endif		// wxSCRIPT_USE_LUA


