// LuFiSeR.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include "md5.h"

using namespace std;

#include <string>


// prototype of encryption algorithm
unsigned char *Crypt(unsigned char *ucpData, int nLength, unsigned int uintInitVal_01);
// prototype of KDB file analyzer algorithm
bool analyzeKDB(string strFilePath_02, unsigned int uintInitVal_02);
// prototype of JPG extractor algorithm
bool analyzeJPG(string strInputFilePath_03);

////////////////////////////////////////////////////////////
//
//  inline uint32_t pack32(): packs char/bytes into 
//                               unsigned 32-bit integer;
//
////////////////////////////////////////////////////////////
//
//  uint8_t byt1: unsigned integer or character; byte 1;
//  uint8_t byt2: unsigned integer or character; byte 2;
//  uint8_t byt3: unsigned integer or character; byte 3;
//  uint8_t byt4: unsigned integer or character; byte 4;
//  
////////////////////////////////////////////////////////////
//
//  Returns: packed 32-bit integer
//
////////////////////////////////////////////////////////////

inline uint32_t pack32(uint8_t byt1, uint8_t byt2, uint8_t byt3, uint8_t byt4)
{
	return (byt1 << 24) | (byt2 << 16) | (byt3 << 8) | byt4;
}

////////////////////////////////////////////////////////////
//
//  inline uint16_t pack16(): packs char/bytes into 
//                               unsigned 16-bit integer;
//
////////////////////////////////////////////////////////////
//
//  uint8_t byt1: unsigned integer or character; byte 1;
//  uint8_t byt2: unsigned integer or character; byte 2;
//  
////////////////////////////////////////////////////////////
//
//  Returns: packed 16-bit integer
//
////////////////////////////////////////////////////////////

inline uint16_t pack16(uint8_t byt1, uint8_t byt2)
{
	return (byt1 << 8) | byt2;
}

