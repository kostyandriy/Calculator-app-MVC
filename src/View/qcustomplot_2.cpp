#include "qcustomplot.h"
/*! \overload

  Creates a new QCPTextElement instance and sets default values.

  The initial text is set to \a text with the specified \a font.
*/
QCPTextElement::QCPTextElement(QCustomPlot *parentPlot, const QString &text,
                               const QFont &font)
    : QCPLayoutElement(parentPlot),
      mText(text),
      mTextFlags(Qt::AlignCenter),
      mFont(font),
      mTextColor(Qt::black),
      mSelectedFont(font),
      mSelectedTextColor(Qt::blue),
      mSelectable(false),
      mSelected(false) {
  setMargins(QMargins(2, 2, 2, 2));
}

/*!
  Sets the text that will be displayed to \a text. Multiple lines can be created
  by insertion of "\n".

  \see setFont, setTextColor, setTextFlags
*/
void QCPTextElement::setText(const QString &text) { mText = text; }

/*!
  Sets options for text alignment and wrapping behaviour. \a flags is a bitwise
  OR-combination of \c Qt::AlignmentFlag and \c Qt::TextFlag enums.

  Possible enums are:
  - Qt::AlignLeft
  - Qt::AlignRight
  - Qt::AlignHCenter
  - Qt::AlignJustify
  - Qt::AlignTop
  - Qt::AlignBottom
  - Qt::AlignVCenter
  - Qt::AlignCenter
  - Qt::TextDontClip
  - Qt::TextSingleLine
  - Qt::TextExpandTabs
  - Qt::TextShowMnemonic
  - Qt::TextWordWrap
  - Qt::TextIncludeTrailingSpaces
*/
void QCPTextElement::setTextFlags(int flags) { mTextFlags = flags; }

/*!
  Sets the \a font of the text.

  \see setTextColor, setSelectedFont
*/
void QCPTextElement::setFont(const QFont &font) { mFont = font; }

/*!
  Sets the \a color of the text.

  \see setFont, setSelectedTextColor
*/
void QCPTextElement::setTextColor(const QColor &color) { mTextColor = color; }

/*!
  Sets the \a font of the text that will be used if the text element is selected
  (\ref setSelected).

  \see setFont
*/
void QCPTextElement::setSelectedFont(const QFont &font) {
  mSelectedFont = font;
}

/*!
  Sets the \a color of the text that will be used if the text element is
  selected (\ref setSelected).

  \see setTextColor
*/
void QCPTextElement::setSelectedTextColor(const QColor &color) {
  mSelectedTextColor = color;
}

/*!
  Sets whether the user may select this text element.

  Note that even when \a selectable is set to <tt>false</tt>, the selection
  state may be changed programmatically via \ref setSelected.
*/
void QCPTextElement::setSelectable(bool selectable) {
  if (mSelectable != selectable) {
    mSelectable = selectable;
    emit selectableChanged(mSelectable);
  }
}

/*!
  Sets the selection state of this text element to \a selected. If the selection
  has changed, \ref selectionChanged is emitted.

  Note that this function can change the selection state independently of the
  current \ref setSelectable state.
*/
void QCPTextElement::setSelected(bool selected) {
  if (mSelected != selected) {
    mSelected = selected;
    emit selectionChanged(mSelected);
  }
}

/* inherits documentation from base class */
void QCPTextElement::applyDefaultAntialiasingHint(QCPPainter *painter) const {
  applyAntialiasingHint(painter, mAntialiased, QCP::aeOther);
}

/* inherits documentation from base class */
void QCPTextElement::draw(QCPPainter *painter) {
  painter->setFont(mainFont());
  painter->setPen(QPen(mainTextColor()));
  painter->drawText(mRect, mTextFlags, mText, &mTextBoundingRect);
}

/* inherits documentation from base class */
QSize QCPTextElement::minimumOuterSizeHint() const {
  QFontMetrics metrics(mFont);
  QSize result(
      metrics.boundingRect(0, 0, 0, 0, Qt::TextDontClip, mText).size());
  result.rwidth() += mMargins.left() + mMargins.right();
  result.rheight() += mMargins.top() + mMargins.bottom();
  return result;
}

/* inherits documentation from base class */
QSize QCPTextElement::maximumOuterSizeHint() const {
  QFontMetrics metrics(mFont);
  QSize result(
      metrics.boundingRect(0, 0, 0, 0, Qt::TextDontClip, mText).size());
  result.setWidth(QWIDGETSIZE_MAX);
  result.rheight() += mMargins.top() + mMargins.bottom();
  return result;
}

/* inherits documentation from base class */
void QCPTextElement::selectEvent(QMouseEvent *event, bool additive,
                                 const QVariant &details,
                                 bool *selectionStateChanged) {
  Q_UNUSED(event)
  Q_UNUSED(details)
  if (mSelectable) {
    bool selBefore = mSelected;
    setSelected(additive ? !mSelected : true);
    if (selectionStateChanged) *selectionStateChanged = mSelected != selBefore;
  }
}

/* inherits documentation from base class */
void QCPTextElement::deselectEvent(bool *selectionStateChanged) {
  if (mSelectable) {
    bool selBefore = mSelected;
    setSelected(false);
    if (selectionStateChanged) *selectionStateChanged = mSelected != selBefore;
  }
}

/*!
  Returns 0.99*selectionTolerance (see \ref QCustomPlot::setSelectionTolerance)
  when \a pos is within the bounding box of the text element's text. Note that
  this bounding box is updated in the draw call.

  If \a pos is outside the text's bounding box or if \a onlySelectable is true
  and this text element is not selectable (\ref setSelectable), returns -1.

  \seebaseclassmethod
*/
double QCPTextElement::selectTest(const QPointF &pos, bool onlySelectable,
                                  QVariant *details) const {
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable) return -1;

  if (mTextBoundingRect.contains(pos.toPoint()))
    return mParentPlot->selectionTolerance() * 0.99;
  else
    return -1;
}

/*!
  Accepts the mouse event in order to emit the according click signal in the
  \ref mouseReleaseEvent.

  \seebaseclassmethod
*/
void QCPTextElement::mousePressEvent(QMouseEvent *event,
                                     const QVariant &details) {
  Q_UNUSED(details)
  event->accept();
}

/*!
  Emits the \ref clicked signal if the cursor hasn't moved by more than a few
  pixels since the \ref mousePressEvent.

  \seebaseclassmethod
*/
void QCPTextElement::mouseReleaseEvent(QMouseEvent *event,
                                       const QPointF &startPos) {
  if ((QPointF(event->pos()) - startPos).manhattanLength() <= 3)
    emit clicked(event);
}

/*!
  Emits the \ref doubleClicked signal.

  \seebaseclassmethod
*/
void QCPTextElement::mouseDoubleClickEvent(QMouseEvent *event,
                                           const QVariant &details) {
  Q_UNUSED(details)
  emit doubleClicked(event);
}

/*! \internal

  Returns the main font to be used. This is mSelectedFont if \ref setSelected is
  set to <tt>true</tt>, else mFont is returned.
*/
QFont QCPTextElement::mainFont() const {
  return mSelected ? mSelectedFont : mFont;
}

/*! \internal

  Returns the main color to be used. This is mSelectedTextColor if \ref
  setSelected is set to <tt>true</tt>, else mTextColor is returned.
*/
QColor QCPTextElement::mainTextColor() const {
  return mSelected ? mSelectedTextColor : mTextColor;
}
/* end of 'src/layoutelements/layoutelement-textelement.cpp' */

/* including file 'src/layoutelements/layoutelement-colorscale.cpp' */
/* modified 2022-11-06T12:45:56, size 26531                         */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPColorScale
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPColorScale
  \brief A color scale for use with color coding data such as QCPColorMap

  This layout element can be placed on the plot to correlate a color gradient
  with data values. It is usually used in combination with one or multiple \ref
  QCPColorMap "QCPColorMaps".

  \image html QCPColorScale.png

  The color scale can be either horizontal or vertical, as shown in the image
  above. The orientation and the side where the numbers appear is controlled
  with \ref setType.

  Use \ref QCPColorMap::setColorScale to connect a color map with a color scale.
  Once they are connected, they share their gradient, data range and data scale
  type (\ref setGradient, \ref setDataRange, \ref setDataScaleType). Multiple
  color maps may be associated with a single color scale, to make them all
  synchronize these properties.

  To have finer control over the number display and axis behaviour, you can
  directly access the \ref axis. See the documentation of QCPAxis for details
  about configuring axes. For example, if you want to change the number of
  automatically generated ticks, call \snippet
  documentation/doc-code-snippets/mainwindow.cpp qcpcolorscale-tickcount

  Placing a color scale next to the main axis rect works like with any other
  layout element: \snippet documentation/doc-code-snippets/mainwindow.cpp
  qcpcolorscale-creation In this case we have placed it to the right of the
  default axis rect, so it wasn't necessary to call \ref setType, since \ref
  QCPAxis::atRight is already the default. The text next to the color scale can
  be set with \ref setLabel.

  For optimum appearance (like in the image above), it may be desirable to line
  up the axis rect and the borders of the color scale. Use a \ref QCPMarginGroup
  to achieve this: \snippet documentation/doc-code-snippets/mainwindow.cpp
  qcpcolorscale-margingroup

  Color scales are initialized with a non-zero minimum top and bottom margin
  (\ref setMinimumMargins), because vertical color scales are most common and
  the minimum top/bottom margin makes sure it keeps some distance to the
  top/bottom widget border. So if you change to a horizontal color scale by
  setting \ref setType to \ref QCPAxis::atBottom or \ref QCPAxis::atTop, you
  might want to also change the minimum margins accordingly, e.g.
  <tt>setMinimumMargins(QMargins(6, 0, 6, 0))</tt>.
*/

/* start documentation of inline functions */

/*! \fn QCPAxis *QCPColorScale::axis() const

  Returns the internal \ref QCPAxis instance of this color scale. You can access
  it to alter the appearance and behaviour of the axis. \ref QCPColorScale
  duplicates some properties in its interface for convenience. Those are \ref
  setDataRange (\ref QCPAxis::setRange), \ref setDataScaleType (\ref
  QCPAxis::setScaleType), and the method \ref setLabel (\ref QCPAxis::setLabel).
  As they each are connected, it does not matter whether you use the method on
  the QCPColorScale or on its QCPAxis.

  If the type of the color scale is changed with \ref setType, the axis returned
  by this method will change, too, to either the left, right, bottom or top
  axis, depending on which type was set.
*/

/* end documentation of signals */
/* start documentation of signals */

/*! \fn void QCPColorScale::dataRangeChanged(const QCPRange &newRange);

  This signal is emitted when the data range changes.

  \see setDataRange
*/

/*! \fn void QCPColorScale::dataScaleTypeChanged(QCPAxis::ScaleType scaleType);

  This signal is emitted when the data scale type changes.

  \see setDataScaleType
*/

/*! \fn void QCPColorScale::gradientChanged(const QCPColorGradient
  &newGradient);

  This signal is emitted when the gradient changes.

  \see setGradient
*/

/* end documentation of signals */

/*!
  Constructs a new QCPColorScale.
*/
QCPColorScale::QCPColorScale(QCustomPlot *parentPlot)
    : QCPLayoutElement(parentPlot),
      mType(QCPAxis::atTop),  // set to atTop such that
      // setType(QCPAxis::atRight) below doesn't skip
      // work because it thinks it's already atRight
      mDataScaleType(QCPAxis::stLinear),
      mGradient(QCPColorGradient::gpCold),
      mBarWidth(20),
      mAxisRect(new QCPColorScaleAxisRectPrivate(this)) {
  setMinimumMargins(QMargins(
      0, 6, 0, 6));  // for default right color scale types, keep some room at
                     // bottom and top (important if no margin group is used)
  setType(QCPAxis::atRight);
  setDataRange(QCPRange(0, 6));
}

QCPColorScale::~QCPColorScale() { delete mAxisRect; }

/* undocumented getter */
QString QCPColorScale::label() const {
  if (!mColorAxis) {
    qDebug() << Q_FUNC_INFO << "internal color axis undefined";
    return QString();
  }

  return mColorAxis.data()->label();
}

/* undocumented getter */
bool QCPColorScale::rangeDrag() const {
  if (!mAxisRect) {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return false;
  }

  return mAxisRect.data()->rangeDrag().testFlag(QCPAxis::orientation(mType)) &&
         mAxisRect.data()->rangeDragAxis(QCPAxis::orientation(mType)) &&
         mAxisRect.data()
                 ->rangeDragAxis(QCPAxis::orientation(mType))
                 ->orientation() == QCPAxis::orientation(mType);
}

/* undocumented getter */
bool QCPColorScale::rangeZoom() const {
  if (!mAxisRect) {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return false;
  }

  return mAxisRect.data()->rangeZoom().testFlag(QCPAxis::orientation(mType)) &&
         mAxisRect.data()->rangeZoomAxis(QCPAxis::orientation(mType)) &&
         mAxisRect.data()
                 ->rangeZoomAxis(QCPAxis::orientation(mType))
                 ->orientation() == QCPAxis::orientation(mType);
}

/*!
  Sets at which side of the color scale the axis is placed, and thus also its
  orientation.

  Note that after setting \a type to a different value, the axis returned by
  \ref axis() will be a different one. The new axis will adopt the following
  properties from the previous axis: The range, scale type, label and ticker
  (the latter will be shared and not copied).
*/
void QCPColorScale::setType(QCPAxis::AxisType type) {
  if (!mAxisRect) {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return;
  }
  if (mType != type) {
    mType = type;
    QCPRange rangeTransfer(0, 6);
    QString labelTransfer;
    QSharedPointer<QCPAxisTicker> tickerTransfer;
    // transfer/revert some settings on old axis if it exists:
    bool doTransfer = !mColorAxis.isNull();
    if (doTransfer) {
      rangeTransfer = mColorAxis.data()->range();
      labelTransfer = mColorAxis.data()->label();
      tickerTransfer = mColorAxis.data()->ticker();
      mColorAxis.data()->setLabel(QString());
      disconnect(mColorAxis.data(), SIGNAL(rangeChanged(QCPRange)), this,
                 SLOT(setDataRange(QCPRange)));
      disconnect(mColorAxis.data(),
                 SIGNAL(scaleTypeChanged(QCPAxis::ScaleType)), this,
                 SLOT(setDataScaleType(QCPAxis::ScaleType)));
    }
    const QList<QCPAxis::AxisType> allAxisTypes =
        QList<QCPAxis::AxisType>() << QCPAxis::atLeft << QCPAxis::atRight
                                   << QCPAxis::atBottom << QCPAxis::atTop;
    foreach (QCPAxis::AxisType atype, allAxisTypes) {
      mAxisRect.data()->axis(atype)->setTicks(atype == mType);
      mAxisRect.data()->axis(atype)->setTickLabels(atype == mType);
    }
    // set new mColorAxis pointer:
    mColorAxis = mAxisRect.data()->axis(mType);
    // transfer settings to new axis:
    if (doTransfer) {
      mColorAxis.data()->setRange(
          rangeTransfer);  // range transfer necessary if axis changes from
                           // vertical to horizontal or vice versa (axes with
                           // same orientation are synchronized via signals)
      mColorAxis.data()->setLabel(labelTransfer);
      mColorAxis.data()->setTicker(tickerTransfer);
    }
    connect(mColorAxis.data(), SIGNAL(rangeChanged(QCPRange)), this,
            SLOT(setDataRange(QCPRange)));
    connect(mColorAxis.data(), SIGNAL(scaleTypeChanged(QCPAxis::ScaleType)),
            this, SLOT(setDataScaleType(QCPAxis::ScaleType)));
    mAxisRect.data()->setRangeDragAxes(QList<QCPAxis *>() << mColorAxis.data());
  }
}

/*!
  Sets the range spanned by the color gradient and that is shown by the axis in
  the color scale.

  It is equivalent to calling QCPColorMap::setDataRange on any of the connected
  color maps. It is also equivalent to directly accessing the \ref axis and
  setting its range with \ref QCPAxis::setRange.

  \see setDataScaleType, setGradient, rescaleDataRange
*/
void QCPColorScale::setDataRange(const QCPRange &dataRange) {
  if (mDataRange.lower != dataRange.lower ||
      mDataRange.upper != dataRange.upper) {
    mDataRange = dataRange;
    if (mColorAxis) mColorAxis.data()->setRange(mDataRange);
    emit dataRangeChanged(mDataRange);
  }
}

/*!
  Sets the scale type of the color scale, i.e. whether values are associated
  with colors linearly or logarithmically.

  It is equivalent to calling QCPColorMap::setDataScaleType on any of the
  connected color maps. It is also equivalent to directly accessing the \ref
  axis and setting its scale type with \ref QCPAxis::setScaleType.

  Note that this method controls the coordinate transformation. For logarithmic
  scales, you will likely also want to use a logarithmic tick spacing and
  labeling, which can be achieved by setting the color scale's \ref axis ticker
  to an instance of \ref QCPAxisTickerLog :

  \snippet documentation/doc-code-snippets/mainwindow.cpp
  qcpaxisticker-log-colorscale

  See the documentation of \ref QCPAxisTickerLog about the details of
  logarithmic axis tick creation.

  \see setDataRange, setGradient
*/
void QCPColorScale::setDataScaleType(QCPAxis::ScaleType scaleType) {
  if (mDataScaleType != scaleType) {
    mDataScaleType = scaleType;
    if (mColorAxis) mColorAxis.data()->setScaleType(mDataScaleType);
    if (mDataScaleType == QCPAxis::stLogarithmic)
      setDataRange(mDataRange.sanitizedForLogScale());
    emit dataScaleTypeChanged(mDataScaleType);
  }
}

/*!
  Sets the color gradient that will be used to represent data values.

  It is equivalent to calling QCPColorMap::setGradient on any of the connected
  color maps.

  \see setDataRange, setDataScaleType
*/
void QCPColorScale::setGradient(const QCPColorGradient &gradient) {
  if (mGradient != gradient) {
    mGradient = gradient;
    if (mAxisRect) mAxisRect.data()->mGradientImageInvalidated = true;
    emit gradientChanged(mGradient);
  }
}

/*!
  Sets the axis label of the color scale. This is equivalent to calling \ref
  QCPAxis::setLabel on the internal \ref axis.
*/
void QCPColorScale::setLabel(const QString &str) {
  if (!mColorAxis) {
    qDebug() << Q_FUNC_INFO << "internal color axis undefined";
    return;
  }

  mColorAxis.data()->setLabel(str);
}

/*!
  Sets the width (or height, for horizontal color scales) the bar where the
  gradient is displayed will have.
*/
void QCPColorScale::setBarWidth(int width) { mBarWidth = width; }

/*!
  Sets whether the user can drag the data range (\ref setDataRange).

  Note that \ref QCP::iRangeDrag must be in the QCustomPlot's interactions (\ref
  QCustomPlot::setInteractions) to allow range dragging.
*/
void QCPColorScale::setRangeDrag(bool enabled) {
  if (!mAxisRect) {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return;
  }

  if (enabled) {
    mAxisRect.data()->setRangeDrag(QCPAxis::orientation(mType));
  } else {
#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
    mAxisRect.data()->setRangeDrag(nullptr);
#else
    mAxisRect.data()->setRangeDrag({});
#endif
  }
}

/*!
  Sets whether the user can zoom the data range (\ref setDataRange) by scrolling
  the mouse wheel.

  Note that \ref QCP::iRangeZoom must be in the QCustomPlot's interactions (\ref
  QCustomPlot::setInteractions) to allow range dragging.
*/
void QCPColorScale::setRangeZoom(bool enabled) {
  if (!mAxisRect) {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return;
  }

  if (enabled) {
    mAxisRect.data()->setRangeZoom(QCPAxis::orientation(mType));
  } else {
#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
    mAxisRect.data()->setRangeDrag(nullptr);
#else
    mAxisRect.data()->setRangeZoom({});
#endif
  }
}

/*!
  Returns a list of all the color maps associated with this color scale.
*/
QList<QCPColorMap *> QCPColorScale::colorMaps() const {
  QList<QCPColorMap *> result;
  for (int i = 0; i < mParentPlot->plottableCount(); ++i) {
    if (QCPColorMap *cm =
            qobject_cast<QCPColorMap *>(mParentPlot->plottable(i)))
      if (cm->colorScale() == this) result.append(cm);
  }
  return result;
}

/*!
  Changes the data range such that all color maps associated with this color
  scale are fully mapped to the gradient in the data dimension.

  \see setDataRange
*/
void QCPColorScale::rescaleDataRange(bool onlyVisibleMaps) {
  QList<QCPColorMap *> maps = colorMaps();
  QCPRange newRange;
  bool haveRange = false;
  QCP::SignDomain sign = QCP::sdBoth;
  if (mDataScaleType == QCPAxis::stLogarithmic)
    sign = (mDataRange.upper < 0 ? QCP::sdNegative : QCP::sdPositive);
  foreach (QCPColorMap *map, maps) {
    if (!map->realVisibility() && onlyVisibleMaps) continue;
    QCPRange mapRange;
    if (map->colorScale() == this) {
      bool currentFoundRange = true;
      mapRange = map->data()->dataBounds();
      if (sign == QCP::sdPositive) {
        if (mapRange.lower <= 0 && mapRange.upper > 0)
          mapRange.lower = mapRange.upper * 1e-3;
        else if (mapRange.lower <= 0 && mapRange.upper <= 0)
          currentFoundRange = false;
      } else if (sign == QCP::sdNegative) {
        if (mapRange.upper >= 0 && mapRange.lower < 0)
          mapRange.upper = mapRange.lower * 1e-3;
        else if (mapRange.upper >= 0 && mapRange.lower >= 0)
          currentFoundRange = false;
      }
      if (currentFoundRange) {
        if (!haveRange)
          newRange = mapRange;
        else
          newRange.expand(mapRange);
        haveRange = true;
      }
    }
  }
  if (haveRange) {
    if (!QCPRange::validRange(
            newRange))  // likely due to range being zero (plottable has only
    // constant data in this dimension), shift current range
    // to at least center the data
    {
      double center =
          (newRange.lower + newRange.upper) *
          0.5;  // upper and lower should be equal anyway, but just to make
                // sure, incase validRange returned false for other reason
      if (mDataScaleType == QCPAxis::stLinear) {
        newRange.lower = center - mDataRange.size() / 2.0;
        newRange.upper = center + mDataRange.size() / 2.0;
      } else  // mScaleType == stLogarithmic
      {
        newRange.lower = center / qSqrt(mDataRange.upper / mDataRange.lower);
        newRange.upper = center * qSqrt(mDataRange.upper / mDataRange.lower);
      }
    }
    setDataRange(newRange);
  }
}

/* inherits documentation from base class */
void QCPColorScale::update(UpdatePhase phase) {
  QCPLayoutElement::update(phase);
  if (!mAxisRect) {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return;
  }

  mAxisRect.data()->update(phase);

  switch (phase) {
    case upMargins: {
      if (mType == QCPAxis::atBottom || mType == QCPAxis::atTop) {
        setMaximumSize(QWIDGETSIZE_MAX,
                       mBarWidth + mAxisRect.data()->margins().top() +
                           mAxisRect.data()->margins().bottom());
        setMinimumSize(0, mBarWidth + mAxisRect.data()->margins().top() +
                              mAxisRect.data()->margins().bottom());
      } else {
        setMaximumSize(mBarWidth + mAxisRect.data()->margins().left() +
                           mAxisRect.data()->margins().right(),
                       QWIDGETSIZE_MAX);
        setMinimumSize(mBarWidth + mAxisRect.data()->margins().left() +
                           mAxisRect.data()->margins().right(),
                       0);
      }
      break;
    }
    case upLayout: {
      mAxisRect.data()->setOuterRect(rect());
      break;
    }
    default:
      break;
  }
}

/* inherits documentation from base class */
void QCPColorScale::applyDefaultAntialiasingHint(QCPPainter *painter) const {
  painter->setAntialiasing(false);
}

/* inherits documentation from base class */
void QCPColorScale::mousePressEvent(QMouseEvent *event,
                                    const QVariant &details) {
  if (!mAxisRect) {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return;
  }
  mAxisRect.data()->mousePressEvent(event, details);
}

/* inherits documentation from base class */
void QCPColorScale::mouseMoveEvent(QMouseEvent *event,
                                   const QPointF &startPos) {
  if (!mAxisRect) {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return;
  }
  mAxisRect.data()->mouseMoveEvent(event, startPos);
}

/* inherits documentation from base class */
void QCPColorScale::mouseReleaseEvent(QMouseEvent *event,
                                      const QPointF &startPos) {
  if (!mAxisRect) {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return;
  }
  mAxisRect.data()->mouseReleaseEvent(event, startPos);
}

/* inherits documentation from base class */
void QCPColorScale::wheelEvent(QWheelEvent *event) {
  if (!mAxisRect) {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return;
  }
  mAxisRect.data()->wheelEvent(event);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPColorScaleAxisRectPrivate
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPColorScaleAxisRectPrivate

  \internal
  \brief An axis rect subclass for use in a QCPColorScale

  This is a private class and not part of the public QCustomPlot interface.

  It provides the axis rect functionality for the QCPColorScale class.
*/

/*!
  Creates a new instance, as a child of \a parentColorScale.
*/
QCPColorScaleAxisRectPrivate::QCPColorScaleAxisRectPrivate(
    QCPColorScale *parentColorScale)
    : QCPAxisRect(parentColorScale->parentPlot(), true),
      mParentColorScale(parentColorScale),
      mGradientImageInvalidated(true) {
  setParentLayerable(parentColorScale);
  setMinimumMargins(QMargins(0, 0, 0, 0));
  const QList<QCPAxis::AxisType> allAxisTypes =
      QList<QCPAxis::AxisType>() << QCPAxis::atBottom << QCPAxis::atTop
                                 << QCPAxis::atLeft << QCPAxis::atRight;
  foreach (QCPAxis::AxisType type, allAxisTypes) {
    axis(type)->setVisible(true);
    axis(type)->grid()->setVisible(false);
    axis(type)->setPadding(0);
    connect(axis(type), SIGNAL(selectionChanged(QCPAxis::SelectableParts)),
            this, SLOT(axisSelectionChanged(QCPAxis::SelectableParts)));
    connect(axis(type), SIGNAL(selectableChanged(QCPAxis::SelectableParts)),
            this, SLOT(axisSelectableChanged(QCPAxis::SelectableParts)));
  }

  connect(axis(QCPAxis::atLeft), SIGNAL(rangeChanged(QCPRange)),
          axis(QCPAxis::atRight), SLOT(setRange(QCPRange)));
  connect(axis(QCPAxis::atRight), SIGNAL(rangeChanged(QCPRange)),
          axis(QCPAxis::atLeft), SLOT(setRange(QCPRange)));
  connect(axis(QCPAxis::atBottom), SIGNAL(rangeChanged(QCPRange)),
          axis(QCPAxis::atTop), SLOT(setRange(QCPRange)));
  connect(axis(QCPAxis::atTop), SIGNAL(rangeChanged(QCPRange)),
          axis(QCPAxis::atBottom), SLOT(setRange(QCPRange)));
  connect(axis(QCPAxis::atLeft), SIGNAL(scaleTypeChanged(QCPAxis::ScaleType)),
          axis(QCPAxis::atRight), SLOT(setScaleType(QCPAxis::ScaleType)));
  connect(axis(QCPAxis::atRight), SIGNAL(scaleTypeChanged(QCPAxis::ScaleType)),
          axis(QCPAxis::atLeft), SLOT(setScaleType(QCPAxis::ScaleType)));
  connect(axis(QCPAxis::atBottom), SIGNAL(scaleTypeChanged(QCPAxis::ScaleType)),
          axis(QCPAxis::atTop), SLOT(setScaleType(QCPAxis::ScaleType)));
  connect(axis(QCPAxis::atTop), SIGNAL(scaleTypeChanged(QCPAxis::ScaleType)),
          axis(QCPAxis::atBottom), SLOT(setScaleType(QCPAxis::ScaleType)));

  // make layer transfers of color scale transfer to axis rect and axes
  // the axes must be set after axis rect, such that they appear above color
  // gradient drawn by axis rect:
  connect(parentColorScale, SIGNAL(layerChanged(QCPLayer *)), this,
          SLOT(setLayer(QCPLayer *)));
  foreach (QCPAxis::AxisType type, allAxisTypes)
    connect(parentColorScale, SIGNAL(layerChanged(QCPLayer *)), axis(type),
            SLOT(setLayer(QCPLayer *)));
}

/*! \internal

  Updates the color gradient image if necessary, by calling \ref
  updateGradientImage, then draws it. Then the axes are drawn by calling the
  \ref QCPAxisRect::draw base class implementation.

  \seebaseclassmethod
*/
void QCPColorScaleAxisRectPrivate::draw(QCPPainter *painter) {
  if (mGradientImageInvalidated) updateGradientImage();

  bool mirrorHorz = false;
  bool mirrorVert = false;
  if (mParentColorScale->mColorAxis) {
    mirrorHorz = mParentColorScale->mColorAxis.data()->rangeReversed() &&
                 (mParentColorScale->type() == QCPAxis::atBottom ||
                  mParentColorScale->type() == QCPAxis::atTop);
    mirrorVert = mParentColorScale->mColorAxis.data()->rangeReversed() &&
                 (mParentColorScale->type() == QCPAxis::atLeft ||
                  mParentColorScale->type() == QCPAxis::atRight);
  }

  painter->drawImage(rect().adjusted(0, -1, 0, -1),
                     mGradientImage.mirrored(mirrorHorz, mirrorVert));
  QCPAxisRect::draw(painter);
}

/*! \internal

  Uses the current gradient of the parent \ref QCPColorScale (specified in the
  constructor) to generate a gradient image. This gradient image will be used in
  the \ref draw method.
*/
void QCPColorScaleAxisRectPrivate::updateGradientImage() {
  if (rect().isEmpty()) return;

  const QImage::Format format = QImage::Format_ARGB32_Premultiplied;
  int n = mParentColorScale->mGradient.levelCount();
  int w, h;
  QVector<double> data(n);
  for (int i = 0; i < n; ++i) data[i] = i;
  if (mParentColorScale->mType == QCPAxis::atBottom ||
      mParentColorScale->mType == QCPAxis::atTop) {
    w = n;
    h = rect().height();
    mGradientImage = QImage(w, h, format);
    QVector<QRgb *> pixels;
    for (int y = 0; y < h; ++y)
      pixels.append(reinterpret_cast<QRgb *>(mGradientImage.scanLine(y)));
    mParentColorScale->mGradient.colorize(data.constData(), QCPRange(0, n - 1),
                                          pixels.first(), n);
    for (int y = 1; y < h; ++y)
      memcpy(pixels.at(y), pixels.first(), size_t(n) * sizeof(QRgb));
  } else {
    w = rect().width();
    h = n;
    mGradientImage = QImage(w, h, format);
    for (int y = 0; y < h; ++y) {
      QRgb *pixels = reinterpret_cast<QRgb *>(mGradientImage.scanLine(y));
      const QRgb lineColor = mParentColorScale->mGradient.color(
          data[h - 1 - y], QCPRange(0, n - 1));
      for (int x = 0; x < w; ++x) pixels[x] = lineColor;
    }
  }
  mGradientImageInvalidated = false;
}

/*! \internal

  This slot is connected to the selectionChanged signals of the four axes in the
  constructor. It synchronizes the selection state of the axes.
*/
void QCPColorScaleAxisRectPrivate::axisSelectionChanged(
    QCPAxis::SelectableParts selectedParts) {
  // axis bases of four axes shall always (de-)selected synchronously:
  const QList<QCPAxis::AxisType> allAxisTypes =
      QList<QCPAxis::AxisType>() << QCPAxis::atBottom << QCPAxis::atTop
                                 << QCPAxis::atLeft << QCPAxis::atRight;
  foreach (QCPAxis::AxisType type, allAxisTypes) {
    if (QCPAxis *senderAxis = qobject_cast<QCPAxis *>(sender()))
      if (senderAxis->axisType() == type) continue;

    if (axis(type)->selectableParts().testFlag(QCPAxis::spAxis)) {
      if (selectedParts.testFlag(QCPAxis::spAxis))
        axis(type)->setSelectedParts(axis(type)->selectedParts() |
                                     QCPAxis::spAxis);
      else
        axis(type)->setSelectedParts(axis(type)->selectedParts() &
                                     ~QCPAxis::spAxis);
    }
  }
}

/*! \internal

  This slot is connected to the selectableChanged signals of the four axes in
  the constructor. It synchronizes the selectability of the axes.
*/
void QCPColorScaleAxisRectPrivate::axisSelectableChanged(
    QCPAxis::SelectableParts selectableParts) {
  // synchronize axis base selectability:
  const QList<QCPAxis::AxisType> allAxisTypes =
      QList<QCPAxis::AxisType>() << QCPAxis::atBottom << QCPAxis::atTop
                                 << QCPAxis::atLeft << QCPAxis::atRight;
  foreach (QCPAxis::AxisType type, allAxisTypes) {
    if (QCPAxis *senderAxis = qobject_cast<QCPAxis *>(sender()))
      if (senderAxis->axisType() == type) continue;

    if (axis(type)->selectableParts().testFlag(QCPAxis::spAxis)) {
      if (selectableParts.testFlag(QCPAxis::spAxis))
        axis(type)->setSelectableParts(axis(type)->selectableParts() |
                                       QCPAxis::spAxis);
      else
        axis(type)->setSelectableParts(axis(type)->selectableParts() &
                                       ~QCPAxis::spAxis);
    }
  }
}
/* end of 'src/layoutelements/layoutelement-colorscale.cpp' */

/* including file 'src/plottables/plottable-graph.cpp' */
/* modified 2022-11-06T12:45:57, size 74926            */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPGraphData
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPGraphData
  \brief Holds the data of one single data point for QCPGraph.

  The stored data is:
  \li \a key: coordinate on the key axis of this data point (this is the \a
  mainKey and the \a sortKey) \li \a value: coordinate on the value axis of this
  data point (this is the \a mainValue)

  The container for storing multiple data points is \ref QCPGraphDataContainer.
  It is a typedef for \ref QCPDataContainer with \ref QCPGraphData as the
  DataType template parameter. See the documentation there for an explanation
  regarding the data type's generic methods.

  \see QCPGraphDataContainer
*/

/* start documentation of inline functions */

/*! \fn double QCPGraphData::sortKey() const

  Returns the \a key member of this data point.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/*! \fn static QCPGraphData QCPGraphData::fromSortKey(double sortKey)

  Returns a data point with the specified \a sortKey. All other members are set
  to zero.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/*! \fn static static bool QCPGraphData::sortKeyIsMainKey()

  Since the member \a key is both the data point key coordinate and the data
  ordering parameter, this method returns true.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/*! \fn double QCPGraphData::mainKey() const

  Returns the \a key member of this data point.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/*! \fn double QCPGraphData::mainValue() const

  Returns the \a value member of this data point.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/*! \fn QCPRange QCPGraphData::valueRange() const

  Returns a QCPRange with both lower and upper boundary set to \a value of this
  data point.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/* end documentation of inline functions */

/*!
  Constructs a data point with key and value set to zero.
*/
QCPGraphData::QCPGraphData() : key(0), value(0) {}

/*!
  Constructs a data point with the specified \a key and \a value.
*/
QCPGraphData::QCPGraphData(double key, double value) : key(key), value(value) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPGraph
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPGraph
  \brief A plottable representing a graph in a plot.

  \image html QCPGraph.png

  Usually you create new graphs by calling QCustomPlot::addGraph. The resulting
  instance can be accessed via QCustomPlot::graph.

  To plot data, assign it with the \ref setData or \ref addData functions.
  Alternatively, you can also access and modify the data via the \ref data
  method, which returns a pointer to the internal \ref QCPGraphDataContainer.

  Graphs are used to display single-valued data. Single-valued means that there
  should only be one data point per unique key coordinate. In other words, the
  graph can't have \a loops. If you do want to plot non-single-valued curves,
  rather use the QCPCurve plottable.

  Gaps in the graph line can be created by adding data points with NaN as value
  (<tt>qQNaN()</tt> or <tt>std::numeric_limits<double>::quiet_NaN()</tt>) in
  between the two data points that shall be separated.

  \section qcpgraph-appearance Changing the appearance

  The appearance of the graph is mainly determined by the line style, scatter
  style, brush and pen of the graph (\ref setLineStyle, \ref setScatterStyle,
  \ref setBrush, \ref setPen).

  \subsection filling Filling under or between graphs

  QCPGraph knows two types of fills: Normal graph fills towards the
  zero-value-line parallel to the key axis of the graph, and fills between two
  graphs, called channel fills. To enable a fill, just set a brush with \ref
  setBrush which is neither Qt::NoBrush nor fully transparent.

  By default, a normal fill towards the zero-value-line will be drawn. To set up
  a channel fill between this graph and another one, call \ref
  setChannelFillGraph with the other graph as parameter.

  \see QCustomPlot::addGraph, QCustomPlot::graph
*/

/* start of documentation of inline functions */

/*! \fn QSharedPointer<QCPGraphDataContainer> QCPGraph::data() const

  Returns a shared pointer to the internal data storage of type \ref
  QCPGraphDataContainer. You may use it to directly manipulate the data, which
  may be more convenient and faster than using the regular \ref setData or \ref
  addData methods.
*/

/* end of documentation of inline functions */

/*!
  Constructs a graph which uses \a keyAxis as its key axis ("x") and \a
  valueAxis as its value axis ("y"). \a keyAxis and \a valueAxis must reside in
  the same QCustomPlot instance and not have the same orientation. If either of
  these restrictions is violated, a corresponding message is printed to the
  debug output (qDebug), the construction is not aborted, though.

  The created QCPGraph is automatically registered with the QCustomPlot instance
  inferred from \a keyAxis. This QCustomPlot instance takes ownership of the
  QCPGraph, so do not delete it manually but use QCustomPlot::removePlottable()
  instead.

  To directly create a graph inside a plot, you can also use the simpler
  QCustomPlot::addGraph function.
*/
QCPGraph::QCPGraph(QCPAxis *keyAxis, QCPAxis *valueAxis)
    : QCPAbstractPlottable1D<QCPGraphData>(keyAxis, valueAxis),
      mLineStyle{},
      mScatterSkip{},
      mAdaptiveSampling{} {
  // special handling for QCPGraphs to maintain the simple graph interface:
  mParentPlot->registerGraph(this);

  setPen(QPen(Qt::blue, 0));
  setBrush(Qt::NoBrush);

  setLineStyle(lsLine);
  setScatterSkip(0);
  setChannelFillGraph(nullptr);
  setAdaptiveSampling(true);
}

QCPGraph::~QCPGraph() {}

/*! \overload

  Replaces the current data container with the provided \a data container.

  Since a QSharedPointer is used, multiple QCPGraphs may share the same data
  container safely. Modifying the data in the container will then affect all
  graphs that share the container. Sharing can be achieved by simply exchanging
  the data containers wrapped in shared pointers: \snippet
  documentation/doc-code-snippets/mainwindow.cpp qcpgraph-datasharing-1

  If you do not wish to share containers, but create a copy from an existing
  container, rather use the \ref QCPDataContainer<DataType>::set method on the
  graph's data container directly: \snippet
  documentation/doc-code-snippets/mainwindow.cpp qcpgraph-datasharing-2

  \see addData
*/
void QCPGraph::setData(QSharedPointer<QCPGraphDataContainer> data) {
  mDataContainer = data;
}

/*! \overload

  Replaces the current data with the provided points in \a keys and \a values.
  The provided vectors should have equal length. Else, the number of added
  points will be the size of the smallest vector.

  If you can guarantee that the passed data points are sorted by \a keys in
  ascending order, you can set \a alreadySorted to true, to improve performance
  by saving a sorting run.

  \see addData
*/
void QCPGraph::setData(const QVector<double> &keys,
                       const QVector<double> &values, bool alreadySorted) {
  mDataContainer->clear();
  addData(keys, values, alreadySorted);
}

/*!
  Sets how the single data points are connected in the plot. For scatter-only
  plots, set \a ls to \ref lsNone and \ref setScatterStyle to the desired
  scatter style.

  \see setScatterStyle
*/
void QCPGraph::setLineStyle(LineStyle ls) { mLineStyle = ls; }

/*!
  Sets the visual appearance of single data points in the plot. If set to \ref
  QCPScatterStyle::ssNone, no scatter points are drawn (e.g. for line-only-plots
  with appropriate line style).

  \see QCPScatterStyle, setLineStyle
*/
void QCPGraph::setScatterStyle(const QCPScatterStyle &style) {
  mScatterStyle = style;
}

/*!
  If scatters are displayed (scatter style not \ref QCPScatterStyle::ssNone), \a
  skip number of scatter points are skipped/not drawn after every drawn scatter
  point.

  This can be used to make the data appear sparser while for example still
  having a smooth line, and to improve performance for very high density plots.

  If \a skip is set to 0 (default), all scatter points are drawn.

  \see setScatterStyle
*/
void QCPGraph::setScatterSkip(int skip) { mScatterSkip = qMax(0, skip); }

/*!
  Sets the target graph for filling the area between this graph and \a
  targetGraph with the current brush (\ref setBrush).

  When \a targetGraph is set to 0, a normal graph fill to the zero-value-line
  will be shown. To disable any filling, set the brush to Qt::NoBrush.

  \see setBrush
*/
void QCPGraph::setChannelFillGraph(QCPGraph *targetGraph) {
  // prevent setting channel target to this graph itself:
  if (targetGraph == this) {
    qDebug() << Q_FUNC_INFO << "targetGraph is this graph itself";
    mChannelFillGraph = nullptr;
    return;
  }
  // prevent setting channel target to a graph not in the plot:
  if (targetGraph && targetGraph->mParentPlot != mParentPlot) {
    qDebug() << Q_FUNC_INFO << "targetGraph not in same plot";
    mChannelFillGraph = nullptr;
    return;
  }

  mChannelFillGraph = targetGraph;
}

/*!
  Sets whether adaptive sampling shall be used when plotting this graph.
  QCustomPlot's adaptive sampling technique can drastically improve the replot
  performance for graphs with a larger number of points (e.g. above 10,000),
  without notably changing the appearance of the graph.

  By default, adaptive sampling is enabled. Even if enabled, QCustomPlot decides
  whether adaptive sampling shall actually be used on a per-graph basis. So
  leaving adaptive sampling enabled has no disadvantage in almost all cases.

  \image html adaptive-sampling-line.png "A line plot of 500,000 points without
  and with adaptive sampling"

  As can be seen, line plots experience no visual degradation from adaptive
  sampling. Outliers are reproduced reliably, as well as the overall shape of
  the data set. The replot time reduces dramatically though. This allows
  QCustomPlot to display large amounts of data in realtime.

  \image html adaptive-sampling-scatter.png "A scatter plot of 100,000 points
  without and with adaptive sampling"

  Care must be taken when using high-density scatter plots in combination with
  adaptive sampling. The adaptive sampling algorithm treats scatter plots more
  carefully than line plots which still gives a significant reduction of replot
  times, but not quite as much as for line plots. This is because scatter plots
  inherently need more data points to be preserved in order to still resemble
  the original, non-adaptive-sampling plot. As shown above, the results still
  aren't quite identical, as banding occurs for the outer data points. This is
  in fact intentional, such that the boundaries of the data cloud stay visible
  to the viewer. How strong the banding appears, depends on the point density,
  i.e. the number of points in the plot.

  For some situations with scatter plots it might thus be desirable to manually
  turn adaptive sampling off. For example, when saving the plot to disk. This
  can be achieved by setting \a enabled to false before issuing a command like
  \ref QCustomPlot::savePng, and setting \a enabled back to true afterwards.
*/
void QCPGraph::setAdaptiveSampling(bool enabled) {
  mAdaptiveSampling = enabled;
}

/*! \overload

  Adds the provided points in \a keys and \a values to the current data. The
  provided vectors should have equal length. Else, the number of added points
  will be the size of the smallest vector.

  If you can guarantee that the passed data points are sorted by \a keys in
  ascending order, you can set \a alreadySorted to true, to improve performance
  by saving a sorting run.

  Alternatively, you can also access and modify the data directly via the \ref
  data method, which returns a pointer to the internal data container.
*/
void QCPGraph::addData(const QVector<double> &keys,
                       const QVector<double> &values, bool alreadySorted) {
  if (keys.size() != values.size())
    qDebug() << Q_FUNC_INFO
             << "keys and values have different sizes:" << keys.size()
             << values.size();
  const int n = qMin(keys.size(), values.size());
  QVector<QCPGraphData> tempData(n);
  QVector<QCPGraphData>::iterator it = tempData.begin();
  const QVector<QCPGraphData>::iterator itEnd = tempData.end();
  int i = 0;
  while (it != itEnd) {
    it->key = keys[i];
    it->value = values[i];
    ++it;
    ++i;
  }
  mDataContainer->add(tempData,
                      alreadySorted);  // don't modify tempData beyond this to
                                       // prevent copy on write
}

/*! \overload

  Adds the provided data point as \a key and \a value to the current data.

  Alternatively, you can also access and modify the data directly via the \ref
  data method, which returns a pointer to the internal data container.
*/
void QCPGraph::addData(double key, double value) {
  mDataContainer->add(QCPGraphData(key, value));
}

/*!
  Implements a selectTest specific to this plottable's point geometry.

  If \a details is not 0, it will be set to a \ref QCPDataSelection, describing
  the closest data point to \a pos.

  \seebaseclassmethod \ref QCPAbstractPlottable::selectTest
*/
double QCPGraph::selectTest(const QPointF &pos, bool onlySelectable,
                            QVariant *details) const {
  if ((onlySelectable && mSelectable == QCP::stNone) ||
      mDataContainer->isEmpty())
    return -1;
  if (!mKeyAxis || !mValueAxis) return -1;

  if (mKeyAxis.data()->axisRect()->rect().contains(pos.toPoint()) ||
      mParentPlot->interactions().testFlag(
          QCP::iSelectPlottablesBeyondAxisRect)) {
    QCPGraphDataContainer::const_iterator closestDataPoint =
        mDataContainer->constEnd();
    double result = pointDistance(pos, closestDataPoint);
    if (details) {
      int pointIndex = int(closestDataPoint - mDataContainer->constBegin());
      details->setValue(
          QCPDataSelection(QCPDataRange(pointIndex, pointIndex + 1)));
    }
    return result;
  } else
    return -1;
}

/* inherits documentation from base class */
QCPRange QCPGraph::getKeyRange(bool &foundRange,
                               QCP::SignDomain inSignDomain) const {
  return mDataContainer->keyRange(foundRange, inSignDomain);
}

/* inherits documentation from base class */
QCPRange QCPGraph::getValueRange(bool &foundRange, QCP::SignDomain inSignDomain,
                                 const QCPRange &inKeyRange) const {
  return mDataContainer->valueRange(foundRange, inSignDomain, inKeyRange);
}

/* inherits documentation from base class */
void QCPGraph::draw(QCPPainter *painter) {
  if (!mKeyAxis || !mValueAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    return;
  }
  if (mKeyAxis.data()->range().size() <= 0 || mDataContainer->isEmpty()) return;
  if (mLineStyle == lsNone && mScatterStyle.isNone()) return;

  QVector<QPointF> lines,
      scatters;  // line and (if necessary) scatter pixel coordinates will be
                 // stored here while iterating over segments

  // loop over and draw segments of unselected/selected data:
  QList<QCPDataRange> selectedSegments, unselectedSegments, allSegments;
  getDataSegments(selectedSegments, unselectedSegments);
  allSegments << unselectedSegments << selectedSegments;
  for (int i = 0; i < allSegments.size(); ++i) {
    bool isSelectedSegment = i >= unselectedSegments.size();
    // get line pixel points appropriate to line style:
    QCPDataRange lineDataRange =
        isSelectedSegment
            ? allSegments.at(i)
            : allSegments.at(i).adjusted(
                  -1, 1);  // unselected segments extend lines to bordering
                           // selected data point (safe to exceed total data
                           // bounds in first/last segment, getLines takes care)
    getLines(&lines, lineDataRange);

// check data validity if flag set:
#ifdef QCUSTOMPLOT_CHECK_DATA
    QCPGraphDataContainer::const_iterator it;
    for (it = mDataContainer->constBegin(); it != mDataContainer->constEnd();
         ++it) {
      if (QCP::isInvalidData(it->key, it->value))
        qDebug() << Q_FUNC_INFO << "Data point at" << it->key << "invalid."
                 << "Plottable name:" << name();
    }
#endif

    // draw fill of graph:
    if (isSelectedSegment && mSelectionDecorator)
      mSelectionDecorator->applyBrush(painter);
    else
      painter->setBrush(mBrush);
    painter->setPen(Qt::NoPen);
    drawFill(painter, &lines);

    // draw line:
    if (mLineStyle != lsNone) {
      if (isSelectedSegment && mSelectionDecorator)
        mSelectionDecorator->applyPen(painter);
      else
        painter->setPen(mPen);
      painter->setBrush(Qt::NoBrush);
      if (mLineStyle == lsImpulse)
        drawImpulsePlot(painter, lines);
      else
        drawLinePlot(painter,
                     lines);  // also step plots can be drawn as a line plot
    }

    // draw scatters:
    QCPScatterStyle finalScatterStyle = mScatterStyle;
    if (isSelectedSegment && mSelectionDecorator)
      finalScatterStyle =
          mSelectionDecorator->getFinalScatterStyle(mScatterStyle);
    if (!finalScatterStyle.isNone()) {
      getScatters(&scatters, allSegments.at(i));
      drawScatterPlot(painter, scatters, finalScatterStyle);
    }
  }

  // draw other selection decoration that isn't just line/scatter pens and
  // brushes:
  if (mSelectionDecorator)
    mSelectionDecorator->drawDecoration(painter, selection());
}

/* inherits documentation from base class */
void QCPGraph::drawLegendIcon(QCPPainter *painter, const QRectF &rect) const {
  // draw fill:
  if (mBrush.style() != Qt::NoBrush) {
    applyFillAntialiasingHint(painter);
    painter->fillRect(QRectF(rect.left(), rect.top() + rect.height() / 2.0,
                             rect.width(), rect.height() / 3.0),
                      mBrush);
  }
  // draw line vertically centered:
  if (mLineStyle != lsNone) {
    applyDefaultAntialiasingHint(painter);
    painter->setPen(mPen);
    painter->drawLine(QLineF(
        rect.left(), rect.top() + rect.height() / 2.0, rect.right() + 5,
        rect.top() + rect.height() / 2.0));  // +5 on x2 else last segment is
                                             // missing from dashed/dotted pens
  }
  // draw scatter symbol:
  if (!mScatterStyle.isNone()) {
    applyScattersAntialiasingHint(painter);
    // scale scatter pixmap if it's too large to fit in legend icon rect:
    if (mScatterStyle.shape() == QCPScatterStyle::ssPixmap &&
        (mScatterStyle.pixmap().size().width() > rect.width() ||
         mScatterStyle.pixmap().size().height() > rect.height())) {
      QCPScatterStyle scaledStyle(mScatterStyle);
      scaledStyle.setPixmap(scaledStyle.pixmap().scaled(
          rect.size().toSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
      scaledStyle.applyTo(painter, mPen);
      scaledStyle.drawShape(painter, QRectF(rect).center());
    } else {
      mScatterStyle.applyTo(painter, mPen);
      mScatterStyle.drawShape(painter, QRectF(rect).center());
    }
  }
}

/*! \internal

  This method retrieves an optimized set of data points via \ref
  getOptimizedLineData, and branches out to the line style specific functions
  such as \ref dataToLines, \ref dataToStepLeftLines, etc. according to the line
  style of the graph.

  \a lines will be filled with points in pixel coordinates, that can be drawn
  with the according draw functions like \ref drawLinePlot and \ref
  drawImpulsePlot. The points returned in \a lines aren't necessarily the
  original data points. For example, step line styles require additional points
  to form the steps when drawn. If the line style of the graph is \ref lsNone,
  the \a lines vector will be empty.

  \a dataRange specifies the beginning and ending data indices that will be
  taken into account for conversion. In this function, the specified range may
  exceed the total data bounds without harm: a correspondingly trimmed data
  range will be used. This takes the burden off the user of this function to
  check for valid indices in \a dataRange, e.g. when extending ranges coming
  from \ref getDataSegments.

  \see getScatters
*/
void QCPGraph::getLines(QVector<QPointF> *lines,
                        const QCPDataRange &dataRange) const {
  if (!lines) return;
  QCPGraphDataContainer::const_iterator begin, end;
  getVisibleDataBounds(begin, end, dataRange);
  if (begin == end) {
    lines->clear();
    return;
  }

  QVector<QCPGraphData> lineData;
  if (mLineStyle != lsNone) getOptimizedLineData(&lineData, begin, end);

  if (mKeyAxis->rangeReversed() !=
      (mKeyAxis->orientation() ==
       Qt::Vertical))  // make sure key pixels are sorted ascending in lineData
    // (significantly simplifies following processing)
    std::reverse(lineData.begin(), lineData.end());

  switch (mLineStyle) {
    case lsNone:
      lines->clear();
      break;
    case lsLine:
      *lines = dataToLines(lineData);
      break;
    case lsStepLeft:
      *lines = dataToStepLeftLines(lineData);
      break;
    case lsStepRight:
      *lines = dataToStepRightLines(lineData);
      break;
    case lsStepCenter:
      *lines = dataToStepCenterLines(lineData);
      break;
    case lsImpulse:
      *lines = dataToImpulseLines(lineData);
      break;
  }
}

/*! \internal

  This method retrieves an optimized set of data points via \ref
  getOptimizedScatterData and then converts them to pixel coordinates. The
  resulting points are returned in \a scatters, and can be passed to \ref
  drawScatterPlot.

  \a dataRange specifies the beginning and ending data indices that will be
  taken into account for conversion. In this function, the specified range may
  exceed the total data bounds without harm: a correspondingly trimmed data
  range will be used. This takes the burden off the user of this function to
  check for valid indices in \a dataRange, e.g. when extending ranges coming
  from \ref getDataSegments.
*/
void QCPGraph::getScatters(QVector<QPointF> *scatters,
                           const QCPDataRange &dataRange) const {
  if (!scatters) return;
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    scatters->clear();
    return;
  }

  QCPGraphDataContainer::const_iterator begin, end;
  getVisibleDataBounds(begin, end, dataRange);
  if (begin == end) {
    scatters->clear();
    return;
  }

  QVector<QCPGraphData> data;
  getOptimizedScatterData(&data, begin, end);

  if (mKeyAxis->rangeReversed() !=
      (mKeyAxis->orientation() ==
       Qt::Vertical))  // make sure key pixels are sorted ascending in data
    // (significantly simplifies following processing)
    std::reverse(data.begin(), data.end());

  scatters->resize(data.size());
  if (keyAxis->orientation() == Qt::Vertical) {
    for (int i = 0; i < data.size(); ++i) {
      if (!qIsNaN(data.at(i).value)) {
        (*scatters)[i].setX(valueAxis->coordToPixel(data.at(i).value));
        (*scatters)[i].setY(keyAxis->coordToPixel(data.at(i).key));
      }
    }
  } else {
    for (int i = 0; i < data.size(); ++i) {
      if (!qIsNaN(data.at(i).value)) {
        (*scatters)[i].setX(keyAxis->coordToPixel(data.at(i).key));
        (*scatters)[i].setY(valueAxis->coordToPixel(data.at(i).value));
      }
    }
  }
}

/*! \internal

  Takes raw data points in plot coordinates as \a data, and returns a vector
  containing pixel coordinate points which are suitable for drawing the line
  style \ref lsLine.

  The source of \a data is usually \ref getOptimizedLineData, and this method is
  called in \a getLines if the line style is set accordingly.

  \see dataToStepLeftLines, dataToStepRightLines, dataToStepCenterLines,
  dataToImpulseLines, getLines, drawLinePlot
*/
QVector<QPointF> QCPGraph::dataToLines(
    const QVector<QCPGraphData> &data) const {
  QVector<QPointF> result;
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    return result;
  }

  result.resize(data.size());

  // transform data points to pixels:
  if (keyAxis->orientation() == Qt::Vertical) {
    for (int i = 0; i < data.size(); ++i) {
      result[i].setX(valueAxis->coordToPixel(data.at(i).value));
      result[i].setY(keyAxis->coordToPixel(data.at(i).key));
    }
  } else  // key axis is horizontal
  {
    for (int i = 0; i < data.size(); ++i) {
      result[i].setX(keyAxis->coordToPixel(data.at(i).key));
      result[i].setY(valueAxis->coordToPixel(data.at(i).value));
    }
  }
  return result;
}

/*! \internal

  Takes raw data points in plot coordinates as \a data, and returns a vector
  containing pixel coordinate points which are suitable for drawing the line
  style \ref lsStepLeft.

  The source of \a data is usually \ref getOptimizedLineData, and this method is
  called in \a getLines if the line style is set accordingly.

  \see dataToLines, dataToStepRightLines, dataToStepCenterLines,
  dataToImpulseLines, getLines, drawLinePlot
*/
QVector<QPointF> QCPGraph::dataToStepLeftLines(
    const QVector<QCPGraphData> &data) const {
  QVector<QPointF> result;
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    return result;
  }

  result.resize(data.size() * 2);

  // calculate steps from data and transform to pixel coordinates:
  if (keyAxis->orientation() == Qt::Vertical) {
    double lastValue = valueAxis->coordToPixel(data.first().value);
    for (int i = 0; i < data.size(); ++i) {
      const double key = keyAxis->coordToPixel(data.at(i).key);
      result[i * 2 + 0].setX(lastValue);
      result[i * 2 + 0].setY(key);
      lastValue = valueAxis->coordToPixel(data.at(i).value);
      result[i * 2 + 1].setX(lastValue);
      result[i * 2 + 1].setY(key);
    }
  } else  // key axis is horizontal
  {
    double lastValue = valueAxis->coordToPixel(data.first().value);
    for (int i = 0; i < data.size(); ++i) {
      const double key = keyAxis->coordToPixel(data.at(i).key);
      result[i * 2 + 0].setX(key);
      result[i * 2 + 0].setY(lastValue);
      lastValue = valueAxis->coordToPixel(data.at(i).value);
      result[i * 2 + 1].setX(key);
      result[i * 2 + 1].setY(lastValue);
    }
  }
  return result;
}

/*! \internal

  Takes raw data points in plot coordinates as \a data, and returns a vector
  containing pixel coordinate points which are suitable for drawing the line
  style \ref lsStepRight.

  The source of \a data is usually \ref getOptimizedLineData, and this method is
  called in \a getLines if the line style is set accordingly.

  \see dataToLines, dataToStepLeftLines, dataToStepCenterLines,
  dataToImpulseLines, getLines, drawLinePlot
*/
QVector<QPointF> QCPGraph::dataToStepRightLines(
    const QVector<QCPGraphData> &data) const {
  QVector<QPointF> result;
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    return result;
  }

  result.resize(data.size() * 2);

  // calculate steps from data and transform to pixel coordinates:
  if (keyAxis->orientation() == Qt::Vertical) {
    double lastKey = keyAxis->coordToPixel(data.first().key);
    for (int i = 0; i < data.size(); ++i) {
      const double value = valueAxis->coordToPixel(data.at(i).value);
      result[i * 2 + 0].setX(value);
      result[i * 2 + 0].setY(lastKey);
      lastKey = keyAxis->coordToPixel(data.at(i).key);
      result[i * 2 + 1].setX(value);
      result[i * 2 + 1].setY(lastKey);
    }
  } else  // key axis is horizontal
  {
    double lastKey = keyAxis->coordToPixel(data.first().key);
    for (int i = 0; i < data.size(); ++i) {
      const double value = valueAxis->coordToPixel(data.at(i).value);
      result[i * 2 + 0].setX(lastKey);
      result[i * 2 + 0].setY(value);
      lastKey = keyAxis->coordToPixel(data.at(i).key);
      result[i * 2 + 1].setX(lastKey);
      result[i * 2 + 1].setY(value);
    }
  }
  return result;
}

/*! \internal

  Takes raw data points in plot coordinates as \a data, and returns a vector
  containing pixel coordinate points which are suitable for drawing the line
  style \ref lsStepCenter.

  The source of \a data is usually \ref getOptimizedLineData, and this method is
  called in \a getLines if the line style is set accordingly.

  \see dataToLines, dataToStepLeftLines, dataToStepRightLines,
  dataToImpulseLines, getLines, drawLinePlot
*/
QVector<QPointF> QCPGraph::dataToStepCenterLines(
    const QVector<QCPGraphData> &data) const {
  QVector<QPointF> result;
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    return result;
  }

  result.resize(data.size() * 2);

  // calculate steps from data and transform to pixel coordinates:
  if (keyAxis->orientation() == Qt::Vertical) {
    double lastKey = keyAxis->coordToPixel(data.first().key);
    double lastValue = valueAxis->coordToPixel(data.first().value);
    result[0].setX(lastValue);
    result[0].setY(lastKey);
    for (int i = 1; i < data.size(); ++i) {
      const double key =
          (keyAxis->coordToPixel(data.at(i).key) + lastKey) * 0.5;
      result[i * 2 - 1].setX(lastValue);
      result[i * 2 - 1].setY(key);
      lastValue = valueAxis->coordToPixel(data.at(i).value);
      lastKey = keyAxis->coordToPixel(data.at(i).key);
      result[i * 2 + 0].setX(lastValue);
      result[i * 2 + 0].setY(key);
    }
    result[data.size() * 2 - 1].setX(lastValue);
    result[data.size() * 2 - 1].setY(lastKey);
  } else  // key axis is horizontal
  {
    double lastKey = keyAxis->coordToPixel(data.first().key);
    double lastValue = valueAxis->coordToPixel(data.first().value);
    result[0].setX(lastKey);
    result[0].setY(lastValue);
    for (int i = 1; i < data.size(); ++i) {
      const double key =
          (keyAxis->coordToPixel(data.at(i).key) + lastKey) * 0.5;
      result[i * 2 - 1].setX(key);
      result[i * 2 - 1].setY(lastValue);
      lastValue = valueAxis->coordToPixel(data.at(i).value);
      lastKey = keyAxis->coordToPixel(data.at(i).key);
      result[i * 2 + 0].setX(key);
      result[i * 2 + 0].setY(lastValue);
    }
    result[data.size() * 2 - 1].setX(lastKey);
    result[data.size() * 2 - 1].setY(lastValue);
  }
  return result;
}

/*! \internal

  Takes raw data points in plot coordinates as \a data, and returns a vector
  containing pixel coordinate points which are suitable for drawing the line
  style \ref lsImpulse.

  The source of \a data is usually \ref getOptimizedLineData, and this method is
  called in \a getLines if the line style is set accordingly.

  \see dataToLines, dataToStepLeftLines, dataToStepRightLines,
  dataToStepCenterLines, getLines, drawImpulsePlot
*/
QVector<QPointF> QCPGraph::dataToImpulseLines(
    const QVector<QCPGraphData> &data) const {
  QVector<QPointF> result;
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    return result;
  }

  result.resize(data.size() * 2);

  // transform data points to pixels:
  if (keyAxis->orientation() == Qt::Vertical) {
    for (int i = 0; i < data.size(); ++i) {
      const QCPGraphData &current = data.at(i);
      if (!qIsNaN(current.value)) {
        const double key = keyAxis->coordToPixel(current.key);
        result[i * 2 + 0].setX(valueAxis->coordToPixel(0));
        result[i * 2 + 0].setY(key);
        result[i * 2 + 1].setX(valueAxis->coordToPixel(current.value));
        result[i * 2 + 1].setY(key);
      } else {
        result[i * 2 + 0] = QPointF(0, 0);
        result[i * 2 + 1] = QPointF(0, 0);
      }
    }
  } else  // key axis is horizontal
  {
    for (int i = 0; i < data.size(); ++i) {
      const QCPGraphData &current = data.at(i);
      if (!qIsNaN(current.value)) {
        const double key = keyAxis->coordToPixel(data.at(i).key);
        result[i * 2 + 0].setX(key);
        result[i * 2 + 0].setY(valueAxis->coordToPixel(0));
        result[i * 2 + 1].setX(key);
        result[i * 2 + 1].setY(valueAxis->coordToPixel(data.at(i).value));
      } else {
        result[i * 2 + 0] = QPointF(0, 0);
        result[i * 2 + 1] = QPointF(0, 0);
      }
    }
  }
  return result;
}

/*! \internal

  Draws the fill of the graph using the specified \a painter, with the currently
  set brush.

  Depending on whether a normal fill or a channel fill (\ref
  setChannelFillGraph) is needed, \ref getFillPolygon or \ref
  getChannelFillPolygon are used to find the according fill polygons.

  In order to handle NaN Data points correctly (the fill needs to be split into
  disjoint areas), this method first determines a list of non-NaN segments with
  \ref getNonNanSegments, on which to operate. In the channel fill case, \ref
  getOverlappingSegments is used to consolidate the non-NaN segments of the two
  involved graphs, before passing the overlapping pairs to \ref
  getChannelFillPolygon.

  Pass the points of this graph's line as \a lines, in pixel coordinates.

  \see drawLinePlot, drawImpulsePlot, drawScatterPlot
*/
void QCPGraph::drawFill(QCPPainter *painter, QVector<QPointF> *lines) const {
  if (mLineStyle == lsImpulse)
    return;  // fill doesn't make sense for impulse plot
  if (painter->brush().style() == Qt::NoBrush ||
      painter->brush().color().alpha() == 0)
    return;

  applyFillAntialiasingHint(painter);
  const QVector<QCPDataRange> segments =
      getNonNanSegments(lines, keyAxis()->orientation());
  if (!mChannelFillGraph) {
    // draw base fill under graph, fill goes all the way to the zero-value-line:
    foreach (QCPDataRange segment, segments)
      painter->drawPolygon(getFillPolygon(lines, segment));
  } else {
    // draw fill between this graph and mChannelFillGraph:
    QVector<QPointF> otherLines;
    mChannelFillGraph->getLines(
        &otherLines, QCPDataRange(0, mChannelFillGraph->dataCount()));
    if (!otherLines.isEmpty()) {
      QVector<QCPDataRange> otherSegments = getNonNanSegments(
          &otherLines, mChannelFillGraph->keyAxis()->orientation());
      QVector<QPair<QCPDataRange, QCPDataRange> > segmentPairs =
          getOverlappingSegments(segments, lines, otherSegments, &otherLines);
      for (int i = 0; i < segmentPairs.size(); ++i)
        painter->drawPolygon(
            getChannelFillPolygon(lines, segmentPairs.at(i).first, &otherLines,
                                  segmentPairs.at(i).second));
    }
  }
}

/*! \internal

  Draws scatter symbols at every point passed in \a scatters, given in pixel
  coordinates. The scatters will be drawn with \a painter and have the
  appearance as specified in \a style.

  \see drawLinePlot, drawImpulsePlot
*/
void QCPGraph::drawScatterPlot(QCPPainter *painter,
                               const QVector<QPointF> &scatters,
                               const QCPScatterStyle &style) const {
  applyScattersAntialiasingHint(painter);
  style.applyTo(painter, mPen);
  foreach (const QPointF &scatter, scatters)
    style.drawShape(painter, scatter.x(), scatter.y());
}

/*!  \internal

  Draws lines between the points in \a lines, given in pixel coordinates.

  \see drawScatterPlot, drawImpulsePlot, QCPAbstractPlottable1D::drawPolyline
*/
void QCPGraph::drawLinePlot(QCPPainter *painter,
                            const QVector<QPointF> &lines) const {
  if (painter->pen().style() != Qt::NoPen &&
      painter->pen().color().alpha() != 0) {
    applyDefaultAntialiasingHint(painter);
    drawPolyline(painter, lines);
  }
}

/*! \internal

  Draws impulses from the provided data, i.e. it connects all line pairs in \a
  lines, given in pixel coordinates. The \a lines necessary for impulses are
  generated by \ref dataToImpulseLines from the regular graph data points.

  \see drawLinePlot, drawScatterPlot
*/
void QCPGraph::drawImpulsePlot(QCPPainter *painter,
                               const QVector<QPointF> &lines) const {
  if (painter->pen().style() != Qt::NoPen &&
      painter->pen().color().alpha() != 0) {
    applyDefaultAntialiasingHint(painter);
    QPen oldPen = painter->pen();
    QPen newPen = painter->pen();
    newPen.setCapStyle(
        Qt::FlatCap);  // so impulse line doesn't reach beyond zero-line
    painter->setPen(newPen);
    painter->drawLines(lines);
    painter->setPen(oldPen);
  }
}

/*! \internal

  Returns via \a lineData the data points that need to be visualized for this
  graph when plotting graph lines, taking into consideration the currently
  visible axis ranges and, if \ref setAdaptiveSampling is enabled, local point
  densities. The considered data can be restricted further by \a begin and \a
  end, e.g. to only plot a certain segment of the data (see \ref
  getDataSegments).

  This method is used by \ref getLines to retrieve the basic working set of
  data.

  \see getOptimizedScatterData
*/
void QCPGraph::getOptimizedLineData(
    QVector<QCPGraphData> *lineData,
    const QCPGraphDataContainer::const_iterator &begin,
    const QCPGraphDataContainer::const_iterator &end) const {
  if (!lineData) return;
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    return;
  }
  if (begin == end) return;

  int dataCount = int(end - begin);
  int maxCount = (std::numeric_limits<int>::max)();
  if (mAdaptiveSampling) {
    double keyPixelSpan = qAbs(keyAxis->coordToPixel(begin->key) -
                               keyAxis->coordToPixel((end - 1)->key));
    if (2 * keyPixelSpan + 2 <
        static_cast<double>((std::numeric_limits<int>::max)()))
      maxCount = int(2 * keyPixelSpan + 2);
  }

  if (mAdaptiveSampling &&
      dataCount >= maxCount)  // use adaptive sampling only if there are at
  // least two points per pixel on average
  {
    QCPGraphDataContainer::const_iterator it = begin;
    double minValue = it->value;
    double maxValue = it->value;
    QCPGraphDataContainer::const_iterator currentIntervalFirstPoint = it;
    int reversedFactor =
        keyAxis->pixelOrientation();  // is used to calculate keyEpsilon pixel
                                      // into the correct direction
    int reversedRound =
        reversedFactor == -1
            ? 1
            : 0;  // is used to switch between floor (normal) and ceil
                  // (reversed) rounding of currentIntervalStartKey
    double currentIntervalStartKey = keyAxis->pixelToCoord(
        int(keyAxis->coordToPixel(begin->key) + reversedRound));
    double lastIntervalEndKey = currentIntervalStartKey;
    double keyEpsilon =
        qAbs(currentIntervalStartKey -
             keyAxis->pixelToCoord(
                 keyAxis->coordToPixel(currentIntervalStartKey) +
                 1.0 * reversedFactor));  // interval of one pixel on screen
                                          // when mapped to plot key coordinates
    bool keyEpsilonVariable =
        keyAxis->scaleType() ==
        QCPAxis::stLogarithmic;  // indicates whether keyEpsilon needs to be
                                 // updated after every interval (for log axes)
    int intervalDataCount = 1;
    ++it;  // advance iterator to second data point because adaptive sampling
           // works in 1 point retrospect
    while (it != end) {
      if (it->key <
          currentIntervalStartKey +
              keyEpsilon)  // data point is still within same pixel, so skip it
      // and expand value span of this cluster if necessary
      {
        if (it->value < minValue)
          minValue = it->value;
        else if (it->value > maxValue)
          maxValue = it->value;
        ++intervalDataCount;
      } else  // new pixel interval started
      {
        if (intervalDataCount >= 2)  // last pixel had multiple data points,
        // consolidate them to a cluster
        {
          if (lastIntervalEndKey <
              currentIntervalStartKey -
                  keyEpsilon)  // last point is further away, so first point of
            // this cluster must be at a real data point
            lineData->append(
                QCPGraphData(currentIntervalStartKey + keyEpsilon * 0.2,
                             currentIntervalFirstPoint->value));
          lineData->append(QCPGraphData(
              currentIntervalStartKey + keyEpsilon * 0.25, minValue));
          lineData->append(QCPGraphData(
              currentIntervalStartKey + keyEpsilon * 0.75, maxValue));
          if (it->key >
              currentIntervalStartKey +
                  keyEpsilon *
                      2)  // new pixel started further away from previous
            // cluster, so make sure the last point of the cluster
            // is at a real data point
            lineData->append(QCPGraphData(
                currentIntervalStartKey + keyEpsilon * 0.8, (it - 1)->value));
        } else
          lineData->append(QCPGraphData(currentIntervalFirstPoint->key,
                                        currentIntervalFirstPoint->value));
        lastIntervalEndKey = (it - 1)->key;
        minValue = it->value;
        maxValue = it->value;
        currentIntervalFirstPoint = it;
        currentIntervalStartKey = keyAxis->pixelToCoord(
            int(keyAxis->coordToPixel(it->key) + reversedRound));
        if (keyEpsilonVariable)
          keyEpsilon = qAbs(currentIntervalStartKey -
                            keyAxis->pixelToCoord(
                                keyAxis->coordToPixel(currentIntervalStartKey) +
                                1.0 * reversedFactor));
        intervalDataCount = 1;
      }
      ++it;
    }
    // handle last interval:
    if (intervalDataCount >= 2)  // last pixel had multiple data points,
    // consolidate them to a cluster
    {
      if (lastIntervalEndKey <
          currentIntervalStartKey -
              keyEpsilon)  // last point wasn't a cluster, so first point of
        // this cluster must be at a real data point
        lineData->append(
            QCPGraphData(currentIntervalStartKey + keyEpsilon * 0.2,
                         currentIntervalFirstPoint->value));
      lineData->append(
          QCPGraphData(currentIntervalStartKey + keyEpsilon * 0.25, minValue));
      lineData->append(
          QCPGraphData(currentIntervalStartKey + keyEpsilon * 0.75, maxValue));
    } else
      lineData->append(QCPGraphData(currentIntervalFirstPoint->key,
                                    currentIntervalFirstPoint->value));

  } else  // don't use adaptive sampling algorithm, transfer points one-to-one
  // from the data container into the output
  {
    lineData->resize(dataCount);
    std::copy(begin, end, lineData->begin());
  }
}

/*! \internal

  Returns via \a scatterData the data points that need to be visualized for this
  graph when plotting scatter points, taking into consideration the currently
  visible axis ranges and, if \ref setAdaptiveSampling is enabled, local point
  densities. The considered data can be restricted further by \a begin and \a
  end, e.g. to only plot a certain segment of the data (see \ref
  getDataSegments).

  This method is used by \ref getScatters to retrieve the basic working set of
  data.

  \see getOptimizedLineData
*/
void QCPGraph::getOptimizedScatterData(
    QVector<QCPGraphData> *scatterData,
    QCPGraphDataContainer::const_iterator begin,
    QCPGraphDataContainer::const_iterator end) const {
  if (!scatterData) return;
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    return;
  }

  const int scatterModulo = mScatterSkip + 1;
  const bool doScatterSkip = mScatterSkip > 0;
  int beginIndex = int(begin - mDataContainer->constBegin());
  int endIndex = int(end - mDataContainer->constBegin());
  while (doScatterSkip && begin != end &&
         beginIndex % scatterModulo !=
             0)  // advance begin iterator to first non-skipped scatter
  {
    ++beginIndex;
    ++begin;
  }
  if (begin == end) return;
  int dataCount = int(end - begin);
  int maxCount = (std::numeric_limits<int>::max)();
  if (mAdaptiveSampling) {
    int keyPixelSpan = int(qAbs(keyAxis->coordToPixel(begin->key) -
                                keyAxis->coordToPixel((end - 1)->key)));
    maxCount = 2 * keyPixelSpan + 2;
  }

  if (mAdaptiveSampling &&
      dataCount >= maxCount)  // use adaptive sampling only if there are at
  // least two points per pixel on average
  {
    double valueMaxRange = valueAxis->range().upper;
    double valueMinRange = valueAxis->range().lower;
    QCPGraphDataContainer::const_iterator it = begin;
    int itIndex = int(beginIndex);
    double minValue = it->value;
    double maxValue = it->value;
    QCPGraphDataContainer::const_iterator minValueIt = it;
    QCPGraphDataContainer::const_iterator maxValueIt = it;
    QCPGraphDataContainer::const_iterator currentIntervalStart = it;
    int reversedFactor =
        keyAxis->pixelOrientation();  // is used to calculate keyEpsilon pixel
                                      // into the correct direction
    int reversedRound =
        reversedFactor == -1
            ? 1
            : 0;  // is used to switch between floor (normal) and ceil
                  // (reversed) rounding of currentIntervalStartKey
    double currentIntervalStartKey = keyAxis->pixelToCoord(
        int(keyAxis->coordToPixel(begin->key) + reversedRound));
    double keyEpsilon =
        qAbs(currentIntervalStartKey -
             keyAxis->pixelToCoord(
                 keyAxis->coordToPixel(currentIntervalStartKey) +
                 1.0 * reversedFactor));  // interval of one pixel on screen
                                          // when mapped to plot key coordinates
    bool keyEpsilonVariable =
        keyAxis->scaleType() ==
        QCPAxis::stLogarithmic;  // indicates whether keyEpsilon needs to be
                                 // updated after every interval (for log axes)
    int intervalDataCount = 1;
    // advance iterator to second (non-skipped) data point because adaptive
    // sampling works in 1 point retrospect:
    if (!doScatterSkip)
      ++it;
    else {
      itIndex += scatterModulo;
      if (itIndex < endIndex)  // make sure we didn't jump over end
        it += scatterModulo;
      else {
        it = end;
        itIndex = endIndex;
      }
    }
    // main loop over data points:
    while (it != end) {
      if (it->key <
          currentIntervalStartKey +
              keyEpsilon)  // data point is still within same pixel, so skip it
      // and expand value span of this pixel if necessary
      {
        if (it->value < minValue && it->value > valueMinRange &&
            it->value < valueMaxRange) {
          minValue = it->value;
          minValueIt = it;
        } else if (it->value > maxValue && it->value > valueMinRange &&
                   it->value < valueMaxRange) {
          maxValue = it->value;
          maxValueIt = it;
        }
        ++intervalDataCount;
      } else  // new pixel started
      {
        if (intervalDataCount >=
            2)  // last pixel had multiple data points, consolidate them
        {
          // determine value pixel span and add as many points in interval to
          // maintain certain vertical data density (this is specific to scatter
          // plot):
          double valuePixelSpan = qAbs(valueAxis->coordToPixel(minValue) -
                                       valueAxis->coordToPixel(maxValue));
          int dataModulo =
              qMax(1, qRound(intervalDataCount /
                             (valuePixelSpan /
                              4.0)));  // approximately every 4 value pixels one
                                       // data point on average
          QCPGraphDataContainer::const_iterator intervalIt =
              currentIntervalStart;
          int c = 0;
          while (intervalIt != it) {
            if ((c % dataModulo == 0 || intervalIt == minValueIt ||
                 intervalIt == maxValueIt) &&
                intervalIt->value > valueMinRange &&
                intervalIt->value < valueMaxRange)
              scatterData->append(*intervalIt);
            ++c;
            if (!doScatterSkip)
              ++intervalIt;
            else
              intervalIt += scatterModulo;  // since we know indices of
            // "currentIntervalStart", "intervalIt" and
            // "it" are multiples of scatterModulo, we
            // can't accidentally jump over "it" here
          }
        } else if (currentIntervalStart->value > valueMinRange &&
                   currentIntervalStart->value < valueMaxRange)
          scatterData->append(*currentIntervalStart);
        minValue = it->value;
        maxValue = it->value;
        currentIntervalStart = it;
        currentIntervalStartKey = keyAxis->pixelToCoord(
            int(keyAxis->coordToPixel(it->key) + reversedRound));
        if (keyEpsilonVariable)
          keyEpsilon = qAbs(currentIntervalStartKey -
                            keyAxis->pixelToCoord(
                                keyAxis->coordToPixel(currentIntervalStartKey) +
                                1.0 * reversedFactor));
        intervalDataCount = 1;
      }
      // advance to next data point:
      if (!doScatterSkip)
        ++it;
      else {
        itIndex += scatterModulo;
        if (itIndex < endIndex)  // make sure we didn't jump over end
          it += scatterModulo;
        else {
          it = end;
          itIndex = endIndex;
        }
      }
    }
    // handle last interval:
    if (intervalDataCount >=
        2)  // last pixel had multiple data points, consolidate them
    {
      // determine value pixel span and add as many points in interval to
      // maintain certain vertical data density (this is specific to scatter
      // plot):
      double valuePixelSpan = qAbs(valueAxis->coordToPixel(minValue) -
                                   valueAxis->coordToPixel(maxValue));
      int dataModulo =
          qMax(1,
               qRound(intervalDataCount /
                      (valuePixelSpan / 4.0)));  // approximately every 4 value
      // pixels one data point on average
      QCPGraphDataContainer::const_iterator intervalIt = currentIntervalStart;
      int intervalItIndex = int(intervalIt - mDataContainer->constBegin());
      int c = 0;
      while (intervalIt != it) {
        if ((c % dataModulo == 0 || intervalIt == minValueIt ||
             intervalIt == maxValueIt) &&
            intervalIt->value > valueMinRange &&
            intervalIt->value < valueMaxRange)
          scatterData->append(*intervalIt);
        ++c;
        if (!doScatterSkip)
          ++intervalIt;
        else  // here we can't guarantee that adding scatterModulo doesn't
        // exceed "it" (because "it" is equal to "end" here, and "end"
        // isn't scatterModulo-aligned), so check via index comparison:
        {
          intervalItIndex += scatterModulo;
          if (intervalItIndex < itIndex)
            intervalIt += scatterModulo;
          else {
            intervalIt = it;
            intervalItIndex = itIndex;
          }
        }
      }
    } else if (currentIntervalStart->value > valueMinRange &&
               currentIntervalStart->value < valueMaxRange)
      scatterData->append(*currentIntervalStart);

  } else  // don't use adaptive sampling algorithm, transfer points one-to-one
  // from the data container into the output
  {
    QCPGraphDataContainer::const_iterator it = begin;
    int itIndex = beginIndex;
    scatterData->reserve(dataCount);
    while (it != end) {
      scatterData->append(*it);
      // advance to next data point:
      if (!doScatterSkip)
        ++it;
      else {
        itIndex += scatterModulo;
        if (itIndex < endIndex)
          it += scatterModulo;
        else {
          it = end;
          itIndex = endIndex;
        }
      }
    }
  }
}

/*!
  This method outputs the currently visible data range via \a begin and \a end.
  The returned range will also never exceed \a rangeRestriction.

  This method takes into account that the drawing of data lines at the axis rect
  border always requires the points just outside the visible axis range. So \a
  begin and \a end may actually indicate a range that contains one additional
  data point to the left and right of the visible axis range.
*/
void QCPGraph::getVisibleDataBounds(
    QCPGraphDataContainer::const_iterator &begin,
    QCPGraphDataContainer::const_iterator &end,
    const QCPDataRange &rangeRestriction) const {
  if (rangeRestriction.isEmpty()) {
    end = mDataContainer->constEnd();
    begin = end;
  } else {
    QCPAxis *keyAxis = mKeyAxis.data();
    QCPAxis *valueAxis = mValueAxis.data();
    if (!keyAxis || !valueAxis) {
      qDebug() << Q_FUNC_INFO << "invalid key or value axis";
      return;
    }
    // get visible data range:
    begin = mDataContainer->findBegin(keyAxis->range().lower);
    end = mDataContainer->findEnd(keyAxis->range().upper);
    // limit lower/upperEnd to rangeRestriction:
    mDataContainer->limitIteratorsToDataRange(
        begin, end,
        rangeRestriction);  // this also ensures rangeRestriction outside data
                            // bounds doesn't break anything
  }
}

/*!  \internal

  This method goes through the passed points in \a lineData and returns a list
  of the segments which don't contain NaN data points.

  \a keyOrientation defines whether the \a x or \a y member of the passed
  QPointF is used to check for NaN. If \a keyOrientation is \c Qt::Horizontal,
  the \a y member is checked, if it is \c Qt::Vertical, the \a x member is
  checked.

  \see getOverlappingSegments, drawFill
*/
QVector<QCPDataRange> QCPGraph::getNonNanSegments(
    const QVector<QPointF> *lineData, Qt::Orientation keyOrientation) const {
  QVector<QCPDataRange> result;
  const int n = lineData->size();

  QCPDataRange currentSegment(-1, -1);
  int i = 0;

  if (keyOrientation == Qt::Horizontal) {
    while (i < n) {
      while (i < n &&
             qIsNaN(lineData->at(i).y()))  // seek next non-NaN data point
        ++i;
      if (i == n) break;
      currentSegment.setBegin(i++);
      while (
          i < n &&
          !qIsNaN(
              lineData->at(i).y()))  // seek next NaN data point or end of data
        ++i;
      currentSegment.setEnd(i++);
      result.append(currentSegment);
    }
  } else  // keyOrientation == Qt::Vertical
  {
    while (i < n) {
      while (i < n &&
             qIsNaN(lineData->at(i).x()))  // seek next non-NaN data point
        ++i;
      if (i == n) break;
      currentSegment.setBegin(i++);
      while (
          i < n &&
          !qIsNaN(
              lineData->at(i).x()))  // seek next NaN data point or end of data
        ++i;
      currentSegment.setEnd(i++);
      result.append(currentSegment);
    }
  }
  return result;
}

/*!  \internal

  This method takes two segment lists (e.g. created by \ref getNonNanSegments)
  \a thisSegments and \a otherSegments, and their associated point data \a
  thisData and \a otherData.

  It returns all pairs of segments (the first from \a thisSegments, the second
  from \a otherSegments), which overlap in plot coordinates.

  This method is useful in the case of a channel fill between two graphs, when
  only those non-NaN segments which actually overlap in their key coordinate
  shall be considered for drawing a channel fill polygon.

  It is assumed that the passed segments in \a thisSegments are ordered
  ascending by index, and that the segments don't overlap themselves. The same
  is assumed for the segments in \a otherSegments. This is fulfilled when the
  segments are obtained via \ref getNonNanSegments.

  \see getNonNanSegments, segmentsIntersect, drawFill, getChannelFillPolygon
*/
QVector<QPair<QCPDataRange, QCPDataRange> > QCPGraph::getOverlappingSegments(
    QVector<QCPDataRange> thisSegments, const QVector<QPointF> *thisData,
    QVector<QCPDataRange> otherSegments,
    const QVector<QPointF> *otherData) const {
  QVector<QPair<QCPDataRange, QCPDataRange> > result;
  if (thisData->isEmpty() || otherData->isEmpty() || thisSegments.isEmpty() ||
      otherSegments.isEmpty())
    return result;

  int thisIndex = 0;
  int otherIndex = 0;
  const bool verticalKey = mKeyAxis->orientation() == Qt::Vertical;
  while (thisIndex < thisSegments.size() && otherIndex < otherSegments.size()) {
    if (thisSegments.at(thisIndex).size() <
        2)  // segments with fewer than two points won't have a fill anyhow
    {
      ++thisIndex;
      continue;
    }
    if (otherSegments.at(otherIndex).size() <
        2)  // segments with fewer than two points won't have a fill anyhow
    {
      ++otherIndex;
      continue;
    }
    double thisLower, thisUpper, otherLower, otherUpper;
    if (!verticalKey) {
      thisLower = thisData->at(thisSegments.at(thisIndex).begin()).x();
      thisUpper = thisData->at(thisSegments.at(thisIndex).end() - 1).x();
      otherLower = otherData->at(otherSegments.at(otherIndex).begin()).x();
      otherUpper = otherData->at(otherSegments.at(otherIndex).end() - 1).x();
    } else {
      thisLower = thisData->at(thisSegments.at(thisIndex).begin()).y();
      thisUpper = thisData->at(thisSegments.at(thisIndex).end() - 1).y();
      otherLower = otherData->at(otherSegments.at(otherIndex).begin()).y();
      otherUpper = otherData->at(otherSegments.at(otherIndex).end() - 1).y();
    }

    int bPrecedence;
    if (segmentsIntersect(thisLower, thisUpper, otherLower, otherUpper,
                          bPrecedence))
      result.append(QPair<QCPDataRange, QCPDataRange>(
          thisSegments.at(thisIndex), otherSegments.at(otherIndex)));

    if (bPrecedence <=
        0)  // otherSegment doesn't reach as far as thisSegment, so continue
      // with next otherSegment, keeping current thisSegment
      ++otherIndex;
    else  // otherSegment reaches further than thisSegment, so continue with
      // next thisSegment, keeping current otherSegment
      ++thisIndex;
  }

  return result;
}

/*!  \internal

  Returns whether the segments defined by the coordinates (aLower, aUpper) and
  (bLower, bUpper) have overlap.

  The output parameter \a bPrecedence indicates whether the \a b segment reaches
  farther than the \a a segment or not. If \a bPrecedence returns 1, segment \a
  b reaches the farthest to higher coordinates (i.e. bUpper > aUpper). If it
  returns -1, segment \a a reaches the farthest. Only if both segment's upper
  bounds are identical, 0 is returned as \a bPrecedence.

  It is assumed that the lower bounds always have smaller or equal values than
  the upper bounds.

  \see getOverlappingSegments
*/
bool QCPGraph::segmentsIntersect(double aLower, double aUpper, double bLower,
                                 double bUpper, int &bPrecedence) const {
  bPrecedence = 0;
  if (aLower > bUpper) {
    bPrecedence = -1;
    return false;
  } else if (bLower > aUpper) {
    bPrecedence = 1;
    return false;
  } else {
    if (aUpper > bUpper)
      bPrecedence = -1;
    else if (aUpper < bUpper)
      bPrecedence = 1;

    return true;
  }
}

/*! \internal

  Returns the point which closes the fill polygon on the zero-value-line
  parallel to the key axis. The logarithmic axis scale case is a bit special,
  since the zero-value-line in pixel coordinates is in positive or negative
  infinity. So this case is handled separately by just closing the fill polygon
  on the axis which lies in the direction towards the zero value.

  \a matchingDataPoint will provide the key (in pixels) of the returned point.
  Depending on whether the key axis of this graph is horizontal or vertical, \a
  matchingDataPoint will provide the x or y value of the returned point,
  respectively.
*/
QPointF QCPGraph::getFillBasePoint(QPointF matchingDataPoint) const {
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    return {};
  }

  QPointF result;
  if (valueAxis->scaleType() == QCPAxis::stLinear) {
    if (keyAxis->orientation() == Qt::Horizontal) {
      result.setX(matchingDataPoint.x());
      result.setY(valueAxis->coordToPixel(0));
    } else  // keyAxis->orientation() == Qt::Vertical
    {
      result.setX(valueAxis->coordToPixel(0));
      result.setY(matchingDataPoint.y());
    }
  } else  // valueAxis->mScaleType == QCPAxis::stLogarithmic
  {
    // In logarithmic scaling we can't just draw to value 0 so we just fill all
    // the way to the axis which is in the direction towards 0
    if (keyAxis->orientation() == Qt::Vertical) {
      if ((valueAxis->range().upper < 0 && !valueAxis->rangeReversed()) ||
          (valueAxis->range().upper > 0 &&
           valueAxis->rangeReversed()))  // if range is negative, zero is on
        // opposite side of key axis
        result.setX(keyAxis->axisRect()->right());
      else
        result.setX(keyAxis->axisRect()->left());
      result.setY(matchingDataPoint.y());
    } else if (keyAxis->axisType() == QCPAxis::atTop ||
               keyAxis->axisType() == QCPAxis::atBottom) {
      result.setX(matchingDataPoint.x());
      if ((valueAxis->range().upper < 0 && !valueAxis->rangeReversed()) ||
          (valueAxis->range().upper > 0 &&
           valueAxis->rangeReversed()))  // if range is negative, zero is on
        // opposite side of key axis
        result.setY(keyAxis->axisRect()->top());
      else
        result.setY(keyAxis->axisRect()->bottom());
    }
  }
  return result;
}

/*! \internal

  Returns the polygon needed for drawing normal fills between this graph and the
  key axis.

  Pass the graph's data points (in pixel coordinates) as \a lineData, and
  specify the \a segment which shall be used for the fill. The collection of \a
  lineData points described by \a segment must not contain NaN data points (see
  \ref getNonNanSegments).

  The returned fill polygon will be closed at the key axis (the zero-value line)
  for linear value axes. For logarithmic value axes the polygon will reach just
  beyond the corresponding axis rect side (see \ref getFillBasePoint).

  For increased performance (due to implicit sharing), keep the returned
  QPolygonF const.

  \see drawFill, getNonNanSegments
*/
const QPolygonF QCPGraph::getFillPolygon(const QVector<QPointF> *lineData,
                                         QCPDataRange segment) const {
  if (segment.size() < 2) return QPolygonF();
  QPolygonF result(segment.size() + 2);

  result[0] = getFillBasePoint(lineData->at(segment.begin()));
  std::copy(lineData->constBegin() + segment.begin(),
            lineData->constBegin() + segment.end(), result.begin() + 1);
  result[result.size() - 1] = getFillBasePoint(lineData->at(segment.end() - 1));

  return result;
}

/*! \internal

  Returns the polygon needed for drawing (partial) channel fills between this
  graph and the graph specified by \ref setChannelFillGraph.

  The data points of this graph are passed as pixel coordinates via \a thisData,
  the data of the other graph as \a otherData. The returned polygon will be
  calculated for the specified data segments \a thisSegment and \a otherSegment,
  pertaining to the respective \a thisData and \a otherData, respectively.

  The passed \a thisSegment and \a otherSegment should correspond to the segment
  pairs returned by \ref getOverlappingSegments, to make sure only segments that
  actually have key coordinate overlap need to be processed here.

  For increased performance due to implicit sharing, keep the returned QPolygonF
  const.

  \see drawFill, getOverlappingSegments, getNonNanSegments
*/
const QPolygonF QCPGraph::getChannelFillPolygon(
    const QVector<QPointF> *thisData, QCPDataRange thisSegment,
    const QVector<QPointF> *otherData, QCPDataRange otherSegment) const {
  if (!mChannelFillGraph) return QPolygonF();

  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    return QPolygonF();
  }
  if (!mChannelFillGraph.data()->mKeyAxis) {
    qDebug() << Q_FUNC_INFO << "channel fill target key axis invalid";
    return QPolygonF();
  }

  if (mChannelFillGraph.data()->mKeyAxis.data()->orientation() !=
      keyAxis->orientation())
    return QPolygonF();  // don't have same axis orientation, can't fill that
                         // (Note: if keyAxis fits, valueAxis will fit too,
                         // because it's always orthogonal to keyAxis)

  if (thisData->isEmpty()) return QPolygonF();
  QVector<QPointF> thisSegmentData(thisSegment.size());
  QVector<QPointF> otherSegmentData(otherSegment.size());
  std::copy(thisData->constBegin() + thisSegment.begin(),
            thisData->constBegin() + thisSegment.end(),
            thisSegmentData.begin());
  std::copy(otherData->constBegin() + otherSegment.begin(),
            otherData->constBegin() + otherSegment.end(),
            otherSegmentData.begin());
  // pointers to be able to swap them, depending which data range needs
  // cropping:
  QVector<QPointF> *staticData = &thisSegmentData;
  QVector<QPointF> *croppedData = &otherSegmentData;

  // crop both vectors to ranges in which the keys overlap (which coord is key,
  // depends on axisType):
  if (keyAxis->orientation() == Qt::Horizontal) {
    // x is key
    // crop lower bound:
    if (staticData->first().x() <
        croppedData->first().x())  // other one must be cropped
      qSwap(staticData, croppedData);
    const int lowBound = findIndexBelowX(croppedData, staticData->first().x());
    if (lowBound == -1) return QPolygonF();  // key ranges have no overlap
    croppedData->remove(0, lowBound);
    // set lowest point of cropped data to fit exactly key position of first
    // static data point via linear interpolation:
    if (croppedData->size() < 2)
      return QPolygonF();  // need at least two points for interpolation
    double slope;
    if (!qFuzzyCompare(croppedData->at(1).x(), croppedData->at(0).x()))
      slope = (croppedData->at(1).y() - croppedData->at(0).y()) /
              (croppedData->at(1).x() - croppedData->at(0).x());
    else
      slope = 0;
    (*croppedData)[0].setY(
        croppedData->at(0).y() +
        slope * (staticData->first().x() - croppedData->at(0).x()));
    (*croppedData)[0].setX(staticData->first().x());

    // crop upper bound:
    if (staticData->last().x() >
        croppedData->last().x())  // other one must be cropped
      qSwap(staticData, croppedData);
    int highBound = findIndexAboveX(croppedData, staticData->last().x());
    if (highBound == -1) return QPolygonF();  // key ranges have no overlap
    croppedData->remove(highBound + 1, croppedData->size() - (highBound + 1));
    // set highest point of cropped data to fit exactly key position of last
    // static data point via linear interpolation:
    if (croppedData->size() < 2)
      return QPolygonF();  // need at least two points for interpolation
    const int li = croppedData->size() - 1;  // last index
    if (!qFuzzyCompare(croppedData->at(li).x(), croppedData->at(li - 1).x()))
      slope = (croppedData->at(li).y() - croppedData->at(li - 1).y()) /
              (croppedData->at(li).x() - croppedData->at(li - 1).x());
    else
      slope = 0;
    (*croppedData)[li].setY(
        croppedData->at(li - 1).y() +
        slope * (staticData->last().x() - croppedData->at(li - 1).x()));
    (*croppedData)[li].setX(staticData->last().x());
  } else  // mKeyAxis->orientation() == Qt::Vertical
  {
    // y is key
    // crop lower bound:
    if (staticData->first().y() <
        croppedData->first().y())  // other one must be cropped
      qSwap(staticData, croppedData);
    int lowBound = findIndexBelowY(croppedData, staticData->first().y());
    if (lowBound == -1) return QPolygonF();  // key ranges have no overlap
    croppedData->remove(0, lowBound);
    // set lowest point of cropped data to fit exactly key position of first
    // static data point via linear interpolation:
    if (croppedData->size() < 2)
      return QPolygonF();  // need at least two points for interpolation
    double slope;
    if (!qFuzzyCompare(
            croppedData->at(1).y(),
            croppedData->at(0).y()))  // avoid division by zero in step plots
      slope = (croppedData->at(1).x() - croppedData->at(0).x()) /
              (croppedData->at(1).y() - croppedData->at(0).y());
    else
      slope = 0;
    (*croppedData)[0].setX(
        croppedData->at(0).x() +
        slope * (staticData->first().y() - croppedData->at(0).y()));
    (*croppedData)[0].setY(staticData->first().y());

    // crop upper bound:
    if (staticData->last().y() >
        croppedData->last().y())  // other one must be cropped
      qSwap(staticData, croppedData);
    int highBound = findIndexAboveY(croppedData, staticData->last().y());
    if (highBound == -1) return QPolygonF();  // key ranges have no overlap
    croppedData->remove(highBound + 1, croppedData->size() - (highBound + 1));
    // set highest point of cropped data to fit exactly key position of last
    // static data point via linear interpolation:
    if (croppedData->size() < 2)
      return QPolygonF();  // need at least two points for interpolation
    int li = croppedData->size() - 1;  // last index
    if (!qFuzzyCompare(croppedData->at(li).y(),
                       croppedData->at(li - 1)
                           .y()))  // avoid division by zero in step plots
      slope = (croppedData->at(li).x() - croppedData->at(li - 1).x()) /
              (croppedData->at(li).y() - croppedData->at(li - 1).y());
    else
      slope = 0;
    (*croppedData)[li].setX(
        croppedData->at(li - 1).x() +
        slope * (staticData->last().y() - croppedData->at(li - 1).y()));
    (*croppedData)[li].setY(staticData->last().y());
  }

  // return joined:
  for (int i = otherSegmentData.size() - 1; i >= 0;
       --i)  // insert reversed, otherwise the polygon will be twisted
    thisSegmentData << otherSegmentData.at(i);
  return QPolygonF(thisSegmentData);
}

/*! \internal

  Finds the smallest index of \a data, whose points x value is just above \a x.
  Assumes x values in \a data points are ordered ascending, as is ensured by
  \ref getLines/\ref getScatters if the key axis is horizontal.

  Used to calculate the channel fill polygon, see \ref getChannelFillPolygon.
*/
int QCPGraph::findIndexAboveX(const QVector<QPointF> *data, double x) const {
  for (int i = data->size() - 1; i >= 0; --i) {
    if (data->at(i).x() < x) {
      if (i < data->size() - 1)
        return i + 1;
      else
        return data->size() - 1;
    }
  }
  return -1;
}

/*! \internal

  Finds the highest index of \a data, whose points x value is just below \a x.
  Assumes x values in \a data points are ordered ascending, as is ensured by
  \ref getLines/\ref getScatters if the key axis is horizontal.

  Used to calculate the channel fill polygon, see \ref getChannelFillPolygon.
*/
int QCPGraph::findIndexBelowX(const QVector<QPointF> *data, double x) const {
  for (int i = 0; i < data->size(); ++i) {
    if (data->at(i).x() > x) {
      if (i > 0)
        return i - 1;
      else
        return 0;
    }
  }
  return -1;
}

/*! \internal

  Finds the smallest index of \a data, whose points y value is just above \a y.
  Assumes y values in \a data points are ordered ascending, as is ensured by
  \ref getLines/\ref getScatters if the key axis is vertical.

  Used to calculate the channel fill polygon, see \ref getChannelFillPolygon.
*/
int QCPGraph::findIndexAboveY(const QVector<QPointF> *data, double y) const {
  for (int i = data->size() - 1; i >= 0; --i) {
    if (data->at(i).y() < y) {
      if (i < data->size() - 1)
        return i + 1;
      else
        return data->size() - 1;
    }
  }
  return -1;
}

/*! \internal

  Calculates the minimum distance in pixels the graph's representation has from
  the given \a pixelPoint. This is used to determine whether the graph was
  clicked or not, e.g. in \ref selectTest. The closest data point to \a
  pixelPoint is returned in \a closestData. Note that if the graph has a line
  representation, the returned distance may be smaller than the distance to the
  \a closestData point, since the distance to the graph line is also taken into
  account.

  If either the graph has no data or if the line style is \ref lsNone and the
  scatter style's shape is \ref QCPScatterStyle::ssNone (i.e. there is no visual
  representation of the graph), returns -1.0.
*/
double QCPGraph::pointDistance(
    const QPointF &pixelPoint,
    QCPGraphDataContainer::const_iterator &closestData) const {
  closestData = mDataContainer->constEnd();
  if (mDataContainer->isEmpty()) return -1.0;
  if (mLineStyle == lsNone && mScatterStyle.isNone()) return -1.0;

  // calculate minimum distances to graph data points and find closestData
  // iterator:
  double minDistSqr = (std::numeric_limits<double>::max)();
  // determine which key range comes into question, taking selection tolerance
  // around pos into account:
  double posKeyMin, posKeyMax, dummy;
  pixelsToCoords(pixelPoint - QPointF(mParentPlot->selectionTolerance(),
                                      mParentPlot->selectionTolerance()),
                 posKeyMin, dummy);
  pixelsToCoords(pixelPoint + QPointF(mParentPlot->selectionTolerance(),
                                      mParentPlot->selectionTolerance()),
                 posKeyMax, dummy);
  if (posKeyMin > posKeyMax) qSwap(posKeyMin, posKeyMax);
  // iterate over found data points and then choose the one with the shortest
  // distance to pos:
  QCPGraphDataContainer::const_iterator begin =
      mDataContainer->findBegin(posKeyMin, true);
  QCPGraphDataContainer::const_iterator end =
      mDataContainer->findEnd(posKeyMax, true);
  for (QCPGraphDataContainer::const_iterator it = begin; it != end; ++it) {
    const double currentDistSqr =
        QCPVector2D(coordsToPixels(it->key, it->value) - pixelPoint)
            .lengthSquared();
    if (currentDistSqr < minDistSqr) {
      minDistSqr = currentDistSqr;
      closestData = it;
    }
  }

  // calculate distance to graph line if there is one (if so, will probably be
  // smaller than distance to closest data point):
  if (mLineStyle != lsNone) {
    // line displayed, calculate distance to line segments:
    QVector<QPointF> lineData;
    getLines(
        &lineData,
        QCPDataRange(
            0,
            dataCount()));  // don't limit data range further since with sharp
                            // data spikes, line segments may be closer to test
                            // point than segments with closer key coordinate
    QCPVector2D p(pixelPoint);
    const int step =
        mLineStyle == lsImpulse
            ? 2
            : 1;  // impulse plot differs from other line styles in that the
                  // lineData points are only pairwise connected
    for (int i = 0; i < lineData.size() - 1; i += step) {
      const double currentDistSqr =
          p.distanceSquaredToLine(lineData.at(i), lineData.at(i + 1));
      if (currentDistSqr < minDistSqr) minDistSqr = currentDistSqr;
    }
  }

  return qSqrt(minDistSqr);
}

/*! \internal

  Finds the highest index of \a data, whose points y value is just below \a y.
  Assumes y values in \a data points are ordered ascending, as is ensured by
  \ref getLines/\ref getScatters if the key axis is vertical.

  Used to calculate the channel fill polygon, see \ref getChannelFillPolygon.
*/
int QCPGraph::findIndexBelowY(const QVector<QPointF> *data, double y) const {
  for (int i = 0; i < data->size(); ++i) {
    if (data->at(i).y() > y) {
      if (i > 0)
        return i - 1;
      else
        return 0;
    }
  }
  return -1;
}
/* end of 'src/plottables/plottable-graph.cpp' */

/* including file 'src/plottables/plottable-curve.cpp' */
/* modified 2022-11-06T12:45:56, size 63851            */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPCurveData
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPCurveData
  \brief Holds the data of one single data point for QCPCurve.

  The stored data is:
  \li \a t: the free ordering parameter of this curve point, like in the
  mathematical vector <em>(x(t), y(t))</em>. (This is the \a sortKey) \li \a
  key: coordinate on the key axis of this curve point (this is the \a mainKey)
  \li \a value: coordinate on the value axis of this curve point (this is the \a
  mainValue)

  The container for storing multiple data points is \ref QCPCurveDataContainer.
  It is a typedef for \ref QCPDataContainer with \ref QCPCurveData as the
  DataType template parameter. See the documentation there for an explanation
  regarding the data type's generic methods.

  \see QCPCurveDataContainer
*/

/* start documentation of inline functions */

/*! \fn double QCPCurveData::sortKey() const

  Returns the \a t member of this data point.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/*! \fn static QCPCurveData QCPCurveData::fromSortKey(double sortKey)

  Returns a data point with the specified \a sortKey (assigned to the data
  point's \a t member). All other members are set to zero.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/*! \fn static static bool QCPCurveData::sortKeyIsMainKey()

  Since the member \a key is the data point key coordinate and the member \a t
  is the data ordering parameter, this method returns false.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/*! \fn double QCPCurveData::mainKey() const

  Returns the \a key member of this data point.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/*! \fn double QCPCurveData::mainValue() const

  Returns the \a value member of this data point.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/*! \fn QCPRange QCPCurveData::valueRange() const

  Returns a QCPRange with both lower and upper boundary set to \a value of this
  data point.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/* end documentation of inline functions */

/*!
  Constructs a curve data point with t, key and value set to zero.
*/
QCPCurveData::QCPCurveData() : t(0), key(0), value(0) {}

/*!
  Constructs a curve data point with the specified \a t, \a key and \a value.
*/
QCPCurveData::QCPCurveData(double t, double key, double value)
    : t(t), key(key), value(value) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPCurve
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPCurve
  \brief A plottable representing a parametric curve in a plot.

  \image html QCPCurve.png

  Unlike QCPGraph, plottables of this type may have multiple points with the
  same key coordinate, so their visual representation can have \a loops. This is
  realized by introducing a third coordinate \a t, which defines the order of
  the points described by the other two coordinates \a x and \a y.

  To plot data, assign it with the \ref setData or \ref addData functions.
  Alternatively, you can also access and modify the curve's data via the \ref
  data method, which returns a pointer to the internal \ref
  QCPCurveDataContainer.

  Gaps in the curve can be created by adding data points with NaN as key and
  value
  (<tt>qQNaN()</tt> or <tt>std::numeric_limits<double>::quiet_NaN()</tt>) in
  between the two data points that shall be separated.

  \section qcpcurve-appearance Changing the appearance

  The appearance of the curve is determined by the pen and the brush (\ref
  setPen, \ref setBrush).

  \section qcpcurve-usage Usage

  Like all data representing objects in QCustomPlot, the QCPCurve is a plottable
  (QCPAbstractPlottable). So the plottable-interface of QCustomPlot applies
  (QCustomPlot::plottable, QCustomPlot::removePlottable, etc.)

  Usually, you first create an instance:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpcurve-creation-1
  which registers it with the QCustomPlot instance of the passed axes. Note that
  this QCustomPlot instance takes ownership of the plottable, so do not delete
  it manually but use QCustomPlot::removePlottable() instead. The newly created
  plottable can be modified, e.g.: \snippet
  documentation/doc-code-snippets/mainwindow.cpp qcpcurve-creation-2
*/

/* start of documentation of inline functions */

/*! \fn QSharedPointer<QCPCurveDataContainer> QCPCurve::data() const

  Returns a shared pointer to the internal data storage of type \ref
  QCPCurveDataContainer. You may use it to directly manipulate the data, which
  may be more convenient and faster than using the regular \ref setData or \ref
  addData methods.
*/

/* end of documentation of inline functions */

/*!
  Constructs a curve which uses \a keyAxis as its key axis ("x") and \a
  valueAxis as its value axis ("y"). \a keyAxis and \a valueAxis must reside in
  the same QCustomPlot instance and not have the same orientation. If either of
  these restrictions is violated, a corresponding message is printed to the
  debug output (qDebug), the construction is not aborted, though.

  The created QCPCurve is automatically registered with the QCustomPlot instance
  inferred from \a keyAxis. This QCustomPlot instance takes ownership of the
  QCPCurve, so do not delete it manually but use QCustomPlot::removePlottable()
  instead.
*/
QCPCurve::QCPCurve(QCPAxis *keyAxis, QCPAxis *valueAxis)
    : QCPAbstractPlottable1D<QCPCurveData>(keyAxis, valueAxis),
      mScatterSkip{},
      mLineStyle{} {
  // modify inherited properties from abstract plottable:
  setPen(QPen(Qt::blue, 0));
  setBrush(Qt::NoBrush);

  setScatterStyle(QCPScatterStyle());
  setLineStyle(lsLine);
  setScatterSkip(0);
}

QCPCurve::~QCPCurve() {}

/*! \overload

  Replaces the current data container with the provided \a data container.

  Since a QSharedPointer is used, multiple QCPCurves may share the same data
  container safely. Modifying the data in the container will then affect all
  curves that share the container. Sharing can be achieved by simply exchanging
  the data containers wrapped in shared pointers: \snippet
  documentation/doc-code-snippets/mainwindow.cpp qcpcurve-datasharing-1

  If you do not wish to share containers, but create a copy from an existing
  container, rather use the \ref QCPDataContainer<DataType>::set method on the
  curve's data container directly: \snippet
  documentation/doc-code-snippets/mainwindow.cpp qcpcurve-datasharing-2

  \see addData
*/
void QCPCurve::setData(QSharedPointer<QCPCurveDataContainer> data) {
  mDataContainer = data;
}

/*! \overload

  Replaces the current data with the provided points in \a t, \a keys and \a
  values. The provided vectors should have equal length. Else, the number of
  added points will be the size of the smallest vector.

  If you can guarantee that the passed data points are sorted by \a t in
  ascending order, you can set \a alreadySorted to true, to improve performance
  by saving a sorting run.

  \see addData
*/
void QCPCurve::setData(const QVector<double> &t, const QVector<double> &keys,
                       const QVector<double> &values, bool alreadySorted) {
  mDataContainer->clear();
  addData(t, keys, values, alreadySorted);
}

/*! \overload

  Replaces the current data with the provided points in \a keys and \a values.
  The provided vectors should have equal length. Else, the number of added
  points will be the size of the smallest vector.

  The t parameter of each data point will be set to the integer index of the
  respective key/value pair.

  \see addData
*/
void QCPCurve::setData(const QVector<double> &keys,
                       const QVector<double> &values) {
  mDataContainer->clear();
  addData(keys, values);
}

/*!
  Sets the visual appearance of single data points in the plot. If set to \ref
  QCPScatterStyle::ssNone, no scatter points are drawn (e.g. for line-only plots
  with appropriate line style).

  \see QCPScatterStyle, setLineStyle
*/
void QCPCurve::setScatterStyle(const QCPScatterStyle &style) {
  mScatterStyle = style;
}

/*!
  If scatters are displayed (scatter style not \ref QCPScatterStyle::ssNone), \a
  skip number of scatter points are skipped/not drawn after every drawn scatter
  point.

  This can be used to make the data appear sparser while for example still
  having a smooth line, and to improve performance for very high density plots.

  If \a skip is set to 0 (default), all scatter points are drawn.

  \see setScatterStyle
*/
void QCPCurve::setScatterSkip(int skip) { mScatterSkip = qMax(0, skip); }

/*!
  Sets how the single data points are connected in the plot or how they are
  represented visually apart from the scatter symbol. For scatter-only plots,
  set \a style to \ref lsNone and \ref setScatterStyle to the desired scatter
  style.

  \see setScatterStyle
*/
void QCPCurve::setLineStyle(QCPCurve::LineStyle style) { mLineStyle = style; }

/*! \overload

  Adds the provided points in \a t, \a keys and \a values to the current data.
  The provided vectors should have equal length. Else, the number of added
  points will be the size of the smallest vector.

  If you can guarantee that the passed data points are sorted by \a keys in
  ascending order, you can set \a alreadySorted to true, to improve performance
  by saving a sorting run.

  Alternatively, you can also access and modify the data directly via the \ref
  data method, which returns a pointer to the internal data container.
*/
void QCPCurve::addData(const QVector<double> &t, const QVector<double> &keys,
                       const QVector<double> &values, bool alreadySorted) {
  if (t.size() != keys.size() || t.size() != values.size())
    qDebug() << Q_FUNC_INFO
             << "ts, keys and values have different sizes:" << t.size()
             << keys.size() << values.size();
  const int n = qMin(qMin(t.size(), keys.size()), values.size());
  QVector<QCPCurveData> tempData(n);
  QVector<QCPCurveData>::iterator it = tempData.begin();
  const QVector<QCPCurveData>::iterator itEnd = tempData.end();
  int i = 0;
  while (it != itEnd) {
    it->t = t[i];
    it->key = keys[i];
    it->value = values[i];
    ++it;
    ++i;
  }
  mDataContainer->add(tempData,
                      alreadySorted);  // don't modify tempData beyond this to
                                       // prevent copy on write
}

/*! \overload

  Adds the provided points in \a keys and \a values to the current data. The
  provided vectors should have equal length. Else, the number of added points
  will be the size of the smallest vector.

  The t parameter of each data point will be set to the integer index of the
  respective key/value pair.

  Alternatively, you can also access and modify the data directly via the \ref
  data method, which returns a pointer to the internal data container.
*/
void QCPCurve::addData(const QVector<double> &keys,
                       const QVector<double> &values) {
  if (keys.size() != values.size())
    qDebug() << Q_FUNC_INFO
             << "keys and values have different sizes:" << keys.size()
             << values.size();
  const int n = qMin(keys.size(), values.size());
  double tStart;
  if (!mDataContainer->isEmpty())
    tStart = (mDataContainer->constEnd() - 1)->t + 1.0;
  else
    tStart = 0;
  QVector<QCPCurveData> tempData(n);
  QVector<QCPCurveData>::iterator it = tempData.begin();
  const QVector<QCPCurveData>::iterator itEnd = tempData.end();
  int i = 0;
  while (it != itEnd) {
    it->t = tStart + i;
    it->key = keys[i];
    it->value = values[i];
    ++it;
    ++i;
  }
  mDataContainer->add(
      tempData,
      true);  // don't modify tempData beyond this to prevent copy on write
}

/*! \overload
  Adds the provided data point as \a t, \a key and \a value to the current data.

  Alternatively, you can also access and modify the data directly via the \ref
  data method, which returns a pointer to the internal data container.
*/
void QCPCurve::addData(double t, double key, double value) {
  mDataContainer->add(QCPCurveData(t, key, value));
}

/*! \overload

  Adds the provided data point as \a key and \a value to the current data.

  The t parameter is generated automatically by increments of 1 for each point,
  starting at the highest t of previously existing data or 0, if the curve data
  is empty.

  Alternatively, you can also access and modify the data directly via the \ref
  data method, which returns a pointer to the internal data container.
*/
void QCPCurve::addData(double key, double value) {
  if (!mDataContainer->isEmpty())
    mDataContainer->add(
        QCPCurveData((mDataContainer->constEnd() - 1)->t + 1.0, key, value));
  else
    mDataContainer->add(QCPCurveData(0.0, key, value));
}

/*!
  Implements a selectTest specific to this plottable's point geometry.

  If \a details is not 0, it will be set to a \ref QCPDataSelection, describing
  the closest data point to \a pos.

  \seebaseclassmethod \ref QCPAbstractPlottable::selectTest
*/
double QCPCurve::selectTest(const QPointF &pos, bool onlySelectable,
                            QVariant *details) const {
  if ((onlySelectable && mSelectable == QCP::stNone) ||
      mDataContainer->isEmpty())
    return -1;
  if (!mKeyAxis || !mValueAxis) return -1;

  if (mKeyAxis.data()->axisRect()->rect().contains(pos.toPoint()) ||
      mParentPlot->interactions().testFlag(
          QCP::iSelectPlottablesBeyondAxisRect)) {
    QCPCurveDataContainer::const_iterator closestDataPoint =
        mDataContainer->constEnd();
    double result = pointDistance(pos, closestDataPoint);
    if (details) {
      int pointIndex = int(closestDataPoint - mDataContainer->constBegin());
      details->setValue(
          QCPDataSelection(QCPDataRange(pointIndex, pointIndex + 1)));
    }
    return result;
  } else
    return -1;
}

/* inherits documentation from base class */
QCPRange QCPCurve::getKeyRange(bool &foundRange,
                               QCP::SignDomain inSignDomain) const {
  return mDataContainer->keyRange(foundRange, inSignDomain);
}

/* inherits documentation from base class */
QCPRange QCPCurve::getValueRange(bool &foundRange, QCP::SignDomain inSignDomain,
                                 const QCPRange &inKeyRange) const {
  return mDataContainer->valueRange(foundRange, inSignDomain, inKeyRange);
}

/* inherits documentation from base class */
void QCPCurve::draw(QCPPainter *painter) {
  if (mDataContainer->isEmpty()) return;

  // allocate line vector:
  QVector<QPointF> lines, scatters;

  // loop over and draw segments of unselected/selected data:
  QList<QCPDataRange> selectedSegments, unselectedSegments, allSegments;
  getDataSegments(selectedSegments, unselectedSegments);
  allSegments << unselectedSegments << selectedSegments;
  for (int i = 0; i < allSegments.size(); ++i) {
    bool isSelectedSegment = i >= unselectedSegments.size();

    // fill with curve data:
    QPen finalCurvePen =
        mPen;  // determine the final pen already here, because the line
               // optimization depends on its stroke width
    if (isSelectedSegment && mSelectionDecorator)
      finalCurvePen = mSelectionDecorator->pen();

    QCPDataRange lineDataRange =
        isSelectedSegment
            ? allSegments.at(i)
            : allSegments.at(i).adjusted(
                  -1,
                  1);  // unselected segments extend lines to bordering selected
                       // data point (safe to exceed total data bounds in
                       // first/last segment, getCurveLines takes care)
    getCurveLines(&lines, lineDataRange, finalCurvePen.widthF());

// check data validity if flag set:
#ifdef QCUSTOMPLOT_CHECK_DATA
    for (QCPCurveDataContainer::const_iterator it =
             mDataContainer->constBegin();
         it != mDataContainer->constEnd(); ++it) {
      if (QCP::isInvalidData(it->t) || QCP::isInvalidData(it->key, it->value))
        qDebug() << Q_FUNC_INFO << "Data point at" << it->key << "invalid."
                 << "Plottable name:" << name();
    }
#endif

    // draw curve fill:
    applyFillAntialiasingHint(painter);
    if (isSelectedSegment && mSelectionDecorator)
      mSelectionDecorator->applyBrush(painter);
    else
      painter->setBrush(mBrush);
    painter->setPen(Qt::NoPen);
    if (painter->brush().style() != Qt::NoBrush &&
        painter->brush().color().alpha() != 0)
      painter->drawPolygon(QPolygonF(lines));

    // draw curve line:
    if (mLineStyle != lsNone) {
      painter->setPen(finalCurvePen);
      painter->setBrush(Qt::NoBrush);
      drawCurveLine(painter, lines);
    }

    // draw scatters:
    QCPScatterStyle finalScatterStyle = mScatterStyle;
    if (isSelectedSegment && mSelectionDecorator)
      finalScatterStyle =
          mSelectionDecorator->getFinalScatterStyle(mScatterStyle);
    if (!finalScatterStyle.isNone()) {
      getScatters(&scatters, allSegments.at(i), finalScatterStyle.size());
      drawScatterPlot(painter, scatters, finalScatterStyle);
    }
  }

  // draw other selection decoration that isn't just line/scatter pens and
  // brushes:
  if (mSelectionDecorator)
    mSelectionDecorator->drawDecoration(painter, selection());
}

/* inherits documentation from base class */
void QCPCurve::drawLegendIcon(QCPPainter *painter, const QRectF &rect) const {
  // draw fill:
  if (mBrush.style() != Qt::NoBrush) {
    applyFillAntialiasingHint(painter);
    painter->fillRect(QRectF(rect.left(), rect.top() + rect.height() / 2.0,
                             rect.width(), rect.height() / 3.0),
                      mBrush);
  }
  // draw line vertically centered:
  if (mLineStyle != lsNone) {
    applyDefaultAntialiasingHint(painter);
    painter->setPen(mPen);
    painter->drawLine(QLineF(
        rect.left(), rect.top() + rect.height() / 2.0, rect.right() + 5,
        rect.top() + rect.height() / 2.0));  // +5 on x2 else last segment is
                                             // missing from dashed/dotted pens
  }
  // draw scatter symbol:
  if (!mScatterStyle.isNone()) {
    applyScattersAntialiasingHint(painter);
    // scale scatter pixmap if it's too large to fit in legend icon rect:
    if (mScatterStyle.shape() == QCPScatterStyle::ssPixmap &&
        (mScatterStyle.pixmap().size().width() > rect.width() ||
         mScatterStyle.pixmap().size().height() > rect.height())) {
      QCPScatterStyle scaledStyle(mScatterStyle);
      scaledStyle.setPixmap(scaledStyle.pixmap().scaled(
          rect.size().toSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
      scaledStyle.applyTo(painter, mPen);
      scaledStyle.drawShape(painter, QRectF(rect).center());
    } else {
      mScatterStyle.applyTo(painter, mPen);
      mScatterStyle.drawShape(painter, QRectF(rect).center());
    }
  }
}

/*!  \internal

  Draws lines between the points in \a lines, given in pixel coordinates.

  \see drawScatterPlot, getCurveLines
*/
void QCPCurve::drawCurveLine(QCPPainter *painter,
                             const QVector<QPointF> &lines) const {
  if (painter->pen().style() != Qt::NoPen &&
      painter->pen().color().alpha() != 0) {
    applyDefaultAntialiasingHint(painter);
    drawPolyline(painter, lines);
  }
}

/*! \internal

  Draws scatter symbols at every point passed in \a points, given in pixel
  coordinates. The scatters will be drawn with \a painter and have the
  appearance as specified in \a style.

  \see drawCurveLine, getCurveLines
*/
void QCPCurve::drawScatterPlot(QCPPainter *painter,
                               const QVector<QPointF> &points,
                               const QCPScatterStyle &style) const {
  // draw scatter point symbols:
  applyScattersAntialiasingHint(painter);
  style.applyTo(painter, mPen);
  foreach (const QPointF &point, points)
    if (!qIsNaN(point.x()) && !qIsNaN(point.y()))
      style.drawShape(painter, point);
}

/*! \internal

  Called by \ref draw to generate points in pixel coordinates which represent
  the line of the curve.

  Line segments that aren't visible in the current axis rect are handled in an
  optimized way. They are projected onto a rectangle slightly larger than the
  visible axis rect and simplified regarding point count. The algorithm makes
  sure to preserve appearance of lines and fills inside the visible axis rect by
  generating new temporary points on the outer rect if necessary.

  \a lines will be filled with points in pixel coordinates, that can be drawn
  with \ref drawCurveLine.

  \a dataRange specifies the beginning and ending data indices that will be
  taken into account for conversion. In this function, the specified range may
  exceed the total data bounds without harm: a correspondingly trimmed data
  range will be used. This takes the burden off the user of this function to
  check for valid indices in \a dataRange, e.g. when extending ranges coming
  from \ref getDataSegments.

  \a penWidth specifies the pen width that will be used to later draw the lines
  generated by this function. This is needed here to calculate an accordingly
  wider margin around the axis rect when performing the line optimization.

  Methods that are also involved in the algorithm are: \ref getRegion, \ref
  getOptimizedPoint, \ref getOptimizedCornerPoints \ref mayTraverse, \ref
  getTraverse, \ref getTraverseCornerPoints.

  \see drawCurveLine, drawScatterPlot
*/
void QCPCurve::getCurveLines(QVector<QPointF> *lines,
                             const QCPDataRange &dataRange,
                             double penWidth) const {
  if (!lines) return;
  lines->clear();
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    return;
  }

  // add margins to rect to compensate for stroke width
  const double strokeMargin =
      qMax(qreal(1.0), qreal(penWidth * 0.75));  // stroke radius + 50% safety
  const double keyMin =
      keyAxis->pixelToCoord(keyAxis->coordToPixel(keyAxis->range().lower) -
                            strokeMargin * keyAxis->pixelOrientation());
  const double keyMax =
      keyAxis->pixelToCoord(keyAxis->coordToPixel(keyAxis->range().upper) +
                            strokeMargin * keyAxis->pixelOrientation());
  const double valueMin = valueAxis->pixelToCoord(
      valueAxis->coordToPixel(valueAxis->range().lower) -
      strokeMargin * valueAxis->pixelOrientation());
  const double valueMax = valueAxis->pixelToCoord(
      valueAxis->coordToPixel(valueAxis->range().upper) +
      strokeMargin * valueAxis->pixelOrientation());
  QCPCurveDataContainer::const_iterator itBegin = mDataContainer->constBegin();
  QCPCurveDataContainer::const_iterator itEnd = mDataContainer->constEnd();
  mDataContainer->limitIteratorsToDataRange(itBegin, itEnd, dataRange);
  if (itBegin == itEnd) return;
  QCPCurveDataContainer::const_iterator it = itBegin;
  QCPCurveDataContainer::const_iterator prevIt = itEnd - 1;
  int prevRegion =
      getRegion(prevIt->key, prevIt->value, keyMin, valueMax, keyMax, valueMin);
  QVector<QPointF>
      trailingPoints;  // points that must be applied after all other points
                       // (are generated only when handling first point to get
                       // virtual segment between last and first point right)
  while (it != itEnd) {
    const int currentRegion =
        getRegion(it->key, it->value, keyMin, valueMax, keyMax, valueMin);
    if (currentRegion !=
        prevRegion)  // changed region, possibly need to add some optimized edge
    // points or original points if entering R
    {
      if (currentRegion !=
          5)  // segment doesn't end in R, so it's a candidate for removal
      {
        QPointF crossA, crossB;
        if (prevRegion ==
            5)  // we're coming from R, so add this point optimized
        {
          lines->append(getOptimizedPoint(currentRegion, it->key, it->value,
                                          prevIt->key, prevIt->value, keyMin,
                                          valueMax, keyMax, valueMin));
          // in the situations 5->1/7/9/3 the segment may leave R and directly
          // cross through two outer regions. In these cases we need to add an
          // additional corner point
          *lines << getOptimizedCornerPoints(
              prevRegion, currentRegion, prevIt->key, prevIt->value, it->key,
              it->value, keyMin, valueMax, keyMax, valueMin);
        } else if (mayTraverse(prevRegion, currentRegion) &&
                   getTraverse(prevIt->key, prevIt->value, it->key, it->value,
                               keyMin, valueMax, keyMax, valueMin, crossA,
                               crossB)) {
          // add the two cross points optimized if segment crosses R and if
          // segment isn't virtual zeroth segment between last and first curve
          // point:
          QVector<QPointF> beforeTraverseCornerPoints,
              afterTraverseCornerPoints;
          getTraverseCornerPoints(prevRegion, currentRegion, keyMin, valueMax,
                                  keyMax, valueMin, beforeTraverseCornerPoints,
                                  afterTraverseCornerPoints);
          if (it != itBegin) {
            *lines << beforeTraverseCornerPoints;
            lines->append(crossA);
            lines->append(crossB);
            *lines << afterTraverseCornerPoints;
          } else {
            lines->append(crossB);
            *lines << afterTraverseCornerPoints;
            trailingPoints << beforeTraverseCornerPoints << crossA;
          }
        } else  // doesn't cross R, line is just moving around in outside
        // regions, so only need to add optimized point(s) at the
        // boundary corner(s)
        {
          *lines << getOptimizedCornerPoints(
              prevRegion, currentRegion, prevIt->key, prevIt->value, it->key,
              it->value, keyMin, valueMax, keyMax, valueMin);
        }
      } else  // segment does end in R, so we add previous point optimized and
      // this point at original position
      {
        if (it == itBegin)  // it is first point in curve and prevIt is last
          // one. So save optimized point for adding it to the
          // lineData in the end
          trailingPoints << getOptimizedPoint(
              prevRegion, prevIt->key, prevIt->value, it->key, it->value,
              keyMin, valueMax, keyMax, valueMin);
        else
          lines->append(getOptimizedPoint(prevRegion, prevIt->key,
                                          prevIt->value, it->key, it->value,
                                          keyMin, valueMax, keyMax, valueMin));
        lines->append(coordsToPixels(it->key, it->value));
      }
    } else  // region didn't change
    {
      if (currentRegion == 5)  // still in R, keep adding original points
      {
        lines->append(coordsToPixels(it->key, it->value));
      } else  // still outside R, no need to add anything
      {
        // see how this is not doing anything? That's the main optimization...
      }
    }
    prevIt = it;
    prevRegion = currentRegion;
    ++it;
  }
  *lines << trailingPoints;
}

/*! \internal

  Called by \ref draw to generate points in pixel coordinates which represent
  the scatters of the curve. If a scatter skip is configured (\ref
  setScatterSkip), the returned points are accordingly sparser.

  Scatters that aren't visible in the current axis rect are optimized away.

  \a scatters will be filled with points in pixel coordinates, that can be drawn
  with \ref drawScatterPlot.

  \a dataRange specifies the beginning and ending data indices that will be
  taken into account for conversion.

  \a scatterWidth specifies the scatter width that will be used to later draw
  the scatters at pixel coordinates generated by this function. This is needed
  here to calculate an accordingly wider margin around the axis rect when
  performing the data point reduction.

  \see draw, drawScatterPlot
*/
void QCPCurve::getScatters(QVector<QPointF> *scatters,
                           const QCPDataRange &dataRange,
                           double scatterWidth) const {
  if (!scatters) return;
  scatters->clear();
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    return;
  }

  QCPCurveDataContainer::const_iterator begin = mDataContainer->constBegin();
  QCPCurveDataContainer::const_iterator end = mDataContainer->constEnd();
  mDataContainer->limitIteratorsToDataRange(begin, end, dataRange);
  if (begin == end) return;
  const int scatterModulo = mScatterSkip + 1;
  const bool doScatterSkip = mScatterSkip > 0;
  int endIndex = int(end - mDataContainer->constBegin());

  QCPRange keyRange = keyAxis->range();
  QCPRange valueRange = valueAxis->range();
  // extend range to include width of scatter symbols:
  keyRange.lower =
      keyAxis->pixelToCoord(keyAxis->coordToPixel(keyRange.lower) -
                            scatterWidth * keyAxis->pixelOrientation());
  keyRange.upper =
      keyAxis->pixelToCoord(keyAxis->coordToPixel(keyRange.upper) +
                            scatterWidth * keyAxis->pixelOrientation());
  valueRange.lower =
      valueAxis->pixelToCoord(valueAxis->coordToPixel(valueRange.lower) -
                              scatterWidth * valueAxis->pixelOrientation());
  valueRange.upper =
      valueAxis->pixelToCoord(valueAxis->coordToPixel(valueRange.upper) +
                              scatterWidth * valueAxis->pixelOrientation());

  QCPCurveDataContainer::const_iterator it = begin;
  int itIndex = int(begin - mDataContainer->constBegin());
  while (doScatterSkip && it != end &&
         itIndex % scatterModulo !=
             0)  // advance begin iterator to first non-skipped scatter
  {
    ++itIndex;
    ++it;
  }
  if (keyAxis->orientation() == Qt::Vertical) {
    while (it != end) {
      if (!qIsNaN(it->value) && keyRange.contains(it->key) &&
          valueRange.contains(it->value))
        scatters->append(QPointF(valueAxis->coordToPixel(it->value),
                                 keyAxis->coordToPixel(it->key)));

      // advance iterator to next (non-skipped) data point:
      if (!doScatterSkip)
        ++it;
      else {
        itIndex += scatterModulo;
        if (itIndex < endIndex)  // make sure we didn't jump over end
          it += scatterModulo;
        else {
          it = end;
          itIndex = endIndex;
        }
      }
    }
  } else {
    while (it != end) {
      if (!qIsNaN(it->value) && keyRange.contains(it->key) &&
          valueRange.contains(it->value))
        scatters->append(QPointF(keyAxis->coordToPixel(it->key),
                                 valueAxis->coordToPixel(it->value)));

      // advance iterator to next (non-skipped) data point:
      if (!doScatterSkip)
        ++it;
      else {
        itIndex += scatterModulo;
        if (itIndex < endIndex)  // make sure we didn't jump over end
          it += scatterModulo;
        else {
          it = end;
          itIndex = endIndex;
        }
      }
    }
  }
}

/*! \internal

  This function is part of the curve optimization algorithm of \ref
  getCurveLines.

  It returns the region of the given point (\a key, \a value) with respect to a
  rectangle defined by \a keyMin, \a keyMax, \a valueMin, and \a valueMax.

  The regions are enumerated from top to bottom (\a valueMin to \a valueMax) and
  left to right (\a keyMin to \a keyMax):

  <table style="width:10em; text-align:center">
    <tr><td>1</td><td>4</td><td>7</td></tr>
    <tr><td>2</td><td style="border:1px solid black">5</td><td>8</td></tr>
    <tr><td>3</td><td>6</td><td>9</td></tr>
  </table>

  With the rectangle being region 5, and the outer regions extending infinitely
  outwards. In the curve optimization algorithm, region 5 is considered to be
  the visible portion of the plot.
*/
int QCPCurve::getRegion(double key, double value, double keyMin,
                        double valueMax, double keyMax, double valueMin) const {
  if (key < keyMin)  // region 123
  {
    if (value > valueMax)
      return 1;
    else if (value < valueMin)
      return 3;
    else
      return 2;
  } else if (key > keyMax)  // region 789
  {
    if (value > valueMax)
      return 7;
    else if (value < valueMin)
      return 9;
    else
      return 8;
  } else  // region 456
  {
    if (value > valueMax)
      return 4;
    else if (value < valueMin)
      return 6;
    else
      return 5;
  }
}

/*! \internal

  This function is part of the curve optimization algorithm of \ref
  getCurveLines.

  This method is used in case the current segment passes from inside the visible
  rect (region 5, see \ref getRegion) to any of the outer regions (\a
  otherRegion). The current segment is given by the line connecting (\a key, \a
  value) with (\a otherKey, \a otherValue).

  It returns the intersection point of the segment with the border of region 5.

  For this function it doesn't matter whether (\a key, \a value) is the point
  inside region 5 or whether it's (\a otherKey, \a otherValue), i.e. whether the
  segment is coming from region 5 or leaving it. It is important though that \a
  otherRegion correctly identifies the other region not equal to 5.
*/
QPointF QCPCurve::getOptimizedPoint(int otherRegion, double otherKey,
                                    double otherValue, double key, double value,
                                    double keyMin, double valueMax,
                                    double keyMax, double valueMin) const {
  // The intersection point interpolation here is done in pixel coordinates, so
  // we don't need to differentiate between different axis scale types. Note
  // that the nomenclature top/left/bottom/right/min/max is with respect to the
  // rect in plot coordinates, wich may be different in pixel coordinates
  // (horz/vert key axes, reversed ranges)

  const double keyMinPx = mKeyAxis->coordToPixel(keyMin);
  const double keyMaxPx = mKeyAxis->coordToPixel(keyMax);
  const double valueMinPx = mValueAxis->coordToPixel(valueMin);
  const double valueMaxPx = mValueAxis->coordToPixel(valueMax);
  const double otherValuePx = mValueAxis->coordToPixel(otherValue);
  const double valuePx = mValueAxis->coordToPixel(value);
  const double otherKeyPx = mKeyAxis->coordToPixel(otherKey);
  const double keyPx = mKeyAxis->coordToPixel(key);
  double intersectKeyPx = keyMinPx;      // initial key just a fail-safe
  double intersectValuePx = valueMinPx;  // initial value just a fail-safe
  switch (otherRegion) {
    case 1:  // top and left edge
    {
      intersectValuePx = valueMaxPx;
      intersectKeyPx = otherKeyPx + (keyPx - otherKeyPx) /
                                        (valuePx - otherValuePx) *
                                        (intersectValuePx - otherValuePx);
      if (intersectKeyPx < qMin(keyMinPx, keyMaxPx) ||
          intersectKeyPx >
              qMax(keyMinPx,
                   keyMaxPx))  // check whether top edge is not intersected,
      // then it must be left edge (qMin/qMax necessary
      // since axes may be reversed)
      {
        intersectKeyPx = keyMinPx;
        intersectValuePx = otherValuePx + (valuePx - otherValuePx) /
                                              (keyPx - otherKeyPx) *
                                              (intersectKeyPx - otherKeyPx);
      }
      break;
    }
    case 2:  // left edge
    {
      intersectKeyPx = keyMinPx;
      intersectValuePx = otherValuePx + (valuePx - otherValuePx) /
                                            (keyPx - otherKeyPx) *
                                            (intersectKeyPx - otherKeyPx);
      break;
    }
    case 3:  // bottom and left edge
    {
      intersectValuePx = valueMinPx;
      intersectKeyPx = otherKeyPx + (keyPx - otherKeyPx) /
                                        (valuePx - otherValuePx) *
                                        (intersectValuePx - otherValuePx);
      if (intersectKeyPx < qMin(keyMinPx, keyMaxPx) ||
          intersectKeyPx >
              qMax(keyMinPx,
                   keyMaxPx))  // check whether bottom edge is not intersected,
      // then it must be left edge (qMin/qMax necessary
      // since axes may be reversed)
      {
        intersectKeyPx = keyMinPx;
        intersectValuePx = otherValuePx + (valuePx - otherValuePx) /
                                              (keyPx - otherKeyPx) *
                                              (intersectKeyPx - otherKeyPx);
      }
      break;
    }
    case 4:  // top edge
    {
      intersectValuePx = valueMaxPx;
      intersectKeyPx = otherKeyPx + (keyPx - otherKeyPx) /
                                        (valuePx - otherValuePx) *
                                        (intersectValuePx - otherValuePx);
      break;
    }
    case 5: {
      break;  // case 5 shouldn't happen for this function but we add it anyway
              // to prevent potential discontinuity in branch table
    }
    case 6:  // bottom edge
    {
      intersectValuePx = valueMinPx;
      intersectKeyPx = otherKeyPx + (keyPx - otherKeyPx) /
                                        (valuePx - otherValuePx) *
                                        (intersectValuePx - otherValuePx);
      break;
    }
    case 7:  // top and right edge
    {
      intersectValuePx = valueMaxPx;
      intersectKeyPx = otherKeyPx + (keyPx - otherKeyPx) /
                                        (valuePx - otherValuePx) *
                                        (intersectValuePx - otherValuePx);
      if (intersectKeyPx < qMin(keyMinPx, keyMaxPx) ||
          intersectKeyPx >
              qMax(keyMinPx,
                   keyMaxPx))  // check whether top edge is not intersected,
      // then it must be right edge (qMin/qMax
      // necessary since axes may be reversed)
      {
        intersectKeyPx = keyMaxPx;
        intersectValuePx = otherValuePx + (valuePx - otherValuePx) /
                                              (keyPx - otherKeyPx) *
                                              (intersectKeyPx - otherKeyPx);
      }
      break;
    }
    case 8:  // right edge
    {
      intersectKeyPx = keyMaxPx;
      intersectValuePx = otherValuePx + (valuePx - otherValuePx) /
                                            (keyPx - otherKeyPx) *
                                            (intersectKeyPx - otherKeyPx);
      break;
    }
    case 9:  // bottom and right edge
    {
      intersectValuePx = valueMinPx;
      intersectKeyPx = otherKeyPx + (keyPx - otherKeyPx) /
                                        (valuePx - otherValuePx) *
                                        (intersectValuePx - otherValuePx);
      if (intersectKeyPx < qMin(keyMinPx, keyMaxPx) ||
          intersectKeyPx >
              qMax(keyMinPx,
                   keyMaxPx))  // check whether bottom edge is not intersected,
      // then it must be right edge (qMin/qMax
      // necessary since axes may be reversed)
      {
        intersectKeyPx = keyMaxPx;
        intersectValuePx = otherValuePx + (valuePx - otherValuePx) /
                                              (keyPx - otherKeyPx) *
                                              (intersectKeyPx - otherKeyPx);
      }
      break;
    }
  }
  if (mKeyAxis->orientation() == Qt::Horizontal)
    return {intersectKeyPx, intersectValuePx};
  else
    return {intersectValuePx, intersectKeyPx};
}

/*! \internal

  This function is part of the curve optimization algorithm of \ref
  getCurveLines.

  In situations where a single segment skips over multiple regions it might
  become necessary to add extra points at the corners of region 5 (see \ref
  getRegion) such that the optimized segment doesn't unintentionally cut through
  the visible area of the axis rect and create plot artifacts. This method
  provides these points that must be added, assuming the original segment
  doesn't start, end, or traverse region 5. (Corner points where region 5 is
  traversed are calculated by \ref getTraverseCornerPoints.)

  For example, consider a segment which directly goes from region 4 to 2 but
  originally is far out to the top left such that it doesn't cross region 5.
  Naively optimizing these points by projecting them on the top and left borders
  of region 5 will create a segment that surely crosses 5, creating a visual
  artifact in the plot. This method prevents this by providing extra points at
  the top left corner, making the optimized curve correctly pass from region 4
  to 1 to 2 without traversing 5.
*/
QVector<QPointF> QCPCurve::getOptimizedCornerPoints(
    int prevRegion, int currentRegion, double prevKey, double prevValue,
    double key, double value, double keyMin, double valueMax, double keyMax,
    double valueMin) const {
  QVector<QPointF> result;
  switch (prevRegion) {
    case 1: {
      switch (currentRegion) {
        case 2: {
          result << coordsToPixels(keyMin, valueMax);
          break;
        }
        case 4: {
          result << coordsToPixels(keyMin, valueMax);
          break;
        }
        case 3: {
          result << coordsToPixels(keyMin, valueMax)
                 << coordsToPixels(keyMin, valueMin);
          break;
        }
        case 7: {
          result << coordsToPixels(keyMin, valueMax)
                 << coordsToPixels(keyMax, valueMax);
          break;
        }
        case 6: {
          result << coordsToPixels(keyMin, valueMax)
                 << coordsToPixels(keyMin, valueMin);
          result.append(result.last());
          break;
        }
        case 8: {
          result << coordsToPixels(keyMin, valueMax)
                 << coordsToPixels(keyMax, valueMax);
          result.append(result.last());
          break;
        }
        case 9: {  // in this case we need another distinction of cases: segment
          // may pass below or above rect, requiring either bottom
          // right or top left corner points
          if ((value - prevValue) / (key - prevKey) * (keyMin - key) + value <
              valueMin)  // segment passes below R
          {
            result << coordsToPixels(keyMin, valueMax)
                   << coordsToPixels(keyMin, valueMin);
            result.append(result.last());
            result << coordsToPixels(keyMax, valueMin);
          } else {
            result << coordsToPixels(keyMin, valueMax)
                   << coordsToPixels(keyMax, valueMax);
            result.append(result.last());
            result << coordsToPixels(keyMax, valueMin);
          }
          break;
        }
      }
      break;
    }
    case 2: {
      switch (currentRegion) {
        case 1: {
          result << coordsToPixels(keyMin, valueMax);
          break;
        }
        case 3: {
          result << coordsToPixels(keyMin, valueMin);
          break;
        }
        case 4: {
          result << coordsToPixels(keyMin, valueMax);
          result.append(result.last());
          break;
        }
        case 6: {
          result << coordsToPixels(keyMin, valueMin);
          result.append(result.last());
          break;
        }
        case 7: {
          result << coordsToPixels(keyMin, valueMax);
          result.append(result.last());
          result << coordsToPixels(keyMax, valueMax);
          break;
        }
        case 9: {
          result << coordsToPixels(keyMin, valueMin);
          result.append(result.last());
          result << coordsToPixels(keyMax, valueMin);
          break;
        }
      }
      break;
    }
    case 3: {
      switch (currentRegion) {
        case 2: {
          result << coordsToPixels(keyMin, valueMin);
          break;
        }
        case 6: {
          result << coordsToPixels(keyMin, valueMin);
          break;
        }
        case 1: {
          result << coordsToPixels(keyMin, valueMin)
                 << coordsToPixels(keyMin, valueMax);
          break;
        }
        case 9: {
          result << coordsToPixels(keyMin, valueMin)
                 << coordsToPixels(keyMax, valueMin);
          break;
        }
        case 4: {
          result << coordsToPixels(keyMin, valueMin)
                 << coordsToPixels(keyMin, valueMax);
          result.append(result.last());
          break;
        }
        case 8: {
          result << coordsToPixels(keyMin, valueMin)
                 << coordsToPixels(keyMax, valueMin);
          result.append(result.last());
          break;
        }
        case 7: {  // in this case we need another distinction of cases: segment
          // may pass below or above rect, requiring either bottom
          // right or top left corner points
          if ((value - prevValue) / (key - prevKey) * (keyMax - key) + value <
              valueMin)  // segment passes below R
          {
            result << coordsToPixels(keyMin, valueMin)
                   << coordsToPixels(keyMax, valueMin);
            result.append(result.last());
            result << coordsToPixels(keyMax, valueMax);
          } else {
            result << coordsToPixels(keyMin, valueMin)
                   << coordsToPixels(keyMin, valueMax);
            result.append(result.last());
            result << coordsToPixels(keyMax, valueMax);
          }
          break;
        }
      }
      break;
    }
    case 4: {
      switch (currentRegion) {
        case 1: {
          result << coordsToPixels(keyMin, valueMax);
          break;
        }
        case 7: {
          result << coordsToPixels(keyMax, valueMax);
          break;
        }
        case 2: {
          result << coordsToPixels(keyMin, valueMax);
          result.append(result.last());
          break;
        }
        case 8: {
          result << coordsToPixels(keyMax, valueMax);
          result.append(result.last());
          break;
        }
        case 3: {
          result << coordsToPixels(keyMin, valueMax);
          result.append(result.last());
          result << coordsToPixels(keyMin, valueMin);
          break;
        }
        case 9: {
          result << coordsToPixels(keyMax, valueMax);
          result.append(result.last());
          result << coordsToPixels(keyMax, valueMin);
          break;
        }
      }
      break;
    }
    case 5: {
      switch (currentRegion) {
        case 1: {
          result << coordsToPixels(keyMin, valueMax);
          break;
        }
        case 7: {
          result << coordsToPixels(keyMax, valueMax);
          break;
        }
        case 9: {
          result << coordsToPixels(keyMax, valueMin);
          break;
        }
        case 3: {
          result << coordsToPixels(keyMin, valueMin);
          break;
        }
      }
      break;
    }
    case 6: {
      switch (currentRegion) {
        case 3: {
          result << coordsToPixels(keyMin, valueMin);
          break;
        }
        case 9: {
          result << coordsToPixels(keyMax, valueMin);
          break;
        }
        case 2: {
          result << coordsToPixels(keyMin, valueMin);
          result.append(result.last());
          break;
        }
        case 8: {
          result << coordsToPixels(keyMax, valueMin);
          result.append(result.last());
          break;
        }
        case 1: {
          result << coordsToPixels(keyMin, valueMin);
          result.append(result.last());
          result << coordsToPixels(keyMin, valueMax);
          break;
        }
        case 7: {
          result << coordsToPixels(keyMax, valueMin);
          result.append(result.last());
          result << coordsToPixels(keyMax, valueMax);
          break;
        }
      }
      break;
    }
    case 7: {
      switch (currentRegion) {
        case 4: {
          result << coordsToPixels(keyMax, valueMax);
          break;
        }
        case 8: {
          result << coordsToPixels(keyMax, valueMax);
          break;
        }
        case 1: {
          result << coordsToPixels(keyMax, valueMax)
                 << coordsToPixels(keyMin, valueMax);
          break;
        }
        case 9: {
          result << coordsToPixels(keyMax, valueMax)
                 << coordsToPixels(keyMax, valueMin);
          break;
        }
        case 2: {
          result << coordsToPixels(keyMax, valueMax)
                 << coordsToPixels(keyMin, valueMax);
          result.append(result.last());
          break;
        }
        case 6: {
          result << coordsToPixels(keyMax, valueMax)
                 << coordsToPixels(keyMax, valueMin);
          result.append(result.last());
          break;
        }
        case 3: {  // in this case we need another distinction of cases: segment
          // may pass below or above rect, requiring either bottom
          // right or top left corner points
          if ((value - prevValue) / (key - prevKey) * (keyMax - key) + value <
              valueMin)  // segment passes below R
          {
            result << coordsToPixels(keyMax, valueMax)
                   << coordsToPixels(keyMax, valueMin);
            result.append(result.last());
            result << coordsToPixels(keyMin, valueMin);
          } else {
            result << coordsToPixels(keyMax, valueMax)
                   << coordsToPixels(keyMin, valueMax);
            result.append(result.last());
            result << coordsToPixels(keyMin, valueMin);
          }
          break;
        }
      }
      break;
    }
    case 8: {
      switch (currentRegion) {
        case 7: {
          result << coordsToPixels(keyMax, valueMax);
          break;
        }
        case 9: {
          result << coordsToPixels(keyMax, valueMin);
          break;
        }
        case 4: {
          result << coordsToPixels(keyMax, valueMax);
          result.append(result.last());
          break;
        }
        case 6: {
          result << coordsToPixels(keyMax, valueMin);
          result.append(result.last());
          break;
        }
        case 1: {
          result << coordsToPixels(keyMax, valueMax);
          result.append(result.last());
          result << coordsToPixels(keyMin, valueMax);
          break;
        }
        case 3: {
          result << coordsToPixels(keyMax, valueMin);
          result.append(result.last());
          result << coordsToPixels(keyMin, valueMin);
          break;
        }
      }
      break;
    }
    case 9: {
      switch (currentRegion) {
        case 6: {
          result << coordsToPixels(keyMax, valueMin);
          break;
        }
        case 8: {
          result << coordsToPixels(keyMax, valueMin);
          break;
        }
        case 3: {
          result << coordsToPixels(keyMax, valueMin)
                 << coordsToPixels(keyMin, valueMin);
          break;
        }
        case 7: {
          result << coordsToPixels(keyMax, valueMin)
                 << coordsToPixels(keyMax, valueMax);
          break;
        }
        case 2: {
          result << coordsToPixels(keyMax, valueMin)
                 << coordsToPixels(keyMin, valueMin);
          result.append(result.last());
          break;
        }
        case 4: {
          result << coordsToPixels(keyMax, valueMin)
                 << coordsToPixels(keyMax, valueMax);
          result.append(result.last());
          break;
        }
        case 1: {  // in this case we need another distinction of cases: segment
          // may pass below or above rect, requiring either bottom
          // right or top left corner points
          if ((value - prevValue) / (key - prevKey) * (keyMin - key) + value <
              valueMin)  // segment passes below R
          {
            result << coordsToPixels(keyMax, valueMin)
                   << coordsToPixels(keyMin, valueMin);
            result.append(result.last());
            result << coordsToPixels(keyMin, valueMax);
          } else {
            result << coordsToPixels(keyMax, valueMin)
                   << coordsToPixels(keyMax, valueMax);
            result.append(result.last());
            result << coordsToPixels(keyMin, valueMax);
          }
          break;
        }
      }
      break;
    }
  }
  return result;
}

/*! \internal

  This function is part of the curve optimization algorithm of \ref
  getCurveLines.

  This method returns whether a segment going from \a prevRegion to \a
  currentRegion (see \ref getRegion) may traverse the visible region 5. This
  function assumes that neither \a prevRegion nor \a currentRegion is 5 itself.

  If this method returns false, the segment for sure doesn't pass region 5. If
  it returns true, the segment may or may not pass region 5 and a more
  fine-grained calculation must be used (\ref getTraverse).
*/
bool QCPCurve::mayTraverse(int prevRegion, int currentRegion) const {
  switch (prevRegion) {
    case 1: {
      switch (currentRegion) {
        case 4:
        case 7:
        case 2:
        case 3:
          return false;
        default:
          return true;
      }
    }
    case 2: {
      switch (currentRegion) {
        case 1:
        case 3:
          return false;
        default:
          return true;
      }
    }
    case 3: {
      switch (currentRegion) {
        case 1:
        case 2:
        case 6:
        case 9:
          return false;
        default:
          return true;
      }
    }
    case 4: {
      switch (currentRegion) {
        case 1:
        case 7:
          return false;
        default:
          return true;
      }
    }
    case 5:
      return false;  // should never occur
    case 6: {
      switch (currentRegion) {
        case 3:
        case 9:
          return false;
        default:
          return true;
      }
    }
    case 7: {
      switch (currentRegion) {
        case 1:
        case 4:
        case 8:
        case 9:
          return false;
        default:
          return true;
      }
    }
    case 8: {
      switch (currentRegion) {
        case 7:
        case 9:
          return false;
        default:
          return true;
      }
    }
    case 9: {
      switch (currentRegion) {
        case 3:
        case 6:
        case 8:
        case 7:
          return false;
        default:
          return true;
      }
    }
    default:
      return true;
  }
}

/*! \internal

  This function is part of the curve optimization algorithm of \ref
  getCurveLines.

  This method assumes that the \ref mayTraverse test has returned true, so there
  is a chance the segment defined by (\a prevKey, \a prevValue) and (\a key, \a
  value) goes through the visible region 5.

  The return value of this method indicates whether the segment actually
  traverses region 5 or not.

  If the segment traverses 5, the output parameters \a crossA and \a crossB
  indicate the entry and exit points of region 5. They will become the optimized
  points for that segment.
*/
bool QCPCurve::getTraverse(double prevKey, double prevValue, double key,
                           double value, double keyMin, double valueMax,
                           double keyMax, double valueMin, QPointF &crossA,
                           QPointF &crossB) const {
  // The intersection point interpolation here is done in pixel coordinates, so
  // we don't need to differentiate between different axis scale types. Note
  // that the nomenclature top/left/bottom/right/min/max is with respect to the
  // rect in plot coordinates, wich may be different in pixel coordinates
  // (horz/vert key axes, reversed ranges)

  QList<QPointF> intersections;
  const double valueMinPx = mValueAxis->coordToPixel(valueMin);
  const double valueMaxPx = mValueAxis->coordToPixel(valueMax);
  const double keyMinPx = mKeyAxis->coordToPixel(keyMin);
  const double keyMaxPx = mKeyAxis->coordToPixel(keyMax);
  const double keyPx = mKeyAxis->coordToPixel(key);
  const double valuePx = mValueAxis->coordToPixel(value);
  const double prevKeyPx = mKeyAxis->coordToPixel(prevKey);
  const double prevValuePx = mValueAxis->coordToPixel(prevValue);
  if (qFuzzyIsNull(keyPx - prevKeyPx))  // line is parallel to value axis
  {
    // due to region filter in mayTraverse(), if line is parallel to value or
    // key axis, region 5 is traversed here
    intersections.append(
        mKeyAxis->orientation() == Qt::Horizontal
            ? QPointF(keyPx, valueMinPx)
            : QPointF(
                  valueMinPx,
                  keyPx));  // direction will be taken care of at end of method
    intersections.append(mKeyAxis->orientation() == Qt::Horizontal
                             ? QPointF(keyPx, valueMaxPx)
                             : QPointF(valueMaxPx, keyPx));
  } else if (qFuzzyIsNull(valuePx -
                          prevValuePx))  // line is parallel to key axis
  {
    // due to region filter in mayTraverse(), if line is parallel to value or
    // key axis, region 5 is traversed here
    intersections.append(
        mKeyAxis->orientation() == Qt::Horizontal
            ? QPointF(keyMinPx, valuePx)
            : QPointF(valuePx, keyMinPx));  // direction will be taken care of
                                            // at end of method
    intersections.append(mKeyAxis->orientation() == Qt::Horizontal
                             ? QPointF(keyMaxPx, valuePx)
                             : QPointF(valuePx, keyMaxPx));
  } else  // line is skewed
  {
    double gamma;
    double keyPerValuePx = (keyPx - prevKeyPx) / (valuePx - prevValuePx);
    // check top of rect:
    gamma = prevKeyPx + (valueMaxPx - prevValuePx) * keyPerValuePx;
    if (gamma >= qMin(keyMinPx, keyMaxPx) &&
        gamma <=
            qMax(keyMinPx,
                 keyMaxPx))  // qMin/qMax necessary since axes may be reversed
      intersections.append(mKeyAxis->orientation() == Qt::Horizontal
                               ? QPointF(gamma, valueMaxPx)
                               : QPointF(valueMaxPx, gamma));
    // check bottom of rect:
    gamma = prevKeyPx + (valueMinPx - prevValuePx) * keyPerValuePx;
    if (gamma >= qMin(keyMinPx, keyMaxPx) &&
        gamma <=
            qMax(keyMinPx,
                 keyMaxPx))  // qMin/qMax necessary since axes may be reversed
      intersections.append(mKeyAxis->orientation() == Qt::Horizontal
                               ? QPointF(gamma, valueMinPx)
                               : QPointF(valueMinPx, gamma));
    const double valuePerKeyPx = 1.0 / keyPerValuePx;
    // check left of rect:
    gamma = prevValuePx + (keyMinPx - prevKeyPx) * valuePerKeyPx;
    if (gamma >= qMin(valueMinPx, valueMaxPx) &&
        gamma <=
            qMax(valueMinPx,
                 valueMaxPx))  // qMin/qMax necessary since axes may be reversed
      intersections.append(mKeyAxis->orientation() == Qt::Horizontal
                               ? QPointF(keyMinPx, gamma)
                               : QPointF(gamma, keyMinPx));
    // check right of rect:
    gamma = prevValuePx + (keyMaxPx - prevKeyPx) * valuePerKeyPx;
    if (gamma >= qMin(valueMinPx, valueMaxPx) &&
        gamma <=
            qMax(valueMinPx,
                 valueMaxPx))  // qMin/qMax necessary since axes may be reversed
      intersections.append(mKeyAxis->orientation() == Qt::Horizontal
                               ? QPointF(keyMaxPx, gamma)
                               : QPointF(gamma, keyMaxPx));
  }

  // handle cases where found points isn't exactly 2:
  if (intersections.size() > 2) {
    // line probably goes through corner of rect, and we got duplicate points
    // there. single out the point pair with greatest distance in between:
    double distSqrMax = 0;
    QPointF pv1, pv2;
    for (int i = 0; i < intersections.size() - 1; ++i) {
      for (int k = i + 1; k < intersections.size(); ++k) {
        QPointF distPoint = intersections.at(i) - intersections.at(k);
        double distSqr =
            distPoint.x() * distPoint.x() + distPoint.y() + distPoint.y();
        if (distSqr > distSqrMax) {
          pv1 = intersections.at(i);
          pv2 = intersections.at(k);
          distSqrMax = distSqr;
        }
      }
    }
    intersections = QList<QPointF>() << pv1 << pv2;
  } else if (intersections.size() != 2) {
    // one or even zero points found (shouldn't happen unless line perfectly
    // tangent to corner), no need to draw segment
    return false;
  }

  // possibly re-sort points so optimized point segment has same direction as
  // original segment:
  double xDelta = keyPx - prevKeyPx;
  double yDelta = valuePx - prevValuePx;
  if (mKeyAxis->orientation() != Qt::Horizontal) qSwap(xDelta, yDelta);
  if (xDelta * (intersections.at(1).x() - intersections.at(0).x()) +
          yDelta * (intersections.at(1).y() - intersections.at(0).y()) <
      0)  // scalar product of both segments < 0 -> opposite direction
    intersections.move(0, 1);
  crossA = intersections.at(0);
  crossB = intersections.at(1);
  return true;
}

/*! \internal

  This function is part of the curve optimization algorithm of \ref
  getCurveLines.

  This method assumes that the \ref getTraverse test has returned true, so the
  segment definitely traverses the visible region 5 when going from \a
  prevRegion to \a currentRegion.

  In certain situations it is not sufficient to merely generate the entry and
  exit points of the segment into/out of region 5, as \ref getTraverse provides.
  It may happen that a single segment, in addition to traversing region 5, skips
  another region outside of region 5, which makes it necessary to add an
  optimized corner point there (very similar to the job \ref
  getOptimizedCornerPoints does for segments that are completely in outside
  regions and don't traverse 5).

  As an example, consider a segment going from region 1 to region 6, traversing
  the lower left corner of region 5. In this configuration, the segment
  additionally crosses the border between region 1 and 2 before entering
  region 5. This makes it necessary to add an additional point in the top left
  corner, before adding the optimized traverse points. So in this case, the
  output parameter \a beforeTraverse will contain the top left corner point, and
  \a afterTraverse will be empty.

  In some cases, such as when going from region 1 to 9, it may even be necessary
  to add additional corner points before and after the traverse. Then both \a
  beforeTraverse and \a afterTraverse return the respective corner points.
*/
void QCPCurve::getTraverseCornerPoints(int prevRegion, int currentRegion,
                                       double keyMin, double valueMax,
                                       double keyMax, double valueMin,
                                       QVector<QPointF> &beforeTraverse,
                                       QVector<QPointF> &afterTraverse) const {
  switch (prevRegion) {
    case 1: {
      switch (currentRegion) {
        case 6: {
          beforeTraverse << coordsToPixels(keyMin, valueMax);
          break;
        }
        case 9: {
          beforeTraverse << coordsToPixels(keyMin, valueMax);
          afterTraverse << coordsToPixels(keyMax, valueMin);
          break;
        }
        case 8: {
          beforeTraverse << coordsToPixels(keyMin, valueMax);
          break;
        }
      }
      break;
    }
    case 2: {
      switch (currentRegion) {
        case 7: {
          afterTraverse << coordsToPixels(keyMax, valueMax);
          break;
        }
        case 9: {
          afterTraverse << coordsToPixels(keyMax, valueMin);
          break;
        }
      }
      break;
    }
    case 3: {
      switch (currentRegion) {
        case 4: {
          beforeTraverse << coordsToPixels(keyMin, valueMin);
          break;
        }
        case 7: {
          beforeTraverse << coordsToPixels(keyMin, valueMin);
          afterTraverse << coordsToPixels(keyMax, valueMax);
          break;
        }
        case 8: {
          beforeTraverse << coordsToPixels(keyMin, valueMin);
          break;
        }
      }
      break;
    }
    case 4: {
      switch (currentRegion) {
        case 3: {
          afterTraverse << coordsToPixels(keyMin, valueMin);
          break;
        }
        case 9: {
          afterTraverse << coordsToPixels(keyMax, valueMin);
          break;
        }
      }
      break;
    }
    case 5: {
      break;
    }  // shouldn't happen because this method only handles full traverses
    case 6: {
      switch (currentRegion) {
        case 1: {
          afterTraverse << coordsToPixels(keyMin, valueMax);
          break;
        }
        case 7: {
          afterTraverse << coordsToPixels(keyMax, valueMax);
          break;
        }
      }
      break;
    }
    case 7: {
      switch (currentRegion) {
        case 2: {
          beforeTraverse << coordsToPixels(keyMax, valueMax);
          break;
        }
        case 3: {
          beforeTraverse << coordsToPixels(keyMax, valueMax);
          afterTraverse << coordsToPixels(keyMin, valueMin);
          break;
        }
        case 6: {
          beforeTraverse << coordsToPixels(keyMax, valueMax);
          break;
        }
      }
      break;
    }
    case 8: {
      switch (currentRegion) {
        case 1: {
          afterTraverse << coordsToPixels(keyMin, valueMax);
          break;
        }
        case 3: {
          afterTraverse << coordsToPixels(keyMin, valueMin);
          break;
        }
      }
      break;
    }
    case 9: {
      switch (currentRegion) {
        case 2: {
          beforeTraverse << coordsToPixels(keyMax, valueMin);
          break;
        }
        case 1: {
          beforeTraverse << coordsToPixels(keyMax, valueMin);
          afterTraverse << coordsToPixels(keyMin, valueMax);
          break;
        }
        case 4: {
          beforeTraverse << coordsToPixels(keyMax, valueMin);
          break;
        }
      }
      break;
    }
  }
}

/*! \internal

  Calculates the (minimum) distance (in pixels) the curve's representation has
  from the given \a pixelPoint in pixels. This is used to determine whether the
  curve was clicked or not, e.g. in \ref selectTest. The closest data point to
  \a pixelPoint is returned in \a closestData. Note that if the curve has a line
  representation, the returned distance may be smaller than the distance to the
  \a closestData point, since the distance to the curve line is also taken into
  account.

  If either the curve has no data or if the line style is \ref lsNone and the
  scatter style's shape is \ref QCPScatterStyle::ssNone (i.e. there is no visual
  representation of the curve), returns -1.0.
*/
double QCPCurve::pointDistance(
    const QPointF &pixelPoint,
    QCPCurveDataContainer::const_iterator &closestData) const {
  closestData = mDataContainer->constEnd();
  if (mDataContainer->isEmpty()) return -1.0;
  if (mLineStyle == lsNone && mScatterStyle.isNone()) return -1.0;

  if (mDataContainer->size() == 1) {
    QPointF dataPoint = coordsToPixels(mDataContainer->constBegin()->key,
                                       mDataContainer->constBegin()->value);
    closestData = mDataContainer->constBegin();
    return QCPVector2D(dataPoint - pixelPoint).length();
  }

  // calculate minimum distances to curve data points and find closestData
  // iterator:
  double minDistSqr = (std::numeric_limits<double>::max)();
  // iterate over found data points and then choose the one with the shortest
  // distance to pos:
  QCPCurveDataContainer::const_iterator begin = mDataContainer->constBegin();
  QCPCurveDataContainer::const_iterator end = mDataContainer->constEnd();
  for (QCPCurveDataContainer::const_iterator it = begin; it != end; ++it) {
    const double currentDistSqr =
        QCPVector2D(coordsToPixels(it->key, it->value) - pixelPoint)
            .lengthSquared();
    if (currentDistSqr < minDistSqr) {
      minDistSqr = currentDistSqr;
      closestData = it;
    }
  }

  // calculate distance to line if there is one (if so, will probably be smaller
  // than distance to closest data point):
  if (mLineStyle != lsNone) {
    QVector<QPointF> lines;
    getCurveLines(
        &lines, QCPDataRange(0, dataCount()),
        mParentPlot->selectionTolerance() *
            1.2);  // optimized lines outside axis rect shouldn't respond to
                   // clicks at the edge, so use 1.2*tolerance as pen width
    for (int i = 0; i < lines.size() - 1; ++i) {
      double currentDistSqr =
          QCPVector2D(pixelPoint)
              .distanceSquaredToLine(lines.at(i), lines.at(i + 1));
      if (currentDistSqr < minDistSqr) minDistSqr = currentDistSqr;
    }
  }

  return qSqrt(minDistSqr);
}
/* end of 'src/plottables/plottable-curve.cpp' */

/* including file 'src/plottables/plottable-bars.cpp' */
/* modified 2022-11-06T12:45:56, size 43907           */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPBarsGroup
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPBarsGroup
  \brief Groups multiple QCPBars together so they appear side by side

  \image html QCPBarsGroup.png

  When showing multiple QCPBars in one plot which have bars at identical keys,
  it may be desirable to have them appearing next to each other at each key.
  This is what adding the respective QCPBars plottables to a QCPBarsGroup
  achieves. (An alternative approach is to stack them on top of each other, see
  \ref QCPBars::moveAbove.)

  \section qcpbarsgroup-usage Usage

  To add a QCPBars plottable to the group, create a new group and then add the
  respective bars intances: \snippet
  documentation/doc-code-snippets/mainwindow.cpp qcpbarsgroup-creation
  Alternatively to appending to the group like shown above, you can also set the
  group on the QCPBars plottable via \ref QCPBars::setBarsGroup.

  The spacing between the bars can be configured via \ref setSpacingType and
  \ref setSpacing. The bars in this group appear in the plot in the order they
  were appended. To insert a bars plottable at a certain index position, or to
  reposition a bars plottable which is already in the group, use \ref insert.

  To remove specific bars from the group, use either \ref remove or call \ref
  QCPBars::setBarsGroup "QCPBars::setBarsGroup(0)" on the respective bars
  plottable.

  To clear the entire group, call \ref clear, or simply delete the group.

  \section qcpbarsgroup-example Example

  The image above is generated with the following code:
  \snippet documentation/doc-image-generator/mainwindow.cpp qcpbarsgroup-example
*/

/* start of documentation of inline functions */

/*! \fn QList<QCPBars*> QCPBarsGroup::bars() const

  Returns all bars currently in this group.

  \see bars(int index)
*/

/*! \fn int QCPBarsGroup::size() const

  Returns the number of QCPBars plottables that are part of this group.

*/

/*! \fn bool QCPBarsGroup::isEmpty() const

  Returns whether this bars group is empty.

  \see size
*/

/*! \fn bool QCPBarsGroup::contains(QCPBars *bars)

  Returns whether the specified \a bars plottable is part of this group.

*/

/* end of documentation of inline functions */

/*!
  Constructs a new bars group for the specified QCustomPlot instance.
*/
QCPBarsGroup::QCPBarsGroup(QCustomPlot *parentPlot)
    : QObject(parentPlot),
      mParentPlot(parentPlot),
      mSpacingType(stAbsolute),
      mSpacing(4) {}

QCPBarsGroup::~QCPBarsGroup() { clear(); }

/*!
  Sets how the spacing between adjacent bars is interpreted. See \ref
  SpacingType.

  The actual spacing can then be specified with \ref setSpacing.

  \see setSpacing
*/
void QCPBarsGroup::setSpacingType(SpacingType spacingType) {
  mSpacingType = spacingType;
}

/*!
  Sets the spacing between adjacent bars. What the number passed as \a spacing
  actually means, is defined by the current \ref SpacingType, which can be set
  with \ref setSpacingType.

  \see setSpacingType
*/
void QCPBarsGroup::setSpacing(double spacing) { mSpacing = spacing; }

/*!
  Returns the QCPBars instance with the specified \a index in this group. If no
  such QCPBars exists, returns \c nullptr.

  \see bars(), size
*/
QCPBars *QCPBarsGroup::bars(int index) const {
  if (index >= 0 && index < mBars.size()) {
    return mBars.at(index);
  } else {
    qDebug() << Q_FUNC_INFO << "index out of bounds:" << index;
    return nullptr;
  }
}

/*!
  Removes all QCPBars plottables from this group.

  \see isEmpty
*/
void QCPBarsGroup::clear() {
  const QList<QCPBars *> oldBars = mBars;
  foreach (QCPBars *bars, oldBars)
    bars->setBarsGroup(nullptr);  // removes itself from mBars via removeBars
}

/*!
  Adds the specified \a bars plottable to this group. Alternatively, you can
  also use \ref QCPBars::setBarsGroup on the \a bars instance.

  \see insert, remove
*/
void QCPBarsGroup::append(QCPBars *bars) {
  if (!bars) {
    qDebug() << Q_FUNC_INFO << "bars is 0";
    return;
  }

  if (!mBars.contains(bars))
    bars->setBarsGroup(this);
  else
    qDebug() << Q_FUNC_INFO << "bars plottable is already in this bars group:"
             << reinterpret_cast<quintptr>(bars);
}

/*!
  Inserts the specified \a bars plottable into this group at the specified index
  position \a i. This gives you full control over the ordering of the bars.

  \a bars may already be part of this group. In that case, \a bars is just moved
  to the new index position.

  \see append, remove
*/
void QCPBarsGroup::insert(int i, QCPBars *bars) {
  if (!bars) {
    qDebug() << Q_FUNC_INFO << "bars is 0";
    return;
  }

  // first append to bars list normally:
  if (!mBars.contains(bars)) bars->setBarsGroup(this);
  // then move to according position:
  mBars.move(mBars.indexOf(bars), qBound(0, i, mBars.size() - 1));
}

/*!
  Removes the specified \a bars plottable from this group.

  \see contains, clear
*/
void QCPBarsGroup::remove(QCPBars *bars) {
  if (!bars) {
    qDebug() << Q_FUNC_INFO << "bars is 0";
    return;
  }

  if (mBars.contains(bars))
    bars->setBarsGroup(nullptr);
  else
    qDebug() << Q_FUNC_INFO << "bars plottable is not in this bars group:"
             << reinterpret_cast<quintptr>(bars);
}

/*! \internal

  Adds the specified \a bars to the internal mBars list of bars. This method
  does not change the barsGroup property on \a bars.

  \see unregisterBars
*/
void QCPBarsGroup::registerBars(QCPBars *bars) {
  if (!mBars.contains(bars)) mBars.append(bars);
}

/*! \internal

  Removes the specified \a bars from the internal mBars list of bars. This
  method does not change the barsGroup property on \a bars.

  \see registerBars
*/
void QCPBarsGroup::unregisterBars(QCPBars *bars) { mBars.removeOne(bars); }

/*! \internal

  Returns the pixel offset in the key dimension the specified \a bars plottable
  should have at the given key coordinate \a keyCoord. The offset is relative to
  the pixel position of the key coordinate \a keyCoord.
*/
double QCPBarsGroup::keyPixelOffset(const QCPBars *bars, double keyCoord) {
  // find list of all base bars in case some mBars are stacked:
  QList<const QCPBars *> baseBars;
  foreach (const QCPBars *b, mBars) {
    while (b->barBelow()) b = b->barBelow();
    if (!baseBars.contains(b)) baseBars.append(b);
  }
  // find base bar this "bars" is stacked on:
  const QCPBars *thisBase = bars;
  while (thisBase->barBelow()) thisBase = thisBase->barBelow();

  // determine key pixel offset of this base bars considering all other base
  // bars in this barsgroup:
  double result = 0;
  int index = baseBars.indexOf(thisBase);
  if (index >= 0) {
    if (baseBars.size() % 2 == 1 &&
        index == (baseBars.size() - 1) /
                     2)  // is center bar (int division on purpose)
    {
      return result;
    } else {
      double lowerPixelWidth, upperPixelWidth;
      int startIndex;
      int dir = (index <= (baseBars.size() - 1) / 2)
                    ? -1
                    : 1;  // if bar is to lower keys of center, dir is negative
      if (baseBars.size() % 2 == 0)  // even number of bars
      {
        startIndex = baseBars.size() / 2 + (dir < 0 ? -1 : 0);
        result += getPixelSpacing(baseBars.at(startIndex), keyCoord) *
                  0.5;  // half of middle spacing
      } else            // uneven number of bars
      {
        startIndex = (baseBars.size() - 1) / 2 + dir;
        baseBars.at((baseBars.size() - 1) / 2)
            ->getPixelWidth(keyCoord, lowerPixelWidth, upperPixelWidth);
        result += qAbs(upperPixelWidth - lowerPixelWidth) *
                  0.5;  // half of center bar
        result += getPixelSpacing(baseBars.at((baseBars.size() - 1) / 2),
                                  keyCoord);  // center bar spacing
      }
      for (int i = startIndex; i != index;
           i += dir)  // add widths and spacings of bars in between center and
      // our bars
      {
        baseBars.at(i)->getPixelWidth(keyCoord, lowerPixelWidth,
                                      upperPixelWidth);
        result += qAbs(upperPixelWidth - lowerPixelWidth);
        result += getPixelSpacing(baseBars.at(i), keyCoord);
      }
      // finally half of our bars width:
      baseBars.at(index)->getPixelWidth(keyCoord, lowerPixelWidth,
                                        upperPixelWidth);
      result += qAbs(upperPixelWidth - lowerPixelWidth) * 0.5;
      // correct sign of result depending on orientation and direction of key
      // axis:
      result *= dir * thisBase->keyAxis()->pixelOrientation();
    }
  }
  return result;
}

/*! \internal

  Returns the spacing in pixels which is between this \a bars and the following
  one, both at the key coordinate \a keyCoord.

  \note Typically the returned value doesn't depend on \a bars or \a keyCoord.
  \a bars is only needed to get access to the key axis transformation and axis
  rect for the modes \ref stAxisRectRatio and \ref stPlotCoords. The \a keyCoord
  is only relevant for spacings given in \ref stPlotCoords on a logarithmic
  axis.
*/
double QCPBarsGroup::getPixelSpacing(const QCPBars *bars, double keyCoord) {
  switch (mSpacingType) {
    case stAbsolute: {
      return mSpacing;
    }
    case stAxisRectRatio: {
      if (bars->keyAxis()->orientation() == Qt::Horizontal)
        return bars->keyAxis()->axisRect()->width() * mSpacing;
      else
        return bars->keyAxis()->axisRect()->height() * mSpacing;
    }
    case stPlotCoords: {
      double keyPixel = bars->keyAxis()->coordToPixel(keyCoord);
      return qAbs(bars->keyAxis()->coordToPixel(keyCoord + mSpacing) -
                  keyPixel);
    }
  }
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPBarsData
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPBarsData
  \brief Holds the data of one single data point (one bar) for QCPBars.

  The stored data is:
  \li \a key: coordinate on the key axis of this bar (this is the \a mainKey and
  the \a sortKey) \li \a value: height coordinate on the value axis of this bar
  (this is the \a mainValue)

  The container for storing multiple data points is \ref QCPBarsDataContainer.
  It is a typedef for \ref QCPDataContainer with \ref QCPBarsData as the
  DataType template parameter. See the documentation there for an explanation
  regarding the data type's generic methods.

  \see QCPBarsDataContainer
*/

/* start documentation of inline functions */

/*! \fn double QCPBarsData::sortKey() const

  Returns the \a key member of this data point.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/*! \fn static QCPBarsData QCPBarsData::fromSortKey(double sortKey)

  Returns a data point with the specified \a sortKey. All other members are set
  to zero.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/*! \fn static static bool QCPBarsData::sortKeyIsMainKey()

  Since the member \a key is both the data point key coordinate and the data
  ordering parameter, this method returns true.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/*! \fn double QCPBarsData::mainKey() const

  Returns the \a key member of this data point.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/*! \fn double QCPBarsData::mainValue() const

  Returns the \a value member of this data point.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/*! \fn QCPRange QCPBarsData::valueRange() const

  Returns a QCPRange with both lower and upper boundary set to \a value of this
  data point.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/* end documentation of inline functions */

/*!
  Constructs a bar data point with key and value set to zero.
*/
QCPBarsData::QCPBarsData() : key(0), value(0) {}

/*!
  Constructs a bar data point with the specified \a key and \a value.
*/
QCPBarsData::QCPBarsData(double key, double value) : key(key), value(value) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPBars
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPBars
  \brief A plottable representing a bar chart in a plot.

  \image html QCPBars.png

  To plot data, assign it with the \ref setData or \ref addData functions.

  \section qcpbars-appearance Changing the appearance

  The appearance of the bars is determined by the pen and the brush (\ref
  setPen, \ref setBrush). The width of the individual bars can be controlled
  with \ref setWidthType and \ref setWidth.

  Bar charts are stackable. This means, two QCPBars plottables can be placed on
  top of each other (see \ref QCPBars::moveAbove). So when two bars are at the
  same key position, they will appear stacked.

  If you would like to group multiple QCPBars plottables together so they appear
  side by side as shown below, use QCPBarsGroup.

  \image html QCPBarsGroup.png

  \section qcpbars-usage Usage

  Like all data representing objects in QCustomPlot, the QCPBars is a plottable
  (QCPAbstractPlottable). So the plottable-interface of QCustomPlot applies
  (QCustomPlot::plottable, QCustomPlot::removePlottable, etc.)

  Usually, you first create an instance:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpbars-creation-1
  which registers it with the QCustomPlot instance of the passed axes. Note that
  this QCustomPlot instance takes ownership of the plottable, so do not delete
  it manually but use QCustomPlot::removePlottable() instead. The newly created
  plottable can be modified, e.g.: \snippet
  documentation/doc-code-snippets/mainwindow.cpp qcpbars-creation-2
*/

/* start of documentation of inline functions */

/*! \fn QSharedPointer<QCPBarsDataContainer> QCPBars::data() const

  Returns a shared pointer to the internal data storage of type \ref
  QCPBarsDataContainer. You may use it to directly manipulate the data, which
  may be more convenient and faster than using the regular \ref setData or \ref
  addData methods.
*/

/*! \fn QCPBars *QCPBars::barBelow() const
  Returns the bars plottable that is directly below this bars plottable.
  If there is no such plottable, returns \c nullptr.

  \see barAbove, moveBelow, moveAbove
*/

/*! \fn QCPBars *QCPBars::barAbove() const
  Returns the bars plottable that is directly above this bars plottable.
  If there is no such plottable, returns \c nullptr.

  \see barBelow, moveBelow, moveAbove
*/

/* end of documentation of inline functions */

/*!
  Constructs a bar chart which uses \a keyAxis as its key axis ("x") and \a
  valueAxis as its value axis ("y"). \a keyAxis and \a valueAxis must reside in
  the same QCustomPlot instance and not have the same orientation. If either of
  these restrictions is violated, a corresponding message is printed to the
  debug output (qDebug), the construction is not aborted, though.

  The created QCPBars is automatically registered with the QCustomPlot instance
  inferred from \a keyAxis. This QCustomPlot instance takes ownership of the
  QCPBars, so do not delete it manually but use QCustomPlot::removePlottable()
  instead.
*/
QCPBars::QCPBars(QCPAxis *keyAxis, QCPAxis *valueAxis)
    : QCPAbstractPlottable1D<QCPBarsData>(keyAxis, valueAxis),
      mWidth(0.75),
      mWidthType(wtPlotCoords),
      mBarsGroup(nullptr),
      mBaseValue(0),
      mStackingGap(1) {
  // modify inherited properties from abstract plottable:
  mPen.setColor(Qt::blue);
  mPen.setStyle(Qt::SolidLine);
  mBrush.setColor(QColor(40, 50, 255, 30));
  mBrush.setStyle(Qt::SolidPattern);
  mSelectionDecorator->setBrush(QBrush(QColor(160, 160, 255)));
}

QCPBars::~QCPBars() {
  setBarsGroup(nullptr);
  if (mBarBelow || mBarAbove)
    connectBars(mBarBelow.data(),
                mBarAbove.data());  // take this bar out of any stacking
}

/*! \overload

  Replaces the current data container with the provided \a data container.

  Since a QSharedPointer is used, multiple QCPBars may share the same data
  container safely. Modifying the data in the container will then affect all
  bars that share the container. Sharing can be achieved by simply exchanging
  the data containers wrapped in shared pointers: \snippet
  documentation/doc-code-snippets/mainwindow.cpp qcpbars-datasharing-1

  If you do not wish to share containers, but create a copy from an existing
  container, rather use the \ref QCPDataContainer<DataType>::set method on the
  bar's data container directly: \snippet
  documentation/doc-code-snippets/mainwindow.cpp qcpbars-datasharing-2

  \see addData
*/
void QCPBars::setData(QSharedPointer<QCPBarsDataContainer> data) {
  mDataContainer = data;
}

/*! \overload

  Replaces the current data with the provided points in \a keys and \a values.
  The provided vectors should have equal length. Else, the number of added
  points will be the size of the smallest vector.

  If you can guarantee that the passed data points are sorted by \a keys in
  ascending order, you can set \a alreadySorted to true, to improve performance
  by saving a sorting run.

  \see addData
*/
void QCPBars::setData(const QVector<double> &keys,
                      const QVector<double> &values, bool alreadySorted) {
  mDataContainer->clear();
  addData(keys, values, alreadySorted);
}

/*!
  Sets the width of the bars.

  How the number passed as \a width is interpreted (e.g. screen pixels, plot
  coordinates,...), depends on the currently set width type, see \ref
  setWidthType and \ref WidthType.
*/
void QCPBars::setWidth(double width) { mWidth = width; }

/*!
  Sets how the width of the bars is defined. See the documentation of \ref
  WidthType for an explanation of the possible values for \a widthType.

  The default value is \ref wtPlotCoords.

  \see setWidth
*/
void QCPBars::setWidthType(QCPBars::WidthType widthType) {
  mWidthType = widthType;
}

/*!
  Sets to which QCPBarsGroup this QCPBars instance belongs to. Alternatively,
  you can also use \ref QCPBarsGroup::append.

  To remove this QCPBars from any group, set \a barsGroup to \c nullptr.
*/
void QCPBars::setBarsGroup(QCPBarsGroup *barsGroup) {
  // deregister at old group:
  if (mBarsGroup) mBarsGroup->unregisterBars(this);
  mBarsGroup = barsGroup;
  // register at new group:
  if (mBarsGroup) mBarsGroup->registerBars(this);
}

/*!
  Sets the base value of this bars plottable.

  The base value defines where on the value coordinate the bars start. How far
  the bars extend from the base value is given by their individual value data.
  For example, if the base value is set to 1, a bar with data value 2 will have
  its lowest point at value coordinate 1 and highest point at 3.

  For stacked bars, only the base value of the bottom-most QCPBars has meaning.

  The default base value is 0.
*/
void QCPBars::setBaseValue(double baseValue) { mBaseValue = baseValue; }

/*!
  If this bars plottable is stacked on top of another bars plottable (\ref
  moveAbove), this method allows specifying a distance in \a pixels, by which
  the drawn bar rectangles will be separated by the bars below it.
*/
void QCPBars::setStackingGap(double pixels) { mStackingGap = pixels; }

/*! \overload

  Adds the provided points in \a keys and \a values to the current data. The
  provided vectors should have equal length. Else, the number of added points
  will be the size of the smallest vector.

  If you can guarantee that the passed data points are sorted by \a keys in
  ascending order, you can set \a alreadySorted to true, to improve performance
  by saving a sorting run.

  Alternatively, you can also access and modify the data directly via the \ref
  data method, which returns a pointer to the internal data container.
*/
void QCPBars::addData(const QVector<double> &keys,
                      const QVector<double> &values, bool alreadySorted) {
  if (keys.size() != values.size())
    qDebug() << Q_FUNC_INFO
             << "keys and values have different sizes:" << keys.size()
             << values.size();
  const int n = qMin(keys.size(), values.size());
  QVector<QCPBarsData> tempData(n);
  QVector<QCPBarsData>::iterator it = tempData.begin();
  const QVector<QCPBarsData>::iterator itEnd = tempData.end();
  int i = 0;
  while (it != itEnd) {
    it->key = keys[i];
    it->value = values[i];
    ++it;
    ++i;
  }
  mDataContainer->add(tempData,
                      alreadySorted);  // don't modify tempData beyond this to
                                       // prevent copy on write
}

/*! \overload
  Adds the provided data point as \a key and \a value to the current data.

  Alternatively, you can also access and modify the data directly via the \ref
  data method, which returns a pointer to the internal data container.
*/
void QCPBars::addData(double key, double value) {
  mDataContainer->add(QCPBarsData(key, value));
}

/*!
  Moves this bars plottable below \a bars. In other words, the bars of this
  plottable will appear below the bars of \a bars. The move target \a bars must
  use the same key and value axis as this plottable.

  Inserting into and removing from existing bar stacking is handled gracefully.
  If \a bars already has a bars object below itself, this bars object is
  inserted between the two. If this bars object is already between two other
  bars, the two other bars will be stacked on top of each other after the
  operation.

  To remove this bars plottable from any stacking, set \a bars to \c nullptr.

  \see moveBelow, barAbove, barBelow
*/
void QCPBars::moveBelow(QCPBars *bars) {
  if (bars == this) return;
  if (bars && (bars->keyAxis() != mKeyAxis.data() ||
               bars->valueAxis() != mValueAxis.data())) {
    qDebug() << Q_FUNC_INFO
             << "passed QCPBars* doesn't have same key and value axis as this "
                "QCPBars";
    return;
  }
  // remove from stacking:
  connectBars(
      mBarBelow.data(),
      mBarAbove.data());  // Note: also works if one (or both) of them is 0
  // if new bar given, insert this bar below it:
  if (bars) {
    if (bars->mBarBelow) connectBars(bars->mBarBelow.data(), this);
    connectBars(this, bars);
  }
}

/*!
  Moves this bars plottable above \a bars. In other words, the bars of this
  plottable will appear above the bars of \a bars. The move target \a bars must
  use the same key and value axis as this plottable.

  Inserting into and removing from existing bar stacking is handled gracefully.
  If \a bars already has a bars object above itself, this bars object is
  inserted between the two. If this bars object is already between two other
  bars, the two other bars will be stacked on top of each other after the
  operation.

  To remove this bars plottable from any stacking, set \a bars to \c nullptr.

  \see moveBelow, barBelow, barAbove
*/
void QCPBars::moveAbove(QCPBars *bars) {
  if (bars == this) return;
  if (bars && (bars->keyAxis() != mKeyAxis.data() ||
               bars->valueAxis() != mValueAxis.data())) {
    qDebug() << Q_FUNC_INFO
             << "passed QCPBars* doesn't have same key and value axis as this "
                "QCPBars";
    return;
  }
  // remove from stacking:
  connectBars(
      mBarBelow.data(),
      mBarAbove.data());  // Note: also works if one (or both) of them is 0
  // if new bar given, insert this bar above it:
  if (bars) {
    if (bars->mBarAbove) connectBars(this, bars->mBarAbove.data());
    connectBars(bars, this);
  }
}

/*!
  \copydoc QCPPlottableInterface1D::selectTestRect
*/
QCPDataSelection QCPBars::selectTestRect(const QRectF &rect,
                                         bool onlySelectable) const {
  QCPDataSelection result;
  if ((onlySelectable && mSelectable == QCP::stNone) ||
      mDataContainer->isEmpty())
    return result;
  if (!mKeyAxis || !mValueAxis) return result;

  QCPBarsDataContainer::const_iterator visibleBegin, visibleEnd;
  getVisibleDataBounds(visibleBegin, visibleEnd);

  for (QCPBarsDataContainer::const_iterator it = visibleBegin; it != visibleEnd;
       ++it) {
    if (rect.intersects(getBarRect(it->key, it->value)))
      result.addDataRange(
          QCPDataRange(int(it - mDataContainer->constBegin()),
                       int(it - mDataContainer->constBegin() + 1)),
          false);
  }
  result.simplify();
  return result;
}

/*!
  Implements a selectTest specific to this plottable's point geometry.

  If \a details is not 0, it will be set to a \ref QCPDataSelection, describing
  the closest data point to \a pos.

  \seebaseclassmethod \ref QCPAbstractPlottable::selectTest
*/
double QCPBars::selectTest(const QPointF &pos, bool onlySelectable,
                           QVariant *details) const {
  Q_UNUSED(details)
  if ((onlySelectable && mSelectable == QCP::stNone) ||
      mDataContainer->isEmpty())
    return -1;
  if (!mKeyAxis || !mValueAxis) return -1;

  if (mKeyAxis.data()->axisRect()->rect().contains(pos.toPoint()) ||
      mParentPlot->interactions().testFlag(
          QCP::iSelectPlottablesBeyondAxisRect)) {
    // get visible data range:
    QCPBarsDataContainer::const_iterator visibleBegin, visibleEnd;
    getVisibleDataBounds(visibleBegin, visibleEnd);
    for (QCPBarsDataContainer::const_iterator it = visibleBegin;
         it != visibleEnd; ++it) {
      if (getBarRect(it->key, it->value).contains(pos)) {
        if (details) {
          int pointIndex = int(it - mDataContainer->constBegin());
          details->setValue(
              QCPDataSelection(QCPDataRange(pointIndex, pointIndex + 1)));
        }
        return mParentPlot->selectionTolerance() * 0.99;
      }
    }
  }
  return -1;
}

/* inherits documentation from base class */
QCPRange QCPBars::getKeyRange(bool &foundRange,
                              QCP::SignDomain inSignDomain) const {
  /* Note: If this QCPBars uses absolute pixels as width (or is in a
QCPBarsGroup with spacing in absolute pixels), using this method to adapt the
key axis range to fit the bars into the currently visible axis range will not
work perfectly. Because in the moment the axis range is changed to the new
range, the fixed pixel widths/spacings will represent different coordinate
spans than before, which in turn would require a different key range to
perfectly fit, and so on. The only solution would be to iteratively approach
the perfect fitting axis range, but the mismatch isn't large enough in most
applications, to warrant this here. If a user does need a better fit, he
should call the corresponding axis rescale multiple times in a row.
*/
  QCPRange range;
  range = mDataContainer->keyRange(foundRange, inSignDomain);

  // determine exact range of bars by including bar width and barsgroup offset:
  if (foundRange && mKeyAxis) {
    double lowerPixelWidth, upperPixelWidth, keyPixel;
    // lower range bound:
    getPixelWidth(range.lower, lowerPixelWidth, upperPixelWidth);
    keyPixel = mKeyAxis.data()->coordToPixel(range.lower) + lowerPixelWidth;
    if (mBarsGroup) keyPixel += mBarsGroup->keyPixelOffset(this, range.lower);
    const double lowerCorrected = mKeyAxis.data()->pixelToCoord(keyPixel);
    if (!qIsNaN(lowerCorrected) && qIsFinite(lowerCorrected) &&
        range.lower > lowerCorrected)
      range.lower = lowerCorrected;
    // upper range bound:
    getPixelWidth(range.upper, lowerPixelWidth, upperPixelWidth);
    keyPixel = mKeyAxis.data()->coordToPixel(range.upper) + upperPixelWidth;
    if (mBarsGroup) keyPixel += mBarsGroup->keyPixelOffset(this, range.upper);
    const double upperCorrected = mKeyAxis.data()->pixelToCoord(keyPixel);
    if (!qIsNaN(upperCorrected) && qIsFinite(upperCorrected) &&
        range.upper < upperCorrected)
      range.upper = upperCorrected;
  }
  return range;
}

/* inherits documentation from base class */
QCPRange QCPBars::getValueRange(bool &foundRange, QCP::SignDomain inSignDomain,
                                const QCPRange &inKeyRange) const {
  // Note: can't simply use mDataContainer->valueRange here because we need to
  // take into account bar base value and possible stacking of multiple bars
  QCPRange range;
  range.lower = mBaseValue;
  range.upper = mBaseValue;
  bool haveLower = true;  // set to true, because baseValue should always be
                          // visible in bar charts
  bool haveUpper = true;  // set to true, because baseValue should always be
                          // visible in bar charts
  QCPBarsDataContainer::const_iterator itBegin = mDataContainer->constBegin();
  QCPBarsDataContainer::const_iterator itEnd = mDataContainer->constEnd();
  if (inKeyRange != QCPRange()) {
    itBegin = mDataContainer->findBegin(inKeyRange.lower, false);
    itEnd = mDataContainer->findEnd(inKeyRange.upper, false);
  }
  for (QCPBarsDataContainer::const_iterator it = itBegin; it != itEnd; ++it) {
    const double current =
        it->value + getStackedBaseValue(it->key, it->value >= 0);
    if (qIsNaN(current)) continue;
    if (inSignDomain == QCP::sdBoth ||
        (inSignDomain == QCP::sdNegative && current < 0) ||
        (inSignDomain == QCP::sdPositive && current > 0)) {
      if (current < range.lower || !haveLower) {
        range.lower = current;
        haveLower = true;
      }
      if (current > range.upper || !haveUpper) {
        range.upper = current;
        haveUpper = true;
      }
    }
  }

  foundRange =
      true;  // return true because bar charts always have the 0-line visible
  return range;
}

/* inherits documentation from base class */
QPointF QCPBars::dataPixelPosition(int index) const {
  if (index >= 0 && index < mDataContainer->size()) {
    QCPAxis *keyAxis = mKeyAxis.data();
    QCPAxis *valueAxis = mValueAxis.data();
    if (!keyAxis || !valueAxis) {
      qDebug() << Q_FUNC_INFO << "invalid key or value axis";
      return {};
    }

    const QCPDataContainer<QCPBarsData>::const_iterator it =
        mDataContainer->constBegin() + index;
    const double valuePixel = valueAxis->coordToPixel(
        getStackedBaseValue(it->key, it->value >= 0) + it->value);
    const double keyPixel =
        keyAxis->coordToPixel(it->key) +
        (mBarsGroup ? mBarsGroup->keyPixelOffset(this, it->key) : 0);
    if (keyAxis->orientation() == Qt::Horizontal)
      return {keyPixel, valuePixel};
    else
      return {valuePixel, keyPixel};
  } else {
    qDebug() << Q_FUNC_INFO << "Index out of bounds" << index;
    return {};
  }
}

/* inherits documentation from base class */
void QCPBars::draw(QCPPainter *painter) {
  if (!mKeyAxis || !mValueAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    return;
  }
  if (mDataContainer->isEmpty()) return;

  QCPBarsDataContainer::const_iterator visibleBegin, visibleEnd;
  getVisibleDataBounds(visibleBegin, visibleEnd);

  // loop over and draw segments of unselected/selected data:
  QList<QCPDataRange> selectedSegments, unselectedSegments, allSegments;
  getDataSegments(selectedSegments, unselectedSegments);
  allSegments << unselectedSegments << selectedSegments;
  for (int i = 0; i < allSegments.size(); ++i) {
    bool isSelectedSegment = i >= unselectedSegments.size();
    QCPBarsDataContainer::const_iterator begin = visibleBegin;
    QCPBarsDataContainer::const_iterator end = visibleEnd;
    mDataContainer->limitIteratorsToDataRange(begin, end, allSegments.at(i));
    if (begin == end) continue;

    for (QCPBarsDataContainer::const_iterator it = begin; it != end; ++it) {
      // check data validity if flag set:
#ifdef QCUSTOMPLOT_CHECK_DATA
      if (QCP::isInvalidData(it->key, it->value))
        qDebug() << Q_FUNC_INFO << "Data point at" << it->key
                 << "of drawn range invalid."
                 << "Plottable name:" << name();
#endif
      // draw bar:
      if (isSelectedSegment && mSelectionDecorator) {
        mSelectionDecorator->applyBrush(painter);
        mSelectionDecorator->applyPen(painter);
      } else {
        painter->setBrush(mBrush);
        painter->setPen(mPen);
      }
      applyDefaultAntialiasingHint(painter);
      painter->drawPolygon(getBarRect(it->key, it->value));
    }
  }

  // draw other selection decoration that isn't just line/scatter pens and
  // brushes:
  if (mSelectionDecorator)
    mSelectionDecorator->drawDecoration(painter, selection());
}

/* inherits documentation from base class */
void QCPBars::drawLegendIcon(QCPPainter *painter, const QRectF &rect) const {
  // draw filled rect:
  applyDefaultAntialiasingHint(painter);
  painter->setBrush(mBrush);
  painter->setPen(mPen);
  QRectF r = QRectF(0, 0, rect.width() * 0.67, rect.height() * 0.67);
  r.moveCenter(rect.center());
  painter->drawRect(r);
}

/*!  \internal

  called by \ref draw to determine which data (key) range is visible at the
  current key axis range setting, so only that needs to be processed. It also
  takes into account the bar width.

  \a begin returns an iterator to the lowest data point that needs to be taken
  into account when plotting. Note that in order to get a clean plot all the way
  to the edge of the axis rect, \a lower may still be just outside the visible
  range.

  \a end returns an iterator one higher than the highest visible data point.
  Same as before, \a end may also lie just outside of the visible range.

  if the plottable contains no data, both \a begin and \a end point to constEnd.
*/
void QCPBars::getVisibleDataBounds(
    QCPBarsDataContainer::const_iterator &begin,
    QCPBarsDataContainer::const_iterator &end) const {
  if (!mKeyAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key axis";
    begin = mDataContainer->constEnd();
    end = mDataContainer->constEnd();
    return;
  }
  if (mDataContainer->isEmpty()) {
    begin = mDataContainer->constEnd();
    end = mDataContainer->constEnd();
    return;
  }

  // get visible data range as QMap iterators
  begin = mDataContainer->findBegin(mKeyAxis.data()->range().lower);
  end = mDataContainer->findEnd(mKeyAxis.data()->range().upper);
  double lowerPixelBound =
      mKeyAxis.data()->coordToPixel(mKeyAxis.data()->range().lower);
  double upperPixelBound =
      mKeyAxis.data()->coordToPixel(mKeyAxis.data()->range().upper);
  bool isVisible = false;
  // walk left from begin to find lower bar that actually is completely outside
  // visible pixel range:
  QCPBarsDataContainer::const_iterator it = begin;
  while (it != mDataContainer->constBegin()) {
    --it;
    const QRectF barRect = getBarRect(it->key, it->value);
    if (mKeyAxis.data()->orientation() == Qt::Horizontal)
      isVisible = ((!mKeyAxis.data()->rangeReversed() &&
                    barRect.right() >= lowerPixelBound) ||
                   (mKeyAxis.data()->rangeReversed() &&
                    barRect.left() <= lowerPixelBound));
    else  // keyaxis is vertical
      isVisible = ((!mKeyAxis.data()->rangeReversed() &&
                    barRect.top() <= lowerPixelBound) ||
                   (mKeyAxis.data()->rangeReversed() &&
                    barRect.bottom() >= lowerPixelBound));
    if (isVisible)
      begin = it;
    else
      break;
  }
  // walk right from ubound to find upper bar that actually is completely
  // outside visible pixel range:
  it = end;
  while (it != mDataContainer->constEnd()) {
    const QRectF barRect = getBarRect(it->key, it->value);
    if (mKeyAxis.data()->orientation() == Qt::Horizontal)
      isVisible = ((!mKeyAxis.data()->rangeReversed() &&
                    barRect.left() <= upperPixelBound) ||
                   (mKeyAxis.data()->rangeReversed() &&
                    barRect.right() >= upperPixelBound));
    else  // keyaxis is vertical
      isVisible = ((!mKeyAxis.data()->rangeReversed() &&
                    barRect.bottom() >= upperPixelBound) ||
                   (mKeyAxis.data()->rangeReversed() &&
                    barRect.top() <= upperPixelBound));
    if (isVisible)
      end = it + 1;
    else
      break;
    ++it;
  }
}

/*! \internal

  Returns the rect in pixel coordinates of a single bar with the specified \a
  key and \a value. The rect is shifted according to the bar stacking (see \ref
  moveAbove) and base value (see \ref setBaseValue), and to have non-overlapping
  border lines with the bars stacked below.
*/
QRectF QCPBars::getBarRect(double key, double value) const {
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    return {};
  }

  double lowerPixelWidth, upperPixelWidth;
  getPixelWidth(key, lowerPixelWidth, upperPixelWidth);
  double base = getStackedBaseValue(key, value >= 0);
  double basePixel = valueAxis->coordToPixel(base);
  double valuePixel = valueAxis->coordToPixel(base + value);
  double keyPixel = keyAxis->coordToPixel(key);
  if (mBarsGroup) keyPixel += mBarsGroup->keyPixelOffset(this, key);
  double bottomOffset = (mBarBelow && mPen != Qt::NoPen ? 1 : 0) *
                        (mPen.isCosmetic() ? 1 : mPen.widthF());
  bottomOffset += mBarBelow ? mStackingGap : 0;
  bottomOffset *= (value < 0 ? -1 : 1) * valueAxis->pixelOrientation();
  if (qAbs(valuePixel - basePixel) <= qAbs(bottomOffset))
    bottomOffset = valuePixel - basePixel;
  if (keyAxis->orientation() == Qt::Horizontal) {
    return QRectF(QPointF(keyPixel + lowerPixelWidth, valuePixel),
                  QPointF(keyPixel + upperPixelWidth, basePixel + bottomOffset))
        .normalized();
  } else {
    return QRectF(QPointF(basePixel + bottomOffset, keyPixel + lowerPixelWidth),
                  QPointF(valuePixel, keyPixel + upperPixelWidth))
        .normalized();
  }
}

/*! \internal

  This function is used to determine the width of the bar at coordinate \a key,
  according to the specified width (\ref setWidth) and width type (\ref
  setWidthType).

  The output parameters \a lower and \a upper return the number of pixels the
  bar extends to lower and higher keys, relative to the \a key coordinate (so
  with a non-reversed horizontal axis, \a lower is negative and \a upper
  positive).
*/
void QCPBars::getPixelWidth(double key, double &lower, double &upper) const {
  lower = 0;
  upper = 0;
  switch (mWidthType) {
    case wtAbsolute: {
      upper = mWidth * 0.5 * mKeyAxis.data()->pixelOrientation();
      lower = -upper;
      break;
    }
    case wtAxisRectRatio: {
      if (mKeyAxis && mKeyAxis.data()->axisRect()) {
        if (mKeyAxis.data()->orientation() == Qt::Horizontal)
          upper = mKeyAxis.data()->axisRect()->width() * mWidth * 0.5 *
                  mKeyAxis.data()->pixelOrientation();
        else
          upper = mKeyAxis.data()->axisRect()->height() * mWidth * 0.5 *
                  mKeyAxis.data()->pixelOrientation();
        lower = -upper;
      } else
        qDebug() << Q_FUNC_INFO << "No key axis or axis rect defined";
      break;
    }
    case wtPlotCoords: {
      if (mKeyAxis) {
        double keyPixel = mKeyAxis.data()->coordToPixel(key);
        upper = mKeyAxis.data()->coordToPixel(key + mWidth * 0.5) - keyPixel;
        lower = mKeyAxis.data()->coordToPixel(key - mWidth * 0.5) - keyPixel;
        // no need to qSwap(lower, higher) when range reversed, because
        // higher/lower are gained by coordinate transform which includes range
        // direction
      } else
        qDebug() << Q_FUNC_INFO << "No key axis defined";
      break;
    }
  }
}

/*! \internal

  This function is called to find at which value to start drawing the base of a
  bar at \a key, when it is stacked on top of another QCPBars (e.g. with \ref
  moveAbove).

  positive and negative bars are separated per stack (positive are stacked above
  baseValue upwards, negative are stacked below baseValue downwards). This can
  be indicated with \a positive. So if the bar for which we need the base value
  is negative, set \a positive to false.
*/
double QCPBars::getStackedBaseValue(double key, bool positive) const {
  if (mBarBelow) {
    double max = 0;  // don't initialize with mBaseValue here because only base
                     // value of bottom-most bar has meaning in a bar stack
    // find bars of mBarBelow that are approximately at key and find largest
    // one:
    double epsilon =
        qAbs(key) *
        (sizeof(key) == 4 ? 1e-6 : 1e-14);  // should be safe even when changed
                                            // to use float at some point
    if (key == 0) epsilon = (sizeof(key) == 4 ? 1e-6 : 1e-14);
    QCPBarsDataContainer::const_iterator it =
        mBarBelow.data()->mDataContainer->findBegin(key - epsilon);
    QCPBarsDataContainer::const_iterator itEnd =
        mBarBelow.data()->mDataContainer->findEnd(key + epsilon);
    while (it != itEnd) {
      if (it->key > key - epsilon && it->key < key + epsilon) {
        if ((positive && it->value > max) || (!positive && it->value < max))
          max = it->value;
      }
      ++it;
    }
    // recurse down the bar-stack to find the total height:
    return max + mBarBelow.data()->getStackedBaseValue(key, positive);
  } else
    return mBaseValue;
}

/*! \internal

  Connects \a below and \a above to each other via their mBarAbove/mBarBelow
  properties. The bar(s) currently above lower and below upper will become
  disconnected to lower/upper.

  If lower is zero, upper will be disconnected at the bottom.
  If upper is zero, lower will be disconnected at the top.
*/
void QCPBars::connectBars(QCPBars *lower, QCPBars *upper) {
  if (!lower && !upper) return;

  if (!lower)  // disconnect upper at bottom
  {
    // disconnect old bar below upper:
    if (upper->mBarBelow && upper->mBarBelow.data()->mBarAbove.data() == upper)
      upper->mBarBelow.data()->mBarAbove = nullptr;
    upper->mBarBelow = nullptr;
  } else if (!upper)  // disconnect lower at top
  {
    // disconnect old bar above lower:
    if (lower->mBarAbove && lower->mBarAbove.data()->mBarBelow.data() == lower)
      lower->mBarAbove.data()->mBarBelow = nullptr;
    lower->mBarAbove = nullptr;
  } else  // connect lower and upper
  {
    // disconnect old bar above lower:
    if (lower->mBarAbove && lower->mBarAbove.data()->mBarBelow.data() == lower)
      lower->mBarAbove.data()->mBarBelow = nullptr;
    // disconnect old bar below upper:
    if (upper->mBarBelow && upper->mBarBelow.data()->mBarAbove.data() == upper)
      upper->mBarBelow.data()->mBarAbove = nullptr;
    lower->mBarAbove = upper;
    upper->mBarBelow = lower;
  }
}
/* end of 'src/plottables/plottable-bars.cpp' */

/* including file 'src/plottables/plottable-statisticalbox.cpp' */
/* modified 2022-11-06T12:45:57, size 28951                     */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPStatisticalBoxData
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPStatisticalBoxData
  \brief Holds the data of one single data point for QCPStatisticalBox.

  The stored data is:

  \li \a key: coordinate on the key axis of this data point (this is the \a
  mainKey and the \a sortKey)

  \li \a minimum: the position of the lower whisker, typically the minimum
  measurement of the sample that's not considered an outlier.

  \li \a lowerQuartile: the lower end of the box. The lower and the upper
  quartiles are the two statistical quartiles around the median of the sample,
  they should contain 50% of the sample data.

  \li \a median: the value of the median mark inside the quartile box. The
  median separates the sample data in half (50% of the sample data is
  below/above the median). (This is the \a mainValue)

  \li \a upperQuartile: the upper end of the box. The lower and the upper
  quartiles are the two statistical quartiles around the median of the sample,
  they should contain 50% of the sample data.

  \li \a maximum: the position of the upper whisker, typically the maximum
  measurement of the sample that's not considered an outlier.

  \li \a outliers: a QVector of outlier values that will be drawn as scatter
  points at the \a key coordinate of this data point (see \ref
  QCPStatisticalBox::setOutlierStyle)

  The container for storing multiple data points is \ref
  QCPStatisticalBoxDataContainer. It is a typedef for \ref QCPDataContainer with
  \ref QCPStatisticalBoxData as the DataType template parameter. See the
  documentation there for an explanation regarding the data type's generic
  methods.

  \see QCPStatisticalBoxDataContainer
*/

/* start documentation of inline functions */

/*! \fn double QCPStatisticalBoxData::sortKey() const

  Returns the \a key member of this data point.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/*! \fn static QCPStatisticalBoxData QCPStatisticalBoxData::fromSortKey(double
  sortKey)

  Returns a data point with the specified \a sortKey. All other members are set
  to zero.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/*! \fn static static bool QCPStatisticalBoxData::sortKeyIsMainKey()

  Since the member \a key is both the data point key coordinate and the data
  ordering parameter, this method returns true.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/*! \fn double QCPStatisticalBoxData::mainKey() const

  Returns the \a key member of this data point.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/*! \fn double QCPStatisticalBoxData::mainValue() const

  Returns the \a median member of this data point.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/*! \fn QCPRange QCPStatisticalBoxData::valueRange() const

  Returns a QCPRange spanning from the \a minimum to the \a maximum member of
  this statistical box data point, possibly further expanded by outliers.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/* end documentation of inline functions */

/*!
  Constructs a data point with key and all values set to zero.
*/
QCPStatisticalBoxData::QCPStatisticalBoxData()
    : key(0),
      minimum(0),
      lowerQuartile(0),
      median(0),
      upperQuartile(0),
      maximum(0) {}

/*!
  Constructs a data point with the specified \a key, \a minimum, \a
  lowerQuartile, \a median, \a upperQuartile, \a maximum and optionally a number
  of \a outliers.
*/
QCPStatisticalBoxData::QCPStatisticalBoxData(
    double key, double minimum, double lowerQuartile, double median,
    double upperQuartile, double maximum, const QVector<double> &outliers)
    : key(key),
      minimum(minimum),
      lowerQuartile(lowerQuartile),
      median(median),
      upperQuartile(upperQuartile),
      maximum(maximum),
      outliers(outliers) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPStatisticalBox
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPStatisticalBox
  \brief A plottable representing a single statistical box in a plot.

  \image html QCPStatisticalBox.png

  To plot data, assign it with the \ref setData or \ref addData functions.
  Alternatively, you can also access and modify the data via the \ref data
  method, which returns a pointer to the internal \ref
  QCPStatisticalBoxDataContainer.

  Additionally each data point can itself have a list of outliers, drawn as
  scatter points at the key coordinate of the respective statistical box data
  point. They can either be set by using the respective \ref
  addData(double,double,double,double,double,double,const QVector<double>&)
  "addData" method or accessing the individual data points through \ref data,
  and setting the <tt>QVector<double> outliers</tt> of the data points directly.

  \section qcpstatisticalbox-appearance Changing the appearance

  The appearance of each data point box, ranging from the lower to the upper
  quartile, is controlled via \ref setPen and \ref setBrush. You may change the
  width of the boxes with \ref setWidth in plot coordinates.

  Each data point's visual representation also consists of two whiskers.
  Whiskers are the lines which reach from the upper quartile to the maximum, and
  from the lower quartile to the minimum. The appearance of the whiskers can be
  modified with: \ref setWhiskerPen, \ref setWhiskerBarPen, \ref
  setWhiskerWidth. The whisker width is the width of the bar perpendicular to
  the whisker at the top (for maximum) and bottom (for minimum). If the whisker
  pen is changed, make sure to set the \c capStyle to \c Qt::FlatCap. Otherwise
  the backbone line might exceed the whisker bars by a few pixels due to the pen
  cap being not perfectly flat.

  The median indicator line inside the box has its own pen, \ref setMedianPen.

  The outlier data points are drawn as normal scatter points. Their look can be
  controlled with \ref setOutlierStyle

  \section qcpstatisticalbox-usage Usage

  Like all data representing objects in QCustomPlot, the QCPStatisticalBox is a
  plottable (QCPAbstractPlottable). So the plottable-interface of QCustomPlot
  applies (QCustomPlot::plottable, QCustomPlot::removePlottable, etc.)

  Usually, you first create an instance:
  \snippet documentation/doc-code-snippets/mainwindow.cpp
  qcpstatisticalbox-creation-1 which registers it with the QCustomPlot instance
  of the passed axes. Note that this QCustomPlot instance takes ownership of the
  plottable, so do not delete it manually but use QCustomPlot::removePlottable()
  instead. The newly created plottable can be modified, e.g.: \snippet
  documentation/doc-code-snippets/mainwindow.cpp qcpstatisticalbox-creation-2
*/

/* start documentation of inline functions */

/*! \fn QSharedPointer<QCPStatisticalBoxDataContainer> QCPStatisticalBox::data()
  const

  Returns a shared pointer to the internal data storage of type \ref
  QCPStatisticalBoxDataContainer. You may use it to directly manipulate the
  data, which may be more convenient and faster than using the regular \ref
  setData or \ref addData methods.
*/

/* end documentation of inline functions */

/*!
  Constructs a statistical box which uses \a keyAxis as its key axis ("x") and
  \a valueAxis as its value axis ("y"). \a keyAxis and \a valueAxis must reside
  in the same QCustomPlot instance and not have the same orientation. If either
  of these restrictions is violated, a corresponding message is printed to the
  debug output (qDebug), the construction is not aborted, though.

  The created QCPStatisticalBox is automatically registered with the QCustomPlot
  instance inferred from \a keyAxis. This QCustomPlot instance takes ownership
  of the QCPStatisticalBox, so do not delete it manually but use
  QCustomPlot::removePlottable() instead.
*/
QCPStatisticalBox::QCPStatisticalBox(QCPAxis *keyAxis, QCPAxis *valueAxis)
    : QCPAbstractPlottable1D<QCPStatisticalBoxData>(keyAxis, valueAxis),
      mWidth(0.5),
      mWhiskerWidth(0.2),
      mWhiskerPen(Qt::black, 0, Qt::DashLine, Qt::FlatCap),
      mWhiskerBarPen(Qt::black),
      mWhiskerAntialiased(false),
      mMedianPen(Qt::black, 3, Qt::SolidLine, Qt::FlatCap),
      mOutlierStyle(QCPScatterStyle::ssCircle, Qt::blue, 6) {
  setPen(QPen(Qt::black));
  setBrush(Qt::NoBrush);
}

/*! \overload

  Replaces the current data container with the provided \a data container.

  Since a QSharedPointer is used, multiple QCPStatisticalBoxes may share the
  same data container safely. Modifying the data in the container will then
  affect all statistical boxes that share the container. Sharing can be achieved
  by simply exchanging the data containers wrapped in shared pointers: \snippet
  documentation/doc-code-snippets/mainwindow.cpp qcpstatisticalbox-datasharing-1

  If you do not wish to share containers, but create a copy from an existing
  container, rather use the \ref QCPDataContainer<DataType>::set method on the
  statistical box data container directly: \snippet
  documentation/doc-code-snippets/mainwindow.cpp qcpstatisticalbox-datasharing-2

  \see addData
*/
void QCPStatisticalBox::setData(
    QSharedPointer<QCPStatisticalBoxDataContainer> data) {
  mDataContainer = data;
}
/*! \overload

  Replaces the current data with the provided points in \a keys, \a minimum, \a
  lowerQuartile, \a median, \a upperQuartile and \a maximum. The provided
  vectors should have equal length. Else, the number of added points will be the
  size of the smallest vector.

  If you can guarantee that the passed data points are sorted by \a keys in
  ascending order, you can set \a alreadySorted to true, to improve performance
  by saving a sorting run.

  \see addData
*/
void QCPStatisticalBox::setData(const QVector<double> &keys,
                                const QVector<double> &minimum,
                                const QVector<double> &lowerQuartile,
                                const QVector<double> &median,
                                const QVector<double> &upperQuartile,
                                const QVector<double> &maximum,
                                bool alreadySorted) {
  mDataContainer->clear();
  addData(keys, minimum, lowerQuartile, median, upperQuartile, maximum,
          alreadySorted);
}

/*!
  Sets the width of the boxes in key coordinates.

  \see setWhiskerWidth
*/
void QCPStatisticalBox::setWidth(double width) { mWidth = width; }

/*!
  Sets the width of the whiskers in key coordinates.

  Whiskers are the lines which reach from the upper quartile to the maximum, and
  from the lower quartile to the minimum.

  \see setWidth
*/
void QCPStatisticalBox::setWhiskerWidth(double width) { mWhiskerWidth = width; }

/*!
  Sets the pen used for drawing the whisker backbone.

  Whiskers are the lines which reach from the upper quartile to the maximum, and
  from the lower quartile to the minimum.

  Make sure to set the \c capStyle of the passed \a pen to \c Qt::FlatCap.
  Otherwise the backbone line might exceed the whisker bars by a few pixels due
  to the pen cap being not perfectly flat.

  \see setWhiskerBarPen
*/
void QCPStatisticalBox::setWhiskerPen(const QPen &pen) { mWhiskerPen = pen; }

/*!
  Sets the pen used for drawing the whisker bars. Those are the lines parallel
  to the key axis at each end of the whisker backbone.

  Whiskers are the lines which reach from the upper quartile to the maximum, and
  from the lower quartile to the minimum.

  \see setWhiskerPen
*/
void QCPStatisticalBox::setWhiskerBarPen(const QPen &pen) {
  mWhiskerBarPen = pen;
}

/*!
  Sets whether the statistical boxes whiskers are drawn with antialiasing or
  not.

  Note that antialiasing settings may be overridden by
  QCustomPlot::setAntialiasedElements and
  QCustomPlot::setNotAntialiasedElements.
*/
void QCPStatisticalBox::setWhiskerAntialiased(bool enabled) {
  mWhiskerAntialiased = enabled;
}

/*!
  Sets the pen used for drawing the median indicator line inside the statistical
  boxes.
*/
void QCPStatisticalBox::setMedianPen(const QPen &pen) { mMedianPen = pen; }

/*!
  Sets the appearance of the outlier data points.

  Outliers can be specified with the method
  \ref addData(double key, double minimum, double lowerQuartile, double median,
  double upperQuartile, double maximum, const QVector<double> &outliers)
*/
void QCPStatisticalBox::setOutlierStyle(const QCPScatterStyle &style) {
  mOutlierStyle = style;
}

/*! \overload

  Adds the provided points in \a keys, \a minimum, \a lowerQuartile, \a median,
  \a upperQuartile and \a maximum to the current data. The provided vectors
  should have equal length. Else, the number of added points will be the size of
  the smallest vector.

  If you can guarantee that the passed data points are sorted by \a keys in
  ascending order, you can set \a alreadySorted to true, to improve performance
  by saving a sorting run.

  Alternatively, you can also access and modify the data directly via the \ref
  data method, which returns a pointer to the internal data container.
*/
void QCPStatisticalBox::addData(const QVector<double> &keys,
                                const QVector<double> &minimum,
                                const QVector<double> &lowerQuartile,
                                const QVector<double> &median,
                                const QVector<double> &upperQuartile,
                                const QVector<double> &maximum,
                                bool alreadySorted) {
  if (keys.size() != minimum.size() || minimum.size() != lowerQuartile.size() ||
      lowerQuartile.size() != median.size() ||
      median.size() != upperQuartile.size() ||
      upperQuartile.size() != maximum.size() || maximum.size() != keys.size())
    qDebug() << Q_FUNC_INFO
             << "keys, minimum, lowerQuartile, median, upperQuartile, maximum "
                "have different sizes:"
             << keys.size() << minimum.size() << lowerQuartile.size()
             << median.size() << upperQuartile.size() << maximum.size();
  const int n = qMin(
      keys.size(),
      qMin(minimum.size(), qMin(lowerQuartile.size(),
                                qMin(median.size(), qMin(upperQuartile.size(),
                                                         maximum.size())))));
  QVector<QCPStatisticalBoxData> tempData(n);
  QVector<QCPStatisticalBoxData>::iterator it = tempData.begin();
  const QVector<QCPStatisticalBoxData>::iterator itEnd = tempData.end();
  int i = 0;
  while (it != itEnd) {
    it->key = keys[i];
    it->minimum = minimum[i];
    it->lowerQuartile = lowerQuartile[i];
    it->median = median[i];
    it->upperQuartile = upperQuartile[i];
    it->maximum = maximum[i];
    ++it;
    ++i;
  }
  mDataContainer->add(tempData,
                      alreadySorted);  // don't modify tempData beyond this to
                                       // prevent copy on write
}

/*! \overload

  Adds the provided data point as \a key, \a minimum, \a lowerQuartile, \a
  median, \a upperQuartile and \a maximum to the current data.

  Alternatively, you can also access and modify the data directly via the \ref
  data method, which returns a pointer to the internal data container.
*/
void QCPStatisticalBox::addData(double key, double minimum,
                                double lowerQuartile, double median,
                                double upperQuartile, double maximum,
                                const QVector<double> &outliers) {
  mDataContainer->add(QCPStatisticalBoxData(key, minimum, lowerQuartile, median,
                                            upperQuartile, maximum, outliers));
}

/*!
  \copydoc QCPPlottableInterface1D::selectTestRect
*/
QCPDataSelection QCPStatisticalBox::selectTestRect(const QRectF &rect,
                                                   bool onlySelectable) const {
  QCPDataSelection result;
  if ((onlySelectable && mSelectable == QCP::stNone) ||
      mDataContainer->isEmpty())
    return result;
  if (!mKeyAxis || !mValueAxis) return result;

  QCPStatisticalBoxDataContainer::const_iterator visibleBegin, visibleEnd;
  getVisibleDataBounds(visibleBegin, visibleEnd);

  for (QCPStatisticalBoxDataContainer::const_iterator it = visibleBegin;
       it != visibleEnd; ++it) {
    if (rect.intersects(getQuartileBox(it)))
      result.addDataRange(
          QCPDataRange(int(it - mDataContainer->constBegin()),
                       int(it - mDataContainer->constBegin() + 1)),
          false);
  }
  result.simplify();
  return result;
}

/*!
  Implements a selectTest specific to this plottable's point geometry.

  If \a details is not 0, it will be set to a \ref QCPDataSelection, describing
  the closest data point to \a pos.

  \seebaseclassmethod \ref QCPAbstractPlottable::selectTest
*/
double QCPStatisticalBox::selectTest(const QPointF &pos, bool onlySelectable,
                                     QVariant *details) const {
  Q_UNUSED(details)
  if ((onlySelectable && mSelectable == QCP::stNone) ||
      mDataContainer->isEmpty())
    return -1;
  if (!mKeyAxis || !mValueAxis) return -1;

  if (mKeyAxis->axisRect()->rect().contains(pos.toPoint()) ||
      mParentPlot->interactions().testFlag(
          QCP::iSelectPlottablesBeyondAxisRect)) {
    // get visible data range:
    QCPStatisticalBoxDataContainer::const_iterator visibleBegin, visibleEnd;
    QCPStatisticalBoxDataContainer::const_iterator closestDataPoint =
        mDataContainer->constEnd();
    getVisibleDataBounds(visibleBegin, visibleEnd);
    double minDistSqr = (std::numeric_limits<double>::max)();
    for (QCPStatisticalBoxDataContainer::const_iterator it = visibleBegin;
         it != visibleEnd; ++it) {
      if (getQuartileBox(it).contains(pos))  // quartile box
      {
        double currentDistSqr = mParentPlot->selectionTolerance() * 0.99 *
                                mParentPlot->selectionTolerance() * 0.99;
        if (currentDistSqr < minDistSqr) {
          minDistSqr = currentDistSqr;
          closestDataPoint = it;
        }
      } else  // whiskers
      {
        const QVector<QLineF> whiskerBackbones = getWhiskerBackboneLines(it);
        const QCPVector2D posVec(pos);
        foreach (const QLineF &backbone, whiskerBackbones) {
          double currentDistSqr = posVec.distanceSquaredToLine(backbone);
          if (currentDistSqr < minDistSqr) {
            minDistSqr = currentDistSqr;
            closestDataPoint = it;
          }
        }
      }
    }
    if (details) {
      int pointIndex = int(closestDataPoint - mDataContainer->constBegin());
      details->setValue(
          QCPDataSelection(QCPDataRange(pointIndex, pointIndex + 1)));
    }
    return qSqrt(minDistSqr);
  }
  return -1;
}

/* inherits documentation from base class */
QCPRange QCPStatisticalBox::getKeyRange(bool &foundRange,
                                        QCP::SignDomain inSignDomain) const {
  QCPRange range = mDataContainer->keyRange(foundRange, inSignDomain);
  // determine exact range by including width of bars/flags:
  if (foundRange) {
    if (inSignDomain != QCP::sdPositive || range.lower - mWidth * 0.5 > 0)
      range.lower -= mWidth * 0.5;
    if (inSignDomain != QCP::sdNegative || range.upper + mWidth * 0.5 < 0)
      range.upper += mWidth * 0.5;
  }
  return range;
}

/* inherits documentation from base class */
QCPRange QCPStatisticalBox::getValueRange(bool &foundRange,
                                          QCP::SignDomain inSignDomain,
                                          const QCPRange &inKeyRange) const {
  return mDataContainer->valueRange(foundRange, inSignDomain, inKeyRange);
}

/* inherits documentation from base class */
void QCPStatisticalBox::draw(QCPPainter *painter) {
  if (mDataContainer->isEmpty()) return;
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    return;
  }

  QCPStatisticalBoxDataContainer::const_iterator visibleBegin, visibleEnd;
  getVisibleDataBounds(visibleBegin, visibleEnd);

  // loop over and draw segments of unselected/selected data:
  QList<QCPDataRange> selectedSegments, unselectedSegments, allSegments;
  getDataSegments(selectedSegments, unselectedSegments);
  allSegments << unselectedSegments << selectedSegments;
  for (int i = 0; i < allSegments.size(); ++i) {
    bool isSelectedSegment = i >= unselectedSegments.size();
    QCPStatisticalBoxDataContainer::const_iterator begin = visibleBegin;
    QCPStatisticalBoxDataContainer::const_iterator end = visibleEnd;
    mDataContainer->limitIteratorsToDataRange(begin, end, allSegments.at(i));
    if (begin == end) continue;

    for (QCPStatisticalBoxDataContainer::const_iterator it = begin; it != end;
         ++it) {
      // check data validity if flag set:
#ifdef QCUSTOMPLOT_CHECK_DATA
      if (QCP::isInvalidData(it->key, it->minimum) ||
          QCP::isInvalidData(it->lowerQuartile, it->median) ||
          QCP::isInvalidData(it->upperQuartile, it->maximum))
        qDebug() << Q_FUNC_INFO << "Data point at" << it->key
                 << "of drawn range has invalid data."
                 << "Plottable name:" << name();
      for (int i = 0; i < it->outliers.size(); ++i)
        if (QCP::isInvalidData(it->outliers.at(i)))
          qDebug() << Q_FUNC_INFO << "Data point outlier at" << it->key
                   << "of drawn range invalid."
                   << "Plottable name:" << name();
#endif

      if (isSelectedSegment && mSelectionDecorator) {
        mSelectionDecorator->applyPen(painter);
        mSelectionDecorator->applyBrush(painter);
      } else {
        painter->setPen(mPen);
        painter->setBrush(mBrush);
      }
      QCPScatterStyle finalOutlierStyle = mOutlierStyle;
      if (isSelectedSegment && mSelectionDecorator)
        finalOutlierStyle =
            mSelectionDecorator->getFinalScatterStyle(mOutlierStyle);
      drawStatisticalBox(painter, it, finalOutlierStyle);
    }
  }

  // draw other selection decoration that isn't just line/scatter pens and
  // brushes:
  if (mSelectionDecorator)
    mSelectionDecorator->drawDecoration(painter, selection());
}

/* inherits documentation from base class */
void QCPStatisticalBox::drawLegendIcon(QCPPainter *painter,
                                       const QRectF &rect) const {
  // draw filled rect:
  applyDefaultAntialiasingHint(painter);
  painter->setPen(mPen);
  painter->setBrush(mBrush);
  QRectF r = QRectF(0, 0, rect.width() * 0.67, rect.height() * 0.67);
  r.moveCenter(rect.center());
  painter->drawRect(r);
}

/*!
  Draws the graphical representation of a single statistical box with the data
  given by the iterator \a it with the provided \a painter.

  If the statistical box has a set of outlier data points, they are drawn with
  \a outlierStyle.

  \see getQuartileBox, getWhiskerBackboneLines, getWhiskerBarLines
*/
void QCPStatisticalBox::drawStatisticalBox(
    QCPPainter *painter, QCPStatisticalBoxDataContainer::const_iterator it,
    const QCPScatterStyle &outlierStyle) const {
  // draw quartile box:
  applyDefaultAntialiasingHint(painter);
  const QRectF quartileBox = getQuartileBox(it);
  painter->drawRect(quartileBox);
  // draw median line with cliprect set to quartile box:
  painter->save();
  painter->setClipRect(quartileBox, Qt::IntersectClip);
  painter->setPen(mMedianPen);
  painter->drawLine(QLineF(coordsToPixels(it->key - mWidth * 0.5, it->median),
                           coordsToPixels(it->key + mWidth * 0.5, it->median)));
  painter->restore();
  // draw whisker lines:
  applyAntialiasingHint(painter, mWhiskerAntialiased, QCP::aePlottables);
  painter->setPen(mWhiskerPen);
  painter->drawLines(getWhiskerBackboneLines(it));
  painter->setPen(mWhiskerBarPen);
  painter->drawLines(getWhiskerBarLines(it));
  // draw outliers:
  applyScattersAntialiasingHint(painter);
  outlierStyle.applyTo(painter, mPen);
  for (int i = 0; i < it->outliers.size(); ++i)
    outlierStyle.drawShape(painter,
                           coordsToPixels(it->key, it->outliers.at(i)));
}

/*!  \internal

  called by \ref draw to determine which data (key) range is visible at the
  current key axis range setting, so only that needs to be processed. It also
  takes into account the bar width.

  \a begin returns an iterator to the lowest data point that needs to be taken
  into account when plotting. Note that in order to get a clean plot all the way
  to the edge of the axis rect, \a lower may still be just outside the visible
  range.

  \a end returns an iterator one higher than the highest visible data point.
  Same as before, \a end may also lie just outside of the visible range.

  if the plottable contains no data, both \a begin and \a end point to constEnd.
*/
void QCPStatisticalBox::getVisibleDataBounds(
    QCPStatisticalBoxDataContainer::const_iterator &begin,
    QCPStatisticalBoxDataContainer::const_iterator &end) const {
  if (!mKeyAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key axis";
    begin = mDataContainer->constEnd();
    end = mDataContainer->constEnd();
    return;
  }
  begin = mDataContainer->findBegin(
      mKeyAxis.data()->range().lower -
      mWidth * 0.5);  // subtract half width of box to include partially visible
                      // data points
  end = mDataContainer->findEnd(mKeyAxis.data()->range().upper +
                                mWidth *
                                    0.5);  // add half width of box to include
                                           // partially visible data points
}

/*!  \internal

  Returns the box in plot coordinates (keys in x, values in y of the returned
  rect) that covers the value range from the lower to the upper quartile, of the
  data given by \a it.

  \see drawStatisticalBox, getWhiskerBackboneLines, getWhiskerBarLines
*/
QRectF QCPStatisticalBox::getQuartileBox(
    QCPStatisticalBoxDataContainer::const_iterator it) const {
  QRectF result;
  result.setTopLeft(coordsToPixels(it->key - mWidth * 0.5, it->upperQuartile));
  result.setBottomRight(
      coordsToPixels(it->key + mWidth * 0.5, it->lowerQuartile));
  return result;
}

/*!  \internal

  Returns the whisker backbones (keys in x, values in y of the returned lines)
  that cover the value range from the minimum to the lower quartile, and from
  the upper quartile to the maximum of the data given by \a it.

  \see drawStatisticalBox, getQuartileBox, getWhiskerBarLines
*/
QVector<QLineF> QCPStatisticalBox::getWhiskerBackboneLines(
    QCPStatisticalBoxDataContainer::const_iterator it) const {
  QVector<QLineF> result(2);
  result[0].setPoints(coordsToPixels(it->key, it->lowerQuartile),
                      coordsToPixels(it->key, it->minimum));  // min backbone
  result[1].setPoints(coordsToPixels(it->key, it->upperQuartile),
                      coordsToPixels(it->key, it->maximum));  // max backbone
  return result;
}

/*!  \internal

  Returns the whisker bars (keys in x, values in y of the returned lines) that
  are placed at the end of the whisker backbones, at the minimum and maximum of
  the data given by \a it.

  \see drawStatisticalBox, getQuartileBox, getWhiskerBackboneLines
*/
QVector<QLineF> QCPStatisticalBox::getWhiskerBarLines(
    QCPStatisticalBoxDataContainer::const_iterator it) const {
  QVector<QLineF> result(2);
  result[0].setPoints(
      coordsToPixels(it->key - mWhiskerWidth * 0.5, it->minimum),
      coordsToPixels(it->key + mWhiskerWidth * 0.5, it->minimum));  // min bar
  result[1].setPoints(
      coordsToPixels(it->key - mWhiskerWidth * 0.5, it->maximum),
      coordsToPixels(it->key + mWhiskerWidth * 0.5, it->maximum));  // max bar
  return result;
}
/* end of 'src/plottables/plottable-statisticalbox.cpp' */

/* including file 'src/plottables/plottable-colormap.cpp' */
/* modified 2022-11-06T12:45:56, size 48189               */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPColorMapData
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPColorMapData
  \brief Holds the two-dimensional data of a QCPColorMap plottable.

  This class is a data storage for \ref QCPColorMap. It holds a two-dimensional
  array, which \ref QCPColorMap then displays as a 2D image in the plot, where
  the array values are represented by a color, depending on the value.

  The size of the array can be controlled via \ref setSize (or \ref setKeySize,
  \ref setValueSize). Which plot coordinates these cells correspond to can be
  configured with \ref setRange (or \ref setKeyRange, \ref setValueRange).

  The data cells can be accessed in two ways: They can be directly addressed by
  an integer index with \ref setCell. This is the fastest method. Alternatively,
  they can be addressed by their plot coordinate with \ref setData. plot
  coordinate to cell index transformations and vice versa are provided by the
  functions \ref coordToCell and \ref cellToCoord.

  A \ref QCPColorMapData also holds an on-demand two-dimensional array of alpha
  values which (if allocated) has the same size as the data map. It can be
  accessed via \ref setAlpha, \ref fillAlpha and \ref clearAlpha. The memory for
  the alpha map is only allocated if needed, i.e. on the first call of \ref
  setAlpha. \ref clearAlpha restores full opacity and frees the alpha map.

  This class also buffers the minimum and maximum values that are in the data
  set, to provide QCPColorMap::rescaleDataRange with the necessary information
  quickly. Setting a cell to a value that is greater than the current maximum
  increases this maximum to the new value. However, setting the cell that
  currently holds the maximum value to a smaller value doesn't decrease the
  maximum again, because finding the true new maximum would require going
  through the entire data array, which might be time consuming. The same holds
  for the data minimum. This functionality is given by \ref
  recalculateDataBounds, such that you can decide when it is sensible to find
  the true current minimum and maximum. The method QCPColorMap::rescaleDataRange
  offers a convenience parameter \a recalculateDataBounds which may be set to
  true to automatically call \ref recalculateDataBounds internally.
*/

/* start of documentation of inline functions */

/*! \fn bool QCPColorMapData::isEmpty() const

  Returns whether this instance carries no data. This is equivalent to having a
  size where at least one of the dimensions is 0 (see \ref setSize).
*/

/* end of documentation of inline functions */

/*!
  Constructs a new QCPColorMapData instance. The instance has \a keySize cells
  in the key direction and \a valueSize cells in the value direction. These
  cells will be displayed by the \ref QCPColorMap at the coordinates \a keyRange
  and \a valueRange.

  \see setSize, setKeySize, setValueSize, setRange, setKeyRange, setValueRange
*/
QCPColorMapData::QCPColorMapData(int keySize, int valueSize,
                                 const QCPRange &keyRange,
                                 const QCPRange &valueRange)
    : mKeySize(0),
      mValueSize(0),
      mKeyRange(keyRange),
      mValueRange(valueRange),
      mIsEmpty(true),
      mData(nullptr),
      mAlpha(nullptr),
      mDataModified(true) {
  setSize(keySize, valueSize);
  fill(0);
}

QCPColorMapData::~QCPColorMapData() {
  delete[] mData;
  delete[] mAlpha;
}

/*!
  Constructs a new QCPColorMapData instance copying the data and range of \a
  other.
*/
QCPColorMapData::QCPColorMapData(const QCPColorMapData &other)
    : mKeySize(0),
      mValueSize(0),
      mIsEmpty(true),
      mData(nullptr),
      mAlpha(nullptr),
      mDataModified(true) {
  *this = other;
}

/*!
  Overwrites this color map data instance with the data stored in \a other. The
  alpha map state is transferred, too.
*/
QCPColorMapData &QCPColorMapData::operator=(const QCPColorMapData &other) {
  if (&other != this) {
    const int keySize = other.keySize();
    const int valueSize = other.valueSize();
    if (!other.mAlpha && mAlpha) clearAlpha();
    setSize(keySize, valueSize);
    if (other.mAlpha && !mAlpha) createAlpha(false);
    setRange(other.keyRange(), other.valueRange());
    if (!isEmpty()) {
      memcpy(mData, other.mData,
             sizeof(mData[0]) * size_t(keySize * valueSize));
      if (mAlpha)
        memcpy(mAlpha, other.mAlpha,
               sizeof(mAlpha[0]) * size_t(keySize * valueSize));
    }
    mDataBounds = other.mDataBounds;
    mDataModified = true;
  }
  return *this;
}

/* undocumented getter */
double QCPColorMapData::data(double key, double value) {
  int keyCell = int((key - mKeyRange.lower) /
                        (mKeyRange.upper - mKeyRange.lower) * (mKeySize - 1) +
                    0.5);
  int valueCell =
      int((value - mValueRange.lower) /
              (mValueRange.upper - mValueRange.lower) * (mValueSize - 1) +
          0.5);
  if (keyCell >= 0 && keyCell < mKeySize && valueCell >= 0 &&
      valueCell < mValueSize)
    return mData[valueCell * mKeySize + keyCell];
  else
    return 0;
}

/* undocumented getter */
double QCPColorMapData::cell(int keyIndex, int valueIndex) {
  if (keyIndex >= 0 && keyIndex < mKeySize && valueIndex >= 0 &&
      valueIndex < mValueSize)
    return mData[valueIndex * mKeySize + keyIndex];
  else
    return 0;
}

/*!
  Returns the alpha map value of the cell with the indices \a keyIndex and \a
  valueIndex.

  If this color map data doesn't have an alpha map (because \ref setAlpha was
  never called after creation or after a call to \ref clearAlpha), returns 255,
  which corresponds to full opacity.

  \see setAlpha
*/
unsigned char QCPColorMapData::alpha(int keyIndex, int valueIndex) {
  if (mAlpha && keyIndex >= 0 && keyIndex < mKeySize && valueIndex >= 0 &&
      valueIndex < mValueSize)
    return mAlpha[valueIndex * mKeySize + keyIndex];
  else
    return 255;
}

/*!
  Resizes the data array to have \a keySize cells in the key dimension and \a
  valueSize cells in the value dimension.

  The current data is discarded and the map cells are set to 0, unless the map
  had already the requested size.

  Setting at least one of \a keySize or \a valueSize to zero frees the internal
  data array and \ref isEmpty returns true.

  \see setRange, setKeySize, setValueSize
*/
void QCPColorMapData::setSize(int keySize, int valueSize) {
  if (keySize != mKeySize || valueSize != mValueSize) {
    mKeySize = keySize;
    mValueSize = valueSize;
    delete[] mData;
    mIsEmpty = mKeySize == 0 || mValueSize == 0;
    if (!mIsEmpty) {
#ifdef __EXCEPTIONS
      try {  // 2D arrays get memory intensive fast. So if the allocation fails,
             // at least output debug message
#endif
        mData = new double[size_t(mKeySize * mValueSize)];
#ifdef __EXCEPTIONS
      } catch (...) {
        mData = nullptr;
      }
#endif
      if (mData)
        fill(0);
      else
        qDebug() << Q_FUNC_INFO << "out of memory for data dimensions "
                 << mKeySize << "*" << mValueSize;
    } else
      mData = nullptr;

    if (mAlpha)  // if we had an alpha map, recreate it with new size
      createAlpha();

    mDataModified = true;
  }
}

/*!
  Resizes the data array to have \a keySize cells in the key dimension.

  The current data is discarded and the map cells are set to 0, unless the map
  had already the requested size.

  Setting \a keySize to zero frees the internal data array and \ref isEmpty
  returns true.

  \see setKeyRange, setSize, setValueSize
*/
void QCPColorMapData::setKeySize(int keySize) { setSize(keySize, mValueSize); }

/*!
  Resizes the data array to have \a valueSize cells in the value dimension.

  The current data is discarded and the map cells are set to 0, unless the map
  had already the requested size.

  Setting \a valueSize to zero frees the internal data array and \ref isEmpty
  returns true.

  \see setValueRange, setSize, setKeySize
*/
void QCPColorMapData::setValueSize(int valueSize) {
  setSize(mKeySize, valueSize);
}

/*!
  Sets the coordinate ranges the data shall be distributed over. This defines
  the rectangular area covered by the color map in plot coordinates.

  The outer cells will be centered on the range boundaries given to this
  function. For example, if the key size (\ref setKeySize) is 3 and \a keyRange
  is set to <tt>QCPRange(2, 3)</tt> there will be cells centered on the key
  coordinates 2, 2.5 and 3.

  \see setSize
*/
void QCPColorMapData::setRange(const QCPRange &keyRange,
                               const QCPRange &valueRange) {
  setKeyRange(keyRange);
  setValueRange(valueRange);
}

/*!
  Sets the coordinate range the data shall be distributed over in the key
  dimension. Together with the value range, This defines the rectangular area
  covered by the color map in plot coordinates.

  The outer cells will be centered on the range boundaries given to this
  function. For example, if the key size (\ref setKeySize) is 3 and \a keyRange
  is set to <tt>QCPRange(2, 3)</tt> there will be cells centered on the key
  coordinates 2, 2.5 and 3.

  \see setRange, setValueRange, setSize
*/
void QCPColorMapData::setKeyRange(const QCPRange &keyRange) {
  mKeyRange = keyRange;
}

/*!
  Sets the coordinate range the data shall be distributed over in the value
  dimension. Together with the key range, This defines the rectangular area
  covered by the color map in plot coordinates.

  The outer cells will be centered on the range boundaries given to this
  function. For example, if the value size (\ref setValueSize) is 3 and \a
  valueRange is set to <tt>QCPRange(2, 3)</tt> there will be cells centered on
  the value coordinates 2, 2.5 and 3.

  \see setRange, setKeyRange, setSize
*/
void QCPColorMapData::setValueRange(const QCPRange &valueRange) {
  mValueRange = valueRange;
}

/*!
  Sets the data of the cell, which lies at the plot coordinates given by \a key
  and \a value, to \a z.

  \note The QCPColorMap always displays the data at equal key/value intervals,
  even if the key or value axis is set to a logarithmic scaling. If you want to
  use QCPColorMap with logarithmic axes, you shouldn't use the \ref
  QCPColorMapData::setData method as it uses a linear transformation to
  determine the cell index. Rather directly access the cell index with \ref
  QCPColorMapData::setCell.

  \see setCell, setRange
*/
void QCPColorMapData::setData(double key, double value, double z) {
  int keyCell = int((key - mKeyRange.lower) /
                        (mKeyRange.upper - mKeyRange.lower) * (mKeySize - 1) +
                    0.5);
  int valueCell =
      int((value - mValueRange.lower) /
              (mValueRange.upper - mValueRange.lower) * (mValueSize - 1) +
          0.5);
  if (keyCell >= 0 && keyCell < mKeySize && valueCell >= 0 &&
      valueCell < mValueSize) {
    mData[valueCell * mKeySize + keyCell] = z;
    if (z < mDataBounds.lower) mDataBounds.lower = z;
    if (z > mDataBounds.upper) mDataBounds.upper = z;
    mDataModified = true;
  }
}

/*!
  Sets the data of the cell with indices \a keyIndex and \a valueIndex to \a z.
  The indices enumerate the cells starting from zero, up to the map's size-1 in
  the respective dimension (see \ref setSize).

  In the standard plot configuration (horizontal key axis and vertical value
  axis, both not range-reversed), the cell with indices (0, 0) is in the bottom
  left corner and the cell with indices (keySize-1, valueSize-1) is in the top
  right corner of the color map.

  \see setData, setSize
*/
void QCPColorMapData::setCell(int keyIndex, int valueIndex, double z) {
  if (keyIndex >= 0 && keyIndex < mKeySize && valueIndex >= 0 &&
      valueIndex < mValueSize) {
    mData[valueIndex * mKeySize + keyIndex] = z;
    if (z < mDataBounds.lower) mDataBounds.lower = z;
    if (z > mDataBounds.upper) mDataBounds.upper = z;
    mDataModified = true;
  } else
    qDebug() << Q_FUNC_INFO << "index out of bounds:" << keyIndex << valueIndex;
}

/*!
  Sets the alpha of the color map cell given by \a keyIndex and \a valueIndex to
  \a alpha. A value of 0 for \a alpha results in a fully transparent cell, and a
  value of 255 results in a fully opaque cell.

  If an alpha map doesn't exist yet for this color map data, it will be created
  here. If you wish to restore full opacity and free any allocated memory of the
  alpha map, call \ref clearAlpha.

  Note that the cell-wise alpha which can be configured here is independent of
  any alpha configured in the color map's gradient (\ref QCPColorGradient). If a
  cell is affected both by the cell-wise and gradient alpha, the alpha values
  will be blended accordingly during rendering of the color map.

  \see fillAlpha, clearAlpha
*/
void QCPColorMapData::setAlpha(int keyIndex, int valueIndex,
                               unsigned char alpha) {
  if (keyIndex >= 0 && keyIndex < mKeySize && valueIndex >= 0 &&
      valueIndex < mValueSize) {
    if (mAlpha || createAlpha()) {
      mAlpha[valueIndex * mKeySize + keyIndex] = alpha;
      mDataModified = true;
    }
  } else
    qDebug() << Q_FUNC_INFO << "index out of bounds:" << keyIndex << valueIndex;
}

/*!
  Goes through the data and updates the buffered minimum and maximum data
  values.

  Calling this method is only advised if you are about to call \ref
  QCPColorMap::rescaleDataRange and can not guarantee that the cells holding the
  maximum or minimum data haven't been overwritten with a smaller or larger
  value respectively, since the buffered maximum/minimum values have been
  updated the last time. Why this is the case is explained in the class
  description (\ref QCPColorMapData).

  Note that the method \ref QCPColorMap::rescaleDataRange provides a parameter
  \a recalculateDataBounds for convenience. Setting this to true will call this
  method for you, before doing the rescale.
*/
void QCPColorMapData::recalculateDataBounds() {
  if (mKeySize > 0 && mValueSize > 0) {
    double minHeight = std::numeric_limits<double>::max();
    double maxHeight = -std::numeric_limits<double>::max();
    const int dataCount = mValueSize * mKeySize;
    for (int i = 0; i < dataCount; ++i) {
      if (mData[i] > maxHeight) maxHeight = mData[i];
      if (mData[i] < minHeight) minHeight = mData[i];
    }
    mDataBounds.lower = minHeight;
    mDataBounds.upper = maxHeight;
  }
}

/*!
  Frees the internal data memory.

  This is equivalent to calling \ref setSize "setSize(0, 0)".
*/
void QCPColorMapData::clear() { setSize(0, 0); }

/*!
  Frees the internal alpha map. The color map will have full opacity again.
*/
void QCPColorMapData::clearAlpha() {
  if (mAlpha) {
    delete[] mAlpha;
    mAlpha = nullptr;
    mDataModified = true;
  }
}

/*!
  Sets all cells to the value \a z.
*/
void QCPColorMapData::fill(double z) {
  const int dataCount = mValueSize * mKeySize;
  memset(mData, z, dataCount * sizeof(*mData));
  mDataBounds = QCPRange(z, z);
  mDataModified = true;
}

/*!
  Sets the opacity of all color map cells to \a alpha. A value of 0 for \a alpha
  results in a fully transparent color map, and a value of 255 results in a
  fully opaque color map.

  If you wish to restore opacity to 100% and free any used memory for the alpha
  map, rather use \ref clearAlpha.

  \see setAlpha
*/
void QCPColorMapData::fillAlpha(unsigned char alpha) {
  if (mAlpha || createAlpha(false)) {
    const int dataCount = mValueSize * mKeySize;
    memset(mAlpha, alpha, dataCount * sizeof(*mAlpha));
    mDataModified = true;
  }
}

/*!
  Transforms plot coordinates given by \a key and \a value to cell indices of
  this QCPColorMapData instance. The resulting cell indices are returned via the
  output parameters \a keyIndex and \a valueIndex.

  The retrieved key/value cell indices can then be used for example with \ref
  setCell.

  If you are only interested in a key or value index, you may pass \c nullptr as
  \a valueIndex or \a keyIndex.

  \note The QCPColorMap always displays the data at equal key/value intervals,
  even if the key or value axis is set to a logarithmic scaling. If you want to
  use QCPColorMap with logarithmic axes, you shouldn't use the \ref
  QCPColorMapData::coordToCell method as it uses a linear transformation to
  determine the cell index.

  \see cellToCoord, QCPAxis::coordToPixel
*/
void QCPColorMapData::coordToCell(double key, double value, int *keyIndex,
                                  int *valueIndex) const {
  if (keyIndex)
    *keyIndex = int((key - mKeyRange.lower) /
                        (mKeyRange.upper - mKeyRange.lower) * (mKeySize - 1) +
                    0.5);
  if (valueIndex)
    *valueIndex =
        int((value - mValueRange.lower) /
                (mValueRange.upper - mValueRange.lower) * (mValueSize - 1) +
            0.5);
}

/*!
  Transforms cell indices given by \a keyIndex and \a valueIndex to cell indices
  of this QCPColorMapData instance. The resulting coordinates are returned via
  the output parameters \a key and \a value.

  If you are only interested in a key or value coordinate, you may pass \c
  nullptr as \a key or \a value.

  \note The QCPColorMap always displays the data at equal key/value intervals,
  even if the key or value axis is set to a logarithmic scaling. If you want to
  use QCPColorMap with logarithmic axes, you shouldn't use the \ref
  QCPColorMapData::cellToCoord method as it uses a linear transformation to
  determine the cell index.

  \see coordToCell, QCPAxis::pixelToCoord
*/
void QCPColorMapData::cellToCoord(int keyIndex, int valueIndex, double *key,
                                  double *value) const {
  if (key)
    *key =
        keyIndex / double(mKeySize - 1) * (mKeyRange.upper - mKeyRange.lower) +
        mKeyRange.lower;
  if (value)
    *value = valueIndex / double(mValueSize - 1) *
                 (mValueRange.upper - mValueRange.lower) +
             mValueRange.lower;
}

/*! \internal

  Allocates the internal alpha map with the current data map key/value size and,
  if \a initializeOpaque is true, initializes all values to 255. If \a
  initializeOpaque is false, the values are not initialized at all. In this
  case, the alpha map should be initialized manually, e.g. with \ref fillAlpha.

  If an alpha map exists already, it is deleted first. If this color map is
  empty (has either key or value size zero, see \ref isEmpty), the alpha map is
  cleared.

  The return value indicates the existence of the alpha map after the call. So
  this method returns true if the data map isn't empty and an alpha map was
  successfully allocated.
*/
bool QCPColorMapData::createAlpha(bool initializeOpaque) {
  clearAlpha();
  if (isEmpty()) return false;

#ifdef __EXCEPTIONS
  try {  // 2D arrays get memory intensive fast. So if the allocation fails, at
         // least output debug message
#endif
    mAlpha = new unsigned char[size_t(mKeySize * mValueSize)];
#ifdef __EXCEPTIONS
  } catch (...) {
    mAlpha = nullptr;
  }
#endif
  if (mAlpha) {
    if (initializeOpaque) fillAlpha(255);
    return true;
  } else {
    qDebug() << Q_FUNC_INFO << "out of memory for data dimensions " << mKeySize
             << "*" << mValueSize;
    return false;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPColorMap
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPColorMap
  \brief A plottable representing a two-dimensional color map in a plot.

  \image html QCPColorMap.png

  The data is stored in the class \ref QCPColorMapData, which can be accessed
  via the data() method.

  A color map has three dimensions to represent a data point: The \a key
  dimension, the \a value dimension and the \a data dimension. As with other
  plottables such as graphs, \a key and \a value correspond to two orthogonal
  axes on the QCustomPlot surface that you specify in the QCPColorMap
  constructor. The \a data dimension however is encoded as the color of the
  point at (\a key, \a value).

  Set the number of points (or \a cells) in the key/value dimension via \ref
  QCPColorMapData::setSize. The plot coordinate range over which these points
  will be displayed is specified via \ref QCPColorMapData::setRange. The first
  cell will be centered on the lower range boundary and the last cell will be
  centered on the upper range boundary. The data can be set by either accessing
  the cells directly with QCPColorMapData::setCell or by addressing the cells
  via their plot coordinates with \ref QCPColorMapData::setData. If possible,
  you should prefer setCell, since it doesn't need to do any coordinate
  transformation and thus performs a bit better.

  The cell with index (0, 0) is at the bottom left, if the color map uses normal
  (i.e. not reversed) key and value axes.

  To show the user which colors correspond to which \a data values, a \ref
  QCPColorScale is typically placed to the right of the axis rect. See the
  documentation there for details on how to add and use a color scale.

  \section qcpcolormap-appearance Changing the appearance

  Most important to the appearance is the color gradient, which can be specified
  via \ref setGradient. See the documentation of \ref QCPColorGradient for
  details on configuring a color gradient.

  The \a data range that is mapped to the colors of the gradient can be
  specified with \ref setDataRange. To make the data range encompass the whole
  data set minimum to maximum, call \ref rescaleDataRange. If your data may
  contain NaN values, use \ref QCPColorGradient::setNanHandling to define how
  they are displayed.

  \section qcpcolormap-transparency Transparency

  Transparency in color maps can be achieved by two mechanisms. On one hand, you
  can specify alpha values for color stops of the \ref QCPColorGradient, via the
  regular QColor interface. This will cause the color map data which gets mapped
  to colors around those color stops to appear with the accordingly interpolated
  transparency.

  On the other hand you can also directly apply an alpha value to each cell
  independent of its data, by using the alpha map feature of \ref
  QCPColorMapData. The relevant methods are \ref QCPColorMapData::setAlpha,
  QCPColorMapData::fillAlpha and \ref QCPColorMapData::clearAlpha().

  The two transparencies will be joined together in the plot and otherwise not
  interfere with each other. They are mixed in a multiplicative matter, so an
  alpha of e.g. 50% (128/255) in both modes simultaneously, will result in a
  total transparency of 25% (64/255).

  \section qcpcolormap-usage Usage

  Like all data representing objects in QCustomPlot, the QCPColorMap is a
  plottable (QCPAbstractPlottable). So the plottable-interface of QCustomPlot
  applies (QCustomPlot::plottable, QCustomPlot::removePlottable, etc.)

  Usually, you first create an instance:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpcolormap-creation-1
  which registers it with the QCustomPlot instance of the passed axes. Note that
  this QCustomPlot instance takes ownership of the plottable, so do not delete
  it manually but use QCustomPlot::removePlottable() instead. The newly created
  plottable can be modified, e.g.: \snippet
  documentation/doc-code-snippets/mainwindow.cpp qcpcolormap-creation-2

  \note The QCPColorMap always displays the data at equal key/value intervals,
  even if the key or value axis is set to a logarithmic scaling. If you want to
  use QCPColorMap with logarithmic axes, you shouldn't use the \ref
  QCPColorMapData::setData method as it uses a linear transformation to
  determine the cell index. Rather directly access the cell index with \ref
  QCPColorMapData::setCell.
*/

/* start documentation of inline functions */

/*! \fn QCPColorMapData *QCPColorMap::data() const

  Returns a pointer to the internal data storage of type \ref QCPColorMapData.
  Access this to modify data points (cells) and the color map key/value range.

  \see setData
*/

/* end documentation of inline functions */

/* start documentation of signals */

/*! \fn void QCPColorMap::dataRangeChanged(const QCPRange &newRange);

  This signal is emitted when the data range changes.

  \see setDataRange
*/

/*! \fn void QCPColorMap::dataScaleTypeChanged(QCPAxis::ScaleType scaleType);

  This signal is emitted when the data scale type changes.

  \see setDataScaleType
*/

/*! \fn void QCPColorMap::gradientChanged(const QCPColorGradient &newGradient);

  This signal is emitted when the gradient changes.

  \see setGradient
*/

/* end documentation of signals */

/*!
  Constructs a color map with the specified \a keyAxis and \a valueAxis.

  The created QCPColorMap is automatically registered with the QCustomPlot
  instance inferred from \a keyAxis. This QCustomPlot instance takes ownership
  of the QCPColorMap, so do not delete it manually but use
  QCustomPlot::removePlottable() instead.
*/
QCPColorMap::QCPColorMap(QCPAxis *keyAxis, QCPAxis *valueAxis)
    : QCPAbstractPlottable(keyAxis, valueAxis),
      mDataScaleType(QCPAxis::stLinear),
      mMapData(new QCPColorMapData(10, 10, QCPRange(0, 5), QCPRange(0, 5))),
      mGradient(QCPColorGradient::gpCold),
      mInterpolate(true),
      mTightBoundary(false),
      mMapImageInvalidated(true) {}

QCPColorMap::~QCPColorMap() { delete mMapData; }

/*!
  Replaces the current \ref data with the provided \a data.

  If \a copy is set to true, the \a data object will only be copied. if false,
  the color map takes ownership of the passed data and replaces the internal
  data pointer with it. This is significantly faster than copying for large
  datasets.
*/
void QCPColorMap::setData(QCPColorMapData *data, bool copy) {
  if (mMapData == data) {
    qDebug() << Q_FUNC_INFO
             << "The data pointer is already in (and owned by) this plottable"
             << reinterpret_cast<quintptr>(data);
    return;
  }
  if (copy) {
    *mMapData = *data;
  } else {
    delete mMapData;
    mMapData = data;
  }
  mMapImageInvalidated = true;
}

/*!
  Sets the data range of this color map to \a dataRange. The data range defines
  which data values are mapped to the color gradient.

  To make the data range span the full range of the data set, use \ref
  rescaleDataRange.

  \see QCPColorScale::setDataRange
*/
void QCPColorMap::setDataRange(const QCPRange &dataRange) {
  if (!QCPRange::validRange(dataRange)) return;
  if (mDataRange.lower != dataRange.lower ||
      mDataRange.upper != dataRange.upper) {
    if (mDataScaleType == QCPAxis::stLogarithmic)
      mDataRange = dataRange.sanitizedForLogScale();
    else
      mDataRange = dataRange.sanitizedForLinScale();
    mMapImageInvalidated = true;
    emit dataRangeChanged(mDataRange);
  }
}

/*!
  Sets whether the data is correlated with the color gradient linearly or
  logarithmically.

  \see QCPColorScale::setDataScaleType
*/
void QCPColorMap::setDataScaleType(QCPAxis::ScaleType scaleType) {
  if (mDataScaleType != scaleType) {
    mDataScaleType = scaleType;
    mMapImageInvalidated = true;
    emit dataScaleTypeChanged(mDataScaleType);
    if (mDataScaleType == QCPAxis::stLogarithmic)
      setDataRange(mDataRange.sanitizedForLogScale());
  }
}

/*!
  Sets the color gradient that is used to represent the data. For more details
  on how to create an own gradient or use one of the preset gradients, see \ref
  QCPColorGradient.

  The colors defined by the gradient will be used to represent data values in
  the currently set data range, see \ref setDataRange. Data points that are
  outside this data range will either be colored uniformly with the respective
  gradient boundary color, or the gradient will repeat, depending on \ref
  QCPColorGradient::setPeriodic.

  \see QCPColorScale::setGradient
*/
void QCPColorMap::setGradient(const QCPColorGradient &gradient) {
  if (mGradient != gradient) {
    mGradient = gradient;
    mMapImageInvalidated = true;
    emit gradientChanged(mGradient);
  }
}

/*!
  Sets whether the color map image shall use bicubic interpolation when
  displaying the color map shrinked or expanded, and not at a 1:1 pixel-to-data
  scale.

  \image html QCPColorMap-interpolate.png "A 10*10 color map, with interpolation
  and without interpolation enabled"
*/
void QCPColorMap::setInterpolate(bool enabled) {
  mInterpolate = enabled;
  mMapImageInvalidated =
      true;  // because oversampling factors might need to change
}

/*!
  Sets whether the outer most data rows and columns are clipped to the specified
  key and value range (see \ref QCPColorMapData::setKeyRange, \ref
  QCPColorMapData::setValueRange).

  if \a enabled is set to false, the data points at the border of the color map
  are drawn with the same width and height as all other data points. Since the
  data points are represented by rectangles of one color centered on the data
  coordinate, this means that the shown color map extends by half a data point
  over the specified key/value range in each direction.

  \image html QCPColorMap-tightboundary.png "A color map, with tight boundary
  enabled and disabled"
*/
void QCPColorMap::setTightBoundary(bool enabled) { mTightBoundary = enabled; }

/*!
  Associates the color scale \a colorScale with this color map.

  This means that both the color scale and the color map synchronize their
  gradient, data range and data scale type (\ref setGradient, \ref setDataRange,
  \ref setDataScaleType). Multiple color maps can be associated with one single
  color scale. This causes the color maps to also synchronize those properties,
  via the mutual color scale.

  This function causes the color map to adopt the current color gradient, data
  range and data scale type of \a colorScale. After this call, you may change
  these properties at either the color map or the color scale, and the setting
  will be applied to both.

  Pass \c nullptr as \a colorScale to disconnect the color scale from this color
  map again.
*/
void QCPColorMap::setColorScale(QCPColorScale *colorScale) {
  if (mColorScale)  // unconnect signals from old color scale
  {
    disconnect(this, SIGNAL(dataRangeChanged(QCPRange)), mColorScale.data(),
               SLOT(setDataRange(QCPRange)));
    disconnect(this, SIGNAL(dataScaleTypeChanged(QCPAxis::ScaleType)),
               mColorScale.data(), SLOT(setDataScaleType(QCPAxis::ScaleType)));
    disconnect(this, SIGNAL(gradientChanged(QCPColorGradient)),
               mColorScale.data(), SLOT(setGradient(QCPColorGradient)));
    disconnect(mColorScale.data(), SIGNAL(dataRangeChanged(QCPRange)), this,
               SLOT(setDataRange(QCPRange)));
    disconnect(mColorScale.data(), SIGNAL(gradientChanged(QCPColorGradient)),
               this, SLOT(setGradient(QCPColorGradient)));
    disconnect(mColorScale.data(),
               SIGNAL(dataScaleTypeChanged(QCPAxis::ScaleType)), this,
               SLOT(setDataScaleType(QCPAxis::ScaleType)));
  }
  mColorScale = colorScale;
  if (mColorScale)  // connect signals to new color scale
  {
    setGradient(mColorScale.data()->gradient());
    setDataRange(mColorScale.data()->dataRange());
    setDataScaleType(mColorScale.data()->dataScaleType());
    connect(this, SIGNAL(dataRangeChanged(QCPRange)), mColorScale.data(),
            SLOT(setDataRange(QCPRange)));
    connect(this, SIGNAL(dataScaleTypeChanged(QCPAxis::ScaleType)),
            mColorScale.data(), SLOT(setDataScaleType(QCPAxis::ScaleType)));
    connect(this, SIGNAL(gradientChanged(QCPColorGradient)), mColorScale.data(),
            SLOT(setGradient(QCPColorGradient)));
    connect(mColorScale.data(), SIGNAL(dataRangeChanged(QCPRange)), this,
            SLOT(setDataRange(QCPRange)));
    connect(mColorScale.data(), SIGNAL(gradientChanged(QCPColorGradient)), this,
            SLOT(setGradient(QCPColorGradient)));
    connect(mColorScale.data(),
            SIGNAL(dataScaleTypeChanged(QCPAxis::ScaleType)), this,
            SLOT(setDataScaleType(QCPAxis::ScaleType)));
  }
}

/*!
  Sets the data range (\ref setDataRange) to span the minimum and maximum values
  that occur in the current data set. This corresponds to the \ref
  rescaleKeyAxis or \ref rescaleValueAxis methods, only for the third data
  dimension of the color map.

  The minimum and maximum values of the data set are buffered in the internal
  QCPColorMapData instance (\ref data). As data is updated via its \ref
  QCPColorMapData::setCell or \ref QCPColorMapData::setData, the buffered
  minimum and maximum values are updated, too. For performance reasons, however,
  they are only updated in an expanding fashion. So the buffered maximum can
  only increase and the buffered minimum can only decrease. In consequence,
  changes to the data that actually lower the maximum of the data set (by
  overwriting the cell holding the current maximum with a smaller value), aren't
  recognized and the buffered maximum overestimates the true maximum of the data
  set. The same happens for the buffered minimum. To recalculate the true
  minimum and maximum by explicitly looking at each cell, the method
  QCPColorMapData::recalculateDataBounds can be used. For convenience, setting
  the parameter \a recalculateDataBounds calls this method before setting the
  data range to the buffered minimum and maximum.

  \see setDataRange
*/
void QCPColorMap::rescaleDataRange(bool recalculateDataBounds) {
  if (recalculateDataBounds) mMapData->recalculateDataBounds();
  setDataRange(mMapData->dataBounds());
}

/*!
  Takes the current appearance of the color map and updates the legend icon,
  which is used to represent this color map in the legend (see \ref QCPLegend).

  The \a transformMode specifies whether the rescaling is done by a faster, low
  quality image scaling algorithm (Qt::FastTransformation) or by a slower,
  higher quality algorithm (Qt::SmoothTransformation).

  The current color map appearance is scaled down to \a thumbSize. Ideally, this
  should be equal to the size of the legend icon (see \ref
  QCPLegend::setIconSize). If it isn't exactly the configured legend icon size,
  the thumb will be rescaled during drawing of the legend item.

  \see setDataRange
*/
void QCPColorMap::updateLegendIcon(Qt::TransformationMode transformMode,
                                   const QSize &thumbSize) {
  if (mMapImage.isNull() && !data()->isEmpty())
    updateMapImage();  // try to update map image if it's null (happens if no
                       // draw has happened yet)

  if (!mMapImage.isNull())  // might still be null, e.g. if data is empty, so
  // check here again
  {
    bool mirrorX =
        (keyAxis()->orientation() == Qt::Horizontal ? keyAxis() : valueAxis())
            ->rangeReversed();
    bool mirrorY =
        (valueAxis()->orientation() == Qt::Vertical ? valueAxis() : keyAxis())
            ->rangeReversed();
    mLegendIcon = QPixmap::fromImage(mMapImage.mirrored(mirrorX, mirrorY))
                      .scaled(thumbSize, Qt::KeepAspectRatio, transformMode);
  }
}

/* inherits documentation from base class */
double QCPColorMap::selectTest(const QPointF &pos, bool onlySelectable,
                               QVariant *details) const {
  Q_UNUSED(details)
  if ((onlySelectable && mSelectable == QCP::stNone) || mMapData->isEmpty())
    return -1;
  if (!mKeyAxis || !mValueAxis) return -1;

  if (mKeyAxis.data()->axisRect()->rect().contains(pos.toPoint()) ||
      mParentPlot->interactions().testFlag(
          QCP::iSelectPlottablesBeyondAxisRect)) {
    double posKey, posValue;
    pixelsToCoords(pos, posKey, posValue);
    if (mMapData->keyRange().contains(posKey) &&
        mMapData->valueRange().contains(posValue)) {
      if (details)
        details->setValue(QCPDataSelection(QCPDataRange(
            0, 1)));  // temporary solution, to facilitate whole-plottable
                      // selection. Replace in future version with segmented 2D
                      // selection.
      return mParentPlot->selectionTolerance() * 0.99;
    }
  }
  return -1;
}

/* inherits documentation from base class */
QCPRange QCPColorMap::getKeyRange(bool &foundRange,
                                  QCP::SignDomain inSignDomain) const {
  foundRange = true;
  QCPRange result = mMapData->keyRange();
  result.normalize();
  if (inSignDomain == QCP::sdPositive) {
    if (result.lower <= 0 && result.upper > 0)
      result.lower = result.upper * 1e-3;
    else if (result.lower <= 0 && result.upper <= 0)
      foundRange = false;
  } else if (inSignDomain == QCP::sdNegative) {
    if (result.upper >= 0 && result.lower < 0)
      result.upper = result.lower * 1e-3;
    else if (result.upper >= 0 && result.lower >= 0)
      foundRange = false;
  }
  return result;
}

/* inherits documentation from base class */
QCPRange QCPColorMap::getValueRange(bool &foundRange,
                                    QCP::SignDomain inSignDomain,
                                    const QCPRange &inKeyRange) const {
  if (inKeyRange != QCPRange()) {
    if (mMapData->keyRange().upper < inKeyRange.lower ||
        mMapData->keyRange().lower > inKeyRange.upper) {
      foundRange = false;
      return {};
    }
  }

  foundRange = true;
  QCPRange result = mMapData->valueRange();
  result.normalize();
  if (inSignDomain == QCP::sdPositive) {
    if (result.lower <= 0 && result.upper > 0)
      result.lower = result.upper * 1e-3;
    else if (result.lower <= 0 && result.upper <= 0)
      foundRange = false;
  } else if (inSignDomain == QCP::sdNegative) {
    if (result.upper >= 0 && result.lower < 0)
      result.upper = result.lower * 1e-3;
    else if (result.upper >= 0 && result.lower >= 0)
      foundRange = false;
  }
  return result;
}

/*! \internal

  Updates the internal map image buffer by going through the internal \ref
  QCPColorMapData and turning the data values into color pixels with \ref
  QCPColorGradient::colorize.

  This method is called by \ref QCPColorMap::draw if either the data has been
  modified or the map image has been invalidated for a different reason (e.g. a
  change of the data range with \ref setDataRange).

  If the map cell count is low, the image created will be oversampled in order
  to avoid a QPainter::drawImage bug which makes inner pixel boundaries jitter
  when stretch-drawing images without smooth transform enabled. Accordingly,
  oversampling isn't performed if \ref setInterpolate is true.
*/
void QCPColorMap::updateMapImage() {
  QCPAxis *keyAxis = mKeyAxis.data();
  if (!keyAxis) return;
  if (mMapData->isEmpty()) return;

  const QImage::Format format = QImage::Format_ARGB32_Premultiplied;
  const int keySize = mMapData->keySize();
  const int valueSize = mMapData->valueSize();
  int keyOversamplingFactor =
      mInterpolate
          ? 1
          : int(1.0 +
                100.0 / double(keySize));  // make mMapImage have at least size
                                           // 100, factor becomes 1 if size >
                                           // 200 or interpolation is on
  int valueOversamplingFactor =
      mInterpolate
          ? 1
          : int(1.0 +
                100.0 /
                    double(valueSize));  // make mMapImage have at least size
                                         // 100, factor becomes 1 if size > 200
                                         // or interpolation is on

  // resize mMapImage to correct dimensions including possible oversampling
  // factors, according to key/value axes orientation:
  if (keyAxis->orientation() == Qt::Horizontal &&
      (mMapImage.width() != keySize * keyOversamplingFactor ||
       mMapImage.height() != valueSize * valueOversamplingFactor))
    mMapImage = QImage(QSize(keySize * keyOversamplingFactor,
                             valueSize * valueOversamplingFactor),
                       format);
  else if (keyAxis->orientation() == Qt::Vertical &&
           (mMapImage.width() != valueSize * valueOversamplingFactor ||
            mMapImage.height() != keySize * keyOversamplingFactor))
    mMapImage = QImage(QSize(valueSize * valueOversamplingFactor,
                             keySize * keyOversamplingFactor),
                       format);

  if (mMapImage.isNull()) {
    qDebug() << Q_FUNC_INFO
             << "Couldn't create map image (possibly too large for memory)";
    mMapImage = QImage(QSize(10, 10), format);
    mMapImage.fill(Qt::black);
  } else {
    QImage *localMapImage =
        &mMapImage;  // this is the image on which the colorization operates.
                     // Either the final mMapImage, or if we need oversampling,
                     // mUndersampledMapImage
    if (keyOversamplingFactor > 1 || valueOversamplingFactor > 1) {
      // resize undersampled map image to actual key/value cell sizes:
      if (keyAxis->orientation() == Qt::Horizontal &&
          (mUndersampledMapImage.width() != keySize ||
           mUndersampledMapImage.height() != valueSize))
        mUndersampledMapImage = QImage(QSize(keySize, valueSize), format);
      else if (keyAxis->orientation() == Qt::Vertical &&
               (mUndersampledMapImage.width() != valueSize ||
                mUndersampledMapImage.height() != keySize))
        mUndersampledMapImage = QImage(QSize(valueSize, keySize), format);
      localMapImage = &mUndersampledMapImage;  // make the colorization run on
                                               // the undersampled image
    } else if (!mUndersampledMapImage.isNull())
      mUndersampledMapImage =
          QImage();  // don't need oversampling mechanism anymore (map size has
                     // changed) but mUndersampledMapImage still has nonzero
                     // size, free it

    const double *rawData = mMapData->mData;
    const unsigned char *rawAlpha = mMapData->mAlpha;
    if (keyAxis->orientation() == Qt::Horizontal) {
      const int lineCount = valueSize;
      const int rowCount = keySize;
      for (int line = 0; line < lineCount; ++line) {
        QRgb *pixels = reinterpret_cast<QRgb *>(localMapImage->scanLine(
            lineCount - 1 -
            line));  // invert scanline index because QImage counts scanlines
                     // from top, but our vertical index counts from bottom
                     // (mathematical coordinate system)
        if (rawAlpha)
          mGradient.colorize(
              rawData + line * rowCount, rawAlpha + line * rowCount, mDataRange,
              pixels, rowCount, 1, mDataScaleType == QCPAxis::stLogarithmic);
        else
          mGradient.colorize(rawData + line * rowCount, mDataRange, pixels,
                             rowCount, 1,
                             mDataScaleType == QCPAxis::stLogarithmic);
      }
    } else  // keyAxis->orientation() == Qt::Vertical
    {
      const int lineCount = keySize;
      const int rowCount = valueSize;
      for (int line = 0; line < lineCount; ++line) {
        QRgb *pixels = reinterpret_cast<QRgb *>(localMapImage->scanLine(
            lineCount - 1 -
            line));  // invert scanline index because QImage counts scanlines
                     // from top, but our vertical index counts from bottom
                     // (mathematical coordinate system)
        if (rawAlpha)
          mGradient.colorize(rawData + line, rawAlpha + line, mDataRange,
                             pixels, rowCount, lineCount,
                             mDataScaleType == QCPAxis::stLogarithmic);
        else
          mGradient.colorize(rawData + line, mDataRange, pixels, rowCount,
                             lineCount,
                             mDataScaleType == QCPAxis::stLogarithmic);
      }
    }

    if (keyOversamplingFactor > 1 || valueOversamplingFactor > 1) {
      if (keyAxis->orientation() == Qt::Horizontal)
        mMapImage = mUndersampledMapImage.scaled(
            keySize * keyOversamplingFactor,
            valueSize * valueOversamplingFactor, Qt::IgnoreAspectRatio,
            Qt::FastTransformation);
      else
        mMapImage = mUndersampledMapImage.scaled(
            valueSize * valueOversamplingFactor,
            keySize * keyOversamplingFactor, Qt::IgnoreAspectRatio,
            Qt::FastTransformation);
    }
  }
  mMapData->mDataModified = false;
  mMapImageInvalidated = false;
}

/* inherits documentation from base class */
void QCPColorMap::draw(QCPPainter *painter) {
  if (mMapData->isEmpty()) return;
  if (!mKeyAxis || !mValueAxis) return;
  applyDefaultAntialiasingHint(painter);

  if (mMapData->mDataModified || mMapImageInvalidated) updateMapImage();

  // use buffer if painting vectorized (PDF):
  const bool useBuffer = painter->modes().testFlag(QCPPainter::pmVectorized);
  QCPPainter *localPainter = painter;  // will be redirected to paint on
                                       // mapBuffer if painting vectorized
  QRectF mapBufferTarget;  // the rect in absolute widget coordinates where the
                           // visible map portion/buffer will end up in
  QPixmap mapBuffer;
  if (useBuffer) {
    const double mapBufferPixelRatio =
        3;  // factor by which DPI is increased in embedded bitmaps
    mapBufferTarget = painter->clipRegion().boundingRect();
    mapBuffer =
        QPixmap((mapBufferTarget.size() * mapBufferPixelRatio).toSize());
    mapBuffer.fill(Qt::transparent);
    localPainter = new QCPPainter(&mapBuffer);
    localPainter->scale(mapBufferPixelRatio, mapBufferPixelRatio);
    localPainter->translate(-mapBufferTarget.topLeft());
  }

  QRectF imageRect = QRectF(coordsToPixels(mMapData->keyRange().lower,
                                           mMapData->valueRange().lower),
                            coordsToPixels(mMapData->keyRange().upper,
                                           mMapData->valueRange().upper))
                         .normalized();
  // extend imageRect to contain outer halves/quarters of bordering/cornering
  // pixels (cells are centered on map range boundary):
  double halfCellWidth = 0;   // in pixels
  double halfCellHeight = 0;  // in pixels
  if (keyAxis()->orientation() == Qt::Horizontal) {
    if (mMapData->keySize() > 1)
      halfCellWidth = 0.5 * imageRect.width() / double(mMapData->keySize() - 1);
    if (mMapData->valueSize() > 1)
      halfCellHeight =
          0.5 * imageRect.height() / double(mMapData->valueSize() - 1);
  } else  // keyAxis orientation is Qt::Vertical
  {
    if (mMapData->keySize() > 1)
      halfCellHeight =
          0.5 * imageRect.height() / double(mMapData->keySize() - 1);
    if (mMapData->valueSize() > 1)
      halfCellWidth =
          0.5 * imageRect.width() / double(mMapData->valueSize() - 1);
  }
  imageRect.adjust(-halfCellWidth, -halfCellHeight, halfCellWidth,
                   halfCellHeight);
  const bool mirrorX =
      (keyAxis()->orientation() == Qt::Horizontal ? keyAxis() : valueAxis())
          ->rangeReversed();
  const bool mirrorY =
      (valueAxis()->orientation() == Qt::Vertical ? valueAxis() : keyAxis())
          ->rangeReversed();
  const bool smoothBackup =
      localPainter->renderHints().testFlag(QPainter::SmoothPixmapTransform);
  localPainter->setRenderHint(QPainter::SmoothPixmapTransform, mInterpolate);
  QRegion clipBackup;
  if (mTightBoundary) {
    clipBackup = localPainter->clipRegion();
    QRectF tightClipRect = QRectF(coordsToPixels(mMapData->keyRange().lower,
                                                 mMapData->valueRange().lower),
                                  coordsToPixels(mMapData->keyRange().upper,
                                                 mMapData->valueRange().upper))
                               .normalized();
    localPainter->setClipRect(tightClipRect, Qt::IntersectClip);
  }
  localPainter->drawImage(imageRect, mMapImage.mirrored(mirrorX, mirrorY));
  if (mTightBoundary) localPainter->setClipRegion(clipBackup);
  localPainter->setRenderHint(QPainter::SmoothPixmapTransform, smoothBackup);

  if (useBuffer)  // localPainter painted to mapBuffer, so now draw buffer with
  // original painter
  {
    delete localPainter;
    painter->drawPixmap(mapBufferTarget.toRect(), mapBuffer);
  }
}

/* inherits documentation from base class */
void QCPColorMap::drawLegendIcon(QCPPainter *painter,
                                 const QRectF &rect) const {
  applyDefaultAntialiasingHint(painter);
  // draw map thumbnail:
  if (!mLegendIcon.isNull()) {
    QPixmap scaledIcon = mLegendIcon.scaled(
        rect.size().toSize(), Qt::KeepAspectRatio, Qt::FastTransformation);
    QRectF iconRect = QRectF(0, 0, scaledIcon.width(), scaledIcon.height());
    iconRect.moveCenter(rect.center());
    painter->drawPixmap(iconRect.topLeft(), scaledIcon);
  }
  /*
// draw frame:
painter->setBrush(Qt::NoBrush);
painter->setPen(Qt::black);
painter->drawRect(rect.adjusted(1, 1, 0, 0));
*/
}
/* end of 'src/plottables/plottable-colormap.cpp' */

/* including file 'src/plottables/plottable-financial.cpp' */
/* modified 2022-11-06T12:45:57, size 42914                */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPFinancialData
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPFinancialData
  \brief Holds the data of one single data point for QCPFinancial.

  The stored data is:
  \li \a key: coordinate on the key axis of this data point (this is the \a
  mainKey and the \a sortKey) \li \a open: The opening value at the data point
  (this is the \a mainValue) \li \a high: The high/maximum value at the data
  point \li \a low: The low/minimum value at the data point \li \a close: The
  closing value at the data point

  The container for storing multiple data points is \ref
  QCPFinancialDataContainer. It is a typedef for \ref QCPDataContainer with \ref
  QCPFinancialData as the DataType template parameter. See the documentation
  there for an explanation regarding the data type's generic methods.

  \see QCPFinancialDataContainer
*/

/* start documentation of inline functions */

/*! \fn double QCPFinancialData::sortKey() const

  Returns the \a key member of this data point.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/*! \fn static QCPFinancialData QCPFinancialData::fromSortKey(double sortKey)

  Returns a data point with the specified \a sortKey. All other members are set
  to zero.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/*! \fn static static bool QCPFinancialData::sortKeyIsMainKey()

  Since the member \a key is both the data point key coordinate and the data
  ordering parameter, this method returns true.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/*! \fn double QCPFinancialData::mainKey() const

  Returns the \a key member of this data point.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/*! \fn double QCPFinancialData::mainValue() const

  Returns the \a open member of this data point.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/*! \fn QCPRange QCPFinancialData::valueRange() const

  Returns a QCPRange spanning from the \a low to the \a high value of this data
  point.

  For a general explanation of what this method is good for in the context of
  the data container, see the documentation of \ref QCPDataContainer.
*/

/* end documentation of inline functions */

/*!
  Constructs a data point with key and all values set to zero.
*/
QCPFinancialData::QCPFinancialData()
    : key(0), open(0), high(0), low(0), close(0) {}

/*!
  Constructs a data point with the specified \a key and OHLC values.
*/
QCPFinancialData::QCPFinancialData(double key, double open, double high,
                                   double low, double close)
    : key(key), open(open), high(high), low(low), close(close) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPFinancial
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPFinancial
  \brief A plottable representing a financial stock chart

  \image html QCPFinancial.png

  This plottable represents time series data binned to certain intervals, mainly
  used for stock charts. The two common representations OHLC
  (Open-High-Low-Close) bars and Candlesticks can be set via \ref setChartStyle.

  The data is passed via \ref setData as a set of open/high/low/close values at
  certain keys (typically times). This means the data must be already binned
  appropriately. If data is only available as a series of values (e.g. \a price
  against \a time), you can use the static convenience function \ref
  timeSeriesToOhlc to generate binned OHLC-data which can then be passed to \ref
  setData.

  The width of the OHLC bars/candlesticks can be controlled with \ref setWidth
  and \ref setWidthType. A typical choice is to set the width type to \ref
  wtPlotCoords (the default) and the width to (or slightly less than) one time
  bin interval width.

  \section qcpfinancial-appearance Changing the appearance

  Charts can be either single- or two-colored (\ref setTwoColored). If set to be
  single-colored, lines are drawn with the plottable's pen (\ref setPen) and
  fills with the brush (\ref setBrush).

  If set to two-colored, positive changes of the value during an interval (\a
  close >= \a open) are represented with a different pen and brush than negative
  changes (\a close < \a open). These can be configured with \ref
  setPenPositive, \ref setPenNegative, \ref setBrushPositive, and \ref
  setBrushNegative. In two-colored mode, the normal plottable pen/brush is
  ignored. Upon selection however, the normal selected pen/brush (provided by
  the \ref selectionDecorator) is used, irrespective of whether the chart is
  single- or two-colored.

  \section qcpfinancial-usage Usage

  Like all data representing objects in QCustomPlot, the QCPFinancial is a
  plottable (QCPAbstractPlottable). So the plottable-interface of QCustomPlot
  applies (QCustomPlot::plottable, QCustomPlot::removePlottable, etc.)

  Usually, you first create an instance:

  \snippet documentation/doc-code-snippets/mainwindow.cpp
  qcpfinancial-creation-1 which registers it with the QCustomPlot instance of
  the passed axes. Note that this QCustomPlot instance takes ownership of the
  plottable, so do not delete it manually but use QCustomPlot::removePlottable()
  instead. The newly created plottable can be modified, e.g.:

  \snippet documentation/doc-code-snippets/mainwindow.cpp
  qcpfinancial-creation-2 Here we have used the static helper method \ref
  timeSeriesToOhlc, to turn a time-price data series into a 24-hour binned
  open-high-low-close data series as QCPFinancial uses.
*/

/* start of documentation of inline functions */

/*! \fn QCPFinancialDataContainer *QCPFinancial::data() const

  Returns a pointer to the internal data storage of type \ref
  QCPFinancialDataContainer. You may use it to directly manipulate the data,
  which may be more convenient and faster than using the regular \ref setData or
  \ref addData methods, in certain situations.
*/

/* end of documentation of inline functions */

/*!
  Constructs a financial chart which uses \a keyAxis as its key axis ("x") and
  \a valueAxis as its value axis ("y"). \a keyAxis and \a valueAxis must reside
  in the same QCustomPlot instance and not have the same orientation. If either
  of these restrictions is violated, a corresponding message is printed to the
  debug output (qDebug), the construction is not aborted, though.

  The created QCPFinancial is automatically registered with the QCustomPlot
  instance inferred from \a keyAxis. This QCustomPlot instance takes ownership
  of the QCPFinancial, so do not delete it manually but use
  QCustomPlot::removePlottable() instead.
*/
QCPFinancial::QCPFinancial(QCPAxis *keyAxis, QCPAxis *valueAxis)
    : QCPAbstractPlottable1D<QCPFinancialData>(keyAxis, valueAxis),
      mChartStyle(csCandlestick),
      mWidth(0.5),
      mWidthType(wtPlotCoords),
      mTwoColored(true),
      mBrushPositive(QBrush(QColor(50, 160, 0))),
      mBrushNegative(QBrush(QColor(180, 0, 15))),
      mPenPositive(QPen(QColor(40, 150, 0))),
      mPenNegative(QPen(QColor(170, 5, 5))) {
  mSelectionDecorator->setBrush(QBrush(QColor(160, 160, 255)));
}

QCPFinancial::~QCPFinancial() {}

/*! \overload

  Replaces the current data container with the provided \a data container.

  Since a QSharedPointer is used, multiple QCPFinancials may share the same data
  container safely. Modifying the data in the container will then affect all
  financials that share the container. Sharing can be achieved by simply
  exchanging the data containers wrapped in shared pointers: \snippet
  documentation/doc-code-snippets/mainwindow.cpp qcpfinancial-datasharing-1

  If you do not wish to share containers, but create a copy from an existing
  container, rather use the \ref QCPDataContainer<DataType>::set method on the
  financial's data container directly: \snippet
  documentation/doc-code-snippets/mainwindow.cpp qcpfinancial-datasharing-2

  \see addData, timeSeriesToOhlc
*/
void QCPFinancial::setData(QSharedPointer<QCPFinancialDataContainer> data) {
  mDataContainer = data;
}

/*! \overload

  Replaces the current data with the provided points in \a keys, \a open, \a
  high, \a low and \a close. The provided vectors should have equal length.
  Else, the number of added points will be the size of the smallest vector.

  If you can guarantee that the passed data points are sorted by \a keys in
  ascending order, you can set \a alreadySorted to true, to improve performance
  by saving a sorting run.

  \see addData, timeSeriesToOhlc
*/
void QCPFinancial::setData(const QVector<double> &keys,
                           const QVector<double> &open,
                           const QVector<double> &high,
                           const QVector<double> &low,
                           const QVector<double> &close, bool alreadySorted) {
  mDataContainer->clear();
  addData(keys, open, high, low, close, alreadySorted);
}

/*!
  Sets which representation style shall be used to display the OHLC data.
*/
void QCPFinancial::setChartStyle(QCPFinancial::ChartStyle style) {
  mChartStyle = style;
}

/*!
  Sets the width of the individual bars/candlesticks to \a width in plot key
  coordinates.

  A typical choice is to set it to (or slightly less than) one bin interval
  width.
*/
void QCPFinancial::setWidth(double width) { mWidth = width; }

/*!
  Sets how the width of the financial bars is defined. See the documentation of
  \ref WidthType for an explanation of the possible values for \a widthType.

  The default value is \ref wtPlotCoords.

  \see setWidth
*/
void QCPFinancial::setWidthType(QCPFinancial::WidthType widthType) {
  mWidthType = widthType;
}

/*!
  Sets whether this chart shall contrast positive from negative trends per data
  point by using two separate colors to draw the respective bars/candlesticks.

  If \a twoColored is false, the normal plottable's pen and brush are used (\ref
  setPen, \ref setBrush).

  \see setPenPositive, setPenNegative, setBrushPositive, setBrushNegative
*/
void QCPFinancial::setTwoColored(bool twoColored) { mTwoColored = twoColored; }

/*!
  If \ref setTwoColored is set to true, this function controls the brush that is
  used to draw fills of data points with a positive trend (i.e.
  bars/candlesticks with close >= open).

  If \a twoColored is false, the normal plottable's pen and brush are used (\ref
  setPen, \ref setBrush).

  \see setBrushNegative, setPenPositive, setPenNegative
*/
void QCPFinancial::setBrushPositive(const QBrush &brush) {
  mBrushPositive = brush;
}

/*!
  If \ref setTwoColored is set to true, this function controls the brush that is
  used to draw fills of data points with a negative trend (i.e.
  bars/candlesticks with close < open).

  If \a twoColored is false, the normal plottable's pen and brush are used (\ref
  setPen, \ref setBrush).

  \see setBrushPositive, setPenNegative, setPenPositive
*/
void QCPFinancial::setBrushNegative(const QBrush &brush) {
  mBrushNegative = brush;
}

/*!
  If \ref setTwoColored is set to true, this function controls the pen that is
  used to draw outlines of data points with a positive trend (i.e.
  bars/candlesticks with close >= open).

  If \a twoColored is false, the normal plottable's pen and brush are used (\ref
  setPen, \ref setBrush).

  \see setPenNegative, setBrushPositive, setBrushNegative
*/
void QCPFinancial::setPenPositive(const QPen &pen) { mPenPositive = pen; }

/*!
  If \ref setTwoColored is set to true, this function controls the pen that is
  used to draw outlines of data points with a negative trend (i.e.
  bars/candlesticks with close < open).

  If \a twoColored is false, the normal plottable's pen and brush are used (\ref
  setPen, \ref setBrush).

  \see setPenPositive, setBrushNegative, setBrushPositive
*/
void QCPFinancial::setPenNegative(const QPen &pen) { mPenNegative = pen; }

/*! \overload

  Adds the provided points in \a keys, \a open, \a high, \a low and \a close to
  the current data. The provided vectors should have equal length. Else, the
  number of added points will be the size of the smallest vector.

  If you can guarantee that the passed data points are sorted by \a keys in
  ascending order, you can set \a alreadySorted to true, to improve performance
  by saving a sorting run.

  Alternatively, you can also access and modify the data directly via the \ref
  data method, which returns a pointer to the internal data container.

  \see timeSeriesToOhlc
*/
void QCPFinancial::addData(const QVector<double> &keys,
                           const QVector<double> &open,
                           const QVector<double> &high,
                           const QVector<double> &low,
                           const QVector<double> &close, bool alreadySorted) {
  if (keys.size() != open.size() || open.size() != high.size() ||
      high.size() != low.size() || low.size() != close.size() ||
      close.size() != keys.size())
    qDebug() << Q_FUNC_INFO
             << "keys, open, high, low, close have different sizes:"
             << keys.size() << open.size() << high.size() << low.size()
             << close.size();
  const int n = qMin(
      keys.size(),
      qMin(open.size(), qMin(high.size(), qMin(low.size(), close.size()))));
  QVector<QCPFinancialData> tempData(n);
  QVector<QCPFinancialData>::iterator it = tempData.begin();
  const QVector<QCPFinancialData>::iterator itEnd = tempData.end();
  int i = 0;
  while (it != itEnd) {
    it->key = keys[i];
    it->open = open[i];
    it->high = high[i];
    it->low = low[i];
    it->close = close[i];
    ++it;
    ++i;
  }
  mDataContainer->add(tempData,
                      alreadySorted);  // don't modify tempData beyond this to
                                       // prevent copy on write
}

/*! \overload

  Adds the provided data point as \a key, \a open, \a high, \a low and \a close
  to the current data.

  Alternatively, you can also access and modify the data directly via the \ref
  data method, which returns a pointer to the internal data container.

  \see timeSeriesToOhlc
*/
void QCPFinancial::addData(double key, double open, double high, double low,
                           double close) {
  mDataContainer->add(QCPFinancialData(key, open, high, low, close));
}

/*!
  \copydoc QCPPlottableInterface1D::selectTestRect
*/
QCPDataSelection QCPFinancial::selectTestRect(const QRectF &rect,
                                              bool onlySelectable) const {
  QCPDataSelection result;
  if ((onlySelectable && mSelectable == QCP::stNone) ||
      mDataContainer->isEmpty())
    return result;
  if (!mKeyAxis || !mValueAxis) return result;

  QCPFinancialDataContainer::const_iterator visibleBegin, visibleEnd;
  getVisibleDataBounds(visibleBegin, visibleEnd);

  for (QCPFinancialDataContainer::const_iterator it = visibleBegin;
       it != visibleEnd; ++it) {
    if (rect.intersects(selectionHitBox(it)))
      result.addDataRange(
          QCPDataRange(int(it - mDataContainer->constBegin()),
                       int(it - mDataContainer->constBegin() + 1)),
          false);
  }
  result.simplify();
  return result;
}

/*!
  Implements a selectTest specific to this plottable's point geometry.

  If \a details is not 0, it will be set to a \ref QCPDataSelection, describing
  the closest data point to \a pos.

  \seebaseclassmethod \ref QCPAbstractPlottable::selectTest
*/
double QCPFinancial::selectTest(const QPointF &pos, bool onlySelectable,
                                QVariant *details) const {
  Q_UNUSED(details)
  if ((onlySelectable && mSelectable == QCP::stNone) ||
      mDataContainer->isEmpty())
    return -1;
  if (!mKeyAxis || !mValueAxis) return -1;

  if (mKeyAxis.data()->axisRect()->rect().contains(pos.toPoint()) ||
      mParentPlot->interactions().testFlag(
          QCP::iSelectPlottablesBeyondAxisRect)) {
    // get visible data range:
    QCPFinancialDataContainer::const_iterator visibleBegin, visibleEnd;
    QCPFinancialDataContainer::const_iterator closestDataPoint =
        mDataContainer->constEnd();
    getVisibleDataBounds(visibleBegin, visibleEnd);
    // perform select test according to configured style:
    double result = -1;
    switch (mChartStyle) {
      case QCPFinancial::csOhlc:
        result =
            ohlcSelectTest(pos, visibleBegin, visibleEnd, closestDataPoint);
        break;
      case QCPFinancial::csCandlestick:
        result = candlestickSelectTest(pos, visibleBegin, visibleEnd,
                                       closestDataPoint);
        break;
    }
    if (details) {
      int pointIndex = int(closestDataPoint - mDataContainer->constBegin());
      details->setValue(
          QCPDataSelection(QCPDataRange(pointIndex, pointIndex + 1)));
    }
    return result;
  }

  return -1;
}

/* inherits documentation from base class */
QCPRange QCPFinancial::getKeyRange(bool &foundRange,
                                   QCP::SignDomain inSignDomain) const {
  QCPRange range = mDataContainer->keyRange(foundRange, inSignDomain);
  // determine exact range by including width of bars/flags:
  if (foundRange) {
    if (inSignDomain != QCP::sdPositive || range.lower - mWidth * 0.5 > 0)
      range.lower -= mWidth * 0.5;
    if (inSignDomain != QCP::sdNegative || range.upper + mWidth * 0.5 < 0)
      range.upper += mWidth * 0.5;
  }
  return range;
}

/* inherits documentation from base class */
QCPRange QCPFinancial::getValueRange(bool &foundRange,
                                     QCP::SignDomain inSignDomain,
                                     const QCPRange &inKeyRange) const {
  return mDataContainer->valueRange(foundRange, inSignDomain, inKeyRange);
}

/*!
  A convenience function that converts time series data (\a value against \a
  time) to OHLC binned data points. The return value can then be passed on to
  \ref QCPFinancialDataContainer::set(const QCPFinancialDataContainer&).

  The size of the bins can be controlled with \a timeBinSize in the same units
  as \a time is given. For example, if the unit of \a time is seconds and single
  OHLC/Candlesticks should span an hour each, set \a timeBinSize to 3600.

  \a timeBinOffset allows to control precisely at what \a time coordinate a bin
  should start. The value passed as \a timeBinOffset doesn't need to be in the
  range encompassed by the \a time keys. It merely defines the mathematical
  offset/phase of the bins that will be used to process the data.
*/
QCPFinancialDataContainer QCPFinancial::timeSeriesToOhlc(
    const QVector<double> &time, const QVector<double> &value,
    double timeBinSize, double timeBinOffset) {
  QCPFinancialDataContainer data;
  int count = qMin(time.size(), value.size());
  if (count == 0) return QCPFinancialDataContainer();

  QCPFinancialData currentBinData(0, value.first(), value.first(),
                                  value.first(), value.first());
  int currentBinIndex =
      qFloor((time.first() - timeBinOffset) / timeBinSize + 0.5);
  for (int i = 0; i < count; ++i) {
    int index = qFloor((time.at(i) - timeBinOffset) / timeBinSize + 0.5);
    if (currentBinIndex ==
        index)  // data point still in current bin, extend high/low:
    {
      if (value.at(i) < currentBinData.low) currentBinData.low = value.at(i);
      if (value.at(i) > currentBinData.high) currentBinData.high = value.at(i);
      if (i == count - 1)  // last data point is in current bin, finalize bin:
      {
        currentBinData.close = value.at(i);
        currentBinData.key = timeBinOffset + (index)*timeBinSize;
        data.add(currentBinData);
      }
    } else  // data point not anymore in current bin, set close of old and open
    // of new bin, and add old to map:
    {
      // finalize current bin:
      currentBinData.close = value.at(i - 1);
      currentBinData.key = timeBinOffset + (index - 1) * timeBinSize;
      data.add(currentBinData);
      // start next bin:
      currentBinIndex = index;
      currentBinData.open = value.at(i);
      currentBinData.high = value.at(i);
      currentBinData.low = value.at(i);
    }
  }

  return data;
}

/* inherits documentation from base class */
void QCPFinancial::draw(QCPPainter *painter) {
  // get visible data range:
  QCPFinancialDataContainer::const_iterator visibleBegin, visibleEnd;
  getVisibleDataBounds(visibleBegin, visibleEnd);

  // loop over and draw segments of unselected/selected data:
  QList<QCPDataRange> selectedSegments, unselectedSegments, allSegments;
  getDataSegments(selectedSegments, unselectedSegments);
  allSegments << unselectedSegments << selectedSegments;
  for (int i = 0; i < allSegments.size(); ++i) {
    bool isSelectedSegment = i >= unselectedSegments.size();
    QCPFinancialDataContainer::const_iterator begin = visibleBegin;
    QCPFinancialDataContainer::const_iterator end = visibleEnd;
    mDataContainer->limitIteratorsToDataRange(begin, end, allSegments.at(i));
    if (begin == end) continue;

    // draw data segment according to configured style:
    switch (mChartStyle) {
      case QCPFinancial::csOhlc:
        drawOhlcPlot(painter, begin, end, isSelectedSegment);
        break;
      case QCPFinancial::csCandlestick:
        drawCandlestickPlot(painter, begin, end, isSelectedSegment);
        break;
    }
  }

  // draw other selection decoration that isn't just line/scatter pens and
  // brushes:
  if (mSelectionDecorator)
    mSelectionDecorator->drawDecoration(painter, selection());
}

/* inherits documentation from base class */
void QCPFinancial::drawLegendIcon(QCPPainter *painter,
                                  const QRectF &rect) const {
  painter->setAntialiasing(false);  // legend icon especially of csCandlestick
                                    // looks better without antialiasing
  if (mChartStyle == csOhlc) {
    if (mTwoColored) {
      // draw upper left half icon with positive color:
      painter->setBrush(mBrushPositive);
      painter->setPen(mPenPositive);
      painter->setClipRegion(QRegion(QPolygon() << rect.bottomLeft().toPoint()
                                                << rect.topRight().toPoint()
                                                << rect.topLeft().toPoint()));
      painter->drawLine(
          QLineF(0, rect.height() * 0.5, rect.width(), rect.height() * 0.5)
              .translated(rect.topLeft()));
      painter->drawLine(QLineF(rect.width() * 0.2, rect.height() * 0.3,
                               rect.width() * 0.2, rect.height() * 0.5)
                            .translated(rect.topLeft()));
      painter->drawLine(QLineF(rect.width() * 0.8, rect.height() * 0.5,
                               rect.width() * 0.8, rect.height() * 0.7)
                            .translated(rect.topLeft()));
      // draw bottom right half icon with negative color:
      painter->setBrush(mBrushNegative);
      painter->setPen(mPenNegative);
      painter->setClipRegion(QRegion(
          QPolygon() << rect.bottomLeft().toPoint() << rect.topRight().toPoint()
                     << rect.bottomRight().toPoint()));
      painter->drawLine(
          QLineF(0, rect.height() * 0.5, rect.width(), rect.height() * 0.5)
              .translated(rect.topLeft()));
      painter->drawLine(QLineF(rect.width() * 0.2, rect.height() * 0.3,
                               rect.width() * 0.2, rect.height() * 0.5)
                            .translated(rect.topLeft()));
      painter->drawLine(QLineF(rect.width() * 0.8, rect.height() * 0.5,
                               rect.width() * 0.8, rect.height() * 0.7)
                            .translated(rect.topLeft()));
    } else {
      painter->setBrush(mBrush);
      painter->setPen(mPen);
      painter->drawLine(
          QLineF(0, rect.height() * 0.5, rect.width(), rect.height() * 0.5)
              .translated(rect.topLeft()));
      painter->drawLine(QLineF(rect.width() * 0.2, rect.height() * 0.3,
                               rect.width() * 0.2, rect.height() * 0.5)
                            .translated(rect.topLeft()));
      painter->drawLine(QLineF(rect.width() * 0.8, rect.height() * 0.5,
                               rect.width() * 0.8, rect.height() * 0.7)
                            .translated(rect.topLeft()));
    }
  } else if (mChartStyle == csCandlestick) {
    if (mTwoColored) {
      // draw upper left half icon with positive color:
      painter->setBrush(mBrushPositive);
      painter->setPen(mPenPositive);
      painter->setClipRegion(QRegion(QPolygon() << rect.bottomLeft().toPoint()
                                                << rect.topRight().toPoint()
                                                << rect.topLeft().toPoint()));
      painter->drawLine(QLineF(0, rect.height() * 0.5, rect.width() * 0.25,
                               rect.height() * 0.5)
                            .translated(rect.topLeft()));
      painter->drawLine(QLineF(rect.width() * 0.75, rect.height() * 0.5,
                               rect.width(), rect.height() * 0.5)
                            .translated(rect.topLeft()));
      painter->drawRect(QRectF(rect.width() * 0.25, rect.height() * 0.25,
                               rect.width() * 0.5, rect.height() * 0.5)
                            .translated(rect.topLeft()));
      // draw bottom right half icon with negative color:
      painter->setBrush(mBrushNegative);
      painter->setPen(mPenNegative);
      painter->setClipRegion(QRegion(
          QPolygon() << rect.bottomLeft().toPoint() << rect.topRight().toPoint()
                     << rect.bottomRight().toPoint()));
      painter->drawLine(QLineF(0, rect.height() * 0.5, rect.width() * 0.25,
                               rect.height() * 0.5)
                            .translated(rect.topLeft()));
      painter->drawLine(QLineF(rect.width() * 0.75, rect.height() * 0.5,
                               rect.width(), rect.height() * 0.5)
                            .translated(rect.topLeft()));
      painter->drawRect(QRectF(rect.width() * 0.25, rect.height() * 0.25,
                               rect.width() * 0.5, rect.height() * 0.5)
                            .translated(rect.topLeft()));
    } else {
      painter->setBrush(mBrush);
      painter->setPen(mPen);
      painter->drawLine(QLineF(0, rect.height() * 0.5, rect.width() * 0.25,
                               rect.height() * 0.5)
                            .translated(rect.topLeft()));
      painter->drawLine(QLineF(rect.width() * 0.75, rect.height() * 0.5,
                               rect.width(), rect.height() * 0.5)
                            .translated(rect.topLeft()));
      painter->drawRect(QRectF(rect.width() * 0.25, rect.height() * 0.25,
                               rect.width() * 0.5, rect.height() * 0.5)
                            .translated(rect.topLeft()));
    }
  }
}

/*! \internal

  Draws the data from \a begin to \a end-1 as OHLC bars with the provided \a
  painter.

  This method is a helper function for \ref draw. It is used when the chart
  style is \ref csOhlc.
*/
void QCPFinancial::drawOhlcPlot(
    QCPPainter *painter, const QCPFinancialDataContainer::const_iterator &begin,
    const QCPFinancialDataContainer::const_iterator &end, bool isSelected) {
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    return;
  }

  if (keyAxis->orientation() == Qt::Horizontal) {
    for (QCPFinancialDataContainer::const_iterator it = begin; it != end;
         ++it) {
      if (isSelected && mSelectionDecorator)
        mSelectionDecorator->applyPen(painter);
      else if (mTwoColored)
        painter->setPen(it->close >= it->open ? mPenPositive : mPenNegative);
      else
        painter->setPen(mPen);
      double keyPixel = keyAxis->coordToPixel(it->key);
      double openPixel = valueAxis->coordToPixel(it->open);
      double closePixel = valueAxis->coordToPixel(it->close);
      // draw backbone:
      painter->drawLine(QPointF(keyPixel, valueAxis->coordToPixel(it->high)),
                        QPointF(keyPixel, valueAxis->coordToPixel(it->low)));
      // draw open:
      double pixelWidth = getPixelWidth(
          it->key,
          keyPixel);  // sign of this makes sure open/close are on correct sides
      painter->drawLine(QPointF(keyPixel - pixelWidth, openPixel),
                        QPointF(keyPixel, openPixel));
      // draw close:
      painter->drawLine(QPointF(keyPixel, closePixel),
                        QPointF(keyPixel + pixelWidth, closePixel));
    }
  } else {
    for (QCPFinancialDataContainer::const_iterator it = begin; it != end;
         ++it) {
      if (isSelected && mSelectionDecorator)
        mSelectionDecorator->applyPen(painter);
      else if (mTwoColored)
        painter->setPen(it->close >= it->open ? mPenPositive : mPenNegative);
      else
        painter->setPen(mPen);
      double keyPixel = keyAxis->coordToPixel(it->key);
      double openPixel = valueAxis->coordToPixel(it->open);
      double closePixel = valueAxis->coordToPixel(it->close);
      // draw backbone:
      painter->drawLine(QPointF(valueAxis->coordToPixel(it->high), keyPixel),
                        QPointF(valueAxis->coordToPixel(it->low), keyPixel));
      // draw open:
      double pixelWidth = getPixelWidth(
          it->key,
          keyPixel);  // sign of this makes sure open/close are on correct sides
      painter->drawLine(QPointF(openPixel, keyPixel - pixelWidth),
                        QPointF(openPixel, keyPixel));
      // draw close:
      painter->drawLine(QPointF(closePixel, keyPixel),
                        QPointF(closePixel, keyPixel + pixelWidth));
    }
  }
}

/*! \internal

  Draws the data from \a begin to \a end-1 as Candlesticks with the provided \a
  painter.

  This method is a helper function for \ref draw. It is used when the chart
  style is \ref csCandlestick.
*/
void QCPFinancial::drawCandlestickPlot(
    QCPPainter *painter, const QCPFinancialDataContainer::const_iterator &begin,
    const QCPFinancialDataContainer::const_iterator &end, bool isSelected) {
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    return;
  }

  if (keyAxis->orientation() == Qt::Horizontal) {
    for (QCPFinancialDataContainer::const_iterator it = begin; it != end;
         ++it) {
      if (isSelected && mSelectionDecorator) {
        mSelectionDecorator->applyPen(painter);
        mSelectionDecorator->applyBrush(painter);
      } else if (mTwoColored) {
        painter->setPen(it->close >= it->open ? mPenPositive : mPenNegative);
        painter->setBrush(it->close >= it->open ? mBrushPositive
                                                : mBrushNegative);
      } else {
        painter->setPen(mPen);
        painter->setBrush(mBrush);
      }
      double keyPixel = keyAxis->coordToPixel(it->key);
      double openPixel = valueAxis->coordToPixel(it->open);
      double closePixel = valueAxis->coordToPixel(it->close);
      // draw high:
      painter->drawLine(QPointF(keyPixel, valueAxis->coordToPixel(it->high)),
                        QPointF(keyPixel, valueAxis->coordToPixel(
                                              qMax(it->open, it->close))));
      // draw low:
      painter->drawLine(QPointF(keyPixel, valueAxis->coordToPixel(it->low)),
                        QPointF(keyPixel, valueAxis->coordToPixel(
                                              qMin(it->open, it->close))));
      // draw open-close box:
      double pixelWidth = getPixelWidth(it->key, keyPixel);
      painter->drawRect(QRectF(QPointF(keyPixel - pixelWidth, closePixel),
                               QPointF(keyPixel + pixelWidth, openPixel)));
    }
  } else  // keyAxis->orientation() == Qt::Vertical
  {
    for (QCPFinancialDataContainer::const_iterator it = begin; it != end;
         ++it) {
      if (isSelected && mSelectionDecorator) {
        mSelectionDecorator->applyPen(painter);
        mSelectionDecorator->applyBrush(painter);
      } else if (mTwoColored) {
        painter->setPen(it->close >= it->open ? mPenPositive : mPenNegative);
        painter->setBrush(it->close >= it->open ? mBrushPositive
                                                : mBrushNegative);
      } else {
        painter->setPen(mPen);
        painter->setBrush(mBrush);
      }
      double keyPixel = keyAxis->coordToPixel(it->key);
      double openPixel = valueAxis->coordToPixel(it->open);
      double closePixel = valueAxis->coordToPixel(it->close);
      // draw high:
      painter->drawLine(
          QPointF(valueAxis->coordToPixel(it->high), keyPixel),
          QPointF(valueAxis->coordToPixel(qMax(it->open, it->close)),
                  keyPixel));
      // draw low:
      painter->drawLine(
          QPointF(valueAxis->coordToPixel(it->low), keyPixel),
          QPointF(valueAxis->coordToPixel(qMin(it->open, it->close)),
                  keyPixel));
      // draw open-close box:
      double pixelWidth = getPixelWidth(it->key, keyPixel);
      painter->drawRect(QRectF(QPointF(closePixel, keyPixel - pixelWidth),
                               QPointF(openPixel, keyPixel + pixelWidth)));
    }
  }
}

/*! \internal

  This function is used to determine the width of the bar at coordinate \a key,
  according to the specified width (\ref setWidth) and width type (\ref
  setWidthType). Provide the pixel position of \a key in \a keyPixel (because
  usually this was already calculated via \ref QCPAxis::coordToPixel when this
  function is called).

  It returns the number of pixels the bar extends to higher keys, relative to
  the \a key coordinate. So with a non-reversed horizontal axis, the return
  value is positive. With a reversed horizontal axis, the return value is
  negative. This is important so the open/close flags on the \ref csOhlc bar are
  drawn to the correct side.
*/
double QCPFinancial::getPixelWidth(double key, double keyPixel) const {
  double result = 0;
  switch (mWidthType) {
    case wtAbsolute: {
      if (mKeyAxis) result = mWidth * 0.5 * mKeyAxis.data()->pixelOrientation();
      break;
    }
    case wtAxisRectRatio: {
      if (mKeyAxis && mKeyAxis.data()->axisRect()) {
        if (mKeyAxis.data()->orientation() == Qt::Horizontal)
          result = mKeyAxis.data()->axisRect()->width() * mWidth * 0.5 *
                   mKeyAxis.data()->pixelOrientation();
        else
          result = mKeyAxis.data()->axisRect()->height() * mWidth * 0.5 *
                   mKeyAxis.data()->pixelOrientation();
      } else
        qDebug() << Q_FUNC_INFO << "No key axis or axis rect defined";
      break;
    }
    case wtPlotCoords: {
      if (mKeyAxis)
        result = mKeyAxis.data()->coordToPixel(key + mWidth * 0.5) - keyPixel;
      else
        qDebug() << Q_FUNC_INFO << "No key axis defined";
      break;
    }
  }
  return result;
}

/*! \internal

  This method is a helper function for \ref selectTest. It is used to test for
  selection when the chart style is \ref csOhlc. It only tests against the data
  points between \a begin and \a end.

  Like \ref selectTest, this method returns the shortest distance of \a pos to
  the graphical representation of the plottable, and \a closestDataPoint will
  point to the respective data point.
*/
double QCPFinancial::ohlcSelectTest(
    const QPointF &pos, const QCPFinancialDataContainer::const_iterator &begin,
    const QCPFinancialDataContainer::const_iterator &end,
    QCPFinancialDataContainer::const_iterator &closestDataPoint) const {
  closestDataPoint = mDataContainer->constEnd();
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    return -1;
  }

  double minDistSqr = (std::numeric_limits<double>::max)();
  if (keyAxis->orientation() == Qt::Horizontal) {
    for (QCPFinancialDataContainer::const_iterator it = begin; it != end;
         ++it) {
      double keyPixel = keyAxis->coordToPixel(it->key);
      // calculate distance to backbone:
      double currentDistSqr = QCPVector2D(pos).distanceSquaredToLine(
          QCPVector2D(keyPixel, valueAxis->coordToPixel(it->high)),
          QCPVector2D(keyPixel, valueAxis->coordToPixel(it->low)));
      if (currentDistSqr < minDistSqr) {
        minDistSqr = currentDistSqr;
        closestDataPoint = it;
      }
    }
  } else  // keyAxis->orientation() == Qt::Vertical
  {
    for (QCPFinancialDataContainer::const_iterator it = begin; it != end;
         ++it) {
      double keyPixel = keyAxis->coordToPixel(it->key);
      // calculate distance to backbone:
      double currentDistSqr = QCPVector2D(pos).distanceSquaredToLine(
          QCPVector2D(valueAxis->coordToPixel(it->high), keyPixel),
          QCPVector2D(valueAxis->coordToPixel(it->low), keyPixel));
      if (currentDistSqr < minDistSqr) {
        minDistSqr = currentDistSqr;
        closestDataPoint = it;
      }
    }
  }
  return qSqrt(minDistSqr);
}

/*! \internal

  This method is a helper function for \ref selectTest. It is used to test for
  selection when the chart style is \ref csCandlestick. It only tests against
  the data points between \a begin and \a end.

  Like \ref selectTest, this method returns the shortest distance of \a pos to
  the graphical representation of the plottable, and \a closestDataPoint will
  point to the respective data point.
*/
double QCPFinancial::candlestickSelectTest(
    const QPointF &pos, const QCPFinancialDataContainer::const_iterator &begin,
    const QCPFinancialDataContainer::const_iterator &end,
    QCPFinancialDataContainer::const_iterator &closestDataPoint) const {
  closestDataPoint = mDataContainer->constEnd();
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    return -1;
  }

  double minDistSqr = (std::numeric_limits<double>::max)();
  if (keyAxis->orientation() == Qt::Horizontal) {
    for (QCPFinancialDataContainer::const_iterator it = begin; it != end;
         ++it) {
      double currentDistSqr;
      // determine whether pos is in open-close-box:
      QCPRange boxKeyRange(it->key - mWidth * 0.5, it->key + mWidth * 0.5);
      QCPRange boxValueRange(it->close, it->open);
      double posKey, posValue;
      pixelsToCoords(pos, posKey, posValue);
      if (boxKeyRange.contains(posKey) &&
          boxValueRange.contains(posValue))  // is in open-close-box
      {
        currentDistSqr = mParentPlot->selectionTolerance() * 0.99 *
                         mParentPlot->selectionTolerance() * 0.99;
      } else {
        // calculate distance to high/low lines:
        double keyPixel = keyAxis->coordToPixel(it->key);
        double highLineDistSqr = QCPVector2D(pos).distanceSquaredToLine(
            QCPVector2D(keyPixel, valueAxis->coordToPixel(it->high)),
            QCPVector2D(keyPixel,
                        valueAxis->coordToPixel(qMax(it->open, it->close))));
        double lowLineDistSqr = QCPVector2D(pos).distanceSquaredToLine(
            QCPVector2D(keyPixel, valueAxis->coordToPixel(it->low)),
            QCPVector2D(keyPixel,
                        valueAxis->coordToPixel(qMin(it->open, it->close))));
        currentDistSqr = qMin(highLineDistSqr, lowLineDistSqr);
      }
      if (currentDistSqr < minDistSqr) {
        minDistSqr = currentDistSqr;
        closestDataPoint = it;
      }
    }
  } else  // keyAxis->orientation() == Qt::Vertical
  {
    for (QCPFinancialDataContainer::const_iterator it = begin; it != end;
         ++it) {
      double currentDistSqr;
      // determine whether pos is in open-close-box:
      QCPRange boxKeyRange(it->key - mWidth * 0.5, it->key + mWidth * 0.5);
      QCPRange boxValueRange(it->close, it->open);
      double posKey, posValue;
      pixelsToCoords(pos, posKey, posValue);
      if (boxKeyRange.contains(posKey) &&
          boxValueRange.contains(posValue))  // is in open-close-box
      {
        currentDistSqr = mParentPlot->selectionTolerance() * 0.99 *
                         mParentPlot->selectionTolerance() * 0.99;
      } else {
        // calculate distance to high/low lines:
        double keyPixel = keyAxis->coordToPixel(it->key);
        double highLineDistSqr = QCPVector2D(pos).distanceSquaredToLine(
            QCPVector2D(valueAxis->coordToPixel(it->high), keyPixel),
            QCPVector2D(valueAxis->coordToPixel(qMax(it->open, it->close)),
                        keyPixel));
        double lowLineDistSqr = QCPVector2D(pos).distanceSquaredToLine(
            QCPVector2D(valueAxis->coordToPixel(it->low), keyPixel),
            QCPVector2D(valueAxis->coordToPixel(qMin(it->open, it->close)),
                        keyPixel));
        currentDistSqr = qMin(highLineDistSqr, lowLineDistSqr);
      }
      if (currentDistSqr < minDistSqr) {
        minDistSqr = currentDistSqr;
        closestDataPoint = it;
      }
    }
  }
  return qSqrt(minDistSqr);
}

/*! \internal

  called by the drawing methods to determine which data (key) range is visible
  at the current key axis range setting, so only that needs to be processed.

  \a begin returns an iterator to the lowest data point that needs to be taken
  into account when plotting. Note that in order to get a clean plot all the way
  to the edge of the axis rect, \a begin may still be just outside the visible
  range.

  \a end returns the iterator just above the highest data point that needs to be
  taken into account. Same as before, \a end may also lie just outside of the
  visible range

  if the plottable contains no data, both \a begin and \a end point to \c
  constEnd.
*/
void QCPFinancial::getVisibleDataBounds(
    QCPFinancialDataContainer::const_iterator &begin,
    QCPFinancialDataContainer::const_iterator &end) const {
  if (!mKeyAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key axis";
    begin = mDataContainer->constEnd();
    end = mDataContainer->constEnd();
    return;
  }
  begin = mDataContainer->findBegin(
      mKeyAxis.data()->range().lower -
      mWidth * 0.5);  // subtract half width of ohlc/candlestick to include
                      // partially visible data points
  end = mDataContainer->findEnd(
      mKeyAxis.data()->range().upper +
      mWidth * 0.5);  // add half width of ohlc/candlestick to include partially
                      // visible data points
}

/*!  \internal

  Returns the hit box in pixel coordinates that will be used for data selection
  with the selection rect (\ref selectTestRect), of the data point given by \a
  it.
*/
QRectF QCPFinancial::selectionHitBox(
    QCPFinancialDataContainer::const_iterator it) const {
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    return {};
  }

  double keyPixel = keyAxis->coordToPixel(it->key);
  double highPixel = valueAxis->coordToPixel(it->high);
  double lowPixel = valueAxis->coordToPixel(it->low);
  double keyWidthPixels =
      keyPixel - keyAxis->coordToPixel(it->key - mWidth * 0.5);
  if (keyAxis->orientation() == Qt::Horizontal)
    return QRectF(keyPixel - keyWidthPixels, highPixel, keyWidthPixels * 2,
                  lowPixel - highPixel)
        .normalized();
  else
    return QRectF(highPixel, keyPixel - keyWidthPixels, lowPixel - highPixel,
                  keyWidthPixels * 2)
        .normalized();
}
/* end of 'src/plottables/plottable-financial.cpp' */

/* including file 'src/plottables/plottable-errorbar.cpp' */
/* modified 2022-11-06T12:45:56, size 37679               */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPErrorBarsData
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPErrorBarsData
  \brief Holds the data of one single error bar for QCPErrorBars.

  The stored data is:
  \li \a errorMinus: how much the error bar extends towards negative coordinates
  from the data point position \li \a errorPlus: how much the error bar extends
  towards positive coordinates from the data point position

  The container for storing the error bar information is \ref
  QCPErrorBarsDataContainer. It is a typedef for <tt>QVector<\ref
  QCPErrorBarsData></tt>.

  \see QCPErrorBarsDataContainer
*/

/*!
  Constructs an error bar with errors set to zero.
*/
QCPErrorBarsData::QCPErrorBarsData() : errorMinus(0), errorPlus(0) {}

/*!
  Constructs an error bar with equal \a error in both negative and positive
  direction.
*/
QCPErrorBarsData::QCPErrorBarsData(double error)
    : errorMinus(error), errorPlus(error) {}

/*!
  Constructs an error bar with negative and positive errors set to \a errorMinus
  and \a errorPlus, respectively.
*/
QCPErrorBarsData::QCPErrorBarsData(double errorMinus, double errorPlus)
    : errorMinus(errorMinus), errorPlus(errorPlus) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPErrorBars
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPErrorBars
  \brief A plottable that adds a set of error bars to other plottables.

  \image html QCPErrorBars.png

  The \ref QCPErrorBars plottable can be attached to other one-dimensional
  plottables (e.g. \ref QCPGraph, \ref QCPCurve, \ref QCPBars, etc.) and equips
  them with error bars.

  Use \ref setDataPlottable to define for which plottable the \ref QCPErrorBars
  shall display the error bars. The orientation of the error bars can be
  controlled with \ref setErrorType.

  By using \ref setData, you can supply the actual error data, either as
  symmetric error or plus/minus asymmetric errors. \ref QCPErrorBars only stores
  the error data. The absolute key/value position of each error bar will be
  adopted from the configured data plottable. The error data of the \ref
  QCPErrorBars are associated one-to-one via their index to the data points of
  the data plottable. You can directly access and manipulate the error bar data
  via \ref data.

  Set either of the plus/minus errors to NaN (<tt>qQNaN()</tt> or
  <tt>std::numeric_limits<double>::quiet_NaN()</tt>) to not show the respective
  error bar on the data point at that index.

  \section qcperrorbars-appearance Changing the appearance

  The appearance of the error bars is defined by the pen (\ref setPen), and the
  width of the whiskers (\ref setWhiskerWidth). Further, the error bar backbones
  may leave a gap around the data point center to prevent that error bars are
  drawn too close to or even through scatter points. This gap size can be
  controlled via \ref setSymbolGap.
*/

/* start of documentation of inline functions */

/*! \fn QSharedPointer<QCPErrorBarsDataContainer> QCPErrorBars::data() const

  Returns a shared pointer to the internal data storage of type \ref
  QCPErrorBarsDataContainer. You may use it to directly manipulate the error
  values, which may be more convenient and faster than using the regular \ref
  setData methods.
*/

/* end of documentation of inline functions */

/*!
  Constructs an error bars plottable which uses \a keyAxis as its key axis ("x")
  and \a valueAxis as its value axis ("y"). \a keyAxis and \a valueAxis must
  reside in the same QCustomPlot instance and not have the same orientation. If
  either of these restrictions is violated, a corresponding message is printed
  to the debug output (qDebug), the construction is not aborted, though.

  It is also important that the \a keyAxis and \a valueAxis are the same for the
  error bars plottable and the data plottable that the error bars shall be drawn
  on (\ref setDataPlottable).

  The created \ref QCPErrorBars is automatically registered with the QCustomPlot
  instance inferred from \a keyAxis. This QCustomPlot instance takes ownership
  of the \ref QCPErrorBars, so do not delete it manually but use \ref
  QCustomPlot::removePlottable() instead.
*/
QCPErrorBars::QCPErrorBars(QCPAxis *keyAxis, QCPAxis *valueAxis)
    : QCPAbstractPlottable(keyAxis, valueAxis),
      mDataContainer(new QVector<QCPErrorBarsData>),
      mErrorType(etValueError),
      mWhiskerWidth(9),
      mSymbolGap(10) {
  setPen(QPen(Qt::black, 0));
  setBrush(Qt::NoBrush);
}

QCPErrorBars::~QCPErrorBars() {}

/*! \overload

  Replaces the current data container with the provided \a data container.

  Since a QSharedPointer is used, multiple \ref QCPErrorBars instances may share
  the same data container safely. Modifying the data in the container will then
  affect all \ref QCPErrorBars instances that share the container. Sharing can
  be achieved by simply exchanging the data containers wrapped in shared
  pointers: \snippet documentation/doc-code-snippets/mainwindow.cpp
  qcperrorbars-datasharing-1

  If you do not wish to share containers, but create a copy from an existing
  container, assign the data containers directly: \snippet
  documentation/doc-code-snippets/mainwindow.cpp qcperrorbars-datasharing-2
  (This uses different notation compared with other plottables, because the \ref
  QCPErrorBars uses a \c QVector<QCPErrorBarsData> as its data container,
  instead of a \ref QCPDataContainer.)

  \see addData
*/
void QCPErrorBars::setData(QSharedPointer<QCPErrorBarsDataContainer> data) {
  mDataContainer = data;
}

/*! \overload

  Sets symmetrical error values as specified in \a error. The errors will be
  associated one-to-one by the data point index to the associated data plottable
  (\ref setDataPlottable).

  You can directly access and manipulate the error bar data via \ref data.

  \see addData
*/
void QCPErrorBars::setData(const QVector<double> &error) {
  mDataContainer->clear();
  addData(error);
}

/*! \overload

  Sets asymmetrical errors as specified in \a errorMinus and \a errorPlus. The
  errors will be associated one-to-one by the data point index to the associated
  data plottable (\ref setDataPlottable).

  You can directly access and manipulate the error bar data via \ref data.

  \see addData
*/
void QCPErrorBars::setData(const QVector<double> &errorMinus,
                           const QVector<double> &errorPlus) {
  mDataContainer->clear();
  addData(errorMinus, errorPlus);
}

/*!
  Sets the data plottable to which the error bars will be applied. The error
  values specified e.g. via \ref setData will be associated one-to-one by the
  data point index to the data points of \a plottable. This means that the error
  bars will adopt the key/value coordinates of the data point with the same
  index.

  The passed \a plottable must be a one-dimensional plottable, i.e. it must
  implement the \ref QCPPlottableInterface1D. Further, it must not be a \ref
  QCPErrorBars instance itself. If either of these restrictions is violated, a
  corresponding qDebug output is generated, and the data plottable of this \ref
  QCPErrorBars instance is set to zero.

  For proper display, care must also be taken that the key and value axes of the
  \a plottable match those configured for this \ref QCPErrorBars instance.
*/
void QCPErrorBars::setDataPlottable(QCPAbstractPlottable *plottable) {
  if (plottable && qobject_cast<QCPErrorBars *>(plottable)) {
    mDataPlottable = nullptr;
    qDebug() << Q_FUNC_INFO
             << "can't set another QCPErrorBars instance as data plottable";
    return;
  }
  if (plottable && !plottable->interface1D()) {
    mDataPlottable = nullptr;
    qDebug() << Q_FUNC_INFO
             << "passed plottable doesn't implement 1d interface, can't "
                "associate with QCPErrorBars";
    return;
  }

  mDataPlottable = plottable;
}

/*!
  Sets in which orientation the error bars shall appear on the data points. If
  your data needs both error dimensions, create two \ref QCPErrorBars with
  different \a type.
*/
void QCPErrorBars::setErrorType(ErrorType type) { mErrorType = type; }

/*!
  Sets the width of the whiskers (the short bars at the end of the actual error
  bar backbones) to \a pixels.
*/
void QCPErrorBars::setWhiskerWidth(double pixels) { mWhiskerWidth = pixels; }

/*!
  Sets the gap diameter around the data points that will be left out when
  drawing the error bar backbones. This gap prevents that error bars are drawn
  too close to or even through scatter points.
*/
void QCPErrorBars::setSymbolGap(double pixels) { mSymbolGap = pixels; }

/*! \overload

  Adds symmetrical error values as specified in \a error. The errors will be
  associated one-to-one by the data point index to the associated data plottable
  (\ref setDataPlottable).

  You can directly access and manipulate the error bar data via \ref data.

  \see setData
*/
void QCPErrorBars::addData(const QVector<double> &error) {
  addData(error, error);
}

/*! \overload

  Adds asymmetrical errors as specified in \a errorMinus and \a errorPlus. The
  errors will be associated one-to-one by the data point index to the associated
  data plottable (\ref setDataPlottable).

  You can directly access and manipulate the error bar data via \ref data.

  \see setData
*/
void QCPErrorBars::addData(const QVector<double> &errorMinus,
                           const QVector<double> &errorPlus) {
  if (errorMinus.size() != errorPlus.size())
    qDebug() << Q_FUNC_INFO
             << "minus and plus error vectors have different sizes:"
             << errorMinus.size() << errorPlus.size();
  const int n = qMin(errorMinus.size(), errorPlus.size());
  mDataContainer->reserve(n);
  for (int i = 0; i < n; ++i)
    mDataContainer->append(QCPErrorBarsData(errorMinus.at(i), errorPlus.at(i)));
}

/*! \overload

  Adds a single symmetrical error bar as specified in \a error. The errors will
  be associated one-to-one by the data point index to the associated data
  plottable (\ref setDataPlottable).

  You can directly access and manipulate the error bar data via \ref data.

  \see setData
*/
void QCPErrorBars::addData(double error) {
  mDataContainer->append(QCPErrorBarsData(error));
}

/*! \overload

  Adds a single asymmetrical error bar as specified in \a errorMinus and \a
  errorPlus. The errors will be associated one-to-one by the data point index to
  the associated data plottable (\ref setDataPlottable).

  You can directly access and manipulate the error bar data via \ref data.

  \see setData
*/
void QCPErrorBars::addData(double errorMinus, double errorPlus) {
  mDataContainer->append(QCPErrorBarsData(errorMinus, errorPlus));
}

/* inherits documentation from base class */
int QCPErrorBars::dataCount() const { return mDataContainer->size(); }

/* inherits documentation from base class */
double QCPErrorBars::dataMainKey(int index) const {
  if (mDataPlottable)
    return mDataPlottable->interface1D()->dataMainKey(index);
  else
    qDebug() << Q_FUNC_INFO << "no data plottable set";
  return 0;
}

/* inherits documentation from base class */
double QCPErrorBars::dataSortKey(int index) const {
  if (mDataPlottable)
    return mDataPlottable->interface1D()->dataSortKey(index);
  else
    qDebug() << Q_FUNC_INFO << "no data plottable set";
  return 0;
}

/* inherits documentation from base class */
double QCPErrorBars::dataMainValue(int index) const {
  if (mDataPlottable)
    return mDataPlottable->interface1D()->dataMainValue(index);
  else
    qDebug() << Q_FUNC_INFO << "no data plottable set";
  return 0;
}

/* inherits documentation from base class */
QCPRange QCPErrorBars::dataValueRange(int index) const {
  if (mDataPlottable) {
    const double value = mDataPlottable->interface1D()->dataMainValue(index);
    if (index >= 0 && index < mDataContainer->size() &&
        mErrorType == etValueError)
      return {value - mDataContainer->at(index).errorMinus,
              value + mDataContainer->at(index).errorPlus};
    else
      return {value, value};
  } else {
    qDebug() << Q_FUNC_INFO << "no data plottable set";
    return {};
  }
}

/* inherits documentation from base class */
QPointF QCPErrorBars::dataPixelPosition(int index) const {
  if (mDataPlottable)
    return mDataPlottable->interface1D()->dataPixelPosition(index);
  else
    qDebug() << Q_FUNC_INFO << "no data plottable set";
  return {};
}

/* inherits documentation from base class */
bool QCPErrorBars::sortKeyIsMainKey() const {
  if (mDataPlottable) {
    return mDataPlottable->interface1D()->sortKeyIsMainKey();
  } else {
    qDebug() << Q_FUNC_INFO << "no data plottable set";
    return true;
  }
}

/*!
  \copydoc QCPPlottableInterface1D::selectTestRect
*/
QCPDataSelection QCPErrorBars::selectTestRect(const QRectF &rect,
                                              bool onlySelectable) const {
  QCPDataSelection result;
  if (!mDataPlottable) return result;
  if ((onlySelectable && mSelectable == QCP::stNone) ||
      mDataContainer->isEmpty())
    return result;
  if (!mKeyAxis || !mValueAxis) return result;

  QCPErrorBarsDataContainer::const_iterator visibleBegin, visibleEnd;
  getVisibleDataBounds(visibleBegin, visibleEnd, QCPDataRange(0, dataCount()));

  QVector<QLineF> backbones, whiskers;
  for (QCPErrorBarsDataContainer::const_iterator it = visibleBegin;
       it != visibleEnd; ++it) {
    backbones.clear();
    whiskers.clear();
    getErrorBarLines(it, backbones, whiskers);
    foreach (const QLineF &backbone, backbones) {
      if (rectIntersectsLine(rect, backbone)) {
        result.addDataRange(
            QCPDataRange(int(it - mDataContainer->constBegin()),
                         int(it - mDataContainer->constBegin() + 1)),
            false);
        break;
      }
    }
  }
  result.simplify();
  return result;
}

/* inherits documentation from base class */
int QCPErrorBars::findBegin(double sortKey, bool expandedRange) const {
  if (mDataPlottable) {
    if (mDataContainer->isEmpty()) return 0;
    int beginIndex =
        mDataPlottable->interface1D()->findBegin(sortKey, expandedRange);
    if (beginIndex >= mDataContainer->size())
      beginIndex = mDataContainer->size() - 1;
    return beginIndex;
  } else
    qDebug() << Q_FUNC_INFO << "no data plottable set";
  return 0;
}

/* inherits documentation from base class */
int QCPErrorBars::findEnd(double sortKey, bool expandedRange) const {
  if (mDataPlottable) {
    if (mDataContainer->isEmpty()) return 0;
    int endIndex =
        mDataPlottable->interface1D()->findEnd(sortKey, expandedRange);
    if (endIndex > mDataContainer->size()) endIndex = mDataContainer->size();
    return endIndex;
  } else
    qDebug() << Q_FUNC_INFO << "no data plottable set";
  return 0;
}

/*!
  Implements a selectTest specific to this plottable's point geometry.

  If \a details is not 0, it will be set to a \ref QCPDataSelection, describing
  the closest data point to \a pos.

  \seebaseclassmethod \ref QCPAbstractPlottable::selectTest
*/
double QCPErrorBars::selectTest(const QPointF &pos, bool onlySelectable,
                                QVariant *details) const {
  if (!mDataPlottable) return -1;

  if ((onlySelectable && mSelectable == QCP::stNone) ||
      mDataContainer->isEmpty())
    return -1;
  if (!mKeyAxis || !mValueAxis) return -1;

  if (mKeyAxis.data()->axisRect()->rect().contains(pos.toPoint()) ||
      mParentPlot->interactions().testFlag(
          QCP::iSelectPlottablesBeyondAxisRect)) {
    QCPErrorBarsDataContainer::const_iterator closestDataPoint =
        mDataContainer->constEnd();
    double result = pointDistance(pos, closestDataPoint);
    if (details) {
      int pointIndex = int(closestDataPoint - mDataContainer->constBegin());
      details->setValue(
          QCPDataSelection(QCPDataRange(pointIndex, pointIndex + 1)));
    }
    return result;
  } else
    return -1;
}

/* inherits documentation from base class */
void QCPErrorBars::draw(QCPPainter *painter) {
  if (!mDataPlottable) return;
  if (!mKeyAxis || !mValueAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    return;
  }
  if (mKeyAxis.data()->range().size() <= 0 || mDataContainer->isEmpty()) return;

  // if the sort key isn't the main key, we must check the visibility for each
  // data point/error bar individually (getVisibleDataBounds applies range
  // restriction, but otherwise can only return full data range):
  bool checkPointVisibility =
      !mDataPlottable->interface1D()->sortKeyIsMainKey();

// check data validity if flag set:
#ifdef QCUSTOMPLOT_CHECK_DATA
  QCPErrorBarsDataContainer::const_iterator it;
  for (it = mDataContainer->constBegin(); it != mDataContainer->constEnd();
       ++it) {
    if (QCP::isInvalidData(it->errorMinus, it->errorPlus))
      qDebug() << Q_FUNC_INFO << "Data point at index"
               << it - mDataContainer->constBegin() << "invalid."
               << "Plottable name:" << name();
  }
#endif

  applyDefaultAntialiasingHint(painter);
  painter->setBrush(Qt::NoBrush);
  // loop over and draw segments of unselected/selected data:
  QList<QCPDataRange> selectedSegments, unselectedSegments, allSegments;
  getDataSegments(selectedSegments, unselectedSegments);
  allSegments << unselectedSegments << selectedSegments;
  QVector<QLineF> backbones, whiskers;
  for (int i = 0; i < allSegments.size(); ++i) {
    QCPErrorBarsDataContainer::const_iterator begin, end;
    getVisibleDataBounds(begin, end, allSegments.at(i));
    if (begin == end) continue;

    bool isSelectedSegment = i >= unselectedSegments.size();
    if (isSelectedSegment && mSelectionDecorator)
      mSelectionDecorator->applyPen(painter);
    else
      painter->setPen(mPen);
    if (painter->pen().capStyle() == Qt::SquareCap) {
      QPen capFixPen(painter->pen());
      capFixPen.setCapStyle(Qt::FlatCap);
      painter->setPen(capFixPen);
    }
    backbones.clear();
    whiskers.clear();
    for (QCPErrorBarsDataContainer::const_iterator it = begin; it != end;
         ++it) {
      if (!checkPointVisibility ||
          errorBarVisible(int(it - mDataContainer->constBegin())))
        getErrorBarLines(it, backbones, whiskers);
    }
    painter->drawLines(backbones);
    painter->drawLines(whiskers);
  }

  // draw other selection decoration that isn't just line/scatter pens and
  // brushes:
  if (mSelectionDecorator)
    mSelectionDecorator->drawDecoration(painter, selection());
}

/* inherits documentation from base class */
void QCPErrorBars::drawLegendIcon(QCPPainter *painter,
                                  const QRectF &rect) const {
  applyDefaultAntialiasingHint(painter);
  painter->setPen(mPen);
  if (mErrorType == etValueError && mValueAxis &&
      mValueAxis->orientation() == Qt::Vertical) {
    painter->drawLine(QLineF(rect.center().x(), rect.top() + 2,
                             rect.center().x(), rect.bottom() - 1));
    painter->drawLine(QLineF(rect.center().x() - 4, rect.top() + 2,
                             rect.center().x() + 4, rect.top() + 2));
    painter->drawLine(QLineF(rect.center().x() - 4, rect.bottom() - 1,
                             rect.center().x() + 4, rect.bottom() - 1));
  } else {
    painter->drawLine(QLineF(rect.left() + 2, rect.center().y(),
                             rect.right() - 2, rect.center().y()));
    painter->drawLine(QLineF(rect.left() + 2, rect.center().y() - 4,
                             rect.left() + 2, rect.center().y() + 4));
    painter->drawLine(QLineF(rect.right() - 2, rect.center().y() - 4,
                             rect.right() - 2, rect.center().y() + 4));
  }
}

/* inherits documentation from base class */
QCPRange QCPErrorBars::getKeyRange(bool &foundRange,
                                   QCP::SignDomain inSignDomain) const {
  if (!mDataPlottable) {
    foundRange = false;
    return {};
  }

  QCPRange range;
  bool haveLower = false;
  bool haveUpper = false;
  QCPErrorBarsDataContainer::const_iterator it;
  for (it = mDataContainer->constBegin(); it != mDataContainer->constEnd();
       ++it) {
    if (mErrorType == etValueError) {
      // error bar doesn't extend in key dimension (except whisker but we ignore
      // that here), so only use data point center
      const double current = mDataPlottable->interface1D()->dataMainKey(
          int(it - mDataContainer->constBegin()));
      if (qIsNaN(current)) continue;
      if (inSignDomain == QCP::sdBoth ||
          (inSignDomain == QCP::sdNegative && current < 0) ||
          (inSignDomain == QCP::sdPositive && current > 0)) {
        if (current < range.lower || !haveLower) {
          range.lower = current;
          haveLower = true;
        }
        if (current > range.upper || !haveUpper) {
          range.upper = current;
          haveUpper = true;
        }
      }
    } else  // mErrorType == etKeyError
    {
      const double dataKey = mDataPlottable->interface1D()->dataMainKey(
          int(it - mDataContainer->constBegin()));
      if (qIsNaN(dataKey)) continue;
      // plus error:
      double current = dataKey + (qIsNaN(it->errorPlus) ? 0 : it->errorPlus);
      if (inSignDomain == QCP::sdBoth ||
          (inSignDomain == QCP::sdNegative && current < 0) ||
          (inSignDomain == QCP::sdPositive && current > 0)) {
        if (current > range.upper || !haveUpper) {
          range.upper = current;
          haveUpper = true;
        }
      }
      // minus error:
      current = dataKey - (qIsNaN(it->errorMinus) ? 0 : it->errorMinus);
      if (inSignDomain == QCP::sdBoth ||
          (inSignDomain == QCP::sdNegative && current < 0) ||
          (inSignDomain == QCP::sdPositive && current > 0)) {
        if (current < range.lower || !haveLower) {
          range.lower = current;
          haveLower = true;
        }
      }
    }
  }

  if (haveUpper && !haveLower) {
    range.lower = range.upper;
    haveLower = true;
  } else if (haveLower && !haveUpper) {
    range.upper = range.lower;
    haveUpper = true;
  }

  foundRange = haveLower && haveUpper;
  return range;
}

/* inherits documentation from base class */
QCPRange QCPErrorBars::getValueRange(bool &foundRange,
                                     QCP::SignDomain inSignDomain,
                                     const QCPRange &inKeyRange) const {
  if (!mDataPlottable) {
    foundRange = false;
    return {};
  }

  QCPRange range;
  const bool restrictKeyRange = inKeyRange != QCPRange();
  bool haveLower = false;
  bool haveUpper = false;
  QCPErrorBarsDataContainer::const_iterator itBegin =
      mDataContainer->constBegin();
  QCPErrorBarsDataContainer::const_iterator itEnd = mDataContainer->constEnd();
  if (mDataPlottable->interface1D()->sortKeyIsMainKey() && restrictKeyRange) {
    itBegin = mDataContainer->constBegin() + findBegin(inKeyRange.lower, false);
    itEnd = mDataContainer->constBegin() + findEnd(inKeyRange.upper, false);
  }
  for (QCPErrorBarsDataContainer::const_iterator it = itBegin; it != itEnd;
       ++it) {
    if (restrictKeyRange) {
      const double dataKey = mDataPlottable->interface1D()->dataMainKey(
          int(it - mDataContainer->constBegin()));
      if (dataKey < inKeyRange.lower || dataKey > inKeyRange.upper) continue;
    }
    if (mErrorType == etValueError) {
      const double dataValue = mDataPlottable->interface1D()->dataMainValue(
          int(it - mDataContainer->constBegin()));
      if (qIsNaN(dataValue)) continue;
      // plus error:
      double current = dataValue + (qIsNaN(it->errorPlus) ? 0 : it->errorPlus);
      if (inSignDomain == QCP::sdBoth ||
          (inSignDomain == QCP::sdNegative && current < 0) ||
          (inSignDomain == QCP::sdPositive && current > 0)) {
        if (current > range.upper || !haveUpper) {
          range.upper = current;
          haveUpper = true;
        }
      }
      // minus error:
      current = dataValue - (qIsNaN(it->errorMinus) ? 0 : it->errorMinus);
      if (inSignDomain == QCP::sdBoth ||
          (inSignDomain == QCP::sdNegative && current < 0) ||
          (inSignDomain == QCP::sdPositive && current > 0)) {
        if (current < range.lower || !haveLower) {
          range.lower = current;
          haveLower = true;
        }
      }
    } else  // mErrorType == etKeyError
    {
      // error bar doesn't extend in value dimension (except whisker but we
      // ignore that here), so only use data point center
      const double current = mDataPlottable->interface1D()->dataMainValue(
          int(it - mDataContainer->constBegin()));
      if (qIsNaN(current)) continue;
      if (inSignDomain == QCP::sdBoth ||
          (inSignDomain == QCP::sdNegative && current < 0) ||
          (inSignDomain == QCP::sdPositive && current > 0)) {
        if (current < range.lower || !haveLower) {
          range.lower = current;
          haveLower = true;
        }
        if (current > range.upper || !haveUpper) {
          range.upper = current;
          haveUpper = true;
        }
      }
    }
  }

  if (haveUpper && !haveLower) {
    range.lower = range.upper;
    haveLower = true;
  } else if (haveLower && !haveUpper) {
    range.upper = range.lower;
    haveUpper = true;
  }

  foundRange = haveLower && haveUpper;
  return range;
}

/*! \internal

  Calculates the lines that make up the error bar belonging to the data point \a
  it.

  The resulting lines are added to \a backbones and \a whiskers. The vectors are
  not cleared, so calling this method with different \a it but the same \a
  backbones and \a whiskers allows to accumulate lines for multiple data points.

  This method assumes that \a it is a valid iterator within the bounds of this
  \ref QCPErrorBars instance and within the bounds of the associated data
  plottable.
*/
void QCPErrorBars::getErrorBarLines(
    QCPErrorBarsDataContainer::const_iterator it, QVector<QLineF> &backbones,
    QVector<QLineF> &whiskers) const {
  if (!mDataPlottable) return;

  int index = int(it - mDataContainer->constBegin());
  QPointF centerPixel = mDataPlottable->interface1D()->dataPixelPosition(index);
  if (qIsNaN(centerPixel.x()) || qIsNaN(centerPixel.y())) return;
  QCPAxis *errorAxis =
      mErrorType == etValueError ? mValueAxis.data() : mKeyAxis.data();
  QCPAxis *orthoAxis =
      mErrorType == etValueError ? mKeyAxis.data() : mValueAxis.data();
  const double centerErrorAxisPixel = errorAxis->orientation() == Qt::Horizontal
                                          ? centerPixel.x()
                                          : centerPixel.y();
  const double centerOrthoAxisPixel = orthoAxis->orientation() == Qt::Horizontal
                                          ? centerPixel.x()
                                          : centerPixel.y();
  const double centerErrorAxisCoord = errorAxis->pixelToCoord(
      centerErrorAxisPixel);  // depending on plottable, this might be different
                              // from just
  // mDataPlottable->interface1D()->dataMainKey/Value
  const double symbolGap = mSymbolGap * 0.5 * errorAxis->pixelOrientation();
  // plus error:
  double errorStart, errorEnd;
  if (!qIsNaN(it->errorPlus)) {
    errorStart = centerErrorAxisPixel + symbolGap;
    errorEnd = errorAxis->coordToPixel(centerErrorAxisCoord + it->errorPlus);
    if (errorAxis->orientation() == Qt::Vertical) {
      if ((errorStart > errorEnd) != errorAxis->rangeReversed())
        backbones.append(QLineF(centerOrthoAxisPixel, errorStart,
                                centerOrthoAxisPixel, errorEnd));
      whiskers.append(
          QLineF(centerOrthoAxisPixel - mWhiskerWidth * 0.5, errorEnd,
                 centerOrthoAxisPixel + mWhiskerWidth * 0.5, errorEnd));
    } else {
      if ((errorStart < errorEnd) != errorAxis->rangeReversed())
        backbones.append(QLineF(errorStart, centerOrthoAxisPixel, errorEnd,
                                centerOrthoAxisPixel));
      whiskers.append(
          QLineF(errorEnd, centerOrthoAxisPixel - mWhiskerWidth * 0.5, errorEnd,
                 centerOrthoAxisPixel + mWhiskerWidth * 0.5));
    }
  }
  // minus error:
  if (!qIsNaN(it->errorMinus)) {
    errorStart = centerErrorAxisPixel - symbolGap;
    errorEnd = errorAxis->coordToPixel(centerErrorAxisCoord - it->errorMinus);
    if (errorAxis->orientation() == Qt::Vertical) {
      if ((errorStart < errorEnd) != errorAxis->rangeReversed())
        backbones.append(QLineF(centerOrthoAxisPixel, errorStart,
                                centerOrthoAxisPixel, errorEnd));
      whiskers.append(
          QLineF(centerOrthoAxisPixel - mWhiskerWidth * 0.5, errorEnd,
                 centerOrthoAxisPixel + mWhiskerWidth * 0.5, errorEnd));
    } else {
      if ((errorStart > errorEnd) != errorAxis->rangeReversed())
        backbones.append(QLineF(errorStart, centerOrthoAxisPixel, errorEnd,
                                centerOrthoAxisPixel));
      whiskers.append(
          QLineF(errorEnd, centerOrthoAxisPixel - mWhiskerWidth * 0.5, errorEnd,
                 centerOrthoAxisPixel + mWhiskerWidth * 0.5));
    }
  }
}

/*! \internal

  This method outputs the currently visible data range via \a begin and \a end.
  The returned range will also never exceed \a rangeRestriction.

  Since error bars with type \ref etKeyError may extend to arbitrarily positive
  and negative key coordinates relative to their data point key, this method
  checks all outer error bars whether they truly don't reach into the visible
  portion of the axis rect, by calling \ref errorBarVisible. On the other hand
  error bars with type \ref etValueError that are associated with data
  plottables whose sort key is equal to the main key (see \ref
  qcpdatacontainer-datatype "QCPDataContainer DataType") can be handled very
  efficiently by finding the visible range of error bars through binary search
  (\ref QCPPlottableInterface1D::findBegin and \ref
  QCPPlottableInterface1D::findEnd).

  If the plottable's sort key is not equal to the main key, this method returns
  the full data range, only restricted by \a rangeRestriction. Drawing
  optimization then has to be done on a point-by-point basis in the \ref draw
  method.
*/
void QCPErrorBars::getVisibleDataBounds(
    QCPErrorBarsDataContainer::const_iterator &begin,
    QCPErrorBarsDataContainer::const_iterator &end,
    const QCPDataRange &rangeRestriction) const {
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    end = mDataContainer->constEnd();
    begin = end;
    return;
  }
  if (!mDataPlottable || rangeRestriction.isEmpty()) {
    end = mDataContainer->constEnd();
    begin = end;
    return;
  }
  if (!mDataPlottable->interface1D()->sortKeyIsMainKey()) {
    // if the sort key isn't the main key, it's not possible to find a
    // contiguous range of visible data points, so this method then only applies
    // the range restriction and otherwise returns the full data range.
    // Visibility checks must be done on a per-datapoin-basis during drawing
    QCPDataRange dataRange(0, mDataContainer->size());
    dataRange = dataRange.bounded(rangeRestriction);
    begin = mDataContainer->constBegin() + dataRange.begin();
    end = mDataContainer->constBegin() + dataRange.end();
    return;
  }

  // get visible data range via interface from data plottable, and then restrict
  // to available error data points:
  const int n =
      qMin(mDataContainer->size(), mDataPlottable->interface1D()->dataCount());
  int beginIndex =
      mDataPlottable->interface1D()->findBegin(keyAxis->range().lower);
  int endIndex = mDataPlottable->interface1D()->findEnd(keyAxis->range().upper);
  int i = beginIndex;
  while (i > 0 && i < n && i > rangeRestriction.begin()) {
    if (errorBarVisible(i)) beginIndex = i;
    --i;
  }
  i = endIndex;
  while (i >= 0 && i < n && i < rangeRestriction.end()) {
    if (errorBarVisible(i)) endIndex = i + 1;
    ++i;
  }
  QCPDataRange dataRange(beginIndex, endIndex);
  dataRange = dataRange.bounded(
      rangeRestriction.bounded(QCPDataRange(0, mDataContainer->size())));
  begin = mDataContainer->constBegin() + dataRange.begin();
  end = mDataContainer->constBegin() + dataRange.end();
}

/*! \internal

  Calculates the minimum distance in pixels the error bars' representation has
  from the given \a pixelPoint. This is used to determine whether the error bar
  was clicked or not, e.g. in \ref selectTest. The closest data point to \a
  pixelPoint is returned in \a closestData.
*/
double QCPErrorBars::pointDistance(
    const QPointF &pixelPoint,
    QCPErrorBarsDataContainer::const_iterator &closestData) const {
  closestData = mDataContainer->constEnd();
  if (!mDataPlottable || mDataContainer->isEmpty()) return -1.0;
  if (!mKeyAxis || !mValueAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    return -1.0;
  }

  QCPErrorBarsDataContainer::const_iterator begin, end;
  getVisibleDataBounds(begin, end, QCPDataRange(0, dataCount()));

  // calculate minimum distances to error backbones (whiskers are ignored for
  // speed) and find closestData iterator:
  double minDistSqr = (std::numeric_limits<double>::max)();
  QVector<QLineF> backbones, whiskers;
  for (QCPErrorBarsDataContainer::const_iterator it = begin; it != end; ++it) {
    getErrorBarLines(it, backbones, whiskers);
    foreach (const QLineF &backbone, backbones) {
      const double currentDistSqr =
          QCPVector2D(pixelPoint).distanceSquaredToLine(backbone);
      if (currentDistSqr < minDistSqr) {
        minDistSqr = currentDistSqr;
        closestData = it;
      }
    }
  }
  return qSqrt(minDistSqr);
}

/*! \internal

  \note This method is identical to \ref QCPAbstractPlottable1D::getDataSegments
  but needs to be reproduced here since the \ref QCPErrorBars plottable, as a
  special case that doesn't have its own key/value data coordinates, doesn't
  derive from \ref QCPAbstractPlottable1D. See the documentation there for
  details.
*/
void QCPErrorBars::getDataSegments(
    QList<QCPDataRange> &selectedSegments,
    QList<QCPDataRange> &unselectedSegments) const {
  selectedSegments.clear();
  unselectedSegments.clear();
  if (mSelectable ==
      QCP::stWhole)  // stWhole selection type draws the entire plottable with
  // selected style if mSelection isn't empty
  {
    if (selected())
      selectedSegments << QCPDataRange(0, dataCount());
    else
      unselectedSegments << QCPDataRange(0, dataCount());
  } else {
    QCPDataSelection sel(selection());
    sel.simplify();
    selectedSegments = sel.dataRanges();
    unselectedSegments = sel.inverse(QCPDataRange(0, dataCount())).dataRanges();
  }
}

/*! \internal

  Returns whether the error bar at the specified \a index is visible within the
  current key axis range.

  This method assumes for performance reasons without checking that the key
  axis, the value axis, and the data plottable (\ref setDataPlottable) are not
  \c nullptr and that \a index is within valid bounds of this \ref QCPErrorBars
  instance and the bounds of the data plottable.
*/
bool QCPErrorBars::errorBarVisible(int index) const {
  QPointF centerPixel = mDataPlottable->interface1D()->dataPixelPosition(index);
  const double centerKeyPixel = mKeyAxis->orientation() == Qt::Horizontal
                                    ? centerPixel.x()
                                    : centerPixel.y();
  if (qIsNaN(centerKeyPixel)) return false;

  double keyMin, keyMax;
  if (mErrorType == etKeyError) {
    const double centerKey = mKeyAxis->pixelToCoord(centerKeyPixel);
    const double errorPlus = mDataContainer->at(index).errorPlus;
    const double errorMinus = mDataContainer->at(index).errorMinus;
    keyMax = centerKey + (qIsNaN(errorPlus) ? 0 : errorPlus);
    keyMin = centerKey - (qIsNaN(errorMinus) ? 0 : errorMinus);
  } else  // mErrorType == etValueError
  {
    keyMax = mKeyAxis->pixelToCoord(
        centerKeyPixel + mWhiskerWidth * 0.5 * mKeyAxis->pixelOrientation());
    keyMin = mKeyAxis->pixelToCoord(
        centerKeyPixel - mWhiskerWidth * 0.5 * mKeyAxis->pixelOrientation());
  }
  return ((keyMax > mKeyAxis->range().lower) &&
          (keyMin < mKeyAxis->range().upper));
}

/*! \internal

  Returns whether \a line intersects (or is contained in) \a pixelRect.

  \a line is assumed to be either perfectly horizontal or perfectly vertical, as
  is the case for error bar lines.
*/
bool QCPErrorBars::rectIntersectsLine(const QRectF &pixelRect,
                                      const QLineF &line) const {
  if (pixelRect.left() > line.x1() && pixelRect.left() > line.x2())
    return false;
  else if (pixelRect.right() < line.x1() && pixelRect.right() < line.x2())
    return false;
  else if (pixelRect.top() > line.y1() && pixelRect.top() > line.y2())
    return false;
  else if (pixelRect.bottom() < line.y1() && pixelRect.bottom() < line.y2())
    return false;
  else
    return true;
}
/* end of 'src/plottables/plottable-errorbar.cpp' */

/* including file 'src/items/item-straightline.cpp' */
/* modified 2022-11-06T12:45:56, size 7596          */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPItemStraightLine
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPItemStraightLine
  \brief A straight line that spans infinitely in both directions

  \image html QCPItemStraightLine.png "Straight line example. Blue dotted
  circles are anchors, solid blue discs are positions."

  It has two positions, \a point1 and \a point2, which define the straight line.
*/

/*!
  Creates a straight line item and sets default values.

  The created item is automatically registered with \a parentPlot. This
  QCustomPlot instance takes ownership of the item, so do not delete it manually
  but use QCustomPlot::removeItem() instead.
*/
QCPItemStraightLine::QCPItemStraightLine(QCustomPlot *parentPlot)
    : QCPAbstractItem(parentPlot),
      point1(createPosition(QLatin1String("point1"))),
      point2(createPosition(QLatin1String("point2"))) {
  point1->setCoords(0, 0);
  point2->setCoords(1, 1);

  setPen(QPen(Qt::black));
  setSelectedPen(QPen(Qt::blue, 2));
}

QCPItemStraightLine::~QCPItemStraightLine() {}

/*!
  Sets the pen that will be used to draw the line

  \see setSelectedPen
*/
void QCPItemStraightLine::setPen(const QPen &pen) { mPen = pen; }

/*!
  Sets the pen that will be used to draw the line when selected

  \see setPen, setSelected
*/
void QCPItemStraightLine::setSelectedPen(const QPen &pen) {
  mSelectedPen = pen;
}

/* inherits documentation from base class */
double QCPItemStraightLine::selectTest(const QPointF &pos, bool onlySelectable,
                                       QVariant *details) const {
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable) return -1;

  return QCPVector2D(pos).distanceToStraightLine(
      point1->pixelPosition(),
      point2->pixelPosition() - point1->pixelPosition());
}

/* inherits documentation from base class */
void QCPItemStraightLine::draw(QCPPainter *painter) {
  QCPVector2D start(point1->pixelPosition());
  QCPVector2D end(point2->pixelPosition());
  // get visible segment of straight line inside clipRect:
  int clipPad = qCeil(mainPen().widthF());
  QLineF line = getRectClippedStraightLine(
      start, end - start,
      clipRect().adjusted(-clipPad, -clipPad, clipPad, clipPad));
  // paint visible segment, if existent:
  if (!line.isNull()) {
    painter->setPen(mainPen());
    painter->drawLine(line);
  }
}

/*! \internal

  Returns the section of the straight line defined by \a base and direction
  vector \a vec, that is visible in the specified \a rect.

  This is a helper function for \ref draw.
*/
QLineF QCPItemStraightLine::getRectClippedStraightLine(
    const QCPVector2D &base, const QCPVector2D &vec, const QRect &rect) const {
  double bx, by;
  double gamma;
  QLineF result;
  if (vec.x() == 0 && vec.y() == 0) return result;
  if (qFuzzyIsNull(vec.x()))  // line is vertical
  {
    // check top of rect:
    bx = rect.left();
    by = rect.top();
    gamma = base.x() - bx + (by - base.y()) * vec.x() / vec.y();
    if (gamma >= 0 && gamma <= rect.width())
      result.setLine(bx + gamma, rect.top(), bx + gamma,
                     rect.bottom());  // no need to check bottom because we know
                                      // line is vertical
  } else if (qFuzzyIsNull(vec.y()))   // line is horizontal
  {
    // check left of rect:
    bx = rect.left();
    by = rect.top();
    gamma = base.y() - by + (bx - base.x()) * vec.y() / vec.x();
    if (gamma >= 0 && gamma <= rect.height())
      result.setLine(rect.left(), by + gamma, rect.right(),
                     by + gamma);  // no need to check right because we know
                                   // line is horizontal
  } else                           // line is skewed
  {
    QList<QCPVector2D> pointVectors;
    // check top of rect:
    bx = rect.left();
    by = rect.top();
    gamma = base.x() - bx + (by - base.y()) * vec.x() / vec.y();
    if (gamma >= 0 && gamma <= rect.width())
      pointVectors.append(QCPVector2D(bx + gamma, by));
    // check bottom of rect:
    bx = rect.left();
    by = rect.bottom();
    gamma = base.x() - bx + (by - base.y()) * vec.x() / vec.y();
    if (gamma >= 0 && gamma <= rect.width())
      pointVectors.append(QCPVector2D(bx + gamma, by));
    // check left of rect:
    bx = rect.left();
    by = rect.top();
    gamma = base.y() - by + (bx - base.x()) * vec.y() / vec.x();
    if (gamma >= 0 && gamma <= rect.height())
      pointVectors.append(QCPVector2D(bx, by + gamma));
    // check right of rect:
    bx = rect.right();
    by = rect.top();
    gamma = base.y() - by + (bx - base.x()) * vec.y() / vec.x();
    if (gamma >= 0 && gamma <= rect.height())
      pointVectors.append(QCPVector2D(bx, by + gamma));

    // evaluate points:
    if (pointVectors.size() == 2) {
      result.setPoints(pointVectors.at(0).toPointF(),
                       pointVectors.at(1).toPointF());
    } else if (pointVectors.size() > 2) {
      // line probably goes through corner of rect, and we got two points there.
      // single out the point pair with greatest distance:
      double distSqrMax = 0;
      QCPVector2D pv1, pv2;
      for (int i = 0; i < pointVectors.size() - 1; ++i) {
        for (int k = i + 1; k < pointVectors.size(); ++k) {
          double distSqr =
              (pointVectors.at(i) - pointVectors.at(k)).lengthSquared();
          if (distSqr > distSqrMax) {
            pv1 = pointVectors.at(i);
            pv2 = pointVectors.at(k);
            distSqrMax = distSqr;
          }
        }
      }
      result.setPoints(pv1.toPointF(), pv2.toPointF());
    }
  }
  return result;
}

/*! \internal

  Returns the pen that should be used for drawing lines. Returns mPen when the
  item is not selected and mSelectedPen when it is.
*/
QPen QCPItemStraightLine::mainPen() const {
  return mSelected ? mSelectedPen : mPen;
}
/* end of 'src/items/item-straightline.cpp' */

/* including file 'src/items/item-line.cpp' */
/* modified 2022-11-06T12:45:56, size 8525  */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPItemLine
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPItemLine
  \brief A line from one point to another

  \image html QCPItemLine.png "Line example. Blue dotted circles are anchors,
  solid blue discs are positions."

  It has two positions, \a start and \a end, which define the end points of the
  line.

  With \ref setHead and \ref setTail you may set different line ending styles,
  e.g. to create an arrow.
*/

/*!
  Creates a line item and sets default values.

  The created item is automatically registered with \a parentPlot. This
  QCustomPlot instance takes ownership of the item, so do not delete it manually
  but use QCustomPlot::removeItem() instead.
*/
QCPItemLine::QCPItemLine(QCustomPlot *parentPlot)
    : QCPAbstractItem(parentPlot),
      start(createPosition(QLatin1String("start"))),
      end(createPosition(QLatin1String("end"))) {
  start->setCoords(0, 0);
  end->setCoords(1, 1);

  setPen(QPen(Qt::black));
  setSelectedPen(QPen(Qt::blue, 2));
}

QCPItemLine::~QCPItemLine() {}

/*!
  Sets the pen that will be used to draw the line

  \see setSelectedPen
*/
void QCPItemLine::setPen(const QPen &pen) { mPen = pen; }

/*!
  Sets the pen that will be used to draw the line when selected

  \see setPen, setSelected
*/
void QCPItemLine::setSelectedPen(const QPen &pen) { mSelectedPen = pen; }

/*!
  Sets the line ending style of the head. The head corresponds to the \a end
  position.

  Note that due to the overloaded QCPLineEnding constructor, you may directly
  specify a QCPLineEnding::EndingStyle here, e.g. \code
  setHead(QCPLineEnding::esSpikeArrow) \endcode

  \see setTail
*/
void QCPItemLine::setHead(const QCPLineEnding &head) { mHead = head; }

/*!
  Sets the line ending style of the tail. The tail corresponds to the \a start
  position.

  Note that due to the overloaded QCPLineEnding constructor, you may directly
  specify a QCPLineEnding::EndingStyle here, e.g. \code
  setTail(QCPLineEnding::esSpikeArrow) \endcode

  \see setHead
*/
void QCPItemLine::setTail(const QCPLineEnding &tail) { mTail = tail; }

/* inherits documentation from base class */
double QCPItemLine::selectTest(const QPointF &pos, bool onlySelectable,
                               QVariant *details) const {
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable) return -1;

  return qSqrt(QCPVector2D(pos).distanceSquaredToLine(start->pixelPosition(),
                                                      end->pixelPosition()));
}

/* inherits documentation from base class */
void QCPItemLine::draw(QCPPainter *painter) {
  QCPVector2D startVec(start->pixelPosition());
  QCPVector2D endVec(end->pixelPosition());
  if (qFuzzyIsNull((startVec - endVec).lengthSquared())) return;
  // get visible segment of straight line inside clipRect:
  int clipPad = int(qMax(mHead.boundingDistance(), mTail.boundingDistance()));
  clipPad = qMax(clipPad, qCeil(mainPen().widthF()));
  QLineF line = getRectClippedLine(
      startVec, endVec,
      clipRect().adjusted(-clipPad, -clipPad, clipPad, clipPad));
  // paint visible segment, if existent:
  if (!line.isNull()) {
    painter->setPen(mainPen());
    painter->drawLine(line);
    painter->setBrush(Qt::SolidPattern);
    if (mTail.style() != QCPLineEnding::esNone)
      mTail.draw(painter, startVec, startVec - endVec);
    if (mHead.style() != QCPLineEnding::esNone)
      mHead.draw(painter, endVec, endVec - startVec);
  }
}

/*! \internal

  Returns the section of the line defined by \a start and \a end, that is
  visible in the specified \a rect.

  This is a helper function for \ref draw.
*/
QLineF QCPItemLine::getRectClippedLine(const QCPVector2D &start,
                                       const QCPVector2D &end,
                                       const QRect &rect) const {
  bool containsStart = rect.contains(qRound(start.x()), qRound(start.y()));
  bool containsEnd = rect.contains(qRound(end.x()), qRound(end.y()));
  if (containsStart && containsEnd) return {start.toPointF(), end.toPointF()};

  QCPVector2D base = start;
  QCPVector2D vec = end - start;
  double bx, by;
  double gamma, mu;
  QLineF result;
  QList<QCPVector2D> pointVectors;

  if (!qFuzzyIsNull(vec.y()))  // line is not horizontal
  {
    // check top of rect:
    bx = rect.left();
    by = rect.top();
    mu = (by - base.y()) / vec.y();
    if (mu >= 0 && mu <= 1) {
      gamma = base.x() - bx + mu * vec.x();
      if (gamma >= 0 && gamma <= rect.width())
        pointVectors.append(QCPVector2D(bx + gamma, by));
    }
    // check bottom of rect:
    bx = rect.left();
    by = rect.bottom();
    mu = (by - base.y()) / vec.y();
    if (mu >= 0 && mu <= 1) {
      gamma = base.x() - bx + mu * vec.x();
      if (gamma >= 0 && gamma <= rect.width())
        pointVectors.append(QCPVector2D(bx + gamma, by));
    }
  }
  if (!qFuzzyIsNull(vec.x()))  // line is not vertical
  {
    // check left of rect:
    bx = rect.left();
    by = rect.top();
    mu = (bx - base.x()) / vec.x();
    if (mu >= 0 && mu <= 1) {
      gamma = base.y() - by + mu * vec.y();
      if (gamma >= 0 && gamma <= rect.height())
        pointVectors.append(QCPVector2D(bx, by + gamma));
    }
    // check right of rect:
    bx = rect.right();
    by = rect.top();
    mu = (bx - base.x()) / vec.x();
    if (mu >= 0 && mu <= 1) {
      gamma = base.y() - by + mu * vec.y();
      if (gamma >= 0 && gamma <= rect.height())
        pointVectors.append(QCPVector2D(bx, by + gamma));
    }
  }

  if (containsStart) pointVectors.append(start);
  if (containsEnd) pointVectors.append(end);

  // evaluate points:
  if (pointVectors.size() == 2) {
    result.setPoints(pointVectors.at(0).toPointF(),
                     pointVectors.at(1).toPointF());
  } else if (pointVectors.size() > 2) {
    // line probably goes through corner of rect, and we got two points there.
    // single out the point pair with greatest distance:
    double distSqrMax = 0;
    QCPVector2D pv1, pv2;
    for (int i = 0; i < pointVectors.size() - 1; ++i) {
      for (int k = i + 1; k < pointVectors.size(); ++k) {
        double distSqr =
            (pointVectors.at(i) - pointVectors.at(k)).lengthSquared();
        if (distSqr > distSqrMax) {
          pv1 = pointVectors.at(i);
          pv2 = pointVectors.at(k);
          distSqrMax = distSqr;
        }
      }
    }
    result.setPoints(pv1.toPointF(), pv2.toPointF());
  }
  return result;
}

/*! \internal

  Returns the pen that should be used for drawing lines. Returns mPen when the
  item is not selected and mSelectedPen when it is.
*/
QPen QCPItemLine::mainPen() const { return mSelected ? mSelectedPen : mPen; }
/* end of 'src/items/item-line.cpp' */

/* including file 'src/items/item-curve.cpp' */
/* modified 2022-11-06T12:45:56, size 7273   */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPItemCurve
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPItemCurve
  \brief A curved line from one point to another

  \image html QCPItemCurve.png "Curve example. Blue dotted circles are anchors,
  solid blue discs are positions."

  It has four positions, \a start and \a end, which define the end points of the
  line, and two control points which define the direction the line exits from
  the start and the direction from which it approaches the end: \a startDir and
  \a endDir.

  With \ref setHead and \ref setTail you may set different line ending styles,
  e.g. to create an arrow.

  Often it is desirable for the control points to stay at fixed relative
  positions to the start/end point. This can be achieved by setting the parent
  anchor e.g. of \a startDir simply to \a start, and then specify the desired
  pixel offset with QCPItemPosition::setCoords on \a startDir.
*/

/*!
  Creates a curve item and sets default values.

  The created item is automatically registered with \a parentPlot. This
  QCustomPlot instance takes ownership of the item, so do not delete it manually
  but use QCustomPlot::removeItem() instead.
*/
QCPItemCurve::QCPItemCurve(QCustomPlot *parentPlot)
    : QCPAbstractItem(parentPlot),
      start(createPosition(QLatin1String("start"))),
      startDir(createPosition(QLatin1String("startDir"))),
      endDir(createPosition(QLatin1String("endDir"))),
      end(createPosition(QLatin1String("end"))) {
  start->setCoords(0, 0);
  startDir->setCoords(0.5, 0);
  endDir->setCoords(0, 0.5);
  end->setCoords(1, 1);

  setPen(QPen(Qt::black));
  setSelectedPen(QPen(Qt::blue, 2));
}

QCPItemCurve::~QCPItemCurve() {}

/*!
  Sets the pen that will be used to draw the line

  \see setSelectedPen
*/
void QCPItemCurve::setPen(const QPen &pen) { mPen = pen; }

/*!
  Sets the pen that will be used to draw the line when selected

  \see setPen, setSelected
*/
void QCPItemCurve::setSelectedPen(const QPen &pen) { mSelectedPen = pen; }

/*!
  Sets the line ending style of the head. The head corresponds to the \a end
  position.

  Note that due to the overloaded QCPLineEnding constructor, you may directly
  specify a QCPLineEnding::EndingStyle here, e.g. \code
  setHead(QCPLineEnding::esSpikeArrow) \endcode

  \see setTail
*/
void QCPItemCurve::setHead(const QCPLineEnding &head) { mHead = head; }

/*!
  Sets the line ending style of the tail. The tail corresponds to the \a start
  position.

  Note that due to the overloaded QCPLineEnding constructor, you may directly
  specify a QCPLineEnding::EndingStyle here, e.g. \code
  setTail(QCPLineEnding::esSpikeArrow) \endcode

  \see setHead
*/
void QCPItemCurve::setTail(const QCPLineEnding &tail) { mTail = tail; }

/* inherits documentation from base class */
double QCPItemCurve::selectTest(const QPointF &pos, bool onlySelectable,
                                QVariant *details) const {
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable) return -1;

  QPointF startVec(start->pixelPosition());
  QPointF startDirVec(startDir->pixelPosition());
  QPointF endDirVec(endDir->pixelPosition());
  QPointF endVec(end->pixelPosition());

  QPainterPath cubicPath(startVec);
  cubicPath.cubicTo(startDirVec, endDirVec, endVec);

  QList<QPolygonF> polygons = cubicPath.toSubpathPolygons();
  if (polygons.isEmpty()) return -1;
  const QPolygonF polygon = polygons.first();
  QCPVector2D p(pos);
  double minDistSqr = (std::numeric_limits<double>::max)();
  for (int i = 1; i < polygon.size(); ++i) {
    double distSqr = p.distanceSquaredToLine(polygon.at(i - 1), polygon.at(i));
    if (distSqr < minDistSqr) minDistSqr = distSqr;
  }
  return qSqrt(minDistSqr);
}

/* inherits documentation from base class */
void QCPItemCurve::draw(QCPPainter *painter) {
  QCPVector2D startVec(start->pixelPosition());
  QCPVector2D startDirVec(startDir->pixelPosition());
  QCPVector2D endDirVec(endDir->pixelPosition());
  QCPVector2D endVec(end->pixelPosition());
  if ((endVec - startVec).length() > 1e10)  // too large curves cause crash
    return;

  QPainterPath cubicPath(startVec.toPointF());
  cubicPath.cubicTo(startDirVec.toPointF(), endDirVec.toPointF(),
                    endVec.toPointF());

  // paint visible segment, if existent:
  const int clipEnlarge = qCeil(mainPen().widthF());
  QRect clip =
      clipRect().adjusted(-clipEnlarge, -clipEnlarge, clipEnlarge, clipEnlarge);
  QRect cubicRect = cubicPath.controlPointRect().toRect();
  if (cubicRect.isEmpty())  // may happen when start and end exactly on same x
    // or y position
    cubicRect.adjust(0, 0, 1, 1);
  if (clip.intersects(cubicRect)) {
    painter->setPen(mainPen());
    painter->drawPath(cubicPath);
    painter->setBrush(Qt::SolidPattern);
    if (mTail.style() != QCPLineEnding::esNone)
      mTail.draw(painter, startVec,
                 M_PI - cubicPath.angleAtPercent(0) / 180.0 * M_PI);
    if (mHead.style() != QCPLineEnding::esNone)
      mHead.draw(painter, endVec, -cubicPath.angleAtPercent(1) / 180.0 * M_PI);
  }
}

/*! \internal

  Returns the pen that should be used for drawing lines. Returns mPen when the
  item is not selected and mSelectedPen when it is.
*/
QPen QCPItemCurve::mainPen() const { return mSelected ? mSelectedPen : mPen; }
/* end of 'src/items/item-curve.cpp' */

/* including file 'src/items/item-rect.cpp' */
/* modified 2022-11-06T12:45:56, size 6472  */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPItemRect
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPItemRect
  \brief A rectangle

  \image html QCPItemRect.png "Rectangle example. Blue dotted circles are
  anchors, solid blue discs are positions."

  It has two positions, \a topLeft and \a bottomRight, which define the
  rectangle.
*/

/*!
  Creates a rectangle item and sets default values.

  The created item is automatically registered with \a parentPlot. This
  QCustomPlot instance takes ownership of the item, so do not delete it manually
  but use QCustomPlot::removeItem() instead.
*/
QCPItemRect::QCPItemRect(QCustomPlot *parentPlot)
    : QCPAbstractItem(parentPlot),
      topLeft(createPosition(QLatin1String("topLeft"))),
      bottomRight(createPosition(QLatin1String("bottomRight"))),
      top(createAnchor(QLatin1String("top"), aiTop)),
      topRight(createAnchor(QLatin1String("topRight"), aiTopRight)),
      right(createAnchor(QLatin1String("right"), aiRight)),
      bottom(createAnchor(QLatin1String("bottom"), aiBottom)),
      bottomLeft(createAnchor(QLatin1String("bottomLeft"), aiBottomLeft)),
      left(createAnchor(QLatin1String("left"), aiLeft)) {
  topLeft->setCoords(0, 1);
  bottomRight->setCoords(1, 0);

  setPen(QPen(Qt::black));
  setSelectedPen(QPen(Qt::blue, 2));
  setBrush(Qt::NoBrush);
  setSelectedBrush(Qt::NoBrush);
}

QCPItemRect::~QCPItemRect() {}

/*!
  Sets the pen that will be used to draw the line of the rectangle

  \see setSelectedPen, setBrush
*/
void QCPItemRect::setPen(const QPen &pen) { mPen = pen; }

/*!
  Sets the pen that will be used to draw the line of the rectangle when selected

  \see setPen, setSelected
*/
void QCPItemRect::setSelectedPen(const QPen &pen) { mSelectedPen = pen; }

/*!
  Sets the brush that will be used to fill the rectangle. To disable filling,
  set \a brush to Qt::NoBrush.

  \see setSelectedBrush, setPen
*/
void QCPItemRect::setBrush(const QBrush &brush) { mBrush = brush; }

/*!
  Sets the brush that will be used to fill the rectangle when selected. To
  disable filling, set \a brush to Qt::NoBrush.

  \see setBrush
*/
void QCPItemRect::setSelectedBrush(const QBrush &brush) {
  mSelectedBrush = brush;
}

/* inherits documentation from base class */
double QCPItemRect::selectTest(const QPointF &pos, bool onlySelectable,
                               QVariant *details) const {
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable) return -1;

  QRectF rect = QRectF(topLeft->pixelPosition(), bottomRight->pixelPosition())
                    .normalized();
  bool filledRect =
      mBrush.style() != Qt::NoBrush && mBrush.color().alpha() != 0;
  return rectDistance(rect, pos, filledRect);
}

/* inherits documentation from base class */
void QCPItemRect::draw(QCPPainter *painter) {
  QPointF p1 = topLeft->pixelPosition();
  QPointF p2 = bottomRight->pixelPosition();
  if (p1.toPoint() == p2.toPoint()) return;
  QRectF rect = QRectF(p1, p2).normalized();
  double clipPad = mainPen().widthF();
  QRectF boundingRect = rect.adjusted(-clipPad, -clipPad, clipPad, clipPad);
  if (boundingRect.intersects(clipRect()))  // only draw if bounding rect of
  // rect item is visible in cliprect
  {
    painter->setPen(mainPen());
    painter->setBrush(mainBrush());
    painter->drawRect(rect);
  }
}

/* inherits documentation from base class */
QPointF QCPItemRect::anchorPixelPosition(int anchorId) const {
  QRectF rect = QRectF(topLeft->pixelPosition(), bottomRight->pixelPosition());
  switch (anchorId) {
    case aiTop:
      return (rect.topLeft() + rect.topRight()) * 0.5;
    case aiTopRight:
      return rect.topRight();
    case aiRight:
      return (rect.topRight() + rect.bottomRight()) * 0.5;
    case aiBottom:
      return (rect.bottomLeft() + rect.bottomRight()) * 0.5;
    case aiBottomLeft:
      return rect.bottomLeft();
    case aiLeft:
      return (rect.topLeft() + rect.bottomLeft()) * 0.5;
  }

  qDebug() << Q_FUNC_INFO << "invalid anchorId" << anchorId;
  return {};
}

/*! \internal

  Returns the pen that should be used for drawing lines. Returns mPen when the
  item is not selected and mSelectedPen when it is.
*/
QPen QCPItemRect::mainPen() const { return mSelected ? mSelectedPen : mPen; }

/*! \internal

  Returns the brush that should be used for drawing fills of the item. Returns
  mBrush when the item is not selected and mSelectedBrush when it is.
*/
QBrush QCPItemRect::mainBrush() const {
  return mSelected ? mSelectedBrush : mBrush;
}
/* end of 'src/items/item-rect.cpp' */

/* including file 'src/items/item-text.cpp' */
/* modified 2022-11-06T12:45:56, size 13335 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPItemText
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPItemText
  \brief A text label

  \image html QCPItemText.png "Text example. Blue dotted circles are anchors,
  solid blue discs are positions."

  Its position is defined by the member \a position and the setting of \ref
  setPositionAlignment. The latter controls which part of the text rect shall be
  aligned with \a position.

  The text alignment itself (i.e. left, center, right) can be controlled with
  \ref setTextAlignment.

  The text may be rotated around the \a position point with \ref setRotation.
*/

/*!
  Creates a text item and sets default values.

  The created item is automatically registered with \a parentPlot. This
  QCustomPlot instance takes ownership of the item, so do not delete it manually
  but use QCustomPlot::removeItem() instead.
*/
QCPItemText::QCPItemText(QCustomPlot *parentPlot)
    : QCPAbstractItem(parentPlot),
      position(createPosition(QLatin1String("position"))),
      topLeft(createAnchor(QLatin1String("topLeft"), aiTopLeft)),
      top(createAnchor(QLatin1String("top"), aiTop)),
      topRight(createAnchor(QLatin1String("topRight"), aiTopRight)),
      right(createAnchor(QLatin1String("right"), aiRight)),
      bottomRight(createAnchor(QLatin1String("bottomRight"), aiBottomRight)),
      bottom(createAnchor(QLatin1String("bottom"), aiBottom)),
      bottomLeft(createAnchor(QLatin1String("bottomLeft"), aiBottomLeft)),
      left(createAnchor(QLatin1String("left"), aiLeft)),
      mText(QLatin1String("text")),
      mPositionAlignment(Qt::AlignCenter),
      mTextAlignment(Qt::AlignTop | Qt::AlignHCenter),
      mRotation(0) {
  position->setCoords(0, 0);

  setPen(Qt::NoPen);
  setSelectedPen(Qt::NoPen);
  setBrush(Qt::NoBrush);
  setSelectedBrush(Qt::NoBrush);
  setColor(Qt::black);
  setSelectedColor(Qt::blue);
}

QCPItemText::~QCPItemText() {}

/*!
  Sets the color of the text.
*/
void QCPItemText::setColor(const QColor &color) { mColor = color; }

/*!
  Sets the color of the text that will be used when the item is selected.
*/
void QCPItemText::setSelectedColor(const QColor &color) {
  mSelectedColor = color;
}

/*!
  Sets the pen that will be used do draw a rectangular border around the text.
  To disable the border, set \a pen to Qt::NoPen.

  \see setSelectedPen, setBrush, setPadding
*/
void QCPItemText::setPen(const QPen &pen) { mPen = pen; }

/*!
  Sets the pen that will be used do draw a rectangular border around the text,
  when the item is selected. To disable the border, set \a pen to Qt::NoPen.

  \see setPen
*/
void QCPItemText::setSelectedPen(const QPen &pen) { mSelectedPen = pen; }

/*!
  Sets the brush that will be used do fill the background of the text. To
  disable the background, set \a brush to Qt::NoBrush.

  \see setSelectedBrush, setPen, setPadding
*/
void QCPItemText::setBrush(const QBrush &brush) { mBrush = brush; }

/*!
  Sets the brush that will be used do fill the background of the text, when the
  item is selected. To disable the background, set \a brush to Qt::NoBrush.

  \see setBrush
*/
void QCPItemText::setSelectedBrush(const QBrush &brush) {
  mSelectedBrush = brush;
}

/*!
  Sets the font of the text.

  \see setSelectedFont, setColor
*/
void QCPItemText::setFont(const QFont &font) { mFont = font; }

/*!
  Sets the font of the text that will be used when the item is selected.

  \see setFont
*/
void QCPItemText::setSelectedFont(const QFont &font) { mSelectedFont = font; }

/*!
  Sets the text that will be displayed. Multi-line texts are supported by
  inserting a line break character, e.g. '\n'.

  \see setFont, setColor, setTextAlignment
*/
void QCPItemText::setText(const QString &text) { mText = text; }

/*!
  Sets which point of the text rect shall be aligned with \a position.

  Examples:
  \li If \a alignment is <tt>Qt::AlignHCenter | Qt::AlignTop</tt>, the text will
  be positioned such that the top of the text rect will be horizontally centered
  on \a position. \li If \a alignment is <tt>Qt::AlignLeft |
  Qt::AlignBottom</tt>, \a position will indicate the bottom left corner of the
  text rect.

  If you want to control the alignment of (multi-lined) text within the text
  rect, use \ref setTextAlignment.
*/
void QCPItemText::setPositionAlignment(Qt::Alignment alignment) {
  mPositionAlignment = alignment;
}

/*!
  Controls how (multi-lined) text is aligned inside the text rect (typically
  Qt::AlignLeft, Qt::AlignCenter or Qt::AlignRight).
*/
void QCPItemText::setTextAlignment(Qt::Alignment alignment) {
  mTextAlignment = alignment;
}

/*!
  Sets the angle in degrees by which the text (and the text rectangle, if
  visible) will be rotated around \a position.
*/
void QCPItemText::setRotation(double degrees) { mRotation = degrees; }

/*!
  Sets the distance between the border of the text rectangle and the text. The
  appearance (and visibility) of the text rectangle can be controlled with \ref
  setPen and \ref setBrush.
*/
void QCPItemText::setPadding(const QMargins &padding) { mPadding = padding; }

/* inherits documentation from base class */
double QCPItemText::selectTest(const QPointF &pos, bool onlySelectable,
                               QVariant *details) const {
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable) return -1;

  // The rect may be rotated, so we transform the actual clicked pos to the
  // rotated coordinate system, so we can use the normal rectDistance function
  // for non-rotated rects:
  QPointF positionPixels(position->pixelPosition());
  QTransform inputTransform;
  inputTransform.translate(positionPixels.x(), positionPixels.y());
  inputTransform.rotate(-mRotation);
  inputTransform.translate(-positionPixels.x(), -positionPixels.y());
  QPointF rotatedPos = inputTransform.map(pos);
  QFontMetrics fontMetrics(mFont);
  QRect textRect = fontMetrics.boundingRect(
      0, 0, 0, 0, Qt::TextDontClip | mTextAlignment, mText);
  QRect textBoxRect = textRect.adjusted(-mPadding.left(), -mPadding.top(),
                                        mPadding.right(), mPadding.bottom());
  QPointF textPos =
      getTextDrawPoint(positionPixels, textBoxRect, mPositionAlignment);
  textBoxRect.moveTopLeft(textPos.toPoint());

  return rectDistance(textBoxRect, rotatedPos, true);
}

/* inherits documentation from base class */
void QCPItemText::draw(QCPPainter *painter) {
  QPointF pos(position->pixelPosition());
  QTransform transform = painter->transform();
  transform.translate(pos.x(), pos.y());
  if (!qFuzzyIsNull(mRotation)) transform.rotate(mRotation);
  painter->setFont(mainFont());
  QRect textRect = painter->fontMetrics().boundingRect(
      0, 0, 0, 0, Qt::TextDontClip | mTextAlignment, mText);
  QRect textBoxRect = textRect.adjusted(-mPadding.left(), -mPadding.top(),
                                        mPadding.right(), mPadding.bottom());
  QPointF textPos = getTextDrawPoint(
      QPointF(0, 0), textBoxRect,
      mPositionAlignment);  // 0, 0 because the transform does the translation
  textRect.moveTopLeft(textPos.toPoint() +
                       QPoint(mPadding.left(), mPadding.top()));
  textBoxRect.moveTopLeft(textPos.toPoint());
  int clipPad = qCeil(mainPen().widthF());
  QRect boundingRect =
      textBoxRect.adjusted(-clipPad, -clipPad, clipPad, clipPad);
  if (transform.mapRect(boundingRect)
          .intersects(painter->transform().mapRect(clipRect()))) {
    painter->setTransform(transform);
    if ((mainBrush().style() != Qt::NoBrush &&
         mainBrush().color().alpha() != 0) ||
        (mainPen().style() != Qt::NoPen && mainPen().color().alpha() != 0)) {
      painter->setPen(mainPen());
      painter->setBrush(mainBrush());
      painter->drawRect(textBoxRect);
    }
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(mainColor()));
    painter->drawText(textRect, Qt::TextDontClip | mTextAlignment, mText);
  }
}

/* inherits documentation from base class */
QPointF QCPItemText::anchorPixelPosition(int anchorId) const {
  // get actual rect points (pretty much copied from draw function):
  QPointF pos(position->pixelPosition());
  QTransform transform;
  transform.translate(pos.x(), pos.y());
  if (!qFuzzyIsNull(mRotation)) transform.rotate(mRotation);
  QFontMetrics fontMetrics(mainFont());
  QRect textRect = fontMetrics.boundingRect(
      0, 0, 0, 0, Qt::TextDontClip | mTextAlignment, mText);
  QRectF textBoxRect = textRect.adjusted(-mPadding.left(), -mPadding.top(),
                                         mPadding.right(), mPadding.bottom());
  QPointF textPos = getTextDrawPoint(
      QPointF(0, 0), textBoxRect,
      mPositionAlignment);  // 0, 0 because the transform does the translation
  textBoxRect.moveTopLeft(textPos.toPoint());
  QPolygonF rectPoly = transform.map(QPolygonF(textBoxRect));

  switch (anchorId) {
    case aiTopLeft:
      return rectPoly.at(0);
    case aiTop:
      return (rectPoly.at(0) + rectPoly.at(1)) * 0.5;
    case aiTopRight:
      return rectPoly.at(1);
    case aiRight:
      return (rectPoly.at(1) + rectPoly.at(2)) * 0.5;
    case aiBottomRight:
      return rectPoly.at(2);
    case aiBottom:
      return (rectPoly.at(2) + rectPoly.at(3)) * 0.5;
    case aiBottomLeft:
      return rectPoly.at(3);
    case aiLeft:
      return (rectPoly.at(3) + rectPoly.at(0)) * 0.5;
  }

  qDebug() << Q_FUNC_INFO << "invalid anchorId" << anchorId;
  return {};
}

/*! \internal

  Returns the point that must be given to the QPainter::drawText function (which
  expects the top left point of the text rect), according to the position \a
  pos, the text bounding box \a rect and the requested \a positionAlignment.

  For example, if \a positionAlignment is <tt>Qt::AlignLeft |
  Qt::AlignBottom</tt> the returned point will be shifted upward by the height
  of \a rect, starting from \a pos. So if the text is finally drawn at that
  point, the lower left corner of the resulting text rect is at \a pos.
*/
QPointF QCPItemText::getTextDrawPoint(const QPointF &pos, const QRectF &rect,
                                      Qt::Alignment positionAlignment) const {
  if (positionAlignment == 0 ||
      positionAlignment == (Qt::AlignLeft | Qt::AlignTop))
    return pos;

  QPointF result = pos;  // start at top left
  if (positionAlignment.testFlag(Qt::AlignHCenter))
    result.rx() -= rect.width() / 2.0;
  else if (positionAlignment.testFlag(Qt::AlignRight))
    result.rx() -= rect.width();
  if (positionAlignment.testFlag(Qt::AlignVCenter))
    result.ry() -= rect.height() / 2.0;
  else if (positionAlignment.testFlag(Qt::AlignBottom))
    result.ry() -= rect.height();
  return result;
}

/*! \internal

  Returns the font that should be used for drawing text. Returns mFont when the
  item is not selected and mSelectedFont when it is.
*/
QFont QCPItemText::mainFont() const {
  return mSelected ? mSelectedFont : mFont;
}

/*! \internal

  Returns the color that should be used for drawing text. Returns mColor when
  the item is not selected and mSelectedColor when it is.
*/
QColor QCPItemText::mainColor() const {
  return mSelected ? mSelectedColor : mColor;
}

/*! \internal

  Returns the pen that should be used for drawing lines. Returns mPen when the
  item is not selected and mSelectedPen when it is.
*/
QPen QCPItemText::mainPen() const { return mSelected ? mSelectedPen : mPen; }

/*! \internal

  Returns the brush that should be used for drawing fills of the item. Returns
  mBrush when the item is not selected and mSelectedBrush when it is.
*/
QBrush QCPItemText::mainBrush() const {
  return mSelected ? mSelectedBrush : mBrush;
}
/* end of 'src/items/item-text.cpp' */

/* including file 'src/items/item-ellipse.cpp' */
/* modified 2022-11-06T12:45:56, size 7881     */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPItemEllipse
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPItemEllipse
  \brief An ellipse

  \image html QCPItemEllipse.png "Ellipse example. Blue dotted circles are
  anchors, solid blue discs are positions."

  It has two positions, \a topLeft and \a bottomRight, which define the rect the
  ellipse will be drawn in.
*/

/*!
  Creates an ellipse item and sets default values.

  The created item is automatically registered with \a parentPlot. This
  QCustomPlot instance takes ownership of the item, so do not delete it manually
  but use QCustomPlot::removeItem() instead.
*/
QCPItemEllipse::QCPItemEllipse(QCustomPlot *parentPlot)
    : QCPAbstractItem(parentPlot),
      topLeft(createPosition(QLatin1String("topLeft"))),
      bottomRight(createPosition(QLatin1String("bottomRight"))),
      topLeftRim(createAnchor(QLatin1String("topLeftRim"), aiTopLeftRim)),
      top(createAnchor(QLatin1String("top"), aiTop)),
      topRightRim(createAnchor(QLatin1String("topRightRim"), aiTopRightRim)),
      right(createAnchor(QLatin1String("right"), aiRight)),
      bottomRightRim(
          createAnchor(QLatin1String("bottomRightRim"), aiBottomRightRim)),
      bottom(createAnchor(QLatin1String("bottom"), aiBottom)),
      bottomLeftRim(
          createAnchor(QLatin1String("bottomLeftRim"), aiBottomLeftRim)),
      left(createAnchor(QLatin1String("left"), aiLeft)),
      center(createAnchor(QLatin1String("center"), aiCenter)) {
  topLeft->setCoords(0, 1);
  bottomRight->setCoords(1, 0);

  setPen(QPen(Qt::black));
  setSelectedPen(QPen(Qt::blue, 2));
  setBrush(Qt::NoBrush);
  setSelectedBrush(Qt::NoBrush);
}

QCPItemEllipse::~QCPItemEllipse() {}

/*!
  Sets the pen that will be used to draw the line of the ellipse

  \see setSelectedPen, setBrush
*/
void QCPItemEllipse::setPen(const QPen &pen) { mPen = pen; }

/*!
  Sets the pen that will be used to draw the line of the ellipse when selected

  \see setPen, setSelected
*/
void QCPItemEllipse::setSelectedPen(const QPen &pen) { mSelectedPen = pen; }

/*!
  Sets the brush that will be used to fill the ellipse. To disable filling, set
  \a brush to Qt::NoBrush.

  \see setSelectedBrush, setPen
*/
void QCPItemEllipse::setBrush(const QBrush &brush) { mBrush = brush; }

/*!
  Sets the brush that will be used to fill the ellipse when selected. To disable
  filling, set \a brush to Qt::NoBrush.

  \see setBrush
*/
void QCPItemEllipse::setSelectedBrush(const QBrush &brush) {
  mSelectedBrush = brush;
}

/* inherits documentation from base class */
double QCPItemEllipse::selectTest(const QPointF &pos, bool onlySelectable,
                                  QVariant *details) const {
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable) return -1;

  QPointF p1 = topLeft->pixelPosition();
  QPointF p2 = bottomRight->pixelPosition();
  QPointF center((p1 + p2) / 2.0);
  double a = qAbs(p1.x() - p2.x()) / 2.0;
  double b = qAbs(p1.y() - p2.y()) / 2.0;
  double x = pos.x() - center.x();
  double y = pos.y() - center.y();

  // distance to border:
  double c = 1.0 / qSqrt(x * x / (a * a) + y * y / (b * b));
  double result = qAbs(c - 1) * qSqrt(x * x + y * y);
  // filled ellipse, allow click inside to count as hit:
  if (result > mParentPlot->selectionTolerance() * 0.99 &&
      mBrush.style() != Qt::NoBrush && mBrush.color().alpha() != 0) {
    if (x * x / (a * a) + y * y / (b * b) <= 1)
      result = mParentPlot->selectionTolerance() * 0.99;
  }
  return result;
}

/* inherits documentation from base class */
void QCPItemEllipse::draw(QCPPainter *painter) {
  QPointF p1 = topLeft->pixelPosition();
  QPointF p2 = bottomRight->pixelPosition();
  if (p1.toPoint() == p2.toPoint()) return;
  QRectF ellipseRect = QRectF(p1, p2).normalized();
  const int clipEnlarge = qCeil(mainPen().widthF());
  QRect clip =
      clipRect().adjusted(-clipEnlarge, -clipEnlarge, clipEnlarge, clipEnlarge);
  if (ellipseRect.intersects(clip))  // only draw if bounding rect of ellipse is
  // visible in cliprect
  {
    painter->setPen(mainPen());
    painter->setBrush(mainBrush());
#ifdef __EXCEPTIONS
    try  // drawEllipse sometimes throws exceptions if ellipse is too big
    {
#endif
      painter->drawEllipse(ellipseRect);
#ifdef __EXCEPTIONS
    } catch (...) {
      qDebug() << Q_FUNC_INFO << "Item too large for memory, setting invisible";
      setVisible(false);
    }
#endif
  }
}

/* inherits documentation from base class */
QPointF QCPItemEllipse::anchorPixelPosition(int anchorId) const {
  QRectF rect = QRectF(topLeft->pixelPosition(), bottomRight->pixelPosition());
  switch (anchorId) {
    case aiTopLeftRim:
      return rect.center() + (rect.topLeft() - rect.center()) * 1 / qSqrt(2);
    case aiTop:
      return (rect.topLeft() + rect.topRight()) * 0.5;
    case aiTopRightRim:
      return rect.center() + (rect.topRight() - rect.center()) * 1 / qSqrt(2);
    case aiRight:
      return (rect.topRight() + rect.bottomRight()) * 0.5;
    case aiBottomRightRim:
      return rect.center() +
             (rect.bottomRight() - rect.center()) * 1 / qSqrt(2);
    case aiBottom:
      return (rect.bottomLeft() + rect.bottomRight()) * 0.5;
    case aiBottomLeftRim:
      return rect.center() + (rect.bottomLeft() - rect.center()) * 1 / qSqrt(2);
    case aiLeft:
      return (rect.topLeft() + rect.bottomLeft()) * 0.5;
    case aiCenter:
      return (rect.topLeft() + rect.bottomRight()) * 0.5;
  }

  qDebug() << Q_FUNC_INFO << "invalid anchorId" << anchorId;
  return {};
}

/*! \internal

  Returns the pen that should be used for drawing lines. Returns mPen when the
  item is not selected and mSelectedPen when it is.
*/
QPen QCPItemEllipse::mainPen() const { return mSelected ? mSelectedPen : mPen; }

/*! \internal

  Returns the brush that should be used for drawing fills of the item. Returns
  mBrush when the item is not selected and mSelectedBrush when it is.
*/
QBrush QCPItemEllipse::mainBrush() const {
  return mSelected ? mSelectedBrush : mBrush;
}
/* end of 'src/items/item-ellipse.cpp' */

/* including file 'src/items/item-pixmap.cpp' */
/* modified 2022-11-06T12:45:56, size 10622   */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPItemPixmap
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPItemPixmap
  \brief An arbitrary pixmap

  \image html QCPItemPixmap.png "Pixmap example. Blue dotted circles are
  anchors, solid blue discs are positions."

  It has two positions, \a topLeft and \a bottomRight, which define the
  rectangle the pixmap will be drawn in. Depending on the scale setting (\ref
  setScaled), the pixmap will be either scaled to fit the rectangle or be drawn
  aligned to the topLeft position.

  If scaling is enabled and \a topLeft is further to the bottom/right than \a
  bottomRight (as shown on the right side of the example image), the pixmap will
  be flipped in the respective orientations.
*/

/*!
  Creates a rectangle item and sets default values.

  The created item is automatically registered with \a parentPlot. This
  QCustomPlot instance takes ownership of the item, so do not delete it manually
  but use QCustomPlot::removeItem() instead.
*/
QCPItemPixmap::QCPItemPixmap(QCustomPlot *parentPlot)
    : QCPAbstractItem(parentPlot),
      topLeft(createPosition(QLatin1String("topLeft"))),
      bottomRight(createPosition(QLatin1String("bottomRight"))),
      top(createAnchor(QLatin1String("top"), aiTop)),
      topRight(createAnchor(QLatin1String("topRight"), aiTopRight)),
      right(createAnchor(QLatin1String("right"), aiRight)),
      bottom(createAnchor(QLatin1String("bottom"), aiBottom)),
      bottomLeft(createAnchor(QLatin1String("bottomLeft"), aiBottomLeft)),
      left(createAnchor(QLatin1String("left"), aiLeft)),
      mScaled(false),
      mScaledPixmapInvalidated(true),
      mAspectRatioMode(Qt::KeepAspectRatio),
      mTransformationMode(Qt::SmoothTransformation) {
  topLeft->setCoords(0, 1);
  bottomRight->setCoords(1, 0);

  setPen(Qt::NoPen);
  setSelectedPen(QPen(Qt::blue));
}

QCPItemPixmap::~QCPItemPixmap() {}

/*!
  Sets the pixmap that will be displayed.
*/
void QCPItemPixmap::setPixmap(const QPixmap &pixmap) {
  mPixmap = pixmap;
  mScaledPixmapInvalidated = true;
  if (mPixmap.isNull()) qDebug() << Q_FUNC_INFO << "pixmap is null";
}

/*!
  Sets whether the pixmap will be scaled to fit the rectangle defined by the \a
  topLeft and \a bottomRight positions.
*/
void QCPItemPixmap::setScaled(bool scaled, Qt::AspectRatioMode aspectRatioMode,
                              Qt::TransformationMode transformationMode) {
  mScaled = scaled;
  mAspectRatioMode = aspectRatioMode;
  mTransformationMode = transformationMode;
  mScaledPixmapInvalidated = true;
}

/*!
  Sets the pen that will be used to draw a border around the pixmap.

  \see setSelectedPen, setBrush
*/
void QCPItemPixmap::setPen(const QPen &pen) { mPen = pen; }

/*!
  Sets the pen that will be used to draw a border around the pixmap when
  selected

  \see setPen, setSelected
*/
void QCPItemPixmap::setSelectedPen(const QPen &pen) { mSelectedPen = pen; }

/* inherits documentation from base class */
double QCPItemPixmap::selectTest(const QPointF &pos, bool onlySelectable,
                                 QVariant *details) const {
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable) return -1;

  return rectDistance(getFinalRect(), pos, true);
}

/* inherits documentation from base class */
void QCPItemPixmap::draw(QCPPainter *painter) {
  bool flipHorz = false;
  bool flipVert = false;
  QRect rect = getFinalRect(&flipHorz, &flipVert);
  int clipPad = mainPen().style() == Qt::NoPen ? 0 : qCeil(mainPen().widthF());
  QRect boundingRect = rect.adjusted(-clipPad, -clipPad, clipPad, clipPad);
  if (boundingRect.intersects(clipRect())) {
    updateScaledPixmap(rect, flipHorz, flipVert);
    painter->drawPixmap(rect.topLeft(), mScaled ? mScaledPixmap : mPixmap);
    QPen pen = mainPen();
    if (pen.style() != Qt::NoPen) {
      painter->setPen(pen);
      painter->setBrush(Qt::NoBrush);
      painter->drawRect(rect);
    }
  }
}

/* inherits documentation from base class */
QPointF QCPItemPixmap::anchorPixelPosition(int anchorId) const {
  bool flipHorz = false;
  bool flipVert = false;
  QRect rect = getFinalRect(&flipHorz, &flipVert);
  // we actually want denormal rects (negative width/height) here, so restore
  // the flipped state:
  if (flipHorz) rect.adjust(rect.width(), 0, -rect.width(), 0);
  if (flipVert) rect.adjust(0, rect.height(), 0, -rect.height());

  switch (anchorId) {
    case aiTop:
      return (rect.topLeft() + rect.topRight()) * 0.5;
    case aiTopRight:
      return rect.topRight();
    case aiRight:
      return (rect.topRight() + rect.bottomRight()) * 0.5;
    case aiBottom:
      return (rect.bottomLeft() + rect.bottomRight()) * 0.5;
    case aiBottomLeft:
      return rect.bottomLeft();
    case aiLeft:
      return (rect.topLeft() + rect.bottomLeft()) * 0.5;
  }

  qDebug() << Q_FUNC_INFO << "invalid anchorId" << anchorId;
  return {};
}

/*! \internal

  Creates the buffered scaled image (\a mScaledPixmap) to fit the specified \a
  finalRect. The parameters \a flipHorz and \a flipVert control whether the
  resulting image shall be flipped horizontally or vertically. (This is used
  when \a topLeft is further to the bottom/right than \a bottomRight.)

  This function only creates the scaled pixmap when the buffered pixmap has a
  different size than the expected result, so calling this function repeatedly,
  e.g. in the \ref draw function, does not cause expensive rescaling every time.

  If scaling is disabled, sets mScaledPixmap to a null QPixmap.
*/
void QCPItemPixmap::updateScaledPixmap(QRect finalRect, bool flipHorz,
                                       bool flipVert) {
  if (mPixmap.isNull()) return;

  if (mScaled) {
#ifdef QCP_DEVICEPIXELRATIO_SUPPORTED
    double devicePixelRatio = mPixmap.devicePixelRatio();
#else
    double devicePixelRatio = 1.0;
#endif
    if (finalRect.isNull()) finalRect = getFinalRect(&flipHorz, &flipVert);
    if (mScaledPixmapInvalidated ||
        finalRect.size() != mScaledPixmap.size() / devicePixelRatio) {
      mScaledPixmap = mPixmap.scaled(finalRect.size() * devicePixelRatio,
                                     mAspectRatioMode, mTransformationMode);
      if (flipHorz || flipVert)
        mScaledPixmap = QPixmap::fromImage(
            mScaledPixmap.toImage().mirrored(flipHorz, flipVert));
#ifdef QCP_DEVICEPIXELRATIO_SUPPORTED
      mScaledPixmap.setDevicePixelRatio(devicePixelRatio);
#endif
    }
  } else if (!mScaledPixmap.isNull())
    mScaledPixmap = QPixmap();
  mScaledPixmapInvalidated = false;
}

/*! \internal

  Returns the final (tight) rect the pixmap is drawn in, depending on the
  current item positions and scaling settings.

  The output parameters \a flippedHorz and \a flippedVert return whether the
  pixmap should be drawn flipped horizontally or vertically in the returned
  rect. (The returned rect itself is always normalized, i.e. the top left corner
  of the rect is actually further to the top/left than the bottom right corner).
  This is the case when the item position \a topLeft is further to the
  bottom/right than \a bottomRight.

  If scaling is disabled, returns a rect with size of the original pixmap and
  the top left corner aligned with the item position \a topLeft. The position \a
  bottomRight is ignored.
*/
QRect QCPItemPixmap::getFinalRect(bool *flippedHorz, bool *flippedVert) const {
  QRect result;
  bool flipHorz = false;
  bool flipVert = false;
  QPoint p1 = topLeft->pixelPosition().toPoint();
  QPoint p2 = bottomRight->pixelPosition().toPoint();
  if (p1 == p2) return {p1, QSize(0, 0)};
  if (mScaled) {
    QSize newSize = QSize(p2.x() - p1.x(), p2.y() - p1.y());
    QPoint topLeft = p1;
    if (newSize.width() < 0) {
      flipHorz = true;
      newSize.rwidth() *= -1;
      topLeft.setX(p2.x());
    }
    if (newSize.height() < 0) {
      flipVert = true;
      newSize.rheight() *= -1;
      topLeft.setY(p2.y());
    }
    QSize scaledSize = mPixmap.size();
#ifdef QCP_DEVICEPIXELRATIO_SUPPORTED
    scaledSize /= mPixmap.devicePixelRatio();
    scaledSize.scale(newSize * mPixmap.devicePixelRatio(), mAspectRatioMode);
#else
    scaledSize.scale(newSize, mAspectRatioMode);
#endif
    result = QRect(topLeft, scaledSize);
  } else {
#ifdef QCP_DEVICEPIXELRATIO_SUPPORTED
    result = QRect(p1, mPixmap.size() / mPixmap.devicePixelRatio());
#else
    result = QRect(p1, mPixmap.size());
#endif
  }
  if (flippedHorz) *flippedHorz = flipHorz;
  if (flippedVert) *flippedVert = flipVert;
  return result;
}

/*! \internal

  Returns the pen that should be used for drawing lines. Returns mPen when the
  item is not selected and mSelectedPen when it is.
*/
QPen QCPItemPixmap::mainPen() const { return mSelected ? mSelectedPen : mPen; }
/* end of 'src/items/item-pixmap.cpp' */

/* including file 'src/items/item-tracer.cpp' */
/* modified 2022-11-06T12:45:56, size 14645   */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPItemTracer
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPItemTracer
  \brief Item that sticks to QCPGraph data points

  \image html QCPItemTracer.png "Tracer example. Blue dotted circles are
  anchors, solid blue discs are positions."

  The tracer can be connected with a QCPGraph via \ref setGraph. Then it will
  automatically adopt the coordinate axes of the graph and update its \a
  position to be on the graph's data. This means the key stays controllable via
  \ref setGraphKey, but the value will follow the graph data. If a QCPGraph is
  connected, note that setting the coordinates of the tracer item directly via
  \a position will have no effect because they will be overriden in the next
  redraw (this is when the coordinate update happens).

  If the specified key in \ref setGraphKey is outside the key bounds of the
  graph, the tracer will stay at the corresponding end of the graph.

  With \ref setInterpolating you may specify whether the tracer may only stay
  exactly on data points or whether it interpolates data points linearly, if
  given a key that lies between two data points of the graph.

  The tracer has different visual styles, see \ref setStyle. It is also possible
  to make the tracer have no own visual appearance (set the style to \ref
  tsNone), and just connect other item positions to the tracer \a position (used
  as an anchor) via \ref QCPItemPosition::setParentAnchor.

  \note The tracer position is only automatically updated upon redraws. So when
  the data of the graph changes and immediately afterwards (without a redraw)
  the position coordinates of the tracer are retrieved, they will not reflect
  the updated data of the graph. In this case \ref updatePosition must be called
  manually, prior to reading the tracer coordinates.
*/

/*!
  Creates a tracer item and sets default values.

  The created item is automatically registered with \a parentPlot. This
  QCustomPlot instance takes ownership of the item, so do not delete it manually
  but use QCustomPlot::removeItem() instead.
*/
QCPItemTracer::QCPItemTracer(QCustomPlot *parentPlot)
    : QCPAbstractItem(parentPlot),
      position(createPosition(QLatin1String("position"))),
      mSize(6),
      mStyle(tsCrosshair),
      mGraph(nullptr),
      mGraphKey(0),
      mInterpolating(false) {
  position->setCoords(0, 0);

  setBrush(Qt::NoBrush);
  setSelectedBrush(Qt::NoBrush);
  setPen(QPen(Qt::black));
  setSelectedPen(QPen(Qt::blue, 2));
}

QCPItemTracer::~QCPItemTracer() {}

/*!
  Sets the pen that will be used to draw the line of the tracer

  \see setSelectedPen, setBrush
*/
void QCPItemTracer::setPen(const QPen &pen) { mPen = pen; }

/*!
  Sets the pen that will be used to draw the line of the tracer when selected

  \see setPen, setSelected
*/
void QCPItemTracer::setSelectedPen(const QPen &pen) { mSelectedPen = pen; }

/*!
  Sets the brush that will be used to draw any fills of the tracer

  \see setSelectedBrush, setPen
*/
void QCPItemTracer::setBrush(const QBrush &brush) { mBrush = brush; }

/*!
  Sets the brush that will be used to draw any fills of the tracer, when
  selected.

  \see setBrush, setSelected
*/
void QCPItemTracer::setSelectedBrush(const QBrush &brush) {
  mSelectedBrush = brush;
}

/*!
  Sets the size of the tracer in pixels, if the style supports setting a size
  (e.g. \ref tsSquare does, \ref tsCrosshair does not).
*/
void QCPItemTracer::setSize(double size) { mSize = size; }

/*!
  Sets the style/visual appearance of the tracer.

  If you only want to use the tracer \a position as an anchor for other items,
  set \a style to \ref tsNone.
*/
void QCPItemTracer::setStyle(QCPItemTracer::TracerStyle style) {
  mStyle = style;
}

/*!
  Sets the QCPGraph this tracer sticks to. The tracer \a position will be set to
  type QCPItemPosition::ptPlotCoords and the axes will be set to the axes of \a
  graph.

  To free the tracer from any graph, set \a graph to \c nullptr. The tracer \a
  position can then be placed freely like any other item position. This is the
  state the tracer will assume when its graph gets deleted while still attached
  to it.

  \see setGraphKey
*/
void QCPItemTracer::setGraph(QCPGraph *graph) {
  if (graph) {
    if (graph->parentPlot() == mParentPlot) {
      position->setType(QCPItemPosition::ptPlotCoords);
      position->setAxes(graph->keyAxis(), graph->valueAxis());
      mGraph = graph;
      updatePosition();
    } else
      qDebug() << Q_FUNC_INFO
               << "graph isn't in same QCustomPlot instance as this item";
  } else {
    mGraph = nullptr;
  }
}

/*!
  Sets the key of the graph's data point the tracer will be positioned at. This
  is the only free coordinate of a tracer when attached to a graph.

  Depending on \ref setInterpolating, the tracer will be either positioned on
  the data point closest to \a key, or will stay exactly at \a key and
  interpolate the value linearly.

  \see setGraph, setInterpolating
*/
void QCPItemTracer::setGraphKey(double key) { mGraphKey = key; }

/*!
  Sets whether the value of the graph's data points shall be interpolated, when
  positioning the tracer.

  If \a enabled is set to false and a key is given with \ref setGraphKey, the
  tracer is placed on the data point of the graph which is closest to the key,
  but which is not necessarily exactly there. If \a enabled is true, the tracer
  will be positioned exactly at the specified key, and the appropriate value
  will be interpolated from the graph's data points linearly.

  \see setGraph, setGraphKey
*/
void QCPItemTracer::setInterpolating(bool enabled) { mInterpolating = enabled; }

/* inherits documentation from base class */
double QCPItemTracer::selectTest(const QPointF &pos, bool onlySelectable,
                                 QVariant *details) const {
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable) return -1;

  QPointF center(position->pixelPosition());
  double w = mSize / 2.0;
  QRect clip = clipRect();
  switch (mStyle) {
    case tsNone:
      return -1;
    case tsPlus: {
      if (clipRect().intersects(
              QRectF(center - QPointF(w, w), center + QPointF(w, w)).toRect()))
        return qSqrt(
            qMin(QCPVector2D(pos).distanceSquaredToLine(center + QPointF(-w, 0),
                                                        center + QPointF(w, 0)),
                 QCPVector2D(pos).distanceSquaredToLine(
                     center + QPointF(0, -w), center + QPointF(0, w))));
      break;
    }
    case tsCrosshair: {
      return qSqrt(qMin(QCPVector2D(pos).distanceSquaredToLine(
                            QCPVector2D(clip.left(), center.y()),
                            QCPVector2D(clip.right(), center.y())),
                        QCPVector2D(pos).distanceSquaredToLine(
                            QCPVector2D(center.x(), clip.top()),
                            QCPVector2D(center.x(), clip.bottom()))));
    }
    case tsCircle: {
      if (clip.intersects(QRectF(center - QPointF(w, w), center + QPointF(w, w))
                              .toRect())) {
        // distance to border:
        double centerDist = QCPVector2D(center - pos).length();
        double circleLine = w;
        double result = qAbs(centerDist - circleLine);
        // filled ellipse, allow click inside to count as hit:
        if (result > mParentPlot->selectionTolerance() * 0.99 &&
            mBrush.style() != Qt::NoBrush && mBrush.color().alpha() != 0) {
          if (centerDist <= circleLine)
            result = mParentPlot->selectionTolerance() * 0.99;
        }
        return result;
      }
      break;
    }
    case tsSquare: {
      if (clip.intersects(QRectF(center - QPointF(w, w), center + QPointF(w, w))
                              .toRect())) {
        QRectF rect = QRectF(center - QPointF(w, w), center + QPointF(w, w));
        bool filledRect =
            mBrush.style() != Qt::NoBrush && mBrush.color().alpha() != 0;
        return rectDistance(rect, pos, filledRect);
      }
      break;
    }
  }
  return -1;
}

/* inherits documentation from base class */
void QCPItemTracer::draw(QCPPainter *painter) {
  updatePosition();
  if (mStyle == tsNone) return;

  painter->setPen(mainPen());
  painter->setBrush(mainBrush());
  QPointF center(position->pixelPosition());
  double w = mSize / 2.0;
  QRect clip = clipRect();
  switch (mStyle) {
    case tsNone:
      return;
    case tsPlus: {
      if (clip.intersects(QRectF(center - QPointF(w, w), center + QPointF(w, w))
                              .toRect())) {
        painter->drawLine(
            QLineF(center + QPointF(-w, 0), center + QPointF(w, 0)));
        painter->drawLine(
            QLineF(center + QPointF(0, -w), center + QPointF(0, w)));
      }
      break;
    }
    case tsCrosshair: {
      if (center.y() > clip.top() && center.y() < clip.bottom())
        painter->drawLine(
            QLineF(clip.left(), center.y(), clip.right(), center.y()));
      if (center.x() > clip.left() && center.x() < clip.right())
        painter->drawLine(
            QLineF(center.x(), clip.top(), center.x(), clip.bottom()));
      break;
    }
    case tsCircle: {
      if (clip.intersects(
              QRectF(center - QPointF(w, w), center + QPointF(w, w)).toRect()))
        painter->drawEllipse(center, w, w);
      break;
    }
    case tsSquare: {
      if (clip.intersects(
              QRectF(center - QPointF(w, w), center + QPointF(w, w)).toRect()))
        painter->drawRect(
            QRectF(center - QPointF(w, w), center + QPointF(w, w)));
      break;
    }
  }
}

/*!
  If the tracer is connected with a graph (\ref setGraph), this function updates
  the tracer's \a position to reside on the graph data, depending on the
  configured key (\ref setGraphKey).

  It is called automatically on every redraw and normally doesn't need to be
  called manually. One exception is when you want to read the tracer coordinates
  via \a position and are not sure that the graph's data (or the tracer key with
  \ref setGraphKey) hasn't changed since the last redraw. In that situation,
  call this function before accessing \a position, to make sure you don't get
  out-of-date coordinates.

  If there is no graph set on this tracer, this function does nothing.
*/
void QCPItemTracer::updatePosition() {
  if (mGraph) {
    if (mParentPlot->hasPlottable(mGraph)) {
      if (mGraph->data()->size() > 1) {
        QCPGraphDataContainer::const_iterator first =
            mGraph->data()->constBegin();
        QCPGraphDataContainer::const_iterator last =
            mGraph->data()->constEnd() - 1;
        if (mGraphKey <= first->key)
          position->setCoords(first->key, first->value);
        else if (mGraphKey >= last->key)
          position->setCoords(last->key, last->value);
        else {
          QCPGraphDataContainer::const_iterator it =
              mGraph->data()->findBegin(mGraphKey);
          if (it !=
              mGraph->data()
                  ->constEnd())  // mGraphKey is not exactly on last iterator,
          // but somewhere between iterators
          {
            QCPGraphDataContainer::const_iterator prevIt = it;
            ++it;  // won't advance to constEnd because we handled that case
                   // (mGraphKey >= last->key) before
            if (mInterpolating) {
              // interpolate between iterators around mGraphKey:
              double slope = 0;
              if (!qFuzzyCompare(double(it->key), double(prevIt->key)))
                slope = (it->value - prevIt->value) / (it->key - prevIt->key);
              position->setCoords(
                  mGraphKey, (mGraphKey - prevIt->key) * slope + prevIt->value);
            } else {
              // find iterator with key closest to mGraphKey:
              if (mGraphKey < (prevIt->key + it->key) * 0.5)
                position->setCoords(prevIt->key, prevIt->value);
              else
                position->setCoords(it->key, it->value);
            }
          } else  // mGraphKey is exactly on last iterator (should actually be
            // caught when comparing first/last keys, but this is a
            // failsafe for fp uncertainty)
            position->setCoords(it->key, it->value);
        }
      } else if (mGraph->data()->size() == 1) {
        QCPGraphDataContainer::const_iterator it = mGraph->data()->constBegin();
        position->setCoords(it->key, it->value);
      } else
        qDebug() << Q_FUNC_INFO << "graph has no data";
    } else
      qDebug() << Q_FUNC_INFO
               << "graph not contained in QCustomPlot instance (anymore)";
  }
}

/*! \internal

  Returns the pen that should be used for drawing lines. Returns mPen when the
  item is not selected and mSelectedPen when it is.
*/
QPen QCPItemTracer::mainPen() const { return mSelected ? mSelectedPen : mPen; }

/*! \internal

  Returns the brush that should be used for drawing fills of the item. Returns
  mBrush when the item is not selected and mSelectedBrush when it is.
*/
QBrush QCPItemTracer::mainBrush() const {
  return mSelected ? mSelectedBrush : mBrush;
}
/* end of 'src/items/item-tracer.cpp' */

/* including file 'src/items/item-bracket.cpp' */
/* modified 2022-11-06T12:45:56, size 10705    */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPItemBracket
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPItemBracket
  \brief A bracket for referencing/highlighting certain parts in the plot.

  \image html QCPItemBracket.png "Bracket example. Blue dotted circles are
  anchors, solid blue discs are positions."

  It has two positions, \a left and \a right, which define the span of the
  bracket. If \a left is actually farther to the left than \a right, the bracket
  is opened to the bottom, as shown in the example image.

  The bracket supports multiple styles via \ref setStyle. The length, i.e. how
  far the bracket stretches away from the embraced span, can be controlled with
  \ref setLength.

  \image html QCPItemBracket-length.png
  <center>Demonstrating the effect of different values for \ref setLength, for
  styles \ref bsCalligraphic and \ref bsSquare. Anchors and positions are
  displayed for reference.</center>

  It provides an anchor \a center, to allow connection of other items, e.g. an
  arrow (QCPItemLine or QCPItemCurve) or a text label (QCPItemText), to the
  bracket.
*/

/*!
  Creates a bracket item and sets default values.

  The created item is automatically registered with \a parentPlot. This
  QCustomPlot instance takes ownership of the item, so do not delete it manually
  but use QCustomPlot::removeItem() instead.
*/
QCPItemBracket::QCPItemBracket(QCustomPlot *parentPlot)
    : QCPAbstractItem(parentPlot),
      left(createPosition(QLatin1String("left"))),
      right(createPosition(QLatin1String("right"))),
      center(createAnchor(QLatin1String("center"), aiCenter)),
      mLength(8),
      mStyle(bsCalligraphic) {
  left->setCoords(0, 0);
  right->setCoords(1, 1);

  setPen(QPen(Qt::black));
  setSelectedPen(QPen(Qt::blue, 2));
}

QCPItemBracket::~QCPItemBracket() {}

/*!
  Sets the pen that will be used to draw the bracket.

  Note that when the style is \ref bsCalligraphic, only the color will be taken
  from the pen, the stroke and width are ignored. To change the apparent stroke
  width of a calligraphic bracket, use \ref setLength, which has a similar
  effect.

  \see setSelectedPen
*/
void QCPItemBracket::setPen(const QPen &pen) { mPen = pen; }

/*!
  Sets the pen that will be used to draw the bracket when selected

  \see setPen, setSelected
*/
void QCPItemBracket::setSelectedPen(const QPen &pen) { mSelectedPen = pen; }

/*!
  Sets the \a length in pixels how far the bracket extends in the direction
  towards the embraced span of the bracket (i.e. perpendicular to the
  <i>left</i>-<i>right</i>-direction)

  \image html QCPItemBracket-length.png
  <center>Demonstrating the effect of different values for \ref setLength, for
  styles \ref bsCalligraphic and \ref bsSquare. Anchors and positions are
  displayed for reference.</center>
*/
void QCPItemBracket::setLength(double length) { mLength = length; }

/*!
  Sets the style of the bracket, i.e. the shape/visual appearance.

  \see setPen
*/
void QCPItemBracket::setStyle(QCPItemBracket::BracketStyle style) {
  mStyle = style;
}

/* inherits documentation from base class */
double QCPItemBracket::selectTest(const QPointF &pos, bool onlySelectable,
                                  QVariant *details) const {
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable) return -1;

  QCPVector2D p(pos);
  QCPVector2D leftVec(left->pixelPosition());
  QCPVector2D rightVec(right->pixelPosition());
  if (leftVec.toPoint() == rightVec.toPoint()) return -1;

  QCPVector2D widthVec = (rightVec - leftVec) * 0.5;
  QCPVector2D lengthVec = widthVec.perpendicular().normalized() * mLength;
  QCPVector2D centerVec = (rightVec + leftVec) * 0.5 - lengthVec;

  switch (mStyle) {
    case QCPItemBracket::bsSquare:
    case QCPItemBracket::bsRound: {
      double a =
          p.distanceSquaredToLine(centerVec - widthVec, centerVec + widthVec);
      double b = p.distanceSquaredToLine(centerVec - widthVec + lengthVec,
                                         centerVec - widthVec);
      double c = p.distanceSquaredToLine(centerVec + widthVec + lengthVec,
                                         centerVec + widthVec);
      return qSqrt(qMin(qMin(a, b), c));
    }
    case QCPItemBracket::bsCurly:
    case QCPItemBracket::bsCalligraphic: {
      double a = p.distanceSquaredToLine(
          centerVec - widthVec * 0.75 + lengthVec * 0.15,
          centerVec + lengthVec * 0.3);
      double b = p.distanceSquaredToLine(
          centerVec - widthVec + lengthVec * 0.7,
          centerVec - widthVec * 0.75 + lengthVec * 0.15);
      double c = p.distanceSquaredToLine(
          centerVec + widthVec * 0.75 + lengthVec * 0.15,
          centerVec + lengthVec * 0.3);
      double d = p.distanceSquaredToLine(
          centerVec + widthVec + lengthVec * 0.7,
          centerVec + widthVec * 0.75 + lengthVec * 0.15);
      return qSqrt(qMin(qMin(a, b), qMin(c, d)));
    }
  }
  return -1;
}

/* inherits documentation from base class */
void QCPItemBracket::draw(QCPPainter *painter) {
  QCPVector2D leftVec(left->pixelPosition());
  QCPVector2D rightVec(right->pixelPosition());
  if (leftVec.toPoint() == rightVec.toPoint()) return;

  QCPVector2D widthVec = (rightVec - leftVec) * 0.5;
  QCPVector2D lengthVec = widthVec.perpendicular().normalized() * mLength;
  QCPVector2D centerVec = (rightVec + leftVec) * 0.5 - lengthVec;

  QPolygon boundingPoly;
  boundingPoly << leftVec.toPoint() << rightVec.toPoint()
               << (rightVec - lengthVec).toPoint()
               << (leftVec - lengthVec).toPoint();
  const int clipEnlarge = qCeil(mainPen().widthF());
  QRect clip =
      clipRect().adjusted(-clipEnlarge, -clipEnlarge, clipEnlarge, clipEnlarge);
  if (clip.intersects(boundingPoly.boundingRect())) {
    painter->setPen(mainPen());
    switch (mStyle) {
      case bsSquare: {
        painter->drawLine((centerVec + widthVec).toPointF(),
                          (centerVec - widthVec).toPointF());
        painter->drawLine((centerVec + widthVec).toPointF(),
                          (centerVec + widthVec + lengthVec).toPointF());
        painter->drawLine((centerVec - widthVec).toPointF(),
                          (centerVec - widthVec + lengthVec).toPointF());
        break;
      }
      case bsRound: {
        painter->setBrush(Qt::NoBrush);
        QPainterPath path;
        path.moveTo((centerVec + widthVec + lengthVec).toPointF());
        path.cubicTo((centerVec + widthVec).toPointF(),
                     (centerVec + widthVec).toPointF(), centerVec.toPointF());
        path.cubicTo((centerVec - widthVec).toPointF(),
                     (centerVec - widthVec).toPointF(),
                     (centerVec - widthVec + lengthVec).toPointF());
        painter->drawPath(path);
        break;
      }
      case bsCurly: {
        painter->setBrush(Qt::NoBrush);
        QPainterPath path;
        path.moveTo((centerVec + widthVec + lengthVec).toPointF());
        path.cubicTo((centerVec + widthVec - lengthVec * 0.8).toPointF(),
                     (centerVec + 0.4 * widthVec + lengthVec).toPointF(),
                     centerVec.toPointF());
        path.cubicTo((centerVec - 0.4 * widthVec + lengthVec).toPointF(),
                     (centerVec - widthVec - lengthVec * 0.8).toPointF(),
                     (centerVec - widthVec + lengthVec).toPointF());
        painter->drawPath(path);
        break;
      }
      case bsCalligraphic: {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QBrush(mainPen().color()));
        QPainterPath path;
        path.moveTo((centerVec + widthVec + lengthVec).toPointF());

        path.cubicTo((centerVec + widthVec - lengthVec * 0.8).toPointF(),
                     (centerVec + 0.4 * widthVec + 0.8 * lengthVec).toPointF(),
                     centerVec.toPointF());
        path.cubicTo((centerVec - 0.4 * widthVec + 0.8 * lengthVec).toPointF(),
                     (centerVec - widthVec - lengthVec * 0.8).toPointF(),
                     (centerVec - widthVec + lengthVec).toPointF());

        path.cubicTo((centerVec - widthVec - lengthVec * 0.5).toPointF(),
                     (centerVec - 0.2 * widthVec + 1.2 * lengthVec).toPointF(),
                     (centerVec + lengthVec * 0.2).toPointF());
        path.cubicTo((centerVec + 0.2 * widthVec + 1.2 * lengthVec).toPointF(),
                     (centerVec + widthVec - lengthVec * 0.5).toPointF(),
                     (centerVec + widthVec + lengthVec).toPointF());

        painter->drawPath(path);
        break;
      }
    }
  }
}

/* inherits documentation from base class */
QPointF QCPItemBracket::anchorPixelPosition(int anchorId) const {
  QCPVector2D leftVec(left->pixelPosition());
  QCPVector2D rightVec(right->pixelPosition());
  if (leftVec.toPoint() == rightVec.toPoint()) return leftVec.toPointF();

  QCPVector2D widthVec = (rightVec - leftVec) * 0.5;
  QCPVector2D lengthVec = widthVec.perpendicular().normalized() * mLength;
  QCPVector2D centerVec = (rightVec + leftVec) * 0.5 - lengthVec;

  switch (anchorId) {
    case aiCenter:
      return centerVec.toPointF();
  }
  qDebug() << Q_FUNC_INFO << "invalid anchorId" << anchorId;
  return {};
}

/*! \internal

  Returns the pen that should be used for drawing lines. Returns mPen when the
  item is not selected and mSelectedPen when it is.
*/
QPen QCPItemBracket::mainPen() const { return mSelected ? mSelectedPen : mPen; }
/* end of 'src/items/item-bracket.cpp' */

/* including file 'src/polar/radialaxis.cpp' */
/* modified 2022-11-06T12:45:57, size 49415  */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPPolarAxisRadial
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPPolarAxisRadial
  \brief The radial axis inside a radial plot

  \warning In this QCustomPlot version, polar plots are a tech preview. Expect
  documentation and functionality to be incomplete, as well as changing public
  interfaces in the future.

  Each axis holds an instance of QCPAxisTicker which is used to generate the
  tick coordinates and tick labels. You can access the currently installed \ref
  ticker or set a new one (possibly one of the specialized subclasses, or your
  own subclass) via \ref setTicker. For details, see the documentation of
  QCPAxisTicker.
*/

/* start of documentation of inline functions */

/*! \fn QSharedPointer<QCPAxisTicker> QCPPolarAxisRadial::ticker() const

  Returns a modifiable shared pointer to the currently installed axis ticker.
  The axis ticker is responsible for generating the tick positions and tick
  labels of this axis. You can access the \ref QCPAxisTicker with this method
  and modify basic properties such as the approximate tick count
  (\ref QCPAxisTicker::setTickCount).

  You can gain more control over the axis ticks by setting a different \ref
  QCPAxisTicker subclass, see the documentation there. A new axis ticker can be
  set with \ref setTicker.

  Since the ticker is stored in the axis as a shared pointer, multiple axes may
  share the same axis ticker simply by passing the same shared pointer to
  multiple axes.

  \see setTicker
*/

/* end of documentation of inline functions */
/* start of documentation of signals */

/*! \fn void QCPPolarAxisRadial::rangeChanged(const QCPRange &newRange)

  This signal is emitted when the range of this axis has changed. You can
  connect it to the \ref setRange slot of another axis to communicate the new
  range to the other axis, in order for it to be synchronized.

  You may also manipulate/correct the range with \ref setRange in a slot
  connected to this signal. This is useful if for example a maximum range span
  shall not be exceeded, or if the lower/upper range shouldn't go beyond certain
  values (see \ref QCPRange::bounded). For example, the following slot would
  limit the x axis to ranges between 0 and 10: \code
  customPlot->xAxis->setRange(newRange.bounded(0, 10))
  \endcode
*/

/*! \fn void QCPPolarAxisRadial::rangeChanged(const QCPRange &newRange, const
  QCPRange &oldRange) \overload

  Additionally to the new range, this signal also provides the previous range
  held by the axis as \a oldRange.
*/

/*! \fn void QCPPolarAxisRadial::scaleTypeChanged(QCPPolarAxisRadial::ScaleType
  scaleType);

  This signal is emitted when the scale type changes, by calls to \ref
  setScaleType
*/

/*! \fn void
  QCPPolarAxisRadial::selectionChanged(QCPPolarAxisRadial::SelectableParts
  selection)

  This signal is emitted when the selection state of this axis has changed,
  either by user interaction or by a direct call to \ref setSelectedParts.
*/

/*! \fn void QCPPolarAxisRadial::selectableChanged(const
  QCPPolarAxisRadial::SelectableParts &parts);

  This signal is emitted when the selectability changes, by calls to \ref
  setSelectableParts
*/

/* end of documentation of signals */

/*!
  Constructs an Axis instance of Type \a type for the axis rect \a parent.

  Usually it isn't necessary to instantiate axes directly, because you can let
  QCustomPlot create them for you with \ref QCPAxisRect::addAxis. If you want to
  use own QCPAxis-subclasses however, create them manually and then inject them
  also via \ref QCPAxisRect::addAxis.
*/
QCPPolarAxisRadial::QCPPolarAxisRadial(QCPPolarAxisAngular *parent)
    : QCPLayerable(parent->parentPlot(), QString(), parent),
      mRangeDrag(true),
      mRangeZoom(true),
      mRangeZoomFactor(0.85),
      // axis base:
      mAngularAxis(parent),
      mAngle(45),
      mAngleReference(arAngularAxis),
      mSelectableParts(spAxis | spTickLabels | spAxisLabel),
      mSelectedParts(spNone),
      mBasePen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
      mSelectedBasePen(QPen(Qt::blue, 2)),
      // axis label:
      mLabelPadding(0),
      mLabel(),
      mLabelFont(mParentPlot->font()),
      mSelectedLabelFont(
          QFont(mLabelFont.family(), mLabelFont.pointSize(), QFont::Bold)),
      mLabelColor(Qt::black),
      mSelectedLabelColor(Qt::blue),
      // tick labels:
      // mTickLabelPadding(0), in label painter
      mTickLabels(true),
      // mTickLabelRotation(0), in label painter
      mTickLabelFont(mParentPlot->font()),
      mSelectedTickLabelFont(QFont(mTickLabelFont.family(),
                                   mTickLabelFont.pointSize(), QFont::Bold)),
      mTickLabelColor(Qt::black),
      mSelectedTickLabelColor(Qt::blue),
      mNumberPrecision(6),
      mNumberFormatChar('g'),
      mNumberBeautifulPowers(true),
      mNumberMultiplyCross(false),
      // ticks and subticks:
      mTicks(true),
      mSubTicks(true),
      mTickLengthIn(5),
      mTickLengthOut(0),
      mSubTickLengthIn(2),
      mSubTickLengthOut(0),
      mTickPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
      mSelectedTickPen(QPen(Qt::blue, 2)),
      mSubTickPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
      mSelectedSubTickPen(QPen(Qt::blue, 2)),
      // scale and range:
      mRange(0, 5),
      mRangeReversed(false),
      mScaleType(stLinear),
      // internal members:
      mRadius(1),  // non-zero initial value, will be overwritten in ::update()
      // according to inner rect
      mTicker(new QCPAxisTicker),
      mLabelPainter(mParentPlot) {
  setParent(parent);
  setAntialiased(true);

  setTickLabelPadding(5);
  setTickLabelRotation(0);
  setTickLabelMode(lmUpright);
  mLabelPainter.setAnchorReferenceType(QCPLabelPainterPrivate::artTangent);
  mLabelPainter.setAbbreviateDecimalPowers(false);
}

QCPPolarAxisRadial::~QCPPolarAxisRadial() {}

QCPPolarAxisRadial::LabelMode QCPPolarAxisRadial::tickLabelMode() const {
  switch (mLabelPainter.anchorMode()) {
    case QCPLabelPainterPrivate::amSkewedUpright:
      return lmUpright;
    case QCPLabelPainterPrivate::amSkewedRotated:
      return lmRotated;
    default:
      qDebug() << Q_FUNC_INFO << "invalid mode for polar axis";
      break;
  }
  return lmUpright;
}

/* No documentation as it is a property getter */
QString QCPPolarAxisRadial::numberFormat() const {
  QString result;
  result.append(mNumberFormatChar);
  if (mNumberBeautifulPowers) {
    result.append(QLatin1Char('b'));
    if (mNumberMultiplyCross) result.append(QLatin1Char('c'));
  }
  return result;
}

/* No documentation as it is a property getter */
int QCPPolarAxisRadial::tickLengthIn() const { return mTickLengthIn; }

/* No documentation as it is a property getter */
int QCPPolarAxisRadial::tickLengthOut() const { return mTickLengthOut; }

/* No documentation as it is a property getter */
int QCPPolarAxisRadial::subTickLengthIn() const { return mSubTickLengthIn; }

/* No documentation as it is a property getter */
int QCPPolarAxisRadial::subTickLengthOut() const { return mSubTickLengthOut; }

/* No documentation as it is a property getter */
int QCPPolarAxisRadial::labelPadding() const { return mLabelPadding; }

void QCPPolarAxisRadial::setRangeDrag(bool enabled) { mRangeDrag = enabled; }

void QCPPolarAxisRadial::setRangeZoom(bool enabled) { mRangeZoom = enabled; }

void QCPPolarAxisRadial::setRangeZoomFactor(double factor) {
  mRangeZoomFactor = factor;
}

/*!
  Sets whether the axis uses a linear scale or a logarithmic scale.

  Note that this method controls the coordinate transformation. For logarithmic
  scales, you will likely also want to use a logarithmic tick spacing and
  labeling, which can be achieved by setting the axis ticker to an instance of
  \ref QCPAxisTickerLog :

  \snippet documentation/doc-code-snippets/mainwindow.cpp
  qcpaxisticker-log-creation

  See the documentation of \ref QCPAxisTickerLog about the details of
  logarithmic axis tick creation.

  \ref setNumberPrecision
*/
void QCPPolarAxisRadial::setScaleType(QCPPolarAxisRadial::ScaleType type) {
  if (mScaleType != type) {
    mScaleType = type;
    if (mScaleType == stLogarithmic) setRange(mRange.sanitizedForLogScale());
    // mCachedMarginValid = false;
    emit scaleTypeChanged(mScaleType);
  }
}

/*!
  Sets the range of the axis.

  This slot may be connected with the \ref rangeChanged signal of another axis
  so this axis is always synchronized with the other axis range, when it
  changes.

  To invert the direction of an axis, use \ref setRangeReversed.
*/
void QCPPolarAxisRadial::setRange(const QCPRange &range) {
  if (range.lower == mRange.lower && range.upper == mRange.upper) return;

  if (!QCPRange::validRange(range)) return;
  QCPRange oldRange = mRange;
  if (mScaleType == stLogarithmic) {
    mRange = range.sanitizedForLogScale();
  } else {
    mRange = range.sanitizedForLinScale();
  }
  emit rangeChanged(mRange);
  emit rangeChanged(mRange, oldRange);
}

/*!
  Sets whether the user can (de-)select the parts in \a selectable by clicking
  on the QCustomPlot surface. (When \ref QCustomPlot::setInteractions contains
  iSelectAxes.)

  However, even when \a selectable is set to a value not allowing the selection
  of a specific part, it is still possible to set the selection of this part
  manually, by calling \ref setSelectedParts directly.

  \see SelectablePart, setSelectedParts
*/
void QCPPolarAxisRadial::setSelectableParts(const SelectableParts &selectable) {
  if (mSelectableParts != selectable) {
    mSelectableParts = selectable;
    emit selectableChanged(mSelectableParts);
  }
}

/*!
  Sets the selected state of the respective axis parts described by \ref
  SelectablePart. When a part is selected, it uses a different pen/font.

  The entire selection mechanism for axes is handled automatically when \ref
  QCustomPlot::setInteractions contains iSelectAxes. You only need to call this
  function when you wish to change the selection state manually.

  This function can change the selection state of a part, independent of the
  \ref setSelectableParts setting.

  emits the \ref selectionChanged signal when \a selected is different from the
  previous selection state.

  \see SelectablePart, setSelectableParts, selectTest, setSelectedBasePen,
  setSelectedTickPen, setSelectedSubTickPen, setSelectedTickLabelFont,
  setSelectedLabelFont, setSelectedTickLabelColor, setSelectedLabelColor
*/
void QCPPolarAxisRadial::setSelectedParts(const SelectableParts &selected) {
  if (mSelectedParts != selected) {
    mSelectedParts = selected;
    emit selectionChanged(mSelectedParts);
  }
}

/*!
  \overload

  Sets the lower and upper bound of the axis range.

  To invert the direction of an axis, use \ref setRangeReversed.

  There is also a slot to set a range, see \ref setRange(const QCPRange &range).
*/
void QCPPolarAxisRadial::setRange(double lower, double upper) {
  if (lower == mRange.lower && upper == mRange.upper) return;

  if (!QCPRange::validRange(lower, upper)) return;
  QCPRange oldRange = mRange;
  mRange.lower = lower;
  mRange.upper = upper;
  if (mScaleType == stLogarithmic) {
    mRange = mRange.sanitizedForLogScale();
  } else {
    mRange = mRange.sanitizedForLinScale();
  }
  emit rangeChanged(mRange);
  emit rangeChanged(mRange, oldRange);
}

/*!
  \overload

  Sets the range of the axis.

  The \a position coordinate indicates together with the \a alignment parameter,
  where the new range will be positioned. \a size defines the size of the new
  axis range. \a alignment may be Qt::AlignLeft, Qt::AlignRight or
  Qt::AlignCenter. This will cause the left border, right border, or center of
  the range to be aligned with \a position. Any other values of \a alignment
  will default to Qt::AlignCenter.
*/
void QCPPolarAxisRadial::setRange(double position, double size,
                                  Qt::AlignmentFlag alignment) {
  if (alignment == Qt::AlignLeft)
    setRange(position, position + size);
  else if (alignment == Qt::AlignRight)
    setRange(position - size, position);
  else  // alignment == Qt::AlignCenter
    setRange(position - size / 2.0, position + size / 2.0);
}

/*!
  Sets the lower bound of the axis range. The upper bound is not changed.
  \see setRange
*/
void QCPPolarAxisRadial::setRangeLower(double lower) {
  if (mRange.lower == lower) return;

  QCPRange oldRange = mRange;
  mRange.lower = lower;
  if (mScaleType == stLogarithmic) {
    mRange = mRange.sanitizedForLogScale();
  } else {
    mRange = mRange.sanitizedForLinScale();
  }
  emit rangeChanged(mRange);
  emit rangeChanged(mRange, oldRange);
}

/*!
  Sets the upper bound of the axis range. The lower bound is not changed.
  \see setRange
*/
void QCPPolarAxisRadial::setRangeUpper(double upper) {
  if (mRange.upper == upper) return;

  QCPRange oldRange = mRange;
  mRange.upper = upper;
  if (mScaleType == stLogarithmic) {
    mRange = mRange.sanitizedForLogScale();
  } else {
    mRange = mRange.sanitizedForLinScale();
  }
  emit rangeChanged(mRange);
  emit rangeChanged(mRange, oldRange);
}

/*!
  Sets whether the axis range (direction) is displayed reversed. Normally, the
  values on horizontal axes increase left to right, on vertical axes bottom to
  top. When \a reversed is set to true, the direction of increasing values is
  inverted.

  Note that the range and data interface stays the same for reversed axes, e.g.
  the \a lower part of the \ref setRange interface will still reference the
  mathematically smaller number than the \a upper part.
*/
void QCPPolarAxisRadial::setRangeReversed(bool reversed) {
  mRangeReversed = reversed;
}

void QCPPolarAxisRadial::setAngle(double degrees) { mAngle = degrees; }

void QCPPolarAxisRadial::setAngleReference(AngleReference reference) {
  mAngleReference = reference;
}

/*!
  The axis ticker is responsible for generating the tick positions and tick
  labels. See the documentation of QCPAxisTicker for details on how to work with
  axis tickers.

  You can change the tick positioning/labeling behaviour of this axis by setting
  a different QCPAxisTicker subclass using this method. If you only wish to
  modify the currently installed axis ticker, access it via \ref ticker.

  Since the ticker is stored in the axis as a shared pointer, multiple axes may
  share the same axis ticker simply by passing the same shared pointer to
  multiple axes.

  \see ticker
*/
void QCPPolarAxisRadial::setTicker(QSharedPointer<QCPAxisTicker> ticker) {
  if (ticker)
    mTicker = ticker;
  else
    qDebug() << Q_FUNC_INFO << "can not set 0 as axis ticker";
  // no need to invalidate margin cache here because produced tick labels are
  // checked for changes in setupTickVector
}

/*!
  Sets whether tick marks are displayed.

  Note that setting \a show to false does not imply that tick labels are
  invisible, too. To achieve that, see \ref setTickLabels.

  \see setSubTicks
*/
void QCPPolarAxisRadial::setTicks(bool show) {
  if (mTicks != show) {
    mTicks = show;
    // mCachedMarginValid = false;
  }
}

/*!
  Sets whether tick labels are displayed. Tick labels are the numbers drawn next
  to tick marks.
*/
void QCPPolarAxisRadial::setTickLabels(bool show) {
  if (mTickLabels != show) {
    mTickLabels = show;
    // mCachedMarginValid = false;
    if (!mTickLabels) mTickVectorLabels.clear();
  }
}

/*!
  Sets the distance between the axis base line (including any outward ticks) and
  the tick labels. \see setLabelPadding, setPadding
*/
void QCPPolarAxisRadial::setTickLabelPadding(int padding) {
  mLabelPainter.setPadding(padding);
}

/*!
  Sets the font of the tick labels.

  \see setTickLabels, setTickLabelColor
*/
void QCPPolarAxisRadial::setTickLabelFont(const QFont &font) {
  if (font != mTickLabelFont) {
    mTickLabelFont = font;
    // mCachedMarginValid = false;
  }
}

/*!
  Sets the color of the tick labels.

  \see setTickLabels, setTickLabelFont
*/
void QCPPolarAxisRadial::setTickLabelColor(const QColor &color) {
  mTickLabelColor = color;
}

/*!
  Sets the rotation of the tick labels. If \a degrees is zero, the labels are
  drawn normally. Else, the tick labels are drawn rotated by \a degrees
  clockwise. The specified angle is bound to values from -90 to 90 degrees.

  If \a degrees is exactly -90, 0 or 90, the tick labels are centered on the
  tick coordinate. For other angles, the label is drawn with an offset such that
  it seems to point toward or away from the tick mark.
*/
void QCPPolarAxisRadial::setTickLabelRotation(double degrees) {
  mLabelPainter.setRotation(degrees);
}

void QCPPolarAxisRadial::setTickLabelMode(LabelMode mode) {
  switch (mode) {
    case lmUpright:
      mLabelPainter.setAnchorMode(QCPLabelPainterPrivate::amSkewedUpright);
      break;
    case lmRotated:
      mLabelPainter.setAnchorMode(QCPLabelPainterPrivate::amSkewedRotated);
      break;
  }
}

/*!
  Sets the number format for the numbers in tick labels. This \a formatCode is
  an extended version of the format code used e.g. by QString::number() and
  QLocale::toString(). For reference about that, see the "Argument Formats"
  section in the detailed description of the QString class.

  \a formatCode is a string of one, two or three characters. The first character
  is identical to the normal format code used by Qt. In short, this means:
  'e'/'E' scientific format, 'f' fixed format, 'g'/'G' scientific or fixed,
  whichever is shorter.

  The second and third characters are optional and specific to QCustomPlot:\n
  If the first char was 'e' or 'g', numbers are/might be displayed in the
  scientific format, e.g. "5.5e9", which is ugly in a plot. So when the second
  char of \a formatCode is set to 'b' (for "beautiful"), those exponential
  numbers are formatted in a more natural way, i.e. "5.5 [multiplication sign]
  10 [superscript] 9". By default, the multiplication sign is a centered dot. If
  instead a cross should be shown (as is usual in the USA), the third char of \a
  formatCode can be set to 'c'. The inserted multiplication signs are the UTF-8
  characters 215 (0xD7) for the cross and 183 (0xB7) for the dot.

  Examples for \a formatCode:
  \li \c g normal format code behaviour. If number is small, fixed format is
  used, if number is large, normal scientific format is used \li \c gb If number
  is small, fixed format is used, if number is large, scientific format is used
  with beautifully typeset decimal powers and a dot as multiplication sign \li
  \c ebc All numbers are in scientific format with beautifully typeset decimal
  power and a cross as multiplication sign \li \c fb illegal format code, since
  fixed format doesn't support (or need) beautifully typeset decimal powers.
  Format code will be reduced to 'f'. \li \c hello illegal format code, since
  first char is not 'e', 'E', 'f', 'g' or 'G'. Current format code will not be
  changed.
*/
void QCPPolarAxisRadial::setNumberFormat(const QString &formatCode) {
  if (formatCode.isEmpty()) {
    qDebug() << Q_FUNC_INFO << "Passed formatCode is empty";
    return;
  }
  // mCachedMarginValid = false;

  // interpret first char as number format char:
  QString allowedFormatChars(QLatin1String("eEfgG"));
  if (allowedFormatChars.contains(formatCode.at(0))) {
    mNumberFormatChar = QLatin1Char(formatCode.at(0).toLatin1());
  } else {
    qDebug() << Q_FUNC_INFO
             << "Invalid number format code (first char not in 'eEfgG'):"
             << formatCode;
    return;
  }

  if (formatCode.length() < 2) {
    mNumberBeautifulPowers = false;
    mNumberMultiplyCross = false;
  } else {
    // interpret second char as indicator for beautiful decimal powers:
    if (formatCode.at(1) == QLatin1Char('b') &&
        (mNumberFormatChar == QLatin1Char('e') ||
         mNumberFormatChar == QLatin1Char('g')))
      mNumberBeautifulPowers = true;
    else
      qDebug() << Q_FUNC_INFO
               << "Invalid number format code (second char not 'b' or first "
                  "char neither 'e' nor 'g'):"
               << formatCode;

    if (formatCode.length() < 3) {
      mNumberMultiplyCross = false;
    } else {
      // interpret third char as indicator for dot or cross multiplication
      // symbol:
      if (formatCode.at(2) == QLatin1Char('c'))
        mNumberMultiplyCross = true;
      else if (formatCode.at(2) == QLatin1Char('d'))
        mNumberMultiplyCross = false;
      else
        qDebug()
            << Q_FUNC_INFO
            << "Invalid number format code (third char neither 'c' nor 'd'):"
            << formatCode;
    }
  }
  mLabelPainter.setSubstituteExponent(mNumberBeautifulPowers);
  mLabelPainter.setMultiplicationSymbol(
      mNumberMultiplyCross ? QCPLabelPainterPrivate::SymbolCross
                           : QCPLabelPainterPrivate::SymbolDot);
}

/*!
  Sets the precision of the tick label numbers. See QLocale::toString(double i,
  char f, int prec) for details. The effect of precisions are most notably for
  number Formats starting with 'e', see \ref setNumberFormat
*/
void QCPPolarAxisRadial::setNumberPrecision(int precision) {
  if (mNumberPrecision != precision) {
    mNumberPrecision = precision;
    // mCachedMarginValid = false;
  }
}

/*!
  Sets the length of the ticks in pixels. \a inside is the length the ticks will
  reach inside the plot and \a outside is the length they will reach outside the
  plot. If \a outside is greater than zero, the tick labels and axis label will
  increase their distance to the axis accordingly, so they won't collide with
  the ticks.

  \see setSubTickLength, setTickLengthIn, setTickLengthOut
*/
void QCPPolarAxisRadial::setTickLength(int inside, int outside) {
  setTickLengthIn(inside);
  setTickLengthOut(outside);
}

/*!
  Sets the length of the inward ticks in pixels. \a inside is the length the
  ticks will reach inside the plot.

  \see setTickLengthOut, setTickLength, setSubTickLength
*/
void QCPPolarAxisRadial::setTickLengthIn(int inside) {
  if (mTickLengthIn != inside) {
    mTickLengthIn = inside;
  }
}

/*!
  Sets the length of the outward ticks in pixels. \a outside is the length the
  ticks will reach outside the plot. If \a outside is greater than zero, the
  tick labels and axis label will increase their distance to the axis
  accordingly, so they won't collide with the ticks.

  \see setTickLengthIn, setTickLength, setSubTickLength
*/
void QCPPolarAxisRadial::setTickLengthOut(int outside) {
  if (mTickLengthOut != outside) {
    mTickLengthOut = outside;
    // mCachedMarginValid = false; // only outside tick length can change margin
  }
}

/*!
  Sets whether sub tick marks are displayed.

  Sub ticks are only potentially visible if (major) ticks are also visible (see
  \ref setTicks)

  \see setTicks
*/
void QCPPolarAxisRadial::setSubTicks(bool show) {
  if (mSubTicks != show) {
    mSubTicks = show;
    // mCachedMarginValid = false;
  }
}

/*!
  Sets the length of the subticks in pixels. \a inside is the length the
  subticks will reach inside the plot and \a outside is the length they will
  reach outside the plot. If \a outside is greater than zero, the tick labels
  and axis label will increase their distance to the axis accordingly, so they
  won't collide with the ticks.

  \see setTickLength, setSubTickLengthIn, setSubTickLengthOut
*/
void QCPPolarAxisRadial::setSubTickLength(int inside, int outside) {
  setSubTickLengthIn(inside);
  setSubTickLengthOut(outside);
}

/*!
  Sets the length of the inward subticks in pixels. \a inside is the length the
  subticks will reach inside the plot.

  \see setSubTickLengthOut, setSubTickLength, setTickLength
*/
void QCPPolarAxisRadial::setSubTickLengthIn(int inside) {
  if (mSubTickLengthIn != inside) {
    mSubTickLengthIn = inside;
  }
}

/*!
  Sets the length of the outward subticks in pixels. \a outside is the length
  the subticks will reach outside the plot. If \a outside is greater than zero,
  the tick labels will increase their distance to the axis accordingly, so they
  won't collide with the ticks.

  \see setSubTickLengthIn, setSubTickLength, setTickLength
*/
void QCPPolarAxisRadial::setSubTickLengthOut(int outside) {
  if (mSubTickLengthOut != outside) {
    mSubTickLengthOut = outside;
    // mCachedMarginValid = false; // only outside tick length can change margin
  }
}

/*!
  Sets the pen, the axis base line is drawn with.

  \see setTickPen, setSubTickPen
*/
void QCPPolarAxisRadial::setBasePen(const QPen &pen) { mBasePen = pen; }

/*!
  Sets the pen, tick marks will be drawn with.

  \see setTickLength, setBasePen
*/
void QCPPolarAxisRadial::setTickPen(const QPen &pen) { mTickPen = pen; }

/*!
  Sets the pen, subtick marks will be drawn with.

  \see setSubTickCount, setSubTickLength, setBasePen
*/
void QCPPolarAxisRadial::setSubTickPen(const QPen &pen) { mSubTickPen = pen; }

/*!
  Sets the font of the axis label.

  \see setLabelColor
*/
void QCPPolarAxisRadial::setLabelFont(const QFont &font) {
  if (mLabelFont != font) {
    mLabelFont = font;
    // mCachedMarginValid = false;
  }
}

/*!
  Sets the color of the axis label.

  \see setLabelFont
*/
void QCPPolarAxisRadial::setLabelColor(const QColor &color) {
  mLabelColor = color;
}

/*!
  Sets the text of the axis label that will be shown below/above or next to the
  axis, depending on its orientation. To disable axis labels, pass an empty
  string as \a str.
*/
void QCPPolarAxisRadial::setLabel(const QString &str) {
  if (mLabel != str) {
    mLabel = str;
    // mCachedMarginValid = false;
  }
}

/*!
  Sets the distance between the tick labels and the axis label.

  \see setTickLabelPadding, setPadding
*/
void QCPPolarAxisRadial::setLabelPadding(int padding) {
  if (mLabelPadding != padding) {
    mLabelPadding = padding;
    // mCachedMarginValid = false;
  }
}

/*!
  Sets the font that is used for tick labels when they are selected.

  \see setTickLabelFont, setSelectableParts, setSelectedParts,
  QCustomPlot::setInteractions
*/
void QCPPolarAxisRadial::setSelectedTickLabelFont(const QFont &font) {
  if (font != mSelectedTickLabelFont) {
    mSelectedTickLabelFont = font;
    // don't set mCachedMarginValid to false here because margin calculation is
    // always done with non-selected fonts
  }
}

/*!
  Sets the font that is used for the axis label when it is selected.

  \see setLabelFont, setSelectableParts, setSelectedParts,
  QCustomPlot::setInteractions
*/
void QCPPolarAxisRadial::setSelectedLabelFont(const QFont &font) {
  mSelectedLabelFont = font;
  // don't set mCachedMarginValid to false here because margin calculation is
  // always done with non-selected fonts
}

/*!
  Sets the color that is used for tick labels when they are selected.

  \see setTickLabelColor, setSelectableParts, setSelectedParts,
  QCustomPlot::setInteractions
*/
void QCPPolarAxisRadial::setSelectedTickLabelColor(const QColor &color) {
  if (color != mSelectedTickLabelColor) {
    mSelectedTickLabelColor = color;
  }
}

/*!
  Sets the color that is used for the axis label when it is selected.

  \see setLabelColor, setSelectableParts, setSelectedParts,
  QCustomPlot::setInteractions
*/
void QCPPolarAxisRadial::setSelectedLabelColor(const QColor &color) {
  mSelectedLabelColor = color;
}

/*!
  Sets the pen that is used to draw the axis base line when selected.

  \see setBasePen, setSelectableParts, setSelectedParts,
  QCustomPlot::setInteractions
*/
void QCPPolarAxisRadial::setSelectedBasePen(const QPen &pen) {
  mSelectedBasePen = pen;
}

/*!
  Sets the pen that is used to draw the (major) ticks when selected.

  \see setTickPen, setSelectableParts, setSelectedParts,
  QCustomPlot::setInteractions
*/
void QCPPolarAxisRadial::setSelectedTickPen(const QPen &pen) {
  mSelectedTickPen = pen;
}

/*!
  Sets the pen that is used to draw the subticks when selected.

  \see setSubTickPen, setSelectableParts, setSelectedParts,
  QCustomPlot::setInteractions
*/
void QCPPolarAxisRadial::setSelectedSubTickPen(const QPen &pen) {
  mSelectedSubTickPen = pen;
}

/*!
  If the scale type (\ref setScaleType) is \ref stLinear, \a diff is added to
  the lower and upper bounds of the range. The range is simply moved by \a diff.

  If the scale type is \ref stLogarithmic, the range bounds are multiplied by \a
  diff. This corresponds to an apparent "linear" move in logarithmic scaling by
  a distance of log(diff).
*/
void QCPPolarAxisRadial::moveRange(double diff) {
  QCPRange oldRange = mRange;
  if (mScaleType == stLinear) {
    mRange.lower += diff;
    mRange.upper += diff;
  } else  // mScaleType == stLogarithmic
  {
    mRange.lower *= diff;
    mRange.upper *= diff;
  }
  emit rangeChanged(mRange);
  emit rangeChanged(mRange, oldRange);
}

/*!
  Scales the range of this axis by \a factor around the center of the current
  axis range. For example, if \a factor is 2.0, then the axis range will double
  its size, and the point at the axis range center won't have changed its
  position in the QCustomPlot widget (i.e. coordinates around the center will
  have moved symmetrically closer).

  If you wish to scale around a different coordinate than the current axis range
  center, use the overload \ref scaleRange(double factor, double center).
*/
void QCPPolarAxisRadial::scaleRange(double factor) {
  scaleRange(factor, range().center());
}

/*! \overload

  Scales the range of this axis by \a factor around the coordinate \a center.
  For example, if \a factor is 2.0, \a center is 1.0, then the axis range will
  double its size, and the point at coordinate 1.0 won't have changed its
  position in the QCustomPlot widget (i.e. coordinates around 1.0 will have
  moved symmetrically closer to 1.0).

  \see scaleRange(double factor)
*/
void QCPPolarAxisRadial::scaleRange(double factor, double center) {
  QCPRange oldRange = mRange;
  if (mScaleType == stLinear) {
    QCPRange newRange;
    newRange.lower = (mRange.lower - center) * factor + center;
    newRange.upper = (mRange.upper - center) * factor + center;
    if (QCPRange::validRange(newRange))
      mRange = newRange.sanitizedForLinScale();
  } else  // mScaleType == stLogarithmic
  {
    if ((mRange.upper < 0 && center < 0) ||
        (mRange.upper > 0 &&
         center > 0))  // make sure center has same sign as range
    {
      QCPRange newRange;
      newRange.lower = qPow(mRange.lower / center, factor) * center;
      newRange.upper = qPow(mRange.upper / center, factor) * center;
      if (QCPRange::validRange(newRange))
        mRange = newRange.sanitizedForLogScale();
    } else
      qDebug() << Q_FUNC_INFO
               << "Center of scaling operation doesn't lie in same logarithmic "
                  "sign domain as range:"
               << center;
  }
  emit rangeChanged(mRange);
  emit rangeChanged(mRange, oldRange);
}

/*!
  Changes the axis range such that all plottables associated with this axis are
  fully visible in that dimension.

  \see QCPAbstractPlottable::rescaleAxes, QCustomPlot::rescaleAxes
*/
void QCPPolarAxisRadial::rescale(bool onlyVisiblePlottables) {
  Q_UNUSED(onlyVisiblePlottables)
  /* TODO
QList<QCPAbstractPlottable*> p = plottables();
QCPRange newRange;
bool haveRange = false;
for (int i=0; i<p.size(); ++i)
{
  if (!p.at(i)->realVisibility() && onlyVisiblePlottables)
    continue;
  QCPRange plottableRange;
  bool currentFoundRange;
  QCP::SignDomain signDomain = QCP::sdBoth;
  if (mScaleType == stLogarithmic)
    signDomain = (mRange.upper < 0 ? QCP::sdNegative : QCP::sdPositive);
  if (p.at(i)->keyAxis() == this)
    plottableRange = p.at(i)->getKeyRange(currentFoundRange, signDomain);
  else
    plottableRange = p.at(i)->getValueRange(currentFoundRange, signDomain);
  if (currentFoundRange)
  {
    if (!haveRange)
      newRange = plottableRange;
    else
      newRange.expand(plottableRange);
    haveRange = true;
  }
}
if (haveRange)
{
  if (!QCPRange::validRange(newRange)) // likely due to range being zero
(plottable has only constant data in this axis dimension), shift current range
to at least center the plottable
  {
    double center = (newRange.lower+newRange.upper)*0.5; // upper and lower
should be equal anyway, but just to make sure, incase validRange returned
false for other reason if (mScaleType == stLinear)
    {
      newRange.lower = center-mRange.size()/2.0;
      newRange.upper = center+mRange.size()/2.0;
    } else // mScaleType == stLogarithmic
    {
      newRange.lower = center/qSqrt(mRange.upper/mRange.lower);
      newRange.upper = center*qSqrt(mRange.upper/mRange.lower);
    }
  }
  setRange(newRange);
}
*/
}

/*!
  Transforms \a value, in pixel coordinates of the QCustomPlot widget, to axis
  coordinates.
*/
void QCPPolarAxisRadial::pixelToCoord(QPointF pixelPos, double &angleCoord,
                                      double &radiusCoord) const {
  QCPVector2D posVector(pixelPos - mCenter);
  radiusCoord = radiusToCoord(posVector.length());
  angleCoord = mAngularAxis->angleRadToCoord(posVector.angle());
}

/*!
  Transforms \a value, in coordinates of the axis, to pixel coordinates of the
  QCustomPlot widget.
*/
QPointF QCPPolarAxisRadial::coordToPixel(double angleCoord,
                                         double radiusCoord) const {
  const double radiusPixel = coordToRadius(radiusCoord);
  const double angleRad = mAngularAxis->coordToAngleRad(angleCoord);
  return QPointF(mCenter.x() + qCos(angleRad) * radiusPixel,
                 mCenter.y() + qSin(angleRad) * radiusPixel);
}

double QCPPolarAxisRadial::coordToRadius(double coord) const {
  if (mScaleType == stLinear) {
    if (!mRangeReversed)
      return (coord - mRange.lower) / mRange.size() * mRadius;
    else
      return (mRange.upper - coord) / mRange.size() * mRadius;
  } else  // mScaleType == stLogarithmic
  {
    if (coord >= 0.0 &&
        mRange.upper < 0.0)  // invalid value for logarithmic scale, just return
      // outside visible range
      return !mRangeReversed ? mRadius + 200 : mRadius - 200;
    else if (coord <= 0.0 &&
             mRange.upper >= 0.0)  // invalid value for logarithmic scale, just
      // return outside visible range
      return !mRangeReversed ? mRadius - 200 : mRadius + 200;
    else {
      if (!mRangeReversed)
        return qLn(coord / mRange.lower) / qLn(mRange.upper / mRange.lower) *
               mRadius;
      else
        return qLn(mRange.upper / coord) / qLn(mRange.upper / mRange.lower) *
               mRadius;
    }
  }
}

double QCPPolarAxisRadial::radiusToCoord(double radius) const {
  if (mScaleType == stLinear) {
    if (!mRangeReversed)
      return (radius) / mRadius * mRange.size() + mRange.lower;
    else
      return -(radius) / mRadius * mRange.size() + mRange.upper;
  } else  // mScaleType == stLogarithmic
  {
    if (!mRangeReversed)
      return qPow(mRange.upper / mRange.lower, (radius) / mRadius) *
             mRange.lower;
    else
      return qPow(mRange.upper / mRange.lower, (-radius) / mRadius) *
             mRange.upper;
  }
}

/*!
  Returns the part of the axis that is hit by \a pos (in pixels). The return
  value of this function is independent of the user-selectable parts defined
  with \ref setSelectableParts. Further, this function does not change the
  current selection state of the axis.

  If the axis is not visible (\ref setVisible), this function always returns
  \ref spNone.

  \see setSelectedParts, setSelectableParts, QCustomPlot::setInteractions
*/
QCPPolarAxisRadial::SelectablePart QCPPolarAxisRadial::getPartAt(
    const QPointF &pos) const {
  Q_UNUSED(pos)  // TODO remove later
  if (!mVisible) return spNone;

  /*
  TODO:
if (mAxisPainter->axisSelectionBox().contains(pos.toPoint()))
  return spAxis;
else if (mAxisPainter->tickLabelsSelectionBox().contains(pos.toPoint()))
  return spTickLabels;
else if (mAxisPainter->labelSelectionBox().contains(pos.toPoint()))
  return spAxisLabel;
else */
  return spNone;
}

/* inherits documentation from base class */
double QCPPolarAxisRadial::selectTest(const QPointF &pos, bool onlySelectable,
                                      QVariant *details) const {
  if (!mParentPlot) return -1;
  SelectablePart part = getPartAt(pos);
  if ((onlySelectable && !mSelectableParts.testFlag(part)) || part == spNone)
    return -1;

  if (details) details->setValue(part);
  return mParentPlot->selectionTolerance() * 0.99;
}

/* inherits documentation from base class */
void QCPPolarAxisRadial::selectEvent(QMouseEvent *event, bool additive,
                                     const QVariant &details,
                                     bool *selectionStateChanged) {
  Q_UNUSED(event)
  SelectablePart part = details.value<SelectablePart>();
  if (mSelectableParts.testFlag(part)) {
    SelectableParts selBefore = mSelectedParts;
    setSelectedParts(additive ? mSelectedParts ^ part : part);
    if (selectionStateChanged)
      *selectionStateChanged = mSelectedParts != selBefore;
  }
}

/* inherits documentation from base class */
void QCPPolarAxisRadial::deselectEvent(bool *selectionStateChanged) {
  SelectableParts selBefore = mSelectedParts;
  setSelectedParts(mSelectedParts & ~mSelectableParts);
  if (selectionStateChanged)
    *selectionStateChanged = mSelectedParts != selBefore;
}

/*! \internal

  This mouse event reimplementation provides the functionality to let the user
  drag individual axes exclusively, by startig the drag on top of the axis.

  For the axis to accept this event and perform the single axis drag, the parent
  \ref QCPAxisRect must be configured accordingly, i.e. it must allow range
  dragging in the orientation of this axis
  (\ref QCPAxisRect::setRangeDrag) and this axis must be a draggable axis (\ref
  QCPAxisRect::setRangeDragAxes)

  \seebaseclassmethod

  \note The dragging of possibly multiple axes at once by starting the drag
  anywhere in the axis rect is handled by the axis rect's mouse event, e.g. \ref
  QCPAxisRect::mousePressEvent.
*/
void QCPPolarAxisRadial::mousePressEvent(QMouseEvent *event,
                                         const QVariant &details) {
  Q_UNUSED(details)
  if (!mParentPlot->interactions().testFlag(QCP::iRangeDrag)) {
    event->ignore();
    return;
  }

  if (event->buttons() & Qt::LeftButton) {
    mDragging = true;
    // initialize antialiasing backup in case we start dragging:
    if (mParentPlot->noAntialiasingOnDrag()) {
      mAADragBackup = mParentPlot->antialiasedElements();
      mNotAADragBackup = mParentPlot->notAntialiasedElements();
    }
    // Mouse range dragging interaction:
    if (mParentPlot->interactions().testFlag(QCP::iRangeDrag))
      mDragStartRange = mRange;
  }
}

/*! \internal

  This mouse event reimplementation provides the functionality to let the user
  drag individual axes exclusively, by startig the drag on top of the axis.

  \seebaseclassmethod

  \note The dragging of possibly multiple axes at once by starting the drag
  anywhere in the axis rect is handled by the axis rect's mouse event, e.g. \ref
  QCPAxisRect::mousePressEvent.

  \see QCPAxis::mousePressEvent
*/
void QCPPolarAxisRadial::mouseMoveEvent(QMouseEvent *event,
                                        const QPointF &startPos) {
  Q_UNUSED(event)     // TODO remove later
  Q_UNUSED(startPos)  // TODO remove later
  if (mDragging) {
    /* TODO
const double startPixel = orientation() == Qt::Horizontal ? startPos.x() :
startPos.y(); const double currentPixel = orientation() == Qt::Horizontal ?
event->pos().x() : event->pos().y(); if (mScaleType ==
QCPPolarAxisRadial::stLinear)
{
  const double diff = pixelToCoord(startPixel) - pixelToCoord(currentPixel);
  setRange(mDragStartRange.lower+diff, mDragStartRange.upper+diff);
} else if (mScaleType == QCPPolarAxisRadial::stLogarithmic)
{
  const double diff = pixelToCoord(startPixel) / pixelToCoord(currentPixel);
  setRange(mDragStartRange.lower*diff, mDragStartRange.upper*diff);
}
*/

    if (mParentPlot->noAntialiasingOnDrag())
      mParentPlot->setNotAntialiasedElements(QCP::aeAll);
    mParentPlot->replot(QCustomPlot::rpQueuedReplot);
  }
}

/*! \internal

  This mouse event reimplementation provides the functionality to let the user
  drag individual axes exclusively, by startig the drag on top of the axis.

  \seebaseclassmethod

  \note The dragging of possibly multiple axes at once by starting the drag
  anywhere in the axis rect is handled by the axis rect's mouse event, e.g. \ref
  QCPAxisRect::mousePressEvent.

  \see QCPAxis::mousePressEvent
*/
void QCPPolarAxisRadial::mouseReleaseEvent(QMouseEvent *event,
                                           const QPointF &startPos) {
  Q_UNUSED(event)
  Q_UNUSED(startPos)
  mDragging = false;
  if (mParentPlot->noAntialiasingOnDrag()) {
    mParentPlot->setAntialiasedElements(mAADragBackup);
    mParentPlot->setNotAntialiasedElements(mNotAADragBackup);
  }
}

/*! \internal

  This mouse event reimplementation provides the functionality to let the user
  zoom individual axes exclusively, by performing the wheel event on top of the
  axis.

  For the axis to accept this event and perform the single axis zoom, the parent
  \ref QCPAxisRect must be configured accordingly, i.e. it must allow range
  zooming in the orientation of this axis
  (\ref QCPAxisRect::setRangeZoom) and this axis must be a zoomable axis (\ref
  QCPAxisRect::setRangeZoomAxes)

  \seebaseclassmethod

  \note The zooming of possibly multiple axes at once by performing the wheel
  event anywhere in the axis rect is handled by the axis rect's mouse event,
  e.g. \ref QCPAxisRect::wheelEvent.
*/
void QCPPolarAxisRadial::wheelEvent(QWheelEvent *event) {
  // Mouse range zooming interaction:
  if (!mParentPlot->interactions().testFlag(QCP::iRangeZoom)) {
    event->ignore();
    return;
  }

  // TODO:
  // const double wheelSteps = event->delta()/120.0; // a single step delta is
  // +/-120 usually const double factor = qPow(mRangeZoomFactor, wheelSteps);
  // scaleRange(factor, pixelToCoord(orientation() == Qt::Horizontal ?
  // event->pos().x() : event->pos().y()));
  mParentPlot->replot();
}

void QCPPolarAxisRadial::updateGeometry(const QPointF &center, double radius) {
  mCenter = center;
  mRadius = radius;
  if (mRadius < 1) mRadius = 1;
}

/*! \internal

  A convenience function to easily set the QPainter::Antialiased hint on the
  provided \a painter before drawing axis lines.

  This is the antialiasing state the painter passed to the \ref draw method is
  in by default.

  This function takes into account the local setting of the antialiasing flag as
  well as the overrides set with \ref QCustomPlot::setAntialiasedElements and
  \ref QCustomPlot::setNotAntialiasedElements.

  \seebaseclassmethod

  \see setAntialiased
*/
void QCPPolarAxisRadial::applyDefaultAntialiasingHint(
    QCPPainter *painter) const {
  applyAntialiasingHint(painter, mAntialiased, QCP::aeAxes);
}

/*! \internal

  Draws the axis with the specified \a painter, using the internal
  QCPAxisPainterPrivate instance.

  \seebaseclassmethod
*/
void QCPPolarAxisRadial::draw(QCPPainter *painter) {
  const double axisAngleRad =
      (mAngle +
       (mAngleReference == arAngularAxis ? mAngularAxis->angle() : 0)) /
      180.0 * M_PI;
  const QPointF axisVector(
      qCos(axisAngleRad),
      qSin(axisAngleRad));  // semantically should be QCPVector2D, but we save
                            // time in loops when we keep it as QPointF
  const QPointF tickNormal =
      QCPVector2D(axisVector)
          .perpendicular()
          .toPointF();  // semantically should be QCPVector2D, but we save time
                        // in loops when we keep it as QPointF

  // draw baseline:
  painter->setPen(getBasePen());
  painter->drawLine(QLineF(mCenter, mCenter + axisVector * (mRadius - 0.5)));

  // draw subticks:
  if (!mSubTickVector.isEmpty()) {
    painter->setPen(getSubTickPen());
    for (int i = 0; i < mSubTickVector.size(); ++i) {
      const QPointF tickPosition =
          mCenter + axisVector * coordToRadius(mSubTickVector.at(i));
      painter->drawLine(QLineF(tickPosition - tickNormal * mSubTickLengthIn,
                               tickPosition + tickNormal * mSubTickLengthOut));
    }
  }

  // draw ticks and labels:
  if (!mTickVector.isEmpty()) {
    mLabelPainter.setAnchorReference(
        mCenter - axisVector);  // subtract (normalized) axisVector, just to
                                // prevent degenerate tangents for tick label at
                                // exact lower axis range
    mLabelPainter.setFont(getTickLabelFont());
    mLabelPainter.setColor(getTickLabelColor());
    const QPen ticksPen = getTickPen();
    painter->setPen(ticksPen);
    for (int i = 0; i < mTickVector.size(); ++i) {
      const double r = coordToRadius(mTickVector.at(i));
      const QPointF tickPosition = mCenter + axisVector * r;
      painter->drawLine(QLineF(tickPosition - tickNormal * mTickLengthIn,
                               tickPosition + tickNormal * mTickLengthOut));
      // possibly draw tick labels:
      if (!mTickVectorLabels.isEmpty()) {
        if ((!mRangeReversed &&
             (i < mTickVectorLabels.count() - 1 || mRadius - r > 10)) ||
            (mRangeReversed &&
             (i > 0 || mRadius - r > 10)))  // skip last label if it's closer
          // than 10 pixels to angular axis
          mLabelPainter.drawTickLabel(
              painter, tickPosition + tickNormal * mSubTickLengthOut,
              mTickVectorLabels.at(i));
      }
    }
  }
}

/*! \internal

  Prepares the internal tick vector, sub tick vector and tick label vector. This
  is done by calling QCPAxisTicker::generate on the currently installed ticker.

  If a change in the label text/count is detected, the cached axis margin is
  invalidated to make sure the next margin calculation recalculates the label
  sizes and returns an up-to-date value.
*/
void QCPPolarAxisRadial::setupTickVectors() {
  if (!mParentPlot) return;
  if ((!mTicks && !mTickLabels) || mRange.size() <= 0) return;

  mTicker->generate(mRange, mParentPlot->locale(), mNumberFormatChar,
                    mNumberPrecision, mTickVector,
                    mSubTicks ? &mSubTickVector : 0,
                    mTickLabels ? &mTickVectorLabels : 0);
}

/*! \internal

  Returns the pen that is used to draw the axis base line. Depending on the
  selection state, this is either mSelectedBasePen or mBasePen.
*/
QPen QCPPolarAxisRadial::getBasePen() const {
  return mSelectedParts.testFlag(spAxis) ? mSelectedBasePen : mBasePen;
}

/*! \internal

  Returns the pen that is used to draw the (major) ticks. Depending on the
  selection state, this is either mSelectedTickPen or mTickPen.
*/
QPen QCPPolarAxisRadial::getTickPen() const {
  return mSelectedParts.testFlag(spAxis) ? mSelectedTickPen : mTickPen;
}

/*! \internal

  Returns the pen that is used to draw the subticks. Depending on the selection
  state, this is either mSelectedSubTickPen or mSubTickPen.
*/
QPen QCPPolarAxisRadial::getSubTickPen() const {
  return mSelectedParts.testFlag(spAxis) ? mSelectedSubTickPen : mSubTickPen;
}

/*! \internal

  Returns the font that is used to draw the tick labels. Depending on the
  selection state, this is either mSelectedTickLabelFont or mTickLabelFont.
*/
QFont QCPPolarAxisRadial::getTickLabelFont() const {
  return mSelectedParts.testFlag(spTickLabels) ? mSelectedTickLabelFont
                                               : mTickLabelFont;
}

/*! \internal

  Returns the font that is used to draw the axis label. Depending on the
  selection state, this is either mSelectedLabelFont or mLabelFont.
*/
QFont QCPPolarAxisRadial::getLabelFont() const {
  return mSelectedParts.testFlag(spAxisLabel) ? mSelectedLabelFont : mLabelFont;
}

/*! \internal

  Returns the color that is used to draw the tick labels. Depending on the
  selection state, this is either mSelectedTickLabelColor or mTickLabelColor.
*/
QColor QCPPolarAxisRadial::getTickLabelColor() const {
  return mSelectedParts.testFlag(spTickLabels) ? mSelectedTickLabelColor
                                               : mTickLabelColor;
}

/*! \internal

  Returns the color that is used to draw the axis label. Depending on the
  selection state, this is either mSelectedLabelColor or mLabelColor.
*/
QColor QCPPolarAxisRadial::getLabelColor() const {
  return mSelectedParts.testFlag(spAxisLabel) ? mSelectedLabelColor
                                              : mLabelColor;
}

/* inherits documentation from base class */
QCP::Interaction QCPPolarAxisRadial::selectionCategory() const {
  return QCP::iSelectAxes;
}
/* end of 'src/polar/radialaxis.cpp' */

/* including file 'src/polar/layoutelement-angularaxis.cpp' */
/* modified 2022-11-06T12:45:57, size 57266                 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPPolarAxisAngular
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPPolarAxisAngular
  \brief The main container for polar plots, representing the angular axis as a
  circle

  \warning In this QCustomPlot version, polar plots are a tech preview. Expect
  documentation and functionality to be incomplete, as well as changing public
  interfaces in the future.
*/

/* start documentation of inline functions */

/*! \fn QCPLayoutInset *QCPPolarAxisAngular::insetLayout() const

  Returns the inset layout of this axis rect. It can be used to place other
  layout elements (or even layouts with multiple other elements) inside/on top
  of an axis rect.

  \see QCPLayoutInset
*/

/*! \fn int QCPPolarAxisAngular::left() const

  Returns the pixel position of the left border of this axis rect. Margins are
  not taken into account here, so the returned value is with respect to the
  inner \ref rect.
*/

/*! \fn int QCPPolarAxisAngular::right() const

  Returns the pixel position of the right border of this axis rect. Margins are
  not taken into account here, so the returned value is with respect to the
  inner \ref rect.
*/

/*! \fn int QCPPolarAxisAngular::top() const

  Returns the pixel position of the top border of this axis rect. Margins are
  not taken into account here, so the returned value is with respect to the
  inner \ref rect.
*/

/*! \fn int QCPPolarAxisAngular::bottom() const

  Returns the pixel position of the bottom border of this axis rect. Margins are
  not taken into account here, so the returned value is with respect to the
  inner \ref rect.
*/

/*! \fn int QCPPolarAxisAngular::width() const

  Returns the pixel width of this axis rect. Margins are not taken into account
  here, so the returned value is with respect to the inner \ref rect.
*/

/*! \fn int QCPPolarAxisAngular::height() const

  Returns the pixel height of this axis rect. Margins are not taken into account
  here, so the returned value is with respect to the inner \ref rect.
*/

/*! \fn QSize QCPPolarAxisAngular::size() const

  Returns the pixel size of this axis rect. Margins are not taken into account
  here, so the returned value is with respect to the inner \ref rect.
*/

/*! \fn QPoint QCPPolarAxisAngular::topLeft() const

  Returns the top left corner of this axis rect in pixels. Margins are not taken
  into account here, so the returned value is with respect to the inner \ref
  rect.
*/

/*! \fn QPoint QCPPolarAxisAngular::topRight() const

  Returns the top right corner of this axis rect in pixels. Margins are not
  taken into account here, so the returned value is with respect to the inner
  \ref rect.
*/

/*! \fn QPoint QCPPolarAxisAngular::bottomLeft() const

  Returns the bottom left corner of this axis rect in pixels. Margins are not
  taken into account here, so the returned value is with respect to the inner
  \ref rect.
*/

/*! \fn QPoint QCPPolarAxisAngular::bottomRight() const

  Returns the bottom right corner of this axis rect in pixels. Margins are not
  taken into account here, so the returned value is with respect to the inner
  \ref rect.
*/

/*! \fn QPoint QCPPolarAxisAngular::center() const

  Returns the center of this axis rect in pixels. Margins are not taken into
  account here, so the returned value is with respect to the inner \ref rect.
*/

/* end documentation of inline functions */

/*!
  Creates a QCPPolarAxis instance and sets default values. An axis is added for
  each of the four sides, the top and right axes are set invisible initially.
*/
QCPPolarAxisAngular::QCPPolarAxisAngular(QCustomPlot *parentPlot)
    : QCPLayoutElement(parentPlot),
      mBackgroundBrush(Qt::NoBrush),
      mBackgroundScaled(true),
      mBackgroundScaledMode(Qt::KeepAspectRatioByExpanding),
      mInsetLayout(new QCPLayoutInset),
      mRangeDrag(false),
      mRangeZoom(false),
      mRangeZoomFactor(0.85),
      // axis base:
      mAngle(-90),
      mAngleRad(mAngle / 180.0 * M_PI),
      mSelectableParts(spAxis | spTickLabels | spAxisLabel),
      mSelectedParts(spNone),
      mBasePen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
      mSelectedBasePen(QPen(Qt::blue, 2)),
      // axis label:
      mLabelPadding(0),
      mLabel(),
      mLabelFont(mParentPlot->font()),
      mSelectedLabelFont(
          QFont(mLabelFont.family(), mLabelFont.pointSize(), QFont::Bold)),
      mLabelColor(Qt::black),
      mSelectedLabelColor(Qt::blue),
      // tick labels:
      // mTickLabelPadding(0), in label painter
      mTickLabels(true),
      // mTickLabelRotation(0), in label painter
      mTickLabelFont(mParentPlot->font()),
      mSelectedTickLabelFont(QFont(mTickLabelFont.family(),
                                   mTickLabelFont.pointSize(), QFont::Bold)),
      mTickLabelColor(Qt::black),
      mSelectedTickLabelColor(Qt::blue),
      mNumberPrecision(6),
      mNumberFormatChar('g'),
      mNumberBeautifulPowers(true),
      mNumberMultiplyCross(false),
      // ticks and subticks:
      mTicks(true),
      mSubTicks(true),
      mTickLengthIn(5),
      mTickLengthOut(0),
      mSubTickLengthIn(2),
      mSubTickLengthOut(0),
      mTickPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
      mSelectedTickPen(QPen(Qt::blue, 2)),
      mSubTickPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
      mSelectedSubTickPen(QPen(Qt::blue, 2)),
      // scale and range:
      mRange(0, 360),
      mRangeReversed(false),
      // internal members:
      mRadius(1),  // non-zero initial value, will be overwritten in ::update()
      // according to inner rect
      mGrid(new QCPPolarGrid(this)),
      mTicker(new QCPAxisTickerFixed),
      mDragging(false),
      mLabelPainter(parentPlot) {
  // TODO:
  // mInsetLayout->initializeParentPlot(mParentPlot);
  // mInsetLayout->setParentLayerable(this);
  // mInsetLayout->setParent(this);

  if (QCPAxisTickerFixed *fixedTicker =
          mTicker.dynamicCast<QCPAxisTickerFixed>().data()) {
    fixedTicker->setTickStep(30);
  }
  setAntialiased(true);
  setLayer(
      mParentPlot->currentLayer());  // it's actually on that layer already, but
                                     // we want it in front of the grid, so we
                                     // place it on there again

  setTickLabelPadding(5);
  setTickLabelRotation(0);
  setTickLabelMode(lmUpright);
  mLabelPainter.setAnchorReferenceType(QCPLabelPainterPrivate::artNormal);
  mLabelPainter.setAbbreviateDecimalPowers(false);
  mLabelPainter.setCacheSize(
      24);  // so we can cache up to 15-degree intervals, polar angular axis
            // uses a bit larger cache than normal axes

  setMinimumSize(50, 50);
  setMinimumMargins(QMargins(30, 30, 30, 30));

  addRadialAxis();
  mGrid->setRadialAxis(radialAxis());
}

QCPPolarAxisAngular::~QCPPolarAxisAngular() {
  delete mGrid;  // delete grid here instead of via parent ~QObject for better
                 // defined deletion order
  mGrid = 0;

  delete mInsetLayout;
  mInsetLayout = 0;

  QList<QCPPolarAxisRadial *> radialAxesList = radialAxes();
  for (int i = 0; i < radialAxesList.size(); ++i)
    removeRadialAxis(radialAxesList.at(i));
}

QCPPolarAxisAngular::LabelMode QCPPolarAxisAngular::tickLabelMode() const {
  switch (mLabelPainter.anchorMode()) {
    case QCPLabelPainterPrivate::amSkewedUpright:
      return lmUpright;
    case QCPLabelPainterPrivate::amSkewedRotated:
      return lmRotated;
    default:
      qDebug() << Q_FUNC_INFO << "invalid mode for polar axis";
      break;
  }
  return lmUpright;
}

/* No documentation as it is a property getter */
QString QCPPolarAxisAngular::numberFormat() const {
  QString result;
  result.append(mNumberFormatChar);
  if (mNumberBeautifulPowers) {
    result.append(QLatin1Char('b'));
    if (mLabelPainter.multiplicationSymbol() ==
        QCPLabelPainterPrivate::SymbolCross)
      result.append(QLatin1Char('c'));
  }
  return result;
}

/*!
  Returns the number of axes on the axis rect side specified with \a type.

  \see axis
*/
int QCPPolarAxisAngular::radialAxisCount() const { return mRadialAxes.size(); }

/*!
  Returns the axis with the given \a index on the axis rect side specified with
  \a type.

  \see axisCount, axes
*/
QCPPolarAxisRadial *QCPPolarAxisAngular::radialAxis(int index) const {
  if (index >= 0 && index < mRadialAxes.size()) {
    return mRadialAxes.at(index);
  } else {
    qDebug() << Q_FUNC_INFO << "Axis index out of bounds:" << index;
    return 0;
  }
}

/*!
  Returns all axes on the axis rect sides specified with \a types.

  \a types may be a single \ref QCPAxis::AxisType or an <tt>or</tt>-combination,
  to get the axes of multiple sides.

  \see axis
*/
QList<QCPPolarAxisRadial *> QCPPolarAxisAngular::radialAxes() const {
  return mRadialAxes;
}

/*!
  Adds a new axis to the axis rect side specified with \a type, and returns it.
  If \a axis is 0, a new QCPAxis instance is created internally. QCustomPlot
  owns the returned axis, so if you want to remove an axis, use \ref removeAxis
  instead of deleting it manually.

  You may inject QCPAxis instances (or subclasses of QCPAxis) by setting \a axis
  to an axis that was previously created outside QCustomPlot. It is important to
  note that QCustomPlot takes ownership of the axis, so you may not delete it
  afterwards. Further, the \a axis must have been created with this axis rect as
  parent and with the same axis type as specified in \a type. If this is not the
  case, a debug output is generated, the axis is not added, and the method
  returns 0.

  This method can not be used to move \a axis between axis rects. The same \a
  axis instance must not be added multiple times to the same or different axis
  rects.

  If an axis rect side already contains one or more axes, the lower and upper
  endings of the new axis (\ref QCPAxis::setLowerEnding, \ref
  QCPAxis::setUpperEnding) are set to \ref QCPLineEnding::esHalfBar.

  \see addAxes, setupFullAxesBox
*/
QCPPolarAxisRadial *QCPPolarAxisAngular::addRadialAxis(
    QCPPolarAxisRadial *axis) {
  QCPPolarAxisRadial *newAxis = axis;
  if (!newAxis) {
    newAxis = new QCPPolarAxisRadial(this);
  } else  // user provided existing axis instance, do some sanity checks
  {
    if (newAxis->angularAxis() != this) {
      qDebug() << Q_FUNC_INFO
               << "passed radial axis doesn't have this angular axis as parent "
                  "angular axis";
      return 0;
    }
    if (radialAxes().contains(newAxis)) {
      qDebug() << Q_FUNC_INFO
               << "passed axis is already owned by this angular axis";
      return 0;
    }
  }
  mRadialAxes.append(newAxis);
  return newAxis;
}

/*!
  Removes the specified \a axis from the axis rect and deletes it.

  Returns true on success, i.e. if \a axis was a valid axis in this axis rect.

  \see addAxis
*/
bool QCPPolarAxisAngular::removeRadialAxis(QCPPolarAxisRadial *radialAxis) {
  if (mRadialAxes.contains(radialAxis)) {
    mRadialAxes.removeOne(radialAxis);
    delete radialAxis;
    return true;
  } else {
    qDebug() << Q_FUNC_INFO
             << "Radial axis isn't associated with this angular axis:"
             << reinterpret_cast<quintptr>(radialAxis);
    return false;
  }
}

QRegion QCPPolarAxisAngular::exactClipRegion() const {
  return QRegion(mCenter.x() - mRadius, mCenter.y() - mRadius,
                 qRound(2 * mRadius), qRound(2 * mRadius), QRegion::Ellipse);
}

/*!
  If the scale type (\ref setScaleType) is \ref stLinear, \a diff is added to
  the lower and upper bounds of the range. The range is simply moved by \a diff.

  If the scale type is \ref stLogarithmic, the range bounds are multiplied by \a
  diff. This corresponds to an apparent "linear" move in logarithmic scaling by
  a distance of log(diff).
*/
void QCPPolarAxisAngular::moveRange(double diff) {
  QCPRange oldRange = mRange;
  mRange.lower += diff;
  mRange.upper += diff;
  emit rangeChanged(mRange);
  emit rangeChanged(mRange, oldRange);
}

/*!
  Scales the range of this axis by \a factor around the center of the current
  axis range. For example, if \a factor is 2.0, then the axis range will double
  its size, and the point at the axis range center won't have changed its
  position in the QCustomPlot widget (i.e. coordinates around the center will
  have moved symmetrically closer).

  If you wish to scale around a different coordinate than the current axis range
  center, use the overload \ref scaleRange(double factor, double center).
*/
void QCPPolarAxisAngular::scaleRange(double factor) {
  scaleRange(factor, range().center());
}

/*! \overload

  Scales the range of this axis by \a factor around the coordinate \a center.
  For example, if \a factor is 2.0, \a center is 1.0, then the axis range will
  double its size, and the point at coordinate 1.0 won't have changed its
  position in the QCustomPlot widget (i.e. coordinates around 1.0 will have
  moved symmetrically closer to 1.0).

  \see scaleRange(double factor)
*/
void QCPPolarAxisAngular::scaleRange(double factor, double center) {
  QCPRange oldRange = mRange;
  QCPRange newRange;
  newRange.lower = (mRange.lower - center) * factor + center;
  newRange.upper = (mRange.upper - center) * factor + center;
  if (QCPRange::validRange(newRange)) mRange = newRange.sanitizedForLinScale();
  emit rangeChanged(mRange);
  emit rangeChanged(mRange, oldRange);
}

/*!
  Changes the axis range such that all plottables associated with this axis are
  fully visible in that dimension.

  \see QCPAbstractPlottable::rescaleAxes, QCustomPlot::rescaleAxes
*/
void QCPPolarAxisAngular::rescale(bool onlyVisiblePlottables) {
  QCPRange newRange;
  bool haveRange = false;
  for (int i = 0; i < mGraphs.size(); ++i) {
    if (!mGraphs.at(i)->realVisibility() && onlyVisiblePlottables) continue;
    QCPRange range;
    bool currentFoundRange;
    if (mGraphs.at(i)->keyAxis() == this)
      range = mGraphs.at(i)->getKeyRange(currentFoundRange, QCP::sdBoth);
    else
      range = mGraphs.at(i)->getValueRange(currentFoundRange, QCP::sdBoth);
    if (currentFoundRange) {
      if (!haveRange)
        newRange = range;
      else
        newRange.expand(range);
      haveRange = true;
    }
  }
  if (haveRange) {
    if (!QCPRange::validRange(
            newRange))  // likely due to range being zero (plottable has only
    // constant data in this axis dimension), shift current
    // range to at least center the plottable
    {
      double center =
          (newRange.lower + newRange.upper) *
          0.5;  // upper and lower should be equal anyway, but just to make
                // sure, incase validRange returned false for other reason
      newRange.lower = center - mRange.size() / 2.0;
      newRange.upper = center + mRange.size() / 2.0;
    }
    setRange(newRange);
  }
}

/*!
  Transforms \a value, in pixel coordinates of the QCustomPlot widget, to axis
  coordinates.
*/
void QCPPolarAxisAngular::pixelToCoord(QPointF pixelPos, double &angleCoord,
                                       double &radiusCoord) const {
  if (!mRadialAxes.isEmpty())
    mRadialAxes.first()->pixelToCoord(pixelPos, angleCoord, radiusCoord);
  else
    qDebug() << Q_FUNC_INFO << "no radial axis configured";
}

/*!
  Transforms \a value, in coordinates of the axis, to pixel coordinates of the
  QCustomPlot widget.
*/
QPointF QCPPolarAxisAngular::coordToPixel(double angleCoord,
                                          double radiusCoord) const {
  if (!mRadialAxes.isEmpty()) {
    return mRadialAxes.first()->coordToPixel(angleCoord, radiusCoord);
  } else {
    qDebug() << Q_FUNC_INFO << "no radial axis configured";
    return QPointF();
  }
}

/*!
  Returns the part of the axis that is hit by \a pos (in pixels). The return
  value of this function is independent of the user-selectable parts defined
  with \ref setSelectableParts. Further, this function does not change the
  current selection state of the axis.

  If the axis is not visible (\ref setVisible), this function always returns
  \ref spNone.

  \see setSelectedParts, setSelectableParts, QCustomPlot::setInteractions
*/
QCPPolarAxisAngular::SelectablePart QCPPolarAxisAngular::getPartAt(
    const QPointF &pos) const {
  Q_UNUSED(pos)  // TODO remove later

  if (!mVisible) return spNone;

  /*
  TODO:
if (mAxisPainter->axisSelectionBox().contains(pos.toPoint()))
  return spAxis;
else if (mAxisPainter->tickLabelsSelectionBox().contains(pos.toPoint()))
  return spTickLabels;
else if (mAxisPainter->labelSelectionBox().contains(pos.toPoint()))
  return spAxisLabel;
else */
  return spNone;
}

/* inherits documentation from base class */
double QCPPolarAxisAngular::selectTest(const QPointF &pos, bool onlySelectable,
                                       QVariant *details) const {
  /*
if (!mParentPlot) return -1;
SelectablePart part = getPartAt(pos);
if ((onlySelectable && !mSelectableParts.testFlag(part)) || part == spNone)
  return -1;

if (details)
  details->setValue(part);
return mParentPlot->selectionTolerance()*0.99;
*/

  Q_UNUSED(details)

  if (onlySelectable) return -1;

  if (QRectF(mOuterRect).contains(pos)) {
    if (mParentPlot)
      return mParentPlot->selectionTolerance() * 0.99;
    else {
      qDebug() << Q_FUNC_INFO << "parent plot not defined";
      return -1;
    }
  } else
    return -1;
}

/*!
  This method is called automatically upon replot and doesn't need to be called
  by users of QCPPolarAxisAngular.

  Calls the base class implementation to update the margins (see \ref
  QCPLayoutElement::update), and finally passes the \ref rect to the inset
  layout (\ref insetLayout) and calls its QCPInsetLayout::update function.

  \seebaseclassmethod
*/
void QCPPolarAxisAngular::update(UpdatePhase phase) {
  QCPLayoutElement::update(phase);

  switch (phase) {
    case upPreparation: {
      setupTickVectors();
      for (int i = 0; i < mRadialAxes.size(); ++i)
        mRadialAxes.at(i)->setupTickVectors();
      break;
    }
    case upLayout: {
      mCenter = mRect.center();
      mRadius = 0.5 * qMin(qAbs(mRect.width()), qAbs(mRect.height()));
      if (mRadius < 1)
        mRadius = 1;  // prevent cases where radius might become 0 which causes
                      // trouble
      for (int i = 0; i < mRadialAxes.size(); ++i)
        mRadialAxes.at(i)->updateGeometry(mCenter, mRadius);

      mInsetLayout->setOuterRect(rect());
      break;
    }
    default:
      break;
  }

  // pass update call on to inset layout (doesn't happen automatically, because
  // QCPPolarAxis doesn't derive from QCPLayout):
  mInsetLayout->update(phase);
}

/* inherits documentation from base class */
QList<QCPLayoutElement *> QCPPolarAxisAngular::elements(bool recursive) const {
  QList<QCPLayoutElement *> result;
  if (mInsetLayout) {
    result << mInsetLayout;
    if (recursive) result << mInsetLayout->elements(recursive);
  }
  return result;
}

bool QCPPolarAxisAngular::removeGraph(QCPPolarGraph *graph) {
  if (!mGraphs.contains(graph)) {
    qDebug() << Q_FUNC_INFO
             << "graph not in list:" << reinterpret_cast<quintptr>(graph);
    return false;
  }

  // remove plottable from legend:
  graph->removeFromLegend();
  // remove plottable:
  delete graph;
  mGraphs.removeOne(graph);
  return true;
}

/* inherits documentation from base class */
void QCPPolarAxisAngular::applyDefaultAntialiasingHint(
    QCPPainter *painter) const {
  applyAntialiasingHint(painter, mAntialiased, QCP::aeAxes);
}

/* inherits documentation from base class */
void QCPPolarAxisAngular::draw(QCPPainter *painter) {
  drawBackground(painter, mCenter, mRadius);

  // draw baseline circle:
  painter->setPen(getBasePen());
  painter->drawEllipse(mCenter, mRadius, mRadius);

  // draw subticks:
  if (!mSubTickVector.isEmpty()) {
    painter->setPen(getSubTickPen());
    for (int i = 0; i < mSubTickVector.size(); ++i) {
      painter->drawLine(
          mCenter + mSubTickVectorCosSin.at(i) * (mRadius - mSubTickLengthIn),
          mCenter + mSubTickVectorCosSin.at(i) * (mRadius + mSubTickLengthOut));
    }
  }

  // draw ticks and labels:
  if (!mTickVector.isEmpty()) {
    mLabelPainter.setAnchorReference(mCenter);
    mLabelPainter.setFont(getTickLabelFont());
    mLabelPainter.setColor(getTickLabelColor());
    const QPen ticksPen = getTickPen();
    painter->setPen(ticksPen);
    for (int i = 0; i < mTickVector.size(); ++i) {
      const QPointF outerTick =
          mCenter + mTickVectorCosSin.at(i) * (mRadius + mTickLengthOut);
      painter->drawLine(
          mCenter + mTickVectorCosSin.at(i) * (mRadius - mTickLengthIn),
          outerTick);
      // draw tick labels:
      if (!mTickVectorLabels.isEmpty()) {
        if (i < mTickVectorLabels.count() - 1 ||
            (mTickVectorCosSin.at(i) - mTickVectorCosSin.first())
                    .manhattanLength() >
                5 / 180.0 * M_PI)  // skip last label if it's closer than approx
          // 5 degrees to first
          mLabelPainter.drawTickLabel(painter, outerTick,
                                      mTickVectorLabels.at(i));
      }
    }
  }
}

/* inherits documentation from base class */
QCP::Interaction QCPPolarAxisAngular::selectionCategory() const {
  return QCP::iSelectAxes;
}

/*!
  Sets \a pm as the axis background pixmap. The axis background pixmap will be
  drawn inside the axis rect. Since axis rects place themselves on the
  "background" layer by default, the axis rect backgrounds are usually drawn
  below everything else.

  For cases where the provided pixmap doesn't have the same size as the axis
  rect, scaling can be enabled with \ref setBackgroundScaled and the scaling
  mode (i.e. whether and how the aspect ratio is preserved) can be set with \ref
  setBackgroundScaledMode. To set all these options in one call, consider using
  the overloaded version of this function.

  Below the pixmap, the axis rect may be optionally filled with a brush, if
  specified with \ref setBackground(const QBrush &brush).

  \see setBackgroundScaled, setBackgroundScaledMode, setBackground(const QBrush
  &brush)
*/
void QCPPolarAxisAngular::setBackground(const QPixmap &pm) {
  mBackgroundPixmap = pm;
  mScaledBackgroundPixmap = QPixmap();
}

/*! \overload

  Sets \a brush as the background brush. The axis rect background will be filled
  with this brush. Since axis rects place themselves on the "background" layer
  by default, the axis rect backgrounds are usually drawn below everything else.

  The brush will be drawn before (under) any background pixmap, which may be
  specified with \ref setBackground(const QPixmap &pm).

  To disable drawing of a background brush, set \a brush to Qt::NoBrush.

  \see setBackground(const QPixmap &pm)
*/
void QCPPolarAxisAngular::setBackground(const QBrush &brush) {
  mBackgroundBrush = brush;
}

/*! \overload

  Allows setting the background pixmap of the axis rect, whether it shall be
  scaled and how it shall be scaled in one call.

  \see setBackground(const QPixmap &pm), setBackgroundScaled,
  setBackgroundScaledMode
*/
void QCPPolarAxisAngular::setBackground(const QPixmap &pm, bool scaled,
                                        Qt::AspectRatioMode mode) {
  mBackgroundPixmap = pm;
  mScaledBackgroundPixmap = QPixmap();
  mBackgroundScaled = scaled;
  mBackgroundScaledMode = mode;
}

/*!
  Sets whether the axis background pixmap shall be scaled to fit the axis rect
  or not. If \a scaled is set to true, you may control whether and how the
  aspect ratio of the original pixmap is preserved with \ref
  setBackgroundScaledMode.

  Note that the scaled version of the original pixmap is buffered, so there is
  no performance penalty on replots. (Except when the axis rect dimensions are
  changed continuously.)

  \see setBackground, setBackgroundScaledMode
*/
void QCPPolarAxisAngular::setBackgroundScaled(bool scaled) {
  mBackgroundScaled = scaled;
}

/*!
  If scaling of the axis background pixmap is enabled (\ref
  setBackgroundScaled), use this function to define whether and how the aspect
  ratio of the original pixmap passed to \ref setBackground is preserved. \see
  setBackground, setBackgroundScaled
*/
void QCPPolarAxisAngular::setBackgroundScaledMode(Qt::AspectRatioMode mode) {
  mBackgroundScaledMode = mode;
}

void QCPPolarAxisAngular::setRangeDrag(bool enabled) { mRangeDrag = enabled; }

void QCPPolarAxisAngular::setRangeZoom(bool enabled) { mRangeZoom = enabled; }

void QCPPolarAxisAngular::setRangeZoomFactor(double factor) {
  mRangeZoomFactor = factor;
}

/*!
  Sets the range of the axis.

  This slot may be connected with the \ref rangeChanged signal of another axis
  so this axis is always synchronized with the other axis range, when it
  changes.

  To invert the direction of an axis, use \ref setRangeReversed.
*/
void QCPPolarAxisAngular::setRange(const QCPRange &range) {
  if (range.lower == mRange.lower && range.upper == mRange.upper) return;

  if (!QCPRange::validRange(range)) return;
  QCPRange oldRange = mRange;
  mRange = range.sanitizedForLinScale();
  emit rangeChanged(mRange);
  emit rangeChanged(mRange, oldRange);
}

/*!
  Sets whether the user can (de-)select the parts in \a selectable by clicking
  on the QCustomPlot surface. (When \ref QCustomPlot::setInteractions contains
  iSelectAxes.)

  However, even when \a selectable is set to a value not allowing the selection
  of a specific part, it is still possible to set the selection of this part
  manually, by calling \ref setSelectedParts directly.

  \see SelectablePart, setSelectedParts
*/
void QCPPolarAxisAngular::setSelectableParts(
    const SelectableParts &selectable) {
  if (mSelectableParts != selectable) {
    mSelectableParts = selectable;
    emit selectableChanged(mSelectableParts);
  }
}

/*!
  Sets the selected state of the respective axis parts described by \ref
  SelectablePart. When a part is selected, it uses a different pen/font.

  The entire selection mechanism for axes is handled automatically when \ref
  QCustomPlot::setInteractions contains iSelectAxes. You only need to call this
  function when you wish to change the selection state manually.

  This function can change the selection state of a part, independent of the
  \ref setSelectableParts setting.

  emits the \ref selectionChanged signal when \a selected is different from the
  previous selection state.

  \see SelectablePart, setSelectableParts, selectTest, setSelectedBasePen,
  setSelectedTickPen, setSelectedSubTickPen, setSelectedTickLabelFont,
  setSelectedLabelFont, setSelectedTickLabelColor, setSelectedLabelColor
*/
void QCPPolarAxisAngular::setSelectedParts(const SelectableParts &selected) {
  if (mSelectedParts != selected) {
    mSelectedParts = selected;
    emit selectionChanged(mSelectedParts);
  }
}

/*!
  \overload

  Sets the lower and upper bound of the axis range.

  To invert the direction of an axis, use \ref setRangeReversed.

  There is also a slot to set a range, see \ref setRange(const QCPRange &range).
*/
void QCPPolarAxisAngular::setRange(double lower, double upper) {
  if (lower == mRange.lower && upper == mRange.upper) return;

  if (!QCPRange::validRange(lower, upper)) return;
  QCPRange oldRange = mRange;
  mRange.lower = lower;
  mRange.upper = upper;
  mRange = mRange.sanitizedForLinScale();
  emit rangeChanged(mRange);
  emit rangeChanged(mRange, oldRange);
}

/*!
  \overload

  Sets the range of the axis.

  The \a position coordinate indicates together with the \a alignment parameter,
  where the new range will be positioned. \a size defines the size of the new
  axis range. \a alignment may be Qt::AlignLeft, Qt::AlignRight or
  Qt::AlignCenter. This will cause the left border, right border, or center of
  the range to be aligned with \a position. Any other values of \a alignment
  will default to Qt::AlignCenter.
*/
void QCPPolarAxisAngular::setRange(double position, double size,
                                   Qt::AlignmentFlag alignment) {
  if (alignment == Qt::AlignLeft)
    setRange(position, position + size);
  else if (alignment == Qt::AlignRight)
    setRange(position - size, position);
  else  // alignment == Qt::AlignCenter
    setRange(position - size / 2.0, position + size / 2.0);
}

/*!
  Sets the lower bound of the axis range. The upper bound is not changed.
  \see setRange
*/
void QCPPolarAxisAngular::setRangeLower(double lower) {
  if (mRange.lower == lower) return;

  QCPRange oldRange = mRange;
  mRange.lower = lower;
  mRange = mRange.sanitizedForLinScale();
  emit rangeChanged(mRange);
  emit rangeChanged(mRange, oldRange);
}

/*!
  Sets the upper bound of the axis range. The lower bound is not changed.
  \see setRange
*/
void QCPPolarAxisAngular::setRangeUpper(double upper) {
  if (mRange.upper == upper) return;

  QCPRange oldRange = mRange;
  mRange.upper = upper;
  mRange = mRange.sanitizedForLinScale();
  emit rangeChanged(mRange);
  emit rangeChanged(mRange, oldRange);
}

/*!
  Sets whether the axis range (direction) is displayed reversed. Normally, the
  values on horizontal axes increase left to right, on vertical axes bottom to
  top. When \a reversed is set to true, the direction of increasing values is
  inverted.

  Note that the range and data interface stays the same for reversed axes, e.g.
  the \a lower part of the \ref setRange interface will still reference the
  mathematically smaller number than the \a upper part.
*/
void QCPPolarAxisAngular::setRangeReversed(bool reversed) {
  mRangeReversed = reversed;
}

void QCPPolarAxisAngular::setAngle(double degrees) {
  mAngle = degrees;
  mAngleRad = mAngle / 180.0 * M_PI;
}

/*!
  The axis ticker is responsible for generating the tick positions and tick
  labels. See the documentation of QCPAxisTicker for details on how to work with
  axis tickers.

  You can change the tick positioning/labeling behaviour of this axis by setting
  a different QCPAxisTicker subclass using this method. If you only wish to
  modify the currently installed axis ticker, access it via \ref ticker.

  Since the ticker is stored in the axis as a shared pointer, multiple axes may
  share the same axis ticker simply by passing the same shared pointer to
  multiple axes.

  \see ticker
*/
void QCPPolarAxisAngular::setTicker(QSharedPointer<QCPAxisTicker> ticker) {
  if (ticker)
    mTicker = ticker;
  else
    qDebug() << Q_FUNC_INFO << "can not set 0 as axis ticker";
  // no need to invalidate margin cache here because produced tick labels are
  // checked for changes in setupTickVector
}

/*!
  Sets whether tick marks are displayed.

  Note that setting \a show to false does not imply that tick labels are
  invisible, too. To achieve that, see \ref setTickLabels.

  \see setSubTicks
*/
void QCPPolarAxisAngular::setTicks(bool show) {
  if (mTicks != show) {
    mTicks = show;
    // mCachedMarginValid = false;
  }
}

/*!
  Sets whether tick labels are displayed. Tick labels are the numbers drawn next
  to tick marks.
*/
void QCPPolarAxisAngular::setTickLabels(bool show) {
  if (mTickLabels != show) {
    mTickLabels = show;
    // mCachedMarginValid = false;
    if (!mTickLabels) mTickVectorLabels.clear();
  }
}

/*!
  Sets the distance between the axis base line (including any outward ticks) and
  the tick labels. \see setLabelPadding, setPadding
*/
void QCPPolarAxisAngular::setTickLabelPadding(int padding) {
  mLabelPainter.setPadding(padding);
}

/*!
  Sets the font of the tick labels.

  \see setTickLabels, setTickLabelColor
*/
void QCPPolarAxisAngular::setTickLabelFont(const QFont &font) {
  mTickLabelFont = font;
}

/*!
  Sets the color of the tick labels.

  \see setTickLabels, setTickLabelFont
*/
void QCPPolarAxisAngular::setTickLabelColor(const QColor &color) {
  mTickLabelColor = color;
}

/*!
  Sets the rotation of the tick labels. If \a degrees is zero, the labels are
  drawn normally. Else, the tick labels are drawn rotated by \a degrees
  clockwise. The specified angle is bound to values from -90 to 90 degrees.

  If \a degrees is exactly -90, 0 or 90, the tick labels are centered on the
  tick coordinate. For other angles, the label is drawn with an offset such that
  it seems to point toward or away from the tick mark.
*/
void QCPPolarAxisAngular::setTickLabelRotation(double degrees) {
  mLabelPainter.setRotation(degrees);
}

void QCPPolarAxisAngular::setTickLabelMode(LabelMode mode) {
  switch (mode) {
    case lmUpright:
      mLabelPainter.setAnchorMode(QCPLabelPainterPrivate::amSkewedUpright);
      break;
    case lmRotated:
      mLabelPainter.setAnchorMode(QCPLabelPainterPrivate::amSkewedRotated);
      break;
  }
}

/*!
  Sets the number format for the numbers in tick labels. This \a formatCode is
  an extended version of the format code used e.g. by QString::number() and
  QLocale::toString(). For reference about that, see the "Argument Formats"
  section in the detailed description of the QString class.

  \a formatCode is a string of one, two or three characters. The first character
  is identical to the normal format code used by Qt. In short, this means:
  'e'/'E' scientific format, 'f' fixed format, 'g'/'G' scientific or fixed,
  whichever is shorter.

  The second and third characters are optional and specific to QCustomPlot:\n If
  the first char was 'e' or 'g', numbers are/might be displayed in the
  scientific format, e.g. "5.5e9", which might be visually unappealing in a
  plot. So when the second char of \a formatCode is set to 'b' (for
  "beautiful"), those exponential numbers are formatted in a more natural way,
  i.e. "5.5 [multiplication sign] 10 [superscript] 9". By default, the
  multiplication sign is a centered dot. If instead a cross should be shown (as
  is usual in the USA), the third char of \a formatCode can be set to 'c'. The
  inserted multiplication signs are the UTF-8 characters 215 (0xD7) for the
  cross and 183 (0xB7) for the dot.

  Examples for \a formatCode:
  \li \c g normal format code behaviour. If number is small, fixed format is
  used, if number is large, normal scientific format is used \li \c gb If number
  is small, fixed format is used, if number is large, scientific format is used
  with beautifully typeset decimal powers and a dot as multiplication sign \li
  \c ebc All numbers are in scientific format with beautifully typeset decimal
  power and a cross as multiplication sign \li \c fb illegal format code, since
  fixed format doesn't support (or need) beautifully typeset decimal powers.
  Format code will be reduced to 'f'. \li \c hello illegal format code, since
  first char is not 'e', 'E', 'f', 'g' or 'G'. Current format code will not be
  changed.
*/
void QCPPolarAxisAngular::setNumberFormat(const QString &formatCode) {
  if (formatCode.isEmpty()) {
    qDebug() << Q_FUNC_INFO << "Passed formatCode is empty";
    return;
  }
  // mCachedMarginValid = false;

  // interpret first char as number format char:
  QString allowedFormatChars(QLatin1String("eEfgG"));
  if (allowedFormatChars.contains(formatCode.at(0))) {
    mNumberFormatChar = QLatin1Char(formatCode.at(0).toLatin1());
  } else {
    qDebug() << Q_FUNC_INFO
             << "Invalid number format code (first char not in 'eEfgG'):"
             << formatCode;
    return;
  }

  if (formatCode.length() < 2) {
    mNumberBeautifulPowers = false;
    mNumberMultiplyCross = false;
  } else {
    // interpret second char as indicator for beautiful decimal powers:
    if (formatCode.at(1) == QLatin1Char('b') &&
        (mNumberFormatChar == QLatin1Char('e') ||
         mNumberFormatChar == QLatin1Char('g')))
      mNumberBeautifulPowers = true;
    else
      qDebug() << Q_FUNC_INFO
               << "Invalid number format code (second char not 'b' or first "
                  "char neither 'e' nor 'g'):"
               << formatCode;

    if (formatCode.length() < 3) {
      mNumberMultiplyCross = false;
    } else {
      // interpret third char as indicator for dot or cross multiplication
      // symbol:
      if (formatCode.at(2) == QLatin1Char('c'))
        mNumberMultiplyCross = true;
      else if (formatCode.at(2) == QLatin1Char('d'))
        mNumberMultiplyCross = false;
      else
        qDebug()
            << Q_FUNC_INFO
            << "Invalid number format code (third char neither 'c' nor 'd'):"
            << formatCode;
    }
  }
  mLabelPainter.setSubstituteExponent(mNumberBeautifulPowers);
  mLabelPainter.setMultiplicationSymbol(
      mNumberMultiplyCross ? QCPLabelPainterPrivate::SymbolCross
                           : QCPLabelPainterPrivate::SymbolDot);
}

/*!
  Sets the precision of the tick label numbers. See QLocale::toString(double i,
  char f, int prec) for details. The effect of precisions are most notably for
  number Formats starting with 'e', see \ref setNumberFormat
*/
void QCPPolarAxisAngular::setNumberPrecision(int precision) {
  if (mNumberPrecision != precision) {
    mNumberPrecision = precision;
    // mCachedMarginValid = false;
  }
}

/*!
  Sets the length of the ticks in pixels. \a inside is the length the ticks will
  reach inside the plot and \a outside is the length they will reach outside the
  plot. If \a outside is greater than zero, the tick labels and axis label will
  increase their distance to the axis accordingly, so they won't collide with
  the ticks.

  \see setSubTickLength, setTickLengthIn, setTickLengthOut
*/
void QCPPolarAxisAngular::setTickLength(int inside, int outside) {
  setTickLengthIn(inside);
  setTickLengthOut(outside);
}

/*!
  Sets the length of the inward ticks in pixels. \a inside is the length the
  ticks will reach inside the plot.

  \see setTickLengthOut, setTickLength, setSubTickLength
*/
void QCPPolarAxisAngular::setTickLengthIn(int inside) {
  if (mTickLengthIn != inside) {
    mTickLengthIn = inside;
  }
}

/*!
  Sets the length of the outward ticks in pixels. \a outside is the length the
  ticks will reach outside the plot. If \a outside is greater than zero, the
  tick labels and axis label will increase their distance to the axis
  accordingly, so they won't collide with the ticks.

  \see setTickLengthIn, setTickLength, setSubTickLength
*/
void QCPPolarAxisAngular::setTickLengthOut(int outside) {
  if (mTickLengthOut != outside) {
    mTickLengthOut = outside;
    // mCachedMarginValid = false; // only outside tick length can change margin
  }
}

/*!
  Sets whether sub tick marks are displayed.

  Sub ticks are only potentially visible if (major) ticks are also visible (see
  \ref setTicks)

  \see setTicks
*/
void QCPPolarAxisAngular::setSubTicks(bool show) {
  if (mSubTicks != show) {
    mSubTicks = show;
    // mCachedMarginValid = false;
  }
}

/*!
  Sets the length of the subticks in pixels. \a inside is the length the
  subticks will reach inside the plot and \a outside is the length they will
  reach outside the plot. If \a outside is greater than zero, the tick labels
  and axis label will increase their distance to the axis accordingly, so they
  won't collide with the ticks.

  \see setTickLength, setSubTickLengthIn, setSubTickLengthOut
*/
void QCPPolarAxisAngular::setSubTickLength(int inside, int outside) {
  setSubTickLengthIn(inside);
  setSubTickLengthOut(outside);
}

/*!
  Sets the length of the inward subticks in pixels. \a inside is the length the
  subticks will reach inside the plot.

  \see setSubTickLengthOut, setSubTickLength, setTickLength
*/
void QCPPolarAxisAngular::setSubTickLengthIn(int inside) {
  if (mSubTickLengthIn != inside) {
    mSubTickLengthIn = inside;
  }
}

/*!
  Sets the length of the outward subticks in pixels. \a outside is the length
  the subticks will reach outside the plot. If \a outside is greater than zero,
  the tick labels will increase their distance to the axis accordingly, so they
  won't collide with the ticks.

  \see setSubTickLengthIn, setSubTickLength, setTickLength
*/
void QCPPolarAxisAngular::setSubTickLengthOut(int outside) {
  if (mSubTickLengthOut != outside) {
    mSubTickLengthOut = outside;
    // mCachedMarginValid = false; // only outside tick length can change margin
  }
}

/*!
  Sets the pen, the axis base line is drawn with.

  \see setTickPen, setSubTickPen
*/
void QCPPolarAxisAngular::setBasePen(const QPen &pen) { mBasePen = pen; }

/*!
  Sets the pen, tick marks will be drawn with.

  \see setTickLength, setBasePen
*/
void QCPPolarAxisAngular::setTickPen(const QPen &pen) { mTickPen = pen; }

/*!
  Sets the pen, subtick marks will be drawn with.

  \see setSubTickCount, setSubTickLength, setBasePen
*/
void QCPPolarAxisAngular::setSubTickPen(const QPen &pen) { mSubTickPen = pen; }

/*!
  Sets the font of the axis label.

  \see setLabelColor
*/
void QCPPolarAxisAngular::setLabelFont(const QFont &font) {
  if (mLabelFont != font) {
    mLabelFont = font;
    // mCachedMarginValid = false;
  }
}

/*!
  Sets the color of the axis label.

  \see setLabelFont
*/
void QCPPolarAxisAngular::setLabelColor(const QColor &color) {
  mLabelColor = color;
}

/*!
  Sets the text of the axis label that will be shown below/above or next to the
  axis, depending on its orientation. To disable axis labels, pass an empty
  string as \a str.
*/
void QCPPolarAxisAngular::setLabel(const QString &str) {
  if (mLabel != str) {
    mLabel = str;
    // mCachedMarginValid = false;
  }
}

/*!
  Sets the distance between the tick labels and the axis label.

  \see setTickLabelPadding, setPadding
*/
void QCPPolarAxisAngular::setLabelPadding(int padding) {
  if (mLabelPadding != padding) {
    mLabelPadding = padding;
    // mCachedMarginValid = false;
  }
}

/*!
  Sets the font that is used for tick labels when they are selected.

  \see setTickLabelFont, setSelectableParts, setSelectedParts,
  QCustomPlot::setInteractions
*/
void QCPPolarAxisAngular::setSelectedTickLabelFont(const QFont &font) {
  if (font != mSelectedTickLabelFont) {
    mSelectedTickLabelFont = font;
    // don't set mCachedMarginValid to false here because margin calculation is
    // always done with non-selected fonts
  }
}

/*!
  Sets the font that is used for the axis label when it is selected.

  \see setLabelFont, setSelectableParts, setSelectedParts,
  QCustomPlot::setInteractions
*/
void QCPPolarAxisAngular::setSelectedLabelFont(const QFont &font) {
  mSelectedLabelFont = font;
  // don't set mCachedMarginValid to false here because margin calculation is
  // always done with non-selected fonts
}

/*!
  Sets the color that is used for tick labels when they are selected.

  \see setTickLabelColor, setSelectableParts, setSelectedParts,
  QCustomPlot::setInteractions
*/
void QCPPolarAxisAngular::setSelectedTickLabelColor(const QColor &color) {
  if (color != mSelectedTickLabelColor) {
    mSelectedTickLabelColor = color;
  }
}

/*!
  Sets the color that is used for the axis label when it is selected.

  \see setLabelColor, setSelectableParts, setSelectedParts,
  QCustomPlot::setInteractions
*/
void QCPPolarAxisAngular::setSelectedLabelColor(const QColor &color) {
  mSelectedLabelColor = color;
}

/*!
  Sets the pen that is used to draw the axis base line when selected.

  \see setBasePen, setSelectableParts, setSelectedParts,
  QCustomPlot::setInteractions
*/
void QCPPolarAxisAngular::setSelectedBasePen(const QPen &pen) {
  mSelectedBasePen = pen;
}

/*!
  Sets the pen that is used to draw the (major) ticks when selected.

  \see setTickPen, setSelectableParts, setSelectedParts,
  QCustomPlot::setInteractions
*/
void QCPPolarAxisAngular::setSelectedTickPen(const QPen &pen) {
  mSelectedTickPen = pen;
}

/*!
  Sets the pen that is used to draw the subticks when selected.

  \see setSubTickPen, setSelectableParts, setSelectedParts,
  QCustomPlot::setInteractions
*/
void QCPPolarAxisAngular::setSelectedSubTickPen(const QPen &pen) {
  mSelectedSubTickPen = pen;
}

/*! \internal

  Draws the background of this axis rect. It may consist of a background fill (a
  QBrush) and a pixmap.

  If a brush was given via \ref setBackground(const QBrush &brush), this
  function first draws an according filling inside the axis rect with the
  provided \a painter.

  Then, if a pixmap was provided via \ref setBackground, this function buffers
  the scaled version depending on \ref setBackgroundScaled and \ref
  setBackgroundScaledMode and then draws it inside the axis rect with the
  provided \a painter. The scaled version is buffered in mScaledBackgroundPixmap
  to prevent expensive rescaling at every redraw. It is only updated, when the
  axis rect has changed in a way that requires a rescale of the background
  pixmap (this is dependent on the \ref setBackgroundScaledMode), or when a
  differend axis background pixmap was set.

  \see setBackground, setBackgroundScaled, setBackgroundScaledMode
*/
void QCPPolarAxisAngular::drawBackground(QCPPainter *painter,
                                         const QPointF &center, double radius) {
  // draw background fill (don't use circular clip, looks bad):
  if (mBackgroundBrush != Qt::NoBrush) {
    QPainterPath ellipsePath;
    ellipsePath.addEllipse(center, radius, radius);
    painter->fillPath(ellipsePath, mBackgroundBrush);
  }

  // draw background pixmap (on top of fill, if brush specified):
  if (!mBackgroundPixmap.isNull()) {
    QRegion clipCircle(center.x() - radius, center.y() - radius,
                       qRound(2 * radius), qRound(2 * radius),
                       QRegion::Ellipse);
    QRegion originalClip = painter->clipRegion();
    painter->setClipRegion(clipCircle);
    if (mBackgroundScaled) {
      // check whether mScaledBackground needs to be updated:
      QSize scaledSize(mBackgroundPixmap.size());
      scaledSize.scale(mRect.size(), mBackgroundScaledMode);
      if (mScaledBackgroundPixmap.size() != scaledSize)
        mScaledBackgroundPixmap = mBackgroundPixmap.scaled(
            mRect.size(), mBackgroundScaledMode, Qt::SmoothTransformation);
      painter->drawPixmap(mRect.topLeft() + QPoint(0, -1),
                          mScaledBackgroundPixmap,
                          QRect(0, 0, mRect.width(), mRect.height()) &
                              mScaledBackgroundPixmap.rect());
    } else {
      painter->drawPixmap(mRect.topLeft() + QPoint(0, -1), mBackgroundPixmap,
                          QRect(0, 0, mRect.width(), mRect.height()));
    }
    painter->setClipRegion(originalClip);
  }
}

/*! \internal

  Prepares the internal tick vector, sub tick vector and tick label vector. This
  is done by calling QCPAxisTicker::generate on the currently installed ticker.

  If a change in the label text/count is detected, the cached axis margin is
  invalidated to make sure the next margin calculation recalculates the label
  sizes and returns an up-to-date value.
*/
void QCPPolarAxisAngular::setupTickVectors() {
  if (!mParentPlot) return;
  if ((!mTicks && !mTickLabels && !mGrid->visible()) || mRange.size() <= 0)
    return;

  mSubTickVector.clear();  // since we might not pass it to mTicker->generate(),
                           // and we don't want old data in there
  mTicker->generate(mRange, mParentPlot->locale(), mNumberFormatChar,
                    mNumberPrecision, mTickVector,
                    mSubTicks ? &mSubTickVector : 0,
                    mTickLabels ? &mTickVectorLabels : 0);

  // fill cos/sin buffers which will be used by draw() and QCPPolarGrid::draw(),
  // so we don't have to calculate it twice:
  mTickVectorCosSin.resize(mTickVector.size());
  for (int i = 0; i < mTickVector.size(); ++i) {
    const double theta = coordToAngleRad(mTickVector.at(i));
    mTickVectorCosSin[i] = QPointF(qCos(theta), qSin(theta));
  }
  mSubTickVectorCosSin.resize(mSubTickVector.size());
  for (int i = 0; i < mSubTickVector.size(); ++i) {
    const double theta = coordToAngleRad(mSubTickVector.at(i));
    mSubTickVectorCosSin[i] = QPointF(qCos(theta), qSin(theta));
  }
}

/*! \internal

  Returns the pen that is used to draw the axis base line. Depending on the
  selection state, this is either mSelectedBasePen or mBasePen.
*/
QPen QCPPolarAxisAngular::getBasePen() const {
  return mSelectedParts.testFlag(spAxis) ? mSelectedBasePen : mBasePen;
}

/*! \internal

  Returns the pen that is used to draw the (major) ticks. Depending on the
  selection state, this is either mSelectedTickPen or mTickPen.
*/
QPen QCPPolarAxisAngular::getTickPen() const {
  return mSelectedParts.testFlag(spAxis) ? mSelectedTickPen : mTickPen;
}

/*! \internal

  Returns the pen that is used to draw the subticks. Depending on the selection
  state, this is either mSelectedSubTickPen or mSubTickPen.
*/
QPen QCPPolarAxisAngular::getSubTickPen() const {
  return mSelectedParts.testFlag(spAxis) ? mSelectedSubTickPen : mSubTickPen;
}

/*! \internal

  Returns the font that is used to draw the tick labels. Depending on the
  selection state, this is either mSelectedTickLabelFont or mTickLabelFont.
*/
QFont QCPPolarAxisAngular::getTickLabelFont() const {
  return mSelectedParts.testFlag(spTickLabels) ? mSelectedTickLabelFont
                                               : mTickLabelFont;
}

/*! \internal

  Returns the font that is used to draw the axis label. Depending on the
  selection state, this is either mSelectedLabelFont or mLabelFont.
*/
QFont QCPPolarAxisAngular::getLabelFont() const {
  return mSelectedParts.testFlag(spAxisLabel) ? mSelectedLabelFont : mLabelFont;
}

/*! \internal

  Returns the color that is used to draw the tick labels. Depending on the
  selection state, this is either mSelectedTickLabelColor or mTickLabelColor.
*/
QColor QCPPolarAxisAngular::getTickLabelColor() const {
  return mSelectedParts.testFlag(spTickLabels) ? mSelectedTickLabelColor
                                               : mTickLabelColor;
}

/*! \internal

  Returns the color that is used to draw the axis label. Depending on the
  selection state, this is either mSelectedLabelColor or mLabelColor.
*/
QColor QCPPolarAxisAngular::getLabelColor() const {
  return mSelectedParts.testFlag(spAxisLabel) ? mSelectedLabelColor
                                              : mLabelColor;
}

/*! \internal

  Event handler for when a mouse button is pressed on the axis rect. If the left
  mouse button is pressed, the range dragging interaction is initialized (the
  actual range manipulation happens in the \ref mouseMoveEvent).

  The mDragging flag is set to true and some anchor points are set that are
  needed to determine the distance the mouse was dragged in the mouse
  move/release events later.

  \see mouseMoveEvent, mouseReleaseEvent
*/
void QCPPolarAxisAngular::mousePressEvent(QMouseEvent *event,
                                          const QVariant &details) {
  Q_UNUSED(details)
  if (event->buttons() & Qt::LeftButton) {
    mDragging = true;
    // initialize antialiasing backup in case we start dragging:
    if (mParentPlot->noAntialiasingOnDrag()) {
      mAADragBackup = mParentPlot->antialiasedElements();
      mNotAADragBackup = mParentPlot->notAntialiasedElements();
    }
    // Mouse range dragging interaction:
    if (mParentPlot->interactions().testFlag(QCP::iRangeDrag)) {
      mDragAngularStart = range();
      mDragRadialStart.clear();
      for (int i = 0; i < mRadialAxes.size(); ++i)
        mDragRadialStart.append(mRadialAxes.at(i)->range());
    }
  }
}

/*! \internal

  Event handler for when the mouse is moved on the axis rect. If range dragging
  was activated in a preceding \ref mousePressEvent, the range is moved
  accordingly.

  \see mousePressEvent, mouseReleaseEvent
*/
void QCPPolarAxisAngular::mouseMoveEvent(QMouseEvent *event,
                                         const QPointF &startPos) {
  Q_UNUSED(startPos)
  bool doReplot = false;
  // Mouse range dragging interaction:
  if (mDragging && mParentPlot->interactions().testFlag(QCP::iRangeDrag)) {
    if (mRangeDrag) {
      doReplot = true;
      double angleCoordStart, radiusCoordStart;
      double angleCoord, radiusCoord;
      pixelToCoord(startPos, angleCoordStart, radiusCoordStart);
      pixelToCoord(event->pos(), angleCoord, radiusCoord);
      double diff = angleCoordStart - angleCoord;
      setRange(mDragAngularStart.lower + diff, mDragAngularStart.upper + diff);
    }

    for (int i = 0; i < mRadialAxes.size(); ++i) {
      QCPPolarAxisRadial *ax = mRadialAxes.at(i);
      if (!ax->rangeDrag()) continue;
      doReplot = true;
      double angleCoordStart, radiusCoordStart;
      double angleCoord, radiusCoord;
      ax->pixelToCoord(startPos, angleCoordStart, radiusCoordStart);
      ax->pixelToCoord(event->pos(), angleCoord, radiusCoord);
      if (ax->scaleType() == QCPPolarAxisRadial::stLinear) {
        double diff = radiusCoordStart - radiusCoord;
        ax->setRange(mDragRadialStart.at(i).lower + diff,
                     mDragRadialStart.at(i).upper + diff);
      } else if (ax->scaleType() == QCPPolarAxisRadial::stLogarithmic) {
        if (radiusCoord != 0) {
          double diff = radiusCoordStart / radiusCoord;
          ax->setRange(mDragRadialStart.at(i).lower * diff,
                       mDragRadialStart.at(i).upper * diff);
        }
      }
    }

    if (doReplot)  // if either vertical or horizontal drag was enabled, do a
    // replot
    {
      if (mParentPlot->noAntialiasingOnDrag())
        mParentPlot->setNotAntialiasedElements(QCP::aeAll);
      mParentPlot->replot(QCustomPlot::rpQueuedReplot);
    }
  }
}

/* inherits documentation from base class */
void QCPPolarAxisAngular::mouseReleaseEvent(QMouseEvent *event,
                                            const QPointF &startPos) {
  Q_UNUSED(event)
  Q_UNUSED(startPos)
  mDragging = false;
  if (mParentPlot->noAntialiasingOnDrag()) {
    mParentPlot->setAntialiasedElements(mAADragBackup);
    mParentPlot->setNotAntialiasedElements(mNotAADragBackup);
  }
}

/*! \internal

  Event handler for mouse wheel events. If rangeZoom is Qt::Horizontal,
  Qt::Vertical or both, the ranges of the axes defined as rangeZoomHorzAxis and
  rangeZoomVertAxis are scaled. The center of the scaling operation is the
  current cursor position inside the axis rect. The scaling factor is dependent
  on the mouse wheel delta (which direction the wheel was rotated) to provide a
  natural zooming feel. The Strength of the zoom can be controlled via \ref
  setRangeZoomFactor.

  Note, that event->delta() is usually +/-120 for single rotation steps.
  However, if the mouse wheel is turned rapidly, many steps may bunch up to one
  event, so the event->delta() may then be multiples of 120. This is taken into
  account here, by calculating \a wheelSteps and using it as exponent of the
  range zoom factor. This takes care of the wheel direction automatically, by
  inverting the factor, when the wheel step is negative (f^-1 = 1/f).
*/
void QCPPolarAxisAngular::wheelEvent(QWheelEvent *event) {
  bool doReplot = false;
  // Mouse range zooming interaction:
  if (mParentPlot->interactions().testFlag(QCP::iRangeZoom)) {
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    const double delta = event->delta();
#else
    const double delta = event->angleDelta().y();
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    const QPointF pos = event->pos();
#else
    const QPointF pos = event->position();
#endif
    const double wheelSteps =
        delta / 120.0;  // a single step delta is +/-120 usually
    if (mRangeZoom) {
      double angleCoord, radiusCoord;
      pixelToCoord(pos, angleCoord, radiusCoord);
      scaleRange(qPow(mRangeZoomFactor, wheelSteps), angleCoord);
    }

    for (int i = 0; i < mRadialAxes.size(); ++i) {
      QCPPolarAxisRadial *ax = mRadialAxes.at(i);
      if (!ax->rangeZoom()) continue;
      doReplot = true;
      double angleCoord, radiusCoord;
      ax->pixelToCoord(pos, angleCoord, radiusCoord);
      ax->scaleRange(qPow(ax->rangeZoomFactor(), wheelSteps), radiusCoord);
    }
  }
  if (doReplot) mParentPlot->replot();
}

bool QCPPolarAxisAngular::registerPolarGraph(QCPPolarGraph *graph) {
  if (mGraphs.contains(graph)) {
    qDebug() << Q_FUNC_INFO
             << "plottable already added:" << reinterpret_cast<quintptr>(graph);
    return false;
  }
  if (graph->keyAxis() != this) {
    qDebug() << Q_FUNC_INFO << "plottable not created with this as axis:"
             << reinterpret_cast<quintptr>(graph);
    return false;
  }

  mGraphs.append(graph);
  // possibly add plottable to legend:
  if (mParentPlot->autoAddPlottableToLegend()) graph->addToLegend();
  if (!graph->layer())  // usually the layer is already set in the constructor
    // of the plottable (via QCPLayerable constructor)
    graph->setLayer(mParentPlot->currentLayer());
  return true;
}
/* end of 'src/polar/layoutelement-angularaxis.cpp' */

/* including file 'src/polar/polargrid.cpp' */
/* modified 2022-11-06T12:45:57, size 7493  */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPPolarGrid
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPPolarGrid
  \brief The grid in both angular and radial dimensions for polar plots

  \warning In this QCustomPlot version, polar plots are a tech preview. Expect
  documentation and functionality to be incomplete, as well as changing public
  interfaces in the future.
*/

/*!
  Creates a QCPPolarGrid instance and sets default values.

  You shouldn't instantiate grids on their own, since every axis brings its own
  grid.
*/
QCPPolarGrid::QCPPolarGrid(QCPPolarAxisAngular *parentAxis)
    : QCPLayerable(parentAxis->parentPlot(), QString(), parentAxis),
      mType(gtNone),
      mSubGridType(gtNone),
      mAntialiasedSubGrid(true),
      mAntialiasedZeroLine(true),
      mParentAxis(parentAxis) {
  // warning: this is called in QCPPolarAxisAngular constructor, so parentAxis
  // members should not be accessed/called
  setParent(parentAxis);
  setType(gtAll);
  setSubGridType(gtNone);

  setAngularPen(QPen(QColor(200, 200, 200), 0, Qt::DotLine));
  setAngularSubGridPen(QPen(QColor(220, 220, 220), 0, Qt::DotLine));

  setRadialPen(QPen(QColor(200, 200, 200), 0, Qt::DotLine));
  setRadialSubGridPen(QPen(QColor(220, 220, 220), 0, Qt::DotLine));
  setRadialZeroLinePen(QPen(QColor(200, 200, 200), 0, Qt::SolidLine));

  setAntialiased(true);
}

void QCPPolarGrid::setRadialAxis(QCPPolarAxisRadial *axis) {
  mRadialAxis = axis;
}

void QCPPolarGrid::setType(GridTypes type) { mType = type; }

void QCPPolarGrid::setSubGridType(GridTypes type) { mSubGridType = type; }

/*!
  Sets whether sub grid lines are drawn antialiased.
*/
void QCPPolarGrid::setAntialiasedSubGrid(bool enabled) {
  mAntialiasedSubGrid = enabled;
}

/*!
  Sets whether zero lines are drawn antialiased.
*/
void QCPPolarGrid::setAntialiasedZeroLine(bool enabled) {
  mAntialiasedZeroLine = enabled;
}

/*!
  Sets the pen with which (major) grid lines are drawn.
*/
void QCPPolarGrid::setAngularPen(const QPen &pen) { mAngularPen = pen; }

/*!
  Sets the pen with which sub grid lines are drawn.
*/
void QCPPolarGrid::setAngularSubGridPen(const QPen &pen) {
  mAngularSubGridPen = pen;
}

void QCPPolarGrid::setRadialPen(const QPen &pen) { mRadialPen = pen; }

void QCPPolarGrid::setRadialSubGridPen(const QPen &pen) {
  mRadialSubGridPen = pen;
}

void QCPPolarGrid::setRadialZeroLinePen(const QPen &pen) {
  mRadialZeroLinePen = pen;
}

/*! \internal

  A convenience function to easily set the QPainter::Antialiased hint on the
  provided \a painter before drawing the major grid lines.

  This is the antialiasing state the painter passed to the \ref draw method is
  in by default.

  This function takes into account the local setting of the antialiasing flag as
  well as the overrides set with \ref QCustomPlot::setAntialiasedElements and
  \ref QCustomPlot::setNotAntialiasedElements.

  \see setAntialiased
*/
void QCPPolarGrid::applyDefaultAntialiasingHint(QCPPainter *painter) const {
  applyAntialiasingHint(painter, mAntialiased, QCP::aeGrid);
}

/*! \internal

  Draws grid lines and sub grid lines at the positions of (sub) ticks of the
  parent axis, spanning over the complete axis rect. Also draws the zero line,
  if appropriate (\ref setZeroLinePen).
*/
void QCPPolarGrid::draw(QCPPainter *painter) {
  if (!mParentAxis) {
    qDebug() << Q_FUNC_INFO << "invalid parent axis";
    return;
  }

  const QPointF center = mParentAxis->mCenter;
  const double radius = mParentAxis->mRadius;

  painter->setBrush(Qt::NoBrush);
  // draw main angular grid:
  if (mType.testFlag(gtAngular))
    drawAngularGrid(painter, center, radius, mParentAxis->mTickVectorCosSin,
                    mAngularPen);
  // draw main radial grid:
  if (mType.testFlag(gtRadial) && mRadialAxis)
    drawRadialGrid(painter, center, mRadialAxis->tickVector(), mRadialPen,
                   mRadialZeroLinePen);

  applyAntialiasingHint(painter, mAntialiasedSubGrid, QCP::aeGrid);
  // draw sub angular grid:
  if (mSubGridType.testFlag(gtAngular))
    drawAngularGrid(painter, center, radius, mParentAxis->mSubTickVectorCosSin,
                    mAngularSubGridPen);
  // draw sub radial grid:
  if (mSubGridType.testFlag(gtRadial) && mRadialAxis)
    drawRadialGrid(painter, center, mRadialAxis->subTickVector(),
                   mRadialSubGridPen);
}

void QCPPolarGrid::drawRadialGrid(QCPPainter *painter, const QPointF &center,
                                  const QVector<double> &coords,
                                  const QPen &pen, const QPen &zeroPen) {
  if (!mRadialAxis) return;
  if (coords.isEmpty()) return;
  const bool drawZeroLine = zeroPen != Qt::NoPen;
  const double zeroLineEpsilon = qAbs(coords.last() - coords.first()) * 1e-6;

  painter->setPen(pen);
  for (int i = 0; i < coords.size(); ++i) {
    const double r = mRadialAxis->coordToRadius(coords.at(i));
    if (drawZeroLine && qAbs(coords.at(i)) < zeroLineEpsilon) {
      applyAntialiasingHint(painter, mAntialiasedZeroLine, QCP::aeZeroLine);
      painter->setPen(zeroPen);
      painter->drawEllipse(center, r, r);
      painter->setPen(pen);
      applyDefaultAntialiasingHint(painter);
    } else {
      painter->drawEllipse(center, r, r);
    }
  }
}

void QCPPolarGrid::drawAngularGrid(QCPPainter *painter, const QPointF &center,
                                   double radius,
                                   const QVector<QPointF> &ticksCosSin,
                                   const QPen &pen) {
  if (ticksCosSin.isEmpty()) return;

  painter->setPen(pen);
  for (int i = 0; i < ticksCosSin.size(); ++i)
    painter->drawLine(center, center + ticksCosSin.at(i) * radius);
}
/* end of 'src/polar/polargrid.cpp' */

/* including file 'src/polar/polargraph.cpp' */
/* modified 2022-11-06T12:45:57, size 44035  */

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPPolarLegendItem
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPPolarLegendItem
  \brief A legend item for polar plots

  \warning In this QCustomPlot version, polar plots are a tech preview. Expect
  documentation and functionality to be incomplete, as well as changing public
  interfaces in the future.
*/
QCPPolarLegendItem::QCPPolarLegendItem(QCPLegend *parent, QCPPolarGraph *graph)
    : QCPAbstractLegendItem(parent), mPolarGraph(graph) {
  setAntialiased(false);
}

void QCPPolarLegendItem::draw(QCPPainter *painter) {
  if (!mPolarGraph) return;
  painter->setFont(getFont());
  painter->setPen(QPen(getTextColor()));
  QSizeF iconSize = mParentLegend->iconSize();
  QRectF textRect = painter->fontMetrics().boundingRect(
      0, 0, 0, iconSize.height(), Qt::TextDontClip, mPolarGraph->name());
  QRectF iconRect(mRect.topLeft(), iconSize);
  int textHeight = qMax(
      textRect.height(),
      iconSize.height());  // if text has smaller height than icon, center text
                           // vertically in icon height, else align tops
  painter->drawText(
      mRect.x() + iconSize.width() + mParentLegend->iconTextPadding(),
      mRect.y(), textRect.width(), textHeight, Qt::TextDontClip,
      mPolarGraph->name());
  // draw icon:
  painter->save();
  painter->setClipRect(iconRect, Qt::IntersectClip);
  mPolarGraph->drawLegendIcon(painter, iconRect);
  painter->restore();
  // draw icon border:
  if (getIconBorderPen().style() != Qt::NoPen) {
    painter->setPen(getIconBorderPen());
    painter->setBrush(Qt::NoBrush);
    int halfPen = qCeil(painter->pen().widthF() * 0.5) + 1;
    painter->setClipRect(mOuterRect.adjusted(
        -halfPen, -halfPen, halfPen,
        halfPen));  // extend default clip rect so thicker pens (especially
                    // during selection) are not clipped
    painter->drawRect(iconRect);
  }
}

QSize QCPPolarLegendItem::minimumOuterSizeHint() const {
  if (!mPolarGraph) return QSize();
  QSize result(0, 0);
  QRect textRect;
  QFontMetrics fontMetrics(getFont());
  QSize iconSize = mParentLegend->iconSize();
  textRect = fontMetrics.boundingRect(0, 0, 0, iconSize.height(),
                                      Qt::TextDontClip, mPolarGraph->name());
  result.setWidth(iconSize.width() + mParentLegend->iconTextPadding() +
                  textRect.width());
  result.setHeight(qMax(textRect.height(), iconSize.height()));
  result.rwidth() += mMargins.left() + mMargins.right();
  result.rheight() += mMargins.top() + mMargins.bottom();
  return result;
}

QPen QCPPolarLegendItem::getIconBorderPen() const {
  return mSelected ? mParentLegend->selectedIconBorderPen()
                   : mParentLegend->iconBorderPen();
}

QColor QCPPolarLegendItem::getTextColor() const {
  return mSelected ? mSelectedTextColor : mTextColor;
}

QFont QCPPolarLegendItem::getFont() const {
  return mSelected ? mSelectedFont : mFont;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPPolarGraph
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPPolarGraph
  \brief A radial graph used to display data in polar plots

  \warning In this QCustomPlot version, polar plots are a tech preview. Expect
  documentation and functionality to be incomplete, as well as changing public
  interfaces in the future.
*/

/* start of documentation of inline functions */

// TODO

/* end of documentation of inline functions */

/*!
  Constructs a graph which uses \a keyAxis as its angular and \a valueAxis as
  its radial axis. \a keyAxis and \a valueAxis must reside in the same
  QCustomPlot, and the radial axis must be associated with the angular axis. If
  either of these restrictions is violated, a corresponding message is printed
  to the debug output (qDebug), the construction is not aborted, though.

  The created QCPPolarGraph is automatically registered with the QCustomPlot
  instance inferred from \a keyAxis. This QCustomPlot instance takes ownership
  of the QCPPolarGraph, so do not delete it manually but use
  QCPPolarAxisAngular::removeGraph() instead.

  To directly create a QCPPolarGraph inside a plot, you shoud use the
  QCPPolarAxisAngular::addGraph method.
*/
QCPPolarGraph::QCPPolarGraph(QCPPolarAxisAngular *keyAxis,
                             QCPPolarAxisRadial *valueAxis)
    : QCPLayerable(keyAxis->parentPlot(), QString(), keyAxis),
      mDataContainer(new QCPGraphDataContainer),
      mName(),
      mAntialiasedFill(true),
      mAntialiasedScatters(true),
      mPen(Qt::black),
      mBrush(Qt::NoBrush),
      mPeriodic(true),
      mKeyAxis(keyAxis),
      mValueAxis(valueAxis),
      mSelectable(QCP::stWhole)
// mSelectionDecorator(0) // TODO
{
  if (keyAxis->parentPlot() != valueAxis->parentPlot())
    qDebug() << Q_FUNC_INFO
             << "Parent plot of keyAxis is not the same as that of valueAxis.";

  mKeyAxis->registerPolarGraph(this);

  // setSelectionDecorator(new QCPSelectionDecorator); // TODO

  setPen(QPen(Qt::blue, 0));
  setBrush(Qt::NoBrush);
  setLineStyle(lsLine);
}

QCPPolarGraph::~QCPPolarGraph() {
  /* TODO
if (mSelectionDecorator)
{
  delete mSelectionDecorator;
  mSelectionDecorator = 0;
}
*/
}

/*!
   The name is the textual representation of this plottable as it is displayed
   in the legend
   (\ref QCPLegend). It may contain any UTF-8 characters, including newlines.
*/
void QCPPolarGraph::setName(const QString &name) { mName = name; }

/*!
  Sets whether fills of this plottable are drawn antialiased or not.

  Note that this setting may be overridden by \ref
  QCustomPlot::setAntialiasedElements and \ref
  QCustomPlot::setNotAntialiasedElements.
*/
void QCPPolarGraph::setAntialiasedFill(bool enabled) {
  mAntialiasedFill = enabled;
}

/*!
  Sets whether the scatter symbols of this plottable are drawn antialiased or
  not.

  Note that this setting may be overridden by \ref
  QCustomPlot::setAntialiasedElements and \ref
  QCustomPlot::setNotAntialiasedElements.
*/
void QCPPolarGraph::setAntialiasedScatters(bool enabled) {
  mAntialiasedScatters = enabled;
}

/*!
  The pen is used to draw basic lines that make up the plottable representation
  in the plot.

  For example, the \ref QCPGraph subclass draws its graph lines with this pen.

  \see setBrush
*/
void QCPPolarGraph::setPen(const QPen &pen) { mPen = pen; }

/*!
  The brush is used to draw basic fills of the plottable representation in the
  plot. The Fill can be a color, gradient or texture, see the usage of QBrush.

  For example, the \ref QCPGraph subclass draws the fill under the graph with
  this brush, when it's not set to Qt::NoBrush.

  \see setPen
*/
void QCPPolarGraph::setBrush(const QBrush &brush) { mBrush = brush; }

void QCPPolarGraph::setPeriodic(bool enabled) { mPeriodic = enabled; }

/*!
  The key axis of a plottable can be set to any axis of a QCustomPlot, as long
  as it is orthogonal to the plottable's value axis. This function performs no
  checks to make sure this is the case. The typical mathematical choice is to
  use the x-axis (QCustomPlot::xAxis) as key axis and the y-axis
  (QCustomPlot::yAxis) as value axis.

  Normally, the key and value axes are set in the constructor of the plottable
  (or \ref QCustomPlot::addGraph when working with QCPGraphs through the
  dedicated graph interface).

  \see setValueAxis
*/
void QCPPolarGraph::setKeyAxis(QCPPolarAxisAngular *axis) { mKeyAxis = axis; }

/*!
  The value axis of a plottable can be set to any axis of a QCustomPlot, as long
  as it is orthogonal to the plottable's key axis. This function performs no
  checks to make sure this is the case. The typical mathematical choice is to
  use the x-axis (QCustomPlot::xAxis) as key axis and the y-axis
  (QCustomPlot::yAxis) as value axis.

  Normally, the key and value axes are set in the constructor of the plottable
  (or \ref QCustomPlot::addGraph when working with QCPGraphs through the
  dedicated graph interface).

  \see setKeyAxis
*/
void QCPPolarGraph::setValueAxis(QCPPolarAxisRadial *axis) {
  mValueAxis = axis;
}

/*!
  Sets whether and to which granularity this plottable can be selected.

  A selection can happen by clicking on the QCustomPlot surface (When \ref
  QCustomPlot::setInteractions contains \ref QCP::iSelectPlottables), by
  dragging a selection rect (When \ref QCustomPlot::setSelectionRectMode is \ref
  QCP::srmSelect), or programmatically by calling \ref setSelection.

  \see setSelection, QCP::SelectionType
*/
void QCPPolarGraph::setSelectable(QCP::SelectionType selectable) {
  if (mSelectable != selectable) {
    mSelectable = selectable;
    QCPDataSelection oldSelection = mSelection;
    mSelection.enforceType(mSelectable);
    emit selectableChanged(mSelectable);
    if (mSelection != oldSelection) {
      emit selectionChanged(selected());
      emit selectionChanged(mSelection);
    }
  }
}

/*!
  Sets which data ranges of this plottable are selected. Selected data ranges
  are drawn differently (e.g. color) in the plot. This can be controlled via the
  selection decorator (see \ref selectionDecorator).

  The entire selection mechanism for plottables is handled automatically when
  \ref QCustomPlot::setInteractions contains iSelectPlottables. You only need to
  call this function when you wish to change the selection state
  programmatically.

  Using \ref setSelectable you can further specify for each plottable whether
  and to which granularity it is selectable. If \a selection is not compatible
  with the current \ref QCP::SelectionType set via \ref setSelectable, the
  resulting selection will be adjusted accordingly (see \ref
  QCPDataSelection::enforceType).

  emits the \ref selectionChanged signal when \a selected is different from the
  previous selection state.

  \see setSelectable, selectTest
*/
void QCPPolarGraph::setSelection(QCPDataSelection selection) {
  selection.enforceType(mSelectable);
  if (mSelection != selection) {
    mSelection = selection;
    emit selectionChanged(selected());
    emit selectionChanged(mSelection);
  }
}

/*! \overload

  Replaces the current data container with the provided \a data container.

  Since a QSharedPointer is used, multiple QCPPolarGraphs may share the same
  data container safely. Modifying the data in the container will then affect
  all graphs that share the container. Sharing can be achieved by simply
  exchanging the data containers wrapped in shared pointers: \snippet
  documentation/doc-code-snippets/mainwindow.cpp QCPPolarGraph-datasharing-1

  If you do not wish to share containers, but create a copy from an existing
  container, rather use the \ref QCPDataContainer<DataType>::set method on the
  graph's data container directly: \snippet
  documentation/doc-code-snippets/mainwindow.cpp QCPPolarGraph-datasharing-2

  \see addData
*/
void QCPPolarGraph::setData(QSharedPointer<QCPGraphDataContainer> data) {
  mDataContainer = data;
}

/*! \overload

  Replaces the current data with the provided points in \a keys and \a values.
  The provided vectors should have equal length. Else, the number of added
  points will be the size of the smallest vector.

  If you can guarantee that the passed data points are sorted by \a keys in
  ascending order, you can set \a alreadySorted to true, to improve performance
  by saving a sorting run.

  \see addData
*/
void QCPPolarGraph::setData(const QVector<double> &keys,
                            const QVector<double> &values, bool alreadySorted) {
  mDataContainer->clear();
  addData(keys, values, alreadySorted);
}

/*!
  Sets how the single data points are connected in the plot. For scatter-only
  plots, set \a ls to \ref lsNone and \ref setScatterStyle to the desired
  scatter style.

  \see setScatterStyle
*/
void QCPPolarGraph::setLineStyle(LineStyle ls) { mLineStyle = ls; }

/*!
  Sets the visual appearance of single data points in the plot. If set to \ref
  QCPScatterStyle::ssNone, no scatter points are drawn (e.g. for line-only-plots
  with appropriate line style).

  \see QCPScatterStyle, setLineStyle
*/
void QCPPolarGraph::setScatterStyle(const QCPScatterStyle &style) {
  mScatterStyle = style;
}

void QCPPolarGraph::addData(const QVector<double> &keys,
                            const QVector<double> &values, bool alreadySorted) {
  if (keys.size() != values.size())
    qDebug() << Q_FUNC_INFO
             << "keys and values have different sizes:" << keys.size()
             << values.size();
  const int n = qMin(keys.size(), values.size());
  QVector<QCPGraphData> tempData(n);
  QVector<QCPGraphData>::iterator it = tempData.begin();
  const QVector<QCPGraphData>::iterator itEnd = tempData.end();
  int i = 0;
  while (it != itEnd) {
    it->key = keys[i];
    it->value = values[i];
    ++it;
    ++i;
  }
  mDataContainer->add(tempData,
                      alreadySorted);  // don't modify tempData beyond this to
                                       // prevent copy on write
}

void QCPPolarGraph::addData(double key, double value) {
  mDataContainer->add(QCPGraphData(key, value));
}

/*!
  Use this method to set an own QCPSelectionDecorator (subclass) instance. This
  allows you to customize the visual representation of selected data ranges
  further than by using the default QCPSelectionDecorator.

  The plottable takes ownership of the \a decorator.

  The currently set decorator can be accessed via \ref selectionDecorator.
*/
/*
void QCPPolarGraph::setSelectionDecorator(QCPSelectionDecorator *decorator)
{
  if (decorator)
  {
    if (decorator->registerWithPlottable(this))
    {
      if (mSelectionDecorator) // delete old decorator if necessary
        delete mSelectionDecorator;
      mSelectionDecorator = decorator;
    }
  } else if (mSelectionDecorator) // just clear decorator
  {
    delete mSelectionDecorator;
    mSelectionDecorator = 0;
  }
}
*/

void QCPPolarGraph::coordsToPixels(double key, double value, double &x,
                                   double &y) const {
  if (mValueAxis) {
    const QPointF point = mValueAxis->coordToPixel(key, value);
    x = point.x();
    y = point.y();
  } else {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
  }
}

const QPointF QCPPolarGraph::coordsToPixels(double key, double value) const {
  if (mValueAxis) {
    return mValueAxis->coordToPixel(key, value);
  } else {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    return QPointF();
  }
}

void QCPPolarGraph::pixelsToCoords(double x, double y, double &key,
                                   double &value) const {
  if (mValueAxis) {
    mValueAxis->pixelToCoord(QPointF(x, y), key, value);
  } else {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
  }
}

void QCPPolarGraph::pixelsToCoords(const QPointF &pixelPos, double &key,
                                   double &value) const {
  if (mValueAxis) {
    mValueAxis->pixelToCoord(pixelPos, key, value);
  } else {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
  }
}

void QCPPolarGraph::rescaleAxes(bool onlyEnlarge) const {
  rescaleKeyAxis(onlyEnlarge);
  rescaleValueAxis(onlyEnlarge);
}

void QCPPolarGraph::rescaleKeyAxis(bool onlyEnlarge) const {
  QCPPolarAxisAngular *keyAxis = mKeyAxis.data();
  if (!keyAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key axis";
    return;
  }

  bool foundRange;
  QCPRange newRange = getKeyRange(foundRange, QCP::sdBoth);
  if (foundRange) {
    if (onlyEnlarge) newRange.expand(keyAxis->range());
    if (!QCPRange::validRange(
            newRange))  // likely due to range being zero (plottable has only
    // constant data in this axis dimension), shift current
    // range to at least center the plottable
    {
      double center =
          (newRange.lower + newRange.upper) *
          0.5;  // upper and lower should be equal anyway, but just to make
                // sure, incase validRange returned false for other reason
      newRange.lower = center - keyAxis->range().size() / 2.0;
      newRange.upper = center + keyAxis->range().size() / 2.0;
    }
    keyAxis->setRange(newRange);
  }
}

void QCPPolarGraph::rescaleValueAxis(bool onlyEnlarge, bool inKeyRange) const {
  QCPPolarAxisAngular *keyAxis = mKeyAxis.data();
  QCPPolarAxisRadial *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    return;
  }

  QCP::SignDomain signDomain = QCP::sdBoth;
  if (valueAxis->scaleType() == QCPPolarAxisRadial::stLogarithmic)
    signDomain =
        (valueAxis->range().upper < 0 ? QCP::sdNegative : QCP::sdPositive);

  bool foundRange;
  QCPRange newRange = getValueRange(foundRange, signDomain,
                                    inKeyRange ? keyAxis->range() : QCPRange());
  if (foundRange) {
    if (onlyEnlarge) newRange.expand(valueAxis->range());
    if (!QCPRange::validRange(
            newRange))  // likely due to range being zero (plottable has only
    // constant data in this axis dimension), shift current
    // range to at least center the plottable
    {
      double center =
          (newRange.lower + newRange.upper) *
          0.5;  // upper and lower should be equal anyway, but just to make
                // sure, incase validRange returned false for other reason
      if (valueAxis->scaleType() == QCPPolarAxisRadial::stLinear) {
        newRange.lower = center - valueAxis->range().size() / 2.0;
        newRange.upper = center + valueAxis->range().size() / 2.0;
      } else  // scaleType() == stLogarithmic
      {
        newRange.lower =
            center / qSqrt(valueAxis->range().upper / valueAxis->range().lower);
        newRange.upper =
            center * qSqrt(valueAxis->range().upper / valueAxis->range().lower);
      }
    }
    valueAxis->setRange(newRange);
  }
}

bool QCPPolarGraph::addToLegend(QCPLegend *legend) {
  if (!legend) {
    qDebug() << Q_FUNC_INFO << "passed legend is null";
    return false;
  }
  if (legend->parentPlot() != mParentPlot) {
    qDebug() << Q_FUNC_INFO
             << "passed legend isn't in the same QCustomPlot as this plottable";
    return false;
  }

  // if (!legend->hasItemWithPlottable(this)) // TODO
  //{
  legend->addItem(new QCPPolarLegendItem(legend, this));
  return true;
  //} else
  //  return false;
}

bool QCPPolarGraph::addToLegend() {
  if (!mParentPlot || !mParentPlot->legend)
    return false;
  else
    return addToLegend(mParentPlot->legend);
}

bool QCPPolarGraph::removeFromLegend(QCPLegend *legend) const {
  if (!legend) {
    qDebug() << Q_FUNC_INFO << "passed legend is null";
    return false;
  }

  QCPPolarLegendItem *removableItem = 0;
  for (int i = 0; i < legend->itemCount(); ++i)  // TODO: reduce this to code in
  // QCPAbstractPlottable::removeFromLegend once unified
  {
    if (QCPPolarLegendItem *pli =
            qobject_cast<QCPPolarLegendItem *>(legend->item(i))) {
      if (pli->polarGraph() == this) {
        removableItem = pli;
        break;
      }
    }
  }

  if (removableItem)
    return legend->removeItem(removableItem);
  else
    return false;
}

bool QCPPolarGraph::removeFromLegend() const {
  if (!mParentPlot || !mParentPlot->legend)
    return false;
  else
    return removeFromLegend(mParentPlot->legend);
}

double QCPPolarGraph::selectTest(const QPointF &pos, bool onlySelectable,
                                 QVariant *details) const {
  if ((onlySelectable && mSelectable == QCP::stNone) ||
      mDataContainer->isEmpty())
    return -1;
  if (!mKeyAxis || !mValueAxis) return -1;

  if (mKeyAxis->rect().contains(pos.toPoint())) {
    QCPGraphDataContainer::const_iterator closestDataPoint =
        mDataContainer->constEnd();
    double result = pointDistance(pos, closestDataPoint);
    if (details) {
      int pointIndex = closestDataPoint - mDataContainer->constBegin();
      details->setValue(
          QCPDataSelection(QCPDataRange(pointIndex, pointIndex + 1)));
    }
    return result;
  } else
    return -1;
}

/* inherits documentation from base class */
QCPRange QCPPolarGraph::getKeyRange(bool &foundRange,
                                    QCP::SignDomain inSignDomain) const {
  return mDataContainer->keyRange(foundRange, inSignDomain);
}

/* inherits documentation from base class */
QCPRange QCPPolarGraph::getValueRange(bool &foundRange,
                                      QCP::SignDomain inSignDomain,
                                      const QCPRange &inKeyRange) const {
  return mDataContainer->valueRange(foundRange, inSignDomain, inKeyRange);
}

/* inherits documentation from base class */
QRect QCPPolarGraph::clipRect() const {
  if (mKeyAxis)
    return mKeyAxis.data()->rect();
  else
    return QRect();
}

void QCPPolarGraph::draw(QCPPainter *painter) {
  if (!mKeyAxis || !mValueAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    return;
  }
  if (mKeyAxis.data()->range().size() <= 0 || mDataContainer->isEmpty()) return;
  if (mLineStyle == lsNone && mScatterStyle.isNone()) return;

  painter->setClipRegion(mKeyAxis->exactClipRegion());

  QVector<QPointF> lines,
      scatters;  // line and (if necessary) scatter pixel coordinates will be
                 // stored here while iterating over segments

  // loop over and draw segments of unselected/selected data:
  QList<QCPDataRange> selectedSegments, unselectedSegments, allSegments;
  getDataSegments(selectedSegments, unselectedSegments);
  allSegments << unselectedSegments << selectedSegments;
  for (int i = 0; i < allSegments.size(); ++i) {
    bool isSelectedSegment = i >= unselectedSegments.size();
    // get line pixel points appropriate to line style:
    QCPDataRange lineDataRange =
        isSelectedSegment
            ? allSegments.at(i)
            : allSegments.at(i).adjusted(
                  -1, 1);  // unselected segments extend lines to bordering
                           // selected data point (safe to exceed total data
                           // bounds in first/last segment, getLines takes care)
    getLines(&lines, lineDataRange);

// check data validity if flag set:
#ifdef QCUSTOMPLOT_CHECK_DATA
    QCPGraphDataContainer::const_iterator it;
    for (it = mDataContainer->constBegin(); it != mDataContainer->constEnd();
         ++it) {
      if (QCP::isInvalidData(it->key, it->value))
        qDebug() << Q_FUNC_INFO << "Data point at" << it->key << "invalid."
                 << "Plottable name:" << name();
    }
#endif

    // draw fill of graph:
    // if (isSelectedSegment && mSelectionDecorator)
    //  mSelectionDecorator->applyBrush(painter);
    // else
    painter->setBrush(mBrush);
    painter->setPen(Qt::NoPen);
    drawFill(painter, &lines);

    // draw line:
    if (mLineStyle != lsNone) {
      // if (isSelectedSegment && mSelectionDecorator)
      //   mSelectionDecorator->applyPen(painter);
      // else
      painter->setPen(mPen);
      painter->setBrush(Qt::NoBrush);
      drawLinePlot(painter, lines);
    }

    // draw scatters:

    QCPScatterStyle finalScatterStyle = mScatterStyle;
    // if (isSelectedSegment && mSelectionDecorator)
    //   finalScatterStyle =
    //   mSelectionDecorator->getFinalScatterStyle(mScatterStyle);
    if (!finalScatterStyle.isNone()) {
      getScatters(&scatters, allSegments.at(i));
      drawScatterPlot(painter, scatters, finalScatterStyle);
    }
  }

  // draw other selection decoration that isn't just line/scatter pens and
  // brushes:
  // if (mSelectionDecorator)
  //  mSelectionDecorator->drawDecoration(painter, selection());
}

QCP::Interaction QCPPolarGraph::selectionCategory() const {
  return QCP::iSelectPlottables;
}

void QCPPolarGraph::applyDefaultAntialiasingHint(QCPPainter *painter) const {
  applyAntialiasingHint(painter, mAntialiased, QCP::aePlottables);
}

/* inherits documentation from base class */
void QCPPolarGraph::selectEvent(QMouseEvent *event, bool additive,
                                const QVariant &details,
                                bool *selectionStateChanged) {
  Q_UNUSED(event)

  if (mSelectable != QCP::stNone) {
    QCPDataSelection newSelection = details.value<QCPDataSelection>();
    QCPDataSelection selectionBefore = mSelection;
    if (additive) {
      if (mSelectable ==
          QCP::stWhole)  // in whole selection mode, we toggle to no selection
      // even if currently unselected point was hit
      {
        if (selected())
          setSelection(QCPDataSelection());
        else
          setSelection(newSelection);
      } else  // in all other selection modes we toggle selections of
      // homogeneously selected/unselected segments
      {
        if (mSelection.contains(
                newSelection))  // if entire newSelection is already selected,
          // toggle selection
          setSelection(mSelection - newSelection);
        else
          setSelection(mSelection + newSelection);
      }
    } else
      setSelection(newSelection);
    if (selectionStateChanged)
      *selectionStateChanged = mSelection != selectionBefore;
  }
}

/* inherits documentation from base class */
void QCPPolarGraph::deselectEvent(bool *selectionStateChanged) {
  if (mSelectable != QCP::stNone) {
    QCPDataSelection selectionBefore = mSelection;
    setSelection(QCPDataSelection());
    if (selectionStateChanged)
      *selectionStateChanged = mSelection != selectionBefore;
  }
}

/*!  \internal

  Draws lines between the points in \a lines, given in pixel coordinates.

  \see drawScatterPlot, drawImpulsePlot, QCPAbstractPlottable1D::drawPolyline
*/
void QCPPolarGraph::drawLinePlot(QCPPainter *painter,
                                 const QVector<QPointF> &lines) const {
  if (painter->pen().style() != Qt::NoPen &&
      painter->pen().color().alpha() != 0) {
    applyDefaultAntialiasingHint(painter);
    drawPolyline(painter, lines);
  }
}

/*! \internal

  Draws the fill of the graph using the specified \a painter, with the currently
  set brush.

  Depending on whether a normal fill or a channel fill (\ref
  setChannelFillGraph) is needed, \ref getFillPolygon or \ref
  getChannelFillPolygon are used to find the according fill polygons.

  In order to handle NaN Data points correctly (the fill needs to be split into
  disjoint areas), this method first determines a list of non-NaN segments with
  \ref getNonNanSegments, on which to operate. In the channel fill case, \ref
  getOverlappingSegments is used to consolidate the non-NaN segments of the two
  involved graphs, before passing the overlapping pairs to \ref
  getChannelFillPolygon.

  Pass the points of this graph's line as \a lines, in pixel coordinates.

  \see drawLinePlot, drawImpulsePlot, drawScatterPlot
*/
void QCPPolarGraph::drawFill(QCPPainter *painter,
                             QVector<QPointF> *lines) const {
  applyFillAntialiasingHint(painter);
  if (painter->brush().style() != Qt::NoBrush &&
      painter->brush().color().alpha() != 0)
    painter->drawPolygon(QPolygonF(*lines));
}

/*! \internal

  Draws scatter symbols at every point passed in \a scatters, given in pixel
  coordinates. The scatters will be drawn with \a painter and have the
  appearance as specified in \a style.

  \see drawLinePlot, drawImpulsePlot
*/
void QCPPolarGraph::drawScatterPlot(QCPPainter *painter,
                                    const QVector<QPointF> &scatters,
                                    const QCPScatterStyle &style) const {
  applyScattersAntialiasingHint(painter);
  style.applyTo(painter, mPen);
  for (int i = 0; i < scatters.size(); ++i)
    style.drawShape(painter, scatters.at(i).x(), scatters.at(i).y());
}

void QCPPolarGraph::drawLegendIcon(QCPPainter *painter,
                                   const QRectF &rect) const {
  // draw fill:
  if (mBrush.style() != Qt::NoBrush) {
    applyFillAntialiasingHint(painter);
    painter->fillRect(QRectF(rect.left(), rect.top() + rect.height() / 2.0,
                             rect.width(), rect.height() / 3.0),
                      mBrush);
  }
  // draw line vertically centered:
  if (mLineStyle != lsNone) {
    applyDefaultAntialiasingHint(painter);
    painter->setPen(mPen);
    painter->drawLine(QLineF(
        rect.left(), rect.top() + rect.height() / 2.0, rect.right() + 5,
        rect.top() + rect.height() / 2.0));  // +5 on x2 else last segment is
                                             // missing from dashed/dotted pens
  }
  // draw scatter symbol:
  if (!mScatterStyle.isNone()) {
    applyScattersAntialiasingHint(painter);
    // scale scatter pixmap if it's too large to fit in legend icon rect:
    if (mScatterStyle.shape() == QCPScatterStyle::ssPixmap &&
        (mScatterStyle.pixmap().size().width() > rect.width() ||
         mScatterStyle.pixmap().size().height() > rect.height())) {
      QCPScatterStyle scaledStyle(mScatterStyle);
      scaledStyle.setPixmap(scaledStyle.pixmap().scaled(
          rect.size().toSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
      scaledStyle.applyTo(painter, mPen);
      scaledStyle.drawShape(painter, QRectF(rect).center());
    } else {
      mScatterStyle.applyTo(painter, mPen);
      mScatterStyle.drawShape(painter, QRectF(rect).center());
    }
  }
}

void QCPPolarGraph::applyFillAntialiasingHint(QCPPainter *painter) const {
  applyAntialiasingHint(painter, mAntialiasedFill, QCP::aeFills);
}

void QCPPolarGraph::applyScattersAntialiasingHint(QCPPainter *painter) const {
  applyAntialiasingHint(painter, mAntialiasedScatters, QCP::aeScatters);
}

double QCPPolarGraph::pointDistance(
    const QPointF &pixelPoint,
    QCPGraphDataContainer::const_iterator &closestData) const {
  closestData = mDataContainer->constEnd();
  if (mDataContainer->isEmpty()) return -1.0;
  if (mLineStyle == lsNone && mScatterStyle.isNone()) return -1.0;

  // calculate minimum distances to graph data points and find closestData
  // iterator:
  double minDistSqr = (std::numeric_limits<double>::max)();
  // determine which key range comes into question, taking selection tolerance
  // around pos into account:
  double posKeyMin, posKeyMax, dummy;
  pixelsToCoords(pixelPoint - QPointF(mParentPlot->selectionTolerance(),
                                      mParentPlot->selectionTolerance()),
                 posKeyMin, dummy);
  pixelsToCoords(pixelPoint + QPointF(mParentPlot->selectionTolerance(),
                                      mParentPlot->selectionTolerance()),
                 posKeyMax, dummy);
  if (posKeyMin > posKeyMax) qSwap(posKeyMin, posKeyMax);
  // iterate over found data points and then choose the one with the shortest
  // distance to pos:
  QCPGraphDataContainer::const_iterator begin =
      mDataContainer->findBegin(posKeyMin, true);
  QCPGraphDataContainer::const_iterator end =
      mDataContainer->findEnd(posKeyMax, true);
  for (QCPGraphDataContainer::const_iterator it = begin; it != end; ++it) {
    const double currentDistSqr =
        QCPVector2D(coordsToPixels(it->key, it->value) - pixelPoint)
            .lengthSquared();
    if (currentDistSqr < minDistSqr) {
      minDistSqr = currentDistSqr;
      closestData = it;
    }
  }

  // calculate distance to graph line if there is one (if so, will probably be
  // smaller than distance to closest data point):
  if (mLineStyle != lsNone) {
    // line displayed, calculate distance to line segments:
    QVector<QPointF> lineData;
    getLines(&lineData, QCPDataRange(0, dataCount()));
    QCPVector2D p(pixelPoint);
    for (int i = 0; i < lineData.size() - 1; ++i) {
      const double currentDistSqr =
          p.distanceSquaredToLine(lineData.at(i), lineData.at(i + 1));
      if (currentDistSqr < minDistSqr) minDistSqr = currentDistSqr;
    }
  }

  return qSqrt(minDistSqr);
}

int QCPPolarGraph::dataCount() const { return mDataContainer->size(); }

void QCPPolarGraph::getDataSegments(
    QList<QCPDataRange> &selectedSegments,
    QList<QCPDataRange> &unselectedSegments) const {
  selectedSegments.clear();
  unselectedSegments.clear();
  if (mSelectable ==
      QCP::stWhole)  // stWhole selection type draws the entire plottable with
  // selected style if mSelection isn't empty
  {
    if (selected())
      selectedSegments << QCPDataRange(0, dataCount());
    else
      unselectedSegments << QCPDataRange(0, dataCount());
  } else {
    QCPDataSelection sel(selection());
    sel.simplify();
    selectedSegments = sel.dataRanges();
    unselectedSegments = sel.inverse(QCPDataRange(0, dataCount())).dataRanges();
  }
}

void QCPPolarGraph::drawPolyline(QCPPainter *painter,
                                 const QVector<QPointF> &lineData) const {
  // if drawing solid line and not in PDF, use much faster line drawing instead
  // of polyline:
  if (mParentPlot->plottingHints().testFlag(QCP::phFastPolylines) &&
      painter->pen().style() == Qt::SolidLine &&
      !painter->modes().testFlag(QCPPainter::pmVectorized) &&
      !painter->modes().testFlag(QCPPainter::pmNoCaching)) {
    int i = 0;
    bool lastIsNan = false;
    const int lineDataSize = lineData.size();
    while (i < lineDataSize &&
           (qIsNaN(lineData.at(i).y()) ||
            qIsNaN(lineData.at(i).x())))  // make sure first point is not NaN
      ++i;
    ++i;  // because drawing works in 1 point retrospect
    while (i < lineDataSize) {
      if (!qIsNaN(lineData.at(i).y()) &&
          !qIsNaN(lineData.at(i).x()))  // NaNs create a gap in the line
      {
        if (!lastIsNan)
          painter->drawLine(lineData.at(i - 1), lineData.at(i));
        else
          lastIsNan = false;
      } else
        lastIsNan = true;
      ++i;
    }
  } else {
    int segmentStart = 0;
    int i = 0;
    const int lineDataSize = lineData.size();
    while (i < lineDataSize) {
      if (qIsNaN(lineData.at(i).y()) || qIsNaN(lineData.at(i).x()) ||
          qIsInf(lineData.at(i).y()))  // NaNs create a gap in the line. Also
      // filter Infs which make drawPolyline block
      {
        painter->drawPolyline(
            lineData.constData() + segmentStart,
            i - segmentStart);  // i, because we don't want to include the
                                // current NaN point
        segmentStart = i + 1;
      }
      ++i;
    }
    // draw last segment:
    painter->drawPolyline(lineData.constData() + segmentStart,
                          lineDataSize - segmentStart);
  }
}

void QCPPolarGraph::getVisibleDataBounds(
    QCPGraphDataContainer::const_iterator &begin,
    QCPGraphDataContainer::const_iterator &end,
    const QCPDataRange &rangeRestriction) const {
  if (rangeRestriction.isEmpty()) {
    end = mDataContainer->constEnd();
    begin = end;
  } else {
    QCPPolarAxisAngular *keyAxis = mKeyAxis.data();
    QCPPolarAxisRadial *valueAxis = mValueAxis.data();
    if (!keyAxis || !valueAxis) {
      qDebug() << Q_FUNC_INFO << "invalid key or value axis";
      return;
    }
    // get visible data range:
    if (mPeriodic) {
      begin = mDataContainer->constBegin();
      end = mDataContainer->constEnd();
    } else {
      begin = mDataContainer->findBegin(keyAxis->range().lower);
      end = mDataContainer->findEnd(keyAxis->range().upper);
    }
    // limit lower/upperEnd to rangeRestriction:
    mDataContainer->limitIteratorsToDataRange(
        begin, end,
        rangeRestriction);  // this also ensures rangeRestriction outside data
                            // bounds doesn't break anything
  }
}

/*! \internal

  This method retrieves an optimized set of data points via \ref
  getOptimizedLineData, an branches out to the line style specific functions
  such as \ref dataToLines, \ref dataToStepLeftLines, etc. according to the line
  style of the graph.

  \a lines will be filled with points in pixel coordinates, that can be drawn
  with the according draw functions like \ref drawLinePlot and \ref
  drawImpulsePlot. The points returned in \a lines aren't necessarily the
  original data points. For example, step line styles require additional points
  to form the steps when drawn. If the line style of the graph is \ref lsNone,
  the \a lines vector will be empty.

  \a dataRange specifies the beginning and ending data indices that will be
  taken into account for conversion. In this function, the specified range may
  exceed the total data bounds without harm: a correspondingly trimmed data
  range will be used. This takes the burden off the user of this function to
  check for valid indices in \a dataRange, e.g. when extending ranges coming
  from \ref getDataSegments.

  \see getScatters
*/
void QCPPolarGraph::getLines(QVector<QPointF> *lines,
                             const QCPDataRange &dataRange) const {
  if (!lines) return;
  QCPGraphDataContainer::const_iterator begin, end;
  getVisibleDataBounds(begin, end, dataRange);
  if (begin == end) {
    lines->clear();
    return;
  }

  QVector<QCPGraphData> lineData;
  if (mLineStyle != lsNone) getOptimizedLineData(&lineData, begin, end);

  switch (mLineStyle) {
    case lsNone:
      lines->clear();
      break;
    case lsLine:
      *lines = dataToLines(lineData);
      break;
  }
}

void QCPPolarGraph::getScatters(QVector<QPointF> *scatters,
                                const QCPDataRange &dataRange) const {
  QCPPolarAxisAngular *keyAxis = mKeyAxis.data();
  QCPPolarAxisRadial *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    return;
  }

  if (!scatters) return;
  QCPGraphDataContainer::const_iterator begin, end;
  getVisibleDataBounds(begin, end, dataRange);
  if (begin == end) {
    scatters->clear();
    return;
  }

  QVector<QCPGraphData> data;
  getOptimizedScatterData(&data, begin, end);

  scatters->resize(data.size());
  for (int i = 0; i < data.size(); ++i) {
    if (!qIsNaN(data.at(i).value))
      (*scatters)[i] =
          valueAxis->coordToPixel(data.at(i).key, data.at(i).value);
  }
}

void QCPPolarGraph::getOptimizedLineData(
    QVector<QCPGraphData> *lineData,
    const QCPGraphDataContainer::const_iterator &begin,
    const QCPGraphDataContainer::const_iterator &end) const {
  lineData->clear();

  // TODO: fix for log axes and thick line style

  const QCPRange range = mValueAxis->range();
  bool reversed = mValueAxis->rangeReversed();
  const double clipMargin =
      range.size() * 0.05;  // extra distance from visible circle, so optimized
                            // outside lines can cover more angle before having
                            // to place a dummy point to prevent tangents
  const double upperClipValue =
      range.upper +
      (reversed
           ? 0
           : range.size() * 0.05 +
                 clipMargin);  // clip slightly outside of actual range to avoid
                               // line thicknesses to peek into visible circle
  const double lowerClipValue =
      range.lower -
      (reversed ? range.size() * 0.05 + clipMargin
                : 0);  // clip slightly outside of actual range to avoid line
                       // thicknesses to peek into visible circle
  const double maxKeySkip =
      qAsin(qSqrt(clipMargin * (clipMargin + 2 * range.size())) /
            (range.size() + clipMargin)) /
      M_PI *
      mKeyAxis->range()
          .size();  // the maximum angle between two points on outer circle
                    // (r=clipValue+clipMargin) before connecting line becomes
                    // tangent to inner circle (r=clipValue)
  double skipBegin = 0;
  bool belowRange = false;
  bool aboveRange = false;
  QCPGraphDataContainer::const_iterator it = begin;
  while (it != end) {
    if (it->value < lowerClipValue) {
      if (aboveRange)  // jumped directly from above to below visible range,
      // draw previous point so entry angle is correct
      {
        aboveRange = false;
        if (!reversed)  // TODO: with inner radius, we'll need else case here
          // with projected border point
          lineData->append(*(it - 1));
      }
      if (!belowRange) {
        skipBegin = it->key;
        lineData->append(QCPGraphData(it->key, lowerClipValue));
        belowRange = true;
      }
      if (it->key - skipBegin >
          maxKeySkip)  // add dummy point if we're exceeding the maximum
      // skippable angle (to prevent unintentional
      // intersections with visible circle)
      {
        skipBegin += maxKeySkip;
        lineData->append(QCPGraphData(skipBegin, lowerClipValue));
      }
    } else if (it->value > upperClipValue) {
      if (belowRange)  // jumped directly from below to above visible range,
      // draw previous point so entry angle is correct (if
      // lower means outer, so if reversed axis)
      {
        belowRange = false;
        if (reversed) lineData->append(*(it - 1));
      }
      if (!aboveRange) {
        skipBegin = it->key;
        lineData->append(QCPGraphData(it->key, upperClipValue));
        aboveRange = true;
      }
      if (it->key - skipBegin >
          maxKeySkip)  // add dummy point if we're exceeding the maximum
      // skippable angle (to prevent unintentional
      // intersections with visible circle)
      {
        skipBegin += maxKeySkip;
        lineData->append(QCPGraphData(skipBegin, upperClipValue));
      }
    } else  // value within bounds where we don't optimize away points
    {
      if (aboveRange) {
        aboveRange = false;
        if (!reversed)
          lineData->append(
              *(it - 1));  // just entered from above, draw previous point so
                           // entry angle is correct (if above means outer, so
                           // if not reversed axis)
      }
      if (belowRange) {
        belowRange = false;
        if (reversed)
          lineData->append(
              *(it - 1));  // just entered from below, draw previous point so
                           // entry angle is correct (if below means outer, so
                           // if reversed axis)
      }
      lineData->append(*it);  // inside visible circle, add point normally
    }
    ++it;
  }
  // to make fill not erratic, add last point normally if it was outside visible
  // circle:
  if (aboveRange) {
    aboveRange = false;
    if (!reversed)
      lineData->append(
          *(it -
            1));  // just entered from above, draw previous point so entry angle
                  // is correct (if above means outer, so if not reversed axis)
  }
  if (belowRange) {
    belowRange = false;
    if (reversed)
      lineData->append(
          *(it -
            1));  // just entered from below, draw previous point so entry angle
                  // is correct (if below means outer, so if reversed axis)
  }
}

void QCPPolarGraph::getOptimizedScatterData(
    QVector<QCPGraphData> *scatterData,
    QCPGraphDataContainer::const_iterator begin,
    QCPGraphDataContainer::const_iterator end) const {
  scatterData->clear();

  const QCPRange range = mValueAxis->range();
  bool reversed = mValueAxis->rangeReversed();
  const double clipMargin = range.size() * 0.05;
  const double upperClipValue =
      range.upper +
      (reversed
           ? 0
           : clipMargin);  // clip slightly outside of actual range to avoid
                           // scatter size to peek into visible circle
  const double lowerClipValue =
      range.lower -
      (reversed ? clipMargin
                : 0);  // clip slightly outside of actual range to avoid scatter
                       // size to peek into visible circle
  QCPGraphDataContainer::const_iterator it = begin;
  while (it != end) {
    if (it->value > lowerClipValue && it->value < upperClipValue)
      scatterData->append(*it);
    ++it;
  }
}

/*! \internal

  Takes raw data points in plot coordinates as \a data, and returns a vector
  containing pixel coordinate points which are suitable for drawing the line
  style \ref lsLine.

  The source of \a data is usually \ref getOptimizedLineData, and this method is
  called in \a getLines if the line style is set accordingly.

  \see dataToStepLeftLines, dataToStepRightLines, dataToStepCenterLines,
  dataToImpulseLines, getLines, drawLinePlot
*/
QVector<QPointF> QCPPolarGraph::dataToLines(
    const QVector<QCPGraphData> &data) const {
  QVector<QPointF> result;
  QCPPolarAxisAngular *keyAxis = mKeyAxis.data();
  QCPPolarAxisRadial *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) {
    qDebug() << Q_FUNC_INFO << "invalid key or value axis";
    return result;
  }

  // transform data points to pixels:
  result.resize(data.size());
  for (int i = 0; i < data.size(); ++i)
    result[i] = mValueAxis->coordToPixel(data.at(i).key, data.at(i).value);
  return result;
}
/* end of 'src/polar/polargraph.cpp' */
