// Coursework1.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "pch.h"
#include <iostream>
#include <stdlib.h>
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"
#include <fstream>
#include <string>


int PrintDataStructure(HEADER_E *pointer,short unsigned int numoItemsInDS = 0) {
	short unsigned int inHeader = 0;
	printf("                  |  Head char: %c - %p - %p | \n\n",pointer->cBegin,pointer->pPrior,pointer->pNext);
	
	for (int i = 0; i < 26; i++) {
		if ((pointer->ppItems)[i] != 0) {
			ITEM9* pparay = (ITEM9*)((pointer->ppItems)[i]);
			
			do {

				printf("%02d) %20s # %d #  Date:%d.%s.%d %p\n", ++inHeader, pparay[0].pID, pparay[0].Code, 
					(pparay[0].pDate)->Day, (pparay[0].pDate)->pMonth, (pparay[0].pDate)->Year,pparay->pNext);
				numoItemsInDS++;

				pparay = pparay->pNext;
			
			} while (pparay != 0);
		}
			
		
	}
	std::cout << "\n-------------------------------------------------------\n\n";

	if (pointer->pNext) {
		PrintDataStructure(pointer->pNext,numoItemsInDS);  // Calls this function with a pointer to Next header, so it can go through A-Z
	}
	else {
		printf("Number of items in this data set: %d   |\n", numoItemsInDS);
		if (numoItemsInDS < 1) {
			std::cout << "Error Error Error !!";
		}
		std::cout << "\n-------------------------------------------------------\n\n";
		return numoItemsInDS;
	}
}

int ItemsInDataStructure(HEADER_E *pointer, short unsigned int numoItemsInDS = 0) {
	short unsigned int inHeader = 0;

	for (int i = 0; i < 26; i++) {
		ITEM9 * testss = (ITEM9*)(pointer->ppItems)[i];

		if ((pointer->ppItems)[i] != 0) {
			ITEM9* pparay = (ITEM9*)((pointer->ppItems)[i]);
			do {
				numoItemsInDS++;
				pparay = pparay->pNext;
			} while (pparay != 0);
		}
	}
	if (pointer->pNext) {
		ItemsInDataStructure(pointer->pNext, numoItemsInDS);  // Calls this function with a pointer to Next header, so it can go through A-Z
	}
	else {
			if (numoItemsInDS < 1) {
			return -1;
		}	
		return numoItemsInDS;
	}
}

int FormatCheck(char *checkString) {
	int formatIntegrity = 1;

	int n_space = 0, n_forbidchar = 0;
	for (size_t i = 0; i < strlen(checkString); i++) {  // counts spaces, forbidden chars and checks for uppercase
		if (i == 0) {
			if (((int)checkString[i]) < 65 || ((int)checkString[i]) > 90) {
				printf("\nError 58: First word doesn't start with an uppercase letter\n");
				formatIntegrity--;
			}
		}
		if (checkString[i] == ' ') {
			n_space++;
			if (i == 0 || i == (strlen(checkString) - 1)) {
				printf("\nError 59: Space can only be in the middle of 2 words\n");
				formatIntegrity--;
			}
			else if (((int)checkString[i + 1]) < 65 || ((int)checkString[i + 1]) > 90) {
				printf("\nError 63: Second word doesn't start with an uppercase letter\n");
			};
		}
		if ((checkString[i] < 65 && checkString[i] != 45 && checkString[i] != 32) || (checkString[i] > 90 && checkString[i] < 97) || checkString[i] > 122) {
			n_forbidchar++;
		}
	}

	if (n_space != 1) {
		printf("\nError 65: Rule of 1 space between words is broken\n");
		formatIntegrity--;
	}
	if (n_forbidchar > 0) {
		printf("\nError 68: Use of forbidden Chars\n");
		formatIntegrity--;
	}
	if (strlen(checkString) < 3) {
		printf("\nError 74: Insufficent letters/words in ID!\n");
		formatIntegrity--;
	}

	return formatIntegrity;
}

