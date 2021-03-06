// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#ifndef SCANTAILOR_SELECT_CONTENT_OPTIONSWIDGET_H_
#define SCANTAILOR_SELECT_CONTENT_OPTIONSWIDGET_H_

#include <AutoManualMode.h>
#include <UnitsListener.h>
#include <core/ConnectionManager.h>

#include <QRectF>
#include <QSizeF>
#include <list>
#include <memory>

#include "Dependencies.h"
#include "FilterOptionsWidget.h"
#include "PageId.h"
#include "PageSelectionAccessor.h"
#include "Params.h"
#include "PhysSizeCalc.h"
#include "ui_OptionsWidget.h"

namespace select_content {
class Settings;

class OptionsWidget : public FilterOptionsWidget, public UnitsListener, private Ui::OptionsWidget {
  Q_OBJECT
 public:
  class UiData {
    // Member-wise copying is OK.
   public:
    UiData();

    ~UiData();

    void setSizeCalc(const PhysSizeCalc& calc);

    void setContentRect(const QRectF& contentRect);

    void setPageRect(const QRectF& contentRect);

    const QRectF& contentRect() const;

    const QRectF& pageRect() const;

    QSizeF contentSizeMM() const;

    void setDependencies(const Dependencies& deps);

    const Dependencies& dependencies() const;

    void setContentDetectionMode(AutoManualMode mode);

    void setPageDetectionMode(AutoManualMode mode);

    bool isFineTuningCornersEnabled() const;

    void setFineTuneCornersEnabled(bool fineTune);

    AutoManualMode contentDetectionMode() const;

    AutoManualMode pageDetectionMode() const;

   private:
    QRectF m_contentRect;  // In virtual image coordinates.
    QRectF m_pageRect;
    PhysSizeCalc m_sizeCalc;
    Dependencies m_deps;
    AutoManualMode m_contentDetectionMode;
    AutoManualMode m_pageDetectionMode;
    bool m_fineTuneCornersEnabled;
  };


  OptionsWidget(std::shared_ptr<Settings> settings, const PageSelectionAccessor& pageSelectionAccessor);

  ~OptionsWidget() override;

  void preUpdateUI(const PageInfo& pageInfo);

  void postUpdateUI(const UiData& uiData);

  void onUnitsChanged(Units units) override;

 public slots:

  void manualContentRectSet(const QRectF& contentRect);

  void manualPageRectSet(const QRectF& pageRect);

  void updatePageRectSize(const QSizeF& size);

 signals:

  void pageRectChangedLocally(const QRectF& pageRect);

  void pageRectStateChanged(bool state);

 private slots:

  void showApplyToDialog();

  void applySelection(const std::set<PageId>& pages, bool applyContentBox, bool applyPageBox);

  void contentDetectToggled(AutoManualMode mode);

  void pageDetectToggled(AutoManualMode mode);

  void fineTuningChanged(bool checked);

  void dimensionsChangedLocally(double);

 private:
  void updateContentModeIndication(AutoManualMode mode);

  void updatePageModeIndication(AutoManualMode mode);

  void updatePageDetectOptionsDisplay();

  void commitCurrentParams();

  void updateDependenciesIfNecessary();

  void setupUiConnections();

  std::shared_ptr<Settings> m_settings;
  UiData m_uiData;
  PageSelectionAccessor m_pageSelectionAccessor;
  PageId m_pageId;
  Dpi m_dpi;

  ConnectionManager m_connectionManager;
};


inline void OptionsWidget::UiData::setSizeCalc(const PhysSizeCalc& calc) {
  m_sizeCalc = calc;
}

inline void OptionsWidget::UiData::setContentRect(const QRectF& contentRect) {
  m_contentRect = contentRect;
}

inline const QRectF& OptionsWidget::UiData::contentRect() const {
  return m_contentRect;
}

inline void OptionsWidget::UiData::setPageRect(const QRectF& pageRect) {
  m_pageRect = pageRect;
}

inline const QRectF& OptionsWidget::UiData::pageRect() const {
  return m_pageRect;
}

inline QSizeF OptionsWidget::UiData::contentSizeMM() const {
  return m_sizeCalc.sizeMM(m_contentRect);
}

inline void OptionsWidget::UiData::setDependencies(const Dependencies& deps) {
  m_deps = deps;
}

inline const Dependencies& OptionsWidget::UiData::dependencies() const {
  return m_deps;
}

inline void OptionsWidget::UiData::setContentDetectionMode(AutoManualMode mode) {
  m_contentDetectionMode = mode;
}

inline AutoManualMode OptionsWidget::UiData::contentDetectionMode() const {
  return m_contentDetectionMode;
}

inline void OptionsWidget::UiData::setPageDetectionMode(AutoManualMode mode) {
  m_pageDetectionMode = mode;
}

inline AutoManualMode OptionsWidget::UiData::pageDetectionMode() const {
  return m_pageDetectionMode;
}
inline void OptionsWidget::UiData::setFineTuneCornersEnabled(bool fineTune) {
  m_fineTuneCornersEnabled = fineTune;
}

inline bool OptionsWidget::UiData::isFineTuningCornersEnabled() const {
  return m_fineTuneCornersEnabled;
}
}  // namespace select_content
#endif  // ifndef SCANTAILOR_SELECT_CONTENT_OPTIONSWIDGET_H_
