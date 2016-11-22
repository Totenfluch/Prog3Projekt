/****************************************************************************
 * srf02 library for Prog3 course at the
 * university of Coburg.
 *
 * @author Peter Emmerling
 * Provides simple functions to control an SRF02 ultrasonic device via i2c.
 *
 *
 *******************************************************************************/

#ifndef _SRF02_H
#define _SRF02_H

#ifdef __cplusplus
extern "C"{
#endif

int writeCommand (int fd, uint8_t command);
int writeByte (int fd, uint8_t command, uint8_t data );
void readBlock (int fd, uint8_t command, uint8_t size, uint8_t *data);
extern int readByte (int fd, uint8_t command);
extern int initi2c();

#ifdef __cplusplus
}
#endif

#endif