int * CheckID(HEADER_E *pointer,char checkName[]) {   //returns 1 if ID is not in specified data structure and returns 0 if the ID already exists
	
	char * fullName = (char*)malloc(50);
	strcpy(fullName, (char *)checkName);

	char * tmpName = (char*)malloc(50);
	strcpy(tmpName, (char *)checkName);

	char * fname = (char*)malloc(50);
	strcpy(fname, strtok(tmpName, " "));

	char * lname = (char*)malloc(50);
	strcpy(lname, strtok(NULL, " "));


	while (1) {
		
		if (pointer->cBegin == fname[0]) {
			int alphabeN = lname[0] - 65;
			//printf("\n - Line 88 - alphaben: %d\n",alphabeN);

			if ((pointer->ppItems)[alphabeN] != 0) {
				ITEM9 * item = (ITEM9*)((pointer->ppItems)[alphabeN]);
				//printf("\n - Line 91 itemID: %s - checkname: %s\n",item->pID,fullName);
				if (strcmp(item->pID, fullName) == 0) { return (int *)item; }

				while (1) {
					if (strcmp(item->pID, fullName) == 0) {
						return (int *)item;    // in case of match, returns pointer of
					}
					else {
						if (item->pNext) {
							item = item->pNext;
						
							//printf("\n - Line 99\n");
						}
						else { 
							//printf(" returned 1 : 100");
							return 0; 
						}
					};
				}
			}
			break;
		}
		
		if (pointer->pNext) {
			pointer = pointer->pNext;
		}
		else {break;}
		
	}
	//printf(" returned 1 : 116");
	return 0;
}

ITEM9 * CheckCode(HEADER_E *pointer, long int code) {
	for (int i = 0; i < 26; i++) {
		if ((pointer->ppItems)[i] != 0) {
			ITEM9* pparay = (ITEM9*)((pointer->ppItems)[i]);

			do {
				if (pparay->Code == code) {
					return pparay;
				}
				pparay = pparay->pNext;

			} while (pparay != 0);
		}
	}

	if (pointer->pNext) {
		CheckCode(pointer->pNext,code);  // Calls this function with a pointer to Next header, so it can go through A-Z
	}
	else {
		return 0;
	}
}

ITEM9 * CheckDateTime(HEADER_E *pointer, DATE3 * date) {
	for (int i = 0; i < 26; i++) {
		if ((pointer->ppItems)[i] != 0) {
			ITEM9* pparay = (ITEM9*)((pointer->ppItems)[i]);

			do {
				if (pparay->pDate == date) {
					return pparay;
				}
				pparay = pparay->pNext;

			} while (pparay != 0);
		}
	}

	if (pointer->pNext) {
		CheckDateTime(pointer->pNext, date);  // Calls this function with a pointer to Next header, so it can go through A-Z
	}
	else {
		return 0;
	}
}

