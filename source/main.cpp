/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <events/mbed_events.h>
#include "ble/BLE.h"
#include "gatt_server_process.h"
#include "mbed-trace/mbed_trace.h"

static EventQueue event_queue(/* event count */ 10 * EVENTS_EVENT_SIZE);

class GattServerDemo : ble::GattServer::EventHandler {

    const static uint16_t EXAMPLE_SERVICE_UUID         = 0xA000;
    const static uint16_t WRITABLE_CHARACTERISTIC_UUID = 0xA001;

public:
    GattServerDemo()
    {
        const UUID uuid = WRITABLE_CHARACTERISTIC_UUID;
        _writable_characteristic = new ReadWriteGattCharacteristic<uint32_t[2]> (uuid, &_characteristic_value);

        if (!_writable_characteristic) {
            printf("Allocation of ReadWriteGattCharacteristic failed\r\n");
        }
    }

    ~GattServerDemo()
    {
    }

    void start(BLE &ble, events::EventQueue &event_queue)
    {
        const UUID uuid = EXAMPLE_SERVICE_UUID;
        GattCharacteristic* charTable[] = { _writable_characteristic };
        GattService wire_cutting(uuid, charTable, 1);

        ble.gattServer().addService(wire_cutting);

        ble.gattServer().setEventHandler(this);

        printf("Example service added with UUID 0xA000\r\n");
        printf("Connect and write to characteristic 0xA001\r\n");
    }
    
public:

    /**
     * This callback allows the LEDService to receive updates to the ledState Characteristic.
     *
     * @param[in] params Information about the characterisitc being updated.
     */
    virtual void onDataWritten(const GattWriteCallbackParams &params)
    {
        if (params.handle == _writable_characteristic->getValueHandle()) {
            printf("New characteristic value written: %x, %x, %x, %x, %x\r\n", params.data[0], params.data[1], params.data[2], params.data[3], params.data[4]);
            left = params.data[0];
            factor = params.data[1];
            rest = params.data[2];
            right = params.data[3];
            amount = params.data[4];
        }
    }

    uint8_t getLeft(void) {
        return left;
    }
    uint8_t getFactor(void) {
        return factor;
    }
    uint8_t getRest(void) {
        return rest;
    }
    uint8_t getRight(void) {
        return right;
    }
    uint8_t getAmount(void) {
        return amount;
    }

private:
    uint8_t left;
    uint8_t factor;
    uint8_t rest;
    uint8_t right;
    uint8_t amount;
    ReadWriteGattCharacteristic<uint32_t[2]> *_writable_characteristic = nullptr;
    uint32_t _characteristic_value[2] = {0,0};
};

int main()
{
    mbed_trace_init();

    BLE &ble = BLE::Instance();

    printf("\r\nGattServer demo of a writable characteristic\r\n");

    GattServerDemo demo;

    /* this process will handle basic setup and advertising for us */
    GattServerProcess ble_process(event_queue, ble);

    /* once it's done it will let us continue with our demo*/
    ble_process.on_init(callback(&demo, &GattServerDemo::start));

    ble_process.start();

    return 0;
}
