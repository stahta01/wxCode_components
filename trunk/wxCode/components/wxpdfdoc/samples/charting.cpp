///////////////////////////////////////////////////////////////////////////////
// Name:        charting.cpp
// Purpose:     Examples of simple charting in wxPdfDocument
// Author:      Ulrich Telle
// Modified by:
// Created:     2005-11-23
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/pdfdoc.h"

/**
* Charting
*
* This example shows how very simple pie and bar charts can be created.
* Additionally the available marker symbols are shown.
*/

class PdfCharting : public wxPdfDocument
{
public:
  void PieChart(double width, double height, int nData, const wxString* label, double* data, const wxColour* colors)
  {
    SetFont(_T("Arial"), _T(""), 10);
    double margin = 2;
    double hLegend = 5;

    // Determine maximal legend width and sum of data values
    double sum = 0.;
    double wLegend = 0;
    double labelWidth;
    int i;
    for (i = 0; i < nData; i++)
    {
      sum = sum + data[i];
      labelWidth = GetStringWidth(label[i]);
      if (labelWidth > wLegend) wLegend = labelWidth;
    }

    double radius = width - margin * 4 - hLegend - wLegend;
    if (radius > height - margin * 2) radius = height - margin * 2;
    radius = floor(radius / 2);
    double xPage = GetX();
    double xDiag = GetX() + margin + radius;
    double yDiag = GetY() + margin + radius;
    //Sectors
    SetLineWidth(0.2);
    double angle = 0;
    double angleStart = 0;
    double angleEnd = 0;
    for (i = 0; i < nData; i++)
    {
      angle = (sum != 0) ? floor((data[i] * 360) / sum) : 0;
      if (angle != 0)
      {
        angleEnd = angleStart + angle;
        SetFillColor(colors[i]);
        Sector(xDiag, yDiag, radius, angleStart, angleEnd);
        angleStart += angle;
      }
    }
    if (angleEnd != 360)
    {
      Sector(xDiag, yDiag, radius, angleStart - angle, 360);
    }

    //Legends
    double x1 = xPage + 2 * radius + 4 * margin;
    double x2 = x1 + hLegend + margin;
    double y1 = yDiag - radius + (2 * radius - nData*(hLegend + margin)) / 2;
    for (i = 0; i < nData; i++)
    {
      SetFillColor(colors[i]);
      Rect(x1, y1, hLegend, hLegend, wxPDF_STYLE_FILLDRAW);
      SetXY(x2, y1);
      Cell(0, hLegend, label[i]);
      y1 += hLegend + margin;
    }
  }

  void BarDiagram(double width, double height, int nData, const wxString* label, double* data, const wxColour& color = wxColour(), double maxVal = 0, int nDiv = 4)
  {
    wxPdfColour saveColor = GetFillColor();
    wxColour localColor = color;
    if (!localColor.Ok())
    {
      localColor = wxColour(155,155,155);
    }

    SetFont(_T("Arial"), _T(""), 10);

    // Determine maximal legend width and sum of data values
    double maxValue = data[0];
    double sum = 0.;
    double wLegend = 0;
    double labelWidth;
    int i;
    for (i = 0; i < nData; i++)
    {
      if (data[i] > maxValue) maxValue = data[i];
      sum = sum + data[i];
      labelWidth = GetStringWidth(label[i]);
      if (labelWidth > wLegend) wLegend = labelWidth;
    }
    if (maxVal == 0)
    {
      maxVal = maxValue;
    }

    double margin = 2;
    double yDiag = GetY() + margin;
    double hDiag = floor(height - margin * 2);
    double xDiag = GetX() + margin * 2 + wLegend;
    double wDiag = floor(width - margin * 3 - wLegend);

    double tickRange = ceil(maxVal / nDiv);
    maxVal = tickRange * nDiv;
    double tickLen = floor(wDiag / nDiv);
    wDiag = tickLen * nDiv;
    double unit = wDiag / maxVal;
    double hBar = floor(hDiag / (nData + 1));
    hDiag = hBar * (nData + 1);
    double eBaton = floor(hBar * 0.8);

    SetLineWidth(0.2);
    Rect(xDiag, yDiag, wDiag, hDiag);

    double xpos, ypos;
    // Scales
    for (i = 0; i <= nDiv; i++)
    {
      xpos = xDiag + tickLen * i;
      Line(xpos, yDiag, xpos, yDiag + hDiag);
      wxString val = wxString::Format(_T("%.2f"), i * tickRange);
      xpos -= GetStringWidth(val) / 2;
      ypos = yDiag + hDiag;
      Text(xpos, ypos + 2*margin, val);
    }

    SetFillColor(color);
    double wval;
    for (i = 0; i < nData; i++)
    {
      // Bar
      wval = (data[i] != 0.0) ? (int)(data[i] * unit) : (int)(2.5 * unit);
      ypos = yDiag + (i + 1) * hBar - eBaton / 2;
      Rect(xDiag, ypos, wval, eBaton, wxPDF_STYLE_FILLDRAW);
      // Legend
      SetXY(0, ypos);
      Cell(xDiag - margin, eBaton, label[i], wxPDF_BORDER_NONE, 0, wxPDF_ALIGN_RIGHT);
    }
    SetFillColor(saveColor);
  }
};

