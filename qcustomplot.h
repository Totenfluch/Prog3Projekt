/**
 * @file qcustomplot.h
 *
 */

/**
 * @file qcustomplot.h
 *
 */

/***************************************************************************
**                                                                        **
**  QCustomPlot, an easy to use, modern plotting widget for Qt            **
**  Copyright (C) 2011-2015 Emanuel Eichhammer                            **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Emanuel Eichhammer                                   **
**  Website/Contact: http://www.qcustomplot.com/                          **
**             Date: 25.04.15                                             **
**          Version: 1.3.1                                                **
****************************************************************************/

#ifndef QCUSTOMPLOT_H
#define QCUSTOMPLOT_H

#include <QObject>
#include <QPointer>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPixmap>
#include <QVector>
#include <QString>
#include <QDateTime>
#include <QMultiMap>
#include <QFlags>
#include <QDebug>
#include <QVector2D>
#include <QStack>
#include <QCache>
#include <QMargins>
#include <qmath.h>
#include <limits>
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#  include <qnumeric.h>
#  include <QPrinter>
#  include <QPrintEngine>
#else
#  include <QtNumeric>
#  include <QtPrintSupport>
#endif

class QCPPainter;
class QCustomPlot;
class QCPLayerable;
class QCPLayoutElement;
class QCPLayout;
class QCPAxis;
class QCPAxisRect;
class QCPAxisPainterPrivate;
class QCPAbstractPlottable;
class QCPGraph;
class QCPAbstractItem;
class QCPItemPosition;
class QCPLayer;
class QCPPlotTitle;
class QCPLegend;
class QCPAbstractLegendItem;
class QCPColorMap;
class QCPColorScale;
class QCPBars;


/*! \file */


// decl definitions for shared library compilation/usage:
#if defined(QCUSTOMPLOT_COMPILE_LIBRARY)
#  define QCP_LIB_DECL Q_DECL_EXPORT
#elif defined(QCUSTOMPLOT_USE_LIBRARY)
#  define QCP_LIB_DECL Q_DECL_IMPORT
#else
#  define QCP_LIB_DECL
#endif

/*!
  The QCP Namespace contains general enums and QFlags used throughout the QCustomPlot library
*/
namespace QCP
{
/*!
  Defines the sides of a rectangular entity to which margins can be applied.
  
  \see QCPLayoutElement::setAutoMargins, QCPAxisRect::setAutoMargins
*/
enum MarginSide { msLeft     = 0x01 ///< <tt>0x01</tt> left margin
                  ,msRight   = 0x02 ///< <tt>0x02</tt> right margin
                  ,msTop     = 0x04 ///< <tt>0x04</tt> top margin
                  ,msBottom  = 0x08 ///< <tt>0x08</tt> bottom margin
                  ,msAll     = 0xFF ///< <tt>0xFF</tt> all margins
                  ,msNone    = 0x00 ///< <tt>0x00</tt> no margin
                };
/**
 * @brief 
 *
 */
Q_DECLARE_FLAGS(MarginSides, MarginSide)

/*!
  Defines what objects of a plot can be forcibly drawn antialiased/not antialiased. If an object is
  neither forcibly drawn antialiased nor forcibly drawn not antialiased, it is up to the respective
  element how it is drawn. Typically it provides a \a setAntialiased function for this.
  
  \c AntialiasedElements is a flag of or-combined elements of this enum type.
  
  \see QCustomPlot::setAntialiasedElements, QCustomPlot::setNotAntialiasedElements
*/
enum AntialiasedElement { aeAxes           = 0x0001 ///< <tt>0x0001</tt> Axis base line and tick marks
                          ,aeGrid          = 0x0002 ///< <tt>0x0002</tt> Grid lines
                          ,aeSubGrid       = 0x0004 ///< <tt>0x0004</tt> Sub grid lines
                          ,aeLegend        = 0x0008 ///< <tt>0x0008</tt> Legend box
                          ,aeLegendItems   = 0x0010 ///< <tt>0x0010</tt> Legend items
                          ,aePlottables    = 0x0020 ///< <tt>0x0020</tt> Main lines of plottables (excluding error bars, see element \ref aeErrorBars) /**< TODO: describe */
                          ,aeItems         = 0x0040 ///< <tt>0x0040</tt> Main lines of items
                          ,aeScatters      = 0x0080 ///< <tt>0x0080</tt> Scatter symbols of plottables (excluding scatter symbols of type ssPixmap)
                          ,aeErrorBars     = 0x0100 ///< <tt>0x0100</tt> Error bars
                          ,aeFills         = 0x0200 ///< <tt>0x0200</tt> Borders of fills (e.g. under or between graphs)
                          ,aeZeroLine      = 0x0400 ///< <tt>0x0400</tt> Zero-lines, see \ref QCPGrid::setZeroLinePen
                          ,aeAll           = 0xFFFF ///< <tt>0xFFFF</tt> All elements
                          ,aeNone          = 0x0000 ///< <tt>0x0000</tt> No elements /**< TODO: describe */
                        };
/**
 * @brief 
 *
 */
Q_DECLARE_FLAGS(AntialiasedElements, AntialiasedElement)

/*!
  Defines plotting hints that control various aspects of the quality and speed of plotting.
  
  \see QCustomPlot::setPlottingHints
*/
enum PlottingHint { phNone            = 0x000 ///< <tt>0x000</tt> No hints are set
                    ,phFastPolylines  = 0x001 ///< <tt>0x001</tt> Graph/Curve lines are drawn with a faster method. This reduces the quality
                                              ///<                especially of the line segment joins. (Only relevant for solid line pens.)
                    ,phForceRepaint   = 0x002 ///< <tt>0x002</tt> causes an immediate repaint() instead of a soft update() when QCustomPlot::replot() is called with parameter \ref QCustomPlot::rpHint. /**< TODO: describe */
                                              ///<                This is set by default to prevent the plot from freezing on fast consecutive replots (e.g. user drags ranges with mouse).
                    ,phCacheLabels    = 0x004 ///< <tt>0x004</tt> axis (tick) labels will be cached as pixmaps, increasing replot performance.
                  };
/**
 * @brief 
 *
 */
Q_DECLARE_FLAGS(PlottingHints, PlottingHint)

/*!
  Defines the mouse interactions possible with QCustomPlot.
  
  \c Interactions is a flag of or-combined elements of this enum type.
  
  \see QCustomPlot::setInteractions
*/
enum Interaction { iRangeDrag         = 0x001 ///< <tt>0x001</tt> Axis ranges are draggable (see \ref QCPAxisRect::setRangeDrag, \ref QCPAxisRect::setRangeDragAxes)
                   ,iRangeZoom        = 0x002 ///< <tt>0x002</tt> Axis ranges are zoomable with the mouse wheel (see \ref QCPAxisRect::setRangeZoom, \ref QCPAxisRect::setRangeZoomAxes)
                   ,iMultiSelect      = 0x004 ///< <tt>0x004</tt> The user can select multiple objects by holding the modifier set by \ref QCustomPlot::setMultiSelectModifier while clicking
                   ,iSelectPlottables = 0x008 ///< <tt>0x008</tt> Plottables are selectable (e.g. graphs, curves, bars,... see QCPAbstractPlottable)
                   ,iSelectAxes       = 0x010 ///< <tt>0x010</tt> Axes are selectable (or parts of them, see QCPAxis::setSelectableParts)
                   ,iSelectLegend     = 0x020 ///< <tt>0x020</tt> Legends are selectable (or their child items, see QCPLegend::setSelectableParts)
                   ,iSelectItems      = 0x040 ///< <tt>0x040</tt> Items are selectable (Rectangles, Arrows, Textitems, etc. see \ref QCPAbstractItem)
                   ,iSelectOther      = 0x080 ///< <tt>0x080</tt> All other objects are selectable (e.g. your own derived layerables, the plot title,...)
                 };
/**
 * @brief 
 *
 */
Q_DECLARE_FLAGS(Interactions, Interaction)

/*! \internal
  
  Returns whether the specified \a value is considered an invalid data value for plottables (i.e.
  is \e nan or \e +/-inf). This function is used to check data validity upon replots, when the
  compiler flag \c QCUSTOMPLOT_CHECK_DATA is set.
*/
inline bool isInvalidData(double value)
{
  return qIsNaN(value) || qIsInf(value);
}

/*! \internal
  \overload
  
  Checks two arguments instead of one.
*/
inline bool isInvalidData(double value1, double value2)
{
  return isInvalidData(value1) || isInvalidData(value2);
}

/*! \internal
  
  Sets the specified \a side of \a margins to \a value
  
  \see getMarginValue
*/
inline void setMarginValue(QMargins &margins, QCP::MarginSide side, int value)
{
  switch (side)
  {
    case QCP::msLeft: margins.setLeft(value); break;
    case QCP::msRight: margins.setRight(value); break;
    case QCP::msTop: margins.setTop(value); break;
    case QCP::msBottom: margins.setBottom(value); break;
    case QCP::msAll: margins = QMargins(value, value, value, value); break;
    default: break;
  }
}

/*! \internal
  
  Returns the value of the specified \a side of \a margins. If \a side is \ref QCP::msNone or
  \ref QCP::msAll, returns 0.
  
  \see setMarginValue
*/
inline int getMarginValue(const QMargins &margins, QCP::MarginSide side)
{
  switch (side)
  {
    case QCP::msLeft: return margins.left();
    case QCP::msRight: return margins.right();
    case QCP::msTop: return margins.top();
    case QCP::msBottom: return margins.bottom();
    default: break;
  }
  return 0;
}

} // end of namespace QCP

