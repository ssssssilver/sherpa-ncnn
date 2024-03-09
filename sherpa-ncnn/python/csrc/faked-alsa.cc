// sherpa-ncnn/python/csrc/faked-alsa.cc
//
// Copyright (c)  2024  Xiaomi Corporation

#include <stdio.h>

#include "sherpa-ncnn/python/csrc/alsa.h"

namespace sherpa_ncnn {
class FakedAlsa {
 public:
  explicit FakedAlsa(const char *) {
    fprintf(stderr, "This function is for Linux only.");
#if (SHERPA_NCNN_ENABLE_ALSA == 0) && (defined(__unix__) || defined(__unix))
    fprintf(stderr, R"doc(
sherpa-ncnn is compiled without alsa support. To enable that, please run
  (1) sudo apt-get install alsa-utils libasound2-dev
  (2) rebuild sherpa-ncnn
)doc");
#endif
    exit(-1);
  }

  std::vector<float> Read(int32_t) const { return {}; }
  int32_t GetExpectedSampleRate() const { return -1; }
  int32_t GetActualSampleRate() const { return -1; }
};

void PybindAlsa(py::module *m) {
  using PyClass = FakedAlsa;
  py::class_<PyClass>(*m, "Alsa")
      .def(py::init<const char *>(), py::arg("device_name"))
      .def(
          "read",
          [](PyClass &self, int32_t num_samples) -> std::vector<float> {
            return self.Read(num_samples);
          },
          py::arg("num_samples"), py::call_guard<py::gil_scoped_release>())
      .def_property_readonly("expected_sample_rate",
                             &PyClass::GetExpectedSampleRate)
      .def_property_readonly("actual_sample_rate",
                             &PyClass::GetActualSampleRate);
}

}  // namespace sherpa_ncnn
