#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <cctype>
#include <regex>
using namespace std;

const int MAX_PATIENTS=100;
const int MAX_DOCTORS=20;
const string DOCTOR_FILE="doctors.txt";   // format per line: username|password|name
const string RECEPTIONIST_FILE="receptionists.txt"; // format: username|password|name

struct Patient
{
    int id;
    string name;
    int age;
    string appointment;
    string doctor;           // doctor name (for display)
    string doctorUsername;   // doctor username (unique, used to match the doctor)
    int priority;
    bool served;
};

struct Doctor
{
    string name;
    string username;
    string password;
};

class Hospital
{
private:
    Patient patients[MAX_PATIENTS];
    Doctor doctors[MAX_DOCTORS];
    int patientCount=0;
    int doctorCount=0;
    int nextPatientId=1;

    void clearInput()
    {
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
    }

    // Splits one line "username|password|name" into a Doctor.
    bool parseDoctorLine(const string& line,Doctor& d)
    {
        size_t p1=line.find('|');
        if(p1==string::npos) return false;
        size_t p2=line.find('|',p1+1);
        if(p2==string::npos) return false;

        d.username=line.substr(0,p1);
        d.password=line.substr(p1+1,p2-p1-1);
        d.name=line.substr(p2+1);
        return true;
    }

    // Splits one line "username|password|name" into a Receptionist.
    bool parseReceptionistLine(const string& line,string& username,string& password,string& name)
    {
        size_t p1=line.find('|');
        if(p1==string::npos) return false;
        size_t p2=line.find('|',p1+1);
        if(p2==string::npos) return false;

        username=line.substr(0,p1);
        password=line.substr(p1+1,p2-p1-1);
        name=line.substr(p2+1);
        return true;
    }

    bool receptionistUsernameInFile(const string& username)
    {
        ifstream in(RECEPTIONIST_FILE);
        string line,storedUsername,storedPassword,storedName;
        while(getline(in,line))
        {
            if(!line.empty()&&line.back()=='\r') line.pop_back();
            if(parseReceptionistLine(line,storedUsername,storedPassword,storedName)
               &&storedUsername==username)
                return true;
        }
        return false;
    }

    // Reads doctors.txt into the doctors[] array (used for the "available doctors" list).
    void loadDoctors()
    {
        ifstream in(DOCTOR_FILE);
        if(!in) return;   // file does not exist yet; it is created on first registration

        string line;
        Doctor d;
        while(doctorCount<MAX_DOCTORS&&getline(in,line))
        {
            if(!line.empty()&&line.back()=='\r') line.pop_back();
            if(parseDoctorLine(line,d))
                doctors[doctorCount++]=d;
        }
    }

    bool usernameInFile(const string& username)
    {
        ifstream in(DOCTOR_FILE);
        string line;
        Doctor d;
        while(getline(in,line))
        {
            if(!line.empty()&&line.back()=='\r') line.pop_back();
            if(parseDoctorLine(line,d)&&d.username==username)
                return true;
        }
        return false;
    }

public:
    Hospital()
    {
        loadDoctors();
    }

