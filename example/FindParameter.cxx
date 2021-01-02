#include <cstdlib>
#include <iostream>
#include <memory>

#include "CubicInterpolation/BicubicSplines.h"
#include "CubicInterpolation/CubicSplines.h"
#include "CubicInterpolation/Interpolant.h"
#include "CubicInterpolation/Axis.h"

float func1dim(float x) { return x * x + x; }
float func2dim(float x, float y) { return x * x + y * y; }

int main() {
  auto def1d = cubic_splines::CubicSplines::Definition();
  def1d.f = func1dim;
  def1d.axis = std::make_unique<cubic_splines::LinAxis>(-2.f, 2.f, (size_t)10);
  auto inter1d = cubic_splines::Interpolant<cubic_splines::CubicSplines>(std::move(def1d), "", "");
  auto function_value = 1.f;
  auto guess = 0.5f;

  auto x = find_parameter(inter1d, function_value, guess);
  std::cout << "f(x) = " << function_value << " -> x = " << x << std::endl;

  auto def2d = cubic_splines::BicubicSplines::Definition();
  def2d.f = func2dim;
  def2d.axis[0] = std::make_unique<cubic_splines::LinAxis>(1.e-5f, 1.e1f, (size_t)10);
  def2d.axis[1] = std::make_unique<cubic_splines::LinAxis>(1.e-5f, 1.e1f, (size_t)10);
  auto inter2d = cubic_splines::Interpolant<cubic_splines::BicubicSplines>(std::move(def2d), "", "");

  function_value = 42.f;
  auto x_val = 0.5f;
  auto y_guess = 3.f;
  auto searched_param = 1;
  auto y = cubic_splines::find_parameter(inter2d, function_value,
                    std::array<float, 2>{x_val, y_guess}, searched_param);
  std::cout << "f(x="<< x_val <<",y) = " << function_value << " -> y = " << y << std::endl;
  return 0;
}