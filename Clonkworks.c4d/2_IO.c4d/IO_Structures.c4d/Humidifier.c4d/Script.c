/*-- Neues Script --*/

#strict 3
#include IO__
#include CXEC

public func WireFrom(){ return(0); }
public func GeyserGasTo(){ return(1); }

//this function returns a list of string that can be output.
//default is a single output.
public func InputList(){
  return(["Humidify"]);
}

func Damage(){
  if(GetDamage() > 50 && !Locked) Explode(16);
}
func GetResearchBase(){ return(CC1_); }

local Barrel;

func Initialize(){
  Barrel = CreateContents(GCAN);
}

func Update(){
  if(GetCon() < 100) return(nil);
  if(!Barrel) Barrel = CreateContents(GCAN);

  var amt = Barrel->GetAmount();
  if(!amt) amt = 0;

  if(amt <= 0 && GetID(Barrel) != GCAN) ChangeDef(GCAN,Barrel);
  if(InputActive("Humidify")){
    if(GetAction() == "Idle" && EnergyCheck(500) && Barrel->GetAmount()) SetAction("Humidify");
    TryDrain();
  }else{
    SetAction("Idle");
  }
}

func TryDrain(){
  if(Barrel->CanisterGas()){
    DoSprinkling();
  }
}

func DoSprinkling(){
  if(GetPhase() == 0 && !Barrel->GetAmount()) return(SetAction("Idle"));
  if(!EnergyCheck(500)) return(SetAction("Idle"));
  if(Random(5)) return(0);

  CreateObject(Barrel->CanisterGas());
  LocalN("iFillLevel",Barrel)-=1;
  if(LocalN("iFillLevel",Barrel) < 0) LocalN("iFillLevel",Barrel) = 0;
  
}