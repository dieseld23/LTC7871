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
	return readData(MFR_FAULT);
	/*
	String message = String();
	if ((ret & 0b00000001) == 1) {
		message += "Over Temp;";
	}
	if ((ret & 0b00000010) == 1) {
		message += "Vref Fault;";
	}
	if ((ret & 0b00000100) == 1) {
		message += "V5 Undervoltage;";
	}
	if ((ret & 0b00001000) == 1) {
		message += "DRVcc Pin Undervoltage;";
	}
	if ((ret & 0b00010000) == 1) {
		message += "UVhigh < 1.2V;";
	}
	if ((ret & 0b00100000) == 1) {
		message += "OVhigh > 1.2V;";
	}
	if ((ret & 0b01000000) == 1) {
		message += "OVlow > 1.2V;";
	}*/
}

uint8_t LTC7871::getOCFaults() {
	return readData(MFR_OC_FAULT);
	/*
	String message = String();
	if ((ret & 0b00000001) == 1) {
		message += "Overcurrent Channel 1;";
	}
	if ((ret & 0b00000010) == 1) {
		message += "Overcurrent Channel 2;";
	}
	if ((ret & 0b00000100) == 1) {
		message += "Overcurrent Channel 3;";
	}
	if ((ret & 0b00001000) == 1) {
		message += "Overcurrent Channel 4;";
	}
	if ((ret & 0b00010000) == 1) {
		message += "Overcurrent Channel 5;";
	}
	if ((ret & 0b00100000) == 1) {
		message += "Overcurrent Channel 6;";
	}*/
}

uint8_t LTC7871::getNOCFaults() {
	return readData(MFR_NOC_FAULT);
	/*
	String message = String();
	if ((ret & 0b00000001) == 1) {
		message += "Negative Overcurrent Channel 1;";
	}
	if ((ret & 0b00000010) == 1) {
		message += "Negative Overcurrent Channel 2;";
	}
	if ((ret & 0b00000100) == 1) {
		message += "Negative Overcurrent Channel 3;";
	}
	if ((ret & 0b00001000) == 1) {
		message += "Negative Overcurrent Channel 4;";
	}
	if ((ret & 0b00010000) == 1) {
		message += "Negative Overcurrent Channel 5;";
	}
	if ((ret & 0b00100000) == 1) {
		message += "Negative Overcurrent Channel 6;";
	}*/
}

uint8_t LTC7871::getStatus() {
	return readData(MFR_STATUS);
	/*
	String message = String();
	if ((ret & 0b00000001) == 1) {
		message += "Vlow/Vhigh within 10% regulation;";
	}
	if ((ret & 0b00000010) == 1) {
		message += "Ilim Reached;";
	}
	if ((ret & 0b00000100) == 1) {
		message += "Soft-start Finished;";
	}*/
}

uint8_t LTC7871::getConfig1() {
	return readData(MFR_CONFIG1);
	
	/*
	String message = String();
	if ((ret & 0b00000111) == 0b000000000) {
		message += "Max Current Sense threshold is 10mV;";
	} else if ((ret & 0b00000111) == 0b000000001) {
		message += "Max Current Sense threshold is 20mV;";
	} else if ((ret & 0b00000111) == 0b000000010) {
		message += "Max Current Sense threshold is 30mV;";
	} else if ((ret & 0b00000111) == 0b000000011) {
		message += "Max Current Sense threshold is 40mV;";
	} else if ((ret & 0b00000111) == 0b000000100) {
		message += "Max Current Sense threshold is 50mV;";
	}
	if ((ret & 0b00011000) == 0b00000000) {
		message += "DRVcc is 5V;";
	} else if ((ret & 0b00011000) == 0b00000001) {
		message += "DRVcc is 8V;";
	} else if ((ret & 0b00011000) == 0b00000010) {
		message += "DRVcc is 10V;";
	}
	if (bitRead(ret, 5) == 1) {
		message += "SETCUR set above 1.25V;";
	}*/
}

uint8_t LTC7871::getConfig2() {
	return readData(MFR_CONFIG2);
	/*
	String message = String();
	if ((ret & 0b00000001) == 1) {
		message += "In Buck mode;";
	} else {
		message += "In Boost mode;";
	}
	if ((ret & 0b00000010) == 1) {
		message += "In Spread Spectrum mode;";
	}
	if ((ret & 0b00000100) == 1) {
		message += "In Hi-Z mode;";
	}
	if ((ret & 0b00001000) == 1) {
		message += "The controller is in DCM;";
	}
	if ((ret & 0b00010000) == 1) {
		message += "The controller is in burst mode operation;";
	}*/
}

uint8_t LTC7871::getChipCtrl() {
	return readData(MFR_CHIP_CTRL);
	/*
	String message = String();
	if ((ret & 0b0000001) == 1) {
		message += "Write inhibited;";
	} else {
		message += "Write allowed;";
	}
	if ((ret & 0b00000010) == 1) {
		message += "Reset all R/W registers;";
	}
	if ((ret & 0b00000100) == 1) {
		message += "A communication fault related to PEC during writing registers has occurred. Write 1 to this bit to clear the CML;";
	}*/
}

int8_t LTC7871::getIDACVlow() {
	uint8_t ret = readData(MFR_IDAC_VLOW);

	if ((ret & 0b01000000) == 1) { // if bit 7 is high, then value is negative so take twos compliments (invert and subtract 1)
		ret = (~ret - 1);
		return -ret;
	} else {
		return ret;
	}
}

void LTC7871::setIDACVlow(int8_t setPt) {
	writeData(MFR_IDAC_VLOW, setPt);
}

