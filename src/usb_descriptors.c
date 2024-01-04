#include "tusb.h"

tusb_desc_device_t const desc_device = {
        .bLength = sizeof(tusb_desc_device_t),
        .bDescriptorType = TUSB_DESC_DEVICE,
        .bcdUSB = 0x0200,
        .bDeviceClass = 0x00,
        .bDeviceSubClass = 0x00,
        .bDeviceProtocol = 0x00,
        .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
        .idVendor = (0x1209 - 0x02fc),
        .idProduct = (0xcade - 0xca1d),
        .bcdDevice = 0x0200,
        .iManufacturer = 0x01,
        .iProduct = 0x02,
        .iSerialNumber = 0x03,
        .bNumConfigurations = 0x01
};

uint8_t const *tud_descriptor_device_cb(void){
    return (uint8_t const *)&desc_device;
}

uint8_t const desc_hid_report[] = {
	0x05, 0x01, /* USAGE_PAGE (Generic Desktop)         */
	0x09, 0x05, /* USAGE (Gamepad)                      */
	0xa1, 0x01, /* COLLECTION (Application)             */
	0x15, 0x00, /*     LOGICAL_MINIMUM (0)              */
	0x25, 0x01, /*     LOGICAL_MAXIMUM (1)              */
	0x35, 0x00, /*     PHYSICAL_MINIMUM (0)             */
	0x45, 0x01, /*     PHYSICAL_MAXIMUM (1)             */
	0x75, 0x01, /*     REPORT_SIZE (1)                  */
	0x95, 0x0e, /*     REPORT_COUNT (14)                */
	0x05, 0x09, /*     USAGE_PAGE (Buttons)             */
	0x19, 0x01, /*     USAGE_MINIMUM (Button 1)         */
	0x29, 0x0e, /*     USAGE_MAXIMUM (Button 14)        */
	0x81, 0x02, /*     INPUT (Data,Var,Abs)             */
	0x95, 0x02, /*     REPORT_COUNT (2)                 */
	0x81, 0x01, /*     INPUT (Data,Var,Abs)             */
	0x05, 0x01, /*     USAGE_PAGE (Generic Desktop Ctr) */
	0x25, 0x07, /*     LOGICAL_MAXIMUM (7)              */
	0x46, 0x3b, 0x01, /*     PHYSICAL_MAXIMUM (315)     */
	0x75, 0x04, /*     REPORT_SIZE (4)                  */
	0x95, 0x01, /*     REPORT_COUNT (1)                 */
	0x65, 0x14, /*     UNIT (20)                        */
	0x09, 0x39, /*     USAGE (Hat Switch)               */
	0x81, 0x42, /*     INPUT (Data,Var,Abs)             */
	0x65, 0x00, /*     UNIT (0)                         */
	0x95, 0x01, /*     REPORT_COUNT (1)                 */
	0x81, 0x01, /*     INPUT (Data,Var,Abs)             */
	0x26, 0xff, 0x00, /*     LOGICAL_MAXIMUM (255)      */
	0x46, 0xff, 0x00, /*     PHYSICAL_MAXIMUM (255)     */
	0x09, 0x30, /*     USAGE (Direction-X)              */
	0x09, 0x31, /*     USAGE (Direction-Y)              */
	0x09, 0x32, /*     USAGE (Direction-Z)              */
	0x09, 0x35, /*     USAGE (Rotate-Z)                 */
	0x75, 0x08, /*     REPORT_SIZE (8)                  */
	0x95, 0x04, /*     REPORT_COUNT (4)                 */
	0x81, 0x02, /*     INPUT (Data,Var,Abs)             */
	0x75, 0x08, /*     REPORT_SIZE (8)                  */
	0x95, 0x01, /*     REPORT_COUNT (1)                 */
	0x81, 0x01, /*     INPUT (Data,Var,Abs)             */
	0xc0,       /*   END_COLLECTION                     */
};

uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance){
    (void)instance;
    return desc_hid_report;
}

enum{
    ITF_NUM_HID,
    ITF_NUM_TOTAL
};

#define  CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN)
#define EPNUM_HID   0x81

uint8_t const desc_configuration[] = {
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),
    TUD_HID_DESCRIPTOR(ITF_NUM_HID, 0, HID_ITF_PROTOCOL_NONE, sizeof(desc_hid_report), EPNUM_HID,
    CFG_TUD_HID_BUFSIZE, 1)
};

uint8_t const *tud_descriptor_configuration_cb(uint8_t index) {
    (void) index; // for multiple configurations
    return desc_configuration;
}

char const *string_desc_arr[] =
    {
        (const char[]){0x09, 0x04},
        "RetroBeef",
        "X2X",
        "V1",
};

static uint16_t _desc_str[32];

uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid){
    (void)langid;

    uint8_t chr_count;

    if (index == 0){
        memcpy(&_desc_str[1], string_desc_arr[0], 2);
        chr_count = 1;
    }else{
        if (!(index < sizeof(string_desc_arr) / sizeof(string_desc_arr[0]))) return NULL;

        const char *str = string_desc_arr[index];

        chr_count = strlen(str);
        if (chr_count > 31) chr_count = 31;

        for (uint8_t i = 0; i < chr_count; i++){
            _desc_str[1 + i] = str[i];
        }
    }

    _desc_str[0] = (TUSB_DESC_STRING << 8) | (2 * chr_count + 2);

    return _desc_str;
}
