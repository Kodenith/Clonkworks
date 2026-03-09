/*-- Neues Script --*/

#strict 3
#include IO__

func Initialize() {
  SetAction("Attached");
  SetComDir(COMD_None);
  return(1);
}

public func WireFrom() { return(0); }

public func InputList(){
  return(["Sound Name","Play"]);
}

func MustBeOnWall(){ return(1); }

func Malfunction(){ OnDetach(); }
func Damage(){
  if(GetDamage() > 50) OnDetach();
}

func CanBeDetached(){ return(1); }
func OnDetach(){
  Sound("Connect");
  Split2Components();
}

func CheckSound(){
    if(GetAction() == "Attached" && InputActive("Play")){
        SetAction("Play");
    }

    if(GetAction() != "Attached" && !InputActive("Play")){
        SetAction("Attached");
    }

    if(InputActive("Play")){
        Sound(InputActive("Sound Name"));
    }
}