/*-- Neues Script --*/

#strict 3
#include IO__

func Initialize() {
  SetAction("Off");
  SetComDir(COMD_None);
  return(1);
}

func CheckGrab(){
  if(GetAction() == "Off" && FindObject2(Find_Action("Push"),Find_ActionTarget(this),Find_NoContainer())){
      SetAction("On");
      Sound("Click");
  }

  if(GetAction() == "On" && !FindObject2(Find_Action("Push"),Find_ActionTarget(this),Find_NoContainer())){
      SetAction("Off");
      Sound("Click");
  }
}

public func WireTo(){ return(0); }

//this function returns a list of string that can be output.
//default is a single output.
public func OutputList(){
  return(["Held","Held Amount"]);
}

func MustBeOnWall(){ return(1); }

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
   if(OutputName == "Held" && GetAction() == "On") return(1);
   if(OutputName == "Held Amount" && GetAction() == "On"){
      return(ObjectCount2(Find_Action("Push"),Find_ActionTarget(this),Find_NoContainer()));
   }

     return(0);
}

func CanBeDetached(){ return(1); }
func OnDetach(){
  Sound("Connect");
  Split2Components();
}

func Malfunction(){ OnDetach(); }
func Damage(){
  if(GetDamage() > 50) OnDetach();
}