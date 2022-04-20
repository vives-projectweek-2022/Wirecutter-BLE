//mbed compile -f --sterm --baudrate 115200

#include "mbed.h"
//#include "wirecutter.h"
#include "ble_wirecutter.h"

using namespace std;

DigitalOut enable(D8);

// Wirecutter wirecutter;

void printWire( GattServerDemo::Config config ) {
    enable = 0;
    if (config.wire_amount)
    {
        printf("Left: %d, Factor: %d, Rest: %d, Right: %d, Amount: %d\n", config.left, config.factor, config.rest, config.right, config.wire_amount);

        for (uint8_t i = 0; i < config.wire_amount; i++)
        {
            // wirecutter.feedCable(config.left);
            // wirecutter.stripCable();
            // wirecutter.feedCable((config.factor * 256) + config.rest);
            // wirecutter.stripCable();
            // wirecutter.feedCable(config.right);
            // wirecutter.cutCable();
        }
    }
    // if (config.reel_amount)
    // {
    //     printf("Type: %d, Length: %d, Amount: %d\n", config.type, config.length, config.reel_amount);

    //     for (uint8_t i = 0; i < config.reel_amount; i++)
    //     {
    //         wirecutter.feedReel(config.length*config.type);
    //         wirecutter.cutCable();
    //     }
    // }
}

int main(void) {

    mbed_trace_init();
    BLE &ble = BLE::Instance();
    GattServerDemo demo;
    demo.onDataListener(callback(printWire));
    /*Make object instances*/

    printf("\r\nGattServer demo of a writable characteristic\r\n");

    /* this process will handle basic setup and advertising for us */
    GattServerProcess ble_process(event_queue, ble);

    /* once it's done it will let us continue with our demo*/
    ble_process.on_init(callback(&demo, &GattServerDemo::start));

    ble_process.start();

    // wirecutter.feedCable(500);
    // ThisThread::sleep_for(chrono::milliseconds(2000));
    // wirecutter.stripCable();
    // ThisThread::sleep_for(chrono::milliseconds(2000));
    // wirecutter.feedCable(2000);
    // ThisThread::sleep_for(chrono::milliseconds(2000));
    // wirecutter.stripCable();
    // ThisThread::sleep_for(chrono::milliseconds(2000));
    // wirecutter.feedCable(500);
    // ThisThread::sleep_for(chrono::milliseconds(2000));
    // wirecutter.cutCable();
    // ThisThread::sleep_for(chrono::milliseconds(2000));

}
