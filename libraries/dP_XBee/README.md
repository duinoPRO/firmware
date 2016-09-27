# USING XBee WITH duinoPRO

The duinoPRO XBee module and associated dP_XBee library are intended to provide a simple way to interface to Digi XBee units.  Generally XBee modules can be addressed via a serial UART interface, or via an SPI interface.  Both methods are supported in the dP_XBee library.

The Digi XBee units come in a range of versions.  At this stage, the Digi XBee Zigbee RF units are preferred for use with duinoPRO.  These are available in versions with a PCB antenna, a wire antenna, or various connectors for external antennas.  Units based on the S2C hardware are preferred.

Digi Xbee-PRO are not supported at the time of writing although it is expected that they will be in the future.

Digi XBee units are available in through-hole and surface-mount versions - duinoPRO supports only the through-hole units.

## UART Interface

To connect to a Digi XBee unit via UART, the duinoPRO XBee module must be located in a module position that provides a UART interface.  For the UNO baseboard, this means it must be in module locations 6/7 (NB: the duinoPRO XBee module takes up 2 module locations, as it is a double size module).  For the DUE baseboard, it must be in module locations 6/7 or 2/3 (it is not possible to use the UART interfaces in module locations 1 and 4 due to the mechanical arrangements of the double size module).

When the duinoPRO XBee module is in location 6/7, the UART is multiplexed with the normal duinoPRO Serial monitor function, meaning that both cannot be used at the same time.

## SPI Interface

When the SPI interface is used, the duinoPRO XBee module can be located in any adjacent pair of module locations (exception: module location 4, due to module footprint orientation).

When SPI is used, the Digi XBee unit must support SPI operation, and must be configured to enable the SPI pins.  For some Digi XBee firmware, SPI is enabled by default.  If it is not, it can be enabled using a Digi XBIB-U-DEV board and Digi XCTU PC software, or using a suitable duinoPRO sketch.

We have tested using the following XBee hardware and firmware:
- XBee S2C hardware, firmware 4055 - this firmware appears to have the relevant SPI pins enabled by default, so configuration is not necessary.
- XBee S2C hardware, firmware 4059 - SPI pins must be explicitly configured.  Relevant pins are D1, D2, D3, D4 and P2.
- XBee S1 hardware - this hardware does not appear to support SPI, and so only the UART interface is available.  For this reason, it is recommended to use S2C hardware.

## General Comments

Resources available for getting going with XBee are available from the Digi website (www.digi.com).  

Some general notes on getting going:

1. One Digi XBee unit needs to be configured to be the co-ordinator, and the others as not a co-ordinator.
2. Ensure all units have the same network ID (PAN ID, where PAN stands for Personal Area Network). 
3. If there are other Zigbee networks operating in the vicinity, it may be necessary to change the PAN ID to a unique value (and in particular away from the default value).
4. Destination address defaults to DH=0, DL=0 (DH and DL are Destination Address High and Destination Address Low respectively).  This default is the address of the PAN co-ordinator.  To send messages to any unit other than the co-ordinator, the destination address will need to be changed.  A useful address is DH=0, DL=FFFF, which is the PAN broadcast address.