    void addPatient()
    {
        if(patientCount==MAX_PATIENTS)
        {
            cout<<"Patient list is full.\n";
            return;
        }

        if(doctorCount==0)
        {
            cout<<"No doctors are available right now. Appointment cannot be booked.\n";
            return;
        }

        Patient p;

        //name should only contain letters and spaces
        while(true){
            cout<<"Patient name: ";
        getline(cin,p.name);
        bool validName = !p.name.empty();
        for (char c:p.name) 
        {
            if(!isalpha(c)&&c!= ' ') 
            {
                validName=false;
                break;
            }
        }
        if(!validName) 
        {
            cout<<"Invalid name. Patient name should only contain letters and spaces. Please re-enter.\n";
            
        }
        else break;
    }

        //00<Age<=120
        while(true){
            cout<<"Age: ";
        if (!(cin>>p.age)) 
        {
            cin.clear();
            clearInput();
            cout<<"Invalid input for age. Please re-enter.\n";
            continue;
        }
        clearInput();
        if (p.age<=0||p.age>120) 
        {
            cout<<"Invalid age. Age must be between 1 and 120. Please re-enter.\n";
            
        }
        else break;
    }
        //Use Regex for Appointment Scheduling (YYYY-MM-DD HH:MM)
        while(true){
            cout<<"Appointment date/time(YYYY-MM-DD HH:MM): ";
        getline(cin,p.appointment);
        regex appointmentPattern(R"(\d{4}-(?:0[1-9]|1[0-2])-(?:0[1-9]|[12]\d|3[01]) (?:[01]\d|2[0-3]):[0-5]\d)");
        if(!regex_match(p.appointment,appointmentPattern)) 
        {
            cout<<"Invalid appointment format. Please use YYYY-MM-DD HH:MM (e.g., 2026-06-05 14:30). Please re-enter.\n";
            
        }
        else break;
    }
    viewAvailableDoctors();
    int doctorChoice;
    while (true)
    {
        
        cout<<"Choose doctor (enter the number from the list): ";
        if(!(cin>>doctorChoice))
        {
            cin.clear();
            clearInput();
            cout<<"Invalid input. Please re-enter.\n";
            return;
        }
        clearInput();

        if(doctorChoice<1||doctorChoice>doctorCount)
        {
            cout<<"Invalid doctor number. Please re-enter.\n";
            
        }
        else break;
    }
    
        
        p.doctor=doctors[doctorChoice-1].name;
        p.doctorUsername=doctors[doctorChoice-1].username;

        while(true){
            cout<<"Priority ID (1 = highest, 2 = medium, 3 = lowest): ";
        cin>>p.priority;
        clearInput();

        if(p.priority<1||p.priority>3)
        {
            cout<<"Invalid priority. Please re-enter.\n";
            
        }
        else break;
    }

        p.id=nextPatientId++;
        p.served=false;
        patients[patientCount++]=p;
        cout<<"Patient added successfully. Your patient ID is "<<p.id<<".\n";
    }

    void viewAppointments()
    {
        bool found=false;
        cout<<"\n--- Your Appointment List ---\n";

        for(int i=0;i<patientCount;i++)
        {
            cout<<"Patient ID: "<<patients[i].id
                <<" | Name: "<<patients[i].name
                <<" | Appointment: "<<patients[i].appointment
                <<" | Doctor: "<<patients[i].doctor
                <<" | Status: "<<(patients[i].served?"Served":"Waiting")<<"\n";
            found=true;
        }

        if(!found)
            cout<<"No appointments found.\n";
    }

    void viewAvailableDoctors()
    {
        cout<<"\n--- Available Doctors ---\n";

        if(doctorCount==0)
        {
            cout<<"No doctors registered yet.\n";
            return;
        }

        for(int i=0;i<doctorCount;i++)
            cout<<i+1<<". "<<doctors[i].name<<"\n";
    }

    void patientReports()
    {
        int waiting=0,served=0;

        for(int i=0;i<patientCount;i++)
        {
            if(patients[i].served)
                served++;
            else
                waiting++;
        }

        cout<<"\n--- Patient Report ---\n";
        cout<<"Total appointments: "<<patientCount<<"\n";
        cout<<"Waiting: "<<waiting<<"\n";
        cout<<"Served: "<<served<<"\n";
    }

