#include "objectmodel.h"
#include "verbosity.h"
#include "utility.h"

raster::raster (object & obj)
{
  // todo_mikal: make sure this is an image
  // Type = XObject
  // SubType = Image
  // Name = <the name>

  // Decoding hints
  dictionary decodeParms;
  if (obj.getDict ().getValue ("DecodeParms", decodeParms))
    {
      if (!decodeParms.getValue ("K", m_k))
	{
	  debug(dlTrace, 
		"Image doesn't specify a K value for the decode hints");
	}
    }
  else
    {
      debug(dlTrace, "Image does not define any decoding hints");
    }

  // Width
  if (!obj.getDict ().getValue ("Width", m_width))
    {
      debug(dlTrace, "Image does not specify a width");
    }

  // Height
  if (!obj.getDict ().getValue ("Height", m_height))
    {
      debug(dlTrace, "Image does not specify a height");
    }

  // Bits per sample
  if (!obj.getDict ().getValue ("BitsPerComponent", m_bps))
    {
      debug(dlTrace, "Image does not specify a number of bits per sample");
    }
  else
    {
      // TODO mikal: this is an unacceptable temporary hack
      if(m_bps == 1) m_spp = 1;
      else m_spp = 3;
    }
}

int
raster::getWidth()
{
  return m_width;
}

int
raster::getHeight()
{
  return m_height;
}

int
raster::getK()
{
  return m_k;
}

int
raster::getBitsPerSample()
{
  return m_bps;
}

int raster::getSamplesPerPixel()
{
  return m_spp;
}
