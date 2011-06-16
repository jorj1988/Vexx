#ifndef SENTITY_H
#define SENTITY_H

#include "sglobal.h"
#include "spropertycontainer.h"
#include "spropertyarray.h"
#include "sobserver.h"
#include "XWeakShared"

class SProperty;
class SDatabase;
class STreeObserver;
class SDirtyObserver;
class SConnectionObserver;

#define S_ENTITY(name, parent, version) S_PROPERTY_CONTAINER(name, parent, version)

#define S_ENTITY_DEFINITION S_PROPERTY_CONTAINER_DEFINITION
#define S_ENTITY_END_DEFINITION(name, parent) S_PROPERTY_CONTAINER_END_DEFINITION(name, parent, saveEntity, loadEntity)

#define S_ENTITY_EMPTY_DEFINITION(name, parent) S_PROPERTY_CONTAINER_EMPTY_DEFINITION(name, parent, saveEntity, loadEntity)

#define S_ENTITY_COMPUTATION(function, ...)


class SHIFT_EXPORT SEntity : public SPropertyContainer, public XWeakSharedData
  {
  S_ENTITY(SEntity, SPropertyContainer, 0);

public:
  SEntity();

  SPropertyArray children;

  void reparent(SEntity *);

  template <typename T>T *addChild(const QString& name)
    {
    T *ent = children.add<T>();
    xAssert(ent);
    ent->setName(name);
    return ent;
    }

  SProperty *addChild(SPropertyType id, const QString& name)
    {
    SProperty *ent = children.add(id);
    xAssert(ent);
    ent->setName(name);
    return ent;
    }

  template <typename T>T *addProperty(const QString& name)
    {
    SProperty *p = addProperty(T::Type, name);
    xAssert(p);
    return p->uncheckedCastTo<T>();
    }

  SProperty *addProperty(SPropertyType t, const QString& name)
    {
    SProperty *p = SPropertyContainer::addProperty(t);
    xAssert(p);
    p->setName(name);
    return p;
    }

  void removeProperty(SProperty *prop)
    {
    SPropertyContainer::removeProperty(prop);
    }

  // accessing child properties by index
  using SPropertyContainer::at;

  SEntity *parentEntity() const;
  SEntity *firstChildEntity() const;
  SEntity *nextSiblingEntity() const;

  virtual bool acceptsChild(const SEntity *) const;
  virtual bool acceptsParent(const SEntity *) const;

  void addDirtyObserver(SDirtyObserver *);
  void addTreeObserver(STreeObserver *);
  void addConnectionObserver(SConnectionObserver *);

  void removeDirtyObserver(SDirtyObserver *);
  void removeTreeObserver(STreeObserver *);
  void removeConnectionObserver(SConnectionObserver *);
  
  void informDirtyObservers(SProperty *prop);
  void informTreeObservers(const SChange *event);
  void informConnectionObservers(const SChange *event);

  SEntity *findChildEntity(const QString &);
  const SEntity *findChildEntity(const QString &) const;

protected:
  static void saveEntity(const SProperty *, SSaver &);
  static SProperty *loadEntity(SPropertyContainer *, SLoader &);

private:
  struct ObserverStruct
    {
    enum
      {
      Dirty,
      Tree,
      Connection
      };
    xuint8 mode;
    SObserver *observer;
    };
  XList<ObserverStruct> _observers;
  };

#endif // SENTITY_H
