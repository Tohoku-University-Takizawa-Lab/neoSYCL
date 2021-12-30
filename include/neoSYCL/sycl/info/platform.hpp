#ifndef CUSTOM_SYCL_INCLUDE_SYCL_INFO_PLATFORM_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_INFO_PLATFORM_HPP_

namespace neosycl::sycl::info {

/** Platform information descriptors

    A SYCL platform can be queried for all of the following information
    using the get_info function. All SYCL contexts have valid devices for
    them, including the SYCL host device.
*/
enum class platform : unsigned int {
  /** Returns the profile name (as a string_class) supported by the im-
      plementation.

      Can be either FULL PROFILE or EMBEDDED PROFILE.
  */
  profile,
  /** Returns the OpenCL software driver version string in the form major
      number.minor number (as a string_class)
  */
  version,
  /** Returns the name of the platform (as a string_class)
   */
  name,
  /** Returns the string provided by the platform vendor (as a string_class)
   */
  vendor,
  /** Returns a space-separated list of extension names supported by the
      platform (as a string_class)
  */
  extensions
};

} // namespace neosycl::sycl::info

#endif // CUSTOM_SYCL_INCLUDE_SYCL_INFO_PLATFORM_HPP_
