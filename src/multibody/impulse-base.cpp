///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2018-2019, LAAS-CNRS
// Copyright note valid unless otherwise stated in individual files.
// All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "crocoddyl/multibody/impulse-base.hpp"

namespace crocoddyl {

ImpulseModelAbstract::ImpulseModelAbstract(StateMultibody& state, unsigned int const& ni) : state_(state), ni_(ni) {}

ImpulseModelAbstract::~ImpulseModelAbstract() {}

void ImpulseModelAbstract::updateVelocity(const boost::shared_ptr<ImpulseDataAbstract>& data,
                                          const Eigen::VectorXd& vnext) const {
  assert(vnext.rows() == state_.get_nv() && "vnext has wrong dimension");
  data->vnext = vnext;
}

void ImpulseModelAbstract::updateVelocityDiff(const boost::shared_ptr<ImpulseDataAbstract>& data,
                                              const Eigen::MatrixXd& dvnext_dx) const {
  assert((dvnext_dx.rows() == state_.get_nv() && dvnext_dx.cols() == state_.get_ndx()) &&
         "dvnext_dx has wrong dimension");
  data->dvnext_dx = dvnext_dx;
}

void ImpulseModelAbstract::updateForceDiff(const boost::shared_ptr<ImpulseDataAbstract>& data,
                                           const Eigen::MatrixXd& df_dq) const {
  assert((df_dq.rows() == ni_ || df_dq.cols() == state_.get_nv()) && "df_dq has wrong dimension");
  data->df_dq = df_dq;
}

boost::shared_ptr<ImpulseDataAbstract> ImpulseModelAbstract::createData(pinocchio::Data* const data) {
  return boost::make_shared<ImpulseDataAbstract>(this, data);
}

StateMultibody& ImpulseModelAbstract::get_state() const { return state_; }

unsigned int const& ImpulseModelAbstract::get_ni() const { return ni_; }

}  // namespace crocoddyl
