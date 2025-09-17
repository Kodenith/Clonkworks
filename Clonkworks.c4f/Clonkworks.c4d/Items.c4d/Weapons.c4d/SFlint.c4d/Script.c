/*-- T-Flint --*/

#strict 2
#include HONY
#include TFLN

local StuckToObject;

func Bendable(){ return(false); }

protected func Hit() {
  if(!ActIdle()) return true;
  Sound("Fuse");
  SetAction("Activated");
  SetPicture(0, 12 + 64, 64, 64);
  return true;
}

public func IsHoney(){ return(false); }

public func IsNotWeapon(){
	return(StuckToObject);
}

func Initialize() {
  return(1);
}

public func RejectEntrance()
{
  if(GetAction() == "Activated" || GetSpeed(this()) > 10) return true;
  return false;
}

func HoneyUpdate(){
	if(StuckToObject){
		// attach(ish) to object
		if(InLiquid()){
			StuckToObject = 0;
			AddVertex(0, 1, this());
			return(0);
		}
		
		SetXDir(GetXDir(StuckToObject));
		SetYDir(GetYDir(StuckToObject)-2);
		//SetPosition(GetX(StuckToObject)-StickX,GetY(StuckToObject)-StickY);
	}else{
		return(_inherited());
	}
}

/* Forschung */

public func GetResearchBase() { return FLNT; }
