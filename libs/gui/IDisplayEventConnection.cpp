/*
 * Copyright (C) 2011 The Android Open Source Project
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

#include <gui/IDisplayEventConnection.h>

#include <private/gui/BitTube.h>

#include <binder/Parcel.h>

namespace android {

enum { GET_DATA_CHANNEL = IBinder::FIRST_CALL_TRANSACTION, SET_VSYNC_RATE, REQUEST_NEXT_VSYNC };

class BpDisplayEventConnection : public BpInterface<IDisplayEventConnection> {
public:
    explicit BpDisplayEventConnection(const sp<IBinder>& impl)
          : BpInterface<IDisplayEventConnection>(impl) {}

    ~BpDisplayEventConnection() override;

    status_t getDataChannel(sp<gui::BitTube>* outChannel) const override {
        Parcel data, reply;
        data.writeInterfaceToken(IDisplayEventConnection::getInterfaceDescriptor());
        remote()->transact(GET_DATA_CHANNEL, data, &reply);
        *outChannel = new gui::BitTube(reply);
        return NO_ERROR;
    }

    status_t setVsyncRate(uint32_t count) override {
        Parcel data, reply;
        data.writeInterfaceToken(IDisplayEventConnection::getInterfaceDescriptor());
        data.writeUint32(count);
        remote()->transact(SET_VSYNC_RATE, data, &reply);
        return NO_ERROR;
    }

    void requestNextVsync() override {
        Parcel data, reply;
        data.writeInterfaceToken(IDisplayEventConnection::getInterfaceDescriptor());
        remote()->transact(REQUEST_NEXT_VSYNC, data, &reply, IBinder::FLAG_ONEWAY);
    }
};

// Out-of-line virtual method definition to trigger vtable emission in this translation unit (see
// clang warning -Wweak-vtables)
BpDisplayEventConnection::~BpDisplayEventConnection() {}

IMPLEMENT_META_INTERFACE(DisplayEventConnection, "android.gui.DisplayEventConnection");

status_t BnDisplayEventConnection::onTransact(uint32_t code, const Parcel& data, Parcel* reply,
                                              uint32_t flags) {
    switch (code) {
        case GET_DATA_CHANNEL: {
            CHECK_INTERFACE(IDisplayEventConnection, data, reply);
            sp<gui::BitTube> channel;
            getDataChannel(&channel);
            channel->writeToParcel(reply);
            return NO_ERROR;
        }
        case SET_VSYNC_RATE: {
            CHECK_INTERFACE(IDisplayEventConnection, data, reply);
            setVsyncRate(data.readUint32());
            return NO_ERROR;
        }
        case REQUEST_NEXT_VSYNC: {
            CHECK_INTERFACE(IDisplayEventConnection, data, reply);
            requestNextVsync();
            return NO_ERROR;
        }
    }
    return BBinder::onTransact(code, data, reply, flags);
}

} // namespace android
