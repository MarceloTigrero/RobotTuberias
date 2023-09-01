#include "robot_tuberias_raiz.h"
#include <SD.h>
#include <SPI.h>
#include <sstream>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

/*
 * Connect the SD card to the following pins:
 *
 * SD Card | ESP32
 *    D2       -
 *    D3       SS
 *    CMD      MOSI
 *    VSS      GND
 *    VDD      3.3V
 *    CLK      SCK
 *    VSS      GND
 *    D0       MISO
 *    D1       -
 */


/*
El ESP32 tiene cuatro periféricos SPI: SPI0, SPI1, SPI2 (comúnmente conocido como HSPI) y 
SPI3 (comúnmente conocido como VSPI)1. Los periféricos SPI0 y SPI1 se utilizan internamente 
para comunicarse con la memoria flash incorporada y no deben utilizarse para otras tareas1. 
Puedes utilizar HSPI y VSPI para comunicarte con otros dispositivos1.

SPI es un protocolo de comunicación serie síncrono utilizado por los microcontroladores para comunicarse con uno 
o más periféricos1. En una comunicación SPI, siempre hay un controlador (también llamado maestro) que controla los 
dispositivos periféricos (también llamados esclavos)1. Los datos pueden enviarse y recibirse simultáneamente, lo qu
e significa que el maestro puede enviar datos a un esclavo y un esclavo puede enviar datos al maestro al mismo tiempo1. 
Puedes tener solo un maestro, que será un microcontrolador (el ESP32), pero puedes tener varios esclavos1. Un esclavo puede 
ser un sensor, una pantalla, una tarjeta microSD, etc., o otro microcontrolador1.

Para la comunicación SPI necesitas cuatro líneas: MISO (Master In Slave Out), MOSI (Master Out Slave In), SCK (Serial Clock) y CS/SS (Chip Select)1. En un dispositivo solo esclavo, como sensores, pantallas y otros, puedes encontrar una terminología diferente: MISO puede estar etiquetado como SDO (Serial Data Out) y MOSI puede estar etiquetado como SDI (Serial Data In)1
SPI	        MOSI	MISO	SCLK	CS
SPI0 (FLASH)	GPIO 13	GPIO 12	GPIO 14	GPIO 15
SPI1 (FLASH)	GPIO 23	GPIO 19	GPIO 18	GPIO 5
VSPI	GPIO 23	GPIO 19	GPIO 18	GPIO 5
HSPI	GPIO 13	GPIO 12	GPIO 14	GPIO 15

*/
  #define VSPI_MISO   MISO
  #define VSPI_MOSI   MOSI
  #define VSPI_SCLK   SCK
  #define VSPI_SS     SS

  #define HSPI_MISO   12
  #define HSPI_MOSI   13
  #define HSPI_SCLK   14
  #define HSPI_SS     15




static const int spiClk = 1000000; // 1 MHz

//uninitalised pointers to SPI objects
//SPIClass * vspi = new SPIClass(VSPI);
//SPIClass *hspi = new SPIClass(HSPI);

 
struct Another_SPI
{
  int SCLK;
  int MISO;
  int MOSI;
  int SS;
};

void SD_begin( SPIClass *spi, int SS);

void initVSPI_SD( SPIClass *vspi);
void initVSPI_SD( SPIClass *vspi , Another_SPI vspi_sd);
void endVSPI_SD( SPIClass *vspi );

void initHSPI_SD( SPIClass *hspi);
void initHSPI_SD( SPIClass *hspi , Another_SPI hspi_sd);
void endHSPI_SD( SPIClass *hspi );
void writeCSV( File file, std::stringstream& ss);

void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
void createDir(fs::FS &fs, const char * path);
void removeDir(fs::FS &fs, const char * path);
void readFile(fs::FS &fs, const char * path);
void writeFile(fs::FS &fs, const char * path, const char * message);
void appendFile(fs::FS &fs, const char * path, const char * message);
void renameFile(fs::FS &fs, const char * path1, const char * path2);
void deleteFile(fs::FS &fs, const char * path);
void testFileIO(fs::FS &fs, const char * path);

//void cargardatos_SD(fs::FS &fs, char *buffer,double hydrophone,double accelX,double accelY,double accelZ,double gyroX,double gyroY,double gyroZ);

/*
//card information
Sd2Card card;
SdVolume volume;
SdFile root;
const int chipSelect = 10;
uint32_t volumesize;

void check_card_type(Sd2Card card);
void check_card_connection(Sd2Card card);
void check_card_size(SdVolume volume);
void check_root_folder(SdFile root, SdVolume volume);
void check_filesystem(SdVolume volume);
void info_SD();
*/




  



