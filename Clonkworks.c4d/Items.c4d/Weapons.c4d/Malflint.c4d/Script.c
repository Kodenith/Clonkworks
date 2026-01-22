/*-- Neues Script --*/

#strict 2

public func ExplodeSize() { return(50); }

func Hit(){
	//explosion
	for(var i in FindObjects(Find_Distance(ExplodeSize()))){
		i->~Malfunction();
	}
	
	//effects
	CastParticles("FSpark", 5,50, 0,0, 100, 200, RGBa(128,128,128,128), RGBa(128,128,128,0));
	CastParticles("FSpark", 5,55, 0,0, 100, 200, RGBa(150,150,150,128), RGBa(150,150,150,0));
	CastParticles("PSpark", 15,55, 0,0, 50, 200, RGBa(0,255,255,0), RGBa(0,0,255,0));
	Sound("Electric");
	ShakeViewPort(ExplodeSize(),0,GetX(),GetY());
	RemoveObject();
}

public func GetResearchBase() { return(SFLN); }
public func IsAdvancedProduct(){ return(1); }
public func IsAnvilProduct(){ return(1); }