#include <iostream>
#include <string>
#include <ctime>
#include <climits>
#include <limits>
#include <fstream>
using namespace std;

	const int catn=5,carn=5,recn=100,destn=10;

	string admin="FBI",adminpass="OTD";

	string catname[catn]={"Hatchback","Sedan","SUV","Luxury","Van"};
	int catsec[catn]={10000,15000,20000,50000,25000};
	int maxcap[catn]={4,4,7,4,12};

	string carname[catn][carn]={
    	{"Suzuki Alto","Toyota Vitz","Kia Picanto","Daihatsu Mira","Nissan Dayz"},
    	{"Honda City","Toyota Yaris","Corolla Altis","Nissan Sunny","Hyundai Elantra"},
    	{"Kia Sportage","Hyundai Tucson","MG HS","Toyota Fortuner","Honda CR-V"},
    	{"Toyota Prado","Audi A6","Mercedes C","BMW 5","Lexus ES"},
    	{"Suzuki Bolan","Toyota Hiace","Hyundai Grand","Nissan Urvan","Ford Transit"}
		};

	int carprice[catn][carn]={
    	{4000,4700,4500,3800,4200},
    	{7000,6500,7500,6200,8000},
    	{13000,13500,14000,16000,15000},
    	{25000,32000,40000,35000,28000},
    	{3500,9000,15000,12000,18000}
		};

	string renter[catn][carn],rentercnic[catn][carn],phone[catn][carn],address[catn][carn],status[catn][carn];
	string rentdate[catn][carn],returndate[catn][carn],destination[catn][carn];
		
		int booked[catn][carn];
		int sday[catn][carn];
		int smonth[catn][carn];
		int syear[catn][carn];
		int secpaid[catn][carn];
		
	bool avail[catn][carn],insured[catn][carn],hasdriver[catn][carn];

	string maintcar[recn],maintreason[recn];
	string maintreportdate[recn],maintrepairdate[recn];
	bool maintfixed[recn];
	int maintcount=0;

	string histname[recn],histcnic[recn],histcar[recn];
	string histdate[recn],histdest[recn];
	int histcount=0;

	long long totalrevenue=0,totalfine=0,totalbonus,totalrepcost=0;

	string destname[destn]={"Murree","Naran","Hunza","Skardu","Peshawar","Islamabad","Karachi","Swat","Fairy Meadows","Neelum Valley"};
	int destdist[destn]={460,620,940,980,520,380,1250,550,780,600};
	int desttoll[destn]={1600,2000,2200,2500,1900,1400,6700,1800,1400,1800};

	int driverrate=2000;
	int fuelprice=299;

void clearinput(){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
}
int getint(string msg,int low,int high){
    int val;
    while (true){
        cout<<msg;
        if (cin >> val){
            if (val >= low && val <= high){
                clearinput();
                return val;
            }
            cout<<"\a\t\t\t\t\t\t\t\t  Please enter a number between "<<low<<" and "<<high<<".\n";
        } else {
            if (cin.eof()) {
   				 cout << "\n\n\t\t\t\t  Input cancelled. Returning to menu...\n\a";
				    cin.clear();
			    return  INT_MIN;
				}
            cout<<"\t\t\t\t\t\t\t\t  Invalid input! Enter a number.\n\a";
            clearinput();
        }
    }
}
long long getlarge(string msg,long long low,long long high){
    long long val;
    while (true){
        cout<<msg;
        if (cin >> val){
            if (val >= low && val <= high){
                clearinput();
                return val;
            }
            cout<<"\a\t\t\t\t\t\t\t\t  Amount must be greater than "<<low<<".\n";
        } else {
            if (cin.eof()) {
   				 cout << "\a\n\n\t\t\t\t  Input cancelled. Returning to menu...\n";
				    cin.clear();
			    return  INT_MIN;
				}
            cout<<"\a\t\t\t\t\t\t\t\t  Invalid input! Enter a number.\n\a";
            clearinput();
        }
    }
}
string gettext(string msg,bool onlydigits=false,int exactlen=-1){
    string s;
    while (true){
        cout<<msg;
        getline(cin,s);
		 if (cin.eof()) {
   				 cout << "\a\n\n\t\t\t\t  Input cancelled. Returning to menu...\n";
				    cin.clear();
			    return "";
				}
        if (s.empty()){
            cout<<"\a\t\t\t\t\t\t\t\t  This field cannot be empty.\n";
            continue;
        }
        if (onlydigits){
            bool ok=true;
            for (int i=0; i<(int)s.size(); i++){
                if (!isdigit((unsigned char)s[i])){ ok=false; break; }
            }
            if (!ok){
                cout<<"\a\t\t\t\t\t\t\t\t  Only digits allowed.\n";
                continue;
            }
        }
        if (exactlen != -1 && (int)s.size() != exactlen){
            cout<<"\a\t\t\t\t\t\t\t\t  Must be exactly "<<exactlen<<" characters.\n";
            continue;
        }
           
        return s;
    }
}
string getcnic(string msg){
    while (true){
        cout<<msg;
        string raw;
        getline(cin,raw); 
		if (cin.eof()) {
   				 cout << "\a\n\n\t\t\t\t  Input cancelled. Returning to menu...\n";
				    cin.clear();
			    return "";
				}
        string digits="";
        for (int i=0; i<(int)raw.size(); i++){
            if (raw[i] != '-') digits += raw[i];
        }
        if ((int)digits.size() != 13){
            cout<<"\a\t\t\t\t\t\t\t\t  CNIC must have 13 digits (dashes optional).\n";
            continue;
        }
        bool alldigits=true;
        for (int i=0; i<(int)digits.size(); i++){
            if (!isdigit((unsigned char)digits[i])){ alldigits=false; break; }
        }
        if (!alldigits){
            cout<<"\a\t\t\t\t\t\t\t\t  CNIC must contain only numbers.\n";
            continue;
        }
           
        return digits;
    }
}

tm currenttime(){
    time_t t=time(0);
    tm* now=localtime(&t);
    return *now;
}
int currentday(){
 return currenttime().tm_mday; 
 }
int currentmonth(){
 return currenttime().tm_mon+1; 
 }
int currentyear(){
 return currenttime().tm_year+1900;
  }
string currentdatetime(){
    time_t now=time(0);
    tm* ltm=localtime(&now);
    char dt[50];
    sprintf(dt,"%02d/%02d/%04d %02d:%02d",
            ltm->tm_mday,ltm->tm_mon+1,ltm->tm_year+1900,
            ltm->tm_hour,ltm->tm_min);
    return string(dt);
}
int daysbetween(int d1, int m1, int y1, int d2, int m2, int y2) {
    tm start = {0}, end = {0};
    start.tm_mday = d1; start.tm_mon = m1-1; start.tm_year = y1-1900;
    end.tm_mday = d2; end.tm_mon = m2-1; end.tm_year = y2-1900;
    time_t ts = mktime(&start);
    time_t te = mktime(&end);
    if (ts == -1 || te == -1) {
        int result = (y2-y1)*365+(m2-m1)*30+(d2-d1);
        if (result<0) result = 0;
        return result;
    }
    int result = (int)(difftime(te, ts) / (60*60*24));
    if (result<0) result = 0;
    return result;
}

