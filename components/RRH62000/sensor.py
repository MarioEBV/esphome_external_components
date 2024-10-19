import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import (
    CONF_ID,
    CONF_TEMPERATURE,
    CONF_HUMIDITY,
    CONF_TVOC,
    CONF_ECO2,
    DEVICE_CLASS_CARBON_DIOXIDE,
    DEVICE_CLASS_HUMIDITY,
    DEVICE_CLASS_PM1,
    DEVICE_CLASS_PM25,
    DEVICE_CLASS_PM10,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_VOLATILE_ORGANIC_COMPOUNDS,
    DEVICE_CLASS_AQI,
    STATE_CLASS_MEASUREMENT,
    UNIT_PARTS_PER_MILLION,
    UNIT_MICROGRAMS_PER_CUBIC_METER,
    UNIT_PERCENT,
    UNIT_CELSIUS,
    UNIT_COUNTS_PER_CUBIC_CENTIMETER,
    ICON_MOLECULE_CO2,
    ICON_CHEMICAL_WEAPON,
    ICON_GRAIN,
    ICON_COUNTER
)

DEPENDENCIES = ["uart"]

rrh62000_ns = cg.esphome_ns.namespace("rrh62000")
RRH62000Sensor = rrh62000_ns.class_("RRH62000Sensor", cg.PollingComponent, uart.UARTDevice)

