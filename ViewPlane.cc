#include "ViewPlane.h"

ViewPlane::ViewPlane(double pixelWidth, double pixelHeight,
  int horizontalResolution, int verticalResolution)
  : pixelHeight_(pixelHeight), pixelWidth_(pixelWidth),
    horizontalResolution_(horizontalResolution),
    verticalResolution_(verticalResolution)
{
}