void initializedata(){
    for (int c=0; c<catn;c++){
        for (int i=0; i <carn; i++){
            avail[c][i]=true;
            status[c][i]="Available";
            renter[c][i]="";
            rentercnic[c][i]="";
            phone[c][i]="";
            address[c][i]="";
            booked[c][i]=0;
            sday[c][i]=0;
            smonth[c][i]=0;
            syear[c][i]=0;
            secpaid[c][i]=0;
            insured[c][i]=false;
            hasdriver[c][i]=false;
            rentdate[c][i]="";
            returndate[c][i]="";
            destination[c][i]="";
        }
    }
}
void savedata(){
    ofstream fout;
    fout.open("config.txt");
    if (fout.is_open()){
        fout<<driverrate<<"\n"<<fuelprice<<"\n";
        fout.close();
    }
    fout.open("finance.txt");
    if (fout.is_open()){
        fout<<totalrevenue<<"\n"<<totalfine<<"\n"<<totalbonus<<"\n"<<totalrepcost<<"\n";
        fout.close();
    }
    fout.open("maintenance.txt");
    if (fout.is_open()){
        fout<<maintcount<<"\n";
        for (int i=0; i<maintcount; i++){
            fout<<maintcar[i]<<"\n"<<maintreason[i]<<"\n"
             <<maintfixed[i]<<"\n"<<maintreportdate[i]<<"\n"
             <<maintrepairdate[i]<<"\n";
        }
        fout.close();
    }
    fout.open("cardata.txt");
    if (fout.is_open()) {
        for (int c = 0; c < catn; c++) {
            for (int i = 0; i < carn; i++) {
                fout << carname[c][i] << "\n" << carprice[c][i] << "\n";
            }
        }
        fout.close();
    }
    fout.open("history.txt");
    if (fout.is_open()){
        fout<<histcount<<"\n";
        for (int i=0; i<histcount; i++){
            fout<<histname[i]<<"\n"<<histcnic[i]<<"\n"
             <<histcar[i]<<"\n"<<histdate[i]<<"\n"
             <<histdest[i]<<"\n";
        }
        fout.close();
    }
    fout.open("cars.txt");
    if (fout.is_open()){
        for (int c=0; c<catn; c++){
            for (int i=0; i<carn; i++){
                fout<<avail[c][i]<<"\n"<<status[c][i]<<"\n";
            }
        }
        fout.close();
    }
    fout.open("customers.txt");
    if (fout.is_open()){
        for (int c=0; c<catn; c++){
            for (int i=0; i<carn; i++){
                fout<<renter[c][i]<<"\n"
                 <<rentercnic[c][i]<<"\n"
                 <<phone[c][i]<<"\n"
                 <<address[c][i]<<"\n"
                 <<booked[c][i]<<"\n"
                 <<insured[c][i]<<"\n"
                 <<hasdriver[c][i]<<"\n"
                 <<secpaid[c][i]<<"\n"
                 <<sday[c][i]<<"\n"
                 <<smonth[c][i]<<"\n"
                 <<syear[c][i]<<"\n"
                 <<rentdate[c][i]<<"\n"
                 <<returndate[c][i]<<"\n"
                 <<destination[c][i]<<"\n";
            }
        }
        fout.close();
    }
}
void loaddata(){
    ifstream fin;
    fin.open("config.txt");
	if (fin.is_open()){
	    if (fin >> driverrate >> fuelprice) {
	        if (driverrate < 0 || driverrate > 100000) {
	            driverrate = 2000;
	            cout << "Warning: Invalid driver rate. Using default.\n\a";
	        }
	        if (fuelprice < 0 || fuelprice > 100000) {
	            fuelprice = 299;
   	 	        cout << "Warning: Invalid fuel price. Using default.\n\a";
    	    }
	    } else {
	        driverrate = 2000;
	        fuelprice = 299;
	        cout << "Warning: Could not read config file. Using defaults.\n\a";
	    }
	    fin.close();
	}
    fin.open("finance.txt");
    if (fin.is_open()){
        fin >> totalrevenue >> totalfine>>totalbonus>>totalrepcost;
        fin.close();
    }
    fin.open("maintenance.txt");
    if (fin.is_open()){
        int cnt;
        fin >> cnt;
		if (cnt < 0) cnt = 0;  
        if (cnt > recn) cnt=recn;
        maintcount=cnt;
        fin.ignore();
        for (int i=0; i<cnt; i++){
            getline(fin,maintcar[i]);
            getline(fin,maintreason[i]);
            fin >> maintfixed[i];
            fin.ignore();
            getline(fin,maintreportdate[i]);
            getline(fin,maintrepairdate[i]);
        }
        fin.close();
    }
	fin.open("cardata.txt");
    if (fin.is_open()) {
        for (int c = 0; c < catn; c++) {
            for (int i = 0; i < carn; i++) {
                getline(fin, carname[c][i]);
                fin >> carprice[c][i];
                fin.ignore();
            }
        }
        fin.close();
    }
    
    fin.open("history.txt");
    if (fin.is_open()){
        int cnt;
        fin >> cnt;
        if (cnt < 0) cnt = 0;  
        if (cnt > recn) cnt=recn;
        histcount=cnt;
        fin.ignore();
        for (int i=0; i<cnt; i++){
            getline(fin,histname[i]);
            getline(fin,histcnic[i]);
            getline(fin,histcar[i]);
            getline(fin,histdate[i]);
            getline(fin,histdest[i]);
        }
        fin.close();
    }
    fin.open("cars.txt");
    if (fin.is_open()){
        for (int c=0; c<catn; c++){
            for (int i=0; i<carn; i++){
                fin >> avail[c][i];
                fin.ignore();
                getline(fin,status[c][i]);
            }
        }
        fin.close();
    }
    fin.open("customers.txt");
    if (fin.is_open()){
        for (int c=0; c<catn; c++){
            for (int i=0; i<carn; i++){
                getline(fin,renter[c][i]);
                getline(fin,rentercnic[c][i]);
                getline(fin,phone[c][i]);
                getline(fin,address[c][i]);
                fin >> booked[c][i]
                  >> insured[c][i]
                  >> hasdriver[c][i]
                  >> secpaid[c][i]
                  >> sday[c][i]
                  >> smonth[c][i]
                  >> syear[c][i];
                fin.ignore();
                getline(fin,rentdate[c][i]);
                getline(fin,returndate[c][i]);
                getline(fin,destination[c][i]);
            }
        }
        fin.close();
    }
}

bool findcust(string cnic,int &cat,int &id){
	
    for (int c=0; c<catn ;c++){
        for (int i=0; i<carn; i++){
            if (rentercnic[c][i]==cnic){
				cat=c;
				id=i;
                return true;
            }
        }
    }
    return false;
}
bool alreadyrented(string cnic){
    int cat,id;
    if (!findcust(cnic,cat,id)) return false;
    return !avail[cat][id];
}

void rentcar(long long credit=0,bool skip=false,string name="",string cell="",string cnic="",string add="",long long *excess=0){
	
	system("cls");
	cout<<"\n\n\n";
    cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|------------ CHOOSE CATEGORY -------------|\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    for (int i=0; i<catn; i++)
        cout<<"\t\t\t\t\t\t\t\t|  "<<i+1<<". "<<catname[i]<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    int temp=getint("\t\t\t\t\t\t\t\t|  Enter Category(1-5): ",1,5);
	if (temp==INT_MIN){system("pause"); return;}
		int cat=temp-1;
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|------------- AVAILABLE CARS -------------|\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|ID | Car Name       | Price   | Status    |\n";
    cout<<"\t\t\t\t\t\t\t\t|------------------------------------------|\n";
    for (int i=0; i<carn; i++){
    	if(avail[cat][i]){
        cout<<"\t\t\t\t\t\t\t\t| "<<i<<" | "<<carname[cat][i];
        int len=carname[cat][i].length();
        for (int p=len; p<15; p++) cout<<" ";
        cout<<"| Rs "<<carprice[cat][i];
        cout<<" | "<<status[cat][i]<<"\n";
		}
    }
    cout<<"\t\t\t\t\t\t\t\t|___|________________|_________|___________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    int id=getint("\t\t\t\t\t\t\t\t|  Enter car ID(0-4): ",0,carn-1);
	if (id==INT_MIN){
			system("pause"); 
		return;}
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    if (!avail[cat][id]){
        cout<<"\t\t\t\t\t\t\t\t|  This car is not available right now.    |\n";
		cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
        system("pause");
        return;
    }
    system("cls");
    cout<<"\n\n\n";
    if (!skip){
        cout<<"\n\n\n";
        cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
        cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
        cout<<"\t\t\t\t\t\t\t\t|============ CUSTOMER DETAILS ============|\n";
        cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
        cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
        name=gettext("\t\t\t\t\t\t\t\t| Cust Name    : ");
        if (name.empty()) { system("pause"); return;}
        cell=gettext("\t\t\t\t\t\t\t\t| Phone Number : ",true,11);
        if (cell.empty()) { system("pause"); return; }
        cnic=getcnic("\t\t\t\t\t\t\t\t| CNIC Number  : ");
        if (cnic.empty()) { system("pause"); return;}
        add=gettext("\t\t\t\t\t\t\t\t| Address      : ");
        if (add.empty()) { system("pause"); return; }
        if (alreadyrented(cnic)){
			cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
			cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
            cout<<"\t\t\t\t\t\t\t\t|   This CNIC already has a car rented.    |\n";
            cout<<"\t\t\t\t\t\t\t\t|         Please return it first.          |\n";    
			cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
            system("pause");
            return;
        }
    }
    int days=getint("\t\t\t\t\t\t\t\t| Num.of Days : ",1,365);
	if (days==INT_MIN){
			system("pause"); 
		return;}
   bool temp1=getint("\t\t\t\t\t\t\t\t|  Add Insurance?(1=Yes,0=No): ",0,1);
	if (temp1==INT_MIN){	system("pause"); return;}
	bool insu=temp1;
   bool temp2=getint("\t\t\t\t\t\t\t\t|  Need Driver?(1=Yes,0=No): ",0,1);
	if (temp2==INT_MIN){system("pause"); return;}
		bool driver=temp2;
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    system("cls");
    int price=carprice[cat][id];
    long long base=price*days;
    long long inscost=insu ? (base*0.30) : 0;
    long long drivercost=driver ? driverrate*days : 0;
    int sec=catsec[cat];
    long long total=base+inscost+drivercost+sec;
    if (credit>total){
    if (excess!=0) *excess=credit-total; 
    credit= total;
}
    long long pay=total-credit;
    if (pay<0) pay=0;

    system("cls");
    cout<<"\n\n\n\n\n\n";
    cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|================= INVOICE ================|\n";
    cout<<"\t\t\t\t\t\t\t\t|------------------------------------------|\n";
    cout<<"\t\t\t\t\t\t\t\t|  Customer    : "<<name<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  CNIC        : "<<cnic<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Phone       : "<<cell<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Address     : "<<add<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Vehicle     : "<<carname[cat][id]<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Insurance   : "<<(insu ? "Yes" : "No")<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Driver      : "<<(driver ? "Yes" : "No")<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Rent Date   : "<<currentdatetime()<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Destination : "<<( destination[cat][id].empty() ? "N/A" :  destination[cat][id])<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Days        : "<<days<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|------------------------------------------|\n";
    cout<<"\t\t\t\t\t\t\t\t|  Base Rent   : Rs "<<base<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Insurance   : Rs "<<inscost<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Driver      : Rs "<<drivercost<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Security    : Rs "<<sec<<"\n";
    if (credit > 0) cout<<"\t\t\t\t\t\t\t\t|  Credit      : -Rs "<<credit<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|------------------------------------------|\n";
    cout<<"\t\t\t\t\t\t\t\t|  TOTAL PAY   : Rs "<<pay<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|==========================================|\n";
    long long cash=getlarge("\t\t\t\t\t\t\t\t|  Cash Received: Rs ",pay,9999999999LL);
	    if (cash==INT_MIN){
			system("pause"); 
		return;}
    cout<<"\t\t\t\t\t\t\t\t|  Change      : Rs "<<(cash-pay)<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|------------------------------------------|\n";
    cout<<"\t\t\t\t\t\t\t\t|           Payment Successful!            |\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";

    avail[cat][id]=false;
    status[cat][id]="Rented";
    renter[cat][id]=name;
    rentercnic[cat][id]=cnic;
    phone[cat][id]=cell;
    address[cat][id]=add;
    booked[cat][id]=days;
    insured[cat][id]=insu;
    hasdriver[cat][id]=driver;
    secpaid[cat][id]=sec;
    sday[cat][id]=currentday();
    smonth[cat][id]=currentmonth();
    syear[cat][id]=currentyear();
    rentdate[cat][id]=currentdatetime();
    destination[cat][id]="N/A";
    totalrevenue += (base+inscost+drivercost);
    savedata();
    system("pause");
}

