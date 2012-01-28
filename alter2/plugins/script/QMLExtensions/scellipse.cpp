#include "scellipse.h"
#include "QPainter"
#include "QPixmapCache"

ScEllipse::ScEllipse(QDeclarativeItem *parent)
  : QDeclarativeItem(parent), _color(Qt::white), _gradient(0), _pen(0), _paintmargin(0)
  {
  // need to disable this flag to draw inside a QDeclarativeItem
  setFlag(QGraphicsItem::ItemHasNoContents, false);
  }

ScEllipse::~ScEllipse()
  {
  delete _pen;
  }

void ScEllipse::doUpdate()
  {
  _ellipseImage = QPixmap();
  const int pw = _pen && _pen->isValid() ? _pen->width() : 0;

  setPaintMargin((pw+1)/2);
  update();
  }

ScPen *ScEllipse::border()
  {
  return getPen();
  }

ScGradient *ScEllipse::gradient() const
  {
  return _gradient;
  }

void ScEllipse::setGradient(ScGradient *gradient)
  {
  if(_gradient == gradient)
    {
    return;
    }

  if(_gradient)
    {
    disconnect(_gradient, SIGNAL(updated()), this, SLOT(doUpdate()));
    }

  _gradient = gradient;

  if(_gradient)
    {
    connect(_gradient, SIGNAL(updated()), this, SLOT(doUpdate()));
    }

  update();
  }

QColor ScEllipse::color() const
  {
  return _color;
  }

void ScEllipse::setColor(const QColor &c)
  {
  if (_color == c)
    {
    return;
    }

  _color = c;
  _ellipseImage = QPixmap();
  update();
  emit colorChanged();
  }

void ScEllipse::generateBorderedEllipse()
  {
  if(_ellipseImage.isNull())
    {
    const int pw = _pen && _pen->isValid() ? _pen->width() : 0;

    QString key = QLatin1String("EL_") + QString::number(pw) + _color.name() + QString::number(_color.alpha(), 16) + QString::number(width()) + QString::number(height());
    if (_pen && _pen->isValid())
      {
      key += _pen->color().name() + QString::number(_pen->color().alpha(), 16);
      }

    if (!QPixmapCache::find(key, &_ellipseImage))
      {
      _ellipseImage = QPixmap(width(), height());
      _ellipseImage.fill(Qt::transparent);
      QPainter p(&(_ellipseImage));
      p.setRenderHint(QPainter::Antialiasing);
      if (_pen && _pen->isValid())
        {
        QPen pn(QColor(_pen->color()), _pen->width());
        pn.setJoinStyle(Qt::MiterJoin);
        p.setPen(pn);
        }
      else
        {
        p.setPen(Qt::NoPen);
        }
      p.setBrush(_color);

      if (pw%2)
        {
        p.drawEllipse(QRectF(qreal(pw)/2+1, qreal(pw)/2+1, _ellipseImage.width()-(pw+1), _ellipseImage.height()-(pw+1)));
        }
      else
        {
        p.drawEllipse(QRectF(qreal(pw)/2, qreal(pw)/2, _ellipseImage.width()-pw, _ellipseImage.height()-pw));
        }

      // end painting before inserting pixmap
      // to pixmap cache to avoid a deep copy
      p.end();
      QPixmapCache::insert(key, _ellipseImage);
      }
    }
  }

void ScEllipse::paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget *)
  {
  if (width() <= 0 || height() <= 0)
    {
    return;
    }

  if ((_pen && _pen->isValid()) || (_gradient && _gradient->gradient()))
    {
    drawEllipse(*p);
    }
  else
    {
    bool oldAA = p->testRenderHint(QPainter::Antialiasing);
    bool sm = smooth();
    if (sm)
      {
      p->setRenderHints(QPainter::Antialiasing, true);
      }

    p->setPen(Qt::transparent);
    p->setBrush(_color);

    p->drawEllipse(QRectF(0, 0, width(), height()));

    if (sm)
      {
      p->setRenderHint(QPainter::Antialiasing, oldAA);
      }
    }
  }

void ScEllipse::drawEllipse(QPainter &p)
  {
  if ((_gradient && _gradient->gradient()) || width() < 3 || height() < 3)
    {
    // XXX This path is still slower than the image path
    // Image path won't work for gradient though
    bool oldAA = p.testRenderHint(QPainter::Antialiasing);
    if(smooth())
      {
      p.setRenderHint(QPainter::Antialiasing);
      }

    if(_pen && _pen->isValid())
      {
      QPen pn(QColor(_pen->color()), _pen->width());
      pn.setJoinStyle(Qt::MiterJoin);
      p.setPen(pn);
      }
    else
      {
      p.setPen(Qt::NoPen);
      }

    if(_gradient && _gradient->gradient())
      {
      p.setBrush(*_gradient->gradient());
      }
    else
      {
      p.setBrush(_color);
      }

    const int pw = _pen && _pen->isValid() ? _pen->width() : 0;
    QRectF rect;
    if (pw%2)
      {
      rect = QRectF(0.5, 0.5, width()-1, height()-1);
      }
    else
      {
      rect = QRectF(0, 0, width(), height());
      }

    p.drawEllipse(rect);

    if (smooth())
      {
      p.setRenderHint(QPainter::Antialiasing, oldAA);
      }
    }
  else
    {
    bool sm = smooth();
    bool oldAA = p.testRenderHint(QPainter::Antialiasing);
    bool oldSmooth = p.testRenderHint(QPainter::SmoothPixmapTransform);
    if(sm)
      {
      p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, sm);
      }

    const int pw = _pen && _pen->isValid() ? (_pen->width()+1)/2*2 : 0;

    generateBorderedEllipse();

    p.drawPixmap(QPoint(-pw/2, -pw/2), _ellipseImage);

    if(sm)
      {
      p.setRenderHint(QPainter::Antialiasing, oldAA);
      p.setRenderHint(QPainter::SmoothPixmapTransform, oldSmooth);
      }
    }
  }

QRectF ScEllipse::boundingRect() const
  {
  return QRectF(-_paintmargin, -_paintmargin, width()+_paintmargin*2, height()+_paintmargin*2);
  }

ScPen *ScEllipse::getPen()
  {
  if(!_pen)
    {
    _pen = new ScPen;

    connect(_pen, SIGNAL(penChanged()), this, SLOT(doUpdate()));
    }

  return _pen;
  }

void ScEllipse::setPaintMargin(qreal margin)
  {
  if(margin == _paintmargin)
    {
    return;
    }

  update();
  _paintmargin = margin;
  }