#include "objectmodel.h"
#include "verbosity.h"
#include "utility.h"

raster::raster (object & obj)
{
  // todo_mikal: make sure this is an image
  // Type = XObject
  // SubType = Image
  // Name = <the name>

  if (!obj.getDict ().getValue ("BitsPerComponent", m_bpc))
    {
      debug(dlTrace, "Image doesn't specify a sample size");
    }

  if (!obj.getDict ().getValue ("ColorSpace", m_cs))
    {
      debug(dlTrace, "Image doesn't specify a color space");
    }

  dictionary decodeParms;
  if (obj.getDict ().getValue ("DecodeParms", decodeParms))
    {
      if (!decodeParms.getValue ("K", m_k))
	{
	  debug(dlTrace, "Image doesn't specify a K value for the decode hints");
	}
    }
  else
    {
      debug(dlTrace, "Image does not define any decoding hints");
    }

  if (!obj.getDict ().getValue ("Width", m_width))
    {
      debug(dlTrace, "Image does not specify a width");
    }

  if (!obj.getDict ().getValue ("Height", m_height))
    {
      debug(dlTrace, "Image does not specify a height");
    }
}

void
raster::setData (char *data)
{
  m_data = data;
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
