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

#ifndef LIBREPCB_EDITOR_CATEGORYTREEMODELLEGACY_H
#define LIBREPCB_EDITOR_CATEGORYTREEMODELLEGACY_H

/*******************************************************************************
 *  Includes
 ******************************************************************************/
#include <librepcb/core/types/uuid.h>

#include <QtCore>

#include <memory>

/*******************************************************************************
 *  Namespace / Forward Declarations
 ******************************************************************************/
namespace librepcb {

class WorkspaceLibraryDb;

namespace editor {

/*******************************************************************************
 *  Class CategoryTreeModelLegacy
 ******************************************************************************/

/**
 * @brief The CategoryTreeModelLegacy class
 */
class CategoryTreeModelLegacy final : public QAbstractItemModel {
  struct Item {
    std::weak_ptr<Item> parent;  ///< nullptr for root categories
    std::optional<Uuid> uuid;  ///< std::nullopt for items without category
    QString text;
    QString tooltip;
    QVector<std::shared_ptr<Item>> childs;
  };

public:
  // Types
  enum class Filter {
    /// Show all component categories, even empty ones
    CmpCat = 1 << 0,
    /// Show component categories containing symbols
    CmpCatWithSymbols = 1 << 1,
    /// Show component categories containing components
    CmpCatWithComponents = 1 << 2,
    /// Show component categories containing devices
    CmpCatWithDevices = 1 << 3,
    /// Show all package categories, even empty ones
    PkgCat = 1 << 4,
    /// Show package categories containing packages
    PkgCatWithPackages = 1 << 5,
  };
  Q_DECLARE_FLAGS(Filters, Filter)

  // Constructors / Destructor
  CategoryTreeModelLegacy() = delete;
  CategoryTreeModelLegacy(const CategoryTreeModelLegacy& other) = delete;
  explicit CategoryTreeModelLegacy(const WorkspaceLibraryDb& library,
                                   const QStringList& localeOrder,
                                   Filters filters) noexcept;
  ~CategoryTreeModelLegacy() noexcept;

  // Setters
  void setLocaleOrder(const QStringList& order) noexcept;

  // Inherited Methods
  int columnCount(
      const QModelIndex& parent = QModelIndex()) const noexcept override;
  int rowCount(
      const QModelIndex& parent = QModelIndex()) const noexcept override;
  QModelIndex index(
      int row, int column,
      const QModelIndex& parent = QModelIndex()) const noexcept override;
  QModelIndex parent(const QModelIndex& index) const noexcept override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const noexcept override;
  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const noexcept override;

  // Operator Overloadings
  CategoryTreeModelLegacy& operator=(const CategoryTreeModelLegacy& rhs) =
      delete;

private:  // Methods
  void update() noexcept;
  QVector<std::shared_ptr<Item>> getChilds(
      std::shared_ptr<Item> parent) const noexcept;
  bool containsItems(const std::optional<Uuid>& uuid) const;
  bool listAll() const noexcept;
  bool listPackageCategories() const noexcept;
  void updateModelItem(
      std::shared_ptr<Item> parentItem,
      const QVector<std::shared_ptr<Item>>& newChilds) noexcept;
  Item* itemFromIndex(const QModelIndex& index) const noexcept;
  QModelIndex indexFromItem(const Item* item) const noexcept;

private:  // Data
  const WorkspaceLibraryDb& mLibrary;
  QStringList mLocaleOrder;
  const Filters mFilters;
  std::shared_ptr<Item> mRootItem;
};

}  // namespace editor
}  // namespace librepcb

Q_DECLARE_OPERATORS_FOR_FLAGS(
    librepcb::editor::CategoryTreeModelLegacy::Filters)

/*******************************************************************************
 *  End of File
 ******************************************************************************/

#endif
