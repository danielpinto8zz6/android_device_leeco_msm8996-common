/*
 * Copyright (C) 2013 The Android Open Source Project
 * Copyright (C) 2018 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "ConsumerIrService"

#include <android-base/logging.h>

#include "ConsumerIr.h"
#include "QuickSetService.h"

#define IR_ENABLE_PATH "/sys/remote/enable"

namespace android {
namespace hardware {
namespace ir {
namespace V1_0 {
namespace implementation {

static hidl_vec<ConsumerIrFreqRange> rangeVec{
    {.min = 30000, .max = 30000}, {.min = 33000, .max = 33000},
    {.min = 36000, .max = 36000}, {.min = 38000, .max = 38000},
    {.min = 40000, .max = 40000}, {.min = 56000, .max = 56000},
};

// Methods from ::android::hardware::ir::V1_0::IConsumerIr follow.
Return<bool> ConsumerIr::transmit(int32_t carrierFreq,
                                  const hidl_vec<int32_t> &pattern) {
  LOG(INFO) << "transmitting pattern at " << carrierFreq << " Hz";

  int fd_enable;

  // Enable IR device
  fd_enable = open(IR_ENABLE_PATH, O_RDWR);
  write(fd_enable, "1", 1);

  sp<IBinder> binder;
  sp<ProcessState> proc(ProcessState::self());

  sp<IServiceManager> sm = defaultServiceManager();
  if (sm != 0)
    LOG(ERROR) << "Error starting service manager";

  do {
    // Search service by SERVICE_NAME
    binder = sm->getService(String16(SERVICE));
    if (binder != 0)
      break;
    sleep(1);
  } while (true);

  LOG(INFO) << "Bind to : " << SERVICE;
  const sp<IControl> &cs = interface_cast<IControl>(binder);
  if (cs != 0)
    LOG(ERROR) << "Error casting interface";
  else {
    cs->transmit((int32_t)carrierFreq, pattern);
    LOG(INFO) << "Transmit" << carrierFreq;
  }

  return false;
}

Return<void> ConsumerIr::getCarrierFreqs(getCarrierFreqs_cb _hidl_cb) {
  _hidl_cb(true, rangeVec);
  return Void();
}

} // namespace implementation
} // namespace V1_0
} // namespace ir
} // namespace hardware
} // namespace android
