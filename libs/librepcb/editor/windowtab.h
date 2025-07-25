/*
 * LibrePCB - Professional EDA for everyone!
 * Copyright (C) 2013 LibrePCB Developers, see AUTHORS.md for contributors.
 * https://librepcb.org/
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
 */

#ifndef LIBREPCB_EDITOR_WINDOWTAB_H
#define LIBREPCB_EDITOR_WINDOWTAB_H

/*******************************************************************************
 *  Includes
 ******************************************************************************/
#include "appwindow.h"

#include <librepcb/core/utils/signalslot.h>

#include <QtCore>

/*******************************************************************************
 *  Namespace / Forward Declarations
 ******************************************************************************/
namespace librepcb {

class LengthUnit;
class Point;

namespace editor {

class GuiApplication;

/*******************************************************************************
 *  Class WindowTab
 ******************************************************************************/

/**
 * @brief The WindowTab class
 */
class WindowTab : public QObject {
  Q_OBJECT

public:
  // Signals
  Signal<WindowTab> onUiDataChanged;

  // Constructors / Destructor
  WindowTab() = delete;
  WindowTab(const WindowTab& other) = delete;
  explicit WindowTab(GuiApplication& app, QObject* parent = nullptr) noexcept;
  virtual ~WindowTab() noexcept;

  // General Methods
  virtual ui::TabData getUiData() const noexcept = 0;
  virtual void setUiData(const ui::TabData& data) noexcept;
  virtual void activate() noexcept {}
  virtual void deactivate() noexcept {}
  virtual void trigger(ui::TabAction a) noexcept;
  virtual slint::Image renderScene(float width, float height,
                                   int scene) noexcept;
  virtual bool processScenePointerEvent(
      const QPointF& pos, slint::private_api::PointerEvent e) noexcept;
  virtual bool processSceneScrolled(
      const QPointF& pos, slint::private_api::PointerScrollEvent e) noexcept;
  virtual bool processSceneKeyEvent(
      const slint::private_api::KeyEvent& e) noexcept;

  /**
   * @brief Request to close the tab
   *
   * If there are unsaved changes to the tab, this method will ask the user
   * whether the changes should be saved or not. If the user clicks on "cancel"
   * or the changes could not be saved successfully, this method will return
   * false. If there were no unsaved changes or they were successfully saved,
   * the method returns true.
   *
   * @retval true   Tab is safe to be closed.
   * @retval false  Tab still has unsaved changes.
   */
  virtual bool requestClose() noexcept { return true; }

  // Operator Overloadings
  WindowTab& operator=(const WindowTab& rhs) = delete;

signals:
  void panelPageRequested(ui::PanelPage p);
  void closeRequested();
  void closeEnforced();
  void statusBarMessageChanged(const QString& message, int timeoutMs);
  void cursorCoordinatesChanged(const Point& pos, const LengthUnit& unit);

protected:
  GuiApplication& mApp;
};

/*******************************************************************************
 *  End of File
 ******************************************************************************/

}  // namespace editor
}  // namespace librepcb

#endif
