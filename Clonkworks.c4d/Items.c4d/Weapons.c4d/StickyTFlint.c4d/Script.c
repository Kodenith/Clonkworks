/*-- T-Flint --*/

#strict 2
#include TFLN

local iLastController; // Fuer Killverfolgung: Letzter Controller vor dem Einsammeln

protected func Hit(int xdir, int ydir) {
  Sound("ArrowHit");
  if(!ActIdle()) return true;
  Sound("Fuse");
  SetAction("Activated");
  //Make sure it doesnt unstick, move by 1 pixel into the ground.
  var iX, iY;
  if(GetXDir() < 0) iX = -1;
  if(GetXDir() > 0) iX = 1;
  if(ydir < 0) iY = -2;
  SetPosition(GetX()+iX,GetY()+iY);
  // Picture anpassen
  SetPicture(0, 12 + 64, 64, 64);
  return true;
}

public func Activate(pClonk)
{
 [$TxtFuse$]
 if(!ActIdle()) return true;
 Sound("Fuse");
 SetAction("Activated");
 iLastController = GetController();
 // Picture anpassen
 SetPicture(0, 12 + 64, 64, 64);
 
 // Attaching to ceilings and walls
 if(GetProcedure(pClonk) == "SCALE"){
	 var steps = 10; //max steps
	 var dir = GetDir2(pClonk);
	 Exit();
	 while(steps > 0 && !GBackSolid()){
		 SetX(GetX()+dir);
		 steps--;
	 }
	 SetX(GetX()+dir);
	 return true;
 }
 
 if(GetProcedure(pClonk) == "HANGLE"){
	 var steps = 10; //max steps
	 Exit();
	 while(steps > 0 && !GBackSolid()){
		 SetY(GetY()-1);
		 steps--;
	 }
	 SetY(GetY()-1);
	 return true;
 }
 
 return true;
}

public func Stickiness(){
	if((GetContact(this(),-1) & 8)&& GetAction() == "Activated") SetActionKeepPhase("ActivatedStick");
	if(!(GetContact(this(),-1) & 8) && GetAction() == "ActivatedStick") SetActionKeepPhase("Activated");
}

public func GetResearchBase() { return(TFLN); }