int * InsertItem(HEADER_E *pointer, char *pNewItemID = 0, ITEM9 * pitem = 0) { // error in inserting

	if (pNewItemID != 0) {
		if (FormatCheck(pNewItemID) < 1) {
			printf("\nError 51: Failed formatcheck\n");
			return 0;
		}
		if (CheckID(pointer, pNewItemID)) {
			printf("\nError 55: Item with same ID already exists in this data structure\n");
			return 0;
		}
	}

	char * fullName = (char*)malloc(50);
	strcpy(fullName, (char *)pNewItemID);

	char * tmpName = (char*)malloc(50);  // used for strtok dummie
	strcpy(tmpName, (char *)pNewItemID);

	char * fname = (char*)malloc(50);
	strcpy(fname, strtok(tmpName, " "));

	char * lname = (char*)malloc(50);
	strcpy(lname, strtok(NULL, " "));
	int alphabeN = lname[0] - 65;

	ITEM9 * tmppoint = (ITEM9*)((pointer->ppItems)[alphabeN]);
	ITEM9 * item;

	if (pitem == 0) {
		item = (ITEM9*)GetItem(9, pNewItemID);
	}
	else {
		item = pitem;
	}
	

	while (1) {    // this loop is responsible for choosing/creating the right header before inserting the item
		HEADER_E * lastHead = new(headerE);
		lastHead = pointer;
		if ( (int)pointer->cBegin == (int)fname[0] ) {

			//printf("\n - Line 88 - alphaben: %d\n", alphabeN);
			
			if (tmppoint!=0) {
				while (tmppoint->pNext != 0) {
					tmppoint = tmppoint->pNext;	
				}
				tmppoint->pNext = (ITEM9*)item;	
				return (int *)item;
			}
			else {
				((pointer->ppItems)[alphabeN]) = (ITEM9*)item;
				return (int *)item;
			}
		}
		

		else if ((int)pointer->cBegin < (int)fname[0]) {
			if (pointer->pNext) {
				pointer = pointer->pNext;		
			} 
			else {
				
				HEADER_E * newHead = new(headerE);
				pointer->pNext = newHead;
				(*newHead).cBegin = fname[0];
				newHead->pPrior = pointer;
				newHead->pNext = (HEADER_E *)0;
				//void ** pupu = (void **)malloc(1000); 
				void ** pupu = (void **)malloc(26 * sizeof(void *));
				newHead->ppItems = pupu;
				for (int j = 0; j < 26; j++) {
					pupu[j] = (void *)0;
				}
				pupu[alphabeN] = item;
				return (int *)item;
				
			}
		}

		if ((int)pointer->cBegin > (int)fname[0]) {

			HEADER_E * newHead = new(headerE);
			
			(*newHead).cBegin = fname[0];
			newHead->pNext = pointer;
			newHead->pPrior = pointer->pPrior;
			void ** pupu = (void **)malloc(1000);
			for (int j = 0; j < 26; j++) {
				pupu[j] = (void *)0;
			}
			newHead->ppItems = pupu;

			lastHead->pNext = newHead;
			pointer->pPrior = newHead;
			pupu[alphabeN] = item;
			return (int *)item;

		}

	}

}

int CheckHeader(HEADER_E * pointer) { // takes pointer to header
	int amount = 0;
	for (int i = 0; i < 26; i++) {

		if ((pointer->ppItems)[i] != 0) {
			std::cout << (pointer->ppItems)[i];
			ITEM9* pparay = (ITEM9*)((pointer->ppItems)[i]);
			
				do {

					amount++;
					if (pparay->pNext) {
						pparay = pparay->pNext;
					}
					else { break; }

				} while (pparay != 0);
			
		
		}	
	}
	return amount;
}

