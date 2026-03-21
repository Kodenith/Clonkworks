/*-- Neues Script --*/

#strict 3
#include IO__

func Initialize() {
  SetAction("Attached");
  SetComDir(COMD_None);
  return(1);
}

public func OutputList(){
  return(["Result"]);
}

public func InputList(){
  return(["X1","Y1","X2","Y2"]);
}

func MustBeOnWall(){ return(1); }

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
    var X1 = InputActive("X1");
    if(GetType(X1) != C4V_Int) X1 = 0;
    var Y1 = InputActive("Y1");
    if(GetType(Y1) != C4V_Int) Y1 = 0;

    var X2 = InputActive("X2");
    if(GetType(X2) != C4V_Int) X2 = 0;
    var Y2 = InputActive("Y2");
    if(GetType(Y2) != C4V_Int) Y2 = 0;

    return(Distance(X1,Y1,X2,Y2));
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