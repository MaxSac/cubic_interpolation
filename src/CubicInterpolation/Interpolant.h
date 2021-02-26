#pragma once

#include "CubicInterpolation/Interpolant.hpp"

#include <string>
#include <type_traits>
#include <utility>

namespace cubic_splines {

/**
 * @brief Utility class for a better handling of interpolants. Take care about
 * transformation of axis building, loading and storage of intepolation tables.
 */
template <typename T1, typename T2 = typename T1::Definition> class Interpolant {
  T2 def;
  T1 inter;

  template <typename... Args> inline auto transform(Args &&...args) const {
    return detail::transform(std::forward<Args>(args)...);
  }

  template <typename... Args> inline auto back_transform(Args &&...args) const {
    return detail::back_transform(std::forward<Args>(args)...);
  }

  template <typename... Args> inline auto back_transform_prime(Args &&...args) const {
    return detail::back_transform_prime(args...);
  }

public:
  /**
   * @brief To initaialize a Interpolant the corresponding definition to the
   * Interpolant type is required and a path where to store the tables, as
   * well as a filename. If no path and filename is specified, tables will not
   * be stored and a warning thrown because they cann't be reused.
   */
  Interpolant(T2 &&_def, std::string _path = "", std::string _filename = "")
      : def(std::forward<T2>(_def)), inter(def, _path, _filename) {}

  /**
   * @brief Evaluation of the interpolant, takeing axis and function value
   * trafo into account. If a one dimensional interpolant is evaluated, a
   * doubleingpoint is required. Otherwise a iterable container with values
   * stored in same order as axis is required.
   */
  template <typename T> inline auto evaluate(T x) const {
    auto val = inter.evaluate(transform(def.GetAxis(), x));
    return back_transform(def.f_trafo.get(), val);
  }

  /**
   * @brief First derive of the interpolant, takeing axis and function value
   * trafo into account. If a one dimensional interpolant is evaluated, a
   * doubleingpoint is required. Otherwise a iterable container with values
   * stored in same order as axis is required.
   */
  template <typename T> inline auto prime(T x) const {
    auto f = inter.evaluate(transform(def.GetAxis(), x));
    auto df = inter.prime(transform(def.GetAxis(), x));
    return back_transform_prime(def.f_trafo.get(), def.GetAxis(), f, df, x);
  }

  /**
   * @brief Definition of interpolant.
   */
  T2 const &GetDefinition() const { return def; };
};

/**
 * @brief Inverts the interpolation evaluation and searches the missing axis-
 * for the given function-value. If a n > 1 dimensional interpolant is passed,
 * the parameterspace must be narrowed. The function values except the searched
 * dimension must be specified and a guess must be given in place of the
 * container.
 *
 * @param interpolant interpolant object required for calculation
 * @param val function value
 * @param x axis values including guess of the parameter to estimate. If
 * parameter to estimate is set to `nan`, the parameterspace for the missing
 * axis value will be shrinked automatically the searched xaxis value.
 * @param n (only if dim > 1) n-th axis to estimate the value
 */
template <typename... Args> auto find_parameter(Args &&...args) {
  return detail::find_parameter(std::forward<Args>(args)...);
}
} // namespace cubic_splines
