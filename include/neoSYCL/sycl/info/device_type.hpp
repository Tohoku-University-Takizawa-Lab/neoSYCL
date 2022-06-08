#pragma once

namespace neosycl::sycl::info {

enum class device_type : unsigned int {
  cpu,
  gpu,
  accelerator,
  custom,
  defaults,
  host,
  all
};

}
