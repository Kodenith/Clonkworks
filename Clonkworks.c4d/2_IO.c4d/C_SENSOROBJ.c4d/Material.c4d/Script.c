/*-- Neues Script --*/

#strict 3
#include IO__

func Initialize() {
  SetAction("Attached");
  SetComDir(COMD_None);
  return(1);
}

public func OutputList(){
  return(["Material Name","In Solid?","In Liquid?"]);
}

public func WireTo(){ return(0); }

func MustBeOnWall(){ return(1); }


//reusable function for all sensors :D
public func GetDetection(){
  return(GBackSolid() || GBackLiquid());
}

private func Detect(){
    var Found = GetDetection();
    if(!Found && GetAction() == "Active"){
      SetAction("Attached");
      Sound("Command");
    }

    if(Found){
      if(GetAction() == "Attached"){
        SetAction("Active");
        Sound("Command");
      }
    }
}

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
    if(GetAction() == "Active"){
        if(OutputName == "Material Name") return(MaterialName(GetMaterial()));
        if(OutputName == "In Solid?") return(GBackSolid());
        if(OutputName == "In Liquid?") return(GBackLiquid());
    }
    return(0);
}

func Malfunction(){ OnDetach(); }
func Damage(){
  if(GetDamage() > 50 && !Locked) OnDetach();
}
func CanBeDetached(){ return(!Locked); }
func OnDetach(pClonk){
  if(pClonk && Hostile(GetOwner(),GetController(pClonk)) && !FindObject(WSAB)) return(0);
  if(Locked) return(0);
  Sound("Connect");
  Split2Components();
}
