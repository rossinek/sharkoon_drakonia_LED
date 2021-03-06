// LED driver for mouses by Sharkoon and Tracer
// Tested on Sharkoon Drakonia / Tracer Recon
//
// author:  Artur Rosa
// version: 0.0.1
//
// dependencies: libusb-1.0
// gcc sharkoonLED.c -o sharkoonLED -lusb-1.0

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

#include <libusb-1.0/libusb.h>

#define VENDOR_ID   0x04d9
#define PRODUCT_ID  0xa067

unsigned char data_colors[][8] = {
    { 0x07, 0x3A, 0x23, 0xB0, 0x07, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x23, 0xB0, 0x07, 0x33, 0xA7, 0xAE },
    { 0x07, 0x3A, 0x23, 0xB0, 0x07, 0x33, 0xA7, 0xA2 },
    { 0x07, 0x42, 0x23, 0xB0, 0x03, 0x33, 0xA7, 0xA6 },
    { 0x07, 0x3A, 0x1B, 0xB0, 0x03, 0x33, 0xA7, 0xAA },
    { 0x07, 0x42, 0x23, 0xB0, 0x03, 0x33, 0xA7, 0xAC },
    { 0x07, 0x42, 0x1B, 0xB0, 0x07, 0x33, 0xA7, 0xAA },
    { 0x07, 0x42, 0x1B, 0xB0, 0x07, 0x33, 0xA7, 0xAE },
    { 0x07, 0x42, 0x23, 0xB0, 0x03, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x42, 0x1B, 0xB0, 0x07, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x32, 0x1B, 0xB0, 0x07, 0x33, 0xA7, 0xBA },
    { 0x07, 0x12, 0x0F, 0xB0, 0x07, 0x33, 0xA7, 0xBA },
    { 0x07, 0x12, 0x2B, 0xB0, 0x07, 0x33, 0xA7, 0xC2 },
    { 0x07, 0x2A, 0x1F, 0xB0, 0x03, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x27, 0xB0, 0x03, 0x33, 0xA7, 0xB0 },
    { 0x07, 0x32, 0x23, 0xB0, 0x03, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x25, 0xB0, 0x03, 0x33, 0xA7, 0xB6 },
    { 0x07, 0x3A, 0x23, 0xB0, 0x07, 0x33, 0xA7, 0xB7 },
    { 0x07, 0x2A, 0x23, 0xB0, 0x07, 0x33, 0xA7, 0xAA },
    { 0x07, 0x3A, 0x2B, 0xB0, 0x03, 0x33, 0xA7, 0x98 },
    { 0x07, 0x32, 0x1B, 0xB0, 0x07, 0x33, 0xA7, 0xAA },
    { 0x07, 0x3A, 0x21, 0xB0, 0x05, 0x33, 0xA7, 0xB0 },
    { 0x07, 0x32, 0x2F, 0xB0, 0x07, 0x33, 0xA7, 0xA2 },
    { 0x07, 0x32, 0x73, 0xB0, 0x03, 0x33, 0xA7, 0xAA }
};

/*
unsigned char data_colors[][8] = {
    { 0x07, 0x3A, 0x2B, 0xB0, 0x17, 0x33, 0xA7, 0xB2 }, // off
    { 0x07, 0x3A, 0x2C, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x2D, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x2E, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x2F, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x30, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x31, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x32, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },

    { 0x07, 0x3A, 0x33, 0xB0, 0x17, 0x33, 0xA7, 0xB2 }, // low
    { 0x07, 0x3A, 0x34, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x35, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x36, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x37, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x38, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x39, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x3A, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },

    { 0x07, 0x3A, 0x3B, 0xB0, 0x17, 0x33, 0xA7, 0xB2 }, // middle
    { 0x07, 0x3A, 0x3C, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x3D, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x3E, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x3F, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x40, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x41, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x42, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },

    { 0x07, 0x3A, 0x43, 0xB0, 0x17, 0x33, 0xA7, 0xB2 }, // high

    { 0x07, 0x3A, 0x43, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x44, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x45, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x46, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x47, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x48, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x49, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x4A, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x4B, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x4C, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x4D, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x4E, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x4F, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },

    { 0x07, 0x3A, 0x53, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x66, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x77, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x88, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x99, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0xAA, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0xBB, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0xCC, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0xDD, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0xEE, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0xFF, 0xB0, 0x17, 0x33, 0xA7, 0xB2 },

};*/

#define COLORS_NUM  sizeof(data_colors)/sizeof(8*sizeof(unsigned char))

unsigned char data_pulsation[][8] = {
    { 0x07, 0x3A, 0x2B, 0xB0, 0x0F, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x33, 0xB0, 0x0F, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x3B, 0xB0, 0x0F, 0x33, 0xA7, 0xB2 },
    { 0x07, 0x3A, 0x43, 0xB0, 0x0F, 0x33, 0xA7, 0xB2 }
};

unsigned char data_intensity[][8] = {
    { 0x07, 0x3A, 0x2B, 0xB0, 0x17, 0x33, 0xA7, 0xB2 }, // off
    { 0x07, 0x3A, 0x33, 0xB0, 0x17, 0x33, 0xA7, 0xB2 }, // low
    { 0x07, 0x3A, 0x3B, 0xB0, 0x17, 0x33, 0xA7, 0xB2 }, // middle
    { 0x07, 0x3A, 0x43, 0xB0, 0x17, 0x33, 0xA7, 0xB2 }, // high
};

struct libusb_device_handle* init_device();
void close_device();
int select_color(struct libusb_device_handle *handle, int intensity, int pulsation);
int change_color(struct libusb_device_handle *handle, int color, int intensity, int pulsation);

int is_number(unsigned char *str);
char getch();

