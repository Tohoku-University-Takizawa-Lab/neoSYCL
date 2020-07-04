//
// Created by WhiteBlue on 2020/6/22.
//

#ifndef SYCL_INCLUDE_CL_SYCL_HANDLER_EVENT_HPP_
#define SYCL_INCLUDE_CL_SYCL_HANDLER_EVENT_HPP_

namespace cl::sycl {

class handler_event {
 public:
  event get_kernel() const;
  event get_complete() const;
  event get_end() const;
};

}

#endif //SYCL_INCLUDE_CL_SYCL_HANDLER_EVENT_HPP_
