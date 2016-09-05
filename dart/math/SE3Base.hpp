/*
 * Copyright (c) 2016, Graphics Lab, Georgia Tech Research Corporation
 * Copyright (c) 2016, Humanoid Lab, Georgia Tech Research Corporation
 * Copyright (c) 2016, Personal Robotics Lab, Carnegie Mellon University
 * All rights reserved.
 *
 * This file is provided under the following "BSD-style" License:
 *   Redistribution and use in source and binary forms, with or
 *   without modification, are permitted provided that the following
 *   conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 *   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *   POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef DART_MATH_SE3BASE_HPP_
#define DART_MATH_SE3BASE_HPP_

#include <type_traits>

#include <Eigen/Eigen>

#include "dart/math/MathTypes.hpp"
#include "dart/math/Geometry.hpp"
#include "dart/math/SO3Base.hpp"
#include "dart/math/SO3AxisAngle.hpp"
#include "dart/math/SO3Quaternion.hpp"
#include "dart/math/SO3RotationMatrix.hpp"
#include "dart/math/SO3RotationVector.hpp"
#include "dart/math/detail/SE3Base.hpp"

namespace dart {
namespace math {

//struct SE3Representation {};

template <typename Derived>
class SE3Base
{
public:

  static constexpr int Dim = 6;

  /// The scalar type of the coefficients
  using S = typename detail::traits<Derived>::S;

  using RotationMatrixType = Eigen::Matrix<S, Dim, Dim>;
  using VectorType = Eigen::Matrix<S, Dim, 1>;

  /// The representation type of this SE(3)
  using Rep = typename detail::traits<Derived>::Rep;

  /// The data type for this SE(3) representation type
  using SO3Type = SO3<S, Rep>;
  using RotationType = SO3Type;
  using TranslationType = Eigen::Matrix<S, 3, 1>;
//  using RepDataType = typename detail::SE3::rep_traits<S, Rep>::RepDataType;

  using SE3Canonical = typename detail::traits<Derived>::SE3Canonical;

  /// The data type for the lie algebra of SE(3) called SE(3)
  using Tangent = Eigen::Matrix<S, Dim, 1>;
  using se3 = Tangent;

  /// Default constructor
  SE3Base() = default;

  /// Copy constructor
  SE3Base(const SE3Base&) = default;

  /// A reference to the derived object
  const Derived& derived() const
  {
    return *static_cast<const Derived*>(this);
  }

  /// A const reference to the derived object
  Derived& derived()
  {
    return *static_cast<Derived*>(this);
  }

//  /// Set this SE(3) from any kinds of SE(3) types
//  template <typename OtherDerived>
//  Derived& operator=(const SE3Base<OtherDerived>& other)
//  {
//    detail::SE3::assign_impl<S, Derived, OtherDerived>::run(
//          derived(), other.derived());

//    return derived();
//  }

//  /// Set this SE(3) from any kinds of SE(3) types
//  template <typename OtherDerived>
//  Derived& operator=(SE3Base<OtherDerived>&& other)
//  {
//    detail::SE3::assign_impl<S, Derived, OtherDerived>::run(
//          derived(), std::move(other.derived()));

//    return derived();
//  }

//  /// Set this SE(3) from the raw representation type
//  template <typename OtherDerived>
//  Derived& operator=(const Eigen::MatrixBase<OtherDerived>& mat)
//  {
//    {
//      using namespace Eigen;
//      EIGEN_STATIC_ASSERT_SAME_MATRIX_SIZE(RepDataType, OtherDerived)
//    }

//    derived().mRepData = mat;

//    return derived();
//  }

//  /// Set this SE(3) from the raw representation type
//  template <typename OtherDerived>
//  Derived& operator=(Eigen::MatrixBase<OtherDerived>&& matrix)
//  {
//    {
//      using namespace Eigen;
//      EIGEN_STATIC_ASSERT_SAME_MATRIX_SIZE(RepDataType, OtherDerived)
//    }

//    derived().mRepData = std::move(matrix);

//    return derived();
//  }

//  /// Group multiplication
//  template <typename OtherDerived>
//  const Derived operator*(const SE3Base<OtherDerived>& other) const
//  {
//    Derived result(derived());
//    result *= other;

//    return result;
//  }

//  /// In-place group multiplication
//  template <typename OtherDerived>
//  void operator*=(const SE3Base<OtherDerived>& other)
//  {
//    detail::SE3::group_inplace_multiplication_impl<Derived, OtherDerived>::run(
//          derived(), other.derived());
//  }

//  bool operator ==(const SE3Base& other)
//  {
//    return derived() == other.derived();
//  }

//  template <typename OtherDerived>
//  bool operator ==(const SE3Base<OtherDerived>& other)
//  {
//    return toRotationMatrix() == other.toRotationMatrix();
//  }

  void setIdentity()
  {
    derived().setIdentity();
  }

  static Derived Identity()
  {
    Derived I;
    I.setIdentity();

    return I;
  }

  bool isIdentity()
  {
    return derived().getRotation().isIdentity()
        && derived().getTranslation() == Eigen::Matrix<S, 3, 1>::Zero();
  }

  void invert()
  {
    derived().mRotation.invert();
    derived().mTranslation = derived().mRotation * -derived().mTranslation;
  }

  const Derived getInverse() const
  {
    RotationType inverse = derived().mRotation.getInverse();

    return Derived(inverse, inverse * -derived().mTranslation);
  }

  void setRandom()
  {
    derived().mRepData.setRandom();
    // TODO(JS): improve
  }

  static Derived Random()
  {
    Derived R;
    R.setRandom();

    return R;
  }

  SO3Type& getRotation()
  {
    return derived().mRotation;
  }

  const SO3Type& getRotation() const
  {
    return derived().getRotation();
  }

  TranslationType& getTranslation()
  {
    return derived().mTranslation;
  }

  const TranslationType& getTranslation() const
  {
    return derived().mTranslation;
  }

  template <typename OtherDerived>
  bool isApprox(const SE3Base<OtherDerived>& other, S tol = 1e-6) const
  {
    return getRotation().isApprox(other.getRotation(), tol)
        && getTranslation().isApprox(other.getTranslation(), tol);
  }

//  /// Inverse this SE(3).
//  void invert()
//  {
//    derived().invert();
//  }

//  /// Return the inversion of this SE(3). This SE(3) doesn't change itself.
//  Derived getInverse() const
//  {
//    return derived().getInverse();
//  }

//  // TODO(JS): add in-place inversion void inverse()

//  static Derived exp(const SE3& tangent)
//  {
//    return derived().exp(tangent);
//  }

//  static SE3 log(const Derived& point)
//  {
//    return derived().log(point);
//  }

//  static RotationMatrixType hat(const Tangent& angleAxis)
//  {
//    RotationMatrixType res;
//    res <<  static_cast<S>(0),     -angleAxis(2),      angleAxis(1),
//                 angleAxis(2), static_cast<S>(0),     -angleAxis(0),
//                -angleAxis(1),      angleAxis(0), static_cast<S>(0);

//    return res;
//  }

//  static Tangent vee(const RotationMatrixType& mat)
//  {
//    // TODO(JS): Add validity check if mat is skew-symmetric for debug mode
//    return Tangent(mat(2, 1), mat(0, 2), mat(1, 0));
//  }

//  RotationMatrixType toRotationMatrix() const
//  {
//    // We assume the canonical representation is the rotation matrix
//    return detail::SE3::rep_convert_to_canonical_impl<S, Rep>::run(
//          derived().matrix());
//  }

//  void fromRotationMatrix(const RotationMatrixType& rotMat)
//  {
//    // We assume the canonical representation is the rotation matrix
//    derived().matrix()
//        = detail::SE3::rep_convert_from_canonical_impl<S, Rep>::run(rotMat);
//  }

//  ///
//  template <typename RepTo>
//  typename detail::SE3::rep_traits<S, RepTo>::RepDataType coordinates() const
//  {
//    // TODO(JS): Check if the raw data of RepTo is a vector type.

//    return detail::SE3::rep_convert_impl<S, Rep, RepTo>::run(derived().mRepData);
//  }

//  /// Return a reference of the raw data of the representation type
//  RepDataType& matrix()
//  {
//    return derived().mRepData;
//    // TODO(JS): Note that we return the raw data of the representation type
//    // rather than rotation matrix here where the matrix size of the raw data
//    // can be different depending on the representation types.
//    // Eigen::RotationBase returns rotation matrix to be conform with the
//    // Eigen::Transform class's naming scheme. I'm not sure whether we should
//    // follow Eigen's policy since haven't seen any compelling reason to do so.
//    // I'm open to this issue.
//  }

//  /// Return a const reference of the raw data of the representation type
//  const RepDataType& matrix() const
//  {
//    return derived().mRepData;
//  }

//  /// \returns A pointer to the data array of internal data type
//  S* data()
//  {
//    return derived().data();
//  }

//  /// \returns the number of rows. \sa cols()
//  std::size_t rows() const
//  {
//    return matrix().rows();
//  }

//  /// \returns the number of columns. \sa rows()
//  std::size_t cols() const
//  {
//    return matrix().cols();
//  }

//  /// \returns the number of coefficients, which is rows()*cols().
//  /// \sa rows(), cols()
//  std::size_t size() const
//  {
//    return rows() * cols();
//  }

//  Canonical canonical()
//  {
//    return canonical(detail::SE3::group_is_canonical<Derived>());
//  }

//  const Canonical canonical() const
//  {
//    return canonical(detail::SE3::group_is_canonical<Derived>());
//  }

//  static constexpr bool isCanonical()
//  {
//    return detail::SE3::group_is_canonical<Derived>::value;
//  }

private:

//  Canonical canonical(std::true_type)
//  {
//    return derived();
//  }

//  const Canonical canonical(std::true_type) const
//  {
//    return derived();
//  }

//  Canonical canonical(std::false_type)
//  {
//    return typename detail::traits<Derived>::Canonical(derived());
//  }

//  const Canonical canonical(std::false_type) const
//  {
//    return typename detail::traits<Derived>::Canonical(derived());
//  }
};

} // namespace math
} // namespace dart

#endif // DART_MATH_SE3BASE_HPP_
