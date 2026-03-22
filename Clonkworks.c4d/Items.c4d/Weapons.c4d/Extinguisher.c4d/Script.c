/*-- Neues Script --*/

#strict 2

public func ExplodeSize() { return(55); }

func Hit(){
	//explosion
	for(var i in FindObjects(Find_Distance(ExplodeSize()))){
		Extinguish(i);
	}
	
	//effects
	CastParticles("FSpark", 5,50, 0,0, 50, 200, RGBa(255,255,255,0), RGBa(255,255,255,0));
	CastParticles("PSpark", 15,35, 0,0, 50, 200, RGBa(255,255,255,0), RGBa(255,255,255,0));
	CastParticles("PSpark", 15,80, 0,0, 5, 20, RGBa(255,255,255,0), RGBa(255,255,255,0));
	Sound("Pop*");
	Sound("Splash2");
	ShakeViewPort(ExplodeSize()/2,0,GetX(),GetY());
	ShakeObjects(GetX(),GetY(),35);
	RemoveObject();
}

public func GetResearchBase() { return(SFLN); }
public func IsAdvancedProduct(){ return(1); }
public func IsChemicalProduct(){ return(1); }