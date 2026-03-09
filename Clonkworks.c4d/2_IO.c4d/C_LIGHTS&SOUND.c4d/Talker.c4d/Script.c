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
  return(["Message","Speak"]);
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

func CheckMessage(){
    if(GetAction() == "Attached" && InputActive("Speak")){
        SetAction("Talk");
        Sound("Command");
    }

    if(GetAction() != "Attached" && !InputActive("Speak")){
        SetAction("Attached");
        Sound("Command");
    }

    if(InputActive("Speak")){
        var Msg = InputActive("Message");
        if(!Msg) Msg = "...";
        else if(GetType(Msg) != C4V_String) Msg = "\"Message\" input not string!";

        Message(Msg,this);
    }
}