void freecar(int cat,int id){
    avail[cat][id]=true;
    status[cat][id]="Available";
    renter[cat][id]="";
    rentercnic[cat][id]="";
    phone[cat][id]="";
    address[cat][id]="";
    booked[cat][id]=0;
    insured[cat][id]=false;
    hasdriver[cat][id]=false;
    secpaid[cat][id]=0;
    returndate[cat][id]=currentdatetime();
}

void normal(int cat,int id,int useddays,int latedays,int dailydate,long long damage){
    long long latefine=(latedays*dailydate*0.20);
    int sec=secpaid[cat][id];
    long long refund=sec-damage-latefine;
    long long pay=(refund<0) ? -refund : 0;

    system("cls");
    cout<<"\n\n\n\n\n\n\n";
    cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|=============== RETURN BILL ==============|\n";
    cout<<"\t\t\t\t\t\t\t\t|------------------------------------------|\n";
    cout<<"\t\t\t\t\t\t\t\t|  Return Date : "<<currentdatetime()<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Used days   : "<<useddays<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Late days   : "<<latedays<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Security    : Rs "<<sec<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Late Fine   : Rs "<<latefine<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Damage Fine : Rs "<<damage<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|------------------------------------------|\n";
    if (refund >= 0)
        cout<<"\t\t\t\t\t\t\t\t|  REFUND      : Rs "<<refund<<"\n";
    else {
        cout<<"\t\t\t\t\t\t\t\t|  Cust Must pay : Rs "<<pay<<"\n";
        long long cash=getlarge("\t\t\t\t\t\t\t\t|  Cash Received: Rs ",pay,9999999999LL);
	    if (cash==INT_MIN){
			system("pause"); 
		return;}
        cout<<"\t\t\t\t\t\t\t\t|  Change      : Rs "<<(cash-pay)<<"\n";
    }
    cout<<"\t\t\t\t\t\t\t\t|==========================================|\n";
    cout<<"\t\t\t\t\t\t\t\t|       Return Processed Successfully!     |\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";

    if (histcount<recn){
        histname[histcount]=renter[cat][id];
        histcnic[histcount]=rentercnic[cat][id];
        histcar[histcount]=carname[cat][id];
        histdate[histcount]=currentdatetime();
        histdest[histcount]=destination[cat][id];
        histcount++;
    } else {
        cout<<"\t\t\t\t\t\t\t\t|  History log is full. Cannot add record.\n";
    }

    totalfine += (damage+latefine);
    freecar(cat,id);
    savedata();
    system("pause");
}

void noneed(int cat,int id,int remdays,int dailydate,long long damage){
    int sec=secpaid[cat][id];
    long long refund=remdays*dailydate+sec-damage;
    totalrevenue -= remdays*dailydate;
    if (totalrevenue<0) totalrevenue=0;
    totalfine += damage;
    long long pay=(refund<0) ? -refund : 0;
system("cls");
cout<<"\n\n\n\n\n";
	cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
	cout<<"\t\t\t\t\t\t\t\t|---------------- RETURN BILL -------------|\n";
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|  Remaining days : "<<remdays<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Damage Fine : Rs "<<damage<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Refund Amount  : Rs "<<refund<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|------------------------------------------|\n";
    if (refund >= 0)
        cout<<"\t\t\t\t\t\t\t\t|  REFUND      : Rs "<<refund<<"\n";
    else {
        cout<<"\t\t\t\t\t\t\t\t|  Cust Must pay : Rs "<<pay<<"\n";
        long long cash=getlarge("\t\t\t\t\t\t\t\t|  Cash Received: Rs ",pay,9999999999LL);
	    if (cash==INT_MIN){
			system("pause"); 
		return;}
        cout<<"\t\t\t\t\t\t\t\t|  Change      : Rs "<<(cash-pay)<<"\n";
    }
    cout<<"\t\t\t\t\t\t\t\t|==========================================|\n";
    cout<<"\t\t\t\t\t\t\t\t|   Early Return Processed Successfully!   |\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";

    if (histcount<recn){
        histname[histcount]=renter[cat][id];
        histcnic[histcount]=rentercnic[cat][id];
        histcar[histcount]=carname[cat][id];
        histdate[histcount]=currentdatetime();
        histdest[histcount]=destination[cat][id];
        histcount++;
    } else {
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|  History log is full. Cannot add record. |\n";
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    }

    freecar(cat,id);
    savedata();
    system("pause");
}

void issue(int cat,int id,int remdays,int dailydate,long long damage){
    if (maintcount >= recn){
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|    Maintenance log is full. Cannot add!  |\n";
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
        system("pause");
        return;
    }
    if (status[cat][id] == "Under Maintenance"){
        cout<<"\t\t\t\t\t\t\t\t|  This car is already under maintenance.\n";
        system("pause");
        return;
    }

	cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|============== SELECT ISSUE ==============|\n";
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|  1. Engine Problem                       |\n";
    cout<<"\t\t\t\t\t\t\t\t|  2. AC Problem                           |\n";
    cout<<"\t\t\t\t\t\t\t\t|  3. Brake Problem                        |\n";
    cout<<"\t\t\t\t\t\t\t\t|  4. Tyre Problem                         |\n";
    cout<<"\t\t\t\t\t\t\t\t|  5. Other                                |\n";
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    int issue=getint("\t\t\t\t\t\t\t\t|  Select Issue: ",1,5);
	    if (issue==INT_MIN){
			system("pause"); 
		return;}
    string reason;
    switch (issue){
        case 1: reason="Engine Problem"; break;
        case 2: reason="AC Problem"; break;
        case 3: reason="Brake Problem"; break;
        case 4: reason="Tyre Problem"; break;
        case 5: reason = gettext("\t\t\t\t\t\t\t\t|  Enter Custom Reason: ");
    }
    long long baseref=remdays*dailydate;
    long long bonus=(baseref*0.25);
    int sec=secpaid[cat][id];
    long long totalref=baseref+bonus+sec-damage;
    long long pay=(totalref<0) ? -totalref : 0;
    totalrevenue -= baseref;
    totalbonus+=bonus;
    totalfine += damage;
    if (totalrevenue<0) totalrevenue=0;

	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
	cout<<"\t\t\t\t\t\t\t\t|------------- COMPENSATION BILL ----------|\n";
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|  Compensation Bonus(+25%) : Rs "<<bonus<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Damage Fine            : Rs "<<damage<<"\n";
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	if (totalref>= 0){
    cout<<"\t\t\t\t\t\t\t\t|  Total Refund           : Rs "<<totalref<<"\n";
	}
    else {
        cout<<"\t\t\t\t\t\t\t\t|  Cust Must pay : Rs "<<pay<<"\n";
        long long cash=getlarge("\t\t\t\t\t\t\t\t|  Cash Received: Rs ",pay,9999999999LL);
	    if (cash==INT_MIN){
			system("pause"); 
		return;}
        cout<<"\t\t\t\t\t\t\t\t|  Change      : Rs "<<(cash-pay)<<"\n";
    }
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|  Car sent to Maintenance. Refund issued. |\n";
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";

    maintcar[maintcount]=carname[cat][id];
    maintreason[maintcount]=reason;
    maintreportdate[maintcount]=currentdatetime();
    maintfixed[maintcount]=false;
    maintrepairdate[maintcount]="";
    maintcount++;

    if (histcount<recn){
        histname[histcount]=renter[cat][id];
        histcnic[histcount]=rentercnic[cat][id];
        histcar[histcount]=carname[cat][id];
        histdate[histcount]=currentdatetime();
        histdest[histcount]=destination[cat][id];
        histcount++;
    } else {
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|  History log is full. Cannot add record. |\n";
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    }

    avail[cat][id]=false;
    status[cat][id]="Under Maintenance";
    renter[cat][id]="";
    rentercnic[cat][id]="";
    phone[cat][id]="";
    address[cat][id]="";
    booked[cat][id]=0;
    insured[cat][id]=false;
    hasdriver[cat][id]=false;
    secpaid[cat][id]=0;
    returndate[cat][id]=currentdatetime();

    savedata();
    system("pause");
}

void upgrade(int cat,int id,int remdays,int dailydate,long long damage,string name,string cell,string cnic,string add){
    int sec=secpaid[cat][id];
    long long credit=remdays*dailydate+sec-damage;
    long long pay=(credit<0) ? -credit : 0;

    system("cls");
    cout<<"\n\n\n\n\n\n\n";
	cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|============= UPGRADE SUMMARY ============|\n";
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|  Security    : Rs "<<sec<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Remaining Days : "<<remdays<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Damage Fine : Rs "<<damage<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|------------------------------------------|\n";
    if (credit >= 0)
        cout<<"\t\t\t\t\t\t\t\t|  CREDIT      : Rs "<<credit<<"\n";
    else {
        cout<<"\t\t\t\t\t\t\t\t|  Cust must pay : Rs "<<pay<<"\n";
        long long cash=getlarge("\t\t\t\t\t\t\t\t|  Cash Received: Rs ",pay,9999999999LL);
	    if (cash==INT_MIN){
			system("pause"); 
		return;}
        cout<<"\t\t\t\t\t\t\t\t|  Change      : Rs "<<(cash-pay)<<"\n";
        credit=0;
    }
    cout<<"\t\t\t\t\t\t\t\t|==========================================|\n";
    totalrevenue -= (remdays * dailydate);   
    totalfine += damage;

    if (histcount<recn){
        histname[histcount]=renter[cat][id];
        histcnic[histcount]=rentercnic[cat][id];
        histcar[histcount]=carname[cat][id];
        histdate[histcount]=currentdatetime();
        histdest[histcount]=destination[cat][id];
        histcount++;
    } else {
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|  History log is full. Cannot add record. |\n";
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    }

    freecar(cat,id);
    savedata();

    cout<<"\n\t\t\t\t     Redirecting to rent a new car with your credit...\n";
    system("pause");
    long long leftover=0;
	rentcar(credit, true, name, cell, cnic, add, &leftover);

	if (leftover > 0) {
    cout << "\n\t\t\t\t  You have extra credit of Rs " << leftover << ".\n";
    cout << "\t\t\t\t  This amount is refunded to you in cash.\n";
    cout << "\t\t\t\t  Please collect your refund at the counter.\n";
    	system("pause");
}
}

