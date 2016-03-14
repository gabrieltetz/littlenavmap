/*****************************************************************************
* Copyright 2015-2016 Alexander Barthel albar965@mailbox.org
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef MAPPAINTER_H
#define MAPPAINTER_H

#include "coordinateconverter.h"
#include "common/maptypes.h"
#include <marble/MarbleWidget.h>

#include <QPen>

namespace atools {
namespace geo {
class Pos;
}
}

class MapLayer;
class MapQuery;
class MapScale;

class MapPainter :
  public CoordinateConverter
{
public:
  MapPainter(Marble::MarbleWidget *marbleWidget, MapQuery *mapQuery, MapScale *mapScale, bool verboseMsg);
  virtual ~MapPainter();

  virtual void paint(const MapLayer *mapLayer, Marble::GeoPainter *painter,
                     Marble::ViewportParams *viewport, maptypes::ObjectTypes objectTypes) = 0;

protected:
  void setRenderHints(Marble::GeoPainter *painter);

  Marble::MarbleWidget *widget;
  MapQuery *query;
  MapScale *scale;
  bool verbose = false;
  void textBox(Marble::GeoPainter *painter, const QStringList& texts, const QPen& textPen, int x, int y,
               bool bold = false, bool italic = false, bool alignRight = false, int transparency = 255);

};

#endif // MAPPAINTER_H
