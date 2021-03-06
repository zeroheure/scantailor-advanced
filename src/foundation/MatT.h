// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#ifndef SCANTAILOR_FOUNDATION_MATT_H_
#define SCANTAILOR_FOUNDATION_MATT_H_

#include <boost/scoped_array.hpp>
#include <cassert>
#include <cstddef>

/**
 * \brief A matrix of elements of type T.
 *
 * \note The memory layout is always column-major, as that's what MatrixCalc uses.
 */
template <typename T>
class MatT {
 public:
  using type = T;

  /**
   * \brief Constructs an empty 0x0 matrix.
   */
  MatT();

  /**
   * \brief Constructs a (rows)x(cols) matrix, initializing all elements to T().
   */
  MatT(size_t rows, size_t cols);

  /**
   * \brief Constructs a (rows)x(cols) matrix, initializing all elements to the provided value.
   */
  MatT(size_t rows, size_t cols, T initialValue);

  /**
   * \brief Construction from an array of elements of possibly different type.
   *
   * Conversion is done by static casts.  Data elements must be in column-major order.
   */
  template <typename OT>
  explicit MatT(size_t rows, size_t cols, const OT* data);

  /**
   * Ordinary copy-construction.
   */
  MatT(const MatT& other);

  /**
   * \brief Construction from a matrix of a different type.
   *
   * Conversion is done by static casts.
   */
  template <typename OT>
  explicit MatT(const MatT<OT>& other);

  /**
   * \brief Ordinary assignment.
   */
  MatT& operator=(const MatT& other);

  /**
   * \brief Assignment from a matrix of a different type.
   *
   * Conversion is done by static casts.
   */
  template <typename OT>
  MatT& operator=(const MatT<OT>& other);

  MatT& operator+=(const MatT& rhs);

  MatT& operator-=(const MatT& rhs);

  MatT& operator*=(T scalar);

  size_t rows() const { return m_rows; }

  size_t cols() const { return m_cols; }

  const T* data() const { return m_data.get(); }

  T* data() { return m_data.get(); }

  const T& operator()(size_t row, size_t col) const {
    assert(row < m_rows && col < m_cols);
    return m_data[row + col * m_rows];
  }

  T& operator()(size_t row, size_t col) {
    assert(row < m_rows && col < m_cols);
    return m_data[row + col * m_rows];
  }

  void fill(const T& value);

  void swap(MatT& other);

 private:
  size_t m_rows;
  size_t m_cols;
  boost::scoped_array<T> m_data;
};


template <typename T>
MatT<T>::MatT() : m_rows(0), m_cols(0) {}

template <typename T>
MatT<T>::MatT(size_t rows, size_t cols)
    : m_rows(rows),
      m_cols(cols),
      m_data(new T[rows * cols]()) {  // The "()" will cause elements to be initialized to T().
}

template <typename T>
MatT<T>::MatT(size_t rows, size_t cols, T initialValue) : m_rows(rows), m_cols(cols), m_data(new T[rows * cols]) {
  const size_t len = rows * cols;
  for (size_t i = 0; i < len; ++i) {
    m_data[i] = initialValue;
  }
}

template <typename T>
template <typename OT>
MatT<T>::MatT(size_t rows, size_t cols, const OT* data) : m_rows(rows), m_cols(cols), m_data(new T[rows * cols]) {
  const size_t len = rows * cols;
  for (size_t i = 0; i < len; ++i) {
    m_data[i] = static_cast<T>(data[i]);
  }
}

template <typename T>
MatT<T>::MatT(const MatT& other) : m_rows(other.rows()), m_cols(other.cols()), m_data(new T[m_rows * m_cols]) {
  const size_t len = m_rows * m_cols;
  const T* otherData = other.data();
  for (size_t i = 0; i < len; ++i) {
    m_data[i] = otherData[i];
  }
}

template <typename T>
template <typename OT>
MatT<T>::MatT(const MatT<OT>& other) : m_rows(other.rows()), m_cols(other.cols()), m_data(new T[m_rows * m_cols]) {
  const size_t len = m_rows * m_cols;
  const T* otherData = other.data();
  for (size_t i = 0; i < len; ++i) {
    m_data[i] = otherData[i];
  }
}

template <typename T>
MatT<T>& MatT<T>::operator=(const MatT& other) {
  MatT(other).swap(*this);
  return *this;
}

template <typename T>
template <typename OT>
MatT<T>& MatT<T>::operator=(const MatT<OT>& other) {
  MatT(other).swap(*this);
  return *this;
}

template <typename T>
MatT<T>& MatT<T>::operator+=(const MatT& rhs) {
  assert(m_rows == rhs.m_rows && m_cols == rhs.m_cols);

  const size_t len = m_rows * m_cols;
  for (size_t i = 0; i < len; ++i) {
    m_data[i] += rhs.m_data[i];
  }
  return *this;
}

template <typename T>
MatT<T>& MatT<T>::operator-=(const MatT& rhs) {
  assert(m_rows == rhs.m_rows && m_cols == rhs.m_cols);

  const size_t len = m_rows * m_cols;
  for (size_t i = 0; i < len; ++i) {
    m_data[i] -= rhs.m_data[i];
  }
  return *this;
}

template <typename T>
MatT<T>& MatT<T>::operator*=(const T scalar) {
  const size_t len = m_rows * m_cols;
  for (size_t i = 0; i < len; ++i) {
    m_data[i] *= scalar;
  }
  return *this;
}

template <typename T>
void MatT<T>::fill(const T& value) {
  const size_t len = m_rows * m_cols;
  for (size_t i = 0; i < len; ++i) {
    m_data[i] = value;
  }
}

template <typename T>
void MatT<T>::swap(MatT& other) {
  size_t tmp = m_rows;
  m_rows = other.m_rows;
  other.m_rows = tmp;

  tmp = m_cols;
  m_cols = other.m_cols;
  other.m_cols = tmp;

  m_data.swap(other.m_data);
}

template <typename T>
void swap(const MatT<T>& o1, const MatT<T>& o2) {
  o1.swap(o2);
}

template <typename T>
MatT<T> operator*(const MatT<T>& mat, double scalar) {
  MatT<T> res(mat);
  res *= scalar;
  return res;
}

template <typename T>
MatT<T> operator*(double scalar, const MatT<T>& mat) {
  MatT<T> res(mat);
  res *= scalar;
  return res;
}

#endif  // ifndef SCANTAILOR_FOUNDATION_MATT_H_
