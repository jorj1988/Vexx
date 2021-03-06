#ifndef SPROPERTYINFORMATION_H
#define SPROPERTYINFORMATION_H

#include "QString"
#include "sglobal.h"
#include "XProperty"
#include "XHash"
#include "QVariant"

class SProperty;
class SLoader;
class SSaver;
class SPropertyContainer;
class SPropertyInformation;

namespace std
{
  template <typename T> class initializer_list;
}

// Child information
class SHIFT_EXPORT SPropertyInstanceInformation
  {
public:
  typedef void (*ComputeFunction)( const SPropertyInstanceInformation *, SPropertyContainer * );
  typedef void (*QueueComputeFunction)( const SPropertyInstanceInformation *, const SPropertyContainer *, SProperty **, xsize &numJobs );

  typedef xuint16 DataKey;
  typedef XHash<DataKey, QVariant> DataHash;

XProperties:
  XProperty(const SPropertyInformation *, childInformation, setChildInformation);
  XRefProperty(QString, name);
  XProperty(SProperty SPropertyContainer::*, location, setLocation);
  XProperty(ComputeFunction, compute, setCompute);
  XProperty(QueueComputeFunction, queueCompute, setQueueCompute);
  XROProperty(SProperty SPropertyContainer::* *, affects);
  // this index is internal to this instance information only
  XProperty(xsize, index, setIndex);
  XProperty(bool, entityChild, setEntityChild);
  XProperty(bool, extra, setExtra);
  XProperty(bool, dynamic, setDynamic);
  XRORefProperty(DataHash, data);

public:
  // extra properties indicate that whilst they are contained within the type itself, the constuctor does not
  // initiate or destroy them, and that the Database should handle this.
  SPropertyInstanceInformation();

  void setAffects(SPropertyInstanceInformation *info);
  void setAffects(SProperty SPropertyContainer::* *affects);

  virtual void initiateProperty(SProperty *X_UNUSED(propertyToInitiate)) const { }
  static DataKey newDataKey();

  bool isComputed() const { return _compute != 0; }
  bool affectsSiblings() const { return _affects != 0; }

  void setData(DataKey, const QVariant &);

  X_ALIGNED_OPERATOR_NEW

private:
  void initiate(const SPropertyInformation *info,
                const QString &name,
                xsize index,
                SProperty SPropertyContainer::* location);

  friend class SProperty;
  friend class SPropertyContainer;
  friend class SPropertyInformation;
  static void defaultQueue(const SPropertyInstanceInformation *, const SPropertyContainer *, SProperty **, xsize &numJobs);
  };

