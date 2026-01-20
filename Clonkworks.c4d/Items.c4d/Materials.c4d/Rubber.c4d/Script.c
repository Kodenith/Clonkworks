/*-- Rubber --*/

#strict 2
local bounces;

public func MaxBounces(){ return(25); }

func Initialize() {
  bounces = MaxBounces();
  return(1);
}

public func Hit(xdir,ydir){
	if(!Random(3)) Sound("RockHit*");
	if(bounces > 0){
		SetXDir(xdir,,100);
		SetYDir(ydir,,100);
		Bounce(GetSpeed());
		bounces--;
	}
}

public func Hit2(xdir,ydir){ return(Hit(xdir,ydir)); }
public func Hit3(xdir,ydir){ return(Hit(xdir,ydir)); }

func Rebounce(){
	if(Contained() || GBackSolid(0,2)) bounces = MaxBounces();
}

public func IsMixerProduct(){ return(1); }