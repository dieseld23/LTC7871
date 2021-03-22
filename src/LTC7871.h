/**
*  Arduino Library for Analog Devices LTC7871 - Six-Phase, Synchronous Bidirectional
*  Buck or Boost Controller
*  
*  @author Dan Maslach <dmaslach@gmail.com>
*/

/**
 * The MIT License
 *
 * Copyright 2018 Dan Maslach <dmaslach@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef LTC7871_h
#define LTC7871_h

#include <Arduino.h>
#include <SPI.h>

#define READ 1
#define WRITE 0

class LTC7871 {
public:
	LTC7871(uint8_t chipSelect);

	void begin();
	uint8_t getFaults();
	uint8_t getOCFaults();
	uint8_t getNOCFaults();
	uint8_t getStatus();
	uint8_t getConfig1();
	uint8_t getConfig2();
	uint8_t getChipCtrl();
	uint8_t getSSFM();
	int8_t getIDACVlow();
	int8_t getIDACVhigh();
	uint8_t getCur();
	void setIDACVlow(int8_t setPt = 0);
	void setIDACVhigh(int8_t setPt = 0);
	void setCur(int8_t setPt = 0);
	void setCML(bool set);
	void setReset(bool set);
	void setWriteProtect(bool set);
	void setFreqSpread(uint8_t freq);
	void setModSigFreq(uint16_t modFreq);

	//Bit constant
	const long int SCLK = 1000000; //  SCLK frequency

private:

	uint8_t cs; // Chip select pin
    
	// Registers
	const uint8_t MFR_FAULT = 0x01;		  // One byte summary of the unit’s fault condition (RO)
	const uint8_t MFR_OC_FAULT = 0x02;	  // One byte summary of the unit’s overcurrent fault condition (RO)
	const uint8_t MFR_NOC_FAULT = 0x03;	  // One byte summary of the unit’s negative overcurrent fault condition (RO)
	const uint8_t MFR_STATUS = 0x04;	  // One byte summary of the unit’s operation status (RO)
	const uint8_t MFR_CONFIG1 = 0x05;	  // One byte summary of the unit’s configuration (RO)
	const uint8_t MFR_CONFIG2 = 0x06;	  // One byte summary of the unit’s configuration (RO)
	const uint8_t MFR_CHIP_CTRL = 0x07;	  // [3] = Communication Fault, [1] = Sticky Bit, [0] = Write Protection (RW - Default 0x00)
	const uint8_t MFR_IDAC_VLOW = 0x08;	  // Adjust the IDAC_VLOW to program VLOW voltage (RW - Default 0x00)
	const uint8_t MFR_IDAC_VHIGH = 0x09;  // Adjust the IDAC_VHIGH to program VHIGH voltage (RW - Default 0x00)
	const uint8_t MFR_IDAC_SETCUR = 0x0A; // Adjust the IDAC_SETCUR to program SETCUR pin’s sourcing current (RW - Default 0x00)
	const uint8_t MFR_SSFM = 0x0B;		  // Adjust the spread spectrum frequency modulation parameters (RW - Default 0x00)

    uint8_t readData(uint8_t reg);
    void writeData(uint8_t reg, int8_t data);
    uint8_t PEC(int8_t data);
};

#endif