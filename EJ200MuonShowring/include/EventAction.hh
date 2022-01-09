#ifndef EVENTACTION_HH
#define EVENTACTION_HH

#include "G4UserEventAction.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"

#include "g4root.hh"

#include "RunAction.hh"


class EventAction : public G4UserEventAction{

public:
    EventAction(RunAction *);
    ~EventAction();

    virtual void BeginOfEventAction(const G4Event *);
    virtual void EndOfEventAction(const G4Event *);

    void AddEdep(G4double edep) { fEdep += edep; };
    void AddnSec(G4double nsec) { fNsec += nsec; };


private:
    G4double fEdep;
    G4double fNsec;
    G4ThreeVector *posMuon;
};
#endif