/////////////////////////////////////////////////////////////////////////////
// Name:        contrib/samples/ogl/studio/doc.h
// Purpose:     Document classes
// Author:      Julian Smart
// Modified by:
// Created:     12/07/98
// RCS-ID:      $Id: doc.h,v 1.1 2007/03/28 15:15:50 frm Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _STUDIO_DOC_H_
#define _STUDIO_DOC_H_

#include "wx/ogl/ogl.h"

#include "shapes.h"

/*
 * A diagram document, which contains a diagram.
 */

class csDiagramDocument : public wxDocument
{
    typedef wxDocument base;
    DECLARE_DYNAMIC_CLASS(csDiagramDocument)
public:
    csDiagramDocument();
    virtual ~csDiagramDocument();

    virtual bool OnSaveDocument(const wxString& file);
    virtual bool OnOpenDocument(const wxString& file);

    wxDiagram *GetDiagram() { return &m_diagram; }

    virtual bool OnCloseDocument();
    virtual bool DeleteContents();

    virtual wxCommandProcessor *OnCreateCommandProcessor();

protected:
    csDiagram         m_diagram;
};

/*
 Do/Undo 30/7/98

 1) We have a csCommandState, and in csDiagramCommand you have a list of
    these. This allows undo to work with several shapes at once.

 2) Instead of storing info about each operation, e.g. separate pens, colours,
    etc., we simply use a copy of the shape.
    In csCommandState, we have a pointer to the actual shape in the canvas, m_currentShape.
    We also have wxShape* m_shapeState which stores the requested or previous state
    (depending on whether it's before the Do or after the Do.

    - In Do: save a temp copy of the old m_currentShape (i.e. the state just before it's changed).
      Change the data pointed to by m_currentShape to the new attributes stored in m_shapeState.
      Now assign the temp copy to m_shapeState, for use in Undo.

      wxShape* temp = m_currentShape->Copy(); // Take a copy of the current state
      m_currentShape->Set(m_shapeState);      // Apply the new state (e.g. moving, changing colour etc.)
      delete m_shapeState;                    // Delete the previous 'old state'.
      m_shapeState = temp;                    // Remember the new 'old state'.

 */


class csCommandState;
class csDiagramCommand : public wxCommand
{
    typedef wxCommand base;
    friend class csCommandState;
public:
    // Multi-purpose constructor for creating, deleting shapes
    csDiagramCommand(const wxString& name, csDiagramDocument*,
        csCommandState* onlyState = NULL); // Allow for the common case of just one state to change

    virtual ~csDiagramCommand();

    bool Do();
    bool Undo();

    // Add a state to the end of the list
    void AddState(csCommandState*);

    // Insert a state at the beginning of the list
    void InsertState(csCommandState*);

    // Schedule all lines connected to the states to be cut.
    void RemoveLines();

    // Find the state that refers to this shape
    csCommandState* FindStateByShape(wxShape* shape);

    const wxList& GetStates() const { return m_states; }

protected:
    csDiagramDocument*    m_doc;
    wxList                m_states;
};

class csCommandState : public wxObject
{
    typedef wxObject base;
    friend class csDiagramCommand;
public:
    csCommandState(int cmd, wxShape* savedState, wxShape* shapeOnCanvas);
    virtual ~csCommandState();

    bool Do();
    bool Undo();

    inline void SetSavedState(wxShape *s) { m_savedState = s; }
    inline wxShape *GetSavedState() const { return m_savedState; }

    inline void SetShapeOnCanvas(wxShape *s) { m_shapeOnCanvas = s; }
    inline wxShape *GetShapeOnCanvas() const { return m_shapeOnCanvas; }
protected:
    wxShape*                m_savedState;       // Previous state, for restoring on Undo
    wxShape*                m_shapeOnCanvas;    // The actual shape on the canvas
    csDiagramDocument*      m_doc;
    int                     m_cmd;

    // These store the line ordering for the shapes at either end,
    // so an un-cut line can restore the ordering properly. Otherwise
    // it just adds the line at an arbitrary position.
    int                     m_linePositionFrom;
    int                     m_linePositionTo;
};

#endif
  // _STUDIO_DOC_H_