HEADER_E  * RemoveItem(HEADER_E *pointer, char *pItemID) { // p, "Vegas Gold"
	HEADER_E * origin = pointer;
	ITEM9 * theitem = new(item9);

	char * fullName = (char*)malloc(50);
	strcpy(fullName, (char *)pItemID);

	char * tmpName = (char*)malloc(50);
	strcpy(tmpName, (char *)pItemID);

	char * fname = (char*)malloc(50);
	strcpy(fname, strtok(tmpName, " "));

	char * lname = (char*)malloc(50);
	strcpy(lname, strtok(NULL, " "));
	int alphabeN = lname[0] - 65;

	if (FormatCheck(pItemID) < 1) {
		printf("\nError 51: Failed formatcheck\n");
		return 0;
	}
	HEADER_E* lastHead;
	HEADER_E* thisHead;
	ITEM9* lastItem;
	ITEM9* thisItem;	

	printf("\n273:\n");

	theitem = (ITEM9*)CheckID(pointer, pItemID);
	printf("\n276: %p\n",theitem);
	if (theitem) {
		if (pointer->cBegin != fname[0]) {
			printf("\n278:\n");
			while (pointer) {
				
				if (pointer->pNext->cBegin==fname[0]) {
					lastHead = pointer;
					thisHead = pointer->pNext;
					ITEM9 * firstItem = (ITEM9 *)(thisHead->ppItems)[alphabeN];
					if (firstItem->pNext == 0) {
						thisHead->ppItems[alphabeN] = 0;
						delete (ITEM9 *)firstItem;
						printf("\n280: item deleted\n");
						

						if (CheckHeader(thisHead) < 1) {         // deletes header if no items left
							lastHead->pNext = thisHead -> pNext;
							if (thisHead->pNext) {
								thisHead->pNext->pPrior = lastHead;
							}
							delete thisHead;
							printf("\n310: Header deleted\n");
						}
						return origin;
					}
					else {
						thisItem = firstItem;
						while (strcmp(thisItem->pNext->pID, fullName) != 0) {
							thisItem = thisItem->pNext;
						}
						
						thisItem->pNext = thisItem->pNext->pNext;
						printf("\n295: item deleted\n");
						thisHead->ppItems[alphabeN] = thisItem;

						delete (ITEM9 *)thisItem->pNext;
						return origin;
					}
				}

				if (pointer->pNext) {
					pointer = pointer->pNext;
				}
				
			}
		} else {
			//action is in first header
			thisHead = pointer;
			ITEM9 * firstItem = (ITEM9 *)(thisHead->ppItems)[alphabeN];
			if (firstItem->pNext == 0) {
				thisHead->ppItems[alphabeN] = 0;
				delete (ITEM9 *)firstItem;

				printf("\n341: item deleted\n");
				int a = CheckHeader(thisHead);
				if (CheckHeader(thisHead) < 1) {         // deletes header if no items left
					thisHead->pNext->pPrior = (HEADER_E *)0;
					HEADER_E *ds = thisHead->pNext;
					delete thisHead;
					printf("\n347:First Header deleted\n");
					return ds;
				}
				return origin;
			}
			else {
				thisItem = firstItem;
				while (strcmp(thisItem->pNext->pID, fullName) != 0) {
					thisItem = thisItem->pNext;
				}

				thisItem->pNext = thisItem->pNext->pNext;
				printf("\n295: item deleted\n");
				thisHead->ppItems[alphabeN] = thisItem;
				delete (ITEM9 *)thisItem->pNext;
				return origin;
			}
		}
	}
	else {
		return 0; // nothing to remove
	}
}

int EraseDataStructure(HEADER_E * pDS) {
	int deleted = 0;
	while (1) {
		ITEM9 ** ppitems = (ITEM9**)(pDS->ppItems);
		for (int i = 0;i < 26; i++) {
			if (ppitems[i] != 0) {
				ITEM9 ** tmp = ppitems;
				ITEM9 * tmpItem = ppitems[i];
				while (1) {
					
					if (tmpItem->pNext) {
						ITEM9 * tmp2 = tmpItem->pNext;
						delete tmpItem;
						deleted++;
						tmpItem = tmp2; 
						}	
					else { 
						delete tmpItem;
						deleted++;
						tmp[i] = 0;
						break;
					} 
				}
			}
		}

	

		if (pDS->pNext == 0) {
			
			delete pDS->ppItems;
			delete pDS;
			break;
		}
		else { 
			HEADER_E * tmp = pDS->pNext;
			delete pDS->ppItems;
			delete pDS;
			pDS = tmp;
			//printf("\nHeader deleted\n");
		}
	} 

	printf("\nData structure erased:\n");
	return deleted;
}

