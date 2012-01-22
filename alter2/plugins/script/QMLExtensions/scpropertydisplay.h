#ifndef SCPROPERTYDISPLAY_H
#define SCPROPERTYDISPLAY_H

#include <QDeclarativeItem>

class ScPropertyItem;
class QDeclarativeComponent;
class SProperty;
class SPropertyContainer;

class ScPropertyDisplay : public QDeclarativeItem
  {
  Q_OBJECT

  Q_PROPERTY(QDeclarativeComponent *property READ propertyComponent WRITE setPropertyComponent)
  Q_PROPERTY(ScPropertyItem *rootItem READ rootItem WRITE setRootItem)

public:
  explicit ScPropertyDisplay(QDeclarativeItem *parent = 0);
  ~ScPropertyDisplay();

  void setRootItem(ScPropertyItem *);
  ScPropertyItem *rootItem() const;

  QDeclarativeComponent *propertyComponent() const;
  void setPropertyComponent(QDeclarativeComponent *);

  bool onPropertyAdded(SProperty *);
  bool onPropertyRemoved(const SPropertyContainer *oldParent, SProperty *);

  const QVector <ScPropertyItem *> properties() { return _properties; }

signals:
  
public slots:

private slots:
  void updateConnectionParenting();
  void clear();
  void rebuild();

private:
  void addProperty(SProperty *p);
  void removeProperty(SProperty *p);
  ScPropertyItem *_parentItem;

  QDeclarativeComponent *_property;
  QVector <ScPropertyItem *> _properties;
  };

#endif // SCPROPERTYDISPLAY_H
