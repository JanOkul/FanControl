idf_component_register(SRCS "FanControl.c"
                    INCLUDE_DIRS "."
                    REQUIRES infrared wifi http nvs_flash esp_pm)
