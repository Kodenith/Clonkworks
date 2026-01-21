/*-- Kraftwerk --*/

#strict 2
#include DOOR
#include BAS4
#include L_FU

// Initialisierung 

protected func Initialize()
{
  // Netterweise gibt's zu Beginn einmal Kohle
  CreateContents(COAL);
}

public func ContextFuel(pCaller)
{
  [$GatherFuel$|Image=COAL]
  return(DemandFuel(CheckContainedFuel()+1, 0, pCaller, this));
}

// Eingangssteuerung 

private func SoundOpenDoor()
{
  Sound("GateOpen");
}

private func SoundCloseDoor()
{
  Sound("GateClose");
}
  
protected func Collection(object pObj)
{
  Sound("Clonk");
}

// Produktion 

private func Burning()
{
  // Rauch
  Smoke(-15,-20,12);
  Smoke(-1,-23,8);

  // Energieerzeugung
  if(!(GetActTime()%200)) AccessStoredFuel(1); //alle 200 Frames eine Einheit Brennstoff verbrauchen
  DoEnergy(25);
  
  // Weiter
  if(GetBurnTime() && CheckContainedFuel()) return(1);
  // Fertig
  SetAction("Idle");
}

func EjectNonFuel(pDumb)
{
   SetCommand(pDumb, "Exit");
}

// Inhaltsüberprüfung 

private func ContentsCheck()
{
  // Noch aktiv
  if(!ActIdle()) return(1);
  var i = -1, obj, def;
  while(obj = Contents(++i) )
    if(obj->~IsLorry() )
      EjectNonFuel(obj);

  // Erstes Inhaltsobjekt prüfen
  obj = Contents(0); 
  // Alles außer Brennmaterial raus
   if(obj)
     if (!(obj->~FurnaceFuelValue()))
       EjectNonFuel(obj);
     else
       if(CheckEnergyNeedChain())
	   {
	     FeedFuel(obj);
         SetAction("Burning");
	   }
}

///Brenndauer 
private func GetBurnTime() { return iStoredFuel*200; }

func CmdAcquireFuel() { return inherited(...); } //pseudo-lokalisierung, weil call nur lokale funktionen unterstützt

func CmdAcquireFuelFailed() { return inherited(...); }