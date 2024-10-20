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
  if ((calculated_checksum != response[38])) {
    ESP_LOGW(TAG, "RRH62000 Checksum doesn't match",
             calculated_checksum);
    this->status_set_warning();
    return;
  }

  this->status_clear_warning();

  ESP_LOGD(TAG, "Successfully read rrh62000 data");

  const uint16_t eco2 = (response[32] * 256) + response[33];
  const float tvoc = (((response[30] * 256) + response[31]) * 10);
  const float pm_1_0_kci = (((response[14] * 256) + response[15]) * 0.1f);
  const float pm_2_5_kci = (((response[16] * 256) + response[17]) * 0.1f);
  const float pm_10_0_kci = (((response[18] * 256) + response[19]) * 0.1f);
  const float pm_1_0_smoke = (((response[20] * 256) + response[21]) * 0.1f);
  const float pm_2_5_smoke = (((response[22] * 256) + response[23]) * 0.1f);
  const float pm_10_0_smoke = (((response[24] * 256) + response[25]) * 0.1f);
  const float temperature = ((response[26] ) > 128)
                                ? (((((response[26] * 256) + response[27]) * 0.01f) - 128) * -1)
                                : (((response[26] * 256) + response[27]) * 0.01f);
  const float humidity = (((response[28] *256) + response[29]) * 0.01f);
  const float nc_0_3 = (((response[4] * 256) + response[5]) * 0.1f );
  const float nc_0_5 = (((response[6] * 256) + response[7]) * 0.1f );
  const float nc_1 = (((response[8] * 256) + response[9]) * 0.1f );
  const float nc_2_5 = (((response[10] * 256) + response[11]) * 0.1f );
  const float nc_4 = (((response[12] * 256) + response[13]) * 0.1f );
  const float iaq = (((response[34] * 256) + response[35]) * 0.01f );
  const uint8_t checksum = response[38];
  
  ESP_LOGD(TAG, "Received ECO₂: %u ppm", eco2);
  if (this->eco2_sensor_ != nullptr)
    this->eco2_sensor_->publish_state(eco2);

  ESP_LOGD(TAG, "Received IAQ: %1f " , iaq);
  if (this->iaq_sensor_ != nullptr)
    this->iaq_sensor_->publish_state(iaq); 

  ESP_LOGD(TAG, "Received TVOC: %f µg/m³", tvoc);
  if (this->tvoc_sensor_ != nullptr)
    this->tvoc_sensor_->publish_state(tvoc);
  
    ESP_LOGD(TAG, "Received PM1.0 KCI: %.1f µg/m³", pm_1_0_kci);
  if (this->pm_1_0_kci_sensor_ != nullptr)
    this->pm_1_0_kci_sensor_->publish_state(pm_1_0_kci);

  ESP_LOGD(TAG, "Received PM2.5 KCI: %.1f µg/m³", pm_2_5_kci);
  if (this->pm_2_5_kci_sensor_ != nullptr)
    this->pm_2_5_kci_sensor_->publish_state(pm_2_5_kci);

  ESP_LOGD(TAG, "Received PM10 KCI: %.1f µg/m³", pm_10_0_kci);
  if (this->pm_10_0_kci_sensor_ != nullptr)
    this->pm_10_0_kci_sensor_->publish_state(pm_10_0_kci);

  ESP_LOGD(TAG, "Received PM1.0 cigarette smoke: %.1f µg/m³", pm_1_0_smoke);
  if (this->pm_1_0_smoke_sensor_ != nullptr)
    this->pm_1_0_smoke_sensor_->publish_state(pm_1_0_smoke);

  ESP_LOGD(TAG, "Received PM2.5 cigarette smoke: %.1f µg/m³", pm_2_5_smoke);
  if (this->pm_2_5_smoke_sensor_ != nullptr)
    this->pm_2_5_smoke_sensor_->publish_state(pm_2_5_smoke);

  ESP_LOGD(TAG, "Received PM10 cigarette smoke: %.1f µg/m³", pm_10_0_smoke);
  if (this->pm_10_0_smoke_sensor_ != nullptr)
    this->pm_10_0_smoke_sensor_->publish_state(pm_10_0_smoke);

  ESP_LOGD(TAG, "Received Temperature: %.2f °C", temperature);
  if (this->temperature_sensor_ != nullptr)
    this->temperature_sensor_->publish_state(temperature);

  ESP_LOGD(TAG, "Received Humidity: %.2f percent", humidity);
  if (this->humidity_sensor_ != nullptr)
    this->humidity_sensor_->publish_state(humidity);
    
  ESP_LOGD(TAG, "Received NC0.3: %.1f N/cm³", nc_0_3);
  if (this->nc_0_3_sensor_ != nullptr)
    this->nc_0_3_sensor_->publish_state(nc_0_3);
    
  ESP_LOGD(TAG, "Received NC0.5: %.1f N/cm³", nc_0_5);
  if (this->nc_0_5_sensor_ != nullptr)
    this->nc_0_5_sensor_->publish_state(nc_0_5);
  
  ESP_LOGD(TAG, "Received NC1: %.1f N/cm³", nc_1);
  if (this->nc_1_sensor_ != nullptr)
    this->nc_1_sensor_->publish_state(nc_1);

  ESP_LOGD(TAG, "Received NC2.5: %.1f N/cm³", nc_2_5);
  if (this->nc_2_5_sensor_ != nullptr)
    this->nc_2_5_sensor_->publish_state(nc_2_5);
    
  ESP_LOGD(TAG, "Received NC4: %.1f N/cm³", nc_4);
  if (this->nc_4_sensor_ != nullptr)
    this->nc_4_sensor_->publish_state(nc_4);
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
  LOG_SENSOR("  ", "IAQ", this->iaq_sensor_);
  LOG_SENSOR("  ", "TVOC", this->tvoc_sensor_);
  LOG_SENSOR("  ", "PM1.0 KCI", this->pm_1_0_kci_sensor_);
  LOG_SENSOR("  ", "PM2.5 KCI", this->pm_2_5_kci_sensor_);
  LOG_SENSOR("  ", "PM10 KCI", this->pm_10_0_kci_sensor_);
  LOG_SENSOR("  ", "PM1.0 smoke", this->pm_1_0_smoke_sensor_);
  LOG_SENSOR("  ", "PM2.5 smoke", this->pm_2_5_smoke_sensor_);
  LOG_SENSOR("  ", "PM10 smoke", this->pm_10_0_smoke_sensor_);
  LOG_SENSOR("  ", "Temperature", this->temperature_sensor_);
  LOG_SENSOR("  ", "Humidity", this->humidity_sensor_);
  LOG_SENSOR("  ", "NC0.3", this->nc_0_3_sensor_);
  LOG_SENSOR("  ", "NC0.5", this->nc_0_5_sensor_);
  LOG_SENSOR("  ", "NC1", this->nc_2_5_sensor_);
  LOG_SENSOR("  ", "NC2.5", this->nc_1_sensor_);
  LOG_SENSOR("  ", "NC4", this->nc_4_sensor_);
  this->check_uart_settings(9600);
}

}  // namespace rrh62000
}  // namespace esphome
