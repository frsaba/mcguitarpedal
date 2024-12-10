#include "snapshot.h"
#include "Arduino.h"


// Define the buffer size for the serial output
#define SERIAL_BUFFER_SIZE 64

void send_serial_data(const uint8_t *data, size_t size) {
    for(size_t i = 0; i < size; i++) {
        // Replace with your platform's serial write function
        Serial.printf("%x", (data[i]));
    }
}

void snapshot_and_send(lv_obj_t *widget) {
    // Take a snapshot of the widget
    lv_draw_buf_t *snapshot = lv_snapshot_take(widget, LV_COLOR_FORMAT_RGB565);
    if (!snapshot) {
        printf("Failed to take snapshot.\n");
        return;
    }

    // Get the width and height of the snapshot
    uint16_t width = lv_obj_get_width(widget);
    uint16_t height = lv_obj_get_height(widget);

    // Send a header to indicate the start of the image data
    printf("SNAPSHOT START\n");
    printf("WIDTH: %u\nHEIGHT: %u\n", width, height);

    // Send the image data through serial in chunks
    const uint8_t *data = (const uint8_t *)snapshot->data;
    uint32_t bytes_sent = 0;
    while (bytes_sent < snapshot->data_size) {
        uint32_t chunk_size = SERIAL_BUFFER_SIZE;
        if (bytes_sent + chunk_size > snapshot->data_size) {
            chunk_size = snapshot->data_size - bytes_sent;
        }
        send_serial_data(data + bytes_sent, chunk_size);
		// Serial.printf("Sent %u/%u bytes\n", bytes_sent, snapshot->data_size);
        bytes_sent += chunk_size;
    }

    // End marker
    printf("\nSNAPSHOT END\n");

    // Free the snapshot memory
    lv_draw_buf_destroy(snapshot);
}
