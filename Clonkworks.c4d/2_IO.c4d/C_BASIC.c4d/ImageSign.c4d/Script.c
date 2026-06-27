/*-- Neues Script --*/

#strict 3
#include IO__

func Initialize() {
  SetAction("Attached");
  SetComDir(COMD_None);
  return(1);
}

func WireFrom(){
  return(0);
}

public func InputList(){
  return(["Object ID"]);
}

func Display(){
  var pId = InputActive("Object ID");
  if(GetType(pId) != C4V_String) pId = nil;
  else pId = C4Id(pId);

  if(!pId){
    SetGraphics(nil,this,GetID(),1,4);
    SetClrModulation(RGBa(255,255,255,255),this,1);
  }else{
    SetGraphics(nil,this,pId,1,4);
    SetObjDrawTransform(800,0,0,0,800,0,this,1);
    SetClrModulation(RGBa(255,255,255,0),this,1);
  }
}

func MustBeOnWall(){ return(1); }

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