    void receptionistMenu()
    {
        int choice;

        do
        {
            cout<<"\n========== RECEPTIONIST DASHBOARD ==========\n";
            cout<<"1. Register patient / book appointment\n";
            cout<<"2. View all appointments\n";
            cout<<"3. View available doctors\n";
            cout<<"4. View patient reports\n";
            cout<<"0. Back to main menu\n";
            cout<<"Choice: ";

            if(!(cin>>choice))
            {
                cin.clear();
                clearInput();
                cout<<"Enter a valid number.\n";
                continue;
            }
            clearInput();

            
            switch(choice)
            {
                case 1: addPatient(); break;
                case 2: viewAppointments(); break;
                case 3: viewAvailableDoctors(); break;
                case 4: patientReports(); break;
                case 0: break;
                default: cout<<"Invalid choice.\n";
            }
        }
        while(choice!=0);
    }

    void registerReceptionist()
    {
        string name,username,password;

        cout<<"Receptionist name: ";
        getline(cin,name);

        bool validName=false,onlyAllowed=true;
        for(char c:name)
        {
            if(isalpha(static_cast<unsigned char>(c)))
                validName=true;
            else if(c!=' ')
            {
                onlyAllowed=false;
                break;
            }
        }

        if(name.empty()||!validName||!onlyAllowed)
        {
            cout<<"Invalid name. Use letters and spaces only.\n";
            return;
        }

        cout<<"Create username: ";
        getline(cin,username);
        cout<<"Create password: ";
        getline(cin,password);

        if(username.empty()||password.empty())
        {
            cout<<"Username and password cannot be empty.\n";
            return;
        }

        if(username.find('|')!=string::npos||password.find('|')!=string::npos
           ||name.find('|')!=string::npos)
        {
            cout<<"The '|' character is not allowed.\n";
            return;
        }

        if(receptionistUsernameInFile(username))
        {
            cout<<"Username already exists. Please choose another username.\n";
            return;
        }

        ofstream out(RECEPTIONIST_FILE,ios::app);
        if(!out)
        {
            cout<<"Error: could not open "<<RECEPTIONIST_FILE<<" for writing.\n";
            return;
        }

        out<<username<<"|"<<password<<"|"<<name<<"\n";
        out.close();
        cout<<"Receptionist registered successfully. Please log in.\n";
    }

    bool receptionistLogin(string& loggedInName,string& loggedInUsername)
    {
        string username,password;
        cout<<"Receptionist username: ";
        getline(cin,username);
        cout<<"Password: ";
        getline(cin,password);

        ifstream in(RECEPTIONIST_FILE);
        if(!in)
        {
            cout<<"No receptionist account found. Please register first.\n";
            return false;
        }

        string line,storedUsername,storedPassword,storedName;
        while(getline(in,line))
        {
            if(!line.empty()&&line.back()=='\r') line.pop_back();
            if(parseReceptionistLine(line,storedUsername,storedPassword,storedName)
               &&storedUsername==username&&storedPassword==password)
            {
                loggedInName=storedName;
                loggedInUsername=storedUsername;
                cout<<"Welcome, "<<loggedInName<<".\n";
                return true;
            }
        }

        cout<<"Login failed. Check your username and password.\n";
        return false;
    }

    void registerDoctor()
    {
        if(doctorCount==MAX_DOCTORS)
        {
            cout<<"Doctor limit reached.\n";
            return;
        }

        Doctor d;

        //name should only contain letters and spaces (and not be blank)
        while(true)
        {
            cout<<"Doctor name: ";
            getline(cin,d.name);

            bool validName=false;   // needs at least one letter
            bool onlyAllowed=true;
            for(char c:d.name)
            {
                if(isalpha(static_cast<unsigned char>(c)))
                    validName=true;
                else if(c!=' ')
                {
                    onlyAllowed=false;
                    break;
                }
            }

            if(!validName||!onlyAllowed)
                cout<<"Invalid name. Doctor name should only contain letters and spaces. Please re-enter.\n";
            else break;
        }

        cout<<"Create username: ";
        getline(cin,d.username);
        cout<<"Create password: ";
        getline(cin,d.password);

        if(d.name.empty()||d.username.empty()||d.password.empty())
        {
            cout<<"Name, username and password cannot be empty.\n";
            return;
        }

        if(d.username.find('|')!=string::npos||d.password.find('|')!=string::npos
           ||d.name.find('|')!=string::npos)
        {
            cout<<"The '|' character is not allowed.\n";
            return;
        }

        if(usernameInFile(d.username))
        {
            cout<<"Username already exists. Please register with another username.\n";
            return;
        }

        // Append the new doctor to the file (file is created if it doesn't exist)
        ofstream out(DOCTOR_FILE,ios::app);
        if(!out)
        {
            cout<<"Error: could not open "<<DOCTOR_FILE<<" for writing.\n";
            return;
        }
        out<<d.username<<"|"<<d.password<<"|"<<d.name<<"\n";
        out.close();

        doctors[doctorCount++]=d;
        cout<<"Doctor registered successfully. Please log in.\n";
    }

