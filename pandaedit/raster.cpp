#include "objectmodel.h"
#include <stdio.h>

raster::raster (object & obj)
{
  // todo_mikal: make sure this is an image
  // Type = XObject
  // SubType = Image
  // Name = <the name>

  if (!obj.getDict ().getValue ("BitsPerComponent", m_bpc))
    {
      printf ("DEBUG: Image doesn't specify a sample size\n");
    }

  if (!obj.getDict ().getValue ("ColorSpace", m_cs))
    {
      printf ("DEBUG: Image doesn't specify a color space\n");
    }

  dictionary decodeParms;
  if (obj.getDict ().getValue ("DecodeParms", decodeParms))
    {
      if (!decodeParms.getValue ("K", m_k))
	{
	  printf
	    ("DEBUG: Image doesn't specify a K value for the decode hints\n");
	}
    }
  else
    {
      printf ("DEBUG: Image does not define any decoding hints\n");
    }

  if (!obj.getDict ().getValue ("Width", m_width))
    {
      printf ("DEBUG: Image does not specify a width\n");
    }

  if (!obj.getDict ().getValue ("Height", m_height))
    {
      printf ("DEBUG: Image does not specify a height\n");
    }
}

void
raster::setData (char *data)
{
  m_data = data;
}