HEADER_E * CopyDataStructure(HEADER_E * copyFrom) {
	int copied = 0;
	HEADER_E * firstHead = new(HEADER_E);
	HEADER_E * header = firstHead;
	
	for (int j = 0; copyFrom; j++) {
		
		header->pPrior = 0;
		header->cBegin = copyFrom->cBegin;
		header->ppItems = new void*[26];
		
		for (int k = 0; k < 26; k++) {
			header->ppItems[k] = NULL;
		}

		ITEM9 ** itemsChainT = (ITEM9 **)header->ppItems;
		ITEM9 ** itemsChainF = (ITEM9**)(copyFrom->ppItems);
		for (int i = 0; i < 26; i++) {
			ITEM9 * firstItemT = new ITEM9;
			if (itemsChainF[i] != 0) {
				ITEM9 * thisItemF = itemsChainF[i];
				ITEM9 * thisItemT = new ITEM9;
				
				
				for (int b = 0; b>-1; b++) {
					
					
					if (b == 0) {
						//delete firstItemT;
						firstItemT = thisItemT;
					}
					
					*thisItemT = *thisItemF;
					printf("\nItem copied: %s\n",thisItemT->pID);
					thisItemT->pNext = 0;

					if (thisItemF->pNext) {
						thisItemT->pNext = new ITEM9;
						thisItemT = thisItemT->pNext;
						thisItemF = thisItemF->pNext;
						
					}
					else {
						break;
					}
				}
				itemsChainT[i] = new ITEM9;
				*itemsChainT[i] = *firstItemT;


				*itemsChainT[i];
				
				printf("");
			}
		}
		
		header->ppItems = (void**)itemsChainT;
		//printf("\n- %s -\n",header->ppItems[8]);
		// new ppitems array must be assigned to left ds


		if (copyFrom->pNext) {

			header->pNext = new HEADER_E;


			header = header->pNext;
			copyFrom = copyFrom->pNext;
		}
		else {
			header->pNext = 0;
			break;
		}

	} // end of while

	printf("\nData structure copied:\n");
	

	//for(int g = 0; g < 26; g++) {
			
		//printf("\n%p - %d\n",firstHead->ppItems[g], ((ITEM9 *)firstHead->ppItems[g])->pID);
	//}
	
	return firstHead;
}

ITEM9 * getitem9(char * name = (char*)"") {
	return (ITEM9*)GetItem(9, name);
}


class DataStructure {

	HEADER_E *ptds = 0;
public:

	DataStructure() {}  // empty constructor

	DataStructure(int amount) {
		ptds = GetStruct5(9, amount);
	}

	DataStructure(char *pFilename) {
		using namespace std;
		fstream file;
		file.open(pFilename, fstream::in);

		if (file.good() == 0) {
			printf("\nNot good! - gcount: %d\n", file.gcount());
			throw;
		}

		if (!file) {
			cout << endl << "Cannot open file!" << endl;
			throw;
		}

		int taken = 0;
		char newl = '\n';
		char period = '.';
		char end = '\0';

		ptds = new HEADER_E;
		//file.read(&ptds->cBegin,1);
		ptds->cBegin = file.get();
		ptds->pNext = 0;
		ptds->pPrior = 0;
		ptds->ppItems = (void **)malloc(26 * sizeof(void *));
		for (int j = 0; j < 26; j++) {
			ptds->ppItems[j] = (void *)0;
		}

		file.seekg(ios_base::beg);
		

		
		while (1) { // gimme all the shit
			char * line = (char*)malloc(100);
			

			file.getline(line, 30, period);
			
			//cout <<endl<<file.good() << file.eof() << file.fail() << file.bad() << file.rdstate()<<endl;


			if (!line) {
				printf("\n%d items taken\n",taken);
				break;
			}

			if (!file.good()) {
				printf("\n645: file not good, %d items copied\n",taken);
				break;
			}

			
			
			ITEM9 * item = getitem9(line);

			file.getline(line, 30, period);
			long int code;
			sscanf(line, "%d", &code);
			item->Code = code;

			file.getline(line, 30, period);
			int day;
			sscanf(line, "%d", &day);
			item->pDate->Day = day;

			file.getline(line, 30, period);
			*item->pDate->pMonth = *line;

			file.getline(line, 5);
			int year;
			sscanf(line, "%d", &year);
			item->pDate->Year = year;

			

			InsertItem(ptds, item->pID, item);
			taken++;
			delete line;
			//cout<<endl<<this->GetItemsNumber();

			if (file.good()==0) {
				printf("\n693: file not good, %d items copied\n", taken);
				break;
			}
		}

	}

	DataStructure(const DataStructure &Original) {
		HEADER_E * dsFrom = Original.GetPtr();

		ptds = CopyDataStructure(dsFrom);

	}
	
	int GetItemsNumber() {
		if (ptds == 0) {
			return 0;
		}
		else {
			return ItemsInDataStructure(ptds);
		}
	} 
	
	HEADER_E * GetPtr() const {return ptds;}