void returncar(){
    system("cls");
    cout<<"\n\n\n";
	cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|============== RETURN A CAR ==============|\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    string cnic=getcnic("\t\t\t\t\t\t\t\t|  Enter Cust CNIC: ");
    int cat,id;
    if (!findcust(cnic,cat,id)){
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|     No customer found with that CNIC.    |\n";
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
        system("pause");
        return;
}
    int sd=sday[cat][id],sm=smonth[cat][id],sy=syear[cat][id];
    int book=booked[cat][id];
    int dailydate=carprice[cat][id];
    string custname=renter[cat][id],carnamestr=carname[cat][id];
    int used=daysbetween(sd,sm,sy,currentday(),currentmonth(),currentyear());
    int rem=(book > used) ? (book-used) : 0;
    int late=(used > book) ? (used-book) : 0;
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|-------------- RENTAL SUMMARY ------------|\n";
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|  Customer : "<<custname<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Car      : "<<carnamestr<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Booked   : "<<book<<" days\n";
    cout<<"\t\t\t\t\t\t\t\t|  Used     : "<<used<<" days\n";
    cout<<"\t\t\t\t\t\t\t\t|  Remaining: "<<rem<<" days\n";
    cout<<"\t\t\t\t\t\t\t\t|  Late     : "<<late<<" days\n";
    long long damage=getlarge("\t\t\t\t\t\t\t\t|  Damage cost(0 if none): Rs ",0,100000000);
	if (damage==INT_MIN){system("pause"); return;}
    if (insured[cat][id]) damage=(damage*0.25);
    if (rem > 0){
		system("cls");
		cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
		cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
        cout<<"\t\t\t\t\t\t\t\t|----------- EARLY RETURN REASON ----------|\n";
		cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
		cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
        cout<<"\t\t\t\t\t\t\t\t|  1. No Longer Needed                     |\n";
        cout<<"\t\t\t\t\t\t\t\t|  2. Car has a Problem                    |\n";
        cout<<"\t\t\t\t\t\t\t\t|  3. Upgrade to a Different car           |\n";
		cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
		cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
        int reason=getint("\t\t\t\t\t\t\t\t|  Select: ",1,3);
	    if (reason==INT_MIN){
			system("pause"); 
		return;}
        if (reason==1)
            noneed(cat,id,rem,dailydate,damage);
        else if(reason==2)
            issue(cat,id,rem,dailydate,damage);
        else if (reason==3){
            string custname2=renter[cat][id];
            string custcnic2=rentercnic[cat][id];
            string custphone2=phone[cat][id];
            string custaddr2=address[cat][id];
            upgrade(cat,id,rem,dailydate,damage,custname2,custphone2,custcnic2,custaddr2);
        }
    } else {
        normal(cat,id,used,late,dailydate,damage);
    }
}

void fareestimate(){
    system("cls");
    cout<<"\n\n\n";
	cout<<"\t\t\t\t\t\t\t\t __________________________________________ \n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|============== FARE ESTIMATOR ============|\n";
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|  Categories and price ranges(per day):   |\n";
    for (int i=0; i<catn; i++){
        cout<<"\t\t\t\t\t\t\t\t|  "<<i+1<<". "<<catname[i]
            <<":  Rs:"<<carprice[i][0]<<" ---> Rs:"<<carprice[i][carn-1]<<"\n";
    }
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
        int temp=getint("\t\t\t\t\t\t\t\t|  Enter Category(1-5): ",1,5);
	if (temp==INT_MIN){system("pause"); return;}
		int cat=temp-1;
    int days=getint("\t\t\t\t\t\t\t\t|  Number of days : ",1,365);
	if (days==INT_MIN){
			system("pause"); 
		return;}
    bool temp1=getint("\t\t\t\t\t\t\t\t|  Add Insurance?(1=Yes,0=No): ",0,1);
	if (temp1==INT_MIN){system("pause"); return;}
	bool insu=temp1;
    bool temp2=getint("\t\t\t\t\t\t\t\t|  Need Driver?(1=Yes,0=No): ",0,1);
	if (temp2==INT_MIN){system("pause"); return;}
		bool driver=temp2;
system("cls");
	cout<<"\n\n\n\n\t\t\t\t\t\t\t\t __________________________________________ \n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|-------------- ESTIMATED BILL ------------|\n";
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    int price=carprice[cat][2];
    long long base=price*days;
    long long inscost=insu ? (base*0.30) : 0;
    long long drivercost=driver ? driverrate*days : 0;
    int sec=catsec[cat];
    long long total=base+inscost+drivercost+sec;
    cout<<"\t\t\t\t\t\t\t\t|  Base Rent    : Rs "<<base<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Insurance    : Rs "<<inscost<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Driver       : Rs "<<drivercost<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Security     : Rs "<<sec<<"\n";
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|  ESTIMATED    : Rs "<<total<<"\n";
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|--- Actual price depends on exact car ----|\n";
    cout<<"\t\t\t\t\t\t\t\t|Security deposit is refundable upon return|\n";
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    system("pause");
}

