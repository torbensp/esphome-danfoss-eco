#pragma once

#include "esphome/core/component.h"
#include "esphome/components/ble_client/ble_client.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"
#include "esphome/components/climate/climate.h"
#include "eco2_base.h"

#ifdef USE_ESP32

#include <esp_gattc_api.h>

namespace esphome
{
  namespace eco2
  {

    namespace espbt = esphome::esp32_ble_tracker;

    static auto ECO2_SERVICE_SETTINGS = espbt::ESPBTUUID::from_raw("10020000-2749-0001-0000-00805f9b042f");
    static auto ECO2_CHARACTERISTIC_PIN = espbt::ESPBTUUID::from_raw("10020001-2749-0001-0000-00805f9b042f");
    static auto ECO2_CHARACTERISTIC_NAME = espbt::ESPBTUUID::from_raw("10020006-2749-0001-0000-00805f9b042f");

    static uint8_t eco2Pin[] = {0x30, 0x30, 0x30, 0x30};

    class DanfossEco2 : public climate::Climate, public esphome::ble_client::BLEClientNode, public PollingComponent
    {
    public:
      void setup() override;
      void loop() override;
      void update() override;
      void gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                               esp_ble_gattc_cb_param_t *param) override;
      void dump_config() override;
      float get_setup_priority() const override { return setup_priority::DATA; }
      climate::ClimateTraits traits() override
      {
        auto traits = climate::ClimateTraits();
        traits.set_supports_current_temperature(true);
        traits.set_supports_heat_mode(true);
        traits.set_visual_min_temperature(25.0);
        traits.set_visual_max_temperature(100.0);
        traits.set_visual_temperature_step(0.1);
        return traits;
      }
      void set_secret_key(const char *);

    protected:
      std::unique_ptr<AnovaCodec> codec_;
      void control(const climate::ClimateCall &call) override;
      uint16_t char_handle_;
      uint8_t current_request_;
      uint8_t *secret_;
    };

  } // namespace eco2
} // namespace esphome

#endif
