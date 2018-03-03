#ifndef QUICKSETSERVICE
#define QUICKSETSERVICE
#include <binder/Binder.h>
#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/Parcel.h>
#include <binder/ProcessState.h>

#include <android-base/logging.h>

#include <hidl/HidlSupport.h>

#define SERVICE "com.uei.quicksetsdk.letv/com.uei.control.Service"
#define ACTION "com.uei.control.IControl"

namespace android {

using ::android::hardware::hidl_vec;

class IControl : public IInterface {
public:
  DECLARE_META_INTERFACE(Control);
  virtual void
  transmit(const int32_t carrierFreq,
           const android::hardware::hidl_vec<int32_t> &pattern) = 0;
};

class BpControl : public BpInterface<IControl> {
public:
  BpControl(const sp<IBinder> &impl);
  virtual void transmit(const int32_t carrierFreq,
                        const android::hardware::hidl_vec<int32_t> &pattern);
};
} // namespace android
#endif
