/*
 * Usage of CDK Matrix
 *
 * CS3377.502 Program6
 * Shurong Tian
 * sxt151030
 * sxt151030@utdallas.edu
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdint.h>
#include "cdk.h"

#define MATRIX_WIDTH 5
#define MATRIX_HEIGHT 3
#define BOX_WIDTH 20
#define MATRIX_NAME_STRING "Binary File Contents"


using namespace std;

//include the binary files 
class BinaryFileHeader{

public:
  uint32_t magicNumber; /* Should be 0xFEEDFACE */
  uint32_t versionNumber;
  uint64_t numRecords;
};


const int maxRecordStringLength = 25;
class BinaryFileRecord{

public:
  uint8_t strLength;
  char stringBuffer[maxRecordStringLength];
};


int main(){

  WINDOW        *window;
  CDKSCREEN     *cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix


  const char            *rowTitles[] = {"R0", "a", "b", "c", "d", "e"};
  const char            *columnTitles[] = {"C0", "a", "b", "c"};
  int           boxWidths[] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int           boxTypes[] = {vMIXED, vMIXED, vMIXED, vMIXED,  vMIXED,  vMIXED};



  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
    window = initscr();
    cdkscreen = initCDKScreen(window);

  /* Start CDK Colors */
  initCDKColor();


  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
   */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT,
                          MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
			  boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  /* Display the Matrix */
  drawCDKMatrix(myMatrix, true);


  //Start Binary File I/O process from here   
  BinaryFileHeader *temp1 = new BinaryFileHeader();
  ifstream inputFile("cs3377.bin",ios::in | ios::binary);
 
  //check if the file is opened
  if(!inputFile.is_open()){
    cout << "Error! Cannot open the file. " << endl;
    exit(-1);
  }

  inputFile.read((char *)temp1, sizeof(BinaryFileHeader));

  //parsing the values from the binary file using temp pointer
  string magic;
  stringstream ss1;
  ss1 << hex << uppercase << temp1->magicNumber;
  magic = "Magic: 0x" + ss1.str();

  string version;
  stringstream ss2;
  ss2 << temp1->versionNumber;
  version = "Version: " + ss2.str();

  string numRecord;
  stringstream ss3;
  ss3 << temp1->numRecords;
  numRecord = "NumRecords: " + ss3.str();

  //set them into the first row of the matrix 
  setCDKMatrixCell(myMatrix, 1, 1, magic.c_str());
  setCDKMatrixCell(myMatrix, 1, 2, version.c_str());
  setCDKMatrixCell(myMatrix, 1, 3, numRecord.c_str());


  //starting with row 2 for BinaryFileRecord
  int row=2;
  BinaryFileRecord *temp2 = new BinaryFileRecord();
  
  //while loop to parse inserting the records one row each time
  while(inputFile.read((char *)temp2, sizeof(BinaryFileRecord)) && row < 6){
    string strlen, text;
    stringstream ss1, ss2;
    ss1 << "strlen: " << static_cast<uint16_t> (temp2->strLength);
    strlen = ss1.str();
    ss2 << temp2->stringBuffer;
    text = ss2.str();

    //only setting values into the first and second columns
    setCDKMatrixCell(myMatrix, row, 1, strlen.c_str());
    setCDKMatrixCell(myMatrix, row, 2, text.c_str());

    //go to the next row
    row++;
  }

  drawCDKMatrix(myMatrix, true);

  // So we can see results, pause until a key is pressed. 
  unsigned char x;
  cin >> x;

  // Cleanup screen
  endCDK();
  return 0;
}
