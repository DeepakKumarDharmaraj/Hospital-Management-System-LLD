#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

using namespace std;

//Practice
//git branch

// ─────────────────────────────────────────
// TEMPLATE — Generic Storage for any type
// ─────────────────────────────────────────
template <typename T>
class Storage {
private:
    vector<T*> items;
public:
    void add(T* item) {
        items.push_back(item);
    }
    T* get(int index) {
        if (index < 0 || index >= (int)items.size())
            throw out_of_range("Invalid index");
        return items[index];
    }
    int count() { return items.size(); }
    vector<T*>& all() { return items; }
};

// ─────────────────────────────────────────
// ABSTRACT BASE CLASS — Person (Abstraction)
// ─────────────────────────────────────────
class Person {
protected:
    string name;
    int    age;
    string id;
public:
    static int totalPersons;

    Person(string n, int a, string i) : name(n), age(a), id(i) {
        totalPersons++;
    }
    virtual ~Person() { totalPersons--; }

    virtual void displayInfo() = 0;
    virtual string getRole()   = 0;

    string getName() { return name; }
    string getId()   { return id;   }
    int    getAge()  { return age;  }
};
int Person::totalPersons = 0;

// ─────────────────────────────────────────
// OPERATOR OVERLOADING — Billing
// ─────────────────────────────────────────
class Bill {
private:
    double consultationFee;
    double medicineCost;
    double labCost;
public:
    Bill(double c = 0, double m = 0, double l = 0)
        : consultationFee(c), medicineCost(m), labCost(l) {}

    Bill operator+(const Bill& b) const {
        return Bill(consultationFee + b.consultationFee,
                    medicineCost   + b.medicineCost,
                    labCost        + b.labCost);
    }

    bool operator>(const Bill& b) const {
        return total() > b.total();
    }

    double total() const {
        return consultationFee + medicineCost + labCost;
    }

    void display() const {
        cout << "  Consultation : Rs." << consultationFee << "\n"
             << "  Medicines    : Rs." << medicineCost    << "\n"
             << "  Lab Tests    : Rs." << labCost         << "\n"
             << "  TOTAL        : Rs." << total()         << "\n";
    }
};

// ─────────────────────────────────────────
// ENCAPSULATION — MedicalRecord
// ─────────────────────────────────────────
class MedicalRecord {
private:
    string diagnosis;
    string prescription;
    string date;

public:
    MedicalRecord(string diag, string pres, string dt)
        : diagnosis(diag), prescription(pres), date(dt) {}

    string getDiagnosis()    { return diagnosis;    }
    string getPrescription() { return prescription; }
    string getDate()         { return date;         }

    void setDiagnosis(string d)    { diagnosis    = d; }
    void setPrescription(string p) { prescription = p; }
};

// ─────────────────────────────────────────
// INHERITANCE — Doctor and Patient inherit Person
// ─────────────────────────────────────────
class Doctor : public Person {
private:
    string specialization;
    double consultationFee;
    int    patientsHandled;

public:
    Doctor(string n, int a, string i, string spec, double fee)
        : Person(n, a, i), specialization(spec),
          consultationFee(fee), patientsHandled(0) {}

    void treat() { patientsHandled++; }

    double getFee()             { return consultationFee;  }
    string getSpecialization()  { return specialization;   }
    int    getPatientsHandled() { return patientsHandled;  }

    void displayInfo() override {
        cout << "[Doctor] " << name << " | ID: " << id
             << " | " << specialization
             << " | Fee: Rs." << consultationFee
             << " | Patients: " << patientsHandled << "\n";
    }

    string getRole() override { return "Doctor"; }
};

class Patient : public Person {
private:
    string        bloodGroup;
    string        disease;
    MedicalRecord* record;
    Bill           bill;

public:
    Patient(string n, int a, string i, string bg, string dis)
        : Person(n, a, i), bloodGroup(bg), disease(dis), record(nullptr) {}

    ~Patient() { delete record; }

    void assignRecord(MedicalRecord* r) { record = r;    }
    void assignBill(const Bill& b)      { bill   = b;    }

    MedicalRecord* getRecord() { return record; }
    Bill&          getBill()   { return bill;   }
    string         getDisease(){ return disease;}

    void displayInfo() override {
        cout << "[Patient] " << name << " | ID: " << id
             << " | Age: " << age
             << " | Blood: " << bloodGroup
             << " | Disease: " << disease << "\n";
        if (record)
            cout << "  Diagnosis: "     << record->getDiagnosis()
                 << " | Prescription: " << record->getPrescription() << "\n";
        cout << "  Bill:\n";
        bill.display();
    }

    string getRole() override { return "Patient"; }
};

// ─────────────────────────────────────────
// INHERITANCE — Specialist inherits Doctor (Multi-level)
// ─────────────────────────────────────────
class Specialist : public Doctor {
private:
    string expertise;
    int    yearsOfExperience;

public:
    Specialist(string n, int a, string i, string spec,
               double fee, string exp, int yrs)
        : Doctor(n, a, i, spec, fee), expertise(exp), yearsOfExperience(yrs) {}

    void displayInfo() override {
        Doctor::displayInfo();
        cout << "  Expertise: " << expertise
             << " | Experience: " << yearsOfExperience << " yrs\n";
    }

    string getRole() override { return "Specialist"; }
};

