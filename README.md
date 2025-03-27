# Teensy 4.1 Guitar Effects Pedal
![20241214_222209](https://github.com/user-attachments/assets/0a60ef59-d743-497a-87c2-4d041d2b118e)

## Hardware
[Teensy 4.1](https://www.pjrc.com/store/teensy41.html)
[Teensy Audio Adaptor board](https://www.pjrc.com/store/teensy3_audio.html)
ILI9488 LCD panel (backlight brightness adjustable with PWM)

#### Custom dual-layer PCB design:
- LM4562 based preamplifier stage with adjustable gain (1-20x) and 3-way attenuation switch for line-level compatibility
- 8+2+1 programmable buttons (up to 8 pushbuttons, 2 footswitches, 1 true bypass switch)
- 3 potentiometers (preamp gain + 2 programmable)
- 3 encoders (3 headers, but 2 are in use for now)
- 8 programmable LEDs
- 9V barrel jack input or USB power

- [PCB design, front side](https://github.com/user-attachments/files/19494445/pcb_front.pdf)
- [PCB design, back side](https://github.com/user-attachments/files/19494519/pcb_back.pdf)

![pcb_3d](https://github.com/user-attachments/assets/49988ae7-5a76-4c8e-84f6-c427fbfa914a)
![20241130_164746_cropped](https://github.com/user-attachments/assets/5694d958-2f48-4fdd-ab0e-5ff6980f60da)

## Software

C++ with PlatformIO
[Teensy Audio Library](https://github.com/PaulStoffregen/Audio)
UI made with [LVGL](https://lvgl.io)
5 implemented effects (more to come):
- Chorus
- Flanger
- Tremolo
- Delay
- Reverb

#### Misc. features
- Tuner mode
- Adjustable LED and display brightness
- Bypass switch
- Signal level feedback LED
- USB audio device for recording
- Headphone monitor out via Audio Board