/* 
 *   A wrapper for the CEP data presentation
 *
 *   Copyright (C) Daniel Fernandez                 2002
 *   Copyright (C) Michael Still                    2002
 *   Copyright (C) Blake Swadling                   2002
 *   Copyright (C) Kristy Van Der Vlist             2002
 *   Copyright (C) Nick Wheatstone                  2002
 *   
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *   
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *   
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "cepCore.h"
#include "cepPresentation.h"
#include "cepPlot.h"

cepPlot::cepPlot (cepDataset * theDataset, cepDataset::direction dir,
		  string cfname, long x, long y, float &horizScale,
		  float &vertScale, long &xminval, long &yminval,
		  long &yrange, bool haveLs, bool freqDomain, float energy,
		  int windowTarget):
m_plotfailed (false)
{
  cepConfiguration *config;
  config = (cepConfiguration *) & cepConfiguration::getInstance ();

  cepPresentation pres (x, y, theDataset->getMatrix (dir),
			theDataset->getOffset (dir));
  if (haveLs)
    pres.setLsParams (theDataset->getB1 (dir), theDataset->getB2 (dir));
  else if (freqDomain)
    pres.setFreqParams (energy);

  cepError err;
  int red = 0, green = 0, blue = 0;
  bool toggle;

  // Errors
  err = config->getValue ("ui-viewmenu-showerrors", true, toggle);
  if (err.isReal ())
    err.display ();
  pres.useErrors (toggle);

  // Grid
  err = config->getValue ("ui-viewmenu-showgrid", true, toggle);
  if (err.isReal ())
    err.display ();
  pres.useGrid (toggle);

  // Axes
  err = config->getValue ("ui-graph-color-axis-r", 255, red);
  if (err.isReal ())
    err.display ();
  err = config->getValue ("ui-graph-color-axis-g", 0, green);
  if (err.isReal ())
    err.display ();
  err = config->getValue ("ui-graph-color-axis-b", 0, blue);
  if (err.isReal ())
    err.display ();
  pres.setAxesColor (red, green, blue);

  // Plot color 1
  err = config->getValue ("ui-graph-color-line1-r", 0, red);
  if (err.isReal ())
    err.display ();
  err = config->getValue ("ui-graph-color-line1-g", 255, green);
  if (err.isReal ())
    err.display ();
  err = config->getValue ("ui-graph-color-line1-b", 0, blue);
  if (err.isReal ())
    err.display ();
  pres.setLineColor (0, red, green, blue);

  // Plot color 2
  err = config->getValue ("ui-graph-color-line2-r", 0, red);
  if (err.isReal ())
    err.display ();
  err = config->getValue ("ui-graph-color-line2-g", 255, green);
  if (err.isReal ())
    err.display ();
  err = config->getValue ("ui-graph-color-line2-b", 0, blue);
  if (err.isReal ())
    err.display ();
  pres.setLineColor (1, red, green, blue);

  // Plot color 3
  err = config->getValue ("ui-graph-color-line3-r", 0, red);
  if (err.isReal ())
    err.display ();
  err = config->getValue ("ui-graph-color-line3-g", 255, green);
  if (err.isReal ())
    err.display ();
  err = config->getValue ("ui-graph-color-line3-b", 0, blue);
  if (err.isReal ())
    err.display ();
  pres.setLineColor (2, red, green, blue);

  // Plot color for removed points
  err = config->getValue ("ui-graph-color-remove-r", 0, red);
  if (err.isReal ())
    err.display ();
  err = config->getValue ("ui-graph-color-remove-g", 255, green);
  if (err.isReal ())
    err.display ();
  err = config->getValue ("ui-graph-color-remove-b", 0, blue);
  if (err.isReal ())
    err.display ();
  pres.setRemoveColor (red, green, blue);

  // Error color
  err = config->getValue ("ui-graph-color-error-r", 127, red);
  if (err.isReal ())
    err.display ();
  err = config->getValue ("ui-graph-color-error-g", 127, green);
  if (err.isReal ())
    err.display ();
  err = config->getValue ("ui-graph-color-error-b", 127, blue);
  if (err.isReal ())
    err.display ();
  pres.setErrorColor (red, green, blue);

  // Ls color
  err = config->getValue ("ui-graph-color-ls-r", 127, red);
  if (err.isReal ())
    err.display ();
  err = config->getValue ("ui-graph-color-ls-g", 127, green);
  if (err.isReal ())
    err.display ();
  err = config->getValue ("ui-graph-color-ls-b", 127, blue);
  if (err.isReal ())
    err.display ();
  pres.setLsColor (red, green, blue);

  // Font color
  err = config->getValue ("ui-graph-color-font-r", 0, red);
  if (err.isReal ())
    err.display ();
  err = config->getValue ("ui-graph-color-font-g", 0, green);
  if (err.isReal ())
    err.display ();
  err = config->getValue ("ui-graph-color-font-b", 0, blue);
  if (err.isReal ())
    err.display ();
  pres.setFontColor (red, green, blue);

  // Grid color
  err = config->getValue ("ui-graph-color-grid-r", 127, red);
  if (err.isReal ())
    err.display ();
  err = config->getValue ("ui-graph-color-grid-g", 127, green);
  if (err.isReal ())
    err.display ();
  err = config->getValue ("ui-graph-color-grid-b", 127, blue);
  if (err.isReal ())
    err.display ();
  pres.setGridColor (red, green, blue);

  // Decide which window to draw
  pres.setDisplayWindow (windowTarget);

  // Create the bitmap
  err =
    pres.createPNG (cfname, horizScale, vertScale, xminval, yminval, yrange);
  if (err.isReal ())
    {
      err.display ();
      m_plotfailed = true;
      return;
    }
}

bool cepPlot::getFailed ()
{
  return m_plotfailed;
}
