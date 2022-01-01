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

#include <Arduino.h>
#include <LTC7871.h>

/*
 * Constructor of the class
 * @param io_pin_cs a byte indicating the pin to be use as the chip select pin (CS)
 */
LTC7871::LTC7871(uint8_t chipSelect) {
	cs = chipSelect;
}

void LTC7871::begin() {
	// Configure the SPI interface
	SPI.begin();
	pinMode(cs, OUTPUT);
	digitalWrite(cs, HIGH);
}

uint8_t LTC7871::PEC(uint16_t data) {
   	byte crc_init = 0x41;
   
	uint8_t addr = data >> 8;
	uint8_t datab = data & 0xFF;
	uint8_t x = crc_init ^ addr;

	uint8_t PEC = crcTable[x >> 4][x & 0xF];
	
	x = PEC ^ datab;
	PEC = crcTable[x >> 4][x & 0xF];
/*
	// Conversion in datasheet, now using lookup table to speed things up
	uint16_t PECa[8] = {1, 0, 0, 0, 0, 0, 1, 0}; //0x01000001, reversed;
	uint16_t DIN[8] = {0};
	uint8_t IN0 = 0;
	uint8_t IN1 = 0;
	uint8_t IN2 = 0;
	for (int j = 0; j <= 7; j++) {
		DIN[j] = bitRead(data,7-j);

		IN0 = DIN[j] ^ PECa[7];
		IN1 = PECa[0] ^ IN0;
		IN2 = PECa[1] ^ IN0;

		PECa[7] = PECa[6];
		PECa[6] = PECa[5];
		PECa[5] = PECa[4];
		PECa[4] = PECa[3];
		PECa[3] = PECa[2];
		PECa[2] = IN2;
		PECa[1] = IN1;
		PECa[0] = IN0;

	}

	uint8_t PEC = 0;
	for (int i = 0; i <= 7; i++) {
		bitWrite(PEC, i, PECa[i]);
	}*/

	return PEC;
}

uint8_t LTC7871::getFaults() {
	uint8_t ret = readData(MFR_FAULT);
	
	if (ret != 0) {
		if (ret & 0b00000001) {
			Serial.print("Over Temp; ");
		}
		if (ret & 0b00000010) {
			Serial.print("Vref Fault; ");
		}
		if (ret & 0b00000100) {
			Serial.print("V5 Undervoltage; ");
		}
		if (ret & 0b00001000) {
			Serial.print("DRVcc Pin Undervoltage; ");
		}
		if (ret & 0b00010000) {
			Serial.print("UVhigh < 1.2V; ");
		}
		if (ret & 0b00100000) {
			Serial.print("OVhigh > 1.2V; ");
		}
		if (ret & 0b01000000) {
			Serial.print("OVlow > 1.2V; ");
		}
	Serial.println();
	}
	
	return ret;
}

uint8_t LTC7871::getOCFaults() {
	uint8_t ret = readData(MFR_OC_FAULT);
	
	if (ret != 0) {
		if (ret & 0b00000001) {
			Serial.print("Overcurrent Channel 1; ");
		}
		if (ret & 0b00000010) {
			Serial.print("Overcurrent Channel 2; ");
		}
		if (ret & 0b00000100) {
			Serial.print("Overcurrent Channel 3; ");
		}
		if (ret & 0b00001000) {
			Serial.print("Overcurrent Channel 4; ");
		}
		if (ret & 0b00010000) {
			Serial.print("Overcurrent Channel 5; ");
		}
		if (ret & 0b00100000) {
			Serial.print("Overcurrent Channel 6; ");
		}
		Serial.println();
	}
	return ret;
}

uint8_t LTC7871::getNOCFaults() {
	uint8_t ret = readData(MFR_NOC_FAULT);

	if (ret != 0) {
		if (ret & 0b00000001) {
			Serial.print("Negative Overcurrent Channel 1; ");
		}
		if (ret & 0b00000010) {
			Serial.print("Negative Overcurrent Channel 2; ");
		}
		if (ret & 0b00000100) {
			Serial.print("Negative Overcurrent Channel 3; ");
		}
		if (ret & 0b00001000) {
			Serial.print("Negative Overcurrent Channel 4; ");
		}
		if (ret & 0b00010000) {
			Serial.print("Negative Overcurrent Channel 5; ");
		}
		if (ret & 0b00100000) {
			Serial.print("Negative Overcurrent Channel 6; ");
		}
		Serial.println();
	}
	return ret;
}

uint8_t LTC7871::getStatus() {
	uint8_t ret = readData(MFR_STATUS);
	
	if (ret != 0) {
		if (ret & 0b00000001) {
			Serial.print("Vlow/Vhigh within 10% regulation; ");
		}
		if (ret & 0b00000010) {
			Serial.print("Ilim Reached; ");
		}
		if (ret & 0b00000100) {
			Serial.print("Soft-start Finished; ");
		}
		Serial.println();
	}
	return ret;
}

