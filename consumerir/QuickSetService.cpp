#define LOG_TAG "QuickSetService"

#include "QuickSetService.h"

namespace android {
IMPLEMENT_META_INTERFACE(Control, SERVICE);

BpControl::BpControl(const sp<IBinder> &impl) : BpInterface<IControl>(impl) {}

void BpControl::transmit(const int32_t carrierFreq,
                         const android::hardware::hidl_vec<int32_t> &pattern) {
  Parcel data, reply;

  int result = 1;

  data.writeInterfaceToken(String16(ACTION));
  data.writeInt32(carrierFreq);
  data.write(pattern.data(), sizeof(int32_t) * pattern.size());

  LOG(INFO) << "Received freq: " << carrierFreq;

  remote()->transact(18, data, &reply, 0);

  reply.readExceptionCode();
  result = reply.readInt32();
}
} // namespace android
