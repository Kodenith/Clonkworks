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
  return(["X Offset","Y Offset","Size","Display"]);
}

func Display(){
  if(InputActive("Display")){
    var iX = InputActive("X Offset");
    if(GetType(iX) != C4V_Int) iX = 0;

    var iY = InputActive("Y Offset");
    if(GetType(iY) != C4V_Int) iY = 0;

    var Size = InputActive("Size");
    if(GetType(Size) != C4V_Int) Size = 25;

    iX = BoundBy(iX,-150,150);
    iY = BoundBy(iY,-150,150);
    Size = BoundBy(Size,5,80);
    var iClr = GetColorDw();

    CreateParticle("NoGravSpark",iX,iY,0,0,Size,iClr,this);
    if(GetAction()=="Attached") SetAction("Shine");
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