// ─────────────────────────────────────────
// FRIEND FUNCTION — Compare two bills
// ─────────────────────────────────────────
class BillingDept {
public:
    friend void compareBills(Patient& p1, Patient& p2);
};

void compareBills(Patient& p1, Patient& p2) {
    cout << "\n--- Bill Comparison ---\n";
    cout << p1.getName() << " bill: Rs." << p1.getBill().total() << "\n";
    cout << p2.getName() << " bill: Rs." << p2.getBill().total() << "\n";
    if (p1.getBill() > p2.getBill())
        cout << p1.getName() << " has the higher bill.\n";
    else
        cout << p2.getName() << " has the higher bill.\n";
}

// ─────────────────────────────────────────
// POLYMORPHISM — Function Overloading in Reception
// ─────────────────────────────────────────
class Reception {
public:
    void registerPerson(Doctor* d) {
        cout << "[Reception] Doctor registered: " << d->getName()
             << " (" << d->getSpecialization() << ")\n";
    }

    void registerPerson(Patient* p) {
        cout << "[Reception] Patient registered: " << p->getName()
             << " for " << p->getDisease() << "\n";
    }

    void registerPerson(Specialist* s) {
        cout << "[Reception] Specialist registered: " << s->getName()
             << " (" << s->getRole() << ")\n";
    }
};

// ─────────────────────────────────────────
// EXCEPTION HANDLING — Appointment Booking
// ─────────────────────────────────────────
class AppointmentSystem {
public:
    static void bookAppointment(Doctor* d, Patient* p, int slot) {
        try {
            if (slot < 1 || slot > 5)
                throw invalid_argument("Slot must be between 1 and 5.");
            if (d->getPatientsHandled() >= 10)
                throw runtime_error("Doctor's schedule is full.");

            d->treat();
            cout << "[Appointment] " << p->getName()
                 << " booked with Dr." << d->getName()
                 << " at slot " << slot << "\n";

            Bill b(d->getFee(), 200.0, 500.0);
            p->assignBill(b);
            p->assignRecord(new MedicalRecord(
                p->getDisease() + " - Diagnosed",
                "Rest + Medication",
                "2025-06-15"
            ));
        }
        catch (const invalid_argument& e) {
            cout << "[Error] Booking failed: " << e.what() << "\n";
        }
        catch (const runtime_error& e) {
            cout << "[Error] Booking failed: " << e.what() << "\n";
        }
    }
};

// ─────────────────────────────────────────
// STATIC MEMBER — Hospital stats tracker
// ─────────────────────────────────────────
class Hospital {
private:
    string name;
    static int totalHospitals;

public:
    Storage<Doctor>  doctors;
    Storage<Patient> patients;

    Hospital(string n) : name(n) { totalHospitals++; }
    ~Hospital()                   { totalHospitals--; }

    static int getTotalHospitals() { return totalHospitals; }
    string     getName()           { return name;           }

    void showAllStaff() {
        cout << "\n=== Staff at " << name << " ===\n";
        for (Person* p : doctors.all())   p->displayInfo();
    }

    void showAllPatients() {
        cout << "\n=== Patients at " << name << " ===\n";
        for (Person* p : patients.all())  p->displayInfo();
    }
};
int Hospital::totalHospitals = 0;

// ─────────────────────────────────────────
// MAIN — Runs the full hospital simulation
// ─────────────────────────────────────────
int main() {
    cout << "=========================================\n";
    cout << "   CITY CARE HOSPITAL MANAGEMENT SYSTEM  \n";
    cout << "=========================================\n\n";

    Hospital hospital("City Care Hospital");

    Doctor*    d1 = new Doctor("Dr. Priya",  45, "D001", "Cardiology",    800.0);
    Specialist* d2 = new Specialist("Dr. Arjun", 52, "D002",
                                    "Neurology", 1200.0, "Brain Surgery", 20);

    Patient* p1 = new Patient("Deepak",  22, "P001", "O+", "Fever");
    Patient* p2 = new Patient("Kumar", 35, "P002", "B+", "Migraine");

    hospital.doctors.add(d1);
    hospital.doctors.add(d2);
    hospital.patients.add(p1);
    hospital.patients.add(p2);

    Reception reception;
    reception.registerPerson(d1);
    reception.registerPerson(d2);
    reception.registerPerson(p1);
    reception.registerPerson(p2);

    cout << "\n--- Booking Appointments ---\n";
    AppointmentSystem::bookAppointment(d1, p1, 2);
    AppointmentSystem::bookAppointment(d2, p2, 4);
    AppointmentSystem::bookAppointment(d1, p1, 99);

    Bill combined = p1->getBill() + p2->getBill();
    cout << "\n--- Combined Bill (both patients) ---\n";
    combined.display();

    compareBills(*p1, *p2);

    hospital.showAllStaff();
    hospital.showAllPatients();

    cout << "\n--- System Stats ---\n";
    cout << "Total Hospitals   : " << Hospital::getTotalHospitals() << "\n";
    cout << "Total Persons     : " << Person::totalPersons          << "\n";

    delete d1;
    delete d2;
    delete p1;
    delete p2;

    cout << "\nPersons after cleanup: " << Person::totalPersons << "\n";
    cout << "\n[System] Hospital shutdown complete.\n";

    cout << "branch-conflict";
    return 0;
}
