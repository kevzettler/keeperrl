/* Copyright (C) 2013-2014 Michal Brzozowski (rusolis@poczta.fm)

   This file is part of KeeperRL.

   KeeperRL is free software; you can redistribute it and/or modify it under the terms of the
   GNU General Public License as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   KeeperRL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
   even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along with this program.
   If not, see http://www.gnu.org/licenses/ . */

#ifndef _WINDOW_VIEW
#define _WINDOW_VIEW

#include "util.h"
#include "view.h"
#include "map_layout.h"
#include "map_gui.h"
#include "minimap_gui.h"
#include "input_queue.h"

class ViewIndex;

/** See view.h for documentation.*/
class WindowView: public View {
  public:
  WindowView(); 
  virtual void initialize() override;
  virtual void reset() override;
  virtual void displaySplash(View::SplashType, bool& ready) override;

  virtual void close() override;

  virtual void addMessage(const string& message) override;
  virtual void addImportantMessage(const string& message) override;
  virtual void clearMessages() override;
  virtual void retireMessages() override;
  virtual void refreshView(const CreatureView*) override;
  virtual void updateView(const CreatureView*) override;
  virtual void drawLevelMap(const CreatureView*) override;
  virtual void resetCenter() override;
  virtual Optional<int> chooseFromList(const string& title, const vector<ListElem>& options, int index = 0,
      MenuType = View::NORMAL_MENU, double* scrollPos = nullptr,
      Optional<UserInput::Type> exitAction = Nothing()) override;
  virtual Optional<Vec2> chooseDirection(const string& message) override;
  virtual bool yesOrNoPrompt(const string& message) override;
  virtual void animateObject(vector<Vec2> trajectory, ViewObject object) override;
  virtual void animation(Vec2 pos, AnimationId) override;

  virtual void presentText(const string& title, const string& text) override;
  virtual void presentList(const string& title, const vector<ListElem>& options, bool scrollDown = false,
      Optional<UserInput::Type> exitAction = Nothing()) override;
  virtual Optional<int> getNumber(const string& title, int min, int max, int increments = 1) override;

  virtual UserInput getAction() override;
  virtual bool travelInterrupt() override;
  virtual int getTimeMilli() override;
  virtual void setTimeMilli(int) override;
  virtual void stopClock() override;
  virtual bool isClockStopped() override;
  virtual void continueClock() override;
  
  static Color getFireColor();

  private:

  void updateMinimap(const CreatureView*);
  Rectangle getMenuPosition(View::MenuType type);
  Optional<int> chooseFromListInternal(const string& title, const vector<ListElem>& options, int index, MenuType,
      double* scrollPos, Optional<UserInput::Type> exitAction, Optional<sf::Event::KeyEvent> exitKey,
      vector<sf::Event::KeyEvent> shortCuts);
  Optional<UserInput::Type> getSimpleInput(sf::Event::KeyEvent key);
  void refreshViewInt(const CreatureView*, bool flipBuffer = true);
  void rebuildGui();
  void drawMap();
  PGuiElem getSunlightInfoGui(GameInfo::SunlightInfo& sunlightInfo);
  PGuiElem getTurnInfoGui(int turn);
  PGuiElem drawBottomPlayerInfo(GameInfo::PlayerInfo&, GameInfo::SunlightInfo&);
  PGuiElem drawRightPlayerInfo(GameInfo::PlayerInfo&);
  PGuiElem drawPlayerStats(GameInfo::PlayerInfo&);
  PGuiElem drawBottomBandInfo(GameInfo::BandInfo& info, GameInfo::SunlightInfo&);
  PGuiElem drawRightBandInfo(GameInfo::BandInfo& info, GameInfo::VillageInfo&);
  PGuiElem drawBuildings(GameInfo::BandInfo& info);
  PGuiElem drawTechnology(GameInfo::BandInfo& info);
  PGuiElem drawWorkshop(GameInfo::BandInfo& info);
  PGuiElem drawVillages(GameInfo::VillageInfo& info);
  PGuiElem drawMinions(GameInfo::BandInfo& info);
  PGuiElem drawMinionWindow(GameInfo::BandInfo& info);
  PGuiElem drawKeeperHelp();
  Optional<sf::Event::KeyEvent> getEventFromMenu();
  void propagateEvent(const Event& event, vector<GuiElem*>);
  void keyboardAction(Event::KeyEvent key);

  void showMessage(const string& message);
  PGuiElem drawListGui(const string& title, const vector<ListElem>& options, int& height,
      int* highlight = nullptr, int* choice = nullptr);
  void drawList(const string& title, const vector<ListElem>& options, int hightlight, int setMousePos = -1);
  void refreshScreen(bool flipBuffer = true);
  void refreshText();
  void drawAndClearBuffer();
  Optional<Vec2> getHighlightedTile();

  Optional<ViewObject> drawObjectAbs(int x, int y, const ViewIndex&, int sizeX, int sizeY, Vec2 tilePos);
  void darkenObjectAbs(int x, int y);
  void clearMessageBox();
  void switchZoom();
  void zoom(bool out);
  void switchTiles();
  void resize(int width, int height, vector<GuiElem*> gui);
  Rectangle getMapGuiBounds() const;
  Rectangle getMinimapBounds() const;
  void resetMapBounds();

  bool considerScrollEvent(sf::Event&);
  bool considerResizeEvent(sf::Event&, vector<GuiElem*> gui);

  int messageInd = 0;
  const static unsigned int maxMsgLength = 90;
  std::deque<string> currentMessage = std::deque<string>(3, "");
  bool oldMessage = false;

  enum class CollectiveTab {
    BUILDINGS,
    MINIONS,
    TECHNOLOGY,
    WORKSHOP,
    VILLAGES,
    KEY_MAPPING,
  };
  
  vector<PGuiElem> drawButtons(vector<GameInfo::BandInfo::Button> buttons, int& active, CollectiveTab);
  void setCollectiveTab(CollectiveTab t);
  void setOptionsMenu(const string& title, const vector<GameInfo::BandInfo::Button::Option>&);
  CollectiveTab collectiveTab = CollectiveTab::BUILDINGS;

  enum class LegendOption {
    STATS,
    OBJECTS,
  };
  
  LegendOption legendOption = LegendOption::STATS;

  Table<Optional<ViewIndex>> objects;

  MapLayout* mapLayout;
  MapGui* mapGui;
  MinimapGui* minimapGui;
  PGuiElem mapDecoration;
  PGuiElem minimapDecoration;
  vector<PGuiElem> tempGuiElems;
  vector<GuiElem*> getAllGuiElems();
  vector<GuiElem*> getClickableGuiElems();
  InputQueue inputQueue;

  bool gameReady = false;

  struct TileLayouts {
    MapLayout normalLayout;
    MapLayout unzoomLayout;
    bool sprites;
  };
  TileLayouts asciiLayouts;
  TileLayouts spriteLayouts;
  TileLayouts currentTileLayout;

  int activeBuilding = 0;
  int activeWorkshop = -1;
  int activeLibrary = -1;
  int activeOption = 0;
  string chosenCreature;

  function<void()> getButtonCallback(UserInput);

  Vec2 lastMousePos;
  struct {
    double x;
    double y;
  } mouseOffset, center;
};


#endif
