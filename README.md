stm32L0x customized bootloader 
features:
1. Connect MCU to pc using UART protocol for transferring commands and .bin file
2. Commands include:\
  2.1 HELP\
  2.2 version\
  2.3 flash sector status\
  2.4 manufacturer ID\
  2.5 jump to specific address\
  2.6 Erase\
  2.7 write
3. Checksum with CRC
4. Packet's format from pc to stm32l0x includes:\
  4.1 header: 1 byte\
  4.2 Data-command: 1 byte\
  4.3 CRC: 4 bytes