void tripplanner(){
    system("cls");
    cout<<"\n\n\n\n";
    cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|=============== TRIP PLANNER =============|\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    for (int i=0; i<destn; i++)
        cout<<"\t\t\t\t\t\t\t\t|  "<<i+1<<". "<<destname[i]<<" ("<<destdist[i]<<" km,toll: Rs "<<desttoll[i]<<")\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    int dest=getint("\t\t\t\t\t\t\t\t|  Select Destination: ",1,10)-1;
	if (dest==INT_MIN){
			system("pause"); 
		return;}
    int people=getint("\t\t\t\t\t\t\t\t|  Number of Passengers: ",1,12);
	if (people==INT_MIN){
			system("pause"); 
		return;}
		
    int days=getint("\t\t\t\t\t\t\t\t|  Number of Days: ",1,365);
	if (days==INT_MIN){system("pause"); return;}
 bool temp2=getint("\t\t\t\t\t\t\t\t|  Need Driver?(1=Yes,0=No): ",0,1);
	if (temp2==INT_MIN){system("pause"); return;}
		bool driver=temp2;
    bool temp1=getint("\t\t\t\t\t\t\t\t|  Add Insurance?(1=Yes,0=No): ",0,1);
	if (temp1==INT_MIN){system("pause"); 	return;}
	bool insu=temp1;
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|-------------- Comfort Level -------------|\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|  1. Economy                              |\n";
    cout<<"\t\t\t\t\t\t\t\t|  2. Standard                             |\n";
    cout<<"\t\t\t\t\t\t\t\t|  3. Premium                              |\n";
    cout<<"\t\t\t\t\t\t\t\t|  4. Luxury                               |\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    int comfort=getint("\t\t\t\t\t\t\t\t|  Select : ",1,4);
	    if (comfort==INT_MIN){
			system("pause"); 
		return;}

    int avgkm=0,priceperday=0,cat;
    if (comfort == 1){
        if (people <= 2){ avgkm=18; priceperday=carprice[0][2]; cat=0; }
        else if (people <= 4){ avgkm=14; priceperday=carprice[1][2];cat=1; }
        else if (people <= 7){ avgkm=10; priceperday=carprice[2][2];cat=2;}
        else if (people <= 12){ avgkm=9; priceperday=carprice[4][2];cat=4;}
        }
		 else if (comfort == 2){
        if (people <= 4){ avgkm=14; priceperday=carprice[1][2];cat=1;}
        else if (people <= 7){ avgkm=10; priceperday=carprice[2][2];cat=2;}
        else if (people <= 12){ avgkm=9; priceperday=carprice[4][2];cat=4;}
    } else if (comfort == 3){
        if (people <= 4){ avgkm=10; priceperday=carprice[2][2];cat=2;}
        else if (people <= 7){ avgkm=9; priceperday=carprice[3][2]; cat=3;}
        else if (people <= 12){ avgkm=9; priceperday=carprice[4][2];cat=4;}
    } else {
        if (people <= 4){ avgkm=10; priceperday=carprice[3][2];cat=3;}
        else if (people <= 12){ avgkm=9; priceperday=carprice[4][2];cat=4;}
    }

    if (insu) priceperday=(int)(priceperday*1.30);
    long long rentcost=priceperday*days;
    long long drivercost=driver ? driverrate*days : 0;
    long long fuelcost=(destdist[dest] / avgkm)*fuelprice;
    long long tollcost=desttoll[dest];
    long long total=rentcost+drivercost+fuelcost+tollcost+catsec[cat];

    system("cls");
    cout<<"\n\n\n\n";
    cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|=========== TRIP COST ESTIMATE ===========|\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|  Destination : "<<destname[dest]<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Distance    : "<<destdist[dest]<<" km\n";
    cout<<"\t\t\t\t\t\t\t\t|  Days        : "<<days<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Passengers  : "<<people<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Suggested   : "<<catname[cat]<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Insurance   : "<<(insu ? "Yes" : "No")<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Rental      : Rs "<<rentcost<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Fuel        : Rs "<<fuelcost<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Toll        : Rs "<<tollcost<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Driver      : Rs "<<drivercost<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|  TOTAL       : Rs "<<total<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
	cout<<"\t\t\t\t\t\t\t\t|      Estimate based on a typical car     |\n";
	cout<<"\t\t\t\t\t\t\t\t|     Exact price depends on chosen car    |\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    int proceed=getint("\t\t\t\t\t\t\t\t|  Book now?(1=Yes,0=No): ",0,1);
	    if (proceed==INT_MIN){
			system("pause"); 
		return;}
    if (!proceed) return;
    
	if (cat == -1){ cout<<"\t\t\t\t\t\t\t\t|  Error: unknown category.\n"; system("pause"); return; }

    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|------------- AVAILABLE CARS -------------|\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|ID | Car Name       | Price   |  Status   |\n";
    cout<<"\t\t\t\t\t\t\t\t|------------------------------------------|\n";
    for (int i=0; i<carn; i++){
    	if(avail[cat][i]){
        cout<<"\t\t\t\t\t\t\t\t| "<<i<<" | "<<carname[cat][i];
        int len=carname[cat][i].length();
        for (int p=len; p<15; p++) cout<<" ";
		cout<<"| Rs "<<carprice[cat][i];
        cout<<" | "<<status[cat][i]<<"\n";
    }
}
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    int id=getint("\t\t\t\t\t\t\t\t|  Enter CAR ID(0-4): ",0,carn-1);
	    if (id==INT_MIN){
			system("pause"); 
		return;}
    if (!avail[cat][id]){
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
        cout<<"\t\t\t\t\t\t\t\t|             Car not available.           |\n";
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
        system("pause");
        return;
    }
    system("cls");
    cout<<"\n\n\n\n\n\n\n";
	cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|========== TRIP BOOKING DETAILS ==========|\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    string name=gettext("\t\t\t\t\t\t\t\t| Cust Name    : ");
        if (name.empty()) { system("pause"); return;}
    string cell=gettext("\t\t\t\t\t\t\t\t| Phone Number : ",true,11);
    if (cell.empty()) { system("pause"); return; }
    string cnic=getcnic("\t\t\t\t\t\t\t\t| CNIC Number  : ");
        if (cnic.empty()) { system("pause"); return;}
    string add=gettext("\t\t\t\t\t\t\t\t| Address      : ");
    if (add.empty()) { system("pause"); return; }
    if (alreadyrented(cnic)){
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
        cout<<"\t\t\t\t\t\t\t\t|   This CNIC already has a car rented.    |\n";
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
        system("pause");
        return;
    }
    int price=carprice[cat][id];
    long long base=price*days;
    long long inscost=insu ? (base*0.30) : 0;
    int sec=catsec[cat];
    long long finaltotal=base+inscost+drivercost+sec+fuelcost+tollcost;

    system("cls");
    cout<<"\n\n\n\n\n\n\n";
    cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|============ TRIP INVOICE ================|\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|  Customer    : "<<name<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  CNIC        : "<<cnic<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Destination : "<<destname[dest]<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Vehicle     : "<<carname[cat][id]<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Days        : "<<days<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Rental      : Rs "<<base<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Insurance   : Rs "<<inscost<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Driver      : Rs "<<drivercost<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Fuel        : Rs "<<fuelcost<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Toll        : Rs "<<tollcost<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Security    : Rs "<<sec<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|  TOTAL       : Rs "<<finaltotal<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    long long cash=getlarge("\t\t\t\t\t\t\t\t|  Cash Received: Rs ",finaltotal,9999999999LL);
	if (cash==INT_MIN){system("pause"); return;}
    cout<<"\t\t\t\t\t\t\t\t|  Change      : Rs "<<(cash-finaltotal)<<"\n";
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
	    cout<<"\t\t\t\t\t\t\t\t|   Booking Confirmed! Have a great trip!  |\n";
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";

    avail[cat][id]=false;
    status[cat][id]="Rented";
    renter[cat][id]=name;
    rentercnic[cat][id]=cnic;
    phone[cat][id]=cell;
    address[cat][id]=add;
    booked[cat][id]=days;
    insured[cat][id]=insu;
    hasdriver[cat][id]=driver;
    secpaid[cat][id]=sec;
    sday[cat][id]=currentday();
    smonth[cat][id]=currentmonth();
    syear[cat][id]=currentyear();
    rentdate[cat][id]=currentdatetime();
    destination[cat][id]=destname[dest];

    totalrevenue += (base+inscost+drivercost);
    savedata();
    system("pause");
}

void extend(){
    system("cls");
    cout<<"\n\n\n";
    cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|============= EXTEND RENTAL ==============|\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    string cnic=getcnic("\t\t\t\t\t\t\t\t| Enter Cust CNIC: ");
    int cat, id;
    if (!findcust(cnic, cat, id)) {
        cout<<"\t\t\t\t\t\t\t\t|  No active rental found for this CNIC.\n";
        system("pause");
        return;
    }
    if (avail[cat][id]) {
        cout<<"\t\t\t\t\t\t\t\t|  This car is not currently rented.\n";
        system("pause");
        return;
    }
    int sd=sday[cat][id], sm=smonth[cat][id], sy=syear[cat][id];
    int book=booked[cat][id];
    int used=daysbetween(sd, sm, sy, currentday(), currentmonth(), currentyear());
    int remaining=book - used;
    if (remaining < 0) remaining=0;

    cout<<"\t\t\t\t\t\t\t\t|  Car : "<<carname[cat][id]<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Booked : "<<book<<" days\n";
    cout<<"\t\t\t\t\t\t\t\t|  Used   : "<<used<<" days\n";
    cout<<"\t\t\t\t\t\t\t\t|  Remaining: "<<remaining<<" days\n";
    cout<<"\t\t\t\t\t\t\t\t|------------------------------------------|\n";
    int extradays=getint("\t\t\t\t\t\t\t\t|  Additional days: ", 1, 365);
    if (extradays == INT_MIN) { system("pause"); return; }

    int dailydate=carprice[cat][id];
    long long extrarent=dailydate * extradays;
    long long extrainsu=insured[cat][id] ?(extrarent * 0.30) : 0;
    long long extradriver=hasdriver[cat][id] ?driverrate * extradays : 0;
    long long extratotal=extrarent+extrainsu+extradriver;
    
    cout<<"\t\t\t\t\t\t\t\t|  Extra Rent   : Rs "<<extrarent<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Extra Ins.   : Rs "<<extrainsu<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Extra Driver : Rs "<<extradriver<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  TOTAL EXTRA  : Rs "<<extratotal<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|------------------------------------------|\n";
    long long cash=getlarge("\t\t\t\t\t\t\t\t|  Cash Received: Rs ", extratotal, 9999999999LL);
    if (cash == INT_MIN) { system("pause"); return; }
    cout<<"\t\t\t\t\t\t\t\t|  Change: Rs "<<(cash - extratotal)<<"\n";
    
    booked[cat][id] += extradays;
    totalrevenue += extratotal;
    savedata();

    cout<<"\t\t\t\t\t\t\t\t|  Rental extended successfully!\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    system("pause");
}

void custhistory() {
    system("cls");
    cout<<"\n\n\n";
    cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|=========== MY RENTAL HISTORY ============|\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    string cnic=getcnic("\t\t\t\t\t\t\t\t| Enter your CNIC: ");
    int found=0;
    for (int i=0; i<histcount; i++) {
        if (histcnic[i] == cnic) {
            found++;
            cout<<"\t\t\t\t\t\t\t\t|  #"<<found<<"\n";
            cout<<"\t\t\t\t\t\t\t\t|  Car      : "<<histcar[i]<<"\n";
            cout<<"\t\t\t\t\t\t\t\t|  Date     : "<<histdate[i]<<"\n";
            cout<<"\t\t\t\t\t\t\t\t|  Dest     : "<<(histdest[i].empty() ? "N/A" : histdest[i])<<"\n";
            cout<<"\t\t\t\t\t\t\t\t|------------------------------------------|\n";
        }
    }
    if (found == 0) {
        cout<<"\t\t\t\t\t\t\t\t|  No rental history found for this CNIC.\n";
    } else {
        cout<<"\t\t\t\t\t\t\t\t|  Total rentals: "<<found<<"\n";
    }
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    system("pause");
}

void customerpanel(){
    bool run=true;
    while (run){
        system("cls");
        cout<<"\n\n\n";
	    cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
		cout<<"\t\t\t\t\t\t\t\t|============== CUSTOMER MENU =============|\n";
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
        cout<<"\t\t\t\t\t\t\t\t|  1. Rent a car                           |\n";
        cout<<"\t\t\t\t\t\t\t\t|  2. Return a car                         |\n";
        cout<<"\t\t\t\t\t\t\t\t|  3. Fare Estimate                        |\n";
        cout<<"\t\t\t\t\t\t\t\t|  4. Plan a Trip                          |\n";
        cout<<"\t\t\t\t\t\t\t\t|  5. Extend Rental                        |\n";
        cout<<"\t\t\t\t\t\t\t\t|  6. Customer History                     |\n";
        cout<<"\t\t\t\t\t\t\t\t|  7. Back to Main Menu                    |\n";
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
        int ch=getint("\t\t\t\t\t\t\t\t|  Enter your choice: ",1,7);
        if (ch == INT_MIN) { system("pause"); return; }
        if (ch == 1) rentcar();
        else if (ch == 2) returncar();
        else if (ch == 3) fareestimate();
        else if (ch == 4) tripplanner();
        else if (ch == 5) extend();
        else if (ch == 6) custhistory();
        else run=false;
    }
}