////////////////////////////////////////////////////////////
//
//  main(): collects user input, calls challenge functions;
//             produces challenge output;
//
////////////////////////////////////////////////////////////
//
//  argc: int; argument count in argv
//  argv: char*; argument array
//
//  Command line example:
//
//  LuFiSeR.exe "_in_01_:apple" 
//              "_iv_01_:305419896" 
//              "_in_02_:W:\OneDriveStorage\OneDrive\Documents\Personal\challenge\magic.kdb" 
//              "_iv_02_:1331118420"
//              "_in_03_:W:\OneDriveStorage\OneDrive\Documents\Personal\challenge\store.kdb" 
//              "_bn_03_:W:\OneDriveStorage\OneDrive\Documents\Personal\challenge\input.bin" 
//              "_iv_03_:1331118420"
//
////////////////////////////////////////////////////////////
//
//  Returns: nothing; excepts on error;
//
////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
	try
	{
		// initialization
		string strData = "";
		string strInitVal_01 = "";
		string strFilePath_02 = "";
		string strInitVal_02 = "";
		string strFilePath_03 = "";
		string strInputFilePath_03 = "";
		string strInitVal_03 = "";
		unsigned char* ucpData = NULL;
		unsigned char* ucpResult = NULL;
		unsigned int uintInitVal_01 = 0x12345678;
		unsigned int uintInitVal_02 = 0x4F574154;
		unsigned int uintInitVal_03 = 0x4F574154;

		// prompt command line usage for next execution
		if (argc <= 0)
		{
			cout << "////////////////////////////////////////////////////////////";
			cout << "//";
			cout << "// NOTE: LuFiSeR accepts key-value command line arguments:";
			cout << "//       _in_01_:";
			cout << "//       _iv_01_:";
			cout << "//       _in_02_:";
			cout << "//       _iv_02_:";
			cout << "//       _in_03_:";
			cout << "//       _bn_03_:";
			cout << "//       _iv_03_:";
			cout << "// E.g.:";
			cout << "//       LuFiSeR.exe \"_in_01_:Apple\" ";
			cout << "//                   \"_iv_01_:0x12345678\"";
			cout << "//       LuFiSeR.exe \"_in_02_:KDB_File_Path\" ";
			cout << "//                   \"_iv_02_:0x4F574154\"";
			cout << "//       LuFiSeR.exe \"_in_03_:KDB_File_Path\" ";
			cout << "//                   \"_bn_03_:Input_File_Path\"";
			cout << "//                   \"_iv_02_:0x4F574154\"";
			cout << "//\n//";
			cout << "// Where:";
			cout << "//       key _in_01_: data to be encrypted; challenge 1";
			cout << "//       key _iv_01_: initial encryption value; challenge 1";
			cout << "//       key _in_02_: KDB filepath to analyze; challenge 2";
			cout << "//       key _iv_02_: initial encryption value; challenge 2";
			cout << "//       key _in_03_: KDB filepath to analyze; challenge 3";
			cout << "//       key _bn_03_: input filepath to analyze; challenge 3";
			cout << "//";
			cout << "////////////////////////////////////////////////////////////";
			cout << "";
		}
		// recover command line inputs
		else
		{
			// cycle arguments
			for (int nCnt = 0; nCnt < argc; nCnt++)
			{
				// get this argument
				string strGet(argv[nCnt]);

				////////////////////
				//
				// Challenge 1 input
				//
				// data to be encrypted, challenge 1
				if (strGet.substr(0, 8) == "_in_01_:")
					strData = strGet.substr(8, strGet.length() - 8);
				// initial encryption value, challenge 1
				else if (strGet.substr(0, 8) == "_iv_01_:")
					strInitVal_01 = strGet.substr(8, strGet.length() - 8);

				////////////////////
				//
				// Challenge 2 input
				//
				// kdb file path, challenge 2
				else if (strGet.substr(0, 8) == "_in_02_:")
					strFilePath_02 = strGet.substr(8, strGet.length() - 8);
				// initial encryption value, challenge 2
				else if (strGet.substr(0, 8) == "_iv_02_:")
					strInitVal_02 = strGet.substr(8, strGet.length() - 8);

				////////////////////
				//
				// Challenge 3 input
				//
				// kdb file path, challenge 2
				else if (strGet.substr(0, 8) == "_in_03_:")
					strFilePath_03 = strGet.substr(8, strGet.length() - 8);
				// input file path, challenge 2
				else if (strGet.substr(0, 8) == "_bn_03_:")
					strInputFilePath_03 = strGet.substr(8, strGet.length() - 8);
				// initial encryption value, challenge 2
				else if (strGet.substr(0, 8) == "_iv_03_:")
					strInitVal_03 = strGet.substr(8, strGet.length() - 8);
			}
		}

		// greeting
		cout << "\n==============================\n=\n= LuFiSeR awaits...\n=\n==============================";

		////////////////////
		//
		// Challenge 1 input
		//
		// query user for encryption data, if missing from command line
		if (strData == "")
		{
			cout << "\nEnter data to be encrypted : ";
			getline(cin, strData);
		}
		// query user for einitial encryption value, if missing from command line
		if (strInitVal_01 == "")
		{
			cout << "\nEnter encryption intial value as integer (e.g., 305419896 [0x12345678]): ";
			getline(cin, strInitVal_01);
		}

		// challenge 1, encrypt with data and initial value
		if ((strData != "") && (strInitVal_01 != ""))
		{
			cout << "\n\n##############################\n# Challenge 1 Performance\n##############################\n";

			// instantiate, initialize
			ucpData = new unsigned char[strData.length()];
			uintInitVal_01 = (unsigned int)stoul(strInitVal_01);//0x12345678;//  

			// copy string data to buffer, set null terminator
			copy(strData.begin(), strData.end(), ucpData);
			ucpData[strData.length()] = 0;

			// update and encrypt data
			if (ucpData != NULL)
			{
				// encrypt
				ucpResult = Crypt(ucpData, strData.length(), uintInitVal_01);

				if (ucpResult != NULL)
				{
					// update
					cout << "\nEncryption result: [" << ucpResult << "]:";
					// display byte values
					for (int nCnt = 0; nCnt < (int)strData.length(); nCnt++)
						printf("%02hhx ", ucpData[nCnt]);

					// dencrypt
					ucpResult = Crypt(ucpData, strData.length(), uintInitVal_01);

					// update
					cout << "\nDecryption result: [" << ucpResult << "]:";
					// display byte values
					for (int nCnt = 0; nCnt < (int)strData.length(); nCnt++)
						printf("%02hhx ", ucpData[nCnt]);

					// update
					cout << "\n\nSUCCESS: challenge 1 analysis complete.\n";
				}
				else
					cout << "\nERROR: data initialization failed.";
			}
			// data error
			else
				cout << "\nERROR: data initialization failed.";
		}
		// error conditions, terminate without encrypting
		else if (strData == "")
			cout << "\nERROR: Data to be encrypted is missing.";
		else
			cout << "\nERROR: Initial value is missing.";
		
		////////////////////
		//
		// Challenge 2 input
		//
		// query user for kdb filepath, if missing from command line
		if (strFilePath_02 == "")
		{
			cout << "\nEnter KDB filepath (challenge 2): ";
			getline(cin, strFilePath_02);
		}
		// query user for initial encryption value, if missing from command line
		if (strInitVal_02 == "")
		{
			cout << "\nEnter (challenge 2) encryption intial value as integer (e.g., 1331118420 [0x4F574154]): ";
			getline(cin, strInitVal_02);
		}

		// challenge 2, analyze kdb file
		if ((strFilePath_02 != "") && (strInitVal_02 != ""))
		{
			cout << "\n\n##############################\n# Challenge 2 Performance\n##############################\n";

			if (analyzeKDB(strFilePath_02, uintInitVal_02) == true)
				cout << "\nSUCCESS: challenge 2 KDB file analysis complete.\n";
			else
				cout << "\nERROR: challenge 2 KDB file analysis failed.\n";
		}
		// error conditions, terminate without analyzing
		else if (strFilePath_02 == "")
			cout << "\nERROR: challenge 2 filepath is missing.";
		else
			cout << "\nERROR: challenge 2 initial value is null.";
		
		////////////////////
		//
		// Challenge 3 input
		//
		// query user for kdb filepath, if missing from command line
		if (strFilePath_03 == "")
		{
			cout << "\nEnter KDB filepath (challenge 3) : ";
			getline(cin, strFilePath_03);
		}
		if (strInputFilePath_03 == "")
		{
			cout << "\nEnter input filepath (challenge 3) : ";
			getline(cin, strInputFilePath_03);
		}
		// query user for initial encryption value, if missing from command line
		if (strInitVal_03 == "")
		{
			cout << "\nEnter (challenge 3) encryption intial value as integer (e.g., 1331118420 [0x4F574154]): ";
			getline(cin, strInitVal_03);
		}

		// challenge 3, analyze kdb file
		if ((strFilePath_03 != "") && (strInputFilePath_03 != "") && (strInitVal_03 != ""))
		{
			cout << "\n\n##############################\n# Challenge 3 Performance\n##############################\n";

			if (analyzeKDB(strFilePath_03, uintInitVal_03) == true)
				cout << "\n\nSUCCESS: challenge 3 KDB file analysis complete.\n";
			else
				cout << "\n\nERROR: challenge 3 KDB file analysis failed.\n";

			if (analyzeJPG(strInputFilePath_03) == true)
				cout << "\n\nSUCCESS: challenge 3 JPG file analysis complete.\n";
			else
				cout << "\n\nERROR: challenge 3 JPG file analysis failed.\n";
		}
		// error conditions, terminate without analyzing
		else if (strFilePath_03 == "")
			cout << "\nERROR: chalenge3 KDB filepath is missing.";
		else if (strInputFilePath_03 == "")
			cout << "\nERROR: challenge 3 input filepath is missing.";
		else
			cout << "\nERROR: challenge 3 initial value is null.";
	}
	catch (exception* e)
	{
		printf("\nERROR: %s\n", e->what());
	}

	cout << "\n##############################\n# END\n##############################\n\n";

}

