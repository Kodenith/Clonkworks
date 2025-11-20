/*-- Neues Script --*/

#strict 2

func Initialize() {
  SetAction("Idle");
  return(1);
}

public func RejectEntrance(){
	if(GetAction() == "Idle") return(0);
	return(1);
}

func CheckThrown(){
	if(GetAction() != "Idle") return(0);
	if(GetSpeed() > 20) SetAction("Tumble");
}

func Hit(xdir,ydir){
	xdir /= 10;
	ydir /= 10;
	
	var ExplodeSize = (xdir+ydir)/2;
	ExplodeSize = BoundBy(ExplodeSize,10,20);
	
	var Exploder = CreateObject(FLNT,RandomX(-3,3),RandomX(-3,3));
	Exploder->Explode(ExplodeSize);
	Sound("BOING");
	Bounce((ExplodeSize*RandomX(5,7)));
	
	//DoDamage(ExplodeSize);
	SetAction("Tumble");
}

public func CheckNoMovement(){
	if(GetSpeed() < 10 && (GetContact(this(), -1) & CNAT_Bottom)){
		SetAction("Idle");
	}
}

func OnBlowDealt(iPower){
	Hit(iPower/2,iPower/2);
}

func Damage(){
	if(GetDamage() > 160) RemoveObject();
}

public func IsChemicalProduct(){ return(1); }
public func IsAdvancedProduct(){ return(1); }
public func GetResearchBase() { return(MINE); }