void viewrented(){
    system("cls");
    cout<<"\n\n\n\n";
    cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|========== CURRENTLY RENTED CARS =========|\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
	cout<<"\t\t\t\t\t\t\t\t|  Filter by:                              |\n";
    cout<<"\t\t\t\t\t\t\t\t|  1. Show all                             |\n";
    cout<<"\t\t\t\t\t\t\t\t|  2. Search by Car Name                   |\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    int choice = getint("\t\t\t\t\t\t\t\t|  Enter choice: ", 1, 2);
    if (choice == INT_MIN) { 
		system("pause");
		 return; }
    bool any=false;
	if(choice==1){
    for (int c=0; c<catn; c++){
        for (int i=0; i<carn; i++){
            if (!avail[c][i] && status[c][i] == "Rented"){
                any=true;
                cout<<"\t\t\t\t\t\t\t\t|  Customer : "<<renter[c][i]<<"\n";
                cout<<"\t\t\t\t\t\t\t\t|  CNIC     : "<<rentercnic[c][i]<<"\n";
                cout<<"\t\t\t\t\t\t\t\t|  Car      : "<<carname[c][i]<<" ("<<catname[c]<<")\n";
                cout<<"\t\t\t\t\t\t\t\t|  Rent Date: "<<rentdate[c][i]<<"\n";
                cout<<"\t\t\t\t\t\t\t\t|  Days     : "<<booked[c][i]<<"\n";
                cout<<"\t\t\t\t\t\t\t\t|  Destination : " << destination[c][i] << "\n";
                cout<<"\t\t\t\t\t\t\t\t|  Driver   : "<<(hasdriver[c][i] ? "Yes" : "No")<<"\n";
                cout<<"\t\t\t\t\t\t\t\t|  Insurance: "<<(insured[c][i] ? "Yes" : "No")<<"\n";
                cout<<"\t\t\t\t\t\t\t\t|------------------------------------------|\n";
            }
			}
        }
    }
else if(choice == 2){
	any=true;
    	system("cls");
	cout<<"\n\n\n";
    cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|------------ CHOOSE CATEGORY -------------|\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    for (int i=0; i<catn; i++)
        cout<<"\t\t\t\t\t\t\t\t|  "<<i+1<<". "<<catname[i]<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    int c=getint("\t\t\t\t\t\t\t\t|  Enter Category(1-5): ",1,5)-1;
	if (c==INT_MIN){
			system("pause"); 
		return;}
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|------------- AVAILABLE CARS -------------|\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|ID | Car Name       | Price   | Status    |\n";
    cout<<"\t\t\t\t\t\t\t\t|------------------------------------------|\n";
    for (int i=0; i<carn; i++){
        cout<<"\t\t\t\t\t\t\t\t| "<<i<<" | "<<carname[c][i];
        int len=carname[c][i].length();
        for (int p=len; p<15; p++) cout<<" ";
        cout<<"| Rs "<<carprice[c][i];
        cout<<" | "<<status[c][i]<<"\n";
    }
    cout<<"\t\t\t\t\t\t\t\t|___|________________|_________|___________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    int i=getint("\t\t\t\t\t\t\t\t|  Enter car ID(0-4): ",0,carn-1);
	if (i==INT_MIN){
			system("pause"); 
		return;}
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    if (!avail[c][i]){
                cout<<"\t\t\t\t\t\t\t\t|  Customer : "<<renter[c][i]<<"\n";
                cout<<"\t\t\t\t\t\t\t\t|  CNIC     : "<<rentercnic[c][i]<<"\n";
                cout<<"\t\t\t\t\t\t\t\t|  Car      : "<<carname[c][i]<<" ("<<catname[c]<<")\n";
                cout<<"\t\t\t\t\t\t\t\t|  Rent Date: "<<rentdate[c][i]<<"\n";
                cout<<"\t\t\t\t\t\t\t\t|  Days     : "<<booked[c][i]<<"\n";
                cout<<"\t\t\t\t\t\t\t\t|  Destination : " << destination[c][i] << "\n";
                cout<<"\t\t\t\t\t\t\t\t|  Driver   : "<<(hasdriver[c][i] ? "Yes" : "No")<<"\n";
                cout<<"\t\t\t\t\t\t\t\t|  Insurance: "<<(insured[c][i] ? "Yes" : "No")<<"\n";
                cout<<"\t\t\t\t\t\t\t\t|------------------------------------------|\n";
            }else if(avail[c][i]){
            	cout<<"\t\t\t\t\t\t\t\t|      This car is Available right now     |\n";
				cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
        		system("pause");
        	return;	
			}
}
    if(!any) cout<<"\t\t\t\t\t\t\t\t|             No cars currently rented.\n";
    system("pause");
}

void viewcust(){
    system("cls");
    cout<<"\n\n\n\n";
    cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|============ CUSTOMER RECORDS ============|\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
	cout<<"\t\t\t\t\t\t\t\t|  Filter by:                              |\n";
    cout<<"\t\t\t\t\t\t\t\t|  1. Show all                             |\n";
    cout<<"\t\t\t\t\t\t\t\t|  2. Search by CNIC                       |\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    int choice = getint("\t\t\t\t\t\t\t\t|  Enter choice: ", 1, 2);
    if (choice == INT_MIN) { 
		system("pause");
		 return; }
	
    bool any=false;
    if(choice==1){
	for (int c=0; c<catn; c++){
        for (int i=0; i<carn; i++){
            if (!avail[c][i] && status[c][i] == "Rented"){
                any=true;
                cout<<"\t\t\t\t\t\t\t\t|  Name : "<<renter[c][i]<<"\n";
                cout<<"\t\t\t\t\t\t\t\t|  Phone : " << phone[c][i] << "\n";
                cout<<"\t\t\t\t\t\t\t\t|  CNIC : "<<rentercnic[c][i]<<"\n";
                cout<<"\t\t\t\t\t\t\t\t|  Address : " <<address[c][i] << "\n";
                cout<<"\t\t\t\t\t\t\t\t|  Car  : "<<carname[c][i]<<" ("<<catname[c]<<")\n";
                cout<<"\t\t\t\t\t\t\t\t|  Status: "<<status[c][i]<<"\n";
                cout<<"\t\t\t\t\t\t\t\t|  Destination : " << destination[c][i] << "\n"; 
				cout<<"\t\t\t\t\t\t\t\t|  Rent Date : "<<rentdate[c][i]<<"\n";
                cout<<"\t\t\t\t\t\t\t\t|  Days      : "<<booked[c][i]<<"\n";
                cout<<"\t\t\t\t\t\t\t\t|  Driver    : "<<(hasdriver[c][i] ? "Yes" : "No")<<"\n";
                cout<<"\t\t\t\t\t\t\t\t|  Insurance : "<<(insured[c][i] ? "Yes" : "No")<<"\n";
                cout<<"\t\t\t\t\t\t\t\t|------------------------------------------|\n";
	}
            }
        }
    }
	else if(choice==2){
		cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
   		 string cnic=getcnic("\t\t\t\t\t\t\t\t|  Enter Cust CNIC: ");
    	int cat,id;
    if (findcust(cnic,cat,id)){
                any=true;    	
		    	cout<<"\t\t\t\t\t\t\t\t|  Name : "<<renter[cat][id]<<"\n";
                cout<<"\t\t\t\t\t\t\t\t|  Phone : " << phone[cat][id] << "\n";
                cout<<"\t\t\t\t\t\t\t\t|  CNIC : "<<rentercnic[cat][id]<<"\n";
                cout<<"\t\t\t\t\t\t\t\t|  Address : " <<address[cat][id] << "\n";
                cout<<"\t\t\t\t\t\t\t\t|  Car  : "<<carname[cat][id]<<" ("<<catname[cat]<<")\n";
                cout<<"\t\t\t\t\t\t\t\t|  Status: "<<status[cat][id]<<"\n";
                cout<<"\t\t\t\t\t\t\t\t|  Destination : " << destination[cat][id] << "\n"; 
				cout<<"\t\t\t\t\t\t\t\t|  Rent Date : "<<rentdate[cat][id]<<"\n";
                cout<<"\t\t\t\t\t\t\t\t|  Days      : "<<booked[cat][id]<<"\n";
                cout<<"\t\t\t\t\t\t\t\t|  Driver    : "<<(hasdriver[cat][id] ? "Yes" : "No")<<"\n";
                cout<<"\t\t\t\t\t\t\t\t|  Insurance : "<<(insured[cat][id] ? "Yes" : "No")<<"\n";
                cout<<"\t\t\t\t\t\t\t\t|------------------------------------------|\n";
	
    	
	}
	}
    
    
    if (!any) cout<<"\t\t\t\t\t\t\t\t|            No customer records found.\n";
    system("pause");
}

void financialreport(){
    system("cls");
    cout<<"\n\n\n\n\n\n\n";
    cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|============ FINANCIAL REPORT ============|\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|  Total Revenue    : Rs "<<totalrevenue<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Total Fines      : Rs "<<totalfine<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Total Bonus     : -Rs "<<totalbonus<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Maintenance Cases: "<<maintcount<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Maintenance Cost : -Rs "<<totalrepcost<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Completed Rentals: "<<histcount<<"\n";
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|  GRAND TOTAL      : Rs "<<(totalrevenue+totalfine-totalbonus-totalrepcost)<<"\n";
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    system("pause");
}

