/*-- Neues Objekt --*/

#strict 2

local Wild;
local StickX;
local StickY;
local VelX, VelY;

public func IsHoney(){ return(true); }
func Bendable(){ return(true); }

public func IsNotWeapon(){
	return(true);
}

func Initialize() {
	SetCon(RandomX(80,170));
  return(1);
}

func HoneyUpdate(){
	//Making the honey stretch
	var xv = GetXDir();
	var yv = GetYDir();
	var ycalc;
	if(yv == 0) ycalc = 1;
	else ycalc = yv;
	if(Bendable())
	SetObjDrawTransform(1000, 30 * xv, 0,0,1000+(20*ycalc),0,this());
	
	if(!StickX){
	VelX = 0; VelY = VelX;
	
	//sticking
	if(GetXDir() < 0) VelX = -1; else VelX = 1;
	if(GetYDir() < 0) VelY = -1; else VelY = 1;
	
	if(GBackSolid(VelX,VelY)){
		StickX = GetX();
		StickY = GetY();
		Sound("ArrowHit");
	}
	}
	
	if(StickX){
		SetXDir(0);
		SetYDir(0);
		SetPosition(StickX,StickY);
		
		if(!GBackSolid(VelX, VelY)){
			StickX = 0;
			Sound("ArrowHit");
		}
	}
	
	//dripping
	if(!Random(60) && !Contained())
	CreateParticle("HoneyDrip", -VelX, -VelY, RandomX(-2,2), 0, RandomX(10,25), RGBa(255,255,255));
}

func Entrance(object pContainer){
	StickX = 0;
	if(GetOCF(pContainer) & OCF_Alive){
		//find nearby bumbs and alert them if this honey isnt from the bumbhouse.
		if(Wild){
			
			for(var found in FindObjects(Find_ID(BUMB), Find_Distance(300))){
			if(found)
			if(ObjectDistance(this(), found) < 300 && GetAction(found) != "Rest")
			LocalN("GrudgeTarget", found) = pContainer;;
			}
		}
	}
}

public func Activate(object pClonk)
{
  [$TxtEat$]
  Eat(pClonk);
  return(1);
}

public func Eat(object pClonk)
{
  pClonk->~Feed(GetCon()/6);
  RemoveObject();
  return(1);
}