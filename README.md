System requirement
- Ubuntu 14.04 or higher
- [Yocto SDK](https://drive.google.com/file/d/1tu3uZDmJ5V4szkXpgja05D6zrZ-gqPg0/view?usp=sharing)

Setup SDK
```
run sdk installation script
set build environment by 
. ./environment-setup-cortexa9hf-neon-poky-linux-gnueabi
```
Make SiS tool
```
    make
```
Parameters
```
    -ba 
    -n=<i2c deivce node>
    -n=/dev/sis_hydra_touch_device 
    -io-interval=<milliseconds dealy>
    -io-interval=2
    -debug
```