////////////////////////////////////////////////////////////
//
//  Crypt(): encrypts data using the indicated initial 
//             value; returns encryption;
//
////////////////////////////////////////////////////////////
//
//  ucpData:     unsigend char*; data to encrypt;
//  nLength:     int; length of data to encrypt;
//  uintInitVal_01: unsigned int; initial encryption value;
//
////////////////////////////////////////////////////////////
//
//  Returns: encrypted string as unsigned char*, if 
//              successful; else, NULL;
//              excepts on error;
//
////////////////////////////////////////////////////////////

unsigned char* Crypt(unsigned char *ucpData, int nLength, unsigned int uintInitVal_01)
{
	// instantiation, initialization
	// least bit
	unsigned int uintResult = NULL;
	// lfs register
	unsigned int ucpLFSR = uintInitVal_01;
	// taps: 1, 6, 9, 10, 15
	unsigned int ucpFeed = 0x87654321;

	try
	{
		// update
		cout << "\nCrypting [" << ucpData << "] using initial value [" << uintInitVal_01 << "]..."; //" : " << 0x12345678 << "]...";

		// display byte values
		cout << "\n...RESULT: ";
		for (int nCnt = 0; nCnt < (int)nLength ; nCnt++)
			printf("%02hhx ", ucpData[nCnt]);

		// validation
		if ((ucpData != NULL) && (nLength > 0) && (uintInitVal_01 > 0))
		{
			// iterate data content
			for (int nCnt = 0; nCnt < nLength; nCnt++)
			{
				// lfsr, stepped by 8s
				for (int nStep = 0; nStep < 8; nStep++)
				{
					// get least bit
					uintResult = ucpLFSR & 1;
					// shift register right 1
					ucpLFSR >>= 1;
					// mask on 1
					if (uintResult > 0)
						ucpLFSR ^= ucpFeed;
				}

				// XOR data content
				ucpData[nCnt] ^= ucpLFSR & 0x00FF;
			}
		}
		// error conditions, terminate without analyzing
		else if (ucpData == NULL)
			cout << "\nERROR: target data is null.";
		else if (nLength <= 0)
			cout << "\nERROR: target data length is <= 0.";
		else
			cout << "\nERROR: initial value is null.";
	}
	catch (exception* e)
	{
		printf("\nERROR: %s\n", e->what());
	}

	return ucpData;
}

