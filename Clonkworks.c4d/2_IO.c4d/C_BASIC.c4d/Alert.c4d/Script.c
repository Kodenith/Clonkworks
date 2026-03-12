/*-- Neues Script --*/

#strict 3
#include IO__

func Initialize() {
  SetAction("Attached");
  SetComDir(COMD_None);
  return(1);
}

public func AllowRecolor(){ return(0); }

public func WireFrom(){ return(0); }

func MustBeOnWall(){ return(1); }

//this function returns a list of string that can be input.
public func InputList(){
  return(["Message","Alert"]);
}

func TryAlert(){
  if(InputActive("Alert")){
    var Messag = InputActive("Message");
    if(!Messag || GetType(Messag) != C4V_String){
      Messag = "Alert!!";
    }

    SetAction("Alert");
    PlayerMessage(GetOwner(),Messag);
    Sound("Alert",true,nil,100,GetOwner()+1);
  }
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

func Grabbed(pByObject,fGrab){
  if(fGrab && !Locked && GetOwner(pByObject) != GetOwner()){
    SetOwner(GetController(pByObject));
    Message("$TxtSet$",this,GetPlayerName(GetOwner()));
    Sound("Click");
  }
}