void
charting()
{
  PdfCharting pdf;

  // Show examples of a simple pie chart and a simple bar chart
  pdf.AddPage();
  pdf.SetFont(_T("Arial"),_T(""),12);

  int nData = 4;
  wxColour colors[] = { wxColour(92,172,238), wxColour(67,205,128), wxColour(255,99,71), wxColour(255,215,0)};
  wxString labels[] = { _T("Job 1"), _T("Job 2"),_T("Job 3"),_T("Job 4") };
  double pieData[] = { 30., 20., 40., 10. };

  pdf.SetX(40);
  pdf.MultiCell(0,4.5, _T("Pie Chart Sample"));
  pdf.Ln(5);
  pdf.SetX(pdf.GetX()+30);
  pdf.PieChart(125, 70, nData, labels, pieData, colors);

  pdf.SetFont(_T("Arial"),_T(""),12);
  pdf.SetXY(40,110);
  pdf.MultiCell(0,4.5, _T("Bar Chart Sample"));
  pdf.SetXY(40,120);
  nData = 3;
  double barData[] = { 50., 80., 25. };
  wxString label[] = { _T("Job 1"), _T("Job 2"), _T("Job 3") };
  pdf.BarDiagram(70, 35, nData, label, barData, wxColour(176,196,222), 100, 2);


  // Show available marker symbols
  pdf.AddPage();

  pdf.Cell(40.,0., _T("Marker symbols and arrows"));
  pdf.Marker(25., 80., wxPDF_MARKER_CIRCLE, 15.0);
  pdf.Arrow(35.,85., 70., 105., 0.5, 8., 3.);
  pdf.Marker(78., 109., wxPDF_MARKER_CIRCLE, 10.0);
  pdf.SetFillColor(wxColour(255,99,71));
  pdf.Arrow(120.,75., 90., 100., 0.2, 6., 2.);
  pdf.SetFillColor(wxColour(255,255,0));

  pdf.SetLineWidth(0.12);
  double x;
  double x0 = 10;
  double y0 = 25.;
  double y1 = 20;
  double y2 = 30;
  pdf.Line(10, y1, 150, y1);
  pdf.Line(10, y2, 150, y2);

  int i;
  for (i = 0; i < wxPDF_MARKER_LAST; i++)
  {
    x = 7. * i + 7. + x0;
    pdf.Line(x, y0 + 9., x, y0 - 9.);
    pdf.Marker(x, y1, (wxPdfMarker) i, 4.2);
  }

  pdf.SetFillColor(wxColour(0,0,0));
  for (i = 0; i < wxPDF_MARKER_LAST; i++)
  {
    x = 7. * i + 7. + x0;
    pdf.Marker(x, y2, (wxPdfMarker) i, 4.2);
  }

  pdf.AddPage();

#if 0
// Get a list of month using the current locale
$months = $gDateLocale->GetShortMonth();

  // Setup the tab title
  $graph->tabtitle->Set('Year 2003');
  $graph->tabtitle->SetFont(FF_ARIAL,FS_BOLD,10);

  // Setup month as labels on the X-axis
  $graph->xaxis->SetTickLabels($months);
  $graph->xaxis->SetFont(FF_ARIAL,FS_NORMAL,8);
  $graph->xaxis->SetLabelAngle(45);

#endif

  pdf.SetLineWidth(0.2);
  pdf.SetDrawColor(0,0,0);
  pdf.Rect(55, 53, 100, 72);

  pdf.SetAlpha(1, 0.5);
  int j;
  pdf.SetFillColor(wxPdfColour(wxString(_T("#BBBBBB"))));
  for (j = 0; j < 3; j++)
  {
    pdf.Rect(55, 53+2*j*12, 100, 12, wxPDF_STYLE_FILL);
  }
  pdf.SetFillColor(wxPdfColour(wxString(_T("#DDDDDD"))));
  for (j = 0; j < 3; j++)
  {
    pdf.Rect(55, 53+2*(j+1)*12-12, 100, 12, wxPDF_STYLE_FILL);
  }
  pdf.SetAlpha();

  wxPdfArrayDouble dash;
  dash.Add(3.);
  dash.Add(3.);
  wxPdfLineStyle dashStyle(0.2, wxPDF_LINECAP_BUTT, wxPDF_LINEJOIN_MITER, dash, 0., wxPdfColour(wxString(_T("gray"))));
  pdf.SetLineStyle(dashStyle);
  for (j = 1; j < 6; j++)
  {
    pdf.Line(55, 53+j*12, 55+100, 53+j*12);
  }
  for (j = 1; j < 10; j++)
  {
    pdf.Line(55+j*10, 53, 55+j*10, 53+72);
  }

  double xdata[]  = { 10,  20, 30,  40,  50,  60, 70, 80,  90, 100 };
  double ydata[]  = { 10, 120, 80, 190, 260, 170, 60, 40,  20, 230 };
  double ydata2[] = { 10,  70, 40, 120, 200,  60, 80, 40,  20,   5 };
  wxPdfColour fcol(wxString(_T("#440000")));
  wxPdfColour tcol(wxString(_T("#FF9090")));
  int grad = pdf.LinearGradient(fcol, tcol, wxPDF_LINEAR_GRADIENT_REFLECTION_LEFT);
  for (j = 0; j < 10; j++)
  {
    pdf.SetFillGradient(xdata[j]-3+50, 125-ydata[j]*0.25, 6, ydata[j]*0.25, grad);
  }

  wxPdfArrayDouble solid;
  wxPdfLineStyle solidStyle(0.1, wxPDF_LINECAP_BUTT, wxPDF_LINEJOIN_MITER, solid, 0., wxPdfColour(wxString(_T("blue"))));
  pdf.SetLineStyle(solidStyle);
  wxPdfArrayDouble xl, yl;
  for (j = 0; j < 10; j++)
  {
    xl.Add(xdata[j]+50); yl.Add(125-ydata2[j]*0.25);
  }
  xl.Add(xdata[9]+50); yl.Add(125);
  xl.Add(xdata[0]+50); yl.Add(125);
  pdf.SetDrawColor(wxPdfColour(wxString(_T("navy"))));
  pdf.SetFillColor(wxPdfColour(wxString(_T("skyblue"))));
  pdf.SetAlpha(0.75,0.5);
  pdf.Polygon(xl, yl, wxPDF_STYLE_FILLDRAW);

  pdf.SetAlpha(0.75,1);
  pdf.SetDrawColor(wxPdfColour(wxString(_T("blue"))));
  pdf.SetFillColor(wxPdfColour(wxString(_T("lightblue"))));
  pdf.SetLineWidth(0.1);
  for (j = 0; j < 10; j++)
  {
    pdf.Marker(xdata[j]+50, 125-ydata2[j]*0.25, wxPDF_MARKER_SQUARE, 2.);
  }
  pdf.SetAlpha();

#if 0
  pdf.SetAlpha(1, 0.5);
  pdf.SetFillColor(wxPdfColour(wxString(_T("skyblue"))));
  pdf.Rect(105, 210, 20, 15, wxPDF_STYLE_FILL);
  pdf.SetAlpha();

  //example of clipping polygon
  wxPdfArrayDouble x1, x2, y;
  x1.Add(30); x2.Add(120); y.Add(135);
  x1.Add(60); x2.Add(150); y.Add(155);
  x1.Add(40); x2.Add(130); y.Add(155);
  x1.Add(70); x2.Add(160); y.Add(160);
  x1.Add(30); x2.Add(120); y.Add(165);

  pdf.ClippingPolygon(x1,y,false);
  pdf.SetFillGradient(20,135,50,30,grad1);
  pdf.UnsetClipping();


  $bplot->SetFillGradient($fcol,$tcol,GRAD_LEFT_REFLECTION);

  // Set line weigth to 0 so that there are no border
  // around each bar
  $bplot->SetWeight(0);

$  graph->Add($bplot);

  // Create filled line plot
  $lplot = new LinePlot($ydata2);
  $lplot->SetFillColor('skyblue@0.5');
  $lplot->SetColor('navy@0.7');
  $lplot->SetBarCenter();

#endif
  pdf.SaveAsFile(_T("charting.pdf"));
}


