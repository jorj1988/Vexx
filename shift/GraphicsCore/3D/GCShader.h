#ifndef GCSHADER_H
#define GCSHADER_H

#include "GCGlobal.h"
#include "sentity.h"
#include "sbaseproperties.h"
#include "sbasepointerproperties.h"
#include "XShader.h"

class GCShaderBindableData : public SStaticInterfaceBase
  {
  S_STATIC_INTERFACE_TYPE(GCShaderBindableData, GCShaderBindableInterface)
public:
  GCShaderBindableData(bool deleteOnNoReferences) : SStaticInterfaceBase(deleteOnNoReferences) { }
  virtual void bindData(XShader *, SProperty *) const = 0;
  };

class GRAPHICSCORE_EXPORT GCShaderComponent : public SEntity
  {
  S_ENTITY(GCShaderComponent, SEntity, 0)
public:
  GCShaderComponent();

  StringProperty source;
  };

class GRAPHICSCORE_EXPORT GCFragmentShaderComponent : public GCShaderComponent
  {
  S_ENTITY(GCFragmentShaderComponent, GCShaderComponent, 0)
  };

class GRAPHICSCORE_EXPORT GCVertexShaderComponent : public GCShaderComponent
  {
  S_ENTITY(GCVertexShaderComponent, GCShaderComponent, 0)
  };

S_TYPED_POINTER_TYPE(GCShaderComponentPointer, GCShaderComponent);

S_TYPED_POINTER_ARRAY_TYPE(GCShaderComponentPointerArray, GCShaderComponentPointer);

DEFINE_POD_PROPERTY(GRAPHICSCORE_EXPORT, GCRuntimeShader, XShader, XShader())

class GRAPHICSCORE_EXPORT GCShader : public SEntity
  {
  S_ENTITY(GCShader, SEntity, 0)
public:
  GCShader();

  void bind(XRenderer *r) const;

  GCShaderComponentPointerArray components;
  GCRuntimeShader runtimeShader;

  static void postChildSet(SPropertyContainer *, SProperty *);

private:
  bool _rebuildShader;
  bool _setVariables;
  void computeShaderRuntime(const SPropertyInstanceInformation *info, SPropertyContainer *cont);
  };

S_TYPED_POINTER_TYPE(GCShaderPointer, GCShader);

#endif // GCSHADER_H
