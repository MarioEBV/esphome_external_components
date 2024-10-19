#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace rrh62000 {

class RRH62000Sensor : public PollingComponent, public uart::UARTDevice {
 public:
  float get_setup_priority() const override { return setup_priority::DATA; }

  void set_eco2_sensor(sensor::Sensor *eco2_sensor) { eco2_sensor_ = eco2_sensor; }
  void set_tvoc_sensor(sensor::Sensor *tvoc_sensor) { tvoc_sensor_ = tvoc_sensor; }
  void set_pm_1_0_kci_sensor(sensor::Sensor *pm_1_0_kci_sensor) { pm_1_0_kci_sensor_ = pm_1_0_kci_sensor; }
  void set_pm_2_5_kci_sensor(sensor::Sensor *pm_2_5__kcisensor) { pm_2_5_kci_sensor_ = pm_2_5_kci_sensor; }
  void set_pm_10_0_kci_sensor(sensor::Sensor *pm_10_0_kci_sensor) { pm_10_0_kci_sensor_ = pm_10_0_kci_sensor; }
  void set_temperature_sensor(sensor::Sensor *temperature_sensor) { temperature_sensor_ = temperature_sensor; }
  void set_humidity_sensor(sensor::Sensor *humidity_sensor) { humidity_sensor_ = humidity_sensor; }
  void set_nc_0_3_sensor(sensor::Sensor *nc_0_3_sensor) { nc_0_3_sensor_ = nc_0_3_sensor; }
  void set_nc_0_5_sensor(sensor::Sensor *nc_0_5_sensor) { nc_0_5_sensor_ = nc_0_5_sensor; }
  void set_nc_1_sensor(sensor::Sensor *nc_1_sensor) { nc_1_sensor_ = nc_1_sensor; }
  void set_nc_2_5_sensor(sensor::Sensor *nc_2_5_sensor) { nc_2_5_sensor_ = nc_2_5_sensor; }
  void set_nc_4_sensor(sensor::Sensor *nc_4_sensor) { nc_4_sensor_ = nc_4_sensor; }

  void update() override;
  void dump_config() override;

 protected:
  uint16_t rrh62000_checksum_(uint8_t *ptr);

  sensor::Sensor *eco2_sensor_{nullptr};
  sensor::Sensor *tvoc_sensor_{nullptr};
  sensor::Sensor *pm_1_0_kci_sensor_{nullptr};
  sensor::Sensor *pm_2_5_kci_sensor_{nullptr};
  sensor::Sensor *pm_10_0__kci_sensor_{nullptr};
  sensor::Sensor *temperature_sensor_{nullptr};
  sensor::Sensor *humidity_sensor_{nullptr};
  sensor::Sensor *nc_0_3_sensor_{nullptr};
  sensor::Sensor *nc_0_5_sensor_{nullptr};
  sensor::Sensor *nc_1_sensor_{nullptr};
  sensor::Sensor *nc_2_5_sensor_{nullptr};
  sensor::Sensor *nc_4_sensor_{nullptr};
};

}  // namespace rrh62000
}  // namespace esphome