	ITEM9 * GetItem(char *pID    ) {return (ITEM9 *)CheckID(ptds,pID);}

	ITEM9 * GetItem(long int code) {return CheckCode(ptds,code)      ;}

	ITEM9 * GetItem(DATE3 * date ) {return CheckDateTime(ptds,date)  ;}

	
	void operator+=(ITEM9 *item) {

		if (CheckID(ptds, ((ITEM9*)item)->pID)== 0) {
			InsertItem(ptds, ((ITEM9*)item)->pID, (ITEM9 *)item);
			}
		else {
			printf("\nID already exists\n");
			//throw "ID already exists";
		}
	}

	void operator-=(char *pID) {
		if (CheckID(ptds, pID) != 0) {
			RemoveItem(ptds, pID);
		}
		else {
			printf("\nNothing to remove\n");
			//throw "ID already exists";
		}
	}

	DataStructure &operator=(const DataStructure &Right) {
		if (ptds != 0) {

			EraseDataStructure(ptds);
			ptds = Right.GetPtr();
		}
		else {
			ptds = Right.GetPtr();
			printf("\ncopied; Left ds was empty \n");
		}
		
		return *this;
	}
 
	friend std::ostream &operator<<(std::ostream &ostr, const DataStructure &str) {
		HEADER_E * ds = str.GetPtr();
		PrintDataStructure(ds);
		return ostr;
	}

	bool operator==(DataStructure &Other) {
		HEADER_E * pointer = Other.GetPtr();
		HEADER_E * thisHead = ptds;

		if (this->GetItemsNumber() != Other.GetItemsNumber()) {
			return 0;
		}

		while (pointer) {
			for (int i = 0; i < 26; i++) {
				if ((pointer->ppItems)[i] != 0) {
					ITEM9* pparay = (ITEM9*)((pointer->ppItems)[i]);

					do {
						if (CheckCode(ptds, pparay->Code)) {
							
						}
						else { return 0; }
						pparay = pparay->pNext;

					} while (pparay != 0);
				}
			}

			if (pointer->pNext) {
				pointer = pointer->pNext;
				thisHead = pointer->pNext;
			}
			else {
				return 1;
			}
		}
	}

	void Write(char * pFilename) {
		using namespace std;
		//ofstream file(pFilename, ios::out);
		//ofstream file ("datastruct.binary", ios::out | ios::binary);
		fstream file;
		file.open(pFilename,fstream::out);

		if (file.good() == 0) { 
			printf("\nNot good! - gcount: %d\n", file.gcount());
			throw;
		}

		if (!file) {
			cout << endl<<"Cannot open file!" << endl;
			throw;
		}

		HEADER_E * pointer = ptds;
		HEADER_E * thisHead = ptds;
		int written = 0;
		char newl = '\n';
		char period = '.';
		char end = '\0';
		
		while (pointer) {
			for (int i = 0; i < 26; i++) {
				if ((pointer->ppItems)[i] != 0) {
					ITEM9* thisItem = (ITEM9*)((pointer->ppItems)[i]);

					do {
						
						file.write(thisItem->pID,strlen(thisItem->pID));
						file << period;
						
						string codestr1 = to_string(thisItem->Code);
						char const *codestr = codestr1.c_str();
						file.write(codestr,strlen(codestr));
						file << period;

						string daystr1 = to_string(thisItem->pDate->Day);
						char const *daystr = daystr1.c_str();
						file.write(daystr,strlen(daystr));
						file << period;

						file.write(thisItem->pDate->pMonth, strlen(thisItem->pDate->pMonth));
						file << period;

						string yearstr1 = to_string(thisItem->pDate->Year);
						char const *yearstr = yearstr1.c_str();
						file.write(yearstr, strlen(yearstr));
						file << newl;

					
						if (file.good() == 0) {
							printf("\nNot good! - gcount: %d\n", file.gcount());
							throw;
						}

						written++;
						thisItem = thisItem->pNext;
					} while (thisItem != 0);
				}
			}

			if (pointer->pNext) {
				pointer = pointer->pNext;
				thisHead = pointer->pNext;
			}
			else {
				printf("\nall items were succesfully written in the file: n = %d \n",written);
				return;
			}
		}

		file.close();
	}  // end of write method