class SHIFT_EXPORT SPropertyInformation
  {
public:
  typedef void (*CreateFunction)(void *data, const SPropertyInformation *type, SPropertyInstanceInformation **info);
  typedef SPropertyInstanceInformation *(*CreateInstanceInformationFunction)(const SPropertyInformation *type,
                                                                            const QString &name,
                                                                            xsize index,
                                                                            SProperty SPropertyContainer::* location);
  typedef void (*SaveFunction)( const SProperty *, SSaver & );
  typedef SProperty *(*LoadFunction)( SPropertyContainer *, SLoader & );
  typedef void (*AssignFunction)( const SProperty *, SProperty * );

  typedef xuint16 DataKey;
  typedef XHash<DataKey, QVariant> DataHash;

XProperties:
  XProperty(CreateFunction, create, setCreate);
  XProperty(CreateInstanceInformationFunction, createInstanceInformation, setCreateInstanceInformation);
  XProperty(SaveFunction, save, setSave);
  XProperty(LoadFunction, load, setLoad);
  XProperty(AssignFunction, assign, setAssign);

  XProperty(xuint32, version, setVersion);

  XRefProperty(QString, typeName);

  XProperty(const SPropertyInformation *, parentTypeInformation, setParentTypeInformation);

  XRefProperty(XList<SPropertyInstanceInformation*>, children);
  XProperty(xsize, size, setSize);
  XProperty(xsize, instanceInformationSize, setInstanceInformationSize);
  XProperty(bool, dynamic, seyDynamic);

  XRORefProperty(DataHash, data);

  XROProperty(xsize, instances);

public:
  template <typename PropType> static SPropertyInformation *create(const QString &typeName)
    {
    SPropertyInformation *info = PropType::ParentType::createTypeInformation();

    info->setCreate(PropType::createProperty);
    info->setCreateInstanceInformation(createInstanceInformation<PropType>);
    info->setSave(PropType::saveProperty);
    info->setLoad(PropType::loadProperty);
    info->setAssign(PropType::assignProperty);
    info->setVersion(PropType::Version);
    info->typeName() = typeName;
    info->setParentTypeInformation(PropType::ParentType::staticTypeInformation());
    info->setSize(sizeof(PropType));
    info->setInstanceInformationSize(sizeof(typename PropType::InstanceInformation));

    return info;
    }

  template <typename PropType> static SPropertyInformation *createNoParent(const QString &typeName)
    {
    SPropertyInformation *info = new SPropertyInformation();

    info->setCreate(PropType::createProperty);
    info->setCreateInstanceInformation(createInstanceInformation<PropType>);
    info->setSave(PropType::saveProperty);
    info->setLoad(PropType::loadProperty);
    info->setAssign(PropType::assignProperty);
    info->setVersion(PropType::Version);
    info->typeName() = typeName;
    info->setSize(sizeof(PropType));
    info->setInstanceInformationSize(sizeof(typename PropType::InstanceInformation));

    return info;
    }

  SPropertyInformation();
  SPropertyInformation(const SPropertyInformation &info);

  ~SPropertyInformation();

  static DataKey newDataKey();

  template <typename T> bool inheritsFromType() const
    {
    return inheritsFromType(T::Type);
    }

  bool inheritsFromType(const SPropertyInformation *type) const;

  // this classes children count
  xsize childCount() const { return children().size(); }

  template <typename T, typename U>
  typename U::InstanceInformation *child(U T::* location)
    { return static_cast<typename U::InstanceInformation*>(child(reinterpret_cast<SProperty SPropertyContainer::*>(location))); }

  template <typename T, typename U>
  const typename U::InstanceInformation *child(U T::* location) const
    { return static_cast<const typename U::InstanceInformation*>(child(reinterpret_cast<SProperty SPropertyContainer::*>(location))); }

  // access the properties from offset of member
  SPropertyInstanceInformation *child(SProperty SPropertyContainer::* location);
  const SPropertyInstanceInformation *child(SProperty SPropertyContainer::* location) const;

  // access child instance information
  SPropertyInstanceInformation *child(xsize index);
  const SPropertyInstanceInformation *child(xsize index) const;

  // size of the property type, and its instance information
  xsize dynamicSize() const { return size() + instanceInformationSize() + X_ALIGN_BYTE_COUNT; }

  template <typename T, typename U> typename U::InstanceInformation *add(U T::* ptr, const QString &name, typename U::InstanceInformation *def=0)
    {
    if(!def)
      {
      def = new typename U::InstanceInformation;
      }

    def->initiate(U::staticTypeInformation(), name, _children.size(), reinterpret_cast<SProperty SPropertyContainer::*>(ptr));

    _children << def;
    return def;
    }

  X_ALIGNED_OPERATOR_NEW

private:
  template <typename T>
    static SPropertyInstanceInformation *createInstanceInformation(const SPropertyInformation *type,
      const QString &name,
      xsize index,
      SProperty SPropertyContainer::* location)
    {
    typename T::InstanceInformation* def = new typename T::InstanceInformation;
    def->initiate(type, name, index, location);
    return def;
    }

  void reference() const;
  void dereference() const;
  friend class SDatabase;
};


#endif // SPROPERTYINFORMATION_H