Q_DECLARE_OPERATORS_FOR_FLAGS(QCP::AntialiasedElements)
Q_DECLARE_OPERATORS_FOR_FLAGS(QCP::PlottingHints)
Q_DECLARE_OPERATORS_FOR_FLAGS(QCP::MarginSides)
Q_DECLARE_OPERATORS_FOR_FLAGS(QCP::Interactions)


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPScatterStyle
{
  Q_GADGET
public:
  /*!
    Defines the shape used for scatter points.

    On plottables/items that draw scatters, the sizes of these visualizations (with exception of
    \ref ssDot and \ref ssPixmap) can be controlled with the \ref setSize function. Scatters are
    drawn with the pen and brush specified with \ref setPen and \ref setBrush.
  */
  Q_ENUMS(ScatterShape)
  enum ScatterShape { ssNone       ///< no scatter symbols are drawn (e.g. in QCPGraph, data only represented with lines)
                      ,ssDot       ///< \enumimage{ssDot.png} a single pixel (use \ref ssDisc or \ref ssCircle if you want a round shape with a certain radius)
                      ,ssCross     ///< \enumimage{ssCross.png} a cross
                      ,ssPlus      ///< \enumimage{ssPlus.png} a plus
                      ,ssCircle    ///< \enumimage{ssCircle.png} a circle /**< TODO: describe */
                      ,ssDisc      ///< \enumimage{ssDisc.png} a circle which is filled with the pen's color (not the brush as with ssCircle) /**< TODO: describe */
                      ,ssSquare    ///< \enumimage{ssSquare.png} a square /**< TODO: describe */
                      ,ssDiamond   ///< \enumimage{ssDiamond.png} a diamond /**< TODO: describe */
                      ,ssStar      ///< \enumimage{ssStar.png} a star with eight arms, i.e. a combination of cross and plus /**< TODO: describe */
                      ,ssTriangle  ///< \enumimage{ssTriangle.png} an equilateral triangle, standing on baseline /**< TODO: describe */
                      ,ssTriangleInverted ///< \enumimage{ssTriangleInverted.png} an equilateral triangle, standing on corner
                      ,ssCrossSquare      ///< \enumimage{ssCrossSquare.png} a square with a cross inside
                      ,ssPlusSquare       ///< \enumimage{ssPlusSquare.png} a square with a plus inside /**< TODO: describe */
                      ,ssCrossCircle      ///< \enumimage{ssCrossCircle.png} a circle with a cross inside /**< TODO: describe */
                      ,ssPlusCircle       ///< \enumimage{ssPlusCircle.png} a circle with a plus inside /**< TODO: describe */
                      ,ssPeace     ///< \enumimage{ssPeace.png} a circle, with one vertical and two downward diagonal lines /**< TODO: describe */
                      ,ssPixmap    ///< a custom pixmap specified by \ref setPixmap, centered on the data point coordinates /**< TODO: describe */
                      ,ssCustom    ///< custom painter operations are performed per scatter (As QPainterPath, see \ref setCustomPath) /**< TODO: describe */
                    };

  /**
   * @brief 
   *
   */
  QCPScatterStyle();
  /**
   * @brief 
   *
   * @param shape
   * @param size
   */
  QCPScatterStyle(ScatterShape shape, double size=6);
  /**
   * @brief 
   *
   * @param shape
   * @param color
   * @param size
   */
  QCPScatterStyle(ScatterShape shape, const QColor &color, double size);
  /**
   * @brief 
   *
   * @param shape
   * @param color
   * @param fill
   * @param size
   */
  QCPScatterStyle(ScatterShape shape, const QColor &color, const QColor &fill, double size);
  /**
   * @brief 
   *
   * @param shape
   * @param pen
   * @param brush
   * @param size
   */
  QCPScatterStyle(ScatterShape shape, const QPen &pen, const QBrush &brush, double size);
  /**
   * @brief 
   *
   * @param pixmap
   */
  QCPScatterStyle(const QPixmap &pixmap);
  /**
   * @brief 
   *
   * @param customPath
   * @param pen
   * @param brush
   * @param size
   */
  QCPScatterStyle(const QPainterPath &customPath, const QPen &pen, const QBrush &brush=Qt::NoBrush, double size=6);
  
  // getters:
  /**
   * @brief 
   *
   * @return double
   */
  double size() const { return mSize; }
  /**
   * @brief 
   *
   * @return ScatterShape
   */
  ScatterShape shape() const { return mShape; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen pen() const { return mPen; }
  /**
   * @brief 
   *
   * @return QBrush
   */
  QBrush brush() const { return mBrush; }
  /**
   * @brief 
   *
   * @return QPixmap
   */
  QPixmap pixmap() const { return mPixmap; }
  /**
   * @brief 
   *
   * @return QPainterPath
   */
  QPainterPath customPath() const { return mCustomPath; }

  // setters:
  /**
   * @brief 
   *
   * @param size
   */
  void setSize(double size);
  /**
   * @brief 
   *
   * @param shape
   */
  void setShape(ScatterShape shape);
  /**
   * @brief 
   *
   * @param pen
   */
  void setPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param brush
   */
  void setBrush(const QBrush &brush);
  /**
   * @brief 
   *
   * @param pixmap
   */
  void setPixmap(const QPixmap &pixmap);
  /**
   * @brief 
   *
   * @param customPath
   */
  void setCustomPath(const QPainterPath &customPath);

  // non-property methods:
  /**
   * @brief 
   *
   * @return bool
   */
  bool isNone() const { return mShape == ssNone; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool isPenDefined() const { return mPenDefined; }
  /**
   * @brief 
   *
   * @param painter
   * @param defaultPen
   */
  void applyTo(QCPPainter *painter, const QPen &defaultPen) const;
  /**
   * @brief 
   *
   * @param painter
   * @param pos
   */
  void drawShape(QCPPainter *painter, QPointF pos) const;
  /**
   * @brief 
   *
   * @param painter
   * @param x
   * @param y
   */
  void drawShape(QCPPainter *painter, double x, double y) const;

protected:
  // property members:
  double mSize; /**< TODO: describe */
  ScatterShape mShape; /**< TODO: describe */
  QPen mPen; /**< TODO: describe */
  QBrush mBrush; /**< TODO: describe */
  QPixmap mPixmap; /**< TODO: describe */
  QPainterPath mCustomPath; /**< TODO: describe */
  
  // non-property members:
  bool mPenDefined; /**< TODO: describe */
};
Q_DECLARE_TYPEINFO(QCPScatterStyle, Q_MOVABLE_TYPE);


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPPainter : public QPainter
{
  Q_GADGET
public:
  /*!
    Defines special modes the painter can operate in. They disable or enable certain subsets of features/fixes/workarounds,
    depending on whether they are wanted on the respective output device.
  */
  enum PainterMode { pmDefault       = 0x00   ///< <tt>0x00</tt> Default mode for painting on screen devices
                     ,pmVectorized   = 0x01   ///< <tt>0x01</tt> Mode for vectorized painting (e.g. PDF export). For example, this prevents some antialiasing fixes.
                     ,pmNoCaching    = 0x02   ///< <tt>0x02</tt> Mode for all sorts of exports (e.g. PNG, PDF,...). For example, this prevents using cached pixmap labels
                     ,pmNonCosmetic  = 0x04   ///< <tt>0x04</tt> Turns pen widths 0 to 1, i.e. disables cosmetic pens. (A cosmetic pen is always drawn with width 1 pixel in the vector image/pdf viewer, independent of zoom.) /**< TODO: describe */
                   };
  Q_FLAGS(PainterMode PainterModes)
  /**
   * @brief 
   *
   */
  Q_DECLARE_FLAGS(PainterModes, PainterMode)
  
  /**
   * @brief 
   *
   */
  QCPPainter();
  /**
   * @brief 
   *
   * @param device
   */
  QCPPainter(QPaintDevice *device);
  /**
   * @brief 
   *
   */
  ~QCPPainter();
  
  // getters:
  /**
   * @brief 
   *
   * @return bool
   */
  bool antialiasing() const { return testRenderHint(QPainter::Antialiasing); }
  /**
   * @brief 
   *
   * @return PainterModes
   */
  PainterModes modes() const { return mModes; }

  // setters:
  /**
   * @brief 
   *
   * @param enabled
   */
  void setAntialiasing(bool enabled);
  /**
   * @brief 
   *
   * @param mode
   * @param enabled
   */
  void setMode(PainterMode mode, bool enabled=true);
  /**
   * @brief 
   *
   * @param modes
   */
  void setModes(PainterModes modes);

  // methods hiding non-virtual base class functions (QPainter bug workarounds):
  /**
   * @brief 
   *
   * @param device
   * @return bool
   */
  bool begin(QPaintDevice *device);
  /**
   * @brief 
   *
   * @param pen
   */
  void setPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param color
   */
  void setPen(const QColor &color);
  /**
   * @brief 
   *
   * @param penStyle
   */
  void setPen(Qt::PenStyle penStyle);
  /**
   * @brief 
   *
   * @param line
   */
  void drawLine(const QLineF &line);
  /**
   * @brief 
   *
   * @param p1
   * @param p2
   */
  void drawLine(const QPointF &p1, const QPointF &p2) {drawLine(QLineF(p1, p2));}
  /**
   * @brief 
   *
   */
  void save();
  /**
   * @brief 
   *
   */
  void restore();
  
  // non-virtual methods:
  /**
   * @brief 
   *
   */
  void makeNonCosmetic();
  
protected:
  // property members:
  PainterModes mModes; /**< TODO: describe */
  bool mIsAntialiasing; /**< TODO: describe */
  
  // non-property members:
  QStack<bool> mAntialiasingStack; /**< TODO: describe */
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QCPPainter::PainterModes)


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPLayer : public QObject
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(QCustomPlot* parentPlot READ parentPlot)
  Q_PROPERTY(QString name READ name)
  Q_PROPERTY(int index READ index)
  Q_PROPERTY(QList<QCPLayerable*> children READ children)
  Q_PROPERTY(bool visible READ visible WRITE setVisible)
  /// \endcond
public:
  /**
   * @brief 
   *
   * @param parentPlot
   * @param layerName
   */
  QCPLayer(QCustomPlot* parentPlot, const QString &layerName);
  /**
   * @brief 
   *
   */
  ~QCPLayer();
  
  // getters:
  /**
   * @brief 
   *
   * @return QCustomPlot
   */
  QCustomPlot *parentPlot() const { return mParentPlot; }
  /**
   * @brief 
   *
   * @return QString
   */
  QString name() const { return mName; }
  /**
   * @brief 
   *
   * @return int
   */
  int index() const { return mIndex; }
  /**
   * @brief 
   *
   * @return QList<QCPLayerable *>
   */
  QList<QCPLayerable*> children() const { return mChildren; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool visible() const { return mVisible; }
  
  // setters:
  /**
   * @brief 
   *
   * @param visible
   */
  void setVisible(bool visible);
  
protected:
  // property members:
  QCustomPlot *mParentPlot; /**< TODO: describe */
  QString mName; /**< TODO: describe */
  int mIndex; /**< TODO: describe */
  QList<QCPLayerable*> mChildren; /**< TODO: describe */
  bool mVisible; /**< TODO: describe */
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @param layerable
   * @param prepend
   */
  void addChild(QCPLayerable *layerable, bool prepend);
  /**
   * @brief 
   *
   * @param layerable
   */
  void removeChild(QCPLayerable *layerable);
  
private:
  /**
   * @brief 
   *
   * @param 
   */
  Q_DISABLE_COPY(QCPLayer)
  
  friend class QCustomPlot;
  friend class QCPLayerable;
};

/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPLayerable : public QObject
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(bool visible READ visible WRITE setVisible)
  Q_PROPERTY(QCustomPlot* parentPlot READ parentPlot)
  Q_PROPERTY(QCPLayerable* parentLayerable READ parentLayerable)
  Q_PROPERTY(QCPLayer* layer READ layer WRITE setLayer NOTIFY layerChanged)
  Q_PROPERTY(bool antialiased READ antialiased WRITE setAntialiased)
  /// \endcond
public:
  /**
   * @brief 
   *
   * @param plot
   * @param targetLayer
   * @param parentLayerable
   */
  QCPLayerable(QCustomPlot *plot, QString targetLayer=QString(), QCPLayerable *parentLayerable=0);
  /**
   * @brief 
   *
   */
  ~QCPLayerable();
  
  // getters:
  /**
   * @brief 
   *
   * @return bool
   */
  bool visible() const { return mVisible; }
  /**
   * @brief 
   *
   * @return QCustomPlot
   */
  QCustomPlot *parentPlot() const { return mParentPlot; }
  /**
   * @brief 
   *
   * @return QCPLayerable
   */
  QCPLayerable *parentLayerable() const { return mParentLayerable.data(); }
  /**
   * @brief 
   *
   * @return QCPLayer
   */
  QCPLayer *layer() const { return mLayer; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool antialiased() const { return mAntialiased; }
  
  // setters:
  /**
   * @brief 
   *
   * @param on
   */
  void setVisible(bool on);
  /**
   * @brief 
   *
   * @param layer
   * @return bool
   */
  Q_SLOT bool setLayer(QCPLayer *layer);
  /**
   * @brief 
   *
   * @param layerName
   * @return bool
   */
  bool setLayer(const QString &layerName);
  /**
   * @brief 
   *
   * @param enabled
   */
  void setAntialiased(bool enabled);
  
  // introduced virtual methods:
  /**
   * @brief 
   *
   * @param pos
   * @param onlySelectable
   * @param details
   * @return double
   */
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
  // non-property methods:
  /**
   * @brief 
   *
   * @return bool
   */
  bool realVisibility() const;
  
signals:
  /**
   * @brief 
   *
   * @param newLayer
   */
  void layerChanged(QCPLayer *newLayer);
  
protected:
  // property members:
  bool mVisible; /**< TODO: describe */
  QCustomPlot *mParentPlot; /**< TODO: describe */
  QPointer<QCPLayerable> mParentLayerable; /**< TODO: describe */
  QCPLayer *mLayer; /**< TODO: describe */
  bool mAntialiased; /**< TODO: describe */
  
  // introduced virtual methods:
  /**
   * @brief 
   *
   * @param parentPlot
   */
  virtual void parentPlotInitialized(QCustomPlot *parentPlot);
  /**
   * @brief 
   *
   * @return QCP::Interaction
   */
  virtual QCP::Interaction selectionCategory() const;
  /**
   * @brief 
   *
   * @return QRect
   */
  virtual QRect clipRect() const;
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const = 0;
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter) = 0;
  // events:
  /**
   * @brief 
   *
   * @param event
   * @param additive
   * @param details
   * @param selectionStateChanged
   */
  virtual void selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged);
  /**
   * @brief 
   *
   * @param selectionStateChanged
   */
  virtual void deselectEvent(bool *selectionStateChanged);
  
  // non-property methods:
  /**
   * @brief 
   *
   * @param parentPlot
   */
  void initializeParentPlot(QCustomPlot *parentPlot);
  /**
   * @brief 
   *
   * @param parentLayerable
   */
  void setParentLayerable(QCPLayerable* parentLayerable);
  /**
   * @brief 
   *
   * @param layer
   * @param prepend
   * @return bool
   */
  bool moveToLayer(QCPLayer *layer, bool prepend);
  /**
   * @brief 
   *
   * @param painter
   * @param localAntialiased
   * @param overrideElement
   */
  void applyAntialiasingHint(QCPPainter *painter, bool localAntialiased, QCP::AntialiasedElement overrideElement) const;
  
private:
  /**
   * @brief 
   *
   * @param 
   */
  Q_DISABLE_COPY(QCPLayerable)
  
  friend class QCustomPlot;
  friend class QCPAxisRect;
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPRange
{
public:
  double lower, upper; /**< TODO: describe */
  
  /**
   * @brief 
   *
   */
  QCPRange();
  /**
   * @brief 
   *
   * @param lower
   * @param upper
   */
  QCPRange(double lower, double upper);
  
  /**
   * @brief 
   *
   * @param other
   * @return bool operator
   */
  bool operator==(const QCPRange& other) const { return lower == other.lower && upper == other.upper; }
  /**
   * @brief 
   *
   * @param other
   * @return bool operator
   */
  bool operator!=(const QCPRange& other) const { return !(*this == other); }
  
  /**
   * @brief 
   *
   * @param value
   * @return QCPRange &operator
   */
  QCPRange &operator+=(const double& value) { lower+=value; upper+=value; return *this; }
  /**
   * @brief 
   *
   * @param value
   * @return QCPRange &operator
   */
  QCPRange &operator-=(const double& value) { lower-=value; upper-=value; return *this; }
  /**
   * @brief 
   *
   * @param value
   * @return QCPRange &operator
   */
  QCPRange &operator*=(const double& value) { lower*=value; upper*=value; return *this; }
  /**
   * @brief 
   *
   * @param value
   * @return QCPRange &operator
   */
  QCPRange &operator/=(const double& value) { lower/=value; upper/=value; return *this; }
  /**
   * @brief 
   *
   * @param 
   * @param double
   * @return const QCPRange operator
   */
  friend inline const QCPRange operator+(const QCPRange&, double);
  /**
   * @brief 
   *
   * @param double
   * @param 
   * @return const QCPRange operator
   */
  friend inline const QCPRange operator+(double, const QCPRange&);
  /**
   * @brief 
   *
   * @param range
   * @param value
   * @return const QCPRange operator
   */
  friend inline const QCPRange operator-(const QCPRange& range, double value);
  /**
   * @brief 
   *
   * @param range
   * @param value
   * @return const QCPRange operator
   */
  friend inline const QCPRange operator*(const QCPRange& range, double value);
  /**
   * @brief 
   *
   * @param value
   * @param range
   * @return const QCPRange operator
   */
  friend inline const QCPRange operator*(double value, const QCPRange& range);
  /**
   * @brief 
   *
   * @param range
   * @param value
   * @return const QCPRange operator
   */
  friend inline const QCPRange operator/(const QCPRange& range, double value);
  
  /**
   * @brief 
   *
   * @return double
   */
  double size() const;
  /**
   * @brief 
   *
   * @return double
   */
  double center() const;
  /**
   * @brief 
   *
   */
  void normalize();
  /**
   * @brief 
   *
   * @param otherRange
   */
  void expand(const QCPRange &otherRange);
  /**
   * @brief 
   *
   * @param otherRange
   * @return QCPRange
   */
  QCPRange expanded(const QCPRange &otherRange) const;
  /**
   * @brief 
   *
   * @return QCPRange
   */
  QCPRange sanitizedForLogScale() const;
  /**
   * @brief 
   *
   * @return QCPRange
   */
  QCPRange sanitizedForLinScale() const;
  /**
   * @brief 
   *
   * @param value
   * @return bool
   */
  bool contains(double value) const;
  
  /**
   * @brief 
   *
   * @param lower
   * @param upper
   * @return bool
   */
  static bool validRange(double lower, double upper);
  /**
   * @brief 
   *
   * @param range
   * @return bool
   */
  static bool validRange(const QCPRange &range);
  static const double minRange; //1e-280; /**< TODO: describe */
  static const double maxRange; //1e280; /**< TODO: describe */
  
};
Q_DECLARE_TYPEINFO(QCPRange, Q_MOVABLE_TYPE);

/* documentation of inline functions */

/*! \fn QCPRange &QCPRange::operator+=(const double& value)
  
  Adds \a value to both boundaries of the range.
*/

/*! \fn QCPRange &QCPRange::operator-=(const double& value)
  
  Subtracts \a value from both boundaries of the range.
*/

/*! \fn QCPRange &QCPRange::operator*=(const double& value)
  
  Multiplies both boundaries of the range by \a value.
*/

/*! \fn QCPRange &QCPRange::operator/=(const double& value)
  
  Divides both boundaries of the range by \a value.
*/

/* end documentation of inline functions */

/*!
  Adds \a value to both boundaries of the range.
*/
inline const QCPRange operator+(const QCPRange& range, double value)
{
  QCPRange result(range);
  result += value;
  return result;
}

/*!
  Adds \a value to both boundaries of the range.
*/
inline const QCPRange operator+(double value, const QCPRange& range)
{
  QCPRange result(range);
  result += value;
  return result;
}

/*!
  Subtracts \a value from both boundaries of the range.
*/
inline const QCPRange operator-(const QCPRange& range, double value)
{
  QCPRange result(range);
  result -= value;
  return result;
}

/*!
  Multiplies both boundaries of the range by \a value.
*/
inline const QCPRange operator*(const QCPRange& range, double value)
{
  QCPRange result(range);
  result *= value;
  return result;
}

/*!
  Multiplies both boundaries of the range by \a value.
*/
inline const QCPRange operator*(double value, const QCPRange& range)
{
  QCPRange result(range);
  result *= value;
  return result;
}

/*!
  Divides both boundaries of the range by \a value.
*/
inline const QCPRange operator/(const QCPRange& range, double value)
{
  QCPRange result(range);
  result /= value;
  return result;
}


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPMarginGroup : public QObject
{
  Q_OBJECT
public:
  /**
   * @brief 
   *
   * @param parentPlot
   */
  QCPMarginGroup(QCustomPlot *parentPlot);
  /**
   * @brief 
   *
   */
  ~QCPMarginGroup();
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @param side
   * @return QList<QCPLayoutElement *>
   */
  QList<QCPLayoutElement*> elements(QCP::MarginSide side) const { return mChildren.value(side); }
  /**
   * @brief 
   *
   * @return bool
   */
  bool isEmpty() const;
  /**
   * @brief 
   *
   */
  void clear();
  
protected:
  // non-property members:
  QCustomPlot *mParentPlot; /**< TODO: describe */
  QHash<QCP::MarginSide, QList<QCPLayoutElement*> > mChildren; /**< TODO: describe */
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @param side
   * @return int
   */
  int commonMargin(QCP::MarginSide side) const;
  /**
   * @brief 
   *
   * @param side
   * @param element
   */
  void addChild(QCP::MarginSide side, QCPLayoutElement *element);
  /**
   * @brief 
   *
   * @param side
   * @param element
   */
  void removeChild(QCP::MarginSide side, QCPLayoutElement *element);
  
private:
  /**
   * @brief 
   *
   * @param 
   */
  Q_DISABLE_COPY(QCPMarginGroup)
  
  friend class QCPLayoutElement;
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPLayoutElement : public QCPLayerable
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(QCPLayout* layout READ layout)
  Q_PROPERTY(QRect rect READ rect)
  Q_PROPERTY(QRect outerRect READ outerRect WRITE setOuterRect)
  Q_PROPERTY(QMargins margins READ margins WRITE setMargins)
  Q_PROPERTY(QMargins minimumMargins READ minimumMargins WRITE setMinimumMargins)
  Q_PROPERTY(QSize minimumSize READ minimumSize WRITE setMinimumSize)
  Q_PROPERTY(QSize maximumSize READ maximumSize WRITE setMaximumSize)
  /// \endcond
public:
  /*!
    Defines the phases of the update process, that happens just before a replot. At each phase,
    \ref update is called with the according UpdatePhase value.
  */
  enum UpdatePhase { upPreparation ///< Phase used for any type of preparation that needs to be done before margin calculation and layout
                     ,upMargins    ///< Phase in which the margins are calculated and set
                     ,upLayout     ///< Final phase in which the layout system places the rects of the elements
                   };
  Q_ENUMS(UpdatePhase)

  /**
   * @brief 
   *
   * @param parentPlot
   */
  explicit QCPLayoutElement(QCustomPlot *parentPlot=0);
  /**
   * @brief 
   *
   */
  virtual ~QCPLayoutElement();
  
  // getters:
  /**
   * @brief 
   *
   * @return QCPLayout
   */
  QCPLayout *layout() const { return mParentLayout; }
  /**
   * @brief 
   *
   * @return QRect
   */
  QRect rect() const { return mRect; }
  /**
   * @brief 
   *
   * @return QRect
   */
  QRect outerRect() const { return mOuterRect; }
  /**
   * @brief 
   *
   * @return QMargins
   */
  QMargins margins() const { return mMargins; }
  /**
   * @brief 
   *
   * @return QMargins
   */
  QMargins minimumMargins() const { return mMinimumMargins; }
  /**
   * @brief 
   *
   * @return QCP::MarginSides
   */
  QCP::MarginSides autoMargins() const { return mAutoMargins; }
  /**
   * @brief 
   *
   * @return QSize
   */
  QSize minimumSize() const { return mMinimumSize; }
  /**
   * @brief 
   *
   * @return QSize
   */
  QSize maximumSize() const { return mMaximumSize; }
  /**
   * @brief 
   *
   * @param side
   * @return QCPMarginGroup
   */
  QCPMarginGroup *marginGroup(QCP::MarginSide side) const { return mMarginGroups.value(side, (QCPMarginGroup*)0); }
  /**
   * @brief 
   *
   * @return QHash<QCP::MarginSide, QCPMarginGroup *>
   */
  QHash<QCP::MarginSide, QCPMarginGroup*> marginGroups() const { return mMarginGroups; }
  
  // setters:
  /**
   * @brief 
   *
   * @param rect
   */
  void setOuterRect(const QRect &rect);
  /**
   * @brief 
   *
   * @param margins
   */
  void setMargins(const QMargins &margins);
  /**
   * @brief 
   *
   * @param margins
   */
  void setMinimumMargins(const QMargins &margins);
  /**
   * @brief 
   *
   * @param sides
   */
  void setAutoMargins(QCP::MarginSides sides);
  /**
   * @brief 
   *
   * @param size
   */
  void setMinimumSize(const QSize &size);
  /**
   * @brief 
   *
   * @param width
   * @param height
   */
  void setMinimumSize(int width, int height);
  /**
   * @brief 
   *
   * @param size
   */
  void setMaximumSize(const QSize &size);
  /**
   * @brief 
   *
   * @param width
   * @param height
   */
  void setMaximumSize(int width, int height);
  /**
   * @brief 
   *
   * @param sides
   * @param group
   */
  void setMarginGroup(QCP::MarginSides sides, QCPMarginGroup *group);
  
  // introduced virtual methods:
  /**
   * @brief 
   *
   * @param phase
   */
  virtual void update(UpdatePhase phase);
  /**
   * @brief 
   *
   * @return QSize
   */
  virtual QSize minimumSizeHint() const;
  /**
   * @brief 
   *
   * @return QSize
   */
  virtual QSize maximumSizeHint() const;
  /**
   * @brief 
   *
   * @param recursive
   * @return QList<QCPLayoutElement *>
   */
  virtual QList<QCPLayoutElement*> elements(bool recursive) const;
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param pos
   * @param onlySelectable
   * @param details
   * @return double
   */
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
protected:
  // property members:
  QCPLayout *mParentLayout; /**< TODO: describe */
  QSize mMinimumSize, mMaximumSize; /**< TODO: describe */
  QRect mRect, mOuterRect; /**< TODO: describe */
  QMargins mMargins, mMinimumMargins; /**< TODO: describe */
  QCP::MarginSides mAutoMargins; /**< TODO: describe */
  QHash<QCP::MarginSide, QCPMarginGroup*> mMarginGroups; /**< TODO: describe */
  
  // introduced virtual methods:
  /**
   * @brief 
   *
   * @param side
   * @return int
   */
  virtual int calculateAutoMargin(QCP::MarginSide side);
  // events:
  /**
   * @brief 
   *
   * @param event
   */
  virtual void mousePressEvent(QMouseEvent *event) {Q_UNUSED(event)}
  /**
   * @brief 
   *
   * @param event
   */
  virtual void mouseMoveEvent(QMouseEvent *event) {Q_UNUSED(event)}
  /**
   * @brief 
   *
   * @param event
   */
  virtual void mouseReleaseEvent(QMouseEvent *event) {Q_UNUSED(event)}
  /**
   * @brief 
   *
   * @param event
   */
  virtual void mouseDoubleClickEvent(QMouseEvent *event) {Q_UNUSED(event)}
  /**
   * @brief 
   *
   * @param event
   */
  virtual void wheelEvent(QWheelEvent *event) {Q_UNUSED(event)}
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const { Q_UNUSED(painter) }
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter) { Q_UNUSED(painter) }
  /**
   * @brief 
   *
   * @param parentPlot
   */
  virtual void parentPlotInitialized(QCustomPlot *parentPlot);

private:
  /**
   * @brief 
   *
   * @param 
   */
  Q_DISABLE_COPY(QCPLayoutElement)
  
  friend class QCustomPlot;
  friend class QCPLayout;
  friend class QCPMarginGroup;
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPLayout : public QCPLayoutElement
{
  Q_OBJECT
public:
  /**
   * @brief 
   *
   */
  explicit QCPLayout();
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param phase
   */
  virtual void update(UpdatePhase phase);
  /**
   * @brief 
   *
   * @param recursive
   * @return QList<QCPLayoutElement *>
   */
  virtual QList<QCPLayoutElement*> elements(bool recursive) const;
  
  // introduced virtual methods:
  /**
   * @brief 
   *
   * @return int
   */
  virtual int elementCount() const = 0;
  /**
   * @brief 
   *
   * @param index
   * @return QCPLayoutElement
   */
  virtual QCPLayoutElement* elementAt(int index) const = 0;
  /**
   * @brief 
   *
   * @param index
   * @return QCPLayoutElement
   */
  virtual QCPLayoutElement* takeAt(int index) = 0;
  /**
   * @brief 
   *
   * @param element
   * @return bool
   */
  virtual bool take(QCPLayoutElement* element) = 0;
  /**
   * @brief 
   *
   */
  virtual void simplify();
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @param index
   * @return bool
   */
  bool removeAt(int index);
  /**
   * @brief 
   *
   * @param element
   * @return bool
   */
  bool remove(QCPLayoutElement* element);
  /**
   * @brief 
   *
   */
  void clear();
  
protected:
  // introduced virtual methods:
  /**
   * @brief 
   *
   */
  virtual void updateLayout();
  
  // non-virtual methods:
  /**
   * @brief 
   *
   */
  void sizeConstraintsChanged() const;
  /**
   * @brief 
   *
   * @param el
   */
  void adoptElement(QCPLayoutElement *el);
  /**
   * @brief 
   *
   * @param el
   */
  void releaseElement(QCPLayoutElement *el);
  /**
   * @brief 
   *
   * @param maxSizes
   * @param minSizes
   * @param stretchFactors
   * @param totalSize
   * @return QVector<int>
   */
  QVector<int> getSectionSizes(QVector<int> maxSizes, QVector<int> minSizes, QVector<double> stretchFactors, int totalSize) const;
  
private:
  /**
   * @brief 
   *
   * @param 
   */
  Q_DISABLE_COPY(QCPLayout)
  friend class QCPLayoutElement;
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPLayoutGrid : public QCPLayout
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(int rowCount READ rowCount)
  Q_PROPERTY(int columnCount READ columnCount)
  Q_PROPERTY(QList<double> columnStretchFactors READ columnStretchFactors WRITE setColumnStretchFactors)
  Q_PROPERTY(QList<double> rowStretchFactors READ rowStretchFactors WRITE setRowStretchFactors)
  Q_PROPERTY(int columnSpacing READ columnSpacing WRITE setColumnSpacing)
  Q_PROPERTY(int rowSpacing READ rowSpacing WRITE setRowSpacing)
  /// \endcond
public:
  /**
   * @brief 
   *
   */
  explicit QCPLayoutGrid();
  /**
   * @brief 
   *
   */
  virtual ~QCPLayoutGrid();
  
  // getters:
  /**
   * @brief 
   *
   * @return int
   */
  int rowCount() const;
  /**
   * @brief 
   *
   * @return int
   */
  int columnCount() const;
  /**
   * @brief 
   *
   * @return QList<double>
   */
  QList<double> columnStretchFactors() const { return mColumnStretchFactors; }
  /**
   * @brief 
   *
   * @return QList<double>
   */
  QList<double> rowStretchFactors() const { return mRowStretchFactors; }
  /**
   * @brief 
   *
   * @return int
   */
  int columnSpacing() const { return mColumnSpacing; }
  /**
   * @brief 
   *
   * @return int
   */
  int rowSpacing() const { return mRowSpacing; }
  
  // setters:
  /**
   * @brief 
   *
   * @param column
   * @param factor
   */
  void setColumnStretchFactor(int column, double factor);
  /**
   * @brief 
   *
   * @param factors
   */
  void setColumnStretchFactors(const QList<double> &factors);
  /**
   * @brief 
   *
   * @param row
   * @param factor
   */
  void setRowStretchFactor(int row, double factor);
  /**
   * @brief 
   *
   * @param factors
   */
  void setRowStretchFactors(const QList<double> &factors);
  /**
   * @brief 
   *
   * @param pixels
   */
  void setColumnSpacing(int pixels);
  /**
   * @brief 
   *
   * @param pixels
   */
  void setRowSpacing(int pixels);
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   */
  virtual void updateLayout();
  /**
   * @brief 
   *
   * @return int
   */
  virtual int elementCount() const;
  /**
   * @brief 
   *
   * @param index
   * @return QCPLayoutElement
   */
  virtual QCPLayoutElement* elementAt(int index) const;
  /**
   * @brief 
   *
   * @param index
   * @return QCPLayoutElement
   */
  virtual QCPLayoutElement* takeAt(int index);
  /**
   * @brief 
   *
   * @param element
   * @return bool
   */
  virtual bool take(QCPLayoutElement* element);
  /**
   * @brief 
   *
   * @param recursive
   * @return QList<QCPLayoutElement *>
   */
  virtual QList<QCPLayoutElement*> elements(bool recursive) const;
  /**
   * @brief 
   *
   */
  virtual void simplify();
  /**
   * @brief 
   *
   * @return QSize
   */
  virtual QSize minimumSizeHint() const;
  /**
   * @brief 
   *
   * @return QSize
   */
  virtual QSize maximumSizeHint() const;
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @param row
   * @param column
   * @return QCPLayoutElement
   */
  QCPLayoutElement *element(int row, int column) const;
  /**
   * @brief 
   *
   * @param row
   * @param column
   * @param element
   * @return bool
   */
  bool addElement(int row, int column, QCPLayoutElement *element);
  /**
   * @brief 
   *
   * @param row
   * @param column
   * @return bool
   */
  bool hasElement(int row, int column);
  /**
   * @brief 
   *
   * @param newRowCount
   * @param newColumnCount
   */
  void expandTo(int newRowCount, int newColumnCount);
  /**
   * @brief 
   *
   * @param newIndex
   */
  void insertRow(int newIndex);
  /**
   * @brief 
   *
   * @param newIndex
   */
  void insertColumn(int newIndex);
  
protected:
  // property members:
  QList<QList<QCPLayoutElement*> > mElements; /**< TODO: describe */
  QList<double> mColumnStretchFactors; /**< TODO: describe */
  QList<double> mRowStretchFactors; /**< TODO: describe */
  int mColumnSpacing, mRowSpacing; /**< TODO: describe */
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @param minColWidths
   * @param minRowHeights
   */
  void getMinimumRowColSizes(QVector<int> *minColWidths, QVector<int> *minRowHeights) const;
  /**
   * @brief 
   *
   * @param maxColWidths
   * @param maxRowHeights
   */
  void getMaximumRowColSizes(QVector<int> *maxColWidths, QVector<int> *maxRowHeights) const;
  
private:
  /**
   * @brief 
   *
   * @param 
   */
  Q_DISABLE_COPY(QCPLayoutGrid)
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPLayoutInset : public QCPLayout
{
  Q_OBJECT
public:
  /*!
    Defines how the placement and sizing is handled for a certain element in a QCPLayoutInset.
  */
  enum InsetPlacement { ipFree            ///< The element may be positioned/sized arbitrarily, see \ref setInsetRect
                        ,ipBorderAligned  ///< The element is aligned to one of the layout sides, see \ref setInsetAlignment
                      };
  
  /**
   * @brief 
   *
   */
  explicit QCPLayoutInset();
  /**
   * @brief 
   *
   */
  virtual ~QCPLayoutInset();
  
  // getters:
  /**
   * @brief 
   *
   * @param index
   * @return InsetPlacement
   */
  InsetPlacement insetPlacement(int index) const;
  /**
   * @brief 
   *
   * @param index
   * @return Qt::Alignment
   */
  Qt::Alignment insetAlignment(int index) const;
  /**
   * @brief 
   *
   * @param index
   * @return QRectF
   */
  QRectF insetRect(int index) const;
  
  // setters:
  /**
   * @brief 
   *
   * @param index
   * @param placement
   */
  void setInsetPlacement(int index, InsetPlacement placement);
  /**
   * @brief 
   *
   * @param index
   * @param alignment
   */
  void setInsetAlignment(int index, Qt::Alignment alignment);
  /**
   * @brief 
   *
   * @param index
   * @param rect
   */
  void setInsetRect(int index, const QRectF &rect);
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   */
  virtual void updateLayout();
  /**
   * @brief 
   *
   * @return int
   */
  virtual int elementCount() const;
  /**
   * @brief 
   *
   * @param index
   * @return QCPLayoutElement
   */
  virtual QCPLayoutElement* elementAt(int index) const;
  /**
   * @brief 
   *
   * @param index
   * @return QCPLayoutElement
   */
  virtual QCPLayoutElement* takeAt(int index);
  /**
   * @brief 
   *
   * @param element
   * @return bool
   */
  virtual bool take(QCPLayoutElement* element);
  /**
   * @brief 
   *
   */
  virtual void simplify() {}
  /**
   * @brief 
   *
   * @param pos
   * @param onlySelectable
   * @param details
   * @return double
   */
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @param element
   * @param alignment
   */
  void addElement(QCPLayoutElement *element, Qt::Alignment alignment);
  /**
   * @brief 
   *
   * @param element
   * @param rect
   */
  void addElement(QCPLayoutElement *element, const QRectF &rect);
  
protected:
  // property members:
  QList<QCPLayoutElement*> mElements; /**< TODO: describe */
  QList<InsetPlacement> mInsetPlacement; /**< TODO: describe */
  QList<Qt::Alignment> mInsetAlignment; /**< TODO: describe */
  QList<QRectF> mInsetRect; /**< TODO: describe */
  
private:
  /**
   * @brief 
   *
   * @param 
   */
  Q_DISABLE_COPY(QCPLayoutInset)
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPLineEnding
{
  Q_GADGET
public:
  /*!
    Defines the type of ending decoration for line-like items, e.g. an arrow.
    
    \image html QCPLineEnding.png
    
    The width and length of these decorations can be controlled with the functions \ref setWidth
    and \ref setLength. Some decorations like \ref esDisc, \ref esSquare, \ref esDiamond and \ref esBar only
    support a width, the length property is ignored.
    
    \see QCPItemLine::setHead, QCPItemLine::setTail, QCPItemCurve::setHead, QCPItemCurve::setTail, QCPAxis::setLowerEnding, QCPAxis::setUpperEnding
  */
  Q_ENUMS(EndingStyle)
  enum EndingStyle { esNone          ///< No ending decoration
                     ,esFlatArrow    ///< A filled arrow head with a straight/flat back (a triangle)
                     ,esSpikeArrow   ///< A filled arrow head with an indented back
                     ,esLineArrow    ///< A non-filled arrow head with open back
                     ,esDisc         ///< A filled circle /**< TODO: describe */
                     ,esSquare       ///< A filled square /**< TODO: describe */
                     ,esDiamond      ///< A filled diamond (45° rotated square)
                     ,esBar          ///< A bar perpendicular to the line
                     ,esHalfBar      ///< A bar perpendicular to the line, pointing out to only one side (to which side can be changed with \ref setInverted)
                     ,esSkewedBar    ///< A bar that is skewed (skew controllable via \ref setLength) /**< TODO: describe */
                   };
  
  /**
   * @brief 
   *
   */
  QCPLineEnding();
  /**
   * @brief 
   *
   * @param style
   * @param width
   * @param length
   * @param inverted
   */
  QCPLineEnding(EndingStyle style, double width=8, double length=10, bool inverted=false);
  
  // getters:
  /**
   * @brief 
   *
   * @return EndingStyle
   */
  EndingStyle style() const { return mStyle; }
  /**
   * @brief 
   *
   * @return double
   */
  double width() const { return mWidth; }
  /**
   * @brief 
   *
   * @return double
   */
  double length() const { return mLength; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool inverted() const { return mInverted; }
  
  // setters:
  /**
   * @brief 
   *
   * @param style
   */
  void setStyle(EndingStyle style);
  /**
   * @brief 
   *
   * @param width
   */
  void setWidth(double width);
  /**
   * @brief 
   *
   * @param length
   */
  void setLength(double length);
  /**
   * @brief 
   *
   * @param inverted
   */
  void setInverted(bool inverted);
  
  // non-property methods:
  /**
   * @brief 
   *
   * @return double
   */
  double boundingDistance() const;
  /**
   * @brief 
   *
   * @return double
   */
  double realLength() const;
  /**
   * @brief 
   *
   * @param painter
   * @param pos
   * @param dir
   */
  void draw(QCPPainter *painter, const QVector2D &pos, const QVector2D &dir) const;
  /**
   * @brief 
   *
   * @param painter
   * @param pos
   * @param angle
   */
  void draw(QCPPainter *painter, const QVector2D &pos, double angle) const;
  
protected:
  // property members:
  EndingStyle mStyle; /**< TODO: describe */
  double mWidth, mLength; /**< TODO: describe */
  bool mInverted; /**< TODO: describe */
};
Q_DECLARE_TYPEINFO(QCPLineEnding, Q_MOVABLE_TYPE);


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPGrid :public QCPLayerable
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(bool subGridVisible READ subGridVisible WRITE setSubGridVisible)
  Q_PROPERTY(bool antialiasedSubGrid READ antialiasedSubGrid WRITE setAntialiasedSubGrid)
  Q_PROPERTY(bool antialiasedZeroLine READ antialiasedZeroLine WRITE setAntialiasedZeroLine)
  Q_PROPERTY(QPen pen READ pen WRITE setPen)
  Q_PROPERTY(QPen subGridPen READ subGridPen WRITE setSubGridPen)
  Q_PROPERTY(QPen zeroLinePen READ zeroLinePen WRITE setZeroLinePen)
  /// \endcond
public:
  /**
   * @brief 
   *
   * @param parentAxis
   */
  QCPGrid(QCPAxis *parentAxis);
  
  // getters:
  /**
   * @brief 
   *
   * @return bool
   */
  bool subGridVisible() const { return mSubGridVisible; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool antialiasedSubGrid() const { return mAntialiasedSubGrid; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool antialiasedZeroLine() const { return mAntialiasedZeroLine; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen pen() const { return mPen; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen subGridPen() const { return mSubGridPen; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen zeroLinePen() const { return mZeroLinePen; }
  
  // setters:
  /**
   * @brief 
   *
   * @param visible
   */
  void setSubGridVisible(bool visible);
  /**
   * @brief 
   *
   * @param enabled
   */
  void setAntialiasedSubGrid(bool enabled);
  /**
   * @brief 
   *
   * @param enabled
   */
  void setAntialiasedZeroLine(bool enabled);
  /**
   * @brief 
   *
   * @param pen
   */
  void setPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param pen
   */
  void setSubGridPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param pen
   */
  void setZeroLinePen(const QPen &pen);
  
protected:
  // property members:
  bool mSubGridVisible; /**< TODO: describe */
  bool mAntialiasedSubGrid, mAntialiasedZeroLine; /**< TODO: describe */
  QPen mPen, mSubGridPen, mZeroLinePen; /**< TODO: describe */
  // non-property members:
  QCPAxis *mParentAxis; /**< TODO: describe */
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter);
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @param painter
   */
  void drawGridLines(QCPPainter *painter) const;
  /**
   * @brief 
   *
   * @param painter
   */
  void drawSubGridLines(QCPPainter *painter) const;
  
  friend class QCPAxis;
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPAxis : public QCPLayerable
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(AxisType axisType READ axisType)
  Q_PROPERTY(QCPAxisRect* axisRect READ axisRect)
  Q_PROPERTY(ScaleType scaleType READ scaleType WRITE setScaleType NOTIFY scaleTypeChanged)
  Q_PROPERTY(double scaleLogBase READ scaleLogBase WRITE setScaleLogBase)
  Q_PROPERTY(QCPRange range READ range WRITE setRange NOTIFY rangeChanged)
  Q_PROPERTY(bool rangeReversed READ rangeReversed WRITE setRangeReversed)
  Q_PROPERTY(bool autoTicks READ autoTicks WRITE setAutoTicks)
  Q_PROPERTY(int autoTickCount READ autoTickCount WRITE setAutoTickCount)
  Q_PROPERTY(bool autoTickLabels READ autoTickLabels WRITE setAutoTickLabels)
  Q_PROPERTY(bool autoTickStep READ autoTickStep WRITE setAutoTickStep)
  Q_PROPERTY(bool autoSubTicks READ autoSubTicks WRITE setAutoSubTicks)
  Q_PROPERTY(bool ticks READ ticks WRITE setTicks)
  Q_PROPERTY(bool tickLabels READ tickLabels WRITE setTickLabels)
  Q_PROPERTY(int tickLabelPadding READ tickLabelPadding WRITE setTickLabelPadding)
  Q_PROPERTY(LabelType tickLabelType READ tickLabelType WRITE setTickLabelType)
  Q_PROPERTY(QFont tickLabelFont READ tickLabelFont WRITE setTickLabelFont)
  Q_PROPERTY(QColor tickLabelColor READ tickLabelColor WRITE setTickLabelColor)
  Q_PROPERTY(double tickLabelRotation READ tickLabelRotation WRITE setTickLabelRotation)
  Q_PROPERTY(LabelSide tickLabelSide READ tickLabelSide WRITE setTickLabelSide)
  Q_PROPERTY(QString dateTimeFormat READ dateTimeFormat WRITE setDateTimeFormat)
  Q_PROPERTY(Qt::TimeSpec dateTimeSpec READ dateTimeSpec WRITE setDateTimeSpec)
  Q_PROPERTY(QString numberFormat READ numberFormat WRITE setNumberFormat)
  Q_PROPERTY(int numberPrecision READ numberPrecision WRITE setNumberPrecision)
  Q_PROPERTY(double tickStep READ tickStep WRITE setTickStep)
  Q_PROPERTY(QVector<double> tickVector READ tickVector WRITE setTickVector)
  Q_PROPERTY(QVector<QString> tickVectorLabels READ tickVectorLabels WRITE setTickVectorLabels)
  Q_PROPERTY(int tickLengthIn READ tickLengthIn WRITE setTickLengthIn)
  Q_PROPERTY(int tickLengthOut READ tickLengthOut WRITE setTickLengthOut)
  Q_PROPERTY(int subTickCount READ subTickCount WRITE setSubTickCount)
  Q_PROPERTY(int subTickLengthIn READ subTickLengthIn WRITE setSubTickLengthIn)
  Q_PROPERTY(int subTickLengthOut READ subTickLengthOut WRITE setSubTickLengthOut)
  Q_PROPERTY(QPen basePen READ basePen WRITE setBasePen)
  Q_PROPERTY(QPen tickPen READ tickPen WRITE setTickPen)
  Q_PROPERTY(QPen subTickPen READ subTickPen WRITE setSubTickPen)
  Q_PROPERTY(QFont labelFont READ labelFont WRITE setLabelFont)
  Q_PROPERTY(QColor labelColor READ labelColor WRITE setLabelColor)
  Q_PROPERTY(QString label READ label WRITE setLabel)
  Q_PROPERTY(int labelPadding READ labelPadding WRITE setLabelPadding)
  Q_PROPERTY(int padding READ padding WRITE setPadding)
  Q_PROPERTY(int offset READ offset WRITE setOffset)
  Q_PROPERTY(SelectableParts selectedParts READ selectedParts WRITE setSelectedParts NOTIFY selectionChanged)
  Q_PROPERTY(SelectableParts selectableParts READ selectableParts WRITE setSelectableParts NOTIFY selectableChanged)
  Q_PROPERTY(QFont selectedTickLabelFont READ selectedTickLabelFont WRITE setSelectedTickLabelFont)
  Q_PROPERTY(QFont selectedLabelFont READ selectedLabelFont WRITE setSelectedLabelFont)
  Q_PROPERTY(QColor selectedTickLabelColor READ selectedTickLabelColor WRITE setSelectedTickLabelColor)
  Q_PROPERTY(QColor selectedLabelColor READ selectedLabelColor WRITE setSelectedLabelColor)
  Q_PROPERTY(QPen selectedBasePen READ selectedBasePen WRITE setSelectedBasePen)
  Q_PROPERTY(QPen selectedTickPen READ selectedTickPen WRITE setSelectedTickPen)
  Q_PROPERTY(QPen selectedSubTickPen READ selectedSubTickPen WRITE setSelectedSubTickPen)
  Q_PROPERTY(QCPLineEnding lowerEnding READ lowerEnding WRITE setLowerEnding)
  Q_PROPERTY(QCPLineEnding upperEnding READ upperEnding WRITE setUpperEnding)
  Q_PROPERTY(QCPGrid* grid READ grid)
  /// \endcond
public:
  /*!
    Defines at which side of the axis rect the axis will appear. This also affects how the tick
    marks are drawn, on which side the labels are placed etc.
  */
  enum AxisType { atLeft    = 0x01  ///< <tt>0x01</tt> Axis is vertical and on the left side of the axis rect
                  ,atRight  = 0x02  ///< <tt>0x02</tt> Axis is vertical and on the right side of the axis rect
                  ,atTop    = 0x04  ///< <tt>0x04</tt> Axis is horizontal and on the top side of the axis rect
                  ,atBottom = 0x08  ///< <tt>0x08</tt> Axis is horizontal and on the bottom side of the axis rect /**< TODO: describe */
                };
  Q_FLAGS(AxisType AxisTypes)
  /**
   * @brief 
   *
   */
  Q_DECLARE_FLAGS(AxisTypes, AxisType)
  /*!
    When automatic tick label generation is enabled (\ref setAutoTickLabels), defines how the
    coordinate of the tick is interpreted, i.e. translated into a string.
    
    \see setTickLabelType
  */
  enum LabelType { ltNumber    ///< Tick coordinate is regarded as normal number and will be displayed as such. (see \ref setNumberFormat)
                   ,ltDateTime ///< Tick coordinate is regarded as a date/time (seconds since 1970-01-01T00:00:00 UTC) and will be displayed and formatted as such. (for details, see \ref setDateTimeFormat)
                 };
  Q_ENUMS(LabelType)
  /*!
    Defines on which side of the axis the tick labels (numbers) shall appear.
    
    \see setTickLabelSide
  */
  enum LabelSide { lsInside    ///< Tick labels will be displayed inside the axis rect and clipped to the inner axis rect
                   ,lsOutside  ///< Tick labels will be displayed outside the axis rect
                 };
  Q_ENUMS(LabelSide)
  /*!
    Defines the scale of an axis.
    \see setScaleType
  */
  enum ScaleType { stLinear       ///< Linear scaling
                   ,stLogarithmic ///< Logarithmic scaling with correspondingly transformed plots and (major) tick marks at every base power (see \ref setScaleLogBase).
                 };
  Q_ENUMS(ScaleType)
  /*!
    Defines the selectable parts of an axis.
    \see setSelectableParts, setSelectedParts
  */
  enum SelectablePart { spNone        = 0      ///< None of the selectable parts
                        ,spAxis       = 0x001  ///< The axis backbone and tick marks
                        ,spTickLabels = 0x002  ///< Tick labels (numbers) of this axis (as a whole, not individually)
                        ,spAxisLabel  = 0x004  ///< The axis label
                      };
  Q_FLAGS(SelectablePart SelectableParts)
  /**
   * @brief 
   *
   */
  Q_DECLARE_FLAGS(SelectableParts, SelectablePart)
  
  /**
   * @brief 
   *
   * @param parent
   * @param type
   */
  explicit QCPAxis(QCPAxisRect *parent, AxisType type);
  /**
   * @brief 
   *
   */
  virtual ~QCPAxis();
  
  // getters:
  /**
   * @brief 
   *
   * @return AxisType
   */
  AxisType axisType() const { return mAxisType; }
  /**
   * @brief 
   *
   * @return QCPAxisRect
   */
  QCPAxisRect *axisRect() const { return mAxisRect; }
  /**
   * @brief 
   *
   * @return ScaleType
   */
  ScaleType scaleType() const { return mScaleType; }
  /**
   * @brief 
   *
   * @return double
   */
  double scaleLogBase() const { return mScaleLogBase; }
  /**
   * @brief 
   *
   * @return const QCPRange
   */
  const QCPRange range() const { return mRange; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool rangeReversed() const { return mRangeReversed; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool autoTicks() const { return mAutoTicks; }
  /**
   * @brief 
   *
   * @return int
   */
  int autoTickCount() const { return mAutoTickCount; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool autoTickLabels() const { return mAutoTickLabels; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool autoTickStep() const { return mAutoTickStep; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool autoSubTicks() const { return mAutoSubTicks; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool ticks() const { return mTicks; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool tickLabels() const { return mTickLabels; }
  /**
   * @brief 
   *
   * @return int
   */
  int tickLabelPadding() const;
  /**
   * @brief 
   *
   * @return LabelType
   */
  LabelType tickLabelType() const { return mTickLabelType; }
  /**
   * @brief 
   *
   * @return QFont
   */
  QFont tickLabelFont() const { return mTickLabelFont; }
  /**
   * @brief 
   *
   * @return QColor
   */
  QColor tickLabelColor() const { return mTickLabelColor; }
  /**
   * @brief 
   *
   * @return double
   */
  double tickLabelRotation() const;
  /**
   * @brief 
   *
   * @return LabelSide
   */
  LabelSide tickLabelSide() const;
  /**
   * @brief 
   *
   * @return QString
   */
  QString dateTimeFormat() const { return mDateTimeFormat; }
  /**
   * @brief 
   *
   * @return Qt::TimeSpec
   */
  Qt::TimeSpec dateTimeSpec() const { return mDateTimeSpec; }
  /**
   * @brief 
   *
   * @return QString
   */
  QString numberFormat() const;
  /**
   * @brief 
   *
   * @return int
   */
  int numberPrecision() const { return mNumberPrecision; }
  /**
   * @brief 
   *
   * @return double
   */
  double tickStep() const { return mTickStep; }
  /**
   * @brief 
   *
   * @return QVector<double>
   */
  QVector<double> tickVector() const { return mTickVector; }
  /**
   * @brief 
   *
   * @return QVector<QString>
   */
  QVector<QString> tickVectorLabels() const { return mTickVectorLabels; }
  /**
   * @brief 
   *
   * @return int
   */
  int tickLengthIn() const;
  /**
   * @brief 
   *
   * @return int
   */
  int tickLengthOut() const;
  /**
   * @brief 
   *
   * @return int
   */
  int subTickCount() const { return mSubTickCount; }
  /**
   * @brief 
   *
   * @return int
   */
  int subTickLengthIn() const;
  /**
   * @brief 
   *
   * @return int
   */
  int subTickLengthOut() const;
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen basePen() const { return mBasePen; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen tickPen() const { return mTickPen; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen subTickPen() const { return mSubTickPen; }
  /**
   * @brief 
   *
   * @return QFont
   */
  QFont labelFont() const { return mLabelFont; }
  /**
   * @brief 
   *
   * @return QColor
   */
  QColor labelColor() const { return mLabelColor; }
  /**
   * @brief 
   *
   * @return QString
   */
  QString label() const { return mLabel; }
  /**
   * @brief 
   *
   * @return int
   */
  int labelPadding() const;
  /**
   * @brief 
   *
   * @return int
   */
  int padding() const { return mPadding; }
  /**
   * @brief 
   *
   * @return int
   */
  int offset() const;
  /**
   * @brief 
   *
   * @return SelectableParts
   */
  SelectableParts selectedParts() const { return mSelectedParts; }
  /**
   * @brief 
   *
   * @return SelectableParts
   */
  SelectableParts selectableParts() const { return mSelectableParts; }
  /**
   * @brief 
   *
   * @return QFont
   */
  QFont selectedTickLabelFont() const { return mSelectedTickLabelFont; }
  /**
   * @brief 
   *
   * @return QFont
   */
  QFont selectedLabelFont() const { return mSelectedLabelFont; }
  /**
   * @brief 
   *
   * @return QColor
   */
  QColor selectedTickLabelColor() const { return mSelectedTickLabelColor; }
  /**
   * @brief 
   *
   * @return QColor
   */
  QColor selectedLabelColor() const { return mSelectedLabelColor; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen selectedBasePen() const { return mSelectedBasePen; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen selectedTickPen() const { return mSelectedTickPen; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen selectedSubTickPen() const { return mSelectedSubTickPen; }
  /**
   * @brief 
   *
   * @return QCPLineEnding
   */
  QCPLineEnding lowerEnding() const;
  /**
   * @brief 
   *
   * @return QCPLineEnding
   */
  QCPLineEnding upperEnding() const;
  /**
   * @brief 
   *
   * @return QCPGrid
   */
  QCPGrid *grid() const { return mGrid; }
  
  // setters:
  /**
   * @brief 
   *
   * @param type
   */
  Q_SLOT void setScaleType(QCPAxis::ScaleType type);
  /**
   * @brief 
   *
   * @param base
   */
  void setScaleLogBase(double base);
  /**
   * @brief 
   *
   * @param range
   */
  Q_SLOT void setRange(const QCPRange &range);
  /**
   * @brief 
   *
   * @param lower
   * @param upper
   */
  void setRange(double lower, double upper);
  /**
   * @brief 
   *
   * @param position
   * @param size
   * @param alignment
   */
  void setRange(double position, double size, Qt::AlignmentFlag alignment);
  /**
   * @brief 
   *
   * @param lower
   */
  void setRangeLower(double lower);
  /**
   * @brief 
   *
   * @param upper
   */
  void setRangeUpper(double upper);
  /**
   * @brief 
   *
   * @param reversed
   */
  void setRangeReversed(bool reversed);
  /**
   * @brief 
   *
   * @param on
   */
  void setAutoTicks(bool on);
  /**
   * @brief 
   *
   * @param approximateCount
   */
  void setAutoTickCount(int approximateCount);
  /**
   * @brief 
   *
   * @param on
   */
  void setAutoTickLabels(bool on);
  /**
   * @brief 
   *
   * @param on
   */
  void setAutoTickStep(bool on);
  /**
   * @brief 
   *
   * @param on
   */
  void setAutoSubTicks(bool on);
  /**
   * @brief 
   *
   * @param show
   */
  void setTicks(bool show);
  /**
   * @brief 
   *
   * @param show
   */
  void setTickLabels(bool show);
  /**
   * @brief 
   *
   * @param padding
   */
  void setTickLabelPadding(int padding);
  /**
   * @brief 
   *
   * @param type
   */
  void setTickLabelType(LabelType type);
  /**
   * @brief 
   *
   * @param font
   */
  void setTickLabelFont(const QFont &font);
  /**
   * @brief 
   *
   * @param color
   */
  void setTickLabelColor(const QColor &color);
  /**
   * @brief 
   *
   * @param degrees
   */
  void setTickLabelRotation(double degrees);
  /**
   * @brief 
   *
   * @param side
   */
  void setTickLabelSide(LabelSide side);
  /**
   * @brief 
   *
   * @param format
   */
  void setDateTimeFormat(const QString &format);
  /**
   * @brief 
   *
   * @param timeSpec
   */
  void setDateTimeSpec(const Qt::TimeSpec &timeSpec);
  /**
   * @brief 
   *
   * @param formatCode
   */
  void setNumberFormat(const QString &formatCode);
  /**
   * @brief 
   *
   * @param precision
   */
  void setNumberPrecision(int precision);
  /**
   * @brief 
   *
   * @param step
   */
  void setTickStep(double step);
  /**
   * @brief 
   *
   * @param vec
   */
  void setTickVector(const QVector<double> &vec);
  /**
   * @brief 
   *
   * @param vec
   */
  void setTickVectorLabels(const QVector<QString> &vec);
  /**
   * @brief 
   *
   * @param inside
   * @param outside
   */
  void setTickLength(int inside, int outside=0);
  /**
   * @brief 
   *
   * @param inside
   */
  void setTickLengthIn(int inside);
  /**
   * @brief 
   *
   * @param outside
   */
  void setTickLengthOut(int outside);
  /**
   * @brief 
   *
   * @param count
   */
  void setSubTickCount(int count);
  /**
   * @brief 
   *
   * @param inside
   * @param outside
   */
  void setSubTickLength(int inside, int outside=0);
  /**
   * @brief 
   *
   * @param inside
   */
  void setSubTickLengthIn(int inside);
  /**
   * @brief 
   *
   * @param outside
   */
  void setSubTickLengthOut(int outside);
  /**
   * @brief 
   *
   * @param pen
   */
  void setBasePen(const QPen &pen);
  /**
   * @brief 
   *
   * @param pen
   */
  void setTickPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param pen
   */
  void setSubTickPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param font
   */
  void setLabelFont(const QFont &font);
  /**
   * @brief 
   *
   * @param color
   */
  void setLabelColor(const QColor &color);
  /**
   * @brief 
   *
   * @param str
   */
  void setLabel(const QString &str);
  /**
   * @brief 
   *
   * @param padding
   */
  void setLabelPadding(int padding);
  /**
   * @brief 
   *
   * @param padding
   */
  void setPadding(int padding);
  /**
   * @brief 
   *
   * @param offset
   */
  void setOffset(int offset);
  /**
   * @brief 
   *
   * @param font
   */
  void setSelectedTickLabelFont(const QFont &font);
  /**
   * @brief 
   *
   * @param font
   */
  void setSelectedLabelFont(const QFont &font);
  /**
   * @brief 
   *
   * @param color
   */
  void setSelectedTickLabelColor(const QColor &color);
  /**
   * @brief 
   *
   * @param color
   */
  void setSelectedLabelColor(const QColor &color);
  /**
   * @brief 
   *
   * @param pen
   */
  void setSelectedBasePen(const QPen &pen);
  /**
   * @brief 
   *
   * @param pen
   */
  void setSelectedTickPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param pen
   */
  void setSelectedSubTickPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param selectableParts
   */
  Q_SLOT void setSelectableParts(const QCPAxis::SelectableParts &selectableParts);
  /**
   * @brief 
   *
   * @param selectedParts
   */
  Q_SLOT void setSelectedParts(const QCPAxis::SelectableParts &selectedParts);
  /**
   * @brief 
   *
   * @param ending
   */
  void setLowerEnding(const QCPLineEnding &ending);
  /**
   * @brief 
   *
   * @param ending
   */
  void setUpperEnding(const QCPLineEnding &ending);
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param pos
   * @param onlySelectable
   * @param details
   * @return double
   */
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
  // non-property methods:
  /**
   * @brief 
   *
   * @return Qt::Orientation
   */
  Qt::Orientation orientation() const { return mOrientation; }
  /**
   * @brief 
   *
   * @param diff
   */
  void moveRange(double diff);
  /**
   * @brief 
   *
   * @param factor
   * @param center
   */
  void scaleRange(double factor, double center);
  /**
   * @brief 
   *
   * @param otherAxis
   * @param ratio
   */
  void setScaleRatio(const QCPAxis *otherAxis, double ratio=1.0);
  /**
   * @brief 
   *
   * @param onlyVisiblePlottables
   */
  void rescale(bool onlyVisiblePlottables=false);
  /**
   * @brief 
   *
   * @param value
   * @return double
   */
  double pixelToCoord(double value) const;
  /**
   * @brief 
   *
   * @param value
   * @return double
   */
  double coordToPixel(double value) const;
  /**
   * @brief 
   *
   * @param pos
   * @return SelectablePart
   */
  SelectablePart getPartAt(const QPointF &pos) const;
  /**
   * @brief 
   *
   * @return QList<QCPAbstractPlottable *>
   */
  QList<QCPAbstractPlottable*> plottables() const;
  /**
   * @brief 
   *
   * @return QList<QCPGraph *>
   */
  QList<QCPGraph*> graphs() const;
  /**
   * @brief 
   *
   * @return QList<QCPAbstractItem *>
   */
  QList<QCPAbstractItem*> items() const;
  
  /**
   * @brief 
   *
   * @param side
   * @return AxisType
   */
  static AxisType marginSideToAxisType(QCP::MarginSide side);
  /**
   * @brief 
   *
   * @param type
   * @return Qt::Orientation
   */
  static Qt::Orientation orientation(AxisType type) { return type==atBottom||type==atTop ? Qt::Horizontal : Qt::Vertical; }
  /**
   * @brief 
   *
   * @param type
   * @return AxisType
   */
  static AxisType opposite(AxisType type);
  
signals:
  /**
   * @brief 
   *
   */
  void ticksRequest();
  /**
   * @brief 
   *
   * @param newRange
   */
  void rangeChanged(const QCPRange &newRange);
  /**
   * @brief 
   *
   * @param newRange
   * @param oldRange
   */
  void rangeChanged(const QCPRange &newRange, const QCPRange &oldRange);
  /**
   * @brief 
   *
   * @param scaleType
   */
  void scaleTypeChanged(QCPAxis::ScaleType scaleType);
  /**
   * @brief 
   *
   * @param parts
   */
  void selectionChanged(const QCPAxis::SelectableParts &parts);
  /**
   * @brief 
   *
   * @param parts
   */
  void selectableChanged(const QCPAxis::SelectableParts &parts);

protected:
  // property members:
  // axis base:
  AxisType mAxisType; /**< TODO: describe */
  QCPAxisRect *mAxisRect; /**< TODO: describe */
  //int mOffset; // in QCPAxisPainter
  int mPadding; /**< TODO: describe */
  Qt::Orientation mOrientation; /**< TODO: describe */
  SelectableParts mSelectableParts, mSelectedParts; /**< TODO: describe */
  QPen mBasePen, mSelectedBasePen; /**< TODO: describe */
  //QCPLineEnding mLowerEnding, mUpperEnding; // in QCPAxisPainter
  // axis label:
  //int mLabelPadding; // in QCPAxisPainter
  QString mLabel; /**< TODO: describe */
  QFont mLabelFont, mSelectedLabelFont; /**< TODO: describe */
  QColor mLabelColor, mSelectedLabelColor; /**< TODO: describe */
  // tick labels:
  //int mTickLabelPadding; // in QCPAxisPainter
  bool mTickLabels, mAutoTickLabels; /**< TODO: describe */
  //double mTickLabelRotation; // in QCPAxisPainter
  LabelType mTickLabelType; /**< TODO: describe */
  QFont mTickLabelFont, mSelectedTickLabelFont; /**< TODO: describe */
  QColor mTickLabelColor, mSelectedTickLabelColor; /**< TODO: describe */
  QString mDateTimeFormat; /**< TODO: describe */
  Qt::TimeSpec mDateTimeSpec; /**< TODO: describe */
  int mNumberPrecision; /**< TODO: describe */
  QLatin1Char mNumberFormatChar; /**< TODO: describe */
  bool mNumberBeautifulPowers; /**< TODO: describe */
  //bool mNumberMultiplyCross; // QCPAxisPainter
  // ticks and subticks:
  bool mTicks; /**< TODO: describe */
  double mTickStep; /**< TODO: describe */
  int mSubTickCount, mAutoTickCount; /**< TODO: describe */
  bool mAutoTicks, mAutoTickStep, mAutoSubTicks; /**< TODO: describe */
  //int mTickLengthIn, mTickLengthOut, mSubTickLengthIn, mSubTickLengthOut; // QCPAxisPainter
  QPen mTickPen, mSelectedTickPen; /**< TODO: describe */
  QPen mSubTickPen, mSelectedSubTickPen; /**< TODO: describe */
  // scale and range:
  QCPRange mRange; /**< TODO: describe */
  bool mRangeReversed; /**< TODO: describe */
  ScaleType mScaleType; /**< TODO: describe */
  double mScaleLogBase, mScaleLogBaseLogInv; /**< TODO: describe */
  
  // non-property members:
  QCPGrid *mGrid; /**< TODO: describe */
  QCPAxisPainterPrivate *mAxisPainter; /**< TODO: describe */
  int mLowestVisibleTick, mHighestVisibleTick; /**< TODO: describe */
  QVector<double> mTickVector; /**< TODO: describe */
  QVector<QString> mTickVectorLabels; /**< TODO: describe */
  QVector<double> mSubTickVector; /**< TODO: describe */
  bool mCachedMarginValid; /**< TODO: describe */
  int mCachedMargin; /**< TODO: describe */
  
  // introduced virtual methods:
  /**
   * @brief 
   *
   */
  virtual void setupTickVectors();
  /**
   * @brief 
   *
   */
  virtual void generateAutoTicks();
  /**
   * @brief 
   *
   * @param tickStep
   * @return int
   */
  virtual int calculateAutoSubTickCount(double tickStep) const;
  /**
   * @brief 
   *
   * @return int
   */
  virtual int calculateMargin();
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter);
  /**
   * @brief 
   *
   * @return QCP::Interaction
   */
  virtual QCP::Interaction selectionCategory() const;
  // events:
  /**
   * @brief 
   *
   * @param event
   * @param additive
   * @param details
   * @param selectionStateChanged
   */
  virtual void selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged);
  /**
   * @brief 
   *
   * @param selectionStateChanged
   */
  virtual void deselectEvent(bool *selectionStateChanged);
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @param lowIndex
   * @param highIndex
   */
  void visibleTickBounds(int &lowIndex, int &highIndex) const;
  /**
   * @brief 
   *
   * @param value
   * @return double
   */
  double baseLog(double value) const;
  /**
   * @brief 
   *
   * @param value
   * @return double
   */
  double basePow(double value) const;
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen getBasePen() const;
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen getTickPen() const;
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen getSubTickPen() const;
  /**
   * @brief 
   *
   * @return QFont
   */
  QFont getTickLabelFont() const;
  /**
   * @brief 
   *
   * @return QFont
   */
  QFont getLabelFont() const;
  /**
   * @brief 
   *
   * @return QColor
   */
  QColor getTickLabelColor() const;
  /**
   * @brief 
   *
   * @return QColor
   */
  QColor getLabelColor() const;
  
private:
  /**
   * @brief 
   *
   * @param 
   */
  Q_DISABLE_COPY(QCPAxis)
  
  friend class QCustomPlot;
  friend class QCPGrid;
  friend class QCPAxisRect;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QCPAxis::SelectableParts)
Q_DECLARE_OPERATORS_FOR_FLAGS(QCPAxis::AxisTypes)
Q_DECLARE_METATYPE(QCPAxis::SelectablePart)


/**
 * @brief 
 *
 */
class QCPAxisPainterPrivate
{
public:
  /**
   * @brief 
   *
   * @param parentPlot
   */
  explicit QCPAxisPainterPrivate(QCustomPlot *parentPlot);
  /**
   * @brief 
   *
   */
  virtual ~QCPAxisPainterPrivate();
  
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter);
  /**
   * @brief 
   *
   * @return int
   */
  virtual int size() const;
  /**
   * @brief 
   *
   */
  void clearCache();
  
  /**
   * @brief 
   *
   * @return QRect
   */
  QRect axisSelectionBox() const { return mAxisSelectionBox; }
  /**
   * @brief 
   *
   * @return QRect
   */
  QRect tickLabelsSelectionBox() const { return mTickLabelsSelectionBox; }
  /**
   * @brief 
   *
   * @return QRect
   */
  QRect labelSelectionBox() const { return mLabelSelectionBox; }
  
  // public property members:
  QCPAxis::AxisType type; /**< TODO: describe */
  QPen basePen; /**< TODO: describe */
  QCPLineEnding lowerEnding, upperEnding; // directly accessed by QCPAxis setters/getters /**< TODO: describe */
  int labelPadding; // directly accessed by QCPAxis setters/getters /**< TODO: describe */
  QFont labelFont; /**< TODO: describe */
  QColor labelColor; /**< TODO: describe */
  QString label; /**< TODO: describe */
  int tickLabelPadding; // directly accessed by QCPAxis setters/getters /**< TODO: describe */
  double tickLabelRotation; // directly accessed by QCPAxis setters/getters /**< TODO: describe */
  QCPAxis::LabelSide tickLabelSide; // directly accessed by QCPAxis setters/getters /**< TODO: describe */
  bool substituteExponent; /**< TODO: describe */
  bool numberMultiplyCross; // directly accessed by QCPAxis setters/getters /**< TODO: describe */
  int tickLengthIn, tickLengthOut, subTickLengthIn, subTickLengthOut; // directly accessed by QCPAxis setters/getters /**< TODO: describe */
  QPen tickPen, subTickPen; /**< TODO: describe */
  QFont tickLabelFont; /**< TODO: describe */
  QColor tickLabelColor; /**< TODO: describe */
  QRect axisRect, viewportRect; /**< TODO: describe */
  double offset; // directly accessed by QCPAxis setters/getters /**< TODO: describe */
  bool abbreviateDecimalPowers; /**< TODO: describe */
  bool reversedEndings; /**< TODO: describe */
  
  QVector<double> subTickPositions; /**< TODO: describe */
  QVector<double> tickPositions; /**< TODO: describe */
  QVector<QString> tickLabels; /**< TODO: describe */
  
protected:
  /**
   * @brief 
   *
   */
  struct CachedLabel
  {
    QPointF offset; /**< TODO: describe */
    QPixmap pixmap; /**< TODO: describe */
  };
  /**
   * @brief 
   *
   */
  struct TickLabelData
  {
    QString basePart, expPart; /**< TODO: describe */
    QRect baseBounds, expBounds, totalBounds, rotatedTotalBounds; /**< TODO: describe */
    QFont baseFont, expFont; /**< TODO: describe */
  };
  QCustomPlot *mParentPlot; /**< TODO: describe */
  QByteArray mLabelParameterHash; // to determine whether mLabelCache needs to be cleared due to changed parameters /**< TODO: describe */
  QCache<QString, CachedLabel> mLabelCache; /**< TODO: describe */
  QRect mAxisSelectionBox, mTickLabelsSelectionBox, mLabelSelectionBox; /**< TODO: describe */
  
  /**
   * @brief 
   *
   * @return QByteArray
   */
  virtual QByteArray generateLabelParameterHash() const;
  
  /**
   * @brief 
   *
   * @param painter
   * @param position
   * @param distanceToAxis
   * @param text
   * @param tickLabelsSize
   */
  virtual void placeTickLabel(QCPPainter *painter, double position, int distanceToAxis, const QString &text, QSize *tickLabelsSize);
  /**
   * @brief 
   *
   * @param painter
   * @param x
   * @param y
   * @param labelData
   */
  virtual void drawTickLabel(QCPPainter *painter, double x, double y, const TickLabelData &labelData) const;
  /**
   * @brief 
   *
   * @param font
   * @param text
   * @return TickLabelData
   */
  virtual TickLabelData getTickLabelData(const QFont &font, const QString &text) const;
  /**
   * @brief 
   *
   * @param labelData
   * @return QPointF
   */
  virtual QPointF getTickLabelDrawOffset(const TickLabelData &labelData) const;
  /**
   * @brief 
   *
   * @param font
   * @param text
   * @param tickLabelsSize
   */
  virtual void getMaxTickLabelSize(const QFont &font, const QString &text, QSize *tickLabelsSize) const;
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPAbstractPlottable : public QCPLayerable
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(QString name READ name WRITE setName)
  Q_PROPERTY(bool antialiasedFill READ antialiasedFill WRITE setAntialiasedFill)
  Q_PROPERTY(bool antialiasedScatters READ antialiasedScatters WRITE setAntialiasedScatters)
  Q_PROPERTY(bool antialiasedErrorBars READ antialiasedErrorBars WRITE setAntialiasedErrorBars)
  Q_PROPERTY(QPen pen READ pen WRITE setPen)
  Q_PROPERTY(QPen selectedPen READ selectedPen WRITE setSelectedPen)
  Q_PROPERTY(QBrush brush READ brush WRITE setBrush)
  Q_PROPERTY(QBrush selectedBrush READ selectedBrush WRITE setSelectedBrush)
  Q_PROPERTY(QCPAxis* keyAxis READ keyAxis WRITE setKeyAxis)
  Q_PROPERTY(QCPAxis* valueAxis READ valueAxis WRITE setValueAxis)
  Q_PROPERTY(bool selectable READ selectable WRITE setSelectable NOTIFY selectableChanged)
  Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectionChanged)
  /// \endcond
public:
  /**
   * @brief 
   *
   * @param keyAxis
   * @param valueAxis
   */
  QCPAbstractPlottable(QCPAxis *keyAxis, QCPAxis *valueAxis);
  
  // getters:
  /**
   * @brief 
   *
   * @return QString
   */
  QString name() const { return mName; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool antialiasedFill() const { return mAntialiasedFill; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool antialiasedScatters() const { return mAntialiasedScatters; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool antialiasedErrorBars() const { return mAntialiasedErrorBars; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen pen() const { return mPen; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen selectedPen() const { return mSelectedPen; }
  /**
   * @brief 
   *
   * @return QBrush
   */
  QBrush brush() const { return mBrush; }
  /**
   * @brief 
   *
   * @return QBrush
   */
  QBrush selectedBrush() const { return mSelectedBrush; }
  /**
   * @brief 
   *
   * @return QCPAxis
   */
  QCPAxis *keyAxis() const { return mKeyAxis.data(); }
  /**
   * @brief 
   *
   * @return QCPAxis
   */
  QCPAxis *valueAxis() const { return mValueAxis.data(); }
  /**
   * @brief 
   *
   * @return bool
   */
  bool selectable() const { return mSelectable; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool selected() const { return mSelected; }
  
  // setters:
  /**
   * @brief 
   *
   * @param name
   */
  void setName(const QString &name);
  /**
   * @brief 
   *
   * @param enabled
   */
  void setAntialiasedFill(bool enabled);
  /**
   * @brief 
   *
   * @param enabled
   */
  void setAntialiasedScatters(bool enabled);
  /**
   * @brief 
   *
   * @param enabled
   */
  void setAntialiasedErrorBars(bool enabled);
  /**
   * @brief 
   *
   * @param pen
   */
  void setPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param pen
   */
  void setSelectedPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param brush
   */
  void setBrush(const QBrush &brush);
  /**
   * @brief 
   *
   * @param brush
   */
  void setSelectedBrush(const QBrush &brush);
  /**
   * @brief 
   *
   * @param axis
   */
  void setKeyAxis(QCPAxis *axis);
  /**
   * @brief 
   *
   * @param axis
   */
  void setValueAxis(QCPAxis *axis);
  /**
   * @brief 
   *
   * @param selectable
   */
  Q_SLOT void setSelectable(bool selectable);
  /**
   * @brief 
   *
   * @param selected
   */
  Q_SLOT void setSelected(bool selected);

  // introduced virtual methods:
  /**
   * @brief 
   *
   */
  virtual void clearData() = 0;
  /**
   * @brief 
   *
   * @param pos
   * @param onlySelectable
   * @param details
   * @return double
   */
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const = 0;
  /**
   * @brief 
   *
   * @return bool
   */
  virtual bool addToLegend();
  /**
   * @brief 
   *
   * @return bool
   */
  virtual bool removeFromLegend() const;
  
  // non-property methods:
  /**
   * @brief 
   *
   * @param onlyEnlarge
   */
  void rescaleAxes(bool onlyEnlarge=false) const;
  /**
   * @brief 
   *
   * @param onlyEnlarge
   */
  void rescaleKeyAxis(bool onlyEnlarge=false) const;
  /**
   * @brief 
   *
   * @param onlyEnlarge
   */
  void rescaleValueAxis(bool onlyEnlarge=false) const;
  
signals:
  /**
   * @brief 
   *
   * @param selected
   */
  void selectionChanged(bool selected);
  /**
   * @brief 
   *
   * @param selectable
   */
  void selectableChanged(bool selectable);
  
protected:
  /*!
    Represents negative and positive sign domain for passing to \ref getKeyRange and \ref getValueRange.
  */
  enum SignDomain { sdNegative  ///< The negative sign domain, i.e. numbers smaller than zero
                    ,sdBoth     ///< Both sign domains, including zero, i.e. all (rational) numbers
                    ,sdPositive ///< The positive sign domain, i.e. numbers greater than zero
                  };
  
  // property members:
  QString mName; /**< TODO: describe */
  bool mAntialiasedFill, mAntialiasedScatters, mAntialiasedErrorBars; /**< TODO: describe */
  QPen mPen, mSelectedPen; /**< TODO: describe */
  QBrush mBrush, mSelectedBrush; /**< TODO: describe */
  QPointer<QCPAxis> mKeyAxis, mValueAxis; /**< TODO: describe */
  bool mSelectable, mSelected; /**< TODO: describe */
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @return QRect
   */
  virtual QRect clipRect() const;
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter) = 0;
  /**
   * @brief 
   *
   * @return QCP::Interaction
   */
  virtual QCP::Interaction selectionCategory() const;
  /**
   * @brief 
   *
   * @param painter
   */
  void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  // events:
  /**
   * @brief 
   *
   * @param event
   * @param additive
   * @param details
   * @param selectionStateChanged
   */
  virtual void selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged);
  /**
   * @brief 
   *
   * @param selectionStateChanged
   */
  virtual void deselectEvent(bool *selectionStateChanged);
  
  // introduced virtual methods:
  /**
   * @brief 
   *
   * @param painter
   * @param rect
   */
  virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const = 0;
  /**
   * @brief 
   *
   * @param foundRange
   * @param inSignDomain
   * @return QCPRange
   */
  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const = 0;
  /**
   * @brief 
   *
   * @param foundRange
   * @param inSignDomain
   * @return QCPRange
   */
  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const = 0;
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @param key
   * @param value
   * @param x
   * @param y
   */
  void coordsToPixels(double key, double value, double &x, double &y) const;
  /**
   * @brief 
   *
   * @param key
   * @param value
   * @return const QPointF
   */
  const QPointF coordsToPixels(double key, double value) const;
  /**
   * @brief 
   *
   * @param x
   * @param y
   * @param key
   * @param value
   */
  void pixelsToCoords(double x, double y, double &key, double &value) const;
  /**
   * @brief 
   *
   * @param pixelPos
   * @param key
   * @param value
   */
  void pixelsToCoords(const QPointF &pixelPos, double &key, double &value) const;
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen mainPen() const;
  /**
   * @brief 
   *
   * @return QBrush
   */
  QBrush mainBrush() const;
  /**
   * @brief 
   *
   * @param painter
   */
  void applyFillAntialiasingHint(QCPPainter *painter) const;
  /**
   * @brief 
   *
   * @param painter
   */
  void applyScattersAntialiasingHint(QCPPainter *painter) const;
  /**
   * @brief 
   *
   * @param painter
   */
  void applyErrorBarsAntialiasingHint(QCPPainter *painter) const;
  /**
   * @brief 
   *
   * @param start
   * @param end
   * @param point
   * @return double
   */
  double distSqrToLine(const QPointF &start, const QPointF &end, const QPointF &point) const;

private:
  /**
   * @brief 
   *
   * @param 
   */
  Q_DISABLE_COPY(QCPAbstractPlottable)
  
  friend class QCustomPlot;
  friend class QCPAxis;
  friend class QCPPlottableLegendItem;
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPItemAnchor
{
public:
  /**
   * @brief 
   *
   * @param parentPlot
   * @param parentItem
   * @param name
   * @param anchorId
   */
  QCPItemAnchor(QCustomPlot *parentPlot, QCPAbstractItem *parentItem, const QString name, int anchorId=-1);
  /**
   * @brief 
   *
   */
  virtual ~QCPItemAnchor();
  
  // getters:
  /**
   * @brief 
   *
   * @return QString
   */
  QString name() const { return mName; }
  /**
   * @brief 
   *
   * @return QPointF
   */
  virtual QPointF pixelPoint() const;
  
protected:
  // property members:
  QString mName; /**< TODO: describe */
  
  // non-property members:
  QCustomPlot *mParentPlot; /**< TODO: describe */
  QCPAbstractItem *mParentItem; /**< TODO: describe */
  int mAnchorId; /**< TODO: describe */
  QSet<QCPItemPosition*> mChildrenX, mChildrenY; /**< TODO: describe */
  
  // introduced virtual methods:
  /**
   * @brief 
   *
   * @return QCPItemPosition
   */
  virtual QCPItemPosition *toQCPItemPosition() { return 0; }
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @param pos
   */
  void addChildX(QCPItemPosition* pos); // called from pos when this anchor is set as parent
  /**
   * @brief 
   *
   * @param pos
   */
  void removeChildX(QCPItemPosition *pos); // called from pos when its parent anchor is reset or pos deleted
  /**
   * @brief 
   *
   * @param pos
   */
  void addChildY(QCPItemPosition* pos); // called from pos when this anchor is set as parent
  /**
   * @brief 
   *
   * @param pos
   */
  void removeChildY(QCPItemPosition *pos); // called from pos when its parent anchor is reset or pos deleted
  
private:
  /**
   * @brief 
   *
   * @param 
   */
  Q_DISABLE_COPY(QCPItemAnchor)
  
  friend class QCPItemPosition;
};



/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPItemPosition : public QCPItemAnchor
{
public:
  /*!
    Defines the ways an item position can be specified. Thus it defines what the numbers passed to
    \ref setCoords actually mean.
    
    \see setType
  */
  enum PositionType { ptAbsolute        ///< Static positioning in pixels, starting from the top left corner of the viewport/widget.
                      ,ptViewportRatio  ///< Static positioning given by a fraction of the viewport size. For example, if you call setCoords(0, 0), the position will be at the top
                                        ///< left corner of the viewport/widget. setCoords(1, 1) will be at the bottom right corner, setCoords(0.5, 0) will be horizontally centered and
                                        ///< vertically at the top of the viewport/widget, etc.
                      ,ptAxisRectRatio  ///< Static positioning given by a fraction of the axis rect size (see \ref setAxisRect). For example, if you call setCoords(0, 0), the position will be at the top
                                        ///< left corner of the axis rect. setCoords(1, 1) will be at the bottom right corner, setCoords(0.5, 0) will be horizontally centered and
                                        ///< vertically at the top of the axis rect, etc. You can also go beyond the axis rect by providing negative coordinates or coordinates larger than 1.
                      ,ptPlotCoords     ///< Dynamic positioning at a plot coordinate defined by two axes (see \ref setAxes).
                    };
  
  /**
   * @brief 
   *
   * @param parentPlot
   * @param parentItem
   * @param name
   */
  QCPItemPosition(QCustomPlot *parentPlot, QCPAbstractItem *parentItem, const QString name);
  /**
   * @brief 
   *
   */
  virtual ~QCPItemPosition();
  
  // getters:
  /**
   * @brief 
   *
   * @return PositionType
   */
  PositionType type() const { return typeX(); }
  /**
   * @brief 
   *
   * @return PositionType
   */
  PositionType typeX() const { return mPositionTypeX; }
  /**
   * @brief 
   *
   * @return PositionType
   */
  PositionType typeY() const { return mPositionTypeY; }
  /**
   * @brief 
   *
   * @return QCPItemAnchor
   */
  QCPItemAnchor *parentAnchor() const { return parentAnchorX(); }
  /**
   * @brief 
   *
   * @return QCPItemAnchor
   */
  QCPItemAnchor *parentAnchorX() const { return mParentAnchorX; }
  /**
   * @brief 
   *
   * @return QCPItemAnchor
   */
  QCPItemAnchor *parentAnchorY() const { return mParentAnchorY; }
  /**
   * @brief 
   *
   * @return double
   */
  double key() const { return mKey; }
  /**
   * @brief 
   *
   * @return double
   */
  double value() const { return mValue; }
  /**
   * @brief 
   *
   * @return QPointF
   */
  QPointF coords() const { return QPointF(mKey, mValue); }
  /**
   * @brief 
   *
   * @return QCPAxis
   */
  QCPAxis *keyAxis() const { return mKeyAxis.data(); }
  /**
   * @brief 
   *
   * @return QCPAxis
   */
  QCPAxis *valueAxis() const { return mValueAxis.data(); }
  /**
   * @brief 
   *
   * @return QCPAxisRect
   */
  QCPAxisRect *axisRect() const;
  /**
   * @brief 
   *
   * @return QPointF
   */
  virtual QPointF pixelPoint() const;
  
  // setters:
  /**
   * @brief 
   *
   * @param type
   */
  void setType(PositionType type);
  /**
   * @brief 
   *
   * @param type
   */
  void setTypeX(PositionType type);
  /**
   * @brief 
   *
   * @param type
   */
  void setTypeY(PositionType type);
  /**
   * @brief 
   *
   * @param parentAnchor
   * @param keepPixelPosition
   * @return bool
   */
  bool setParentAnchor(QCPItemAnchor *parentAnchor, bool keepPixelPosition=false);
  /**
   * @brief 
   *
   * @param parentAnchor
   * @param keepPixelPosition
   * @return bool
   */
  bool setParentAnchorX(QCPItemAnchor *parentAnchor, bool keepPixelPosition=false);
  /**
   * @brief 
   *
   * @param parentAnchor
   * @param keepPixelPosition
   * @return bool
   */
  bool setParentAnchorY(QCPItemAnchor *parentAnchor, bool keepPixelPosition=false);
  /**
   * @brief 
   *
   * @param key
   * @param value
   */
  void setCoords(double key, double value);
  /**
   * @brief 
   *
   * @param coords
   */
  void setCoords(const QPointF &coords);
  /**
   * @brief 
   *
   * @param keyAxis
   * @param valueAxis
   */
  void setAxes(QCPAxis* keyAxis, QCPAxis* valueAxis);
  /**
   * @brief 
   *
   * @param axisRect
   */
  void setAxisRect(QCPAxisRect *axisRect);
  /**
   * @brief 
   *
   * @param pixelPoint
   */
  void setPixelPoint(const QPointF &pixelPoint);
  
protected:
  // property members:
  PositionType mPositionTypeX, mPositionTypeY; /**< TODO: describe */
  QPointer<QCPAxis> mKeyAxis, mValueAxis; /**< TODO: describe */
  QPointer<QCPAxisRect> mAxisRect; /**< TODO: describe */
  double mKey, mValue; /**< TODO: describe */
  QCPItemAnchor *mParentAnchorX, *mParentAnchorY; /**< TODO: describe */
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @return QCPItemPosition
   */
  virtual QCPItemPosition *toQCPItemPosition() { return this; }
  
private:
  /**
   * @brief 
   *
   * @param 
   */
  Q_DISABLE_COPY(QCPItemPosition)
  
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPAbstractItem : public QCPLayerable
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(bool clipToAxisRect READ clipToAxisRect WRITE setClipToAxisRect)
  Q_PROPERTY(QCPAxisRect* clipAxisRect READ clipAxisRect WRITE setClipAxisRect)
  Q_PROPERTY(bool selectable READ selectable WRITE setSelectable NOTIFY selectableChanged)
  Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectionChanged)
  /// \endcond
public:
  /**
   * @brief 
   *
   * @param parentPlot
   */
  QCPAbstractItem(QCustomPlot *parentPlot);
  /**
   * @brief 
   *
   */
  virtual ~QCPAbstractItem();
  
  // getters:
  /**
   * @brief 
   *
   * @return bool
   */
  bool clipToAxisRect() const { return mClipToAxisRect; }
  /**
   * @brief 
   *
   * @return QCPAxisRect
   */
  QCPAxisRect *clipAxisRect() const;
  /**
   * @brief 
   *
   * @return bool
   */
  bool selectable() const { return mSelectable; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool selected() const { return mSelected; }
  
  // setters:
  /**
   * @brief 
   *
   * @param clip
   */
  void setClipToAxisRect(bool clip);
  /**
   * @brief 
   *
   * @param rect
   */
  void setClipAxisRect(QCPAxisRect *rect);
  /**
   * @brief 
   *
   * @param selectable
   */
  Q_SLOT void setSelectable(bool selectable);
  /**
   * @brief 
   *
   * @param selected
   */
  Q_SLOT void setSelected(bool selected);
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param pos
   * @param onlySelectable
   * @param details
   * @return double
   */
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const = 0;
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @return QList<QCPItemPosition *>
   */
  QList<QCPItemPosition*> positions() const { return mPositions; }
  /**
   * @brief 
   *
   * @return QList<QCPItemAnchor *>
   */
  QList<QCPItemAnchor*> anchors() const { return mAnchors; }
  /**
   * @brief 
   *
   * @param name
   * @return QCPItemPosition
   */
  QCPItemPosition *position(const QString &name) const;
  /**
   * @brief 
   *
   * @param name
   * @return QCPItemAnchor
   */
  QCPItemAnchor *anchor(const QString &name) const;
  /**
   * @brief 
   *
   * @param name
   * @return bool
   */
  bool hasAnchor(const QString &name) const;
  
signals:
  /**
   * @brief 
   *
   * @param selected
   */
  void selectionChanged(bool selected);
  /**
   * @brief 
   *
   * @param selectable
   */
  void selectableChanged(bool selectable);
  
protected:
  // property members:
  bool mClipToAxisRect; /**< TODO: describe */
  QPointer<QCPAxisRect> mClipAxisRect; /**< TODO: describe */
  QList<QCPItemPosition*> mPositions; /**< TODO: describe */
  QList<QCPItemAnchor*> mAnchors; /**< TODO: describe */
  bool mSelectable, mSelected; /**< TODO: describe */
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @return QCP::Interaction
   */
  virtual QCP::Interaction selectionCategory() const;
  /**
   * @brief 
   *
   * @return QRect
   */
  virtual QRect clipRect() const;
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter) = 0;
  // events:
  /**
   * @brief 
   *
   * @param event
   * @param additive
   * @param details
   * @param selectionStateChanged
   */
  virtual void selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged);
  /**
   * @brief 
   *
   * @param selectionStateChanged
   */
  virtual void deselectEvent(bool *selectionStateChanged);
  
  // introduced virtual methods:
  /**
   * @brief 
   *
   * @param anchorId
   * @return QPointF
   */
  virtual QPointF anchorPixelPoint(int anchorId) const;
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @param start
   * @param end
   * @param point
   * @return double
   */
  double distSqrToLine(const QPointF &start, const QPointF &end, const QPointF &point) const;
  /**
   * @brief 
   *
   * @param rect
   * @param pos
   * @param filledRect
   * @return double
   */
  double rectSelectTest(const QRectF &rect, const QPointF &pos, bool filledRect) const;
  /**
   * @brief 
   *
   * @param name
   * @return QCPItemPosition
   */
  QCPItemPosition *createPosition(const QString &name);
  /**
   * @brief 
   *
   * @param name
   * @param anchorId
   * @return QCPItemAnchor
   */
  QCPItemAnchor *createAnchor(const QString &name, int anchorId);
  
private:
  /**
   * @brief 
   *
   * @param 
   */
  Q_DISABLE_COPY(QCPAbstractItem)
  
  friend class QCustomPlot;
  friend class QCPItemAnchor;
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCustomPlot : public QWidget
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(QRect viewport READ viewport WRITE setViewport)
  Q_PROPERTY(QPixmap background READ background WRITE setBackground)
  Q_PROPERTY(bool backgroundScaled READ backgroundScaled WRITE setBackgroundScaled)
  Q_PROPERTY(Qt::AspectRatioMode backgroundScaledMode READ backgroundScaledMode WRITE setBackgroundScaledMode)
  Q_PROPERTY(QCPLayoutGrid* plotLayout READ plotLayout)
  Q_PROPERTY(bool autoAddPlottableToLegend READ autoAddPlottableToLegend WRITE setAutoAddPlottableToLegend)
  Q_PROPERTY(int selectionTolerance READ selectionTolerance WRITE setSelectionTolerance)
  Q_PROPERTY(bool noAntialiasingOnDrag READ noAntialiasingOnDrag WRITE setNoAntialiasingOnDrag)
  Q_PROPERTY(Qt::KeyboardModifier multiSelectModifier READ multiSelectModifier WRITE setMultiSelectModifier)
  /// \endcond
public:
  /*!
    Defines how a layer should be inserted relative to an other layer.

    \see addLayer, moveLayer
  */
  enum LayerInsertMode { limBelow  ///< Layer is inserted below other layer
                         ,limAbove ///< Layer is inserted above other layer
                       };
  Q_ENUMS(LayerInsertMode)
  
  /*!
    Defines with what timing the QCustomPlot surface is refreshed after a replot.

    \see replot
  */
  enum RefreshPriority { rpImmediate ///< The QCustomPlot surface is immediately refreshed, by calling QWidget::repaint() after the replot
                         ,rpQueued   ///< Queues the refresh such that it is performed at a slightly delayed point in time after the replot, by calling QWidget::update() after the replot
                         ,rpHint     ///< Whether to use immediate repaint or queued update depends on whether the plotting hint \ref QCP::phForceRepaint is set, see \ref setPlottingHints.
                       };
  
  /**
   * @brief 
   *
   * @param parent
   */
  explicit QCustomPlot(QWidget *parent = 0);
  /**
   * @brief 
   *
   */
  virtual ~QCustomPlot();
  
  // getters:
  /**
   * @brief 
   *
   * @return QRect
   */
  QRect viewport() const { return mViewport; }
  /**
   * @brief 
   *
   * @return QPixmap
   */
  QPixmap background() const { return mBackgroundPixmap; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool backgroundScaled() const { return mBackgroundScaled; }
  /**
   * @brief 
   *
   * @return Qt::AspectRatioMode
   */
  Qt::AspectRatioMode backgroundScaledMode() const { return mBackgroundScaledMode; }
  /**
   * @brief 
   *
   * @return QCPLayoutGrid
   */
  QCPLayoutGrid *plotLayout() const { return mPlotLayout; }
  /**
   * @brief 
   *
   * @return QCP::AntialiasedElements
   */
  QCP::AntialiasedElements antialiasedElements() const { return mAntialiasedElements; }
  /**
   * @brief 
   *
   * @return QCP::AntialiasedElements
   */
  QCP::AntialiasedElements notAntialiasedElements() const { return mNotAntialiasedElements; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool autoAddPlottableToLegend() const { return mAutoAddPlottableToLegend; }
  /**
   * @brief 
   *
   * @return const QCP::Interactions
   */
  const QCP::Interactions interactions() const { return mInteractions; }
  /**
   * @brief 
   *
   * @return int
   */
  int selectionTolerance() const { return mSelectionTolerance; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool noAntialiasingOnDrag() const { return mNoAntialiasingOnDrag; }
  /**
   * @brief 
   *
   * @return QCP::PlottingHints
   */
  QCP::PlottingHints plottingHints() const { return mPlottingHints; }
  /**
   * @brief 
   *
   * @return Qt::KeyboardModifier
   */
  Qt::KeyboardModifier multiSelectModifier() const { return mMultiSelectModifier; }

  // setters:
  /**
   * @brief 
   *
   * @param rect
   */
  void setViewport(const QRect &rect);
  /**
   * @brief 
   *
   * @param pm
   */
  void setBackground(const QPixmap &pm);
  /**
   * @brief 
   *
   * @param pm
   * @param scaled
   * @param mode
   */
  void setBackground(const QPixmap &pm, bool scaled, Qt::AspectRatioMode mode=Qt::KeepAspectRatioByExpanding);
  /**
   * @brief 
   *
   * @param brush
   */
  void setBackground(const QBrush &brush);
  /**
   * @brief 
   *
   * @param scaled
   */
  void setBackgroundScaled(bool scaled);
  /**
   * @brief 
   *
   * @param mode
   */
  void setBackgroundScaledMode(Qt::AspectRatioMode mode);
  /**
   * @brief 
   *
   * @param antialiasedElements
   */
  void setAntialiasedElements(const QCP::AntialiasedElements &antialiasedElements);
  /**
   * @brief 
   *
   * @param antialiasedElement
   * @param enabled
   */
  void setAntialiasedElement(QCP::AntialiasedElement antialiasedElement, bool enabled=true);
  /**
   * @brief 
   *
   * @param notAntialiasedElements
   */
  void setNotAntialiasedElements(const QCP::AntialiasedElements &notAntialiasedElements);
  /**
   * @brief 
   *
   * @param notAntialiasedElement
   * @param enabled
   */
  void setNotAntialiasedElement(QCP::AntialiasedElement notAntialiasedElement, bool enabled=true);
  /**
   * @brief 
   *
   * @param on
   */
  void setAutoAddPlottableToLegend(bool on);
  /**
   * @brief 
   *
   * @param interactions
   */
  void setInteractions(const QCP::Interactions &interactions);
  /**
   * @brief 
   *
   * @param interaction
   * @param enabled
   */
  void setInteraction(const QCP::Interaction &interaction, bool enabled=true);
  /**
   * @brief 
   *
   * @param pixels
   */
  void setSelectionTolerance(int pixels);
  /**
   * @brief 
   *
   * @param enabled
   */
  void setNoAntialiasingOnDrag(bool enabled);
  /**
   * @brief 
   *
   * @param hints
   */
  void setPlottingHints(const QCP::PlottingHints &hints);
  /**
   * @brief 
   *
   * @param hint
   * @param enabled
   */
  void setPlottingHint(QCP::PlottingHint hint, bool enabled=true);
  /**
   * @brief 
   *
   * @param modifier
   */
  void setMultiSelectModifier(Qt::KeyboardModifier modifier);
  
  // non-property methods:
  // plottable interface:
  /**
   * @brief 
   *
   * @param index
   * @return QCPAbstractPlottable
   */
  QCPAbstractPlottable *plottable(int index);
  /**
   * @brief 
   *
   * @return QCPAbstractPlottable
   */
  QCPAbstractPlottable *plottable();
  /**
   * @brief 
   *
   * @param plottable
   * @return bool
   */
  bool addPlottable(QCPAbstractPlottable *plottable);
  /**
   * @brief 
   *
   * @param plottable
   * @return bool
   */
  bool removePlottable(QCPAbstractPlottable *plottable);
  /**
   * @brief 
   *
   * @param index
   * @return bool
   */
  bool removePlottable(int index);
  /**
   * @brief 
   *
   * @return int
   */
  int clearPlottables();
  /**
   * @brief 
   *
   * @return int
   */
  int plottableCount() const;
  /**
   * @brief 
   *
   * @return QList<QCPAbstractPlottable *>
   */
  QList<QCPAbstractPlottable*> selectedPlottables() const;
  /**
   * @brief 
   *
   * @param pos
   * @param onlySelectable
   * @return QCPAbstractPlottable
   */
  QCPAbstractPlottable *plottableAt(const QPointF &pos, bool onlySelectable=false) const;
  /**
   * @brief 
   *
   * @param plottable
   * @return bool
   */
  bool hasPlottable(QCPAbstractPlottable *plottable) const;
 
  // specialized interface for QCPGraph:
  /**
   * @brief 
   *
   * @param index
   * @return QCPGraph
   */
  QCPGraph *graph(int index) const;
  /**
   * @brief 
   *
   * @return QCPGraph
   */
  QCPGraph *graph() const;
  /**
   * @brief 
   *
   * @param keyAxis
   * @param valueAxis
   * @return QCPGraph
   */
  QCPGraph *addGraph(QCPAxis *keyAxis=0, QCPAxis *valueAxis=0);
  /**
   * @brief 
   *
   * @param graph
   * @return bool
   */
  bool removeGraph(QCPGraph *graph);
  /**
   * @brief 
   *
   * @param index
   * @return bool
   */
  bool removeGraph(int index);
  /**
   * @brief 
   *
   * @return int
   */
  int clearGraphs();
  /**
   * @brief 
   *
   * @return int
   */
  int graphCount() const;
  /**
   * @brief 
   *
   * @return QList<QCPGraph *>
   */
  QList<QCPGraph*> selectedGraphs() const;

  // item interface:
  /**
   * @brief 
   *
   * @param index
   * @return QCPAbstractItem
   */
  QCPAbstractItem *item(int index) const;
  /**
   * @brief 
   *
   * @return QCPAbstractItem
   */
  QCPAbstractItem *item() const;
  /**
   * @brief 
   *
   * @param item
   * @return bool
   */
  bool addItem(QCPAbstractItem* item);
  /**
   * @brief 
   *
   * @param item
   * @return bool
   */
  bool removeItem(QCPAbstractItem *item);
  /**
   * @brief 
   *
   * @param index
   * @return bool
   */
  bool removeItem(int index);
  /**
   * @brief 
   *
   * @return int
   */
  int clearItems();
  /**
   * @brief 
   *
   * @return int
   */
  int itemCount() const;
  /**
   * @brief 
   *
   * @return QList<QCPAbstractItem *>
   */
  QList<QCPAbstractItem*> selectedItems() const;
  /**
   * @brief 
   *
   * @param pos
   * @param onlySelectable
   * @return QCPAbstractItem
   */
  QCPAbstractItem *itemAt(const QPointF &pos, bool onlySelectable=false) const;
  /**
   * @brief 
   *
   * @param item
   * @return bool
   */
  bool hasItem(QCPAbstractItem *item) const;
  
  // layer interface:
  /**
   * @brief 
   *
   * @param name
   * @return QCPLayer
   */
  QCPLayer *layer(const QString &name) const;
  /**
   * @brief 
   *
   * @param index
   * @return QCPLayer
   */
  QCPLayer *layer(int index) const;
  /**
   * @brief 
   *
   * @return QCPLayer
   */
  QCPLayer *currentLayer() const;
  /**
   * @brief 
   *
   * @param name
   * @return bool
   */
  bool setCurrentLayer(const QString &name);
  /**
   * @brief 
   *
   * @param layer
   * @return bool
   */
  bool setCurrentLayer(QCPLayer *layer);
  /**
   * @brief 
   *
   * @return int
   */
  int layerCount() const;
  /**
   * @brief 
   *
   * @param name
   * @param otherLayer
   * @param insertMode
   * @return bool
   */
  bool addLayer(const QString &name, QCPLayer *otherLayer=0, LayerInsertMode insertMode=limAbove);
  /**
   * @brief 
   *
   * @param layer
   * @return bool
   */
  bool removeLayer(QCPLayer *layer);
  /**
   * @brief 
   *
   * @param layer
   * @param otherLayer
   * @param insertMode
   * @return bool
   */
  bool moveLayer(QCPLayer *layer, QCPLayer *otherLayer, LayerInsertMode insertMode=limAbove);
  
  // axis rect/layout interface:
  /**
   * @brief 
   *
   * @return int
   */
  int axisRectCount() const;
  /**
   * @brief 
   *
   * @param index
   * @return QCPAxisRect
   */
  QCPAxisRect* axisRect(int index=0) const;
  /**
   * @brief 
   *
   * @return QList<QCPAxisRect *>
   */
  QList<QCPAxisRect*> axisRects() const;
  /**
   * @brief 
   *
   * @param pos
   * @return QCPLayoutElement
   */
  QCPLayoutElement* layoutElementAt(const QPointF &pos) const;
  /**
   * @brief 
   *
   * @param onlyVisiblePlottables
   */
  Q_SLOT void rescaleAxes(bool onlyVisiblePlottables=false);
  
  /**
   * @brief 
   *
   * @return QList<QCPAxis *>
   */
  QList<QCPAxis*> selectedAxes() const;
  /**
   * @brief 
   *
   * @return QList<QCPLegend *>
   */
  QList<QCPLegend*> selectedLegends() const;
  /**
   * @brief 
   *
   */
  Q_SLOT void deselectAll();
  
  /**
   * @brief 
   *
   * @param fileName
   * @param noCosmeticPen
   * @param width
   * @param height
   * @param pdfCreator
   * @param pdfTitle
   * @return bool
   */
  bool savePdf(const QString &fileName, bool noCosmeticPen=false, int width=0, int height=0, const QString &pdfCreator=QString(), const QString &pdfTitle=QString());
  /**
   * @brief 
   *
   * @param fileName
   * @param width
   * @param height
   * @param scale
   * @param quality
   * @return bool
   */
  bool savePng(const QString &fileName, int width=0, int height=0, double scale=1.0, int quality=-1);
  /**
   * @brief 
   *
   * @param fileName
   * @param width
   * @param height
   * @param scale
   * @param quality
   * @return bool
   */
  bool saveJpg(const QString &fileName, int width=0, int height=0, double scale=1.0, int quality=-1);
  /**
   * @brief 
   *
   * @param fileName
   * @param width
   * @param height
   * @param scale
   * @return bool
   */
  bool saveBmp(const QString &fileName, int width=0, int height=0, double scale=1.0);
  /**
   * @brief 
   *
   * @param fileName
   * @param width
   * @param height
   * @param scale
   * @param format
   * @param quality
   * @return bool
   */
  bool saveRastered(const QString &fileName, int width, int height, double scale, const char *format, int quality=-1);
  /**
   * @brief 
   *
   * @param width
   * @param height
   * @param scale
   * @return QPixmap
   */
  QPixmap toPixmap(int width=0, int height=0, double scale=1.0);
  /**
   * @brief 
   *
   * @param painter
   * @param width
   * @param height
   */
  void toPainter(QCPPainter *painter, int width=0, int height=0);
  /**
   * @brief 
   *
   * @param refreshPriority
   */
  Q_SLOT void replot(QCustomPlot::RefreshPriority refreshPriority=QCustomPlot::rpHint);
  
  QCPAxis *xAxis, *yAxis, *xAxis2, *yAxis2; /**< TODO: describe */
  QCPLegend *legend; /**< TODO: describe */
  
signals:
  /**
   * @brief 
   *
   * @param event
   */
  void mouseDoubleClick(QMouseEvent *event);
  /**
   * @brief 
   *
   * @param event
   */
  void mousePress(QMouseEvent *event);
  /**
   * @brief 
   *
   * @param event
   */
  void mouseMove(QMouseEvent *event);
  /**
   * @brief 
   *
   * @param event
   */
  void mouseRelease(QMouseEvent *event);
  /**
   * @brief 
   *
   * @param event
   */
  void mouseWheel(QWheelEvent *event);
  
  /**
   * @brief 
   *
   * @param plottable
   * @param event
   */
  void plottableClick(QCPAbstractPlottable *plottable, QMouseEvent *event);
  /**
   * @brief 
   *
   * @param plottable
   * @param event
   */
  void plottableDoubleClick(QCPAbstractPlottable *plottable, QMouseEvent *event);
  /**
   * @brief 
   *
   * @param item
   * @param event
   */
  void itemClick(QCPAbstractItem *item, QMouseEvent *event);
  /**
   * @brief 
   *
   * @param item
   * @param event
   */
  void itemDoubleClick(QCPAbstractItem *item, QMouseEvent *event);
  /**
   * @brief 
   *
   * @param axis
   * @param part
   * @param event
   */
  void axisClick(QCPAxis *axis, QCPAxis::SelectablePart part, QMouseEvent *event);
  /**
   * @brief 
   *
   * @param axis
   * @param part
   * @param event
   */
  void axisDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part, QMouseEvent *event);
  /**
   * @brief 
   *
   * @param legend
   * @param item
   * @param event
   */
  void legendClick(QCPLegend *legend, QCPAbstractLegendItem *item, QMouseEvent *event);
  /**
   * @brief 
   *
   * @param legend
   * @param item
   * @param event
   */
  void legendDoubleClick(QCPLegend *legend,  QCPAbstractLegendItem *item, QMouseEvent *event);
  /**
   * @brief 
   *
   * @param event
   * @param title
   */
  void titleClick(QMouseEvent *event, QCPPlotTitle *title);
  /**
   * @brief 
   *
   * @param event
   * @param title
   */
  void titleDoubleClick(QMouseEvent *event, QCPPlotTitle *title);
  
  /**
   * @brief 
   *
   */
  void selectionChangedByUser();
  /**
   * @brief 
   *
   */
  void beforeReplot();
  /**
   * @brief 
   *
   */
  void afterReplot();
  
protected:
  // property members:
  QRect mViewport; /**< TODO: describe */
  QCPLayoutGrid *mPlotLayout; /**< TODO: describe */
  bool mAutoAddPlottableToLegend; /**< TODO: describe */
  QList<QCPAbstractPlottable*> mPlottables; /**< TODO: describe */
  QList<QCPGraph*> mGraphs; // extra list of plottables also in mPlottables that are of type QCPGraph /**< TODO: describe */
  QList<QCPAbstractItem*> mItems; /**< TODO: describe */
  QList<QCPLayer*> mLayers; /**< TODO: describe */
  QCP::AntialiasedElements mAntialiasedElements, mNotAntialiasedElements; /**< TODO: describe */
  QCP::Interactions mInteractions; /**< TODO: describe */
  int mSelectionTolerance; /**< TODO: describe */
  bool mNoAntialiasingOnDrag; /**< TODO: describe */
  QBrush mBackgroundBrush; /**< TODO: describe */
  QPixmap mBackgroundPixmap; /**< TODO: describe */
  QPixmap mScaledBackgroundPixmap; /**< TODO: describe */
  bool mBackgroundScaled; /**< TODO: describe */
  Qt::AspectRatioMode mBackgroundScaledMode; /**< TODO: describe */
  QCPLayer *mCurrentLayer; /**< TODO: describe */
  QCP::PlottingHints mPlottingHints; /**< TODO: describe */
  Qt::KeyboardModifier mMultiSelectModifier; /**< TODO: describe */
  
  // non-property members:
  QPixmap mPaintBuffer; /**< TODO: describe */
  QPoint mMousePressPos; /**< TODO: describe */
  QPointer<QCPLayoutElement> mMouseEventElement; /**< TODO: describe */
  bool mReplotting; /**< TODO: describe */
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @return QSize
   */
  virtual QSize minimumSizeHint() const;
  /**
   * @brief 
   *
   * @return QSize
   */
  virtual QSize sizeHint() const;
  /**
   * @brief 
   *
   * @param event
   */
  virtual void paintEvent(QPaintEvent *event);
  /**
   * @brief 
   *
   * @param event
   */
  virtual void resizeEvent(QResizeEvent *event);
  /**
   * @brief 
   *
   * @param event
   */
  virtual void mouseDoubleClickEvent(QMouseEvent *event);
  /**
   * @brief 
   *
   * @param event
   */
  virtual void mousePressEvent(QMouseEvent *event);
  /**
   * @brief 
   *
   * @param event
   */
  virtual void mouseMoveEvent(QMouseEvent *event);
  /**
   * @brief 
   *
   * @param event
   */
  virtual void mouseReleaseEvent(QMouseEvent *event);
  /**
   * @brief 
   *
   * @param event
   */
  virtual void wheelEvent(QWheelEvent *event);
  
  // introduced virtual methods:
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter);
  /**
   * @brief 
   *
   * @param axis
   */
  virtual void axisRemoved(QCPAxis *axis);
  /**
   * @brief 
   *
   * @param legend
   */
  virtual void legendRemoved(QCPLegend *legend);
  
  // non-virtual methods:
  /**
   * @brief 
   *
   */
  void updateLayerIndices() const;
  /**
   * @brief 
   *
   * @param pos
   * @param onlySelectable
   * @param selectionDetails
   * @return QCPLayerable
   */
  QCPLayerable *layerableAt(const QPointF &pos, bool onlySelectable, QVariant *selectionDetails=0) const;
  /**
   * @brief 
   *
   * @param painter
   */
  void drawBackground(QCPPainter *painter);
  
  friend class QCPLegend;
  friend class QCPAxis;
  friend class QCPLayer;
  friend class QCPAxisRect;
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPColorGradient
{
  Q_GADGET
public:
  /*!
    Defines the color spaces in which color interpolation between gradient stops can be performed.
    
    \see setColorInterpolation
  */
  enum ColorInterpolation { ciRGB  ///< Color channels red, green and blue are linearly interpolated
                            ,ciHSV ///< Color channels hue, saturation and value are linearly interpolated (The hue is interpolated over the shortest angle distance)
                          };
  Q_ENUMS(ColorInterpolation)
  
  /*!
    Defines the available presets that can be loaded with \ref loadPreset. See the documentation
    there for an image of the presets.
  */
  enum GradientPreset { gpGrayscale  ///< Continuous lightness from black to white (suited for non-biased data representation)
                        ,gpHot       ///< Continuous lightness from black over firey colors to white (suited for non-biased data representation)
                        ,gpCold      ///< Continuous lightness from black over icey colors to white (suited for non-biased data representation)
                        ,gpNight     ///< Continuous lightness from black over weak blueish colors to white (suited for non-biased data representation)
                        ,gpCandy     ///< Blue over pink to white
                        ,gpGeography ///< Colors suitable to represent different elevations on geographical maps
                        ,gpIon       ///< Half hue spectrum from black over purple to blue and finally green (creates banding illusion but allows more precise magnitude estimates)
                        ,gpThermal   ///< Colors suitable for thermal imaging, ranging from dark blue over purple to orange, yellow and white
                        ,gpPolar     ///< Colors suitable to emphasize polarity around the center, with blue for negative, black in the middle and red for positive values
                        ,gpSpectrum  ///< An approximation of the visible light spectrum (creates banding illusion but allows more precise magnitude estimates)
                        ,gpJet       ///< Hue variation similar to a spectrum, often used in numerical visualization (creates banding illusion but allows more precise magnitude estimates)
                        ,gpHues      ///< Full hue cycle, with highest and lowest color red (suitable for periodic data, such as angles and phases, see \ref setPeriodic)
                      };
  Q_ENUMS(GradientPreset)
  
  /**
   * @brief 
   *
   * @param preset
   */
  QCPColorGradient(GradientPreset preset=gpCold);
  /**
   * @brief 
   *
   * @param other
   * @return bool operator
   */
  bool operator==(const QCPColorGradient &other) const;
  /**
   * @brief 
   *
   * @param other
   * @return bool operator
   */
  bool operator!=(const QCPColorGradient &other) const { return !(*this == other); }
  
  // getters:
  /**
   * @brief 
   *
   * @return int
   */
  int levelCount() const { return mLevelCount; }
  /**
   * @brief 
   *
   * @return QMap<double, QColor>
   */
  QMap<double, QColor> colorStops() const { return mColorStops; }
  /**
   * @brief 
   *
   * @return ColorInterpolation
   */
  ColorInterpolation colorInterpolation() const { return mColorInterpolation; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool periodic() const { return mPeriodic; }
  
  // setters:
  /**
   * @brief 
   *
   * @param n
   */
  void setLevelCount(int n);
  /**
   * @brief 
   *
   * @param QMap<double
   * @param colorStops
   */
  void setColorStops(const QMap<double, QColor> &colorStops);
  /**
   * @brief 
   *
   * @param position
   * @param color
   */
  void setColorStopAt(double position, const QColor &color);
  /**
   * @brief 
   *
   * @param interpolation
   */
  void setColorInterpolation(ColorInterpolation interpolation);
  /**
   * @brief 
   *
   * @param enabled
   */
  void setPeriodic(bool enabled);
  
  // non-property methods:
  /**
   * @brief 
   *
   * @param data
   * @param range
   * @param scanLine
   * @param n
   * @param dataIndexFactor
   * @param logarithmic
   */
  void colorize(const double *data, const QCPRange &range, QRgb *scanLine, int n, int dataIndexFactor=1, bool logarithmic=false);
  /**
   * @brief 
   *
   * @param position
   * @param range
   * @param logarithmic
   * @return QRgb
   */
  QRgb color(double position, const QCPRange &range, bool logarithmic=false);
  /**
   * @brief 
   *
   * @param preset
   */
  void loadPreset(GradientPreset preset);
  /**
   * @brief 
   *
   */
  void clearColorStops();
  /**
   * @brief 
   *
   * @return QCPColorGradient
   */
  QCPColorGradient inverted() const;
  
protected:
  /**
   * @brief 
   *
   */
  void updateColorBuffer();
  
  // property members:
  int mLevelCount; /**< TODO: describe */
  QMap<double, QColor> mColorStops; /**< TODO: describe */
  ColorInterpolation mColorInterpolation; /**< TODO: describe */
  bool mPeriodic; /**< TODO: describe */
  
  // non-property members:
  QVector<QRgb> mColorBuffer; /**< TODO: describe */
  bool mColorBufferInvalidated; /**< TODO: describe */
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPAxisRect : public QCPLayoutElement
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(QPixmap background READ background WRITE setBackground)
  Q_PROPERTY(bool backgroundScaled READ backgroundScaled WRITE setBackgroundScaled)
  Q_PROPERTY(Qt::AspectRatioMode backgroundScaledMode READ backgroundScaledMode WRITE setBackgroundScaledMode)
  Q_PROPERTY(Qt::Orientations rangeDrag READ rangeDrag WRITE setRangeDrag)
  Q_PROPERTY(Qt::Orientations rangeZoom READ rangeZoom WRITE setRangeZoom)
  /// \endcond
public:
  /**
   * @brief 
   *
   * @param parentPlot
   * @param setupDefaultAxes
   */
  explicit QCPAxisRect(QCustomPlot *parentPlot, bool setupDefaultAxes=true);
  /**
   * @brief 
   *
   */
  virtual ~QCPAxisRect();
  
  // getters:
  /**
   * @brief 
   *
   * @return QPixmap
   */
  QPixmap background() const { return mBackgroundPixmap; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool backgroundScaled() const { return mBackgroundScaled; }
  /**
   * @brief 
   *
   * @return Qt::AspectRatioMode
   */
  Qt::AspectRatioMode backgroundScaledMode() const { return mBackgroundScaledMode; }
  /**
   * @brief 
   *
   * @return Qt::Orientations
   */
  Qt::Orientations rangeDrag() const { return mRangeDrag; }
  /**
   * @brief 
   *
   * @return Qt::Orientations
   */
  Qt::Orientations rangeZoom() const { return mRangeZoom; }
  /**
   * @brief 
   *
   * @param orientation
   * @return QCPAxis
   */
  QCPAxis *rangeDragAxis(Qt::Orientation orientation);
  /**
   * @brief 
   *
   * @param orientation
   * @return QCPAxis
   */
  QCPAxis *rangeZoomAxis(Qt::Orientation orientation);
  /**
   * @brief 
   *
   * @param orientation
   * @return double
   */
  double rangeZoomFactor(Qt::Orientation orientation);
  
  // setters:
  /**
   * @brief 
   *
   * @param pm
   */
  void setBackground(const QPixmap &pm);
  /**
   * @brief 
   *
   * @param pm
   * @param scaled
   * @param mode
   */
  void setBackground(const QPixmap &pm, bool scaled, Qt::AspectRatioMode mode=Qt::KeepAspectRatioByExpanding);
  /**
   * @brief 
   *
   * @param brush
   */
  void setBackground(const QBrush &brush);
  /**
   * @brief 
   *
   * @param scaled
   */
  void setBackgroundScaled(bool scaled);
  /**
   * @brief 
   *
   * @param mode
   */
  void setBackgroundScaledMode(Qt::AspectRatioMode mode);
  /**
   * @brief 
   *
   * @param orientations
   */
  void setRangeDrag(Qt::Orientations orientations);
  /**
   * @brief 
   *
   * @param orientations
   */
  void setRangeZoom(Qt::Orientations orientations);
  /**
   * @brief 
   *
   * @param horizontal
   * @param vertical
   */
  void setRangeDragAxes(QCPAxis *horizontal, QCPAxis *vertical);
  /**
   * @brief 
   *
   * @param horizontal
   * @param vertical
   */
  void setRangeZoomAxes(QCPAxis *horizontal, QCPAxis *vertical);
  /**
   * @brief 
   *
   * @param horizontalFactor
   * @param verticalFactor
   */
  void setRangeZoomFactor(double horizontalFactor, double verticalFactor);
  /**
   * @brief 
   *
   * @param factor
   */
  void setRangeZoomFactor(double factor);
  
  // non-property methods:
  /**
   * @brief 
   *
   * @param type
   * @return int
   */
  int axisCount(QCPAxis::AxisType type) const;
  /**
   * @brief 
   *
   * @param type
   * @param index
   * @return QCPAxis
   */
  QCPAxis *axis(QCPAxis::AxisType type, int index=0) const;
  /**
   * @brief 
   *
   * @param types
   * @return QList<QCPAxis *>
   */
  QList<QCPAxis*> axes(QCPAxis::AxisTypes types) const;
  /**
   * @brief 
   *
   * @return QList<QCPAxis *>
   */
  QList<QCPAxis*> axes() const;
  /**
   * @brief 
   *
   * @param type
   * @param axis
   * @return QCPAxis
   */
  QCPAxis *addAxis(QCPAxis::AxisType type, QCPAxis *axis=0);
  /**
   * @brief 
   *
   * @param types
   * @return QList<QCPAxis *>
   */
  QList<QCPAxis*> addAxes(QCPAxis::AxisTypes types);
  /**
   * @brief 
   *
   * @param axis
   * @return bool
   */
  bool removeAxis(QCPAxis *axis);
  /**
   * @brief 
   *
   * @return QCPLayoutInset
   */
  QCPLayoutInset *insetLayout() const { return mInsetLayout; }
  
  /**
   * @brief 
   *
   * @param connectRanges
   */
  void setupFullAxesBox(bool connectRanges=false);
  /**
   * @brief 
   *
   * @return QList<QCPAbstractPlottable *>
   */
  QList<QCPAbstractPlottable*> plottables() const;
  /**
   * @brief 
   *
   * @return QList<QCPGraph *>
   */
  QList<QCPGraph*> graphs() const;
  /**
   * @brief 
   *
   * @return QList<QCPAbstractItem *>
   */
  QList<QCPAbstractItem*> items() const;
  
  // read-only interface imitating a QRect:
  /**
   * @brief 
   *
   * @return int
   */
  int left() const { return mRect.left(); }
  /**
   * @brief 
   *
   * @return int
   */
  int right() const { return mRect.right(); }
  /**
   * @brief 
   *
   * @return int
   */
  int top() const { return mRect.top(); }
  /**
   * @brief 
   *
   * @return int
   */
  int bottom() const { return mRect.bottom(); }
  /**
   * @brief 
   *
   * @return int
   */
  int width() const { return mRect.width(); }
  /**
   * @brief 
   *
   * @return int
   */
  int height() const { return mRect.height(); }
  /**
   * @brief 
   *
   * @return QSize
   */
  QSize size() const { return mRect.size(); }
  /**
   * @brief 
   *
   * @return QPoint
   */
  QPoint topLeft() const { return mRect.topLeft(); }
  /**
   * @brief 
   *
   * @return QPoint
   */
  QPoint topRight() const { return mRect.topRight(); }
  /**
   * @brief 
   *
   * @return QPoint
   */
  QPoint bottomLeft() const { return mRect.bottomLeft(); }
  /**
   * @brief 
   *
   * @return QPoint
   */
  QPoint bottomRight() const { return mRect.bottomRight(); }
  /**
   * @brief 
   *
   * @return QPoint
   */
  QPoint center() const { return mRect.center(); }
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param phase
   */
  virtual void update(UpdatePhase phase);
  /**
   * @brief 
   *
   * @param recursive
   * @return QList<QCPLayoutElement *>
   */
  virtual QList<QCPLayoutElement*> elements(bool recursive) const;

protected:
  // property members:
  QBrush mBackgroundBrush; /**< TODO: describe */
  QPixmap mBackgroundPixmap; /**< TODO: describe */
  QPixmap mScaledBackgroundPixmap; /**< TODO: describe */
  bool mBackgroundScaled; /**< TODO: describe */
  Qt::AspectRatioMode mBackgroundScaledMode; /**< TODO: describe */
  QCPLayoutInset *mInsetLayout; /**< TODO: describe */
  Qt::Orientations mRangeDrag, mRangeZoom; /**< TODO: describe */
  QPointer<QCPAxis> mRangeDragHorzAxis, mRangeDragVertAxis, mRangeZoomHorzAxis, mRangeZoomVertAxis; /**< TODO: describe */
  double mRangeZoomFactorHorz, mRangeZoomFactorVert; /**< TODO: describe */
  // non-property members:
  QCPRange mDragStartHorzRange, mDragStartVertRange; /**< TODO: describe */
  QCP::AntialiasedElements mAADragBackup, mNotAADragBackup; /**< TODO: describe */
  QPoint mDragStart; /**< TODO: describe */
  bool mDragging; /**< TODO: describe */
  QHash<QCPAxis::AxisType, QList<QCPAxis*> > mAxes; /**< TODO: describe */
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter);
  /**
   * @brief 
   *
   * @param side
   * @return int
   */
  virtual int calculateAutoMargin(QCP::MarginSide side);
  // events:
  /**
   * @brief 
   *
   * @param event
   */
  virtual void mousePressEvent(QMouseEvent *event);
  /**
   * @brief 
   *
   * @param event
   */
  virtual void mouseMoveEvent(QMouseEvent *event);
  /**
   * @brief 
   *
   * @param event
   */
  virtual void mouseReleaseEvent(QMouseEvent *event);
  /**
   * @brief 
   *
   * @param event
   */
  virtual void wheelEvent(QWheelEvent *event);
  
  // non-property methods:
  /**
   * @brief 
   *
   * @param painter
   */
  void drawBackground(QCPPainter *painter);
  /**
   * @brief 
   *
   * @param type
   */
  void updateAxesOffset(QCPAxis::AxisType type);
  
private:
  /**
   * @brief 
   *
   * @param 
   */
  Q_DISABLE_COPY(QCPAxisRect)
  
  friend class QCustomPlot;
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPAbstractLegendItem : public QCPLayoutElement
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(QCPLegend* parentLegend READ parentLegend)
  Q_PROPERTY(QFont font READ font WRITE setFont)
  Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
  Q_PROPERTY(QFont selectedFont READ selectedFont WRITE setSelectedFont)
  Q_PROPERTY(QColor selectedTextColor READ selectedTextColor WRITE setSelectedTextColor)
  Q_PROPERTY(bool selectable READ selectable WRITE setSelectable NOTIFY selectionChanged)
  Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectableChanged)
  /// \endcond
public:
  /**
   * @brief 
   *
   * @param parent
   */
  explicit QCPAbstractLegendItem(QCPLegend *parent);
  
  // getters:
  /**
   * @brief 
   *
   * @return QCPLegend
   */
  QCPLegend *parentLegend() const { return mParentLegend; }
  /**
   * @brief 
   *
   * @return QFont
   */
  QFont font() const { return mFont; }
  /**
   * @brief 
   *
   * @return QColor
   */
  QColor textColor() const { return mTextColor; }
  /**
   * @brief 
   *
   * @return QFont
   */
  QFont selectedFont() const { return mSelectedFont; }
  /**
   * @brief 
   *
   * @return QColor
   */
  QColor selectedTextColor() const { return mSelectedTextColor; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool selectable() const { return mSelectable; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool selected() const { return mSelected; }
  
  // setters:
  /**
   * @brief 
   *
   * @param font
   */
  void setFont(const QFont &font);
  /**
   * @brief 
   *
   * @param color
   */
  void setTextColor(const QColor &color);
  /**
   * @brief 
   *
   * @param font
   */
  void setSelectedFont(const QFont &font);
  /**
   * @brief 
   *
   * @param color
   */
  void setSelectedTextColor(const QColor &color);
  /**
   * @brief 
   *
   * @param selectable
   */
  Q_SLOT void setSelectable(bool selectable);
  /**
   * @brief 
   *
   * @param selected
   */
  Q_SLOT void setSelected(bool selected);
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param pos
   * @param onlySelectable
   * @param details
   * @return double
   */
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
signals:
  /**
   * @brief 
   *
   * @param selected
   */
  void selectionChanged(bool selected);
  /**
   * @brief 
   *
   * @param selectable
   */
  void selectableChanged(bool selectable);
  
protected:
  // property members:
  QCPLegend *mParentLegend; /**< TODO: describe */
  QFont mFont; /**< TODO: describe */
  QColor mTextColor; /**< TODO: describe */
  QFont mSelectedFont; /**< TODO: describe */
  QColor mSelectedTextColor; /**< TODO: describe */
  bool mSelectable, mSelected; /**< TODO: describe */
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @return QCP::Interaction
   */
  virtual QCP::Interaction selectionCategory() const;
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  /**
   * @brief 
   *
   * @return QRect
   */
  virtual QRect clipRect() const;
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter) = 0;
  // events:
  /**
   * @brief 
   *
   * @param event
   * @param additive
   * @param details
   * @param selectionStateChanged
   */
  virtual void selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged);
  /**
   * @brief 
   *
   * @param selectionStateChanged
   */
  virtual void deselectEvent(bool *selectionStateChanged);
  
private:
  /**
   * @brief 
   *
   * @param 
   */
  Q_DISABLE_COPY(QCPAbstractLegendItem)
  
  friend class QCPLegend;
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPPlottableLegendItem : public QCPAbstractLegendItem
{
  Q_OBJECT
public:
  /**
   * @brief 
   *
   * @param parent
   * @param plottable
   */
  QCPPlottableLegendItem(QCPLegend *parent, QCPAbstractPlottable *plottable);
  
  // getters:
  /**
   * @brief 
   *
   * @return QCPAbstractPlottable
   */
  QCPAbstractPlottable *plottable() { return mPlottable; }
  
protected:
  // property members:
  QCPAbstractPlottable *mPlottable; /**< TODO: describe */
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter);
  /**
   * @brief 
   *
   * @return QSize
   */
  virtual QSize minimumSizeHint() const;
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen getIconBorderPen() const;
  /**
   * @brief 
   *
   * @return QColor
   */
  QColor getTextColor() const;
  /**
   * @brief 
   *
   * @return QFont
   */
  QFont getFont() const;
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPLegend : public QCPLayoutGrid
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(QPen borderPen READ borderPen WRITE setBorderPen)
  Q_PROPERTY(QBrush brush READ brush WRITE setBrush)
  Q_PROPERTY(QFont font READ font WRITE setFont)
  Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
  Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize)
  Q_PROPERTY(int iconTextPadding READ iconTextPadding WRITE setIconTextPadding)
  Q_PROPERTY(QPen iconBorderPen READ iconBorderPen WRITE setIconBorderPen)
  Q_PROPERTY(SelectableParts selectableParts READ selectableParts WRITE setSelectableParts NOTIFY selectionChanged)
  Q_PROPERTY(SelectableParts selectedParts READ selectedParts WRITE setSelectedParts NOTIFY selectableChanged)
  Q_PROPERTY(QPen selectedBorderPen READ selectedBorderPen WRITE setSelectedBorderPen)
  Q_PROPERTY(QPen selectedIconBorderPen READ selectedIconBorderPen WRITE setSelectedIconBorderPen)
  Q_PROPERTY(QBrush selectedBrush READ selectedBrush WRITE setSelectedBrush)
  Q_PROPERTY(QFont selectedFont READ selectedFont WRITE setSelectedFont)
  Q_PROPERTY(QColor selectedTextColor READ selectedTextColor WRITE setSelectedTextColor)
  /// \endcond
public:
  /*!
    Defines the selectable parts of a legend
    
    \see setSelectedParts, setSelectableParts
  */
  enum SelectablePart { spNone       = 0x000  ///< <tt>0x000</tt> None
                        ,spLegendBox  = 0x001 ///< <tt>0x001</tt> The legend box (frame)
                        ,spItems      = 0x002 ///< <tt>0x002</tt> Legend items individually (see \ref selectedItems)
                      };
  Q_FLAGS(SelectablePart SelectableParts)
  /**
   * @brief 
   *
   */
  Q_DECLARE_FLAGS(SelectableParts, SelectablePart)
  
  /**
   * @brief 
   *
   */
  explicit QCPLegend();
  /**
   * @brief 
   *
   */
  virtual ~QCPLegend();
  
  // getters:
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen borderPen() const { return mBorderPen; }
  /**
   * @brief 
   *
   * @return QBrush
   */
  QBrush brush() const { return mBrush; }
  /**
   * @brief 
   *
   * @return QFont
   */
  QFont font() const { return mFont; }
  /**
   * @brief 
   *
   * @return QColor
   */
  QColor textColor() const { return mTextColor; }
  /**
   * @brief 
   *
   * @return QSize
   */
  QSize iconSize() const { return mIconSize; }
  /**
   * @brief 
   *
   * @return int
   */
  int iconTextPadding() const { return mIconTextPadding; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen iconBorderPen() const { return mIconBorderPen; }
  /**
   * @brief 
   *
   * @return SelectableParts
   */
  SelectableParts selectableParts() const { return mSelectableParts; }
  /**
   * @brief 
   *
   * @return SelectableParts
   */
  SelectableParts selectedParts() const;
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen selectedBorderPen() const { return mSelectedBorderPen; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen selectedIconBorderPen() const { return mSelectedIconBorderPen; }
  /**
   * @brief 
   *
   * @return QBrush
   */
  QBrush selectedBrush() const { return mSelectedBrush; }
  /**
   * @brief 
   *
   * @return QFont
   */
  QFont selectedFont() const { return mSelectedFont; }
  /**
   * @brief 
   *
   * @return QColor
   */
  QColor selectedTextColor() const { return mSelectedTextColor; }
  
  // setters:
  /**
   * @brief 
   *
   * @param pen
   */
  void setBorderPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param brush
   */
  void setBrush(const QBrush &brush);
  /**
   * @brief 
   *
   * @param font
   */
  void setFont(const QFont &font);
  /**
   * @brief 
   *
   * @param color
   */
  void setTextColor(const QColor &color);
  /**
   * @brief 
   *
   * @param size
   */
  void setIconSize(const QSize &size);
  /**
   * @brief 
   *
   * @param width
   * @param height
   */
  void setIconSize(int width, int height);
  /**
   * @brief 
   *
   * @param padding
   */
  void setIconTextPadding(int padding);
  /**
   * @brief 
   *
   * @param pen
   */
  void setIconBorderPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param selectableParts
   */
  Q_SLOT void setSelectableParts(const SelectableParts &selectableParts);
  /**
   * @brief 
   *
   * @param selectedParts
   */
  Q_SLOT void setSelectedParts(const SelectableParts &selectedParts);
  /**
   * @brief 
   *
   * @param pen
   */
  void setSelectedBorderPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param pen
   */
  void setSelectedIconBorderPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param brush
   */
  void setSelectedBrush(const QBrush &brush);
  /**
   * @brief 
   *
   * @param font
   */
  void setSelectedFont(const QFont &font);
  /**
   * @brief 
   *
   * @param color
   */
  void setSelectedTextColor(const QColor &color);
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param pos
   * @param onlySelectable
   * @param details
   * @return double
   */
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @param index
   * @return QCPAbstractLegendItem
   */
  QCPAbstractLegendItem *item(int index) const;
  /**
   * @brief 
   *
   * @param plottable
   * @return QCPPlottableLegendItem
   */
  QCPPlottableLegendItem *itemWithPlottable(const QCPAbstractPlottable *plottable) const;
  /**
   * @brief 
   *
   * @return int
   */
  int itemCount() const;
  /**
   * @brief 
   *
   * @param item
   * @return bool
   */
  bool hasItem(QCPAbstractLegendItem *item) const;
  /**
   * @brief 
   *
   * @param plottable
   * @return bool
   */
  bool hasItemWithPlottable(const QCPAbstractPlottable *plottable) const;
  /**
   * @brief 
   *
   * @param item
   * @return bool
   */
  bool addItem(QCPAbstractLegendItem *item);
  /**
   * @brief 
   *
   * @param index
   * @return bool
   */
  bool removeItem(int index);
  /**
   * @brief 
   *
   * @param item
   * @return bool
   */
  bool removeItem(QCPAbstractLegendItem *item);
  /**
   * @brief 
   *
   */
  void clearItems();
  /**
   * @brief 
   *
   * @return QList<QCPAbstractLegendItem *>
   */
  QList<QCPAbstractLegendItem*> selectedItems() const;
  
signals:
  /**
   * @brief 
   *
   * @param parts
   */
  void selectionChanged(QCPLegend::SelectableParts parts);
  /**
   * @brief 
   *
   * @param parts
   */
  void selectableChanged(QCPLegend::SelectableParts parts);
  
protected:
  // property members:
  QPen mBorderPen, mIconBorderPen; /**< TODO: describe */
  QBrush mBrush; /**< TODO: describe */
  QFont mFont; /**< TODO: describe */
  QColor mTextColor; /**< TODO: describe */
  QSize mIconSize; /**< TODO: describe */
  int mIconTextPadding; /**< TODO: describe */
  SelectableParts mSelectedParts, mSelectableParts; /**< TODO: describe */
  QPen mSelectedBorderPen, mSelectedIconBorderPen; /**< TODO: describe */
  QBrush mSelectedBrush; /**< TODO: describe */
  QFont mSelectedFont; /**< TODO: describe */
  QColor mSelectedTextColor; /**< TODO: describe */
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param parentPlot
   */
  virtual void parentPlotInitialized(QCustomPlot *parentPlot);
  /**
   * @brief 
   *
   * @return QCP::Interaction
   */
  virtual QCP::Interaction selectionCategory() const;
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter);
  // events:
  /**
   * @brief 
   *
   * @param event
   * @param additive
   * @param details
   * @param selectionStateChanged
   */
  virtual void selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged);
  /**
   * @brief 
   *
   * @param selectionStateChanged
   */
  virtual void deselectEvent(bool *selectionStateChanged);
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen getBorderPen() const;
  /**
   * @brief 
   *
   * @return QBrush
   */
  QBrush getBrush() const;
  
private:
  /**
   * @brief 
   *
   * @param 
   */
  Q_DISABLE_COPY(QCPLegend)
  
  friend class QCustomPlot;
  friend class QCPAbstractLegendItem;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QCPLegend::SelectableParts)
Q_DECLARE_METATYPE(QCPLegend::SelectablePart)


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPPlotTitle : public QCPLayoutElement
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(QString text READ text WRITE setText)
  Q_PROPERTY(QFont font READ font WRITE setFont)
  Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
  Q_PROPERTY(QFont selectedFont READ selectedFont WRITE setSelectedFont)
  Q_PROPERTY(QColor selectedTextColor READ selectedTextColor WRITE setSelectedTextColor)
  Q_PROPERTY(bool selectable READ selectable WRITE setSelectable NOTIFY selectableChanged)
  Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectionChanged)
  /// \endcond
public:
  /**
   * @brief 
   *
   * @param parentPlot
   */
  explicit QCPPlotTitle(QCustomPlot *parentPlot);
  /**
   * @brief 
   *
   * @param parentPlot
   * @param text
   */
  explicit QCPPlotTitle(QCustomPlot *parentPlot, const QString &text);
  
  // getters:
  /**
   * @brief 
   *
   * @return QString
   */
  QString text() const { return mText; }
  /**
   * @brief 
   *
   * @return QFont
   */
  QFont font() const { return mFont; }
  /**
   * @brief 
   *
   * @return QColor
   */
  QColor textColor() const { return mTextColor; }
  /**
   * @brief 
   *
   * @return QFont
   */
  QFont selectedFont() const { return mSelectedFont; }
  /**
   * @brief 
   *
   * @return QColor
   */
  QColor selectedTextColor() const { return mSelectedTextColor; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool selectable() const { return mSelectable; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool selected() const { return mSelected; }
  
  // setters:
  /**
   * @brief 
   *
   * @param text
   */
  void setText(const QString &text);
  /**
   * @brief 
   *
   * @param font
   */
  void setFont(const QFont &font);
  /**
   * @brief 
   *
   * @param color
   */
  void setTextColor(const QColor &color);
  /**
   * @brief 
   *
   * @param font
   */
  void setSelectedFont(const QFont &font);
  /**
   * @brief 
   *
   * @param color
   */
  void setSelectedTextColor(const QColor &color);
  /**
   * @brief 
   *
   * @param selectable
   */
  Q_SLOT void setSelectable(bool selectable);
  /**
   * @brief 
   *
   * @param selected
   */
  Q_SLOT void setSelected(bool selected);
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param pos
   * @param onlySelectable
   * @param details
   * @return double
   */
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
signals:
  /**
   * @brief 
   *
   * @param selected
   */
  void selectionChanged(bool selected);
  /**
   * @brief 
   *
   * @param selectable
   */
  void selectableChanged(bool selectable);
  
protected:
  // property members:
  QString mText; /**< TODO: describe */
  QFont mFont; /**< TODO: describe */
  QColor mTextColor; /**< TODO: describe */
  QFont mSelectedFont; /**< TODO: describe */
  QColor mSelectedTextColor; /**< TODO: describe */
  QRect mTextBoundingRect; /**< TODO: describe */
  bool mSelectable, mSelected; /**< TODO: describe */
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter);
  /**
   * @brief 
   *
   * @return QSize
   */
  virtual QSize minimumSizeHint() const;
  /**
   * @brief 
   *
   * @return QSize
   */
  virtual QSize maximumSizeHint() const;
  // events:
  /**
   * @brief 
   *
   * @param event
   * @param additive
   * @param details
   * @param selectionStateChanged
   */
  virtual void selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged);
  /**
   * @brief 
   *
   * @param selectionStateChanged
   */
  virtual void deselectEvent(bool *selectionStateChanged);
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @return QFont
   */
  QFont mainFont() const;
  /**
   * @brief 
   *
   * @return QColor
   */
  QColor mainTextColor() const;
  
private:
  /**
   * @brief 
   *
   * @param 
   */
  Q_DISABLE_COPY(QCPPlotTitle)
};


/**
 * @brief 
 *
 */
class QCPColorScaleAxisRectPrivate : public QCPAxisRect
{
  Q_OBJECT
public:
  /**
   * @brief 
   *
   * @param parentColorScale
   */
  explicit QCPColorScaleAxisRectPrivate(QCPColorScale *parentColorScale);
protected:
  QCPColorScale *mParentColorScale; /**< TODO: describe */
  QImage mGradientImage; /**< TODO: describe */
  bool mGradientImageInvalidated; /**< TODO: describe */
  // re-using some methods of QCPAxisRect to make them available to friend class QCPColorScale
  using QCPAxisRect::calculateAutoMargin;
  using QCPAxisRect::mousePressEvent;
  using QCPAxisRect::mouseMoveEvent;
  using QCPAxisRect::mouseReleaseEvent;
  using QCPAxisRect::wheelEvent;
  using QCPAxisRect::update;
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter);
  /**
   * @brief 
   *
   */
  void updateGradientImage();
  /**
   * @brief 
   *
   * @param selectedParts
   */
  Q_SLOT void axisSelectionChanged(QCPAxis::SelectableParts selectedParts);
  /**
   * @brief 
   *
   * @param selectableParts
   */
  Q_SLOT void axisSelectableChanged(QCPAxis::SelectableParts selectableParts);
  friend class QCPColorScale;
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPColorScale : public QCPLayoutElement
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(QCPAxis::AxisType type READ type WRITE setType)
  Q_PROPERTY(QCPRange dataRange READ dataRange WRITE setDataRange NOTIFY dataRangeChanged)
  Q_PROPERTY(QCPAxis::ScaleType dataScaleType READ dataScaleType WRITE setDataScaleType NOTIFY dataScaleTypeChanged)
  Q_PROPERTY(QCPColorGradient gradient READ gradient WRITE setGradient NOTIFY gradientChanged)
  Q_PROPERTY(QString label READ label WRITE setLabel)
  Q_PROPERTY(int barWidth READ barWidth WRITE setBarWidth)
  Q_PROPERTY(bool rangeDrag READ rangeDrag WRITE setRangeDrag)
  Q_PROPERTY(bool rangeZoom READ rangeZoom WRITE setRangeZoom)
  /// \endcond
public:
  /**
   * @brief 
   *
   * @param parentPlot
   */
  explicit QCPColorScale(QCustomPlot *parentPlot);
  /**
   * @brief 
   *
   */
  virtual ~QCPColorScale();
  
  // getters:
  /**
   * @brief 
   *
   * @return QCPAxis
   */
  QCPAxis *axis() const { return mColorAxis.data(); }
  /**
   * @brief 
   *
   * @return QCPAxis::AxisType
   */
  QCPAxis::AxisType type() const { return mType; }
  /**
   * @brief 
   *
   * @return QCPRange
   */
  QCPRange dataRange() const { return mDataRange; }
  /**
   * @brief 
   *
   * @return QCPAxis::ScaleType
   */
  QCPAxis::ScaleType dataScaleType() const { return mDataScaleType; }
  /**
   * @brief 
   *
   * @return QCPColorGradient
   */
  QCPColorGradient gradient() const { return mGradient; }
  /**
   * @brief 
   *
   * @return QString
   */
  QString label() const;
  /**
   * @brief 
   *
   * @return int
   */
  int barWidth () const { return mBarWidth; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool rangeDrag() const;
  /**
   * @brief 
   *
   * @return bool
   */
  bool rangeZoom() const;
  
  // setters:
  /**
   * @brief 
   *
   * @param type
   */
  void setType(QCPAxis::AxisType type);
  /**
   * @brief 
   *
   * @param dataRange
   */
  Q_SLOT void setDataRange(const QCPRange &dataRange);
  /**
   * @brief 
   *
   * @param scaleType
   */
  Q_SLOT void setDataScaleType(QCPAxis::ScaleType scaleType);
  /**
   * @brief 
   *
   * @param gradient
   */
  Q_SLOT void setGradient(const QCPColorGradient &gradient);
  /**
   * @brief 
   *
   * @param str
   */
  void setLabel(const QString &str);
  /**
   * @brief 
   *
   * @param width
   */
  void setBarWidth(int width);
  /**
   * @brief 
   *
   * @param enabled
   */
  void setRangeDrag(bool enabled);
  /**
   * @brief 
   *
   * @param enabled
   */
  void setRangeZoom(bool enabled);
  
  // non-property methods:
  /**
   * @brief 
   *
   * @return QList<QCPColorMap *>
   */
  QList<QCPColorMap*> colorMaps() const;
  /**
   * @brief 
   *
   * @param onlyVisibleMaps
   */
  void rescaleDataRange(bool onlyVisibleMaps);
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param phase
   */
  virtual void update(UpdatePhase phase);
  
signals:
  /**
   * @brief 
   *
   * @param newRange
   */
  void dataRangeChanged(QCPRange newRange);
  /**
   * @brief 
   *
   * @param scaleType
   */
  void dataScaleTypeChanged(QCPAxis::ScaleType scaleType);
  /**
   * @brief 
   *
   * @param newGradient
   */
  void gradientChanged(QCPColorGradient newGradient);

protected:
  // property members:
  QCPAxis::AxisType mType; /**< TODO: describe */
  QCPRange mDataRange; /**< TODO: describe */
  QCPAxis::ScaleType mDataScaleType; /**< TODO: describe */
  QCPColorGradient mGradient; /**< TODO: describe */
  int mBarWidth; /**< TODO: describe */
  
  // non-property members:
  QPointer<QCPColorScaleAxisRectPrivate> mAxisRect; /**< TODO: describe */
  QPointer<QCPAxis> mColorAxis; /**< TODO: describe */
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  // events:
  /**
   * @brief 
   *
   * @param event
   */
  virtual void mousePressEvent(QMouseEvent *event);
  /**
   * @brief 
   *
   * @param event
   */
  virtual void mouseMoveEvent(QMouseEvent *event);
  /**
   * @brief 
   *
   * @param event
   */
  virtual void mouseReleaseEvent(QMouseEvent *event);
  /**
   * @brief 
   *
   * @param event
   */
  virtual void wheelEvent(QWheelEvent *event);
  
private:
  /**
   * @brief 
   *
   * @param 
   */
  Q_DISABLE_COPY(QCPColorScale)
  
  friend class QCPColorScaleAxisRectPrivate;
};


/*! \file */



/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPData
{
public:
  /**
   * @brief 
   *
   */
  QCPData();
  /**
   * @brief 
   *
   * @param key
   * @param value
   */
  QCPData(double key, double value);
  double key, value; /**< TODO: describe */
  double keyErrorPlus, keyErrorMinus; /**< TODO: describe */
  double valueErrorPlus, valueErrorMinus; /**< TODO: describe */
};
Q_DECLARE_TYPEINFO(QCPData, Q_MOVABLE_TYPE);

/*! \typedef QCPDataMap
  Container for storing \ref QCPData items in a sorted fashion. The key of the map
  is the key member of the QCPData instance.
  
  This is the container in which QCPGraph holds its data.
  \see QCPData, QCPGraph::setData
*/
typedef QMap<double, QCPData> QCPDataMap;
typedef QMapIterator<double, QCPData> QCPDataMapIterator;
typedef QMutableMapIterator<double, QCPData> QCPDataMutableMapIterator;


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPGraph : public QCPAbstractPlottable
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(LineStyle lineStyle READ lineStyle WRITE setLineStyle)
  Q_PROPERTY(QCPScatterStyle scatterStyle READ scatterStyle WRITE setScatterStyle)
  Q_PROPERTY(ErrorType errorType READ errorType WRITE setErrorType)
  Q_PROPERTY(QPen errorPen READ errorPen WRITE setErrorPen)
  Q_PROPERTY(double errorBarSize READ errorBarSize WRITE setErrorBarSize)
  Q_PROPERTY(bool errorBarSkipSymbol READ errorBarSkipSymbol WRITE setErrorBarSkipSymbol)
  Q_PROPERTY(QCPGraph* channelFillGraph READ channelFillGraph WRITE setChannelFillGraph)
  Q_PROPERTY(bool adaptiveSampling READ adaptiveSampling WRITE setAdaptiveSampling)
  /// \endcond
public:
  /*!
    Defines how the graph's line is represented visually in the plot. The line is drawn with the
    current pen of the graph (\ref setPen).
    \see setLineStyle
  */
  enum LineStyle { lsNone        ///< data points are not connected with any lines (e.g. data only represented
                                 ///< with symbols according to the scatter style, see \ref setScatterStyle)
                   ,lsLine       ///< data points are connected by a straight line
                   ,lsStepLeft   ///< line is drawn as steps where the step height is the value of the left data point
                   ,lsStepRight  ///< line is drawn as steps where the step height is the value of the right data point
                   ,lsStepCenter ///< line is drawn as steps where the step is in between two data points
                   ,lsImpulse    ///< each data point is represented by a line parallel to the value axis, which reaches from the data point to the zero-value-line
                 };
  Q_ENUMS(LineStyle)
  /*!
    Defines what kind of error bars are drawn for each data point
  */
  enum ErrorType { etNone   ///< No error bars are shown
                   ,etKey   ///< Error bars for the key dimension of the data point are shown
                   ,etValue ///< Error bars for the value dimension of the data point are shown
                   ,etBoth  ///< Error bars for both key and value dimensions of the data point are shown
                 };
  Q_ENUMS(ErrorType)
  
  /**
   * @brief 
   *
   * @param keyAxis
   * @param valueAxis
   */
  explicit QCPGraph(QCPAxis *keyAxis, QCPAxis *valueAxis);
  /**
   * @brief 
   *
   */
  virtual ~QCPGraph();
  
  // getters:
  /**
   * @brief 
   *
   * @return QCPDataMap
   */
  QCPDataMap *data() const { return mData; }
  /**
   * @brief 
   *
   * @return LineStyle
   */
  LineStyle lineStyle() const { return mLineStyle; }
  /**
   * @brief 
   *
   * @return QCPScatterStyle
   */
  QCPScatterStyle scatterStyle() const { return mScatterStyle; }
  /**
   * @brief 
   *
   * @return ErrorType
   */
  ErrorType errorType() const { return mErrorType; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen errorPen() const { return mErrorPen; }
  /**
   * @brief 
   *
   * @return double
   */
  double errorBarSize() const { return mErrorBarSize; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool errorBarSkipSymbol() const { return mErrorBarSkipSymbol; }
  /**
   * @brief 
   *
   * @return QCPGraph
   */
  QCPGraph *channelFillGraph() const { return mChannelFillGraph.data(); }
  /**
   * @brief 
   *
   * @return bool
   */
  bool adaptiveSampling() const { return mAdaptiveSampling; }
  
  // setters:
  /**
   * @brief 
   *
   * @param data
   * @param copy
   */
  void setData(QCPDataMap *data, bool copy=false);
  /**
   * @brief 
   *
   * @param key
   * @param value
   */
  void setData(const QVector<double> &key, const QVector<double> &value);
  /**
   * @brief 
   *
   * @param key
   * @param value
   * @param keyError
   */
  void setDataKeyError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &keyError);
  /**
   * @brief 
   *
   * @param key
   * @param value
   * @param keyErrorMinus
   * @param keyErrorPlus
   */
  void setDataKeyError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &keyErrorMinus, const QVector<double> &keyErrorPlus);
  /**
   * @brief 
   *
   * @param key
   * @param value
   * @param valueError
   */
  void setDataValueError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &valueError);
  /**
   * @brief 
   *
   * @param key
   * @param value
   * @param valueErrorMinus
   * @param valueErrorPlus
   */
  void setDataValueError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &valueErrorMinus, const QVector<double> &valueErrorPlus);
  /**
   * @brief 
   *
   * @param key
   * @param value
   * @param keyError
   * @param valueError
   */
  void setDataBothError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &keyError, const QVector<double> &valueError);
  /**
   * @brief 
   *
   * @param key
   * @param value
   * @param keyErrorMinus
   * @param keyErrorPlus
   * @param valueErrorMinus
   * @param valueErrorPlus
   */
  void setDataBothError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &keyErrorMinus, const QVector<double> &keyErrorPlus, const QVector<double> &valueErrorMinus, const QVector<double> &valueErrorPlus);
  /**
   * @brief 
   *
   * @param ls
   */
  void setLineStyle(LineStyle ls);
  /**
   * @brief 
   *
   * @param style
   */
  void setScatterStyle(const QCPScatterStyle &style);
  /**
   * @brief 
   *
   * @param errorType
   */
  void setErrorType(ErrorType errorType);
  /**
   * @brief 
   *
   * @param pen
   */
  void setErrorPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param size
   */
  void setErrorBarSize(double size);
  /**
   * @brief 
   *
   * @param enabled
   */
  void setErrorBarSkipSymbol(bool enabled);
  /**
   * @brief 
   *
   * @param targetGraph
   */
  void setChannelFillGraph(QCPGraph *targetGraph);
  /**
   * @brief 
   *
   * @param enabled
   */
  void setAdaptiveSampling(bool enabled);
  
  // non-property methods:
  /**
   * @brief 
   *
   * @param dataMap
   */
  void addData(const QCPDataMap &dataMap);
  /**
   * @brief 
   *
   * @param data
   */
  void addData(const QCPData &data);
  /**
   * @brief 
   *
   * @param key
   * @param value
   */
  void addData(double key, double value);
  /**
   * @brief 
   *
   * @param keys
   * @param values
   */
  void addData(const QVector<double> &keys, const QVector<double> &values);
  /**
   * @brief 
   *
   * @param key
   */
  void removeDataBefore(double key);
  /**
   * @brief 
   *
   * @param key
   */
  void removeDataAfter(double key);
  /**
   * @brief 
   *
   * @param fromKey
   * @param toKey
   */
  void removeData(double fromKey, double toKey);
  /**
   * @brief 
   *
   * @param key
   */
  void removeData(double key);
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   */
  virtual void clearData();
  /**
   * @brief 
   *
   * @param pos
   * @param onlySelectable
   * @param details
   * @return double
   */
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  using QCPAbstractPlottable::rescaleAxes;
  using QCPAbstractPlottable::rescaleKeyAxis;
  using QCPAbstractPlottable::rescaleValueAxis;
  /**
   * @brief 
   *
   * @param onlyEnlarge
   * @param includeErrorBars
   */
  void rescaleAxes(bool onlyEnlarge, bool includeErrorBars) const; // overloads base class interface
  /**
   * @brief 
   *
   * @param onlyEnlarge
   * @param includeErrorBars
   */
  void rescaleKeyAxis(bool onlyEnlarge, bool includeErrorBars) const; // overloads base class interface
  /**
   * @brief 
   *
   * @param onlyEnlarge
   * @param includeErrorBars
   */
  void rescaleValueAxis(bool onlyEnlarge, bool includeErrorBars) const; // overloads base class interface
  
protected:
  // property members:
  QCPDataMap *mData; /**< TODO: describe */
  QPen mErrorPen; /**< TODO: describe */
  LineStyle mLineStyle; /**< TODO: describe */
  QCPScatterStyle mScatterStyle; /**< TODO: describe */
  ErrorType mErrorType; /**< TODO: describe */
  double mErrorBarSize; /**< TODO: describe */
  bool mErrorBarSkipSymbol; /**< TODO: describe */
  QPointer<QCPGraph> mChannelFillGraph; /**< TODO: describe */
  bool mAdaptiveSampling; /**< TODO: describe */
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter);
  /**
   * @brief 
   *
   * @param painter
   * @param rect
   */
  virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const;
  /**
   * @brief 
   *
   * @param foundRange
   * @param inSignDomain
   * @return QCPRange
   */
  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  /**
   * @brief 
   *
   * @param foundRange
   * @param inSignDomain
   * @return QCPRange
   */
  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  /**
   * @brief 
   *
   * @param foundRange
   * @param inSignDomain
   * @param includeErrors
   * @return QCPRange
   */
  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain, bool includeErrors) const; // overloads base class interface
  /**
   * @brief 
   *
   * @param foundRange
   * @param inSignDomain
   * @param includeErrors
   * @return QCPRange
   */
  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain, bool includeErrors) const; // overloads base class interface
  
  // introduced virtual methods:
  /**
   * @brief 
   *
   * @param painter
   * @param lineData
   */
  virtual void drawFill(QCPPainter *painter, QVector<QPointF> *lineData) const;
  /**
   * @brief 
   *
   * @param painter
   * @param scatterData
   */
  virtual void drawScatterPlot(QCPPainter *painter, QVector<QCPData> *scatterData) const;
  /**
   * @brief 
   *
   * @param painter
   * @param lineData
   */
  virtual void drawLinePlot(QCPPainter *painter, QVector<QPointF> *lineData) const;
  /**
   * @brief 
   *
   * @param painter
   * @param lineData
   */
  virtual void drawImpulsePlot(QCPPainter *painter, QVector<QPointF> *lineData) const;
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @param lineData
   * @param scatterData
   */
  void getPreparedData(QVector<QCPData> *lineData, QVector<QCPData> *scatterData) const;
  /**
   * @brief 
   *
   * @param lineData
   * @param scatterData
   */
  void getPlotData(QVector<QPointF> *lineData, QVector<QCPData> *scatterData) const;
  /**
   * @brief 
   *
   * @param scatterData
   */
  void getScatterPlotData(QVector<QCPData> *scatterData) const;
  /**
   * @brief 
   *
   * @param linePixelData
   * @param scatterData
   */
  void getLinePlotData(QVector<QPointF> *linePixelData, QVector<QCPData> *scatterData) const;
  /**
   * @brief 
   *
   * @param linePixelData
   * @param scatterData
   */
  void getStepLeftPlotData(QVector<QPointF> *linePixelData, QVector<QCPData> *scatterData) const;
  /**
   * @brief 
   *
   * @param linePixelData
   * @param scatterData
   */
  void getStepRightPlotData(QVector<QPointF> *linePixelData, QVector<QCPData> *scatterData) const;
  /**
   * @brief 
   *
   * @param linePixelData
   * @param scatterData
   */
  void getStepCenterPlotData(QVector<QPointF> *linePixelData, QVector<QCPData> *scatterData) const;
  /**
   * @brief 
   *
   * @param linePixelData
   * @param scatterData
   */
  void getImpulsePlotData(QVector<QPointF> *linePixelData, QVector<QCPData> *scatterData) const;
  /**
   * @brief 
   *
   * @param painter
   * @param x
   * @param y
   * @param data
   */
  void drawError(QCPPainter *painter, double x, double y, const QCPData &data) const;
  /**
   * @brief 
   *
   * @param lower
   * @param upper
   */
  void getVisibleDataBounds(QCPDataMap::const_iterator &lower, QCPDataMap::const_iterator &upper) const;
  /**
   * @brief 
   *
   * @param lower
   * @param upper
   * @param maxCount
   * @return int
   */
  int countDataInBounds(const QCPDataMap::const_iterator &lower, const QCPDataMap::const_iterator &upper, int maxCount) const;
  /**
   * @brief 
   *
   * @param lineData
   */
  void addFillBasePoints(QVector<QPointF> *lineData) const;
  /**
   * @brief 
   *
   * @param lineData
   */
  void removeFillBasePoints(QVector<QPointF> *lineData) const;
  /**
   * @brief 
   *
   * @param lowerKey
   * @return QPointF
   */
  QPointF lowerFillBasePoint(double lowerKey) const;
  /**
   * @brief 
   *
   * @param upperKey
   * @return QPointF
   */
  QPointF upperFillBasePoint(double upperKey) const;
  /**
   * @brief 
   *
   * @param lineData
   * @return const QPolygonF
   */
  const QPolygonF getChannelFillPolygon(const QVector<QPointF> *lineData) const;
  /**
   * @brief 
   *
   * @param data
   * @param x
   * @return int
   */
  int findIndexBelowX(const QVector<QPointF> *data, double x) const;
  /**
   * @brief 
   *
   * @param data
   * @param x
   * @return int
   */
  int findIndexAboveX(const QVector<QPointF> *data, double x) const;
  /**
   * @brief 
   *
   * @param data
   * @param y
   * @return int
   */
  int findIndexBelowY(const QVector<QPointF> *data, double y) const;
  /**
   * @brief 
   *
   * @param data
   * @param y
   * @return int
   */
  int findIndexAboveY(const QVector<QPointF> *data, double y) const;
  /**
   * @brief 
   *
   * @param pixelPoint
   * @return double
   */
  double pointDistance(const QPointF &pixelPoint) const;
  
  friend class QCustomPlot;
  friend class QCPLegend;
};


/*! \file */



/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPCurveData
{
public:
  /**
   * @brief 
   *
   */
  QCPCurveData();
  /**
   * @brief 
   *
   * @param t
   * @param key
   * @param value
   */
  QCPCurveData(double t, double key, double value);
  double t, key, value; /**< TODO: describe */
};
Q_DECLARE_TYPEINFO(QCPCurveData, Q_MOVABLE_TYPE);

/*! \typedef QCPCurveDataMap
  Container for storing \ref QCPCurveData items in a sorted fashion. The key of the map
  is the t member of the QCPCurveData instance.
  
  This is the container in which QCPCurve holds its data.
  \see QCPCurveData, QCPCurve::setData
*/

typedef QMap<double, QCPCurveData> QCPCurveDataMap;
typedef QMapIterator<double, QCPCurveData> QCPCurveDataMapIterator;
/**
 * @brief 
 *
 */
typedef QMutableMapIterator<double, QCPCurveData> QCPCurveDataMutableMapIterator;


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPCurve : public QCPAbstractPlottable
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(QCPScatterStyle scatterStyle READ scatterStyle WRITE setScatterStyle)
  Q_PROPERTY(LineStyle lineStyle READ lineStyle WRITE setLineStyle)
  /// \endcond
public:
  /*!
    Defines how the curve's line is represented visually in the plot. The line is drawn with the
    current pen of the curve (\ref setPen).
    \see setLineStyle
  */
  enum LineStyle { lsNone  ///< No line is drawn between data points (e.g. only scatters)
                   ,lsLine ///< Data points are connected with a straight line
                 };
  /**
   * @brief 
   *
   * @param keyAxis
   * @param valueAxis
   */
  explicit QCPCurve(QCPAxis *keyAxis, QCPAxis *valueAxis);
  /**
   * @brief 
   *
   */
  virtual ~QCPCurve();
  
  // getters:
  /**
   * @brief 
   *
   * @return QCPCurveDataMap
   */
  QCPCurveDataMap *data() const { return mData; }
  /**
   * @brief 
   *
   * @return QCPScatterStyle
   */
  QCPScatterStyle scatterStyle() const { return mScatterStyle; }
  /**
   * @brief 
   *
   * @return LineStyle
   */
  LineStyle lineStyle() const { return mLineStyle; }
  
  // setters:
  /**
   * @brief 
   *
   * @param data
   * @param copy
   */
  void setData(QCPCurveDataMap *data, bool copy=false);
  /**
   * @brief 
   *
   * @param t
   * @param key
   * @param value
   */
  void setData(const QVector<double> &t, const QVector<double> &key, const QVector<double> &value);
  /**
   * @brief 
   *
   * @param key
   * @param value
   */
  void setData(const QVector<double> &key, const QVector<double> &value);
  /**
   * @brief 
   *
   * @param style
   */
  void setScatterStyle(const QCPScatterStyle &style);
  /**
   * @brief 
   *
   * @param style
   */
  void setLineStyle(LineStyle style);
  
  // non-property methods:
  /**
   * @brief 
   *
   * @param dataMap
   */
  void addData(const QCPCurveDataMap &dataMap);
  /**
   * @brief 
   *
   * @param data
   */
  void addData(const QCPCurveData &data);
  /**
   * @brief 
   *
   * @param t
   * @param key
   * @param value
   */
  void addData(double t, double key, double value);
  /**
   * @brief 
   *
   * @param key
   * @param value
   */
  void addData(double key, double value);
  /**
   * @brief 
   *
   * @param ts
   * @param keys
   * @param values
   */
  void addData(const QVector<double> &ts, const QVector<double> &keys, const QVector<double> &values);
  /**
   * @brief 
   *
   * @param t
   */
  void removeDataBefore(double t);
  /**
   * @brief 
   *
   * @param t
   */
  void removeDataAfter(double t);
  /**
   * @brief 
   *
   * @param fromt
   * @param tot
   */
  void removeData(double fromt, double tot);
  /**
   * @brief 
   *
   * @param t
   */
  void removeData(double t);
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   */
  virtual void clearData();
  /**
   * @brief 
   *
   * @param pos
   * @param onlySelectable
   * @param details
   * @return double
   */
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
protected:
  // property members:
  QCPCurveDataMap *mData; /**< TODO: describe */
  QCPScatterStyle mScatterStyle; /**< TODO: describe */
  LineStyle mLineStyle; /**< TODO: describe */
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter);
  /**
   * @brief 
   *
   * @param painter
   * @param rect
   */
  virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const;
  /**
   * @brief 
   *
   * @param foundRange
   * @param inSignDomain
   * @return QCPRange
   */
  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  /**
   * @brief 
   *
   * @param foundRange
   * @param inSignDomain
   * @return QCPRange
   */
  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  
  // introduced virtual methods:
  /**
   * @brief 
   *
   * @param painter
   * @param pointData
   */
  virtual void drawScatterPlot(QCPPainter *painter, const QVector<QPointF> *pointData) const;
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @param lineData
   */
  void getCurveData(QVector<QPointF> *lineData) const;
  /**
   * @brief 
   *
   * @param x
   * @param y
   * @param rectLeft
   * @param rectTop
   * @param rectRight
   * @param rectBottom
   * @return int
   */
  int getRegion(double x, double y, double rectLeft, double rectTop, double rectRight, double rectBottom) const;
  /**
   * @brief 
   *
   * @param prevRegion
   * @param prevKey
   * @param prevValue
   * @param key
   * @param value
   * @param rectLeft
   * @param rectTop
   * @param rectRight
   * @param rectBottom
   * @return QPointF
   */
  QPointF getOptimizedPoint(int prevRegion, double prevKey, double prevValue, double key, double value, double rectLeft, double rectTop, double rectRight, double rectBottom) const;
  /**
   * @brief 
   *
   * @param prevRegion
   * @param currentRegion
   * @param prevKey
   * @param prevValue
   * @param key
   * @param value
   * @param rectLeft
   * @param rectTop
   * @param rectRight
   * @param rectBottom
   * @return QVector<QPointF>
   */
  QVector<QPointF> getOptimizedCornerPoints(int prevRegion, int currentRegion, double prevKey, double prevValue, double key, double value, double rectLeft, double rectTop, double rectRight, double rectBottom) const;
  /**
   * @brief 
   *
   * @param prevRegion
   * @param currentRegion
   * @return bool
   */
  bool mayTraverse(int prevRegion, int currentRegion) const;
  /**
   * @brief 
   *
   * @param prevKey
   * @param prevValue
   * @param key
   * @param value
   * @param rectLeft
   * @param rectTop
   * @param rectRight
   * @param rectBottom
   * @param crossA
   * @param crossB
   * @return bool
   */
  bool getTraverse(double prevKey, double prevValue, double key, double value, double rectLeft, double rectTop, double rectRight, double rectBottom, QPointF &crossA, QPointF &crossB) const;
  /**
   * @brief 
   *
   * @param prevRegion
   * @param currentRegion
   * @param rectLeft
   * @param rectTop
   * @param rectRight
   * @param rectBottom
   * @param beforeTraverse
   * @param afterTraverse
   */
  void getTraverseCornerPoints(int prevRegion, int currentRegion, double rectLeft, double rectTop, double rectRight, double rectBottom, QVector<QPointF> &beforeTraverse, QVector<QPointF> &afterTraverse) const;
  /**
   * @brief 
   *
   * @param pixelPoint
   * @return double
   */
  double pointDistance(const QPointF &pixelPoint) const;
  
  friend class QCustomPlot;
  friend class QCPLegend;
};


/*! \file */



/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPBarsGroup : public QObject
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(SpacingType spacingType READ spacingType WRITE setSpacingType)
  Q_PROPERTY(double spacing READ spacing WRITE setSpacing)
  /// \endcond
public:
  /*!
    Defines the ways the spacing between bars in the group can be specified. Thus it defines what
    the number passed to \ref setSpacing actually means.
    
    \see setSpacingType, setSpacing
  */
  enum SpacingType { stAbsolute       ///< Bar spacing is in absolute pixels
                     ,stAxisRectRatio ///< Bar spacing is given by a fraction of the axis rect size
                     ,stPlotCoords    ///< Bar spacing is in key coordinates and thus scales with the key axis range
                 };
  /**
   * @brief 
   *
   * @param parentPlot
   */
  QCPBarsGroup(QCustomPlot *parentPlot);
  /**
   * @brief 
   *
   */
  ~QCPBarsGroup();
  
  // getters:
  /**
   * @brief 
   *
   * @return SpacingType
   */
  SpacingType spacingType() const { return mSpacingType; }
  /**
   * @brief 
   *
   * @return double
   */
  double spacing() const { return mSpacing; }
  
  // setters:
  /**
   * @brief 
   *
   * @param spacingType
   */
  void setSpacingType(SpacingType spacingType);
  /**
   * @brief 
   *
   * @param spacing
   */
  void setSpacing(double spacing);
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @return QList<QCPBars *>
   */
  QList<QCPBars*> bars() const { return mBars; }
  /**
   * @brief 
   *
   * @param index
   * @return QCPBars
   */
  QCPBars* bars(int index) const;
  /**
   * @brief 
   *
   * @return int
   */
  int size() const { return mBars.size(); }
  /**
   * @brief 
   *
   * @return bool
   */
  bool isEmpty() const { return mBars.isEmpty(); }
  /**
   * @brief 
   *
   */
  void clear();
  /**
   * @brief 
   *
   * @param bars
   * @return bool
   */
  bool contains(QCPBars *bars) const { return mBars.contains(bars); }
  /**
   * @brief 
   *
   * @param bars
   */
  void append(QCPBars *bars);
  /**
   * @brief 
   *
   * @param i
   * @param bars
   */
  void insert(int i, QCPBars *bars);
  /**
   * @brief 
   *
   * @param bars
   */
  void remove(QCPBars *bars);
  
protected:
  // non-property members:
  QCustomPlot *mParentPlot; /**< TODO: describe */
  SpacingType mSpacingType; /**< TODO: describe */
  double mSpacing; /**< TODO: describe */
  QList<QCPBars*> mBars; /**< TODO: describe */
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @param bars
   */
  void registerBars(QCPBars *bars);
  /**
   * @brief 
   *
   * @param bars
   */
  void unregisterBars(QCPBars *bars);
  
  // virtual methods:
  /**
   * @brief 
   *
   * @param bars
   * @param keyCoord
   * @return double
   */
  double keyPixelOffset(const QCPBars *bars, double keyCoord);
  /**
   * @brief 
   *
   * @param bars
   * @param keyCoord
   * @return double
   */
  double getPixelSpacing(const QCPBars *bars, double keyCoord);
  
private:
  /**
   * @brief 
   *
   * @param 
   */
  Q_DISABLE_COPY(QCPBarsGroup)
  
  friend class QCPBars;
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPBarData
{
public:
  /**
   * @brief 
   *
   */
  QCPBarData();
  /**
   * @brief 
   *
   * @param key
   * @param value
   */
  QCPBarData(double key, double value);
  double key, value; /**< TODO: describe */
};
Q_DECLARE_TYPEINFO(QCPBarData, Q_MOVABLE_TYPE);

/*! \typedef QCPBarDataMap
  Container for storing \ref QCPBarData items in a sorted fashion. The key of the map
  is the key member of the QCPBarData instance.
  
  This is the container in which QCPBars holds its data.
  \see QCPBarData, QCPBars::setData
*/
typedef QMap<double, QCPBarData> QCPBarDataMap;
typedef QMapIterator<double, QCPBarData> QCPBarDataMapIterator;
typedef QMutableMapIterator<double, QCPBarData> QCPBarDataMutableMapIterator;


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPBars : public QCPAbstractPlottable
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(double width READ width WRITE setWidth)
  Q_PROPERTY(WidthType widthType READ widthType WRITE setWidthType)
  Q_PROPERTY(QCPBarsGroup* barsGroup READ barsGroup WRITE setBarsGroup)
  Q_PROPERTY(double baseValue READ baseValue WRITE setBaseValue)
  Q_PROPERTY(QCPBars* barBelow READ barBelow)
  Q_PROPERTY(QCPBars* barAbove READ barAbove)
  /// \endcond
public:
  /*!
    Defines the ways the width of the bar can be specified. Thus it defines what the number passed
    to \ref setWidth actually means.
    
    \see setWidthType, setWidth
  */
  enum WidthType { wtAbsolute       ///< Bar width is in absolute pixels
                   ,wtAxisRectRatio ///< Bar width is given by a fraction of the axis rect size
                   ,wtPlotCoords    ///< Bar width is in key coordinates and thus scales with the key axis range
                 };
   Q_ENUMS(WidthType)
  
  /**
   * @brief 
   *
   * @param keyAxis
   * @param valueAxis
   */
  explicit QCPBars(QCPAxis *keyAxis, QCPAxis *valueAxis);
  /**
   * @brief 
   *
   */
  virtual ~QCPBars();
  
  // getters:
  /**
   * @brief 
   *
   * @return double
   */
  double width() const { return mWidth; }
  /**
   * @brief 
   *
   * @return WidthType
   */
  WidthType widthType() const { return mWidthType; }
  /**
   * @brief 
   *
   * @return QCPBarsGroup
   */
  QCPBarsGroup *barsGroup() const { return mBarsGroup; }
  /**
   * @brief 
   *
   * @return double
   */
  double baseValue() const { return mBaseValue; }
  /**
   * @brief 
   *
   * @return QCPBars
   */
  QCPBars *barBelow() const { return mBarBelow.data(); }
  /**
   * @brief 
   *
   * @return QCPBars
   */
  QCPBars *barAbove() const { return mBarAbove.data(); }
  /**
   * @brief 
   *
   * @return QCPBarDataMap
   */
  QCPBarDataMap *data() const { return mData; }
  
  // setters:
  /**
   * @brief 
   *
   * @param width
   */
  void setWidth(double width);
  /**
   * @brief 
   *
   * @param widthType
   */
  void setWidthType(WidthType widthType);
  /**
   * @brief 
   *
   * @param barsGroup
   */
  void setBarsGroup(QCPBarsGroup *barsGroup);
  /**
   * @brief 
   *
   * @param baseValue
   */
  void setBaseValue(double baseValue);
  /**
   * @brief 
   *
   * @param data
   * @param copy
   */
  void setData(QCPBarDataMap *data, bool copy=false);
  /**
   * @brief 
   *
   * @param key
   * @param value
   */
  void setData(const QVector<double> &key, const QVector<double> &value);
  
  // non-property methods:
  /**
   * @brief 
   *
   * @param bars
   */
  void moveBelow(QCPBars *bars);
  /**
   * @brief 
   *
   * @param bars
   */
  void moveAbove(QCPBars *bars);
  /**
   * @brief 
   *
   * @param dataMap
   */
  void addData(const QCPBarDataMap &dataMap);
  /**
   * @brief 
   *
   * @param data
   */
  void addData(const QCPBarData &data);
  /**
   * @brief 
   *
   * @param key
   * @param value
   */
  void addData(double key, double value);
  /**
   * @brief 
   *
   * @param keys
   * @param values
   */
  void addData(const QVector<double> &keys, const QVector<double> &values);
  /**
   * @brief 
   *
   * @param key
   */
  void removeDataBefore(double key);
  /**
   * @brief 
   *
   * @param key
   */
  void removeDataAfter(double key);
  /**
   * @brief 
   *
   * @param fromKey
   * @param toKey
   */
  void removeData(double fromKey, double toKey);
  /**
   * @brief 
   *
   * @param key
   */
  void removeData(double key);
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   */
  virtual void clearData();
  /**
   * @brief 
   *
   * @param pos
   * @param onlySelectable
   * @param details
   * @return double
   */
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
protected:
  // property members:
  QCPBarDataMap *mData; /**< TODO: describe */
  double mWidth; /**< TODO: describe */
  WidthType mWidthType; /**< TODO: describe */
  QCPBarsGroup *mBarsGroup; /**< TODO: describe */
  double mBaseValue; /**< TODO: describe */
  QPointer<QCPBars> mBarBelow, mBarAbove; /**< TODO: describe */
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter);
  /**
   * @brief 
   *
   * @param painter
   * @param rect
   */
  virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const;
  /**
   * @brief 
   *
   * @param foundRange
   * @param inSignDomain
   * @return QCPRange
   */
  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  /**
   * @brief 
   *
   * @param foundRange
   * @param inSignDomain
   * @return QCPRange
   */
  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @param lower
   * @param upperEnd
   */
  void getVisibleDataBounds(QCPBarDataMap::const_iterator &lower, QCPBarDataMap::const_iterator &upperEnd) const;
  /**
   * @brief 
   *
   * @param key
   * @param value
   * @return QPolygonF
   */
  QPolygonF getBarPolygon(double key, double value) const;
  /**
   * @brief 
   *
   * @param key
   * @param lower
   * @param upper
   */
  void getPixelWidth(double key, double &lower, double &upper) const;
  /**
   * @brief 
   *
   * @param key
   * @param positive
   * @return double
   */
  double getStackedBaseValue(double key, bool positive) const;
  /**
   * @brief 
   *
   * @param lower
   * @param upper
   */
  static void connectBars(QCPBars* lower, QCPBars* upper);
  
  friend class QCustomPlot;
  friend class QCPLegend;
  friend class QCPBarsGroup;
};


/*! \file */



/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPStatisticalBox : public QCPAbstractPlottable
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(double key READ key WRITE setKey)
  Q_PROPERTY(double minimum READ minimum WRITE setMinimum)
  Q_PROPERTY(double lowerQuartile READ lowerQuartile WRITE setLowerQuartile)
  Q_PROPERTY(double median READ median WRITE setMedian)
  Q_PROPERTY(double upperQuartile READ upperQuartile WRITE setUpperQuartile)
  Q_PROPERTY(double maximum READ maximum WRITE setMaximum)
  Q_PROPERTY(QVector<double> outliers READ outliers WRITE setOutliers)
  Q_PROPERTY(double width READ width WRITE setWidth)
  Q_PROPERTY(double whiskerWidth READ whiskerWidth WRITE setWhiskerWidth)
  Q_PROPERTY(QPen whiskerPen READ whiskerPen WRITE setWhiskerPen)
  Q_PROPERTY(QPen whiskerBarPen READ whiskerBarPen WRITE setWhiskerBarPen)
  Q_PROPERTY(QPen medianPen READ medianPen WRITE setMedianPen)
  Q_PROPERTY(QCPScatterStyle outlierStyle READ outlierStyle WRITE setOutlierStyle)
  /// \endcond
public:
  /**
   * @brief 
   *
   * @param keyAxis
   * @param valueAxis
   */
  explicit QCPStatisticalBox(QCPAxis *keyAxis, QCPAxis *valueAxis);
  
  // getters:
  /**
   * @brief 
   *
   * @return double
   */
  double key() const { return mKey; }
  /**
   * @brief 
   *
   * @return double
   */
  double minimum() const { return mMinimum; }
  /**
   * @brief 
   *
   * @return double
   */
  double lowerQuartile() const { return mLowerQuartile; }
  /**
   * @brief 
   *
   * @return double
   */
  double median() const { return mMedian; }
  /**
   * @brief 
   *
   * @return double
   */
  double upperQuartile() const { return mUpperQuartile; }
  /**
   * @brief 
   *
   * @return double
   */
  double maximum() const { return mMaximum; }
  /**
   * @brief 
   *
   * @return QVector<double>
   */
  QVector<double> outliers() const { return mOutliers; }
  /**
   * @brief 
   *
   * @return double
   */
  double width() const { return mWidth; }
  /**
   * @brief 
   *
   * @return double
   */
  double whiskerWidth() const { return mWhiskerWidth; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen whiskerPen() const { return mWhiskerPen; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen whiskerBarPen() const { return mWhiskerBarPen; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen medianPen() const { return mMedianPen; }
  /**
   * @brief 
   *
   * @return QCPScatterStyle
   */
  QCPScatterStyle outlierStyle() const { return mOutlierStyle; }

  // setters:
  /**
   * @brief 
   *
   * @param key
   */
  void setKey(double key);
  /**
   * @brief 
   *
   * @param value
   */
  void setMinimum(double value);
  /**
   * @brief 
   *
   * @param value
   */
  void setLowerQuartile(double value);
  /**
   * @brief 
   *
   * @param value
   */
  void setMedian(double value);
  /**
   * @brief 
   *
   * @param value
   */
  void setUpperQuartile(double value);
  /**
   * @brief 
   *
   * @param value
   */
  void setMaximum(double value);
  /**
   * @brief 
   *
   * @param values
   */
  void setOutliers(const QVector<double> &values);
  /**
   * @brief 
   *
   * @param key
   * @param minimum
   * @param lowerQuartile
   * @param median
   * @param upperQuartile
   * @param maximum
   */
  void setData(double key, double minimum, double lowerQuartile, double median, double upperQuartile, double maximum);
  /**
   * @brief 
   *
   * @param width
   */
  void setWidth(double width);
  /**
   * @brief 
   *
   * @param width
   */
  void setWhiskerWidth(double width);
  /**
   * @brief 
   *
   * @param pen
   */
  void setWhiskerPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param pen
   */
  void setWhiskerBarPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param pen
   */
  void setMedianPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param style
   */
  void setOutlierStyle(const QCPScatterStyle &style);
  
  // non-property methods:
  /**
   * @brief 
   *
   */
  virtual void clearData();
  /**
   * @brief 
   *
   * @param pos
   * @param onlySelectable
   * @param details
   * @return double
   */
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
protected:
  // property members:
  QVector<double> mOutliers; /**< TODO: describe */
  double mKey, mMinimum, mLowerQuartile, mMedian, mUpperQuartile, mMaximum; /**< TODO: describe */
  double mWidth; /**< TODO: describe */
  double mWhiskerWidth; /**< TODO: describe */
  QPen mWhiskerPen, mWhiskerBarPen, mMedianPen; /**< TODO: describe */
  QCPScatterStyle mOutlierStyle; /**< TODO: describe */
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter);
  /**
   * @brief 
   *
   * @param painter
   * @param rect
   */
  virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const;
  /**
   * @brief 
   *
   * @param foundRange
   * @param inSignDomain
   * @return QCPRange
   */
  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  /**
   * @brief 
   *
   * @param foundRange
   * @param inSignDomain
   * @return QCPRange
   */
  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  
  // introduced virtual methods:
  /**
   * @brief 
   *
   * @param painter
   * @param quartileBox
   */
  virtual void drawQuartileBox(QCPPainter *painter, QRectF *quartileBox=0) const;
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void drawMedian(QCPPainter *painter) const;
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void drawWhiskers(QCPPainter *painter) const;
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void drawOutliers(QCPPainter *painter) const;
  
  friend class QCustomPlot;
  friend class QCPLegend;
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPColorMapData
{
public:
  /**
   * @brief 
   *
   * @param keySize
   * @param valueSize
   * @param keyRange
   * @param valueRange
   */
  QCPColorMapData(int keySize, int valueSize, const QCPRange &keyRange, const QCPRange &valueRange);
  /**
   * @brief 
   *
   */
  ~QCPColorMapData();
  /**
   * @brief 
   *
   * @param other
   */
  QCPColorMapData(const QCPColorMapData &other);
  /**
   * @brief 
   *
   * @param other
   * @return QCPColorMapData &operator
   */
  QCPColorMapData &operator=(const QCPColorMapData &other);
  
  // getters:
  /**
   * @brief 
   *
   * @return int
   */
  int keySize() const { return mKeySize; }
  /**
   * @brief 
   *
   * @return int
   */
  int valueSize() const { return mValueSize; }
  /**
   * @brief 
   *
   * @return QCPRange
   */
  QCPRange keyRange() const { return mKeyRange; }
  /**
   * @brief 
   *
   * @return QCPRange
   */
  QCPRange valueRange() const { return mValueRange; }
  /**
   * @brief 
   *
   * @return QCPRange
   */
  QCPRange dataBounds() const { return mDataBounds; }
  /**
   * @brief 
   *
   * @param key
   * @param value
   * @return double
   */
  double data(double key, double value);
  /**
   * @brief 
   *
   * @param keyIndex
   * @param valueIndex
   * @return double
   */
  double cell(int keyIndex, int valueIndex);
  
  // setters:
  /**
   * @brief 
   *
   * @param keySize
   * @param valueSize
   */
  void setSize(int keySize, int valueSize);
  /**
   * @brief 
   *
   * @param keySize
   */
  void setKeySize(int keySize);
  /**
   * @brief 
   *
   * @param valueSize
   */
  void setValueSize(int valueSize);
  /**
   * @brief 
   *
   * @param keyRange
   * @param valueRange
   */
  void setRange(const QCPRange &keyRange, const QCPRange &valueRange);
  /**
   * @brief 
   *
   * @param keyRange
   */
  void setKeyRange(const QCPRange &keyRange);
  /**
   * @brief 
   *
   * @param valueRange
   */
  void setValueRange(const QCPRange &valueRange);
  /**
   * @brief 
   *
   * @param key
   * @param value
   * @param z
   */
  void setData(double key, double value, double z);
  /**
   * @brief 
   *
   * @param keyIndex
   * @param valueIndex
   * @param z
   */
  void setCell(int keyIndex, int valueIndex, double z);
  
  // non-property methods:
  /**
   * @brief 
   *
   */
  void recalculateDataBounds();
  /**
   * @brief 
   *
   */
  void clear();
  /**
   * @brief 
   *
   * @param z
   */
  void fill(double z);
  /**
   * @brief 
   *
   * @return bool
   */
  bool isEmpty() const { return mIsEmpty; }
  /**
   * @brief 
   *
   * @param key
   * @param value
   * @param keyIndex
   * @param valueIndex
   */
  void coordToCell(double key, double value, int *keyIndex, int *valueIndex) const;
  /**
   * @brief 
   *
   * @param keyIndex
   * @param valueIndex
   * @param key
   * @param value
   */
  void cellToCoord(int keyIndex, int valueIndex, double *key, double *value) const;
  
protected:
  // property members:
  int mKeySize, mValueSize; /**< TODO: describe */
  QCPRange mKeyRange, mValueRange; /**< TODO: describe */
  bool mIsEmpty; /**< TODO: describe */
  // non-property members:
  double *mData; /**< TODO: describe */
  QCPRange mDataBounds; /**< TODO: describe */
  bool mDataModified; /**< TODO: describe */
  
  friend class QCPColorMap;
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPColorMap : public QCPAbstractPlottable
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(QCPRange dataRange READ dataRange WRITE setDataRange NOTIFY dataRangeChanged)
  Q_PROPERTY(QCPAxis::ScaleType dataScaleType READ dataScaleType WRITE setDataScaleType NOTIFY dataScaleTypeChanged)
  Q_PROPERTY(QCPColorGradient gradient READ gradient WRITE setGradient NOTIFY gradientChanged)
  Q_PROPERTY(bool interpolate READ interpolate WRITE setInterpolate)
  Q_PROPERTY(bool tightBoundary READ tightBoundary WRITE setTightBoundary)
  Q_PROPERTY(QCPColorScale* colorScale READ colorScale WRITE setColorScale)
  /// \endcond
public:
  /**
   * @brief 
   *
   * @param keyAxis
   * @param valueAxis
   */
  explicit QCPColorMap(QCPAxis *keyAxis, QCPAxis *valueAxis);
  /**
   * @brief 
   *
   */
  virtual ~QCPColorMap();
  
  // getters:
  /**
   * @brief 
   *
   * @return QCPColorMapData
   */
  QCPColorMapData *data() const { return mMapData; }
  /**
   * @brief 
   *
   * @return QCPRange
   */
  QCPRange dataRange() const { return mDataRange; }
  /**
   * @brief 
   *
   * @return QCPAxis::ScaleType
   */
  QCPAxis::ScaleType dataScaleType() const { return mDataScaleType; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool interpolate() const { return mInterpolate; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool tightBoundary() const { return mTightBoundary; }
  /**
   * @brief 
   *
   * @return QCPColorGradient
   */
  QCPColorGradient gradient() const { return mGradient; }
  /**
   * @brief 
   *
   * @return QCPColorScale
   */
  QCPColorScale *colorScale() const { return mColorScale.data(); }
  
  // setters:
  /**
   * @brief 
   *
   * @param data
   * @param copy
   */
  void setData(QCPColorMapData *data, bool copy=false);
  /**
   * @brief 
   *
   * @param dataRange
   */
  Q_SLOT void setDataRange(const QCPRange &dataRange);
  /**
   * @brief 
   *
   * @param scaleType
   */
  Q_SLOT void setDataScaleType(QCPAxis::ScaleType scaleType);
  /**
   * @brief 
   *
   * @param gradient
   */
  Q_SLOT void setGradient(const QCPColorGradient &gradient);
  /**
   * @brief 
   *
   * @param enabled
   */
  void setInterpolate(bool enabled);
  /**
   * @brief 
   *
   * @param enabled
   */
  void setTightBoundary(bool enabled);
  /**
   * @brief 
   *
   * @param colorScale
   */
  void setColorScale(QCPColorScale *colorScale);
  
  // non-property methods:
  /**
   * @brief 
   *
   * @param recalculateDataBounds
   */
  void rescaleDataRange(bool recalculateDataBounds=false);
  /**
   * @brief 
   *
   * @param transformMode
   * @param thumbSize
   */
  Q_SLOT void updateLegendIcon(Qt::TransformationMode transformMode=Qt::SmoothTransformation, const QSize &thumbSize=QSize(32, 18));
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   */
  virtual void clearData();
  /**
   * @brief 
   *
   * @param pos
   * @param onlySelectable
   * @param details
   * @return double
   */
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
signals:
  /**
   * @brief 
   *
   * @param newRange
   */
  void dataRangeChanged(QCPRange newRange);
  /**
   * @brief 
   *
   * @param scaleType
   */
  void dataScaleTypeChanged(QCPAxis::ScaleType scaleType);
  /**
   * @brief 
   *
   * @param newGradient
   */
  void gradientChanged(QCPColorGradient newGradient);
  
protected:
  // property members:
  QCPRange mDataRange; /**< TODO: describe */
  QCPAxis::ScaleType mDataScaleType; /**< TODO: describe */
  QCPColorMapData *mMapData; /**< TODO: describe */
  QCPColorGradient mGradient; /**< TODO: describe */
  bool mInterpolate; /**< TODO: describe */
  bool mTightBoundary; /**< TODO: describe */
  QPointer<QCPColorScale> mColorScale; /**< TODO: describe */
  // non-property members:
  QImage mMapImage, mUndersampledMapImage; /**< TODO: describe */
  QPixmap mLegendIcon; /**< TODO: describe */
  bool mMapImageInvalidated; /**< TODO: describe */
  
  // introduced virtual methods:
  /**
   * @brief 
   *
   */
  virtual void updateMapImage();
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter);
  /**
   * @brief 
   *
   * @param painter
   * @param rect
   */
  virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const;
  /**
   * @brief 
   *
   * @param foundRange
   * @param inSignDomain
   * @return QCPRange
   */
  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  /**
   * @brief 
   *
   * @param foundRange
   * @param inSignDomain
   * @return QCPRange
   */
  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  
  friend class QCustomPlot;
  friend class QCPLegend;
};


/*! \file */



/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPFinancialData
{
public:
  /**
   * @brief 
   *
   */
  QCPFinancialData();
  /**
   * @brief 
   *
   * @param key
   * @param open
   * @param high
   * @param low
   * @param close
   */
  QCPFinancialData(double key, double open, double high, double low, double close);
  double key, open, high, low, close; /**< TODO: describe */
};
Q_DECLARE_TYPEINFO(QCPFinancialData, Q_MOVABLE_TYPE);

/*! \typedef QCPFinancialDataMap
  Container for storing \ref QCPFinancialData items in a sorted fashion. The key of the map
  is the key member of the QCPFinancialData instance.
  
  This is the container in which QCPFinancial holds its data.
  \see QCPFinancial, QCPFinancial::setData
*/
typedef QMap<double, QCPFinancialData> QCPFinancialDataMap;
typedef QMapIterator<double, QCPFinancialData> QCPFinancialDataMapIterator;
typedef QMutableMapIterator<double, QCPFinancialData> QCPFinancialDataMutableMapIterator;


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPFinancial : public QCPAbstractPlottable
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(ChartStyle chartStyle READ chartStyle WRITE setChartStyle)
  Q_PROPERTY(double width READ width WRITE setWidth)
  Q_PROPERTY(bool twoColored READ twoColored WRITE setTwoColored)
  Q_PROPERTY(QBrush brushPositive READ brushPositive WRITE setBrushPositive)
  Q_PROPERTY(QBrush brushNegative READ brushNegative WRITE setBrushNegative)
  Q_PROPERTY(QPen penPositive READ penPositive WRITE setPenPositive)
  Q_PROPERTY(QPen penNegative READ penNegative WRITE setPenNegative)
  /// \endcond
public:
  /*!
    Defines the possible representations of OHLC data in the plot.
    
    \see setChartStyle
  */
  enum ChartStyle { csOhlc         ///< Open-High-Low-Close bar representation
                   ,csCandlestick  ///< Candlestick representation
                  };
  Q_ENUMS(ChartStyle)
  
  /**
   * @brief 
   *
   * @param keyAxis
   * @param valueAxis
   */
  explicit QCPFinancial(QCPAxis *keyAxis, QCPAxis *valueAxis);
  /**
   * @brief 
   *
   */
  virtual ~QCPFinancial();
  
  // getters:
  /**
   * @brief 
   *
   * @return QCPFinancialDataMap
   */
  QCPFinancialDataMap *data() const { return mData; }
  /**
   * @brief 
   *
   * @return ChartStyle
   */
  ChartStyle chartStyle() const { return mChartStyle; }
  /**
   * @brief 
   *
   * @return double
   */
  double width() const { return mWidth; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool twoColored() const { return mTwoColored; }
  /**
   * @brief 
   *
   * @return QBrush
   */
  QBrush brushPositive() const { return mBrushPositive; }
  /**
   * @brief 
   *
   * @return QBrush
   */
  QBrush brushNegative() const { return mBrushNegative; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen penPositive() const { return mPenPositive; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen penNegative() const { return mPenNegative; }
  
  
  // setters:
  /**
   * @brief 
   *
   * @param data
   * @param copy
   */
  void setData(QCPFinancialDataMap *data, bool copy=false);
  /**
   * @brief 
   *
   * @param key
   * @param open
   * @param high
   * @param low
   * @param close
   */
  void setData(const QVector<double> &key, const QVector<double> &open, const QVector<double> &high, const QVector<double> &low, const QVector<double> &close);
  /**
   * @brief 
   *
   * @param style
   */
  void setChartStyle(ChartStyle style);
  /**
   * @brief 
   *
   * @param width
   */
  void setWidth(double width);
  /**
   * @brief 
   *
   * @param twoColored
   */
  void setTwoColored(bool twoColored);
  /**
   * @brief 
   *
   * @param brush
   */
  void setBrushPositive(const QBrush &brush);
  /**
   * @brief 
   *
   * @param brush
   */
  void setBrushNegative(const QBrush &brush);
  /**
   * @brief 
   *
   * @param pen
   */
  void setPenPositive(const QPen &pen);
  /**
   * @brief 
   *
   * @param pen
   */
  void setPenNegative(const QPen &pen);
  
  // non-property methods:
  /**
   * @brief 
   *
   * @param dataMap
   */
  void addData(const QCPFinancialDataMap &dataMap);
  /**
   * @brief 
   *
   * @param data
   */
  void addData(const QCPFinancialData &data);
  /**
   * @brief 
   *
   * @param key
   * @param open
   * @param high
   * @param low
   * @param close
   */
  void addData(double key, double open, double high, double low, double close);
  /**
   * @brief 
   *
   * @param key
   * @param open
   * @param high
   * @param low
   * @param close
   */
  void addData(const QVector<double> &key, const QVector<double> &open, const QVector<double> &high, const QVector<double> &low, const QVector<double> &close);
  /**
   * @brief 
   *
   * @param key
   */
  void removeDataBefore(double key);
  /**
   * @brief 
   *
   * @param key
   */
  void removeDataAfter(double key);
  /**
   * @brief 
   *
   * @param fromKey
   * @param toKey
   */
  void removeData(double fromKey, double toKey);
  /**
   * @brief 
   *
   * @param key
   */
  void removeData(double key);
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   */
  virtual void clearData();
  /**
   * @brief 
   *
   * @param pos
   * @param onlySelectable
   * @param details
   * @return double
   */
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
  // static methods:
  /**
   * @brief 
   *
   * @param time
   * @param value
   * @param timeBinSize
   * @param timeBinOffset
   * @return QCPFinancialDataMap
   */
  static QCPFinancialDataMap timeSeriesToOhlc(const QVector<double> &time, const QVector<double> &value, double timeBinSize, double timeBinOffset = 0);
  
protected:
  // property members:
  QCPFinancialDataMap *mData; /**< TODO: describe */
  ChartStyle mChartStyle; /**< TODO: describe */
  double mWidth; /**< TODO: describe */
  bool mTwoColored; /**< TODO: describe */
  QBrush mBrushPositive, mBrushNegative; /**< TODO: describe */
  QPen mPenPositive, mPenNegative; /**< TODO: describe */
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter);
  /**
   * @brief 
   *
   * @param painter
   * @param rect
   */
  virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const;
  /**
   * @brief 
   *
   * @param foundRange
   * @param inSignDomain
   * @return QCPRange
   */
  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  /**
   * @brief 
   *
   * @param foundRange
   * @param inSignDomain
   * @return QCPRange
   */
  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @param painter
   * @param begin
   * @param end
   */
  void drawOhlcPlot(QCPPainter *painter, const QCPFinancialDataMap::const_iterator &begin, const QCPFinancialDataMap::const_iterator &end);
  /**
   * @brief 
   *
   * @param painter
   * @param begin
   * @param end
   */
  void drawCandlestickPlot(QCPPainter *painter, const QCPFinancialDataMap::const_iterator &begin, const QCPFinancialDataMap::const_iterator &end);
  /**
   * @brief 
   *
   * @param pos
   * @param begin
   * @param end
   * @return double
   */
  double ohlcSelectTest(const QPointF &pos, const QCPFinancialDataMap::const_iterator &begin, const QCPFinancialDataMap::const_iterator &end) const;
  /**
   * @brief 
   *
   * @param pos
   * @param begin
   * @param end
   * @return double
   */
  double candlestickSelectTest(const QPointF &pos, const QCPFinancialDataMap::const_iterator &begin, const QCPFinancialDataMap::const_iterator &end) const;
  /**
   * @brief 
   *
   * @param lower
   * @param upper
   */
  void getVisibleDataBounds(QCPFinancialDataMap::const_iterator &lower, QCPFinancialDataMap::const_iterator &upper) const;
  
  friend class QCustomPlot;
  friend class QCPLegend;
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPItemStraightLine : public QCPAbstractItem
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(QPen pen READ pen WRITE setPen)
  Q_PROPERTY(QPen selectedPen READ selectedPen WRITE setSelectedPen)
  /// \endcond
public:
  /**
   * @brief 
   *
   * @param parentPlot
   */
  QCPItemStraightLine(QCustomPlot *parentPlot);
  /**
   * @brief 
   *
   */
  virtual ~QCPItemStraightLine();
  
  // getters:
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen pen() const { return mPen; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen selectedPen() const { return mSelectedPen; }
  
  // setters;
  /**
   * @brief 
   *
   * @param pen
   */
  void setPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param pen
   */
  void setSelectedPen(const QPen &pen);
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param pos
   * @param onlySelectable
   * @param details
   * @return double
   */
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
  QCPItemPosition * const point1; /**< TODO: describe */
  QCPItemPosition * const point2; /**< TODO: describe */
  
protected:
  // property members:
  QPen mPen, mSelectedPen; /**< TODO: describe */
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter);
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @param point1
   * @param vec
   * @param point
   * @return double
   */
  double distToStraightLine(const QVector2D &point1, const QVector2D &vec, const QVector2D &point) const;
  /**
   * @brief 
   *
   * @param point1
   * @param vec
   * @param rect
   * @return QLineF
   */
  QLineF getRectClippedStraightLine(const QVector2D &point1, const QVector2D &vec, const QRect &rect) const;
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen mainPen() const;
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPItemLine : public QCPAbstractItem
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(QPen pen READ pen WRITE setPen)
  Q_PROPERTY(QPen selectedPen READ selectedPen WRITE setSelectedPen)
  Q_PROPERTY(QCPLineEnding head READ head WRITE setHead)
  Q_PROPERTY(QCPLineEnding tail READ tail WRITE setTail)
  /// \endcond
public:
  /**
   * @brief 
   *
   * @param parentPlot
   */
  QCPItemLine(QCustomPlot *parentPlot);
  /**
   * @brief 
   *
   */
  virtual ~QCPItemLine();
  
  // getters:
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen pen() const { return mPen; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen selectedPen() const { return mSelectedPen; }
  /**
   * @brief 
   *
   * @return QCPLineEnding
   */
  QCPLineEnding head() const { return mHead; }
  /**
   * @brief 
   *
   * @return QCPLineEnding
   */
  QCPLineEnding tail() const { return mTail; }
  
  // setters;
  /**
   * @brief 
   *
   * @param pen
   */
  void setPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param pen
   */
  void setSelectedPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param head
   */
  void setHead(const QCPLineEnding &head);
  /**
   * @brief 
   *
   * @param tail
   */
  void setTail(const QCPLineEnding &tail);
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param pos
   * @param onlySelectable
   * @param details
   * @return double
   */
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
  QCPItemPosition * const start; /**< TODO: describe */
  QCPItemPosition * const end; /**< TODO: describe */
  
protected:
  // property members:
  QPen mPen, mSelectedPen; /**< TODO: describe */
  QCPLineEnding mHead, mTail; /**< TODO: describe */
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter);
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @param start
   * @param end
   * @param rect
   * @return QLineF
   */
  QLineF getRectClippedLine(const QVector2D &start, const QVector2D &end, const QRect &rect) const;
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen mainPen() const;
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPItemCurve : public QCPAbstractItem
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(QPen pen READ pen WRITE setPen)
  Q_PROPERTY(QPen selectedPen READ selectedPen WRITE setSelectedPen)
  Q_PROPERTY(QCPLineEnding head READ head WRITE setHead)
  Q_PROPERTY(QCPLineEnding tail READ tail WRITE setTail)
  /// \endcond
public:
  /**
   * @brief 
   *
   * @param parentPlot
   */
  QCPItemCurve(QCustomPlot *parentPlot);
  /**
   * @brief 
   *
   */
  virtual ~QCPItemCurve();
  
  // getters:
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen pen() const { return mPen; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen selectedPen() const { return mSelectedPen; }
  /**
   * @brief 
   *
   * @return QCPLineEnding
   */
  QCPLineEnding head() const { return mHead; }
  /**
   * @brief 
   *
   * @return QCPLineEnding
   */
  QCPLineEnding tail() const { return mTail; }
  
  // setters;
  /**
   * @brief 
   *
   * @param pen
   */
  void setPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param pen
   */
  void setSelectedPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param head
   */
  void setHead(const QCPLineEnding &head);
  /**
   * @brief 
   *
   * @param tail
   */
  void setTail(const QCPLineEnding &tail);
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param pos
   * @param onlySelectable
   * @param details
   * @return double
   */
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
  QCPItemPosition * const start; /**< TODO: describe */
  QCPItemPosition * const startDir; /**< TODO: describe */
  QCPItemPosition * const endDir; /**< TODO: describe */
  QCPItemPosition * const end; /**< TODO: describe */
  
protected:
  // property members:
  QPen mPen, mSelectedPen; /**< TODO: describe */
  QCPLineEnding mHead, mTail; /**< TODO: describe */
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter);
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen mainPen() const;
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPItemRect : public QCPAbstractItem
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(QPen pen READ pen WRITE setPen)
  Q_PROPERTY(QPen selectedPen READ selectedPen WRITE setSelectedPen)
  Q_PROPERTY(QBrush brush READ brush WRITE setBrush)
  Q_PROPERTY(QBrush selectedBrush READ selectedBrush WRITE setSelectedBrush)
  /// \endcond
public:
  /**
   * @brief 
   *
   * @param parentPlot
   */
  QCPItemRect(QCustomPlot *parentPlot);
  /**
   * @brief 
   *
   */
  virtual ~QCPItemRect();
  
  // getters:
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen pen() const { return mPen; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen selectedPen() const { return mSelectedPen; }
  /**
   * @brief 
   *
   * @return QBrush
   */
  QBrush brush() const { return mBrush; }
  /**
   * @brief 
   *
   * @return QBrush
   */
  QBrush selectedBrush() const { return mSelectedBrush; }
  
  // setters;
  /**
   * @brief 
   *
   * @param pen
   */
  void setPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param pen
   */
  void setSelectedPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param brush
   */
  void setBrush(const QBrush &brush);
  /**
   * @brief 
   *
   * @param brush
   */
  void setSelectedBrush(const QBrush &brush);
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param pos
   * @param onlySelectable
   * @param details
   * @return double
   */
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
  QCPItemPosition * const topLeft; /**< TODO: describe */
  QCPItemPosition * const bottomRight; /**< TODO: describe */
  QCPItemAnchor * const top; /**< TODO: describe */
  QCPItemAnchor * const topRight; /**< TODO: describe */
  QCPItemAnchor * const right; /**< TODO: describe */
  QCPItemAnchor * const bottom; /**< TODO: describe */
  QCPItemAnchor * const bottomLeft; /**< TODO: describe */
  QCPItemAnchor * const left; /**< TODO: describe */
  
protected:
  /**
   * @brief 
   *
   */
  enum AnchorIndex {aiTop, aiTopRight, aiRight, aiBottom, aiBottomLeft, aiLeft};
  
  // property members:
  QPen mPen, mSelectedPen; /**< TODO: describe */
  QBrush mBrush, mSelectedBrush; /**< TODO: describe */
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter);
  /**
   * @brief 
   *
   * @param anchorId
   * @return QPointF
   */
  virtual QPointF anchorPixelPoint(int anchorId) const;
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen mainPen() const;
  /**
   * @brief 
   *
   * @return QBrush
   */
  QBrush mainBrush() const;
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPItemText : public QCPAbstractItem
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(QColor color READ color WRITE setColor)
  Q_PROPERTY(QColor selectedColor READ selectedColor WRITE setSelectedColor)
  Q_PROPERTY(QPen pen READ pen WRITE setPen)
  Q_PROPERTY(QPen selectedPen READ selectedPen WRITE setSelectedPen)
  Q_PROPERTY(QBrush brush READ brush WRITE setBrush)
  Q_PROPERTY(QBrush selectedBrush READ selectedBrush WRITE setSelectedBrush)
  Q_PROPERTY(QFont font READ font WRITE setFont)
  Q_PROPERTY(QFont selectedFont READ selectedFont WRITE setSelectedFont)
  Q_PROPERTY(QString text READ text WRITE setText)
  Q_PROPERTY(Qt::Alignment positionAlignment READ positionAlignment WRITE setPositionAlignment)
  Q_PROPERTY(Qt::Alignment textAlignment READ textAlignment WRITE setTextAlignment)
  Q_PROPERTY(double rotation READ rotation WRITE setRotation)
  Q_PROPERTY(QMargins padding READ padding WRITE setPadding)
  /// \endcond
public:
  /**
   * @brief 
   *
   * @param parentPlot
   */
  QCPItemText(QCustomPlot *parentPlot);
  /**
   * @brief 
   *
   */
  virtual ~QCPItemText();
  
  // getters:
  /**
   * @brief 
   *
   * @return QColor
   */
  QColor color() const { return mColor; }
  /**
   * @brief 
   *
   * @return QColor
   */
  QColor selectedColor() const { return mSelectedColor; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen pen() const { return mPen; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen selectedPen() const { return mSelectedPen; }
  /**
   * @brief 
   *
   * @return QBrush
   */
  QBrush brush() const { return mBrush; }
  /**
   * @brief 
   *
   * @return QBrush
   */
  QBrush selectedBrush() const { return mSelectedBrush; }
  /**
   * @brief 
   *
   * @return QFont
   */
  QFont font() const { return mFont; }
  /**
   * @brief 
   *
   * @return QFont
   */
  QFont selectedFont() const { return mSelectedFont; }
  /**
   * @brief 
   *
   * @return QString
   */
  QString text() const { return mText; }
  /**
   * @brief 
   *
   * @return Qt::Alignment
   */
  Qt::Alignment positionAlignment() const { return mPositionAlignment; }
  /**
   * @brief 
   *
   * @return Qt::Alignment
   */
  Qt::Alignment textAlignment() const { return mTextAlignment; }
  /**
   * @brief 
   *
   * @return double
   */
  double rotation() const { return mRotation; }
  /**
   * @brief 
   *
   * @return QMargins
   */
  QMargins padding() const { return mPadding; }
  
  // setters;
  /**
   * @brief 
   *
   * @param color
   */
  void setColor(const QColor &color);
  /**
   * @brief 
   *
   * @param color
   */
  void setSelectedColor(const QColor &color);
  /**
   * @brief 
   *
   * @param pen
   */
  void setPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param pen
   */
  void setSelectedPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param brush
   */
  void setBrush(const QBrush &brush);
  /**
   * @brief 
   *
   * @param brush
   */
  void setSelectedBrush(const QBrush &brush);
  /**
   * @brief 
   *
   * @param font
   */
  void setFont(const QFont &font);
  /**
   * @brief 
   *
   * @param font
   */
  void setSelectedFont(const QFont &font);
  /**
   * @brief 
   *
   * @param text
   */
  void setText(const QString &text);
  /**
   * @brief 
   *
   * @param alignment
   */
  void setPositionAlignment(Qt::Alignment alignment);
  /**
   * @brief 
   *
   * @param alignment
   */
  void setTextAlignment(Qt::Alignment alignment);
  /**
   * @brief 
   *
   * @param degrees
   */
  void setRotation(double degrees);
  /**
   * @brief 
   *
   * @param padding
   */
  void setPadding(const QMargins &padding);
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param pos
   * @param onlySelectable
   * @param details
   * @return double
   */
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
  QCPItemPosition * const position; /**< TODO: describe */
  QCPItemAnchor * const topLeft; /**< TODO: describe */
  QCPItemAnchor * const top; /**< TODO: describe */
  QCPItemAnchor * const topRight; /**< TODO: describe */
  QCPItemAnchor * const right; /**< TODO: describe */
  QCPItemAnchor * const bottomRight; /**< TODO: describe */
  QCPItemAnchor * const bottom; /**< TODO: describe */
  QCPItemAnchor * const bottomLeft; /**< TODO: describe */
  QCPItemAnchor * const left; /**< TODO: describe */
  
protected:
  /**
   * @brief 
   *
   */
  enum AnchorIndex {aiTopLeft, aiTop, aiTopRight, aiRight, aiBottomRight, aiBottom, aiBottomLeft, aiLeft};
  
  // property members:
  QColor mColor, mSelectedColor; /**< TODO: describe */
  QPen mPen, mSelectedPen; /**< TODO: describe */
  QBrush mBrush, mSelectedBrush; /**< TODO: describe */
  QFont mFont, mSelectedFont; /**< TODO: describe */
  QString mText; /**< TODO: describe */
  Qt::Alignment mPositionAlignment; /**< TODO: describe */
  Qt::Alignment mTextAlignment; /**< TODO: describe */
  double mRotation; /**< TODO: describe */
  QMargins mPadding; /**< TODO: describe */
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter);
  /**
   * @brief 
   *
   * @param anchorId
   * @return QPointF
   */
  virtual QPointF anchorPixelPoint(int anchorId) const;
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @param pos
   * @param rect
   * @param positionAlignment
   * @return QPointF
   */
  QPointF getTextDrawPoint(const QPointF &pos, const QRectF &rect, Qt::Alignment positionAlignment) const;
  /**
   * @brief 
   *
   * @return QFont
   */
  QFont mainFont() const;
  /**
   * @brief 
   *
   * @return QColor
   */
  QColor mainColor() const;
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen mainPen() const;
  /**
   * @brief 
   *
   * @return QBrush
   */
  QBrush mainBrush() const;
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPItemEllipse : public QCPAbstractItem
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(QPen pen READ pen WRITE setPen)
  Q_PROPERTY(QPen selectedPen READ selectedPen WRITE setSelectedPen)
  Q_PROPERTY(QBrush brush READ brush WRITE setBrush)
  Q_PROPERTY(QBrush selectedBrush READ selectedBrush WRITE setSelectedBrush)
  /// \endcond
public:
  /**
   * @brief 
   *
   * @param parentPlot
   */
  QCPItemEllipse(QCustomPlot *parentPlot);
  /**
   * @brief 
   *
   */
  virtual ~QCPItemEllipse();
  
  // getters:
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen pen() const { return mPen; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen selectedPen() const { return mSelectedPen; }
  /**
   * @brief 
   *
   * @return QBrush
   */
  QBrush brush() const { return mBrush; }
  /**
   * @brief 
   *
   * @return QBrush
   */
  QBrush selectedBrush() const { return mSelectedBrush; }
  
  // setters;
  /**
   * @brief 
   *
   * @param pen
   */
  void setPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param pen
   */
  void setSelectedPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param brush
   */
  void setBrush(const QBrush &brush);
  /**
   * @brief 
   *
   * @param brush
   */
  void setSelectedBrush(const QBrush &brush);
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param pos
   * @param onlySelectable
   * @param details
   * @return double
   */
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
  QCPItemPosition * const topLeft; /**< TODO: describe */
  QCPItemPosition * const bottomRight; /**< TODO: describe */
  QCPItemAnchor * const topLeftRim; /**< TODO: describe */
  QCPItemAnchor * const top; /**< TODO: describe */
  QCPItemAnchor * const topRightRim; /**< TODO: describe */
  QCPItemAnchor * const right; /**< TODO: describe */
  QCPItemAnchor * const bottomRightRim; /**< TODO: describe */
  QCPItemAnchor * const bottom; /**< TODO: describe */
  QCPItemAnchor * const bottomLeftRim; /**< TODO: describe */
  QCPItemAnchor * const left; /**< TODO: describe */
  QCPItemAnchor * const center; /**< TODO: describe */
  
protected:
  /**
   * @brief 
   *
   */
  enum AnchorIndex {aiTopLeftRim, aiTop, aiTopRightRim, aiRight, aiBottomRightRim, aiBottom, aiBottomLeftRim, aiLeft, aiCenter};
  
  // property members:
  QPen mPen, mSelectedPen; /**< TODO: describe */
  QBrush mBrush, mSelectedBrush; /**< TODO: describe */
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter);
  /**
   * @brief 
   *
   * @param anchorId
   * @return QPointF
   */
  virtual QPointF anchorPixelPoint(int anchorId) const;
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen mainPen() const;
  /**
   * @brief 
   *
   * @return QBrush
   */
  QBrush mainBrush() const;
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPItemPixmap : public QCPAbstractItem
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap)
  Q_PROPERTY(bool scaled READ scaled WRITE setScaled)
  Q_PROPERTY(Qt::AspectRatioMode aspectRatioMode READ aspectRatioMode)
  Q_PROPERTY(Qt::TransformationMode transformationMode READ transformationMode)
  Q_PROPERTY(QPen pen READ pen WRITE setPen)
  Q_PROPERTY(QPen selectedPen READ selectedPen WRITE setSelectedPen)
  /// \endcond
public:
  /**
   * @brief 
   *
   * @param parentPlot
   */
  QCPItemPixmap(QCustomPlot *parentPlot);
  /**
   * @brief 
   *
   */
  virtual ~QCPItemPixmap();
  
  // getters:
  /**
   * @brief 
   *
   * @return QPixmap
   */
  QPixmap pixmap() const { return mPixmap; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool scaled() const { return mScaled; }
  /**
   * @brief 
   *
   * @return Qt::AspectRatioMode
   */
  Qt::AspectRatioMode aspectRatioMode() const { return mAspectRatioMode; }
  /**
   * @brief 
   *
   * @return Qt::TransformationMode
   */
  Qt::TransformationMode transformationMode() const { return mTransformationMode; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen pen() const { return mPen; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen selectedPen() const { return mSelectedPen; }
  
  // setters;
  /**
   * @brief 
   *
   * @param pixmap
   */
  void setPixmap(const QPixmap &pixmap);
  /**
   * @brief 
   *
   * @param scaled
   * @param aspectRatioMode
   * @param transformationMode
   */
  void setScaled(bool scaled, Qt::AspectRatioMode aspectRatioMode=Qt::KeepAspectRatio, Qt::TransformationMode transformationMode=Qt::SmoothTransformation);
  /**
   * @brief 
   *
   * @param pen
   */
  void setPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param pen
   */
  void setSelectedPen(const QPen &pen);
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param pos
   * @param onlySelectable
   * @param details
   * @return double
   */
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
  QCPItemPosition * const topLeft; /**< TODO: describe */
  QCPItemPosition * const bottomRight; /**< TODO: describe */
  QCPItemAnchor * const top; /**< TODO: describe */
  QCPItemAnchor * const topRight; /**< TODO: describe */
  QCPItemAnchor * const right; /**< TODO: describe */
  QCPItemAnchor * const bottom; /**< TODO: describe */
  QCPItemAnchor * const bottomLeft; /**< TODO: describe */
  QCPItemAnchor * const left; /**< TODO: describe */
  
protected:
  /**
   * @brief 
   *
   */
  enum AnchorIndex {aiTop, aiTopRight, aiRight, aiBottom, aiBottomLeft, aiLeft};
  
  // property members:
  QPixmap mPixmap; /**< TODO: describe */
  QPixmap mScaledPixmap; /**< TODO: describe */
  bool mScaled; /**< TODO: describe */
  Qt::AspectRatioMode mAspectRatioMode; /**< TODO: describe */
  Qt::TransformationMode mTransformationMode; /**< TODO: describe */
  QPen mPen, mSelectedPen; /**< TODO: describe */
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter);
  /**
   * @brief 
   *
   * @param anchorId
   * @return QPointF
   */
  virtual QPointF anchorPixelPoint(int anchorId) const;
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @param finalRect
   * @param flipHorz
   * @param flipVert
   */
  void updateScaledPixmap(QRect finalRect=QRect(), bool flipHorz=false, bool flipVert=false);
  /**
   * @brief 
   *
   * @param flippedHorz
   * @param flippedVert
   * @return QRect
   */
  QRect getFinalRect(bool *flippedHorz=0, bool *flippedVert=0) const;
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen mainPen() const;
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPItemTracer : public QCPAbstractItem
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(QPen pen READ pen WRITE setPen)
  Q_PROPERTY(QPen selectedPen READ selectedPen WRITE setSelectedPen)
  Q_PROPERTY(QBrush brush READ brush WRITE setBrush)
  Q_PROPERTY(QBrush selectedBrush READ selectedBrush WRITE setSelectedBrush)
  Q_PROPERTY(double size READ size WRITE setSize)
  Q_PROPERTY(TracerStyle style READ style WRITE setStyle)
  Q_PROPERTY(QCPGraph* graph READ graph WRITE setGraph)
  Q_PROPERTY(double graphKey READ graphKey WRITE setGraphKey)
  Q_PROPERTY(bool interpolating READ interpolating WRITE setInterpolating)
  /// \endcond
public:
  /*!
    The different visual appearances a tracer item can have. Some styles size may be controlled with \ref setSize.
    
    \see setStyle
  */
  enum TracerStyle { tsNone        ///< The tracer is not visible
                     ,tsPlus       ///< A plus shaped crosshair with limited size
                     ,tsCrosshair  ///< A plus shaped crosshair which spans the complete axis rect
                     ,tsCircle     ///< A circle
                     ,tsSquare     ///< A square
                   };
  Q_ENUMS(TracerStyle)

  /**
   * @brief 
   *
   * @param parentPlot
   */
  QCPItemTracer(QCustomPlot *parentPlot);
  /**
   * @brief 
   *
   */
  virtual ~QCPItemTracer();

  // getters:
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen pen() const { return mPen; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen selectedPen() const { return mSelectedPen; }
  /**
   * @brief 
   *
   * @return QBrush
   */
  QBrush brush() const { return mBrush; }
  /**
   * @brief 
   *
   * @return QBrush
   */
  QBrush selectedBrush() const { return mSelectedBrush; }
  /**
   * @brief 
   *
   * @return double
   */
  double size() const { return mSize; }
  /**
   * @brief 
   *
   * @return TracerStyle
   */
  TracerStyle style() const { return mStyle; }
  /**
   * @brief 
   *
   * @return QCPGraph
   */
  QCPGraph *graph() const { return mGraph; }
  /**
   * @brief 
   *
   * @return double
   */
  double graphKey() const { return mGraphKey; }
  /**
   * @brief 
   *
   * @return bool
   */
  bool interpolating() const { return mInterpolating; }

  // setters;
  /**
   * @brief 
   *
   * @param pen
   */
  void setPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param pen
   */
  void setSelectedPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param brush
   */
  void setBrush(const QBrush &brush);
  /**
   * @brief 
   *
   * @param brush
   */
  void setSelectedBrush(const QBrush &brush);
  /**
   * @brief 
   *
   * @param size
   */
  void setSize(double size);
  /**
   * @brief 
   *
   * @param style
   */
  void setStyle(TracerStyle style);
  /**
   * @brief 
   *
   * @param graph
   */
  void setGraph(QCPGraph *graph);
  /**
   * @brief 
   *
   * @param key
   */
  void setGraphKey(double key);
  /**
   * @brief 
   *
   * @param enabled
   */
  void setInterpolating(bool enabled);

  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param pos
   * @param onlySelectable
   * @param details
   * @return double
   */
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
  // non-virtual methods:
  /**
   * @brief 
   *
   */
  void updatePosition();

  QCPItemPosition * const position; /**< TODO: describe */

protected:
  // property members:
  QPen mPen, mSelectedPen; /**< TODO: describe */
  QBrush mBrush, mSelectedBrush; /**< TODO: describe */
  double mSize; /**< TODO: describe */
  TracerStyle mStyle; /**< TODO: describe */
  QCPGraph *mGraph; /**< TODO: describe */
  double mGraphKey; /**< TODO: describe */
  bool mInterpolating; /**< TODO: describe */

  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter);

  // non-virtual methods:
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen mainPen() const;
  /**
   * @brief 
   *
   * @return QBrush
   */
  QBrush mainBrush() const;
};


/**
 * @brief 
 *
 */
class QCP_LIB_DECL QCPItemBracket : public QCPAbstractItem
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  Q_PROPERTY(QPen pen READ pen WRITE setPen)
  Q_PROPERTY(QPen selectedPen READ selectedPen WRITE setSelectedPen)
  Q_PROPERTY(double length READ length WRITE setLength)
  Q_PROPERTY(BracketStyle style READ style WRITE setStyle)
  /// \endcond
public:
  /**
   * @brief 
   *
   */
  enum BracketStyle { bsSquare  ///< A brace with angled edges
                      ,bsRound  ///< A brace with round edges
                      ,bsCurly  ///< A curly brace
                      ,bsCalligraphic ///< A curly brace with varying stroke width giving a calligraphic impression
  };

  /**
   * @brief 
   *
   * @param parentPlot
   */
  QCPItemBracket(QCustomPlot *parentPlot);
  /**
   * @brief 
   *
   */
  virtual ~QCPItemBracket();
  
  // getters:
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen pen() const { return mPen; }
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen selectedPen() const { return mSelectedPen; }
  /**
   * @brief 
   *
   * @return double
   */
  double length() const { return mLength; }
  /**
   * @brief 
   *
   * @return BracketStyle
   */
  BracketStyle style() const { return mStyle; }
  
  // setters;
  /**
   * @brief 
   *
   * @param pen
   */
  void setPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param pen
   */
  void setSelectedPen(const QPen &pen);
  /**
   * @brief 
   *
   * @param length
   */
  void setLength(double length);
  /**
   * @brief 
   *
   * @param style
   */
  void setStyle(BracketStyle style);
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param pos
   * @param onlySelectable
   * @param details
   * @return double
   */
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
  QCPItemPosition * const left; /**< TODO: describe */
  QCPItemPosition * const right; /**< TODO: describe */
  QCPItemAnchor * const center; /**< TODO: describe */
  
protected:
  // property members:
  /**
   * @brief 
   *
   */
  enum AnchorIndex {aiCenter};
  QPen mPen, mSelectedPen; /**< TODO: describe */
  double mLength; /**< TODO: describe */
  BracketStyle mStyle; /**< TODO: describe */
  
  // reimplemented virtual methods:
  /**
   * @brief 
   *
   * @param painter
   */
  virtual void draw(QCPPainter *painter);
  /**
   * @brief 
   *
   * @param anchorId
   * @return QPointF
   */
  virtual QPointF anchorPixelPoint(int anchorId) const;
  
  // non-virtual methods:
  /**
   * @brief 
   *
   * @return QPen
   */
  QPen mainPen() const;
};

#endif // QCUSTOMPLOT_H