	~DataStructure() {
		EraseDataStructure(ptds);
		
	}
};


int test1()	{
    std::cout << "Hello , this is demonstration of coursework1!\n\n"; 
	int Itype = 9;
	int Iamount = 100;

	char * checkStr1 = (char *)".snasas1";
	char * checkStr2 = (char *)"Ket Isgood";
	char * checkStr3 = (char *)"Atomic Tangerine";
	char * checkStr4 = (char *)"Antique White";
	char * checkStr5 = (char *)"Antique Fuchsia";
	
	
HEADER_E *p = GetStruct5(Itype, Iamount);					    // constructs a data structure of Itype & Iamount
	
	//printf("\nFormatCheck output: %d\n", FormatCheck(checkStr1 ));  // tests FormatCheck function

	ITEM9 * testItem1 = (ITEM9*)GetItem(9);							// Creates Automatic ID selected item
	ITEM9 * testItem2 = (ITEM9*)GetItem(9, checkStr2);				// Creates Manual    ID selected item

	//InsertItem(p, checkStr1);
	//InsertItem(p, checkStr2);
	//InsertItem(p, checkStr3);


	//printf("\nId: %s in data structure (1 - doesn't exist , 0 - exists) = %d\n",checkStr2, CheckID(p, (char *)checkStr2)); // tests CheckID function


	//PrintDataStructure(p);

	
	//pointer_to_data_structure = RemoveItem(pointer_to_data_structure, char *pItemID);  // prototype
	p=RemoveItem(p, checkStr3);
	p=RemoveItem(p, checkStr4);
	p=RemoveItem(p, checkStr5);



	PrintDataStructure(p);
	

	return 0;
}

int test3() {
	DataStructure *testds = new DataStructure(100);
	//std::cout <<std::endl<< testds->GetItemsNumber()<<std::endl;  // tests GetItemNumber func

	char * testStr1 = (char*)"Vegas Gold";
	//printf("\nItemCheck: %s - %p\n",testStr1, testds->GetItem(testStr1)); // tests GetItem func 

	

	char * zegas = (char *)"Zegas Told";
	InsertItem(testds->GetPtr(), zegas);
	ITEM9 * item = testds->GetItem(zegas);
	ITEM9 * test = testds->GetItem(item->Code);
	printf("\nItem inserted: %s It's code: %d,- %s <- the id that is responsible for this CodeCheck\n",zegas,item->Code,test->pID); // tests GetItem by code;

	DATE3 * checkdate = item->pDate;
	ITEM9 * test2 = testds->GetItem(checkdate);
	printf("\n%s\n", test2->pID);   // Tests Getitem by date


	ITEM9 * test3 = (ITEM9*)(GetItem(9));
	*testds+=test3;


	//PrintDataStructure(testds->GetPtr());

	return 0;
}

int test4() {
	char * zegas = (char *)"Zegas Told";
	DataStructure *testds = new DataStructure(100);
	DataStructure *testds2 = new DataStructure(60);
	ITEM9 * test3 = (ITEM9*)(GetItem(9,zegas));
	
	//std::cout<<testds->GetItemsNumber();
	//*testds += test3;
	//*testds -= zegas;
	*testds = *testds2;
	
	PrintDataStructure(testds->GetPtr());

	
	return 0;
}

int test5() {

	DataStructure *testds = new DataStructure(99);
	
	DataStructure *testds2 = new DataStructure(*testds);

	//PrintDataStructure(testds2->GetPtr());
	//printf("\n-------%d",*testds2==*testds);
	//std::cout << *testds2;
	

	return 0;
}

int test6() {
	char * filename= (char*)"datastruct.txt";
	DataStructure *testds = new DataStructure(100);
	testds->Write(filename);

	DataStructure *dsfromFile = new DataStructure(filename);
	std::cout << *dsfromFile;

	return 0;
}



int main(){   test6(); return 0;}    // choose what to test