void maintpanel(){
    system("cls");
    cout<<"\n\n\n\n\n\n\n";
    cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|============ MAINTENANCE PANEL ===========|\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|  1. View All Records                     |\n";
    cout<<"\t\t\t\t\t\t\t\t|  2. Mark as Repaired                     |\n";
    cout<<"\t\t\t\t\t\t\t\t|  3. Send Car to Maintenance              |\n";
    cout<<"\t\t\t\t\t\t\t\t|  4. Back                                 |\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    int choice=getint("\t\t\t\t\t\t\t\t|  Choose: ",1,4);
    if (choice==INT_MIN){
	system("pause"); 
	return;}
    if (choice == 1){
        if (maintcount == 0){
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
        cout<<"\t\t\t\t\t\t\t\t|         No maintenance records.          |\n";
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
            system("pause");
            return;
        }
        for (int i=0; i<maintcount; i++){
            cout<<"\t\t\t\t\t\t\t\t|  Record #"<<i+1<<"\n";
            cout<<"\t\t\t\t\t\t\t\t|  Car        : "<<maintcar[i]<<"\n";
            cout<<"\t\t\t\t\t\t\t\t|  Reason     : "<<maintreason[i]<<"\n";
            cout<<"\t\t\t\t\t\t\t\t|  Reported   : "<<maintreportdate[i]<<"\n";
            if (maintfixed[i])
                cout<<"\t\t\t\t\t\t\t\t|  Status     : REPAIRED on "<<maintrepairdate[i]<<"\n";
            else
                cout<<"\t\t\t\t\t\t\t\t|  Status     : Under Maintenance\n";
            cout<<"\t\t\t\t\t\t\t\t|------------------------------------------|\n";
        }
        system("pause");
    } else if(choice==2) {
        if (maintcount == 0){
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
        cout<<"\t\t\t\t\t\t\t\t|           No records to update.          |\n";
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
            system("pause");
            return;
        }
        
        for (int i=0; i<maintcount; i++){
            cout<<"\t\t\t\t\t\t\t\t|  #"<<i+1<<": "<<maintcar[i]<<"-"<<(maintfixed[i] ? "Repaired" : "Broken")<<"\n";
        }
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
        int rec=getint("\t\t\t\t\t\t\t\t|  Enter Record Number to mark repaired: ",1,maintcount);
        if (rec == INT_MIN) { system("pause"); return; }
    long long repcost=getlarge("\t\t\t\t\t\t\t\t|  Maintenance cost: Rs ",0,100000000);
	if (repcost==INT_MIN){system("pause"); return;}
        int id=rec-1;
        if (maintfixed[id]){
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
        cout<<"\t\t\t\t\t\t\t\t|              Already repaired.           |\n";
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
            system("pause");
            return;
        }
        maintfixed[id]=true;
        maintrepairdate[id]=currentdatetime();
        string car=maintcar[id];
        for (int c=0; c<catn; c++){
            for (int i=0; i<carn; i++){
                if (carname[c][i] == car && status[c][i] == "Under Maintenance"){
                    avail[c][i]=true;
                    status[c][i]="Available";
                }
            }
        }
        totalrepcost +=repcost;
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
        cout<<"\t\t\t\t\t\t\t\t|Car marked as repaired & set to Available.|\n";
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
        savedata();
        system("pause");
    } else if(choice == 3){
    			
    system("cls");
    cout<<"\n\n\n\n";
    cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|========= SEND CAR TO MAINTENANCE ========|\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|------------ CHOOSE CATEGORY -------------|\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    for (int i=0; i<catn; i++)
        cout<<"\t\t\t\t\t\t\t\t|  "<<i+1<<". "<<catname[i]<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
        int temp=getint("\t\t\t\t\t\t\t\t|  Enter Category(1-5): ",1,5);
	if (temp==INT_MIN){system("pause"); return;}
		int cat=temp-1;
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|------------- AVAILABLE CARS -------------|\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|ID | Car Name       | Price   | Status    |\n";
    cout<<"\t\t\t\t\t\t\t\t|------------------------------------------|\n";
    bool available = false;
    for (int i=0; i<carn; i++){
        if (avail[cat][i] && status[cat][i] == "Available"){
            available = true;
            cout<<"\t\t\t\t\t\t\t\t| "<<i<<" | "<<carname[cat][i];
            int len=carname[cat][i].length();
            for (int p=len; p<15; p++) cout<<" ";
            cout<<"| Rs "<<carprice[cat][i];
            cout<<" | "<<status[cat][i]<<"\n";
        }
    }
    cout<<"\t\t\t\t\t\t\t\t|___|________________|_________|___________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    if (!available){
        cout<<"\t\t\t\t\t\t\t\t|  No available cars in this category.   |\n";
        cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
        system("pause");
        return;    }
    int id = getint("\t\t\t\t\t\t\t\t|  Enter car ID(0-4): ",0,carn-1);
    if (id==INT_MIN){ 
	system("pause");
	 return; }
    if (!avail[cat][id] || status[cat][id] != "Available"){
        cout<<"\t\t\t\t\t\t\t\t|  This car is not available.              |\n";
        cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
        system("pause");
        return;
    }
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|----->>Car Selected: "<<carname[cat][id]<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    cout<<"\t\t\t\t\t\t\t\t|  Are you sure you want to send this car  |\n";
    cout<<"\t\t\t\t\t\t\t\t|  to maintenance? : (1=Yes, 0=No)\n ";
    int confirm = getint("\t\t\t\t\t\t\t\t|  Confirm: ",0,1);
    if (confirm==INT_MIN || confirm==0){ 	system("pause"); 	return; }
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|  Select Issue Reason:                    |\n";
    cout<<"\t\t\t\t\t\t\t\t|  1. Engine Problem                       |\n";
    cout<<"\t\t\t\t\t\t\t\t|  2. AC Problem                           |\n";
    cout<<"\t\t\t\t\t\t\t\t|  3. Brake Problem                        |\n";
    cout<<"\t\t\t\t\t\t\t\t|  4. Tyre Problem                         |\n";
    cout<<"\t\t\t\t\t\t\t\t|  5. Other (Enter custom reason)          |\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    int choice = getint("\t\t\t\t\t\t\t\t|  Select: ",1,5);
    if (choice==INT_MIN){ system("pause"); return; }
    
    string reason;
    switch(choice){
        case 1: reason = "Engine Problem"; break;
        case 2: reason = "AC Problem"; break;
        case 3: reason = "Brake Problem"; break;
        case 4: reason = "Tyre Problem"; break;
        case 5: reason = gettext("\t\t\t\t\t\t\t\t|  Enter Custom Reason: ");
    }
    if (maintcount >= recn){
        cout<<"\t\t\t\t\t\t\t\t|  Maintenance log is full. Cannot add!   |\n";
        cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
        system("pause");
        return;
    }
    maintcar[maintcount] = carname[cat][id];
    maintreason[maintcount] = reason;
    maintreportdate[maintcount] = currentdatetime();
    maintfixed[maintcount] = false;
    maintrepairdate[maintcount] = "";
    maintcount++;
    
    avail[cat][id] = false;
    status[cat][id] = "Under Maintenance";
    
    renter[cat][id] = "";
    rentercnic[cat][id] = "";
    phone[cat][id] = "";
    address[cat][id] = "";
    booked[cat][id] = 0;
    insured[cat][id] = false;
    hasdriver[cat][id] = false;
    secpaid[cat][id] = 0;
    savedata();
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|  Car sent to maintenance successfully!   |\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    system("pause");
    	
	}
	savedata();
}

void rentalhistory(){
    system("cls");
    cout<<"\n\n\n\n\n\n\n";
	    cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
	    cout<<"\t\t\t\t\t\t\t\t|============== RENTAL HISTORY ============|\n";
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    if (histcount == 0){
        cout<<"\t\t\t\t\t\t\t\t|  No history records.\n";
        system("pause");
        return;
    }
    for (int i=0; i<histcount; i++){
        cout<<"\t\t\t\t\t\t\t\t|  #"<<i+1<<"\n";
        cout<<"\t\t\t\t\t\t\t\t|  Customer : "<<histname[i]<<"\n";
        cout<<"\t\t\t\t\t\t\t\t|  CNIC     : "<<histcnic[i]<<"\n";
        cout<<"\t\t\t\t\t\t\t\t|  Car      : "<<histcar[i]<<"\n";
        cout<<"\t\t\t\t\t\t\t\t|  Date     : "<<histdate[i]<<"\n";
        cout<<"\t\t\t\t\t\t\t\t|  Dest     : "<<(histdest[i].empty() ? "N/A" : histdest[i])<<"\n";
        cout<<"\t\t\t\t\t\t\t\t|------------------------------------------|\n";
    }
    system("pause");
}

void viewdue(){
    system("cls");
    cout<<"\n\n\n";
    cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|============== OVERDUE RENTALS ===========|\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";

    int totaldue=0;
    long long TF=0;
	long long TR=0;
    for (int c=0; c<catn; c++) {
        for (int i=0; i<carn; i++) {
            if (!avail[c][i] && status[c][i] == "Rented") {
                int used=daysbetween(sday[c][i], smonth[c][i], syear[c][i],currentday(), currentmonth(), currentyear());
                int book=booked[c][i];
                if (used > book) {
                    totaldue++;
                    int overduedays=used - book;
                    int daily=carprice[c][i];
                    long long fine=(overduedays * daily * 0.20);
                    long long total=fine+(daily*overduedays);
                    cout<<"\t\t\t\t\t\t\t\t|  Customer : "<<renter[c][i]<<"\n";
                    cout<<"\t\t\t\t\t\t\t\t|  CNIC     : "<<rentercnic[c][i]<<"\n";
                    cout<<"\t\t\t\t\t\t\t\t|  Phone    : "<<phone[c][i]<<"\n";
                    cout<<"\t\t\t\t\t\t\t\t|  Car      : "<<carname[c][i]<<"\n";
                    cout<<"\t\t\t\t\t\t\t\t|  Booked   : "<<book<<" days\n";
                    cout<<"\t\t\t\t\t\t\t\t|  Overdue  : "<<overduedays<<" days\n";
                    cout<<"\t\t\t\t\t\t\t\t|  Dailyrate: Rs "<<daily<<"\n";
                    cout<<"\t\t\t\t\t\t\t\t|  Late Fine: Rs "<<fine<<"\n";
                    cout<<"\t\t\t\t\t\t\t\t|  Total cost: Rs "<<total<<"\n";
                    cout<<"\t\t\t\t\t\t\t\t|------------------------------------------|\n";
                    TF += fine;
                    TR +=(daily*overduedays);
                }
            }
        }
    }

    if (totaldue == 0) {
        cout<<"\t\t\t\t\t\t\t\t|  No overdue rentals found.\n";
    } else {
        cout<<"\t\t\t\t\t\t\t\t|  Total Overdue Cars: "<<totaldue<<"\n";
        cout<<"\t\t\t\t\t\t\t\t|  Total Late Fines : Rs "<<TF<<"\n";
        cout<<"\t\t\t\t\t\t\t\t|  Total Revenu    : Rs "<<TR<<"\n";
    }
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    system("pause");
}

