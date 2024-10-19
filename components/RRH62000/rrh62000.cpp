#include "rrh62000.h"
#include "esphome/core/log.h"

namespace esphome {
namespace rrh62000 {

static const char *const TAG = "rrh62000";
static const uint8_t RRH62000_RESPONSE_LENGTH = 39;

void RRH62000Sensor::update() {
  uint8_t response[RRH62000_RESPONSE_LENGTH];
  uint8_t peeked;

  while (this->available() > 0 && this->peek_byte(&peeked) && peeked != 0xFF)
    this->read();

  bool read_success = read_array(response, RRH62000_RESPONSE_LENGTH);

  if (!read_success) {
    ESP_LOGW(TAG, "Reading data from RRH62000 failed!");
    status_set_warning();
    return;
  }

  if (response[0] != 0xFF || response[1] != 0xFA) {
    ESP_LOGW(TAG, "Invalid preamble for RRH62000 response!");
    this->status_set_warning();
    return;
  }

  uint16_t calculated_checksum = this->rrh62000_checksum_(response);
  // Occasionally the checksum has a +/- 0x80 offset. Negative temperatures are
  // responsible for some of these. The rest are unknown/undocumented.
  if ((calculated_checksum != response[RRH62000_RESPONSE_LENGTH - 1]) &&
      (calculated_checksum - 0x80 != response[RRH62000_RESPONSE_LENGTH - 1]) &&
      (calculated_checksum + 0x80 != response[RRH62000_RESPONSE_LENGTH - 1])) {
    ESP_LOGW(TAG, "RRH62000 Checksum doesn't match: 0x%02X!=0x%02X", response[RRH62000_RESPONSE_LENGTH - 1],
             calculated_checksum);
    this->status_set_warning();
    return;
  }

  this->status_clear_warning();

  ESP_LOGD(TAG, "Successfully read rrh62000 data");

  const uint16_t eco2 = (response[32] * 256) + response[33];
  const uint16_t tvoc = (response[30] * 256) + response[31];
  const uint16_t pm_1_0 = (response[14] * 256) + response[15];
  const uint16_t pm_2_5 = (response[16] * 256) + response[17];
  const uint16_t pm_10_0 = (response[18] * 256) + response[19];
  const float temperature = ((response[26] ) > 128)
                                ? (((((response[26] * 256) + response[27]) * 0.01f) - 128) * -1)
                                : (((response[26] * 256) + response[27]) * 0.01f);
  const float humidity = (((response[28] *256) + response[29]) * 0.01f);
  const uint16_t nc_0_3 = (response[4] * 256) + response[5];

  ESP_LOGD(TAG, "Received ECO₂: %u ppm", eco2);
  if (this->eco2_sensor_ != nullptr)
    this->eco2_sensor_->publish_state(eco2);

  ESP_LOGD(TAG, "Received TVOC: %u µg/m³", tvoc);
  if (this->tvoc_sensor_ != nullptr)
    this->tvoc_sensor_->publish_state(tvoc);
  
    ESP_LOGD(TAG, "Received PM1.0: %u µg/m³", pm_1_0);
  if (this->pm_1_0_sensor_ != nullptr)
    this->pm_1_0_sensor_->publish_state(pm_1_0);

  ESP_LOGD(TAG, "Received PM2.5: %u µg/m³", pm_2_5);
  if (this->pm_2_5_sensor_ != nullptr)
    this->pm_2_5_sensor_->publish_state(pm_2_5);

  ESP_LOGD(TAG, "Received PM10: %u µg/m³", pm_10_0);
  if (this->pm_10_0_sensor_ != nullptr)
    this->pm_10_0_sensor_->publish_state(pm_10_0);

  ESP_LOGD(TAG, "Received Temperature: %.2f °C", temperature);
  if (this->temperature_sensor_ != nullptr)
    this->temperature_sensor_->publish_state(temperature);

  ESP_LOGD(TAG, "Received Humidity: %.2f percent", humidity);
  if (this->humidity_sensor_ != nullptr)
    this->humidity_sensor_->publish_state(humidity);
    
  ESP_LOGD(TAG, "Received NC0.3: %u count/cm²", nc_0_3);
  if (this->nc_0_3_sensor_ != nullptr)
    this->nc_0_3_sensor_->publish_state(nc_0_3);
}

uint16_t RRH62000Sensor::rrh62000_checksum_(uint8_t *ptr) {
  uint8_t sum = 0;
  for (int i = 0; i < (RRH62000_RESPONSE_LENGTH - 1); i++) {
    sum += *ptr++;
  }
  return sum;
}

void RRH62000Sensor::dump_config() {
  ESP_LOGCONFIG(TAG, "RRH62000:");
  LOG_SENSOR("  ", "eCO2", this->eco2_sensor_);
  LOG_SENSOR("  ", "TVOC", this->tvoc_sensor_);
  LOG_SENSOR("  ", "PM1.0", this->pm_1_0_sensor_);
  LOG_SENSOR("  ", "PM2.5", this->pm_2_5_sensor_);
  LOG_SENSOR("  ", "PM10", this->pm_10_0_sensor_);
  LOG_SENSOR("  ", "Temperature", this->temperature_sensor_);
  LOG_SENSOR("  ", "Humidity", this->humidity_sensor_);
  LOG_SENSOR("  ", "NC0.3", this->nc_0_3_sensor_);
  this->check_uart_settings(9600);
}

}  // namespace rrh62000
}  // namespace esphome
