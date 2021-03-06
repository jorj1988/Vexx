#ifndef GCRENDERABLE_H
#define GCRENDERABLE_H

#include "GCGlobal.h"
#include "sentity.h"
#include "sbasepointerproperties.h"

class XRenderer;

class GRAPHICSCORE_EXPORT GCRenderable : public SEntity
  {
  S_ABSTRACT_PROPERTY(GCRenderable, SEntity, 0)

public:
  GCRenderable();

  virtual void render(XRenderer *) const;
  };

S_TYPED_POINTER_TYPE(GCRenderablePointer, GCRenderable)

#endif // GCRENDERABLE_H
