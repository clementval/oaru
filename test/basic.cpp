#include <cassert>
#include <iostream>
#include "openacc.h"

#define N 10

int main() {

  float a[N];

  for (int i = 0; i < N; ++i) {
    a[i] = 1.5;
  }

  acc_init(acc_device_default);

  int cudaDevice = acc_get_num_devices(acc_device_nvidia);
  assert(cudaDevice >= 1);

  assert(acc_get_device_type() == acc_device_nvidia);

  assert(acc_get_property(0, acc_device_nvidia, acc_property_memory) > 0);
  assert(acc_get_property(0, acc_device_nvidia, acc_property_free_memory) > 0);
  assert(acc_get_property(0, acc_device_nvidia, 
                          acc_property_shared_memory_support) > 0);

  assert(acc_get_property_string(0, acc_device_nvidia, acc_property_name) != NULL);
  assert(acc_get_property_string(0, acc_device_nvidia, acc_property_vendor) != NULL);
  assert(acc_get_property_string(0, acc_device_nvidia, acc_property_driver) != NULL);

  assert(acc_is_present(a, sizeof(float) * N) == 0);
  d_void *a_dptr = acc_create(a, sizeof(float) * N);
  assert(acc_is_present(a, sizeof(float) * N) == 1);
  acc_update_device(a, sizeof(float) * N);
  acc_update_self(a, sizeof(float) * N);

  for (int i = 0; i < N; ++i) {
    assert(a[i] == 1.5);
  }

  acc_delete(a, sizeof(float) * N);
  assert(acc_is_present(a, sizeof(float) * N) == 0);
  acc_shutdown(acc_device_default);
  return 0;
}
