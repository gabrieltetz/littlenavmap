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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <marble/ElevationModel.h>

#include "geo/pos.h"
#include "sql/sqldatabase.h"
#include "marble/MarbleGlobal.h"
#include "common/maptypes.h"

class QElapsedTimer;
class Controller;
class ColumnList;
class SearchController;
class RouteController;
class QComboBox;
class QLabel;
class Search;
class DatabaseLoader;
class WeatherReporter;

namespace Marble {
class LegendWidget;
class MarbleAboutDialog;
}

namespace atools {
namespace gui {
class Dialog;
class ErrorHandler;
class HelpHandler;
}

}

namespace Ui {
class MainWindow;
}

class MapWidget;
class MapQuery;

class MainWindow :
  public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  void mapContextMenu(const QPoint& pos);

  Ui::MainWindow *getUi() const
  {
    return ui;
  }

  MapWidget *getMapWidget() const
  {
    return navMapWidget;
  }

  RouteController *getRouteController() const
  {
    return routeController;
  }

  void setMessageText(const QString& text = QString(), const QString& tooltipText = QString());

  const Marble::ElevationModel *getElevationModel();

  WeatherReporter *getWeatherReporter() const
  {
    return weatherReporter;
  }

signals:
  /* Emitted when window is shown the first time */
  void windowShown();

private:
  SearchController *searchController;
  RouteController *routeController;

  QComboBox *mapThemeComboBox = nullptr, *mapProjectionComboBox = nullptr;
  int mapDetailFactor;
  /* Work on the close event that also catches clicking the close button
   * in the window frame */
  virtual void closeEvent(QCloseEvent *event) override;

  /* Emit a signal windowShown after first appearance */
  virtual void showEvent(QShowEvent *event) override;

  Ui::MainWindow *ui;
  MapWidget *navMapWidget = nullptr;
  QLabel *mapDistanceLabel, *mapPosLabel, *renderStatusLabel, *detailLabel, *messageLabel;

  bool hasDatabaseLoadStatus = false;

  Marble::LegendWidget *legendWidget = nullptr;
  Marble::MarbleAboutDialog *marbleAbout = nullptr;
  atools::gui::Dialog *dialog = nullptr;
  atools::gui::ErrorHandler *errorHandler = nullptr;
  atools::gui::HelpHandler *helpHandler = nullptr;
  DatabaseLoader *databaseLoader = nullptr;
  WeatherReporter *weatherReporter = nullptr;

  void openDatabase();
  void closeDatabase();

  atools::sql::SqlDatabase db;
  MapQuery *mapQuery;
  QString databaseFile;
  void connectAllSlots();
  void mainWindowShown();

  bool firstStart = true;
  void writeSettings();
  void readSettings();
  void updateActionStates();
  void setupUi();

  void createNavMap();
  void options();
  void preDatabaseLoad();
  void postDatabaseLoad();
  void createEmptySchema();

  void updateHistActions(int minIndex, int curIndex, int maxIndex);

  void updateMapShowFeatures();

  void increaseMapDetail();
  void decreaseMapDetail();
  void defaultMapDetail();
  void setMapDetail(int factor);
  void selectionChanged(const Search *source, int selected, int visible, int total);
  void routeSelectionChanged(int selected, int total);

  void routeNew();
  void routeOpen();
  void routeSave();
  bool routeSaveAs();
  void routeCenter();
  void renderStatusChanged(Marble::RenderStatus status);
  void resultTruncated(maptypes::MapObjectTypes type, int truncatedTo);
  bool routeCheckForChanges();
  void routeCheckForStartAndDest();

};

#endif // MAINWINDOW_H
