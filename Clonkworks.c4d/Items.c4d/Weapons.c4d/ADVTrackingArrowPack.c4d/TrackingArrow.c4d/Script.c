/*-- Sprengpfeil --*/

#strict 2

#include ARRW

local Armed;
local Target;

/* Abfeuern */

public func Launch(object pByObject)
{
  // Der Sprengpfeil wird nur durch korrektes Abfeuern scharf gemacht
  Armed = 1;    
  return(_inherited() );
}

/* Pfeileigenschaften */
public func PackTo() {  return(ATRP); }
public func ArrowStrength() { return(2+Random(2)); }
public func ArrowTumbleRandom() {return(2);}
public func ArrowStopsOnHit() { return(0); }

/* Aufschlag */

protected func Hit()
{
  if(!Target){
   if (Armed)
    Explode(Random(5) + 15);
  }else{
	 if (Armed && ObjectDistance(Target) < 50)
    Explode(Random(5) + 15);
  }
}

/* Treffer */

private func HitTarget(pTarget)
{
 Hit();
 
 return(_inherited(pTarget));
}

private func InFlight(){
	//finding enemy players to set as targets.
	if(GetAction() != "Tracking"){
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

public func StartTracking(What){
	if(GetAction() == "Idle") return(0);
	SetAction("Tracking");
	SetXDir(GetXDir()/2);
	SetYDir(GetYDir()/2);
	Target = What;
}

public func Tracking(){
	if(!GetCommand()) SetCommand(this(),"Follow",Target);
	var Rotation = Angle(GetX(),GetY(),GetX()+GetXDir(),GetY()+GetYDir());
	SetR(Rotation);
	
	//turning away from walls
	if(GBackSolid(-10,0)) SetXDir(GetXDir()+Random(3));
	if(GBackSolid(10,0)) SetXDir(GetXDir()-Random(3));
	if(GBackSolid(0,10)) SetYDir(GetYDir()-Random(3));
	if(GBackSolid(0,-10)) SetYDir(GetYDir()+Random(3));

	if(GetComDir() == COMD_Left || GetComDir() == COMD_Right) SlowDirY(3);
	if(GetComDir() == COMD_Up || GetComDir() == COMD_Down) SlowDirY(3);
	
	if((ObjectDistance(Target) < 150 && !Random(20)) || ObjectDistance(Target) < 35){
		SetXDir(GetXDir()*2);
		SetYDir(GetYDir()*2);
	}
	
	if(GetActTime() > 36*TrackMaxTime() || InLiquid() || ObjectDistance(Target) > TrackSearchDistance()*2 || Contained() || !Target || !GetAlive(Target)){
		Armed = false;
		Target = 0;
		SetAction("Idle");
	}
	
			
	var iXDir = Sin(GetR(), 5);
    var iYDir = -Cos(GetR(), 5);
	if(!Random(10)) Smoke(0,0,Random(10));
	if(!Random(5)) CreateParticle("PSpark",0,0,-iXDir,-iYDir,Random(30),RGBa(255,145,0));
	
	InFlight();
}

global func SlowDirY(Max){
	if(GetYDir() > 0) SetYDir(GetYDir()-Random(Max));
	else if(GetYDir() < 0) SetYDir(GetYDir()+Random(Max));
}

global func SlowDirX(Max){
	if(GetXDir() > 0) SetXDir(GetXDir()-Random(Max));
	else if(GetXDir() < 0) SetXDir(GetXDir()+Random(Max));
}

public func TrackSearchDistance(){ return(550); }
public func TrackMaxTime(){ return(25); }
public func TrackSound(){ Sound("ArrowTrack"); }
public func TrackStopSound(){ Sound("LostTrack"); }

public func RejectEntrance(pInto)
{
 // Aus einem Fahrzeug direkt in die Basis? Dann nicht packen lassen
 // Hack, damit die Pfeile aus der Armbrust beim verkaufen richtig angerechnet werden
 if(Contained()) if (GetCategory(Contained()) & C4D_Vehicle)
   if (FindObject2(Find_ID(FLAG), Find_ActionTarget(pInto)))
     AddEffect("IntNoPacking", this, 1, 1);

 // Kann nicht aus dem Flug von moeglichen Zielen gefangen werden
 if(GetAction() == "Shot" && GetAlive(pInto))return(true);
 if(GetAction() == "Tracking") return(true);
 
 // Kann direkt nach Treffer nicht weggesammelt werden
 if(GetEffect("CannotBeCollected",this()))return(true);
 return(false);
}

func Entrance(pContainer){
	SetOwner(GetOwner(pContainer));
	return(_inherited(pContainer));
}