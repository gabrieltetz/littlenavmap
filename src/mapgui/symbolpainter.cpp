#include "mapquery.h"
#include "symbolpainter.h"
#include "common/mapcolors.h"

#include <QPainter>

SymbolPainter::SymbolPainter()
{
}

void SymbolPainter::drawAirportSymbol(QPainter *painter, const MapAirport& ap, int x, int y, int size,
                                      bool isAirportDiagram, bool fast)
{
  painter->save();
  QColor apColor = mapcolors::colorForAirport(ap);

  int radius = size / 2;
  painter->setBackgroundMode(Qt::OpaqueMode);

  if(ap.flags.testFlag(HARD) && !ap.flags.testFlag(MIL) && !ap.flags.testFlag(CLOSED))
    // Use filled circle
    painter->setBrush(QBrush(apColor));
  else
    // Use white filled circle
    painter->setBrush(QBrush(mapcolors::airportSymbolFillColor));

  if(!fast || isAirportDiagram)
    if(ap.flags.testFlag(FUEL) && !ap.flags.testFlag(MIL) && !ap.flags.testFlag(CLOSED) && size > 6)
    {
      // Draw fuel spikes
      int fuelRadius = static_cast<int>(std::round(static_cast<double>(radius) * 1.4));
      if(fuelRadius < radius + 2)
        fuelRadius = radius + 2;
      painter->setPen(QPen(QBrush(apColor), size / 4, Qt::SolidLine, Qt::FlatCap));
      painter->drawLine(x, y - fuelRadius, x, y + fuelRadius);
      painter->drawLine(x - fuelRadius, y, x + fuelRadius, y);
    }

  painter->setPen(QPen(QBrush(apColor), size / 5, Qt::SolidLine, Qt::FlatCap));
  painter->drawEllipse(QPoint(x, y), radius, radius);

  if(!fast || isAirportDiagram)
  {
    if(ap.flags.testFlag(MIL))
      painter->drawEllipse(QPoint(x, y), radius / 2, radius / 2);

    if(ap.waterOnly() && size > 6)
    {
      int lineWidth = size / 7;
      painter->setPen(QPen(QBrush(apColor), lineWidth, Qt::SolidLine, Qt::FlatCap));
      painter->drawLine(x - lineWidth / 4, y - radius / 2, x - lineWidth / 4, y + radius / 2);
      painter->drawArc(x - radius / 2, y - radius / 2, radius, radius, 0 * 16, -180 * 16);
    }

    if(ap.isHeliport() && size > 6)
    {
      int lineWidth = size / 7;
      painter->setPen(QPen(QBrush(apColor), lineWidth, Qt::SolidLine, Qt::FlatCap));
      painter->drawLine(x - lineWidth / 4 - size / 5, y - radius / 2,
                        x - lineWidth / 4 - size / 5, y + radius / 2);

      painter->drawLine(x - lineWidth / 4 - size / 5, y,
                        x - lineWidth / 4 + size / 5, y);

      painter->drawLine(x - lineWidth / 4 + size / 5, y - radius / 2,
                        x - lineWidth / 4 + size / 5, y + radius / 2);
    }

    if(ap.flags.testFlag(CLOSED) && size > 6)
    {
      // Cross out whatever was painted before
      painter->setPen(QPen(QBrush(apColor), size / 7, Qt::SolidLine, Qt::FlatCap));
      painter->drawLine(x - radius, y - radius, x + radius, y + radius);
      painter->drawLine(x - radius, y + radius, x + radius, y - radius);
    }
  }

  if(!fast || isAirportDiagram)
    if(ap.flags.testFlag(HARD) && !ap.flags.testFlag(MIL) && !ap.flags.testFlag(CLOSED) && size > 6)
    {
      // Draw line inside circle
      painter->translate(x, y);
      painter->rotate(ap.longestRunwayHeading);
      painter->setPen(QPen(QBrush(mapcolors::airportSymbolFillColor), size / 5, Qt::SolidLine, Qt::RoundCap));
      painter->drawLine(0, -radius + 2, 0, radius - 2);
      painter->resetTransform();
    }

  painter->restore();
}