void usage() {
    printf("Usage: ./sharkoonLED [intensity] [pulsation] [color]\n");
    printf("\tintensity:\n");
    printf("\t\t0 - off\n");
    printf("\t\t1 - low (default)\n");
    printf("\t\t2 - middle\n");
    printf("\t\t3 - high\n");
    printf("\tpulsation:\n");
    printf("\t\t0 - steady (default)\n");
    printf("\t\t1 - slow\n");
    printf("\t\t2 - middle\n");
    printf("\t\t3 - fast\n");
    printf("\tcolor:\n");
    printf("\t\t0 - select color in program (default)\n");
    printf("\t\t1-24 - color index\n");
}

int main(int argc, char **argv) {
    int color = 0;
    int intensity = 1;
    int pulsation = 0;

    if(argc>1) {
        if(is_number(argv[1])) intensity = atoi(argv[1]);
        else intensity = -1;
    } if(argc>2) {
        if(is_number(argv[2])) pulsation = atoi(argv[2]);
        else pulsation = -1;
    } if(argc>3) {
        if(is_number(argv[3])) color = atoi(argv[3]);
        else color = -1;
    } if(argc>4 || color<0 || color>24 || intensity<0 || intensity>3 || pulsation<0 || pulsation>3) {
        usage();
        return 0;
    }

    struct libusb_device_handle *usb_handle;
    usb_handle = init_device();
    if(usb_handle == NULL) {
        fprintf(stderr, "Device initialization faild.\n");
        return -1;
    }

    color = color-1;
    if(color < 0) {
        if(intensity) color = select_color(usb_handle, intensity, pulsation);
        else color = 0;
    } else {
        change_color(usb_handle, color, intensity, pulsation);
    }

    close_device(usb_handle);

    return 0;
}

int select_color(struct libusb_device_handle *handle, int intensity, int pulsation) {
    printf("Select color by pressing left/right arrow. ");
    printf("Press enter to confirm.\n");

    int color = 0;
    change_color(handle, color, intensity?intensity:1, pulsation);

    char c;
    while((c=getch()) != '\n') {
        // Pressing arrow key push tree values into the buffer:
        // '\033', '[', 'A'|'B'|'C'|'D'
        if (c == '\033') {
            getch(); // skip the [
            switch(getch()) { // the real value
                case 'D': // code for left arrow
                    color = (color-1 < 0) ? COLORS_NUM-1 : color-1;
                    break;
                case 'C': // code for right arrow
                    color = (color+1 >= COLORS_NUM) ? 0 : color+1;
                    break;
                default:
                    continue;
            }
            change_color(handle, color, -1, -1);
        } else {
            printf("Wrong key.\n");
        }
    }

    printf("You have selected color with index %d.\n", color+1);
    return color;
}

int change_color(struct libusb_device_handle *handle, int color, int intensity, int pulsation) {
    int res;
    // color
    if((res=libusb_control_transfer(handle, 0x21, 0x9, 0x307, 0x1, (char*)(data_colors[color]), 0x8, 5000))<=0) {
        fprintf(stderr, "libusb_control_transfer() failed. (%s)\n", libusb_strerror(res));
        return -1;
    }
    // pulsation
    if(pulsation>=0) {
        if((res=libusb_control_transfer(handle, 0x21, 0x9, 0x307, 0x1, (char*)(data_pulsation[pulsation]), 0x8, 5000))<=0) {
            fprintf(stderr, "libusb_control_transfer() failed. (%s)\n", libusb_strerror(res));
            return -1;
        }
    }
    // intensity
    if(intensity>=0){
        if((res=libusb_control_transfer(handle, 0x21, 0x9, 0x307, 0x1, (char*)(data_intensity[intensity]), 0x8, 5000))<=0) {
            fprintf(stderr, "libusb_control_transfer() failed. (%s)\n", libusb_strerror(res));
            return -1;
        }
    }

    return 0;
}

struct libusb_device_handle* init_device() {
    int res = 0;
    struct libusb_device_handle *usb_handle;

    libusb_init(NULL);
    usb_handle = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, PRODUCT_ID);

    if(usb_handle == NULL) {
        fprintf(stderr, "Unable to open USB device (probably you need sudo)!\n");
        return NULL;
    }
    #ifndef __APPLE__
        if(res=libusb_detach_kernel_driver(usb_handle, 1)){
            fprintf(stderr, "usb_detach_kernel_driver_np Error. (%s)\n", libusb_strerror(res));
            close_device(usb_handle);
            return NULL;
        }
        if((res=libusb_claim_interface(usb_handle, 1)) < 0) {
            fprintf(stderr, "libusb_claim_interface Error. (%s)\n", libusb_strerror(res));
            close_device(usb_handle);
            return NULL;
        }
    #endif

    return usb_handle;
}

void close_device(struct libusb_device_handle* usb_handle) {
    int res;
    if(usb_handle) {
        #ifndef __APPLE__
            if(res=libusb_release_interface(usb_handle, 1)) {
                fprintf(stderr, "usb_release_interface() failed. (%s)\n", libusb_strerror(res));
            }
            if(res=libusb_attach_kernel_driver(usb_handle, 1)) {
                fprintf(stderr, "usb_attach_kernel_driver_np Error. (%s)\n", libusb_strerror(res));
            }
        #endif
        if(res=libusb_reset_device(usb_handle)) {
            fprintf(stderr, "usb_reset Error.(%s)\n", libusb_strerror(res));
        }
        libusb_close(usb_handle);
    }
}

int is_number(unsigned char *str) {
    char buf[7] = {0};
    return (strtok(strncpy(buf, str, sizeof(buf)), "0123456789") == NULL);
}

// by mf_: http://stackoverflow.com/users/2096350/mf
char getch(){
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    return buf;
 }
