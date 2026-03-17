/*-- Neues Script --*/

#strict 3
#include IO__

func Initialize() {
  SetAction("Attached");
  SetComDir(COMD_None);
  return(1);
}

public func OutputList(){
  return(["Formatted"]);
}

public func InputList(){
  return(["String","F1","F2","F3","F4","F5"]);
}

func MustBeOnWall(){ return(1); }

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
   if(GetType(InputActive("String")) != C4V_String) return(0);
    var F1 = InputActive("F1");
    if(!F1) F1 = 0;
    var F2 = InputActive("F2");
    if(!F2) F2 = 0;
    var F3 = InputActive("F3");
    if(!F3) F3 = 0;
    var F4 = InputActive("F4");
    if(!F4) F4 = 0;
    var F5 = InputActive("F5");
    if(!F5) F5 = 0;
   return(Format(InputActive("String"),F1,F2,F3,F4,F5));
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