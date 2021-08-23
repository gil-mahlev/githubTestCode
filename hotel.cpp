/**************************************
 * this file use sqlite database in order to 
 * create database of hotel
 * parameters validation not checked 
 * ************************************/

#include <algorithm>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <regex>
#include <fstream>
#include <sstream> 
#include <sqlite3.h>
#include <unistd.h>
#include <iomanip> 


using namespace std;


class date
{
private:
	int day;
	int month;
	int year; 
public:
	date(){}
	date(int d,int m,int y)
	{
		day=d;
		month=m;
		year=y;			
	}
	date(const date& c) { day = c.day; month = c.month; year = c.year;}
	int getMonth(void)
	{
		return month;
	}
	int getYear(void)
	{
		return year;
	}	
	int getDay(void)
	{
		return day;
	}
//	void operator=(const date& newDate)
//	{
//		day = newDate.day;
//		month = newDate.month;
//		year  = newDate.year;
//	}
	void increment(void)
	{
		month = (month+1)%13;
		if(month == 0)
		{
			month=1;
			year++;
		}
	}
	void decrement()
	{
		month--;
		if(month == 0)
		{
			month=12;
			year--;
		}
	}	
};


class customer
{
private:
	string name;
	string address;
	int id;
	int numOfGuests;
	string residence;
	date  checkin;
	date  checkout;
	
public:
	customer(const string& CusName,const string& CusAddress,const int& CusId,const string& CusResidence,const date& in,const date& out,const int& Guests)
	{
		name.assign(CusName);
		address.assign(CusAddress);
		id =CusId; // passport id
		residence.assign(CusResidence);
		checkin=in;
		checkout=out;
		numOfGuests = Guests;
	}
	friend class customersDatabase;
};

class customersDatabase
{
private:
	sqlite3 *db;
	string sqlFileName;
public:
	customersDatabase()
	{
		sqlFileName="c:/sqlite/sqliteDb/hoteldb.db";
		db = NULL;
	}
	~customersDatabase()
	{
		if(db != NULL)
		{
			cout << "free sql database"<<endl;
			sqlite3_close(db);
		}		
	}
	bool createTable(void);
	bool addEntryToTable(class customer& NewCustomer);
	bool queryEntriesFromTable(void);
	bool deleteEntryFromTable(int id);
};

bool customersDatabase::deleteEntryFromTable(int id)
{
	string sql = "DELETE FROM Customer WHERE ID = "+to_string(id)+";";
	
    int rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, NULL);
	
	
	if(rc != SQLITE_OK)
	{
	  cout << "failed to delete entry from table:"<<rc << endl;
	  return false;
	}
	return true;	
}

int getintLen(int num)
{
	int len=0;
	if(num == 0)
		return 1;
	while(num)
	{
		len++;
		num /= 10;
	}
	return len;
}

bool customersDatabase::queryEntriesFromTable(void)
{
	sqlite3_stmt *stmt;
	int n=0;
	int spaces=0;
	string sql = "SELECT * FROM Customer;";
	
	cout << "ID             |NAME           |ADDRESS             |RESIDENCE      |NUM OF GUEST   |CHECKIN             |CHECKOUT            |"<<endl;
	cout << "==============================================================================================================================="<<endl;

	int rc = sqlite3_prepare(db, sql.c_str(), -1, &stmt, 0);
	if(rc != SQLITE_OK) 
	{
		fprintf(stderr, "sql error #%d: %s\n", rc, sqlite3_errmsg(db));
	} 
	else 
	{
		while((rc = sqlite3_step(stmt)) != SQLITE_DONE) 
		{
			//cout << "got next entry in table:" << rc << endl;
			switch(rc) 
			{
				case SQLITE_BUSY:
					cout << "busy, wait 1 seconds" << endl;
					sleep(1);
					break;
				case SQLITE_ERROR:
					cout << "step error: "<< sqlite3_errmsg(db) << endl;
					break;
				case SQLITE_ROW:
					n = sqlite3_column_count(stmt);
					int i;
					for(i=0; i<n;i++)
					{
						//cout << sqlite3_column_name(stmt, i) << " = ";
						string name_column = sqlite3_column_name(stmt, i);
						switch(sqlite3_column_type(stmt, i)) 
						{
							case SQLITE_TEXT:
								//cout <<  sqlite3_column_text(stmt, i) << "      ";
								spaces=0;
								if(name_column.compare("NAME") == 0)
								{
									//15 characters
									spaces=16;
								}
								else if(name_column.compare("ADDRESS") == 0)
								{
									// 20 characters
									spaces=21;
								}
								else if(name_column.compare("RESIDENCE") == 0)
								{
									// 15 characters
									spaces=16;
								}
								else if(name_column.compare("CHECKIN") == 0)
								{
									// 20 characters
									spaces=21;
								}
								else if(name_column.compare("CHECKOUT") == 0)
								{
									// 20 characters
									spaces=21;
								}
								cout <<  sqlite3_column_text(stmt, i) << setw (spaces-strlen((const char *)sqlite3_column_text(stmt, i)))<<"|";
								break;
							case SQLITE_INTEGER:
								spaces=0;
								if(name_column.compare("ID") == 0)
								{
									// 10 characers
									spaces=16;
								}
								else if(name_column.compare("NUMOFGUEST") == 0)
								{
									// 15 charactars
									spaces=16;
								}
								cout <<  sqlite3_column_int(stmt, i) << setw(spaces-getintLen(sqlite3_column_int(stmt, i)))<<"|";
								break;
							case SQLITE_FLOAT:
								cout <<  sqlite3_column_double(stmt, i) << endl;
								break;
							case SQLITE_BLOB:
								cout <<  "(blob)" << endl;
								break;
							case SQLITE_NULL:
								cout <<  "(null)"<< endl;
								break;
							default:
								cout << "unknown type:" << sqlite3_column_type(stmt, i) << endl;
								break;
						}
					}
					break;
				default:
					cout << "unexpected result "<< rc << endl;
					goto exit;
					break;
					
			}
			cout << endl;
		}
	}
exit:
	sqlite3_finalize(stmt);
	
	return true;	
}