    // Checks credentials against doctors.txt. On success, fills loggedIn with that doctor.
    bool doctorLogin(Doctor& loggedIn)
    {
        string username,password;

        cout<<"Username: ";
        getline(cin,username);
        cout<<"Password: ";
        getline(cin,password);

        ifstream in(DOCTOR_FILE);
        if(!in)
        {
            cout<<"No doctors registered yet. Please register first.\n";
            return false;
        }

        string line;
        Doctor d;
        while(getline(in,line))
        {
            if(!line.empty()&&line.back()=='\r') line.pop_back();
            if(parseDoctorLine(line,d)&&d.username==username&&d.password==password)
            {
                loggedIn=d;
                cout<<"Welcome, Dr. "<<loggedIn.name<<".\n";
                return true;
            }
        }

        cout<<"Login failed. Check your username and password.\n";
        return false;
    }

    void viewPatientList(const string& doctorUsername)
    {
        cout<<"\n--- Patient List ---\n";
        bool found=false;

        for(int i=0;i<patientCount;i++)
        {
            if(patients[i].doctorUsername!=doctorUsername)
                continue;

            found=true;
            cout<<"ID: "<<patients[i].id
                <<" | Name: "<<patients[i].name
                <<" | Age: "<<patients[i].age
                <<" | Appointment: "<<patients[i].appointment
                <<" | Doctor: "<<patients[i].doctor
                <<" | Priority: "<<patients[i].priority
                <<" | Status: "<<(patients[i].served?"Served":"Waiting")<<"\n";
        }

        if(!found)
            cout<<"No patient records found.\n";
    }

    void serveNextPatient(const string& doctorUsername)
    {
        int best=-1;

        // Custom priority queue: scan the array and choose the smallest
        // priority ID among waiting patients. Earlier arrival wins ties.
        for(int i=0;i<patientCount;i++)
        {
            if(patients[i].served||patients[i].doctorUsername!=doctorUsername)
                continue;

            if(best==-1||patients[i].priority<patients[best].priority)
                best=i;
        }

        if(best==-1)
        {
            cout<<"No waiting patients to serve.\n";
            return;
        }

        cout<<"\nNext patient:\n";
        cout<<"ID: "<<patients[best].id
            <<" | Name: "<<patients[best].name
            <<" | Priority ID: "<<patients[best].priority<<"\n";

        patients[best].served=true;
        cout<<"Patient marked as served.\n";
    }

    void viewServedRecords(const string& doctorUsername)
    {
        bool found=false;
        cout<<"\n--- Served Patient Records ---\n";

        for(int i=0;i<patientCount;i++)
        {
            if(patients[i].served&&patients[i].doctorUsername==doctorUsername)
            {
                found=true;
                cout<<"ID: "<<patients[i].id
                    <<" | Name: "<<patients[i].name
                    <<" | Doctor: "<<patients[i].doctor
                    <<" | Appointment: "<<patients[i].appointment
                    <<" | Priority: "<<patients[i].priority<<"\n";
            }
        }

        if(!found)
            cout<<"No patients have been served yet.\n";
    }

