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
  return(["R","G","B"]);
}

func MustBeOnWall(){ return(1); }

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
   var R = InputActive("R");
   var G = InputActive("G");
   var B = InputActive("B");
   if(R == nil || R == 0 || GetType(R) != C4V_Int) R =1;
   if(G == nil || G == 0 || GetType(G) != C4V_Int) G =1;
   if(B == nil || B == 0 || GetType(B) != C4V_Int) B =1;

   R = BoundBy(R,1,255);
   G = BoundBy(G,1,255);
   B = BoundBy(B,1,255);

   return(RGBa(R,G,B));
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