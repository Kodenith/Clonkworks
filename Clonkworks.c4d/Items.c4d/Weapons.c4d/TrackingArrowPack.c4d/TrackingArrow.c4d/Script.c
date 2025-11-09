/*-- Sprengpfeil --*/

#strict 2

#include ARRW

local Armed;
local Target;
local TrackModeOn;

/* Abfeuern */

public func Launch(object pByObject)
{
  // Der Sprengpfeil wird nur durch korrektes Abfeuern scharf gemacht
  Armed = 1;    
  TrackModeOn = 1;
  return(_inherited() );
}

/* Pfeileigenschaften */
public func PackTo() {  return(TARP); }
public func ArrowStrength() { return(2+Random(2)); }
public func ArrowTumbleRandom() {return(2);}
public func ArrowStopsOnHit() { return(1); }

/* Treffer */

private func HitTarget(pTarget)
{
 Target = 0;
 return(_inherited(pTarget));
}

private func Hit(){
	TrackModeOn = false;
	Target = 0;
	SetAction("Idle");
	return(_inherited());
}

private func InFlight(){
	if(Target != 0){
		//has a target? track them :D
		var Rotation = Angle(GetX(),GetY(),GetX(Target),GetY(Target));
		Rotation = (Rotation + 180) % 360 - 180;
		
		if(Rotation < GetR()) SetRDir(-45);
		else if(Rotation > GetR()) SetRDir(45);
		else SetRDir(0);
		
		//DebugLog("Target Rotation: %d, Current: %d",Rotation,GetR());
		
	    var iXDir = Sin(GetR(), 5);
		var iYDir = -Cos(GetR(), 5);
		SetXDir(GetXDir()+iXDir);
		SetYDir(GetYDir()+iYDir);
		
		//if(!Random(10)) Smoke(0,0,Random(10));
		if(!Random(5)) CreateParticle("PSpark",0,0,-iXDir,-iYDir,Random(30),RGBa(0,255,255));
		
		//the homing arrow can home for 5 seconds.
		if(!CanTrack()) Target = 0;
		return(_inherited());
	}
	
	if(CanTrack() && !Target){
	//finding enemy players to set as targets.
	var find = FindObject2(Find_NoContainer(), Find_OCF(OCF_CrewMember), Find_OCF(OCF_Alive), Find_Hostile(GetOwner()), Find_Distance(TrackSearchDistance()), Sort_Distance());
	if(find){
		StartTracking(find);
		return(_inherited());
	}
	//no enemy clonks? how about monsters?
	var find = FindObject2(Find_NoContainer(), Find_Or(Find_ID(MONS),Find_ID(FMNS)), Find_OCF(OCF_Alive), Find_Distance(TrackSearchDistance()), Sort_Distance());
	if(find){
		StartTracking(find);
		return(_inherited());
	}
	}
	
	return(_inherited());
}

public func TrackSearchDistance(){ return(300); }

func StartTracking(pObj){
		if(Target != 0) return(0);
		Target = pObj;
		SetAction("Idle");
		SetAction("Shot");
		var Rotation = Angle(GetX(),GetY(),GetX(Target),GetY(Target));
		Rotation = (Rotation + 180) % 360 - 180;
		//DebugLog("Tracking %v! Starting rotation: %d",pObj,Rotation);
		SetR(Rotation);
		SetXDir(GetXDir()/2);
		SetYDir(GetYDir()/2);
}

public func CanTrack(){
	if(GetAction() == "Shot" && GetActTime() > 180 && TrackModeOn) return(0);
	return(1);
}

func Entrance(pContainer){
	SetOwner(GetController(pContainer));
	return(_inherited(pContainer));
}