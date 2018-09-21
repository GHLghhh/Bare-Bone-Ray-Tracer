#pragma once

class ViewPlane {
public:
  ViewPlane() = default;
  ViewPlane(double pixelWidth, double pixelHeight,
    int horizontalResolution, int verticalResolution);
  double PixelHeight() const { return pixelHeight_; };
  double PixelWidth() const { return pixelWidth_; };
  int HorizontalResolution() const { return horizontalResolution_; };
  int VerticalResolution() const { return verticalResolution_; };

private:
  double pixelHeight_;
  double pixelWidth_;
  int verticalResolution_;
  int horizontalResolution_;
};