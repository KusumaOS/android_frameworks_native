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

#pragma once

#include <binder/IInterface.h>

#include <utils/Errors.h>

#include <sys/types.h>
#include <cstdint>

namespace android {

namespace gui {
class BitTube;
} // namespace gui

class IDisplayEventConnection : public IInterface {
public:
    DECLARE_META_INTERFACE(DisplayEventConnection)

    /*
     * getDataChannel() returns a BitTube where to receive the events from
     */
    virtual status_t getDataChannel(sp<gui::BitTube>* outChannel) const = 0;

    /*
     * setVsyncRate() sets the vsync event delivery rate. A value of 1 returns every vsync event.
     * A value of 2 returns every other event, etc. A value of 0 returns no event unless
     * requestNextVsync() has been called.
     */
    virtual status_t setVsyncRate(uint32_t count) = 0;

    /*
     * requestNextVsync() schedules the next vsync event. It has no effect if the vsync rate is > 0.
     */
    virtual void requestNextVsync() = 0; // Asynchronous
};

class BnDisplayEventConnection : public BnInterface<IDisplayEventConnection> {
public:
    virtual status_t onTransact(uint32_t code, const Parcel& data, Parcel* reply,
                                uint32_t flags = 0);
};

} // namespace android