    void servedPatientReport(const string& doctorUsername)
    {
        int served=0;
        int priorityCount[4]={0,0,0,0};

        for(int i=0;i<patientCount;i++)
        {
            if(patients[i].served&&patients[i].doctorUsername==doctorUsername)
            {
                served++;
                priorityCount[patients[i].priority]++;
            }
        }

        cout<<"\n--- Report for Served Patients ---\n";
        cout<<"Total served: "<<served<<"\n";
        cout<<"Priority 1 served: "<<priorityCount[1]<<"\n";
        cout<<"Priority 2 served: "<<priorityCount[2]<<"\n";
        cout<<"Priority 3 served: "<<priorityCount[3]<<"\n";
    }

    void doctorMenu(const Doctor& doc)
    {
        int choice;

        do
        {
            cout<<"\n========== DOCTOR MENU ==========\n";
            cout<<"Logged in as Dr. "<<doc.name<<"\n";
            cout<<"1. View patient list\n";
            cout<<"2. Serve next patient (priority queue)\n";
            cout<<"3. View served patient records\n";
            cout<<"4. Generate report for served patients\n";
            cout<<"0. Logout\n";
            cout<<"Choice: ";

            if(!(cin>>choice))
            {
                cin.clear();
                clearInput();
                cout<<"Enter a valid number.\n";
                continue;
            }
            clearInput();

            switch(choice)     
            {
                case 1: viewPatientList(doc.username); break;
                case 2: serveNextPatient(doc.username); break;
                case 3: viewServedRecords(doc.username); break;
                case 4: servedPatientReport(doc.username); break;
                case 0: cout<<"Logged out.\n"; break;
                default: cout<<"Invalid choice.\n";
            }
        }
        while(choice!=0);
    }
};

int main()
{
    Hospital hospital;
    int choice;

    do
    {
        cout<<"\n========== HOSPITAL MANAGEMENT SYSTEM ==========\n";
        cout<<"1. Receptionist\n";
        cout<<"2. Doctor\n";
        cout<<"0. Exit\n";
        cout<<"Choose user type: ";

        if(!(cin>>choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout<<"Enter a valid number.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(),'\n');

        switch(choice)
        {
            case 1:
            {
                int receptionistChoice;
                cout<<"\n1. Register new receptionist\n";
                cout<<"2. Login\n";
                cout<<"0. Back\n";
                cout<<"Choice: ";

                if(!(cin>>receptionistChoice))
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(),'\n');
                    cout<<"Enter a valid number.\n";
                    break;
                }
                cin.ignore(numeric_limits<streamsize>::max(),'\n');

                if(receptionistChoice==1)
                    hospital.registerReceptionist();
                else if(receptionistChoice==2)
                {
                    string loggedInName,loggedInUsername;
                    if(hospital.receptionistLogin(loggedInName,loggedInUsername))
                        hospital.receptionistMenu();
                }
                else if(receptionistChoice!=0)
                    cout<<"Invalid choice.\n";
                break;
            }

            case 2:
            {
                int doctorChoice;
                cout<<"\n1. Register new doctor\n";
                cout<<"2. Login\n";
                cout<<"0. Back\n";
                cout<<"Choice: ";

                if(!(cin>>doctorChoice))
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(),'\n');
                    cout<<"Enter a valid number.\n";
                    break;
                }
                cin.ignore(numeric_limits<streamsize>::max(),'\n');

                if(doctorChoice==1)
                    hospital.registerDoctor();
                else if(doctorChoice==2)
                {
                    Doctor loggedDoctor;
                    if(hospital.doctorLogin(loggedDoctor))
                        hospital.doctorMenu(loggedDoctor);
                }
                else if(doctorChoice!=0)
                    cout<<"Invalid choice.\n";
                break;
            }

            case 0:
                cout<<"Goodbye!\n";
                break;

            default:
                cout<<"Invalid choice.\n";
        }
    }
    while(choice!=0);

    return 0;
}
