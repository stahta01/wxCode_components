/////////////////////////////////////////////////////////////////////////////
// Name:        scunderc.h
// Purpose:     wxScriptFunctionUnderC, wxScriptFileUnderC, wxUnderC
// Author:      Francesco Montorsi
// Modified by: 
// Created:     2004/29/4
// RCS-ID:      
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////



#ifdef wxSCRIPT_USE_UNDERC

// includes
#include "ucdl.h"
#include "wx/script.h"


//! An UnderC interpreted function.
class wxScriptFunctionUnderC : public wxScriptFunction
{
public:

	//! Creates & set the properties of the object.
	wxScriptFunctionUnderC(const wxString &name = wxEmptyString, 
						   const wxString &ret = wxEmptyString,
						   wxScriptTypeInfo *arr = NULL, int n = 0) { 
		Set(name, ret, arr, n);
	}


	virtual wxScriptFunction *Clone() const {
		wxScriptFunction *newf = new wxScriptFunctionUnderC();
		newf->DeepCopy(this);
		return newf;
	}

	virtual bool Exec(wxScriptVar &ret, wxScriptVar *arg) const;
};




//! An UnderC file script.
class wxScriptFileUnderC : public wxScriptFile
{
public:
	wxScriptFileUnderC(const wxString &toload = wxEmptyString) { 
		m_tScriptFile = wxUNDERC_SCRIPTFILE;
		if (!toload.IsEmpty())
			Load(toload);
	}

	virtual bool Load(const wxString &filename);
};




//! The UnderC interpreter.
class wxUnderC : public wxScriptInterpreter
{
protected:
	
	//! TRUE if the script interpreter is ready to work.
	bool m_bInit;

	//! The array of the standard functions of UnderC.
	wxScriptFunctionArray m_arrStd;

	//! Returns the array containing ALL currently interpreted functions.
	//! This functions is different from #GetFunctionList() because
	//! it doesn't exclude functions loaded by default by UnderC.
	void GetFunctionListComplete(wxScriptFunctionArray &) const;

public:
	wxUnderC() : m_bInit(FALSE) {}
	virtual ~wxUnderC() { uc_finis(); }

	//! Returns the global instance of this class.
	static wxUnderC *Get() { return m_pUnderC; }

	//! Inits UnderC.
	virtual bool Init();

	//! Returns TRUE if UnderC is ready.
	virtual bool isReady() const { return m_bInit; }

	//! Returns the list of the functions currently recognized by the interpreter.
	virtual void GetFunctionList(wxScriptFunctionArray &) const;
};



#endif		// wxSCRIPT_USE_UNDERC


