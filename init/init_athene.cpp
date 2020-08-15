/*
   Copyright (c) 2014, The Linux Foundation. All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.
   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#include <stdio.h>

#include "android-base/properties.h"
#include "property_service.h"
#include "vendor_init.h"

using android::base::GetProperty
using android::init::property_set

static void num_sims(void);

void property_override(char const prop[], char const value[])
{
    prop_info *pi;

    pi = (prop_info*) __system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

void property_override_dual(char const system_prop[], char const vendor_prop[], char const value[])
{
    property_override(system_prop, value);
    property_override(vendor_prop, value);
}

void vendor_load_properties()
{

    std::string platform;
    std::string device_boot;
    std::string sku;
    std::string radio;
    std::string device;
    std::string carrier;

    platform = GetProperty("ro.board.platform", "");
    if (platform != ANDROID_TARGET)
        return;

    device_boot = GetProperty("ro.boot.device", "");
    property_set("ro.hw.device", device_boot.c_str());

    sku = GetProperty("ro.boot.hardware.sku", "");

    carrier = GetProperty("ro.boot.carrier", "");

    radio = GetProperty("ro.boot.radio", "");
    property_set("ro.hw.radio", radio.c_str());

    /* Common for all models */
    property_override("ro.build.product", "athene");
    num_sims();

    if (device_boot == "athene_13mp") {
        /* Moto G4 (XT162x) */
        property_override_dual("ro.product.device", "ro.vendor.product.device", "athene");
        property_override("ro.build.description", "athene-user 8.1.0 OPJ28.111-22-1 1ec4b release-keys");
        property_override_dual("ro.build.fingerprint", "ro.vendor.build.fingerprint", "motorola/athene/athene:8.1.0/OPJ28.111-22-1/1ec4b:user/release-keys");
        property_override_dual("ro.product.model", "ro.vendor.product.model", "Moto G4");
        property_set("ro.telephony.default_network", "10");
    } else {
        /* Moto G4 Plus (XT164x) */
        property_override_dual("ro.product.device", "ro.vendor.product.device", "athene_f");
        property_override("ro.build.description", "athene_f-user 8.1.0 OPJ28.111-22-1 1ec4b release-keys");
        property_override_dual("ro.build.fingerprint", "ro.vendor.build.fingerprint", "motorola/athene_f/athene_f:8.1.0/OPJ28.111-22-1/1ec4b:user/release-keys");
        property_override_dual("ro.product.model", "ro.vendor.product.model", "Moto G4 Plus");
        property_set("ro.telephony.default_network", "10,0");
    }

   if (sku == "XT1625" || sku == "XT1644") {
        property_set("net.tethering.noprovisioning", "true");
        property_set("persist.radio.is_wps_enabled", "true");
        property_set("ro.radio.imei.sv", "4");
        property_set("tether_dun_required", "0");
   }

    if (sku == "XT1621" || sku == "XT1622" || sku == "XT1640" || sku == "XT1642" || sku == "XT1643") {
        if (radio == "India") {
            property_set("ro.radio.imei.sv", "8");
            property_set("persist.radio.is_wps_enabled", "true");
        } else {
            property_set("ro.radio.imei.sv", "3");
        }
    }

    if (sku == "XT1626" || sku == "XT1641") {
        property_set("ro.radio.imei.sv", "2");
        property_set("persist.radio.is_wps_enabled", "true");
        property_set("persist.radio.pb.max.match", "10");
    }
}

static void num_sims(void) {
    std::string dualsim;

    dualsim = GetProperty("ro.boot.dualsim", "");

    property_set("ro.hw.dualsim", dualsim.c_str());

    if (dualsim == "true") {
        property_set("persist.radio.multisim.config", "dsds");
        property_set("ro.telephony.ril.config", "simactivation");
    } else {
        property_set("persist.radio.multisim.config", "");
    }
}