uint8_t LTC7871::getConfig1() {
	uint8_t ret = readData(MFR_CONFIG1);
	
	
	if ((ret & 0b00000111) == 0b000000000) {
		Serial.print("Max Current Sense threshold is 10mV; ");
	} else if ((ret & 0b00000111) == 0b000000001) {
		Serial.print("Max Current Sense threshold is 20mV; ");
	} else if ((ret & 0b00000111) == 0b000000010) {
		Serial.print("Max Current Sense threshold is 30mV; ");
	} else if ((ret & 0b00000111) == 0b000000011) {
		Serial.print("Max Current Sense threshold is 40mV; ");
	} else if ((ret & 0b00000111) == 0b000000100) {
		Serial.print("Max Current Sense threshold is 50mV; ");
	}
	if ((ret & 0b00011000) == 0b00000000) {
		Serial.print("DRVcc is 5V; ");
	} else if ((ret & 0b00011000) == 0b00001000) {
		Serial.print("DRVcc is 8V; ");
	} else if ((ret & 0b00011000) == 0b00010000) {
		Serial.print("DRVcc is 10V; ");
	}
	if (bitRead(ret, 5)) {
		Serial.print("SETCUR set above 1.25V; ");
	}
	Serial.println();
	return ret;
}

uint8_t LTC7871::getConfig2() {
	uint8_t ret = readData(MFR_CONFIG2);
	
	if (ret & 0b00000001) {
		Serial.print("In Buck mode; ");
	} else {
		Serial.print("In Boost mode; ");
	}
	if (ret & 0b00000010) {
		Serial.print("In Spread Spectrum mode; ");
	}
	if (ret & 0b00000100) {
		Serial.print("In Hi-Z mode; ");
	}
	if (ret & 0b00001000) {
		Serial.print("The controller is in DCM; ");
	}
	if (ret & 0b00010000) {
		Serial.print("The controller is in burst mode operation; ");
	}
	Serial.println();
	return ret;
}

uint8_t LTC7871::getChipCtrl() {
	uint8_t ret = readData(MFR_CHIP_CTRL);
	
	if (ret & 0b00000001) {
		Serial.print("Write inhibited; ");
	} else {
		Serial.print("Write allowed; ");
	}
	if (ret & 0b00000010) {
		Serial.print("Reset all R/W registers; ");
	}
	if (ret & 0b00000100) {
		Serial.print("A communication fault related to PEC during writing registers has occurred. Write 1 to this bit to clear the CML; ");
	}
	Serial.println();
	return ret;
}

int8_t LTC7871::getIDACVlow() {
	int8_t ret = readData(MFR_IDAC_VLOW);

	if ((ret & 0b01000000) >> 6) { // if bit 7 is high, then value is negative so take twos compliments (invert and add 1)
		ret = -((~ret & 0b00111111) + 1);
	}

	return ret;
}

void LTC7871::setIDACVlow(int8_t setPt) {
	writeData(MFR_IDAC_VLOW, setPt);
}

int8_t LTC7871::getIDACVhigh() {
	int8_t ret = readData(MFR_IDAC_VHIGH);

	if ((ret & 0b01000000) >> 6) { // if bit 7 is high, then value is negative so take twos compliments (invert and add 1)
		ret = -((~ret & 0b00111111) + 1);
	}

	return ret;
}

void LTC7871::setIDACVhigh(int8_t setPt) {
	writeData(MFR_IDAC_VHIGH, setPt);
}

uint8_t LTC7871::getCur() {
	uint8_t ret = readData(MFR_IDAC_SETCUR);
	
	if ((ret & 0b00010000) >> 4) { // convert to 5 bit twos compliment
		ret = ret - 16;
	} else {
		ret = ret + 16;
	}

	return ret;
}

void LTC7871::setCur(uint8_t setPt) {
	if (setPt < 0) {
		setPt = 0;
	}
	else if (setPt > 31) {
		setPt = 31;
	}
		
	if (setPt >= 16) {
		setPt = (~setPt + 1) & 0b00001111;
	}
	else {
		setPt = (setPt & 0b00001111) | 0b00010000;
	}
	writeData(MFR_IDAC_SETCUR, setPt);
}