////////////////////////////////////////////////////////////
//
//  analyzeKDB(): analyzes KDB file, decrypting contents;
//
////////////////////////////////////////////////////////////
//
//  strFilePath_02:    string; KDB file path to analyze;
//  uintInitVal_02: unsigned int; initial encryption value;
//
////////////////////////////////////////////////////////////
//
//  Returns: true, if successful, else false; 
//              excepts on error;
//
////////////////////////////////////////////////////////////

bool analyzeKDB(string strFilePath_02, unsigned int uintInitVal_02)
{
	// instatiation, initialization
	// return flag, default to fail (false)
	bool bReturn = false;
	// KDB file handle
	ifstream* ispKDB = NULL;
	// KDB file length
	unsigned long ulFileLength = 0;
	// KDB file buffer
	char* chpKDB = NULL;
	// magic
	unsigned char uchrMagic[7];
	// entries offset
	uint32_t ulEntriesOffset = 0;
	// concatenated output
	stringstream ssCat;
	string strConcat = "";

	try
	{
		// validation
		if ((strFilePath_02 != "") && (uintInitVal_02 > 0))
		{
			// update
			cout << "\nDecrypting [" << strFilePath_02 << "]\n    using initial value [" << uintInitVal_02 << "]...";

			// open and read file
			ispKDB = new ifstream(strFilePath_02, ifstream::binary);

			// validation
			if (ispKDB != NULL) 
			{
				// file length
				ispKDB->seekg(0, ispKDB->end);
				ulFileLength = (long)ispKDB->tellg();
				ispKDB->seekg(0, ispKDB->beg);

				// validation
				if (ulFileLength > 0)
				{
					// instantiation, initialization
					chpKDB = new char[ulFileLength];

					// validation
					if (chpKDB != NULL)
					{
						// update
						cout << "\nReading [" << ulFileLength << "] bytes...";

						// read file
						ispKDB->read(chpKDB, ulFileLength);

						// update
						cout << "\nAnalyzing...";

						// simple, stupid but clear buffer extraction
						////////////////////////////////////////
						//
						// get magic
						//
						uchrMagic[0] = chpKDB[0];
						uchrMagic[1] = chpKDB[1];
						uchrMagic[2] = chpKDB[2];
						uchrMagic[3] = chpKDB[3];
						uchrMagic[4] = chpKDB[4];
						uchrMagic[5] = chpKDB[5];
						// null te6rminator
						uchrMagic[6] = 0x00u;

						////////////////////////////////////////
						//
						// get entries offset, little endian
						//
						ulEntriesOffset = pack32(chpKDB[9], chpKDB[8], chpKDB[7], chpKDB[6]);

						////////////////////////////////////////
						//
						// report magic
						cout << "\n...Magic: " << uchrMagic;
						//
						////////////////////////////////////////
						//
						// report entries offset
						cout << "\n...Entries Offset: " << ulEntriesOffset << "";

						////////////////////////////////////////
						//
						// process entries
						//
						// entry-list-level counter
						unsigned long ulEntryLongCnt = 0;
						// entry-level counter
						int nEntryShortCnt = 0;
						// entry name counter
						int nEntryNameCnt = 1;
						// entry name
						unsigned char uchrEntryName[16];
						// block offset
						uint32_t ulBlockOffset = 0;
						// read entries
						do
						{
							// length of entry name
							if (nEntryShortCnt == 16)
							{
								// get block offset, little endian
								ulBlockOffset = pack32(chpKDB[ulEntriesOffset + ulEntryLongCnt + 3], chpKDB[ulEntriesOffset + ulEntryLongCnt + 2], chpKDB[ulEntriesOffset + ulEntryLongCnt + 1], chpKDB[ulEntriesOffset + ulEntryLongCnt + 0]);

								// report name
								cout << "\n...Block [" << nEntryNameCnt << "] Name: [" << uchrEntryName << "]";
								// report offset
								cout << "\n...Block [" << nEntryNameCnt << "] Offset: [" << ulBlockOffset << "]";
								
								////////////////////////////////////////
								//
								// process blocks
								//
								// block-list-level counter
								unsigned long ulBlockLongCnt = 0;
								// block offset
								uint16_t uintDataLength = 0;
								// block offset
								uint32_t ulDataOffset = 0;
								// read entries
								do
								{
									// get data length, little endian
									uintDataLength = pack16(chpKDB[ulBlockOffset + ulBlockLongCnt + 1], chpKDB[ulBlockOffset + ulBlockLongCnt + 0]);
									// get data offset, little endian
									ulDataOffset = pack32(chpKDB[ulBlockOffset + ulBlockLongCnt + 5], chpKDB[ulBlockOffset + ulBlockLongCnt + 4], chpKDB[ulBlockOffset + ulBlockLongCnt + 3], chpKDB[ulBlockOffset + ulBlockLongCnt + 2]);

									// report data length
									cout << "\n...Block [" << nEntryNameCnt << "] Data length [" << uintDataLength << "] ";
									// report data offset
									cout << "\n...Block [" << nEntryNameCnt << "] Data offset [" << ulDataOffset << "]";

									////////////////////////////////////////
									//
									// process data
									//
									// data buffer
									unsigned char* ucpData = NULL;
									ucpData = new unsigned char[uintDataLength+1];
									// buffer counter
									int nBufferCnt = 0;
									if (ucpData != NULL)
									{
										// read data
										for (unsigned long ulDataIndex = ulDataOffset; ulDataIndex < ulDataOffset + uintDataLength; ulDataIndex++)
										{
											ucpData[nBufferCnt] = chpKDB[ulDataIndex];
											nBufferCnt++;
										}
										// null terminator
										ucpData[uintDataLength] = 0x00u;

										// report encrypted data
										// display byte values	
										cout << "\n\n...Block [" << nEntryNameCnt << "] Decrypted Data: [" << ucpData << "] [";
										//for (int nCnt = 0; nCnt < (int)uintDataLength; nCnt++)
										//	printf("%02hhx ", ucpData[nCnt]);
										cout << "]";

										// crypt
										ucpData = Crypt(ucpData, uintDataLength, uintInitVal_02);

										// concatenate
										ssCat << ucpData << " ";

										// report encrypted data
										// display byte values	
										cout << "\n\n...Block [" << nEntryNameCnt << "] Decrypted Data: [" << ucpData << "] [";
										//for (int nCnt = 0; nCnt < (int)uintDataLength ; nCnt++)
										//	printf("%02hhx ", ucpData[nCnt]);
										cout << "]";
									}
									// error condition
									else
										cout << "\nERROR: block [" << nEntryNameCnt << "] data buffer is null.";

									// iterate
									ulBlockLongCnt += 6;
								} 
								// end of block list
								while (chpKDB[ulBlockOffset + ulBlockLongCnt] != 0xFFFFFFFF);

								// reset counters
								nEntryNameCnt++;
								nEntryShortCnt = 0;
								ulEntryLongCnt += 4;
							}
							// collecting entry name
							else
							{
								uchrEntryName[nEntryShortCnt] = chpKDB[ulEntriesOffset + ulEntryLongCnt];
							}

							// iterate
							nEntryShortCnt++;
							ulEntryLongCnt++;
						}
						// end of entry list
						while (chpKDB[ulEntriesOffset + ulEntryLongCnt] != 0xFFFFFFFF);

						// report concatenated output
						strConcat = ssCat.str();
						cout << "\n\nENCRYPTED PAYLOAD:\n\n[" << strConcat << "]\n\nEND\n";

						// close and clear
						delete[] chpKDB;

						// set return flag, success
						bReturn = true;
					}
					// error condition
					else
						cout << "\nERROR: KDB file buffer is null.";
				}
				// error condition
				else
					cout << "\nERROR: KDB file length is zero.";

				// close and clear
				ispKDB->close();
				delete ispKDB;
			}
			// error condition
			else
				cout << "\nERROR: KDB file handle is null.";
		}
		// error conditions, terminate without analyzing
		else if (strFilePath_02 == "")
			cout << "\nERROR: target data is null.";
		else
			cout << "\nERROR: initial value is null.";
	}
	catch (exception* e)
	{
		printf("\nERROR: %s\n", e->what());
	}

	return bReturn;
}

