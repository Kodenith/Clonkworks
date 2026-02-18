/*-- Hochofen --*/

#strict

#include BAS5
#include L_FU
#include DOOR

global func FindRawDef(typ){
	var def, i;
	while(def = GetDefinition(i,C4D_Object)){
		if(DefinitionCall(def,"SmeltResult") == typ) return(def);
		i++;
	}
	
	return(0);
}

/* Produktion */

public func IsProducerOf (object clonk, id def) {
  return (def == METL && IsBuilt());
}

public func HowToProduce (object clonk, id def) {
  AddCommand (clonk, "Wait", 0, 0, 0, 0, 0, 190);
  Production (clonk, def);
}

public func StartProduction(object clonk)
{
  // Arbeitsbefehl setzen
  CreateMenu(METL, clonk, this(), 4, "Foo", , 0, false, 1);
  var def, i;
  while(def = GetDefinition(i,C4D_Object)){
	  if(DefinitionCall(def,"SmeltResult"))
		    AddMenuItem(GetName(, DefinitionCall(def,"SmeltResult")), "SelectedMat", DefinitionCall(def,"SmeltResult"),clonk, 0, clonk);
	  i++;
  }
}

public func SelectedMat(id item, object clonk){
	  AddCommand(clonk,"Call",this(),item,0,0,0,"Production");
}

public func ContextConstruction(object pCaller)
{
  [$TxtProducemetal$|Image=METL|Condition=IsBuilt]
  return(StartProduction(pCaller));
}

public func Production(object clonk, id idProduct)
{
    // Der Arbeiter soll Brennstoff beschaffen, wenn nötig
    DemandFuel(10, 0, clonk, this);

    if (!FindContents(idProduct))
    {
    // Der Arbeiter soll welches beschaffen
    AddCommand(clonk,"Call",this(),0,0,0,0,"Acquisition", 0, 3);
    AddCommand(clonk,"Put",this(),0,0,0,0,FindRawDef(idProduct));
    AddCommand(clonk,"Acquire",0,0,0,0,0,FindRawDef(idProduct),3);  
    }
  // Fertig
  return(1);
}
  
// --------------------------------------------------

public func Acquisition(object pWorker)
{
  // Materialbeschaffung erfolgreich
  return(1);
}

public func AcquisitionFailed(object pWorker)
{
  // Materialbeschaffung fehlgeschlagen: Meldung ausgeben
  var noOre = true;
  for(var i in FindObjects(Find_Category(C4D_Object),Find_Container(this()))){
	  if(i->~SmeltResult()) noOre = false;
  }
  
  
  if (noOre) 
    return(Message("$TxtNooreavailable$", pWorker));
  Message("$TxtNeedsfueltoburn$", pWorker);
  return(1);
}

/* Kontext */

public func ContextConstruction(object pCaller)
{
  [$TxtProducemetal$|Image=METL|Condition=IsBuilt]
  return(StartProduction(pCaller));
}

protected func IsBuilt() {
  return(GetCon() >= 100);
}

/* Steuerung */

protected func ContainedUp(object pCaller)
{
  [$TxtProducemetal$|Image=METL]
  return(StartProduction(pCaller));
}

/* Eingangssteuerung */

private func SoundOpenDoor() { return(Sound("GateOpen")); }

private func SoundCloseDoor() { return(Sound("GateClose")); }

protected func Collection() { ContentsCheck(); return(Sound("Clonk")); }

protected func Collection2()
{
 ContentsCheck();
 return true;
}

/* Produktion */

private func Burning()
{
  // Rauch
  Smoke(+23,Random(3)-24,Random(5)+8);
  // Noch nicht fertig
  if (GetActTime()<150) return(1);
  // Fertig
  SetAction("Idle");
  // Erz suchen
  var pOre;
  for(var i in FindObjects(Find_Category(C4D_Object),Find_Container(this()))){
	  if(i->~SmeltResult()){
		  pOre = i;
		  break;
	  }
  }
  if(!pOre) return(1);
    // Metall auswerfen
  Exit(CreateContents(DefinitionCall(GetID(pOre), "SmeltResult")), -27,+13,0, -1);
  // Erz verbrauchen
  RemoveObject(pOre);
  // Sound
  Sound("Pshshsh");
  // Fertig
  return(1);
}

/* Inhaltsüberprüfung */  
  
private func ContentsCheck()
{
  // Warten...
  if (!ActIdle()) 
    return(1);
  // Loren rausschicken
  var obj;
  for(var i = 0; obj = Contents(i); i++)
    if(obj->~IsLorry())
      SetCommand(obj,"Exit");
  // Erstes Inhaltsobjekt überprüfen
  obj = Contents(0); var id = GetID(obj);
  // Alles außer Mannschaftsmitgliedern, Rohmaterial und Loren sofort auswerfen. Warum die Loren!? die fliegen doch schon raus
  if(obj)
    if( !(  GetOCF(obj) & OCF_CrewMember()
            || obj->~IsLorry()
            || obj->~FurnaceFuelValue()
			|| obj->~SmeltResult()
       ) )
	     EjectNonFuel(obj);
		 
    var noOre = true;
  for(var i in FindObjects(Find_Category(C4D_Object),Find_Container(this()))){
	  if(i->~SmeltResult()) noOre = false;
  }
  // Erz prüfen
  if(noOre) { /*Message("gimme ore ffs",this);*/ return false; }
  if(CheckContainedFuel()<10) { /*Message("omg gimme fuel",this);*/ return false; }
  ConsumeFuel(10,this,0,true);
  SetAction("Burning");
  return true;
}

func EjectNonFuel(obj)
{
 if(obj->~SmeltResult())
   return false;
 if(GetDefFragile(GetID(obj)) || GetCategory(obj) & C4D_Vehicle)
   SetCommand(obj, "Exit");
 else
   Exit(obj,-27,+13,0,-1);
 if(GetCategory(obj) & C4D_Living)
   obj->SetAction("Tumble");
 }
 
 func CmdAcquireFuel() { return inherited(...); } //pseudo-lokalisierung, weil call nur lokale funktionen unterstützt

func CmdAcquireFuelFailed() { return inherited(...); }