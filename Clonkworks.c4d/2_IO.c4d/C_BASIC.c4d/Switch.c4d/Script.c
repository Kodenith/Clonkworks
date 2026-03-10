/*-- Neues Script --*/

#strict 3
#include IO__

func Initialize() {
  SetAction("Off");
  SetComDir(COMD_None);
  return(1);
}

func ControlThrow(){
  [$TxtClick$]
  if(GetAction() == "Off"){
    SetAction("On");
    Sound("Click");
  }else
  if(GetAction() == "On"){
    SetAction("Off");
    Sound("Click");
  }
}

public func WireTo(){ return(0); }

//this function returns a list of string that can be output.
//default is a single output.
public func OutputList(){
  return(["On"]);
}

func MustBeOnWall(){ return(1); }

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
   if(OutputName == "On" && GetAction() == "On") return(1);
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