string quotesql( const string& s ) {
    return string("'") + s + string("'");
}

void getDateFromString(date& newDate,string& parseDate)
{
	int value[3];
	int counter=0;
	
	stringstream ss(parseDate);
	
	 while (ss.good()) 
	{
        string substr="";
        getline(ss, substr, '-');
		if(counter < 3)
		{
			value[counter] = atoi( substr.c_str() );
		}		
		counter++;
    }
	if(counter == 3)
	{
		date temp(value[0],value[1],value[2]);
		newDate = temp;
	}
}

bool customersDatabase::addEntryToTable(class customer& NewCustomer)
{
	int rc=0;
	// Save SQL insert data
	string stringDateIn=to_string(NewCustomer.checkin.getYear())+'-'+to_string(NewCustomer.checkin.getMonth())+'-'+to_string(NewCustomer.checkin.getDay());
	string stringDateOut=to_string(NewCustomer.checkout.getYear())+'-'+to_string(NewCustomer.checkout.getMonth())+'-'+to_string(NewCustomer.checkout.getDay());
	
	string table = "INSERT INTO Customer ('ID', 'NAME','ADDRESS','RESIDENCE','NUMOFGUEST','CHECKIN','CHECKOUT') VALUES (" \
					+quotesql(to_string(NewCustomer.id))+','\
					+quotesql(NewCustomer.name)+','
					+quotesql(NewCustomer.address)+','
					+quotesql(NewCustomer.residence)+','
					+quotesql(to_string(NewCustomer.numOfGuests))+','
					+quotesql(stringDateIn)+','
					+quotesql(stringDateOut) + ");";
    
    // Run the SQL (convert the string to a C-String with c_str() )
    rc = sqlite3_exec(db, table.c_str(), NULL, NULL, NULL);
	
	
	if(rc != SQLITE_OK)
	{
	  cout << "failed to insert table:"<<rc << endl;
	  cout << table << endl;
	  return false;
	}
	return true;
}

bool customersDatabase::createTable(void)
{
	const char *sql;
	int rc = sqlite3_open(sqlFileName.c_str(), &db);
	
	if(rc != SQLITE_OK)
	{
		cout << "failed to open file:"<<rc << endl;
		return false;
	}
	
	sql = "CREATE TABLE IF NOT EXISTS Customer ("  \
	  "ID INT PRIMARY KEY     NOT NULL," \
	  "NAME           TEXT    NOT NULL," \
	  "ADDRESS        TEXT    NOT NULL," \
	  "RESIDENCE      TEXT    NOT NULL," \
	  "NUMOFGUEST     INTEGER NOT NULL," \
	  "CHECKIN        TEXT    NOT NULL," \
	  "CHECKOUT       TEXT    NOT NULL);";
		  
	  rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
	  
	  if(rc != SQLITE_OK)
	  {
		  cout << "failed to create table:"<<rc << endl;
		  return false;
	  }

	return true;
}



class calander:public date
{
public:
	calander(int d,int m,int y):date(d,m,y)
	{	
	}
	int getDayNumber(void);
	string getDayOfTheWeek(void);
	string getMonthOfTheYear(void);
	int getNumDaysOfTheMonth(void);
	void printMonth(void);
	void printDate(void);
};
/*A Function that returns the index of the day
  of the date- day/month/year
  For e.g-
 
  Index     Day
  0         Sunday
  1         Monday
  2         Tuesday
  3         Wednesday
  4         Thursday
  5         Friday
  6         Saturday*/
