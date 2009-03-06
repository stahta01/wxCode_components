/***************************************************************
 * Name:      SFEvents.h
 * Purpose:   Defines shape events classes
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-09-11
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef _WXSFEVENTS_H
#define _WXSFEVENTS_H

#include <wx/event.h>
#include <wx/dnd.h>

#include "Defs.h"
#include "ShapeBase.h"

class WXDLLIMPEXP_SF wxSFShapeEvent;
class WXDLLIMPEXP_SF wxSFShapeTextEvent;
class WXDLLIMPEXP_SF wxSFShapeDropEvent;
class WXDLLIMPEXP_SF wxSFShapePasteEvent;

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_SF, wxEVT_SF_LINE_DONE, 7770)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_SF, wxEVT_SF_TEXT_CHANGE, 7771)
	DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_SF, wxEVT_SF_ON_DROP, 7772)
	DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_SF, wxEVT_SF_ON_PASTE, 7773)
END_DECLARE_EVENT_TYPES()

typedef void (wxEvtHandler::*wxSFShapeEventFunction)(wxSFShapeEvent&);
typedef void (wxEvtHandler::*wxSFShapeTextEventFunction)(wxSFShapeTextEvent&);
typedef void (wxEvtHandler::*wxSFShapeDropEventFunction)(wxSFShapeDropEvent&);
typedef void (wxEvtHandler::*wxSFShapePasteEventFunction)(wxSFShapePasteEvent&);

#define wxSFShapeEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxSFShapeEventFunction, &func)

#define wxSFShapeTextEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxSFShapeTextEventFunction, &func)

#define wxSFShapeDropEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxSFShapeDropEventFunction, &func)

#define wxSFShapePasteEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxSFShapePasteEventFunction, &func)


/*! \brief Event table macro mapping event wxEVT_SF_LINE_DONE. This event occures
 * when the interactive connection creation process is finished. The generated event
 * object holds a pointer to the new line shape. */
#define EVT_SF_LINE_DONE(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
        wxEVT_SF_LINE_DONE, id, wxID_ANY, \
        (wxObjectEventFunction)(wxEventFunction) wxStaticCastEvent( wxSFShapeEventFunction, &fn ), \
        (wxObject *) NULL \
    ),

/*! \brief Event table macro mapping event wxEVT_SF_TEXT_CHANGE. This event occures
 * when the editable text shape's content is changed. */
#define EVT_SF_TEXT_CHANGE(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
        wxEVT_SF_TEXT_CHANGE, id, wxID_ANY, \
        (wxObjectEventFunction)(wxEventFunction) wxStaticCastEvent( wxSFShapeTextEventFunction, &fn ), \
        (wxObject *) NULL \
    ),

/*! \brief Event table macro mapping event wxEVT_SF_ON_DROP. This event occures
 * when dragged shapes (via D&D operation) are dropped to a canvas. */
#define EVT_SF_ON_DROP(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
        wxEVT_SF_ON_DROP, id, wxID_ANY, \
        (wxObjectEventFunction)(wxEventFunction) wxStaticCastEvent( wxSFShapeDropEventFunction, &fn ), \
        (wxObject *) NULL \
    ),

/*! \brief Event table macro mapping event wxEVT_SF_ON_PASTE. This event occures
 * when shapes stored in the clipboard are pasted to a canvas. */
#define EVT_SF_ON_PASTE(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
        wxEVT_SF_ON_PASTE, id, wxID_ANY, \
        (wxObjectEventFunction)(wxEventFunction) wxStaticCastEvent( wxSFShapePasteEventFunction, &fn ), \
        (wxObject *) NULL \
    ),

/*!
 * \brief Class encapsulating generic wxSF shapes' event.
 */
class WXDLLIMPEXP_SF wxSFShapeEvent : public wxEvent
{
public:
    /*! \brief Constructor */
    wxSFShapeEvent(wxEventType cmdType = wxEVT_NULL, int id = 0);
    /*! \brief Copy constructor */
    wxSFShapeEvent(const wxSFShapeEvent& obj);
    /*! \brief Destructor */
    virtual ~wxSFShapeEvent();

    // public member data accessors
    /*!
     * \brief Insert a shape object to the event object.
     * \param shape Pointer to the shape object
     */
    void SetShape(wxSFShapeBase* shape){m_pShape = shape;}
    /*!
     * \brief Get a shape object from the event object.
     * \return Pointer to the shape object.
     */
    wxSFShapeBase* GetShape(){return m_pShape;}

    /*! \brief Clone this event object and return pointer to the new instance. */
    wxEvent* Clone() const { return new wxSFShapeEvent(*this); }


private:
    // private data members
    /*! \brief Pointer to stored shape object. */
    wxSFShapeBase* m_pShape;
};

class WXDLLIMPEXP_SF wxSFShapeTextEvent : public wxEvent
{
public:
    /*! \brief Constructor */
    wxSFShapeTextEvent(wxEventType cmdType = wxEVT_NULL, int id = 0);
    /*! \brief Copy constructor */
    wxSFShapeTextEvent(const wxSFShapeTextEvent& obj);
    /*! \brief Destructor */
    virtual ~wxSFShapeTextEvent();