////////////////////////////////////////////////////////////
//
//  analyzeJPG(): analyzes cloaked jpg file, 
//                   output contents to filename_offset.jpg;
//
////////////////////////////////////////////////////////////
//
//  strInputFilePath_02: string; input file path to analyze;
//
////////////////////////////////////////////////////////////
//
//  Returns: true, if successful, else false; 
//              excepts on error;
//
////////////////////////////////////////////////////////////

bool analyzeJPG(string strInputFilePath_03)
{
	// instatiation, initialization
	// return flag, default to fail (false)
	bool bReturn = false;
	// JPG file handle
	ifstream* ispJPG = NULL;
	// JPG file length
	unsigned long ulFileLength = 0;
	// JPG file buffer
	unsigned char* ucpJPG = NULL;

	try
	{
		// validation
		if (strInputFilePath_03 != "")
		{
			// update
			cout << "\nExtracting [" << strInputFilePath_03 << "]...";

			// open and read file
			ispJPG = new ifstream(strInputFilePath_03, ifstream::binary);

			// validation
			if (ispJPG != NULL)
			{
				// file length
				ispJPG->seekg(0, ispJPG->end);
				ulFileLength = (long)ispJPG->tellg();
				ispJPG->seekg(0, ispJPG->beg);

				// validation
				if (ulFileLength > 0)
				{
					// instantiation, initialization
					ucpJPG = new unsigned char[ulFileLength];

					// validation
					if (ucpJPG != NULL)
					{
						// update
						cout << "\nReading [" << ulFileLength << "] bytes...";

						// read file
						ispJPG->read((char*)ucpJPG, ulFileLength);

						// update
						cout << "\nAnalyzing...";

						////////////////////////////////////////
						//
						// process data
						//
						// extracted image buffer
						unsigned char* ucpImage = NULL;
						// image start offset
						unsigned long ulStartOffset = 0;
						// image end offset
						unsigned long ulEndOffset = 0;
						// modification type
						bool bTForAlteredFForStripped = true;
						// process file buffer
						for (unsigned long ulCnt = 0 ; ulCnt < ulFileLength ; ulCnt++)
						{
							// check for valid markers
							// 
							// 
							//
							// SOI	0xFF, 0xD8 //	none	Start Of Image
							if ((ucpJPG[ulCnt] == 0xFFu) && (ucpJPG[ulCnt + 1] == 0xD8u) && (ucpJPG[ulCnt + 2] == 0xFFu))
							{
								// update
								cout << "\nStart marker at [" << ulCnt << "]...";
							}

							// SOI	0xFF, 0xD8 //	none	Start Of Image
							if ((ucpJPG[ulCnt] == 0x4Du) && (ucpJPG[ulCnt + 1] == 0x42u) && (ucpJPG[ulCnt + 2] == 0x4Bu))
							{
								// update
								cout << "\nMagic marker at [" << ulCnt << "]...";

								// increment start offset
								ulStartOffset = ulCnt;
							}

							// check for end markers
							// 255, 217
							// 0xFF, 0xD9
							//
							if ((ulCnt > 0) && (ucpJPG[ulCnt - 1] == 0xFFu) && (ucpJPG[ulCnt] == 0xD9u))
							{
								// mark end
								ulEndOffset = ulCnt + 1;

								// update
								if (bTForAlteredFForStripped)
									cout << "\n\nExtracting altered JPG at [" << ulStartOffset << "] : [" << ulEndOffset << "]...";
								else
									cout << "\n\nExtracting stripped JPG at [" << ulStartOffset << "] : [" << ulEndOffset << "]...";

								// instantiate and copy
								// if start bytes have been altered, rather than stripped
								if(bTForAlteredFForStripped)
									ucpImage = new unsigned char[(ulEndOffset - ulStartOffset) + 0];
								else
									ucpImage = new unsigned char[(ulEndOffset - ulStartOffset) + 3];

								// validation
								if (ucpImage != NULL)
								{
									// if start bytes have been stripped, rather than altered
									if (!bTForAlteredFForStripped)
									{
										ucpImage[0] = 0xFFu;
										ucpImage[1] = 0xD8u;
										ucpImage[2] = 0xFFu;
									}

									// copy
									// image array counter
									unsigned long ulTargetCnt = 0;

									// if start bytes have been stripped, rather than altered
									if(!bTForAlteredFForStripped)
										ulTargetCnt = 3;

									//unsigned long ulTargetCnt = 0;
									for (unsigned long ulSourceCnt = ulStartOffset ; ulSourceCnt < ulEndOffset ; ulSourceCnt++)
									{
										// if start bytes have been altered, rather than stripped
										if (bTForAlteredFForStripped)
										{
											if (ulTargetCnt == 0)
												ucpImage[0] = 0xFFu;
											else if (ulTargetCnt == 1)
												ucpImage[1] = 0xD8u;
											else if (ulTargetCnt == 2)
												ucpImage[2] = 0xFFu;
											else
												ucpImage[ulTargetCnt] = ucpJPG[ulSourceCnt];
										}
										// if start bytes have been stripped, rather than altered
										else
											ucpImage[ulTargetCnt] = ucpJPG[ulSourceCnt];

										// increment
										ulTargetCnt++;
									}

									// format output file name
									string strOutFileName = strInputFilePath_03 + "_" + to_string(ulStartOffset) + ".jpg";

									// export extracted JPG file, open
									ofstream ofOut(strOutFileName);

									// validation
									if (ofOut)
									{
										// write and close
										if(bTForAlteredFForStripped)
											ofOut.write((char*)ucpImage, (ulEndOffset - ulStartOffset) + 0);
										else
											ofOut.write((char*)ucpImage, (ulEndOffset - ulStartOffset) + 3);
										ofOut.close();

										cout << "\n\n";

										// hash (copied from site, has its own messages embedded)
										MD5 md5;
										string strDigest((char*)ucpImage);
										puts(md5.digestString((char*)strDigest.c_str()));
										// print the digest for a binary file on disk.
										puts(md5.digestFile((char*)strOutFileName.c_str()));

										// update
										if (bTForAlteredFForStripped)
											cout << "\n...Extracted and hashed altered JPG [" << strOutFileName << "]";
										else
											cout << "\n...Extracted and hashed stripped JPG [" << strOutFileName << "]";
										cout << "\n0: " << (int)ucpImage[0] << " : " << 0xFFu;
										cout << "\n1: " << (int)ucpImage[1] << " : " << 0xD8u;
										cout << "\n2: " << (int)ucpImage[2] << " : " << 0xFFu;
										if (bTForAlteredFForStripped)
										{
											cout << "\n-1: " << (int)ucpImage[(ulEndOffset - ulStartOffset) - 1] << " : " << 0xFFu;
											cout << "\n-0: " << (int)ucpImage[(ulEndOffset - ulStartOffset) - 0] << " : " << 0xD9u;
										}
										else
										{
											cout << "\n-1: " << (int)ucpImage[((ulEndOffset - ulStartOffset) + 3) - 1] << " : " << 0xFFu;
											cout << "\n-0: " << (int)ucpImage[((ulEndOffset - ulStartOffset) + 3) - 0] << " : " << 0xD9u;
										}
									}
									// error condition
									else
										cout << "\nERROR: failed write to [" << strOutFileName << "].";

									delete[] ucpImage;
									ucpImage = NULL;
								}
								// error condition
								else
									cout << "\nERROR: image buffer is null.";

								// increment start offset
								//ulStartOffset = ulCnt + 1;
							}
						}

						// close and clear
						delete[] ucpJPG;

						// set return flag, success
						bReturn = true;
					}
					// error condition
					else
						cout << "\nERROR: JPG file buffer is null.";
				}
				// error condition
				else
					cout << "\nERROR: KDB file length is zero.";

				// close and clear
				ispJPG->close();
				delete ispJPG;
			}
			// error condition 
			else
				cout << "\nERROR: JPG file handle is null.";
		}
		// error conditions, terminate without analyzing
		else
			cout << "\nERROR: target data is null.";
	}
	catch (exception* e)
	{
		printf("\nERROR: %s\n", e->what());
	}

	return bReturn;
}