void editcardetails(){
    system("cls");
    cout<<"\n\n\n";
    cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|============ EDIT CAR DETAILS ============|\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|------------ CHOOSE CATEGORY -------------|\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    for (int i=0; i<catn; i++)
        cout<<"\t\t\t\t\t\t\t\t|  "<<i+1<<". "<<catname[i]<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    int temp=getint("\t\t\t\t\t\t\t\t|  Enter Category(1-5): ",1,5);
	if (temp==INT_MIN){system("pause"); return;}
		int cat=temp-1;
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|------------- AVAILABLE CARS -------------|\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|ID | Car Name       | Price   | Status    |\n";
    cout<<"\t\t\t\t\t\t\t\t|------------------------------------------|\n";
    for (int i=0; i<carn; i++){
        cout<<"\t\t\t\t\t\t\t\t| "<<i<<" | "<<carname[cat][i];
        int len=carname[cat][i].length();
        for (int p=len; p<15; p++) cout<<" ";
        cout<<"| Rs "<<carprice[cat][i];
        cout<<" | "<<status[cat][i]<<"\n";
    }
    cout<<"\t\t\t\t\t\t\t\t|___|________________|_________|___________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    int id=getint("\t\t\t\t\t\t\t\t|  Enter car ID(0-4): ",0,carn-1);
	if (id==INT_MIN){
			system("pause"); 
		return;}
	cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    if (!avail[cat][id]){
        cout<<"\t\t\t\t\t\t\t\t|   This car is not available right now.   |\n";
		cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
        system("pause");
        return;
    }
    system("cls");
    
    cout<<"\t\t\t\t\t\t\t\t|  Current Name: "<<carname[cat][id]<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|  Current Price: Rs "<<carprice[cat][id]<<"\n";
    cout<<"\t\t\t\t\t\t\t\t|------------------------------------------|\n";
    string newname=gettext("\t\t\t\t\t\t\t\t|  New Name (Enter to keep): ");
    if (!newname.empty()) carname[cat][id]=newname;
    int newprice=getint("\t\t\t\t\t\t\t\t|  New Price (0 to keep): ", 0, 999999);
    if (newprice != 0) carprice[cat][id]=newprice;
    savedata();
    cout<<"\t\t\t\t\t\t\t\t|     Car details updated successfully!\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    system("pause");
}

void adminconfig(){
    system("cls");
    cout<<"\n\n\n\n\n\n\n";
	cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|=========== CONFIGURATION PANEL ==========|\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|  Current Driver Rate : Rs "<<driverrate<<" / day\n";
    cout<<"\t\t\t\t\t\t\t\t|  Current Fuel Price  : Rs "<<fuelprice<<" / liter\n";
    cout<<"\t\t\t\t\t\t\t\t|  1. Change Driver Rate\n";
    cout<<"\t\t\t\t\t\t\t\t|  2. Change Fuel Price\n";
    cout<<"\t\t\t\t\t\t\t\t|  3. Back to Dashboard\n";
    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    int choice=getint("\t\t\t\t\t\t\t\t|  Enter choice: ",1,3);
	    if (choice==INT_MIN){
			system("pause"); 
		return;}
    if (choice == 1){
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
        driverrate=getint("\t\t\t\t\t\t\t\t|  Enter new Driver Rate : ",0,100000);
	    if (driverrate==INT_MIN){
			system("pause"); 
		return;}
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
        cout<<"\t\t\t\t\t\t\t\t|  Driver Rate updated to Rs "<<driverrate<<"\n";
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
        savedata();
        system("pause");
    } else if (choice == 2){
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
        fuelprice=getint("\t\t\t\t\t\t\t\t|  Enter new Fuel Price : ",0,100000);
	    if (fuelprice==INT_MIN){
			system("pause"); 
		return;}
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
        cout<<"\t\t\t\t\t\t\t\t|  Fuel Price updated to Rs "<<fuelprice<<"\n";
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
        savedata();
        system("pause");
    }
}

void resetdata(){
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
	    cout<<"\t\t\t\t\t\t\t\t|--WARNING: This will delete ALL records!--|\n\a";
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    string pass=gettext("\t\t\t\t\t\t\t\t|  Admin Password: ");
    if (pass.empty()) { system("pause"); return; }
    if (pass != adminpass){
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
        cout<<"\t\t\t\t\t\t\t\t|   Incorrect password. Reset cancelled.   |\n\a";
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
        system("pause");
        return;
    }
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
	    cout<<"\t\t\t\t\t\t\t\t|  Type YES to confirm: ";
    string confirm;
    getline(cin,confirm);
    if (confirm != "YES"){
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
        cout<<"\t\t\t\t\t\t\t\t|               Reset cancelled.           |\n";
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
        system("pause");
        return;
    }
    initializedata();
    maintcount=0;
    totalrepcost=0;
	histcount=0;
    totalrevenue=0;
    totalfine=0;
    totalbonus=0;
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
	    cout<<"\t\t\t\t\t\t\t\t|           System reset complete.         |\n";
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    savedata();
    system("pause");
}

void adminpanel(){
    system("cls");
    cout<<"\n\n\n\n\n\n\n";
	cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
	cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    cout<<"\t\t\t\t\t\t\t\t|=============== ADMIN LOGIN ==============|\n";
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
    string id=gettext("\t\t\t\t\t\t\t\t|  Admin ID: ");
    if (id.empty()) { system("pause"); return; }
    string pass=gettext("\t\t\t\t\t\t\t\t|  Admin Password: ");
    if (pass.empty()) { system("pause"); return; }
    if (id != admin || pass != adminpass){
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n\a";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
        cout<<"\t\t\t\t\t\t\t\t|               Access Denied.             |\n";
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
        system("pause");
        return;
    }
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
	    cout<<"\t\t\t\t\t\t\t\t|             Login Successful!            |\n";
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
    bool run=true;
    while (run){
        system("cls");
        cout<<"\n\n\n\n\n\n\n";
	    cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
        cout<<"\t\t\t\t\t\t\t\t|============= ADMIN DASHBOARD ============|\n";
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
        cout<<"\t\t\t\t\t\t\t\t|  1. View All Rented cars                 |\n";
        cout<<"\t\t\t\t\t\t\t\t|  2. View Customer Records                |\n";
        cout<<"\t\t\t\t\t\t\t\t|  3. Financial Report                     |\n";
        cout<<"\t\t\t\t\t\t\t\t|  4. Maintenance Details                  |\n";
        cout<<"\t\t\t\t\t\t\t\t|  5. View Rental History                  |\n";
        cout<<"\t\t\t\t\t\t\t\t|  6. View Overdue Rentals                 |\n";
        cout<<"\t\t\t\t\t\t\t\t|  7. Edit Car Details                     |\n";
        cout<<"\t\t\t\t\t\t\t\t|  8. Configuration                        |\n";
        cout<<"\t\t\t\t\t\t\t\t|  9. Reset All Data                       |\n";
        cout<<"\t\t\t\t\t\t\t\t|  10. Logout                              |\n";
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
        int ch=getint("\t\t\t\t\t\t\t\t|  Enter your choice: ",1,10);
        if (ch == INT_MIN) { system("pause"); return; }
        if (ch == 1) viewrented();
        else if (ch == 2) viewcust();
        else if (ch == 3) financialreport();
        else if (ch == 4) maintpanel();
        else if (ch == 5) rentalhistory();
		else if (ch == 6) viewdue();
		else if (ch == 7) editcardetails();
        else if (ch == 8) adminconfig();
        else if (ch == 9) resetdata();        else if (ch == 8) resetdata();
        else if (ch == 9) adminconfig();
        else run=false;
    }
}

void mainmenu(){
    bool run=true;
    while (run){
        system("cls");
		cout<<"\n\n\n\a";
	    cout<<"\t\t\t\t\t\t\t\t __________________________________________\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
        cout<<"\t\t\t\t\t\t\t\t|       PREMIUM MOTORS RENTAL SYSTEM       |\n";
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
        cout<<"\t\t\t\t\t\t\t\t|  1. CUSTOMER PORTAL                      |\n";
        cout<<"\t\t\t\t\t\t\t\t|  2. ADMIN DASHBOARD                      |\n";
        cout<<"\t\t\t\t\t\t\t\t|  3. Exit                                 |\n";
	    cout<<"\t\t\t\t\t\t\t\t|__________________________________________|\n";
	    cout<<"\t\t\t\t\t\t\t\t|                                          |\n";
        int ch=getint("\t\t\t\t\t\t\t\t|  Enter your choice: ",1,3);
        if (ch == INT_MIN) { system("pause"); return; }
        if (ch == 1) customerpanel();
        else if (ch == 2) adminpanel();
        else run=false;
    }
}

int main(){
    system("color F0");
    initializedata();
    loaddata();
    mainmenu();
    savedata();
    return 0;
}