    // public member data accessors
    /*!
     * \brief Insert a shape object to the event object.
     * \param shape Pointer to the shape object
     */
    void SetShape(wxSFShapeBase* shape){m_pShape = shape;}
    /*!
     * \brief Get a shape object from the event object.
     * \return Pointer to the shape object.
     */
    wxSFShapeBase* GetShape(){return m_pShape;}
    /*!
     * \brief Set new text shape.
     * \param txt New text content.
     */
	void SetText(const wxString& txt){m_sText = txt;}
    /*!
     * \brief Get a shape text.
     * \return Shape text content.
     */
	wxString GetText(){return m_sText;}

    /*! \brief Clone this event object and return pointer to the new instance. */
    wxEvent* Clone() const { return new wxSFShapeTextEvent(*this); }


private:
    // private data members
    /*! \brief Pointer to stored shape object. */
    wxSFShapeBase* m_pShape;
	/*! \brief New shape text. */
	wxString m_sText;
};

/*!
 * \brief Class encapsulating wxEVT_SF_ON_DROP event.
 */
class WXDLLIMPEXP_SF wxSFShapeDropEvent : public wxEvent
{
public:
    /*! \brief Constructor */
    wxSFShapeDropEvent(wxEventType cmdType = wxEVT_NULL, wxCoord x = 0, wxCoord y = 0, wxSFShapeCanvas* target = NULL, wxDragResult def = wxDragNone, int id = 0);
    /*! \brief Copy constructor */
    wxSFShapeDropEvent(const wxSFShapeDropEvent& obj);
    /*! \brief Destructor */
    virtual ~wxSFShapeDropEvent();

    // public member data accessors
    /*!
     * \brief Copy given shapes to the internal list of dropped shapes.
     * \param list Reference to a list of copied shapes
     */
    void SetDroppedShapes(const ShapeList& list);
	 /*!
     * \brief Set a position where the shapes were dropped.
     * \param pos Position
     */
	void SetDropPosition(const wxPoint& pos){m_nDropPosition = pos;}
	 /*!
     * \brief Set drag result.
     * \param def Drag result
     */
	void SetDragResult(wxDragResult def){m_nDragResult = def;}
	/*!
	 * \brief Set drop target (shape canvas where shapes have been dropped to).
	 * \param target Pointer to drop target (shape canvas)
	 */
	void SetDropTarget(wxSFShapeCanvas *target){m_pDropTarget = target;}
    /*!
     * \brief Get a shape object from the event object.
     * \return Pointer to the shape object.
     */
    ShapeList& GetDroppedShapes(){return m_lstDroppedShapes;}
    /*!
     * \brief Get drop position.
     * \return Position.
     */
	wxPoint GetDropPosition(){return m_nDropPosition;}
    /*!
     * \brief Get drag result.
     * \return Drag result.
     */
	wxDragResult GetDragResult(){return m_nDragResult;}
	/*!
	 * \brief Get drop target (shape canvas where shapes have been dropped to).
	 * \return Pointer to drop target (shape canvas)
	 */
	wxSFShapeCanvas* GetDropTarget(){return m_pDropTarget;}

    /*! \brief Clone this event object and return pointer to the new instance. */
    wxEvent* Clone() const { return new wxSFShapeDropEvent(*this); }


private:
    // private data members
    /*! \brief List of dropped shapes. */
    ShapeList m_lstDroppedShapes;
	/*! \brief Drop target. */
	wxSFShapeCanvas *m_pDropTarget;
	/*! \brief Drop position. */
	wxPoint m_nDropPosition;
	/*! \brief Drag result. */
	wxDragResult m_nDragResult;
};

/*!
 * \brief Class encapsulating wxEVT_SF_ON_PASTE event.
 */
class WXDLLIMPEXP_SF wxSFShapePasteEvent : public wxEvent
{
public:
    /*! \brief Constructor */
    wxSFShapePasteEvent(wxEventType cmdType = wxEVT_NULL, wxSFShapeCanvas *target = NULL, int id = 0);
    /*! \brief Copy constructor */
    wxSFShapePasteEvent(const wxSFShapePasteEvent& obj);
    /*! \brief Destructor */
    virtual ~wxSFShapePasteEvent();

    // public member data accessors
    /*!
     * \brief Copy given shapes to the internal list of pasted shapes.
     * \param list Reference to a list of copied shapes
     */
    void SetPastedShapes(const ShapeList& list);
	/*!
	 * \brief Set drop target (shape canvas where shapes have been pasted to).
	 * \param target Pointer to drop target (shape canvas)
	 */
	void SetDropTarget(wxSFShapeCanvas *target){m_pDropTarget = target;}
    /*!
     * \brief Get a shape object from the event object.
     * \return Pointer to the shape object.
     */
    ShapeList& GetPastedShapes(){return m_lstPastedShapes;}
	/*!
	 * \brief Get drop target (shape canvas where shapes have been pasted to).
	 * \return Pointer to drop target (shape canvas)
	 */
	wxSFShapeCanvas* GetDropTarget(){return m_pDropTarget;}

    /*! \brief Clone this event object and return pointer to the new instance. */
    wxEvent* Clone() const { return new wxSFShapePasteEvent(*this); }


private:
    // private data members
    /*! \brief List of pasted shapes. */
    ShapeList m_lstPastedShapes;
	/*! \brief Drop target. */
	wxSFShapeCanvas *m_pDropTarget;
};

#endif // _WXSFEVENTS_H