uint8_t LTC7871::getSSFM() {
	uint8_t ret = readData(MFR_IDAC_VHIGH);

	if ((ret & 0b00000111) == 0b00000000 || (ret & 0b00000111) == 0b00000111) {
		Serial.print("Sw. Freq / 512;");
	} else if ((ret & 0b00000111) == 0b00000001) {
		Serial.print("Sw. Freq / 1024;");
	} else if ((ret & 0b00000111) == 0b00000010) {
		Serial.print("Sw. Freq / 2048;");
	} else if ((ret & 0b00000111) == 0b00000011) {
		Serial.print("Sw. Freq / 4096;");
	} else if ((ret & 0b00000111) == 0b00000100) {
		Serial.print("Sw. Freq / 256;");
	} else if ((ret & 0b00000111) == 0b00000101) {
		Serial.print("Sw. Freq / 128;");
	} else if ((ret & 0b00000111) == 0b00000110) {
		Serial.print("Sw. Freq / 64;");
	}
	if ((ret & 0b00011000) == 0b00000000) {
		Serial.print("±12%;");
	} else if ((ret & 0b00011000) == 0b00000001) {
		Serial.print("±15%;");
	} else if ((ret & 0b00011000) == 0b00000010) {
		Serial.print("±10%;");
	} else if ((ret & 0b00011000) == 0b00000011) {
		Serial.print("±8%;");
	}
	Serial.println();
	return ret;
}

void LTC7871::setCML(bool set) {
	uint8_t data = 0b00000100;
	writeData(MFR_CHIP_CTRL, data);
}

void LTC7871::setReset(bool set) {
	uint8_t data = 0b00000010;
	writeData(MFR_CHIP_CTRL, data);
}

void LTC7871::setWriteProtect(bool set) {
	uint8_t data = 0b00000000;
	if (set == true) {
		data = 0b00000001;
	}
	writeData(MFR_CHIP_CTRL, data);
}

void LTC7871::setFreqSpread(uint8_t freq) {
	//writeData(MFR_SSFM, )
}

void LTC7871::setModSigFreq(uint16_t modFreq) {
	//writeData(MFR_SSFM, ....)
}

void LTC7871::writeData(uint8_t reg, uint8_t data) {
	uint8_t CRC = PEC((((reg << 1) | WRITE) << 8) + data);

	//Serial.println(((reg << 1) | WRITE),BIN);
	//Serial.println(data,BIN);
	//Serial.println(CRC,BIN);

	SPI.beginTransaction(SPISettings(SCLK, MSBFIRST, SPI_MODE0));
	digitalWrite(cs, LOW);
	delayMicroseconds(5);
	SPI.transfer(reg << 1 | WRITE);
	SPI.transfer(data);
	SPI.transfer(CRC);
	delayMicroseconds(5);
	digitalWrite(cs, HIGH);
	SPI.endTransaction();

}

uint8_t LTC7871::readData(uint8_t reg) {
	//Serial.print("Read: 0b");
	//Serial.println(reg,BIN);
	//Serial.print("Read: 0b");
	//Serial.println(reg << 1 | READ,BIN);


	SPI.beginTransaction(SPISettings(SCLK, MSBFIRST, SPI_MODE0));
	digitalWrite(cs, LOW);
	delayMicroseconds(5);
	reg = (reg << 1 | READ);
	SPI.transfer(reg);
	uint8_t retData = SPI.transfer(0);
	uint8_t CRC = SPI.transfer(0);
	delayMicroseconds(5);
	digitalWrite(cs, HIGH);
	SPI.endTransaction();

	//Serial.print("Data: 0b"); Serial.println(retData, HEX);
	//Serial.print("Ret CRC: 0b"); Serial.println(CRC, BIN);

	if (PEC((reg << 8) + retData) != CRC) {
		Serial.print("CRC Mismatch: 0b"); Serial.println(PEC((reg << 8) + retData));
	};

	return retData;
/*
Serial.print("Address: 0b");
 for(int i = 7; i >= 0; i--) {
    	Serial.print(bitRead(reg << 1 | READ, i));
    }
Serial.println();
Serial.print("AddressCRC: 0b");
uint8_t PECCRC = PEC(reg << 1 | READ);
 for(int i = 7; i >= 0; i--) {
    	Serial.print(bitRead(PECCRC, i));
    }
Serial.println();
	Serial.print("Data: 0b");
    for(int i = 7; i >= 0; i--) {
    	Serial.print(bitRead(retData, i));
    }
	//	Serial.print(bitRead(retData, i));
	Serial.println();

	//Serial.println(retData,BIN);
	Serial.print("CRC: 0b");
	for(int i = 7; i >= 0; i--) {
    	Serial.print(bitRead(CRC, i));
    }
	Serial.println();


Serial.print("Calc_CRC: 0b");
	for(int i = 7; i >= 0; i--) {
    	Serial.print(bitRead(PEC(retData), i));
    }
	//Serial.println(PEC(retData),BIN);
	Serial.println();
	//Serial.println(CRC,BIN);
	return retData;*/
}