int calander::getDayNumber(void)
{
   int t[] = { 0, 3, 2, 5, 0, 3, 5, 1,
                       4, 6, 2, 4 };
    int newYear = getYear() - getMonth() < 3;
    return ( newYear + newYear/4 - newYear/100 +
             newYear/400 + t[getMonth()-1] + getDay()) % 7;	
}

string calander::getDayOfTheWeek(void)
{
	string dayOfTheWeek[] = {"Sun", "Mon","Tue", "Wed","Thu","Fri","Sat"};
	
	return dayOfTheWeek[getDayNumber()];		
}

string calander::getMonthOfTheYear(void)
{
	string monthOfTheYear[] = {"Jenuary","Febuary","March","April","May","Jone","July","August","september","october","november","december"};
	
	return monthOfTheYear[getMonth()-1];	
}
int calander::getNumDaysOfTheMonth(void)
{
	int daysInMonth[] = {31,29,31,30,31,30,31,31,30,31,30,31};
    // January
 
    // February
    if (getMonth() == 1)
    {
        // If the year is leap then February has
        // 29 days
        if (getYear() % 400 == 0 ||
                (getYear() % 4 == 0 && getYear() % 100 != 0))
            return (29);
        else
            return (28);
    }
	return daysInMonth[getMonth()-1];
}

void calander::printMonth(void)
{
    int days;
 
    int current = getDayNumber();
 
	days = getNumDaysOfTheMonth ();
 
	// Print the current month name
	cout <<endl <<"------------"<<getMonthOfTheYear()<<" "<<getYear()<<"-------------" << endl;
	
 
        // Print the columns
        cout << "  Sun  Mon  Tue  Wed  Thu  Fri  Sat\n";
 
 
	// Print appropriate spaces
	int k;
	for (k = 0; k < current; k++)
	{
            printf("     ");
	}
 
	for (int j = 1; j <= days; j++)
	{
		printf( "%5d",j);
		if (++k > 6)
		{
			k = 0;
			cout <<endl;
		}
    }	
}

void calander::printDate(void)
{
	cout << "day:" << getDayOfTheWeek() <<" month:" << getMonthOfTheYear() << " days in this month:" << getNumDaysOfTheMonth() << endl;
}
void DeleteEntry(customersDatabase& db)
{
	int id=0;
	system ("CLS");
	cout <<"ID entry to delete : ";
	cin >> id;	
	
	db.deleteEntryFromTable(id);
}
void createNewOrder(customersDatabase& db)
{
	system ("CLS");

	
	cout <<"Name : ";
	string name="";
	getline(cin, name);
	
	cout <<"address : ";
	string address="";
	getline(cin, address);

	
	cout <<"residence : ";
	string residence="";
	getline(cin, residence);
	
	cout <<"start date year-month-day: ";
	string fromdate="";
	getline(cin, fromdate);
	date  checkin;
	getDateFromString(checkin,fromdate);
	
	cout <<"end date year-month-day: ";
	string todate="";
	getline(cin, todate);
	date  checkout;
	getDateFromString(checkout,todate);
	
	cout <<"id : ";
	int id=0;
	cin >> id;		
	
	cout <<"number of guest : ";
	int numOfGuests=0;
	cin >> numOfGuests;	
	
	
	customer newCustoer(name,address,id,residence,checkin,checkout,numOfGuests);
	
	db.addEntryToTable(newCustoer);
}

void printDate(void)
{
	string getUser;
	customersDatabase customerDb;
	
	if(customerDb.createTable() != true)
	{
		cout << "failed to use database" << endl;
		return;
	}
	
	calander month1(1,9,2021);
	calander month2(1,10,2021);
	calander month3(1,11,2021);
	//Gil.printDate();
	
	while(1)
	{
		system ("CLS");
		month1.printMonth();
		month2.printMonth();
		month3.printMonth();
		cout << endl;
		cout<<"1. new order"<< endl;
		cout<<"2. show"<< endl;
		cout<<"3. delete order"<< endl;
		cout<<"4. next"<< endl;
		cout<<"5. prev"<< endl;
		cout<<"6. exit"<< endl;
		
		getUser="";
		getline(cin, getUser);
		if(getUser.compare("next") == 0)
		{
			month1.increment();
			month2.increment();
			month3.increment();
		}
		else if(getUser.compare("prev") == 0)
		{
			month1.decrement();
			month2.decrement();
			month3.decrement();			
		}
		else if(getUser.compare("new order") == 0)
		{
			createNewOrder(customerDb);
		}
		else if(getUser.compare("show") == 0)
		{
			customerDb.queryEntriesFromTable();
			cin >> getUser;
		}
		else if(getUser.compare("delete order") == 0)
		{
			DeleteEntry(customerDb);
		}
		else if(getUser.compare("exit") == 0)
		{
			break;
		}
	}
	
}

