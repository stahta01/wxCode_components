/////////////////////////////////////////////////////////////////////////////
// Name:        contrib/samples/ogl/ogledit/view.h
// Purpose:     View-related classes
// Author:      Julian Smart
// Modified by:
// Created:     12/07/98
// RCS-ID:      $Id: view.h,v 1.1 2007/03/28 15:15:49 frm Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _OGLSAMPLE_VIEW_H_
#define _OGLSAMPLE_VIEW_H_

#include "doc.h"
#include "wx/ogl/ogl.h"

class MyCanvas: public wxShapeCanvas
{
// DECLARE_DYNAMIC_CLASS(wxShapeCanvas)
protected:
  wxView* m_view;
public:

  MyCanvas(wxView*, wxWindow *parent = NULL, wxWindowID id = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
            long style = wxRETAINED);
  virtual ~MyCanvas(void);

  wxDocument* GetDocument() const { return m_view->GetDocument(); }

  virtual void OnLeftClick(double x, double y, int keys = 0);
  virtual void OnRightClick(double x, double y, int keys = 0);

  virtual void OnDragLeft(bool draw, double x, double y, int keys=0); // Erase if draw false
  virtual void OnBeginDragLeft(double x, double y, int keys=0);
  virtual void OnEndDragLeft(double x, double y, int keys=0);

  virtual void OnDragRight(bool draw, double x, double y, int keys=0); // Erase if draw false
  virtual void OnBeginDragRight(double x, double y, int keys=0);
  virtual void OnEndDragRight(double x, double y, int keys=0);

protected:
  void OnMouseEvent(wxMouseEvent&);
  void OnPaint(wxPaintEvent&);
  DECLARE_EVENT_TABLE()

  friend class DiagramView;
};

class DiagramDocument;
class DiagramView : public wxView
{
  //typedef wxView base;
  DECLARE_DYNAMIC_CLASS(DiagramView)
protected:
  MyCanvas* m_canvas;
public:

  DiagramView(void) { m_canvas = NULL; }
  virtual ~DiagramView(void) {}
  virtual void OnChangeFilename();

  DiagramDocument* GetDocument();

  virtual bool OnCreate(wxDocument*, long flags);
  virtual void OnDraw(wxDC*);
  virtual void OnUpdate(wxView *sender, wxObject *hint = NULL);
  virtual bool OnClose(bool deleteWindow = true);

  wxShape *FindSelectedShape(void);
protected:
  void OnCut(wxCommandEvent&);
  void OnChangeBackgroundColour(wxCommandEvent&);
  void OnEditLabel(wxCommandEvent&);
  DECLARE_EVENT_TABLE()
};

#endif
    // _OGLSAMPLE_VIEW_H_