CONF_IAQ = "iaq"
CONF_NC_0_3 = "nc_0_3"
CONF_NC_0_5 = "nc_0_5"
CONF_NC_1 = "nc_1"
CONF_NC_2_5 = "nc_2_5"
CONF_NC_4 = "nc_4"
CONF_PM_1_0_KCI = "pm_1_0_kci"
CONF_PM_2_5_KCI = "pm_2_5_kci"
CONF_PM_10_0_KCI = "pm_10_0_kci"
CONF_PM_1_0_SMOKE = "pm_1_0_smoke"
CONF_PM_2_5_SMOKE = "pm_2_5_smoke"
CONF_PM_10_0_SMOKE = "pm_10_0_smoke"

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(RRH62000Sensor),
            cv.Optional(CONF_ECO2): sensor.sensor_schema(
                unit_of_measurement=UNIT_PARTS_PER_MILLION,
                icon=ICON_MOLECULE_CO2,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_CARBON_DIOXIDE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_TVOC): sensor.sensor_schema(
                unit_of_measurement=UNIT_MICROGRAMS_PER_CUBIC_METER,
                icon=ICON_CHEMICAL_WEAPON,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_VOLATILE_ORGANIC_COMPOUNDS,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_PM_1_0_KCI): sensor.sensor_schema(
                unit_of_measurement=UNIT_MICROGRAMS_PER_CUBIC_METER,
                icon=ICON_GRAIN,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_PM1,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_PM_2_5_KCI): sensor.sensor_schema(
                unit_of_measurement=UNIT_MICROGRAMS_PER_CUBIC_METER,
                icon=ICON_GRAIN,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_PM25,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_PM_10_0_KCI): sensor.sensor_schema(
                unit_of_measurement=UNIT_MICROGRAMS_PER_CUBIC_METER,
                icon=ICON_GRAIN,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_PM10,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_PM_1_0_SMOKE): sensor.sensor_schema(
                unit_of_measurement=UNIT_MICROGRAMS_PER_CUBIC_METER,
                icon=ICON_GRAIN,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_PM1,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_PM_2_5_SMOKE): sensor.sensor_schema(
                unit_of_measurement=UNIT_MICROGRAMS_PER_CUBIC_METER,
                icon=ICON_GRAIN,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_PM25,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_PM_10_0_SMOKE): sensor.sensor_schema(
                unit_of_measurement=UNIT_MICROGRAMS_PER_CUBIC_METER,
                icon=ICON_GRAIN,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_PM10,
                state_class=STATE_CLASS_MEASUREMENT
            ),
            cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_HUMIDITY): sensor.sensor_schema(
                unit_of_measurement=UNIT_PERCENT,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_HUMIDITY,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_NC_0_3): sensor.sensor_schema(
                unit_of_measurement=UNIT_COUNTS_PER_CUBIC_CENTIMETER,
                icon=ICON_COUNTER,
                accuracy_decimals=1,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_NC_0_5): sensor.sensor_schema(
                unit_of_measurement=UNIT_COUNTS_PER_CUBIC_CENTIMETER,
                icon=ICON_COUNTER,
                accuracy_decimals=1,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_NC_1): sensor.sensor_schema(
                unit_of_measurement=UNIT_COUNTS_PER_CUBIC_CENTIMETER,
                icon=ICON_COUNTER,
                accuracy_decimals=1,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_NC_2_5): sensor.sensor_schema(
                unit_of_measurement=UNIT_COUNTS_PER_CUBIC_CENTIMETER,
                icon=ICON_COUNTER,
                accuracy_decimals=1,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_NC_4): sensor.sensor_schema(
                unit_of_measurement=UNIT_COUNTS_PER_CUBIC_CENTIMETER,
                icon=ICON_COUNTER,
                accuracy_decimals=1,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            
        }
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(uart.UART_DEVICE_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    if CONF_ECO2 in config:
        sens = await sensor.new_sensor(config[CONF_ECO2])
        cg.add(var.set_eco2_sensor(sens))
    if CONF_TVOC in config:
        sens = await sensor.new_sensor(config[CONF_TVOC])
        cg.add(var.set_tvoc_sensor(sens))
    if CONF_PM_1_0_KCI in config:
        sens = await sensor.new_sensor(config[CONF_PM_1_0_KCI])
        cg.add(var.set_pm_1_0_kci_sensor(sens))
    if CONF_PM_2_5_KCI in config:
        sens = await sensor.new_sensor(config[CONF_PM_2_5_KCI])
        cg.add(var.set_pm_2_5_kci_sensor(sens))
    if CONF_PM_10_0_KCI in config:
        sens = await sensor.new_sensor(config[CONF_PM_10_0_KCI])
        cg.add(var.set_pm_10_0_kci_sensor(sens))
    if CONF_PM_1_0_SMOKE in config:
        sens = await sensor.new_sensor(config[CONF_PM_1_0_SMOKE])
        cg.add(var.set_pm_1_0_smoke_sensor(sens))
    if CONF_PM_2_5_SMOKE in config:
        sens = await sensor.new_sensor(config[CONF_PM_2_5_SMOKE])
        cg.add(var.set_pm_2_5_smoke_sensor(sens))
    if CONF_PM_10_0_SMOKE in config:
        sens = await sensor.new_sensor(config[CONF_PM_10_0_SMOKE])
        cg.add(var.set_pm_10_0_smoke_sensor(sens))
    if CONF_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATURE])
        cg.add(var.set_temperature_sensor(sens))
    if CONF_HUMIDITY in config:
        sens = await sensor.new_sensor(config[CONF_HUMIDITY])
        cg.add(var.set_humidity_sensor(sens))
    if CONF_NC_0_3 in config:
        sens = await sensor.new_sensor(config[CONF_NC_0_3])
        cg.add(var.set_nc_0_3_sensor(sens))
    if CONF_NC_0_5 in config:
        sens = await sensor.new_sensor(config[CONF_NC_0_5])
        cg.add(var.set_nc_0_5_sensor(sens))
    if CONF_NC_1 in config:
        sens = await sensor.new_sensor(config[CONF_NC_1])
        cg.add(var.set_nc_1_sensor(sens))
    if CONF_NC_2_5 in config:
        sens = await sensor.new_sensor(config[CONF_NC_2_5])
        cg.add(var.set_nc_2_5_sensor(sens))
    if CONF_NC_4 in config:
        sens = await sensor.new_sensor(config[CONF_NC_4])
        cg.add(var.set_nc_4_sensor(sens))
