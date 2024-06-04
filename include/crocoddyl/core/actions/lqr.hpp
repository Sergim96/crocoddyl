///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2019-2024, LAAS-CNRS, University of Edinburgh
//                          Heriot-Watt University
// Copyright note valid unless otherwise stated in individual files.
// All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef CROCODDYL_CORE_ACTIONS_LQR_HPP_
#define CROCODDYL_CORE_ACTIONS_LQR_HPP_

#include <stdexcept>

#include "crocoddyl/core/action-base.hpp"
#include "crocoddyl/core/fwd.hpp"
#include "crocoddyl/core/states/euclidean.hpp"

namespace crocoddyl {

template <typename _Scalar>
class ActionModelLQRTpl : public ActionModelAbstractTpl<_Scalar> {
 public:
  typedef _Scalar Scalar;
  typedef ActionDataAbstractTpl<Scalar> ActionDataAbstract;
  typedef ActionModelAbstractTpl<Scalar> Base;
  typedef ActionDataLQRTpl<Scalar> Data;
  typedef StateVectorTpl<Scalar> StateVector;
  typedef MathBaseTpl<Scalar> MathBase;
  typedef typename MathBase::VectorXs VectorXs;
  typedef typename MathBase::MatrixXs MatrixXs;

  ActionModelLQRTpl(const std::size_t nx, const std::size_t nu,
                    const bool drift_free = true);
  virtual ~ActionModelLQRTpl();

  virtual void calc(const boost::shared_ptr<ActionDataAbstract>& data,
                    const Eigen::Ref<const VectorXs>& x,
                    const Eigen::Ref<const VectorXs>& u);
  virtual void calc(const boost::shared_ptr<ActionDataAbstract>& data,
                    const Eigen::Ref<const VectorXs>& x);
  virtual void calcDiff(const boost::shared_ptr<ActionDataAbstract>& data,
                        const Eigen::Ref<const VectorXs>& x,
                        const Eigen::Ref<const VectorXs>& u);
  virtual void calcDiff(const boost::shared_ptr<ActionDataAbstract>& data,
                        const Eigen::Ref<const VectorXs>& x);
  virtual boost::shared_ptr<ActionDataAbstract> createData();
  virtual bool checkData(const boost::shared_ptr<ActionDataAbstract>& data);

  const MatrixXs& get_Fx() const;
  const MatrixXs& get_Fu() const;
  const VectorXs& get_f0() const;
  const VectorXs& get_lx() const;
  const VectorXs& get_lu() const;
  const MatrixXs& get_Lxx() const;
  const MatrixXs& get_Lxu() const;
  const MatrixXs& get_Luu() const;

  void set_Fx(const MatrixXs& Fx);
  void set_Fu(const MatrixXs& Fu);
  void set_f0(const VectorXs& f0);
  void set_lx(const VectorXs& lx);
  void set_lu(const VectorXs& lu);
  void set_Lxx(const MatrixXs& Lxx);
  void set_Lxu(const MatrixXs& Lxu);
  void set_Luu(const MatrixXs& Luu);

  /**
   * @brief Print relevant information of the LQR model
   *
   * @param[out] os  Output stream object
   */
  virtual void print(std::ostream& os) const;

 protected:
  using Base::nu_;     //!< Control dimension
  using Base::state_;  //!< Model of the state

 private:
  bool drift_free_;
  MatrixXs A_;
  MatrixXs B_;
  MatrixXs Q_;
  MatrixXs R_;
  MatrixXs N_;
  VectorXs f_;
  VectorXs q_;
  VectorXs r_;
};

template <typename _Scalar>
struct ActionDataLQRTpl : public ActionDataAbstractTpl<_Scalar> {
  typedef _Scalar Scalar;
  typedef MathBaseTpl<Scalar> MathBase;
  typedef ActionDataAbstractTpl<Scalar> Base;
  typedef typename MathBase::VectorXs VectorXs;

  template <template <typename Scalar> class Model>
  explicit ActionDataLQRTpl(Model<Scalar>* const model)
      : Base(model),
        R_u_tmp(VectorXs::Zero(static_cast<Eigen::Index>(model->get_nu()))),
        Q_x_tmp(VectorXs::Zero(
            static_cast<Eigen::Index>(model->get_state()->get_ndx()))) {
    // Setting the linear model and quadratic cost here because they are
    // constant
    Fx = model->get_Fx();
    Fu = model->get_Fu();
    Lxx = model->get_Lxx();
    Luu = model->get_Luu();
    Lxu = model->get_Lxu();
  }

  using Base::cost;
  using Base::Fu;
  using Base::Fx;
  using Base::Lu;
  using Base::Luu;
  using Base::Lx;
  using Base::Lxu;
  using Base::Lxx;
  using Base::r;
  using Base::xnext;
  VectorXs R_u_tmp;  // Temporary variable for storing Hessian-vector product
                     // (size: nu)
  VectorXs Q_x_tmp;  // Temporary variable for storing Hessian-vector product
                     // (size: nx)
};

}  // namespace crocoddyl

/* --- Details -------------------------------------------------------------- */
/* --- Details -------------------------------------------------------------- */
/* --- Details -------------------------------------------------------------- */
#include "crocoddyl/core/actions/lqr.hxx"

#endif  // CROCODDYL_CORE_ACTIONS_LQR_HPP_