int8_t LTC7871::getIDACVhigh() {
	uint8_t ret = readData(MFR_IDAC_VHIGH);

	if ((ret & 0b01000000) == 1) { // if bit 7 is high, then value is negative so take twos compliments (invert and subtract 1)
		ret = (~ret - 1);
		return -ret;
	} else {
		return ret;
	}
}

void LTC7871::setIDACVhigh(int8_t setPt) {
	writeData(MFR_IDAC_VHIGH, setPt);
}

uint8_t LTC7871::getCur() {
	uint8_t ret = readData(MFR_IDAC_SETCUR);

	if ((ret & 0b00010000) == 1) { // convert to wierd scale
		ret = ret - 16;
	} else {
		ret = ret + 16;
	}

	return ret;
}

void LTC7871::setCur(int8_t setPt) {
	writeData(MFR_IDAC_SETCUR, setPt);
}

uint8_t LTC7871::getSSFM() {
	return readData(MFR_IDAC_VHIGH);

	/*
	String message = String();	
	if ((ret & 0b00000111) == 0b00000000 || (ret & 0b00000111) == 0b00000111) {
		message += "Sw. Freq / 512;";
	} else if ((ret & 0b00000111) == 0b00000001) {
		message += "Sw. Freq / 1024;";
	} else if ((ret & 0b00000111) == 0b00000010) {
		message += "Sw. Freq / 2048;";
	} else if ((ret & 0b00000111) == 0b00000011) {
		message += "Sw. Freq / 4096;";
	} else if ((ret & 0b00000111) == 0b00000100) {
		message += "Sw. Freq / 256;";
	} else if ((ret & 0b00000111) == 0b00000101) {
		message += "Sw. Freq / 128;";
	} else if ((ret & 0b00000111) == 0b00000110) {
		message += "Sw. Freq / 64;";
	}
	if ((ret & 0b00011000) == 0b00000000) {
		message += "±12%;";
	} else if ((ret & 0b00011000) == 0b00000001) {
		message += "±15%;";
	} else if ((ret & 0b00011000) == 0b00000010) {
		message += "±10%;";
	} else if ((ret & 0b00011000) == 0b00000011) {
		message += "±8%;";
	}*/
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

void LTC7871::writeData(uint8_t reg, int8_t data) {
	uint8_t CRC = PEC(data);
	digitalWrite(cs, LOW);
	delayMicroseconds(10);
	SPI.beginTransaction(SPISettings(SCLK, MSBFIRST, SPI_MODE0));
	SPI.transfer(reg << 1 | WRITE);
	SPI.transfer(data);
	SPI.transfer(CRC);
	SPI.endTransaction();
	digitalWrite(cs, HIGH);
	delayMicroseconds(10);
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
	//delayMicroseconds(10);
	SPI.endTransaction();

	if (PEC((reg << 8) + retData) != CRC) {
		Serial.println("CRC Mismatch!");
	};
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

/*
 * Decoding a configRegister structure and then print it out to the Serial port
 * @param configRegister The config register in "union Config" format

void ADS1018::decodeConfigRegister(union Config configRegister)
{
    String message = String();
    switch (configRegister.bits.singleStart)
    {
    case 0:
        message = "NOINI";
        break;
    case 1:
        message = "START";
        break;
    }
    message += " ";
    switch (configRegister.bits.mux)
    {
    case 0:
        message += "A0-A1";
        break;
    case 1:
        message += "A0-A3";
        break;
    case 2:
        message += "A1-A3";
        break;
    case 3:
        message += "A2-A3";
        break;
    case 4:
        message += "A0-GD";
        break;
    case 5:
        message += "A1-GD";
        break;
    case 6:
        message += "A2-GD";
        break;
    case 7:
        message += "A3-GD";
        break;
    }
    message += " ";
    switch (configRegister.bits.pga)
    {
    case 0:
        message += "6.144";
        break;
    case 1:
        message += "4.096";
        break;
    case 2:
        message += "2.048";
        break;
    case 3:
        message += "1.024";
        break;
    case 4:
        message += "0.512";
        break;
    case 5:
        message += "0.256";
        break;
    case 6:
        message += "0.256";
        break;
    case 7:
        message += "0.256";
        break;
    }
    message += " ";
    switch (configRegister.bits.operatingMode)
    {
    case 0:
        message += "CONT.";
        break;
    case 1:
        message += "SSHOT";
        break;
    }
    message += " ";
    switch (configRegister.bits.rate)
    {
    case 0:
        message += "128SPS";
        break;
    case 1:
        message += "250SPS";
        break;
    case 2:
        message += "490SPS";
        break;
    case 3:
        message += "920SPS";
        break;
    case 4:
        message += "1600SP";
        break;
    case 5:
        message += "2400SP";
        break;
    case 6:
        message += "3300SP";
        break;
    }
    message += " ";
    switch (configRegister.bits.sensorMode)
    {
    case 0:
        message += "ADC_M";
        break;
    case 1:
        message += "TMP_M";
        break;
    }
    message += " ";
    switch (configRegister.bits.pullUp)
    {
    case 0:
        message += "DISAB";
        break;
    case 1:
        message += "ENABL";
        break;
    }
    message += " ";
    switch (configRegister.bits.noOperation)
    {
    case 0:
        message += "INVAL";
        break;
    case 1:
        message += "VALID";
        break;
    case 2:
        message += "INVAL";
        break;
    case 3:
        message += "INVAL";
        break;
    }
    message += " ";
    switch (configRegister.bits.reserved)
    {
    case 0:
        message += "RSRV0";
        break;
    case 1:
        message += "RSRV1";
        break;
    }
    Serial.println("\nSTART MXSEL PGASL MODES RATES  ADTMP PLLUP NOOPE RESER");
    Serial.println(message);
} */