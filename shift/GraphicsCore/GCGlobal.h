#ifndef GCGLOBAL_H
#define GCGLOBAL_H

#include "XGlobal"
#include "sglobal.h"

#ifdef SHIFTGRAPHICSCORE_BUILD
# define GRAPHICSCORE_EXPORT X_DECL_EXPORT
#else
# define GRAPHICSCORE_EXPORT X_DECL_IMPORT
#endif

#define GRAPHICSCORE_TYPE(name, id) S_PROPERTY_TYPE(name, 500, id)

GRAPHICSCORE_TYPE(GCImageChannelData, 1);
GRAPHICSCORE_TYPE(GCImageChannel, 2);
GRAPHICSCORE_TYPE(GCImage, 3);

GRAPHICSCORE_TYPE(GCQImage, 3)

#endif // GCGLOBAL_H
