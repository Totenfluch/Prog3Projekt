/**
 * @file srf02.h
 *
 */

/**
 * @file srf02.h
 *
 */

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

/**
 * @brief 
 *
 * @param fd
 * @param command
 * @return int
 */
int writeCommand (int fd, uint8_t command);
/**
 * @brief 
 *
 * @param fd
 * @param command
 * @param data
 * @return int
 */
int writeByte (int fd, uint8_t command, uint8_t data );
/**
 * @brief 
 *
 * @param fd
 * @param command
 * @param size
 * @param data
 */
void readBlock (int fd, uint8_t command, uint8_t size, uint8_t *data);
/**
 * @brief 
 *
 * @param fd
 * @param command
 * @return int
 */
extern int readByte (int fd, uint8_t command);
/**
 * @brief 
 *
 * @return int
 */
extern int initi2c();

#ifdef __cplusplus
}
#endif

#endif
