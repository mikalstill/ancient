
/* 
 *   A wrapper for the CEP data presentation
 *   Copyright (C) Michael Still                    2002
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

cepPlot::cepPlot(cepDataset *theDataset, cepDataset::direction dir, string cfname, long presHeight):
  m_plotfailed(false)
{
  cepConfiguration *config;
  config = (cepConfiguration *) &cepConfiguration::getInstance();

  cepPresentation pres (theDataset->getData (dir).size () + 20,
			presHeight);
  
  // Add the datapoints
  for (unsigned int i = 0; i < theDataset->getData (dir).size (); i++)
    {
      pres.addDataPoint (i,
			 (long)(theDataset->getData (dir)[i].
				sample * 10000),
			 (long)(theDataset->getData (dir)[i].
				error * 10000));
      cepDebugPrint ("Data point: " +
                     cepToString ((long)
				  (theDataset->getData (dir)[i].
				   sample * 10000)));
    }
  
  cepError err;
  int red = 0, green = 0, blue = 0;
  bool toggle;
  
  // Averages
  err = config->getValue("ui-viewmenu-showaverages", false, toggle);
  if(err.isReal()) err.display();
  pres.useAverage(toggle);
  
  // Errors
  err = config->getValue("ui-viewmenu-showerrors", true, toggle);
  if(err.isReal()) err.display();    
  pres.useErrors(toggle);
  
  // Axes
  err = config->getValue("ui-graph-color-axis-r", 255, red);
  if(err.isReal()) err.display();
  err = config->getValue("ui-graph-color-axis-g", 0, green);
  if(err.isReal()) err.display();
  err = config->getValue("ui-graph-color-axis-b", 0, blue);
  if(err.isReal()) err.display();
  pres.setAxesColor(red, green, blue);
  
  // Plot color
  err = config->getValue("ui-graph-color-line-r", 0, red);
  if(err.isReal()) err.display();
  err = config->getValue("ui-graph-color-line-g", 255, green);
  if(err.isReal()) err.display();
  err = config->getValue("ui-graph-color-line-b", 0, blue);
  if(err.isReal()) err.display();
  pres.setLineColor(red, green, blue);

  // Average color
  err = config->getValue("ui-graph-color-average-r", 0, red);
  if(err.isReal()) err.display();
  err = config->getValue("ui-graph-color-average-g", 0, green);
  if(err.isReal()) err.display();
  err = config->getValue("ui-graph-color-average-b", 255, blue);
  if(err.isReal()) err.display();
  pres.setAverageColor(red, green, blue);
  
  // Error color
  err = config->getValue("ui-graph-color-error-r", 127, red);
  if(err.isReal()) err.display();
  err = config->getValue("ui-graph-color-error-g", 127, green);
  if(err.isReal()) err.display();
  err = config->getValue("ui-graph-color-error-b", 127, blue);
  if(err.isReal()) err.display();
  pres.setErrorColor(red, green, blue);
  
  // Which view?
  int currentView;
  err = config->getValue("ui-viewmenu-currentview", 
			   (int) cepPresentation::viewCentered, currentView);
  if(err.isReal()) err.display();
  pres.setView((cepPresentation::view) currentView);
  
  // Various textual labels
  pres.yAxisTitle("This is a foo");

  // Create the bitmap
  err = pres.createPNG (cfname);
  if (err.isReal ()){
    err.display ();
    m_plotfailed = true;
    return;
  }
}

bool cepPlot::getFailed()
{
  return m_plotfailed;
}
