/*-- Neues Script --*/

#strict 3
#include BAS2
#include IO__
#include CXEC

public func WireFrom(){ return(0); }

//this function returns a list of string that can be output.
//default is a single output.
public func InputList(){
  return(["Close"]);
}

func Damage(){
  if(GetDamage() > 80 && !Locked) Explode(30);
}
func GetResearchBase(){ return(MGAT); }

local basement2;

func Initialize(){
  RemoveObject(basement);
  basement2 = CreateObject(GBAS,-23,5);
  basement = CreateObject(GBAS,23,5);
  SetAction("Open");
}

func Destruction(){
  if(basement2) RemoveObject(basement2);
  return(_inherited());
}

func HandleGate(){
  if(GetCon() < 100) return(0);
  if(GetAction() == "Open"){
    SetSolidMask(35*GetPhase()-1,0,35,10,0,0);
    if(GetPhase() >= 15 && InputActive("Close") && EnergyCheck(EnergyCheck(10000))){
      SetAction("Close");
      DoEnergy(-10000);
    }
  }

  if(GetAction() == "Close"){
    SetSolidMask(35*(16-GetPhase()-1),0,35,10,0,0);
    if(GetPhase() >= 15 && !InputActive("Close") && EnergyCheck(EnergyCheck(10000))){
      SetAction("Open");
      DoEnergy(-10000);
    }
  }

}

func OpenSound(){ Sound("Airlock1"); }
func CrashSound(){ Sound("Discharge"); }
