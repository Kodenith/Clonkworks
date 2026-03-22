/*-- Neues Script --*/

#strict 3
#include IO__

func Initialize() {
  SetAction("Attached");
  SetComDir(COMD_None);
  return(1);
}

public func WireFrom(){ return(0); }

func MustBeOnWall(){ return(1); }

//this function returns a list of string that can be input.
public func InputList(){
  return(["Blow Up"]);
}

func Malfunction(){ OnDetach(); }
func Damage(){
  if(GetDamage() > 200 && !Locked) OnDetach();
}
func CanBeDetached(){ return(!Locked); }
func OnDetach(pClonk){
  if(pClonk && Hostile(GetOwner(),GetController(pClonk)) && !FindObject(WSAB)) return(0);
  if(Locked) return(0);
  Sound("Connect");
  Split2Components();
}

func RejectCollect(pId,pObject){
  if(pId == MINE || pId == LFLN || pId == STFS || pId == TFLS) return(1);
  if(pObject->~ExplodeSize() != nil && pObject->~ExplodeSize() > 0) return(0);
  return(1);
}

func Check(){
  if(Contents()){
    SetPhase(1);
    if(InputActive("Blow Up")){
      var kabum = Contents();
      kabum->Exit();
      kabum->~Hit();
      Sound("Click");
    }
  }else{
    SetPhase(0);
  }
}

public func RejectGrabbed(pObj){
  if(Locked) return(1);
  if(Hostile(GetOwner(),GetOwner(pObj)) && !FindObject(WSAB)) return(1);
}