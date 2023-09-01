#include "robot_tuberias_sd.h"

void SD_begin( SPIClass *spi, int SS)
{
  if (!SD.begin(SS, *spi)) {
      Serial << "SD.begin() failed";
      while (1);
    }
}

void initVSPI_SD( SPIClass *vspi){
  //capturamos si hay un error al inicializar el VSPI
  
  try {
    vspi->setHwCs(false);
    vspi->begin(VSPI_SCLK, VSPI_MISO, VSPI_MOSI, VSPI_SS);
    SD.begin(VSPI_SS, *vspi);
  } catch (...) {
    Serial << "VSPI.begin() failed";
  }
  pinMode(vspi->pinSS(), OUTPUT);
}


void initVSPI_SD( SPIClass *vspi , Another_SPI vspi_sd)
{
  //capturamos si hay un error al inicializar el VSPI
  try {
    vspi->setHwCs(false);
    vspi->begin(vspi_sd.SCLK, vspi_sd.MISO, vspi_sd.MOSI, vspi_sd.SS);
    SD.begin(vspi_sd.SS, *vspi);
  } catch (...) {
    Serial << "VSPI.begin() failed";
  }
  pinMode(vspi_sd.SS, OUTPUT);
}

void endVSPI_SD( SPIClass *vspi )
{
  //capturamos si hay un error al inicializar el VSPI
  try {
    vspi->end();
  } catch (...) {
    Serial << "VSPI.end() failed";
  }
}

void initHSPI_SD( SPIClass *hspi )
{
  try {
    // capturamos si hay un error al inicializar el HSPI
    hspi->setHwCs(false);
    hspi->begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_SS);
    SD.begin(HSPI_SS, *hspi);

  } catch (...) {
    Serial << "HSPI.begin() failed";
  }
  pinMode(hspi->pinSS(), OUTPUT);
}

// inicializamos el HSPI
void initHSPI_SD( SPIClass *hspi , Another_SPI hspi_sd)
{
  // capturamos si hay un error al inicializar el HSPI
  
  try {
    hspi->setHwCs(false);
    hspi->begin(hspi_sd.SCLK, hspi_sd.MISO, hspi_sd.MOSI, hspi_sd.SS);
    SD.begin(hspi_sd.SS, *hspi);
  } catch (...) {
    Serial << "HSPI.begin() failed";
  }
  pinMode(hspi_sd.SS, OUTPUT);
}

// finalizamos el HSPI
void endHSPI_SD( SPIClass *hspi )
{
  // capturamos si hay un error al inicializar el HSPI
  try {
    hspi->end();
  } catch (...) {
    Serial << "HSPI.end() failed";
  }
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void createDir(fs::FS &fs, const char * path){
    Serial.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path)){
        Serial.println("Dir created");
    } else {
        Serial.println("mkdir failed");
    }
}

void removeDir(fs::FS &fs, const char * path){
    Serial.printf("Removing Dir: %s\n", path);
    if(fs.rmdir(path)){
        Serial.println("Dir removed");
    } else {
        Serial.println("rmdir failed");
    }
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("File renamed");
    } else {
        Serial.println("Rename failed");
    }
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
}

void testFileIO(fs::FS &fs, const char * path){
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if(file){
        len = file.size();
        size_t flen = len;
        start = millis();
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        Serial.printf("%u bytes read for %u ms\n", flen, end);
        file.close();
    } else {
        Serial.println("Failed to open file for reading");
    }


    file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }

    size_t i;
    start = millis();
    for(i=0; i<2048; i++){
        file.write(buf, 512);
    }
    end = millis() - start;
    Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
    file.close();
}




// escribimos en el archivo CSV
void writeCSV( File file, std::stringstream& ss)
{
  file.print(ss.str().c_str()); // guardamos la informaciÃ³n en el archivo
  file.flush();                 // aseguramos que se guarde la informaciÃ³n en la SD
  file.close();                 // cerramos el archivo
  Serial << ss.str().c_str();         // imprimimos en el monitor serial la informaciÃ³n guardada
  ss.str("");
  ss.clear();
}


/*

void cargardatos_SD(fs::FS &fs,char *buffer,double hydrophone,double accelX,double accelY,double accelZ,double gyroX,double gyroY,double gyroZ){
  // nombre del archivo "/" + buffer + ".csv"
  char s[20] = "/";
  strcat(s,buffer);
  strcat(s,".csv");
  //abrir archivo

  if (!filecreated){
    writeFile(fs, s, "Hora,Hydrophone,AccelX,AccelY,AccelZ,GyroX,GyroY,GyroZ\n");
    filecreated = true;
  }else{
    appendFile(fs, s, "123202,12,12,12,12,12,12,12\n");
  }
}

void check_card_type(Sd2Card card) {
  Serial << "\nCard type: " ;
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial << "SD1\n";
      break;
    case SD_CARD_TYPE_SD2:
      Serial << "SD2\n";
      break;
    case SD_CARD_TYPE_SDHC:
      Serial << "SDHC\n";
      break;
    default:
      Serial << "Unknown\n";
  }
}

void check_card_connection(Sd2Card card) {
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial << "initialization failed. Things to check:\n* is a card inserted?\n* is your wiring correct?\n* did you change the chipSelect pin to match your shield or module?\n";
    while (1);
  } else {
    Serial << "Wiring is correct and a card is present.\n";
  }
}

void check_card_size(SdVolume volume) {
    Serial << "\nClusters:          " << volume.clusterCount();
    Serial << "\nBlocks x Cluster:  " << volume.blocksPerCluster();
    Serial << "\nTotal Blocks:      " << volume.blocksPerCluster() * volume.clusterCount();
  uint32_t volumesize;
  Serial << "\nVolume type is FAT" << int(volume.fatType());
  Serial << "\nVolume size (Kb): ";
  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize /= 2;                           // SD card blocks are always 512 bytes (2 blocks are 1KB)
  Serial << volumesize << "\nVolume size (Mb): " << volumesize / 1024;
  Serial << "\nVolume size (Gb): " << float(volumesize) / 1048576.0;
}

void check_root_folder(SdFile root, SdVolume volume) {
  Serial << "\nFiles found in the root:\n";
  root.openRoot(volume);
    // list all files in the card with date and size
    root.ls(LS_R | LS_DATE | LS_SIZE);
    root.close();
    Serial << "\n";
}

void check_filesystem(SdVolume volume) {
  Serial << "\nChecking filesystem...";
  if (!volume.init(card)) {
    Serial << "failed!\n";
    return;
  }
  Serial << "OK!\n";
}

void info_SD() {
    check_card_connection(card);
    check_card_type(card);
    check_filesystem(volume);
    check_card_size(volume);
    check_root_folder(root, volume);
}


*/