void SymbolPainter::drawWaypointSymbol(QPainter *painter, const MapWaypoint& wp, int x, int y, int size,
                                       bool fast)
{
  Q_UNUSED(wp);
  painter->save();
  painter->setBrush(Qt::NoBrush);

  painter->setPen(QPen(mapcolors::waypointSymbolColor, 1.5, Qt::SolidLine, Qt::SquareCap));
  painter->setBackgroundMode(Qt::TransparentMode);

  if(!fast)
  {
    int radius = size / 2;
    QPolygon polygon;
    polygon << QPoint(x, y - radius)
            << QPoint(x + radius, y + radius)
            << QPoint(x - radius, y + radius);

    painter->drawConvexPolygon(polygon);
  }
  else
    painter->drawPoint(x, y);

  painter->restore();
}

void SymbolPainter::drawVorSymbol(QPainter *painter, const MapVor& vor, int x, int y, int size, bool fast,
                                  int largeSize)
{
  painter->save();
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(mapcolors::vorSymbolColor, 1.5, Qt::SolidLine, Qt::SquareCap));
  painter->setBackgroundMode(Qt::TransparentMode);

  if(!fast)
  {
    painter->translate(x, y);

    if(largeSize > 0 && !vor.dmeOnly)
      painter->rotate(-vor.magvar);

    int radius = size / 2;
    if(vor.hasDme)
      painter->drawRect(-size / 2, -size / 2, size, size);
    if(!vor.dmeOnly)
    {
      int corner = 2;
      QPolygon polygon;
      polygon << QPoint(-radius / corner, -radius)
              << QPoint(radius / corner, -radius)
              << QPoint(radius, 0)
              << QPoint(radius / corner, radius)
              << QPoint(-radius / corner, radius)
              << QPoint(-radius, 0);

      painter->drawConvexPolygon(polygon);
    }

    if(largeSize > 0 && !vor.dmeOnly)
    {
      painter->setPen(QPen(mapcolors::vorSymbolColor, 1, Qt::SolidLine, Qt::SquareCap));
      painter->drawEllipse(QPoint(0, 0), radius * 5, radius * 5);

      for(int i = 0; i < 360; i += 10)
      {
        if(i == 0)
          painter->drawLine(0, 0, 0, -radius * 5);
        else if((i % 90) == 0)
          painter->drawLine(0, static_cast<int>(-radius * 4), 0, -radius * 5);
        else
          painter->drawLine(0, static_cast<int>(-radius * 4.5), 0, -radius * 5);
        painter->rotate(10);
      }
    }
    painter->resetTransform();

  }

  if(size > 14)
    painter->setPen(QPen(mapcolors::vorSymbolColor, size / 4, Qt::SolidLine, Qt::RoundCap));
  else
    painter->setPen(QPen(mapcolors::vorSymbolColor, size / 3, Qt::SolidLine, Qt::RoundCap));
  painter->drawPoint(x, y);

  painter->drawPoint(x, y);
  painter->restore();
}

void SymbolPainter::drawNdbSymbol(QPainter *painter, const MapNdb& ndb, int x, int y, int size, bool fast)
{
  Q_UNUSED(ndb);
  painter->save();
  int radius = size / 2;

  painter->setBackgroundMode(Qt::TransparentMode);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(mapcolors::ndbSymbolColor, 1.5, size > 12 ? Qt::DotLine : Qt::SolidLine, Qt::RoundCap));

  if(!fast)
    // Draw outer dotted circle
    painter->drawEllipse(QPoint(x, y), radius, radius);

  if(size > 12)
  {
    if(!fast)
      // If big enought draw inner dotted circle
      painter->drawEllipse(QPoint(x, y), radius * 2 / 3, radius * 2 / 3);
    painter->setPen(QPen(mapcolors::ndbSymbolColor, size / 4, Qt::SolidLine, Qt::RoundCap));
  }
  else
    painter->setPen(QPen(mapcolors::ndbSymbolColor, size / 3, Qt::SolidLine, Qt::RoundCap));

  painter->drawPoint(x, y);

  painter->restore();
}

void SymbolPainter::drawMarkerSymbol(QPainter *painter, const MapMarker& marker, int x, int y, int size,
                                     bool fast)
{
  Q_UNUSED(marker);
  painter->save();
  int radius = size / 2;

  painter->setBackgroundMode(Qt::TransparentMode);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(mapcolors::markerSymbolColor, 1.5, Qt::SolidLine, Qt::RoundCap));

  if(!fast)
  {
    // Draw rotated lens / ellipse
    painter->translate(x, y);
    painter->rotate(marker.heading);
    painter->drawEllipse(QPoint(0, 0), radius, radius / 2);
    painter->resetTransform();
  }

  painter->setPen(QPen(mapcolors::markerSymbolColor, size / 4, Qt::SolidLine, Qt::RoundCap));

  painter->drawPoint(x, y);

  painter->restore();
}
